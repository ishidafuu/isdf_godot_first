using System;

namespace db;

public class BallCompositeState : BaseBehavior
{
    private RefereeState RefereeState => RefereeStateManager.Instance.RefereeState;
    private BallMainState BallMainState => BallStateManager.Instance.Main;
    private TeamState[] Teams => TeamStateManager.Instance.GetAllTeams();
    public IBallMainStateGetter MainState => BallMainState;


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
                return Coordinate.VelocityY < 0;
            case JumpBallType.Side0:
                return side == 0;
            case JumpBallType.Side1:
                return side == 1;
            default:
                throw new ArgumentOutOfRangeException();
        }
    }
}
