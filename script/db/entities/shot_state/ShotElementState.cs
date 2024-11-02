using System;

namespace db;

public class ShotElementState : IShotElementStateGetter, IShotElementStateSetter
{
    public int ShotId { get; set; }
    public int[] Element { get; set; } = new int[Defines.HsElemMax];
    public int[] Rarity { get; set; } = new int[Defines.HsElemMax];
    public int Power { get; set; }
    public int Spin { get; set; }
    public int OriginalNameNo { get; set; }

    public void Initialize()
    {
        ShotId = default;
        Power = default;
        Spin = default;
        OriginalNameNo = default;

        Array.Clear(Element, 0, Element.Length);
        Array.Clear(Rarity, 0, Rarity.Length);
    }
}

public interface IShotElementStateGetter
{
    int ShotId { get; }
    int[] Element { get; }
    int[] Rarity { get; }
    int Power { get; }
    int Spin { get; }
    int OriginalNameNo { get; }
}

public interface IShotElementStateSetter
{
    int ShotId { set; }
    int[] Element { set; }
    int[] Rarity { set; }
    int Power { set; }
    int Spin { set; }
    int OriginalNameNo { set; }
    void Initialize();
}