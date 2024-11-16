namespace db;

public class BallShotState : IBallShotStateGetter, IBallShotStateSetter
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

public interface IBallShotStateGetter
{
    ShotType ShotType { get; }
    int ShotSpeed { get; }
    int ShotRank { get; }
    bool IsHShot { get; }
    int HShotTiming { get; }
    bool IsNoTargetHShot { get; }
    int HShotAnime { get; }
    int ShotPower { get; }
    int ShotTech { get; }
    bool IsNice { get; }
    bool IsJumpShot { get; }
    bool IsDashShot { get; }
    bool IsNoTarget { get; }
    int DistanceX { get; }
    int DistanceZ { get; }
    int DistanceY { get; }
    int DistanceXSign { get; }
    int DistanceZSign { get; }
    int DistanceYSign { get; }
    int resttime { get; }
    int orishno { get; }
    int crvtag { get; }
}

public interface IBallShotStateSetter
{
    ShotType ShotType { set; }
    int ShotSpeed { set; }
    int ShotRank { set; }
    bool IsHShot { set; }
    int HShotTiming { set; }
    bool IsNoTargetHShot { set; }
    int HShotAnime { set; }
    int ShotPower { set; }
    int ShotTech { set; }
    bool IsNice { set; }
    bool IsJumpShot { set; }
    bool IsDashShot { set; }
    bool IsNoTarget { set; }
    int DistanceX { set; }
    int DistanceZ { set; }
    int DistanceY { set; }
    int DistanceXSign { set; }
    int DistanceZSign { set; }
    int DistanceYSign { set; }
    int resttime { set; }
    int orishno { set; }
    int crvtag { set; }
    void Initialize();
}
