namespace db;

public class BallShotState
{
    public ShotType ShotType { get; set; }
    public int ShotSpeed { get; set; }
    public int ShotRank { get; set; }
    public bool IsHShot { get; set; }
    public int HShotTiming { get; set; }
    public bool IsNoTargetHShot { get; set; }

    public int HShotAnime { get; set; }

    // シュートパワー（投げ手ぱわー＋必殺補正）
    public int ShotPower { get; set; }

    // シュートテク（投げ手すぴん＋必殺補正）
    public int ShotTech { get; set; }
    public bool IsNice { get; set; }
    public bool IsJumpShot { get; set; }

    public bool IsDashShot { get; set; }

    // タゲ無し必殺（効果音のみ）
    public bool IsNoTarget { get; set; }

    public int DistanceX { get; set; }
    public int DistanceZ { get; set; }
    public int DistanceY { get; set; }
    public int DistanceXSign { get; set; }
    public int DistanceZSign { get; set; }
    public int DistanceYSign { get; set; }
    public int resttime { get; set; }
    public int orishno { get; set; }
    public int crvtag { get; set; }

    public void Initialize()
    {
        ShotType = default;
        ShotSpeed = 0;
        ShotRank = 0;
        IsHShot = false;
        HShotTiming = 0;
        IsNoTargetHShot = false;
        HShotAnime = 0;
        ShotPower = 0;
        ShotTech = 0;
        IsNice = false;
        IsJumpShot = false;
        IsDashShot = false;
        IsNoTarget = false;
        DistanceX = 0;
        DistanceZ = 0;
        DistanceY = 0;
        DistanceXSign = 0;
        DistanceZSign = 0;
        DistanceYSign = 0;
        resttime = 0;
        orishno = 0;
    }
}