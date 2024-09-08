namespace db;

public class MasterManager
{
    private static MasterManager _instance;
    public static MasterManager Instance => _instance ??= new MasterManager();

    public ComActionMaster ComActionMaster { get; } = new();
    public ComPlanMaster ComPlanMaster { get; } = new();
    public RankMaster RankMaster { get; } = new();
    public SettingMaster SettingMaster { get; } = new();
    public BaseMotionMaster BaseMotionMaster { get; } = new();

    private MasterManager()
    {
        LoadAll();
    }

    private void LoadAll()
    {
        ComActionMaster.Load();
        ComPlanMaster.Load();
        RankMaster.Load();
        SettingMaster.Load();
        BaseMotionMaster.Load();
    }
}