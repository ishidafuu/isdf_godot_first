namespace db;

/// <summary>
/// 向き・移動・ダッシュ・汎用アクションに関するステータス
/// </summary>
public class CharaMoveStateGetter : ICharaMoveStateGetter, ICharaMoveStateSetter
{
    // ジャンプしゃがみカウンタ
    public UpCounter JumpCrouchCount { get; set; }

    // ダッシュ加速カウンタ
    public UpCounter DashAccelCount { get; set; }

    // 泥足場ステップ数
    public UpCounter MadStepCount { get; set; }

    // ダッシュ加速IOSフラグ
    public bool IsDashAccelIOS { get; set; }

    // 最後に向いた向き（２，３番外野パス用）
    public DirectionXType LastKeyDirectionX { get; set; }

    // 動きなしフラグ
    public bool IsNoMove { get; set; }

    // オート向き変えが入る前の向き
    public DirectionXType LastDirectionX { get; set; }

    // オート向き変えが入る前の奥行き向き
    public DirectionZType LastDirectionZ { get; set; }

    public void Initialize()
    {
        JumpCrouchCount.Clear();
        DashAccelCount.Clear();
        MadStepCount.Clear();
        IsDashAccelIOS = false;
        LastKeyDirectionX = default;
        IsNoMove = false;
        LastDirectionX = default;
        LastDirectionZ = default;
    }

    public void IncrementMadStepCount()
    {
        MadStepCount.Add();
    }
}

public interface ICharaMoveStateGetter
{
    UpCounter JumpCrouchCount { get; }
    UpCounter DashAccelCount { get; }
    UpCounter MadStepCount { get; }
    bool IsDashAccelIOS { get; }
    DirectionXType LastKeyDirectionX { get; }
    bool IsNoMove { get; }
    DirectionXType LastDirectionX { get; }
    DirectionZType LastDirectionZ { get; }
}

public interface ICharaMoveStateSetter
{
    UpCounter JumpCrouchCount { set; }
    UpCounter DashAccelCount { set; }
    UpCounter MadStepCount { set; }
    bool IsDashAccelIOS { set; }
    DirectionXType LastKeyDirectionX { set; }
    bool IsNoMove { set; }
    DirectionXType LastDirectionX { set; }
    DirectionZType LastDirectionZ { set; }

    void Initialize();
    void IncrementMadStepCount();
}