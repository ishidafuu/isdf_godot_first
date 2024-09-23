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