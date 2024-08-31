namespace db;

/// <summary>
/// キャラ情報に関するステータス
/// </summary>
public class CharaProfileState
{
    // キャラID
    public long CharaId { get; private set; }

    public void Initialize()
    {
        CharaId = 0;
    }
}