namespace db;

public class BallBehaviorManager
{
    private static BallBehaviorManager _instance;
    public static BallBehaviorManager Instance => _instance ??= new BallBehaviorManager();

    private readonly BallBehavior _referee = new BallBehavior();

    public BallBehavior Get() => _referee;

    private BallBehaviorManager()
    {
    }

    public void UpdateProgress()
    {
        // _referee.UpdateProgress();
    }
}