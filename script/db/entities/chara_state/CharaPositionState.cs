namespace db;

/// <summary>
/// 番号・ポジションに関するステータス
/// </summary>
public class CharaPositionState
{
    // キャラのＩＤ
    public long CharaId { get; set; }

    // ポジション番号（0～6）
    public long PositionIndex { get; set; }

    public void Initialize(long positionIndex)
    {
        CharaId = 0;
        PositionIndex = positionIndex;
    }

    public void Setup(long charaId)
    {
        CharaId = charaId;
    }
}