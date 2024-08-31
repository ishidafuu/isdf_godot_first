namespace db;

/// <summary>
/// 番号・ポジションに関するステータス
/// </summary>
public class CharaOrderState
{
    // ポジション番号（0～6）
    public int OrderIndex { get; set; }

    public void Initialize(int orderIndex)
    {
        OrderIndex = orderIndex;
    }

}