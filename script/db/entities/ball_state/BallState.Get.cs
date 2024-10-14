using System;

namespace db;

public partial class BallState
{
    /// <summary>
    /// ボール保持者かどうか
    /// </summary>
    public bool IsBallHolder(int side, int order)
    {
        return side == HolderSide
               && order == HolderOrder;
    }

    /// <summary>
    /// シュートターゲットかどうか
    /// </summary>
    public bool IsShotTarget(int side, int order)
    {
        return MotionType is BallMotionType.Hold or BallMotionType.Shoot
               && side == ShotTargetSide
               && order == ShotTargetOrder;
    }

    /// <summary>
    /// パスターゲットかどうか
    /// </summary>
    public bool IsPassTarget(int side, int order)
    {
        return MotionType is BallMotionType.Hold
               && side == PassTargetSide
               && order == PassTargetOrder
               && OvLine == false;
    }

    /// <summary>
    /// 獲得可能なジャンプボール
    /// </summary>
    /// <returns></returns>
    public bool CanHoldJumpBall(int side)
    {
        switch (JumpBallType)
        {
            case JumpBallType.Normal:
                return true;
            case JumpBallType.Jpball:
                return  Coordinate.VelocityY < 0;
            case JumpBallType.Side0:
                return side == 0;
            case JumpBallType.Side1:
                return side == 1;
            default:
                throw new ArgumentOutOfRangeException();
        }
    }
}