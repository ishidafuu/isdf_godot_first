namespace db;

/// <summary>
/// 座標と移動に関する状態を管理するクラス
/// </summary>
public class CoordinateState
{
    public DirectionX DirectionX { get; set; }
    public DirectionZ DirectionZ { get; set; }
    public DirectionX DashDirection { get; set; }
    public int X { get; set; }
    public int Y { get; set; }
    public int Z { get; set; }
    public int VelocityX { get; set; }
    public int VelocityY { get; set; }
    public int VelocityZ { get; set; }
    public int FrictionCount { get; set; }
    public int Priority { get; set; }

    public CoordinateState()
    {
        Initialize();
    }

    /// <summary>
    /// 状態を初期化します
    /// </summary>
    public void Initialize()
    {
        DirectionX = default;
        DirectionZ = default;
        DashDirection = default;
        X = 0;
        Y = 0;
        Z = 0;
        VelocityX = 0;
        VelocityY = 0;
        VelocityZ = 0;
        FrictionCount = 0;
        Priority = 0;
    }

    /// <summary>
    /// X方向の速度の符号を返します
    /// </summary>
    public int SignVelocityX()
    {
        return VelocityX switch
        {
            > 0 => 1,
            < 0 => -1,
            _ => 0
        };
    }

    /// <summary>
    /// Z方向の速度の符号を返します
    /// </summary>
    public int SignVelocityZ()
    {
        return VelocityZ switch
        {
            > 0 => 1,
            < 0 => -1,
            _ => 0
        };
    }
}