using System;
using System.Diagnostics;

namespace db;

public partial class CharaBehavior
{

    private void SetMotionType(CharaMotionType motionType, bool isForce = false)
    {
        // モーション変更可能チェック
        if (EnableSetMotion(motionType, isForce) == false)
        {
            return;
        }

        // モーション差し替え
        motionType = ShiftMotionType(motionType);

        // パス待ちモーションの時は絵柄だけ変わるように、それ以外の時は初期化
        var isSwitchPassWait =
            (motionType == CharaMotionType.Ds && MyState.Motion.MotionNo == CharaMotionNo.PWDS)
            || (motionType == CharaMotionType.PWDs && MyState.Motion.MotionNo == CharaMotionNo.DS)
            || (motionType == CharaMotionType.Wk && MyState.Motion.MotionNo == CharaMotionNo.PWWKF)
            || (motionType == CharaMotionType.Wk && MyState.Motion.MotionNo == CharaMotionNo.PWWKN)
            || (motionType == CharaMotionType.Wk && MyState.Motion.MotionNo == CharaMotionNo.PWWKR)
            || (motionType == CharaMotionType.PWWk && MyState.Motion.MotionNo == CharaMotionNo.WKF)
            || (motionType == CharaMotionType.PWWk && MyState.Motion.MotionNo == CharaMotionNo.WKN)
            || (motionType == CharaMotionType.PWWk && MyState.Motion.MotionNo == CharaMotionNo.WKR);

        if (isSwitchPassWait == false)
        {
            MyState.Motion.MotionCount.Clear();
            MyState.Anime.Initialize();
            MyState.Catch.CatchCount.Clear();
            MyState.Dodge.EnemyCortDodgeCount.Clear();
            MyState.Move.DashAccelCount.Clear();
            MyState.Coordinate.FrictionCount.Clear();
            MyState.Air.IsLandSet = false;
            MyState.Shoot.IsUTurn = false;
            //     //TODO:地上についたら空中復帰消す これはIsMFlagsが変化したタイミングが適切
            //     if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr) == FALSE)
            //     {
            //       st_.pstMyCh_->AirRev_f = FALSE;
            //     }

            // キャッチ待ち時間クリア
            if (motionType != CharaMotionType.St
                && motionType != CharaMotionType.Wk
                && motionType != CharaMotionType.Ca
                && motionType != CharaMotionType.JCa)
            {
                MyState.Catch.ResetCacheWait();
            }

            // 立ちパス待ちカウンタ
            if (IsPassWait() == false)
            {
                MyState.Pass.PassStandWaitCount.Clear();
            }

            // ダウンカウンタクリア
            if (motionType != CharaMotionType.FlF
                && motionType != CharaMotionType.FlB
                && motionType != CharaMotionType.DnF
                && motionType != CharaMotionType.DnB
                && motionType != CharaMotionType.DnHF
                && motionType != CharaMotionType.DnHB)
            {
                MyState.Damage.DownCount.Clear();
            }

            // 歩数リセット
            // TODO:（ダッシュ以外にしなくてよいか確認）
            // ダッシュ継続以外
            if (motionType == CharaMotionType.St
                || motionType == CharaMotionType.Wk
                || (motionType == CharaMotionType.Ds && MyState.Motion.HasFlag(CharaMotionFlag.Ds) == false))
            {
                MyState.Move.MadStepCount.Clear();
                MyState.Shoot.Step.Clear();
            }

            if (motionType == CharaMotionType.ARv)
            {
                MyState.Air.IsAirAction = true;
            }

            if (motionType == CharaMotionType.DnF || motionType == CharaMotionType.DnB)
            {
                // ダウン効果音
                // SESet(seDownLv0);//SEならす
            }
            
            // シュートモーション時間
            if (motionType == CharaMotionType.Sh 
                || motionType == CharaMotionType.RtSh
                || motionType == CharaMotionType.JSh
                || motionType == CharaMotionType.RtJSh)
            {
                MyState.Shoot.ShootWaitCount = GetLevelRank(RankLevelType.ShStMotion);
                MyState.Shoot.ShootEndWaitCount = GetLevelRank(RankLevelType.ShEdMotion);
            }
            
            // Uターンシュート
            if (motionType == CharaMotionType.RtSh)
            {
                MyState.Coordinate.DirectionZ = DirectionZType.Neutral;
                MyState.Shoot.IsUTurn = true;
            }

        }

        switch (motionType)
        {
            // 立ち
            case CharaMotionType.St:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.STF,
                    CharaMotionFlag.JpOK | CharaMotionFlag.Muki | CharaMotionFlag.DmOK | CharaMotionFlag.AtCa,
                    MyState.Coordinate.DirectionZ);
                break;
            // ブレス
            case CharaMotionType.Breath:
                // 扱いは立ち状態
                MyState.Motion.SetMotionNo(CharaMotionType.St, CharaMotionNo.BRF,
                    CharaMotionFlag.JpOK | CharaMotionFlag.Muki | CharaMotionFlag.DmOK | CharaMotionFlag.AtCa,
                    MyState.Coordinate.DirectionZ);
                break;
            // 歩き
            case CharaMotionType.Wk:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.WKF,
                    CharaMotionFlag.JpOK | CharaMotionFlag.Muki | CharaMotionFlag.DmOK | CharaMotionFlag.AtCa,
                    MyState.Coordinate.DirectionZ);
                break;
            // ダッシュ
            case CharaMotionType.Ds:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.DS,
                    CharaMotionFlag.Ds | CharaMotionFlag.JpOK | CharaMotionFlag.Muki | CharaMotionFlag.AtCa);
                break;
            // ジャンプ前しゃがみ
            case CharaMotionType.JCr:
            {
                var flag = MyState.Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionFlag.Act | CharaMotionFlag.Ar | CharaMotionFlag.Ds
                    : CharaMotionFlag.Act | CharaMotionFlag.Ar;
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.JCF, flag,
                    MyState.Coordinate.DirectionZ);
            }
                break;
            // キャンセルジャンプ前しゃがみ
            case CharaMotionType.CJCr:
                // ダッシュフラグ消す、扱いはジャンプ前しゃがみ
                MyState.Motion.SetMotionNo(CharaMotionType.JCr, CharaMotionNo.CRF,
                    CharaMotionFlag.Act | CharaMotionFlag.Ar,
                    MyState.Coordinate.DirectionZ);
                break;
            // ジャンプ
            case CharaMotionType.JUp:
            case CharaMotionType.JDn:
            {
                var motionNo = motionType is CharaMotionType.JUp
                    ? CharaMotionNo.AIRUPF
                    : CharaMotionNo.AIRDNF;
                var flag = MyState.Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionFlag.Ar | CharaMotionFlag.Muki | CharaMotionFlag.AtCa | CharaMotionFlag.Ds
                    : CharaMotionFlag.Ar | CharaMotionFlag.Muki | CharaMotionFlag.AtCa;
                MyState.Motion.SetMotionNo(motionType, motionNo, flag,
                    MyState.Coordinate.DirectionZ);
            }
                break;
            // 空中復帰
            case CharaMotionType.ARv:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.AIRDNF,
                    CharaMotionFlag.Ar | CharaMotionFlag.Muki | CharaMotionFlag.AtCa,
                    MyState.Coordinate.DirectionZ);
                break;
            // しゃがみ
            case CharaMotionType.Cr:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.CRF,
                    CharaMotionFlag.Act | CharaMotionFlag.AtCa,
                    MyState.Coordinate.DirectionZ);
                break;
            // ふっとび
            case CharaMotionType.FlF:
            case CharaMotionType.FlB:
            {
                var motionNo = motionType is CharaMotionType.FlF
                    ? CharaMotionNo.FLF
                    : CharaMotionNo.FLB;
                MyState.Motion.SetMotionNo(motionType, motionNo,
                    CharaMotionFlag.Ar | CharaMotionFlag.Dam | CharaMotionFlag.MTK);
            }
                break;
            // パスヒット
            case CharaMotionType.PHF:
            case CharaMotionType.PHB:
            {
                var motionNo = motionType is CharaMotionType.PHF
                    ? CharaMotionNo.PHF
                    : CharaMotionNo.PHB;
                MyState.Motion.SetMotionNo(motionType, motionNo,
                    CharaMotionFlag.PHit);
            }
                break;
            // ダウンヒット
            case CharaMotionType.DnHF:
            case CharaMotionType.DnHB:
            {
                var motionNo = motionType is CharaMotionType.DnHF
                    ? CharaMotionNo.DNHF
                    : CharaMotionNo.DNHB;
                MyState.Motion.SetMotionNo(motionType, motionNo,
                    CharaMotionFlag.Dam | CharaMotionFlag.Dn);
            }
                break;
            // かがみ
            case CharaMotionType.KG:
            {
                var motionNo = MyState.Live.Hp > Defines.KAGAMI2HP
                    ? CharaMotionNo.KG
                    : CharaMotionNo.KG2;
                MyState.Motion.SetMotionNo(motionType, motionNo,
                    CharaMotionFlag.Dam | CharaMotionFlag.KG);
            }
                break;
            // ダウン
            case CharaMotionType.DnF:
            case CharaMotionType.DnB:
            {
                var motionNo = motionType is CharaMotionType.DnF
                    ? CharaMotionNo.DNF
                    : CharaMotionNo.DNB;
                MyState.Motion.SetMotionNo(motionType, motionNo,
                    CharaMotionFlag.Dam | CharaMotionFlag.Dn);
            }
                break;
            // ダウンからの復帰
            case CharaMotionType.DRv:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.DRVF,
                    CharaMotionFlag.Dam | CharaMotionFlag.Dn);
                break;
            // キャッチモーション
            case CharaMotionType.CM:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.CMF,
                    CharaMotionFlag.Act);
                break;
            case CharaMotionType.JCM:
                break;
            // ファンブル
            case CharaMotionType.FB:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.FBF,
                    CharaMotionFlag.Act,
                    MyState.Coordinate.DirectionZ);
                break;
            // ジャンプファンブル
            case CharaMotionType.JFB:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.JFBF,
                    CharaMotionFlag.Act,
                    MyState.Coordinate.DirectionZ);
                break;
            // パス待ち
            case CharaMotionType.PW:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.PWF,
                    CharaMotionFlag.JpOK | CharaMotionFlag.Muki | CharaMotionFlag.PW | CharaMotionFlag.DmOK,
                    MyState.Coordinate.DirectionZ);
                break;
            // パス待ち歩き
            case CharaMotionType.PWWk:
                // 歩き扱い
                MyState.Motion.SetMotionNo(CharaMotionType.Wk, CharaMotionNo.PWWKF,
                    CharaMotionFlag.JpOK | CharaMotionFlag.Muki | CharaMotionFlag.PW | CharaMotionFlag.DmOK,
                    MyState.Coordinate.DirectionZ);
                break;
            // パス待ちダッシュ
            case CharaMotionType.PWDs:
                // ダッシュ扱い
                MyState.Motion.SetMotionNo(CharaMotionType.Ds, CharaMotionNo.PWDS,
                    CharaMotionFlag.Ds | CharaMotionFlag.JpOK | CharaMotionFlag.PW,
                    MyState.Coordinate.DirectionZ);
                break;
            // スリップ
            case CharaMotionType.Sl:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.SL,
                    CharaMotionFlag.Act | CharaMotionFlag.JpOK | CharaMotionFlag.Slip | CharaMotionFlag.AtCa);
                break;
            // シュート、振り返りシュート
            case CharaMotionType.Sh:
            case CharaMotionType.RtSh:
            {
                var motionNo = motionType is CharaMotionType.Sh
                    ? CharaMotionNo.SHF
                    : CharaMotionNo.RTNSH;
                var flag = CharaMotionFlag.Act;
                if (MyState.Motion.HasFlag(CharaMotionFlag.Slip))
                {
                    flag |= CharaMotionFlag.Slip;
                }
                else if (MyState.Motion.HasFlag(CharaMotionFlag.Ds))
                {
                    flag |= CharaMotionFlag.Ds;
                }

                MyState.Motion.SetMotionNo(CharaMotionType.Sh, motionNo,
                    flag,
                    MyState.Coordinate.DirectionZ);
            }
                break;
            case CharaMotionType.Pa:
                break;
            case CharaMotionType.JSh:
                break;
            case CharaMotionType.RtJSh:
                break;
            case CharaMotionType.JPa:
                break;
            case CharaMotionType.Ca:
                break;
            case CharaMotionType.JCa:
                break;
            case CharaMotionType.Dg:
                break;
            case CharaMotionType.JDg:
                break;
            case CharaMotionType.RoF:
                break;
            case CharaMotionType.RoB:
                break;
            case CharaMotionType.DRAW:
                break;
            case CharaMotionType.WIN:
                break;
            case CharaMotionType.LOSE:
                break;
            case CharaMotionType.OvL:
                break;
            case CharaMotionType.USA:
                break;
            case CharaMotionType.USA2:
                break;
            case CharaMotionType.IKI:
                break;
            case CharaMotionType.LOOK:
                break;
            case CharaMotionType.LOOK2:
                break;
            case CharaMotionType.FALL:
                break;
            case CharaMotionType.AGE2:
                break;
            case CharaMotionType.AGE3:
                break;
            case CharaMotionType.AGE4:
                break;
            case CharaMotionType.AGE5:
                break;
            case CharaMotionType.DO1:
                break;
            case CharaMotionType.DO2:
                break;
            case CharaMotionType.ANG:
                break;
            default:
                throw new ArgumentOutOfRangeException(nameof(motionType), motionType, null);
        }
    }
    //     //ごろごろ
    //   case dbmtRoF:
    //   case dbmtRoB:
    //     st_.pstMyCh_->Motion.MNo = (tMtype == dbmtRoF)
    //       ? dbmnROF //前転がり
    //       : dbmnROB;//後ろ転がり
    //     st_.pstMyCh_->Motion.MFlags = (dbmfDam | dbmfDn);
    //     SESet(seRoll);//SEならす
    //     break;

    //     //パス待ち歩き
    //   case dbmtPWWk:
    //     //st_.pstMyCh_->Motion.MNo = dbmnPWWK;
    //     st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnPWWKF);
    //     st_.pstMyCh_->Motion.MFlags = (dbmfJpOK | dbmfMuki | dbmfPW | dbmfDmOK);
    //     tMtype = dbmtWk;
    //     break;
    //
    //     //パス待ちダッシュ★
    //   case dbmtPWDs:
    //     st_.pstMyCh_->Motion.MNo = dbmnPWDS;
    //     st_.pstMyCh_->Motion.MFlags = (dbmfDs | dbmfJpOK | dbmfPW);
    //     tMtype = dbmtDs;
    //
    //     break;
    //
    //     //スリップ
    //   case dbmtSl:
    //     st_.pstMyCh_->Motion.MNo = dbmnSL;
    //     st_.pstMyCh_->Motion.MFlags = (dbmfAct | dbmfJpOK | dbmfSlip | dbmfAtCa);
    //     break;
    //
    //     //シュート・パス
    //   case dbmtSh:
    //   case dbmtRtSh:
    //   case dbmtPa:
    //     //シュート
    //     if ((tMtype == dbmtSh) || (tMtype == dbmtRtSh))
    //     {
    //       //シュートクリア
    //       SetTutoOK(sta_ShotAct);
    //
    //       st_.pstMyCh_->Motion.MNo = (tMtype == dbmtSh)
    //         ? RevMNoZ(dbmnSHF)
    //         : dbmnRTNSH;
    //
    //       //振り替えリシュート
    //       if (tMtype == dbmtRtSh)
    //       {
    //         st_.pstMyCh_->Zahyou.MukiZ = mzN;
    //         st_.pstMyCh_->Utrun_f = TRUE;
    //       }
    //       tMtype = dbmtSh;
    //
    //       //投げ始め終わりの長さ
    //       st_.pstMyCh_->ShStWait_c = RankLevel(rkShStMotion);
    //       st_.pstMyCh_->ShEdWait_c = RankLevel(rkShEdMotion);
    //     }
    //     else//パス
    //     {
    //       st_.pstMyCh_->PaStWait_c = pmgEO_->mgDt_.dtSet_.GetDtPass(setPaStWait);
    //       st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnPAF);
    //
    //       //トスパスフラグ
    //       //st_.pstMyCh_->QuickPass_f = TRUE;
    //       st_.pstMyCh_->TossPass_f = TRUE;
    //     }
    //
    //     if (st_.pstMyCh_->Motion.IsMFlags(dbmfSlip))
    //     {
    //       st_.pstMyCh_->Motion.MFlags = (dbmfAct | dbmfSlip);//スリップ
    //     }
    //     else if (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
    //     {
    //       st_.pstMyCh_->Motion.MFlags = (dbmfAct | dbmfDs);//ダッシュフラグ残す
    //     }
    //     else
    //     {
    //       st_.pstMyCh_->Motion.MFlags = (dbmfAct);
    //     }
    //
    //     pmgSG_->stBa_.shotmotion_f = TRUE;
    //
    //     break;
    //
    //     //ジャンプシュート・パス
    //   case dbmtJSh:
    //   case dbmtRtJSh:
    //   case dbmtJPa:
    //
    //     //ダッシュフラグ残す
    //     st_.pstMyCh_->Motion.MFlags = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
    //       ? (dbmfAr | dbmfAct | dbmfDs)
    //       : (dbmfAr | dbmfAct);
    //
    //
    //     if ((tMtype == dbmtJSh) || (tMtype == dbmtRtJSh))//ジャンプシュート
    //     {
    //       //シュートクリア
    //       SetTutoOK(sta_ShotAct);
    //
    //       //敵のコートの方向を向いているか
    //       BOOL enMuki_f = ((st_.mysideNo_ == 0) && (st_.pstMyCh_->Zahyou.Muki == mR))
    //         || ((st_.mysideNo_ == 1) && (st_.pstMyCh_->Zahyou.Muki == mL));
    //       //パスタゲが内野
    //       BOOL gaiyatg_f = (pmgSG_->stBa_.PaTgPNo >= DBMEMBER_INF);
    //
    //
    //       if (enMuki_f)
    //       {
    //         if (IsSelfCtrl() == FALSE)
    //         {
    //           gaiyatg_f = TRUE;
    //         }
    //         else
    //         {
    //           if (gaiyatg_f == FALSE)
    //           {
    //             //どうせ届かないからタゲの状態はどうでもいいか
    //             gaiyatg_f = TRUE;
    //           }
    //         }
    //       }
    //
    //
    //       //投げ始め終わりの長さ
    //       st_.pstMyCh_->ShStWait_c = RankLevel(rkShStMotion);
    //       st_.pstMyCh_->ShEdWait_c = RankLevel(rkShEdMotion);
    //       st_.pstMyCh_->Motion.MNo = (tMtype == dbmtJSh)
    //         ? RevMNoZ(dbmnJSHF)
    //         : dbmnRTNJSH;
    //
    //
    //       tMtype = dbmtJSh;
    //
    //     }
    //     else//ジャンプパス
    //     {
    //       st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnJPAF);
    //       //トスパスフラグ
    //       //st_.pstMyCh_->QuickPass_f = TRUE;
    //       st_.pstMyCh_->TossPass_f = TRUE;
    //     }
    //     //ぶぶぶもあるので一律
    //     st_.pstMyCh_->PaStWait_c = pmgEO_->mgDt_.dtSet_.GetDtPass(setPaStWait);
    //
    //     st_.pstMyCh_->AirAct_f = TRUE;//空中アクション
    //
    //     pmgSG_->stBa_.shotmotion_f = TRUE;
    //     break;
    //
    //     //キャッチ・よけ
    //   case dbmtCa:
    //   case dbmtDg:
    //
    //
    //     if (tMtype == dbmtCa)//キャッチ
    //     {
    //       //キャッチボタンが押された
    //       SetActBtn(sta_CatchAct);
    //
    //       st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnCAF);
    //       //キャッチ待ち時間
    //       st_.pstMyCh_->CatchW_c = pmgEO_->mgDt_.dtSet_.GetDtCatch(setCatchWait);
    //
    //       //ダッシュフラグ残す
    //       st_.pstMyCh_->Motion.MFlags = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
    //         ? (dbmfAct | dbmfDs)
    //         : (dbmfAct);
    //     }
    //     else//よけ
    //     {
    //       //避けボタンが押された
    //       SetActBtn(sta_DodgeAct);
    //
    //       st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnDGF);
    //
    //       //ダッシュフラグ残す
    //       st_.pstMyCh_->Motion.MFlags = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
    //         ? (dbmfAct | dbmfDs | dbmfDg)
    //         : (dbmfAct | dbmfDg);
    //     }
    //     ////ダッシュフラグ残す
    //     //st_.pstMyCh_->Motion.MFlags = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
    //     //? (dbmfAct | dbmfDs)
    //     //: (dbmfAct);
    //     break;
    //
    //     //ジャンプキャッチ・よけ
    //   case dbmtJCa:
    //   case dbmtJDg:
    //     if (tMtype == dbmtJCa)//キャッチ
    //     {
    //       //キャッチボタンが押された
    //       SetActBtn(sta_CatchAct);
    //
    //       st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnJCAF);
    //
    //       //ダッシュフラグ残す
    //       st_.pstMyCh_->Motion.MFlags = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
    //         ? (dbmfAr | dbmfAct | dbmfDs | dbmfDg)
    //         : (dbmfAr | dbmfAct);
    //     }
    //     else//よけ
    //     {
    //       //避けボタンが押された
    //       SetActBtn(sta_DodgeAct);
    //
    //       st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnJDGF);
    //
    //       //ダッシュフラグ残す
    //       st_.pstMyCh_->Motion.MFlags = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
    //         ? (dbmfAr | dbmfAct | dbmfDs | dbmfDg)
    //         : (dbmfAr | dbmfAct);
    //     }
    //
    //     st_.pstMyCh_->AirAct_f = TRUE;//空中アクション
    //     st_.pstMyCh_->AirCatch_f = TRUE;//取れていようが居まいが構わないか
    //     break;
    //
    //     //ジャンプキャッチ・よけ
    //   case dbmtJCM:
    //   case dbmtJFB:
    //     if (tMtype == dbmtJCa)//キャッチ
    //     {
    //       st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnJCMF);
    //     }
    //     else//よけ
    //     {
    //       st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnJFBF);
    //     }
    //     //ダッシュフラグ残す
    //     st_.pstMyCh_->Motion.MFlags = (dbmfAr | dbmfAct);
    //
    //     st_.pstMyCh_->AirAct_f = TRUE;//空中アクション
    //     st_.pstMyCh_->AirCatch_f = TRUE;//取れていようが居まいが構わないか
    //     break;
    //
    //     //オーバーライン
    //   case dbmtOvL:
    //     st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnOVLF);
    //     //ダッシュフラグ残す
    //     st_.pstMyCh_->Motion.MFlags = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
    //       ? (dbmfAct | dbmfDs)
    //       : (dbmfAct);
    //
    //     //★オーバーラインホイッスル
    //     if (pmgSG_->stRf_.GameSet_f == FALSE)
    //     {
    //       st_.pmgRf_->SetMotion(dbrfShortWhistle);
    //       st_.pmgRf_->RefereeMsg("オーバーライン");
    //
    //       //チュートＮＧオーバーライン
    //       SetTutoNG(sta_ShotAct, rta_shot_ngovline);
    //
    //       //                    //オーバーラインにならない時間
    //       //                    if (st_.pstMyCh_->Steal_c == 0)
    //       //                    {
    //       //                        AddRec(recOverLine_c);
    //       //                        //★成績//オーバーライン数合計
    //       //                        //AddRec(recOverLine_c)++;
    //       //                    }
    //     }
    //     break;
    //
    //     //天使
    //   case dbmtANG:
    //     st_.pstMyCh_->Motion.MNo = dbmnANG;
    //     st_.pstMyCh_->Motion.MFlags = (dbmfAr | dbmfMTK | dbmfDam | dbmfANG);
    //
    //     st_.pstMyCh_->Dead_f = TRUE;//死亡
    //     ZeroXYZ();
    //
    //     //天使生成
    //     st_.pmgMyTm_->SetAngel(st_.posNo_);
    //
    //
    //     break;
    //
    //     //引き分け
    //   case dbmtDRAW:
    //     st_.pstMyCh_->Motion.MNo = dbmnDRAW;
    //     st_.pstMyCh_->Motion.MFlags = (dbmfRES);
    //     //強制地上（特殊）
    //     st_.pstMyCh_->Zahyou.Y = 0;
    //     //強制向き
    //     st_.pstMyCh_->Zahyou.Muki = (IsInfield() ^ (st_.mysideNo_ == 1))
    //       ? mR
    //       : mL;
    //     break;
    //
    //     //勝利
    //   case dbmtWIN:
    //     st_.pstMyCh_->Motion.MNo = dbmnWIN;
    //     st_.pstMyCh_->Motion.MFlags = (dbmfRES);
    //     //強制地上（特殊）
    //     st_.pstMyCh_->Zahyou.Y = 0;
    //     //強制向き
    //     st_.pstMyCh_->Zahyou.Muki = (IsInfield() ^ (st_.mysideNo_ == 1))
    //       ? mR
    //       : mL;
    //     break;
    //
    //     //敗北
    //   case dbmtLOSE:
    //     st_.pstMyCh_->Motion.MNo = dbmnLOSE;
    //     st_.pstMyCh_->Motion.MFlags = (dbmfRES);
    //     //強制地上（特殊）
    //     st_.pstMyCh_->Zahyou.Y = 0;
    //     //強制向き
    //     st_.pstMyCh_->Zahyou.Muki = (IsInfield() ^ (st_.mysideNo_ == 1))
    //       ? mR
    //       : mL;
    //     break;
    //
    //     //  //うさぎ跳び
    //     //case dbmtUSA:
    //     //  st_.pstMyCh_->Motion.MNo = dbmnUSA;
    //     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
    //     //  break;
    //     //
    //     //  //うさぎ跳び2
    //     //case dbmtUSA2:
    //     //  st_.pstMyCh_->Motion.MNo = dbmnUSA2;
    //     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
    //     //  break;
    //     //
    //     //  //息切れ
    //     //case dbmtIKI:
    //     //  st_.pstMyCh_->Motion.MNo = dbmnIKI;
    //     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
    //     //  break;
    //     //
    //     //  //こっちみる
    //     //case dbmtLOOK:
    //     //  st_.pstMyCh_->Motion.MNo = dbmnLOOK;
    //     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
    //     //  break;
    //     //
    //     //  //こっちみて笑う
    //     //case dbmtLOOK2:
    //     //  st_.pstMyCh_->Motion.MNo = dbmnLOOK2;
    //     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
    //     //  break;
    //     //
    //     //  ////おちる
    //     //  //case dbmtFALL:
    //     //  //  st_.pstMyCh_->Motion.MNo = dbmnFALL;
    //     //  //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
    //     //  //  break;
    //     //
    //     //  //胴上げられ
    //     //case dbmtDO1:
    //     //  st_.pstMyCh_->Motion.MNo = dbmnDO1;
    //     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
    //     //  break;
    //     //
    //     //  //胴上げられ
    //     //case dbmtDO2:
    //     //  st_.pstMyCh_->Motion.MNo = dbmnDO2;
    //     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
    //     //  break;
    //     //
    //     //  //胴上げ
    //     //case dbmtAGE2:
    //     //  st_.pstMyCh_->Motion.MNo = dbmnAGEF;
    //     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
    //     //  break;
    //     //  //胴上げ
    //     //case dbmtAGE3:
    //     //  st_.pstMyCh_->Motion.MNo = dbmnAGEN;
    //     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
    //     //  break;
    //     //  //胴上げ
    //     //case dbmtAGE4:
    //     //  st_.pstMyCh_->Motion.MNo = dbmnAGER;
    //     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
    //     //  break;
    //     //  //胴上げ
    //     //case dbmtAGE5:
    //     //  st_.pstMyCh_->Motion.MNo = dbmnAGEC;
    //     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
    //     //  break;
    //     //
    //   default:
    //     ////ないと思うけど一応
    //     //st_.pstMyCh_->Motion.MNo = dbmnFALL;
    //     break;
    //   }
    //
    //
    //
    //   //空中ではなくなったとき
    //   if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr) == FALSE)
    //   {
    //     st_.pstMyCh_->Air_c = 0;
    //     st_.pstMyCh_->ShAir_c = 0;
    //     st_.pstMyCh_->AirCatch_f = FALSE;
    //     st_.pstMyCh_->AirAct_f = FALSE;
    //
    //     //if (st_.pstMyCh_->Zahyou.Y > 0)
    //     //{
    //     //  //★浮いちゃ行けないところで浮いてしまっている
    //     //  s32 a = 0;//ここにブレークポイントを置いて呼び出し履歴を見る
    //     //}
    //   }
    //
    //   if (st_.pstMyCh_->Motion.IsMFlags(dbmfAct)
    //     && (st_.pstMyCh_->Auto.AutoType == dbatFree))
    //   {
    //     st_.pstMyCh_->FreeAct_f = TRUE;//フリー状態からのアクション
    //   }
    //   else if ((tMtype == dbmtJCr) || (tMtype == dbmtJUp))
    //   {
    //     //フリー状態を継続
    //   }
    //   else
    //   {
    //     st_.pstMyCh_->FreeAct_f = FALSE;//フリー状態からのアクション
    //   }
    //
    //   //ダッシュではなくなったとき★★
    //   if (st_.pstMyCh_->Motion.IsMFlags(dbmfDs) == FALSE)
    //   {
    //     if (st_.pstMyCh_->DashmanNo != NGNUM)
    //     {
    //       st_.pstMyCh_->DashmanNo = NGNUM;//埋め
    //     }
    //
    //     //ダッシュフラグが消えたらIOS加速ダッシュを消す
    //     st_.pstMyCh_->DsAccIOS_f = FALSE;
    //   }
    //
    //   //モーションをセット
    //   st_.pstMyCh_->Motion.Mtype = tMtype;
    //
    //   //パス待ち変化ではない
    //   if (switchPW_f == FALSE)
    //   {
    //     //フレーム情報のセット
    //     SetFrameData(FALSE);
    //   }
    // }

    private CharaMotionType ShiftMotionType(CharaMotionType motionType)
    {
        // 空中の場合、地上モーションを空中用に変更
        if (MyState.Motion.HasFlag(CharaMotionFlag.Ar))
        {
            motionType = motionType switch
            {
                CharaMotionType.St => CharaMotionType.JUp,
                CharaMotionType.Sh => CharaMotionType.JSh,
                CharaMotionType.RtSh => CharaMotionType.RtJSh,
                CharaMotionType.Pa => CharaMotionType.JPa,
                CharaMotionType.Ca => CharaMotionType.JCa,
                _ => motionType
            };
        }
        else
        {
            motionType = motionType switch
            {
                CharaMotionType.JUp or CharaMotionType.JDn => CharaMotionType.St,
                CharaMotionType.JSh => CharaMotionType.Sh,
                CharaMotionType.RtJSh => CharaMotionType.RtSh,
                CharaMotionType.JPa => CharaMotionType.Pa,
                CharaMotionType.JCa => CharaMotionType.Ca,
                _ => motionType
            };
        }

        // 試合終了してるときは天使にならないでダウンに変化
        if (motionType == CharaMotionType.ANG && RefereeState.IsGameSet)
        {
            motionType = MyState.Motion.MotionType == CharaMotionType.FlF
                ? CharaMotionType.DnF
                : CharaMotionType.DnB;
            // TODO:審判にアクセスするタイミングとしてはここではないので移動する
            //  st_.pmgRf_->SetMotion(dbrfLongWhistle);
        }

        // 下降中はジャンプ下降中に変更
        if (motionType == CharaMotionType.JUp && MyState.Coordinate.VelocityY < 0)
        {
            motionType = CharaMotionType.JDn;
        }

        return motionType;
    }

    private bool EnableSetMotion(CharaMotionType motionType, bool isForce)
    {
        // 同じモーション間は変更不可
        if (isForce == false
            && MyState.Motion.MotionType == motionType)
        {
            return false;
        }

        // 天使状態は変更付加
        if (MyState.Motion.MotionType == CharaMotionType.ANG)
        {
            return false;
        }

        // 勝ち負けポーズ同士の変更不可
        if (MyState.Motion.HasFlag(CharaMotionFlag.RES)
            && motionType is CharaMotionType.WIN or CharaMotionType.LOSE or CharaMotionType.DRAW)
        {
            return false;
        }

        // かがみは立ち歩き走り以外ではならない
        if (motionType == CharaMotionType.KG)
        {
            if (MyState.Motion.MotionType != CharaMotionType.St
                && MyState.Motion.MotionType != CharaMotionType.Wk
                && MyState.Motion.MotionType != CharaMotionType.Ds
                && MyState.Motion.MotionType != CharaMotionType.KG)
            {
                return false;
            }
        }

        return true;
    }

}