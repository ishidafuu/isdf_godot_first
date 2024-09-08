namespace db;

public partial class CharaBehavior
{

    /// <summary>
    /// サイド操作権を渡せるキャラか
    /// </summary>
    private bool CanControl()
    {
        // 死亡していない、ダメージ中でない、手動操作中でない
        return MyState.Live.IsDead == false
               && MyState.Motion.HasFlag(CharaMotionFlag.Dam) == false
               && MyState.Pad.IsManualControl == false;
    }

    /// <summary>
    /// ボール持ちかどうか
    /// </summary>
    private bool IsBallHolder()
    {
        return BallState.IsBallHolder(MySideIndex, OrderIndex);
    }

    /// <summary>
    /// 自分がシュートターゲット
    /// </summary>
    private bool IsShotTarget()
    {
        return BallState.IsShotTarget(MySideIndex, OrderIndex);
    }

    /// <summary>
    /// 自分がパスターゲット
    /// </summary>
    private bool IsPassTarget()
    {
        return BallState.IsPassTarget(MySideIndex, OrderIndex);
    }

    /// <summary>
    /// パス待ち状態
    /// </summary>
    private bool IsPassWait()
    {
        return IsPassTarget()
               && IsBallHolder() == false
               && MyState.Pad.IsManualControl;
        // && IsShiai() //試合中チェック
    }
}