using System;

namespace db;

public class TmpState
{
    public int[] sltgXZ { get; set; } = new int[Defines.DBMEMBER_INF];
    public enNaiyaTag[] sltg_f { get; set; } = new enNaiyaTag[Defines.DBMEMBER_INF];
    public int[] tgOrd { get; set; } = new int[Defines.DBMEMBER_INF];
    public int[] sortDt { get; set; } = new int[Defines.DBMEMBER_INF];

    public void Clear()
    {
        Array.Clear(sltgXZ);
        Array.Clear(sltg_f);
        Array.Clear(tgOrd);
        Array.Clear(sortDt);
    }

}