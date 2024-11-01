namespace db;

/// <summary>
/// 能力値に関するステータス
/// </summary>
public class CharaLevelState : ICharaLevelState
{
    // レベル（チームレア＋補正）
    public int Level { get; set; }

    // ぱわー
    public int Power { get; set; }

    // きあい
    public int Guts { get; set; }

    // きれ
    public int Kire { get; set; }

    // てく
    public int Tech { get; set; }

    // すぴーど（ランク）
    public int Speed { get; set; }

    public void Initialize()
    {
        Level = 0;
        Power = 0;
        Guts = 0;
        Kire = 0;
        Tech = 0;
        Speed = 0;
    }
}

public interface ICharaLevelState
{
    int Level { get; }
    int Power { get; }
    int Guts { get; }
    int Kire { get; }
    int Tech { get; }
    int Speed { get; }
}