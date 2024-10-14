namespace db;

public class TmpStateManager
{
    private static TmpStateManager _instance;
    public static TmpStateManager Instance => _instance ??= new TmpStateManager();

    private readonly TmpState _tmpState = new TmpState();
    public TmpState TmpState => _tmpState;

    private TmpStateManager()
    {
        _tmpState.Clear();
    }
}