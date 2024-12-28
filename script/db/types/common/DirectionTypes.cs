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

/// <summary>
/// DirectionTypes拡張メソッド
/// </summary>
public static class DirectionTypesExtensions
{
    /// <summary>
    /// X軸方向を十字方向に変換
    /// </summary>
    public static DirectionCrossType ToCrossType(this DirectionXType type)
    {
        return type switch
        {
            DirectionXType.Left => DirectionCrossType.Left,
            DirectionXType.Right => DirectionCrossType.Right,
            _ => DirectionCrossType.Neutral,
        };
    }

    /// <summary>
    /// Z軸方向を十字方向に変換
    /// </summary>
    public static DirectionCrossType ToCrossType(this DirectionZType type)
    {
        return type switch
        {
            DirectionZType.Forward => DirectionCrossType.Up,
            DirectionZType.Backward => DirectionCrossType.Down,
            _ => DirectionCrossType.Neutral,
        };
    }
}