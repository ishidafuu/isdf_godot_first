using isdf;

namespace db;

/// <summary>
/// 入力に関するステータス
/// </summary>
public class CharaInputState : ICharaInputState
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

public interface ICharaInputState
{
    bool IsManualControl { get; }
    DirectionXType LastXKey { get; }
}