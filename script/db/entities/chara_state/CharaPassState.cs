namespace db;

/// <summary>
/// パスに関するステータス
/// </summary>
public class CharaPassState : ICharaPassStateGetter, ICharaPassStateSetter
{
    // 立ちパス待ちカウンタ
    public DownCounter PassStandWaitCount { get; set; }

    // ミラーパスカウンタ
    public UpCounter MirrorPassCount { get; set; }

    // ミラーパス的シュートカウンタ
    public UpCounter MirrorShotCount { get; set; }

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

public interface ICharaPassStateGetter
{
    DownCounter PassStandWaitCount { get; }
    UpCounter MirrorPassCount { get; }
    UpCounter MirrorShotCount { get; }
    DownCounter MirrorShotLimitCount { get; }
    bool IsTossPass { get; }
}

public interface ICharaPassStateSetter
{
    DownCounter PassStandWaitCount { set; }
    UpCounter MirrorPassCount { set; }
    UpCounter MirrorShotCount { set; }
    DownCounter MirrorShotLimitCount { set; }
    bool IsTossPass { set; }

    void Initialize();
    void DecrementMirrorShotLimitCount();
    void ResetPassStandWaitCount(int value);
}