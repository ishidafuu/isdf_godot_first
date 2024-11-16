namespace db;

/// <summary>
/// 生存・体力・判定に関するステータス
/// </summary>
public class CharaLiveState : ICharaLiveStateGetter, ICharaLiveStateSetter
{
    // 生存
    public bool IsAlive { get; set; }

    // 死亡 天使の間 GenSaiObjのLive_f=Falseは完全消滅
    public bool IsDead { get; set; }

    // 外野天使、HP0でダウンしたタイミングでTRUE
    public bool IsAngel { get; set; }

    // 体力
    public int Hp { get; set; }

    // Max体力
    public int MaxHp { get; set; }

    // 体力全快
    public bool IsMaxHp { get; set; }

    // 天使カウンタ
    public int AngelCount { get; set; }

    // 復活回数
    public int ReviveCount { get; set; }

    // 汎用カウンタ
    public int EtcCount { get; set; }

    // 何週目か
    public int LoopCount { get; set; }

    // バイオリズム
    public UpCounter BiorhythmCount { get; set; }

    public void Initialize()
    {
        IsAlive = false;
        IsDead = false;
        IsAngel = false;
        Hp = 0;
        IsMaxHp = false;
        AngelCount = 0;
        ReviveCount = 0;
        EtcCount = 0;
        LoopCount = 0;
        BiorhythmCount.Clear();
    }

    public void IncrementBiorhythmCount()
    {
        BiorhythmCount.Add();
    }
}

public interface ICharaLiveStateGetter
{
    bool IsAlive { get; }
    bool IsDead { get; }
    bool IsAngel { get; }
    int Hp { get; }
    int MaxHp { get; }
    bool IsMaxHp { get; }
    int AngelCount { get; }
    int ReviveCount { get; }
    int EtcCount { get; }
    int LoopCount { get; }
    UpCounter BiorhythmCount { get; }
}

public interface ICharaLiveStateSetter
{
    bool IsAlive { set; }
    bool IsDead { set; }
    bool IsAngel { set; }
    int Hp { set; }
    int MaxHp { set; }
    bool IsMaxHp { set; }
    int AngelCount { set; }
    int ReviveCount { set; }
    int EtcCount { set; }
    int LoopCount { set; }
    UpCounter BiorhythmCount { set; }

    void Initialize();
    void IncrementBiorhythmCount();
}