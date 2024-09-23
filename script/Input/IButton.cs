namespace isdf;

public interface IButton
{
    bool IsPressed { get; }
    bool IsJustPressed { get; }
    bool IsJustReleased { get; }
    void Update();
}