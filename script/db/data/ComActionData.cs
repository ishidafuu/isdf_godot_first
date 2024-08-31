using System;
using db.types.com;

namespace db.masters;

public class ComActionData
{
    public int[][] NormalSheet;
    public int[][] PinchSheet;

    public ComActionData()
    {
        var actionTypeLength = Enum.GetValues<ComActionType>().Length;
        NormalSheet = new int[actionTypeLength][];
        PinchSheet = new int[actionTypeLength][];

        for (var i = 0; i < actionTypeLength; i++)
        {
            NormalSheet[i] = new int[Defines.MemberCount];
            PinchSheet[i] = new int[Defines.MemberCount];
        }
    }

    //     //読み込み
    //     void LoadDt(const char *CSVDir, const char *CSVFilename_ys, const char *CSVFilename_rs);
    //     //取得
    //     s32 GetDt(enDtComAct dtNo, s32 ptn, BOOL ressei_f);//参照CDtの取得
}