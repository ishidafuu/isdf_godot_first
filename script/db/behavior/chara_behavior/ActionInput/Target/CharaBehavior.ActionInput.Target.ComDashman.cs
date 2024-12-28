using System;
using isdf;

namespace db;

public partial class CharaBehavior
{
    /// <summary>
    /// COMダッシュマンのパスタイプを表す列挙型
    /// </summary>
    private enum enCOMDMPassType
    {
        /// <summary>一つオーダー上位（上位なら最下位）</summary>
        cpmUpOrder,
        /// <summary>一番近い人</summary>
        cpmNear,
        /// <summary>一番Ｚ軸が遠い人</summary>
        cpmFar,
        /// <summary>一つ後ろの人(居なければ一番前の人)</summary>
        cpmRear
    }

    /// <summary>
    /// COMダッシュマンのパスターゲットを設定します
    /// 現在のダッシュマンの状態に基づいて最適なパスターゲットを決定します
    /// </summary>
    /// <param name="nowOKonly_f">現在パス可能な対象のみを選択するかどうか</param>
    /// <returns>選択されたパスターゲットの順序インデックス</returns>
    private OrderIndexType GetCOMDMPassTag(bool nowOKonly_f)
    {
        TmpStateManager.Instance.TmpState.Clear();
        var targetDist = TmpStateManager.Instance.TmpState.targetDist;
        var isSelectTarget = TmpStateManager.Instance.TmpState.isSelectTarget;
        var targetOrder = TmpStateManager.Instance.TmpState.targetOrder;
        var sortValue = TmpStateManager.Instance.TmpState.sortValue;

        bool topord_f = true; // 最上位オーダーフラグ
        bool bottom_f = true; // 最後尾フラグ
        bool Notag_f = true; // 完全にパスタゲが居ない

        // パスのタイプ
        var patype = (enCOMDMPassType)MyTeam.AiPattern.GetPlanPattern(AiPlanType.comDMPaTag);

        // 触ってない人だけ
        bool NTOnly_f = MyTeam.AiPattern.GetPlanPattern(AiPlanType.comDMPaNum) == 0;

        // 内野全員との距離を取る
        for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
        {
            if ((OrderIndexType)order == MyOrderIndex)
            {
                continue;
            }

            var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order);
            targetDist[order] = Coordinate.DistanceXZ(chara.Coordinate);
        }

        // ダッシュマンパススピード
        int paspd = GetSettingPass(SettingPassType.DMPaSpd);

        // 内野全員との角度を取る
        for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
        {
            if (order == (int)MyOrderIndex) // 自分
            {
                isSelectTarget[order] = enNaiyaTag.TGNG;
                continue;
            }

            var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order);

            if (!chara.Composite.IsDashman) // 現在ダッシュマンでない
            {
                isSelectTarget[order] = enNaiyaTag.TGNG;
                continue;
            }

            const int YOYUU = 10; // 投げモーションと相手が離れていく分
            int reachtime = (int)(targetDist[order] / paspd + YOYUU);

            if (!chara.Composite.IsCom // マニュアル以外
                && chara.Dashman.EnabledPassCount.Value < reachtime) // おそらくとどかない
            {
                isSelectTarget[order] = enNaiyaTag.TGNG;
                continue;
            }

            if (chara.Coordinate.VelocityY < 0) // 下降に入ってる
            {
                isSelectTarget[order] = enNaiyaTag.TGNG;
                continue;
            }

            if (!NTOnly_f // 一人一回ではない
                || !MyTeam.AiAction((OrderIndexType)order).BallTouchedF) // まだ触ってない
            {
                isSelectTarget[order] = enNaiyaTag.TGOK;
            }
            else
            {
                isSelectTarget[order] = enNaiyaTag.TGNG;
            }

            if (isSelectTarget[order] != enNaiyaTag.TGNG)
            {
                if (chara.Composite.LeftCourtX < Composite.LeftCourtX) // 後ろに人がいる
                {
                    bottom_f = false; // 最後尾ではない
                }

                if (MyOrderIndex > (OrderIndexType)order) // 上位に人がいる
                {
                    topord_f = false; // 先頭ではない
                }

                Notag_f = false;
            }
        }

        if (Notag_f) // タゲが居ない
        {
            return OrderIndexType.Disabled;
        }

        var f = 0;

        for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
        {
            sortValue[order] = 0; // 初期化

            if (isSelectTarget[order] != enNaiyaTag.TGOK)
            {
                targetOrder[f] = OrderIndexType.Disabled;
                continue;
            }

            // 値が少ないほど優先
            switch (patype)
            {
                case enCOMDMPassType.cpmUpOrder: // 一つオーダー上位（上位なら最下位）
                    if (topord_f) // 上位に人がいない
                    {
                        sortValue[order] = -order; // 最下位オーダー
                    }
                    else
                    {
                        sortValue[order] = order > (int)MyOrderIndex // タゲの方が前にいる
                            ? Defines.DBMEMBER_INF
                            : (int)MyOrderIndex - order; // 自分より上位で一番近い人
                    }
                    break;
                case enCOMDMPassType.cpmNear: // 一番近い人
                    sortValue[order] = targetDist[order];
                    break;
                case enCOMDMPassType.cpmFar: // 一番Ｚ軸が遠い人
                    sortValue[order] = -Math.Abs(CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order).Coordinate.Z);
                    break;
                case enCOMDMPassType.cpmRear: // 一つ後ろの人(居なければ一番前の人)
                    if (bottom_f) // 一番後ろ
                    {
                        sortValue[order] = -(int)CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order).Composite.LeftCourtX; // 一番前にいる人
                    }
                    else
                    {
                        var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order);
                        sortValue[order] = chara.Composite.LeftCourtX > Composite.LeftCourtX // タゲの方が前にいる
                            ? Defines.DBCRT_W
                            : (int)Composite.LeftCourtX - (int)chara.Composite.LeftCourtX; // 自分より後ろで一番近い人
                    }
                    break;
            }

            targetOrder[f++] = (OrderIndexType)order;
        }

        // ソート
        for (var i = 0; i < Defines.DBMEMBER_INF - 1; ++i)
        {
            for (var i2 = 0; i2 < Defines.DBMEMBER_INF - 1; i2++)
            {
                if (i == i2) continue; // 同じ

                if (targetOrder[i] != OrderIndexType.Disabled && targetOrder[i2] != OrderIndexType.Disabled)
                {
                    if (sortValue[(int)targetOrder[i]] < sortValue[(int)targetOrder[i2]]) // 小さい方優先
                    {
                        (targetOrder[i2], targetOrder[i]) = (targetOrder[i], targetOrder[i2]);
                    }
                }
            }
        }

        // ソート１位
        var res = targetOrder[0];

        // 現状ＯＫなやつしか駄目な場合
        if (nowOKonly_f)
        {
            if (isSelectTarget[(int)res] != enNaiyaTag.TGOK)
            {
                res = OrderIndexType.Disabled;
            }
        }

        return res;
    }
}