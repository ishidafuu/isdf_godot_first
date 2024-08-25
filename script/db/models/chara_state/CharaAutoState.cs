namespace db;

/// <summary>
/// オート情報
/// </summary>
public class CharaAutoState
{
    public AutoType AutoType { get; set; }
    public DirectionX DirectionX { get; set; }
    public DirectionZ DirectionZ { get; set; }

    public bool IsDash { get; set; }

    // 一旦止まったフラグ
    public bool IsDashStopped { get; set; }

    public void Initialize()
    {
        AutoType = default;
        DirectionX = default;
        DirectionZ = default;
        IsDash = false;
        IsDashStopped = false;
    }
}