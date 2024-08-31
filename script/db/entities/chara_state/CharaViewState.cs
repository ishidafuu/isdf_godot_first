namespace db;

/// <summary>
/// アニメーション・見た目・セリフに関するステータス
/// </summary>
public class CharaViewState
{
    // 息継ぎカウンタ
    public int BreathCount { get; set; }

    // 長持ちセリフ用
    public int LongKeepCount { get; set; }

    // パス待ちセリフ用
    public int PassWaitCount { get; set; }

    // ターゲット
    public Counter TargetCount { get; set; }

    public void Initialize()
    {
        BreathCount = 0;
        LongKeepCount = 0;
        PassWaitCount = 0;
        TargetCount.Clear();
    }

    public void IncrementTargetCount()
    {
        TargetCount.Add();
    }
    
    public void ResetTargetCount()
    {
        TargetCount.Clear();
    }
}