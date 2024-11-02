namespace db;

/// <summary>
/// キャッチングに関するステータス
/// </summary>
public class CharaCatchStateGetter : ICharaCatchStateGetter, ICharaCatchStateSetter
{
    // ボールをキャッチできるカウンタ
    public UpCounter CatchCount { get; set; }
    public int CatchCountValue => CatchCount.Value;

    // 次のキャッチまでのラグ
    public DownCounter CatchWaitCount { get; set; }
    public int CatchWaitCountValue => CatchWaitCount.Value;

    // ブロックカウンタ
    public int BlockCount { get; set; }

    // キャッチ滑り時摩擦
    public int FricX { get; set; }

    public int FricZ { get; set; }

    // 敵シュートキャッチ
    public bool IsEnemyShotCatch { get; set; }

    // キャッチ後長押し維持でジャンプしてしまうのを防ぐためのフラグ
    public bool IsEnabledJump { get; set; }

    // キャッチ後リリースでシュートしてしまうのを防ぐためのフラグ
    public bool IsEnabledShot { get; set; }

    public void Initialize()
    {
        CatchCount.Clear();
        CatchWaitCount.Clear();
        BlockCount = 0;
        FricX = 0;
        FricZ = 0;
        IsEnemyShotCatch = false;
        IsEnabledJump = false;
        IsEnabledShot = false;
    }

    public void Progress()
    {
        CatchWaitCount.Sub();
    }

    public void ResetCacheWait()
    {
        CatchWaitCount.Clear();
        IsEnabledJump = false;
        IsEnabledShot = false;
    }

    public void SetCacheWait(int value)
    {
        CatchWaitCount.Set(value);
    }
}

public interface ICharaCatchStateGetter
{
    int CatchCountValue { get; }
    int CatchWaitCountValue { get; }
    int BlockCount { get; }
    int FricX { get; }
    int FricZ { get; }
    bool IsEnemyShotCatch { get; }
    bool IsEnabledJump { get; }
    bool IsEnabledShot { get; }
}

public interface ICharaCatchStateSetter
{
    UpCounter CatchCount { get; }
    DownCounter CatchWaitCount { get; }
    int BlockCount { set; }
    int FricX { set; }
    int FricZ { set; }
    bool IsEnemyShotCatch { set; }
    bool IsEnabledJump { set; }
    bool IsEnabledShot { set; }

    void Progress();
    void ResetCacheWait();
    void SetCacheWait(int value);
}