namespace db;

public partial class CharaBehavior
{
    private int _mySide;
    private int _enemySide;
    private int _index;

    private CharaState MyState => CharaStateManager.Instance.Get(_mySide, _index);
    private CharaState[] MySide => CharaStateManager.Instance.GetSideCharas(_mySide);
    private CharaState[] EnemySide => CharaStateManager.Instance.GetSideCharas(_enemySide);

    public CharaBehavior(int side, int index)
    {
        _mySide = side;
        _enemySide = side == 0 ? 1 : 0;
        _index = index;
    }
}