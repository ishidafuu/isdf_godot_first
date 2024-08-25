namespace db;

/// <summary>
/// ダメージに関するステータス
/// </summary>
public class CharaDamageState
{
    // キャッチダメージ顔
    public int DamageAfterCount { get; set; }

    // ダウンカウンタ
    public int DownCount { get; set; }

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

    public void Initialize()
    {
        DamageAfterCount = 0;
        DownCount = 0;
        RollCount = 0;
        KagamiCount = 0;
        KagamiRevCount = 0;
        KagamiRevKgNo = 0;
        IsHitStop = false;
    }
}