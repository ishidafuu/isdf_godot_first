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

    public void Initialize()
    {
        IsOutFieldOverLine = false;
        IsLandEnemyCourt = false;
    }
}