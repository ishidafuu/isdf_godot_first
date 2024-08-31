namespace db;

public class RefereeStateManager
{
    private static RefereeStateManager _instance;
    public static RefereeStateManager Instance => _instance ??= new RefereeStateManager();

    private readonly RefereeState _refereeState = new RefereeState();
    public RefereeState RefereeState => _refereeState;

    private RefereeStateManager()
    {
        _refereeState.Initialize();
    }
}