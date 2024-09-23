using System;
using System.Collections.Generic;

namespace db;

public class BaseMotionData
{
    public int KomaCount => BaseMotionKomaDataList.Count;
    private IReadOnlyList<BaseMotionKomaData> BaseMotionKomaDataList { get; set; }

    public BaseMotionData()
    {
        BaseMotionKomaDataList = new List<BaseMotionKomaData>();
    }

    public void Load()
    {
    }

    public BaseMotionKomaData Get(int komaNo)
    {
        return BaseMotionKomaDataList[komaNo];
    }

    //     //読み込み
    //     void LoadDt(const char *CSVDir, const char *CSVFilename_ys, const char *CSVFilename_rs);
    //     //取得
    //     s32 GetDt(enDtComAct dtNo, s32 ptn, BOOL ressei_f);//参照CDtの取得
}