namespace db;

/// <summary>
/// ドッジボールのコートに関するステータス
/// </summary>
public class CharaCourtState
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

    public void Initialize()
    {
        IsOutFieldOverLine = false;
        IsLandEnemyCourt = false;

        ECDdg_f = false;
        ECDjp_f = false;
        EnemyCortDodgeCount.Clear();
    }
}