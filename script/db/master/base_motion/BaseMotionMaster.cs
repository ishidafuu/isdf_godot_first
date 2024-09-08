using System;
using System.Collections.Generic;

namespace db;

public class BaseMotionMaster
{
    private IReadOnlyList<BaseMotionData> BaseMotionDataList { get; set; }

    public BaseMotionMaster()
    {
        BaseMotionDataList = new List<BaseMotionData>();
        var charaMotionTypeLength = Enum.GetValues<CharaMotionType>().Length;
        // Sheet = new BaseMotionKomaData[charaMotionTypeLength][];
        //
        // for (var i = 0; i < charaMotionTypeLength; i++)
        // {
        //     Sheet[i] = new int[Defines.ComPattern];
        // }
    }

    public void Load()
    {
    }

    public BaseMotionData Get(CharaMotionType charaMotionType)
    {
        return BaseMotionDataList[(int)charaMotionType];
    }

    //     //読み込み
    //     void LoadDt(const char *CSVDir, const char *CSVFilename_ys, const char *CSVFilename_rs);
    //     //取得
    //     s32 GetDt(enDtComAct dtNo, s32 ptn, BOOL ressei_f);//参照CDtの取得
}