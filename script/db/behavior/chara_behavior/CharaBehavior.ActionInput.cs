﻿using System;

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
                    //       if ((IsBall() == false)
                    //         && (st_.pstMyCh_.Motion.Mtype == dbmtJDg))
                    //       {
                    //         st_.pstMyCh_.Anime.Ani_c = 0;//こうやる必要ないか？
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
        //   switch (st_.pstMyCh_.Auto.AutoType)
        //   {
        //     //自由
        //   case dbatFree:
        //     if (IsSelfControl)
        //     {
        //       //自分で操作**********
        //       pCommon_.SelfCtrl(ATType);
        //     }
        //     else
        //     {
        //       if ((IsBall() == false)
        //         && (st_.pstMyCh_.Motion.Mtype == dbmtJDg))
        //       {
        //         st_.pstMyCh_.Anime.Ani_c = 0;//こうやる必要ないか？
        //       }
        //
        //       pCommon_.AutoPickUp();
        //       //CheckFreeBallPickUp();
        //     }
        //     break;
        //
        //     //ポジショニング
        //   case dbatDfPos:
        //     pCommon_.AutoPickUp();
        //     //CheckFreeBallPickUp();
        //     break;
        //
        //     //戻り
        //   case dbatReturn:
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
        //     break;
        //
        //     //拾いに行く
        //   case dbatGet:
        //     switch (st_.pstMyCh_.Motion.Mtype)
        //     {
        //     case dbmtSt:
        //     case dbmtWk:
        //     case dbmtDs:
        //       pCommon_.AutoPickUp();//自動拾い
        //       break;
        //     }
        //     break;
        //
        //     //よけ
        //   case dbatDg:
        //   {
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
        PaTagSet();

        //拾った時点でタゲ無しの時はタゲだけ無理矢理近いキャラから取った方が無難かも
        if (st_.pmgEnTm_.IsAllDead() == false)
        {
            //シュートタゲは向き反映
            int tstg = GetTag(false);
            if (tstg == NGNUM)
            {
                if (IsSelfControl == false)
                {
                    //CPUだけ無理矢理。後で外すかも
                    pmgSG_.stBa_.ShTgTNo = st_.ensideNo_;
                    pmgSG_.stBa_.ShTgPNo = st_.pmgEnTm_.st_.pstMyTm_.CtrlNo;
                }
            }
            else
            {
                //st_.pmgEnTm_.SetCtrl(tstg);
                pmgSG_.stBa_.ShTgTNo = st_.ensideNo_;
                pmgSG_.stBa_.ShTgPNo = tstg;
            }

            //カーソルは強制的に内野
            int tEnctrl = GetTag(true);
            if (tEnctrl != NGNUM) //ないとはおもうが
            {
                st_.pmgEnTm_.SetCtrl(tEnctrl);
            }
        }
#ifdef __K_DEBUG_SHIAI__
        kdebug::DebugSystem* pDs = kdebug::DebugSystem::GetInstance();
        if (pDs.IsReturnBall())
        {
            if (st_.posNo_ != 0)
            {
                // ボールが手元に帰ってくる処理
                pmgGO_.pmgBa_.baCommon_.ResetRefPos_Prev(false);

                // 操作キャラを強制的にボール持ってる人に
                //st_.pmgTm_[SIDE0].st_.pmgMyTm_.SetCtrlBallGet(0);
            }

            // 自動シュート状態を取得
            int step = pDs.GetAutoShootStep();
            if (step == kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_WAIT)
            {
                pDs.SetReturnBallFlg(false); // フラグを落とす
                pDs.SetAutoShootStep(kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_CHOOSE);
            }
        }
#endif // #ifdef __K_DEBUG_SHIAI__
    }

    private void PaTagSet()
    {
        DirectionXType infieldDirectionX = MySideIndex == 0
            ? DirectionXType.Left
            : DirectionXType.Right;

        int ptg = MyState.Order.IsInfield
            ? GetNaiyaPassTag()
            : GetGaiyaPassTag();

        pmgSG_.stBa_.PaTgTNo = st_.mysideNo_;

        if (ptg == NGNUM)
        {
            //パスタゲがパス出せないとき
            NGPaTagShift();
        }
        else
        {
            pmgSG_.stBa_.PaTgPNo = ptg;
        }

        //パスカットキャラセット
        PaCtTagSet();
    }

    //内野パスタゲセット★
    private OrderIndexType GetNaiyaPassTag()
    {
        var AllNoTag_f = true; //完全にタゲが居ない
        var NoTag_f = true; //向き方向にタゲが居ない

        var passDirectionX = MyState.Coordinate.DirectionX;
        var passDirectionZ = MyState.Coordinate.DirectionZ;

        var CrsL_f = false;
        var CrsR_f = false;
        var CrsU_f = false;
        var CrsD_f = false;

        if (IsSelfControl)
        {
            CrsL_f = MyPad.KeyLeft.IsPressed; //パス方向入力
            CrsR_f = MyPad.KeyRight.IsPressed;
            CrsU_f = MyPad.KeyUp.IsPressed;
            CrsD_f = MyPad.KeyDown.IsPressed;

            // //ここ再チェック
            // if (mid::midIsTBL())
            // {
            //     if (CrsL_f)
            //     {
            //         paMuki = mL;
            //         //Z方向を無視
            //         if ((CrsU_f || CrsD_f) == false) paMukiZ = mzN;
            //     }
            //     else if (CrsR_f)
            //     {
            //         paMuki = mR;
            //         //Z方向を無視
            //         if ((CrsU_f || CrsD_f) == false) paMukiZ = mzN;
            //     }
            //
            //     if (CrsU_f)
            //     {
            //         paMukiZ = mzB;
            //         //X方向を無視
            //         if ((CrsL_f || CrsR_f) == false) paMuki = mN;
            //     }
            //     else if (CrsD_f)
            //     {
            //         paMukiZ = mzF;
            //         //X方向を無視
            //         if ((CrsL_f || CrsR_f) == false) paMuki = mN;
            //     }
            // }
        }

        //内野方向を向いてる
        var infMuki_f = MySideIndex == 0
            ? passDirectionX == DirectionXType.Left
            : passDirectionX == DirectionXType.Right;

        bool infCrs_f = (MySideIndex == 0 && CrsL_f) || (MySideIndex == 1 && CrsR_f);

        bool enmCrs_f = (MySideIndex == 0 && CrsR_f) || (MySideIndex == 1 && CrsL_f);

        //十字入ってない
        bool neutral_f = (CrsL_f || CrsR_f || CrsU_f || CrsD_f) == false;

        //左コート時、内野内で一番右にいる
        bool topPos_f = true;

        bool frontPos_f = true; //一番手前にいる
        bool backPos_f = true; //一番奥にいる

        //ダッシュマンへパス
        bool isDashmanPass = MyTeamState.PositionState.DashmanNum > 0;

        int distO2 = Math.Abs(MyState.Coordinate.Z - Defines.DBCRT_BL);
        int distO3 = Math.Abs(MyState.Coordinate.Z - Defines.DBCRT_FL);
        bool nearO2_f = (distO2 < distO3);

        //優先順位初期化
        // for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
        // {
        //     tgOrd[i] = NGNUM;
        // }

        var sltgXZ = TmpStateManager.Instance.TmpState.sltgXZ;
        var sltg_f = TmpStateManager.Instance.TmpState.sltg_f;
        var tgOrd = TmpStateManager.Instance.TmpState.tgOrd;
        var sortDt = TmpStateManager.Instance.TmpState.sortDt;

        //内野全員との距離を取る
        for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
        {
            if (order == OrderIndex)
            {
                continue;
            }

            TmpStateManager.Instance.TmpState.sltgXZ[order] = MyState.Coordinate.DistanceXZ(MySideOrders[order].Coordinate);
        }

        //パスが出せるダッシュマンがいるか
        if (isDashmanPass) //ダッシュマンへパス
        {
            //内野全員との角度を取る
            for (var order = 0; order < Defines.DBMEMBER_INF; ++order)
            {
                if (order == OrderIndex)
                {
                    continue;
                }

                var chara = CharaBehaviorManager.Instance.GetChara(MySideIndex, order);

                if (chara.IsDashman == false)
                {
                    continue;
                }

                //向き方向に居る
                sltg_f[order] = enNaiyaTag.TGOK;

                NoTag_f = false; //一人でも向き方向にタゲが見つかった

                //右にダッシュマンがいる
                if (chara.LeftCourtX > LeftCourtX)
                {
                    topPos_f = false;
                }

                //奥にダッシュマンがいる
                if (chara.MyState.Coordinate.Z > MyState.Coordinate.Z)
                {
                    backPos_f = false;
                }

                //手前にダッシュマンがいる
                if (chara.MyState.Coordinate.Z < MyState.Coordinate.Z)
                {
                    frontPos_f = false;
                }
            }

            //パスが出せるダッシュマンがいない
            if (NoTag_f)
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
                    sltg_f[order] = enNaiyaTag.TGNG;
                    continue;
                }
                AllNoTag_f = false; //一応タゲ可能は人はいる

                var chara = CharaBehaviorManager.Instance.GetChara(MySideIndex, order);

                if (IsCheckNoAgl(chara.MyState.Coordinate.X, chara.MyState.Coordinate.Z))
                {
                    //向きに居ない
                    sltg_f[order] = enNaiyaTag.TGNOAGL;
                }
                else
                {
                    //向き方向に居る
                    sltg_f[order] = enNaiyaTag.TGOK;
                    NoTag_f = false; //一人でも向き方向にタゲが見つかった
                }

                //誰か右にいる
                if (chara.LeftCourtX > LeftCourtX)
                {
                    topPos_f = false;
                }

                //奥にいる
                if (chara.MyState.Coordinate.Z > MyState.Coordinate.Z)
                {
                    backPos_f = false;
                }

                //手前にいる
                if (chara.MyState.Coordinate.Z < MyState.Coordinate.Z)
                {
                    frontPos_f = false;
                }
            }
        }

        //ダッシュマンいるとき
        if (isDashmanPass)
        {
            if (topPos_f && enmCrs_f) //先頭で右→が入ってるのときのみ
            {
                if (CrsU_f)
                {
                    return OrderIndexType.Outfield2; //右上
                }

                if (CrsD_f)
                {
                    return OrderIndexType.Outfield3; //右下
                }

                //右のみ
                return nearO2_f
                    ? OrderIndexType.Outfield2
                    : OrderIndexType.Outfield3;
            }

            if (frontPos_f) //一番手前に居る
            {
                if (CrsD_f)
                {
                    return OrderIndexType.Outfield3; //右下
                }
            }

            if (backPos_f) //一番奥に居る
            {
                if (CrsU_f)
                {
                    return OrderIndexType.Outfield2; //右下
                }
            }
        }
        else if ((infMuki_f == false && topPos_f) || AllNoTag_f) //右向き時しかも先頭もしくは孤立(→外野パス)
        {
            if (MyState.Motion.MotionType == CharaMotionType.Ds) //ダッシュ中
            {
                if (IsSelfControl)
                {
                    if (CrsU_f) return OrderIndexType.Outfield2; //上
                    if (CrsD_f) return OrderIndexType.Outfield3; //下
                    if (enmCrs_f) return OrderIndexType.Outfield4; //右

                    return nearO2_f
                        ? OrderIndexType.Outfield2
                        : OrderIndexType.Outfield3;
                }

                return MyState.Auto.DirectionZ switch
                {
                    DirectionZType.Backward => OrderIndexType.Outfield2,
                    DirectionZType.Forward => OrderIndexType.Outfield3
                };
            }

            switch (passDirectionZ)
            {
                case DirectionZType.Backward:
                    return OrderIndexType.Outfield2;
                case DirectionZType.Forward:
                    return OrderIndexType.Outfield3;
                default:
                    if (enmCrs_f) return OrderIndexType.Outfield4;
                    return nearO2_f
                        ? OrderIndexType.Outfield2
                        : OrderIndexType.Outfield3;
            }

            //しかも先頭もしくは孤立は4番でFA
            return OrderIndexType.Outfield4;
        }

        ここから
        
        int f = 0;
        for (int i = 0; i < DBMEMBER_INF; ++i)
        {
            //向き方向に人なしのとき

            sortDt[i] = 0; //初期化

            if ((sltg_f[i] == TGOK)
                || (NoTag_f && (sltg_f[i] != TGNG)))
            {
                if (neutral_f) //ニュートラル
                {
                    sortDt[i] = (int)sltgXZ[i]; //内野間は距離が近い人
                }
                else
                {
                    //ダッシュマンが居るときは現在Ｚではなく、目標Ｚ
                    int tgZ = (isDashmanPass)
                        ? st_.pmgMyTm_.st_.pmgMyCh_[i].st_.pstMyCh_.DashmanTgZ
                        : st_.pmgMyTm_.st_.pmgMyCh_[i].MyState.Coordinate.Z;

                    //上
                    if (CrsU_f)
                    {
                        sortDt[i] = -tgZ; //Ｚのマイナス（上ほど優先）
                    }
                    else if (CrsD_f) //下
                    {
                        sortDt[i] = +tgZ; //Ｚ（下ほど優先）
                    }

                    //上下が入ってるとき用に合計値
                    if (CrsL_f) //左
                    {
                        sortDt[i] += (st_.pmgMyTm_.st_.pmgMyCh_[i].MyState.Coordinate.X); //Ｘ（左ほど優先）
                    }
                    else if (CrsR_f) //右
                    {
                        sortDt[i] -= (st_.pmgMyTm_.st_.pmgMyCh_[i].MyState.Coordinate.X); //Ｘのマイナス（右ほど優先）
                    }
                }
                tgOrd[f++] = i;
            }
        }

        //ソート
        for (int i = 0; i < (DBMEMBER_INF - 1); ++i)
        {
            for (int i2 = 0; i2 < (DBMEMBER_INF - 1); i2++)
            {
                if (i == i2) continue; //同じ

                if ((tgOrd[i] != NGNUM) && (tgOrd[i2] != NGNUM))
                {
                    if (sortDt[tgOrd[i]] < sortDt[tgOrd[i2]]) //小さい方優先
                    {
                        int tmp;
                        tmp = tgOrd[i2];
                        tgOrd[i2] = tgOrd[i];
                        tgOrd[i] = tmp;
                    }
                }
            }
        }

        //ソート１位
        int res = tgOrd[0];

        return res;
    }

    //角度に入っていない
    private bool IsCheckNoAgl(int targetX, int targetZ)
    {
        var isInAngle = MyState.Coordinate.DirectionZ switch
        {
            DirectionZType.Forward => targetZ < MyState.Coordinate.Z,
            DirectionZType.Neutral => true,
            DirectionZType.Backward => targetZ > MyState.Coordinate.Z
        };

        if (isInAngle)
        {
            isInAngle = MyState.Coordinate.DirectionX switch
            {
                DirectionXType.Left => targetX < MyState.Coordinate.X,
                DirectionXType.Neutral => true,
                DirectionXType.Right => targetX > MyState.Coordinate.X,
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

    //パスタゲにならない★★再確認
    bool IsNGPassTag(int order)
    {
        if (order == OrderIndex) //自分
        {
            return true; //パス不可
        }

        var chara = CharaBehaviorManager.Instance.GetChara(MySideIndex, order);

        //★ダッシュマンはだいじょぶ
        if (chara.IsDashman)
            //&& ((st_.pmgMyTm_->st_.pmgMyCh_[pos]->MyState.Coordinate.dY >= (-XYMAG))
            //  || (st_.pstMyCh_->MirPass_c > 0)))//下降ではない
        {
            return false;
        }

        //空中の人はパスタゲにならないように
        return chara.IsFree(true) == false
               || chara.MyState.Motion.HasFlag(CharaMotionFlag.Ar);
    }

    //自分で操作
    private void ControlSelf()
    {
        var actionType = GetActionType();
//
//         const int MIRWAIT = 4;
//         //モーション変更前の向き
//         enMukiType lastMuki = MyState.Coordinate.Muki;
//         enMukiZType lastMukiZ = MyState.Coordinate.MukiZ;
//         bool LastMukiKeep_f = false;
//         bool autoPickUp_f = true;
//         bool atlook_f = MyPad.IsAutoLook(); //自動タゲ向き（ニュートラル）
//
//         //プレビュー中パス禁止
//         bool pabtn_f = MyPad.IsPass() && (pmgGO_.pmgRf_.IsPrev() == false);
//         bool pabtn2_f = MyPad.IsPass2() && (pmgGO_.pmgRf_.IsPrev() == false);
//         bool dgbtn_f = MyPad.IsDodge();
//         bool dgbtn2_f = MyPad.IsDodge2();
//         bool cabtn_f = MyPad.IsCatch();
//         bool shbtn_f = MyPad.IsShot();
//         bool shbtn2_f = MyPad.IsShot2();
//         bool uppos_f = (pabtn_f || dgbtn_f);
//         bool dnpos_f = (cabtn_f || shbtn_f);
//
//         bool ng_f = false;
//
//         //シュート以外の時はきゃっちよけがでないように
//         if (st_.pstMyTm_.smart_f_)
//         {
//             bool caok_f = (((pmgSG_.stBa_.Motion == bmShoot) && (pmgSG_.stBa_.PichTNo == st_.ensideNo_))
//                            || ((pmgSG_.stBa_.Motion == bmPass) && (pmgSG_.stBa_.PichTNo == st_.ensideNo_))
//                            || ((pmgSG_.stBa_.Motion == bmHold) && pmgSG_.stBa_.shotmotion_f));
//
//             if (caok_f == false)
//             {
//                 //dgbtn_f = false;
//                 //dgbtn2_f = false;
//                 //ダッシュがにゅうりょくされてなければ
//                 cabtn_f = (MyPad.IsCatch3() //左下短押しリリース
//                            && (MyPad.IsDash(IsBall()) == false)); //ダッシュにゅうりょく無し
//             }
//
//             if (st_.pstMyCh_.shotok_f_ == false)
//             {
//                 if (shbtn2_f == false)
//                 {
//                     st_.pstMyCh_.shotok_f_ = true;
//                 }
//                 else
//                 {
//                     shbtn_f = false; //キャッチ後ジャンプおしっぱで出てしまうのカット
//                 }
//             }
//
//             //ダッシュ中前フリックはシュート
//             if (st_.pstMyCh_.Motion.IsMFlags(dbmfDs))
//             {
//                 if (MyState.Coordinate.DsMuki == mL)
//                 {
//                     shbtn_f |= MyPad.IsDsShot(dxL);
//                 }
//                 else if (MyState.Coordinate.DsMuki == mR)
//                 {
//                     shbtn_f |= MyPad.IsDsShot(dxR);
//                 }
//             }
//
//             //スマホ操作ではおしっぱシュートは使わない
//             shbtn2_f = false;
//         }
//
// #ifdef __K_DEBUG_SHIAI__
//         if (kdebug::DebugSystem::GetInstance().IsEnemyLastOne())
//         {
//             // 今いるやつを全員殺す
//             for (int i = 0; i < DBMEMBER_INF; i++)
//             {
//                 pmgGO_.pmgCh_[st_.ensideNo_][i].st_.pstMyCh_.ANGEL_f = true;
//                 pmgGO_.pmgCh_[st_.ensideNo_][i].st_.pstMyCh_.HP = 0;
//             }
//             pmgGO_.pmgTm_[st_.ensideNo_].CheckChangePos();
//             // 一人を残して全員殺す
//             for (int i = 0; i < DBMEMBER_INF - 2; i++)
//             {
//                 pmgGO_.pmgCh_[st_.ensideNo_][i].st_.pstMyCh_.ANGEL_f = true;
//                 pmgGO_.pmgCh_[st_.ensideNo_][i].st_.pstMyCh_.HP = 0;
//             }
//             pmgGO_.pmgTm_[st_.ensideNo_].CheckChangePos();
//         }
// #endif
//
//         //チュートリアルのＮＧ行為（押し位置間違いチェック）
//         if (pmgEO_.stShiai_.IsTuto() && (st_.mysideNo_ == SIDE0))
//         {
//             switch (st_.pmgRf_.GetTutoNo())
//             {
//                 case sta_DodgeAct:
//                     if (dnpos_f)
//                     {
//                         st_.pmgRf_.SetTutoNG(st_.pmgRf_.GetTutoNo(), rta_dodge_ngpos);
//                         ng_f = true;
//                     }
//                     break;
//                 case sta_WalkAct:
//                     if (uppos_f)
//                     {
//                         st_.pmgRf_.SetTutoNG(st_.pmgRf_.GetTutoNo(), rta_walk_ngpos);
//                         ng_f = true;
//                     }
//                     break;
//                 case sta_JumpAct:
//                     if (uppos_f)
//                     {
//                         st_.pmgRf_.SetTutoNG(st_.pmgRf_.GetTutoNo(), rta_jump_ngpos);
//                         ng_f = true;
//                     }
//                 case sta_CatchAct:
//                     if (uppos_f)
//                     {
//                         st_.pmgRf_.SetTutoNG(st_.pmgRf_.GetTutoNo(), rta_catch_ngpos);
//                         ng_f = true;
//                     }
//                     break;
//                 case sta_DashAct:
//                     if (uppos_f)
//                     {
//                         st_.pmgRf_.SetTutoNG(st_.pmgRf_.GetTutoNo(), rta_dash_ngpos);
//                         ng_f = true;
//                     }
//                     shbtn_f = false;
//                     pabtn_f = false;
//                     break;
//                 case sta_ShotAct:
//                     if (uppos_f)
//                     {
//                         st_.pmgRf_.SetTutoNG(st_.pmgRf_.GetTutoNo(), rta_shot_ngpos);
//                         ng_f = true;
//                     }
//                     break;
//                 case sta_GameAct:
//                     break;
//                 default:
//                     ng_f = true;
//                     break;
//             }
//         }
//
//         if (ng_f)
//         {
//             pabtn_f = false;
//             pabtn2_f = false;
//             dgbtn_f = false;
//             dgbtn2_f = false;
//             cabtn_f = false;
//             shbtn_f = false;
//         }
//
//         if (st_.pstMyCh_.Nomove_f)
//         {
//             st_.pstMyCh_.Nomove_f = false;
//             return;
//         }
//
//         //シュートとパスは投げる瞬間にターゲットの方向を自動で向くようにする
//         LastMukiKeep_f = true;
//
//         bool shotok_f = true;
//
//         //スマート
//         if (st_.pstMyTm_.smart_f_)
//         {
//             //内野で立ち、歩きの時はシュートしない
//             if (IsInfield()
//                 && ((st_.pstMyCh_.Motion.Mtype == dbmtSt) || (st_.pstMyCh_.Motion.Mtype == dbmtWk)))
//             {
//                 shotok_f = false;
//             }
//         }
//
// #ifdef __K_DEBUG_SHIAI__
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
//
//         switch (st_.pstMyCh_.Motion.Mtype)
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
//                             Passing(false);
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
//                             LookTg(pmgSG_.stBa_.ShTgPNo, false, atlook_f); // && (st_.pstMyCh_.Motion.Mtype == dbmtSt)
//
//                             //ダッシュ方向とシュート方向があっているときは振り返り扱いにしない
//                             bool nortst_f = (st_.pstMyCh_.Motion.IsMFlags(dbmfDs))
//                                             && (MyState.Coordinate.DsMuki == MyState.Coordinate.Muki);
//
//                             //内野で向きに変わるときは振り向きシュート
//                             if (IsInfield()
//                                 && (nortst_f == false)
//                                 && (lastMuki != MyState.Coordinate.Muki)
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
//                             kdebug::DebugSystem::GetInstance().CreateShootDebugLog();
//                             // 自動シュートフラグを落とす
//                             if (isAutoShot)
//                             {
//                                 pDs.SetAutoShootStep(kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_SHOOT);
//                             }
// #endif // #ifdef __K_DEBUG_SHIAI__
//                         }
//                         else
//                         {
//                             if (shbtn2_f) //シュート入力おしっぱ
//                             {
//                                 st_.pstMyCh_.MirPass_c = 0;
//                             }
//
//                             if (st_.pstMyCh_.MirPass_c > 0) //ミラーパス状態
//                             {
//                                 if (pabtn2_f)
//                                 {
//                                     if (lib_num::UpToR(&st_.pstMyCh_.MirPass_c, MIRWAIT))
//                                     {
//                                         if (pmgSG_.stBa_.PaTgPNo != NGNUM)
//                                         {
//                                             if (st_.pmgMyTm_.st_.pmgMyCh_[pmgSG_.stBa_.PaTgPNo].IsDashman())
//                                             {
//                                                 Passing(false);
//                                             }
//                                             else
//                                             {
//                                                 st_.pstMyCh_.MirPass_c = 0;
//                                             }
//                                         }
//                                     }
//                                 }
//                                 else
//                                 {
//                                     st_.pstMyCh_.MirPass_c = 0;
//                                 }
//                             }
//
//                             if (st_.pstMyCh_.MirShot_c > 0) //ミラーシュート状態
//                             {
//                                 if (shbtn2_f && (pabtn2_f == false)) //シュート入力おしっぱ
//                                 {
//                                     if (lib_num::UpToR(&st_.pstMyCh_.MirShot_c, MIRWAIT))
//                                     {
//                                         LookTg(pmgSG_.stBa_.ShTgPNo, false, true); //居ないときはオートで探す
//
//                                         //ダッシュ方向とシュート方向があっているときは振り返り扱いにしない
//                                         bool nortst_f = (st_.pstMyCh_.Motion.IsMFlags(dbmfDs))
//                                                         && (MyState.Coordinate.DsMuki == MyState.Coordinate.Muki);
//
//                                         if (IsInfield()
//                                             && (nortst_f == false)
//                                             && (lastMuki != MyState.Coordinate.Muki)
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
//                                     st_.pstMyCh_.MirShot_c = 1;
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
//                             if (IsPickUpPos(false)) BallGet(false, false);
//                         }
//                         else if (cabtn_f && (st_.pstMyCh_.CatchW_c == 0)) //キャッチ入力
//                         {
//                             //キャッチもボール方向向くようにしてみる
//                             SetCatchMuki();
//                             SetMtype(dbmtCa);
//                         }
//                         else if (autoPickUp_f && IsPickUpPos(false)) //自動拾い★
//                         {
//                             BallGet(false, false);
//                         }
//                         break;
//
//                     case ATF: //拾い
//                         if ((cabtn_f || dgbtn_f) //キャッチ入力
//                             && (st_.pstMyCh_.CatchW_c == 0))
//                         {
//                             //キャッチもボール方向向くようにしてみる
//                             SetCatchMuki();
//                             SetMtype(dbmtCa);
//                         }
//                         else if (autoPickUp_f && IsPickUpPos(false)) //自動拾い★
//                         {
//                             BallGet(false, false);
//                         }
//                         break;
//                 }
//             }
//                 break;
//             case dbmtJCa:
//                 if (IsPickUpPos(true))
//                 {
//                     BallGet(false, false);
//                 }
//                 break;
//             case dbmtCa:
//                 if (IsPickUpPos(true))
//                 {
//                     BallGet(false, false);
//
//                     if (IsInfield()
//                         && (st_.pstMyCh_.Auto.AutoType == dbatFree)
//                         && (st_.pstMyCh_.Motion.IsMFlags(dbmfDs)))
//                     {
//                         if (((MyState.Coordinate.DsMuki == mL)
//                              && MyPad.IsCatchDash(dxL)) //キャッチ後ダッシュ継続入力
//                             || ((MyState.Coordinate.DsMuki == mR)
//                                 && MyPad.IsCatchDash(dxR)))
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
//                     && (IsBall() == false))
//                 {
//                     st_.pstMyCh_.Anime.Ani_c = 0; //こんなんでいいのだろうか
//                 }
//                 break;
//             case dbmtJUp:
//             case dbmtJDn:
// //      if ((st_.pstMyCh_.AirAct_f == false)
// //        && st_.pstMyCh_.Motion.IsMFlags(dbmfAr)
// //        && ((MyState.Coordinate.dY > 0) || (MyState.Coordinate.Y >= (JPINVALIDHEIGHT * XYMAG))))
//                 if (
//                     ((st_.pstMyCh_.AirAct_f == false)
//                      && st_.pstMyCh_.Motion.IsMFlags(dbmfAr)
//                      && ((MyState.Coordinate.dY > 0) || (MyState.Coordinate.Y >= (JPINVALIDHEIGHT * XYMAG)))
//                     )
//                     ||
//                     ( //マリオネット効果中
//                         (st_.pstMyCh_.symCtrl_f == true)
//                         && st_.pstMyCh_.Motion.IsMFlags(dbmfAr)
//                     )
//                 )
//                 {
//                     if (ATType == ATA)
//                     {
//                         if (pabtn_f)
//                         {
//                             Passing(true);
//                         }
//                         else if (MyPad.IsJumpShot()) //ジャンプシュート入力
//                         {
//                             LookTg(pmgSG_.stBa_.ShTgPNo, false, atlook_f);
//                             SetMtype(dbmtJSh);
//                         }
//                         else
//                         {
//                             if (shbtn2_f) //シュート入力おしっぱ
//                             {
//                                 st_.pstMyCh_.MirPass_c = 0;
//                             }
//
//                             if (st_.pstMyCh_.MirPass_c > 0) //ミラーパス状態
//                             {
//                                 if (pabtn2_f)
//                                 {
//                                     if (lib_num::UpToR(&st_.pstMyCh_.MirPass_c, MIRWAIT))
//                                     {
//                                         if (pmgSG_.stBa_.PaTgPNo != NGNUM)
//                                         {
//                                             if (st_.pmgMyTm_.st_.pmgMyCh_[pmgSG_.stBa_.PaTgPNo].IsDashman())
//                                             {
//                                                 Passing(true);
//                                             }
//                                             else
//                                             {
//                                                 st_.pstMyCh_.MirPass_c = 0;
//                                             }
//                                         }
//                                     }
//                                 }
//                                 else
//                                 {
//                                     st_.pstMyCh_.MirPass_c = 0;
//                                 }
//                             }
//
//                             if (st_.pstMyCh_.MirShot_c > 0) //ミラーシュート状態
//                             {
//                                 if (shbtn2_f && (pabtn2_f == false)) //シュート入力おしっぱ
//                                 {
//                                     if (lib_num::UpToR(&st_.pstMyCh_.MirShot_c, MIRWAIT))
//                                     {
//                                         LookTg(pmgSG_.stBa_.ShTgPNo, false, true);
//                                         SetMtype(dbmtJSh);
//                                     }
//                                 }
//                                 else
//                                 {
//                                     st_.pstMyCh_.MirShot_c = 1;
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
//                             if ((st_.pmgMyTm_.st_.pstMyTm_.CtrlNo == pmgSG_.stBa_.PaTgPNo)
//                                 && (pmgSG_.stBa_.Motion == bmPass)
//                                 && (pmgSG_.stBa_.PaTgTNo == st_.mysideNo_)
//                                 && (pmgSG_.stBa_.PaTgPNo <= (int)dbpoI3))
//                             {
//                                 st_.pmgMyTm_.SetCtrl(st_.pmgMyTm_.st_.pstMyTm_.CvrNo);
//                                 st_.pmgMyTm_.st_.pmgMyCh_[st_.pmgMyTm_.st_.pstMyTm_.CtrlNo].st_.pstMyCh_.Nomove_f = true;
//                                 st_.pmgMyTm_.SeekCover(st_.pmgMyTm_.st_.pstMyTm_.CtrlNo, pmgSG_.stBa_.PichPNo, pmgSG_.stBa_.PaTgPNo, true); //新しいカバーマン
//                             }
//                         }
//                         else if (cabtn_f)
//                         {
//                             //キャッチもボール方向向くようにしてみる
//                             SetCatchMuki();
//                             SetMtype(dbmtJCa);
//                             //                            CatchSE();
//                             if (IsPickUpPos(true)) BallGet(false, false);
//                         }
//                         else if (autoPickUp_f && IsPickUpPos(false)) //自動拾い★
//                         {
//                             BallGet(false, false);
//                         }
//                     }
//                 }
//                 break;
//             case dbmtSh: //振り向きシュート
//                 if ((st_.pstMyCh_.Utrun_f == false)
//                     && IsBall()
//                     && IsInfield()
//                     //&& (st_.pstMyCh_.Motion.IsMFlags(dbmfDs) || st_.pstMyCh_.Motion.IsMFlags(dbmfSlip))
//                    )
//                 {
//                     bool utrn_f = false;
//                     if ((MyState.Coordinate.Muki == mL)
//                         && MyPad.IsWalk2(dxR)
//                         && (st_.mysideNo_ == 0))
//                     {
//                         utrn_f = true;
//                         MyState.Coordinate.Muki = mR;
//                         MyState.Coordinate.MukiZ = mzN;
//                         MyState.Shoot.Angle12 = 1; //1 2 3 4
//                         SetMukiAgl(false, true, false, false);
//                     }
//                     else if ((MyState.Coordinate.Muki == mR)
//                              && MyPad.IsWalk2(dxL)
//                              && (st_.mysideNo_ == 1))
//                     {
//                         utrn_f = true;
//                         MyState.Coordinate.Muki = mL;
//                         MyState.Coordinate.MukiZ = mzN;
//                         MyState.Shoot.Angle12 = 7; //7 8 9 10
//                         SetMukiAgl(true, false, false, false);
//                     }
//
//                     if (utrn_f)
//                     {
//                         SetMtype(dbmtRtSh);
//                         //ダッシュに復帰
//                         if (st_.pstMyCh_.Motion.IsMFlags(dbmfSlip))
//                         {
//                             st_.pstMyCh_.Motion.MFlags |= dbmfDs;
//                         }
//                     }
//                 }
//                 break;
//         }
//
//         //モーション変更前の向き
//         if (LastMukiKeep_f)
//         {
//             st_.pstMyCh_.LastMuki = lastMuki;
//             st_.pstMyCh_.LastMukiZ = lastMukiZ;
//         }
    }
}