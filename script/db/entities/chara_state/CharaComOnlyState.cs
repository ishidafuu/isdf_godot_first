namespace db;

/// <summary>
/// Com操作に関するステータス
/// </summary>
public class CharaComOnlyStateGetter : ICharaComOnlyStateGetter, ICharaComOnlyStateSetter
{
    // 敵キャッチ確率
    public int ComCatchProbability { get; set; }

    // 敵ナイスキャッチ確率
    public int ComNiceCatchProbability { get; set; }

    // キャッチカウンター状態
    public bool IsCatchCounter { get; set; }

    // 内野からのトスパスを受けた状態
    public bool IsComTossPassGet { get; set; }

    public void Initialize()
    {
        ComCatchProbability = 0;
        ComNiceCatchProbability = 0;
        IsCatchCounter = false;
        IsComTossPassGet = false;
    }
}

public interface ICharaComOnlyStateGetter
{
    int ComCatchProbability { get; }
    int ComNiceCatchProbability { get; }
    bool IsCatchCounter { get; }
    bool IsComTossPassGet { get; }
}

public interface ICharaComOnlyStateSetter
{
    int ComCatchProbability { set; }
    int ComNiceCatchProbability { set; }
    bool IsCatchCounter { set; }
    bool IsComTossPassGet { set; }
}