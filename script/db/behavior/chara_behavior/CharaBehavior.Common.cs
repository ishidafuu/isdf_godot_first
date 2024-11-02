using isdf;

namespace db;

public partial class CharaBehavior
{

    /// <summary>
    /// ボールとの距離を計算
    /// </summary>
    private void UpdateBallDistance()
    {
        var ballDist = 0;
        var ballLandDist = 0;
        var ballLandLineDist = 0;

        // 操作可能キャラのみ
        if (CanControl)
        {
            ballDist = Coordinate.DistanceXZ(BallState.Coordinate);
            ballLandDist = ballDist;
            ballLandLineDist = ballDist;

            // 着地予定のみ
            if (BallState.MotionType is BallMotionType.Bound or BallMotionType.Pass)
            {
                ballLandDist = Coordinate.DistanceXZ(BallState.LandX, BallState.LandZ);

                // ライン超える場合のみ
                ballLandLineDist = BallState.LandLine
                    ? Coordinate.DistanceXZ(BallState.LandXLine, BallState.LandZLine)
                    : ballLandDist;
            }
        }

        DistanceSet.Set(ballDist, ballLandDist, ballLandLineDist);
    }
}