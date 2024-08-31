namespace db;

public class TeamStateManager
{
    private static TeamStateManager _instance;
    public static TeamStateManager Instance => _instance ??= new TeamStateManager();

    private readonly TeamState[] _allTeams = new TeamState[Defines.SideCount];

    public TeamState[] GetAllTeams() => _allTeams;

    public TeamState Get(int side) => _allTeams[side];

    private TeamStateManager()
    {
        Setup();
    }

    private void Setup()
    {
        for (var i = 0; i < _allTeams.Length; i++)
        {
            _allTeams[i] = new TeamState(i);
        }
    }

}