namespace db;

/// <summary>
/// オーダー番号に関するステータス
/// </summary>
public class TeamOrderState
{
    public int[] MemberOfOrder { get; private set; } = new int[Defines.MemberCount];
    public int[] OrderOfMember { get; private set; } = new int[Defines.MemberCount];

    public int MemberIndex(int orderIndex)
    {
        return MemberOfOrder[orderIndex];
    }

    public int OrderIndex(int memberIndex)
    {
        return OrderOfMember[memberIndex];
    }

    /// <summary>
    /// オーダー入れ替え
    /// </summary>
    public void ChangeOrder(int orderA, int orderB)
    {
        var memberA = MemberIndex(orderA);
        var memberB = MemberIndex(orderB);

        MemberOfOrder[orderA] = memberB;
        MemberOfOrder[orderB] = memberA;
        OrderOfMember[memberA] = orderB;
        OrderOfMember[memberB] = orderA;
    }
}