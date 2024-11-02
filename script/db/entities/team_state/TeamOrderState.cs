namespace db;

/// <summary>
/// オーダー番号に関するステータス
/// </summary>
public class TeamOrderState : ITeamOrderStateGetter, ITeamOrderStateSetter
{
    public int[] MemberOfOrder { get; } = new int[Defines.MemberCount];
    public int[] OrderOfMember { get; } = new int[Defines.MemberCount];

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

public interface ITeamOrderStateGetter
{
    int[] MemberOfOrder { get; }
    int[] OrderOfMember { get; }
    int MemberIndex(int orderIndex);
    int OrderIndex(int memberIndex);
}

public interface ITeamOrderStateSetter
{
    void ChangeOrder(int orderA, int orderB);
}