namespace db;

/// <summary>
/// シュート・ターゲットに関するステータス
/// </summary>
public class CharaShootState
{
    // シュート待ちカウンタ
    public int ShootWaitCount { get; set; }

    // シュート終了待ちカウンタ
    public int ShootEndWaitCount { get; set; }

    // 必殺タイミング
    public int Step { get; set; }

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
        ShootWaitCount = 0;
        ShootEndWaitCount = 0;
        Step = 0;
        ShootTiming = default;
        Angle12 = 0;
        ShCv = 0;
        IsBackSh = false;
        IsUTurn = false;
        FumbleCount = 0;
    }
}