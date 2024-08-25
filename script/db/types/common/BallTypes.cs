namespace db;

/// <summary>
/// ボール状態
/// </summary>
public enum BallMotionType
{
    Free,
    Hold,
    Shoot,
    Pass,
    Bound,
    Referee
}

/// <summary>
/// ジャンプボール状態
/// </summary>
public enum JumpBallType
{
    Normal,
    Jpball,
    Side0,
    Side1
}