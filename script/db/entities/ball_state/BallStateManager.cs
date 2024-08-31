namespace db;

public class BallStateManager
{
    private static BallStateManager _instance;
    public static BallStateManager Instance => _instance ??= new BallStateManager();

    private readonly BallState _ballState = new BallState();
    public BallState BallState => _ballState;

    private BallStateManager()
    {
        _ballState.Initialize();
    }
}