using Godot.Collections;

namespace db;

public class ShotGouseiState
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