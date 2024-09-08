namespace db;

public class TeamBehaviorManager
{
    private static TeamBehaviorManager _instance;
    public static TeamBehaviorManager Instance => _instance ??= new TeamBehaviorManager();

    /// <summary>
    /// 全チーム（Index順）
    /// </summary>
    private readonly TeamBehavior[] _allTeams = new TeamBehavior[Defines.SideCount];

    public TeamBehavior[] GetAllTeams() => _allTeams;
    public TeamBehavior Get(int sideIndex) => _allTeams[sideIndex];

    private TeamBehaviorManager()
    {
        SetupAll();
    }

    private void SetupAll()
    {
        for (var i = 0; i < _allTeams.Length; i++)
        {
            _allTeams[i] = new TeamBehavior();
        }
    }

    public void UpdateProgress()
    {
        // foreach (var team in _allTeams)
        // {
        //     team.UpdateProgress();
        // }
    }
}