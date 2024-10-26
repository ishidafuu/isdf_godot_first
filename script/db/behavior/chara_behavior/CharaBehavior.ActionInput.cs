using System;
using isdf;
using Defines = db.Defines;

namespace db;

public partial class CharaBehavior
{
    public void UpdateActionInput()
    {
        if (RefereeState.ShiaiPahse != ShiaiPhase.Shiai
            || RefereeState.IsGameSet)
        {
            return;
        }

        if (MyState.Motion.HasFlag(CharaMotionFlag.Dam)
            || MyState.Motion.HasFlag(CharaMotionFlag.ANG)
            || MyState.Live.IsDead)
        {
            return;
        }

        if (MyTeamState.MainState.IsCancelActionInput)
        {
            return;
        }

        // 敵コート避け可能フラグ
        var canEnemyCourtDodge = GetCanDodgeEnemyCourt();

        switch (MyState.Auto.AutoType)
        {
            // 自由
            case AutoType.Free:
                if (IsSelfControl)
                {
                    ControlSelf();
                }
                else
                {
                    AutoPickUp();
                }
                break;
            // コートに戻る
            case AutoType.Return:
                AutoReturnAction();
                break;
            // 拾いに行く
            case AutoType.Get:
                if (MyState.Motion.MotionType
                    is CharaMotionType.St
                    or CharaMotionType.Wk
                    or CharaMotionType.Ds)
                {
                    AutoPickUp();
                }
                break;
            // ポジショニング
            case AutoType.DfPos:
                AutoPickUp();
                break;

            // よけ
            case AutoType.Dg:
                AutoDodgeAction();
                break;

            // COMアクション
            case AutoType.CPUShPa:
                ComAction();
                break;
        }
    }

    private void AutoReturnAction()
    {
        //     if (IsSelfControl//←いらないかも
        //       || (IsCOM() == false))
        //     {
        //
        //       bool muki_f = ((st_.mysideNo_ == 0) && (MyState.Coordinate.Muki == mL))
        //         || ((st_.mysideNo_ == 1) && (MyState.Coordinate.Muki == mR));
        //
        //       switch (st_.pstMyCh_.Motion.Mtype)
        //       {
        //       case dbmtCr:
        //         break;
        //       case dbmtSt:
        //       case dbmtWk:
        //       case dbmtDs:
        //       case dbmtSl://スリップも追加オーバーラインの瞬間なぞのキャッチできてしまう
        //         if ((st_.pstMyCh_.ECDdg_f == false) && (ecdgOK_f))
        //         {
        //           if (MyPad.IsJump()//ジャンプ入力
        //             && muki_f
        //             && (st_.pstMyCh_.ECDjp_f == false))
        //           {
        //             pCommon_.JumpSet(false, false, false);//ジャンプ
        //             //mid::midLog("J0\n");
        //             st_.pstMyCh_.ECDdg_f = true;//１回だけ
        //             st_.pstMyCh_.ECDjp_f = true;//１回だけ
        //           }
        //           else if (MyPad.IsDodge())//よけ
        //           {
        //             pCommon_.SetMtype(dbmtDg);
        //             st_.pstMyCh_.ECDdg_f = true;//１回だけ
        //           }
        //           //else if (MyPad.IsCatch())//キャッチ入力
        //           //{
        //           //  pCommon_.SetCatchMuki();
        //           //  pCommon_.SetMtype(dbmtCa);
        //           //  st_.pstMyCh_.ECDdg_f = true;//１回だけ
        //           //}
        //
        //           if (st_.pstMyCh_.ECDdg_f)
        //           {
        //             pCommon_.CatchSE();
        //             st_.pstMyCh_.LastMuki = MyState.Coordinate.Muki;
        //             st_.pstMyCh_.LastMukiZ = MyState.Coordinate.MukiZ;
        //           }
        //         }
        //         break;
        //       case dbmtSh:
        //       case dbmtPa:
        //         if (ecdgOK_f) pCommon_.CanselJump(false);
        //         break;
        //       case dbmtCa:
        //       case dbmtJCa:
        //         if ((st_.pstMyCh_.ECDjp_f == false) && ecdgOK_f)
        //         {
        //           //mid::midLog("J1\n");
        //           if (pCommon_.CanselJump(false) && muki_f)
        //           {
        //             st_.pstMyCh_.ECDjp_f = true;//１回だけ
        //             st_.pstMyCh_.ECDdg_f = true;//１回だけ
        //           }
        //           else
        //           {
        //             pCommon_.AutoPickUp();
        //           }
        //         }
        //         //オーバラインは審判息なのでもう不要
        //         break;
        //       case dbmtDg:
        //         if ((st_.pstMyCh_.ECDjp_f == false) && ecdgOK_f)
        //         {
        //           //mid::midLog("J2\n");
        //           if (pCommon_.CanselJump(false) && muki_f)
        //           {
        //             st_.pstMyCh_.ECDjp_f = true;//１回だけ
        //             st_.pstMyCh_.ECDdg_f = true;//１回だけ
        //           }
        //           else
        //           {
        //             //よけ限界時間
        //             ++st_.pstMyCh_.ECDdg_c;
        //             if ((st_.pstMyCh_.ECDdg_c < pmgEO_.mgDt_.dtSet_.GetDtInfield(setEnCourtCrTime))
        //               && MyPad.IsDodge2())//押しっぱなしで避け続けるようにする
        //             {
        //               st_.pstMyCh_.Anime.Ani_c = 0;
        //             }
        //           }
        //         }
        //
        //         break;
        //       }
        //     }
    }

    private void AutoDodgeAction()
    {
        //     //COMの的コートオートよけ
        //     if ((IsSelfControl == false)
        //       && (pmgSG_.stBa_.Motion == bmShoot)
        //       && (ecdgOK_f))
        //     {
        //       switch (st_.pstMyCh_.Motion.Mtype)
        //       {
        //       case dbmtSt:
        //       case dbmtWk:
        //       case dbmtDs:
        //         if (st_.pstMyCh_.ECDdg_f == false)
        //         {
        //           pCommon_.SetMtype(dbmtDg);
        //           pCommon_.CatchSE();
        //           //st_.pstMyCh_.ECDdg_f = true;//１回だけ
        //         }
        //         break;
        //       case dbmtDg:
        //         //よけ限界時間
        //         ++st_.pstMyCh_.ECDdg_c;
        //
        //         if (st_.pstMyCh_.ECDdg_c < pmgEO_.mgDt_.dtSet_.GetDtInfield(setEnCourtCrTime))
        //         {
        //           st_.pstMyCh_.Anime.Ani_c = 0;
        //         }
        //         break;
        //       }
        //     }
        //     else
        //     {
        //       switch (st_.pstMyCh_.Motion.Mtype)
        //       {
        //       case dbmtSt:
        //       case dbmtWk:
        //       case dbmtDs:
        //         pCommon_.SetMtypeReset(dbmtDg);
        //         pCommon_.CatchSE();
        //         break;
        //       case dbmtDg:
        //         //避け続けるようにする
        //         st_.pstMyCh_.Anime.Ani_c = 0;
        //         break;
        //       }
        //     }
        //   }
    }

    private void ComAction()
    {
    }

    private ActionType GetActionType()
    {
        ActionType actionType;
        switch (BallState.MotionType)
        {
            case BallMotionType.Hold when BallState.HolderSide == MySideIndex:
                actionType = ActionType.ATA;
                break;
            case BallMotionType.Free:
            case BallMotionType.Bound:
            case BallMotionType.Referee:
            case BallMotionType.Hold when BallState.HolderSide == MySideIndex:
            case BallMotionType.Pass when BallState.ThrowerSideNo == MySideIndex:
                actionType = ActionType.ATF;
                break;
            default:
                actionType = ActionType.ATD;
                break;
        }
        return actionType;
    }

    private bool GetCanDodgeEnemyCourt()
    {
        bool canEnemyCourtDodge;
        if (IsBallHolder || MyState.Order.IsOutfield)
        {
            // ボール持ち、外野はよけNG
            canEnemyCourtDodge = false;
        }
        else if (IsSelfControl || MyState.Auto.IsFreeAction)
        {
            // 自由状態
            canEnemyCourtDodge = true;
        }
        else if (BallState.MotionType == BallMotionType.Hold)
        {
            // 敵がボール保持
            canEnemyCourtDodge = BallState.HolderSide == EnemySideIndex;
        }
        else if (BallState.MotionType == BallMotionType.Shoot)
        {
            // 味方がターゲットのシュート
            canEnemyCourtDodge = BallState.ShotTargetSide == MySideIndex;
        }
        else
        {
            // 敵ボール
            canEnemyCourtDodge = BallState.TimerSide == EnemySideIndex;
        }
        return canEnemyCourtDodge;
    }

    //勝手に拾う処理
    private void AutoPickUp()
    {
        if (MyState.Motion.HasFlag(CharaMotionFlag.Act) == false
            && IsFree(false)
            && IsPickUpPos())
        {
            HoldBall(false, false);
        }
    }

    //拾える位置関係か
    private bool IsPickUpPos()
    {
        //ジャンプボール上昇中は捕れない
        //審判投げ入れボールも相手外野は捕れない
        //内野も取れないように
        if (BallState.CanHoldJumpBall(MySideIndex) == false)
        {
            return false;
        }

        // さわれない指定が出ているボール
        if (BallState.IsNGGet)
        {
            if (BallState.NGGetTNo == MySideIndex
                && BallState.NGGetPNo == MemberIndex)
            {
                return false;
            }
        }

        // 停止、バウンド以外は拾えない
        if (BallState.MotionType != BallMotionType.Free
            && BallState.MotionType != BallMotionType.Bound
            && BallState.MotionType != BallMotionType.Referee)
        {
            return false;
        }

        //当たり
        var atariDepth = GetSettingBall(SettingBallType.FAtariDepth2);
        var isHitDepth = Math.Abs(BallState.Coordinate.Z - MyState.Coordinate.Z) <= atariDepth;
        return isHitDepth && MyState.Coordinate.HitBox.IsPile(BallState.Atari);
    }

    //ボール持った処理
    private void HoldBall(bool isNoSe, bool isLookBall)
    {
        // 保持状況をチームに渡す
        CallTeamHoldBall();

        MyState.Damage.KagamiCount.Clear();
        MyState.Air.IsAirAction = false;
        MyState.Move.LastDirectionXType = DirectionXType.Neutral;
        MyState.Com.IsCatchCounter = false;
        MyState.Com.IsComTossPassGet = false;
        MyState.Pass.MirrorShotLimitCount.Set(Defines.MIRLIM);

        HoldBallSetMirrorState();

        if (isLookBall)
        {
            ResetAutoDirection();

            MyState.Auto.DirectionX = BallState.Coordinate.VelocityX switch
            {
                > 0 => DirectionXType.Right,
                < 0 => DirectionXType.Left,
                _ => MyState.Auto.DirectionX
            };

            if (Math.Abs(BallState.Coordinate.VelocityZ) > Math.Abs(BallState.Coordinate.VelocityX))
            {
                MyState.Auto.DirectionZ = BallState.Coordinate.VelocityZ switch
                {
                    > 0 => DirectionZType.Backward,
                    < 0 => DirectionZType.Forward,
                    _ => MyState.Auto.DirectionZ
                };
            }
            else
            {
                MyState.Auto.DirectionZ = DirectionZType.Neutral;
            }

            MukiSetAuto();
        }

        StopHomingSe();
        if (isNoSe == false)
        {
            PlaySe(SeType.Take);
        }

        //ボール側の処理
        CallBallHold();

        //拾った瞬間dbaFreeのときはその瞬間のタゲをカーソルキャラに
        //それ以外はfreeに戻ったタイミング
        //chCommon_.
        SetMukiAglFromDirection();

        //最初のタゲを敵の操作キャラに
        //外野の可能性もでるのでなんとかする
        //ShTagSet(false); //拾った時点でタゲ無しの時はタゲだけ無理矢理近いキャラから取った方が無難かも
        var passTargetOrderIndex = MyState.Order.IsInfield
            ? GetNaiyaPassTag()
            : GetGaiyaPassTag();

        SetPassTarget(passTargetOrderIndex);

        // 拾った時点でタゲ無しの時はタゲだけ無理矢理近いキャラから取った方が無難かも
        if (EnemyTeamState.IsAllOut == false)
        {
            //シュートタゲは向き反映
            var orderIndex = GetTarget(false);
            if (orderIndex == OrderIndexType.Disabled)
            {
                if (IsSelfControl == false)
                {
                    //CPUだけ無理矢理。後で外すかも
                    CallBallChangeShootTarget(EnemyTeamState.MainState.ControlOrderIndex);
                }
            }
            else
            {
                CallBallChangeShootTarget(orderIndex);
            }

            //カーソルは強制的に内野
            var enemyControlOrderIndex = GetTarget(true);
            if (enemyControlOrderIndex != OrderIndexType.Disabled) //ないとはおもうが
            {
                CallEnemyTeamChangeControl(enemyControlOrderIndex);
            }
        }
// #ifdef __K_DEBUG_SHIAI__
//         kdebug::DebugSystem* pDs = kdebug::DebugSystem::GetInstance();
//         if (pDs.IsReturnBall())
//         {
//             if (st_.posNo_ != 0)
//             {
//                 // ボールが手元に帰ってくる処理
//                 pmgGO_.pmgBa_.baCommon_.ResetRefPos_Prev(false);
//
//                 // 操作キャラを強制的にボール持ってる人に
//                 //st_.pmgTm_[SIDE0].st_.pmgMyTm_.SetCtrlBallGet(0);
//             }
//
//             // 自動シュート状態を取得
//             int step = pDs.GetAutoShootStep();
//             if (step == kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_WAIT)
//             {
//                 pDs.SetReturnBallFlg(false); // フラグを落とす
//                 pDs.SetAutoShootStep(kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_CHOOSE);
//             }
//         }
// #endif // #ifdef __K_DEBUG_SHIAI__
    }

    private void SetShootTarget(OrderIndexType orderIndex)
    {
        if (IsNGPassTag(orderIndex))
        {
            if (MyOrderIndex != OrderIndexType.Outfield4 && IsNGPassTag(OrderIndexType.Outfield4) == false)
            {
                orderIndex = OrderIndexType.Outfield4;
            }
            else if (MyOrderIndex != OrderIndexType.Outfield3 && IsNGPassTag(OrderIndexType.Outfield3) == false)
            {
                orderIndex = OrderIndexType.Outfield3;
            }
            else if (MyOrderIndex != OrderIndexType.Outfield2 && IsNGPassTag(OrderIndexType.Outfield2) == false)
            {
                orderIndex = OrderIndexType.Outfield2;
            }
            else
            {
                if (MyTeamState.PositionState.PassAbleCount > 0
                    && MyOrderIndex != MyTeamState.PositionState.Postman
                    && IsNGPassTag(MyTeamState.PositionState.Postman) == false)
                {
                    orderIndex = MyTeamState.PositionState.Postman;
                }
                else
                {
                    orderIndex = OrderIndexType.Outfield4;
                }
            }
        }

        CallBallChangePassTarget(orderIndex);
    }

    //タゲセット
    //isIgnoreDirection向きを反映しない（強制的に内野タゲ）
    private OrderIndexType GetTarget(bool isIgnoreDirection)
    {
        TmpStateManager.Instance.TmpState.Clear();
        var targetX = TmpStateManager.Instance.TmpState.targetX;
        var targetY = TmpStateManager.Instance.TmpState.targetY;
        var targetDist = TmpStateManager.Instance.TmpState.targetDist;
        var isSelectTarget = TmpStateManager.Instance.TmpState.isSelectTarget;

        SetMukiAglFromDirection();

        //敵内野全員との角度を取る
        for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
        {
            var enemyChara = CharaBehaviorManager.Instance.GetChara(EnemySideIndex, order);

            if (enemyChara.IsEnableShootTarget == false)
            {
                continue;
            }

            isSelectTarget[order] = enNaiyaTag.TGOK;
            targetX[order] = enemyChara.X - MyState.Coordinate.X;
            var targetZ = enemyChara.Z - MyState.Coordinate.Z;
            targetY[order] = enemyChara.Y - MyState.Coordinate.Y;
            targetDist[order] = Defines.Hypot(targetX[order], targetZ); //距離

            //向きを反映しない（強制的に内野タゲ）
            if (isIgnoreDirection == false)
            {
                const int angle12 = 12;
                var angle2 = GetTagAgl2(targetX[order], targetZ); //新12時法
                //メインアングルチェック
                if (angle2 != (MyState.Shoot.Angle12 + 0) % angle12
                    && angle2 != (MyState.Shoot.Angle12 + 1) % angle12
                    && angle2 != (MyState.Shoot.Angle12 + 2) % angle12
                    && angle2 != (MyState.Shoot.Angle12 + 3) % angle12)
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

    //新タゲ角度(12時計算)
    private int GetTagAgl2(int tX, int tZ)
    {
        var res = -1;
        var absX = Math.Abs(tX);
        var absZ = Math.Abs(tZ);
        var isMinusX = tX < 0;
        var isMinusZ = tZ < 0;

        const int root3 = 173;
        const int root1 = 100;
        const int angle90 = 3;

        if (absX * root3 < absZ * root1)
        {
            res = -1;
        }
        else if (absZ * root3 < absX * root1)
        {
            res = 0;
        }
        else
        {
            res = +1;
        }

        if (isMinusZ ^ isMinusX) //符号反転
        {
            res *= -1;
        }

        res += 1;

        if (isMinusX && isMinusZ)
        {
            res += angle90 * 2;
        }
        else if (isMinusX)
        {
            res += angle90 * 3;
        }
        else if (isMinusZ)
        {
            res += angle90 * 1;
        }

        return res;
    }

    private void SetPassTarget(OrderIndexType orderIndex)
    {
        if (IsNGPassTag(orderIndex))
        {
            if (MyOrderIndex != OrderIndexType.Outfield4 && IsNGPassTag(OrderIndexType.Outfield4) == false)
            {
                orderIndex = OrderIndexType.Outfield4;
            }
            else if (MyOrderIndex != OrderIndexType.Outfield3 && IsNGPassTag(OrderIndexType.Outfield3) == false)
            {
                orderIndex = OrderIndexType.Outfield3;
            }
            else if (MyOrderIndex != OrderIndexType.Outfield2 && IsNGPassTag(OrderIndexType.Outfield2) == false)
            {
                orderIndex = OrderIndexType.Outfield2;
            }
            else
            {
                if (MyTeamState.PositionState.PassAbleCount > 0
                    && MyOrderIndex != MyTeamState.PositionState.Postman
                    && IsNGPassTag(MyTeamState.PositionState.Postman) == false)
                {
                    orderIndex = MyTeamState.PositionState.Postman;
                }
                else
                {
                    orderIndex = OrderIndexType.Outfield4;
                }
            }
        }

        CallBallChangePassTarget(orderIndex);
    }

    private void NGPaTagShift()
    {
    }

    //内野パスタゲセット★
    private OrderIndexType GetNaiyaPassTag()
    {
        var isNoneTarget = true; //完全にタゲが居ない
        var isNoneAngleTarget = true; //向き方向にタゲが居ない

        var passDirectionX = MyState.Coordinate.DirectionX;
        var passDirectionZ = MyState.Coordinate.DirectionZ;

        var isLeftKey = false;
        var isRightKey = false;
        var isUpKey = false;
        var isDownKey = false;

        if (IsSelfControl)
        {
            isLeftKey = MyPad.KeyLeft.IsPressed; //パス方向入力
            isRightKey = MyPad.KeyRight.IsPressed;
            isUpKey = MyPad.KeyUp.IsPressed;
            isDownKey = MyPad.KeyDown.IsPressed;
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
        var isDashmanPass = MyTeamState.PositionState.DashmanNum > 0;

        var distO2 = Math.Abs(MyState.Coordinate.Z - Defines.DBCRT_BL);
        var distO3 = Math.Abs(MyState.Coordinate.Z - Defines.DBCRT_FL);
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

            targetDist[order] = MyState.Coordinate.DistanceXZ(MySideOrders[order].Coordinate);
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

                var chara = CharaBehaviorManager.Instance.GetChara(MySideIndex, order);

                if (chara.IsDashman == false)
                {
                    continue;
                }

                //向き方向に居る
                isSelectTarget[order] = enNaiyaTag.TGOK;

                isNoneAngleTarget = false; //一人でも向き方向にタゲが見つかった

                //右にダッシュマンがいる
                if (chara.LeftCourtX > LeftCourtX)
                {
                    isTopPosition = false;
                }

                //奥にダッシュマンがいる
                if (chara.MyState.Coordinate.Z > MyState.Coordinate.Z)
                {
                    isBackwardPosition = false;
                }

                //手前にダッシュマンがいる
                if (chara.MyState.Coordinate.Z < MyState.Coordinate.Z)
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

                var chara = CharaBehaviorManager.Instance.GetChara(MySideIndex, order);

                if (IsCheckNoAgl(chara.MyState.Coordinate.X, chara.MyState.Coordinate.Z))
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
                if (chara.LeftCourtX > LeftCourtX)
                {
                    isTopPosition = false;
                }

                //奥にいる
                if (chara.MyState.Coordinate.Z > MyState.Coordinate.Z)
                {
                    isBackwardPosition = false;
                }

                //手前にいる
                if (chara.MyState.Coordinate.Z < MyState.Coordinate.Z)
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
            if (MyState.Motion.MotionType == CharaMotionType.Ds) //ダッシュ中
            {
                if (IsSelfControl)
                {
                    if (isUpKey) return OrderIndexType.Outfield2; //上
                    if (isDownKey) return OrderIndexType.Outfield3; //下
                    if (isEnemyCourtKey) return OrderIndexType.Outfield4; //右

                    return isNearO2
                        ? OrderIndexType.Outfield2
                        : OrderIndexType.Outfield3;
                }

                return MyState.Auto.DirectionZ switch
                {
                    DirectionZType.Backward => OrderIndexType.Outfield2,
                    DirectionZType.Forward => OrderIndexType.Outfield3,
                    _ => OrderIndexType.Outfield4
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

            //しかも先頭もしくは孤立は4番でFA
            return OrderIndexType.Outfield4;
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
                var chara = CharaBehaviorManager.Instance.GetChara(MySideIndex, order);

                //ダッシュマンが居るときは現在Ｚではなく、目標Ｚ
                var tgZ = isDashmanPass
                    ? chara.MyState.Dashman.TargetZ
                    : chara.MyState.Coordinate.Z;

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
                    sortValue[order] += chara.MyState.Coordinate.X; //Ｘ（左ほど優先）
                }
                else if (isRightKey) //右
                {
                    sortValue[order] -= chara.MyState.Coordinate.X; //Ｘのマイナス（右ほど優先）
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

    //外野間パスタゲセット★
    private OrderIndexType GetGaiyaPassTag()
    {
        var passTarget = OrderIndexType.Disabled; //パスタゲ

        var infieldDirectionX = MySideIndex == 0
            ? DirectionXType.Left
            : DirectionXType.Right;

        //ダッシュマンへパス
        var isDashmanPass = MyTeamState.PositionState.DashmanNum > 0;

        var passDirectionX = MyState.Coordinate.DirectionX;
        var passDirectionZ = MyState.Coordinate.DirectionZ;

        var lastXKey = MyState.Pad.LastXKey;

        var isLeftKey = false;
        var isRightKey = false;
        var isUpKey = false;
        var isDownKey = false;

        if (IsSelfControl)
        {
            isLeftKey = MyPad.KeyLeft.IsPressed; //パス方向入力
            isRightKey = MyPad.KeyRight.IsPressed;
            isUpKey = MyPad.KeyUp.IsPressed;
            isDownKey = MyPad.KeyDown.IsPressed;
        }

        //内野向きが押されてる
        var isInfieldKey = (MySideIndex == 0 && isLeftKey) || (MySideIndex == 1 && isRightKey);

        var isNoneXKey = isLeftKey == false && isRightKey == false;
        var isNoneZKey = isUpKey == false && isDownKey == false;

        //十字入ってない
        var isNeutralKey = (isLeftKey || isRightKey || isUpKey || isDownKey) == false;

        var postMan = MyTeamState.PositionState.Postman;

        //とりあえずパスタゲを出す
        switch (MyState.Order.OrderIndex)
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
                            var distO2 = Math.Abs(MyState.Coordinate.Z - Defines.DBCRT_BL);
                            var distO3 = Math.Abs(MyState.Coordinate.Z - Defines.DBCRT_FL);

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
            var chara = CharaBehaviorManager.Instance.GetChara(MySideIndex, order);
            if (chara.IsDashman
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
            targetX[order] = chara.LeftCourtX; //自分より右に居れば＋
            //Z距離
            var targetZ = Math.Abs(chara.MyState.Coordinate.Z - MyState.Coordinate.Z); //自分より上にいれば＋
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
                var chara = CharaBehaviorManager.Instance.GetChara(MySideIndex, order);

                // //ダッシュマンが居るときは現在Ｚではなく、目標Ｚ
                // var tgZ = isDashmanPass
                //     ? chara.MyState.Dashman.TargetZ
                //     : chara.MyState.Coordinate.Z;
                //ダッシュマンが居るときは現在Ｚではなく、目標Ｚ
                var tgZ = chara.MyState.Dashman.TargetZ;

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
                    sortValue[order] += chara.X; //Ｘ（左ほど優先）
                }
                else if (isRightKey) //右
                {
                    sortValue[order] -= chara.X; //Ｘのマイナス（右ほど優先）
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

    private bool IsCheckLandEnPos(int order)
    {
        var chara = CharaBehaviorManager.Instance.GetChara(MySideIndex, order);
        var tLandX = MySideIndex == 0
            ? chara.MyState.Air.LandX
            : Defines.DBCRT_CL - (chara.MyState.Air.LandX - Defines.DBCRT_CL);

        return chara.MyState.Order.IsInfield && tLandX > Defines.DBCRT_CLI;
    }

    //角度に入っていない
    private bool IsCheckNoAgl(int targetX, int targetZ)
    {
        var isInAngle = MyState.Coordinate.DirectionZ switch
        {
            DirectionZType.Forward => targetZ < MyState.Coordinate.Z,
            DirectionZType.Neutral => true,
            DirectionZType.Backward => targetZ > MyState.Coordinate.Z,
            _ => throw new ArgumentOutOfRangeException()
        };

        if (isInAngle)
        {
            isInAngle = MyState.Coordinate.DirectionX switch
            {
                DirectionXType.Left => targetX < MyState.Coordinate.X,
                DirectionXType.Neutral => true,
                DirectionXType.Right => targetX > MyState.Coordinate.X,
                _ => throw new ArgumentOutOfRangeException()
            };
        }

        return isInAngle == false;
    }

    private void HoldBallSetMirrorState()
    {
        const int activeCount = 1;
        const int inactiveCount = 0;

        if (BallState.MotionType == BallMotionType.Pass
            && BallState.ThrowerSideNo == MySideIndex)
        {
            MyState.Pass.MirrorShotCount.Set(activeCount);

            var mirrorPassCount = MyPad.ButtonB.IsPressed
                ? activeCount
                : inactiveCount;
            MyState.Pass.MirrorPassCount.Set(mirrorPassCount);
        }
        else
        {
            MyState.Pass.MirrorPassCount.Set(inactiveCount);

            // キャッチボタン不要で取れるボール
            var isActiveMirrorShot = BallState.MotionType is BallMotionType.Free or BallMotionType.Bound or BallMotionType.Referee
                                     && MyState.Damage.FumbleCount.Value == 0;

            var mirrorShotCount = isActiveMirrorShot
                ? activeCount
                : inactiveCount;
            MyState.Pass.MirrorShotCount.Set(mirrorShotCount);
        }
    }

    //向き変え(強制的にAUTOMUKIに向かせる)
    private bool MukiSetAuto()
    {
        var lastDirectionX = MyState.Coordinate.DirectionX;
        var lastDirectionZ = MyState.Coordinate.DirectionZ;

        MyState.Coordinate.DirectionX = MyState.Auto.DirectionX switch
        {
            DirectionXType.Left => DirectionXType.Left,
            DirectionXType.Right => DirectionXType.Right,
            _ => MyState.Coordinate.DirectionX
        };

        MyState.Coordinate.DirectionZ = MyState.Auto.DirectionZ switch
        {
            DirectionZType.Forward => DirectionZType.Forward,
            DirectionZType.Backward => DirectionZType.Backward,
            DirectionZType.Neutral => DirectionZType.Neutral,
            _ => MyState.Coordinate.DirectionZ
        };

        // 向き変えありかどうか
        var isChanged = lastDirectionX != MyState.Coordinate.DirectionX
                        || lastDirectionZ != MyState.Coordinate.DirectionZ;

        if (isChanged)
        {
            //タゲも変える
            SetMukiAglFromDirection();
        }

        return isChanged;
    }

    // AUTO向き変え初期化
    private void ResetAutoDirection()
    {
        MyState.Auto.DirectionX = MyState.Coordinate.DirectionX switch
        {
            DirectionXType.Left => DirectionXType.Left,
            DirectionXType.Neutral => DirectionXType.Neutral,
            DirectionXType.Right => DirectionXType.Right,
            _ => throw new ArgumentOutOfRangeException()
        };
        MyState.Auto.DirectionZ = MyState.Coordinate.DirectionZ switch
        {
            DirectionZType.Forward => DirectionZType.Forward,
            DirectionZType.Neutral => DirectionZType.Neutral,
            DirectionZType.Backward => DirectionZType.Backward,
            _ => throw new ArgumentOutOfRangeException()
        };
    }

    private void SetMukiAglFromDirection()
    {
        SetMukiAgl(MyState.Coordinate.DirectionX == DirectionXType.Left,
            MyState.Coordinate.DirectionX == DirectionXType.Right,
            MyState.Coordinate.DirectionZ == DirectionZType.Backward,
            MyState.Coordinate.DirectionZ == DirectionZType.Forward);
    }

    //ターゲッティング用向き
    private void SetMukiAgl(bool isLeft, bool isRight, bool isUp, bool isDown)
    {
        switch (MyState.Order.GetOrderFieldType())
        {
            case OrderFieldType.Infield:
                if (isRight)
                {
                    if (isUp)
                    {
                        MyState.Shoot.Angle12 = 11; //11 0 1 2
                    }
                    else if (isDown)
                    {
                        MyState.Shoot.Angle12 = 3; //3 4 5 6
                    }
                    else
                    {
                        MyState.Shoot.Angle12 = 1; //1 2 3 4
                    }
                }
                else if (isLeft)
                {
                    if (isUp)
                    {
                        MyState.Shoot.Angle12 = 9; //9 10 11 0
                    }
                    else if (isDown)
                    {
                        MyState.Shoot.Angle12 = 5; //5 6 7 8
                    }
                    else
                    {
                        MyState.Shoot.Angle12 = 7; //7 8 9 10
                    }
                }
                break;
            case OrderFieldType.Outfield2:
                if (isLeft)
                {
                    MyState.Shoot.Angle12 = 5; //5678
                }
                else if (isRight)
                {
                    MyState.Shoot.Angle12 = 3; //3456
                }
                break;
            case OrderFieldType.Outfield3:
                if (isLeft)
                {
                    MyState.Shoot.Angle12 = 9; //9 10 11 0
                }
                else if (isRight)
                {
                    MyState.Shoot.Angle12 = 11; //11 0 1 2
                }
                break;
            case OrderFieldType.Outfield4:
                if (MySideIndex == 0)
                {
                    if (isUp)
                    {
                        MyState.Shoot.Angle12 = 9; //9 10 11 0
                    }
                    else if (isDown)
                    {
                        MyState.Shoot.Angle12 = 5; //5 6 7 8
                    }
                    else
                    {
                        MyState.Shoot.Angle12 = 7; //7 8 9 10
                    }
                }
                else
                {
                    if (isUp)
                    {
                        MyState.Shoot.Angle12 = 11; //11 0 1 2
                    }
                    else if (isDown)
                    {
                        MyState.Shoot.Angle12 = 3; //3 4 5 6
                    }
                    else
                    {
                        MyState.Shoot.Angle12 = 1; //1 2 3 4
                    }
                }
                break;
            default:
                throw new ArgumentOutOfRangeException();
        }
    }

    /// <summary>
    /// パスタゲにならない
    /// </summary>
    bool IsNGPassTag(OrderIndexType order)
    {
        if (order == MyOrderIndex
            || order == OrderIndexType.Disabled)
        {
            return true; //パス不可
        }

        var chara = CharaBehaviorManager.Instance.GetChara(MySideIndex, order);

        //★ダッシュマンはだいじょぶ
        if (chara.IsDashman)
            //&& ((st_.pmgMyTm_.st_.pmgMyCh_[pos].MyState.Coordinate.dY >= (-XYMAG))
            //  || (st_.pstMyCh_.MirPass_c > 0)))//下降ではない
        {
            return false;
        }

        //空中の人はパスタゲにならないように
        return chara.IsFree(true) == false
               || chara.MyState.Motion.HasFlag(CharaMotionFlag.Ar);
    }

    bool IsNGPassTag(int order)
    {
        return IsNGPassTag((OrderIndexType)order);
    }

    //自分で操作
    private void ControlSelf()
    {
        var actionType = GetActionType();

        const int MIRWAIT = 4;
        //モーション変更前の向き
        var lastMuki = MyState.Coordinate.DirectionX;
        var lastMukiZ = MyState.Coordinate.DirectionZ;
        bool LastMukiKeep_f = false;
        bool autoPickUp_f = true;
        bool atlook_f = MyPad.IsPressedAnyCross() == false;

        //プレビュー中パス禁止
        bool pabtn_f = MyPad.ButtonA.IsJustPressed;
        bool pabtn2_f = MyPad.ButtonA.IsPressed;
        bool dgbtn_f = MyPad.ButtonA.IsJustPressed;
        bool dgbtn2_f = MyPad.ButtonA.IsPressed;
        bool cabtn_f = MyPad.ButtonA.IsJustPressed;
        bool shbtn_f = MyPad.ButtonB.IsJustPressed;
        bool shbtn2_f = MyPad.ButtonB.IsPressed;
        bool uppos_f = (pabtn_f || dgbtn_f);
        bool dnpos_f = (cabtn_f || shbtn_f);

        bool ng_f = false;

        // DegbugKill();

        // 処理検討
        // if (st_.pstMyCh_.Nomove_f)
        // {
        //     st_.pstMyCh_.Nomove_f = false;
        //     return;
        // }

        //シュートとパスは投げる瞬間にターゲットの方向を自動で向くようにする
        LastMukiKeep_f = true;

        bool shotok_f = true;

        switch (MyState.Motion.MotionType)
        {
            case CharaMotionType.St:
            case CharaMotionType.Wk:
            case CharaMotionType.Ds:
            case CharaMotionType.Sl:
                switch (actionType)
                {
                    case ActionType.ATA:
                        GroundAttack();
                        break;
                    case ActionType.ATD:
                        GroundDefence();
                        break;
                    case ActionType.ATF:
                        GroundFree();
                        break;
                }
                break;
            case CharaMotionType.JUp:
            case CharaMotionType.JDn:
                var canAirAction = MyState.Air.IsAirAction == false
                                   && MyState.Motion.HasFlag(CharaMotionFlag.Ar)
                                   && (MyState.Coordinate.VelocityY > 0 || MyState.Coordinate.Y >= Defines.JPINVALIDHEIGHT);
                var isMarionette = MyState.BallEffect.symCtrl_f && MyState.Motion.HasFlag(CharaMotionFlag.Ar);

                if (canAirAction || isMarionette)
                {
                    if (actionType == ActionType.ATA)
                    {
                        AirAttack();
                    }
                    else
                    {
                        AirDefenceOrFree();
                    }
                }

                break;
            case CharaMotionType.Sh:
                if (IsBallHolder
                    && MyState.Shoot.IsUTurn == false
                    && MyState.Order.IsInfield)
                {
                    bool utrn_f = false;
                    if (MySideIndex == 0
                        && (MyState.Coordinate.DirectionX == DirectionXType.Left)
                        && MyPad.KeyRight.IsPressed)
                    {
                        utrn_f = true;
                        MyState.Coordinate.DirectionX = DirectionXType.Right;
                        MyState.Coordinate.DirectionZ = DirectionZType.Neutral;
                        MyState.Shoot.Angle12 = 1; //1 2 3 4
                        SetMukiAgl(false, true, false, false);
                    }
                    else if (MySideIndex == 1
                             && (MyState.Coordinate.DirectionX == DirectionXType.Right)
                             && MyPad.KeyLeft.IsPressed)
                    {
                        utrn_f = true;
                        MyState.Coordinate.DirectionX = DirectionXType.Left;
                        MyState.Coordinate.DirectionZ = DirectionZType.Neutral;
                        MyState.Shoot.Angle12 = 7; //7 8 9 10
                        SetMukiAgl(true, false, false, false);
                    }

                    if (utrn_f)
                    {
                        SetMotionType(CharaMotionType.RtSh);
                        //ダッシュに復帰
                        if (MyState.Motion.HasFlag(CharaMotionFlag.Slip))
                        {
                            MyState.Motion.AddMotionFlag(CharaMotionFlag.Ds);
                        }
                    }
                }
                break;
            case CharaMotionType.Ca:
                if (IsPickUpPos())
                {
                    HoldBall(false, false);

                    // キャッチダッシュ継続
                    if (MyState.Order.IsInfield
                        && MyState.Auto.AutoType == AutoType.Free
                        && MyState.Motion.HasFlag(CharaMotionFlag.Ds))
                    {
                        if ((MyState.Coordinate.DashDirection == DirectionXType.Left && MyPad.KeyLeft.IsPressed)
                            || (MyState.Coordinate.DashDirection == DirectionXType.Right && MyPad.KeyRight.IsPressed))
                        {
                            //継続なので向きセットも歩数リセットもいらない
                            SetMotionType(CharaMotionType.Ds);
                        }
                    }
                }
                break;
            case CharaMotionType.JCa:
                if (IsPickUpPos())
                {
                    HoldBall(false, false);
                }
                break;
            case CharaMotionType.Dg:
            case CharaMotionType.JDg:
                //押しっぱなしで避け続けるようにする
                if (dgbtn2_f && IsBallHolder == false)
                {
                    // ほかとアニメーション継続の形を合わせる
                    // st_.pstMyCh_.Anime.Ani_c = 0; //こんなんでいいのだろうか
                }
                break;
        }

        //モーション変更前の向き
        if (LastMukiKeep_f)
        {
            MyState.Move.LastDirectionXType = lastMuki;
            MyState.Move.LastDirectionZType = lastMukiZ;
        }
    }

    void AirDefenceOrFree()
    {
        if (dgbtn_f)
        {
            SetMtype(dbmtJDg);
            CatchSE();
            if ((st_.pmgMyTm_.st_.pstMyTm_.CtrlNo == pmgSG_.stBa_.PaTgPNo)
                && (pmgSG_.stBa_.Motion == bmPass)
                && (pmgSG_.stBa_.PaTgTNo == st_.mysideNo_)
                && (pmgSG_.stBa_.PaTgPNo <= (int)dbpoI3))
            {
                st_.pmgMyTm_.SetCtrl(st_.pmgMyTm_.st_.pstMyTm_.CvrNo);
                st_.pmgMyTm_.st_.pmgMyCh_[st_.pmgMyTm_.st_.pstMyTm_.CtrlNo].st_.pstMyCh_.Nomove_f = true;
                st_.pmgMyTm_.SeekCover(st_.pmgMyTm_.st_.pstMyTm_.CtrlNo, pmgSG_.stBa_.PichPNo, pmgSG_.stBa_.PaTgPNo, true); //新しいカバーマン
            }
        }
        else if (cabtn_f)
        {
            //キャッチもボール方向向くようにしてみる
            SetCatchMuki();
            SetMtype(dbmtJCa);
            //                            CatchSE();
            if (IsPickUpPos(true)) BallGet(false, false);
        }
        else if (autoPickUp_f && IsPickUpPos(false)) //自動拾い★
        {
            BallGet(false, false);
        }
    }

    void AirAttack()
    {
        if (pabtn_f)
        {
            Passing(true);
        }
        else if (MyPad.IsJumpShot()) //ジャンプシュート入力
        {
            LookTg(pmgSG_.stBa_.ShTgPNo, false, atlook_f);
            SetMtype(dbmtJSh);
        }
        else
        {
            if (shbtn2_f) //シュート入力おしっぱ
            {
                st_.pstMyCh_.MirPass_c = 0;
            }

            if (st_.pstMyCh_.MirPass_c > 0) //ミラーパス状態
            {
                if (pabtn2_f)
                {
                    if (Defines.UpToR(&st_.pstMyCh_.MirPass_c, MIRWAIT))
                    {
                        if (pmgSG_.stBa_.PaTgPNo != NGNUM)
                        {
                            if (st_.pmgMyTm_.st_.pmgMyCh_[pmgSG_.stBa_.PaTgPNo].IsDashman())
                            {
                                Passing(true);
                            }
                            else
                            {
                                st_.pstMyCh_.MirPass_c = 0;
                            }
                        }
                    }
                }
                else
                {
                    st_.pstMyCh_.MirPass_c = 0;
                }
            }

            if (st_.pstMyCh_.MirShot_c > 0) //ミラーシュート状態
            {
                if (shbtn2_f && (pabtn2_f == false)) //シュート入力おしっぱ
                {
                    if (Defines.UpToR(&st_.pstMyCh_.MirShot_c, MIRWAIT))
                    {
                        LookTg(pmgSG_.stBa_.ShTgPNo, false, true);
                        SetMtype(dbmtJSh);
                    }
                }
                else
                {
                    st_.pstMyCh_.MirShot_c = 1;
                }
            }
        }
    }

    void GroundFree()
    {
        if ((cabtn_f || dgbtn_f) //キャッチ入力
            && (st_.pstMyCh_.CatchW_c == 0))
        {
            //キャッチもボール方向向くようにしてみる
            SetCatchMuki();
            SetMtype(dbmtCa);
        }
        else if (autoPickUp_f && IsPickUpPos(false)) //自動拾い★
        {
            BallGet(false, false);
        }
    }

    void GroundDefence()
    {
        if (dgbtn_f) //避けボタン
        {
            SetMtype(dbmtDg);
            CatchSE();
            if (IsPickUpPos(false)) BallGet(false, false);
        }
        else if (cabtn_f && (st_.pstMyCh_.CatchW_c == 0)) //キャッチ入力
        {
            //キャッチもボール方向向くようにしてみる
            SetCatchMuki();
            SetMtype(dbmtCa);
        }
        else if (autoPickUp_f && IsPickUpPos(false)) //自動拾い★
        {
            BallGet(false, false);
        }
    }

    void GroundAttack()
    {
        if (pabtn_f) //
        {
            //内野の場合後ろ内野内パスなので、相手の方向を向かないと行けない
            Passing(false);
        }

        // DegbugShot();

        if (shbtn_f && shotok_f) //シュート入力
        {
            //外野２３からＺ軸シュートのとき、一応相手の方向を向く
            LookTg(pmgSG_.stBa_.ShTgPNo, false, atlook_f); // && (st_.pstMyCh_.Motion.Mtype == dbmtSt)

            //ダッシュ方向とシュート方向があっているときは振り返り扱いにしない
            bool nortst_f = (st_.pstMyCh_.Motion.IsMFlags(dbmfDs))
                            && (MyState.Coordinate.DsMuki == MyState.Coordinate.Muki);

            //内野で向きに変わるときは振り向きシュート
            if (IsInfield()
                && (nortst_f == false)
                && (lastMuki != MyState.Coordinate.Muki)
               )
            {
                SetMtype(dbmtRtSh);
            }
            else
            {
                SetMtype(dbmtSh);
            }
#ifdef __K_DEBUG_SHIAI__
            // シュート情報のログを書き出す
            kdebug::DebugSystem::GetInstance().CreateShootDebugLog();
            // 自動シュートフラグを落とす
            if (isAutoShot)
            {
                pDs.SetAutoShootStep(kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_SHOOT);
            }
#endif // #ifdef __K_DEBUG_SHIAI__
        }
        else
        {
            if (shbtn2_f) //シュート入力おしっぱ
            {
                st_.pstMyCh_.MirPass_c = 0;
            }

            if (st_.pstMyCh_.MirPass_c > 0) //ミラーパス状態
            {
                if (pabtn2_f)
                {
                    if (Defines.UpToR(&st_.pstMyCh_.MirPass_c, MIRWAIT))
                    {
                        if (pmgSG_.stBa_.PaTgPNo != NGNUM)
                        {
                            if (st_.pmgMyTm_.st_.pmgMyCh_[pmgSG_.stBa_.PaTgPNo].IsDashman())
                            {
                                Passing(false);
                            }
                            else
                            {
                                st_.pstMyCh_.MirPass_c = 0;
                            }
                        }
                    }
                }
                else
                {
                    st_.pstMyCh_.MirPass_c = 0;
                }
            }

            if (st_.pstMyCh_.MirShot_c > 0) //ミラーシュート状態
            {
                if (shbtn2_f && (pabtn2_f == false)) //シュート入力おしっぱ
                {
                    if (Defines.UpToR(&st_.pstMyCh_.MirShot_c, MIRWAIT))
                    {
                        LookTg(pmgSG_.stBa_.ShTgPNo, false, true); //居ないときはオートで探す

                        //ダッシュ方向とシュート方向があっているときは振り返り扱いにしない
                        bool nortst_f = (st_.pstMyCh_.Motion.IsMFlags(dbmfDs))
                                        && (MyState.Coordinate.DsMuki == MyState.Coordinate.Muki);

                        if (IsInfield()
                            && (nortst_f == false)
                            && (lastMuki != MyState.Coordinate.Muki)
                           )
                        {
                            SetMtype(dbmtRtSh);
                        }
                        else
                        {
                            SetMtype(dbmtSh);
                        }
                    }
                }
                else
                {
                    st_.pstMyCh_.MirShot_c = 1;
                }
            }
        }
    }

    //★パス開始処理
    void Passing(bool Jp_f)
    {
        //パスタゲがパス出せないとき
        NGPaTagShift();

        //内野の場合後ろ内野内パスなので、
        //相手の方向を向かないと行けない
        //★ミラーダッシュマンのときはそちらを向かない
        //if (st_.pmgMyTm_.st_.pmgMyCh_[pmgSG_.stBa_.PaTgPNo].IsDashman() == false)
        //{
        LookTg(pmgSG_.stBa_.PaTgPNo, TRUE, false);
        //}

        if (Jp_f)
        {
            SetMtype(dbmtJPa);
        }
        else
        {
            SetMtype(dbmtPa);
        }
    }

    //タゲ方向を向く
    void LookTg(OrderIndexType TgPNo, bool Pa_f, bool AtLook_f)
    {
        TgPNo = 0;

        CharaBehavior TgSt = null;
        bool Notag_f = false;
        int fX, fZ; //SHLAG後の自分の位置

        bool dmPass_f = false;
        bool alleyoop_f = false;

        if (Pa_f)
        {
            if (BallState.PassTargetSide != MySideIndex
                || BallState.PassTargetOrder == OrderIndexType.Disabled)
            {
                Notag_f = true;
            }
            else
            {
                TgSt = CharaBehaviorManager.Instance.GetChara(MySideIndex, BallState.PassTargetOrder);
                dmPass_f = TgSt.IsDashman;
                alleyoop_f = BallState.PaAlPa;
            }
        }
        else
        {
            if (BallState.ShotTargetSide != MySideIndex
                || BallState.ShotTargetOrder == OrderIndexType.Disabled)
            {
                Notag_f = true;
            }
            else //タゲがいる
            {
                TgSt = CharaBehaviorManager.Instance.GetChara(MySideIndex, BallState.ShotTargetOrder);
            }
        }

        //先の位置
        fX = MyState.Coordinate.X + MyState.Coordinate.VelocityX * Defines.SHLAG;
        fZ = MyState.Coordinate.Z + MyState.Coordinate.VelocityZ * Defines.SHLAG;

        //タゲあり
        if (Notag_f == false && TgSt != null)
        {
            bool near_f = ((Math.Abs(TgSt.MyState.Coordinate.X - fX) < Defines.NEARDISTX)
                           && (Math.Abs(TgSt.MyState.Coordinate.Z - fZ) < Defines.NEARDISTZ));

            if ((MyState.Order.IsInfield == false)
                || Pa_f
                || (near_f == false))
            {
                //オート向き初期化
                AutoMukiInit();

                if (lib_num::Sign(TgSt.Coordinate.X - st_.pstMyCh_.Coordinate.X) == lib_num::Sign(TgSt.Coordinate.X - fX))
                {
                    st_.pstMyCh_.Auto.AMuki = (TgSt.Coordinate.X < fX)
                        ? maL
                        : maR;
                }

                //ダッシュマンパス
                if (dmPass_f)
                {
                    st_.pstMyCh_.Auto.AMukiZ = mzaN;
                }
                else
                {
                    //bool Z_f = ((abs(TgSt.Coordinate.Z - fZ) * 2) > abs(TgSt.Coordinate.X - fX));
                    bool Z_f = ((abs(TgSt.Coordinate.Z - fZ)) > abs(TgSt.Coordinate.X - fX)); //★斜めのとき

                    if (TgSt.Coordinate.Z < fZ)
                    {
                        st_.pstMyCh_.Auto.AMukiZ = (Z_f)
                            ? mzaF
                            : mzaN;
                    }
                    else
                    {
                        st_.pstMyCh_.Auto.AMukiZ = (Z_f)
                            ? mzaB
                            : mzaN;
                    }
                }

                MukiSetAuto();
            }
        }
        else //タゲ無し
        {
            if ((IsInfield() == false)
                || (IsSelfCtrl() == false)
                || (AtLook_f && Notag_f))
            {
                //２，３番外野が真横に投げてしまうのを阻止
                switch (st_.posNo_)
                {
                    case (int)dbpoO2:
                        st_.pstMyCh_.Auto.AMukiZ = mzaF;
                        MukiSetAuto();
                        ShTagSet(false);
                        break;
                    case (int)dbpoO3:
                        st_.pstMyCh_.Auto.AMukiZ = mzaB;
                        MukiSetAuto();
                        ShTagSet(false);
                        break;
                    case (int)dbpoO4:
                        st_.pstMyCh_.Auto.AMuki = (st_.mysideNo_ == 0)
                            ? maL
                            : maR;
                        MukiSetAuto();
                        ShTagSet(false);
                        break;
                }

                if (Pa_f == false)
                {
                    //全方向チェック
                    enMukiType lastMuki = st_.pstMyCh_.Coordinate.Muki;
                    enMukiZType lastMukiZ = st_.pstMyCh_.Coordinate.MukiZ;

                    if (IsShTag()) return;

                    //ニュートラルから探す
                    st_.pstMyCh_.Auto.AMukiZ = mzaN;
                    MukiSetAuto();
                    ShTagSet(false);
                    if (IsShTag()) return;

                    st_.pstMyCh_.Auto.AMukiZ = mzaF;
                    MukiSetAuto();
                    ShTagSet(false);
                    if (IsShTag()) return;

                    st_.pstMyCh_.Auto.AMukiZ = mzaB;
                    MukiSetAuto();
                    ShTagSet(false);
                    if (IsShTag()) return;

                    //逆向き
                    st_.pstMyCh_.Auto.AMukiZ = mzaN;
                    st_.pstMyCh_.Auto.AMuki = (st_.pstMyCh_.Coordinate.Muki == mL)
                        ? maR
                        : maL;
                    MukiSetAuto();
                    ShTagSet(false);
                    if (IsShTag()) return;

                    st_.pstMyCh_.Auto.AMukiZ = mzaF;
                    MukiSetAuto();
                    ShTagSet(false);
                    if (IsShTag()) return;

                    st_.pstMyCh_.Auto.AMukiZ = mzaB;
                    MukiSetAuto();
                    ShTagSet(false);
                    if (IsShTag()) return;

                    //みつからなかった
                    lastMuki = st_.pstMyCh_.LastMuki;
                    lastMukiZ = st_.pstMyCh_.LastMukiZ;
                }
            }
        }
    }

    void DegbugKill()
    {
        // #ifdef __K_DEBUG_SHIAI__
//         if (kdebug::DebugSystem::GetInstance().IsEnemyLastOne())
//         {
//             // 今いるやつを全員殺す
//             for (int i = 0; i < Defines.DBMEMBER_INF; i++)
//             {
//                 pmgGO_.pmgCh_[st_.ensideNo_][i].st_.pstMyCh_.ANGEL_f = true;
//                 pmgGO_.pmgCh_[st_.ensideNo_][i].st_.pstMyCh_.HP = 0;
//             }
//             pmgGO_.pmgTm_[st_.ensideNo_].CheckChangePos();
//             // 一人を残して全員殺す
//             for (int i = 0; i < Defines.DBMEMBER_INF - 2; i++)
//             {
//                 pmgGO_.pmgCh_[st_.ensideNo_][i].st_.pstMyCh_.ANGEL_f = true;
//                 pmgGO_.pmgCh_[st_.ensideNo_][i].st_.pstMyCh_.HP = 0;
//             }
//             pmgGO_.pmgTm_[st_.ensideNo_].CheckChangePos();
//         }
// #endif
    }

    void DebugShot()
    {
//         #ifdef __K_DEBUG_SHIAI__
//         kdebug::DebugSystem* pDs = kdebug::DebugSystem::GetInstance();
//         bool isAutoShot = false;
//         // 自動シュート状態を取得
//         int step = pDs.GetAutoShootStep();
//         if (step == kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_STANDBY)
//         {
//             // この状態で手元にボールがないのがおかしいので
//             // 強制的に手元に戻す
//             // なのでこのif文内は例外処理
//             if ((pmgSG_.stBa_.Motion != bmHold)
//                 || (pmgSG_.stBa_.HoldTNo != 0)
//                 || (pmgSG_.stBa_.HoldPNo != 0))
//             {
//                 // パスできる状態になったら手元に戻す
//                 if (!IsNGPassTag(0))
//                 {
//                     // ボールが手元に帰ってくる処理
//                     pmgGO_.pmgBa_.baCommon_.ResetRefPos_Prev(false);
//                 }
//                 return;
//             }
//             else
//             {
//                 // 操作キャラをボール持ってるやつに
//                 st_.pmgMyTm_.SetCtrlBallGet(0);
//             }
//
//             // 自動シュート終わりますよフラグが立っていれば投げない
//             if (!pDs.IsAutoShootEnd())
//             {
//                 isAutoShot = true;
//             }
//         }
//         else
//         {
//             // もしもボールを自分が持っていて待っている状態であれば次のシーケンスに進めてしまう
//             if (pDs.IsReturnBall())
//             {
//                 // 自動シュート状態を取得
//                 if (step == kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_WAIT)
//                 {
//                     // すでに手に持っているので次のステートに移行する
//                     if ((pmgSG_.stBa_.Motion == bmHold)
//                         && (pmgSG_.stBa_.HoldTNo == 0)
//                         && (pmgSG_.stBa_.HoldPNo == 0))
//                     {
//                         pDs.SetAutoShootStep(kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_CHOOSE);
//                         pDs.SetReturnBallFlg(false); // フラグを落とす
//                     }
//                     // 他人が持ってる
//                     else
//                     {
//                         // パスできる状態になったら手元に戻す
//                         if (!IsNGPassTag(0))
//                         {
//                             // ボールが手元に帰ってくる処理
//                             pmgGO_.pmgBa_.baCommon_.ResetRefPos_Prev(false);
//                             return;
//                         }
//                     }
//                 }
//             }
//         }
// #endif // #ifdef __K_DEBUG_SHIAI__
    }
}