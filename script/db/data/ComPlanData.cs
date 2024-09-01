using System;
using db.types.com;

namespace db.masters;

public class ComPlanData
{
    public static int PlanTypeLength => Enum.GetValues<enComPlanType>().Length;
    public int[][] Sheet { get; private set; }

    public ComPlanData()
    {
        var planTypeLength = PlanTypeLength;
        Sheet = new int[planTypeLength][];

        for (var i = 0; i < planTypeLength; i++)
        {
            Sheet[i] = new int[Defines.ComPattern];
        }
    }

    //     //読み込み
    //     void LoadDt(const char *CSVDir, const char *CSVFilename_ys, const char *CSVFilename_rs);
    //     //取得
    //     s32 GetDt(enDtComAct dtNo, s32 ptn, BOOL ressei_f);//参照CDtの取得
}