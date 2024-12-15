namespace db;

/// <summary>
/// X軸方向
/// </summary>
public enum DirectionXType
{
    Left = -1,
    Neutral = 0,
    Right = +1
}

/// <summary>
/// Z軸方向
/// </summary>
public enum DirectionZType
{
    Forward = -1,
    Neutral = 0,
    Backward = +1
}


/// <summary>
/// 十字方向
/// </summary>
public enum DirectionCrossType
{
    Neutral = 0,
    Up = 1,
    Down = 2,
    Left = 3,
    Right = 4
}