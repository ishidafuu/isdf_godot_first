namespace db;

/// <summary>
/// 座標と移動に関する状態を管理するクラス
/// </summary>
public class CoordinateState
{
    public DirectionXType DirectionXType { get; set; }
    public DirectionZType DirectionZType { get; set; }
    public DirectionXType DashDirection { get; set; }
    public int X { get; set; }
    public int Y { get; set; }
    public int Z { get; set; }
    public int VelocityX { get; set; }
    public int VelocityY { get; set; }
    public int VelocityZ { get; set; }
    public Counter FrictionCount { get; set; }
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
        DirectionXType = default;
        DirectionZType = default;
        DashDirection = default;
        X = 0;
        Y = 0;
        Z = 0;
        VelocityX = 0;
        VelocityY = 0;
        VelocityZ = 0;
        FrictionCount.Clear();
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

    /// <summary>
    /// XZ距離
    /// </summary>
    public int DistanceXZ(CoordinateState target)
    {
        return DistanceXZ(target.X, target.Z);
    }

    /// <summary>
    /// XZ距離
    /// </summary>
    public int DistanceXZ(int targetX, int targetZ)
    {
        return (int)System.Math.Sqrt(System.Math.Pow(X - targetX, 2) + System.Math.Pow(Z - targetZ, 2));
    }

}