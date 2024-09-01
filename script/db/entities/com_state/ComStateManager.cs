namespace db;

public class ComStateManager
{
    private static ComStateManager _instance;
    public static ComStateManager Instance => _instance ??= new ComStateManager();

    private readonly ComState[] _all = new ComState[Defines.SideCount];

    public ComState[] GetAllTeams() => _all;

    public ComState Get(int side) => _all[side];

    private ComStateManager()
    {
        Setup();
    }

    private void Setup()
    {
        for (var i = 0; i < _all.Length; i++)
        {
            _all[i] = new ComState(i);
        }
    }

}