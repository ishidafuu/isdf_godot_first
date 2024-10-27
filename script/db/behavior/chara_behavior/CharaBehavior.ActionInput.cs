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
        if (IsSelfControl == false && IsCom)
        {
            return;
        }

        bool muki_f = ((MySideIndex == 0) && (MyCoordinate.DirectionX == DirectionXType.Left))
                      || ((MySideIndex == 1) && (MyCoordinate.DirectionX == DirectionXType.Right));

        // 敵コート避け可能フラグ
        var canEnemyCourtDodge = GetCanDodgeEnemyCourt();

        switch (MyMotion.MotionType)
        {
            case CharaMotionType.St:
            case CharaMotionType.Wk:
            case CharaMotionType.Ds:
            case CharaMotionType.Sl:
                //スリップも追加オーバーラインの瞬間なぞのキャッチできてしまう
                if (MyCourt.ECDdg_f == false && canEnemyCourtDodge)
                {
                    if (MyPad.IsJustPressedAbButton() //ジャンプ入力
                        && muki_f
                        && (MyCourt.ECDjp_f == false))
                    {
                        JumpSet(false, false, false); //ジャンプ
                        MyCourt.ECDdg_f = true; //１回だけ
                        MyCourt.ECDjp_f = true; //１回だけ
                    }
                    else if (MyPad.ButtonA.IsJustPressed) //よけ
                    {
                        SetMotionType(CharaMotionType.Dg);
                        MyCourt.ECDdg_f = true; //１回だけ
                    }

                    if (MyCourt.ECDdg_f)
                    {
                        MyMove.LastDirectionX = MyCoordinate.DirectionX;
                        MyMove.LastDirectionZ = MyCoordinate.DirectionZ;
                    }
                }

                break;
            case CharaMotionType.Sh:
            case CharaMotionType.Pa:
                if (canEnemyCourtDodge)
                {
                    CanselJump(false);
                }
                break;
            case CharaMotionType.Ca:
            case CharaMotionType.JCa:
                if (MyCourt.ECDjp_f == false && canEnemyCourtDodge)
                {
                    if (CanselJump(false) && muki_f)
                    {
                        MyCourt.ECDjp_f = true; //１回だけ
                        MyCourt.ECDdg_f = true; //１回だけ
                    }
                    else
                    {
                        AutoPickUp();
                    }
                }
                break;
            case CharaMotionType.Dg:
                if (MyCourt.ECDdg_f == false && canEnemyCourtDodge)
                {
                    if (CanselJump(false) && muki_f)
                    {
                        MyCourt.ECDjp_f = true; //１回だけ
                        MyCourt.ECDdg_f = true; //１回だけ
                    }
                    else
                    {
                        //よけ限界時間
                        MyCourt.EnemyCortDodgeCount.Add();
                        //押しっぱなしで避け続けるようにする
                        if (MyPad.ButtonA.IsPressed
                            && MyCourt.EnemyCortDodgeCount.Value < GetSettingInfield(SettingInfieldType.EnCourtCrTime))
                        {
                            // 他と合わせる
                            // MyAnime.Ani_c = 0;
                        }
                    }
                }
                break;
        }
    }

    //キャンセルジャンプ
    bool CanselJump(bool canselDs_f)
    {
        //判定出るまでもしくは6f以内はジャンプキャンセル可能
        //ココでも可能にして大丈夫か

        if (MyMotion.HasFlag(CharaMotionFlag.Ar)
            || MyMotion.MotionCount.Value >= GetSettingJump(SettingJumpType.JumpCanselTime)
            || !IsSelfControl
            || MyPad.IsJustPressedAbButton() == false)
        {
            return false;
        }

        //キャンセルが掛かる前のモーションの向きに戻す
        MyCoordinate.DirectionX = MyMove.LastDirectionX;
        MyCoordinate.DirectionZ = MyMove.LastDirectionZ;
        JumpSet(false, canselDs_f, false); //ジャンプ

        return true;
    }

    //ジャンプ
    void JumpSet(bool mfCheck_f, bool canselDs_f, bool vjp_f)
    {
        if (MyMotion.HasFlag(CharaMotionFlag.PHit))
        {
            return;
        }

        if (mfCheck_f && MyMotion.HasFlag(CharaMotionFlag.JpOK) == false)
        {
            return;
        }

        if (canselDs_f || MyOrder.IsInfield == false) //外野はダッシュフラグ消す
        {
            MyMotion.SubMotionFlag(CharaMotionFlag.Ds);
        }

        SetMotionType(CharaMotionType.JCr);
        MyAir.IsVerticalJump = vjp_f; //垂直ジャンプ
        MyAir.IsLongJump = vjp_f; //垂直ジャンプ
        MyAir.IsLongJump = MyMove.IsDashAccelIOS; //ロングジャンプ
        MyCoordinate.ZeroVelocity();
    }

    private void AutoDodgeAction()
    {
        // 敵コート避け可能フラグ
        var canEnemyCourtDodge = GetCanDodgeEnemyCourt();

        //COMの敵コートオートよけ
        if (IsSelfControl == false
            && BallState.MotionType == BallMotionType.Shoot
            && canEnemyCourtDodge)
        {
            switch (MyMotion.MotionType)
            {
                case CharaMotionType.St:
                case CharaMotionType.Wk:
                case CharaMotionType.Ds:
                    if (MyCourt.ECDdg_f == false)
                    {
                        SetMotionType(CharaMotionType.Dg);
                    }
                    break;

                case CharaMotionType.Dg:
                    if (MyCourt.ECDdg_f == false)
                    {
                        //よけ限界時間
                        MyCourt.EnemyCortDodgeCount.Add();
                        //押しっぱなしで避け続けるようにする
                        if (MyPad.ButtonA.IsPressed
                            && MyCourt.EnemyCortDodgeCount.Value < GetSettingInfield(SettingInfieldType.EnCourtCrTime))
                        {
                            // 他と合わせる
                            // MyAnime.Ani_c = 0;
                        }
                    }
                    break;
            }
        }
        else
        {
            switch (MyMotion.MotionType)
            {
                case CharaMotionType.St:
                case CharaMotionType.Wk:
                case CharaMotionType.Ds:
                    SetMotionType(CharaMotionType.Dg);
                    break;

                case CharaMotionType.Dg:
                    // 他と合わせる
                    // 避け続ける
                    // MyAnime.Ani_c = 0;
                    break;
            }
        }
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
        bool result;
        if (IsBallHolder || MyState.Order.IsOutfield)
        {
            // ボール持ち、外野はよけNG
            result = false;
        }
        else if (IsSelfControl || MyState.Auto.IsFreeAction)
        {
            // 自由状態
            result = true;
        }
        else if (BallState.MotionType == BallMotionType.Hold)
        {
            // 敵がボール保持
            result = BallState.HolderSide == EnemySideIndex;
        }
        else if (BallState.MotionType == BallMotionType.Shoot)
        {
            // 味方がターゲットのシュート
            result = BallState.ShotTargetSide == MySideIndex;
        }
        else
        {
            // 敵ボール
            result = BallState.TimerSide == EnemySideIndex;
        }
        return result;
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
        var isHitDepth = Math.Abs(BallState.Coordinate.Z - MyCoordinate.Z) <= atariDepth;
        return isHitDepth && MyCoordinate.HitBox.IsPile(BallState.Atari);
    }

    //ボール持った処理
    private void HoldBall(bool isNoSe, bool isLookBall)
    {
        // 保持状況をチームに渡す
        CallTeamHoldBall();

        MyState.Damage.KagamiCount.Clear();
        MyState.Air.IsAirAction = false;
        MyState.Move.LastDirectionX = DirectionXType.Neutral;
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
        MyState.Shoot.Angle12 = GetMukiAglFromDirection();

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
            var orderIndex = GetShootTarget(MyState.Shoot.Angle12, false);
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
            var enemyControlOrderIndex = GetShootTarget(MyState.Shoot.Angle12, true);
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
            var enemyChara = CharaBehaviorManager.Instance.GetChara(EnemySideIndex, order);

            if (enemyChara.IsEnableShootTarget == false)
            {
                continue;
            }

            isSelectTarget[order] = enNaiyaTag.TGOK;
            targetX[order] = enemyChara.MyCoordinate.X - MyCoordinate.X;
            var targetZ = enemyChara.MyCoordinate.Z - MyCoordinate.Z;
            targetY[order] = enemyChara.MyCoordinate.Y - MyCoordinate.Y;
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

        var passDirectionX = MyCoordinate.DirectionX;
        var passDirectionZ = MyCoordinate.DirectionZ;

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

        var distO2 = Math.Abs(MyCoordinate.Z - Defines.DBCRT_BL);
        var distO3 = Math.Abs(MyCoordinate.Z - Defines.DBCRT_FL);
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

            targetDist[order] = MyCoordinate.DistanceXZ(MySideOrders[order].Coordinate);
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
                if (chara.MyCoordinate.Z > MyCoordinate.Z)
                {
                    isBackwardPosition = false;
                }

                //手前にダッシュマンがいる
                if (chara.MyCoordinate.Z < MyCoordinate.Z)
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

                if (IsCheckNoAgl(chara.MyCoordinate.X, chara.MyCoordinate.Z))
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
                if (chara.MyCoordinate.Z > MyCoordinate.Z)
                {
                    isBackwardPosition = false;
                }

                //手前にいる
                if (chara.MyCoordinate.Z < MyCoordinate.Z)
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
                    : chara.MyCoordinate.Z;

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
                    sortValue[order] += chara.MyCoordinate.X; //Ｘ（左ほど優先）
                }
                else if (isRightKey) //右
                {
                    sortValue[order] -= chara.MyCoordinate.X; //Ｘのマイナス（右ほど優先）
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

        var passDirectionX = MyCoordinate.DirectionX;
        var passDirectionZ = MyCoordinate.DirectionZ;

        var lastXKey = MyState.Input.LastXKey;

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
                            var distO2 = Math.Abs(MyCoordinate.Z - Defines.DBCRT_BL);
                            var distO3 = Math.Abs(MyCoordinate.Z - Defines.DBCRT_FL);

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
            var targetZ = Math.Abs(chara.MyCoordinate.Z - MyCoordinate.Z); //自分より上にいれば＋
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
                //     : chara.MyCoordinate.Z;
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
                    sortValue[order] += chara.MyCoordinate.X; //Ｘ（左ほど優先）
                }
                else if (isRightKey) //右
                {
                    sortValue[order] -= chara.MyCoordinate.X; //Ｘのマイナス（右ほど優先）
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
        var isInAngle = MyCoordinate.DirectionZ switch
        {
            DirectionZType.Forward => targetZ < MyCoordinate.Z,
            DirectionZType.Neutral => true,
            DirectionZType.Backward => targetZ > MyCoordinate.Z,
            _ => throw new ArgumentOutOfRangeException()
        };

        if (isInAngle)
        {
            isInAngle = MyCoordinate.DirectionX switch
            {
                DirectionXType.Left => targetX < MyCoordinate.X,
                DirectionXType.Neutral => true,
                DirectionXType.Right => targetX > MyCoordinate.X,
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
        var lastDirectionX = MyCoordinate.DirectionX;
        var lastDirectionZ = MyCoordinate.DirectionZ;

        MyMove.LastDirectionX = lastDirectionX;
        MyMove.LastDirectionZ = lastDirectionZ;

        MyCoordinate.DirectionX = MyState.Auto.DirectionX switch
        {
            DirectionXType.Left => DirectionXType.Left,
            DirectionXType.Right => DirectionXType.Right,
            _ => MyCoordinate.DirectionX
        };

        MyCoordinate.DirectionZ = MyState.Auto.DirectionZ switch
        {
            DirectionZType.Forward => DirectionZType.Forward,
            DirectionZType.Backward => DirectionZType.Backward,
            DirectionZType.Neutral => DirectionZType.Neutral,
            _ => MyCoordinate.DirectionZ
        };

        // 向き変えありかどうか
        var isChanged = lastDirectionX != MyCoordinate.DirectionX
                        || lastDirectionZ != MyCoordinate.DirectionZ;

        if (isChanged)
        {
            //タゲも変える
            MyState.Shoot.Angle12 = GetMukiAglFromDirection();
        }

        return isChanged;
    }

    // AUTO向き変え初期化
    private void ResetAutoDirection()
    {
        MyState.Auto.DirectionX = MyCoordinate.DirectionX switch
        {
            DirectionXType.Left => DirectionXType.Left,
            DirectionXType.Neutral => DirectionXType.Neutral,
            DirectionXType.Right => DirectionXType.Right,
            _ => throw new ArgumentOutOfRangeException()
        };
        MyState.Auto.DirectionZ = MyCoordinate.DirectionZ switch
        {
            DirectionZType.Forward => DirectionZType.Forward,
            DirectionZType.Neutral => DirectionZType.Neutral,
            DirectionZType.Backward => DirectionZType.Backward,
            _ => throw new ArgumentOutOfRangeException()
        };
    }

    private int GetMukiAglFromDirection()
    {
        return GetMukiAgl(MyCoordinate.DirectionX, MyCoordinate.DirectionZ);
    }

    private int GetMukiAgl(DirectionXType directionX, DirectionZType directionZ)
    {
        return GetMukiAgl(directionX == DirectionXType.Left,
            directionX == DirectionXType.Right,
            directionZ == DirectionZType.Backward,
            directionZ == DirectionZType.Forward);
    }

    //ターゲッティング用向き
    private int GetMukiAgl(bool isLeft, bool isRight, bool isUp, bool isDown)
    {
        switch (MyState.Order.GetOrderFieldType())
        {
            case OrderFieldType.Infield:
                if (isRight)
                {
                    if (isUp)
                    {
                        return 11; //11 0 1 2
                    }
                    if (isDown)
                    {
                        return 3; //3 4 5 6
                    }
                    return 1; //1 2 3 4
                }

                if (isLeft)
                {
                    if (isUp)
                    {
                        return 9; //9 10 11 0
                    }
                    if (isDown)
                    {
                        return 5; //5 6 7 8
                    }
                    return 7; //7 8 9 10
                }
                break;
            case OrderFieldType.Outfield2:
                if (isLeft)
                {
                    return 5; //5678
                }
                if (isRight)
                {
                    return 3; //3456
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
                        return 9; //9 10 11 0
                    }
                    if (isDown)
                    {
                        return 5; //5 6 7 8
                    }
                    return 7; //7 8 9 10
                }
                if (isUp)
                {
                    return 11; //11 0 1 2
                }
                if (isDown)
                {
                    return 3; //3 4 5 6
                }
                return 1; //1 2 3 4
            default:
                throw new ArgumentOutOfRangeException();
        }

        // 来ない想定
        return 0;
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
            //&& ((st_.pmgMyTm_.st_.pmgMyCh_[pos].MyCoordinate.dY >= (-XYMAG))
            //  || (MyMirPass_c > 0)))//下降ではない
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

        //モーション変更前の向き
        var lastMuki = MyCoordinate.DirectionX;
        var lastMukiZ = MyCoordinate.DirectionZ;
        bool LastMukiKeep_f = false;
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
        // if (MyNomove_f)
        // {
        //     MyNomove_f = false;
        //     return;
        // }

        //シュートとパスは投げる瞬間にターゲットの方向を自動で向くようにする
        LastMukiKeep_f = true;

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
                                   && (MyCoordinate.VelocityY > 0 || MyCoordinate.Y >= Defines.JPINVALIDHEIGHT);
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
                        && (MyCoordinate.DirectionX == DirectionXType.Left)
                        && MyPad.KeyRight.IsPressed)
                    {
                        utrn_f = true;
                        MyCoordinate.DirectionX = DirectionXType.Right;
                        MyCoordinate.DirectionZ = DirectionZType.Neutral;
                        MyState.Shoot.Angle12 = GetMukiAglFromDirection();
                    }
                    else if (MySideIndex == 1
                             && (MyCoordinate.DirectionX == DirectionXType.Right)
                             && MyPad.KeyLeft.IsPressed)
                    {
                        utrn_f = true;
                        MyCoordinate.DirectionX = DirectionXType.Left;
                        MyCoordinate.DirectionZ = DirectionZType.Neutral;
                        MyState.Shoot.Angle12 = GetMukiAglFromDirection();
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
                        if ((MyCoordinate.DashDirection == DirectionXType.Left && MyPad.KeyLeft.IsPressed)
                            || (MyCoordinate.DashDirection == DirectionXType.Right && MyPad.KeyRight.IsPressed))
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
                    // MyAnime.Ani_c = 0; //こんなんでいいのだろうか
                }
                break;
        }

        //モーション変更前の向き
        if (LastMukiKeep_f)
        {
            MyState.Move.LastDirectionX = lastMuki;
            MyState.Move.LastDirectionZ = lastMukiZ;
        }
    }

    void AirDefenceOrFree()
    {
        if (MyPad.ButtonA.IsJustPressed)
        {
            SetMotionType(CharaMotionType.Dg);
            // キャッチ時間で変えるとよいかも
            PlaySeCatchSe();

            // パスをスルーしてカバーマンに操作権を渡す
            if (BallState.MotionType == BallMotionType.Pass
                && BallState.PassTargetSide == MySideIndex
                && MyTeamState.MainState.ControlOrderIndex == BallState.PassTargetOrder
                && BallState.PassTargetOrder >= OrderIndexType.Infield0
                && BallState.PassTargetOrder <= OrderIndexType.Infield3)
            {
                CallTeamChangeControlCoverMan();
            }
        }
        else if (MyPad.ButtonB.IsJustPressed)
        {
            SetCatchMuki();
            SetMotionType(CharaMotionType.JCa);
            PlaySeCatchSe();
        }
        else if (IsPickUpPos()) //自動拾い★
        {
            HoldBall(false, false);
        }
    }

    void SetCatchMuki()
    {
        AutoMukiInit();
        SetBallMukiX();
        SetBallMukiZ();
        MukiSetAuto();
    }

    void SetBallMukiX()
    {
        if (IsBallHolder)
        {
            return;
        }

        //自分がタゲでパスが飛んできてるときはそのままの向き自分が投げたときも
        if (MyOrder.IsInfield == false
            && BallState.MotionType == BallMotionType.Pass
            && BallState.ThrowerSideNo == MySideIndex
            && BallState.ThrowerOrderNo == MyOrderIndex)
        {
            return;
        }

        if (BallState.Coordinate.X < MyCoordinate.X)
        {
            MyNextAuto.DirectionX = DirectionXType.Left;
        }
        else if (BallState.Coordinate.X < MyCoordinate.X)
        {
            MyNextAuto.DirectionX = DirectionXType.Right;
        }
    }

    void SetBallMukiZ()
    {
        if (IsBallHolder)
        {
            return;
        }

        // 自分がタゲでパスが飛んできてるときはそのままの向き
        // 自分が投げたときも
        if (MyOrder.IsInfield == false && BallState.MotionType == BallMotionType.Pass)
        {
            if (BallState.ThrowerSideNo == MySideIndex
                && BallState.ThrowerOrderNo == MyOrderIndex)
            {
                return;
            }
            // パスの時はパス先を向く
            MyNextAuto.DirectionZ = BallState.ThrowerOrderNo switch
            {
                OrderIndexType.Outfield2 => DirectionZType.Backward,
                OrderIndexType.Outfield3 => DirectionZType.Forward,
                _ => DirectionZType.Neutral
            };
        }
        else
        {
            if (BallState.Coordinate.Z < MyCoordinate.Z)
            {
                if (BallState.Coordinate.Z < MyCoordinate.Z - Defines.DEFDISTZ)
                {
                    MyNextAuto.DirectionZ = DirectionZType.Forward;
                }
                else if (BallState.Coordinate.Z > MyCoordinate.Z - Defines.DEFDISTZ / 2
                         || MyNextAuto.DirectionZ == DirectionZType.Backward)
                {
                    MyNextAuto.DirectionZ = DirectionZType.Neutral;
                }
            }
            else
            {
                if (BallState.Coordinate.Z > MyCoordinate.Z + Defines.DEFDISTZ)
                {
                    MyNextAuto.DirectionZ = DirectionZType.Backward;
                }
                else if (BallState.Coordinate.Z < MyCoordinate.Z + Defines.DEFDISTZ / 2
                         || MyNextAuto.DirectionZ == DirectionZType.Forward)
                {
                    MyNextAuto.DirectionZ = DirectionZType.Neutral;
                }
            }
        }
    }

    //AUTO向き変え初期化
    void AutoMukiInit()
    {
        MyAuto.DirectionX = MyCoordinate.DirectionX;
        MyAuto.DirectionZ = MyCoordinate.DirectionZ;
    }

    void AirAttack()
    {
        if (MyPad.ButtonA.IsJustPressed)
        {
            Passing();
        }
        else if (MyPad.ButtonB.IsJustPressed)
        {
            bool atlook_f = MyPad.IsPressedAnyCross() == false;
            LookTg(BallState.ShotTargetOrder, false, atlook_f);
            SetMotionType(CharaMotionType.JSh);
        }
        else
        {
            MirrorAttack();
        }
    }

    private void MirrorAttack()
    {
        if (MyPad.ButtonB.IsPressed) //シュート入力おしっぱ
        {
            MyPass.MirrorPassCount.Clear();
        }

        if (MyPass.MirrorPassCount.Value > 0) //ミラーパス状態
        {
            if (MyPad.ButtonA.IsPressed)
            {
                if (MyPass.MirrorPassCount.AddUntil(Defines.MIRWAIT))
                {
                    if (BallState.PassTargetOrder != OrderIndexType.Disabled)
                    {
                        var chara = CharaBehaviorManager.Instance.GetChara(MySideIndex, BallState.PassTargetOrder);
                        if (chara.IsDashman)
                        {
                            Passing();
                        }
                        else
                        {
                            MyPass.MirrorPassCount.Clear();
                        }
                    }
                }
            }
            else
            {
                MyPass.MirrorPassCount.Clear();
            }
        }

        if (MyPass.MirrorShotCount.Value > 0) //ミラーシュート状態
        {
            if (MyPad.ButtonB.IsPressed && MyPad.ButtonA.IsPressed == false) //シュート入力おしっぱ
            {
                if (MyPass.MirrorPassCount.AddUntil(Defines.MIRWAIT))
                {
                    LookTg(BallState.ShotTargetOrder, false, true);
                    Shooting();
                }
            }
            else
            {
                MyPass.MirrorShotCount.Set(1);
            }
        }
    }

    void GroundFree()
    {
        if (MyPad.IsJustPressedAnyButton() && MyCatch.CatchWaitCount.Value == 0)
        {
            //キャッチもボール方向向くようにしてみる
            SetCatchMuki();
            SetMotionType(CharaMotionType.Ca);
        }
        else if (IsPickUpPos()) //自動拾い★
        {
            HoldBall(false, false);
        }
    }

    void GroundDefence()
    {
        if (MyPad.ButtonA.IsJustPressed) //避けボタン
        {
            SetMotionType(CharaMotionType.Dg);
            PlaySeCatchSe();
        }
        else if (MyPad.ButtonB.IsJustPressed && MyCatch.CatchWaitCount.Value == 0) //キャッチ入力
        {
            //キャッチもボール方向向くようにしてみる
            SetCatchMuki();
            SetMotionType(CharaMotionType.Ca);
        }
        else if (IsPickUpPos()) //自動拾い★
        {
            HoldBall(false, false);
        }
    }

    void GroundAttack()
    {
        if (MyPad.ButtonA.IsJustPressed)
        {
            //内野の場合後ろ内野内パスなので、相手の方向を向かないと行けない
            Passing();
        }

        // DegbugShot();

        if (MyPad.ButtonB.IsJustPressed) //シュート入力
        {
            bool atlook_f = MyPad.IsPressedAnyCross() == false;
            //外野２３からＺ軸シュートのとき、一応相手の方向を向く
            LookTg(BallState.ShotTargetOrder, false, atlook_f);

            Shooting();

            // #ifdef __K_DEBUG_SHIAI__
//             // シュート情報のログを書き出す
//             kdebug::DebugSystem::GetInstance().CreateShootDebugLog();
//             // 自動シュートフラグを落とす
//             if (isAutoShot)
//             {
//                 pDs.SetAutoShootStep(kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_SHOOT);
//             }
// #endif // #ifdef __K_DEBUG_SHIAI__
        }
        else
        {
            MirrorAttack();
        }
    }

    private void Shooting()
    {
        //ダッシュ方向とシュート方向があっているときは振り返り扱いにしない
        bool nortst_f = MyMotion.HasFlag(CharaMotionFlag.Ds)
                        && MyCoordinate.DashDirection == MyCoordinate.DirectionX;

        //内野で向きに変わるときは振り向きシュート
        bool isReturnShot = MyOrder.IsInfield
                            && nortst_f == false
                            && MyMove.LastDirectionX != MyCoordinate.DirectionX;

        CharaMotionType shootMotion;

        if (MyMotion.HasFlag(CharaMotionFlag.Ar))
        {
            shootMotion = CharaMotionType.JSh;
        }
        else
        {
            shootMotion = isReturnShot ? CharaMotionType.RtSh : CharaMotionType.Sh;
        }

        SetMotionType(shootMotion);
    }

    //★パス開始処理
    void Passing()
    {
        //パスタゲがパス出せないとき
        NGPaTagShift();

        //内野の場合後ろ内野内パスなので、
        //相手の方向を向かないと行けない
        //★ミラーダッシュマンのときはそちらを向かない
        //if (st_.pmgMyTm_.st_.pmgMyCh_[pmgSG_.stBa_.PaTgPNo].IsDashman() == false)
        //{
        LookTg(BallState.PassTargetOrder, true, false);
        //}

        CharaMotionType passMotion = MyMotion.HasFlag(CharaMotionFlag.Ar)
            ? CharaMotionType.JPa
            : CharaMotionType.Pa;
        SetMotionType(passMotion);
    }

    //タゲ方向を向く
    void
        LookTg(OrderIndexType TgPNo, bool Pa_f, bool AtLook_f)
    {
        TgPNo = 0;

        CharaBehavior TgSt = null;
        bool Notag_f = false;
        int futureX, futureZ; //SHLAG後の自分の位置

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

        // シュートタイミングの位置
        futureX = MyCoordinate.X + MyCoordinate.VelocityX * Defines.SHLAG;
        futureZ = MyCoordinate.Z + MyCoordinate.VelocityZ * Defines.SHLAG;

        //タゲあり
        if (Notag_f == false && TgSt != null)
        {
            bool near_f = Math.Abs(TgSt.MyCoordinate.X - futureX) < Defines.NEARDISTX
                          && Math.Abs(TgSt.MyCoordinate.Z - futureZ) < Defines.NEARDISTZ;

            if (MyState.Order.IsInfield == false
                || Pa_f
                || near_f == false)
            {
                //オート向き初期化
                AutoMukiInit();

                var nowDist = TgSt.MyCoordinate.X - MyCoordinate.X;
                var futureDist = TgSt.MyCoordinate.X - futureX;
                var isSameDirection = nowDist * futureDist > 0;

                if (isSameDirection)
                {
                    MyAuto.DirectionX = (TgSt.MyCoordinate.X < futureX)
                        ? DirectionXType.Left
                        : DirectionXType.Right;
                }

                //ダッシュマンパス
                if (dmPass_f)
                {
                    MyAuto.DirectionZ = DirectionZType.Neutral;
                }
                else
                {
                    bool Z_f = Math.Abs(TgSt.MyCoordinate.Z - futureZ) > Math.Abs(TgSt.MyCoordinate.X - futureX); //★斜めのとき

                    if (Z_f)
                    {
                        MyAuto.DirectionZ = TgSt.MyCoordinate.Z < futureZ
                            ? DirectionZType.Forward
                            : DirectionZType.Backward;
                    }
                    else
                    {
                        MyAuto.DirectionZ = DirectionZType.Neutral;
                    }
                }

                MukiSetAuto();
            }
        }
        else //タゲ無し
        {
            if (MyOrder.IsInfield == false
                || IsSelfControl == false
                || (AtLook_f && Notag_f))
            {
                //２，３番外野が真横に投げてしまうのを阻止
                switch (MyOrderIndex)
                {
                    case OrderIndexType.Outfield2:
                        MyAuto.DirectionZ = DirectionZType.Forward;
                        MukiSetAuto();
                        SetShTagFromMyShootAngle12(false);
                        break;
                    case OrderIndexType.Outfield3:
                        MyAuto.DirectionZ = DirectionZType.Backward;
                        MukiSetAuto();
                        SetShTagFromMyShootAngle12(false);
                        break;
                    case OrderIndexType.Outfield4:
                        MyAuto.DirectionX = MySideIndex == 0
                            ? DirectionXType.Left
                            : DirectionXType.Right;
                        MukiSetAuto();
                        SetShTagFromMyShootAngle12(false);
                        break;
                }

                if (Pa_f == false && IsShTag() == false)
                {
                    //全方向チェック
                    var directionX = MyCoordinate.DirectionX;
                    //ニュートラルから探す
                    var directionZ = DirectionZType.Neutral;
                    if (SearchShootTarget(directionX, directionZ))
                    {
                        SetAutoTarget(directionX, directionZ);
                        return;
                    }

                    directionZ = DirectionZType.Forward;
                    if (SearchShootTarget(directionX, directionZ))
                    {
                        SetAutoTarget(directionX, directionZ);
                        return;
                    }

                    directionZ = DirectionZType.Backward;
                    if (SearchShootTarget(directionX, directionZ))
                    {
                        SetAutoTarget(directionX, directionZ);
                        return;
                    }

                    //逆向き
                    directionX = MyCoordinate.DirectionX == DirectionXType.Left
                        ? DirectionXType.Right
                        : DirectionXType.Left;

                    //ニュートラルから探す
                    directionZ = DirectionZType.Neutral;
                    if (SearchShootTarget(directionX, directionZ))
                    {
                        SetAutoTarget(directionX, directionZ);
                        return;
                    }

                    directionZ = DirectionZType.Forward;
                    if (SearchShootTarget(directionX, directionZ))
                    {
                        SetAutoTarget(directionX, directionZ);
                        return;
                    }

                    directionZ = DirectionZType.Backward;
                    if (SearchShootTarget(directionX, directionZ))
                    {
                        SetAutoTarget(directionX, directionZ);
                        return;
                    }
                }
            }
        }
    }

    private bool SearchShootTarget(DirectionXType directionX, DirectionZType directionZ)
    {
        var angle12 = GetMukiAgl(directionX, directionZ);
        return (GetShootTarget(angle12, false) != OrderIndexType.Disabled);
    }

    private void SetAutoTarget(DirectionXType directionX, DirectionZType directionZ)
    {
        MyState.Auto.DirectionX = directionX;
        MyState.Auto.DirectionZ = directionZ;
        MukiSetAuto();
        SetShTagFromMyShootAngle12(false);
    }

    bool IsShTag()
    {
        return BallState.ShotTargetSide != MySideIndex
               || BallState.ShotTargetOrder == OrderIndexType.Disabled;
    }

    //シュートタゲセット
    void SetShTagFromMyShootAngle12(bool isIgnoreDirection)
    {
        CallBallChangeShootTarget(GetShootTarget(MyShoot.Angle12, isIgnoreDirection));
    }

    void DegbugKill()
    {
        // #ifdef __K_DEBUG_SHIAI__
//         if (kdebug::DebugSystem::GetInstance().IsEnemyLastOne())
//         {
//             // 今いるやつを全員殺す
//             for (int i = 0; i < Defines.DBMEMBER_INF; i++)
//             {
//                 pmgGO_.pmgCh_[st_.ensideNo_][i].MyANGEL_f = true;
//                 pmgGO_.pmgCh_[st_.ensideNo_][i].MyHP = 0;
//             }
//             pmgGO_.pmgTm_[st_.ensideNo_].CheckChangePos();
//             // 一人を残して全員殺す
//             for (int i = 0; i < Defines.DBMEMBER_INF - 2; i++)
//             {
//                 pmgGO_.pmgCh_[st_.ensideNo_][i].MyANGEL_f = true;
//                 pmgGO_.pmgCh_[st_.ensideNo_][i].MyHP = 0;
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