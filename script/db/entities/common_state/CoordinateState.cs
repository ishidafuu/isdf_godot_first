namespace db;

/// <summary>
/// 座標と移動に関する状態を管理するクラス
/// </summary>
public class CoordinateState
{
    public DirectionXType DirectionX { get; set; }
    public DirectionZType DirectionZ { get; set; }
    public DirectionXType DashDirection { get; set; }
    public int X { get; set; }
    public int Y { get; set; }
    public int Z { get; set; }
    public int VelocityX { get; set; }
    public int VelocityY { get; set; }
    public int VelocityZ { get; set; }
    public DownCounter FrictionCount { get; set; }
    public int Priority { get; set; }
    public Rect HitBox { get; set; }

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
        FrictionCount.Clear();
        Priority = 0;
        HitBox.Initialize();
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
        return Defines.Hypot(X - targetX, Z - targetZ);
    }

    /// <summary>
    /// 速度０
    /// </summary>
    public void ZeroVelocity()
    {
        VelocityX = 0;
        VelocityY = 0;
        VelocityZ = 0;
    }

    /// <summary>
    /// ゲームセット時の座標設定
    /// </summary>
    public void SetGameOverCoordinate(int sideNo, bool isInfield)
    {
        Y = 0;
        ZeroVelocity();
        var isRightSide = sideNo == 1;
        DirectionX = isInfield ^ isRightSide
            ? DirectionXType.Right
            : DirectionXType.Left;
        DirectionZ = DirectionZType.Neutral;
    }
}