namespace db;

public partial class TeamState
{
    private int SideIndex { get; }
    private int EnemySideIndex => SideIndex == 0 ? 1 : 0;

    public TeamOrderState OrderState { get; } = new TeamOrderState();
    public TeamPositionState PositionState { get; } = new TeamPositionState();
    public TeamSemiAutoState SemiAutoState { get; } = new TeamSemiAutoState();
    public TeamMainState MainState { get; } = new TeamMainState();

    private CharaState[] MySideCharas => CharaStateManager.Instance.GetSideCharas(SideIndex);
    private CharaState[] MySideOrders => CharaStateManager.Instance.GetSideOrders(SideIndex);
    private CharaState[] EnemySideCharas => CharaStateManager.Instance.GetSideCharas(EnemySideIndex);
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