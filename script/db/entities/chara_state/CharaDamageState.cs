namespace db;

/// <summary>
/// ダメージに関するステータス
/// </summary>
public class CharaDamageStateGetter : ICharaDamageStateGetter, ICharaDamageStateSetter
{
    // キャッチダメージ顔
    public DownCounter DamageAfterCount { get; set; }
    public int DamageAfterCountValue => DamageAfterCount.Value;

    // ダウンカウンタ
    public UpCounter DownCount { get; set; }
    public int DownCountValue => DownCount.Value;

    // 回転カウンタ
    public DownCounter RollCount { get; set; }
    public int RollCountValue => RollCount.Value;

    // 屈みカウンタ
    public DownCounter KagamiCount { get; set; }
    public int KagamiCountValue => KagamiCount.Value;

    // よりコート中心に戻れるカウンタ
    public int KagamiRevCount { get; set; }

    // 屈み戻り予定位置
    public int KagamiRevKgNo { get; set; }

    // ヒットストップ
    public bool IsHitStop { get; set; }

    // ファンブルカウンタ
    public DownCounter FumbleCount { get; set; }
    public int FumbleCountValue => FumbleCount.Value;

    public void Initialize()
    {
        DamageAfterCount.Clear();
        DownCount.Clear();
        RollCount.Clear();
        KagamiCount.Clear();
        KagamiRevCount = 0;
        KagamiRevKgNo = 0;
        IsHitStop = false;
        FumbleCount.Clear();
    }

    public void Progress()
    {
        FumbleCount.Sub();
        DamageAfterCount.Sub();
    }
}

public interface ICharaDamageStateGetter
{
    int DamageAfterCountValue { get; }
    int DownCountValue { get; }
    int RollCountValue { get; }
    int KagamiCountValue { get; }
    int KagamiRevCount { get; }
    int KagamiRevKgNo { get; }
    bool IsHitStop { get; }
    int FumbleCountValue { get; }
}

public interface ICharaDamageStateSetter
{
    DownCounter DamageAfterCount { get; }
    UpCounter DownCount { get; }
    DownCounter RollCount { get; }
    DownCounter KagamiCount { get; }
    int KagamiRevCount { set; }
    int KagamiRevKgNo { set; }
    bool IsHitStop { set; }
    DownCounter FumbleCount { get; }

    void Progress();
}