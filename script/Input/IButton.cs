namespace isdf_godot_first.script.Input;

public interface IButton
{
    bool IsPressed { get; }
    bool IsJustPressed { get; }
    bool IsJustReleased { get; }
    void Update();
}