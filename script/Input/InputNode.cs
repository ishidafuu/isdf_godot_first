using db;
using Godot;

namespace isdf;

public partial class InputNode : Node, IPad
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

    public IButton KeyUp => _keyUp;
    public IButton KeyDown => _keyDown;
    public IButton KeyLeft => _keyLeft;
    public IButton KeyRight => _keyRight;
    public IButton ButtonA => _buttonA;
    public IButton ButtonB => _buttonB;

    public override void _Ready()
    {
        base._Ready();
        _keyUp = new Button(KeyType.KeyUp);
        _keyDown = new Button(KeyType.KeyDown);
        _keyLeft = new Button(KeyType.KeyLeft);
        _keyRight = new Button(KeyType.KeyRight);
        _buttonA = new Button(KeyType.ButtonA);
        _buttonB = new Button(KeyType.ButtonB);

        PadBehaviorManager.Instance.AttachPad(this);
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