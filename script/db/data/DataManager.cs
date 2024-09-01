namespace db;

public class DataManager
{
    private static DataManager _instance;
    public static DataManager Instance => _instance ??= new DataManager();

    public ComActionData ComActionData { get; } = new();
    public ComPlanData ComPlanData { get; } = new();
    public RankData RankData { get; } = new();
    public SettingData SettingData { get; } = new();

    private DataManager()
    {
        Setup();
    }

    private void Setup()
    {
        ComActionData.Load();
        ComPlanData.Load();
        RankData.Load();
        SettingData.Load();
    }
}