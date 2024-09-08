namespace db;

public class RefereeBehaviorManager
{
    private static RefereeBehaviorManager _instance;
    public static RefereeBehaviorManager Instance => _instance ??= new RefereeBehaviorManager();

    private readonly RefereeBehavior _referee = new RefereeBehavior();

    public RefereeBehavior Get() => _referee;

    private RefereeBehaviorManager()
    {
    }

    public void UpdateProgress()
    {
        // _referee.UpdateProgress();
    }
}