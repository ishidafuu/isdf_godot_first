namespace db;

public class BallStateManager
{
    private static BallStateManager _instance;
    public static BallStateManager Instance => _instance ??= new BallStateManager();

    public BallState Get() => _ballState;

    private readonly BallState _ballState = new BallState();


    private BallStateManager()
    {
        _ballState.Initialize();
    }
}
