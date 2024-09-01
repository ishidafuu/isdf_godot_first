namespace db;

/// <summary>
/// ダメージに関するステータス
/// </summary>
public class CharaDamageState
{
    // キャッチダメージ顔
    public Counter DamageAfterCount { get; set; }

    // ダウンカウンタ
    public Counter DownCount { get; set; }

    // 回転カウンタ
    public int RollCount { get; set; }

    // 屈みカウンタ
    public int KagamiCount { get; set; }

    // よりコート中心に戻れるカウンタ
    public int KagamiRevCount { get; set; }

    // 屈み戻り予定位置
    public int KagamiRevKgNo { get; set; }

    // ヒットストップ
    public bool IsHitStop { get; set; }

    // ファンブルカウンタ
    public Counter FumbleCount { get; set; }

    public void Initialize()
    {
        DamageAfterCount.Clear();
        DownCount.Clear();
        RollCount = 0;
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