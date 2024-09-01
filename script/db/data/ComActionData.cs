using System;
using db;

namespace db;

public class ComActionData
{
    public static int ActionTypeLength => Enum.GetValues<ComActionType>().Length;
    public int[][] NormalSheet { get; private set; }
    public int[][] PinchSheet { get; private set; }

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

    //     //読み込み
    //     void LoadDt(const char *CSVDir, const char *CSVFilename_ys, const char *CSVFilename_rs);
    //     //取得
    //     s32 GetDt(enDtComAct dtNo, s32 ptn, BOOL ressei_f);//参照CDtの取得
}