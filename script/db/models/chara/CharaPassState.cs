namespace db;

/// <summary>
/// パスに関するステータス
/// </summary>
public class CharaPassState
{
    // 立ちパス待ちカウンタ
    public int PassStandWaitCount { get; set; }

    // トスパスフラグ
    public bool IsTossPass { get; set; }

    // ミラーパスカウンタ
    public int MirrorPassCount { get; set; }

    // ミラーパス的シュートカウンタ
    public int MirrorShotCount { get; set; }

    // ミラーパス的シュート受付時間
    public int MirrorShotLimitCount { get; set; }

    public void Initialize()
    {
        PassStandWaitCount = 0;
        IsTossPass = false;
        MirrorPassCount = 0;
        MirrorShotCount = 0;
        MirrorShotLimitCount = 0;
    }
}