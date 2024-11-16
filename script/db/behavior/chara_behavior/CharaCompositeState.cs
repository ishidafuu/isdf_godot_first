using isdf;

namespace db;

public class CharaCompositeState : BaseBehavior, ICompositeStateGetter
{
    private int EnemySideIndex => MySideIndex == 0 ? 1 : 0;
    private int MyMemberIndex => RawState.Index.MemberIndex;

    private CharaState RawState => CharaStateManager.Instance.Get(MySideIndex, MyMemberIndex);
    private CharaState[] MySideOrders => CharaStateManager.Instance.GetSideOrders(MySideIndex);
    private CharaState[] EnemySideOrders => CharaStateManager.Instance.GetSideOrders(EnemySideIndex);
    private BallBehavior Ball => BallBehaviorManager.Instance.Get();
    private RefereeState RefereeState => RefereeStateManager.Instance.RefereeState;
    private TeamBehavior MyTeam => TeamBehaviorManager.Instance.Get(MySideIndex);
    private ITeamAiActionStateGetter MyTeamAiAction => MyTeam.ComAction(MyMemberIndex);
    private TeamBehavior EnemyTeam => TeamBehaviorManager.Instance.Get(EnemySideIndex);
    private IPad Pad => PadBehaviorManager.Instance.Get(MySideIndex).Pad;

    /// <summary>
    /// Getアクセスインターフェイス
    /// </summary>

    public int MySideIndex => RawState.Index.SideIndex;

    public OrderIndexType MyOrderIndex => RawState.Order.OrderIndex;
    public ICharaIndexStateGetter Index => RawState.Index;
    public ICharaOrderStateGetter Order => RawState.Order;
    public ICharaInputStateGetter Input => RawState.Input;
    public ICharaLiveStateGetter Live => RawState.Live;
    public ICoordinateStateGetter Coordinate => RawState.Coordinate;
    public ICharaMotionStateGetter Motion => RawState.Motion;
    public ICharaAutoStateGetter Auto => RawState.Auto;
    public ICharaAutoStateGetter NextAuto => RawState.NextAuto;
    public ICharaComAutoStateGetter ComAuto => RawState.ComAuto;
    public ICharaGouseiStateGetter Gousei => RawState.Gousei;
    public ICharaDamageStateGetter Damage => RawState.Damage;
    public ICharaLevelStateGetter Level => RawState.Level;
    public ICharaDistanceStateGetter Distance => RawState.Distance;
    public ICharaMoveStateGetter Move => RawState.Move;
    public ICharaAirStateGetter Air => RawState.Air;
    public ICharaViewStateGetter View => RawState.View;
    public ICharaCatchStateGetter Catch => RawState.Catch;
    public ICharaShootStateGetter Shoot => RawState.Shoot;
    public ICharaCourtStateGetter Court => RawState.Court;
    public ICharaPassStateGetter Pass => RawState.Pass;
    public ICharaDashmanStateGetter Dashman => RawState.Dashman;
    public ICharaComOnlyStateGetter ComOnly => RawState.ComOnly;
    public ICharaBallEffectStateGetter BallEffect => RawState.BallEffect;

    /// <summary>
    /// アウト扱い
    /// </summary>
    public bool IsOut => Live.Hp == 0 || Live.IsAngel;

    /// <summary>
    /// サイド操作権を渡せるキャラか
    /// 死亡していない、ダメージ中でない、手動操作中でない
    /// </summary>
    public bool CanControl => Live.IsDead == false
                              && Motion.HasFlag(CharaMotionFlag.Dam) == false
                              && Input.IsManualControl == false;

    /// <summary>
    /// 操作権キャラ
    /// </summary>
    public bool IsControl => MyTeam.MainState.ControlOrderIndex == MyOrderIndex;

    /// <summary>
    /// COM操作中かどうか
    /// </summary>
    public bool IsCom => Input.IsManualControl == false || MyTeam.IsCom;

    /// <summary>
    /// 自分で操作するキャラかどうか
    /// </summary>
    public bool IsSelfControl => Input.IsManualControl
                                 || (IsControl && IsCom == false);
    
    /// <summary>
    /// ボール保持者かどうか
    /// </summary>
    public bool IsBallHolder => MySideIndex == Ball.MainState.HolderSide
                                && MyOrderIndex == Ball.MainState.HolderOrder;

    /// <summary>
    /// シュートターゲットかどうか
    /// </summary>
    public bool IsShotTarget => Ball.MainState.MotionType is BallMotionType.Hold or BallMotionType.Shoot
                                && MySideIndex == Ball.MainState.ShotTargetSide
                                && MyOrderIndex == Ball.MainState.ShotTargetOrder;

    /// <summary>
    /// パスターゲットかどうか
    /// </summary>
    public bool IsPassTarget => Ball.MainState.MotionType is BallMotionType.Hold
                                && MySideIndex == Ball.MainState.PassTargetSide
                                && MyOrderIndex == Ball.MainState.PassTargetOrder
                                && Ball.MainState.OvLine == false;

    /// <summary>
    /// パス待ち状態
    /// </summary>
    public bool IsPassWait => IsPassTarget
                              && IsBallHolder == false
                              && Input.IsManualControl;

    /// <summary>
    /// かがみ中（予約中）かどうか
    /// </summary>
    public bool IsKagami => Damage.KagamiCountValue > 0;

    /// <summary>
    /// 現在のモーションデータ
    /// </summary>
    /// <returns></returns>
    public BaseMotionData CurrentBaseMotionData => MasterManager.Instance.BaseMotionMaster.Get(Motion.MotionType);

    /// <summary>
    /// 現在のモーションのコマデータ
    /// </summary>
    /// <returns></returns>
    public BaseMotionKomaData CurrentBaseMotionKoma => CurrentBaseMotionData.Get(Motion.KomaNoValue);

    /// <summary>
    /// 現在のモーションのコマデータ
    /// </summary>
    /// <returns></returns>
    public BaseMotionKomaData NextBaseMotionKoma => CurrentBaseMotionData.Get(Motion.KomaNoValue);

    /// <summary>
    /// 最終コマか
    /// </summary>
    public bool IsLastKoma => Motion.KomaNoValue == CurrentBaseMotionData.KomaCount - 1;


    /// <summary>
    /// ダッシュマン（メンバー操作も含む）
    /// </summary>
    /// <returns></returns>
    public bool IsDashman
    {
        get
        {
            if (IsFree(false) == false)
            {
                return false;
            }

            if (Auto.AutoType == AutoType.Dashman)
            {
                return true;
            }

            //メンバー操作＆下降開始まで＆（敵方向ダッシュＯＲコート前方ジャンプ）でダッシュマン扱い
            var isForwardJump = Motion.HasFlag(CharaMotionFlag.Ds)
                                && Motion.HasFlag(CharaMotionFlag.Ar)
                                && Motion.MotionType != CharaMotionType.Sl
                                && Coordinate.VelocityY > Defines.DIVELIMDY
                                && LeftCourtX > Defines.DBCRT_CLXL;

            // 前方ジャンプ状態であればダッシュマン扱い（ミラーパスの対象とするため）
            if (isForwardJump)
            {
                return true;
            }

            // 敵コート方向へのダッシュ
            var isForwardDash = Motion.MotionType == CharaMotionType.Ds
                                && ((MySideIndex == 0 && Coordinate.DashDirection == DirectionXType.Right)
                                    || (MySideIndex == 1 && Coordinate.DashDirection == DirectionXType.Left));

            // マニュアル操作ダッシュマン
            return Input.IsManualControl
                   && IsBallHolder == false
                   && (isForwardDash || isForwardJump);
        }
    }


    /// <summary>
    /// 左側コート換算でのX座標
    /// </summary>
    /// <returns></returns>
    public int LeftCourtX
    {
        get
        {
            // T0の外野とT1の内野は右に居るので反転
            var isLeftCrt = Order.IsInfield ^ (MySideIndex != 0);

            return isLeftCrt
                ? Coordinate.X
                : Defines.DBCRT_W - Coordinate.X;
        }
    }


    /// <summary>
    /// 自由状態かどうか
    /// </summary>
    public bool IsFree(bool isCheckAutoType)
    {
        if (Live.IsDead
            || !Live.IsAlive
            || IsKagami
            || Motion.HasFlag(CharaMotionFlag.Dam)
            || Motion.HasFlag(CharaMotionFlag.ANG)
            || Motion.HasFlag(CharaMotionFlag.PHit))
        {
            return false;
        }

        if (isCheckAutoType) //オートの時
        {
            //どれかならＯＫ
            return Auto.AutoType
                is AutoType.Free
                or AutoType.Get
                or AutoType.Alleyoop
                or AutoType.DfPos
                or AutoType.CPUHelp
                or AutoType.ShCv
                or AutoType.CPUShPa
                or AutoType.PassCvr;
        }

        return true;
    }


    /// <summary>
    /// ダッシュ加速ボタンが押されているか
    /// </summary>
    /// <returns></returns>
    public bool IsPressedDashAccelKey
    {
        get
        {
            if (IsSelfControl == false
                || Motion.HasFlag(CharaMotionFlag.Ds) == false)
            {
                return false;
            }

            return (Coordinate.DashDirection == DirectionXType.Left && Pad.KeyLeft.IsPressed)
                   || (Coordinate.DashDirection == DirectionXType.Right && Pad.KeyRight.IsPressed);
        }
    }


    /// <summary>
    /// 敵コート方向継続ダッシュ可能状態
    /// </summary>
    /// <returns></returns>
    public bool CanContinuationDash
    {
        get
        {
            var isForwardDash = (MySideIndex == 0 && Coordinate.DashDirection == DirectionXType.Right)
                                || (MySideIndex == 1 && Coordinate.DashDirection == DirectionXType.Left);

            return Motion.HasFlag(CharaMotionFlag.Ds)
                   && Motion.HasFlag(CharaMotionFlag.Ar) == false
                   && isForwardDash;
        }
    }


    /// <summary>
    /// シュートターゲットになれる
    /// </summary>
    /// <returns></returns>
    public bool IsEnableShootTarget
    {
        get
        {
            // if (MyState.Live.IsDead
            //     || MyState.Live.Hp <= 0
            //     || MyState.Motion.HasFlag(CharaMotionFlag.ANG))
            if (IsOut)
            {
                return false;
            }

            if (Motion.HasFlag(CharaMotionFlag.Ar)
                && Motion.HasFlag(CharaMotionFlag.Dam))
            {
                return Coordinate.Y <= GetSettingShoot(SettingShotType.FlyTagHeight) * Defines.Percent;
            }

            return true;
        }
    }


    /// <summary>
    /// ノーガード状態
    /// isContainDamage:ダメージ状態含む
    /// </summary>
    /// <returns></returns>
    public bool IsNoGuard(bool isContainDamage)
    {
        return IsKagami
               || Motion.HasFlag(CharaMotionFlag.Dn)
               || (isContainDamage && Motion.HasFlag(CharaMotionFlag.Dam));
    }
}


public interface ICompositeStateGetter
{
    bool IsOut { get; }
    bool CanControl { get; }
    bool IsControl { get; }
    bool IsCom { get; }
    bool IsSelfControl { get; }
    bool IsBallHolder { get; }
    bool IsShotTarget { get; }
    bool IsPassTarget { get; }
    bool IsPassWait { get; }
    bool IsKagami { get; }
    BaseMotionData CurrentBaseMotionData { get; }
    BaseMotionKomaData CurrentBaseMotionKoma { get; }
    BaseMotionKomaData NextBaseMotionKoma { get; }
    bool IsLastKoma { get; }
    bool IsDashman { get; }
    int LeftCourtX { get; }
    bool IsFree(bool isCheckAutoType);
    bool IsPressedDashAccelKey { get; }
    bool CanContinuationDash { get; }
    bool IsEnableShootTarget { get; }
    bool IsNoGuard(bool isContainDamage);
}
