namespace db;

/// <summary>
/// キャッチングに関するステータス
/// </summary>
public class CharaCatchState
{
    // ボールをキャッチできるカウンタ
    public Counter CatchCount { get; set; }

    // 次のキャッチまでのラグ
    public Counter CatchWaitCount { get; set; }

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