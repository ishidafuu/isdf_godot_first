namespace db;

/// <summary>
/// 向き・移動・ダッシュ・汎用アクションに関するステータス
/// </summary>
public class CharaMoveState : ICharaMoveStateGetter, ICharaMoveStateSetter
{
    // 必殺タイミング
    public UpCounter StepCount { get; set; }
    public int StepCountValue => StepCount.Value;
    
    // ジャンプしゃがみカウンタ
    public UpCounter JumpCrouchCount { get; set; }
    public int JumpCrouchCountValue => JumpCrouchCount.Value;

    // ダッシュ加速カウンタ
    public UpCounter DashAccelCount { get; set; }
    public int DashAccelCountValue => DashAccelCount.Value;

    // 泥足場ステップ数
    public UpCounter MadStepCount { get; set; }
    public int MadStepCountValue => MadStepCount.Value;

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
        StepCount.Clear();
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
    int StepCountValue { get; }
    int JumpCrouchCountValue { get; }
    int DashAccelCountValue { get; }
    int MadStepCountValue { get; }
    bool IsDashAccelIOS { get; }
    DirectionXType LastKeyDirectionX { get; }
    bool IsNoMove { get; }
    DirectionXType LastDirectionX { get; }
    DirectionZType LastDirectionZ { get; }
}

public interface ICharaMoveStateSetter
{
    UpCounter StepCount { get; }
    UpCounter JumpCrouchCount { get; }
    UpCounter DashAccelCount { get; }
    UpCounter MadStepCount { get; }
    bool IsDashAccelIOS { set; }
    DirectionXType LastKeyDirectionX { set; }
    bool IsNoMove { set; }
    DirectionXType LastDirectionX { set; }
    DirectionZType LastDirectionZ { set; }

    void Initialize();
    void IncrementMadStepCount();
}
