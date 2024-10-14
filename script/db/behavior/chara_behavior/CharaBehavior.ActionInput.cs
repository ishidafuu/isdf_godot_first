using System;

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
            case AutoType.Free:
                if (IsSelfControl)
                {
                    ControlSelf();
                }
                else
                {
                    //       if ((IsBall() == FALSE)
                    //         && (st_.pstMyCh_->Motion.Mtype == dbmtJDg))
                    //       {
                    //         st_.pstMyCh_->Anime.Ani_c = 0;//こうやる必要ないか？
                    //       }
                    AutoPickUp();
                }
                break;
            case AutoType.Alleyoop:
                break;
            case AutoType.Return:
                break;
            case AutoType.KgReturn:
                break;
            case AutoType.Get:
                break;
            case AutoType.PassCvr:
                break;
            case AutoType.DfPos:
                break;
            case AutoType.PassCach:
                break;
            case AutoType.PassJp:
                break;
            case AutoType.Dg:
                break;
            case AutoType.DgJp:
                break;
            case AutoType.DgSw:
                break;
            case AutoType.ShCv:
                break;
            case AutoType.CPUPaCut:
                break;
            case AutoType.CPUHelp:
                break;
            case AutoType.CPUCa:
                break;
            case AutoType.CPUShPa:
                break;
            case AutoType.CPUJumpBall:
                break;
            case AutoType.Gohome:
                break;
            case AutoType.Dashman:
                break;
            default:
                throw new ArgumentOutOfRangeException();
        }

        //
        //
        //   switch (st_.pstMyCh_->Auto.AutoType)
        //   {
        //     //自由
        //   case dbatFree:
        //     if (IsSelfCtrl())
        //     {
        //       //自分で操作**********
        //       pCommon_->SelfCtrl(ATType);
        //     }
        //     else
        //     {
        //       if ((IsBall() == FALSE)
        //         && (st_.pstMyCh_->Motion.Mtype == dbmtJDg))
        //       {
        //         st_.pstMyCh_->Anime.Ani_c = 0;//こうやる必要ないか？
        //       }
        //
        //       pCommon_->AutoPickUp();
        //       //CheckFreeBallPickUp();
        //     }
        //     break;
        //
        //     //ポジショニング
        //   case dbatDfPos:
        //     pCommon_->AutoPickUp();
        //     //CheckFreeBallPickUp();
        //     break;
        //
        //     //戻り
        //   case dbatReturn:
        //     if (IsSelfCtrl()//←いらないかも
        //       || (IsCOM() == FALSE))
        //     {
        //
        //       BOOL muki_f = ((st_.mysideNo_ == 0) && (st_.pstMyCh_->Zahyou.Muki == mL))
        //         || ((st_.mysideNo_ == 1) && (st_.pstMyCh_->Zahyou.Muki == mR));
        //
        //       switch (st_.pstMyCh_->Motion.Mtype)
        //       {
        //       case dbmtCr:
        //         break;
        //       case dbmtSt:
        //       case dbmtWk:
        //       case dbmtDs:
        //       case dbmtSl://スリップも追加オーバーラインの瞬間なぞのキャッチできてしまう
        //         if ((st_.pstMyCh_->ECDdg_f == FALSE) && (ecdgOK_f))
        //         {
        //           if (MyPad()->IsJump()//ジャンプ入力
        //             && muki_f
        //             && (st_.pstMyCh_->ECDjp_f == FALSE))
        //           {
        //             pCommon_->JumpSet(FALSE, FALSE, FALSE);//ジャンプ
        //             //mid::midLog("J0\n");
        //             st_.pstMyCh_->ECDdg_f = TRUE;//１回だけ
        //             st_.pstMyCh_->ECDjp_f = TRUE;//１回だけ
        //           }
        //           else if (MyPad()->IsDodge())//よけ
        //           {
        //             pCommon_->SetMtype(dbmtDg);
        //             st_.pstMyCh_->ECDdg_f = TRUE;//１回だけ
        //           }
        //           //else if (MyPad()->IsCatch())//キャッチ入力
        //           //{
        //           //  pCommon_->SetCatchMuki();
        //           //  pCommon_->SetMtype(dbmtCa);
        //           //  st_.pstMyCh_->ECDdg_f = TRUE;//１回だけ
        //           //}
        //
        //           if (st_.pstMyCh_->ECDdg_f)
        //           {
        //             pCommon_->CatchSE();
        //             st_.pstMyCh_->LastMuki = st_.pstMyCh_->Zahyou.Muki;
        //             st_.pstMyCh_->LastMukiZ = st_.pstMyCh_->Zahyou.MukiZ;
        //           }
        //         }
        //         break;
        //       case dbmtSh:
        //       case dbmtPa:
        //         if (ecdgOK_f) pCommon_->CanselJump(FALSE);
        //         break;
        //       case dbmtCa:
        //       case dbmtJCa:
        //         if ((st_.pstMyCh_->ECDjp_f == FALSE) && ecdgOK_f)
        //         {
        //           //mid::midLog("J1\n");
        //           if (pCommon_->CanselJump(FALSE) && muki_f)
        //           {
        //             st_.pstMyCh_->ECDjp_f = TRUE;//１回だけ
        //             st_.pstMyCh_->ECDdg_f = TRUE;//１回だけ
        //           }
        //           else
        //           {
        //             pCommon_->AutoPickUp();
        //           }
        //         }
        //         //オーバラインは審判息なのでもう不要
        //         break;
        //       case dbmtDg:
        //         if ((st_.pstMyCh_->ECDjp_f == FALSE) && ecdgOK_f)
        //         {
        //           //mid::midLog("J2\n");
        //           if (pCommon_->CanselJump(FALSE) && muki_f)
        //           {
        //             st_.pstMyCh_->ECDjp_f = TRUE;//１回だけ
        //             st_.pstMyCh_->ECDdg_f = TRUE;//１回だけ
        //           }
        //           else
        //           {
        //             //よけ限界時間
        //             ++st_.pstMyCh_->ECDdg_c;
        //             if ((st_.pstMyCh_->ECDdg_c < pmgEO_->mgDt_.dtSet_.GetDtInfield(setEnCourtCrTime))
        //               && MyPad()->IsDodge2())//押しっぱなしで避け続けるようにする
        //             {
        //               st_.pstMyCh_->Anime.Ani_c = 0;
        //             }
        //           }
        //         }
        //
        //         break;
        //       }
        //     }
        //     break;
        //
        //     //拾いに行く
        //   case dbatGet:
        //     switch (st_.pstMyCh_->Motion.Mtype)
        //     {
        //     case dbmtSt:
        //     case dbmtWk:
        //     case dbmtDs:
        //       pCommon_->AutoPickUp();//自動拾い
        //       break;
        //     }
        //     break;
        //
        //     //よけ
        //   case dbatDg:
        //   {
        //     //COMの的コートオートよけ
        //     if ((IsSelfCtrl() == FALSE)
        //       && (pmgSG_->stBa_.Motion == bmShoot)
        //       && (ecdgOK_f))
        //     {
        //       switch (st_.pstMyCh_->Motion.Mtype)
        //       {
        //       case dbmtSt:
        //       case dbmtWk:
        //       case dbmtDs:
        //         if (st_.pstMyCh_->ECDdg_f == FALSE)
        //         {
        //           pCommon_->SetMtype(dbmtDg);
        //           pCommon_->CatchSE();
        //           //st_.pstMyCh_->ECDdg_f = TRUE;//１回だけ
        //         }
        //         break;
        //       case dbmtDg:
        //         //よけ限界時間
        //         ++st_.pstMyCh_->ECDdg_c;
        //
        //         if (st_.pstMyCh_->ECDdg_c < pmgEO_->mgDt_.dtSet_.GetDtInfield(setEnCourtCrTime))
        //         {
        //           st_.pstMyCh_->Anime.Ani_c = 0;
        //         }
        //         break;
        //       }
        //     }
        //     else
        //     {
        //       switch (st_.pstMyCh_->Motion.Mtype)
        //       {
        //       case dbmtSt:
        //       case dbmtWk:
        //       case dbmtDs:
        //         pCommon_->SetMtypeReset(dbmtDg);
        //         pCommon_->CatchSE();
        //         break;
        //       case dbmtDg:
        //         //避け続けるようにする
        //         st_.pstMyCh_->Anime.Ani_c = 0;
        //         break;
        //       }
        //     }
        //   }
        //   break;
        //   //ＣＯＭキャッチ
        //   case dbatCPUCa:
        //     break;
        //     //ＣＯＭ思考行動★★
        //   case dbatCPUShPa:
        //     COMAction();
        //     break;
        //   }
        // }
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
    private void HoldBall(bool isNoSE, bool isLookBall) //FALSE,FALSE
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

        if (isNoSE == false)
        {
            PlaySe(SeType.Take);
        }

        //自分を操作キャラに
        //st_.pmgMyTm_->SetCtrl(st_.posNo_);
        st_.pmgMyTm_->SetCtrlBallGet(st_.posNo_); //強制

        //ボール側の処理

        //とりあえずAniNoをクリア
        st_.pmgBa_->baCommon_.SetNmlBall(FALSE);

        //タイマーリセット
        st_.pmgBa_->baCommon_.ResetTimer(st_.mysideNo_, FALSE);

        //自分でキャッチしたときも音を止める
        pmgEO_->mgSound_.StopHMNG();

        //★成績//リバウンド
        if (pmgSG_->stBa_.RbwTNo == st_.mysideNo_)
        {
            AddRec(recRebound_c);
        }

        st_.pmgBa_->baCommon_.SetMTypeHold(st_.mysideNo_, st_.posNo_);

        pmgSG_->stBa_.NGGet_f = FALSE;
        pmgSG_->stBa_.ShTgTNo = st_.ensideNo_;
        pmgSG_->stBa_.ShTgPNo = 0;
        pmgSG_->stBa_.PaTgTNo = st_.mysideNo_;
        pmgSG_->stBa_.PaTgPNo = 0;

        //拾った瞬間dbaFreeのときはその瞬間のタゲをカーソルキャラに
        //それ以外はfreeに戻ったタイミング
        //chCommon_.
        SetMukiAgl(
            (st_.pstMyCh_->Zahyou.Muki == mL),
            (st_.pstMyCh_->Zahyou.Muki == mR),
            (st_.pstMyCh_->Zahyou.MukiZ == mzB),
            (st_.pstMyCh_->Zahyou.MukiZ == mzF));

        //最初のタゲを敵の操作キャラに
        //外野の可能性もでるのでなんとかする
        //ShTagSet(FALSE); //拾った時点でタゲ無しの時はタゲだけ無理矢理近いキャラから取った方が無難かも
        PaTagSet();

        //拾った時点でタゲ無しの時はタゲだけ無理矢理近いキャラから取った方が無難かも
        if (st_.pmgEnTm_->IsAllDead() == FALSE)
        {
            //シュートタゲは向き反映
            s32 tstg = GetTag(FALSE);
            if (tstg == NGNUM)
            {
                if (IsSelfCtrl() == FALSE)
                {
                    //CPUだけ無理矢理。後で外すかも
                    pmgSG_->stBa_.ShTgTNo = st_.ensideNo_;
                    pmgSG_->stBa_.ShTgPNo = st_.pmgEnTm_->st_.pstMyTm_->CtrlNo;
                }
            }
            else
            {
                //st_.pmgEnTm_->SetCtrl(tstg);
                pmgSG_->stBa_.ShTgTNo = st_.ensideNo_;
                pmgSG_->stBa_.ShTgPNo = tstg;
            }

            //カーソルは強制的に内野
            s32 tEnctrl = GetTag(TRUE);
            if (tEnctrl != NGNUM) //ないとはおもうが
            {
                st_.pmgEnTm_->SetCtrl(tEnctrl);
            }
        }
#ifdef __K_DEBUG_SHIAI__
        kdebug::DebugSystem* pDs = kdebug::DebugSystem::GetInstance();
        if (pDs->IsReturnBall())
        {
            if (st_.posNo_ != 0)
            {
                // ボールが手元に帰ってくる処理
                pmgGO_->pmgBa_->baCommon_.ResetRefPos_Prev(FALSE);

                // 操作キャラを強制的にボール持ってる人に
                //st_.pmgTm_[SIDE0]->st_.pmgMyTm_->SetCtrlBallGet(0);
            }

            // 自動シュート状態を取得
            s32 step = pDs->GetAutoShootStep();
            if (step == kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_WAIT)
            {
                pDs->SetReturnBallFlg(FALSE); // フラグを落とす
                pDs->SetAutoShootStep(kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_CHOOSE);
            }
        }
#endif // #ifdef __K_DEBUG_SHIAI__
    }

    private void HoldBallSetMirrorState()
    {
        const int activeCount = 1;
        const int inactiveCount = 0;

        if (MyState.Pad.IsValid
            && BallState.MotionType == BallMotionType.Pass
            && BallState.ThrowerSideNo == MySideIndex)
        {
            MyState.Pass.MirrorShotCount.Set(activeCount);

            var mirrorPassCount = MyState.Pad.Pad.ButtonB.IsPressed
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
            SetMukiAgl(MyState.Coordinate.DirectionX == DirectionXType.Left,
                MyState.Coordinate.DirectionX == DirectionXType.Right,
                MyState.Coordinate.DirectionZ == DirectionZType.Backward,
                MyState.Coordinate.DirectionZ == DirectionZType.Forward);
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

    //ターゲッティング用向き
    private void SetMukiAgl(bool isLeft, bool isRight, bool isUp, bool isDown)
    {
        switch (MyState.Order.GetOrderType())
        {
            case OrderType.Infield:
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
            case OrderType.Outfield2:
                if (isLeft)
                {
                    MyState.Shoot.Angle12 = 5; //5678
                }
                else if (isRight)
                {
                    MyState.Shoot.Angle12 = 3; //3456
                }
                break;
            case OrderType.Outfield3:
                if (isLeft)
                {
                    MyState.Shoot.Angle12 = 9; //9 10 11 0
                }
                else if (isRight)
                {
                    MyState.Shoot.Angle12 = 11; //11 0 1 2
                }
                break;
            case OrderType.Outfield4:
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

    //自分で操作
    private void ControlSelf()
    {
        var actionType = GetActionType();
//
//         const s32 MIRWAIT = 4;
//         //モーション変更前の向き
//         enMukiType lastMuki = st_.pstMyCh_->Zahyou.Muki;
//         enMukiZType lastMukiZ = st_.pstMyCh_->Zahyou.MukiZ;
//         BOOL LastMukiKeep_f = FALSE;
//         BOOL autoPickUp_f = TRUE;
//         BOOL atlook_f = MyPad()->IsAutoLook(); //自動タゲ向き（ニュートラル）
//
//         //プレビュー中パス禁止
//         BOOL pabtn_f = MyPad()->IsPass() && (pmgGO_->pmgRf_->IsPrev() == FALSE);
//         BOOL pabtn2_f = MyPad()->IsPass2() && (pmgGO_->pmgRf_->IsPrev() == FALSE);
//         BOOL dgbtn_f = MyPad()->IsDodge();
//         BOOL dgbtn2_f = MyPad()->IsDodge2();
//         BOOL cabtn_f = MyPad()->IsCatch();
//         BOOL shbtn_f = MyPad()->IsShot();
//         BOOL shbtn2_f = MyPad()->IsShot2();
//         BOOL uppos_f = (pabtn_f || dgbtn_f);
//         BOOL dnpos_f = (cabtn_f || shbtn_f);
//
//         BOOL ng_f = FALSE;
//
//         //シュート以外の時はきゃっちよけがでないように
//         if (st_.pstMyTm_->smart_f_)
//         {
//             BOOL caok_f = (((pmgSG_->stBa_.Motion == bmShoot) && (pmgSG_->stBa_.PichTNo == st_.ensideNo_))
//                            || ((pmgSG_->stBa_.Motion == bmPass) && (pmgSG_->stBa_.PichTNo == st_.ensideNo_))
//                            || ((pmgSG_->stBa_.Motion == bmHold) && pmgSG_->stBa_.shotmotion_f));
//
//             if (caok_f == FALSE)
//             {
//                 //dgbtn_f = FALSE;
//                 //dgbtn2_f = FALSE;
//                 //ダッシュがにゅうりょくされてなければ
//                 cabtn_f = (MyPad()->IsCatch3() //左下短押しリリース
//                            && (MyPad()->IsDash(IsBall()) == FALSE)); //ダッシュにゅうりょく無し
//             }
//
//             if (st_.pstMyCh_->shotok_f_ == FALSE)
//             {
//                 if (shbtn2_f == FALSE)
//                 {
//                     st_.pstMyCh_->shotok_f_ = TRUE;
//                 }
//                 else
//                 {
//                     shbtn_f = FALSE; //キャッチ後ジャンプおしっぱで出てしまうのカット
//                 }
//             }
//
//             //ダッシュ中前フリックはシュート
//             if (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
//             {
//                 if (st_.pstMyCh_->Zahyou.DsMuki == mL)
//                 {
//                     shbtn_f |= MyPad()->IsDsShot(dxL);
//                 }
//                 else if (st_.pstMyCh_->Zahyou.DsMuki == mR)
//                 {
//                     shbtn_f |= MyPad()->IsDsShot(dxR);
//                 }
//             }
//
//             //スマホ操作ではおしっぱシュートは使わない
//             shbtn2_f = FALSE;
//         }
//
// #ifdef __K_DEBUG_SHIAI__
//         if (kdebug::DebugSystem::GetInstance()->IsEnemyLastOne())
//         {
//             // 今いるやつを全員殺す
//             for (s32 i = 0; i < DBMEMBER_INF; i++)
//             {
//                 pmgGO_->pmgCh_[st_.ensideNo_][i]->st_.pstMyCh_->ANGEL_f = TRUE;
//                 pmgGO_->pmgCh_[st_.ensideNo_][i]->st_.pstMyCh_->HP = 0;
//             }
//             pmgGO_->pmgTm_[st_.ensideNo_]->CheckChangePos();
//             // 一人を残して全員殺す
//             for (s32 i = 0; i < DBMEMBER_INF - 2; i++)
//             {
//                 pmgGO_->pmgCh_[st_.ensideNo_][i]->st_.pstMyCh_->ANGEL_f = TRUE;
//                 pmgGO_->pmgCh_[st_.ensideNo_][i]->st_.pstMyCh_->HP = 0;
//             }
//             pmgGO_->pmgTm_[st_.ensideNo_]->CheckChangePos();
//         }
// #endif
//
//         //チュートリアルのＮＧ行為（押し位置間違いチェック）
//         if (pmgEO_->stShiai_.IsTuto() && (st_.mysideNo_ == SIDE0))
//         {
//             switch (st_.pmgRf_->GetTutoNo())
//             {
//                 case sta_DodgeAct:
//                     if (dnpos_f)
//                     {
//                         st_.pmgRf_->SetTutoNG(st_.pmgRf_->GetTutoNo(), rta_dodge_ngpos);
//                         ng_f = TRUE;
//                     }
//                     break;
//                 case sta_WalkAct:
//                     if (uppos_f)
//                     {
//                         st_.pmgRf_->SetTutoNG(st_.pmgRf_->GetTutoNo(), rta_walk_ngpos);
//                         ng_f = TRUE;
//                     }
//                     break;
//                 case sta_JumpAct:
//                     if (uppos_f)
//                     {
//                         st_.pmgRf_->SetTutoNG(st_.pmgRf_->GetTutoNo(), rta_jump_ngpos);
//                         ng_f = TRUE;
//                     }
//                 case sta_CatchAct:
//                     if (uppos_f)
//                     {
//                         st_.pmgRf_->SetTutoNG(st_.pmgRf_->GetTutoNo(), rta_catch_ngpos);
//                         ng_f = TRUE;
//                     }
//                     break;
//                 case sta_DashAct:
//                     if (uppos_f)
//                     {
//                         st_.pmgRf_->SetTutoNG(st_.pmgRf_->GetTutoNo(), rta_dash_ngpos);
//                         ng_f = TRUE;
//                     }
//                     shbtn_f = FALSE;
//                     pabtn_f = FALSE;
//                     break;
//                 case sta_ShotAct:
//                     if (uppos_f)
//                     {
//                         st_.pmgRf_->SetTutoNG(st_.pmgRf_->GetTutoNo(), rta_shot_ngpos);
//                         ng_f = TRUE;
//                     }
//                     break;
//                 case sta_GameAct:
//                     break;
//                 default:
//                     ng_f = TRUE;
//                     break;
//             }
//         }
//
//         if (ng_f)
//         {
//             pabtn_f = FALSE;
//             pabtn2_f = FALSE;
//             dgbtn_f = FALSE;
//             dgbtn2_f = FALSE;
//             cabtn_f = FALSE;
//             shbtn_f = FALSE;
//         }
//
//         if (st_.pstMyCh_->Nomove_f)
//         {
//             st_.pstMyCh_->Nomove_f = FALSE;
//             return;
//         }
//
//         //シュートとパスは投げる瞬間にターゲットの方向を自動で向くようにする
//         LastMukiKeep_f = TRUE;
//
//         BOOL shotok_f = TRUE;
//
//         //スマート
//         if (st_.pstMyTm_->smart_f_)
//         {
//             //内野で立ち、歩きの時はシュートしない
//             if (IsInfield()
//                 && ((st_.pstMyCh_->Motion.Mtype == dbmtSt) || (st_.pstMyCh_->Motion.Mtype == dbmtWk)))
//             {
//                 shotok_f = FALSE;
//             }
//         }
//
// #ifdef __K_DEBUG_SHIAI__
//         kdebug::DebugSystem* pDs = kdebug::DebugSystem::GetInstance();
//         bool isAutoShot = FALSE;
//         // 自動シュート状態を取得
//         s32 step = pDs->GetAutoShootStep();
//         if (step == kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_STANDBY)
//         {
//             // この状態で手元にボールがないのがおかしいので
//             // 強制的に手元に戻す
//             // なのでこのif文内は例外処理
//             if ((pmgSG_->stBa_.Motion != bmHold)
//                 || (pmgSG_->stBa_.HoldTNo != 0)
//                 || (pmgSG_->stBa_.HoldPNo != 0))
//             {
//                 // パスできる状態になったら手元に戻す
//                 if (!IsNGPassTag(0))
//                 {
//                     // ボールが手元に帰ってくる処理
//                     pmgGO_->pmgBa_->baCommon_.ResetRefPos_Prev(FALSE);
//                 }
//                 return;
//             }
//             else
//             {
//                 // 操作キャラをボール持ってるやつに
//                 st_.pmgMyTm_->SetCtrlBallGet(0);
//             }
//
//             // 自動シュート終わりますよフラグが立っていれば投げない
//             if (!pDs->IsAutoShootEnd())
//             {
//                 isAutoShot = TRUE;
//             }
//         }
//         else
//         {
//             // もしもボールを自分が持っていて待っている状態であれば次のシーケンスに進めてしまう
//             if (pDs->IsReturnBall())
//             {
//                 // 自動シュート状態を取得
//                 if (step == kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_WAIT)
//                 {
//                     // すでに手に持っているので次のステートに移行する
//                     if ((pmgSG_->stBa_.Motion == bmHold)
//                         && (pmgSG_->stBa_.HoldTNo == 0)
//                         && (pmgSG_->stBa_.HoldPNo == 0))
//                     {
//                         pDs->SetAutoShootStep(kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_CHOOSE);
//                         pDs->SetReturnBallFlg(FALSE); // フラグを落とす
//                     }
//                     // 他人が持ってる
//                     else
//                     {
//                         // パスできる状態になったら手元に戻す
//                         if (!IsNGPassTag(0))
//                         {
//                             // ボールが手元に帰ってくる処理
//                             pmgGO_->pmgBa_->baCommon_.ResetRefPos_Prev(FALSE);
//                             return;
//                         }
//                     }
//                 }
//             }
//         }
// #endif // #ifdef __K_DEBUG_SHIAI__
//
//         switch (st_.pstMyCh_->Motion.Mtype)
//         {
//             case dbmtCr:
//                 break;
//             case dbmtSt:
//             case dbmtWk:
//             case dbmtDs:
//             case dbmtSl:
//             {
//                 switch (ATType)
//                 {
//                     case ATA: //攻撃/////////////////////////////////
//                         if (pabtn_f) //
//                         {
//                             //内野の場合後ろ内野内パスなので、相手の方向を向かないと行けない
//                             Passing(FALSE);
//                         }
// #ifdef __K_DEBUG_SHIAI__
//                         // シュート条件に自動シュートフラグも入れる
//                         else if ((shbtn_f && shotok_f) || isAutoShot) //シュート入力
// #else
//                             else
//                         if (shbtn_f && shotok_f) //シュート入力
// #endif // #ifdef __K_DEBUG_SHIAI__
//
//                         {
//                             //外野２３からＺ軸シュートのとき、一応相手の方向を向く
//                             LookTg(pmgSG_->stBa_.ShTgPNo, FALSE, atlook_f); // && (st_.pstMyCh_->Motion.Mtype == dbmtSt)
//
//                             //ダッシュ方向とシュート方向があっているときは振り返り扱いにしない
//                             BOOL nortst_f = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
//                                             && (st_.pstMyCh_->Zahyou.DsMuki == st_.pstMyCh_->Zahyou.Muki);
//
//                             //内野で向きに変わるときは振り向きシュート
//                             if (IsInfield()
//                                 && (nortst_f == FALSE)
//                                 && (lastMuki != st_.pstMyCh_->Zahyou.Muki)
//                                )
//                             {
//                                 SetMtype(dbmtRtSh);
//                             }
//                             else
//                             {
//                                 SetMtype(dbmtSh);
//                             }
// #ifdef __K_DEBUG_SHIAI__
//                             // シュート情報のログを書き出す
//                             kdebug::DebugSystem::GetInstance()->CreateShootDebugLog();
//                             // 自動シュートフラグを落とす
//                             if (isAutoShot)
//                             {
//                                 pDs->SetAutoShootStep(kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_SHOOT);
//                             }
// #endif // #ifdef __K_DEBUG_SHIAI__
//                         }
//                         else
//                         {
//                             if (shbtn2_f) //シュート入力おしっぱ
//                             {
//                                 st_.pstMyCh_->MirPass_c = 0;
//                             }
//
//                             if (st_.pstMyCh_->MirPass_c > 0) //ミラーパス状態
//                             {
//                                 if (pabtn2_f)
//                                 {
//                                     if (lib_num::UpToR(&st_.pstMyCh_->MirPass_c, MIRWAIT))
//                                     {
//                                         if (pmgSG_->stBa_.PaTgPNo != NGNUM)
//                                         {
//                                             if (st_.pmgMyTm_->st_.pmgMyCh_[pmgSG_->stBa_.PaTgPNo]->IsDashman())
//                                             {
//                                                 Passing(FALSE);
//                                             }
//                                             else
//                                             {
//                                                 st_.pstMyCh_->MirPass_c = 0;
//                                             }
//                                         }
//                                     }
//                                 }
//                                 else
//                                 {
//                                     st_.pstMyCh_->MirPass_c = 0;
//                                 }
//                             }
//
//                             if (st_.pstMyCh_->MirShot_c > 0) //ミラーシュート状態
//                             {
//                                 if (shbtn2_f && (pabtn2_f == FALSE)) //シュート入力おしっぱ
//                                 {
//                                     if (lib_num::UpToR(&st_.pstMyCh_->MirShot_c, MIRWAIT))
//                                     {
//                                         LookTg(pmgSG_->stBa_.ShTgPNo, FALSE, TRUE); //居ないときはオートで探す
//
//                                         //ダッシュ方向とシュート方向があっているときは振り返り扱いにしない
//                                         BOOL nortst_f = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
//                                                         && (st_.pstMyCh_->Zahyou.DsMuki == st_.pstMyCh_->Zahyou.Muki);
//
//                                         if (IsInfield()
//                                             && (nortst_f == FALSE)
//                                             && (lastMuki != st_.pstMyCh_->Zahyou.Muki)
//                                            )
//                                         {
//                                             SetMtype(dbmtRtSh);
//                                         }
//                                         else
//                                         {
//                                             SetMtype(dbmtSh);
//                                         }
//                                     }
//                                 }
//                                 else
//                                 {
//                                     st_.pstMyCh_->MirShot_c = 1;
//                                 }
//                             }
//                         }
//                         break;
//
//                     case ATD: //守備/////////////////////////////////////
//                         if (dgbtn_f) //避けボタン
//                         {
//                             SetMtype(dbmtDg);
//                             CatchSE();
//                             if (IsPickUpPos(FALSE)) BallGet(FALSE, FALSE);
//                         }
//                         else if (cabtn_f && (st_.pstMyCh_->CatchW_c == 0)) //キャッチ入力
//                         {
//                             //キャッチもボール方向向くようにしてみる
//                             SetCatchMuki();
//                             SetMtype(dbmtCa);
//                         }
//                         else if (autoPickUp_f && IsPickUpPos(FALSE)) //自動拾い★
//                         {
//                             BallGet(FALSE, FALSE);
//                         }
//                         break;
//
//                     case ATF: //拾い
//                         if ((cabtn_f || dgbtn_f) //キャッチ入力
//                             && (st_.pstMyCh_->CatchW_c == 0))
//                         {
//                             //キャッチもボール方向向くようにしてみる
//                             SetCatchMuki();
//                             SetMtype(dbmtCa);
//                         }
//                         else if (autoPickUp_f && IsPickUpPos(FALSE)) //自動拾い★
//                         {
//                             BallGet(FALSE, FALSE);
//                         }
//                         break;
//                 }
//             }
//                 break;
//             case dbmtJCa:
//                 if (IsPickUpPos(TRUE))
//                 {
//                     BallGet(FALSE, FALSE);
//                 }
//                 break;
//             case dbmtCa:
//                 if (IsPickUpPos(TRUE))
//                 {
//                     BallGet(FALSE, FALSE);
//
//                     if (IsInfield()
//                         && (st_.pstMyCh_->Auto.AutoType == dbatFree)
//                         && (st_.pstMyCh_->Motion.IsMFlags(dbmfDs)))
//                     {
//                         if (((st_.pstMyCh_->Zahyou.DsMuki == mL)
//                              && MyPad()->IsCatchDash(dxL)) //キャッチ後ダッシュ継続入力
//                             || ((st_.pstMyCh_->Zahyou.DsMuki == mR)
//                                 && MyPad()->IsCatchDash(dxR)))
//                         {
//                             //継続なので向きセットも歩数リセットもいらない
//                             SetMtype(dbmtDs);
//                         }
//                     }
//                 }
//                 break;
//             case dbmtDg:
//             case dbmtJDg:
//                 //押しっぱなしで避け続けるようにする
//                 if (dgbtn2_f
//                     && (IsBall() == FALSE))
//                 {
//                     st_.pstMyCh_->Anime.Ani_c = 0; //こんなんでいいのだろうか
//                 }
//                 break;
//             case dbmtJUp:
//             case dbmtJDn:
// //      if ((st_.pstMyCh_->AirAct_f == FALSE)
// //        && st_.pstMyCh_->Motion.IsMFlags(dbmfAr)
// //        && ((st_.pstMyCh_->Zahyou.dY > 0) || (st_.pstMyCh_->Zahyou.Y >= (JPINVALIDHEIGHT * XYMAG))))
//                 if (
//                     ((st_.pstMyCh_->AirAct_f == FALSE)
//                      && st_.pstMyCh_->Motion.IsMFlags(dbmfAr)
//                      && ((st_.pstMyCh_->Zahyou.dY > 0) || (st_.pstMyCh_->Zahyou.Y >= (JPINVALIDHEIGHT * XYMAG)))
//                     )
//                     ||
//                     ( //マリオネット効果中
//                         (st_.pstMyCh_->symCtrl_f == TRUE)
//                         && st_.pstMyCh_->Motion.IsMFlags(dbmfAr)
//                     )
//                 )
//                 {
//                     if (ATType == ATA)
//                     {
//                         if (pabtn_f)
//                         {
//                             Passing(TRUE);
//                         }
//                         else if (MyPad()->IsJumpShot()) //ジャンプシュート入力
//                         {
//                             LookTg(pmgSG_->stBa_.ShTgPNo, FALSE, atlook_f);
//                             SetMtype(dbmtJSh);
//                         }
//                         else
//                         {
//                             if (shbtn2_f) //シュート入力おしっぱ
//                             {
//                                 st_.pstMyCh_->MirPass_c = 0;
//                             }
//
//                             if (st_.pstMyCh_->MirPass_c > 0) //ミラーパス状態
//                             {
//                                 if (pabtn2_f)
//                                 {
//                                     if (lib_num::UpToR(&st_.pstMyCh_->MirPass_c, MIRWAIT))
//                                     {
//                                         if (pmgSG_->stBa_.PaTgPNo != NGNUM)
//                                         {
//                                             if (st_.pmgMyTm_->st_.pmgMyCh_[pmgSG_->stBa_.PaTgPNo]->IsDashman())
//                                             {
//                                                 Passing(TRUE);
//                                             }
//                                             else
//                                             {
//                                                 st_.pstMyCh_->MirPass_c = 0;
//                                             }
//                                         }
//                                     }
//                                 }
//                                 else
//                                 {
//                                     st_.pstMyCh_->MirPass_c = 0;
//                                 }
//                             }
//
//                             if (st_.pstMyCh_->MirShot_c > 0) //ミラーシュート状態
//                             {
//                                 if (shbtn2_f && (pabtn2_f == FALSE)) //シュート入力おしっぱ
//                                 {
//                                     if (lib_num::UpToR(&st_.pstMyCh_->MirShot_c, MIRWAIT))
//                                     {
//                                         LookTg(pmgSG_->stBa_.ShTgPNo, FALSE, TRUE);
//                                         SetMtype(dbmtJSh);
//                                     }
//                                 }
//                                 else
//                                 {
//                                     st_.pstMyCh_->MirShot_c = 1;
//                                 }
//                             }
//                         }
//                     }
//                     else
//                     {
//                         if (dgbtn_f)
//                         {
//                             SetMtype(dbmtJDg);
//                             CatchSE();
//                             if ((st_.pmgMyTm_->st_.pstMyTm_->CtrlNo == pmgSG_->stBa_.PaTgPNo)
//                                 && (pmgSG_->stBa_.Motion == bmPass)
//                                 && (pmgSG_->stBa_.PaTgTNo == st_.mysideNo_)
//                                 && (pmgSG_->stBa_.PaTgPNo <= (s32)dbpoI3))
//                             {
//                                 st_.pmgMyTm_->SetCtrl(st_.pmgMyTm_->st_.pstMyTm_->CvrNo);
//                                 st_.pmgMyTm_->st_.pmgMyCh_[st_.pmgMyTm_->st_.pstMyTm_->CtrlNo]->st_.pstMyCh_->Nomove_f = TRUE;
//                                 st_.pmgMyTm_->SeekCover(st_.pmgMyTm_->st_.pstMyTm_->CtrlNo, pmgSG_->stBa_.PichPNo, pmgSG_->stBa_.PaTgPNo, TRUE); //新しいカバーマン
//                             }
//                         }
//                         else if (cabtn_f)
//                         {
//                             //キャッチもボール方向向くようにしてみる
//                             SetCatchMuki();
//                             SetMtype(dbmtJCa);
//                             //                            CatchSE();
//                             if (IsPickUpPos(TRUE)) BallGet(FALSE, FALSE);
//                         }
//                         else if (autoPickUp_f && IsPickUpPos(FALSE)) //自動拾い★
//                         {
//                             BallGet(FALSE, FALSE);
//                         }
//                     }
//                 }
//                 break;
//             case dbmtSh: //振り向きシュート
//                 if ((st_.pstMyCh_->Utrun_f == FALSE)
//                     && IsBall()
//                     && IsInfield()
//                     //&& (st_.pstMyCh_->Motion.IsMFlags(dbmfDs) || st_.pstMyCh_->Motion.IsMFlags(dbmfSlip))
//                    )
//                 {
//                     BOOL utrn_f = FALSE;
//                     if ((st_.pstMyCh_->Zahyou.Muki == mL)
//                         && MyPad()->IsWalk2(dxR)
//                         && (st_.mysideNo_ == 0))
//                     {
//                         utrn_f = TRUE;
//                         st_.pstMyCh_->Zahyou.Muki = mR;
//                         st_.pstMyCh_->Zahyou.MukiZ = mzN;
//                         MyState.Shoot.Angle12 = 1; //1 2 3 4
//                         SetMukiAgl(FALSE, TRUE, FALSE, FALSE);
//                     }
//                     else if ((st_.pstMyCh_->Zahyou.Muki == mR)
//                              && MyPad()->IsWalk2(dxL)
//                              && (st_.mysideNo_ == 1))
//                     {
//                         utrn_f = TRUE;
//                         st_.pstMyCh_->Zahyou.Muki = mL;
//                         st_.pstMyCh_->Zahyou.MukiZ = mzN;
//                         MyState.Shoot.Angle12 = 7; //7 8 9 10
//                         SetMukiAgl(TRUE, FALSE, FALSE, FALSE);
//                     }
//
//                     if (utrn_f)
//                     {
//                         SetMtype(dbmtRtSh);
//                         //ダッシュに復帰
//                         if (st_.pstMyCh_->Motion.IsMFlags(dbmfSlip))
//                         {
//                             st_.pstMyCh_->Motion.MFlags |= dbmfDs;
//                         }
//                     }
//                 }
//                 break;
//         }
//
//         //モーション変更前の向き
//         if (LastMukiKeep_f)
//         {
//             st_.pstMyCh_->LastMuki = lastMuki;
//             st_.pstMyCh_->LastMukiZ = lastMukiZ;
//         }
    }
}