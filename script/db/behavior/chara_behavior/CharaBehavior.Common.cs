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
        if (Composite.CanControl)
        {
            ballDist = Coordinate.DistanceXZ(Ball.Main.Coordinate);
            ballLandDist = ballDist;
            ballLandLineDist = ballDist;

            // 着地予定のみ
            if (Ball.Main.MotionType is BallMotionType.Bound or BallMotionType.Pass)
            {
                ballLandDist = Coordinate.DistanceXZ(Ball.Main.LandX, Ball.Main.LandZ);

                // ライン超える場合のみ
                ballLandLineDist = Ball.Main.LandLine
                    ? Coordinate.DistanceXZ(Ball.Main.LandXLine, Ball.Main.LandZLine)
                    : ballLandDist;
            }
        }

        DistanceSet.Set(ballDist, ballLandDist, ballLandLineDist);
    }
}
