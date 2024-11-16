namespace db;

public partial class TeamBehavior : BaseBehavior
{
    private int MySideIndex => MyTeamState.SideIndex;
    private int EnemySideIndex => MySideIndex == 0 ? 1 : 0;

    // private CharaState[] MySideCharas => CharaStateManager.Instance.GetSideCharas(MySideIndex);
    // private CharaState[] MySideOrders => CharaStateManager.Instance.GetSideOrders(MySideIndex);
    private CharaBehavior[] MySideOrders => CharaBehaviorManager.Instance.GetSideCharas(MySideIndex);

    private CharaBehavior[] EnemySideOrders => CharaBehaviorManager.Instance.GetSideCharas(EnemySideIndex);
    // private CharaState[] EnemySideCharas => CharaStateManager.Instance.GetSideCharas(EnemySideIndex);
    // private CharaState[] EnemySideOrders => CharaStateManager.Instance.GetSideOrders(EnemySideIndex);

    private BallBehavior Ball => BallBehaviorManager.Instance.Get();
    private RefereeState RefereeState => RefereeStateManager.Instance.RefereeState;
    private TeamState MyTeamState => TeamStateManager.Instance.Get(MySideIndex);
    private TeamAiState MyTeamAiState => TeamAiStateManager.Instance.Get(MySideIndex);
    private TeamBehavior EnemyTeam => TeamBehaviorManager.Instance.Get(EnemySideIndex);

    public CharaBehavior CaptainChara => MySideOrders[0];

    public ITeamOrderStateGetter Order => MyTeamState.Order;
    public ITeamPositionStateGetter Position => MyTeamState.Position;
    public ITeamSemiAutoStateGetter SemiAuto => MyTeamState.SemiAuto;
    public ITeamMainStateGetter Main => MyTeamState.MainState;
    public ITeamAiMainStateGetter AiMain => MyTeamAiState.Main;
    public ITeamAiMemberStateGetter AiMember => MyTeamAiState.Member;
    public ITeamAiPatternStateGetter AiPattern => MyTeamAiState.Pattern;
    public ITeamAiAttackStateGetter AiAttack => MyTeamAiState.Attack;
    public ITeamAiActionStateGetter AiAction(OrderIndexType orderIndexType) => MyTeamAiState.Action[(int)orderIndexType];
    public ITeamAiInfieldStateGetter AiInfield(OrderIndexType orderIndexType) => MyTeamAiState.Infield[(int)orderIndexType];
}
