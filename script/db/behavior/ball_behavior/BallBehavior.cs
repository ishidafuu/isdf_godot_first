namespace db;

public partial class BallBehavior
{
    private RefereeState RefereeState => RefereeStateManager.Instance.RefereeState;
    private BallState BallState => BallStateManager.Instance.BallState;
    private TeamState[] Teams => TeamStateManager.Instance.GetAllTeams();

    public BallBehavior()
    {
    }
}