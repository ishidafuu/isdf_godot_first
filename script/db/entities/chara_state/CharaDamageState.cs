namespace db;

/// <summary>
/// ダメージに関するステータス
/// </summary>
public class CharaDamageState
{
    // キャッチダメージ顔
    public DownCounter DamageAfterCount { get; set; }

    // ダウンカウンタ
    public UpCounter DownCount { get; set; }

    // 回転カウンタ
    public DownCounter RollCount { get; set; }

    // 屈みカウンタ
    public int KagamiCount { get; set; }

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
        KagamiCount = 0;
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