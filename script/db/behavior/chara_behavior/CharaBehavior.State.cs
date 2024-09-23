using Godot;

namespace db;

public partial class CharaBehavior
{

    /// <summary>
    /// サイド操作権を渡せるキャラか
    /// 死亡していない、ダメージ中でない、手動操作中でない
    /// </summary>
    private bool CanControl => MyState.Live.IsDead == false
                               && MyState.Motion.HasFlag(CharaMotionFlag.Dam) == false
                               && MyState.Pad.IsManualControl == false;

    /// <summary>
    /// 操作権キャラ
    /// </summary>
    private bool IsControl => MyTeamState.MainState.ControlOrderIndex == MyState.Order.OrderIndex;

    /// <summary>
    /// COM操作中かどうか
    /// </summary>
    private bool IsCom => MyState.Pad.IsManualControl == false || MyTeamState.IsCom();

    /// <summary>
    /// 自分で操作するキャラかどうか
    /// </summary>
    private bool IsSelfControl => MyState.Pad.IsManualControl
                                  || (IsControl && IsCom == false); //サイド

    /// <summary>
    /// ボール持ちかどうか
    /// </summary>
    private bool IsBallHolder => BallState.IsBallHolder(MySideIndex, OrderIndex);

    /// <summary>
    /// 自分がシュートターゲット
    /// </summary>
    private bool IsShotTarget => BallState.IsShotTarget(MySideIndex, OrderIndex);

    /// <summary>
    /// 自分がパスターゲット
    /// </summary>
    private bool IsPassTarget => BallState.IsPassTarget(MySideIndex, OrderIndex);

    /// <summary>
    /// パス待ち状態
    /// </summary>
    private bool IsPassWait => IsPassTarget
                               && IsBallHolder == false
                               && MyState.Pad.IsManualControl;

    /// <summary>
    /// 現在のモーションデータ
    /// </summary>
    /// <returns></returns>
    private BaseMotionData NowBaseMotionData => MasterManager.Instance.BaseMotionMaster.Get(MyState.Motion.MotionType);

    /// <summary>
    /// 現在のモーションのコマデータ
    /// </summary>
    /// <returns></returns>
    private BaseMotionKomaData NowBaseMotionKoma => NowBaseMotionData.Get(MyState.Motion.KomaNo);

}