namespace db;

/// <summary>
/// 向き・移動・ダッシュ・汎用アクションに関するステータス
/// </summary>
public class CharaMove
{
    // ダッシュ加速カウンタ
    public int DashAccelCount { get; set; }

    // ダッシュ加速IOSフラグ
    public bool IsDashAccelIOS { get; set; }

    // 最後に向いた向き（２，３番外野パス用）
    public DirectionX LastKeyDirectionX { get; set; }

    // 動きなしフラグ
    public bool IsNoMove { get; set; }

    // オート向き変えが入る前の向き
    public DirectionX LastDirectionX { get; set; }

    // オート向き変えが入る前の奥行き向き
    public DirectionZ LastDirectionZ { get; set; }

    // フリーアクションフラグ
    public bool IsFreeAction { get; set; }

    public void Initialize()
    {
        DashAccelCount = 0;
        IsDashAccelIOS = false;
        LastKeyDirectionX = default;
        IsNoMove = false;
        LastDirectionX = default;
        LastDirectionZ = default;
        IsFreeAction = false;
    }
}