namespace db;

public partial class BallState
{

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
