using System;
using isdf;

namespace db;

public partial class CharaBehavior
{
    /// <summary>
    /// アクション入力の更新処理を行います
    /// 試合中のキャラクターの入力処理を管理し、状態に応じて適切な行動を実行します
    /// </summary>
    public void UpdateActionInput()
    {
        if (Referee.Main.ShiaiPahse != ShiaiPhase.Shiai
            || Referee.Main.IsGameSet)
        {
            return;
        }

        if (Motion.HasFlag(CharaMotionFlag.Dam)
            || Motion.HasFlag(CharaMotionFlag.ANG)
            || Live.IsDead)
        {
            return;
        }

        if (MyTeam.Main.IsCancelActionInput)
        {
            return;
        }

        switch (Auto.AutoType)
        {
            // 自由
            case AutoType.Free:
                if (Composite.IsSelfControl)
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
                if (Motion.MotionType
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


    /// <summary>
    /// 自動復帰行動の処理を行います
    /// キャラクターが自動的にコートに戻る際の行動を制御します
    /// </summary>
    private void AutoReturnAction()
    {
        if (Composite.IsSelfControl == false && Composite.IsCom)
        {
            return;
        }

        bool muki_f = (MySideIndex == 0 && Coordinate.DirectionX == DirectionXType.Left)
                      || (MySideIndex == 1 && Coordinate.DirectionX == DirectionXType.Right);

        // 敵コート避け可能フラグ
        var canEnemyCourtDodge = GetCanDodgeEnemyCourt();

        switch (Motion.MotionType)
        {
            case CharaMotionType.St:
            case CharaMotionType.Wk:
            case CharaMotionType.Ds:
            case CharaMotionType.Sl:
                //スリップも追加オーバーラインの瞬間なぞのキャッチできてしまう
                if (Court.ECDdg_f == false && canEnemyCourtDodge)
                {
                    if (Pad.IsJustPressedAbButton() //ジャンプ入力
                        && muki_f
                        && Court.ECDjp_f == false)
                    {
                        JumpSet(false, false, false); //ジャンプ
                        CourtSet.ECDdg_f = true; //１回だけ
                        CourtSet.ECDjp_f = true; //１回だけ
                    }
                    else if (Pad.ButtonA.IsJustPressed) //よけ
                    {
                        SetMotionType(CharaMotionType.Dg);
                        CourtSet.ECDdg_f = true; //１回だけ
                    }

                    if (Court.ECDdg_f)
                    {
                        MoveSet.LastDirectionX = Coordinate.DirectionX;
                        MoveSet.LastDirectionZ = Coordinate.DirectionZ;
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
                if (Court.ECDjp_f == false && canEnemyCourtDodge)
                {
                    if (CanselJump(false) && muki_f)
                    {
                        CourtSet.ECDjp_f = true; //１回だけ
                        CourtSet.ECDdg_f = true; //１回だけ
                    }
                    else
                    {
                        AutoPickUp();
                    }
                }
                break;
            case CharaMotionType.Dg:
                if (Court.ECDdg_f == false && canEnemyCourtDodge)
                {
                    if (CanselJump(false) && muki_f)
                    {
                        CourtSet.ECDjp_f = true; //１回だけ
                        CourtSet.ECDdg_f = true; //１回だけ
                    }
                    else
                    {
                        //よけ限界時間
                        CourtSet.EnemyCortDodgeCount.Add();

                        //押しっぱなしで避け続けるようにする
                        if (Pad.ButtonA.IsPressed
                            && CourtSet.EnemyCortDodgeCount.Value < GetSettingInfield(SettingInfieldType.EnCourtCrTime))
                        {
                            // 他と合わせる
                            // MyAnime.Ani_c = 0;
                        }
                    }
                }
                break;
        }
    }


    /// <summary>
    /// 自動回避行動を実行します
    /// 状況に応じて適切な回避行動を選択し実行します
    /// </summary>
    private void AutoDodgeAction()
    {
        // 敵コート避け可能フラグ
        var canEnemyCourtDodge = GetCanDodgeEnemyCourt();

        //COMの敵コートオートよけ
        if (Composite.IsSelfControl == false
            && Ball.Main.MotionType == BallMotionType.Shoot
            && canEnemyCourtDodge)
        {
            switch (Motion.MotionType)
            {
                case CharaMotionType.St:
                case CharaMotionType.Wk:
                case CharaMotionType.Ds:
                    if (Court.ECDdg_f == false)
                    {
                        SetMotionType(CharaMotionType.Dg);
                    }
                    break;

                case CharaMotionType.Dg:
                    if (Court.ECDdg_f == false)
                    {
                        //よけ限界時間
                        CourtSet.EnemyCortDodgeCount.Add();

                        //押しっぱなしで避け続けるようにする
                        if (Pad.ButtonA.IsPressed
                            && CourtSet.EnemyCortDodgeCount.Value < GetSettingInfield(SettingInfieldType.EnCourtCrTime))
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
            switch (Motion.MotionType)
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


    /// <summary>
    /// アクションタイプを取得します
    /// 現在の状態に基づいて適切なアクションタイプを決定します
    /// </summary>
    /// <returns>決定されたアクションタイプ</returns>
    private ActionType GetActionType()
    {
        ActionType actionType;

        switch (Ball.Main.MotionType)
        {
            case BallMotionType.Hold when Ball.Main.HolderSide == MySideIndex:
                actionType = ActionType.ATA;
                break;
            case BallMotionType.Free:
            case BallMotionType.Bound:
            case BallMotionType.Referee:
            case BallMotionType.Hold when Ball.Main.HolderSide == MySideIndex:
            case BallMotionType.Pass when Ball.Main.ThrowerSideNo == MySideIndex:
                actionType = ActionType.ATF;
                break;
            default:
                actionType = ActionType.ATD;
                break;
        }
        return actionType;
    }


    /// <summary>
    /// 敵コートを回避できるかどうかを判定します
    /// キャラクターの状態と位置に基づいて敵コートへの侵入を回避できるか判定します
    /// </summary>
    /// <returns>敵コートを回避できる場合はtrue</returns>
    private bool GetCanDodgeEnemyCourt()
    {
        bool result;

        if (Composite.IsBallHolder || Order.IsOutfield)
        {
            // ボール持ち、外野はよけNG
            result = false;
        }
        else if (Composite.IsSelfControl || Auto.IsFreeAction)
        {
            // 自由状態
            result = true;
        }
        else if (Ball.Main.MotionType == BallMotionType.Hold)
        {
            // 敵がボール保持
            result = Ball.Main.HolderSide == EnemySideIndex;
        }
        else if (Ball.Main.MotionType == BallMotionType.Shoot)
        {
            // 味方がターゲットのシュート
            result = Ball.Main.ShotTargetSide == MySideIndex;
        }
        else
        {
            // 敵ボール
            result = Ball.Main.TimerSide == EnemySideIndex;
        }
        return result;
    }


    /// <summary>
    /// 自動ボール拾い処理を実行します
    /// キャラクターの状態とボールの位置に基づいて自動的にボールを拾う処理を行います
    /// </summary>
    private void AutoPickUp()
    {
        if (Motion.HasFlag(CharaMotionFlag.Act) == false
            && Composite.IsFree(false)
            && IsPickUpPos())
        {
            HoldBall(false, false);
        }
    }


    /// <summary>
    /// ボールを拾える位置にいるかどうかを判定します
    /// キャラクターとボールの位置関係から拾える状態かを判定します
    /// </summary>
    /// <returns>ボールを拾える位置にいる場合はtrue</returns>
    private bool IsPickUpPos()
    {
        //ジャンプボール上昇中は捕れない
        //審判投げ入れボールも相手外野は捕れない
        //内野も取れないように
        if (Ball.Main.CanHoldJumpBall(MySideIndex) == false)
        {
            return false;
        }

        // さわれない指定が出ているボール
        if (Ball.Main.IsNGGet)
        {
            if (Ball.Main.NGGetTNo == MySideIndex
                && Ball.Main.NGGetPNo == MyMemberIndex)
            {
                return false;
            }
        }

        // 停止、バウンド以外は拾えない
        if (Ball.Main.MotionType != BallMotionType.Free
            && Ball.Main.MotionType != BallMotionType.Bound
            && Ball.Main.MotionType != BallMotionType.Referee)
        {
            return false;
        }

        //当たり
        var atariDepth = GetSettingBall(SettingBallType.FAtariDepth2);
        var isHitDepth = Math.Abs(Ball.Main.Coordinate.Z - Coordinate.Z) <= atariDepth;
        return isHitDepth && Coordinate.HitBox.IsPile(Ball.Main.Atari);
    }


    //ボール持った処理
    private void HoldBall(bool isNoSe, bool isLookBall)
    {
        // 保持状況をチームに渡す
        MyTeam.CallHoldBall(MyOrderIndex);

        DamageSet.KagamiCount.Clear();
        AirSet.IsAirAction = false;
        MoveSet.LastDirectionX = DirectionXType.Neutral;
        ComAutoSet.IsComCounter = false;
        ComAutoSet.IsComTossPassGet = false;
        Pass.MirrorShotLimitCount.Set(Defines.MIRLIM);

        HoldBallSetMirrorState();

        if (isLookBall)
        {
            ResetAutoDirection();

            AutoSet.DirectionX = Ball.Main.Coordinate.VelocityX switch
            {
                > 0 => DirectionXType.Right,
                < 0 => DirectionXType.Left,
                _ => Auto.DirectionX,
            };

            if (Math.Abs(Ball.Main.Coordinate.VelocityZ) > Math.Abs(Ball.Main.Coordinate.VelocityX))
            {
                AutoSet.DirectionZ = Ball.Main.Coordinate.VelocityZ switch
                {
                    > 0 => DirectionZType.Backward,
                    < 0 => DirectionZType.Forward,
                    _ => Auto.DirectionZ,
                };
            }
            else
            {
                AutoSet.DirectionZ = DirectionZType.Neutral;
            }

            MukiSetAuto();
        }

        StopHomingSe();

        if (isNoSe == false)
        {
            PlaySe(SeType.Take);
        }

        //ボール側の処理
        Ball.CallHold(MySideIndex, MyOrderIndex);

        //拾った瞬間dbaFreeのときはその瞬間のタゲをカーソルキャラに
        //それ以外はfreeに戻ったタイミング
        //chCommon_.
        ShootSet.Angle12 = GetMukiAglFromDirection();

        //最初のタゲを敵の操作キャラに
        //外野の可能性もでるのでなんとかする
        //ShTagSet(false); //拾った時点でタゲ無しの時はタゲだけ無理矢理近いキャラから取った方が無難かも
        var passTargetOrderIndex = Order.IsInfield
            ? GetNaiyaPassTag()
            : GetGaiyaPassTag();

        SetPassTarget(passTargetOrderIndex);

        // 拾った時点でタゲ無しの時はタゲだけ無理矢理近いキャラから取った方が無難かも
        if (EnemyTeam.IsAllOut == false)
        {
            //シュートタゲは向き反映
            var orderIndex = GetShootTarget(Shoot.Angle12, false);

            if (orderIndex == OrderIndexType.Disabled)
            {
                if (Composite.IsSelfControl == false)
                {
                    //CPUだけ無理矢理。後で外すかも
                    Ball.CallChangeShootTarget(EnemySideIndex, EnemyTeam.Main.ControlOrderIndex);
                }
            }
            else
            {
                Ball.CallChangeShootTarget(EnemySideIndex, orderIndex);
            }

            //カーソルは強制的に内野
            var enemyControlOrderIndex = GetShootTarget(Shoot.Angle12, true);

            if (enemyControlOrderIndex != OrderIndexType.Disabled) //ないとはおもうが
            {
                EnemyTeam.CallChangeControl(enemyControlOrderIndex);
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
//             // 自動シュー��状態を取得
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
                if (MyTeam.Position.PassAbleCount > 0
                    && MyOrderIndex != MyTeam.Position.Postman
                    && IsNGPassTag(MyTeam.Position.Postman) == false)
                {
                    orderIndex = MyTeam.Position.Postman;
                }
                else
                {
                    orderIndex = OrderIndexType.Outfield4;
                }
            }
        }

        Ball.CallChangePassTarget(MySideIndex, orderIndex);
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
                if (MyTeam.Position.PassAbleCount > 0
                    && MyOrderIndex != MyTeam.Position.Postman
                    && IsNGPassTag(MyTeam.Position.Postman) == false)
                {
                    orderIndex = MyTeam.Position.Postman;
                }
                else
                {
                    orderIndex = OrderIndexType.Outfield4;
                }
            }
        }

        Ball.CallChangePassTarget(MySideIndex, orderIndex);
    }


    private void NGPaTagShift() { }


    //パスカットタゲセット
    private void PaCtTagSet(OrderIndexType orderIndex)
    {
        //パスカットキャラセット
        var passTargetChara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, orderIndex);
        var passTargetCharaX = passTargetChara.Coordinate.X;
        var passTargetCharaZ = passTargetChara.Coordinate.Z;

        var passCutOrderIndex = OrderIndexType.Disabled;
        var maxDist = int.MaxValue;

        for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
        {
            var targetX = passTargetCharaX - EnemySideOrders[order].Coordinate.X;
            var targetZ = Defines.PercentageOf(passTargetCharaZ - EnemySideOrders[order].Coordinate.Z, Defines.ZPER);
            var dist = Defines.Hypot(targetX, targetZ);

            if (dist >= maxDist)
            {
                continue;
            }

            maxDist = dist;
            passCutOrderIndex = (OrderIndexType)order;
        }

        Ball.CallChangePassTarget(MySideIndex, passCutOrderIndex);
    }


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

    /// <summary>
    /// 敵コートに着地するかどうかをチェックします
    /// キャラクターの着地位置が敵コートになるかを判定します
    /// </summary>
    /// <param name="order">チェック対象のオーダーインデックス</param>
    /// <returns>敵コートに着地する場合はtrue</returns>
    private bool IsCheckLandEnPos(int order)
    {
        var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order);

        var tLandX = MySideIndex == 0
            ? chara.Air.LandX
            : Defines.DBCRT_CL - (chara.Air.LandX - Defines.DBCRT_CL);

        return chara.Order.IsInfield && tLandX > Defines.DBCRT_CLI;
    }


    /// <summary>
    /// 指定された角度が現在の向きに対して有効かチェックします
    /// キャラクターの向きと目標位置の角度関係を判定します
    /// </summary>
    /// <param name="targetX">目標X座標</param>
    /// <param name="targetZ">目標Z座標</param>
    /// <returns>角度が有効でない場合はtrue</returns>
    private bool IsCheckNoAgl(int targetX, int targetZ)
    {
        var isInAngle = Coordinate.DirectionZ switch
        {
            DirectionZType.Forward => targetZ < Coordinate.Z,
            DirectionZType.Neutral => true,
            DirectionZType.Backward => targetZ > Coordinate.Z,
            _ => throw new ArgumentOutOfRangeException(),
        };

        if (isInAngle)
        {
            isInAngle = Coordinate.DirectionX switch
            {
                DirectionXType.Left => targetX < Coordinate.X,
                DirectionXType.Neutral => true,
                DirectionXType.Right => targetX > Coordinate.X,
                _ => throw new ArgumentOutOfRangeException(),
            };
        }

        return isInAngle == false;
    }


    /// <summary>
    /// ボール保持時のミラー状態を設定します
    /// ミラーショットとミラーパスの状態を管理します
    /// </summary>
    private void HoldBallSetMirrorState()
    {
        const int activeCount = 1;
        const int inactiveCount = 0;

        if (Ball.Main.MotionType == BallMotionType.Pass
            && Ball.Main.ThrowerSideNo == MySideIndex)
        {
            Pass.MirrorShotCount.Set(activeCount);

            var mirrorPassCount = Pad.ButtonB.IsPressed
                ? activeCount
                : inactiveCount;
            Pass.MirrorPassCount.Set(mirrorPassCount);
        }
        else
        {
            Pass.MirrorPassCount.Set(inactiveCount);

            // キャッチボタン不要で取れるボール
            var isActiveMirrorShot = Ball.Main.MotionType is BallMotionType.Free or BallMotionType.Bound or BallMotionType.Referee
                                     && Damage.FumbleCountValue == 0;

            var mirrorShotCount = isActiveMirrorShot
                ? activeCount
                : inactiveCount;
            Pass.MirrorShotCount.Set(mirrorShotCount);
        }
    }


    /// <summary>
    /// 自動向きの設定を行います
    /// キャラクターの向きを自動的に設定し、変更があったかを返します
    /// </summary>
    /// <returns>向きが変更された場合はtrue</returns>
    private bool MukiSetAuto()
    {
        var lastDirectionX = Coordinate.DirectionX;
        var lastDirectionZ = Coordinate.DirectionZ;

        MoveSet.LastDirectionX = lastDirectionX;
        MoveSet.LastDirectionZ = lastDirectionZ;

        CoordinateSet.DirectionX = Auto.DirectionX switch
        {
            DirectionXType.Left => DirectionXType.Left,
            DirectionXType.Right => DirectionXType.Right,
            _ => Coordinate.DirectionX,
        };

        CoordinateSet.DirectionZ = Auto.DirectionZ switch
        {
            DirectionZType.Forward => DirectionZType.Forward,
            DirectionZType.Backward => DirectionZType.Backward,
            DirectionZType.Neutral => DirectionZType.Neutral,
            _ => Coordinate.DirectionZ,
        };

        // 向き変えありかどうか
        var isChanged = lastDirectionX != Coordinate.DirectionX
                        || lastDirectionZ != Coordinate.DirectionZ;

        if (isChanged)
        {
            //タゲも変える
            ShootSet.Angle12 = GetMukiAglFromDirection();
        }

        return isChanged;
    }


    /// <summary>
    /// 自動向きの初期化を行います
    /// キャラクターの現在の向きを基に自動向きを初期化します
    /// </summary>
    private void ResetAutoDirection()
    {
        AutoSet.DirectionX = Coordinate.DirectionX switch
        {
            DirectionXType.Left => DirectionXType.Left,
            DirectionXType.Neutral => DirectionXType.Neutral,
            DirectionXType.Right => DirectionXType.Right,
            _ => throw new ArgumentOutOfRangeException(),
        };

        AutoSet.DirectionZ = Coordinate.DirectionZ switch
        {
            DirectionZType.Forward => DirectionZType.Forward,
            DirectionZType.Neutral => DirectionZType.Neutral,
            DirectionZType.Backward => DirectionZType.Backward,
            _ => throw new ArgumentOutOfRangeException(),
        };
    }


    /// <summary>
    /// 現在の向きから角度を取得します
    /// キャラクターの向きから12時方向の角度を計算します
    /// </summary>
    /// <returns>12時方向の角度</returns>
    private int GetMukiAglFromDirection()
    {
        return GetMukiAgl(Coordinate.DirectionX, Coordinate.DirectionZ);
    }


    /// <summary>
    /// 指定された向きから角度を取得します
    /// 指定されたX方向とZ方向から12時方向の角度を計算します
    /// </summary>
    /// <param name="directionX">X方向の向き</param>
    /// <param name="directionZ">Z方向の向き</param>
    /// <returns>12時方向の角度</returns>
    private int GetMukiAgl(DirectionXType directionX, DirectionZType directionZ)
    {
        return GetMukiAgl(directionX == DirectionXType.Left,
            directionX == DirectionXType.Right,
            directionZ == DirectionZType.Backward,
            directionZ == DirectionZType.Forward);
    }


    /// <summary>
    /// 向きから角度を取得します
    /// 各方向の入力状態から12時方向の角度を計算します
    /// </summary>
    /// <param name="isLeft">左方向の入力状態</param>
    /// <param name="isRight">右方向の入力状態</param>
    /// <param name="isUp">上方向の入力状態</param>
    /// <param name="isDown">下方向の入力状態</param>
    /// <returns>12時方向の角度</returns>
    private int GetMukiAgl(bool isLeft, bool isRight, bool isUp, bool isDown)
    {
        switch (Order.GetOrderFieldType())
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
                    ShootSet.Angle12 = 9; //9 10 11 0
                }
                else if (isRight)
                {
                    ShootSet.Angle12 = 11; //11 0 1 2
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
    /// パスターゲットとして無効かどうかをチェックします
    /// 指定されたオーダーがパスターゲットとして有効かを判定します
    /// </summary>
    /// <param name="order">チェック対象のオーダー</param>
    /// <returns>パスターゲットとして無効な場合はtrue</returns>
    private bool IsNGPassTag(OrderIndexType order)
    {
        if (order == MyOrderIndex
            || order == OrderIndexType.Disabled)
        {
            return true; //パス不可
        }

        var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, order);

        //★ダッシュマンはだいじょぶ
        if (chara.Composite.IsDashman)
            //&& ((st_.pmgMyTm_.st_.pmgMyCh_[pos].MyCoordinate.dY >= (-XYMAG))
            //  || (MyMirPass_c > 0)))//下降ではない
        {
            return false;
        }

        //空中の人はパスタゲにならないように
        return chara.Composite.IsFree(true) == false
               || chara.Motion.HasFlag(CharaMotionFlag.Ar);
    }


    /// <summary>
    /// パスターゲットとして無効かどうかをチェックします（int版）
    /// </summary>
    /// <param name="order">チェック対象のオーダー番号</param>
    /// <returns>パスターゲットとして無効な場合はtrue</returns>
    private bool IsNGPassTag(int order)
    {
        return IsNGPassTag((OrderIndexType)order);
    }


    /// <summary>
    /// 自己操作の処理を行います
    /// プレイヤーの入力に基づいてキャラクターの動作を制御します
    /// </summary>
    private void ControlSelf()
    {
        var actionType = GetActionType();

        //モーション変更前の向き
        var lastMuki = Coordinate.DirectionX;
        var lastMukiZ = Coordinate.DirectionZ;
        bool LastMukiKeep_f = false;
        bool atlook_f = Pad.IsPressedAnyCross() == false;

        //プレビュー中パス禁止
        bool pabtn_f = Pad.ButtonA.IsJustPressed;
        bool pabtn2_f = Pad.ButtonA.IsPressed;
        bool dgbtn_f = Pad.ButtonA.IsJustPressed;
        bool dgbtn2_f = Pad.ButtonA.IsPressed;
        bool cabtn_f = Pad.ButtonA.IsJustPressed;
        bool shbtn_f = Pad.ButtonB.IsJustPressed;
        bool shbtn2_f = Pad.ButtonB.IsPressed;
        bool uppos_f = pabtn_f || dgbtn_f;
        bool dnpos_f = cabtn_f || shbtn_f;

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

        switch (Motion.MotionType)
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
                var canAirAction = Air.IsAirAction == false
                                   && Motion.HasFlag(CharaMotionFlag.Ar)
                                   && (Coordinate.VelocityY > 0 || Coordinate.Y >= Defines.JPINVALIDHEIGHT);
                var isMarionette = BallEffect.symCtrl_f && Motion.HasFlag(CharaMotionFlag.Ar);

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
                if (Composite.IsBallHolder
                    && Shoot.IsUTurn == false
                    && Order.IsInfield)
                {
                    bool utrn_f = false;

                    if (MySideIndex == 0
                        && Coordinate.DirectionX == DirectionXType.Left
                        && Pad.KeyRight.IsPressed)
                    {
                        utrn_f = true;
                        CoordinateSet.DirectionX = DirectionXType.Right;
                        CoordinateSet.DirectionZ = DirectionZType.Neutral;
                        ShootSet.Angle12 = GetMukiAglFromDirection();
                    }
                    else if (MySideIndex == 1
                             && Coordinate.DirectionX == DirectionXType.Right
                             && Pad.KeyLeft.IsPressed)
                    {
                        utrn_f = true;
                        CoordinateSet.DirectionX = DirectionXType.Left;
                        CoordinateSet.DirectionZ = DirectionZType.Neutral;
                        ShootSet.Angle12 = GetMukiAglFromDirection();
                    }

                    if (utrn_f)
                    {
                        SetMotionType(CharaMotionType.RtSh);

                        //ダッシュに復帰
                        if (Motion.HasFlag(CharaMotionFlag.Slip))
                        {
                            MotionSet.AddMotionFlag(CharaMotionFlag.Ds);
                        }
                    }
                }
                break;
            case CharaMotionType.Ca:
                if (IsPickUpPos())
                {
                    HoldBall(false, false);

                    // キャッチダッシュ継続
                    if (Order.IsInfield
                        && Auto.AutoType == AutoType.Free
                        && Motion.HasFlag(CharaMotionFlag.Ds))
                    {
                        if ((Coordinate.DashDirection == DirectionXType.Left && Pad.KeyLeft.IsPressed)
                            || (Coordinate.DashDirection == DirectionXType.Right && Pad.KeyRight.IsPressed))
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
                if (dgbtn2_f && Composite.IsBallHolder == false)
                {
                    // ほかとアニメーション継続の形を合わせる
                    // MyAnime.Ani_c = 0; //こんなんでいいのだろうか
                }
                break;
        }

        //モーション変更前の向き
        if (LastMukiKeep_f)
        {
            MoveSet.LastDirectionX = lastMuki;
            MoveSet.LastDirectionZ = lastMukiZ;
        }
    }


    /// <summary>
    /// 空中での防御/フリー状態の処理を行います
    /// 空中でのキャッチや回避などの防御行動を制御します
    /// </summary>
    private void AirDefenceOrFree()
    {
        if (Pad.ButtonA.IsJustPressed)
        {
            SetMotionType(CharaMotionType.Dg);
            // キャッチ時間で変えるとよいかも
            PlaySeCatchSe();

            // パスをスルーしてカバーマンに操作権を渡す
            if (Ball.Main.MotionType == BallMotionType.Pass
                && Ball.Main.PassTargetSide == MySideIndex
                && MyTeam.Main.ControlOrderIndex == Ball.Main.PassTargetOrder
                && Ball.Main.PassTargetOrder >= OrderIndexType.Infield0
                && Ball.Main.PassTargetOrder <= OrderIndexType.Infield3)
            {
                MyTeam.CallChangeControlCoverMan();
            }
        }
        else if (Pad.ButtonB.IsJustPressed)
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


    /// <summary>
    /// キャッチ時の向き設定を行います
    /// ボールをキャッチする際の適切な向きを設定します
    /// </summary>
    private void SetCatchMuki()
    {
        AutoMukiInit();
        SetBallMukiX();
        SetBallMukiZ();
        MukiSetAuto();
    }


    /// <summary>
    /// ボールに対するX方向の向きを設定します
    /// ボールの位置に基づいて左右の向きを決定します
    /// </summary>
    private void SetBallMukiX()
    {
        if (Composite.IsBallHolder)
        {
            return;
        }

        //自分がタゲでパスが飛んできてるときはそのままの向き自分が投げたときも
        if (Order.IsInfield == false
            && Ball.Main.MotionType == BallMotionType.Pass
            && Ball.Main.ThrowerSideNo == MySideIndex
            && Ball.Main.ThrowerOrderNo == MyOrderIndex)
        {
            return;
        }

        if (Ball.Main.Coordinate.X < Coordinate.X)
        {
            NextAutoSet.DirectionX = DirectionXType.Left;
        }
        else if (Ball.Main.Coordinate.X < Coordinate.X)
        {
            NextAutoSet.DirectionX = DirectionXType.Right;
        }
    }


    /// <summary>
    /// ボールに対するZ方向の向きを設定します
    /// ボールの位置に基づいて前後の向きを決定します
    /// </summary>
    private void SetBallMukiZ()
    {
        if (Composite.IsBallHolder)
        {
            return;
        }

        // 自分がタゲでパスが飛んできてるときはそのままの向き
        // 自分が投げたときも
        if (Order.IsInfield == false && Ball.Main.MotionType == BallMotionType.Pass)
        {
            if (Ball.Main.ThrowerSideNo == MySideIndex
                && Ball.Main.ThrowerOrderNo == MyOrderIndex)
            {
                return;
            }

            // パスの時はパス先を向く
            NextAutoSet.DirectionZ = Ball.Main.ThrowerOrderNo switch
            {
                OrderIndexType.Outfield2 => DirectionZType.Backward,
                OrderIndexType.Outfield3 => DirectionZType.Forward,
                _ => DirectionZType.Neutral,
            };
        }
        else
        {
            if (Ball.Main.Coordinate.Z < Coordinate.Z)
            {
                if (Ball.Main.Coordinate.Z < Coordinate.Z - Defines.DEFDISTZ)
                {
                    NextAutoSet.DirectionZ = DirectionZType.Forward;
                }
                else if (Ball.Main.Coordinate.Z > Coordinate.Z - Defines.DEFDISTZ / 2
                         || NextAuto.DirectionZ == DirectionZType.Backward)
                {
                    NextAutoSet.DirectionZ = DirectionZType.Neutral;
                }
            }
            else
            {
                if (Ball.Main.Coordinate.Z > Coordinate.Z + Defines.DEFDISTZ)
                {
                    NextAutoSet.DirectionZ = DirectionZType.Backward;
                }
                else if (Ball.Main.Coordinate.Z < Coordinate.Z + Defines.DEFDISTZ / 2
                         || NextAuto.DirectionZ == DirectionZType.Forward)
                {
                    NextAutoSet.DirectionZ = DirectionZType.Neutral;
                }
            }
        }
    }


    /// <summary>
    /// 自動向きの初期化を行います
    /// 現在の向きを自動向きの初期値として設定します
    /// </summary>
    private void AutoMukiInit()
    {
        AutoSet.DirectionX = Coordinate.DirectionX;
        AutoSet.DirectionZ = Coordinate.DirectionZ;
    }


    /// <summary>
    /// 空中での攻撃処理を行います
    /// 空中でのパスやシュートなどの攻撃行動を制御します
    /// </summary>
    private void AirAttack()
    {
        if (Pad.ButtonA.IsJustPressed)
        {
            Passing();
        }
        else if (Pad.ButtonB.IsJustPressed)
        {
            bool atlook_f = Pad.IsPressedAnyCross() == false;
            LookTg(Ball.Main.ShotTargetOrder, false, atlook_f);
            SetMotionType(CharaMotionType.JSh);
        }
        else
        {
            MirrorAttack();
        }
    }


    /// <summary>
    /// ミラー攻撃の処理を行います
    /// ミラーパスとミラーシュートの制御を行います
    /// </summary>
    private void MirrorAttack()
    {
        if (Pad.ButtonB.IsPressed) //シュート入力おしっぱ
        {
            Pass.MirrorPassCount.Clear();
        }

        if (Pass.MirrorPassCount.Value > 0) //ミラーパス状態
        {
            if (Pad.ButtonA.IsPressed)
            {
                if (Pass.MirrorPassCount.AddUntil(Defines.MIRWAIT))
                {
                    if (Ball.Main.PassTargetOrder != OrderIndexType.Disabled)
                    {
                        var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, Ball.Main.PassTargetOrder);

                        if (chara.Composite.IsDashman)
                        {
                            Passing();
                        }
                        else
                        {
                            Pass.MirrorPassCount.Clear();
                        }
                    }
                }
            }
            else
            {
                Pass.MirrorPassCount.Clear();
            }
        }

        if (Pass.MirrorShotCount.Value > 0) //ミラーシュート状態
        {
            if (Pad.ButtonB.IsPressed && Pad.ButtonA.IsPressed == false) //シュート入力おしっぱ
            {
                if (Pass.MirrorPassCount.AddUntil(Defines.MIRWAIT))
                {
                    LookTg(Ball.Main.ShotTargetOrder, false, true);
                    Shooting();
                }
            }
            else
            {
                Pass.MirrorShotCount.Set(1);
            }
        }
    }


    /// <summary>
    /// シュート処理を実行します
    /// シュートモーションの設定と実行を行います
    /// </summary>
    private void Shooting()
    {
        //ダッシュ方向とシュート方向があっているときは振り返り扱いにしない
        bool nortst_f = Motion.HasFlag(CharaMotionFlag.Ds)
                        && Coordinate.DashDirection == Coordinate.DirectionX;

        //内野で向きに変わるときは振り向きシュート
        bool isReturnShot = Order.IsInfield
                            && nortst_f == false
                            && Move.LastDirectionX != Coordinate.DirectionX;

        CharaMotionType shootMotion;

        if (Motion.HasFlag(CharaMotionFlag.Ar))
        {
            shootMotion = CharaMotionType.JSh;
        }
        else
        {
            shootMotion = isReturnShot ? CharaMotionType.RtSh : CharaMotionType.Sh;
        }

        SetMotionType(shootMotion);
    }


    /// <summary>
    /// パス処理を実行します
    /// パスの方向設定とモーション実行を行います
    /// </summary>
    private void Passing()
    {
        //パスタゲがパス出せないとき
        NGPaTagShift();

        //内野の場合後ろ内野内パスなので、
        //相手の方向を向かないと行けない
        //★ミラーダッシュマンのときはそちらを向かない
        //if (st_.pmgMyTm_.st_.pmgMyCh_[pmgSG_.stBa_.PaTgPNo].IsDashman() == false)
        //{
        LookTg(Ball.Main.PassTargetOrder, true, false);
        //}

        CharaMotionType passMotion = Motion.HasFlag(CharaMotionFlag.Ar)
            ? CharaMotionType.JPa
            : CharaMotionType.Pa;
        SetMotionType(passMotion);
    }


    /// <summary>
    /// ターゲットの方向を向く処理を行います
    /// パスやシュート時のターゲットに対する向きを設定します
    /// </summary>
    /// <param name="TgPNo">ターゲットのオーダー</param>
    /// <param name="Pa_f">パス処理かどうか</param>
    /// <param name="AtLook_f">自動向き設定を行うかどうか</param>
    private void LookTg(OrderIndexType TgPNo, bool Pa_f, bool AtLook_f)
    {
        TgPNo = 0;

        CharaBehavior TgSt = null;
        bool Notag_f = false;
        int futureX, futureZ; //SHLAG後の自分の位置

        bool dmPass_f = false;
        bool alleyoop_f = false;

        if (Pa_f)
        {
            if (Ball.Main.PassTargetSide != MySideIndex
                || Ball.Main.PassTargetOrder == OrderIndexType.Disabled)
            {
                Notag_f = true;
            }
            else
            {
                TgSt = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, Ball.Main.PassTargetOrder);
                dmPass_f = TgSt.Composite.IsDashman;
                alleyoop_f = Ball.Main.PaAlPa;
            }
        }
        else
        {
            if (Ball.Main.ShotTargetSide != MySideIndex
                || Ball.Main.ShotTargetOrder == OrderIndexType.Disabled)
            {
                Notag_f = true;
            }
            else //タゲがいる
            {
                TgSt = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, Ball.Main.ShotTargetOrder);
            }
        }

        // シュートタイミングの位置
        futureX = Coordinate.X + Coordinate.VelocityX * Defines.SHLAG;
        futureZ = Coordinate.Z + Coordinate.VelocityZ * Defines.SHLAG;

        //タゲあり
        if (Notag_f == false && TgSt != null)
        {
            bool near_f = Math.Abs(TgSt.Coordinate.X - futureX) < Defines.NEARDISTX
                          && Math.Abs(TgSt.Coordinate.Z - futureZ) < Defines.NEARDISTZ;

            if (Order.IsInfield == false
                || Pa_f
                || near_f == false)
            {
                //オート向き初期化
                AutoMukiInit();

                var nowDist = TgSt.Coordinate.X - Coordinate.X;
                var futureDist = TgSt.Coordinate.X - futureX;
                var isSameDirection = nowDist * futureDist > 0;

                if (isSameDirection)
                {
                    AutoSet.DirectionX = TgSt.Coordinate.X < futureX
                        ? DirectionXType.Left
                        : DirectionXType.Right;
                }

                //ダッシュマンパス
                if (dmPass_f)
                {
                    AutoSet.DirectionZ = DirectionZType.Neutral;
                }
                else
                {
                    bool Z_f = Math.Abs(TgSt.Coordinate.Z - futureZ) > Math.Abs(TgSt.Coordinate.X - futureX); //★斜めのとき

                    if (Z_f)
                    {
                        AutoSet.DirectionZ = TgSt.Coordinate.Z < futureZ
                            ? DirectionZType.Forward
                            : DirectionZType.Backward;
                    }
                    else
                    {
                        AutoSet.DirectionZ = DirectionZType.Neutral;
                    }
                }

                MukiSetAuto();
            }
        }
        else //タゲ無し
        {
            if (Order.IsInfield == false
                || Composite.IsSelfControl == false
                || (AtLook_f && Notag_f))
            {
                //２，３番外野が真横に投げてしまうのを阻止
                switch (MyOrderIndex)
                {
                    case OrderIndexType.Outfield2:
                        AutoSet.DirectionZ = DirectionZType.Forward;
                        MukiSetAuto();
                        SetShTagFromMyShootAngle12(false);
                        break;
                    case OrderIndexType.Outfield3:
                        AutoSet.DirectionZ = DirectionZType.Backward;
                        MukiSetAuto();
                        SetShTagFromMyShootAngle12(false);
                        break;
                    case OrderIndexType.Outfield4:
                        AutoSet.DirectionX = MySideIndex == 0
                            ? DirectionXType.Left
                            : DirectionXType.Right;
                        MukiSetAuto();
                        SetShTagFromMyShootAngle12(false);
                        break;
                }

                if (Pa_f == false && IsShTag() == false)
                {
                    //全方向チェック
                    var directionX = Coordinate.DirectionX;
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
                    directionX = Coordinate.DirectionX == DirectionXType.Left
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
                    }
                }
            }
        }
    }


    /// <summary>
    /// シュートターゲットの探索を行います
    /// 指定された方向でシュート可能なターゲットを探します
    /// </summary>
    /// <param name="directionX">X方向の向き</param>
    /// <param name="directionZ">Z方向の向き</param>
    /// <returns>ターゲットが見つかった場合はtrue</returns>
    private bool SearchShootTarget(DirectionXType directionX, DirectionZType directionZ)
    {
        var angle12 = GetMukiAgl(directionX, directionZ);
        return GetShootTarget(angle12, false) != OrderIndexType.Disabled;
    }


    /// <summary>
    /// 自動ターゲットの設定を行います
    /// 指定された方向に基づいて自動的にターゲットを設定します
    /// </summary>
    /// <param name="directionX">X方向の向き</param>
    /// <param name="directionZ">Z方向の向き</param>
    private void SetAutoTarget(DirectionXType directionX, DirectionZType directionZ)
    {
        AutoSet.DirectionX = directionX;
        AutoSet.DirectionZ = directionZ;
        MukiSetAuto();
        SetShTagFromMyShootAngle12(false);
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


    /// <summary>
    /// デバッグ用の強制終了処理を行います
    /// デバッグモード時の特殊な終了処理を実行します
    /// </summary>
    private void DegbugKill()
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


    /// <summary>
    /// デバッグ用のシュート処理を行います
    /// デバッグモード時の特殊なシュート処理を実行します
    /// </summary>
    private void DebugShot()
    {
//         #ifdef __K_DEBUG_SHIAI__
//         kdebug::DebugSystem* pDs = kdebug::DebugSystem::GetInstance();
//         bool isAutoShot = false;
//         // ���動シュート状態を取得
//         int step = pDs.GetAutoShootStep();
//         if (step == kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_STANDBY)
//         {
//             // この状態で手元にボールがないのがおかしいので
//             // 強制的に手元に戻す
//             // なのでこのif文���は例外処理
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
