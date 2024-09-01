namespace db;

public class DataManager
{
    private static DataManager _instance;
    public static DataManager Instance => _instance ??= new DataManager();

    private ComActionData ComActionData { get; } = new();
    private ComPlanData ComPlanData { get; } = new();
    private RankData RankData { get; } = new();

    private DataManager()
    {
        Setup();
    }

    private void Setup()
    {
        ComActionData.Load();
        ComPlanData.Load();
        RankData.Load();
    }
}