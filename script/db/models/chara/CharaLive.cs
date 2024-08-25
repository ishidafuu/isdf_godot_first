namespace db;

/// <summary>
/// 生存・体力・判定に関するステータス
/// </summary>
public class CharaLive
{
    // キャラのＩＤ
    public long CharaId { get; set; }

    // 生存
    public bool IsAlive { get; set; }

    // 死亡 天使の間 GenSaiObjのLive_f=Falseは完全消滅
    public bool IsDead { get; set; }

    // 外野天使
    public bool IsAngel { get; set; }

    // 喰らい判定
    public Rect HitBox { get; set; }

    // 体力
    public int Hp { get; set; }

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
    public int BiorhythmCount { get; set; }

    public void Initialize()
    {
        CharaId = 0;
        IsAlive = false;
        IsDead = false;
        IsAngel = false;
        HitBox.Initialize();
        Hp = 0;
        IsMaxHp = false;
        AngelCount = 0;
        ReviveCount = 0;
        EtcCount = 0;
        LoopCount = 0;
        BiorhythmCount = 0;
    }
}