﻿namespace db;

public partial class CharaBehavior
{

    /// <summary>
    /// サイド操作権を渡せるキャラか
    /// </summary>
    private bool CanControl()
    {
        return MyState.Live.IsDead == false
               && MyState.Motion.HasFlag(CharaMotionFlag.Dam) == false
               && MyState.Pad.IsManualControl == false;
    }

    /// <summary>
    /// ボールとの距離を計算
    /// </summary>
    private void CalculateBallDistance()
    {
        var ballDist = 0;
        var ballLandDist = 0;
        var ballLandLineDist = 0;

        //操作可能キャラのみ
        if (CanControl())
        {
            ballDist = MyState.Coordinate.DistanceXZ(BallState.Coordinate);
            ballLandDist = ballDist;
            ballLandLineDist = ballDist;
            
            // 着地予定のみ
            if (BallState.MotionType is BallMotionType.Bound or BallMotionType.Pass)
            {
                ballLandDist = MyState.Coordinate.DistanceXZ(BallState.LandX, BallState.LandZ);
                
                // ライン超える場合のみ
                ballLandLineDist = BallState.LandLine
                    ? MyState.Coordinate.DistanceXZ(BallState.LandXLine, BallState.LandZLine)
                    : ballLandDist;
            }
        }

        MyState.Distance.BallDist = ballDist;
        MyState.Distance.BallLandDist = ballLandDist;
        MyState.Distance.BallLandLineDist = ballLandLineDist;
    }
}