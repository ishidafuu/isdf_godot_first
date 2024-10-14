using isdf;

namespace db;

public partial class PadBehavior
{
    public int PadIndex { get; init; }
    public IPad Pad { get; set; }
    public bool IsDummy => Pad is DummyPad;

    public PadBehavior(int padIndex, IPad pad = null)
    {
        PadIndex = padIndex;
        Pad = pad ?? new DummyPad();
    }

    public void SetPad(IPad pad)
    {
        Pad = pad;
    }

}