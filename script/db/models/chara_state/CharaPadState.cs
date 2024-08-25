namespace db;

// //めんばーぱっど情報
// struct TStChManPad
// {
//     BOOL MANChar_f;//キャラ操作
//     s32 MANCharPadNo;
//     TMgPad* pCharPad_;//パッド
// };

public class CharaPadState
{
    /// <summary>
    /// 操作キャラ
    /// </summary>
    public bool IsManualControl;

    public int PadNo;
    // public TMgPad pCharPad_; // パッド

    public void Initialize()
    {
        IsManualControl = false;
        PadNo = 0;
    }
}