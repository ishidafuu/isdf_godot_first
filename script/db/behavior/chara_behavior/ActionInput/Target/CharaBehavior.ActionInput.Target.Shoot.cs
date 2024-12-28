using System;
using isdf;

namespace db;

public partial class CharaBehavior
{
    /// <summary>
    /// シュートターゲットを取得します
    /// </summary>
    /// <param name="angle">角度</param>
    /// <param name="isIgnoreDirection">向きを反映しない（強制的に内野タゲ）</param>
    /// <returns>選択されたシュートターゲットのオーダーインデックス</returns>
    private OrderIndexType GetShootTarget(int angle, bool isIgnoreDirection)
    {
        TmpStateManager.Instance.TmpState.Clear();
        var targetX = TmpStateManager.Instance.TmpState.targetX;
        var targetY = TmpStateManager.Instance.TmpState.targetY;
        var targetDist = TmpStateManager.Instance.TmpState.targetDist;
        var isSelectTarget = TmpStateManager.Instance.TmpState.isSelectTarget;

        // 敵内野全員との角度を取る
        for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
        {
            var enemyChara = CharaBehaviorManager.Instance.GetOrderChara(EnemySideIndex, order);

            if (enemyChara.Composite.IsEnableShootTarget == false)
            {
                continue;
            }

            isSelectTarget[order] = enNaiyaTag.TGOK;
            targetX[order] = enemyChara.Coordinate.X - Coordinate.X;
            var targetZ = enemyChara.Coordinate.Z - Coordinate.Z;
            targetY[order] = enemyChara.Coordinate.Y - Coordinate.Y;
            targetDist[order] = Defines.Hypot(targetX[order], targetZ); // 距離

            // 角度を無視しない場合は角度チェックを行う
            if (isIgnoreDirection == false)
            {
                const int angle12 = 12;
                var targetAngle = GetTagAgl2(targetX[order], targetZ); // 新12時法

                // メインアングルチェック
                if (targetAngle != (angle + 0) % angle12
                    && targetAngle != (angle + 1) % angle12
                    && targetAngle != (angle + 2) % angle12
                    && targetAngle != (angle + 3) % angle12)
                {
                    isSelectTarget[order] = enNaiyaTag.TGNG;
                }
            }
        }

        var targetOrderIndex = OrderIndexType.Disabled;
        var subTargetOrderIndex = OrderIndexType.Disabled;
        var nearTargetDist = 0;
        var subNearTargetDist = 0;

        // 距離による優先順位
        for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
        {
            if (isSelectTarget[order] == enNaiyaTag.TGNG)
            {
                continue;
            }

            // 第二候補
            if (subNearTargetDist == 0
                || targetDist[order] < subNearTargetDist)
            {
                subTargetOrderIndex = (OrderIndexType)order;
                subNearTargetDist = targetDist[order];
            }

            // 近すぎ
            if (targetDist[order] < Defines.SHTAG_NEARDIST)
            {
                continue;
            }

            // Y離れすぎ
            if (Defines.PercentageOf(targetDist[order], Defines.SHTAG_FARDIST_Y) < Math.Abs(targetY[order]))
            {
                continue;
            }

            if (nearTargetDist == 0
                || targetDist[order] < nearTargetDist)
            {
                targetOrderIndex = (OrderIndexType)order;
                nearTargetDist = targetDist[order];
            }
        }

        return targetOrderIndex == OrderIndexType.Disabled
            ? subTargetOrderIndex
            : targetOrderIndex;
    }
}