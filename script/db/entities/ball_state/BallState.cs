namespace db;

public class BallState
{
    public BallMainState Main { get; } = new BallMainState();
    public BallPassState Pass { get; } = new BallPassState();
    public BallShotState Shoot { get; } = new BallShotState();


    public BallState()
    {
        Initialize();
    }


    /// <summary>
    /// 初期化（ゼロクリア）
    /// </summary>
    public void Initialize()
    {
        Main.Initialize();
        Pass.Initialize();
        Shoot.Initialize();
    }
}
