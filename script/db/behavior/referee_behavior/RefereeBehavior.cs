namespace db;

public partial class RefereeBehavior : BaseBehavior
{
    private RefereeState RawState => RefereeStateManager.Instance.RefereeState;
    private BallBehavior Ball => BallBehaviorManager.Instance.Get();
    private TeamBehavior[] Teams => TeamBehaviorManager.Instance.GetAllTeams();

    public IRefereeStateGetter Main => RawState;
    private IRefereeStateSetter MainSet => RawState;
}
