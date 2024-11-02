namespace db;

public partial class CharaState
{
    public CharaIndexStateGetter Index { get; set; } = new();
    public CharaOrderStateGetter Order { get; set; } = new();
    public CharaInputStateGetter Input { get; set; } = new();
    public CharaLiveStateGetter Live { get; set; } = new();
    public CoordinateState Coordinate { get; set; } = new();
    public CharaMotionStateGetter Motion { get; set; } = new();
    public CharaAutoStateGetter Auto { get; set; } = new();
    public CharaAutoStateGetter NextAuto { get; set; } = new();
    public CharaComAutoStateGetter ComAuto { get; set; } = new();
    public CharaGouseiStateGetter Gousei { get; set; } = new();
    public CharaDamageStateGetter Damage { get; set; } = new();
    public CharaLevelStateGetter Level { get; set; } = new();
    public CharaDistanceStateGetter Distance { get; set; } = new();
    public CharaMoveStateGetter Move { get; set; } = new();
    public CharaAirStateGetter Air { get; set; } = new();
    public CharaViewStateGetter View { get; set; } = new();
    public CharaCatchStateGetter Catch { get; set; } = new();
    public CharaShootStateGetter Shoot { get; set; } = new();
    public CharaCourtStateGetter Court { get; set; } = new();
    public CharaPassStateGetter Pass { get; set; } = new();
    public CharaDashmanStateGetter Dashman { get; set; } = new();
    public CharaComOnlyStateGetter ComOnly { get; set; } = new();
    public CharaBallEffectStateGetter BallEffect { get; set; } = new();

    public CharaState(int sideIndex, int memberIndex)
    {
        Index.Initialize(sideIndex, memberIndex);
        Initialize();
    }

    /// <summary>
    /// 初期化（ゼロクリア）
    /// Indexは変化しないため省く
    /// </summary>
    private void Initialize()
    {
        // 初期化時はメンバーIndexをポジション番号とする
        Order.Initialize(Index.MemberIndex);
        Input.Initialize();
        Live.Initialize();
        Coordinate.Initialize();
        Motion.Initialize();
        Auto.Initialize();
        NextAuto.Initialize();
        ComAuto.Initialize();
        Gousei.Initialize();
        Damage.Initialize();
        Level.Initialize();
        Distance.Initialize();
        Move.Initialize();
        Air.Initialize();
        View.Initialize();
        Catch.Initialize();
        Shoot.Initialize();
        Court.Initialize();
        Pass.Initialize();
        Dashman.Initialize();
        ComOnly.Initialize();
        BallEffect.Initialize();
    }

    /// <summary>
    /// 試合開始準備
    /// </summary>
    public void Setup(long charaId)
    {
        Initialize();
    }
}