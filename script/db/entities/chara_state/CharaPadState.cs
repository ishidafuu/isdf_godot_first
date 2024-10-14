using isdf;

namespace db;

/// <summary>
/// パッド入力に関するステータス
/// </summary>
public class CharaPadState
{
    /// <summary>
    /// 操作キャラ
    /// </summary>
    public bool IsManualControl { get; set; }

    // public int PadNo;
    //
    // // public TMgPad pCharPad_; // パッド
    // public IPad Pad;

    public void Initialize()
    {
        IsManualControl = false;
        // PadNo = 0;
    }

    // public bool IsValid => Pad != null;
}