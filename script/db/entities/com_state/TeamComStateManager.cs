namespace db;

public class TeamComStateManager
{
    private static TeamComStateManager _instance;
    public static TeamComStateManager Instance => _instance ??= new TeamComStateManager();

    private readonly TeamComState[] _all = new TeamComState[Defines.SideCount];

    public TeamComState[] GetAllTeams() => _all;

    public TeamComState Get(int side) => _all[side];

    private TeamComStateManager()
    {
        Setup();
    }

    private void Setup()
    {
        for (var i = 0; i < _all.Length; i++)
        {
            _all[i] = new TeamComState(i);
        }
    }

}