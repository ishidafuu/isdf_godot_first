using System;
using System.Collections.Generic;

namespace db;

public class ComActionMaster
{
    public List<ComActionData> ComActionDataList { get; private set; }

    public ComActionMaster()
    {
        ComActionDataList = new List<ComActionData>();
    }

    public void Load()
    {
    }

    public ComActionData Get(int teamNo)
    {
        return ComActionDataList[teamNo];
    }

    //     //読み込み
    //     void LoadDt(const char *CSVDir, const char *CSVFilename_ys, const char *CSVFilename_rs);
    //     //取得
    //     s32 GetDt(enDtComAct dtNo, s32 ptn, BOOL ressei_f);//参照CDtの取得
}