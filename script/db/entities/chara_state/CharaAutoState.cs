namespace db;

/// <summary>
/// オート情報
/// </summary>
public class CharaAutoState : ICharaAutoState
{
    public AutoType AutoType { get; set; }
    public DirectionXType DirectionX { get; set; }
    public DirectionZType DirectionZ { get; set; }

    public bool IsDash { get; set; }

    // 一旦止まったフラグ
    public bool IsDashStopped { get; set; }

    // フリーアクションフラグ
    public bool IsFreeAction { get; set; }

    public void Initialize()
    {
        AutoType = default;
        DirectionX = default;
        DirectionZ = default;
        IsDash = false;
        IsDashStopped = false;
        IsFreeAction = false;
    }
}

public interface ICharaAutoState
{
    AutoType AutoType { get; }
    DirectionXType DirectionX { get; }
    DirectionZType DirectionZ { get; }
    bool IsDash { get; }
    bool IsDashStopped { get; }
    bool IsFreeAction { get; }
}