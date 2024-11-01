namespace db;

/// <summary>
/// 距離に関するステータス
/// </summary>
public class CharaDistanceState : ICharaDistanceState
{
    // ボールとの距離
    public int BallDist { get; set; }

    // ボール着地地点との距離
    public int BallLandDist { get; set; }

    // ボール着地地点のラインとの距離
    public int BallLandLineDist { get; set; }

    public void Initialize()
    {
        BallDist = 0;
        BallLandDist = 0;
        BallLandLineDist = 0;
    }

    public void Set(int ballDist, int ballLandDist, int ballLandLineDist)
    {
        BallDist = ballDist;
        BallLandDist = ballLandDist;
        BallLandLineDist = ballLandLineDist;
    }
}

public interface ICharaDistanceState
{
    int BallDist { get; }
    int BallLandDist { get; }
    int BallLandLineDist { get; }
}