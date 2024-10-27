namespace isdf;

public interface IPad
{
    public IButton KeyUp { get; }
    public IButton KeyDown { get; }
    public IButton KeyLeft { get; }
    public IButton KeyRight { get; }
    public IButton ButtonA { get; }
    public IButton ButtonB { get; }
}

public static class IPadExtensions
{
    public static bool IsPressedAnyCross(this IPad pad)
    {
        return pad.KeyUp.IsPressed || pad.KeyDown.IsPressed || pad.KeyLeft.IsPressed || pad.KeyRight.IsPressed;
    }
    
    public static bool IsJustPressedAnyButton(this IPad pad)
    {
        return pad.ButtonA.IsJustPressed || pad.ButtonB.IsJustPressed;
    }
}