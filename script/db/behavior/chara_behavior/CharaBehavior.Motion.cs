namespace db;

public partial class CharaBehavior
{

    private void SetMotionType(CharaMotionType motionType, bool isForce = false)
    {
        // 同じモーション間は変更不可
        if (isForce == false
            && MyState.Motion.MotionType == motionType)
        {
            return;
        }

        // 天使状態は変更付加
        if (MyState.Motion.MotionType == CharaMotionType.ANG)
        {
            return;
        }
        
        // 勝ち負けポーズ同士の変更不可
        if (MyState.Motion.HasFlag(CharaMotionFlag.RES)
            && motionType is CharaMotionType.WIN or CharaMotionType.LOSE or CharaMotionType.DRAW)
        {
            return;
        }
        
        if (motionType == CharaMotionType.ANG)
        {
            
        }
        
    }


    //
    //   //試合終了してるときは天使にならない→相手が全滅しているとき
    //   //★これは審判クラスを参照した方がいいか
    //   if (tMtype == dbmtANG)
    //   {
    //     if (st_.pmgEnTm_->IsAllDead()
    //       || pmgSG_->stRf_.TimeUp_f)
    //     {
    //       if (st_.pstMyCh_->Motion.Mtype == dbmtFlF)
    //       {
    //         tMtype = dbmtDnF;//ただのダウンに変更
    //       }
    //       else
    //       {
    //         tMtype = dbmtDnB;//ただのダウンに変更
    //       }
    //     }
    //     else if (st_.pmgMyTm_->IsLastOne() && (pmgSG_->stRf_.Suddun_f == FALSE))
    //     {
    //       st_.pmgRf_->SetMotion(dbrfLongWhistle);
    //     }
    //   }
    //
    //   //かがみは立ち歩き走り以外ではならない
    //   if (tMtype == dbmtKG)
    //   {
    //     if ((st_.pstMyCh_->Motion.Mtype != dbmtSt)
    //       && (st_.pstMyCh_->Motion.Mtype != dbmtWk)
    //       && (st_.pstMyCh_->Motion.Mtype != dbmtDs)
    //       && (st_.pstMyCh_->Motion.Mtype != dbmtKG))
    //     {
    //       return;
    //     }
    //   }
    //
    //   BOOL switchPW_f = ((tMtype == dbmtDs) && (st_.pstMyCh_->Motion.MNo == dbmnPWDS))
    //     || ((tMtype == dbmtPWDs) && (st_.pstMyCh_->Motion.MNo == dbmnDS))
    //     || ((tMtype == dbmtWk) && (st_.pstMyCh_->Motion.MNo == dbmnPWWKF))
    //     || ((tMtype == dbmtWk) && (st_.pstMyCh_->Motion.MNo == dbmnPWWKN))
    //     || ((tMtype == dbmtWk) && (st_.pstMyCh_->Motion.MNo == dbmnPWWKR))
    //     || ((tMtype == dbmtPWWk) && (st_.pstMyCh_->Motion.MNo == dbmnWKF))
    //     || ((tMtype == dbmtPWWk) && (st_.pstMyCh_->Motion.MNo == dbmnWKN))
    //     || ((tMtype == dbmtPWWk) && (st_.pstMyCh_->Motion.MNo == dbmnWKR))
    //     ;
    //
    //   //パス待ちモーションの時は絵柄だけ変わるように
    //   //それ以外の時は初期化
    //   if (switchPW_f == FALSE)
    //   {
    //     //初期化
    //
    //     st_.pstMyCh_->Motion.M_c = 0;
    //     st_.pstMyCh_->Anime.FrameNo = 0;
    //     st_.pstMyCh_->Anime.LoopStNo = 0;
    //
    //     //キャッチカウンタＯＦＦ
    //     st_.pstMyCh_->Catch_c = NGNUM;
    //
    //     //敵コート避け時間
    //     st_.pstMyCh_->ECDdg_c = 0;
    //
    //     //ダッシュ加速
    //     st_.pstMyCh_->DsAcc_c = 0;
    //     //摩擦係数
    //     st_.pstMyCh_->Zahyou.Fric_c = 0;
    //
    //     //着地計算まだよ
    //     st_.pstMyCh_->LandSet_f = FALSE;
    //
    //     //振り返りフラグ
    //     st_.pstMyCh_->Utrun_f = FALSE;
    //
    //
    //     //地上についたら空中復帰消す
    //     if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr) == FALSE)
    //     {
    //       st_.pstMyCh_->AirRev_f = FALSE;
    //     }
    //
    //
    //     
    //     switch (tMtype)
    //     {
    //     case dbmtSt:
    //     case dbmtWk:
    //     case dbmtCa:
    //     case dbmtJCa:
    //       //キャッチ連打が効いてしまう
    //       break;
    //     default:
    //       //キャッチ待ち時間ゼロにしてみる
    //       st_.pstMyCh_->CatchW_c = 0;
    //       //おしっぱジャンプ暴発防ぐフラグ
    //       st_.pstMyCh_->jumpok_f_ = FALSE;
    //       st_.pstMyCh_->shotok_f_ = FALSE;
    //       break;
    //     }
    //
    //
    //
    //   }
    //
    //   if (PassWait_f() == FALSE)
    //   {
    //     st_.pstMyCh_->PassWait_c = 0;
    //   }
    //
    //   //ダウンカウンタのリセット（ここでいいのか怪しい
    //   if ((tMtype != dbmtFlF)
    //     && (tMtype != dbmtFlB)
    //     && (tMtype != dbmtDnF)
    //     && (tMtype != dbmtDnB)
    //     && (tMtype != dbmtDnHF)
    //     && (tMtype != dbmtDnHB)
    //     )
    //   {
    //     st_.pstMyCh_->Down_c = 0;
    //   }
    //
    //   //空中ではあり得ないモーション
    //   if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr))
    //   {
    //     switch (tMtype)
    //     {
    //     case dbmtSt: tMtype = dbmtJUp; break;
    //     case dbmtSh: tMtype = dbmtJSh; break;
    //     case dbmtRtSh: tMtype = dbmtRtJSh; break;
    //     case dbmtPa: tMtype = dbmtJPa; break;
    //     case dbmtCa: tMtype = dbmtJCa; break;
    //     }
    //   }
    //   else//地上ではあり得ないモーション
    //   {
    //     switch (tMtype)
    //     {
    //     case dbmtJUp: tMtype = dbmtSt; break;
    //     case dbmtJDn: tMtype = dbmtSt; break;
    //     case dbmtJSh: tMtype = dbmtSh; break;
    //     case dbmtRtJSh: tMtype = dbmtRtSh; break;
    //     case dbmtJPa: tMtype = dbmtPa; break;
    //     case dbmtJCa: tMtype = dbmtCa; break;
    //     }
    //   }
    //
    //
    //   const s32 KAGAMI2HP = 8;//辛息切れボーダーＨＰ
    //   s32 livenum = st_.pmgMyTm_->GetLiveNum();
    //
    //
    //   //歩数リセット
    //   switch (tMtype)
    //   {
    //   case dbmtSt:
    //   case dbmtWk:
    //     st_.pstMyCh_->Step_c = 0;//歩数を初期化
    //     st_.pstMyCh_->MadStep_c = 0;//歩数を初期化
    //     break;
    //   }
    //
    //
    //   //フラグの初期化
    //   switch (tMtype)
    //   {
    //     //立ち
    //   case dbmtSt:
    //
    //     st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnSTF);
    //     st_.pstMyCh_->Motion.MFlags = (dbmfJpOK | dbmfMuki | dbmfDmOK | dbmfAtCa);
    //
    //     break;
    //
    //     //息継ぎ
    //   case dbmtBreath:
    //     //Ｚ向きでモーション変える
    //     st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnBRF);
    //     st_.pstMyCh_->Motion.MFlags = (dbmfJpOK | dbmfMuki | dbmfDmOK | dbmfAtCa);
    //     //建前上、立ちにする
    //     tMtype = dbmtSt;
    //     break;
    //
    //     //歩き
    //   case dbmtWk:
    //
    //     //st_.pstMyCh_->Motion.MNo = dbmnWK;
    //     st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnWKF);
    //     st_.pstMyCh_->Motion.MFlags = (dbmfJpOK | dbmfMuki | dbmfDmOK | dbmfAtCa);
    //     break;
    //
    //     //ダッシュ
    //   case dbmtDs:
    //     //ダッシュ継続以外
    //     if (lib_num::IsFlags((s32)st_.pstMyCh_->Motion.MFlags, (s32)dbmfDs) == FALSE)
    //     {
    //       st_.pstMyCh_->Step_c = 0;//歩数を初期化
    //       st_.pstMyCh_->MadStep_c = 0;//歩数を初期化
    //     }
    //     st_.pstMyCh_->Motion.MNo = dbmnDS;
    //     st_.pstMyCh_->Motion.MFlags = (dbmfDs | dbmfJpOK | dbmfAtCa);
    //     break;
    //
    //     //ジャンプ前しゃがみ
    //   case dbmtJCr:
    //     SetTutoOK(sta_JumpAct);//ジャンプクリア
    //     st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnJCF);
    //     st_.pstMyCh_->Motion.MFlags = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
    //       ? (dbmfAct | dbmfAr | dbmfDs)
    //       : (dbmfAct | dbmfAr);
    //     break;
    //
    //     //キャンセルジャンプ前しゃがみ（Dsフラグが入らない）
    //   case dbmtCJCr:
    //     st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnCRF);
    //     st_.pstMyCh_->Motion.MFlags = (dbmfAct | dbmfAr);
    //     tMtype = dbmtJCr;
    //     break;
    //
    //     //ジャンプ中
    //   case dbmtJUp:
    //   case dbmtJDn:
    //
    //     if (st_.pstMyCh_->Zahyou.dY < 0) tMtype = dbmtJDn;
    //
    //     st_.pstMyCh_->Motion.MNo = (tMtype == dbmtJUp)
    //       ? RevMNoZ(dbmnAIRUPF)
    //       : RevMNoZ(dbmnAIRDNF);
    //
    //     st_.pstMyCh_->Motion.MFlags = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
    //       ? (dbmfAr | dbmfMuki | dbmfAtCa | dbmfDs)
    //       : (dbmfAr | dbmfMuki | dbmfAtCa);
    //
    //     break;
    //
    //     //空中復帰
    //   case dbmtARv:
    //     st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnAIRDNF);
    //     st_.pstMyCh_->Motion.MFlags = (dbmfAr | dbmfMuki | dbmfAtCa);
    //     st_.pstMyCh_->AirAct_f = TRUE;//復帰した後やっぱし動けるように→行動不能だけどオートキャッチ
    //     tMtype = dbmtJDn;
    //     break;
    //
    //     //しゃがみ
    //   case dbmtCr:
    //     st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnCRF);
    //     st_.pstMyCh_->Motion.MFlags = (dbmfAct | dbmfAtCa);//一応セミオートのためオートキャッチ可能フラグ追加
    //     break;
    //
    //     //吹っ飛ばされ
    //   case dbmtFlF:
    //   case dbmtFlB:
    //     //st_.pstMyCh_->Anime.FrameNo = (tMtype == dbmtFlF)
    //     //  ? 0 //前向き吹っ飛ばされ
    //     //  : 1;//後ろ向き吹っ飛ばされ
    //     st_.pstMyCh_->Motion.MNo = (tMtype == dbmtFlF)
    //       ? dbmnFLF
    //       : dbmnFLB;
    //     st_.pstMyCh_->Motion.MFlags = (dbmfAr | dbmfDam | dbmfMTK);
    //     break;
    //
    //     //ごろごろ
    //   case dbmtRoF:
    //   case dbmtRoB:
    //     st_.pstMyCh_->Motion.MNo = (tMtype == dbmtRoF)
    //       ? dbmnROF //前転がり
    //       : dbmnROB;//後ろ転がり
    //     st_.pstMyCh_->Motion.MFlags = (dbmfDam | dbmfDn);
    //     SESet(seRoll);//SEならす
    //     break;
    //
    //     //パスヒット
    //   case dbmtPHF:
    //   case dbmtPHB:
    //     st_.pstMyCh_->Motion.MNo = (tMtype == dbmtPHF)
    //       ? dbmnPHF
    //       : dbmnPHB;
    //     st_.pstMyCh_->Motion.MFlags = (dbmfPHit);
    //     break;
    //
    //     //かがみ
    //   case dbmtKG:
    //     //のこりＨＰでかがみっぷりを変える
    //
    //     st_.pstMyCh_->Motion.MNo = (st_.pstMyCh_->HP > KAGAMI2HP)
    //       ? dbmnKG
    //       : dbmnKG2;
    //     st_.pstMyCh_->Motion.MFlags = (dbmfDam | dbmfKG);
    //
    //     break;
    //
    //     //ダウンヒット
    //   case dbmtDnHF:
    //   case dbmtDnHB:
    //     st_.pstMyCh_->Motion.MNo = (tMtype == dbmtDnHF)
    //       ? dbmnDNHF
    //       : dbmnDNHB;
    //     st_.pstMyCh_->Motion.MFlags = (dbmfDam | dbmfDn);
    //     break;
    //
    //     //ダウン
    //   case dbmtDnF:
    //   case dbmtDnB:
    //     st_.pstMyCh_->Motion.MNo = (tMtype == dbmtDnF)
    //       ? dbmnDNF
    //       : dbmnDNB;
    //     st_.pstMyCh_->Motion.MFlags = (dbmfDam | dbmfDn);
    //
    //     //if (pmgEO_->mgStSn_.EtcSS.IceCourt_f())
    //     //{
    //     //  s32 spd = (abs(st_.pstMyCh_->Zahyou.dX) > abs(st_.pstMyCh_->Zahyou.dZ))
    //     //    ? abs(st_.pstMyCh_->Zahyou.dX)
    //     //    : abs(st_.pstMyCh_->Zahyou.dZ);
    //
    //     //  //ダウン滑り効果音
    //     //  if (spd > 200)
    //     //  {
    //     //    SESet(seDownLv3);
    //     //  }
    //     //  else if (spd > 100)
    //     //  {
    //     //    SESet(seDownLv2);
    //     //  }
    //     //  else if (spd > 25)
    //     //  {
    //     //    SESet(seDownLv1);
    //     //  }
    //     //  else
    //     //  {
    //     //    SESet(seDownLv0);
    //     //  }
    //     //  //MvDefSl(pmgEO_->mgDt_.dtSet_.GetDtCourt(setDownBrkIce));
    //     //}
    //     //else
    //     {
    //       SESet(seDownLv0);//SEならす
    //     }
    //     //SetSerifu(srfDown);//セリフ
    //     break;
    //
    //     //ダウンからの復帰
    //   case dbmtDRv:
    //     st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnDRVF);
    //     st_.pstMyCh_->Motion.MFlags = (dbmfDam | dbmfDn);
    //     break;
    //
    //     //キャッチモーション
    //   case dbmtCM:
    //     //キャッチクリア
    //     SetTutoOK(sta_CatchAct);
    //     st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnCMF);
    //     st_.pstMyCh_->Motion.MFlags = (dbmfAct);
    //     break;
    //
    //     //ファンブル
    //   case dbmtFB:
    //     st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnFBF);
    //     st_.pstMyCh_->Motion.MFlags = (dbmfAct);
    //     break;
    //
    //     //パス待ち
    //   case dbmtPW:
    //     st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnPWF);
    //     st_.pstMyCh_->Motion.MFlags = (dbmfJpOK | dbmfMuki | dbmfPW | dbmfDmOK);
    //     break;
    //
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
}