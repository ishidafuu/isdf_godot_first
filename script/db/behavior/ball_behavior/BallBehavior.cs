namespace db;

public partial class BallBehavior : BaseBehavior
{
    private RefereeState RefereeState => RefereeStateManager.Instance.RefereeState;
    private BallMainState BallMainState => BallStateManager.Instance.BallState;
    private TeamState[] Teams => TeamStateManager.Instance.GetAllTeams();
    public IBallMainStateGetter MainState => BallMainState;
    
    

    
    public BallBehavior()
    {
    }
}
