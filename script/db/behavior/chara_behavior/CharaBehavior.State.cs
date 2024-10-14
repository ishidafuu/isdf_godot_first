using Godot;

namespace db;

public partial class CharaBehavior
{

    /// <summary>
    /// サイド操作権を渡せるキャラか
    /// 死亡していない、ダメージ中でない、手動操作中でない
    /// </summary>
    public bool CanControl => MyState.Live.IsDead == false
                               && MyState.Motion.HasFlag(CharaMotionFlag.Dam) == false
                               && MyState.Pad.IsManualControl == false;

    /// <summary>
    /// 操作権キャラ
    /// </summary>
    public bool IsControl => MyTeamState.MainState.ControlOrderIndex == OrderIndex;

    /// <summary>
    /// COM操作中かどうか
    /// </summary>
    public bool IsCom => MyState.Pad.IsManualControl == false || MyTeamState.IsCom();

    /// <summary>
    /// 自分で操作するキャラかどうか
    /// </summary>
    public bool IsSelfControl => MyState.Pad.IsManualControl
                                  || (IsControl && IsCom == false);

    /// <summary>
    /// ボール持ちかどうか
    /// </summary>
    public bool IsBallHolder => BallState.IsBallHolder(MySideIndex, OrderIndex);

    /// <summary>
    /// 自分がシュートターゲット
    /// </summary>
    public bool IsShotTarget => BallState.IsShotTarget(MySideIndex, OrderIndex);

    /// <summary>
    /// 自分がパスターゲット
    /// </summary>
    public bool IsPassTarget => BallState.IsPassTarget(MySideIndex, OrderIndex);

    /// <summary>
    /// パス待ち状態
    /// </summary>
    public bool IsPassWait => IsPassTarget
                               && IsBallHolder == false
                               && MyState.Pad.IsManualControl;

    /// <summary>
    /// かがみ中（予約中）かどうか
    /// </summary>
    public bool IsKagami => MyState.Damage.KagamiCount.Value > 0;

    /// <summary>
    /// 現在のモーションデータ
    /// </summary>
    /// <returns></returns>
    public BaseMotionData CurrentBaseMotionData => MasterManager.Instance.BaseMotionMaster.Get(MyState.Motion.MotionType);

    /// <summary>
    /// 現在のモーションのコマデータ
    /// </summary>
    /// <returns></returns>
    public BaseMotionKomaData CurrentBaseMotionKoma => CurrentBaseMotionData.Get(MyState.Motion.KomaNo.Value);

    /// <summary>
    /// 現在のモーションのコマデータ
    /// </summary>
    /// <returns></returns>
    public BaseMotionKomaData NextBaseMotionKoma => CurrentBaseMotionData.Get(MyState.Motion.KomaNo.Value);

    /// <summary>
    /// 最終コマか
    /// </summary>
    public bool IsLastKoma => MyState.Motion.KomaNo.Value == CurrentBaseMotionData.KomaCount - 1;

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

            if (MyState.Auto.AutoType == AutoType.Dashman)
            {
                return true;
            }

            //メンバー操作＆下降開始まで＆（敵方向ダッシュＯＲコート前方ジャンプ）でダッシュマン扱い
            var isForwardJump = MyState.Motion.HasFlag(CharaMotionFlag.Ds)
                                && MyState.Motion.HasFlag(CharaMotionFlag.Ar)
                                && MyState.Motion.MotionType != CharaMotionType.Sl
                                && MyState.Coordinate.VelocityY > Defines.DIVELIMDY
                                && LeftCourtX > Defines.DBCRT_CLXL;

            // 前方ジャンプ状態であればダッシュマン扱い（ミラーパスの対象とするため）
            if (isForwardJump)
            {
                return true;
            }

            // 敵コート方向へのダッシュ
            var isForwardDash = MyState.Motion.MotionType == CharaMotionType.Ds
                                && ((MySideIndex == 0 && MyState.Coordinate.DashDirection == DirectionXType.Right)
                                    || (MySideIndex == 1 && MyState.Coordinate.DashDirection == DirectionXType.Left));

            // マニュアル操作ダッシュマン
            return MyState.Pad.IsManualControl
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
            var isLeftCrt = MyState.Order.IsInfield ^ (MySideIndex != 0);
            return isLeftCrt
                ? MyState.Coordinate.X
                : Defines.DBCRT_W - MyState.Coordinate.X;
        }
    }

    /// <summary>
    /// 自由状態かどうか
    /// </summary>
    public bool IsFree(bool isCheckAutoType)
    {
        if (MyState.Live.IsDead
            || !MyState.Live.IsAlive
            || IsKagami
            || MyState.Motion.HasFlag(CharaMotionFlag.Dam)
            || MyState.Motion.HasFlag(CharaMotionFlag.ANG)
            || MyState.Motion.HasFlag(CharaMotionFlag.PHit))
        {
            return false;
        }

        if (isCheckAutoType) //オートの時
        {
            //どれかならＯＫ
            return MyState.Auto.AutoType
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
                || MyState.Motion.HasFlag(CharaMotionFlag.Ds) == false)
            {
                return false;
            }

            return MyState.Coordinate.DashDirection == DirectionXType.Left && MyPad.KeyLeft.IsPressed
                   || MyState.Coordinate.DashDirection == DirectionXType.Right && MyPad.KeyRight.IsPressed;
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
            var isForwardDash = (MySideIndex == 0 && MyState.Coordinate.DashDirection == DirectionXType.Right)
                                || (MySideIndex == 1 && MyState.Coordinate.DashDirection == DirectionXType.Left);

            return MyState.Motion.HasFlag(CharaMotionFlag.Ds)
                   && MyState.Motion.HasFlag(CharaMotionFlag.Ar) == false
                   && isForwardDash;
        }
    }
}