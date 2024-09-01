namespace db;

public class ComStateManager
{
    private static ComStateManager _instance;
    public static ComStateManager Instance => _instance ??= new ComStateManager();

    private readonly ComState[] _allTeams = new ComState[Defines.SideCount];

    public ComState[] GetAllTeams() => _allTeams;

    public ComState Get(int side) => _allTeams[side];

    private ComStateManager()
    {
        Setup();
    }

    private void Setup()
    {
        for (var i = 0; i < _allTeams.Length; i++)
        {
            _allTeams[i] = new ComState(i);
        }
    }

}