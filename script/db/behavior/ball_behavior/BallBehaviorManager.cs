namespace db;

public class BallBehaviorManager
{
    private static BallBehaviorManager _instance;
    public static BallBehaviorManager Instance => _instance ??= new BallBehaviorManager();

    private readonly BallBehavior _ball = new();

    public BallBehavior Get() => _ball;

    private BallBehaviorManager()
    {
    }

    public void UpdateProgress()
    {
        // _referee.UpdateProgress();
    }
}
