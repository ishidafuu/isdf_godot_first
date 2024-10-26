using isdf;

namespace db;

/// <summary>
/// 入力に関するステータス
/// </summary>
public class CharaInputState
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