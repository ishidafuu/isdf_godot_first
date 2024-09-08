namespace db;

public partial class RefereeBehavior
{
    private RefereeState MyState => RefereeStateManager.Instance.RefereeState;
    private BallState BallState => BallStateManager.Instance.BallState;
    private TeamState[] Teams => TeamStateManager.Instance.GetAllTeams();

    public RefereeBehavior()
    {
    }
}