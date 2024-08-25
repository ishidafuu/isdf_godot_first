namespace db;

/// <summary>
/// 能力値に関するステータス
/// </summary>
public class CharaLevelState
{
    // レベル（チームレア＋補正）
    public int Level { get; set; }

    // ぱわー
    public long Power { get; set; }

    // きあい
    public long Guts { get; set; }

    // きれ
    public long Kire { get; set; }

    // てく
    public long Tech { get; set; }

    public void Initialize()
    {
        Level = 0;
        Power = 0;
        Guts = 0;
        Kire = 0;
        Tech = 0;
    }
}