namespace db;

public partial class RefereeBehavior : BaseBehavior
{
    private RefereeState MyState => RefereeStateManager.Instance.RefereeState;
    private BallMainState BallMainState => BallStateManager.Instance.BallState;
    private TeamState[] Teams => TeamStateManager.Instance.GetAllTeams();

    public RefereeBehavior()
    {
    }
}