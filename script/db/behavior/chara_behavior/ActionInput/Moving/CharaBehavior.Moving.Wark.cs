using System;
using isdf;

namespace db;

public partial class CharaBehavior
{
    //歩き
    private void MvWk()
    {
        CoordinateSet.ZeroVelocity();

        //X
        if (Order.IsInfield
            || Auto.AutoType != AutoType.Free
            || Order.OrderIndex == OrderIndexType.Outfield2
            || Order.OrderIndex == OrderIndexType.Outfield3)
        {
            var walkX = GetSpeedRank(RankSpeedType.WkX);
            var direction = GetMoveMuki(false);
            if (direction != DirectionXType.Neutral)
            {
                CoordinateSet.VelocityX = walkX * (direction == DirectionXType.Right ? 1 : -1);
            }
        }

        //Z
        if (Order.IsInfield
            || Auto.AutoType != AutoType.Free
            || Order.OrderIndex == OrderIndexType.Outfield4)
        {

            var walkZ = GetSpeedRank(RankSpeedType.WkX) * Defines.Percent / GetSettingCourt(SetingCourtType.WkZPer);

            var direction = GetMoveMukiZ(false);
            if (direction != DirectionZType.Neutral)
            {
                CoordinateSet.VelocityZ = walkZ * (direction == DirectionZType.Forward ? 1 : -1);
            }
        }

        //斜め
        if (Coordinate.VelocityX != 0 && Coordinate.VelocityZ != 0)
        {
            CoordinateSet.VelocityX = Coordinate.VelocityX * (Coordinate.VelocityX > 0 ? 1 : -1);
            CoordinateSet.VelocityZ = Coordinate.VelocityZ * (Coordinate.VelocityZ > 0 ? 1 : -1);
        }
    }

    private DirectionXType GetMoveMuki(bool Input_f)
    {
        if (Composite.IsSelfControl == false)
        {
            return Auto.DirectionX;
        }
        else if (Input_f || (Auto.AutoType == AutoType.Free))
        {
            if (Pad.IsWalk2(DirectionCrossType.Left))//歩き入力
            {
                return DirectionXType.Left;
            }
            else if (Pad.IsWalk2(DirectionCrossType.Right))//歩き入力
            {
                return DirectionXType.Right;
            }
            else
            {
                return DirectionXType.Neutral;
            }
        }
        else
        {
            return Auto.DirectionX;
        }
    }

    private DirectionZType GetMoveMukiZ(bool Input_f)
    {
        if (Composite.IsSelfControl == false)
        {
            return Auto.DirectionZ;
        }
        else if (Input_f || (Auto.AutoType == AutoType.Free))
        {
            if (Pad.IsWalk2(DirectionCrossType.Up))//歩き入力
            {
                return DirectionZType.Backward;
            }
            else if (Pad.IsWalk2(DirectionCrossType.Down))//歩き入力
            {
                return DirectionZType.Forward;
            }
            else
            {
                return DirectionZType.Neutral;
            }
        }
        else
        {
            return Auto.DirectionZ;
        }
    }

    private DirectionZType GetMoveMukiZMukiLock(bool Input_f)
    {
        if (Composite.IsSelfControl == false)
        {
            return Auto.DirectionZ;
        }
        else if (Input_f || (Auto.AutoType == AutoType.Free))
        {
            if (Pad.IsWalk2(DirectionCrossType.Down))
            {
                return DirectionZType.Backward;
            }
            else if (Pad.IsWalk2(DirectionCrossType.Up))
            {
                return DirectionZType.Forward;
            }
            else
            {
                return DirectionZType.Neutral;
            }
        }
        else
        {
            return Auto.DirectionZ;
        }
    }
}
