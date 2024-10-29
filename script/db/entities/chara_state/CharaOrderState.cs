namespace db;

/// <summary>
/// 番号・ポジションに関するステータス
/// </summary>
public class CharaOrderState : ICharaOrderState
{
    // ポジション番号（0～6）
    public OrderIndexType OrderIndex { get; set; }

    public void Initialize(int orderIndex)
    {
        OrderIndex = (OrderIndexType)orderIndex;
    }

    public bool IsInfield => OrderIndex <= OrderIndexType.Infield3;
    public bool IsOutfield => IsInfield == false;

    public OrderFieldType GetOrderFieldType()
    {
        return OrderIndex switch
        {
            OrderIndexType.Outfield2 => OrderFieldType.Outfield2,
            OrderIndexType.Outfield3 => OrderFieldType.Outfield3,
            OrderIndexType.Outfield4 => OrderFieldType.Outfield4,
            _ => OrderFieldType.Infield,
        };
    }
}

public interface ICharaOrderState
{
    OrderIndexType OrderIndex { get; }
    bool IsInfield { get; }
    bool IsOutfield { get; }
    OrderFieldType GetOrderFieldType();
}