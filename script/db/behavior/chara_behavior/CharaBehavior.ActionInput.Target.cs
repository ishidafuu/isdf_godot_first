using System;
using isdf;

namespace db;

public partial class CharaBehavior
{
    /// <summary>
    /// 内野のパスターゲットを取得します
    /// 内野プレイヤーの中から適切なパスターゲットを選択します
    /// </summary>
    /// <returns>選択されたパスターゲットのオーダーインデックス</returns>
    private OrderIndexType GetNaiyaPassTag()
    {
        var isNoneTarget = true; //完全にタゲが居ない
        var isNoneAngleTarget = true; //向き方向にタゲが居ない

        var passDirectionX = Coordinate.DirectionX;
        var passDirectionZ = Coordinate.DirectionZ;

        var isLeftKey = false;
        var isRightKey = false;
        var isUpKey = false;
        var isDownKey = false;

        if (Composite.IsSelfControl)
        {
            isLeftKey = Pad.KeyLeft.IsPressed; //パス方向入力
            isRightKey = Pad.KeyRight.IsPressed;
            isUpKey = Pad.KeyUp.IsPressed;
            isDownKey = Pad.KeyDown.IsPressed;
        }

        //内野方向を向いてる
        var isInfieldDirection = MySideIndex == 0
            ? passDirectionX == DirectionXType.Left
            : passDirectionX == DirectionXType.Right;

        var isEnemyCourtKey = (MySideIndex == 0 && isRightKey) || (MySideIndex == 1 && isLeftKey);

        //十字入ってない
        var isNeutralKey = (isLeftKey || isRightKey || isUpKey || isDownKey) == false;

        //左コート時、内野内で一番右にいる
        var isTopPosition = true;
        var isFrontPosition = true; //一番手前にいる
        var isBackwardPosition = true; //一番奥にいる

        //ダッシュマンへパス
        var isDashmanPass = MyTeam.Position.DashmanNum > 0;

        var distO2 = Math.Abs(Coordinate.Z - Defines.DBCRT_BL);
        var distO3 = Math.Abs(Coordinate.Z - Defines.DBCRT_FL);
        var isNearO2 = distO2 < distO3;

        TmpStateManager.Instance.TmpState.Clear();
        var targetDist = TmpStateManager.Instance.TmpState.targetDist;
        var isSelectTarget = TmpStateManager.Instance.TmpState.isSelectTarget;
        var targetOrder = TmpStateManager.Instance.TmpState.targetOrder;
        var sortValue = TmpStateManager.Instance.TmpState.sortValue;

        //内野全員との距離を取る
        for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
        {
            if ((OrderIndexType)order == MyOrderIndex)
            {
                continue;
            }

            targetDist[order] = Coordinate.DistanceXZ(MySideOrders[order].Coordinate);
        }

        //パスが出せるダッシュマンがいるか
        if (isDashmanPass) //ダッシュマンへパス
        {
            //内野全員との角度を取る
            for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
            {
                if ((OrderIndexType)order == MyOrderIndex)
                {
                    continue;
                }

                var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order);

                if (chara.Composite.IsDashman == false)
                {
                    continue;
                }

                //向き方向に居る
                isSelectTarget[order] = enNaiyaTag.TGOK;

                isNoneAngleTarget = false; //一人でも向き方向にタゲが見つかった

                //右にダッシュマンがいる
                if (chara.Composite.LeftCourtX > Composite.LeftCourtX)
                {
                    isTopPosition = false;
                }

                //奥にダッシュマンがいる
                if (chara.Coordinate.Z > Coordinate.Z)
                {
                    isBackwardPosition = false;
                }

                //手前にダッシュマンがいる
                if (chara.Coordinate.Z < Coordinate.Z)
                {
                    isFrontPosition = false;
                }
            }

            //パスが出せるダッシュマンがいない
            if (isNoneAngleTarget)
            {
                isDashmanPass = false;
            }
        }

        //最終的にダッシュマンいない
        if (isDashmanPass == false)
        {
            //内野全員との角度を取る
            for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
            {
                if (IsNGPassTag(order))
                {
                    isSelectTarget[order] = enNaiyaTag.TGNG;
                    continue;
                }
                isNoneTarget = false; //一応タゲ可能は人はいる

                var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order);

                if (IsCheckNoAgl(chara.Coordinate.X, chara.Coordinate.Z))
                {
                    //向きに居ない
                    isSelectTarget[order] = enNaiyaTag.TGNOAGL;
                }
                else
                {
                    //向き方向に居る
                    isSelectTarget[order] = enNaiyaTag.TGOK;
                    isNoneAngleTarget = false; //一人でも向き方向にタゲが見つかった
                }

                //誰か右にいる
                if (chara.Composite.LeftCourtX > Composite.LeftCourtX)
                {
                    isTopPosition = false;
                }

                //奥にいる
                if (chara.Coordinate.Z > Coordinate.Z)
                {
                    isBackwardPosition = false;
                }

                //手前にいる
                if (chara.Coordinate.Z < Coordinate.Z)
                {
                    isFrontPosition = false;
                }
            }
        }

        //ダッシュマンいるとき
        if (isDashmanPass)
        {
            if (isTopPosition && isEnemyCourtKey) //先頭で右→が入ってるのときのみ
            {
                if (isUpKey)
                {
                    return OrderIndexType.Outfield2; //右上
                }

                if (isDownKey)
                {
                    return OrderIndexType.Outfield3; //右下
                }

                //右のみ
                return isNearO2
                    ? OrderIndexType.Outfield2
                    : OrderIndexType.Outfield3;
            }

            if (isFrontPosition) //一番手前に居る
            {
                if (isDownKey)
                {
                    return OrderIndexType.Outfield3; //右下
                }
            }

            if (isBackwardPosition) //一番奥に居る
            {
                if (isUpKey)
                {
                    return OrderIndexType.Outfield2; //右下
                }
            }
        }
        else if ((isInfieldDirection == false && isTopPosition) || isNoneTarget) //右向き時しかも先頭もしくは孤立(→外野パス)
        {
            if (Motion.MotionType == CharaMotionType.Ds) //ダッシュ中
            {
                if (Composite.IsSelfControl)
                {
                    if (isUpKey) return OrderIndexType.Outfield2; //上
                    if (isDownKey) return OrderIndexType.Outfield3; //下
                    if (isEnemyCourtKey) return OrderIndexType.Outfield4; //右

                    return isNearO2
                        ? OrderIndexType.Outfield2
                        : OrderIndexType.Outfield3;
                }

                return Auto.DirectionZ switch
                {
                    DirectionZType.Backward => OrderIndexType.Outfield2,
                    DirectionZType.Forward => OrderIndexType.Outfield3,
                    _ => OrderIndexType.Outfield4,
                };
            }

            switch (passDirectionZ)
            {
                case DirectionZType.Backward:
                    return OrderIndexType.Outfield2;
                case DirectionZType.Forward:
                    return OrderIndexType.Outfield3;
                default:
                    if (isEnemyCourtKey)
                    {
                        return OrderIndexType.Outfield4;
                    }

                    return isNearO2
                        ? OrderIndexType.Outfield2
                        : OrderIndexType.Outfield3;
            }
        }

        var f = 0;

        for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
        {
            //向き方向に人なしのとき

            if (isSelectTarget[order] != enNaiyaTag.TGOK
                && (!isNoneAngleTarget || isSelectTarget[order] == enNaiyaTag.TGNG))
            {
                targetOrder[f] = OrderIndexType.Disabled;
                continue;
            }

            if (isNeutralKey) //ニュートラル
            {
                sortValue[order] = targetDist[order]; //内野間は距離が近い人
            }
            else
            {
                var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order);

                //ダッシュマンが居るときは現在Ｚではなく、目標Ｚ
                var tgZ = isDashmanPass
                    ? chara.Dashman.TargetZ
                    : chara.Coordinate.Z;

                //上
                if (isUpKey)
                {
                    sortValue[order] = -tgZ; //Ｚのマイナス（上ほど優先）
                }
                else if (isDownKey) //下
                {
                    sortValue[order] = +tgZ; //Ｚ（下ほど優先）
                }

                //上下が入ってるとき用に合計値
                if (isLeftKey) //左
                {
                    sortValue[order] += chara.Coordinate.X; //Ｘ（左ほど優先）
                }
                else if (isRightKey) //右
                {
                    sortValue[order] -= chara.Coordinate.X; //Ｘのマイナス（右ほど優先）
                }
            }

            targetOrder[f] = (OrderIndexType)order;

            f++;
        }

        //ソート
        for (var i = 0; i < Defines.DBMEMBER_INF - 1; ++i)
        {
            for (var i2 = 0; i2 < Defines.DBMEMBER_INF - 1; i2++)
            {
                if (i == i2
                    || targetOrder[i] == OrderIndexType.Disabled
                    || targetOrder[i2] == OrderIndexType.Disabled)
                {
                    continue;
                }

                if (sortValue[(int)targetOrder[i]] < sortValue[(int)targetOrder[i2]]) //小さい方優先
                {
                    (targetOrder[i2], targetOrder[i]) = (targetOrder[i], targetOrder[i2]);
                }
            }
        }

        //ソート１位
        return targetOrder[0];
    }


    /// <summary>
    /// 外野のパスターゲットを取得します
    /// 外野プレイヤーの中から適切なパスターゲットを選択します
    /// </summary>
    /// <returns>選択されたパスターゲットのオーダーインデックス</returns>
    private OrderIndexType GetGaiyaPassTag()
    {
        var passTarget = OrderIndexType.Disabled; //パスタゲ

        var infieldDirectionX = MySideIndex == 0
            ? DirectionXType.Left
            : DirectionXType.Right;

        //ダッシュマンへパス
        var isDashmanPass = MyTeam.Position.DashmanNum > 0;

        var passDirectionX = Coordinate.DirectionX;
        var passDirectionZ = Coordinate.DirectionZ;

        var lastXKey = Input.LastXKey;

        var isLeftKey = false;
        var isRightKey = false;
        var isUpKey = false;
        var isDownKey = false;

        if (Composite.IsSelfControl)
        {
            isLeftKey = Pad.KeyLeft.IsPressed; //パス方向入力
            isRightKey = Pad.KeyRight.IsPressed;
            isUpKey = Pad.KeyUp.IsPressed;
            isDownKey = Pad.KeyDown.IsPressed;
        }

        //内野向きが押されてる
        var isInfieldKey = (MySideIndex == 0 && isLeftKey) || (MySideIndex == 1 && isRightKey);

        var isNoneXKey = isLeftKey == false && isRightKey == false;
        var isNoneZKey = isUpKey == false && isDownKey == false;

        //十字入ってない
        var isNeutralKey = (isLeftKey || isRightKey || isUpKey || isDownKey) == false;

        var postMan = MyTeam.Position.Postman;

        //とりあえずパスタゲを出す
        switch (Order.OrderIndex)
        {
            case OrderIndexType.Outfield2:
                if (lastXKey == DirectionXType.Neutral)
                {
                    if (isDownKey && isNoneXKey)
                    {
                        passTarget = OrderIndexType.Outfield3;
                    }
                    else
                    {
                        passTarget = passDirectionX == infieldDirectionX
                            ? postMan
                            : OrderIndexType.Outfield4;
                    }
                }
                else
                {
                    passTarget = lastXKey == infieldDirectionX
                        ? postMan
                        : OrderIndexType.Outfield4;
                }
                break;

            case OrderIndexType.Outfield3:
                if (lastXKey == DirectionXType.Neutral)
                {
                    if (isUpKey && isNoneXKey)
                    {
                        passTarget = OrderIndexType.Outfield2;
                    }
                    else
                    {
                        passTarget = passDirectionX == infieldDirectionX
                            ? postMan
                            : OrderIndexType.Outfield4;
                    }
                }
                else
                {
                    passTarget = lastXKey == infieldDirectionX
                        ? postMan
                        : OrderIndexType.Outfield4;
                }
                break;

            case OrderIndexType.Outfield4:

                if (isInfieldKey && isNoneZKey) //内野方向入ってたら内野
                {
                    passTarget = postMan;
                }
                else
                {
                    switch (passDirectionZ)
                    {
                        case DirectionZType.Backward:
                            passTarget = OrderIndexType.Outfield2;
                            break;
                        case DirectionZType.Forward:
                            passTarget = OrderIndexType.Outfield3;
                            break;
                        default:
                            {
                                var distO2 = Math.Abs(Coordinate.Z - Defines.DBCRT_BL);
                                var distO3 = Math.Abs(Coordinate.Z - Defines.DBCRT_FL);

                                passTarget = distO2 < distO3
                                    ? OrderIndexType.Outfield2
                                    : OrderIndexType.Outfield3;
                            }
                            break;
                    }
                }
                break;
        }

        //ダッシュマンいるとき(十字ニュートラルも)
        if (!isDashmanPass || (passTarget != postMan && !isNeutralKey))
        {
            return passTarget;
        }

        var isNoneTarget = true; //タゲが居ない
        TmpStateManager.Instance.TmpState.Clear();
        var targetX = TmpStateManager.Instance.TmpState.targetX;
        var targetDist = TmpStateManager.Instance.TmpState.targetDist;
        var isSelectTarget = TmpStateManager.Instance.TmpState.isSelectTarget;
        var targetOrder = TmpStateManager.Instance.TmpState.targetOrder;
        var sortValue = TmpStateManager.Instance.TmpState.sortValue;

        //内野全員との角度を取る
        for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
        {
            var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order);

            if (chara.Composite.IsDashman
                && (OrderIndexType)order != MyOrderIndex //自分
                && IsCheckLandEnPos(order) == false) //外野からのときは敵コート着地キャラはなしに
            {
                isSelectTarget[order] = enNaiyaTag.TGOK;
                isNoneTarget = false; //一人でも向き方向にタゲが見つかった
            }
            else
            {
                isSelectTarget[order] = enNaiyaTag.TGNG;
            }

            //X距離外野はGetLeftCrtX()が左コートなので絶対値を使う
            targetX[order] = chara.Composite.LeftCourtX; //自分より右に居れば＋
            //Z距離
            var targetZ = Math.Abs(chara.Coordinate.Z - Coordinate.Z); //自分より上にいれば＋
            //距離
            targetDist[order] = Defines.Hypot(targetX[order], targetZ);
        }

        //ダッシュマンいるけど敵コートに着地しちゃう場合もある
        //NoTagのときはそのままポストマンに
        if (isNoneTarget)
        {
            return passTarget;
        }

        //優先順位初期化
        for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
        {
            targetOrder[order] = OrderIndexType.Disabled;
        }

        var f = 0;

        for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
        {
            sortValue[order] = 0; //初期化

            if (isSelectTarget[order] != enNaiyaTag.TGOK)
            {
                continue;
            }

            if (isNeutralKey) //ニュートラル
            {
                sortValue[order] = -targetX[order]; //外野からのときは右（先頭を走ってる人）
            }
            else
            {
                var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order);

                // //ダッシュマンが居るときは現在Ｚではなく、目標Ｚ
                // var tgZ = isDashmanPass
                //     ? chara.MyState.Dashman.TargetZ
                //     : chara.MyCoordinate.Z;
                //ダッシュマンが居るときは現在Ｚではなく、目標Ｚ
                var tgZ = chara.Dashman.TargetZ;

                //上
                if (isUpKey)
                {
                    sortValue[order] = -tgZ; //Ｚのマイナス（上ほど優先）
                }
                else if (isDownKey) //下
                {
                    sortValue[order] = +tgZ; //Ｚ（下ほど優先）
                }

                if (isLeftKey) //左
                {
                    sortValue[order] += chara.Coordinate.X; //Ｘ（左ほど優先）
                }
                else if (isRightKey) //右
                {
                    sortValue[order] -= chara.Coordinate.X; //Ｘのマイナス（右ほど優先）
                }
                //else//ニュートラル
                //{
                //  //sortValue[i] = (var)sltgXZ[i];//距離
                //  sortValue[i] = (var)sltgX[i];//先頭を走ってる人
                //}
            }

            targetOrder[f++] = (OrderIndexType)order;
        }

        //ソート
        for (var i = 0; i < Defines.DBMEMBER_INF - 1; ++i)
        {
            for (var i2 = 0; i2 < Defines.DBMEMBER_INF - 1; i2++)
            {
                if (i == i2
                    || targetOrder[i] == OrderIndexType.Disabled
                    || targetOrder[i2] == OrderIndexType.Disabled)
                {
                    continue;
                }

                var orderA = (int)targetOrder[i];
                var orderB = (int)targetOrder[i2];

                var dist = Math.Abs(sortValue[orderA] - sortValue[orderB]);

                var isSwitch = false;

                if (dist <= Defines.Percent)
                {
                    // ほぼ同じ場合(1dot以内)は絶対距離で判断
                    if (targetDist[orderA] < targetDist[orderB]) //小さい方優先
                    {
                        isSwitch = true;
                    }
                }
                else if (sortValue[orderA] < sortValue[orderB]) //小さい方優先
                {
                    isSwitch = true;
                }

                if (isSwitch)
                {
                    (targetOrder[i2], targetOrder[i]) = (targetOrder[i], targetOrder[i2]);
                }
            }
        }

        //ソート１位
        return targetOrder[0];
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

    //シュートタゲセット
    //isIgnoreDirection:向きを反映しない（強制的に内野タゲ）
    private OrderIndexType GetShootTarget(int angle, bool isIgnoreDirection)
    {
        TmpStateManager.Instance.TmpState.Clear();
        var targetX = TmpStateManager.Instance.TmpState.targetX;
        var targetY = TmpStateManager.Instance.TmpState.targetY;
        var targetDist = TmpStateManager.Instance.TmpState.targetDist;
        var isSelectTarget = TmpStateManager.Instance.TmpState.isSelectTarget;

        //敵内野全員との角度を取る
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
            targetDist[order] = Defines.Hypot(targetX[order], targetZ); //距離

            // 角度を無視しない場合は角度チェックを行う
            if (isIgnoreDirection == false)
            {
                const int angle12 = 12;
                var targetAngle = GetTagAgl2(targetX[order], targetZ); //新12時法

                //メインアングルチェック
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

        //距離による優先順位
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