using System;

namespace db;

public class TmpState
{
    public int[] targetX { get; set; } = new int[Defines.DBMEMBER_INF];
    public int[] targetY { get; set; } = new int[Defines.DBMEMBER_INF];
    public int[] targetZ { get; set; } = new int[Defines.DBMEMBER_INF];
    public int[] targetDist { get; set; } = new int[Defines.DBMEMBER_INF];
    public enNaiyaTag[] isSelectTarget { get; set; } = new enNaiyaTag[Defines.DBMEMBER_INF];
    public enNaiyaTag[] isSelectTarget2 { get; set; } = new enNaiyaTag[Defines.DBMEMBER_INF];
    public OrderIndexType[] targetOrder { get; set; } = new OrderIndexType[Defines.DBMEMBER_INF];
    public int[] sortValue { get; set; } = new int[Defines.DBMEMBER_INF];

    public void Clear()
    {
        Array.Clear(targetX);
        Array.Clear(targetY);
        Array.Clear(targetZ);
        Array.Clear(targetDist);
        Array.Clear(isSelectTarget);
        Array.Clear(isSelectTarget2);
        Array.Clear(targetOrder);
        Array.Clear(sortValue);
    }

}