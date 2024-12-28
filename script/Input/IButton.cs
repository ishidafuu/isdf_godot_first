namespace isdf;

public interface IButton
{
    bool IsPressed { get; }
    bool IsJustPressed { get; }
    bool IsJustReleased { get; }
    bool IsDoubleTapped { get; }
    void Update();
}