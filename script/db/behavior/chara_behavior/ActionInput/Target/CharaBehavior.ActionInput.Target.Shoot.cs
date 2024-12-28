using System;
using isdf;

namespace db;

public partial class CharaBehavior
{
    /// <summary>
    /// シュートターゲットを取得します
    /// 指定された角度と方向無視フラグに基づいてターゲットを決定します
    /// </summary>
    /// <param name="angle12">12時方向の角度</param>
    /// <param name="isIgnoreDirection">方向を無視するかどうか</param>
    /// <returns>シュートターゲットのオーダーインデックス</returns>
    private OrderIndexType GetShootTarget(int angle12, bool isIgnoreDirection)
    {
        var orderIndex = OrderIndexType.Disabled;
        var maxDist = int.MaxValue;

        for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
        {
            if (IsNGPassTag(order))
            {
                continue;
            }

            var targetX = EnemySideOrders[order].Coordinate.X - Coordinate.X;
            var targetZ = Defines.PercentageOf(EnemySideOrders[order].Coordinate.Z - Coordinate.Z, Defines.ZPER);

            if (isIgnoreDirection == false && IsCheckNoAgl(targetX, targetZ))
            {
                continue;
            }

            var dist = Defines.Hypot(targetX, targetZ);

            if (dist >= maxDist)
            {
                continue;
            }

            maxDist = dist;
            orderIndex = (OrderIndexType)order;
        }

        return orderIndex;
    }

    /// <summary>
    /// シュートターゲットの存在をチェックします
    /// 有効なシュートターゲットが存在するかを確認します
    /// </summary>
    /// <returns>シュートターゲットが存在しない場合はtrue</returns>
    private bool IsShTag()
    {
        return Ball.Main.ShotTargetSide != MySideIndex
               || Ball.Main.ShotTargetOrder == OrderIndexType.Disabled;
    }

    /// <summary>
    /// シュートターゲットの設定を行います
    /// 現在の角度に基づいてシュートターゲットを設定します
    /// </summary>
    /// <param name="isIgnoreDirection">向きを無視するかどうか</param>
    private void SetShTagFromMyShootAngle12(bool isIgnoreDirection)
    {
        Ball.CallChangeShootTarget(EnemySideIndex, GetShootTarget(Shoot.Angle12, isIgnoreDirection));
    }
}