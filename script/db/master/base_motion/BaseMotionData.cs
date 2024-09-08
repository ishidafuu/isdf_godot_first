using System;
using System.Collections.Generic;

namespace db;

public class BaseMotionData
{
    public int KomaCount { get; private set; }
    public List<BaseMotionKomaData> BaseMotionKomaDataList { get; private set; }

    public BaseMotionData()
    {
        BaseMotionKomaDataList = new List<BaseMotionKomaData>();
    }

    public void Load()
    {
    }

    //     //読み込み
    //     void LoadDt(const char *CSVDir, const char *CSVFilename_ys, const char *CSVFilename_rs);
    //     //取得
    //     s32 GetDt(enDtComAct dtNo, s32 ptn, BOOL ressei_f);//参照CDtの取得
}