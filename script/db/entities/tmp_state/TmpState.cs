using System;

namespace db;

public class TmpState
{
    public int[] sltgX { get; set; } = new int[Defines.DBMEMBER_INF];
    public int[] sltgXZ { get; set; } = new int[Defines.DBMEMBER_INF];
    public enNaiyaTag[] sltg_f { get; set; } = new enNaiyaTag[Defines.DBMEMBER_INF];
    public OrderIndexType[] targetOrder { get; set; } = new OrderIndexType[Defines.DBMEMBER_INF];
    public int[] sortValue { get; set; } = new int[Defines.DBMEMBER_INF];

    public void Clear()
    {
        Array.Clear(sltgXZ);
        Array.Clear(sltg_f);
        Array.Clear(targetOrder);
        Array.Clear(sortValue);
    }

}