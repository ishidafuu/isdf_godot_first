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
        ButtonX,
        ButtonY,
    }

    public class Button : IButton
    {
        private KeyType KeyType { get; init; }
        private readonly string _keyMap;
        public bool IsPressed { get; private set; }
        public bool IsJustPressed { get; private set; }
        public bool IsJustReleased { get; private set; }
        public bool IsDoubleTapped { get; private set; }

        // 過去10フレームのJustPressed履歴を保持する配列
        private bool[] _justPressedHistory;
        private const int MAX_DOUBLE_TAP_FRAMES = 10;
        private int _currentHistoryIndex = 0;

        public Button(KeyType keyType)
        {
            KeyType = keyType;
            _keyMap = KeyType.ToString();
            _justPressedHistory = new bool[MAX_DOUBLE_TAP_FRAMES];
        }

        public void Update()
        {
            // 現在のフレームの入力状態
            bool currentPressed = Input.IsActionPressed(_keyMap);
            bool currentJustPressed = Input.IsActionJustPressed(_keyMap);

            // JustPressed履歴の更新
            _justPressedHistory[_currentHistoryIndex] = currentJustPressed;

            // インデックスを進める（循環バッファのように）
            _currentHistoryIndex = (_currentHistoryIndex + 1) % MAX_DOUBLE_TAP_FRAMES;

            // 現在のフレームの入力状態を更新
            IsPressed = currentPressed;
            IsJustPressed = currentJustPressed;
            IsJustReleased = Input.IsActionJustReleased(_keyMap);

            // 連打判定のロジック
            IsDoubleTapped = CheckDoubleTap();
        }

        private bool CheckDoubleTap()
        {
            // 現在のフレームでJustPressedでない場合は連打判定しない
            if (!IsJustPressed) return false;

            // 直近のフレームでJustPressedがあるかチェック
            int justPressedCount = 0;
            for (int i = 0; i < MAX_DOUBLE_TAP_FRAMES; i++)
            {
                if (_justPressedHistory[i])
                {
                    justPressedCount++;
                }
            }

            // 現在のフレームを含めて2回以上JustPressedがあれば連打とみなす
            return justPressedCount >= 2;
        }
    }

    private Button _keyUp;
    private Button _keyDown;
    private Button _keyLeft;
    private Button _keyRight;
    private Button _buttonA;
    private Button _buttonB;
    private Button _buttonX;
    private Button _buttonY;

    public IButton KeyUp => _keyUp;
    public IButton KeyDown => _keyDown;
    public IButton KeyLeft => _keyLeft;
    public IButton KeyRight => _keyRight;
    public IButton ButtonA => _buttonA;
    public IButton ButtonB => _buttonB;
    public IButton ButtonX => _buttonX;
    public IButton ButtonY => _buttonY;

    public override void _Ready()
    {
        base._Ready();
        _keyUp = new Button(KeyType.KeyUp);
        _keyDown = new Button(KeyType.KeyDown);
        _keyLeft = new Button(KeyType.KeyLeft);
        _keyRight = new Button(KeyType.KeyRight);
        _buttonA = new Button(KeyType.ButtonA);
        _buttonB = new Button(KeyType.ButtonB);
        _buttonX = new Button(KeyType.ButtonX);
        _buttonY = new Button(KeyType.ButtonY);

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
        _buttonX.Update();
        _buttonY.Update();
    }
}
