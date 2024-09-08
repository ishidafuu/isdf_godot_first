using System.Collections.Generic;

namespace db;

public class ComPlanMaster
{
    public IReadOnlyList<ComPlanData> ComPlanDataList { get; private set; }

    public ComPlanMaster()
    {
        ComPlanDataList = new List<ComPlanData>();
    }

    public void Load()
    {
    }

    public ComPlanData Get(int teamNo)
    {
        return ComPlanDataList[teamNo];
    }

    //     //読み込み
    //     void LoadDt(const char *CSVDir, const char *CSVFilename_ys, const char *CSVFilename_rs);
    //     //取得
    //     s32 GetDt(enDtComAct dtNo, s32 ptn, BOOL ressei_f);//参照CDtの取得
}