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
        var passTargets = Order.IsInfield
            ? GetNaiyaPassTarget()
            : GetGaiyaPassTarget();

        // 最初のターゲットを設定
        if (passTargets.Length > 0)
        {
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
        if (order == MyOrderIndex || order == OrderIndexType.Disabled)
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
        return chara.Composite.IsFree(true) == false || chara.Motion.HasFlag(CharaMotionFlag.Ar);
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
                    // MyAnime.Ani_c = 0; //こんなんでいのだろうか
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
    /// 自動向きの初期化を行います
    /// 現在の向きを自動向きの初期値として設定します
    /// </summary>
    private void AutoMukiInit()
    {
        AutoSet.DirectionX = Coordinate.DirectionX;
        AutoSet.DirectionZ = Coordinate.DirectionZ;
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
        //         // 動シュート状態を取得
        //         int step = pDs.GetAutoShootStep();
        //         if (step == kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_STANDBY)
        //         {
        //             // この状態で手元にボールがないのがおかしいので
        //             // 強制的に手元に戻す
        //             // なのでこのif文は例外処理
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
