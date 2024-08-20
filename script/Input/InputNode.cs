using Godot;
using isdf_godot_first.script.Input;

public partial class InputNode : Node
{
    public enum KeyType
    {
        KeyUp,
        KeyDown,
        KeyLeft,
        KeyRight,
        ButtonA,
        ButtonB,
    }

    public class Button : IButton
    {
        private KeyType KeyType { get; init; }
        private readonly string _keyMap;
        public bool IsPressed { get; private set; }
        public bool IsJustPressed { get; private set; }
        public bool IsJustReleased { get; private set; }

        public Button(KeyType keyType)
        {
            KeyType = keyType;
            _keyMap = KeyType.ToString();
        }

        public void Update()
        {
            IsPressed = Input.IsActionPressed(_keyMap);
            IsJustPressed = Input.IsActionJustPressed(_keyMap);
            IsJustReleased = Input.IsActionJustReleased(_keyMap);
        }
    }

    private Button _keyUp;
    private Button _keyDown;
    private Button _keyLeft;
    private Button _keyRight;
    private Button _buttonA;
    private Button _buttonB;

    public Button KeyUp => _keyUp;
    public Button KeyDown => _keyDown;
    public Button KeyLeft => _keyLeft;
    public Button KeyRight => _keyRight;
    public Button ButtonA => _buttonA;
    public Button ButtonB => _buttonB;

    public override void _Ready()
    {
        base._Ready();
        _keyUp = new(KeyType.KeyUp);
        _keyDown = new(KeyType.KeyDown);
        _keyLeft = new(KeyType.KeyLeft);
        _keyRight = new(KeyType.KeyRight);
        _buttonA = new(KeyType.ButtonA);
        _buttonB = new(KeyType.ButtonB);
    }

    public override void _Process(double delta)
    {
        _keyUp.Update();
        _keyDown.Update();
        _keyLeft.Update();
        _keyRight.Update();
        _buttonA.Update();
        _buttonB.Update();
    }
}