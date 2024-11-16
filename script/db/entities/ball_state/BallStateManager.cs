namespace db;

public class BallStateManager
{
    private static BallStateManager _instance;
    public static BallStateManager Instance => _instance ??= new BallStateManager();

    private readonly BallMainState _main = new BallMainState();
    private readonly BallPassState _pass = new BallPassState();
    private readonly BallShotState _shoot = new BallShotState();
    public BallMainState Main => _main;
    public BallPassState Pass => _pass;
    public BallShotState Shot => _shoot;


    private BallStateManager()
    {
        _main.Initialize();
        _pass.Initialize();
        _shoot.Initialize();
    }
}
