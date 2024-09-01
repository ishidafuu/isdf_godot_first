namespace db;

public partial class CharaBehavior
{
    private int MySideIndex => MyState.Index.SideIndex;
    private int EnemySideIndex => MySideIndex == 0 ? 1 : 0;
    private int MemberIndex => MyState.Index.MemberIndex;
    private int OrderIndex => MyState.Order.OrderIndex;

    private CharaState MyState => CharaStateManager.Instance.Get(MySideIndex, MemberIndex);
    private CharaState[] MySideCharas => CharaStateManager.Instance.GetSideCharas(MySideIndex);
    private CharaState[] MySideOrders => CharaStateManager.Instance.GetSideOrders(MySideIndex);
    private CharaState[] EnemySideCharas => CharaStateManager.Instance.GetSideCharas(EnemySideIndex);
    private CharaState[] EnemySideOrders => CharaStateManager.Instance.GetSideOrders(EnemySideIndex);
    private BallState BallState => BallStateManager.Instance.BallState;
    private RefereeState RefereeState => RefereeStateManager.Instance.RefereeState;
    private TeamState MyTeam => TeamStateManager.Instance.Get(MySideIndex);
    private ComState MyCom => ComStateManager.Instance.Get(MySideIndex);
    private TeamState EnemyTeam => TeamStateManager.Instance.Get(EnemySideIndex);

    public CharaBehavior()
    {
    }
}