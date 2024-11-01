namespace db;

/// <summary>
/// キャラ情報に関するステータス
/// </summary>
public class CharaProfileState : ICharaProfileState
{
    // キャラID
    public long CharaId { get; private set; }

    public void Initialize()
    {
        CharaId = 0;
    }
}

public interface ICharaProfileState
{
    long CharaId { get; }
}