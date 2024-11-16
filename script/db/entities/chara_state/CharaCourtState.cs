namespace db;

/// <summary>
/// ドッジボールのコートに関するステータス
/// </summary>
public class CharaCourtState : ICharaCourtStateGetter, ICharaCourtStateSetter
{
    //外野でもオーバーラインになるフラグ
    public bool IsOutFieldOverLine { get; set; }

    //敵コートに着地予定
    public bool IsLandEnemyCourt { get; set; }

    //敵コート避け
    public bool ECDdg_f { get; set; }

    //敵コートジャンプ
    public bool ECDjp_f { get; set; }

    // 敵コート避け時間
    public UpCounter EnemyCortDodgeCount { get; set; }
    public int EnemyCortDodgeCountValue => EnemyCortDodgeCount.Value;

    public void Initialize()
    {
        IsOutFieldOverLine = false;
        IsLandEnemyCourt = false;

        ECDdg_f = false;
        ECDjp_f = false;
        EnemyCortDodgeCount.Clear();
    }
}

public interface ICharaCourtStateGetter
{
    bool IsOutFieldOverLine { get; }
    bool IsLandEnemyCourt { get; }
    bool ECDdg_f { get; }
    bool ECDjp_f { get; }
    int EnemyCortDodgeCountValue { get; }
}

public interface ICharaCourtStateSetter
{
    bool IsOutFieldOverLine { set; }
    bool IsLandEnemyCourt { set; }
    bool ECDdg_f { set; }
    bool ECDjp_f { set; }
    UpCounter EnemyCortDodgeCount { get; }
}