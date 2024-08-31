namespace db;

public class TeamState
{

    public int SideIndex { get; }

    public TeamState(int sideIndex)
    {
        SideIndex = sideIndex;
        Initialize();
    }

    public void Initialize()
    {
    }

}