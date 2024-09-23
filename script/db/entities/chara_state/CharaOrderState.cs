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

    public bool IsInfield()
    {
        return OrderIndex < Defines.InfieldCount;
    }

    public OrderType GetOrderType()
    {
        if (OrderIndex < Defines.InfieldCount)
        {
            return OrderType.Infield;
        }

        var outfieldIndex = OrderIndex - Defines.InfieldCount;
        return outfieldIndex switch
        {
            0 => OrderType.Outfield2,
            1 => OrderType.Outfield3,
            _ => OrderType.Outfield4
        };
    }

}