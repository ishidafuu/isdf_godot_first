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
}
