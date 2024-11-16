namespace db;

public partial class TeamBehavior : BaseBehavior
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
    private TeamAiState MyTeamAi => TeamAiStateManager.Instance.Get(MySide);
    private TeamState EnemyTeam => TeamStateManager.Instance.Get(EnemySide);

    public CharaState CaptainChara => MySideCharas[0];

    public ITeamOrderStateGetter Order => MyTeam.Order;
    public ITeamPositionStateGetter Position => MyTeam.Position;
    public ITeamSemiAutoStateGetter SemiAuto => MyTeam.SemiAuto;
    public ITeamMainStateGetter MainState => MyTeam.MainState;
    public ITeamAiMainStateGetter AiMain => MyTeamAi.Main;
    public ITeamAiMemberStateGetter AiMember => MyTeamAi.Member;
    public ITeamAiPatternStateGetter AiPattern => MyTeamAi.Pattern;
    public ITeamAiAttackStateGetter AiAttack => MyTeamAi.Attack;
    public ITeamAiActionStateGetter ComAction(int memberIndex) => MyTeamAi.Action[memberIndex];
    public ITeamAiInfieldStateGetter ComAction(OrderIndexType orderIndexType) => MyTeamAi.Infield[(int)orderIndexType];

    public TeamBehavior()
    {
    }

}
