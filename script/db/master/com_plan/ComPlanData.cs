using System;

namespace db;

public class ComPlanData
{
    public static int PlanTypeLength => Enum.GetValues<AiPlanType>().Length;
    private int[][] Sheet { get; set; }

    public ComPlanData()
    {
        Sheet = new int[PlanTypeLength][];

        for (var i = 0; i < PlanTypeLength; i++)
        {
            Sheet[i] = new int[Defines.ComPattern];
        }
    }

    public void Load()
    {
    }

    public int Get(AiPlanType aiPlanType, int comPattern)
    {
        return Sheet[(int)aiPlanType][comPattern];
    }

    //     //読み込み
    //     void LoadDt(const char *CSVDir, const char *CSVFilename_ys, const char *CSVFilename_rs);
    //     //取得
    //     s32 GetDt(enDtComAct dtNo, s32 ptn, BOOL ressei_f);//参照CDtの取得
}