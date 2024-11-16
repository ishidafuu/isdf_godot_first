using System;

namespace db;

public class ComActionData
{
    public static int ActionTypeLength => Enum.GetValues<AiActionType>().Length;
    private int[][] NormalSheet { get; set; }
    private int[][] PinchSheet { get; set; }

    public ComActionData()
    {
        NormalSheet = new int[ActionTypeLength][];
        PinchSheet = new int[ActionTypeLength][];

        for (var i = 0; i < ActionTypeLength; i++)
        {
            NormalSheet[i] = new int[Defines.ComPattern];
            PinchSheet[i] = new int[Defines.ComPattern];
        }
    }

    public void Load()
    {
    }

    public int Get(AiActionType aiActionType, int comPattern, bool isPinch)
    {
        return isPinch
            ? PinchSheet[(int)aiActionType][comPattern]
            : NormalSheet[(int)aiActionType][comPattern];
    }

    //     //読み込み
    //     void LoadDt(const char *CSVDir, const char *CSVFilename_ys, const char *CSVFilename_rs);
    //     //取得
    //     s32 GetDt(enDtComAct dtNo, s32 ptn, BOOL ressei_f);//参照CDtの取得
}