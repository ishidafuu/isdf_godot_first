namespace db;

/// <summary>
/// シュート・ターゲットに関するステータス
/// </summary>
public class CharaShootState
{
    // シュート待ちカウンタ
    public DownCounter ShootWaitCount { get; set; }

    // シュート終了待ちカウンタ
    public DownCounter ShootEndWaitCount { get; set; }

    // 必殺タイミング
    public UpCounter Step { get; set; }

    // 必殺アシスト状態
    public ShootTiming ShootTiming { get; set; }

    // ターゲッティング角度（１２分割、一番左側の番号、そこから４つ）
    public int Angle12 { get; set; }

    // がたつき防止
    public int ShCv { get; set; }

    // ４番外野後ろ狙う
    public bool IsBackSh { get; set; }

    // 振り返りシュート
    public bool IsUTurn { get; set; }

    // ファンブルした後オートシュートしないためのカウンタ
    public int FumbleCount { get; set; }

    public void Initialize()
    {
        ShootWaitCount.Clear();
        ShootEndWaitCount.Clear();
        Step.Clear();
        ShootTiming = default;
        Angle12 = -1;
        ShCv = 0;
        IsBackSh = false;
        IsUTurn = false;
        FumbleCount = 0;
    }

    public void SetWaitCount(int startCount, int endCount)
    {
        ShootWaitCount.Set(startCount);
        ShootEndWaitCount.Set(endCount);
    }
}