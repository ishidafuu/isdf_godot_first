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

    public DirectionXType LastXKey { get; set; }

    public void Initialize()
    {
        IsManualControl = false;
        LastXKey = DirectionXType.Neutral;
    }

}