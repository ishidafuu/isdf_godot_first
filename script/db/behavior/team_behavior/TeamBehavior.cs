namespace db;

public partial class TeamBehavior
{
    private int MySide => MyTeam.SideIndex;
    private int EnemySide => MySide == 0 ? 1 : 0;

    private CharaState[] MySideCharas => CharaStateManager.Instance.GetSideCharas(MySide);
    private CharaState[] MySideOrders => CharaStateManager.Instance.GetSideOrders(MySide);
    private CharaState[] EnemySideCharas => CharaStateManager.Instance.GetSideCharas(EnemySide);
    private CharaState[] EnemySideOrders => CharaStateManager.Instance.GetSideOrders(EnemySide);
    private BallState BallState => BallStateManager.Instance.BallState;
    private RefereeState RefereeState => RefereeStateManager.Instance.RefereeState;
    private TeamState MyTeam => TeamStateManager.Instance.Get(MySide);
    private ComState MyCom => ComStateManager.Instance.Get(MySide);
    private TeamState EnemyTeam => TeamStateManager.Instance.Get(EnemySide);

    public TeamBehavior()
    {
    }
    

}