namespace db;

/// <summary>
/// パスに関するステータス
/// </summary>
public class CharaPassState
{
    // 立ちパス待ちカウンタ
    public DownCounter PassStandWaitCount { get; set; }

    // ミラーパスカウンタ
    public DownCounter MirrorPassCount { get; set; }

    // ミラーパス的シュートカウンタ
    public DownCounter MirrorShotCount { get; set; }

    // ミラーパス的シュート受付時間
    public DownCounter MirrorShotLimitCount { get; set; }

    // トスパスフラグ
    public bool IsTossPass { get; set; }

    public void Initialize()
    {
        PassStandWaitCount.Clear();
        MirrorPassCount.Clear();
        MirrorShotCount.Clear();
        MirrorShotLimitCount.Clear();
        IsTossPass = false;
    }

    public void DecrementMirrorShotLimitCount()
    {
        if (MirrorShotLimitCount.Sub())
        {
            MirrorShotCount.Clear();
        }
    }

    public void ResetPassStandWaitCount(int value)
    {
        PassStandWaitCount.Set(value);
    }
}