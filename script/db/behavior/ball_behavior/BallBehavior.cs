namespace db;

public partial class BallBehavior : BaseBehavior
{
    private RefereeState RefereeState => RefereeStateManager.Instance.RefereeState;
    private BallState RawState => BallStateManager.Instance.Get();
    private TeamState[] Teams => TeamStateManager.Instance.GetAllTeams();

    // private readonly BallCompositeState _rawComposite = new BallCompositeState();

    public IBallMainStateGetter Main => RawState.Main;
    public IBallPassStateGetter Pass => RawState.Pass;
    public IBallShotStateGetter Shot => RawState.Shoot;
    // public IBallCompositeStateGetter Composite => _rawComposite;

    public IBallMainStateSetter MainSet => RawState.Main;
    public IBallPassStateSetter PassSet => RawState.Pass;
    public IBallShotStateSetter ShotSet => RawState.Shoot;
}
