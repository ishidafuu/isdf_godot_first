namespace db;

/// <summary>
/// ダメージに関するステータス
/// </summary>
public class CharaDamageState : ICharaDamageState
{
    // キャッチダメージ顔
    public DownCounter DamageAfterCount { get; set; }

    // ダウンカウンタ
    public UpCounter DownCount { get; set; }

    // 回転カウンタ
    public DownCounter RollCount { get; set; }

    // 屈みカウンタ
    public DownCounter KagamiCount { get; set; }

    // よりコート中心に戻れるカウンタ
    public int KagamiRevCount { get; set; }

    // 屈み戻り予定位置
    public int KagamiRevKgNo { get; set; }

    // ヒットストップ
    public bool IsHitStop { get; set; }

    // ファンブルカウンタ
    public DownCounter FumbleCount { get; set; }

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

public interface ICharaDamageState
{
    DownCounter DamageAfterCount { get; }
    UpCounter DownCount { get; }
    DownCounter RollCount { get; }
    DownCounter KagamiCount { get; }
    int KagamiRevCount { get; }
    int KagamiRevKgNo { get; }
    bool IsHitStop { get; }
    DownCounter FumbleCount { get; }
}