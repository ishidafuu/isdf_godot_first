namespace db;

public class ShotGouseiState : IShotGouseiStateGetter, IShotGouseiStateSetter
{
    public ShotElementState Element { get; set; } = new();
    public bool IsSet { get; set; }
    public bool IsSyn { get; set; }

    public void Initialize()
    {
        Element.Initialize();
        IsSet = false;
        IsSyn = false;
    }
}

public interface IShotGouseiStateGetter
{
    ShotElementState Element { get; }
    bool IsSet { get; }
    bool IsSyn { get; }
}

public interface IShotGouseiStateSetter
{
    ShotElementState Element { set; }
    bool IsSet { set; }
    bool IsSyn { set; }
    void Initialize();
}