namespace db;

public class TeamAiStateManager
{
    private static TeamAiStateManager _instance;
    public static TeamAiStateManager Instance => _instance ??= new TeamAiStateManager();

    private readonly TeamAiState[] _all = new TeamAiState[Defines.SideCount];

    public TeamAiState[] GetAllTeams() => _all;

    public TeamAiState Get(int side) => _all[side];

    private TeamAiStateManager()
    {
        Setup();
    }

    private void Setup()
    {
        for (var i = 0; i < _all.Length; i++)
        {
            _all[i] = new TeamAiState(i);
        }
    }

}
