namespace db;

/// <summary>
/// 向き・移動・ダッシュ・汎用アクションに関するステータス
/// </summary>
public class CharaMoveState
{
    // ダッシュ加速カウンタ
    public int DashAccelCount { get; set; }

    // 泥足場ステップ数
    public Counter MadStepCount { get; set; }

    // ダッシュ加速IOSフラグ
    public bool IsDashAccelIOS { get; set; }

    // 最後に向いた向き（２，３番外野パス用）
    public DirectionXType LastKeyDirectionX { get; set; }

    // 動きなしフラグ
    public bool IsNoMove { get; set; }

    // オート向き変えが入る前の向き
    public DirectionXType LastDirectionXType { get; set; }

    // オート向き変えが入る前の奥行き向き
    public DirectionZType LastDirectionZType { get; set; }

    // フリーアクションフラグ
    public bool IsFreeAction { get; set; }

    public void Initialize()
    {
        DashAccelCount = 0;
        MadStepCount.Clear();
        IsDashAccelIOS = false;
        LastKeyDirectionX = default;
        IsNoMove = false;
        LastDirectionXType = default;
        LastDirectionZType = default;
        IsFreeAction = false;
    }

    public void IncrementMadStepCount()
    {
        MadStepCount.Add();
    }
}