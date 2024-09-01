namespace db;

public class TeamState
{
    public int SideIndex { get; }

    public TeamOrderState OrderState { get; } = new TeamOrderState();
    public TeamPositionState PositionState { get; } = new TeamPositionState();
    public TeamSemiAutoState SemiAutoState { get; } = new TeamSemiAutoState();
    public TeamMainState MainState { get; } = new TeamMainState();

    public TeamState(int sideIndex)
    {
        SideIndex = sideIndex;

        Initialize();
    }

    public void Initialize()
    {
        
        
    }

}