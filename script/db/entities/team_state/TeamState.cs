namespace db;

public partial class TeamState
{
    public int SideIndex { get; }
    private int EnemySideIndex => SideIndex == 0 ? 1 : 0;

    public TeamOrderState OrderState { get; } = new TeamOrderState();
    public TeamPositionState PositionState { get; } = new TeamPositionState();
    public TeamSemiAutoState SemiAutoState { get; } = new TeamSemiAutoState();
    public TeamMainState MainState { get; } = new TeamMainState();

    private CharaBehavior[] MySideCharas => CharaBehaviorManager.Instance.GetSideCharas(SideIndex);
    private CharaState[] MySideOrders => CharaStateManager.Instance.GetSideOrders(SideIndex);
    private CharaBehavior[] EnemySideCharas =>  CharaBehaviorManager.Instance.GetSideCharas(EnemySideIndex);
    private CharaState[] EnemySideOrders => CharaStateManager.Instance.GetSideOrders(EnemySideIndex);

    public TeamState(int sideIndex)
    {
        SideIndex = sideIndex;
        Initialize();
    }

    public void Initialize()
    {
    }

}