#include "dbChGen.h"
#include "dbGame.h"

namespace db
{
  //コンストラクタ
  TChGen::TChGen(TGame* pGame, s32 sideNo, s32 objNo)
  : TGameObjGen(pGame)
  {
    st_.Init();
    st_.mysideNo_ = sideNo;
    st_.objNo_ = objNo;
    st_.ensideNo_ = (st_.mysideNo_ == 0) ? 1 : 0;//敵サイド番号
    
    //RefSet();
  }
  TChGen::~TChGen()
  {
  }
  //初期化（ポインタセットし直し、全てのオブジェが生成された後でないとポインタが確定してない）
  void TChGen::RefSet()
  {
    //マネージャーポインタ
    st_.pmgMyTm_ = pmgGO_->pmgTm_[st_.mysideNo_];
    st_.pmgEnTm_ = pmgGO_->pmgTm_[st_.ensideNo_];
    st_.pmgBa_ = pmgGO_->pmgBa_;
    st_.pmgRf_ = pmgGO_->pmgRf_;
    
    //ステータスポインタ
    st_.pstRf_ = &pmgSG_->stRf_;//レフェリーステータス
    st_.pstBa_ = &pmgSG_->stBa_;//ボールステータス
    st_.pstMyTm_ = &pmgSG_->stTm_[st_.mysideNo_];//チームステータス
    st_.pstEnTm_ = &pmgSG_->stTm_[st_.ensideNo_];//チームステータス
    st_.pstMyCh_ = &pmgSG_->stCh_[st_.mysideNo_][st_.objNo_];//とりあえず（まだチーム内ポジションが確定してない）
    
    DoRefSet();
  }
  void TChGen::DoRefSet()
  {
    //継承先で必要あれば定義
  }
  //チーム情報が確定してから
  void TChGen::Ready()
  {
    st_.posNo_ = st_.pstMyTm_->PosSt.PosofC[st_.objNo_];
    st_.pstMyCh_ = &pmgSG_->stCh_[st_.mysideNo_][st_.posNo_];
    
	st_.id_kantoku_ = st_.pstMyTm_->PosSt.id_kantoku_;
    st_.id_team_ = st_.pstMyTm_->PosSt.id_team_;
    st_.mid_team_ = st_.pstMyTm_->PosSt.mid_team_;
    st_.capid_ = pmgEO_->mgCoM_.GetCharID(st_.mid_team_, 0);
    DoReady();
  }
  void TChGen::DoReady()
  {
    //継承先で必要あれば定義
  }
  
  TMgPad* TChGen::MyPad()
  {
    if (st_.pstMyCh_->stPad.MANChar_f)//★専用操作
    {
      return st_.pstMyCh_->stPad.pCharPad_;
    }
    else
    {
      return st_.pmgMyTm_->st_.pSidePad_;
    }
  }
  
  s32 TChGen::MyStLv(endbStateType dtNo)//自データ
  {
    s32 res = 0;
    switch (dtNo)
    {
    case db::dbst_Level:
      res = st_.pstMyCh_->level; //レベル
      break;
    case db::dbst_HP:
      res = pmgEO_->mgDt_.dtRank_.GetRankHP(st_.pstMyCh_->HP, pmgEO_->mgDt_.dtSet_.GetDtDamage(setHP));
      break;
    case db::dbst_Speed:
      res = pmgEO_->mgDt_.GetState(dtNo, st_.pstMyCh_->id_char);//各ステータス
      break;
    default:
		/*
	  // 元々マスターに直接書かれていたパラメータをレベルに応じて計算して出すように修正
	  s64 memid = st_.pstMyCh_->id_char;
	  s64 id_team = st_.id_team_;
	  s64 mid_team = st_.mid_team_;
	  s32 posno = st_.posNo_;

	  // チームレベル(限界突破値)を取得
	  s64 tmLv = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);
	  // 実際に計算する
	  s64 Lv = pmgEO_->mgCoU_.GetTmCharaLvByUserID(id_team, posno);

	  switch(dtNo)
	  {
	  case dbst_Pow: res = pmgEO_->CalcSt(mid_team, id_team, memid, clv_Pow, Lv, tmLv);break;//ぼーるぱわー	
	  case dbst_Guts:res = pmgEO_->CalcSt(mid_team, id_team, memid, clv_Guts, Lv, tmLv);break;//きあい
	  case dbst_Tech:res = pmgEO_->CalcSt(mid_team, id_team, memid, clv_Tech, Lv, tmLv);break;//てく
	  case dbst_Kire:res = pmgEO_->CalcSt(mid_team, id_team, memid, clv_Kire, Lv, tmLv);break;//きれ
	  }
	  */
	  
		switch (dtNo)
		{
		case dbst_Pow: res = st_.pstMyCh_->stPow; break;//ぼーるぱわー	
		case dbst_Guts:res = st_.pstMyCh_->stGuts; break;//きあい
		case dbst_Kire:res = st_.pstMyCh_->stKire; break;//きれ
		case dbst_Tech:res = st_.pstMyCh_->stTech; break;//てく
		}

      //特効倍率
	  if ((st_.mysideNo_ == SIDE0) && (pmgEO_->stShiai_.IsNormal() || pmgEO_->stShiai_.IsTimeAttack()))
      {
		res = lib_num::Percent(res, pmgEO_->stShiai_.GetTokkouMag());

		//s64 dt = pmgEO_->mgDt_.GetState(dtNo, st_.pstMyCh_->id_char);
        //res = lib_num::Percent(dt, pmgEO_->stShiai_.GetTokkouMag());
      }
      break;
    }

    return res;
  }
  s32 TChGen::MyPDt(enCharLv dtNo)//自データ
  {
	  s32 res = 0;
	  switch(dtNo)
	  {
	  case clv_Pow:	st_.pstMyCh_->stPow; break;//ぼーるぱわー	
	  case clv_Guts:st_.pstMyCh_->stGuts; break;//きあい
	  case clv_Kire:st_.pstMyCh_->stKire; break;//きれ
	  case clv_Tech:st_.pstMyCh_->stTech; break;//てく
	  /*
	  {
		  // 元々マスターに直接書かれていたパラメータをレベルに応じて計算して出すように修正
		  s64 memid = st_.pstMyCh_->id_char;
		  s64 id_team = st_.id_team_;
		  s64 mid_team = st_.mid_team_;
		  s32 posno = st_.posNo_;

		  // チームレベル(限界突破値)を取得
		  s64 tmLv = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);
		  // 実際に計算する
		  s64 Lv = pmgEO_->mgCoU_.GetTmCharaLvByUserID(id_team, posno);

		  res = pmgEO_->CalcSt(mid_team, id_team, memid, dtNo, Lv, tmLv);
	  }
		  break;
		  */
	  default:
		  res = pmgEO_->mgDt_.dtChar_.GetLv(dtNo, st_.pstMyCh_->id_char);
		  break;
	  }

	  return res;
  }
  std::string TChGen::MyName()//自データ
  {
    return pmgEO_->mgDt_.dtChar_.GetName(st_.pstMyCh_->id_char);
  }
  std::string TChGen::MySerifu()//自データ
  {
    return pmgEO_->mgDt_.dtChar_.GetSerifu(st_.pstMyCh_->id_char);
  }

  BOOL TChGen::IsHSOK(enCharLv dtNo)//ひっさつＯＫ
  {
    if ((MyPDt(dtNo) == 0)) return FALSE;
    return TRUE;
  }
  
  //状態取得
  BOOL TChGen::IsInfield()//内野
  {
    return (st_.posNo_ <= (s32)dbpoI3);
  }
  BOOL TChGen::Kagami_f()//かがみ待ちかどうか
  {
    return (st_.pstMyCh_->Kagami_c > 0);
  }
  BOOL TChGen::IsCtrl()//操作権
  {
    return (st_.pmgMyTm_->st_.pstMyTm_->CtrlNo == st_.posNo_);
  }
  BOOL TChGen::IsCOM() //COM
  {
    if (st_.pstMyCh_->stPad.MANChar_f)//★専用操作
    {
      return FALSE;// st_.pstMyCh_->stPad.OmksChar_f;
    }
    else
    {
      return st_.pmgMyTm_->IsCOM(); //サイドがＣＯＭ
    }
  }
  BOOL TChGen::IsSelfCtrl()//自分で操作するキャラかどうか
  {
    if (st_.pstMyCh_->stPad.MANChar_f)
    {
      return TRUE;// (st_.pstMyCh_->stPad.OmksChar_f == FALSE);//★専用操作
    }
    else
    {
      return (IsCtrl() && (IsCOM() == FALSE)); //サイド
    }
  }
  
  BOOL TChGen::IsMAN()//自分で操作するキャラ（操作権やカバーマンを渡せない状態）
  {
    return (st_.pstMyCh_->stPad.MANChar_f);// && (st_.pstMyCh_->stPad.OmksChar_f == FALSE));
  }
  
  
  BOOL TChGen::IsCtrlOK()//サイド操作権を渡せるキャラか
  {
    return ((st_.pstMyCh_->Dead_f == FALSE)
            && (st_.pstMyCh_->Motion.IsMFlags(dbmfDam) == FALSE)
            && (IsMAN() == FALSE));//★専用操作以外かお任せ状態か
  }
  BOOL TChGen::PaCvrMan_f()
  {
    return ((st_.pmgMyTm_->st_.pstMyTm_->CvrNo == st_.posNo_) || (st_.pmgMyTm_->st_.pstMyTm_->Cvr2No == st_.posNo_))
    && (pmgSG_->stBa_.Motion == bmPass)
    && (pmgSG_->stBa_.PaTgTNo == st_.mysideNo_)
    && (pmgSG_->stBa_.PaTgPNo != st_.posNo_)
    && (pmgSG_->stBa_.PaTgPNo <= (s32)dbpoI3);
  }
  BOOL TChGen::IsShTgOK()//ねらえる状態かどうか
  {
    BOOL res_f = FALSE;
    
    if ((st_.pstMyCh_->Dead_f == FALSE)
        && (st_.pstMyCh_->HP > 0)
        && (st_.pstMyCh_->Motion.IsMFlags(dbmfANG) == FALSE))
    {
      if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr)
          && st_.pstMyCh_->Motion.IsMFlags(dbmfDam))
      {
        res_f = (st_.pstMyCh_->Zahyou.Y <= (pmgEO_->mgDt_.dtSet_.GetDtShot(setFlyTagHeight) * XYMAG));
      }
      else
      {
        res_f = TRUE;
      }
    }
    return res_f;
    
    
  }
  
  BOOL TChGen::IsCallingOK()//呼べる状態か
  {
    BOOL res_f = FALSE;
    
    if ((st_.pstMyCh_->Dead_f == FALSE)
        && (st_.pstMyCh_->HP > 0)
        && (st_.pstMyCh_->Motion.IsMFlags(dbmfANG) == FALSE))
    {
      
      if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr) && st_.pstMyCh_->Motion.IsMFlags(dbmfDam))
      {
        res_f = FALSE;
      }
      else if ((st_.pstMyCh_->Motion.Mtype == dbmtRoB)
               || (st_.pstMyCh_->Motion.Mtype == dbmtRoF))
      {
        res_f = FALSE;
      }
      else
      {
        res_f = TRUE;
      }
    }
    return res_f;
  }
  
  
  BOOL TChGen::IsNoGuard(BOOL adDam_f)//ノーガード状態か
  {
    if (adDam_f)
    {
      return (Kagami_f()
              || st_.pstMyCh_->Motion.IsMFlags(dbmfDn)
              || st_.pstMyCh_->Motion.IsMFlags(dbmfDam));//ダメージ状態も
    }
    else
    {
      return (Kagami_f() || st_.pstMyCh_->Motion.IsMFlags(dbmfDn));
    }
  }
  
  s32 TChGen::GetLeftCrtX()//T0の外野とT1の内野は右に居るので反転
  {
    //T0の外野とT1の内野は右に居るので反転
    if ((IsInfield() && (st_.mysideNo_ == 0))
        || ((IsInfield() == FALSE) && (st_.mysideNo_ != 0)))
    {
      return st_.pstMyCh_->Zahyou.X;
    }
    else
    {
      return (DBCRT_W - st_.pstMyCh_->Zahyou.X);
    }
  }
  
  BOOL TChGen::IsDMOKPos()
  {
    return (GetLeftCrtX() < DBCRT_CLXL + (32 * XYMAG));//コートの中央より後ろ側のみ
  }
  void TChGen::SetLeftCrtX(s32 value)//T0の外野とT1の内野は右に居るので反転
  {
    
    if ((IsInfield() && (st_.mysideNo_ == 0))
        || ((IsInfield() == FALSE) && (st_.mysideNo_ != 0)))
    {
      st_.pstMyCh_->Zahyou.X = value;
    }
    else
    {
      st_.pstMyCh_->Zahyou.X = (DBCRT_W - value);
    }
  }
  void TChGen::SetLeftCrtX(s32 value, enCrtLine lineType)//T0の外野とT1の内野は右に居るので反転
  {
    BOOL posrev_f = FALSE;
    s32 pos = value;
    
    if ((IsInfield() && (st_.mysideNo_ == 0))
        || ((IsInfield() == FALSE) && (st_.mysideNo_ != 0)))
    {
      //内野のＬＬＩＮＥはエンドラインなので沿って↑押しでがたつかないようにここは強制
      if (lineType == crtLLine)
      {
        //posrev_f = ((st_.pstMyCh_->Zahyou.dX < 0) && (st_.pstMyCh_->Zahyou.X < pos));
        posrev_f = (st_.pstMyCh_->Zahyou.X < pos);
      }
      else if (lineType == crtRLine)
      {
        posrev_f = ((st_.pstMyCh_->Zahyou.dX > 0) && (st_.pstMyCh_->Zahyou.X > pos));
      }
    }
    else
    {
      pos = (DBCRT_W - value);
      
      if (lineType == crtLLine)
      {
        posrev_f = ((st_.pstMyCh_->Zahyou.dX > 0) && (st_.pstMyCh_->Zahyou.X > pos));
      }
      else if (lineType == crtRLine)
      {
        posrev_f = ((st_.pstMyCh_->Zahyou.dX < 0) && (st_.pstMyCh_->Zahyou.X < pos));
      }
    }
    
    if (posrev_f) st_.pstMyCh_->Zahyou.X = pos;
  }
  
  s32 TChGen::NowKoma(enBMCommonDt dtNo)
  {
    return pmgEO_->mgDt_.dtBM_.GetDt((s32)st_.pstMyCh_->Motion.MNo, st_.pstMyCh_->Anime.FrameNo, dtNo);
  }
  BOOL TChGen::NowKomaBOOL(enBMCommonDt dtNo)
  {
    return (NowKoma(dtNo) != 0);
  }
  
  //最終コマか
  BOOL TChGen::IsLastKoma()
  {
    return (pmgEO_->mgDt_.dtBM_.Getkomanum((s32)st_.pstMyCh_->Motion.MNo) == (st_.pstMyCh_->Anime.FrameNo + 1));
  }
  
  BOOL TChGen::IsInplay()//動作可能か
  {
    return (pmgSG_->stRf_.DBSSt == dbssShiai)
    || (pmgSG_->stRf_.DBSSt == dbssGameEnd)
    || (pmgSG_->stRf_.DBSSt == dbssGohome);
  }
  
  BOOL TChGen::IsShiai()//操作可能か
  {
    return (st_.pmgRf_->IsShiai());
  }


  BOOL TChGen::IsTuto()//チュートリアルか
  {
    return (pmgEO_->stShiai_.IsTuto());
  }
  BOOL TChGen::IsNoDamage()//自軍ダメージ無し
  {
    return (st_.pmgRf_->IsNoDamage());
  }
  BOOL TChGen::IsOKDodge()//避けＯＫ
  {
    return (st_.pmgRf_->IsOKDodge());
  }
  BOOL TChGen::IsOKCatch()//キャッチＯＫ
  {
    return (st_.pmgRf_->IsOKCatch());
  }
  BOOL TChGen::IsOKWalk()//あるきＯＫ
  {
    return (st_.pmgRf_->IsOKWalk());
  }
  BOOL TChGen::IsOKDash()//ダッシュＯＫ
  {
    return (st_.pmgRf_->IsOKDash());
  }
  BOOL TChGen::IsOKJump()//ジャンプＯＫ
  {
    return (st_.pmgRf_->IsOKJump());
  }
  BOOL TChGen::IsOKShot()//シュートＯＫ
  {
    return (st_.pmgRf_->IsOKShot());
  }
  BOOL TChGen::IsOKPass()//パスＯＫ
  {
    return (st_.pmgRf_->IsOKPass());
  }
  void TChGen::SetActBtn(enSendTutoAct tutono)//アクションボタンが押された
  {
    st_.pmgRf_->SetTutoActBtn(tutono);
  }
  void TChGen::SetTutoNG(enSendTutoAct tutono, s32 ngno)//チュートリアルＮＧ
  {
    st_.pmgRf_->SetTutoNG(tutono, ngno);
  }
  void TChGen::SetTutoOK(enSendTutoAct tutono)//チュートリアルＯＫ
  {
    st_.pmgRf_->SetTutoOK(tutono);
  }
  void TChGen::IncTutoAct_c(enSendTutoAct tutono)//アクションカウンタ増加
  {
    st_.pmgRf_->IncTutoAct_c(tutono);
  }
  
  BOOL TChGen::GameEndWait_f()//操作可能か
  {
    return pmgSG_->stRf_.GameSet_f;
  }
  
  BOOL TChGen::IsDashman()//メンバー操作も含む★
  {
    //メンバー操作＆（敵方向ダッシュＯＲコート前方ジャンプ）でダッシュマン扱い
    BOOL endsMuki_f = ((st_.mysideNo_ == 0) && (st_.pstMyCh_->Zahyou.DsMuki == mR))
    || ((st_.mysideNo_ == 1) && (st_.pstMyCh_->Zahyou.DsMuki == mL));
    
    BOOL ends_f = ((st_.pstMyCh_->Motion.Mtype == dbmtDs) && endsMuki_f);
    
    BOOL enjp_f = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs)
                   && st_.pstMyCh_->Motion.IsMFlags(dbmfAr)
                   && (st_.pstMyCh_->Motion.Mtype != dbmtSl)
                   && (GetLeftCrtX() > DBCRT_CLXL)//ある程度前方
                   );
    
    BOOL mandsmn_f = (IsMAN()
                      && (IsBall() == FALSE)
                      && (ends_f || enjp_f));
    
    
    
    BOOL dsman_f = ((st_.pstMyCh_->Auto.AutoType == dbatDashman)
                    || enjp_f//★ミラー（ジャンプ後ダッシュマンでなくなってしまう）
                    || mandsmn_f);
    
    const s32 DIVELIMDY = (-XYMAG * 1);
    BOOL nodive_f = (st_.pstMyCh_->Zahyou.dY > DIVELIMDY);
    
    return (dsman_f && nodive_f && IsFreeMotion(FALSE));
  }

  
  //ダッシュ加速ボタン
  BOOL TChGen::DashAccBtn_f()
  {
    if (IsSelfCtrl() == FALSE) return FALSE;

    //向きと同じ方向のＬＲが押されてる
    if (((st_.pstMyCh_->Zahyou.Muki == mL) && MyPad()->IsDashAcc(dxL))
      || ((st_.pstMyCh_->Zahyou.Muki == mR) && MyPad()->IsDashAcc(dxR))
        || st_.pstMyCh_->DsAccIOS_f)//ダッシュ後追加ダッシュが入った
    {
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }

  //バックダッシュ
  BOOL TChGen::BackDs_f()
  {
    if (IsSelfCtrl() == FALSE) return FALSE;

    //向きとは逆方向に走っている
    if (((st_.pstMyCh_->Zahyou.Muki == mL) && (st_.pstMyCh_->Zahyou.DsMuki == mR))
        || ((st_.pstMyCh_->Zahyou.Muki == mR) && (st_.pstMyCh_->Zahyou.DsMuki == mL)))
    {
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }
  
  //エンドライン
  s32 TChGen::GetSLX()
  {
    return (DBCRT_SL + ((st_.pstMyCh_->Zahyou.Z - DBCRT_FL) * 10 / DBCRT_SLR));
  }
  //シュートタゲかどうか
  BOOL TChGen::ShTag_f()
  {
    return (((pmgSG_->stBa_.Motion == bmHold)
             || (pmgSG_->stBa_.Motion == bmShoot))
            && (st_.mysideNo_ == pmgSG_->stBa_.ShTgTNo)
            && (st_.posNo_ == pmgSG_->stBa_.ShTgPNo));
  }
  //パスタゲかどうか
  BOOL TChGen::PaTag_f()
  {
    return ((pmgSG_->stBa_.Motion == bmHold)
            && (pmgSG_->stBa_.OvLine_f == FALSE)
            && (st_.mysideNo_ == pmgSG_->stBa_.PaTgTNo)
            && (st_.posNo_ == pmgSG_->stBa_.PaTgPNo));
  }
  //パス待ちかどうか
  BOOL TChGen::PassWait_f()
  {
    return (PaTag_f()
            && (IsBall() == FALSE)
            //&& (IsCtrl() == FALSE)
            && (IsMAN() == FALSE)
            && IsShiai());
  }
  //ボール持ちかどうか
  BOOL TChGen::IsBall()
  {
    return ((st_.mysideNo_ == pmgSG_->stBa_.HoldTNo)
            && (st_.posNo_ == pmgSG_->stBa_.HoldPNo));
  }
  
  //デバッグ表示用
  BOOL TChGen::Monitor_f()
  {
    return (IsCtrl() && (st_.mysideNo_ == 1));
  }
  
  //自由状態かどうか
  BOOL TChGen::IsFreeMotion(BOOL checkAutoType_f)
  {
    if ((st_.pstMyCh_->Dead_f == FALSE)
        && (st_.pstMyCh_->Live_f)
        && (Kagami_f() == FALSE)
        && (st_.pstMyCh_->Motion.IsMFlags(dbmfDam) == FALSE)
        && (st_.pstMyCh_->Motion.IsMFlags(dbmfANG) == FALSE)
        && (st_.pstMyCh_->Motion.IsMFlags(dbmfPHit) == FALSE))
    {
      if (checkAutoType_f)//オートの時
      {
        //どれかならＯＫ
        return ((st_.pstMyCh_->Auto.AutoType == dbatFree)
                || (st_.pstMyCh_->Auto.AutoType == dbatGet)//新規追加
                || (st_.pstMyCh_->Auto.AutoType == dbatAlleyoop)
                || (st_.pstMyCh_->Auto.AutoType == dbatDfPos)
                || (st_.pstMyCh_->Auto.AutoType == dbatCPUHelp)
                || (st_.pstMyCh_->Auto.AutoType == dbatShCv)
                || (st_.pstMyCh_->Auto.AutoType == dbatCPUShPa)
                || (st_.pstMyCh_->Auto.AutoType == dbatPassCvr)
                );
      }
      else
      {
        return TRUE;
      }
    }
    else
    {
      return FALSE;
    }
    
  }
  
  //ランク
  s32 TChGen::RankLevel(enRankLevel r)
  {
    return pmgEO_->mgDt_.dtRank_.GetRankDtLevel(r, (MyStLv(dbst_Guts)));
  }
  s32 TChGen::RankHP(enRankHP r)
  {
    return pmgEO_->mgDt_.dtRank_.GetRankDtHP(r, (MyStLv(dbst_Guts)));
  }
  s32 TChGen::RankSpeed(enRankSpeed r)
  {
    return pmgEO_->mgDt_.dtRank_.GetRankDtSpeed(r, (MyStLv(dbst_Speed)));
  }

  
}
