using isdf;

namespace db;

public class DummyPad : IPad
{
    public IButton KeyUp { get; } = new DummyButton();
    public IButton KeyDown { get; } = new DummyButton();
    public IButton KeyLeft { get; } = new DummyButton();
    public IButton KeyRight { get; } = new DummyButton();
    public IButton ButtonA { get; } = new DummyButton();
    public IButton ButtonB { get; } = new DummyButton();
}

public class DummyButton : IButton
{
    public bool IsPressed { get; } = false;
    public bool IsJustPressed { get; } = false;
    public bool IsJustReleased { get; } = false;

    public void Update()
    {
    }

}