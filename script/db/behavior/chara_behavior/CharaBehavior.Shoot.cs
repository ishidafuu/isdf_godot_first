namespace db;

public partial class CharaBehavior
{
    private void SetShooting(bool isJumpShoot)
    {
        
    }
    
//       //新シュート
//   void TChCommon::SetShoot(BOOL tJSh_f)
//   {
//     //攻撃時間制限で手放された
//     if (IsBall() == FALSE) return;
//
//     //★もう一度ここでタゲをとりなおしてみる
//     ShTagSet(FALSE);
//     //敵チーム
//     st_.pmgEnTm_->SetCtrl(pmgSG_->stBa_.ShTgPNo);//タゲにカーソル移動
//     //st_.pmgEnTm_->ResetHitStop();//敵全員のヒットストップリセット
//     st_.pmgBa_->baCommon_.SetMtype(bmShoot);//クリア処理
//
//     TShotInfo ShDt;
//     ShDt.Init();
//     ShDt.tJSh_f = tJSh_f;
//     ShDt.tDSh_f = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs) && IsInfield());
//
//     ShDt.tShRank = MyStLv(dbst_Level);
//     ShDt.tShPow = MyStLv(dbst_Pow);
//     ShDt.tShTech = MyStLv(dbst_Tech);
//
//     if (pmgEO_->stShiai_.IsPrev())
//     {
//       ShDt.tShPow = XYMAG;
//       ShDt.tShTech = XYMAG;
//     }
//
//
//     ShDt.tShSp = pmgEO_->mgDt_.dtRank_.GetRankDtLevel(rkBallSpd, MyStLv(dbst_Level));
//
//     //シュート種類
//     GetShotType(&ShDt);
//
//     ////敵のコートの方向を向いているか
//     //BOOL enMuki_f = ((st_.mysideNo_ == 0) && (st_.pstMyCh_->Zahyou.Muki == mR))
//     //  || ((st_.mysideNo_ == 1) && (st_.pstMyCh_->Zahyou.Muki == mL));
//
//     pmgSG_->stSh_.Init();
//
//
// #ifdef __K_DEBUG_SHIAI__
//     // プレイヤーの撃つシュートは全て必殺にしてしまう！
//     // ※ここもあとででばっぐで好きに設定できるようにする※
//     if (st_.mysideNo_ == 0)
//     {
//       // プレイヤーの撃つシュートは全て必殺にしてしまう！
//       if (kdebug::DebugSystem::GetInstance()->IsPlayerEveryMixShoot())
//       {
//         if (ShDt.tJSh_f){ ShDt.sType = shtJHS; }
//         else { ShDt.sType = shtHS; }
//         ShDt.HSTiming = 1;
//       }
//     }
//     // 逆に敵には必殺シュートうたせない！
//     else
//     {
//       // 敵も毎回必殺シュート
//       if (kdebug::DebugSystem::GetInstance()->IsEnemyEveryMixShoot())
//       {
//         if (ShDt.tJSh_f){ ShDt.sType = shtJHS; }
//         else { ShDt.sType = shtHS; }
//         ShDt.HSTiming = 1;
//       }
//       // 必殺うたない
//       else
//       {
//         if (ShDt.tJSh_f){ ShDt.sType = shtJS; }
//         else { ShDt.sType = shtS; }
//       }
//     }
// #endif	// #ifdef __K_DEBUG_SHIAI__
//     //    st_.pmgBa_->baCommon_.SetMtype(bmShoot);
//     //シュート種類
//     switch (ShDt.sType)
//     {
//     case shtHS:
//     case shtJHS:
//       ShDt.HS_f = TRUE;
//       //GetHShSpeed(&ShDt);//必殺
//       SortShot(ShDt.HSTiming);
//       break;
//     default:
//       GetShSpeed(&ShDt);//通常、ナイス
//       break;
//     }
//
//
//
//     if (ShDt.HS_f)
//     {
//       TStChShotElems& elms = (ShDt.HSTiming == NGNUM)
//         ? (st_.pstMyCh_->stGousei.vjshot_)
//         : (st_.pstMyCh_->stGousei.shot_[ShDt.HSTiming]);
//
//       ShDt.tShPow = (ShDt.tShPow * elms.pow_) / XYMAG;
//       ShDt.tShTech = (ShDt.tShTech * elms.spin_) / XYMAG;
//       ShDt.orishno = elms.oriNameNo_;
//       pmgSG_->stSh_.idata.shotId = elms.shotId;
//     }
//     else
//     {
//       ShDt.orishno = 0;
//     }
//     //idataに初期情報を渡す
//     pmgSG_->stSh_.idata.sp_f = ShDt.HS_f;
//     pmgSG_->stSh_.idata.spin = RankLevel(rkGrpSpin);
//     pmgSG_->stSh_.idata.nice_f = ShDt.tNice_f;
//     pmgSG_->stSh_.idata.counter_f = pmgSG_->stBa_.counter_f;
//     pmgSG_->stSh_.idata.jsh_f = tJSh_f;
//     pmgSG_->stSh_.idata.dsh_f = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs) && IsInfield());
//     pmgSG_->stSh_.idata.side = st_.mysideNo_;
//     pmgSG_->stSh_.idata.frmPos = pmgSG_->stBa_.PichPNo;
//     pmgSG_->stSh_.idata.pow = ShDt.tShPow;
//     pmgSG_->stSh_.idata.sharp = ShDt.tShTech;
//     pmgSG_->stSh_.idata.rank = ShDt.tShRank;
//     pmgSG_->stSh_.idata.roll_f = (ShDt.tShPow >= pmgEO_->mgDt_.dtSet_.GetDtShot(setRollPow));
//     //    st_.pmgBa_->baCommon_.SetMtype(bmShoot);
//     pmgSG_->stBa_.NoHit_c = (tJSh_f)
//       ? 0
//       : pmgEO_->mgDt_.dtSet_.GetDtShot(setHitStartTime);
//     //pmgSG_->stBa_.HitStop_c = 0;//ヒットストップ★
//     //idata.iballの初期設定★
//     //※distを求める際 distX = tagposX - posX とし、絶対値は取らないでOKです。distY、Zも同様です。
//     pmgSG_->stSh_.idata.iball.posX = (pmgSG_->stBa_.Zahyou.X / XYMAG);
//     pmgSG_->stSh_.idata.iball.posY = (pmgSG_->stBa_.Zahyou.Y / XYMAG);
//     pmgSG_->stSh_.idata.iball.posZ = (pmgSG_->stBa_.Zahyou.Z / XYMAG);
//     pmgSG_->stSh_.idata.iball.spd = (f32)pmgEO_->mgDt_.dtSet_.GetDtShot(setNormalShotSpd) / (f32)XYMAG;//ノーマルシュート速度
//
//     //とりあえず強制終了回避
//     if (pmgSG_->stBa_.ShTgPNo < 0)
//     {
//       pmgSG_->stBa_.ShTgPNo = 0;
//     }
//     pmgSG_->stSh_.idata.iball.tagID = pmgSG_->stBa_.ShTgPNo;
//     TZahyouRec* tagzahyou = &(st_.pmgEnTm_->st_.pmgMyCh_[pmgSG_->stBa_.ShTgPNo]->st_.pstMyCh_->Zahyou);
//     pmgSG_->stSh_.idata.iball.tagposX = (tagzahyou->X / XYMAG);
//     pmgSG_->stSh_.idata.iball.tagposY = (tagzahyou->Y / XYMAG);
//     pmgSG_->stSh_.idata.iball.tagposZ = (tagzahyou->Z / XYMAG);
//
//     //初期向き
//     //標的・球の差から初期の向きを求める
//     ////波軌道終了後、波軌道で使っていた「主軌道からの乖離値」を主軌道に埋め込むとき
//     ////主軌道移動がなかった場合、加算減算のジャッジに使う
//     f32 defBalltoTarg = pmgSG_->stSh_.idata.iball.tagposX - pmgSG_->stSh_.idata.iball.posX;
//     pmgSG_->stSh_.idata.iball.initDirX = 1.0f;
//     if (defBalltoTarg != 0.0f)
//     {//０でないなら初期向きは標的・球距離でOk
//       pmgSG_->stSh_.idata.iball.initDirX = defBalltoTarg / fabs(defBalltoTarg);
//     }
//     else
//     {//０なら初期向きはどっちチームかに依存。左チームなら右へ投げ、右チームなら左へ
//       //ただし外野シュートには対応してないので注意
//       if (pmgSG_->stSh_.idata.side != 0) pmgSG_->stSh_.idata.iball.initDirX = -1.0f;
//     }
//
//     //初期Zシュートチェック
//     pmgSG_->stSh_.idata.initZShot_f = FALSE;
//     f32 distX_a = fabs(pmgSG_->stSh_.idata.iball.tagposX - pmgSG_->stSh_.idata.iball.posX);
//     f32 distZ_a = fabs(pmgSG_->stSh_.idata.iball.tagposZ - pmgSG_->stSh_.idata.iball.posZ);
//     if (distX_a < distZ_a) pmgSG_->stSh_.idata.initZShot_f = TRUE;
//
//
//     //外野シュート判別用フラグ　
//     pmgSG_->stSh_.cmn.shotFromNorthOF_f = FALSE;//北側外野ポジションシュートフラグ
//     pmgSG_->stSh_.cmn.shotFromSouthOF_f = FALSE;//南側外野ポジションシュートフラグ
//     pmgSG_->stSh_.cmn.shotNSOF_f = FALSE;//上記２フラグどちらかが立っていたら立てるフラグ
//     if (pmgSG_->stBa_.PichPNo == 4) pmgSG_->stSh_.cmn.shotFromNorthOF_f = TRUE;
//     if (pmgSG_->stBa_.PichPNo == 5) pmgSG_->stSh_.cmn.shotFromSouthOF_f = TRUE;
//     if (
//       (pmgSG_->stSh_.cmn.shotFromNorthOF_f == TRUE)
//       ||
//       (pmgSG_->stSh_.cmn.shotFromSouthOF_f == TRUE)
//       )
//       pmgSG_->stSh_.cmn.shotNSOF_f = TRUE;//上記２フラグどちらかが立っていたら立てる
//
// #ifdef __K_DEBUG_SHIAI__
//     // 投げ手のチームで判断するようにします
//     // st_.mysideNo_
//     // が左チーム０右チーム１なので
//     // デバッグターゲットに１のとき入るようにすればだいじょぶだとおもいます
//     // by いしださん
//     if (st_.mysideNo_ == 1)
//     {
//       // 敵のターゲット位置フリーカーソルモードじゃなければNULLが入ってるはず
//       kdebug::DebugSystem* pDs = kdebug::DebugSystem::GetInstance();
//       if (pDs->IsFreeTargetPosMode())
//       {
//         pmgSG_->stSh_.idata.iball.tagposX = pDs->GetFreeTargetPosX();
//         pmgSG_->stSh_.idata.iball.tagposY = pDs->GetFreeTargetPosY();
//         pmgSG_->stSh_.idata.iball.tagposZ = pDs->GetFreeTargetPosZ();
//       }
//     }
// #endif	// #ifdef __K_DEBUG_SHIAI__
//
//     pmgSG_->stSh_.idata.iball.distX = (pmgSG_->stSh_.idata.iball.tagposX - pmgSG_->stSh_.idata.iball.posX);
//     pmgSG_->stSh_.idata.iball.distY = (pmgSG_->stSh_.idata.iball.tagposY - pmgSG_->stSh_.idata.iball.posY);
//     pmgSG_->stSh_.idata.iball.distZ = (pmgSG_->stSh_.idata.iball.tagposZ - pmgSG_->stSh_.idata.iball.posZ);
//
//     //iballをphsに写す
//     pmgSG_->stSh_.phs = pmgSG_->stSh_.idata.iball;
//     pmgSG_->stSh_.phs.spdDecFromWave = 1.0f;
//     //必殺シュートなら素材を移す
//     if ((ShDt.sType == shtHS) || ShDt.sType == shtJHS)
//     {
//       //必殺シュートフラグON
//       pmgSG_->stSh_.idata.iball.spShot_f = TRUE;
//
//       //現段階のsdataからElmに移動
//       pmgSG_->stSh_.RemoveSdataToElm(&pmgEO_->mgDt_.dtElem_);
//       //地上変化素材があるかのチェック
//       pmgSG_->stSh_.cmn.groundElm_f = pmgSG_->stSh_.sdata.groundElm_f;
//
//       //標的素材Elm⇒Crt
//       pmgSG_->stSh_.RemoveElmToCrt();
//       //速度決定…まずはデフォ、速度＋、速度－素材で増減
//       //pmgEO_->mgDt_.dtSet_.GetDtShot(setHissatsuShotSpd) / (f32)XYMAG;
//       //デフォ速度を入れる
//       pmgSG_->stSh_.cmn.defSpd = (f32)pmgEO_->mgDt_.dtSet_.GetDtShot(setHissatsuShotSpd) / (f32)XYMAG;
//       //速度＋素材  速度－素材
//       s32 speedTotalLv = pmgSG_->stSh_.idata.eff.speedInc - pmgSG_->stSh_.idata.eff.speedDec;
//       pmgSG_->stSh_.phs.spdLv = speedTotalLv;
//
//       //とりあえず速度加算データ
//       enRankLevel rankSwitch = rkBallSpdInc;
//
//       //速度減算データ→読み込みデータは速度マイナス用、Lvはー１を掛けて正にする
//       if (speedTotalLv < 0)  rankSwitch = rkBallSpdDec, speedTotalLv = speedTotalLv * (-1);
//       //速度加算or減算があったらデフォ速度を更新する
//       if (speedTotalLv != 0)
//       {
//         f32 rankSpd = pmgEO_->mgDt_.dtRank_.GetRankDtLevel(rankSwitch, speedTotalLv);
//         f32 spdSign = (f32)pmgSG_->stSh_.phs.spdLv / fabs(pmgSG_->stSh_.phs.spdLv);
//         pmgSG_->stSh_.idata.iball.spd = pmgSG_->stSh_.cmn.defSpd + (rankSpd * spdSign) / XYMAG;
//       }
//       else pmgSG_->stSh_.idata.iball.spd = pmgSG_->stSh_.cmn.defSpd;
//       pmgSG_->stSh_.phs.spd = pmgSG_->stSh_.idata.iball.spd;
//
//       //速度追加、減少の効果素材があるかもなので
//       //効果素材のチェックを行う
//       pmgSG_->stSh_.SetEffectElmFlag();//効果素材初期処理
//       pmgSG_->stSh_.CheckEffectElmFlag();//効果素材段階チェック
//       pmgSG_->stSh_.cmn.hShot_f = TRUE;
//       //速度レベル
//       //      s32 spdLv = pmgSG_->stSh_.phs.spdLv;
//       s32 spdLv = pmgSG_->stSh_.phs.spdLv + pmgSG_->stSh_.phs.spdMomLv;
//       //Lv０のときのデフォ速度
//       pmgSG_->stSh_.phs.spd = pmgSG_->stSh_.cmn.defSpd;
//       //Lv０以外のとき
//       if (spdLv != 0)
//       {
//         //とりあえず速度加算データ
//         enRankLevel rankSwitch = rkBallSpdInc;
//
//         //加算or減算
//         f32 spdSign = spdLv / abs(spdLv);
//
//         //速度減算データ→読み込みデータは速度マイナス用、Lvはー１を掛けて正にする
//         if (spdLv < 0)  rankSwitch = rkBallSpdDec, spdLv = spdLv * (-1);
//         //
//         f32 rkSpd = pmgEO_->mgDt_.dtRank_.GetRankDtLevel(rankSwitch, spdLv);
//         //速度加算or減算があったらデフォ速度を更新する
//         pmgSG_->stSh_.phs.spd = pmgSG_->stSh_.phs.spd + (spdSign * rkSpd) / XYMAG;
//         if (pmgSG_->stSh_.phs.spd <= 0.5f) pmgSG_->stSh_.phs.spd = 0.5f;
//       }
//
//     }
//
//     //現在の速度位置を保存用データに入れる
//     pmgSG_->stSh_.cmn.prsPX = st_.pstBa_->Zahyou.X;//現在の位置X
//     pmgSG_->stSh_.cmn.prsPY = st_.pstBa_->Zahyou.Y;//現在の位置Y
//     pmgSG_->stSh_.cmn.prsPZ = st_.pstBa_->Zahyou.Z;//現在の位置Z
//     pmgSG_->stSh_.cmn.prsSX = st_.pstBa_->Zahyou.dX;//現在の速度X
//     pmgSG_->stSh_.cmn.prsSY = st_.pstBa_->Zahyou.dY;//現在の速度Y
//     pmgSG_->stSh_.cmn.prsSZ = st_.pstBa_->Zahyou.dZ;//現在の速度Z
//     pmgSG_->stSh_.cmn.prsTX = tagzahyou->X;//現在の敵位置X
//     pmgSG_->stSh_.cmn.prsTY = tagzahyou->Y;//現在の敵位置Y
//     pmgSG_->stSh_.cmn.prsTZ = tagzahyou->Z;//現在の敵位置Z
//     pmgSG_->stSh_.cmn.stopPstData_f = FALSE;
//
//     //主軌道計算
//     pmgSG_->stSh_.SAngle();
//     //二次・三角関数初期化フラグをオフ
//     pmgSG_->stSh_.idata.initAccTri_f = FALSE;
//
//     //ボール形状
//     pmgSG_->stSh_.InitShapeIdLv();//ID,Lvの初期化
//     pmgSG_->stSh_.InitShapeData(&pmgEO_->mgDt_.dtBaAni_, &pmgEO_->mgDt_.dtElem_);
//
//     //    st_.pmgBa_->baCommon_.SetMtype(bmShoot);
//
//     //投げ終わり隙カウンタ
//     const s32 QSHCAREVTIME = 40;
//     st_.pmgMyTm_->st_.pstMyTm_->COMDt.shAfter_c = QSHCAREVTIME;
//
//     //タイマーリセット
//     if ((pmgSG_->stBa_.ShTgPNo != NGNUM)
//       && (pmgSG_->stBa_.ShTgPNo <= (s32)dbpoI3))
//     {
//       st_.pmgBa_->baCommon_.ResetTimer(NGNUM, TRUE);
//     }
//
//     //効果音
//     //    if (ShDt.HS_f) st_.pmgBa_->baCommon_.HomingSESet(seHShot);
//     //    else if (ShDt.tNoTagHS_f) st_.pmgBa_->baCommon_.HomingSESet(seHShot);
//     //    else if (ShDt.tNice_f || pmgSG_->stBa_.counter_f) st_.pmgBa_->baCommon_.HomingSESet(seNShot);
//     //    else st_.pmgBa_->baCommon_.HomingSESet(seShot);
//
//     //マリオネット効果を持っていたら標的にフラグを渡してしまう。ChActionで色々やる
//     if (pmgSG_->stSh_.cmn.efEl.symCtrl_f == TRUE)
//     {
//         st_.pmgEnTm_->st_.pmgMyCh_[pmgSG_->stBa_.ShTgPNo]->st_.pstMyCh_->symCtrl_f = TRUE;
// //        st_.pmgEnTm_->st_.pmgMyCh_[pmgSG_->stBa_.ShTgPNo]->st_.pstMyCh_->Zahyou.Y = 1 * XYMAG;
//     }
//     if (ShDt.HS_f)//必殺フラグ
//     {
//       //必殺シュートの音をデフォで入れておく
//       enSEID seID = seHShot;
//       //０段階目に鳴る効果音があればその番号の音IDをセットする
//       if ((pmgSG_->stSh_.cmn.efEl.sePhase.phase_f == TRUE) && (pmgSG_->stSh_.cmn.efEl.sePhase.incld_f == TRUE))
//       {
//         s32 zero_f = 1;
//         if (pmgSG_->stSh_.idata.eff.sePhase_Num[0] == 100) zero_f = 0;
//         seID = (enSEID)((pmgSG_->stSh_.GetSENum(pmgSG_->stSh_.idata.eff.sePhase_Num[0] + 1)) * zero_f);
//       }
//       else if (pmgSG_->stSh_.crt.trg.backward_Time != 0) seID = (enSEID)36;//逆進中はため音
//
//       //上で音の変更無く、１段階しかなく、最終音フラグが立っていたらならす
//       if ((seID == seHShot) && (pmgSG_->stSh_.cmn.phaseAll == 0) && (pmgSG_->stSh_.cmn.efEl.finalSE_f == TRUE))
//       {
//         //ファイナルSEあればそれを鳴らす。
//         s32 zero_f = 1;
//         if (pmgSG_->stSh_.idata.eff.finalSE == 100) zero_f = 0;
//
//         seID = (enSEID)((pmgSG_->stSh_.idata.eff.finalSE + 1) * zero_f);
//       }
//
//       if (seID != 36) st_.pmgBa_->baCommon_.HomingSESet(seID);
//       else st_.pmgBa_->baCommon_.SESet(seID);
//     }
//     else if (ShDt.tNoTagHS_f) st_.pmgBa_->baCommon_.HomingSESet(seHShot);
//     else if (ShDt.tNice_f || pmgSG_->stBa_.counter_f) st_.pmgBa_->baCommon_.HomingSESet(seNShot);
//     else st_.pmgBa_->baCommon_.HomingSESet(seShot);
//
//
//     //IOS
//     if (mid::midIsTBL())
//     {
//       if (ShDt.HS_f) //ひっさつ
//       {
//         // プレビュー時は素材全て表示
//         if (pmgSG_->stRf_.Prev_f)
//         {
//           pmgGO_->pmgCommon_->ResetPopupMsg();
//
//           for (s32 i = 0; i < 8; i++)
//           {
//             std::string hsname = "";
//             s64 sozai = st_.pstMyCh_->stGousei.shot_[0].elem_[i];
//             if (sozai <= 0){ break; }
//             hsname = pmgEO_->mgDt_.dtShElm_.GetDtStr(sozai, szs_name);//素材名
//
//             pmgGO_->pmgCommon_->PopupMsgDelay(pu_Act, hsname.c_str(),
//               st_.mysideNo_, st_.pstMyCh_->Zahyou.X + 1200, st_.pstMyCh_->Zahyou.Y, st_.pstMyCh_->Zahyou.Z/* + (i * -1200)*/, FC2_shiai, (i * 40), FALSE);
//           }
//           return;
//         }
//
//         BOOL syn_f = (ShDt.HSTiming == NGNUM)
//           ? st_.pstMyCh_->stGousei.vjsyn_f_//垂直ジャンプ
//           : st_.pstMyCh_->stGousei.syn_f_[ShDt.HSTiming];
//         //素材名
//         std::string hsname = "";
//         if (syn_f)
//         {
//           hsname = pmgEO_->mgDt_.dtShSyn_.GetName(pmgSG_->stSh_.idata.shotId);//合成名
//         }
//         else
//         {
//           if (ShDt.orishno != 0)
//           {
//             hsname = pmgEO_->mgDt_.dtShSyn_.GetOriShName(ShDt.orishno);//オリジナル名
//           }
//           else
//           {
//             hsname = pmgEO_->mgDt_.dtShElm_.GetDtStr(pmgSG_->stSh_.idata.shotId, szs_name);//素材名
//           }
//         }
//
//         pmgGO_->pmgCommon_->PopupMsg(pu_Act, hsname.c_str(),
//           st_.mysideNo_, st_.pstMyCh_->Zahyou.X, st_.pstMyCh_->Zahyou.Y, st_.pstMyCh_->Zahyou.Z, FC2_shiai, FALSE);
//       }
//     }
//     else//Windows
//     {
//       // プレビュー時は素材全て表示
//       if (pmgSG_->stRf_.Prev_f)
//       {
//         pmgGO_->pmgCommon_->ResetPopupMsg();
//
//         for (s32 i = 0; i < 8; i++)
//         {
//           std::string hsname = "";
//           s64 sozai = st_.pstMyCh_->stGousei.shot_[0].elem_[i];
//           if (sozai <= 0){ break; }
//           hsname = pmgEO_->mgDt_.dtShElm_.GetDtStr(sozai, szs_name);//素材名
//
//           pmgGO_->pmgCommon_->PopupMsgDelay(pu_Act, hsname.c_str(),
//             st_.mysideNo_, st_.pstMyCh_->Zahyou.X + 1200, st_.pstMyCh_->Zahyou.Y, st_.pstMyCh_->Zahyou.Z/* + (i * -1200)*/, FC2_shiai, (i * 40), FALSE);
//         }
//         return;
//       }
//
//       //プレビューでエラー出るのでちょっといったん消す
//       ////合成名
//       //std::string hsname = pmgEO_->mgDt_.dtShSyn_.GetName(pmgSG_->stSh_.idata.shotId);//まだ仮
//       //pmgGO_->pmgCommon_->PopupMsg(pu_Act, hsname.c_str(), st_.mysideNo_, st_.pstMyCh_->Zahyou.X, st_.pstMyCh_->Zahyou.Y, st_.pstMyCh_->Zahyou.Z, FC2_shiai, FALSE);
//     }
//   }
}