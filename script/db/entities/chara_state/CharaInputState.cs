using isdf;

namespace db;

/// <summary>
/// 入力に関するステータス
/// </summary>
public class CharaInputStateGetter : ICharaInputStateGetter, ICharaInputStateSetter
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

public interface ICharaInputStateGetter
{
    bool IsManualControl { get; }
    DirectionXType LastXKey { get; }
}

public interface ICharaInputStateSetter
{
    bool IsManualControl { set; }
    DirectionXType LastXKey { set; }

    void Initialize();
}