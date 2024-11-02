namespace db;

/// <summary>
/// キャラ情報に関するステータス
/// </summary>
public class CharaProfileStateGetter : ICharaProfileStateGetter, ICharaProfileStateSetter
{
    // キャラID
    public long CharaId { get; private set; }

    public void Initialize()
    {
        CharaId = 0;
    }
}

public interface ICharaProfileStateGetter
{
    long CharaId { get; }
}

public interface ICharaProfileStateSetter
{
    void Initialize();
}