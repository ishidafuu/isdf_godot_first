#include "dbChCommon.h"
#include "dbGame.h"

namespace db
{

  const s32 ENABLE = 1;
  const s32 GOUSEI = 2;
  const s32 VJHSTIMING = 3;

  //コンストラクタ
  TChCommon::TChCommon(TGame* pGame, s32 sideNo, s32 charNo)
    : TChGen(pGame, sideNo, charNo)
  {
  }

  TChCommon::~TChCommon()
  {
  }

  void TChCommon::SetShotPrev()
  {
    if (pmgEO_->stShiai_.GetIDShotPrev() == 0) return;
    if (pmgEO_->stShiai_.GetIDShotPrev() == NGNUM) return;

    s64 id_shot = pmgEO_->stShiai_.GetIDShotPrev();
    s32 shotstate = pmgEO_->mgCoU_.mdShot_.GetPKDt(id_shot, mdshot_state);

    pmgEO_->mgDt_.dtShSyn_.ClearOrgShName();

    s32 orishno = 0;
    if (pmgEO_->mgCoU_.mdShot_.GetPKDtStr(id_shot, mdshot_name_c32) != "")
    {
      orishno = pmgEO_->mgDt_.dtShSyn_.SetOriShName(pmgEO_->mgCoU_.mdShot_.GetPKDtStr(id_shot, mdshot_name_c32));
    }

    //全てのステップにプレビューシュートをつける
    if (((shotstate == ENABLE) || (shotstate == GOUSEI))
      && (st_.mysideNo_ == SIDE0))
    {

      for (s32 step = 0; step < STEPNUM; ++step)
      {
        //シュート装備フラグ
        st_.pstMyCh_->stGousei.set_f_[step] = TRUE;
        st_.pstMyCh_->stGousei.syn_f_[step] = FALSE;//自作シュート
        //素材をさらう
        SOZAIDTVECTOR sozaidtvec = pmgEO_->mgCoU_.GetSozaiDataAll(id_shot);

        st_.pstMyCh_->stGousei.shot_[step].shotId = sozaidtvec[0].mid_sozai_;//デバッグ用データ
        st_.pstMyCh_->stGousei.shot_[step].oriNameNo_ = orishno;
        s32 totalpow = 0;
        s32 totalspin = 0;
        pmgEO_->mgCoM_.GetBasePowSpin(sozaidtvec[0], &totalpow, &totalspin, FALSE);
        for (s32 i3 = 0; i3 < sozaidtvec.size(); ++i3)
        {
          st_.pstMyCh_->stGousei.shot_[step].elem_[i3] = sozaidtvec[i3].mid_sozai_;
          st_.pstMyCh_->stGousei.shot_[step].rare_[i3] = sozaidtvec[i3].rarity_;
          s32 revpow = 0;
          s32 revspin = 0;
          pmgEO_->mgCoM_.GetRevPowSpin(sozaidtvec[i3], &revpow, &revspin);
          totalpow += revpow;
          totalspin += revspin;
        }
        st_.pstMyCh_->stGousei.shot_[step].pow_ = totalpow;
        st_.pstMyCh_->stGousei.shot_[step].spin_ = totalspin;
      }
    }
  }

  void TChCommon::SetShotShiai()
  {
    //固定ショット
    const s32 DEFSTEP = 3;
    //const s32 DEFPOWSPIN = 50;//デフォシュートの威力

    s32 defshot = pmgEO_->mgDt_.dtChar_.GetLv(clv_DefShotID, st_.pstMyCh_->id_char);
    for (s32 i2 = 0; i2 < HSELEMMAX; ++i2)
    {
      st_.pstMyCh_->stGousei.set_f_[DEFSTEP] = TRUE;
      st_.pstMyCh_->stGousei.syn_f_[DEFSTEP] = TRUE;//固定シュート
      st_.pstMyCh_->stGousei.shot_[DEFSTEP].elem_[i2] = pmgEO_->mgDt_.dtShSyn_.GetDt(defshot, (enDtShSyn)(gs_base + (i2 * 2)));
      st_.pstMyCh_->stGousei.shot_[DEFSTEP].rare_[i2] = pmgEO_->mgDt_.dtShSyn_.GetDt(defshot, (enDtShSyn)(gs_base_rare + (i2 * 2)));
      st_.pstMyCh_->stGousei.shot_[DEFSTEP].shotId = defshot;//デバッグ用データ
      st_.pstMyCh_->stGousei.shot_[DEFSTEP].pow_ = pmgEO_->mgDt_.dtShSyn_.GetDt(defshot, gs_power);
      st_.pstMyCh_->stGousei.shot_[DEFSTEP].spin_ = pmgEO_->mgDt_.dtShSyn_.GetDt(defshot, gs_spin);
    }

    s64 id_shset = pmgEO_->mgCoU_.mdTeam_.GetPKDt(st_.id_team_, mdteam_id_shset);

    s32 tmrarity = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(st_.mid_team_, mdm_team_rarity);
    s32 overlap = pmgEO_->mgCoU_.mdTeam_.GetPKDt(st_.id_team_, mdteam_overlap);


    for (s32 i = 0; i < SLOTNUM; ++i)
    {
      //問い合わせるチームのスロット番号0~48
      s32 slot = ((i * DBMEMBER_ALL) + st_.posNo_);
      s32 needover = pmgEO_->mgCoM_.mdm_Slot_.GetLineDt(tmrarity, (enmdm_Slot)(mdm_slot_slot00 + slot));//必要なオーバーラップ

      s64 id_shot = NGID;
      if (needover <= overlap)//レベル足りてる
      {
        if (id_shset == NGID)//チーム設定
        {
          id_shot = pmgEO_->mgCoU_.mdTeam_.GetPKDt(st_.id_team_, (enmdTeam)(mdteam_id_shot00 + slot));
        }
        else//シュートセット
        {
          id_shot = pmgEO_->mgCoU_.mdShSet_.GetPKDt(id_shset, (enmdShSet)(mdshset_id_shot00 + slot));
        }
      }

      //s64 id_shot = pmgEO_->mgCoU_.mdTeam_.GetPKDt(st_.id_team_, (enmdTeam)(mdteam_id_shot00 + slot));
      if (id_shot == NGID) continue;

      s32 steptype = pmgEO_->mgDt_.dtChar_.GetLv(clv_Speed, st_.pstMyCh_->id_char);
      s32 step = SHOTSTEP[(steptype * SLOTNUM) + i];
      s32 orgshno = 0;
      if (pmgEO_->mgCoU_.mdShot_.GetPKDtStr(id_shot, mdshot_name_c32) != "")
      {
        orgshno = pmgEO_->mgDt_.dtShSyn_.SetOriShName(pmgEO_->mgCoU_.mdShot_.GetPKDtStr(id_shot, mdshot_name_c32));
      }

      s32 shotstate = pmgEO_->mgCoU_.mdShot_.GetPKDt(id_shot, mdshot_state);

      //全てのステップにプレビューシュートをつける
      s32 useslot = 0;
      if (shotstate == ENABLE)
      {
        useslot = pmgEO_->mgCoU_.mdShot_.GetPKDt(id_shot, mdshot_useslot);
      }
      else if (shotstate == GOUSEI)
      {
        //合成中はuseslotが合成中も含めてしまうので一つ減らす(合成後のおーけーで混ざるのでー１いらない、というかそもそもー１ではない・・・)
        useslot = pmgEO_->mgCoU_.mdShot_.GetPKDt(id_shot, mdshot_useslot)/* - 1*/;
      }
      else
      {
        continue;
      }

      //垂直ジャンプ
      if (step == VJICONSTEP)
      {
        st_.pstMyCh_->stGousei.vjset_f_ = TRUE;
        st_.pstMyCh_->stGousei.vjsyn_f_ = FALSE;//自作シュート
      }
      else
      {
        st_.pstMyCh_->stGousei.set_f_[step] = TRUE;
        st_.pstMyCh_->stGousei.syn_f_[step] = FALSE;//自作シュート
      }

      ////シュート装備フラグ
      //st_.pstMyCh_->stGousei.set_f_[step] = TRUE;
      //st_.pstMyCh_->stGousei.syn_f_[step] = FALSE;//自作シュート
      //素材をさらう
      SOZAIDTVECTOR sozaidtvec = pmgEO_->mgCoU_.GetSozaiDataAll(id_shot);

      //特効倍率
      s32 tokkouper = 100;
      if (pmgEO_->stSeq_.GetMidEvent() > 0)
      {
        tokkouper += pmgEO_->mgCoM_.GetTokkouSozaiMag(pmgEO_->stSeq_.GetMidEvent(), sozaidtvec);
      }


      s32 totalpow = 0;
      s32 totalspin = 0;
      pmgEO_->mgCoM_.GetBasePowSpin(sozaidtvec[0], &totalpow, &totalspin, FALSE);

      if (step == VJICONSTEP)
      {
        st_.pstMyCh_->stGousei.vjshot_.shotId = sozaidtvec[0].mid_sozai_;//デバッグ用データ
        st_.pstMyCh_->stGousei.vjshot_.oriNameNo_ = orgshno;
        for (s32 i3 = 0; i3 < sozaidtvec.size(); ++i3)
        {
          st_.pstMyCh_->stGousei.vjshot_.elem_[i3] = sozaidtvec[i3].mid_sozai_;
          st_.pstMyCh_->stGousei.vjshot_.rare_[i3] = sozaidtvec[i3].rarity_;
          s32 revpow = 0;
          s32 revspin = 0;
          pmgEO_->mgCoM_.GetRevPowSpin(sozaidtvec[i3], &revpow, &revspin);
          totalpow += revpow;
          totalspin += revspin;
        }
        st_.pstMyCh_->stGousei.vjshot_.pow_ = lib_num::Percent(totalpow, tokkouper);
        st_.pstMyCh_->stGousei.vjshot_.spin_ = lib_num::Percent(totalspin, tokkouper);
      }
      else
      {
        st_.pstMyCh_->stGousei.shot_[step].shotId = sozaidtvec[0].mid_sozai_;//デバッグ用データ
        st_.pstMyCh_->stGousei.shot_[step].oriNameNo_ = orgshno;
        for (s32 i3 = 0; i3 < sozaidtvec.size(); ++i3)
        {
          st_.pstMyCh_->stGousei.shot_[step].elem_[i3] = sozaidtvec[i3].mid_sozai_;
          st_.pstMyCh_->stGousei.shot_[step].rare_[i3] = sozaidtvec[i3].rarity_;
          s32 revpow = 0;
          s32 revspin = 0;
          pmgEO_->mgCoM_.GetRevPowSpin(sozaidtvec[i3], &revpow, &revspin);
          totalpow += revpow;
          totalspin += revspin;
        }
		st_.pstMyCh_->stGousei.shot_[step].pow_ = lib_num::Percent(totalpow, tokkouper);
		st_.pstMyCh_->stGousei.shot_[step].spin_ = lib_num::Percent(totalspin, tokkouper);
      }
    }
  }
  void TChCommon::SetShotTest()
  {
    //所持合成シュート（仮）
    for (s32 i = 0; i < STEPNUM; ++i)
    {
      if ((i < 2) || (i > 4)) continue;//てきとう

      st_.pstMyCh_->stGousei.set_f_[i] = TRUE;
      st_.pstMyCh_->stGousei.syn_f_[i] = TRUE;//固定シュート

      s32 charid = pmgEO_->mgCoM_.GetCharID(pmgEO_->stShiai_.GetMidTeam(SIDE1), st_.posNo_);
      s32 shotId = pmgEO_->mgDt_.dtChar_.GetLv(clv_DefShotID, charid);

      for (s32 i2 = 0; i2 < HSELEMMAX; ++i2)
      {
        st_.pstMyCh_->stGousei.shot_[i].elem_[i2] = pmgEO_->mgDt_.dtShSyn_.GetDt(shotId, (enDtShSyn)(gs_base + (i2 * 2)));
        st_.pstMyCh_->stGousei.shot_[i].rare_[i2] = pmgEO_->mgDt_.dtShSyn_.GetDt(shotId, (enDtShSyn)(gs_base_rare + (i2 * 2)));
        st_.pstMyCh_->stGousei.shot_[i].shotId = shotId;//デバッグ用データ
        st_.pstMyCh_->stGousei.shot_[i].pow_ = pmgEO_->mgDt_.dtShSyn_.GetDt(shotId, gs_power);
        st_.pstMyCh_->stGousei.shot_[i].spin_ = pmgEO_->mgDt_.dtShSyn_.GetDt(shotId, gs_spin);
      }
    }
  }


  //シュート変更
  s32 TChCommon::ChangeStShotDebug(s32 mv)
  {
    s32 shotId = st_.pstMyCh_->stGousei.shot_[0].shotId + mv;
    s32 maxid = pmgEO_->mgDt_.dtShSyn_.GetIntvec2Size() - 1;
    if (shotId > maxid) shotId = maxid;
    if (shotId < 0) shotId = 0;
    //所持合成シュート（仮）
    for (s32 i = 0; i < STEPNUM; ++i)
    {
      st_.pstMyCh_->stGousei.set_f_[i] = TRUE;
      for (s32 i2 = 0; i2 < HSELEMMAX; ++i2)
      {
        st_.pstMyCh_->stGousei.shot_[i].elem_[i2] = pmgEO_->mgDt_.dtShSyn_.GetDt(shotId, (enDtShSyn)(gs_base + (i2 * 2)));
        st_.pstMyCh_->stGousei.shot_[i].rare_[i2] = pmgEO_->mgDt_.dtShSyn_.GetDt(shotId, (enDtShSyn)(gs_base_rare + (i2 * 2)));
        st_.pstMyCh_->stGousei.shot_[i].shotId = shotId;//デバッグ用データ
        st_.pstMyCh_->stGousei.shot_[i].pow_ = pmgEO_->mgDt_.dtShSyn_.GetDt(shotId, gs_power);
        st_.pstMyCh_->stGousei.shot_[i].spin_ = pmgEO_->mgDt_.dtShSyn_.GetDt(shotId, gs_spin);
      }
    }

    return shotId;
  }

  //シュート変更
  void TChCommon::ChangeStShotPrev(const SOZAIDTVECTOR& sozaivec, s32 totalpow, s32 totalspin)
  {
    st_.pstMyCh_->stGousei.Init();


    //全てのステップに指定の素材ベクタをつける
    for (s32 step = 0; step < STEPNUM; ++step)
    {
      //シュート装備フラグ
      st_.pstMyCh_->stGousei.set_f_[step] = TRUE;
      st_.pstMyCh_->stGousei.syn_f_[step] = FALSE;//自作シュート
      st_.pstMyCh_->stGousei.shot_[step].oriNameNo_ = 0;//無名


      s32 totalpow = 0;
      s32 totalspin = 0;
      pmgEO_->mgCoM_.GetBasePowSpin(sozaivec[0], &totalpow, &totalspin, FALSE);

      //素材をさらう
      for (s32 i3 = 0; i3 < sozaivec.size(); ++i3)
      {
        st_.pstMyCh_->stGousei.shot_[step].elem_[i3] = sozaivec[i3].mid_sozai_;
        st_.pstMyCh_->stGousei.shot_[step].rare_[i3] = sozaivec[i3].rarity_;
        if (i3 == 0) st_.pstMyCh_->stGousei.shot_[step].shotId = sozaivec[i3].mid_sozai_;
        s32 revpow = 0;
        s32 revspin = 0;
        pmgEO_->mgCoM_.GetRevPowSpin(sozaivec[i3], &revpow, &revspin);
        totalpow += revpow;
        totalspin += revspin;
      }
      st_.pstMyCh_->stGousei.shot_[step].pow_ = totalpow;
      st_.pstMyCh_->stGousei.shot_[step].spin_ = totalspin;
    }
  }

  //ステータス初期化
  void TChCommon::StInit(BOOL iochange_f)
  {
    if (iochange_f)//メンバー操作情報を保持
    {
      TStChManPad tPadSt = st_.pstMyCh_->stPad;
      //ここで初期化
      st_.pstMyCh_->Init();
      st_.pstMyCh_->stPad = tPadSt;
      st_.pstMyCh_->ANGEL_f = TRUE;//初期化してるので再度
      st_.pstMyCh_->Auto.AutoType = dbatReturn;
      st_.pstMyCh_->NextAuto.AutoType = dbatReturn;
    }
    else
    {
      //ここで初期化
      st_.pstMyCh_->Init();
    }

    //ＩＤはpstMyCh_が持つ(MyStLvなどで使うので最初に入れる)
    st_.pstMyCh_->id_char = pmgEO_->mgCoM_.GetCharID(st_.mid_team_, st_.posNo_);

    s32 tmrarity = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(st_.mid_team_, mdm_team_rarity);
    s32 revlevel = pmgEO_->mgDt_.dtChar_.GetLv(clv_Rev, st_.pstMyCh_->id_char);

    st_.pstMyCh_->level = (tmrarity + revlevel);
    st_.pstMyCh_->HP = pmgEO_->mgDt_.dtSet_.GetDtDamage(setHP);

	//---------------------------------------------------------------------------------------------------------
	// 元々マスターに直接書かれていたパラメータをレベルに応じて計算して出すように修正
	s64 memid = st_.pstMyCh_->id_char;
	s64 id_kantoku = st_.id_kantoku_;
	s64 id_team = st_.id_team_;
	s64 mid_team = st_.mid_team_;
	s32 posno = st_.posNo_;

	// チームレベル(限界突破値)を取得
	s64 tmLv = 1;
	s64 Lv = 1;


	// 通信対戦時
	if (pmgEO_->stShiai_.IsVSHum())
	{
		s32 myside = mid::midGetMyIdx();
		
		// 自分
		if (myside == st_.mysideNo_)
		{
			tmLv = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);
			Lv = pmgEO_->mgCoU_.GetMyTmCharaLv(mid_team, posno);
		}
		// 相手
		else
		{
			// 相手の監督データから取得
			tmLv = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);
			Lv = pmgEO_->mgCoU_.GetTmCharaLvByKantokuID(id_kantoku, mid_team, posno);
		}
	}
	// 通信対戦以外
	else
	{
		// 自分
		if (SIDE0 == st_.mysideNo_)
		{
			tmLv = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);
			Lv = pmgEO_->mgCoU_.GetMyTmCharaLv(mid_team, posno);
		}
		// 勝ち抜きイベント時の相手チーム
		else if (pmgEO_->stShiai_.IsTour() && SIDE1 == st_.mysideNo_)
		{
			tmLv = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);
			Lv = pmgEO_->mgCoU_.GetTmCharaLvByKantokuID(id_kantoku, mid_team, posno);
		}
		// COM
		else
		{
			/*
			//敵番号
			s32 enemyNo = pmgEO_->mgCoU_.mdMap_.GetPKDt(stTSL_.id_map_, mdmap_enemyNo);
			enmdm_Maptip enemyTeam = mdm_maptip_id_m_teamA;
			switch (stTSL_.enemyno_)
			{
			case 0: enemyTeam = mdm_maptip_id_m_teamA; break;
			case 1: enemyTeam = mdm_maptip_id_m_teamB; break;
			case 2:enemyTeam = mdm_maptip_id_m_teamC; break;
			}

			//敵ＭＩＤ
			stTSL_.mid_team_en_ = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), enemyTeam);
			*/

			// COMのデータを取得
			// いずれは↑のmdMapやmdm_Maptipなどを使ってcsvに入っているレベルを反映するようにする
			tmLv = 1;
			Lv = 1;
		}
	}


	// 実際に計算する
	for (s32 i = clv_Pow; i <= clv_Tech; i++)
	{
		switch (i)
		{
		case clv_Pow: st_.pstMyCh_->stPow = pmgEO_->CalcSt(mid_team, id_team, memid, clv_Pow, Lv, tmLv); break;//ぼーるぱわー	
		case clv_Guts:st_.pstMyCh_->stGuts = pmgEO_->CalcSt(mid_team, id_team, memid, clv_Guts, Lv, tmLv); break;//きあい
		case clv_Kire:st_.pstMyCh_->stKire = pmgEO_->CalcSt(mid_team, id_team, memid, clv_Kire, Lv, tmLv); break;//きれ
		case clv_Tech:st_.pstMyCh_->stTech = pmgEO_->CalcSt(mid_team, id_team, memid, clv_Tech, Lv, tmLv); break;//てく
		}
	}
	//---------------------------------------------------------------------------------------------------------

    st_.pstMyCh_->comCaPer_ = pmgEO_->stShiai_.GetCaPer();//敵キャッチ確率
    st_.pstMyCh_->comNCaPer_ = pmgEO_->stShiai_.GetNCaPer();//敵ナイスキャッチ確率

    st_.pstMyCh_->Live_f = TRUE;
    st_.pstMyCh_->MHP_f = TRUE;
    st_.pstMyCh_->Catch_c = NGNUM;
    st_.pstMyCh_->ShTg_c = NGNUM;
    st_.pstMyCh_->MukiAgl12 = NGNUM;
    st_.pstMyCh_->Biorhythm_c = 0;// pmgGO_->pmgCommon_->GetRand(0xFFFF);//適当なスタート
    st_.pstMyCh_->Zahyou.Pri = ((st_.mysideNo_ * DBMEMBER_ALL) + st_.posNo_);// *Graphics::PRVLAYRE;



    const s32 ENABLE = 1;
    const s32 GOUSEI = 2;

    //EtcSSに、ここでプレビューフラグが立っていたらシュートをそれに変える
    if (pmgEO_->stShiai_.IsPrev())
    {
      SetShotPrev();
    }//左サイド＆携帯端末＆サバ使用
    else if (((st_.mysideNo_ == SIDE0) || pmgEO_->stShiai_.IsVSHum() || pmgEO_->stShiai_.IsTour())
      && (mid::midIsTBL() && (pmgEO_->stSeq_.IsNoSv() == FALSE)))
    {
      SetShotShiai();
    }
    else
    {
      SetShotTest();
    }

    ZeroXYZ();//とまる

    SetMtypeReset(dbmtSt);
  }

  //復活
  void TChCommon::RespawnInit()
  {

    //復活サイド
    if (st_.mysideNo_ == SIDE0)
    {
      //ＩＤはpstMyCh_が持つ(MyStLvなどで使うので最初に入れる)
      st_.pstMyCh_->id_char = pmgEO_->mgCoM_.GetCharID(st_.mid_team_, st_.posNo_);

      s32 tmrarity = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(st_.mid_team_, mdm_team_rarity);
      s32 revlevel = pmgEO_->mgDt_.dtChar_.GetLv(clv_Rev, st_.pstMyCh_->id_char);

      st_.pstMyCh_->level = (tmrarity + revlevel);
      st_.pstMyCh_->HP = pmgEO_->mgDt_.dtSet_.GetDtDamage(setHP);
      //st_.pstMyCh_->comCaPer_ = pmgEO_->mgStSn_.EtcSS.caper_;//敵キャッチ確率
      //st_.pstMyCh_->comNCaPer_ = pmgEO_->mgStSn_.EtcSS.ncaper_;//敵ナイスキャッチ確率

	  //---------------------------------------------------------------------------------------------------------
	  // 元々マスターに直接書かれていたパラメータをレベルに応じて計算して出すように修正
	  s64 memid = st_.pstMyCh_->id_char;
	  s64 id_kantoku = st_.id_kantoku_;
	  s64 id_team = st_.id_team_;
	  s64 mid_team = st_.mid_team_;
	  s32 posno = st_.posNo_;

	  // チームレベル(限界突破値)を取得
	  s64 tmLv = 1;
	  s64 Lv = 1;


	  // 通信対戦時
	  if (pmgEO_->stShiai_.IsVSHum())
	  {
		  s32 myside = mid::midGetMyIdx();

		  // 自分
		  if (myside == st_.mysideNo_)
		  {
			  tmLv = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);
			  Lv = pmgEO_->mgCoU_.GetMyTmCharaLv(mid_team, posno);
		  }
		  // 相手
		  else
		  {
			  // 相手の監督データから取得
			  tmLv = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);
			  Lv = pmgEO_->mgCoU_.GetTmCharaLvByKantokuID(id_kantoku, mid_team, posno);
		  }
	  }
	  // 通信対戦以外
	  else
	  {
		  // 自分
		  if (SIDE0 == st_.mysideNo_)
		  {
			  tmLv = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);
			  Lv = pmgEO_->mgCoU_.GetMyTmCharaLv(mid_team, posno);
		  }
		  // 勝ち抜きイベント時の相手チーム
		  else if (pmgEO_->stShiai_.IsTour() && SIDE1 == st_.mysideNo_)
		  {
			  tmLv = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);
			  Lv = pmgEO_->mgCoU_.GetTmCharaLvByKantokuID(id_kantoku, mid_team, posno);
		  }
		  // COM
		  else
		  {
			  /*
			  //敵番号
			  s32 enemyNo = pmgEO_->mgCoU_.mdMap_.GetPKDt(stTSL_.id_map_, mdmap_enemyNo);
			  enmdm_Maptip enemyTeam = mdm_maptip_id_m_teamA;
			  switch (stTSL_.enemyno_)
			  {
			  case 0: enemyTeam = mdm_maptip_id_m_teamA; break;
			  case 1: enemyTeam = mdm_maptip_id_m_teamB; break;
			  case 2:enemyTeam = mdm_maptip_id_m_teamC; break;
			  }

			  //敵ＭＩＤ
			  stTSL_.mid_team_en_ = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), enemyTeam);
			  */

			  // COMのデータを取得
			  // いずれは↑のmdMapやmdm_Maptipなどを使ってcsvに入っているレベルを反映するようにする
			  tmLv = 1;
			  Lv = 1;
		  }
	  }


	  // 実際に計算する
	  for (s32 i = clv_Pow; i <= clv_Tech; i++)
	  {
		  switch (i)
		  {
		  case clv_Pow: st_.pstMyCh_->stPow = pmgEO_->CalcSt(mid_team, id_team, memid, clv_Pow, Lv, tmLv); break;//ぼーるぱわー	
		  case clv_Guts:st_.pstMyCh_->stGuts = pmgEO_->CalcSt(mid_team, id_team, memid, clv_Guts, Lv, tmLv); break;//きあい
		  case clv_Kire:st_.pstMyCh_->stKire = pmgEO_->CalcSt(mid_team, id_team, memid, clv_Kire, Lv, tmLv); break;//きれ
		  case clv_Tech:st_.pstMyCh_->stTech = pmgEO_->CalcSt(mid_team, id_team, memid, clv_Tech, Lv, tmLv); break;//てく
		  }
	  }
	  //---------------------------------------------------------------------------------------------------------

      st_.pstMyCh_->Live_f = TRUE;
      st_.pstMyCh_->MHP_f = TRUE;
      st_.pstMyCh_->Catch_c = NGNUM;
      st_.pstMyCh_->ShTg_c = NGNUM;
      st_.pstMyCh_->MukiAgl12 = NGNUM;
      st_.pstMyCh_->Biorhythm_c = 0;// pmgGO_->pmgCommon_->GetRand(0xFFFF);//適当なスタート
      st_.pstMyCh_->Zahyou.Pri = ((st_.mysideNo_ * DBMEMBER_ALL) + st_.posNo_);// *Graphics::PRVLAYRE;

      //st_.pstMyCh_->HP = pmgEO_->mgDt_.dtSet_.GetDtDamage(setHP);
      //st_.pstMyCh_->Live_f = TRUE;
      //st_.pstMyCh_->MHP_f = TRUE;
      //st_.pstMyCh_->ANGEL_f = FALSE;
      //st_.pstMyCh_->Catch_c = NGNUM;
      //st_.pstMyCh_->ShTg_c = NGNUM;
      //st_.pstMyCh_->MukiAgl12 = NGNUM;

      st_.pstMyCh_->Zahyou.X = 0;

      if (pmgEO_->stSeq_.IsNoSv() == FALSE) SetShotShiai();
    }

    ZeroXYZ();//とまる
    st_.pstMyCh_->Motion.SubMFlags(dbmfRES);//結果モーションは変わらないのでフラグをたたむ
    SetMtypeReset(dbmtSt);

    st_.pstMyCh_->Zahyou.Muki = (st_.mysideNo_ == SIDE0)
      ? mR
      : mL;


    ////復活位置
    //s32 resposX = (st_.mysideNo_ == SIDE0)
    //  ? 0
    //  : DBCRT_W;

    //s32 ITPX[] = { DBCRT_CL, resposX, resposX };
    //s32 ITPZ[] = { 0, 0, DBCRT_BL };

    //f32 hp[] =
    //{
    //  lib_num::Hypot((pmgSG_->stBa_.Zahyou.X - ITPX[0]), (pmgSG_->stBa_.Zahyou.Z - ITPZ[0])),
    //  lib_num::Hypot((pmgSG_->stBa_.Zahyou.X - ITPX[1]), (pmgSG_->stBa_.Zahyou.Z - ITPZ[1])),
    //  lib_num::Hypot((pmgSG_->stBa_.Zahyou.X - ITPX[2]), (pmgSG_->stBa_.Zahyou.Z - ITPZ[2]))
    //};
    //const s32 HPNUM = 3;

    //s32 resPosNo = 0;

    //for (s32 i = 0; i < (HPNUM - 1); ++i)
    //{
    //  if (hp[resPosNo] < hp[i + 1])
    //  {
    //    resPosNo = i + 1;
    //  }
    //}

    //st_.pstMyCh_->Zahyou.X = 0; ITPX[resPosNo];
    //st_.pstMyCh_->Zahyou.Z = ITPZ[resPosNo];
  }

  //逆向き
  enMukiType TChCommon::RevMuki(enMukiType tMuki)
  {
    if (tMuki == mL)
    {
      return mR;
    }
    else
    {
      return mL;
    }
  }
  //効果音
  void TChCommon::SESet(enSEID SENo)
  {
    SESetInt((s32)SENo);
  }
  //効果音
  void TChCommon::SESetInt(s32 SENo)
  {
    if (pmgSG_->stRf_.NoSE_f == FALSE)
    {
      //if (SENo != 13)
      pmgEO_->mgSound_.PlaySE(SENo);//, FALSE);
    }
  }
  //効果音
  void TChCommon::CatchSE()
  {
    SESetInt(seCatchLv0 + ((MyStLv(dbst_Tech) / 3) % 7));
  }

  //モーション切り替え
  void TChCommon::SetMtype(enDBMotionType tMtype)
  {
    //状態が変わらない場合も出る
    if (tMtype == st_.pstMyCh_->Motion.Mtype)
    {
      return;
    }

    GenSetMtype(tMtype);
  }
  void TChCommon::SetMtypeReset(enDBMotionType tMtype)
  {
    GenSetMtype(tMtype);
  }

  //フレーム情報ＳＥ
  void TChCommon::SettingSE()
  {
    const s32 DEFANISE = 1;
    const s32 NOANISE = 0;
    s32 anise = NowKoma(bmc_se);

    switch (anise)
    {
    case NOANISE:
    case DEFANISE:
      break;
    default:
      SESetInt(anise);
      break;
    }
  }

  //フレーム情報のセット
  void TChCommon::SetFrameData(BOOL InLoop_f)
  {
    //ループ始点の場合
    if (InLoop_f == FALSE)
    {
      switch (NowKoma(bmc_loopSt))
      {
      case (s32)bmlp_St:
        st_.pstMyCh_->Anime.Loop_c = NowKoma(bmc_loopNum);
        st_.pstMyCh_->Anime.LoopStNo = st_.pstMyCh_->Anime.FrameNo;
        break;

      case (s32)bmlp_StEd:
        st_.pstMyCh_->Anime.Loop_c = NowKoma(bmc_loopNum);
        break;
      }
      SettingSE();
    }
    else
    {
      if (NowKomaBOOL(bmc_seloop_f))
      {
        SettingSE();
      }
    }

    //初期化
    st_.pstMyCh_->Anime.Ani_c = 0;
    st_.pstMyCh_->Anime.ActP_f = NowKomaBOOL(bmc_ActP_f);


    //アクションポイント
    if ((st_.pstMyCh_->Anime.Ani_c == 0) && (st_.pstMyCh_->Anime.ActP_f))
    {
      switch (st_.pstMyCh_->Motion.Mtype)
      {
        //ダッシュ
      case dbmtDs:
        ++st_.pstMyCh_->Step_c;

        if (st_.pstMyCh_->Step_c % 2 == 1)
        {
          SESet(seDash);
        }
        break;
        //シュート
      case dbmtSh:
        SetShoot(FALSE);
        break;
        //ジャンプシュート
      case dbmtJSh:
        SetShoot(TRUE);
        break;
        //パス
      case dbmtPa:
        SetPass();
        break;
        //ジャンプパス
      case dbmtJPa:
        SetPass();
        break;

        //キャッチ
      case dbmtCa:
      case dbmtJCa:
        st_.pstMyCh_->Catch_c = 0;
        break;
        //オーバーライン
      case dbmtOvL:
        if (IsBall())//攻撃時間オーバーとの重なり防止
        {
          //審判の元へ
          if (st_.mysideNo_ == 0)
          {
            st_.pmgRf_->SetMotion(dbrfOvLine, mR);
          }
          else
          {
            st_.pmgRf_->SetMotion(dbrfOvLine, mL);
          }
          //タイマーリセット
          st_.pmgBa_->baCommon_.ResetTimer(NGNUM, FALSE);
        }
        break;
      default:
        break;
      }
    }
  }


  //動き停止
  void TChCommon::ZeroXYZ()
  {
    st_.pstMyCh_->Zahyou.dX = 0;
    st_.pstMyCh_->Zahyou.dY = 0;
    st_.pstMyCh_->Zahyou.dZ = 0;
    st_.pstMyCh_->Zahyou.Fric_c = 0;
  }

  void TChCommon::AddRec(enRecord dt)
  {
    ++st_.pmgMyTm_->st_.pstMyTm_->Rec.dt[dt];
  }
  void TChCommon::AddRec(enRecord dt, s32 addnum)
  {
    st_.pmgMyTm_->st_.pstMyTm_->Rec.dt[dt] += addnum;
  }
  void TChCommon::MaxRec(enRecord dt, s32 num)
  {
    if (num > st_.pmgMyTm_->st_.pstMyTm_->Rec.dt[dt])
    {
      st_.pmgMyTm_->st_.pstMyTm_->Rec.dt[dt] = num;
    }
  }

  //ターゲッティング用向き
  void TChCommon::SetMukiAgl(BOOL fL, BOOL fR, BOOL fU, BOOL fD)
  {
    switch (st_.posNo_)
    {
      //奥外野
    case (s32)dbpoO2:
      if (fL)
      {
        st_.pstMyCh_->MukiAgl12 = 5;//5678
      }
      else if (fR)
      {
        st_.pstMyCh_->MukiAgl12 = 3;//3456
      }
      break;
      //手前外野
    case (s32)dbpoO3:
      if (fL)
      {
        st_.pstMyCh_->MukiAgl12 = 9;//9 10 11 0
      }
      else if (fR)
      {
        st_.pstMyCh_->MukiAgl12 = 11;//11 0 1 2
      }
      break;
      //横外野
    case (s32)dbpoO4:
      if (st_.mysideNo_ == 0)
      {
        if (fU)
        {
          st_.pstMyCh_->MukiAgl12 = 9;//9 10 11 0
        }
        else if (fD)
        {
          st_.pstMyCh_->MukiAgl12 = 5;//5 6 7 8
        }
        else
        {
          st_.pstMyCh_->MukiAgl12 = 7;//7 8 9 10
        }
      }
      else
      {
        if (fU)
        {
          st_.pstMyCh_->MukiAgl12 = 11;//11 0 1 2
        }
        else if (fD)
        {
          st_.pstMyCh_->MukiAgl12 = 3;//3 4 5 6
        }
        else
        {
          st_.pstMyCh_->MukiAgl12 = 1;//1 2 3 4
        }
      }
      break;
      //内野
    default:
      if (fR)
      {
        if (fU)
        {
          st_.pstMyCh_->MukiAgl12 = 11;//11 0 1 2
        }
        else if (fD)
        {
          st_.pstMyCh_->MukiAgl12 = 3;//3 4 5 6
        }
        else
        {
          st_.pstMyCh_->MukiAgl12 = 1;//1 2 3 4
        }
      }
      else if (fL)
      {
        if (fU)
        {
          st_.pstMyCh_->MukiAgl12 = 9;//9 10 11 0
        }
        else if (fD)
        {
          st_.pstMyCh_->MukiAgl12 = 5;//5 6 7 8
        }
        else
        {
          st_.pstMyCh_->MukiAgl12 = 7;//7 8 9 10
        }
      }
      //if (fL) { st_.pstMyCh_->MukiAgl = 6; }
      //else if (fR) { st_.pstMyCh_->MukiAgl = 2; }

      //if (fU)
      //{
      //  switch (st_.pstMyCh_->MukiAgl)
      //  {
      //    case 2: st_.pstMyCh_->MukiAgl = 1; break;
      //    case 6: st_.pstMyCh_->MukiAgl = 7; break;
      //    default: st_.pstMyCh_->MukiAgl = 0; break;
      //  }
      //}
      //else if (fD)
      //{
      //  switch (st_.pstMyCh_->MukiAgl)
      //  {
      //    case 2: st_.pstMyCh_->MukiAgl = 3; break;
      //    case 6: st_.pstMyCh_->MukiAgl = 5; break;
      //    default: st_.pstMyCh_->MukiAgl = 4; break;
      //  }
      //}
      break;
    }
  }

  //AUTO向き変え初期化
  void TChCommon::AutoMukiInit()
  {
    //左右
    switch (st_.pstMyCh_->Zahyou.Muki)
    {
    case mL:
      st_.pstMyCh_->Auto.AMuki = maL;
      break;
    case mR:
      st_.pstMyCh_->Auto.AMuki = maR;
      break;
    }
    //前奥
    switch (st_.pstMyCh_->Zahyou.MukiZ)
    {
    case mzF:
      st_.pstMyCh_->Auto.AMukiZ = mzaF;
      break;
    case mzN:
      st_.pstMyCh_->Auto.AMukiZ = mzaN;
      break;
    case mzB:
      st_.pstMyCh_->Auto.AMukiZ = mzaB;
      break;
    }
  }


  enMukiTypeA TChCommon::GetMoveMuki(BOOL Input_f)
  {
    if (IsSelfCtrl() == FALSE)
    {
      return st_.pstMyCh_->Auto.AMuki;
    }
    else if (Input_f || (st_.pstMyCh_->Auto.AutoType == dbatFree))
    {
      if (MyPad()->IsWalk2(dxL))//歩き入力
      {
        return maL;
      }
      else if (MyPad()->IsWalk2(dxR))//歩き入力
      {
        return maR;
      }
      else
      {
        return maN;
      }
    }
    else
    {
      return st_.pstMyCh_->Auto.AMuki;
    }
  }

  enMukiZTypeA TChCommon::GetMoveMukiZ(BOOL Input_f)
  {
    if (IsSelfCtrl() == FALSE)
    {
      return st_.pstMyCh_->Auto.AMukiZ;
    }
    else if (Input_f || (st_.pstMyCh_->Auto.AutoType == dbatFree))
    {
      if (MyPad()->IsWalk2(dxU))//歩き入力
      {
        return mzaB;
      }
      else if (MyPad()->IsWalk2(dxD))//歩き入力
      {
        return mzaF;
      }
      else
      {
        return mzaN;
      }
    }
    else
    {
      return st_.pstMyCh_->Auto.AMukiZ;
    }
  }

  enMukiZTypeA TChCommon::GetMoveMukiZMukiLock(BOOL Input_f)
  {
    if (IsSelfCtrl() == FALSE)
    {
      return st_.pstMyCh_->Auto.AMukiZ;
    }
    else if (Input_f || (st_.pstMyCh_->Auto.AutoType == dbatFree))
    {
      if (MyPad()->IsWalk2(dxD))
      {
        return mzaB;
      }
      else if (MyPad()->IsWalk2(dxU))
      {
        return mzaF;
      }
      else
      {
        return mzaN;
      }
    }
    else
    {
      return st_.pstMyCh_->Auto.AMukiZ;
    }
  }

  //向き変え
  BOOL TChCommon::MukiSet(BOOL PosMuki_f, BOOL PosMuki2_f, BOOL input_f)
  {
    st_.pstMyCh_->MukiAgl12 = NGNUM;

    enMukiType lastMuki = st_.pstMyCh_->Zahyou.Muki;
    enMukiZType lastMukiZ = st_.pstMyCh_->Zahyou.MukiZ;
    enMukiTypeA tMuki;
    enMukiZTypeA tMukiZ;


    //タゲ方向
    if (IsSelfCtrl())
    {
      SetMukiAgl(
        MyPad()->IsWalk2(dxL),//歩き入力
        MyPad()->IsWalk2(dxR),
        MyPad()->IsWalk2(dxU),
        MyPad()->IsWalk2(dxD));
    }
    else
    {
      SetMukiAgl(
        (st_.pstMyCh_->Auto.AMuki == maL),
        (st_.pstMyCh_->Auto.AMuki == maR),
        (st_.pstMyCh_->Auto.AMukiZ == mzaB),
        (st_.pstMyCh_->Auto.AMukiZ == mzaF));
    }

    tMuki = GetMoveMuki(input_f);
    tMukiZ = GetMoveMukiZ(input_f);


    if (PosMuki_f)
    {
      switch (st_.posNo_)
      {
      case (s32)dbpoO2:
        if (PosMuki2_f)
        {
          tMukiZ = mzaF;
        }
        else if (tMukiZ == mzaB)
        {
          tMukiZ = mzaN;
        }
        break;
      case (s32)dbpoO3:
        if (PosMuki2_f)
        {
          tMukiZ = mzaB;
        }
        else if (tMukiZ == mzaF)
        {
          tMukiZ = mzaN;
        }
        break;
      case (s32)dbpoO4:

        tMuki = (st_.mysideNo_ == 0)
          ? maL
          : maR;
        break;
      }
    }


    if (tMuki == maL)
    {
      st_.pstMyCh_->Zahyou.Muki = mL;
      st_.pstMyCh_->Zahyou.MukiZ = mzN;
    }
    else if (tMuki == maR)
    {
      st_.pstMyCh_->Zahyou.Muki = mR;
      st_.pstMyCh_->Zahyou.MukiZ = mzN;
    }

    if (tMukiZ == mzaB)
    {
      st_.pstMyCh_->Zahyou.MukiZ = mzB;
    }
    else if (tMukiZ == mzaF)
    {
      st_.pstMyCh_->Zahyou.MukiZ = mzF;
    }

    //再度タゲ方向
    if (st_.pstMyCh_->MukiAgl12 == NGNUM)
    {
      SetMukiAgl(
        (st_.pstMyCh_->Zahyou.Muki == mL),
        (st_.pstMyCh_->Zahyou.Muki == mR),
        (st_.pstMyCh_->Zahyou.MukiZ == mzB),
        (st_.pstMyCh_->Zahyou.MukiZ == mzF));
    }

    //向き変えあり
    return ((lastMuki != st_.pstMyCh_->Zahyou.Muki) || (lastMukiZ != st_.pstMyCh_->Zahyou.MukiZ));
  }

  //向き変え(強制的にAUTOMUKIに向かせる)
  BOOL TChCommon::MukiSetAuto()
  {
    enMukiType lastMuki = st_.pstMyCh_->Zahyou.Muki;
    enMukiZType lastMukiZ = st_.pstMyCh_->Zahyou.MukiZ;

    //左右
    switch (st_.pstMyCh_->Auto.AMuki)
    {
    case maL:
      st_.pstMyCh_->Zahyou.Muki = mL;
      st_.pstMyCh_->Zahyou.MukiZ = mzN;
      break;
    case maR:
      st_.pstMyCh_->Zahyou.Muki = mR;
      st_.pstMyCh_->Zahyou.MukiZ = mzN;
      break;
    }
    //前奥
    switch (st_.pstMyCh_->Auto.AMukiZ)
    {
    case mzaF:
      st_.pstMyCh_->Zahyou.MukiZ = mzF;
      break;
    case mzaB:
      st_.pstMyCh_->Zahyou.MukiZ = mzB;
      break;
    }

    //向き変えありかどうか

    if ((lastMuki != st_.pstMyCh_->Zahyou.Muki) || (lastMukiZ != st_.pstMyCh_->Zahyou.MukiZ))
    {
      //タゲも変える
      SetMukiAgl(
        (st_.pstMyCh_->Zahyou.Muki == mL),
        (st_.pstMyCh_->Zahyou.Muki == mR),
        (st_.pstMyCh_->Zahyou.MukiZ == mzB),
        (st_.pstMyCh_->Zahyou.MukiZ == mzF));
      return TRUE;
    }
    else
    {
      return FALSE;
    }

  }

  //ダッシュ向き変え(強制的にAUTOMUKIに向かせる)
  BOOL TChCommon::MukiSetDash(enMukiType dsMuki)
  {
    st_.pstMyCh_->Zahyou.DsMuki = dsMuki;

    enMukiType lastMuki = st_.pstMyCh_->Zahyou.Muki;
    enMukiZType lastMukiZ = st_.pstMyCh_->Zahyou.MukiZ;

    //        st_.pstMyCh_->Zahyou.Muki = dsMuki;//ダッシュ方向を向くとは限らない//オートバックダッシュ
    st_.pstMyCh_->Zahyou.MukiZ = mzN;

    //敵ボールである
    BOOL lookball_f = (pmgSG_->stBa_.HoldTNo != st_.mysideNo_);

    //ダッシュ開始時、ボールの方向を向きながらダッシュ
    switch (st_.pstMyCh_->NextAuto.AutoType)
    {
    case dbatReturn://戻ってくる
    case dbatKgReturn:
    case dbatPassCvr:
    case dbatShCv://シュートカバー
    case dbatDfPos://ポジショニング
    case dbatPassCach://パスキャッチモーション
    case dbatPassJp://ジャンプ//ジャンプ避け
    case dbatDgJp:
    case dbatDgSw://移動避け
    case dbatCPUPaCut://COMパスカット
    case dbatCPUHelp://COMヘルプ
      lookball_f = TRUE;

      break;
    }

    if (lookball_f)
    {
      AutoMukiInit();
      SetBallMukiX();
      st_.pstMyCh_->Auto = st_.pstMyCh_->NextAuto;
      MukiSetAuto();
    }
    else//それ以外ではダッシュ方向を向く
    {
      if (st_.pstMyCh_->NextAuto.AutoType != dbatFree)//自分操作の時は別にいいか
      {
        st_.pstMyCh_->Zahyou.Muki = dsMuki;
      }
    }

    //向き変えあり
    if ((lastMuki != st_.pstMyCh_->Zahyou.Muki)
      || (lastMukiZ != st_.pstMyCh_->Zahyou.MukiZ))
    {
      //タゲも変える
      SetMukiAgl((st_.pstMyCh_->Zahyou.Muki == mL),
        (st_.pstMyCh_->Zahyou.Muki == mR),
        (st_.pstMyCh_->Zahyou.MukiZ == mzB),
        (st_.pstMyCh_->Zahyou.MukiZ == mzF));

      return TRUE;
    }
    else
    {
      return FALSE;
    }


  }

  s32 TChCommon::GetWkX()
  {
    return RankSpeed(rkWkX);
  }
  s32 TChCommon::GetWkZ()
  {
    return lib_num::Percent(RankSpeed(rkWkX), pmgEO_->mgDt_.dtSet_.GetDtCourt(setWkZPer));
  }
  s32 TChCommon::GetNWkX()
  {
    return lib_num::Percent(RankSpeed(rkWkX), pmgEO_->mgDt_.dtSet_.GetDtCourt(setNWkXPer));
  }
  s32 TChCommon::GetNWkZ()
  {
    return lib_num::Percent(RankSpeed(rkWkX), pmgEO_->mgDt_.dtSet_.GetDtCourt(setNWkZPer));
  }
  s32 TChCommon::GetWkJpX()
  {
    return RankSpeed(rkWkJPX);
  }
  s32 TChCommon::GetWkJpZ()
  {
    return lib_num::Percent(RankSpeed(rkWkJPX), pmgEO_->mgDt_.dtSet_.GetDtCourt(setWkZPer));
  }
  s32 TChCommon::GetNWkJpX()
  {
    return lib_num::Percent(RankSpeed(rkWkJPX), pmgEO_->mgDt_.dtSet_.GetDtCourt(setNWkXPer));

  }
  s32 TChCommon::GetNWkJpZ()
  {
    return lib_num::Percent(RankSpeed(rkWkJPX), pmgEO_->mgDt_.dtSet_.GetDtCourt(setNWkZPer));
  }

  //スリップ量セット＆状態をスリップに
  void TChCommon::SetSlip()
  {
    SetMtypeReset(dbmtSl);
  }

  //ジャンプ
  void TChCommon::JumpSet(BOOL mfCheck_f, BOOL canselDs_f, BOOL vjp_f)
  {
    if (((mfCheck_f == FALSE) || (st_.pstMyCh_->Motion.IsMFlags(dbmfJpOK)))
      && ((st_.pstMyCh_->Motion.IsMFlags(dbmfPHit) == FALSE)))
    {
      if (canselDs_f || (IsInfield() == FALSE))//外野はダッシュフラグ消す
      {
        st_.pstMyCh_->Motion.SubMFlags(dbmfDs);
      }

      BOOL ljump_f = st_.pstMyCh_->DsAccIOS_f;

      SetMtype(dbmtJCr);

      st_.pstMyCh_->VJump_f = vjp_f;//垂直ジャンプ

      //ロングジャンプ
      st_.pstMyCh_->LJump_f = st_.pstMyCh_->DsAccIOS_f;

      ZeroXYZ();//ドッジのジャンプアクションはアクションポイントから初速
    }
  }

  //キャンセルジャンプ
  BOOL TChCommon::CanselJump(BOOL canselDs_f)
  {
    //判定出るまでもしくは6f以内はジャンプキャンセル可能
    //ココでも可能にして大丈夫か
    if ((st_.pstMyCh_->Motion.IsMFlags(dbmfAr) == FALSE)
      && (st_.pstMyCh_->Motion.M_c < pmgEO_->mgDt_.dtSet_.GetDtJump(setJumpCanselTime)
      && IsSelfCtrl()))
    {
      if (MyPad()->IsJump())//ジャンプ入力
      {
        //キャンセルが掛かる前のモーションの向きに戻す
        st_.pstMyCh_->Zahyou.Muki = st_.pstMyCh_->LastMuki;
        st_.pstMyCh_->Zahyou.MukiZ = st_.pstMyCh_->LastMukiZ;
        JumpSet(FALSE, canselDs_f, FALSE);//ジャンプ
        return TRUE;
      }
      else
      {
        return FALSE;
      }
    }
    else
    {
      return FALSE;
    }
  }

  //ボールとの距離
  void TChCommon::GetBallDist()
  {
    //操作可能キャラのみ
    if (IsCtrlOK())
    {
      s32 dx = (st_.pstMyCh_->Zahyou.X - pmgSG_->stBa_.Zahyou.X);
      s32 dz = (st_.pstMyCh_->Zahyou.Z - pmgSG_->stBa_.Zahyou.Z);

      st_.pstMyCh_->BallDist = (s32)lib_num::Hypot(dx, dz);

      //パスとバウンドの時
      if ((pmgSG_->stBa_.Motion == bmBound)
        || (pmgSG_->stBa_.Motion == bmPass))
      {
        s32 ldx = (st_.pstMyCh_->Zahyou.X - pmgSG_->stBa_.LandX);
        s32 ldz = (st_.pstMyCh_->Zahyou.Z - pmgSG_->stBa_.LandZ);
        st_.pstMyCh_->BallLandDist = (s32)lib_num::Hypot(ldx, ldz);

        //LineDistの方も
        if (pmgSG_->stBa_.LandLine_f)
        {
          s32 lldx = (st_.pstMyCh_->Zahyou.X - pmgSG_->stBa_.LandXLine);
          s32 lldz = (st_.pstMyCh_->Zahyou.Z - pmgSG_->stBa_.LandZLine);
          st_.pstMyCh_->BallLandLineDist = (s32)lib_num::Hypot(lldx, lldz);
        }
        else
        {
          st_.pstMyCh_->BallLandLineDist = st_.pstMyCh_->BallLandDist;
        }
      }
      else
      {
        st_.pstMyCh_->BallLandDist = st_.pstMyCh_->BallDist;
        st_.pstMyCh_->BallLandLineDist = st_.pstMyCh_->BallDist;
      }
    }
    else
    {
      st_.pstMyCh_->BallDist = NGNUM;
      st_.pstMyCh_->BallLandDist = NGNUM;
      st_.pstMyCh_->BallLandLineDist = NGNUM;
    }
  }

  //ボール持った処理
  void TChCommon::BallGet(BOOL NoSE_f, BOOL LookBall_f)//FALSE,FALSE
  {
    //チーム
    st_.pmgMyTm_->st_.pstMyTm_->getJpball_f = (pmgSG_->stBa_.JumpBall == jbJpball);
    st_.pmgMyTm_->st_.pstMyTm_->COMCall.CallingPa_f[st_.posNo_] = FALSE;//パス要求もクリア
    //ボール触った
    st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].BallTouched_f = TRUE;

    //自分
    st_.pstMyCh_->Kagami_c = 0;//固まるの回避
    st_.pstMyCh_->AirAct_f = FALSE; //ここでFALSEにしてokか
    st_.pstMyCh_->LastLRKey = maN;
    st_.pstMyCh_->LongKeep_c = 0;
    st_.pstMyCh_->COMCounter_f = FALSE;
    st_.pstMyCh_->COMTossPassGet_f = FALSE;

    const s32 MIRLIM = 10;

    //ミラーシュート受付時間
    st_.pstMyCh_->MirShotLim_c = MIRLIM;

    //ミラーパスシュート
    if ((MyPad() != NULL)
      && (pmgSG_->stBa_.Motion == bmPass)
      && (pmgSG_->stBa_.PichTNo == st_.mysideNo_))
    {

      st_.pstMyCh_->MirPass_c = (MyPad()->IsPass2())
        ? 1
        : 0;

      st_.pstMyCh_->MirShot_c = 1;
    }
    else
    {

      //キャッチボタン不要で取れるボール
      if ((pmgSG_->stBa_.Motion == bmReferee)
        || (pmgSG_->stBa_.Motion == bmFree)
        || (pmgSG_->stBa_.Motion == bmBound))
      {
        st_.pstMyCh_->MirShot_c = (st_.pstMyCh_->Fmbl_c == 0)//ファンブル直後はオートシュートしない
          ? 1
          : 0;
      }
      else
      {
        st_.pstMyCh_->MirShot_c = 0;
      }

      st_.pstMyCh_->MirPass_c = 0;

    }


    if (LookBall_f)
    {
      AutoMukiInit();
      if (pmgSG_->stBa_.Zahyou.dX > 0)
      {
        st_.pstMyCh_->Auto.AMuki = maL;
      }
      else if (pmgSG_->stBa_.Zahyou.dX < 0)
      {
        st_.pstMyCh_->Auto.AMuki = maR;
      }

      if (abs(pmgSG_->stBa_.Zahyou.dX) < abs(pmgSG_->stBa_.Zahyou.dZ))
      {
        if (pmgSG_->stBa_.Zahyou.dZ > 0)
        {
          st_.pstMyCh_->Auto.AMukiZ = mzaF;
        }
        else if (pmgSG_->stBa_.Zahyou.dZ < 0)
        {
          st_.pstMyCh_->Auto.AMukiZ = mzaB;
        }
      }
      else
      {
        st_.pstMyCh_->Auto.AMukiZ = mzaN;
      }
      //chCommon_.
      MukiSetAuto();
    }

    if (NoSE_f == FALSE)
    {
      //chCommon_.
      SESet(seTake);
    }

    //自分を操作キャラに
    //st_.pmgMyTm_->SetCtrl(st_.posNo_);
    st_.pmgMyTm_->SetCtrlBallGet(st_.posNo_);//強制


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
      if (tEnctrl != NGNUM)//ないとはおもうが
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
        pDs->SetReturnBallFlg(FALSE);// フラグを落とす
        pDs->SetAutoShootStep(kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_CHOOSE);
      }
    }
#endif	// #ifdef __K_DEBUG_SHIAI__
  }

  //新タゲ角度(12時計算)
  s32 TChCommon::GetTagAgl2(s32 tX, s32 tZ)
  {
    s32 res = NGNUM;
    s32 absX = abs(tX);
    s32 absZ = abs(tZ);
    BOOL minX_f = (tX < 0);
    BOOL minZ_f = (tZ < 0);

    const s32 ROOT3 = 173;
    const s32 ROOT1 = 100;
    const s32 AGL90 = 3;

    //X距離*
    if ((absX * ROOT3) < (absZ * ROOT1))
    {
      res = -1;
    }
    else if ((absZ * ROOT3) < (absX * ROOT1))
    {
      res = 0;
    }
    else
    {
      res = +1;
    }

    if (minZ_f ^ minX_f)//符号反転
    {
      res *= -1;
    }

    res += 1;

    if (minX_f && minZ_f)
    {
      res += (AGL90 * 2);
    }
    else if (minX_f)
    {
      res += (AGL90 * 3);
    }
    else if (minZ_f)
    {
      res += (AGL90 * 1);
    }

    return res;
  }

  //タゲセット
  s32 TChCommon::GetTag(BOOL NoRefMuki_f)
  {
    s32 sltgX[DBMEMBER_INF];
    s32 sltgZ[DBMEMBER_INF];
    s32 sltgY[DBMEMBER_INF];
    f32 sltgXZ[DBMEMBER_INF];
    BOOL sltg_f[DBMEMBER_INF];
    BOOL sltg_f2[DBMEMBER_INF];
    f32 sltgXZ2 = NGNUM;
    s32 sltgAg2[DBMEMBER_ALL];
    s32 res = NGNUM;

    //向きが決まってないとき
    if (st_.pstMyCh_->MukiAgl12 == NGNUM)
    {
      SetMukiAgl(
        (st_.pstMyCh_->Zahyou.Muki == mL),
        (st_.pstMyCh_->Zahyou.Muki == mR),
        (st_.pstMyCh_->Zahyou.MukiZ == mzB),
        (st_.pstMyCh_->Zahyou.MukiZ == mzF));
    }

    //敵内野全員との角度を取る
    for (s32 i = 0; i < DBMEMBER_INF; ++i)
    {

      if (st_.pmgEnTm_->st_.pmgMyCh_[i]->IsShTgOK() == FALSE)
      {
        sltg_f[i] = FALSE;
        sltg_f2[i] = FALSE;
        continue;
      }
      else
      {
        sltg_f[i] = TRUE;
        sltg_f2[i] = FALSE;
      }

      sltgX[i] = (st_.pmgEnTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.X - st_.pstMyCh_->Zahyou.X);
      sltgZ[i] = (st_.pmgEnTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z - st_.pstMyCh_->Zahyou.Z);
      sltgY[i] = (st_.pmgEnTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Y - st_.pstMyCh_->Zahyou.Y);

      sltgXZ[i] = lib_num::Hypot(sltgX[i], sltgZ[i]);//距離
      sltgAg2[i] = GetTagAgl2(sltgX[i], sltgZ[i]);//新12時法

      //向きを反映しない（強制的に内野タゲ）
      if (NoRefMuki_f == FALSE)
      {
        const s32 AGL12 = 12;

        //メインアングルチェック
        if ((sltgAg2[i] != ((st_.pstMyCh_->MukiAgl12 + 1) % AGL12))
          && (sltgAg2[i] != ((st_.pstMyCh_->MukiAgl12 + 2) % AGL12))
          && (sltgAg2[i] != ((st_.pstMyCh_->MukiAgl12 + 0) % AGL12))
          && (sltgAg2[i] != ((st_.pstMyCh_->MukiAgl12 + 3) % AGL12)))
        {
          sltg_f[i] = FALSE;
        }

      }
    }


    //距離による優先順位
    for (s32 i = 0; i < DBMEMBER_INF; ++i)
    {
      if (sltg_f[i] == FALSE) continue;

      const s32 NEARDIST = (9 * XYMAG);
      //近すぎ
      if (sltgXZ[i] < NEARDIST) continue;

      //Y離れすぎ
      if (abs(sltgXZ[i] * 3) < abs(sltgY[i] * 2)) continue;

      if ((sltgXZ2 == NGNUM) || (sltgXZ[i] < sltgXZ2))
      {
        res = i;
        sltgXZ2 = sltgXZ[i];
      }
    }


    //見つからなかった→Ｙ離れすぎをカットして再検索
    if (res == NGNUM)
    {
      //距離による優先順位
      for (s32 i = 0; i < DBMEMBER_INF; ++i)
      {
        if (sltg_f[i] == FALSE) continue;

        const s32 NEARDIST = (9 * XYMAG);
        //近すぎ
        if (sltgXZ[i] < NEARDIST) continue;

        //Y離れすぎ
        //if (abs(sltgXZ[i] * 3) < abs(sltgY[i] * 2)) continue;

        if ((sltgXZ2 == NGNUM) || (sltgXZ[i] < sltgXZ2))
        {
          res = i;
          sltgXZ2 = sltgXZ[i];
        }
      }

    }


    return res;
  }

  //変化球狙いタゲ
  s32 TChCommon::GetCrvTag()
  {

    s32 res = NGNUM;
    s32 tgst = NGNUM;

    for (s32 i = 0; i < DBMEMBER_INF; ++i)//上から検索かけていく
    {
      //ねらえない
      if (st_.pmgEnTm_->st_.pmgMyCh_[i]->IsShTgOK() == FALSE) continue;

      //近接パス狙い
      s32 nptg = st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaNrPaTag];

      switch (nptg)
      {
      case 0://上位オーダーに近い人
        if (res == NGNUM)
        {
          res = i;
        }
        break;
      case 1://攻撃力高いに近い人
      {
        s32 pwfc = (st_.pmgEnTm_->st_.pmgMyCh_[i]->MyStLv(dbst_Pow)
          + st_.pmgEnTm_->st_.pmgMyCh_[i]->MyStLv(dbst_Guts));
        //パワー比較
        if ((tgst == NGNUM) || (tgst < pwfc))
        {
          res = i;
          tgst = pwfc;
        }
      }
      break;
      case 2://HPすくない人に近い人
      {
        s32 hp = st_.pmgEnTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->HP;
        //ＨＰ比較
        if ((tgst == NGNUM) || (hp < tgst))
        {
          res = i;
          tgst = hp;
        }
      }
      break;
      default://守備が下手に近い人
      {
        s32 def = (st_.pmgEnTm_->st_.pmgMyCh_[i]->MyStLv(dbst_Kire)
          + st_.pmgEnTm_->st_.pmgMyCh_[i]->MyStLv(dbst_Tech));
        //守備比較
        if ((tgst == NGNUM) || (def < tgst))
        {
          res = i;
          tgst = def;
        }
      }
      break;
      }
    }

    return res;

  }

  //角度に入っていない
  BOOL TChCommon::IsCheckNoAgl(s32 tgX, s32 tgZ)
  {
    BOOL res_f = FALSE;
    const s32 ZWIDE = 24 * XYMAG;

    switch (st_.pstMyCh_->Zahyou.MukiZ)
    {
    case mzF:
      res_f = (tgZ < st_.pstMyCh_->Zahyou.Z);
      break;
    case mzN:
      res_f = TRUE;//(abs(tgZ - st_.pstMyCh_->Zahyou.Z) > ZWIDE);
      break;
    case mzB:
      res_f = (tgZ > st_.pstMyCh_->Zahyou.Z);
      break;
    }

    if (res_f)
    {
      switch (st_.pstMyCh_->Zahyou.Muki)
      {
      case mL:
        res_f = (tgX < st_.pstMyCh_->Zahyou.X);
        break;
      case mR:
        res_f = (tgX > st_.pstMyCh_->Zahyou.X);
        break;
      }
    }


    return !(res_f);
  }
  //敵陣地に着地するか
  BOOL TChCommon::IsCheckLandEnPos(s32 tPNo)
  {
    s32 tLandX = (st_.mysideNo_ == 0)
      ? st_.pmgMyTm_->st_.pmgMyCh_[tPNo]->st_.pstMyCh_->LandX
      : DBCRT_CL - (st_.pmgMyTm_->st_.pmgMyCh_[tPNo]->st_.pstMyCh_->LandX - DBCRT_CL);

    return (st_.pmgMyTm_->st_.pmgMyCh_[tPNo]->IsInfield() && (tLandX > DBCRT_CLI));
  }

  //コートに入ってるかどうか
  BOOL TChCommon::IsInCourt()
  {
    return (
      (GetLeftCrtX() > GetSLX())
      && (GetLeftCrtX() < DBCRT_CLI)
      && (st_.pstMyCh_->Zahyou.Z > DBCRT_FL)
      && (st_.pstMyCh_->Zahyou.Z < DBCRT_BL));
  }

  //内野パスタゲセット★
  s32 TChCommon::GetNaiyaPassTag()
  {
    s32 sltgX[DBMEMBER_INF];
    s32 sltgZ[DBMEMBER_INF];
    f32 sltgXZ[DBMEMBER_INF];
    enNaiyaTag sltg_f[DBMEMBER_INF];
    s32 tgOrd[DBMEMBER_INF];

    BOOL AllNoTag_f = TRUE;//完全にタゲが居ない
    BOOL NoTag_f = TRUE;//向き方向にタゲが居ない

    s32 sortDt[DBMEMBER_INF];

    enMukiType paMuki = st_.pstMyCh_->Zahyou.Muki;//向き
    enMukiZType paMukiZ = st_.pstMyCh_->Zahyou.MukiZ;//奥行き向き

    BOOL CrsL_f = FALSE;
    BOOL CrsR_f = FALSE;
    BOOL CrsU_f = FALSE;
    BOOL CrsD_f = FALSE;

    if (IsSelfCtrl())
    {
      CrsL_f = MyPad()->IsPassCrs(dxL);//パス方向入力
      CrsR_f = MyPad()->IsPassCrs(dxR);
      CrsU_f = MyPad()->IsPassCrs(dxU);
      CrsD_f = MyPad()->IsPassCrs(dxD);

      //ここ再チェック
      if (mid::midIsTBL())
      {
        if (CrsL_f)
        {
          paMuki = mL;
          //Z方向を無視
          if ((CrsU_f || CrsD_f) == FALSE) paMukiZ = mzN;
        }
        else if (CrsR_f)
        {
          paMuki = mR;
          //Z方向を無視
          if ((CrsU_f || CrsD_f) == FALSE) paMukiZ = mzN;
        }

        if (CrsU_f)
        {
          paMukiZ = mzB;
          //X方向を無視
          if ((CrsL_f || CrsR_f) == FALSE) paMuki = mN;
        }
        else if (CrsD_f)
        {
          paMukiZ = mzF;
          //X方向を無視
          if ((CrsL_f || CrsR_f) == FALSE) paMuki = mN;
        }
      }
    }

    //内野方向を向いてる
    BOOL infMuki_f = (((st_.mysideNo_ == 0) && (paMuki == mL))
      || ((st_.mysideNo_ == 1) && (paMuki == mR)));


    BOOL infCrs_f = (((st_.mysideNo_ == 0) && CrsL_f)
      || ((st_.mysideNo_ == 1) && CrsR_f));

    BOOL enmCrs_f = (((st_.mysideNo_ == 0) && CrsR_f)
      || ((st_.mysideNo_ == 1) && CrsL_f));

    //十字入ってない
    BOOL neutral_f = !(CrsL_f || CrsR_f || CrsU_f || CrsD_f);

    //左コート時、内野内で一番右にいる
    BOOL topPos_f = TRUE;

    BOOL frontPos_f = TRUE;//一番手前にいる
    BOOL backPos_f = TRUE;//一番奥にいる


    //ダッシュマンへパス
    BOOL dashman_f = (st_.pmgMyTm_->st_.pstMyTm_->PosMove.DashmanNum > 0);


    s32 distO2 = abs(st_.pstMyCh_->Zahyou.Z - DBCRT_BL);
    s32 distO3 = abs(st_.pstMyCh_->Zahyou.Z - DBCRT_FL);
    BOOL nearO2_f = (distO2 < distO3);


    //優先順位初期化
    for (s32 i = 0; i < DBMEMBER_INF; ++i)
    {
      tgOrd[i] = NGNUM;
    }


    //内野全員との距離を取る
    for (s32 i = 0; i < DBMEMBER_INF; ++i)
    {
      if (i != st_.posNo_)
      {
        //X距離
        sltgX[i] = (st_.pmgMyTm_->st_.pmgMyCh_[i]->GetLeftCrtX() - GetLeftCrtX());//自分より右に居れば＋
        //Z距離
        sltgZ[i] = (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z - st_.pstMyCh_->Zahyou.Z);//自分より上にいれば＋
        //距離
        sltgXZ[i] = lib_num::Hypot(sltgX[i], sltgZ[i]);
      }
    }

    //パスが出せるダッシュマンがいるか
    if (dashman_f)//ダッシュマンへパス
    {
      //内野全員との角度を取る
      for (s32 i = 0; i < DBMEMBER_INF; ++i)
      {
        if (i == st_.posNo_)
        {
          sltg_f[i] = TGNG;
          continue;//自分
        }

        if (st_.pmgMyTm_->st_.pmgMyCh_[i]->IsDashman()
          )
        {
          //向き方向に居る
          sltg_f[i] = TGOK;
          NoTag_f = FALSE;//一人でも向き方向にタゲが見つかった

          //右にダッシュマンがいる
          if (st_.pmgMyTm_->st_.pmgMyCh_[i]->GetLeftCrtX() > GetLeftCrtX())
          {
            topPos_f = FALSE;
          }

          //奥にダッシュマンがいる
          if (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z > st_.pstMyCh_->Zahyou.Z)
          {
            backPos_f = FALSE;
          }

          //手前にダッシュマンがいる
          if (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z < st_.pstMyCh_->Zahyou.Z)
          {
            frontPos_f = FALSE;
          }
        }
        else
        {
          sltg_f[i] = TGNG;
        }

      }
      //パスが出せるダッシュマンがいない
      if (NoTag_f) dashman_f = FALSE;
    }


    //最終的にダッシュマンいない
    if (dashman_f == FALSE)
    {
      //内野全員との角度を取る
      for (s32 i = 0; i < DBMEMBER_INF; ++i)
      {

        if (IsNGPassTag(i))
        {
          sltg_f[i] = TGNG;
          continue;
        }
        else if (IsCheckNoAgl(st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.X, st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z))
        {
          //向きに居ない
          sltg_f[i] = TGNOAGL;
        }
        else
        {
          //向き方向に居る
          sltg_f[i] = TGOK;
          NoTag_f = FALSE;//一人でも向き方向にタゲが見つかった
        }

        AllNoTag_f = FALSE;//一応タゲ可能は人はいる


        //誰か右にいる
        if (st_.pmgMyTm_->st_.pmgMyCh_[i]->GetLeftCrtX() > GetLeftCrtX())
        {
          topPos_f = FALSE;
        }

        //奥にいる
        if (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z > st_.pstMyCh_->Zahyou.Z)
        {
          backPos_f = FALSE;
        }

        //手前にいる
        if (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z < st_.pstMyCh_->Zahyou.Z)
        {
          frontPos_f = FALSE;
        }
      }
    }


    //ダッシュマンいるとき
    if (dashman_f)
    {

      if (topPos_f && enmCrs_f)//先頭で右→が入ってるのときのみ
      {
        if (CrsU_f)
        {
          return (s32)dbpoO2;//右上
        }
        else if (CrsD_f)
        {
          return (s32)dbpoO3;//右下
        }
        else//右のみ
        {
          if (nearO2_f)
          {
            return (s32)dbpoO2;
          }
          else
          {
            return (s32)dbpoO3;
          }
        }
      }

      if (frontPos_f)//一番手前に居る
      {
        if (CrsD_f)
        {
          return (s32)dbpoO3;//右下
        }
      }

      if (backPos_f)//一番奥に居る
      {
        if (CrsU_f)
        {
          return (s32)dbpoO2;//右下
        }
      }

    }
    else if (((infMuki_f == FALSE) && topPos_f) || AllNoTag_f)//右向き時しかも先頭もしくは孤立(→外野パス)
    {
      if (st_.pstMyCh_->Motion.Mtype == dbmtDs)//ダッシュ中
      {
        if (IsSelfCtrl())
        {
          if (CrsU_f) return (s32)dbpoO2;//上
          if (CrsD_f) return (s32)dbpoO3;//下
          if (enmCrs_f) return (s32)dbpoO4;//右
          if (nearO2_f)
          {
            return (s32)dbpoO2;
          }
          else
          {
            return (s32)dbpoO3;
          }
        }
        else
        {
          switch (st_.pstMyCh_->Auto.AMukiZ)
          {
          case mzaB: return (s32)dbpoO2;
          case mzaF: return (s32)dbpoO3;
          }
        }
      }
      else
      {
        switch (paMukiZ)
        {
        case mzB: return (s32)dbpoO2;
        case mzF: return (s32)dbpoO3;
        default:
          if (enmCrs_f) return (s32)dbpoO4;//右
          if (nearO2_f)
          {
            return (s32)dbpoO2;
          }
          else
          {
            return (s32)dbpoO3;
          }
          break;
        }
      }
      //しかも先頭もしくは孤立は4番でFA
      return (s32)dbpoO4;
    }


    s32 f = 0;
    for (s32 i = 0; i < DBMEMBER_INF; ++i)
    {
      //向き方向に人なしのとき

      sortDt[i] = 0;//初期化

      if ((sltg_f[i] == TGOK)
        || (NoTag_f && (sltg_f[i] != TGNG)))
      {
        if (neutral_f)//ニュートラル
        {
          sortDt[i] = (s32)sltgXZ[i];//内野間は距離が近い人
        }
        else
        {

          //ダッシュマンが居るときは現在Ｚではなく、目標Ｚ
          s32 tgZ = (dashman_f)
            ? st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->DashmanTgZ
            : st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z;

          //上
          if (CrsU_f)
          {
            sortDt[i] = -tgZ;//Ｚのマイナス（上ほど優先）
          }
          else if (CrsD_f)//下
          {
            sortDt[i] = +tgZ;//Ｚ（下ほど優先）
          }

          //上下が入ってるとき用に合計値
          if (CrsL_f)//左
          {
            sortDt[i] += (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.X);//Ｘ（左ほど優先）
          }
          else if (CrsR_f)//右
          {
            sortDt[i] -= (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.X);//Ｘのマイナス（右ほど優先）
          }
        }
        tgOrd[f++] = i;
      }
    }

    //ソート
    for (s32 i = 0; i < (DBMEMBER_INF - 1); ++i)
    {
      for (s32 i2 = 0; i2 < (DBMEMBER_INF - 1); i2++)
      {
        if (i == i2) continue;//同じ

        if ((tgOrd[i] != NGNUM) && (tgOrd[i2] != NGNUM))
        {
          if (sortDt[tgOrd[i]] < sortDt[tgOrd[i2]])//小さい方優先
          {
            s32 tmp;
            tmp = tgOrd[i2];
            tgOrd[i2] = tgOrd[i];
            tgOrd[i] = tmp;
          }
        }
      }
    }

    //ソート１位
    s32 res = tgOrd[0];

    return res;
  }

  //外野間パスタゲセット★
  s32 TChCommon::GetGaiyaPassTag()
  {
    enMukiType NaiyaMuki = (st_.mysideNo_ == 0)
      ? mL
      : mR;

    enMukiTypeA NaiyaMukiA = (st_.mysideNo_ == 0)
      ? maL
      : maR;

    s32 ptg = NGNUM;//パスタゲ

    //ダッシュマンへパス
    BOOL dashman_f = (st_.pmgMyTm_->st_.pstMyTm_->PosMove.DashmanNum > 0);

    enMukiType paMuki = st_.pstMyCh_->Zahyou.Muki;//向き
    enMukiZType paMukiZ = st_.pstMyCh_->Zahyou.MukiZ;//奥行き向き
    enMukiTypeA paLastLRKey = st_.pstMyCh_->LastLRKey;

    BOOL CrsL_f = FALSE;
    BOOL CrsR_f = FALSE;
    BOOL CrsU_f = FALSE;
    BOOL CrsD_f = FALSE;

    if (IsSelfCtrl())
    {

      CrsL_f = MyPad()->IsPassCrs(dxL);//パス方向入力
      CrsR_f = MyPad()->IsPassCrs(dxR);
      CrsU_f = MyPad()->IsPassCrs(dxU);
      CrsD_f = MyPad()->IsPassCrs(dxD);

      //フリックによるパス方向
      if (mid::midIsTBL())
      {
        if (CrsL_f)
        {
          paMuki = mL;
          //Z方向を無視
          if ((CrsU_f || CrsD_f) == FALSE) paMukiZ = mzN;

          paLastLRKey = maL;
        }
        else if (CrsR_f)
        {
          paMuki = mR;
          //Z方向を無視
          if ((CrsU_f || CrsD_f) == FALSE) paMukiZ = mzN;

          paLastLRKey = maR;
        }

        if (CrsU_f)
        {
          paMukiZ = mzB;
          //X方向を無視
          if ((CrsL_f || CrsR_f) == FALSE) paMuki = mN;
        }
        else if (CrsD_f)
        {
          paMukiZ = mzF;
          //X方向を無視
          if ((CrsL_f || CrsR_f) == FALSE) paMuki = mN;
        }

      }
    }

    //内野向きが押されてる
    BOOL infCrs_f = (((st_.mysideNo_ == 0) && CrsL_f)
      || ((st_.mysideNo_ == 1) && CrsR_f));

    BOOL noLR_f = (CrsL_f == FALSE) && (CrsR_f == FALSE);

    //十字入ってない
    BOOL neutral_f = !(CrsL_f || CrsR_f || CrsU_f || CrsD_f);

    //とりあえずパスタゲを出す
    switch (st_.posNo_)
    {
    case (s32)dbpoO2:
      if (paLastLRKey == maN)
      {
        if (CrsD_f && noLR_f)//(st_.pstMyCh_->Zahyou.MukiZ == mzF)
        {
          ptg = (s32)dbpoO3;
        }
        else
        {
          ptg = (paMuki == NaiyaMuki)
            ? st_.pmgMyTm_->st_.pstMyTm_->PosMove.Postman
            : (s32)dbpoO4;
        }
      }
      else
      {
        ptg = (paLastLRKey == NaiyaMukiA)
          ? st_.pmgMyTm_->st_.pstMyTm_->PosMove.Postman
          : (s32)dbpoO4;
      }
      break;

    case (s32)dbpoO3:
      if (paLastLRKey == maN)
      {
        if (CrsU_f && noLR_f)//(st_.pstMyCh_->Zahyou.MukiZ == mzB)
        {
          ptg = (s32)dbpoO2;
        }
        else
        {
          ptg = (paMuki == NaiyaMuki)
            ? st_.pmgMyTm_->st_.pstMyTm_->PosMove.Postman
            : (s32)dbpoO4;
        }
      }
      else
      {
        ptg = (paLastLRKey == NaiyaMukiA)
          ? st_.pmgMyTm_->st_.pstMyTm_->PosMove.Postman
          : (s32)dbpoO4;
      }
      break;

    case (s32)dbpoO4:


      if (infCrs_f && (CrsU_f == FALSE) && (CrsD_f == FALSE))//内野方向入ってたら内野
      {
        ptg = st_.pmgMyTm_->st_.pstMyTm_->PosMove.Postman;
      }
      else
      {
        switch (paMukiZ)
        {
        case mzB: ptg = (s32)dbpoO2; break;
        case mzF: ptg = (s32)dbpoO3; break;
        default:
        {
          s32 distO2 = abs(st_.pstMyCh_->Zahyou.Z - DBCRT_BL);
          s32 distO3 = abs(st_.pstMyCh_->Zahyou.Z - DBCRT_FL);

          ptg = (distO2 < distO3)
            ? dbpoO2
            : dbpoO3;
        }
        break;
        }
      }
      break;
    }



    //ダッシュマンいるとき(十字ニュートラルも)
    if (dashman_f
      && ((ptg == st_.pmgMyTm_->st_.pstMyTm_->PosMove.Postman) || neutral_f))
    {
      BOOL NoTag_f = TRUE;//タゲが居ない
      enNaiyaTag sltg_f[DBMEMBER_INF];
      s32 tgOrd[DBMEMBER_INF];
      s32 sortDt[DBMEMBER_INF];

      s32 sltgX[DBMEMBER_INF];
      s32 sltgZ[DBMEMBER_INF];
      f32 sltgXZ[DBMEMBER_INF];

      //内野全員との角度を取る
      for (s32 i = 0; i < DBMEMBER_INF; ++i)
      {
        if (st_.pmgMyTm_->st_.pmgMyCh_[i]->IsDashman()
          && (i != st_.posNo_)//自分
          //&& (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.dY >= (-XYMAG))//下降ではない
          && (IsCheckLandEnPos(i) == FALSE))//外野からのときは敵コート着地キャラはなしに
        {
          sltg_f[i] = TGOK;
          NoTag_f = FALSE;//一人でも向き方向にタゲが見つかった
        }
        else
        {
          sltg_f[i] = TGNG;
        }
        //X距離外野はGetLeftCrtX()が左コートなので絶対値を使う
        sltgX[i] = (st_.pmgMyTm_->st_.pmgMyCh_[i]->GetLeftCrtX());//自分より右に居れば＋
        //Z距離
        sltgZ[i] = abs(st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z - st_.pstMyCh_->Zahyou.Z);//自分より上にいれば＋
        //距離
        sltgXZ[i] = lib_num::Hypot(sltgX[i], sltgZ[i]);
      }


      //ダッシュマンいるけど敵コートに着地しちゃう場合もある
      //NoTagのときはそのままポストマンに
      if (NoTag_f == FALSE)
      {

        //優先順位初期化
        for (s32 i = 0; i < DBMEMBER_INF; ++i)
        {
          tgOrd[i] = NGNUM;
        }

        s32 f = 0;
        for (s32 i = 0; i < DBMEMBER_INF; ++i)
        {
          sortDt[i] = 0;//初期化

          if (sltg_f[i] == TGOK)//ＯＫ
          {


            if (neutral_f)//ニュートラル
            {
              sortDt[i] = -(s32)sltgX[i];//外野からのときは右（先頭を走ってる人）

            }
            else
            {
              //ダッシュマンが居るときは現在Ｚではなく、目標Ｚ
              s32 tgZ = (dashman_f)
                ? st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->DashmanTgZ
                : st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z;

              //上
              if (CrsU_f)
              {
                sortDt[i] = -tgZ;//Ｚのマイナス（上ほど優先）
              }
              else if (CrsD_f)//下
              {
                sortDt[i] = +tgZ;//Ｚ（下ほど優先）
              }

              if (CrsL_f)//左
              {
                sortDt[i] += (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.X);//Ｘ（左ほど優先）
              }
              else if (CrsR_f)//右
              {
                sortDt[i] -= (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.X);//Ｘのマイナス（右ほど優先）
              }
              //else//ニュートラル
              //{
              //  //sortDt[i] = (s32)sltgXZ[i];//距離
              //  sortDt[i] = (s32)sltgX[i];//先頭を走ってる人
              //}
            }

            tgOrd[f++] = i;
          }
        }


        //ソート
        for (s32 i = 0; i < (DBMEMBER_INF - 1); ++i)
        {
          for (s32 i2 = 0; i2 < (DBMEMBER_INF - 1); i2++)
          {

            if (i == i2) continue;//同じ

            if ((tgOrd[i] != NGNUM) && (tgOrd[i2] != NGNUM))
            {

              s32 dist = abs(sortDt[tgOrd[i]] - sortDt[tgOrd[i2]]);

              //ほぼ同じ場合(1dotいない)
              if (dist <= XYMAG)
              {
                //絶対距離で判断
                if (sltgXZ[tgOrd[i]] < sltgXZ[tgOrd[i2]])//小さい方優先
                {
                  s32 tmp;
                  tmp = tgOrd[i2];
                  tgOrd[i2] = tgOrd[i];
                  tgOrd[i] = tmp;
                }
              }
              else if (sortDt[tgOrd[i]] < sortDt[tgOrd[i2]])//小さい方優先
              {
                s32 tmp;
                tmp = tgOrd[i2];
                tgOrd[i2] = tgOrd[i];
                tgOrd[i] = tmp;
              }
            }
          }
        }

        //ソート１位
        ptg = tgOrd[0];
      }
    }

    return ptg;
  }

  //パスカットタゲセット
  void TChCommon::PaCtTagSet()
  {
    //パスカットキャラセット

    //敵の内野と現在パスタゲとの距離
    pmgSG_->stBa_.PaCtTNo = st_.ensideNo_;
    pmgSG_->stBa_.PaCtPNo = (s32)dbpoI0;

    const s32 ZPER = 150;
    s32 pcX[DBMEMBER_INF];
    s32 pcZ[DBMEMBER_INF];
    f32 pcXZ[DBMEMBER_INF];
    f32 pcXZ2 = NGNUM;

    for (s32 i = 0; i < DBMEMBER_INF; ++i)
    {
      pcX[i] = (st_.pmgMyTm_->st_.pmgEnTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.X
        - st_.pmgMyTm_->st_.pmgMyCh_[pmgSG_->stBa_.PaTgPNo]->st_.pstMyCh_->Zahyou.X);
      pcZ[i] = (st_.pmgMyTm_->st_.pmgEnTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z
        - st_.pmgMyTm_->st_.pmgMyCh_[pmgSG_->stBa_.PaTgPNo]->st_.pstMyCh_->Zahyou.Z);
      pcZ[i] = lib_num::Percent(pcZ[i], ZPER);

      pcXZ[i] = lib_num::Hypot(pcX[i], pcZ[i]);
    }

    for (s32 i = 0; i < DBMEMBER_INF; ++i)
    {
      if ((pcXZ2 == NGNUM) || (pcXZ[i] < pcXZ2))
      {
        //ボールに直にタゲを入れてしまえばよいか
        pmgSG_->stBa_.PaCtPNo = i;
        pcXZ2 = pcXZ[i];
      }
    }
  }

  void TChCommon::NGPaTagShift()
  {

    if (IsNGPassTag(pmgSG_->stBa_.PaTgPNo))
    {
      //パスタゲがパス出せないとき
      BOOL OKIN_f = FALSE;
      if (st_.pmgMyTm_->st_.pstMyTm_->PosMove.PassAbleNum > 0)
      {
        OKIN_f = ((IsNGPassTag(st_.pmgMyTm_->st_.pstMyTm_->PosMove.Postman) == FALSE)
          && (st_.posNo_ != st_.pmgMyTm_->st_.pstMyTm_->PosMove.Postman));
      }
      BOOL OKO2_f = ((IsNGPassTag((s32)dbpoO2) == FALSE) && (st_.posNo_ != (s32)dbpoO2));
      BOOL OKO3_f = ((IsNGPassTag((s32)dbpoO3) == FALSE) && (st_.posNo_ != (s32)dbpoO3));
      BOOL OKO4_f = ((IsNGPassTag((s32)dbpoO4) == FALSE) && (st_.posNo_ != (s32)dbpoO4));

      if (OKO4_f)
      {
        pmgSG_->stBa_.PaTgPNo = (s32)dbpoO4;
      }
      else if (OKO3_f)
      {
        pmgSG_->stBa_.PaTgPNo = (s32)dbpoO3;
      }
      else if (OKO2_f)
      {
        pmgSG_->stBa_.PaTgPNo = (s32)dbpoO2;
      }
      else if (OKIN_f)
      {
        pmgSG_->stBa_.PaTgPNo = st_.pmgMyTm_->st_.pstMyTm_->PosMove.Postman;
      }
      else
      {
        pmgSG_->stBa_.PaTgPNo = (s32)dbpoO4;
      }
    }

  }

  //パスタゲセット★
  void TChCommon::PaTagSet()
  {

    enMukiType NaiyaMuki = (st_.mysideNo_ == 0)
      ? mL
      : mR;

    enMukiTypeA NaiyaMukiA = (st_.mysideNo_ == 0)
      ? maL
      : maR;

    s32 ptg = NGNUM;//パスタゲ

    ptg = (IsInfield())
      ? GetNaiyaPassTag()
      : GetGaiyaPassTag();


    pmgSG_->stBa_.PaTgTNo = st_.mysideNo_;

    if (ptg == NGNUM)
    {
      //パスタゲがパス出せないとき
      NGPaTagShift();
    }
    else
    {
      pmgSG_->stBa_.PaTgPNo = ptg;
    }

    //パスカットキャラセット
    PaCtTagSet();
  }

  //モーション切り替え
  void TChCommon::GenSetMtype(enDBMotionType tMtype)//=FALSE
  {

    //変えられない
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfANG))
    {
      return;
    }

    //勝ち負けポーズ同士の変更も無し
    if ((st_.pstMyCh_->Motion.IsMFlags(dbmfRES))
      && ((tMtype == dbmtWIN) || (tMtype == dbmtLOSE) || (tMtype == dbmtDRAW)))
    {
      return;
    }


    //試合終了してるときは天使にならない→相手が全滅しているとき
    //★これは審判クラスを参照した方がいいか
    if (tMtype == dbmtANG)
    {
      if (st_.pmgEnTm_->IsAllDead()
        || pmgSG_->stRf_.TimeUp_f)
      {
        if (st_.pstMyCh_->Motion.Mtype == dbmtFlF)
        {
          tMtype = dbmtDnF;//ただのダウンに変更
        }
        else
        {
          tMtype = dbmtDnB;//ただのダウンに変更
        }
      }
      else if (st_.pmgMyTm_->IsLastOne() && (pmgSG_->stRf_.Suddun_f == FALSE))
      {
        st_.pmgRf_->SetMotion(dbrfLongWhistle);
      }
    }

    //かがみは立ち歩き走り以外ではならない
    if (tMtype == dbmtKG)
    {
      if ((st_.pstMyCh_->Motion.Mtype != dbmtSt)
        && (st_.pstMyCh_->Motion.Mtype != dbmtWk)
        && (st_.pstMyCh_->Motion.Mtype != dbmtDs)
        && (st_.pstMyCh_->Motion.Mtype != dbmtKG))
      {
        return;
      }
    }

    BOOL switchPW_f = ((tMtype == dbmtDs) && (st_.pstMyCh_->Motion.MNo == dbmnPWDS))
      || ((tMtype == dbmtPWDs) && (st_.pstMyCh_->Motion.MNo == dbmnDS))
      || ((tMtype == dbmtWk) && (st_.pstMyCh_->Motion.MNo == dbmnPWWKF))
      || ((tMtype == dbmtWk) && (st_.pstMyCh_->Motion.MNo == dbmnPWWKN))
      || ((tMtype == dbmtWk) && (st_.pstMyCh_->Motion.MNo == dbmnPWWKR))
      || ((tMtype == dbmtPWWk) && (st_.pstMyCh_->Motion.MNo == dbmnWKF))
      || ((tMtype == dbmtPWWk) && (st_.pstMyCh_->Motion.MNo == dbmnWKN))
      || ((tMtype == dbmtPWWk) && (st_.pstMyCh_->Motion.MNo == dbmnWKR))
      ;

    //パス待ちモーションの時は絵柄だけ変わるように
    //それ以外の時は初期化
    if (switchPW_f == FALSE)
    {
      //初期化

      st_.pstMyCh_->Motion.M_c = 0;
      st_.pstMyCh_->Anime.FrameNo = 0;
      st_.pstMyCh_->Anime.LoopStNo = 0;

      //キャッチカウンタＯＦＦ
      st_.pstMyCh_->Catch_c = NGNUM;

      //敵コート避け時間
      st_.pstMyCh_->ECDdg_c = 0;

      //ダッシュ加速
      st_.pstMyCh_->DsAcc_c = 0;
      //摩擦係数
      st_.pstMyCh_->Zahyou.Fric_c = 0;

      //着地計算まだよ
      st_.pstMyCh_->LandSet_f = FALSE;

      //振り返りフラグ
      st_.pstMyCh_->Utrun_f = FALSE;


      //地上についたら空中復帰消す
      if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr) == FALSE)
      {
        st_.pstMyCh_->AirRev_f = FALSE;
      }


      
      switch (tMtype)
      {
      case dbmtSt:
      case dbmtWk:
      case dbmtCa:
      case dbmtJCa:
        //キャッチ連打が効いてしまう
        break;
      default:
        //キャッチ待ち時間ゼロにしてみる
        st_.pstMyCh_->CatchW_c = 0;
        //おしっぱジャンプ暴発防ぐフラグ
        st_.pstMyCh_->jumpok_f_ = FALSE;
        st_.pstMyCh_->shotok_f_ = FALSE;
        break;
      }



    }

    if (PassWait_f() == FALSE)
    {
      st_.pstMyCh_->PassWait_c = 0;
    }

    //ダウンカウンタのリセット（ここでいいのか怪しい
    if ((tMtype != dbmtFlF)
      && (tMtype != dbmtFlB)
      && (tMtype != dbmtDnF)
      && (tMtype != dbmtDnB)
      && (tMtype != dbmtDnHF)
      && (tMtype != dbmtDnHB)
      )
    {
      st_.pstMyCh_->Down_c = 0;
    }

    //空中ではあり得ないモーション
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr))
    {
      switch (tMtype)
      {
      case dbmtSt: tMtype = dbmtJUp; break;
      case dbmtSh: tMtype = dbmtJSh; break;
      case dbmtRtSh: tMtype = dbmtRtJSh; break;
      case dbmtPa: tMtype = dbmtJPa; break;
      case dbmtCa: tMtype = dbmtJCa; break;
      }
    }
    else//地上ではあり得ないモーション
    {
      switch (tMtype)
      {
      case dbmtJUp: tMtype = dbmtSt; break;
      case dbmtJDn: tMtype = dbmtSt; break;
      case dbmtJSh: tMtype = dbmtSh; break;
      case dbmtRtJSh: tMtype = dbmtRtSh; break;
      case dbmtJPa: tMtype = dbmtPa; break;
      case dbmtJCa: tMtype = dbmtCa; break;
      }
    }


    const s32 KAGAMI2HP = 8;//辛息切れボーダーＨＰ
    s32 livenum = st_.pmgMyTm_->GetLiveNum();


    //歩数リセット
    switch (tMtype)
    {
    case dbmtSt:
    case dbmtWk:
      st_.pstMyCh_->Step_c = 0;//歩数を初期化
      st_.pstMyCh_->MadStep_c = 0;//歩数を初期化
      break;
    }


    //フラグの初期化
    switch (tMtype)
    {
      //立ち
    case dbmtSt:

      st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnSTF);
      st_.pstMyCh_->Motion.MFlags = (dbmfJpOK | dbmfMuki | dbmfDmOK | dbmfAtCa);

      break;

      //息継ぎ
    case dbmtBreath:
      //Ｚ向きでモーション変える
      st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnBRF);
      st_.pstMyCh_->Motion.MFlags = (dbmfJpOK | dbmfMuki | dbmfDmOK | dbmfAtCa);
      //建前上、立ちにする
      tMtype = dbmtSt;
      break;

      //歩き
    case dbmtWk:

      //st_.pstMyCh_->Motion.MNo = dbmnWK;
      st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnWKF);
      st_.pstMyCh_->Motion.MFlags = (dbmfJpOK | dbmfMuki | dbmfDmOK | dbmfAtCa);
      break;

      //ダッシュ
    case dbmtDs:
      //ダッシュ継続以外
      if (lib_num::IsFlags((s32)st_.pstMyCh_->Motion.MFlags, (s32)dbmfDs) == FALSE)
      {
        st_.pstMyCh_->Step_c = 0;//歩数を初期化
        st_.pstMyCh_->MadStep_c = 0;//歩数を初期化
      }
      st_.pstMyCh_->Motion.MNo = dbmnDS;
      st_.pstMyCh_->Motion.MFlags = (dbmfDs | dbmfJpOK | dbmfAtCa);
      break;

      //ジャンプ前しゃがみ
    case dbmtJCr:
      SetTutoOK(sta_JumpAct);//ジャンプクリア
      st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnJCF);
      st_.pstMyCh_->Motion.MFlags = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
        ? (dbmfAct | dbmfAr | dbmfDs)
        : (dbmfAct | dbmfAr);
      break;

      //キャンセルジャンプ前しゃがみ（Dsフラグが入らない）
    case dbmtCJCr:
      st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnCRF);
      st_.pstMyCh_->Motion.MFlags = (dbmfAct | dbmfAr);
      tMtype = dbmtJCr;
      break;

      //ジャンプ中
    case dbmtJUp:
    case dbmtJDn:

      if (st_.pstMyCh_->Zahyou.dY < 0) tMtype = dbmtJDn;

      st_.pstMyCh_->Motion.MNo = (tMtype == dbmtJUp)
        ? RevMNoZ(dbmnAIRUPF)
        : RevMNoZ(dbmnAIRDNF);

      st_.pstMyCh_->Motion.MFlags = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
        ? (dbmfAr | dbmfMuki | dbmfAtCa | dbmfDs)
        : (dbmfAr | dbmfMuki | dbmfAtCa);

      break;

      //空中復帰
    case dbmtARv:
      st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnAIRDNF);
      st_.pstMyCh_->Motion.MFlags = (dbmfAr | dbmfMuki | dbmfAtCa);
      st_.pstMyCh_->AirAct_f = TRUE;//復帰した後やっぱし動けるように→行動不能だけどオートキャッチ
      tMtype = dbmtJDn;
      break;

      //しゃがみ
    case dbmtCr:
      st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnCRF);
      st_.pstMyCh_->Motion.MFlags = (dbmfAct | dbmfAtCa);//一応セミオートのためオートキャッチ可能フラグ追加
      break;

      //吹っ飛ばされ
    case dbmtFlF:
    case dbmtFlB:
      //st_.pstMyCh_->Anime.FrameNo = (tMtype == dbmtFlF)
      //  ? 0 //前向き吹っ飛ばされ
      //  : 1;//後ろ向き吹っ飛ばされ
      st_.pstMyCh_->Motion.MNo = (tMtype == dbmtFlF)
        ? dbmnFLF
        : dbmnFLB;
      st_.pstMyCh_->Motion.MFlags = (dbmfAr | dbmfDam | dbmfMTK);
      break;

      //ごろごろ
    case dbmtRoF:
    case dbmtRoB:
      st_.pstMyCh_->Motion.MNo = (tMtype == dbmtRoF)
        ? dbmnROF //前転がり
        : dbmnROB;//後ろ転がり
      st_.pstMyCh_->Motion.MFlags = (dbmfDam | dbmfDn);
      SESet(seRoll);//SEならす
      break;

      //パスヒット
    case dbmtPHF:
    case dbmtPHB:
      st_.pstMyCh_->Motion.MNo = (tMtype == dbmtPHF)
        ? dbmnPHF
        : dbmnPHB;
      st_.pstMyCh_->Motion.MFlags = (dbmfPHit);
      break;

      //かがみ
    case dbmtKG:
      //のこりＨＰでかがみっぷりを変える

      st_.pstMyCh_->Motion.MNo = (st_.pstMyCh_->HP > KAGAMI2HP)
        ? dbmnKG
        : dbmnKG2;
      st_.pstMyCh_->Motion.MFlags = (dbmfDam | dbmfKG);

      break;

      //ダウンヒット
    case dbmtDnHF:
    case dbmtDnHB:
      st_.pstMyCh_->Motion.MNo = (tMtype == dbmtDnHF)
        ? dbmnDNHF
        : dbmnDNHB;
      st_.pstMyCh_->Motion.MFlags = (dbmfDam | dbmfDn);
      break;

      //ダウン
    case dbmtDnF:
    case dbmtDnB:
      st_.pstMyCh_->Motion.MNo = (tMtype == dbmtDnF)
        ? dbmnDNF
        : dbmnDNB;
      st_.pstMyCh_->Motion.MFlags = (dbmfDam | dbmfDn);

      //if (pmgEO_->mgStSn_.EtcSS.IceCourt_f())
      //{
      //  s32 spd = (abs(st_.pstMyCh_->Zahyou.dX) > abs(st_.pstMyCh_->Zahyou.dZ))
      //    ? abs(st_.pstMyCh_->Zahyou.dX)
      //    : abs(st_.pstMyCh_->Zahyou.dZ);

      //  //ダウン滑り効果音
      //  if (spd > 200)
      //  {
      //    SESet(seDownLv3);
      //  }
      //  else if (spd > 100)
      //  {
      //    SESet(seDownLv2);
      //  }
      //  else if (spd > 25)
      //  {
      //    SESet(seDownLv1);
      //  }
      //  else
      //  {
      //    SESet(seDownLv0);
      //  }
      //  //MvDefSl(pmgEO_->mgDt_.dtSet_.GetDtCourt(setDownBrkIce));
      //}
      //else
      {
        SESet(seDownLv0);//SEならす
      }
      //SetSerifu(srfDown);//セリフ
      break;

      //ダウンからの復帰
    case dbmtDRv:
      st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnDRVF);
      st_.pstMyCh_->Motion.MFlags = (dbmfDam | dbmfDn);
      break;

      //キャッチモーション
    case dbmtCM:
      //キャッチクリア
      SetTutoOK(sta_CatchAct);
      st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnCMF);
      st_.pstMyCh_->Motion.MFlags = (dbmfAct);
      break;

      //ファンブル
    case dbmtFB:
      st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnFBF);
      st_.pstMyCh_->Motion.MFlags = (dbmfAct);
      break;

      //パス待ち
    case dbmtPW:
      st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnPWF);
      st_.pstMyCh_->Motion.MFlags = (dbmfJpOK | dbmfMuki | dbmfPW | dbmfDmOK);
      break;

      //パス待ち歩き
    case dbmtPWWk:
      //st_.pstMyCh_->Motion.MNo = dbmnPWWK;
      st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnPWWKF);
      st_.pstMyCh_->Motion.MFlags = (dbmfJpOK | dbmfMuki | dbmfPW | dbmfDmOK);
      tMtype = dbmtWk;
      break;

      //パス待ちダッシュ★
    case dbmtPWDs:
      st_.pstMyCh_->Motion.MNo = dbmnPWDS;
      st_.pstMyCh_->Motion.MFlags = (dbmfDs | dbmfJpOK | dbmfPW);
      tMtype = dbmtDs;

      break;

      //スリップ
    case dbmtSl:
      st_.pstMyCh_->Motion.MNo = dbmnSL;
      st_.pstMyCh_->Motion.MFlags = (dbmfAct | dbmfJpOK | dbmfSlip | dbmfAtCa);
      break;

      //シュート・パス
    case dbmtSh:
    case dbmtRtSh:
    case dbmtPa:
      //シュート
      if ((tMtype == dbmtSh) || (tMtype == dbmtRtSh))
      {
        //シュートクリア
        SetTutoOK(sta_ShotAct);

        st_.pstMyCh_->Motion.MNo = (tMtype == dbmtSh)
          ? RevMNoZ(dbmnSHF)
          : dbmnRTNSH;

        //振り替えリシュート
        if (tMtype == dbmtRtSh)
        {
          st_.pstMyCh_->Zahyou.MukiZ = mzN;
          st_.pstMyCh_->Utrun_f = TRUE;
        }
        tMtype = dbmtSh;

        //投げ始め終わりの長さ
        st_.pstMyCh_->ShStWait_c = RankLevel(rkShStMotion);
        st_.pstMyCh_->ShEdWait_c = RankLevel(rkShEdMotion);
      }
      else//パス
      {
        st_.pstMyCh_->PaStWait_c = pmgEO_->mgDt_.dtSet_.GetDtPass(setPaStWait);
        st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnPAF);

        //トスパスフラグ
        //st_.pstMyCh_->QuickPass_f = TRUE;
        st_.pstMyCh_->TossPass_f = TRUE;
      }

      if (st_.pstMyCh_->Motion.IsMFlags(dbmfSlip))
      {
        st_.pstMyCh_->Motion.MFlags = (dbmfAct | dbmfSlip);//スリップ
      }
      else if (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
      {
        st_.pstMyCh_->Motion.MFlags = (dbmfAct | dbmfDs);//ダッシュフラグ残す
      }
      else
      {
        st_.pstMyCh_->Motion.MFlags = (dbmfAct);
      }

      pmgSG_->stBa_.shotmotion_f = TRUE;

      break;

      //ジャンプシュート・パス
    case dbmtJSh:
    case dbmtRtJSh:
    case dbmtJPa:

      //ダッシュフラグ残す
      st_.pstMyCh_->Motion.MFlags = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
        ? (dbmfAr | dbmfAct | dbmfDs)
        : (dbmfAr | dbmfAct);


      if ((tMtype == dbmtJSh) || (tMtype == dbmtRtJSh))//ジャンプシュート
      {
        //シュートクリア
        SetTutoOK(sta_ShotAct);

        //敵のコートの方向を向いているか
        BOOL enMuki_f = ((st_.mysideNo_ == 0) && (st_.pstMyCh_->Zahyou.Muki == mR))
          || ((st_.mysideNo_ == 1) && (st_.pstMyCh_->Zahyou.Muki == mL));
        //パスタゲが内野
        BOOL gaiyatg_f = (pmgSG_->stBa_.PaTgPNo >= DBMEMBER_INF);


        if (enMuki_f)
        {
          if (IsSelfCtrl() == FALSE)
          {
            gaiyatg_f = TRUE;
          }
          else
          {
            if (gaiyatg_f == FALSE)
            {
              //どうせ届かないからタゲの状態はどうでもいいか
              gaiyatg_f = TRUE;
            }
          }
        }


        //投げ始め終わりの長さ
        st_.pstMyCh_->ShStWait_c = RankLevel(rkShStMotion);
        st_.pstMyCh_->ShEdWait_c = RankLevel(rkShEdMotion);
        st_.pstMyCh_->Motion.MNo = (tMtype == dbmtJSh)
          ? RevMNoZ(dbmnJSHF)
          : dbmnRTNJSH;


        tMtype = dbmtJSh;

      }
      else//ジャンプパス
      {
        st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnJPAF);
        //トスパスフラグ
        //st_.pstMyCh_->QuickPass_f = TRUE;
        st_.pstMyCh_->TossPass_f = TRUE;
      }
      //ぶぶぶもあるので一律
      st_.pstMyCh_->PaStWait_c = pmgEO_->mgDt_.dtSet_.GetDtPass(setPaStWait);

      st_.pstMyCh_->AirAct_f = TRUE;//空中アクション

      pmgSG_->stBa_.shotmotion_f = TRUE;
      break;

      //キャッチ・よけ
    case dbmtCa:
    case dbmtDg:


      if (tMtype == dbmtCa)//キャッチ
      {
        //キャッチボタンが押された
        SetActBtn(sta_CatchAct);

        st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnCAF);
        //キャッチ待ち時間
        st_.pstMyCh_->CatchW_c = pmgEO_->mgDt_.dtSet_.GetDtCatch(setCatchWait);

        //ダッシュフラグ残す
        st_.pstMyCh_->Motion.MFlags = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
          ? (dbmfAct | dbmfDs)
          : (dbmfAct);
      }
      else//よけ
      {
        //避けボタンが押された
        SetActBtn(sta_DodgeAct);

        st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnDGF);

        //ダッシュフラグ残す
        st_.pstMyCh_->Motion.MFlags = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
          ? (dbmfAct | dbmfDs | dbmfDg)
          : (dbmfAct | dbmfDg);
      }
      ////ダッシュフラグ残す
      //st_.pstMyCh_->Motion.MFlags = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
      //? (dbmfAct | dbmfDs)
      //: (dbmfAct);
      break;

      //ジャンプキャッチ・よけ
    case dbmtJCa:
    case dbmtJDg:
      if (tMtype == dbmtJCa)//キャッチ
      {
        //キャッチボタンが押された
        SetActBtn(sta_CatchAct);

        st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnJCAF);

        //ダッシュフラグ残す
        st_.pstMyCh_->Motion.MFlags = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
          ? (dbmfAr | dbmfAct | dbmfDs | dbmfDg)
          : (dbmfAr | dbmfAct);
      }
      else//よけ
      {
        //避けボタンが押された
        SetActBtn(sta_DodgeAct);

        st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnJDGF);

        //ダッシュフラグ残す
        st_.pstMyCh_->Motion.MFlags = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
          ? (dbmfAr | dbmfAct | dbmfDs | dbmfDg)
          : (dbmfAr | dbmfAct);
      }

      st_.pstMyCh_->AirAct_f = TRUE;//空中アクション
      st_.pstMyCh_->AirCatch_f = TRUE;//取れていようが居まいが構わないか
      break;

      //ジャンプキャッチ・よけ
    case dbmtJCM:
    case dbmtJFB:
      if (tMtype == dbmtJCa)//キャッチ
      {
        st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnJCMF);
      }
      else//よけ
      {
        st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnJFBF);
      }
      //ダッシュフラグ残す
      st_.pstMyCh_->Motion.MFlags = (dbmfAr | dbmfAct);

      st_.pstMyCh_->AirAct_f = TRUE;//空中アクション
      st_.pstMyCh_->AirCatch_f = TRUE;//取れていようが居まいが構わないか
      break;

      //オーバーライン
    case dbmtOvL:
      st_.pstMyCh_->Motion.MNo = RevMNoZ(dbmnOVLF);
      //ダッシュフラグ残す
      st_.pstMyCh_->Motion.MFlags = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
        ? (dbmfAct | dbmfDs)
        : (dbmfAct);

      //★オーバーラインホイッスル
      if (pmgSG_->stRf_.GameSet_f == FALSE)
      {
        st_.pmgRf_->SetMotion(dbrfShortWhistle);
        st_.pmgRf_->RefereeMsg("オーバーライン");

        //チュートＮＧオーバーライン
        SetTutoNG(sta_ShotAct, rta_shot_ngovline);

        //                    //オーバーラインにならない時間
        //                    if (st_.pstMyCh_->Steal_c == 0)
        //                    {
        //                        AddRec(recOverLine_c);
        //                        //★成績//オーバーライン数合計
        //                        //AddRec(recOverLine_c)++;
        //                    }
      }
      break;

      //天使
    case dbmtANG:
      st_.pstMyCh_->Motion.MNo = dbmnANG;
      st_.pstMyCh_->Motion.MFlags = (dbmfAr | dbmfMTK | dbmfDam | dbmfANG);

      st_.pstMyCh_->Dead_f = TRUE;//死亡
      ZeroXYZ();

      //天使生成
      st_.pmgMyTm_->SetAngel(st_.posNo_);


      break;

      //引き分け
    case dbmtDRAW:
      st_.pstMyCh_->Motion.MNo = dbmnDRAW;
      st_.pstMyCh_->Motion.MFlags = (dbmfRES);
      //強制地上（特殊）
      st_.pstMyCh_->Zahyou.Y = 0;
      //強制向き
      st_.pstMyCh_->Zahyou.Muki = (IsInfield() ^ (st_.mysideNo_ == 1))
        ? mR
        : mL;
      break;

      //勝利
    case dbmtWIN:
      st_.pstMyCh_->Motion.MNo = dbmnWIN;
      st_.pstMyCh_->Motion.MFlags = (dbmfRES);
      //強制地上（特殊）
      st_.pstMyCh_->Zahyou.Y = 0;
      //強制向き
      st_.pstMyCh_->Zahyou.Muki = (IsInfield() ^ (st_.mysideNo_ == 1))
        ? mR
        : mL;
      break;

      //敗北
    case dbmtLOSE:
      st_.pstMyCh_->Motion.MNo = dbmnLOSE;
      st_.pstMyCh_->Motion.MFlags = (dbmfRES);
      //強制地上（特殊）
      st_.pstMyCh_->Zahyou.Y = 0;
      //強制向き
      st_.pstMyCh_->Zahyou.Muki = (IsInfield() ^ (st_.mysideNo_ == 1))
        ? mR
        : mL;
      break;

      //  //うさぎ跳び
      //case dbmtUSA:
      //  st_.pstMyCh_->Motion.MNo = dbmnUSA;
      //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
      //  break;
      //
      //  //うさぎ跳び2
      //case dbmtUSA2:
      //  st_.pstMyCh_->Motion.MNo = dbmnUSA2;
      //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
      //  break;
      //
      //  //息切れ
      //case dbmtIKI:
      //  st_.pstMyCh_->Motion.MNo = dbmnIKI;
      //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
      //  break;
      //
      //  //こっちみる
      //case dbmtLOOK:
      //  st_.pstMyCh_->Motion.MNo = dbmnLOOK;
      //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
      //  break;
      //
      //  //こっちみて笑う
      //case dbmtLOOK2:
      //  st_.pstMyCh_->Motion.MNo = dbmnLOOK2;
      //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
      //  break;
      //
      //  ////おちる
      //  //case dbmtFALL:
      //  //  st_.pstMyCh_->Motion.MNo = dbmnFALL;
      //  //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
      //  //  break;
      //
      //  //胴上げられ
      //case dbmtDO1:
      //  st_.pstMyCh_->Motion.MNo = dbmnDO1;
      //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
      //  break;
      //
      //  //胴上げられ
      //case dbmtDO2:
      //  st_.pstMyCh_->Motion.MNo = dbmnDO2;
      //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
      //  break;
      //
      //  //胴上げ
      //case dbmtAGE2:
      //  st_.pstMyCh_->Motion.MNo = dbmnAGEF;
      //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
      //  break;
      //  //胴上げ
      //case dbmtAGE3:
      //  st_.pstMyCh_->Motion.MNo = dbmnAGEN;
      //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
      //  break;
      //  //胴上げ
      //case dbmtAGE4:
      //  st_.pstMyCh_->Motion.MNo = dbmnAGER;
      //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
      //  break;
      //  //胴上げ
      //case dbmtAGE5:
      //  st_.pstMyCh_->Motion.MNo = dbmnAGEC;
      //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
      //  break;
      //
    default:
      ////ないと思うけど一応
      //st_.pstMyCh_->Motion.MNo = dbmnFALL;
      break;
    }



    //空中ではなくなったとき
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr) == FALSE)
    {
      st_.pstMyCh_->Air_c = 0;
      st_.pstMyCh_->ShAir_c = 0;
      st_.pstMyCh_->AirCatch_f = FALSE;
      st_.pstMyCh_->AirAct_f = FALSE;

      //if (st_.pstMyCh_->Zahyou.Y > 0)
      //{
      //  //★浮いちゃ行けないところで浮いてしまっている
      //  s32 a = 0;//ここにブレークポイントを置いて呼び出し履歴を見る
      //}
    }

    if (st_.pstMyCh_->Motion.IsMFlags(dbmfAct)
      && (st_.pstMyCh_->Auto.AutoType == dbatFree))
    {
      st_.pstMyCh_->FreeAct_f = TRUE;//フリー状態からのアクション
    }
    else if ((tMtype == dbmtJCr) || (tMtype == dbmtJUp))
    {
      //フリー状態を継続
    }
    else
    {
      st_.pstMyCh_->FreeAct_f = FALSE;//フリー状態からのアクション
    }

    //ダッシュではなくなったとき★★
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfDs) == FALSE)
    {
      if (st_.pstMyCh_->DashmanNo != NGNUM)
      {
        st_.pstMyCh_->DashmanNo = NGNUM;//埋め
      }

      //ダッシュフラグが消えたらIOS加速ダッシュを消す
      st_.pstMyCh_->DsAccIOS_f = FALSE;
    }

    //モーションをセット
    st_.pstMyCh_->Motion.Mtype = tMtype;

    //パス待ち変化ではない
    if (switchPW_f == FALSE)
    {
      //フレーム情報のセット
      SetFrameData(FALSE);
    }
  }

  //Ｚ向きごとのアニメーションオフセット
  enDBMotionNo TChCommon::RevMNoZ(enDBMotionNo mno)
  {
    switch (st_.pstMyCh_->Zahyou.MukiZ)
    {
    case mzN: return (enDBMotionNo)(mno + 1);
    case mzB: return (enDBMotionNo)(mno + 2);
    default:  return mno;
    }
  }

  //シュートタゲセット
  void TChCommon::ShTagSet(BOOL NoRefMuki_f)
  {
    pmgSG_->stBa_.ShTgPNo = (st_.pmgRf_->IsCapOnlyCtrl())
      ? 0
      : GetTag(NoRefMuki_f);
    pmgSG_->stBa_.ShTgTNo = st_.ensideNo_;
  }

  s32 TChCommon::GetSLX2(s32 tz, BOOL sideR_f)
  {
    if (sideR_f)
    {
      return DBCRT_W - (DBCRT_SL + ((tz - DBCRT_FL) * 10 / DBCRT_SLR));
    }
    else
    {
      return (DBCRT_SL + ((tz - DBCRT_FL) * 10 / DBCRT_SLR));
    }
  }

  void TChCommon::SetNoneTagPos(TShotInfo* ShDt)
  {
    ShDt->tdys = -1;

    //真横向き
    if (st_.pstMyCh_->Zahyou.MukiZ == mzN)
    {
      const s32 DEFDX = 200;
      const s32 DEFDY = 150;

      ShDt->tdx = DEFDX;
      ShDt->tdz = 0;

      ShDt->tdy = (ShDt->tJSh_f)
        ? DEFDY
        : 0;
    }
    else//奥手前向き
    {
      const f32 NOTAGAGL_ST = 45.0;
      const f32 NOTAGAGL_JP = 30.0;



      f32 notagagl = lib_num::DegToRad(NOTAGAGL_ST);
      ShDt->tdx = (s32)abs((cos(notagagl) * 1000));
      ShDt->tdz = (s32)abs((sin(notagagl) * 1000));

      switch (st_.pstMyCh_->Zahyou.MukiZ)
      {
      case mzF: ShDt->tdzs = -1; break;
      case mzB: ShDt->tdzs = +1; break;
      }
      //ジャンプ
      if (ShDt->tJSh_f)
      {

        f32 hyp = lib_num::Hypot(ShDt->tdx, ShDt->tdz);
        f32 notagaglj = lib_num::DegToRad(NOTAGAGL_JP);
        ShDt->tdy = (s32)(hyp * notagaglj);
      }
      else
      {
        ShDt->tdy = 0;
      }
    }
  }


  //ｎフレーム後の空中位置
  s32 TChCommon::GetArTagY(TStChar* tagPSt, s32 t)
  {
    s32 grv = pmgEO_->mgDt_.dtSet_.GetDtGrv(setGRV_Y);

    s32 v0;
    //Y初速
    if (tagPSt->Motion.Mtype == dbmtJCr)
    {
      v0 = (tagPSt->Motion.IsMFlags(dbmfDs))
        ? pmgEO_->mgDt_.dtSet_.GetDtJump(setDashJump_vY0)
        : pmgEO_->mgDt_.dtSet_.GetDtJump(setJump_vY0);
    }
    else
    {
      v0 = tagPSt->Zahyou.dY;
    }

    s32 res = (tagPSt->Zahyou.Y + ((v0 * t) + (((-grv) * t * t) / 2)));

    if (res < 0) res = 0;

    return res;
  }


  //シュートタゲまでの距離
  void TChCommon::GetShotTagDist(TShotInfo* ShDt)
  {
    //円輪、加速、かっくん、ブーメランきょだい、ぶーめらん、で特殊処理すてっぷ

    const s32 BODYY = 16 * XYMAG;

    //タゲ無し
    if (pmgSG_->stBa_.ShTgPNo == NGNUM)
    {
      SetNoneTagPos(ShDt);
      return;
    }
    else//タゲあり
    {
      //タゲのショートカット
      s32 tgCharNo = pmgSG_->stBa_.ShTgPNo;// st_.pmgEnTm_->st_.pstMyTm_->PosSt.CharofP[pmgSG_->stBa_.ShTgPNo]; //死んだ後のきゃらをねらってしまうので修正
      TMgChar* tgP = (pmgGO_->pmgCh_[st_.ensideNo_][tgCharNo]);//pmgCh_オブジェはポジションを扱う

      //水平シュート
      BOOL level_f = (ShDt->tJSh_f == FALSE)//ジャンプしてない＆相手がダウンしてない
        && (tgP->st_.pstMyCh_->Motion.Mtype != dbmtDnF)
        && (tgP->st_.pstMyCh_->Motion.Mtype != dbmtDnB);

      if ((ShDt->tJSh_f == FALSE) && ShDt->HS_f) level_f = TRUE;

      //着地狙い
      BOOL airtag_f = FALSE;

      //相手の位置
      s32 ttx = tgP->st_.pstMyCh_->Zahyou.X;
      s32 ttz = tgP->st_.pstMyCh_->Zahyou.Z;
      s32 tty = tgP->st_.pstMyCh_->Zahyou.Y;

      //ボールの位置
      s32 bx = pmgSG_->stBa_.Zahyou.X;
      s32 bz = pmgSG_->stBa_.Zahyou.Z;
      s32 by = pmgSG_->stBa_.Zahyou.Y;
      if (st_.pstMyCh_->Zahyou.MukiZ == mzN)//真横向きの時はボール位置ではなくキャラ位置を基準に
      {
        bx = st_.pstMyCh_->Zahyou.X;
        bz = st_.pstMyCh_->Zahyou.Z;
      }

      //相手との距離(ボールの位置基準)
      s32 tdx = abs(ttx - bx);
      s32 tdz = abs(ttz - bz);
      s32 tdy = abs(tty - by);

      //届くまでの時間
      s32 t = (s32)(lib_num::Hypot(tdx, tdy, tdz) / ShDt->tShSp);
      ShDt->resttime = t;

      //異動先ねらい（自分も空中）
      //if (pmgEO_->mgDt_.dtShot_.GetDtBOOL(ShDt->tHSNo, shdtNoRevAgl) == FALSE)
      {

        if (ShDt->tJSh_f && (tgP->st_.pstMyCh_->Motion.IsMFlags(dbmfAr)))
        {
          //先にタゲが地面に付く
          if (tgP->st_.pstMyCh_->LandRest < t)
          {
            //着地位置
            ttx = tgP->st_.pstMyCh_->Zahyou.X + (tgP->st_.pstMyCh_->Zahyou.dX * tgP->st_.pstMyCh_->LandRest);
            ttz = tgP->st_.pstMyCh_->Zahyou.Z + (tgP->st_.pstMyCh_->Zahyou.dZ * tgP->st_.pstMyCh_->LandRest);
            tty = 0;
          }
          else
          {
            //空中位置
            ttx = tgP->st_.pstMyCh_->Zahyou.X + (tgP->st_.pstMyCh_->Zahyou.dX * t);
            ttz = tgP->st_.pstMyCh_->Zahyou.Z + (tgP->st_.pstMyCh_->Zahyou.dZ * t);
            tty = tgP->chCommon_.GetArTagY(tgP->st_.pstMyCh_, t);

            //ボールより高い位置だったら同じに
            if (by < tty)
            {
              tty = by;
            }
          }

          tdx = abs(ttx - bx);
          tdz = abs(ttx - bz);
          tdy = abs(tty - by);

          //届くまでの時間
          t = (s32)(lib_num::Hypot(tdx, tdy, tdz) / ShDt->tShSp);
          ShDt->resttime = t;

          airtag_f = TRUE;
        }
        else
        {
          //異動先
          ttx = tgP->st_.pstMyCh_->Zahyou.X + ((tgP->st_.pstMyCh_->Zahyou.dX / 2) * t);

          if (tgP->st_.pstMyCh_->Motion.IsMFlags(dbmfAr) == FALSE)
          {
            //センターラインを越えるとき
            if (((st_.mysideNo_ == 0) && (tgP->st_.pstMyCh_->Zahyou.X < DBCRT_CLI) && (ttx > DBCRT_CLI))
              || ((st_.mysideNo_ == 1) && (tgP->st_.pstMyCh_->Zahyou.X > DBCRT_CLI) && (ttx < DBCRT_CLI)))
            {
              ttx = DBCRT_CLI;
            }

            //エンドラインを越えるとき
            if ((st_.mysideNo_ == 0)
              && (tgP->st_.pstMyCh_->Zahyou.X > GetSLX2(tgP->st_.pstMyCh_->Zahyou.Z, FALSE))
              && (ttx < GetSLX2(tgP->st_.pstMyCh_->Zahyou.Z, FALSE)))
            {
              ttx = GetSLX2(tgP->st_.pstMyCh_->Zahyou.Z, FALSE);
            }
            else if ((st_.mysideNo_ == 1)
              && (tgP->st_.pstMyCh_->Zahyou.X < GetSLX2(tgP->st_.pstMyCh_->Zahyou.Z, TRUE))
              && (ttx > GetSLX2(tgP->st_.pstMyCh_->Zahyou.Z, TRUE)))
            {
              ttx = GetSLX2(tgP->st_.pstMyCh_->Zahyou.Z, TRUE);
            }
          }

        }
      }

      //符号
      ShDt->tdxs = (ttx >= bx)
        ? +1
        : -1;

      //距離Ｘ
      ShDt->tdx = lib_num::Max(abs(ttx - bx), XYMAG);
      //距離Ｚ
      ShDt->tdz = lib_num::Max(abs(ttz - bz), XYMAG);

      //符号
      ShDt->tdzs = (ttz >= bz)
        ? +1
        : -1;

      //Ｙ
      //符号
      ShDt->tdys = -1;
      //ShDt->tdys = pmgEO_->mgDt_.dtShot_.GetDtBOOL(ShDt->tHSNo,shdtYamanari)//やなまり
      //? +1
      //: -1;

      if (level_f && (airtag_f == FALSE))
      {
        ShDt->tdy = 0;//水平
      }
      else
      {
        const s32 GOSA = (8 * XYMAG);
        const s32 GOSA2 = (16 * XYMAG);

        //シュートはボールリリース位置がボールの座標のままなので
        //ここはボールの座標じゃないと当たってくれない
        //相手が普通状態のときはやや高い位置を狙う
        if ((st_.pstMyCh_->Zahyou.MukiZ != mzN)//(ShDt->tdz > ShDt->tdx)
          && (tgP->st_.pstMyCh_->Motion.Mtype != dbmtDnF)
          && (tgP->st_.pstMyCh_->Motion.Mtype != dbmtDnB)
          && (tgP->st_.pstMyCh_->Motion.Mtype != dbmtFlB)
          && (tgP->st_.pstMyCh_->Motion.Mtype != dbmtFlF))
        {
          ShDt->tdy = abs(tty - by + GOSA2);//abs(tty - by + GOSA2);
        }
        else
        {
          ShDt->tdy = abs(tty - by);//abs(tty - by);
        }

        if (ShDt->tdy < 0)
        {
          ShDt->tdy = 0;//水平より上を向かないように
        }
      }
    }
  }
  //シュート角度
  void TChCommon::SAngle(TShotInfo* ShDt)
  {
    //f32 eX, eY, eZ;
    //const s32 NEARBORDER = 100;
    //f32 d, j;
    //d = lib_num::Hypot(ShDt->tdx, ShDt->tdz);//XZ斜辺の長さ
    //j = (f32)(ShDt->tdy * ((ShDt->tdx * pmgEO_->mgDt_.dtSet_.GetDtShot(setLimitAngleX)) + (ShDt->tdz * pmgEO_->mgDt_.dtSet_.GetDtShot(setLimitAngleZ))))
    //  / (ShDt->tdx + ShDt->tdz);
    //if (abs(d * XYMAG) < abs(j))
    //{
    //  s32 ttdx = (s32)((j * ShDt->tdx) / (ShDt->tdx + ShDt->tdz) / XYMAG);
    //  s32 ttdz = (s32)((j * ShDt->tdz) / (ShDt->tdx + ShDt->tdz) / XYMAG);
    //  ShDt->tdx = abs(ttdx);
    //  ShDt->tdz = abs(ttdz);
    //}
    //d = lib_num::Hypot(ShDt->tdx, ShDt->tdz);//斜辺の長さ
    ////一定以下の高さ（すけすけ状態では鋭角シュートも可能）
    //if ((ShDt->tdy * XYMAG) < pmgEO_->mgDt_.dtSet_.GetDtShot(setFreeAngleY))
    //{
    //  //投げおろし角度が45度(Y:d=1:1)より小さくならないように
    //  if (abs(ShDt->tdy) > (d))
    //  {
    //    ShDt->tdy = lib_num::Sign(ShDt->tdy) * (s32)d;
    //  }
    //}
    //j = lib_num::Hypot((s32)d, ShDt->tdy);//３辺の斜辺の長さ
    //if (j == 0)
    //{
    //  ShDt->tdx = ShDt->tShSp;
    //  ShDt->tdy = 0;
    //  ShDt->tdz = 0;
    //}
    //else
    //{
    //  eX = ((ShDt->tdx * ShDt->tShSp) / j);
    //  eY = ((ShDt->tdy * ShDt->tShSp) / j);
    //  eZ = ((ShDt->tdz * ShDt->tShSp) / j);
    //  ShDt->tdx = (s32)eX;
    //  ShDt->tdy = (s32)eY;
    //  ShDt->tdz = (s32)eZ;
    //}
  }

  //空中シュートタイミング判定
  s32 TChCommon::GetAirShootTiming()
  {
    s32 res = NGNUM;
    s32 tStart = (st_.pstMyCh_->TopTiming + JHSSTART);

    for (s32 i = 0; i < STEPNUM; ++i)
    {
      s32 jpos = i;
      if ((st_.pstMyCh_->ShAir_c >= tStart + (JHSRANGE * (jpos - 1)))
        && (st_.pstMyCh_->ShAir_c < tStart + (JHSRANGE * (jpos + 0))))
      {
        res = i;
        break;
      }
    }

    return res;
  }
  //ダッシュシュートタイミング判定
  s32 TChCommon::GetDashShootTiming()
  {
    s32 res = NGNUM;

    for (s32 i = 0; i < STEPNUM; ++i)
    {
      s32 step = i;
      if ((st_.pstMyCh_->Step_c >(DHSRANGE * step))
        && (st_.pstMyCh_->Step_c <= (DHSRANGE * (step + 1))))
      {
        res = i;
        break;
      }
    }
    return res;
  }


  //シュート種類
  BOOL TChCommon::IsHSTiming()
  {
    BOOL res = FALSE;


    if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr))
    {
      s32 timing = GetAirShootTiming();
      if (st_.pstMyCh_->Motion.IsMFlags(dbmfDs) && IsInfield())
      {
        if ((timing != NGNUM) && (st_.pstMyCh_->stGousei.set_f_[timing]))
        {
          res = TRUE;
        }
      }
      else
      {
        if ((timing == VJHSTIMING) && st_.pstMyCh_->stGousei.vjset_f_)
        {
          res = TRUE;
        }
      }
    }
    else
    {

      if (st_.pstMyCh_->Motion.IsMFlags(dbmfDs) && IsInfield())//ダッシュ系（内野のみ）
      {
        s32 timing = GetDashShootTiming();
        if (timing == NGNUM)
        {
        }
        else if (st_.pstMyCh_->stGousei.set_f_[timing])
        {
          res = TRUE;
        }
      }
    }

    return res;

  }

  //シュート種類
  void TChCommon::GetShotType(TShotInfo* ShDt)
  {
    ShDt->sType = shtS;
    ShDt->HSTiming = NGNUM;

    //プレビューは常に必殺
    if (pmgGO_->pmgRf_->IsPrev())
    {
      if (ShDt->tJSh_f)
      {
        ShDt->sType = shtJHS;
      }
      else
      {
        ShDt->sType = shtHS;
      }
      ShDt->HSTiming = 0;//タイミングはステップのゼロ
    }
    else
    {
      if (ShDt->tJSh_f)
      {
        s32 timing = GetAirShootTiming();

        if (st_.pstMyCh_->Motion.IsMFlags(dbmfDs) && IsInfield())
        {
          if (timing == NGNUM)
          {
            ShDt->sType = shtDJS;
          }
          else if (st_.pstMyCh_->stGousei.set_f_[timing])
          {
            ShDt->sType = shtJHS;
            ShDt->HSTiming = timing;
          }
          else
          {
            ShDt->sType = shtDJS;
          }
        }
        else
        {
          if (st_.pstMyCh_->stGousei.vjset_f_ && (timing == VJHSTIMING))
          {
            ShDt->sType = shtJHS;
            ShDt->HSTiming = NGNUM;
          }
          else
          {
            ShDt->sType = shtJS;
          }
        }
      }
      else
      {

        if (st_.pstMyCh_->Motion.IsMFlags(dbmfDs) && IsInfield())//ダッシュ系（内野のみ）
        {
          s32 timing = GetDashShootTiming();
          if (timing == NGNUM)
          {
            ShDt->sType = shtDS;
          }
          else if (st_.pstMyCh_->stGousei.set_f_[timing])
          {
            ShDt->sType = shtHS;
            ShDt->HSTiming = timing;
          }
          else
          {
            ShDt->sType = shtDS;
          }
        }
      }
    }



    //ライン超え振り向きＤＪシュートもあるので左コートに居るときは右向きのみ
    BOOL mukiOK_f = ((st_.mysideNo_ == 0) && (st_.pstMyCh_->Zahyou.Muki == mR))
      || ((st_.mysideNo_ == 1) && (st_.pstMyCh_->Zahyou.Muki == mL));

    //外野なら向き問わない
    mukiOK_f |= (IsInfield() == FALSE);


    if ((mukiOK_f == FALSE) //むきちがう
      || (pmgSG_->stBa_.ShTgPNo == NGNUM)
      || (st_.pmgRf_->IsCapOnlyCtrl() && (st_.mysideNo_ == SIDE1)))//たげいない
    {
      //ノーマルシュートにリセット
      if (ShDt->tJSh_f)
      {
        ShDt->sType = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
          ? shtDJS
          : shtJS;
      }
      else
      {
        ShDt->sType = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
          ? shtDS
          : shtS;
      }
    }
  }

  //必殺以外のシュートスピード
  void TChCommon::GetShSpeed(TShotInfo* ShDt)
  {
    s32 tShRate = 0;

    switch (ShDt->sType)
    {
    case shtJS:
    case shtJNS:
      tShRate = pmgEO_->mgDt_.dtSet_.GetDtShot(setJumpShotRate);
      break;
    case shtDS:
    case shtDNS:
      tShRate = pmgEO_->mgDt_.dtSet_.GetDtShot(setDashShotRate);
      break;
    case shtDJS:
    case shtDJNS:
      tShRate = pmgEO_->mgDt_.dtSet_.GetDtShot(setDashJumpShotRate);
      break;
    default:
      tShRate = pmgEO_->mgDt_.dtSet_.GetDtShot(setStandShotRate);
      break;
    }

    switch (ShDt->sType)
    {
    case shtJNS:
    case shtDNS:
    case shtDJNS:
      ShDt->tNice_f = TRUE;//ナイス
      break;
    }

    ShDt->tShPow = lib_num::Percent(ShDt->tShPow, tShRate);
    ShDt->tShTech = lib_num::Percent(ShDt->tShTech, tShRate);


  }


  //新シュートソート
  void TChCommon::SortShot(s32 timing)
  {
    //原料に入ってる素材
    INTVECTOR2 elms2;
    elms2.clear();

    s32 elemnum = 0;
    s32 prt_init = 0;
    s32 prt_term = 0;
    s32 ELE2 = 2;//合成二回目まで


    TStChShotElems& elms = (timing == NGNUM)
      ? (st_.pstMyCh_->stGousei.vjshot_)
      : (st_.pstMyCh_->stGousei.shot_[timing]);

    //pmgSG_->stSh_.idata.shotId =

#ifdef __K_DEBUG_SHIAI__
    // 合成シュートデバッグモードじゃなければNULLが入ってるはず
    kdebug::DebugSystem* pDs = kdebug::DebugSystem::GetInstance();
    if (pDs->IsMixShootMode())
    {
      // 最初のID、アタック、スピンはスキップしておく
      pmgSG_->stSh_.idata.shotId = pDs->GetMixShootParam(db::gs_shot_syn_mid);	// シュートID上書きしちゃう
    }
#endif	// #ifdef __K_DEBUG_SHIAI__

    //合成数
    for (s32 i = 0; i < HSELEMMAX; ++i)
    {
#ifdef __K_DEBUG_SHIAI__
      // ここで好きな値を入れると合成シュートでいろんなことができる！
      s32 elem, rarity;
      if (pDs->IsMixShootMode())
      {
        elem = pDs->GetMixShootParam((db::enDtShSyn)(db::gs_base + (i * 2)));

        // このままではだめなのでキーに変える
        elem = pmgEO_->mgDt_.dtShElm_.GetKey(elem);

        //これ以上は合成されてない
        if (elem == -1) break;

        ++elemnum;
        rarity = pDs->GetMixShootParam((db::enDtShSyn)(db::gs_base_rare + (i * 2)));
      }
      //--------------------------------------------------------------------------------
      // 従来の処理
      else
      {
        //-----------------------------------
        // 元ソース部分
        ///*s32*/ elem = st_.pstMyCh_->stGousei.shot_[timing].elem_[i];
        elem = elms.elem_[i];
        //これ以上は合成されてない
        if (elem == 0) break;

        ++elemnum;

        ///*s32*/ rarity = st_.pstMyCh_->stGousei.shot_[timing].rare_[i];
        rarity = elms.rare_[i];
      }
#else
      //s32 elem = st_.pstMyCh_->stGousei.shot_[timing].elem_[i];
      s32 elem = elms.elem_[i];
      //これ以上は合成されてない
      if (elem == 0) break;

      ++elemnum;
      //s32 rarity = st_.pstMyCh_->stGousei.shot_[timing].rare_[i];
      s32 rarity = elms.rare_[i];
#endif	// #ifdef __K_DEBUG_SHIAI__

      //原料に入ってる素材
      INTVECTOR elms;
      elms.clear();


      s32 opennum = pmgEO_->mgDt_.dtShElm_.GetDt(elem, (enDtShElm)(sz_lock_rare1 + rarity));// rarityはゼロスタートになっているか確認
      if (opennum == 0)//ロック０のときは全て解放
      {
        opennum = HSATMMAX;
      }

      s32 syn_init = (pmgEO_->mgDt_.dtShElm_.GetDt(elem, sz_syn_init));
      s32 syn_term = (pmgEO_->mgDt_.dtShElm_.GetDt(elem, sz_syn_term));

      for (s32 i2 = 0; i2 < opennum; ++i2)
      {
        //封印チェック
        s32 atm = pmgEO_->mgDt_.dtShElm_.GetDt(elem, (enDtShElm)(sz_elem1 + i2));
        //空だったらそこで終わり
        if (atm == 0) break;

        //合成２回目までは限定される
        if ((i < ELE2) && (syn_init != 0))
        {
          //合成開始、合成終了に入っていたら追加する
          if ((i2 >= syn_init) && (i2 <= syn_term))
          {
            elms.push_back(atm);
          }
        }
        else
        {
          elms.push_back(atm);
        }
      }
      //ベースの保護領域
      prt_init = pmgEO_->mgDt_.dtShElm_.GetDt(elem, (enDtShElm)(sz_prt_init));
      prt_term = pmgEO_->mgDt_.dtShElm_.GetDt(elem, (enDtShElm)(sz_prt_term));
      //未ソート各合成素材
      elms2.push_back(elms);
    }

    //pmgSG_->stSh_.sdata.nonesort_elms = elms2;
    pmgSG_->stSh_.sdata.prt_init = prt_init;
    pmgSG_->stSh_.sdata.prt_term = prt_term;

    //ソート
    pmgSG_->stSh_.sdata.SortElem(elms2);
    //形状と効果の抽出
    pmgSG_->stSh_.SortElem_ShpEff(&pmgEO_->mgDt_.dtElem_);
  }

  //新シュート
  void TChCommon::SetShoot(BOOL tJSh_f)
  {
    //攻撃時間制限で手放された
    if (IsBall() == FALSE) return;

    //★もう一度ここでタゲをとりなおしてみる
    ShTagSet(FALSE);
    //敵チーム
    st_.pmgEnTm_->SetCtrl(pmgSG_->stBa_.ShTgPNo);//タゲにカーソル移動
    //st_.pmgEnTm_->ResetHitStop();//敵全員のヒットストップリセット
    st_.pmgBa_->baCommon_.SetMtype(bmShoot);//クリア処理

    TShotInfo ShDt;
    ShDt.Init();
    ShDt.tJSh_f = tJSh_f;
    ShDt.tDSh_f = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs) && IsInfield());

    ShDt.tShRank = MyStLv(dbst_Level);
    ShDt.tShPow = MyStLv(dbst_Pow);
    ShDt.tShTech = MyStLv(dbst_Tech);

    if (pmgEO_->stShiai_.IsPrev())
    {
      ShDt.tShPow = XYMAG;
      ShDt.tShTech = XYMAG;
    }


    ShDt.tShSp = pmgEO_->mgDt_.dtRank_.GetRankDtLevel(rkBallSpd, MyStLv(dbst_Level));

    //シュート種類
    GetShotType(&ShDt);

    ////敵のコートの方向を向いているか
    //BOOL enMuki_f = ((st_.mysideNo_ == 0) && (st_.pstMyCh_->Zahyou.Muki == mR))
    //  || ((st_.mysideNo_ == 1) && (st_.pstMyCh_->Zahyou.Muki == mL));

    pmgSG_->stSh_.Init();


#ifdef __K_DEBUG_SHIAI__
    // プレイヤーの撃つシュートは全て必殺にしてしまう！
    // ※ここもあとででばっぐで好きに設定できるようにする※
    if (st_.mysideNo_ == 0)
    {
      // プレイヤーの撃つシュートは全て必殺にしてしまう！
      if (kdebug::DebugSystem::GetInstance()->IsPlayerEveryMixShoot())
      {
        if (ShDt.tJSh_f){ ShDt.sType = shtJHS; }
        else { ShDt.sType = shtHS; }
        ShDt.HSTiming = 1;
      }
    }
    // 逆に敵には必殺シュートうたせない！
    else
    {
      // 敵も毎回必殺シュート
      if (kdebug::DebugSystem::GetInstance()->IsEnemyEveryMixShoot())
      {
        if (ShDt.tJSh_f){ ShDt.sType = shtJHS; }
        else { ShDt.sType = shtHS; }
        ShDt.HSTiming = 1;
      }
      // 必殺うたない
      else
      {
        if (ShDt.tJSh_f){ ShDt.sType = shtJS; }
        else { ShDt.sType = shtS; }
      }
    }
#endif	// #ifdef __K_DEBUG_SHIAI__
    //    st_.pmgBa_->baCommon_.SetMtype(bmShoot);
    //シュート種類
    switch (ShDt.sType)
    {
    case shtHS:
    case shtJHS:
      ShDt.HS_f = TRUE;
      //GetHShSpeed(&ShDt);//必殺
      SortShot(ShDt.HSTiming);
      break;
    default:
      GetShSpeed(&ShDt);//通常、ナイス
      break;
    }



    if (ShDt.HS_f)
    {
      TStChShotElems& elms = (ShDt.HSTiming == NGNUM)
        ? (st_.pstMyCh_->stGousei.vjshot_)
        : (st_.pstMyCh_->stGousei.shot_[ShDt.HSTiming]);

      ShDt.tShPow = (ShDt.tShPow * elms.pow_) / XYMAG;
      ShDt.tShTech = (ShDt.tShTech * elms.spin_) / XYMAG;
      ShDt.orishno = elms.oriNameNo_;
      pmgSG_->stSh_.idata.shotId = elms.shotId;
    }
    else
    {
      ShDt.orishno = 0;
    }
    //idataに初期情報を渡す
    pmgSG_->stSh_.idata.sp_f = ShDt.HS_f;
    pmgSG_->stSh_.idata.spin = RankLevel(rkGrpSpin);
    pmgSG_->stSh_.idata.nice_f = ShDt.tNice_f;
    pmgSG_->stSh_.idata.counter_f = pmgSG_->stBa_.counter_f;
    pmgSG_->stSh_.idata.jsh_f = tJSh_f;
    pmgSG_->stSh_.idata.dsh_f = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs) && IsInfield());
    pmgSG_->stSh_.idata.side = st_.mysideNo_;
    pmgSG_->stSh_.idata.frmPos = pmgSG_->stBa_.PichPNo;
    pmgSG_->stSh_.idata.pow = ShDt.tShPow;
    pmgSG_->stSh_.idata.sharp = ShDt.tShTech;
    pmgSG_->stSh_.idata.rank = ShDt.tShRank;
    pmgSG_->stSh_.idata.roll_f = (ShDt.tShPow >= pmgEO_->mgDt_.dtSet_.GetDtShot(setRollPow));
    //    st_.pmgBa_->baCommon_.SetMtype(bmShoot);
    pmgSG_->stBa_.NoHit_c = (tJSh_f)
      ? 0
      : pmgEO_->mgDt_.dtSet_.GetDtShot(setHitStartTime);
    //pmgSG_->stBa_.HitStop_c = 0;//ヒットストップ★
    //idata.iballの初期設定★
    //※distを求める際 distX = tagposX - posX とし、絶対値は取らないでOKです。distY、Zも同様です。
    pmgSG_->stSh_.idata.iball.posX = (pmgSG_->stBa_.Zahyou.X / XYMAG);
    pmgSG_->stSh_.idata.iball.posY = (pmgSG_->stBa_.Zahyou.Y / XYMAG);
    pmgSG_->stSh_.idata.iball.posZ = (pmgSG_->stBa_.Zahyou.Z / XYMAG);
    pmgSG_->stSh_.idata.iball.spd = (f32)pmgEO_->mgDt_.dtSet_.GetDtShot(setNormalShotSpd) / (f32)XYMAG;//ノーマルシュート速度

    //とりあえず強制終了回避
    if (pmgSG_->stBa_.ShTgPNo < 0)
    {
      pmgSG_->stBa_.ShTgPNo = 0;
    }
    pmgSG_->stSh_.idata.iball.tagID = pmgSG_->stBa_.ShTgPNo;
    TZahyouRec* tagzahyou = &(st_.pmgEnTm_->st_.pmgMyCh_[pmgSG_->stBa_.ShTgPNo]->st_.pstMyCh_->Zahyou);
    pmgSG_->stSh_.idata.iball.tagposX = (tagzahyou->X / XYMAG);
    pmgSG_->stSh_.idata.iball.tagposY = (tagzahyou->Y / XYMAG);
    pmgSG_->stSh_.idata.iball.tagposZ = (tagzahyou->Z / XYMAG);

    //初期向き
    //標的・球の差から初期の向きを求める
    ////波軌道終了後、波軌道で使っていた「主軌道からの乖離値」を主軌道に埋め込むとき
    ////主軌道移動がなかった場合、加算減算のジャッジに使う
    f32 defBalltoTarg = pmgSG_->stSh_.idata.iball.tagposX - pmgSG_->stSh_.idata.iball.posX;
    pmgSG_->stSh_.idata.iball.initDirX = 1.0f;
    if (defBalltoTarg != 0.0f)
    {//０でないなら初期向きは標的・球距離でOk
      pmgSG_->stSh_.idata.iball.initDirX = defBalltoTarg / fabs(defBalltoTarg);
    }
    else
    {//０なら初期向きはどっちチームかに依存。左チームなら右へ投げ、右チームなら左へ
      //ただし外野シュートには対応してないので注意
      if (pmgSG_->stSh_.idata.side != 0) pmgSG_->stSh_.idata.iball.initDirX = -1.0f;
    }

    //初期Zシュートチェック
    pmgSG_->stSh_.idata.initZShot_f = FALSE;
    f32 distX_a = fabs(pmgSG_->stSh_.idata.iball.tagposX - pmgSG_->stSh_.idata.iball.posX);
    f32 distZ_a = fabs(pmgSG_->stSh_.idata.iball.tagposZ - pmgSG_->stSh_.idata.iball.posZ);
    if (distX_a < distZ_a) pmgSG_->stSh_.idata.initZShot_f = TRUE;


    //外野シュート判別用フラグ　
    pmgSG_->stSh_.cmn.shotFromNorthOF_f = FALSE;//北側外野ポジションシュートフラグ
    pmgSG_->stSh_.cmn.shotFromSouthOF_f = FALSE;//南側外野ポジションシュートフラグ
    pmgSG_->stSh_.cmn.shotNSOF_f = FALSE;//上記２フラグどちらかが立っていたら立てるフラグ
    if (pmgSG_->stBa_.PichPNo == 4) pmgSG_->stSh_.cmn.shotFromNorthOF_f = TRUE;
    if (pmgSG_->stBa_.PichPNo == 5) pmgSG_->stSh_.cmn.shotFromSouthOF_f = TRUE;
    if (
      (pmgSG_->stSh_.cmn.shotFromNorthOF_f == TRUE)
      ||
      (pmgSG_->stSh_.cmn.shotFromSouthOF_f == TRUE)
      )
      pmgSG_->stSh_.cmn.shotNSOF_f = TRUE;//上記２フラグどちらかが立っていたら立てる

#ifdef __K_DEBUG_SHIAI__
    // 投げ手のチームで判断するようにします
    // st_.mysideNo_
    // が左チーム０右チーム１なので
    // デバッグターゲットに１のとき入るようにすればだいじょぶだとおもいます
    // by いしださん
    if (st_.mysideNo_ == 1)
    {
      // 敵のターゲット位置フリーカーソルモードじゃなければNULLが入ってるはず
      kdebug::DebugSystem* pDs = kdebug::DebugSystem::GetInstance();
      if (pDs->IsFreeTargetPosMode())
      {
        pmgSG_->stSh_.idata.iball.tagposX = pDs->GetFreeTargetPosX();
        pmgSG_->stSh_.idata.iball.tagposY = pDs->GetFreeTargetPosY();
        pmgSG_->stSh_.idata.iball.tagposZ = pDs->GetFreeTargetPosZ();
      }
    }
#endif	// #ifdef __K_DEBUG_SHIAI__

    pmgSG_->stSh_.idata.iball.distX = (pmgSG_->stSh_.idata.iball.tagposX - pmgSG_->stSh_.idata.iball.posX);
    pmgSG_->stSh_.idata.iball.distY = (pmgSG_->stSh_.idata.iball.tagposY - pmgSG_->stSh_.idata.iball.posY);
    pmgSG_->stSh_.idata.iball.distZ = (pmgSG_->stSh_.idata.iball.tagposZ - pmgSG_->stSh_.idata.iball.posZ);

    //iballをphsに写す
    pmgSG_->stSh_.phs = pmgSG_->stSh_.idata.iball;
    pmgSG_->stSh_.phs.spdDecFromWave = 1.0f;
    //必殺シュートなら素材を移す
    if ((ShDt.sType == shtHS) || ShDt.sType == shtJHS)
    {
      //必殺シュートフラグON
      pmgSG_->stSh_.idata.iball.spShot_f = TRUE;

      //現段階のsdataからElmに移動
      pmgSG_->stSh_.RemoveSdataToElm(&pmgEO_->mgDt_.dtElem_);
      //地上変化素材があるかのチェック
      pmgSG_->stSh_.cmn.groundElm_f = pmgSG_->stSh_.sdata.groundElm_f;

      //標的素材Elm⇒Crt
      pmgSG_->stSh_.RemoveElmToCrt();
      //速度決定…まずはデフォ、速度＋、速度－素材で増減
      //pmgEO_->mgDt_.dtSet_.GetDtShot(setHissatsuShotSpd) / (f32)XYMAG;
      //デフォ速度を入れる
      pmgSG_->stSh_.cmn.defSpd = (f32)pmgEO_->mgDt_.dtSet_.GetDtShot(setHissatsuShotSpd) / (f32)XYMAG;
      //速度＋素材  速度－素材
      s32 speedTotalLv = pmgSG_->stSh_.idata.eff.speedInc - pmgSG_->stSh_.idata.eff.speedDec;
      pmgSG_->stSh_.phs.spdLv = speedTotalLv;

      //とりあえず速度加算データ
      enRankLevel rankSwitch = rkBallSpdInc;

      //速度減算データ→読み込みデータは速度マイナス用、Lvはー１を掛けて正にする
      if (speedTotalLv < 0)  rankSwitch = rkBallSpdDec, speedTotalLv = speedTotalLv * (-1);
      //速度加算or減算があったらデフォ速度を更新する
      if (speedTotalLv != 0)
      {
        f32 rankSpd = pmgEO_->mgDt_.dtRank_.GetRankDtLevel(rankSwitch, speedTotalLv);
        f32 spdSign = (f32)pmgSG_->stSh_.phs.spdLv / fabs(pmgSG_->stSh_.phs.spdLv);
        pmgSG_->stSh_.idata.iball.spd = pmgSG_->stSh_.cmn.defSpd + (rankSpd * spdSign) / XYMAG;
      }
      else pmgSG_->stSh_.idata.iball.spd = pmgSG_->stSh_.cmn.defSpd;
      pmgSG_->stSh_.phs.spd = pmgSG_->stSh_.idata.iball.spd;

      //速度追加、減少の効果素材があるかもなので
      //効果素材のチェックを行う
      pmgSG_->stSh_.SetEffectElmFlag();//効果素材初期処理
      pmgSG_->stSh_.CheckEffectElmFlag();//効果素材段階チェック
      pmgSG_->stSh_.cmn.hShot_f = TRUE;
      //速度レベル
      //      s32 spdLv = pmgSG_->stSh_.phs.spdLv;
      s32 spdLv = pmgSG_->stSh_.phs.spdLv + pmgSG_->stSh_.phs.spdMomLv;
      //Lv０のときのデフォ速度
      pmgSG_->stSh_.phs.spd = pmgSG_->stSh_.cmn.defSpd;
      //Lv０以外のとき
      if (spdLv != 0)
      {
        //とりあえず速度加算データ
        enRankLevel rankSwitch = rkBallSpdInc;

        //加算or減算
        f32 spdSign = spdLv / abs(spdLv);

        //速度減算データ→読み込みデータは速度マイナス用、Lvはー１を掛けて正にする
        if (spdLv < 0)  rankSwitch = rkBallSpdDec, spdLv = spdLv * (-1);
        //
        f32 rkSpd = pmgEO_->mgDt_.dtRank_.GetRankDtLevel(rankSwitch, spdLv);
        //速度加算or減算があったらデフォ速度を更新する
        pmgSG_->stSh_.phs.spd = pmgSG_->stSh_.phs.spd + (spdSign * rkSpd) / XYMAG;
        if (pmgSG_->stSh_.phs.spd <= 0.5f) pmgSG_->stSh_.phs.spd = 0.5f;
      }

    }

    //現在の速度位置を保存用データに入れる
    pmgSG_->stSh_.cmn.prsPX = st_.pstBa_->Zahyou.X;//現在の位置X
    pmgSG_->stSh_.cmn.prsPY = st_.pstBa_->Zahyou.Y;//現在の位置Y
    pmgSG_->stSh_.cmn.prsPZ = st_.pstBa_->Zahyou.Z;//現在の位置Z
    pmgSG_->stSh_.cmn.prsSX = st_.pstBa_->Zahyou.dX;//現在の速度X
    pmgSG_->stSh_.cmn.prsSY = st_.pstBa_->Zahyou.dY;//現在の速度Y
    pmgSG_->stSh_.cmn.prsSZ = st_.pstBa_->Zahyou.dZ;//現在の速度Z
    pmgSG_->stSh_.cmn.prsTX = tagzahyou->X;//現在の敵位置X
    pmgSG_->stSh_.cmn.prsTY = tagzahyou->Y;//現在の敵位置Y
    pmgSG_->stSh_.cmn.prsTZ = tagzahyou->Z;//現在の敵位置Z
    pmgSG_->stSh_.cmn.stopPstData_f = FALSE;

    //主軌道計算
    pmgSG_->stSh_.SAngle();
    //二次・三角関数初期化フラグをオフ
    pmgSG_->stSh_.idata.initAccTri_f = FALSE;

    //ボール形状
    pmgSG_->stSh_.InitShapeIdLv();//ID,Lvの初期化
    pmgSG_->stSh_.InitShapeData(&pmgEO_->mgDt_.dtBaAni_, &pmgEO_->mgDt_.dtElem_);

    //    st_.pmgBa_->baCommon_.SetMtype(bmShoot);

    //投げ終わり隙カウンタ
    const s32 QSHCAREVTIME = 40;
    st_.pmgMyTm_->st_.pstMyTm_->COMDt.shAfter_c = QSHCAREVTIME;

    //タイマーリセット
    if ((pmgSG_->stBa_.ShTgPNo != NGNUM)
      && (pmgSG_->stBa_.ShTgPNo <= (s32)dbpoI3))
    {
      st_.pmgBa_->baCommon_.ResetTimer(NGNUM, TRUE);
    }

    //効果音
    //    if (ShDt.HS_f) st_.pmgBa_->baCommon_.HomingSESet(seHShot);
    //    else if (ShDt.tNoTagHS_f) st_.pmgBa_->baCommon_.HomingSESet(seHShot);
    //    else if (ShDt.tNice_f || pmgSG_->stBa_.counter_f) st_.pmgBa_->baCommon_.HomingSESet(seNShot);
    //    else st_.pmgBa_->baCommon_.HomingSESet(seShot);

    //マリオネット効果を持っていたら標的にフラグを渡してしまう。ChActionで色々やる
    if (pmgSG_->stSh_.cmn.efEl.symCtrl_f == TRUE)
    {
        st_.pmgEnTm_->st_.pmgMyCh_[pmgSG_->stBa_.ShTgPNo]->st_.pstMyCh_->symCtrl_f = TRUE;
//        st_.pmgEnTm_->st_.pmgMyCh_[pmgSG_->stBa_.ShTgPNo]->st_.pstMyCh_->Zahyou.Y = 1 * XYMAG;
    }
    if (ShDt.HS_f)//必殺フラグ
    {
      //必殺シュートの音をデフォで入れておく
      enSEID seID = seHShot;
      //０段階目に鳴る効果音があればその番号の音IDをセットする
      if ((pmgSG_->stSh_.cmn.efEl.sePhase.phase_f == TRUE) && (pmgSG_->stSh_.cmn.efEl.sePhase.incld_f == TRUE))
      {
        s32 zero_f = 1;
        if (pmgSG_->stSh_.idata.eff.sePhase_Num[0] == 100) zero_f = 0;
        seID = (enSEID)((pmgSG_->stSh_.GetSENum(pmgSG_->stSh_.idata.eff.sePhase_Num[0] + 1)) * zero_f);
      }
      else if (pmgSG_->stSh_.crt.trg.backward_Time != 0) seID = (enSEID)36;//逆進中はため音

      //上で音の変更無く、１段階しかなく、最終音フラグが立っていたらならす
      if ((seID == seHShot) && (pmgSG_->stSh_.cmn.phaseAll == 0) && (pmgSG_->stSh_.cmn.efEl.finalSE_f == TRUE))
      {
        //ファイナルSEあればそれを鳴らす。
        s32 zero_f = 1;
        if (pmgSG_->stSh_.idata.eff.finalSE == 100) zero_f = 0;

        seID = (enSEID)((pmgSG_->stSh_.idata.eff.finalSE + 1) * zero_f);
      }

      if (seID != 36) st_.pmgBa_->baCommon_.HomingSESet(seID);
      else st_.pmgBa_->baCommon_.SESet(seID);
    }
    else if (ShDt.tNoTagHS_f) st_.pmgBa_->baCommon_.HomingSESet(seHShot);
    else if (ShDt.tNice_f || pmgSG_->stBa_.counter_f) st_.pmgBa_->baCommon_.HomingSESet(seNShot);
    else st_.pmgBa_->baCommon_.HomingSESet(seShot);


    //IOS
    if (mid::midIsTBL())
    {
      if (ShDt.HS_f) //ひっさつ
      {
        // プレビュー時は素材全て表示
        if (pmgSG_->stRf_.Prev_f)
        {
          pmgGO_->pmgCommon_->ResetPopupMsg();

          for (s32 i = 0; i < 8; i++)
          {
            std::string hsname = "";
            s64 sozai = st_.pstMyCh_->stGousei.shot_[0].elem_[i];
            if (sozai <= 0){ break; }
            hsname = pmgEO_->mgDt_.dtShElm_.GetDtStr(sozai, szs_name);//素材名

            pmgGO_->pmgCommon_->PopupMsgDelay(pu_Act, hsname.c_str(),
              st_.mysideNo_, st_.pstMyCh_->Zahyou.X + 1200, st_.pstMyCh_->Zahyou.Y, st_.pstMyCh_->Zahyou.Z/* + (i * -1200)*/, FC2_shiai, (i * 40), FALSE);
          }
          return;
        }

        BOOL syn_f = (ShDt.HSTiming == NGNUM)
          ? st_.pstMyCh_->stGousei.vjsyn_f_//垂直ジャンプ
          : st_.pstMyCh_->stGousei.syn_f_[ShDt.HSTiming];
        //素材名
        std::string hsname = "";
        if (syn_f)
        {
          hsname = pmgEO_->mgDt_.dtShSyn_.GetName(pmgSG_->stSh_.idata.shotId);//合成名
        }
        else
        {
          if (ShDt.orishno != 0)
          {
            hsname = pmgEO_->mgDt_.dtShSyn_.GetOriShName(ShDt.orishno);//オリジナル名
          }
          else
          {
            hsname = pmgEO_->mgDt_.dtShElm_.GetDtStr(pmgSG_->stSh_.idata.shotId, szs_name);//素材名
          }
        }

        pmgGO_->pmgCommon_->PopupMsg(pu_Act, hsname.c_str(),
          st_.mysideNo_, st_.pstMyCh_->Zahyou.X, st_.pstMyCh_->Zahyou.Y, st_.pstMyCh_->Zahyou.Z, FC2_shiai, FALSE);
      }
    }
    else//Windows
    {
      // プレビュー時は素材全て表示
      if (pmgSG_->stRf_.Prev_f)
      {
        pmgGO_->pmgCommon_->ResetPopupMsg();

        for (s32 i = 0; i < 8; i++)
        {
          std::string hsname = "";
          s64 sozai = st_.pstMyCh_->stGousei.shot_[0].elem_[i];
          if (sozai <= 0){ break; }
          hsname = pmgEO_->mgDt_.dtShElm_.GetDtStr(sozai, szs_name);//素材名

          pmgGO_->pmgCommon_->PopupMsgDelay(pu_Act, hsname.c_str(),
            st_.mysideNo_, st_.pstMyCh_->Zahyou.X + 1200, st_.pstMyCh_->Zahyou.Y, st_.pstMyCh_->Zahyou.Z/* + (i * -1200)*/, FC2_shiai, (i * 40), FALSE);
        }
        return;
      }

      //プレビューでエラー出るのでちょっといったん消す
      ////合成名
      //std::string hsname = pmgEO_->mgDt_.dtShSyn_.GetName(pmgSG_->stSh_.idata.shotId);//まだ仮
      //pmgGO_->pmgCommon_->PopupMsg(pu_Act, hsname.c_str(), st_.mysideNo_, st_.pstMyCh_->Zahyou.X, st_.pstMyCh_->Zahyou.Y, st_.pstMyCh_->Zahyou.Z, FC2_shiai, FALSE);
    }
  }
  //距離セット
  void TChCommon::SetTagDist(TPassInfo* PaDt, s32 ttx, s32 tty, s32 ttz)
  {
    //タゲまでの距離
    PaDt->tdx = abs(ttx - pmgSG_->stBa_.Zahyou.X);//st_.pstMyCh_->Zahyou.X
    PaDt->tdz = abs(ttz - pmgSG_->stBa_.Zahyou.Z);
    PaDt->tdy = abs(tty - st_.pstMyCh_->Zahyou.Y);

    PaDt->tdys = (tty >= st_.pstMyCh_->Zahyou.Y)
      ? +1
      : -1;

    PaDt->tdxs = (ttx >= pmgSG_->stBa_.Zahyou.X)
      ? +1
      : -1;

    PaDt->tdzs = (ttz >= pmgSG_->stBa_.Zahyou.Z)
      ? +1
      : -1;
  }
  //パスタゲまでの距離★★
  void TChCommon::GetPassTagDist(TPassInfo* PaDt)
  {
    const s32 GOSA = (4 * XYMAG);

    //タゲのショートカット
    TMgChar* tgP = st_.pmgMyTm_->st_.pmgMyCh_[pmgSG_->stBa_.PaTgPNo];

    //パスのライン
    if (((st_.posNo_ == (s32)dbpoO2) || (st_.posNo_ == (s32)dbpoO3))
      && ((tgP->st_.posNo_ == (s32)dbpoO2) || (tgP->st_.posNo_ == (s32)dbpoO3)))
    {
      PaDt->tPaLine = pl23;
    }
    else if (IsInfield() && (tgP->st_.posNo_ <= (s32)dbpoI3))
    {
      PaDt->tPaLine = pl11;
    }
    else if ((IsInfield() || (st_.posNo_ == (s32)dbpoO4))
      && (tgP->IsInfield() || (tgP->st_.posNo_ == (s32)dbpoO4)))
    {
      PaDt->tPaLine = pl14;
    }
    else
    {
      PaDt->tPaLine = plNml;
    }

    //タゲの座標
    s32 ttx = tgP->st_.pstMyCh_->Zahyou.X;
    s32 ttz = tgP->st_.pstMyCh_->Zahyou.Z;
    s32 tty = tgP->st_.pstMyCh_->Zahyou.Y;

    //最後の一人のときは戻る位置を狙う
    if (st_.pmgMyTm_->IsLastOne()
      && (IsInfield() == FALSE)
      && (tgP->IsInfield())
      && (tgP->IsDashman() == FALSE))
    {
      if ((tgP->IsFreeMotion(TRUE) == FALSE) || st_.pstMyCh_->Motion.IsMFlags(dbmfAr))
      {
        ttx = (st_.mysideNo_ == 0)
          ? lib_num::Between(tgP->st_.pstMyCh_->Zahyou.X, (DBCRT_SL + GOSA), (DBCRT_CL - GOSA))
          : lib_num::Between(tgP->st_.pstMyCh_->Zahyou.X, (DBCRT_SL2 - GOSA), (DBCRT_CL + GOSA));

        ttz = lib_num::Between(tgP->st_.pstMyCh_->Zahyou.Z, (DBCRT_FL + GOSA), (DBCRT_BL - GOSA));
      }
    }

    //対角線外野近すぎトスパスは捕れないことがある
    if (PaDt->tPaLine == plNml)
    {
      SetTagDist(PaDt, ttx, 0, ttz);
      s32 dist = (s32)lib_num::Hypot(PaDt->tdx, PaDt->tdz) / XYMAG;
      if (dist < (pmgEO_->mgDt_.dtSet_.GetDtPass(setNearDistDef) / 2))
      {
        PaDt->Quick_f = TRUE;//速攻に
      }
    }

    PaDt->tdm_f = tgP->IsDashman();//タゲがダッシュマン★
    PaDt->Late_f = FALSE;

    //タゲの移動速度★偏差射撃でのみ使ってる
    PaDt->tvx = tgP->st_.pstMyCh_->Zahyou.dX;
    PaDt->tvy = tgP->st_.pstMyCh_->Zahyou.dY;
    PaDt->tvz = tgP->st_.pstMyCh_->Zahyou.dZ;
    PaDt->tvz_time = NGNUM;

    PaDt->alleyoop = 0;
    PaDt->ArriveTime = 0;
    PaDt->tgrv = pmgEO_->mgDt_.dtSet_.GetDtGrv(setGRV_Y);
    PaDt->stopDm_f = FALSE;

    //タゲがダッシュマンではない
    if (PaDt->tdm_f == FALSE)
    {
      //内野間トスパス
      if ((PaDt->Quick_f == FALSE) && (PaDt->tPaLine == pl11))
      {
        s32 dXSign = (st_.mysideNo_ == 0)
          ? +1
          : -1;

        //移動速度をジャンプ速度に
        s32 jvx = 0;//dXSign * (lib_num::Percent(tgP->Rank(rkomNDSJPX), dxper));
        s32 jvy = pmgEO_->mgDt_.dtSet_.GetDtJump(setJump_vY0);//pmgEO_->mgDt_.dtSet_.GetDtJump(setDashJump_vY0);

        //速度
        s32 dvx = dXSign * (tgP->RankSpeed(rkWkX));

        const s32 CRTIME = 4;//ジャンプ前のしゃがみ
        //パススピード
        s32 paspd = pmgEO_->mgDt_.dtSet_.GetDtPass(setDMPaSpd);

        //とりあえずポストマンの位置
        const s32 TAGX = POSPOST_X;

        //移動先
        s32 tgx = (st_.mysideNo_ == 0)
          ? (TAGX)
          : (DBCRT_W - TAGX);

        //仮
        s32 tgz = tgP->st_.pstMyCh_->Zahyou.Z;

        //ウォークマンがジャンプラインに到達するまでの距離
        s32 jumpDistX = (TAGX - tgP->GetLeftCrtX());

        //ウォークしている時間
        s32 timedash = abs(jumpDistX / dvx);

        //ウォークマンがジャンプラインに到達するまでの時間 + ジャンプしゃがみ時間
        s32 timedjl = (timedash + CRTIME);

        //ボールマンからジャンプラインまでの距離
        s32 jlx = abs(tgx - st_.pstMyCh_->Zahyou.X);
        s32 jlz = abs(tgz - st_.pstMyCh_->Zahyou.Z);
        f32 jlh = lib_num::Hypot(jlx, jlz);

        //ジャンプラインに速攻パスを出したときに掛かる時間
        s32 timepjl = ((s32)jlh / paspd);

        s32 tct = pmgEO_->mgDt_.dtSet_.GetDtPass(setDsmnTossCatchTime);
        s32 tal = abs(jumpDistX / dvx) + CRTIME + tct;//アリウープパス時間

        //タゲまでの距離
        SetTagDist(PaDt, ttx, 0, ttz);

        if (tal < pmgEO_->mgDt_.dtSet_.GetDtPass(setAlleyoopTime))
        {
          tal = pmgEO_->mgDt_.dtSet_.GetDtPass(setAlleyoopTime);//最低時間
        }


        PaDt->alleyoop = tal;
        //移動先
        tty = (jvy * tct) - (PaDt->tgrv * tct * tct / 2);//地上から決まった距離★
        ttx = tgx + (jvx * tct);
        ttz = tgz;

        //タゲまでの距離
        SetTagDist(PaDt, ttx, tty, ttz);

      }
      else
      {
        //if (PaDt->Quick_f )//速攻（内野間は必ず）
        if (PaDt->Quick_f || (PaDt->tPaLine == pl11))
        {

          //空中にいる
          if (tgP->st_.pstMyCh_->Zahyou.Y > 0)
          {
            PaDt->tdm_f = TRUE;//ダッシュマン扱いにしてみるか

            PaDt->Quick_f = TRUE;
            tty = tgP->st_.pstMyCh_->Zahyou.Y;
          }
          else
          {
            PaDt->Quick_f = TRUE;
            tty = tgP->st_.pstMyCh_->Zahyou.Y
              + (pmgEO_->mgDt_.dtSet_.GetDtPass(setQuickPassTagY) * XYMAG);
            PaDt->tgrv = 0;//地上
          }
        }
        else
        {
          tty = pmgEO_->mgDt_.dtSet_.GetDtPass(setTossPassTagY) * XYMAG;//地上から決まった距離★設定できるように
        }

        //タゲまでの距離
        SetTagDist(PaDt, ttx, tty, ttz);
      }
    }
    else//タゲがダッシュマン//メンバー操作含む★
    {

      s32 dsx = pmgEO_->mgDt_.dtSet_.GetDtCourt(setDashX);
      s32 dsz = pmgEO_->mgDt_.dtSet_.GetDtCourt(setDashZ);

      //ダッシュ加速中
      if (tgP->st_.pstMyCh_->DsAcc_c > 0)
      {
        dsx += tgP->RankSpeed(rkDashMAXAcc);
      }

      s32 dXSign = 0;

      switch (tgP->st_.pstMyCh_->Zahyou.DsMuki)
      {
      case mL: dXSign = -1; break;
      case mR: dXSign = +1; break;
      }

      //タゲの移動速度の更新（海岸などで乱れるため）★偏差射撃でのみ使ってる
      PaDt->tvx = dsx * dXSign;

      //移動速度をジャンプ速度に

      s32 jvx = dXSign * pmgEO_->mgDt_.dtSet_.GetDtCourt(setDashJpX);
      s32 jvy = pmgEO_->mgDt_.dtSet_.GetDtJump(setDashJump_vY0);

      const s32 CRTIME = 4;//ジャンプ前のしゃがみ
      //パススピード
      s32 paspd = pmgEO_->mgDt_.dtSet_.GetDtPass(setDMPaSpd);

      s32 tgx = (st_.mysideNo_ == 0)
        ? (DBCRT_CL - st_.pmgMyTm_->GenGetAtcLineX(TRUE, TRUE))
        : (DBCRT_CL + st_.pmgMyTm_->GenGetAtcLineX(TRUE, TRUE));

      //予定位置（目標方向へ向かった際のアタックラインまでに勧めるＺ）
      PaDt->tdmz = tgP->st_.pstMyCh_->Zahyou.Z;

      const s32 QPATGY = (pmgEO_->mgDt_.dtSet_.GetDtPass(setQuickPassTagY) * XYMAG);

      if (tgP->st_.pstMyCh_->Motion.Mtype == dbmtSl)//スリップ
      {
        tty = tgP->st_.pstMyCh_->Zahyou.Y
          + (pmgEO_->mgDt_.dtSet_.GetDtPass(setQuickPassTagY) * XYMAG);

        //タゲまでの距離
        SetTagDist(PaDt, ttx, tty, ttz);

        PaDt->stopDm_f = TRUE;//結局定点を狙う★

      }
      else if (tgP->st_.pstMyCh_->Motion.Mtype == dbmtDs)//ダッシュ中
      {
        //ダッシュマンがジャンプラインに到達するまでの距離
        s32 jumpDistX = (st_.pmgMyTm_->GetAtcLineX(TRUE, TRUE) - tgP->GetLeftCrtX());//★ジャンプライン
        //既に超えてる
        BOOL overAtcLine_f = (jumpDistX < 0);

        //ダッシュしている時間
        s32 timedash = (overAtcLine_f)//既に超えてる
          ? 0
          : abs(jumpDistX / dsx);

        //ダッシュマンがジャンプラインに到達するまでの時間 + ジャンプしゃがみ時間
        s32 timedjl = (timedash + CRTIME);

        s32 signZ = 0;

        if (tgP->IsMAN() == FALSE)
        {
          if (tgP->st_.pstMyCh_->Zahyou.Z < tgP->st_.pstMyCh_->DashmanTgZ) signZ = +1;
          if (tgP->st_.pstMyCh_->Zahyou.Z > tgP->st_.pstMyCh_->DashmanTgZ) signZ = -1;
        }

        //到着予定Ｚ
        PaDt->tdmz = tgP->st_.pstMyCh_->Zahyou.Z + (signZ * dsz * timedash);

        if (signZ != 0)//Z移動中
        {
          //タゲの移動速度の更新（海岸などで乱れるため）★偏差射撃でのみ使ってる
          PaDt->tvz = (dsz * signZ);


          //Ｚ移動してる時間
          PaDt->tvz_time = timedash;

          BOOL deftgz_f = FALSE;
          if (signZ > 0)//奥移動中
          {
            //予定Ｚが目標Ｚを超える
            if (PaDt->tdmz > tgP->st_.pstMyCh_->DashmanTgZ)
            {
              deftgz_f = TRUE;
            }
          }
          else if (signZ < 0)//手前移動中
          {
            //予定Ｚが目標Ｚを超える
            if (PaDt->tdmz < tgP->st_.pstMyCh_->DashmanTgZ)
            {
              deftgz_f = TRUE;
            }
          }

          //ダッシュマン予定位置
          if (deftgz_f)
          {
            PaDt->tdmz = tgP->st_.pstMyCh_->DashmanTgZ;
            s32 distz = abs(tgP->st_.pstMyCh_->Zahyou.Z - tgP->st_.pstMyCh_->DashmanTgZ);//Z距離
            PaDt->tvz_time = abs(distz / dsz);//Ｚ到着するまでの時間
          }
        }
        else
        {
          //Ｚ移動してる時間
          PaDt->tvz_time = NGNUM;
        }

        //ボールマンからジャンプラインまでの距離
        s32 jlx = abs(tgx - st_.pstMyCh_->Zahyou.X);
        s32 jlz = abs(PaDt->tdmz - st_.pstMyCh_->Zahyou.Z);
        f32 jlh = lib_num::Hypot(jlx, jlz);

        //ジャンプラインに速攻パスを出したときに掛かる時間
        s32 timepjl = ((s32)jlh / paspd);

        s32 tct = pmgEO_->mgDt_.dtSet_.GetDtPass(setDsmnTossCatchTime);
        s32 tal = 0;


        ////飛ばないダッシュマンのチーム★
        //if (st_.pmgMyTm_->IsNoJpDashman())
        //{
        //  //アタックラインに速攻パスを出したときに掛かる時間  <
        //  //    ダッシュマンがジャンプラインに到達するまでの時間 + ジャンプしゃがみ時間
        //  //速攻パスが間に合うとき
        //  BOOL arrival_f = ((timepjl < timedash) && (overAtcLine_f == FALSE));

        //  PaDt->Quick_f = TRUE;
        //  tty = tgP->st_.pstMyCh_->Zahyou.Y + QPATGY;
        //  PaDt->tgrv = 0;//地上

        //  if (arrival_f)
        //  {
        //    //タゲまでの距離
        //    SetTagDist(PaDt, ttx, tty, ttz);
        //  }
        //  else//間に合わない
        //  {
        //    PaDt->Late_f = TRUE;//偏差射撃にしない
        //    ttx = tgx;//目標位置に
        //    SetTagDist(PaDt, ttx, tty, ttz);
        //  }
        //}
        //飛ばないダッシュマンのチーム★
        if (st_.pmgMyTm_->IsNoJpDashman())
        {

          //クイックパス
          if (PaDt->Quick_f)
          {
            //アタックラインに速攻パスを出したときに掛かる時間  <
            //    ダッシュマンがジャンプラインに到達するまでの時間 + ジャンプしゃがみ時間
            //速攻パスが間に合うとき
            BOOL arrival_f = ((timepjl < timedash) && (overAtcLine_f == FALSE));

            PaDt->Quick_f = TRUE;
            tty = tgP->st_.pstMyCh_->Zahyou.Y + QPATGY;
            PaDt->tgrv = 0;//地上

            if (arrival_f)
            {
              //タゲまでの距離
              SetTagDist(PaDt, ttx, tty, ttz);
            }
            else//間に合わない
            {
              PaDt->Late_f = TRUE;//偏差射撃にしない
              ttx = tgx;//目標位置に
              ttz = PaDt->tdmz;//★
              SetTagDist(PaDt, ttx, tty, ttz);
            }
          }
          else//トスパス
          {


            //浮かす
            PaDt->Quick_f = FALSE;

            const s32 MANOVATCLINEX = 4 * XYMAG;
            tgx = (st_.mysideNo_ == 0)
              ? (DBCRT_CL - MANOVATCLINEX)
              : (DBCRT_CL + MANOVATCLINEX);

            //ダッシュマンがジャンプラインに到達するまでの距離
            jumpDistX = ((DBCRT_CL - MANOVATCLINEX) - tgP->GetLeftCrtX());//上書
            //既に超えてる
            overAtcLine_f = (jumpDistX < 0);//上書

            //ダッシュしている時間
            timedash = (overAtcLine_f)//既に超えてる
              ? 0
              : abs(jumpDistX / dsx);//上書

            //ダッシュマンがジャンプラインに到達するまでの時間 + ジャンプしゃがみ時間
            timedjl = (timedash + CRTIME);//上書

            //ボールマンからジャンプラインまでの距離
            jlx = abs(tgx - st_.pstMyCh_->Zahyou.X);//上書
            jlh = lib_num::Hypot(jlx, jlz);//上書

            //ジャンプラインに速攻パスを出したときに掛かる時間
            timepjl = ((s32)jlh / paspd);//上書

            //アリウープパス時間

            tal = abs(jumpDistX / dsx) + CRTIME + tct;//上書
            //タゲまでの距離
            SetTagDist(PaDt, ttx, 0, ttz);//上書

            //ジャンプラインに速攻パスを出したときに掛かる時間  <
            //    ダッシュマンがジャンプラインに到達するまでの時間 + ジャンプしゃがみ時間
            //速攻パスが間に合うとき//空中で受け取るまでの時間もプラス
            BOOL arrival_f = ((timepjl < (timedjl + (tct * 2))) && (overAtcLine_f == FALSE));//上書

            if (tal == 0) tal = 1;

            if (arrival_f)
            {
              PaDt->alleyoop = tal;
              //移動先
              tty = (jvy * tct) - (PaDt->tgrv * tct * tct / 2);//地上から決まった距離★
              ttx = tgx + (jvx * tct);
              ttz = PaDt->tdmz;//★

              //タゲまでの距離
              SetTagDist(PaDt, ttx, tty, ttz);
            }
            else
            {

              //間に合わない
              PaDt->Quick_f = TRUE;
              tty = tgP->st_.pstMyCh_->Zahyou.Y + QPATGY;
              PaDt->tgrv = 0;//地上

              PaDt->Late_f = TRUE;//偏差射撃にしない
              ttx = tgx;//目標位置に
              ttz = PaDt->tdmz;//★
              SetTagDist(PaDt, ttx, tty, ttz);

            }
          }


        }
        else//じゃんぷまん
        {
          //アリウープパス時間
          if (PaDt->Quick_f == FALSE)
          {
            tal = abs(jumpDistX / dsx) + CRTIME + tct;

            //タゲまでの距離
            SetTagDist(PaDt, ttx, 0, ttz);
          }

          //ジャンプラインに速攻パスを出したときに掛かる時間  <
          //    ダッシュマンがジャンプラインに到達するまでの時間 + ジャンプしゃがみ時間
          //速攻パスが間に合うとき
          BOOL arrival_f = ((timepjl < timedjl) && (overAtcLine_f == FALSE));//ジャンプまでに


          //★マニュアル操作で間に合わない場合は、ジャンプラインをセンターラインに変えて取り直す
          if ((arrival_f == FALSE) && tgP->IsMAN())
          {
            //浮かす
            PaDt->Quick_f = FALSE;

            const s32 MANOVATCLINEX = 4 * XYMAG;
            tgx = (st_.mysideNo_ == 0)
              ? (DBCRT_CL - MANOVATCLINEX)
              : (DBCRT_CL + MANOVATCLINEX);

            //ダッシュマンがジャンプラインに到達するまでの距離
            jumpDistX = ((DBCRT_CL - MANOVATCLINEX) - tgP->GetLeftCrtX());//上書
            //既に超えてる
            overAtcLine_f = (jumpDistX < 0);//上書

            //ダッシュしている時間
            timedash = (overAtcLine_f)//既に超えてる
              ? 0
              : abs(jumpDistX / dsx);//上書

            //ダッシュマンがジャンプラインに到達するまでの時間 + ジャンプしゃがみ時間
            timedjl = (timedash + CRTIME);//上書

            //ボールマンからジャンプラインまでの距離
            jlx = abs(tgx - st_.pstMyCh_->Zahyou.X);//上書
            jlh = lib_num::Hypot(jlx, jlz);//上書

            //ジャンプラインに速攻パスを出したときに掛かる時間
            timepjl = ((s32)jlh / paspd);//上書

            //アリウープパス時間

            tal = abs(jumpDistX / dsx) + CRTIME + tct;//上書
            //タゲまでの距離
            SetTagDist(PaDt, ttx, 0, ttz);//上書

            //ジャンプラインに速攻パスを出したときに掛かる時間  <
            //    ダッシュマンがジャンプラインに到達するまでの時間 + ジャンプしゃがみ時間
            //速攻パスが間に合うとき
            arrival_f = ((timepjl < timedjl) && (overAtcLine_f == FALSE));//上書

          }


          if (arrival_f)
          {
            if (PaDt->Quick_f)//直線パス
            {
              //PaDt->tvz_time = NGNUM;
              tty = tgP->st_.pstMyCh_->Zahyou.Y + QPATGY;
              PaDt->tgrv = 0;//地上

              //タゲまでの距離
              SetTagDist(PaDt, ttx, tty, ttz);
            }
            else
            {
              if (tal > 0)//浮かすパス
              {
                PaDt->alleyoop = tal;
                //移動先
                tty = (jvy * tct) - (PaDt->tgrv * tct * tct / 2);//地上から決まった距離★
                ttx = tgx + (jvx * tct);
                ttz = PaDt->tdmz;//★

                //タゲまでの距離
                SetTagDist(PaDt, ttx, tty, ttz);
              }
              else//直線パス(★クイックと同じか)
              {

                tty = tgP->st_.pstMyCh_->Zahyou.Y + QPATGY;
                //(tgP->st_.pstMyCh_->Zahyou.Y + CHARH);//設定値より若干滑っているので高めを狙う

                PaDt->tgrv = 0;//地上

                //タゲまでの距離
                SetTagDist(PaDt, ttx, tty, ttz);
              }
            }

          }
          else//間に合わないとき
          {
            PaDt->Quick_f = TRUE;
            PaDt->Late_f = TRUE;

            //ジャンプ頂点までの時間
            s32 timejtop = (jvy / PaDt->tgrv);
            ttx = (tgx + (jvx * timejtop));
            ttz = PaDt->tdmz;//★

            //とりあえずタゲまでの距離
            SetTagDist(PaDt, ttx, QPATGY, ttz);

            //とりあえず頂点xz位置までの時間
            f32 h = lib_num::Hypot(PaDt->tdx, PaDt->tdz);
            s32 arrivalTime = (s32)(h / paspd);//ボール到着時間
            //実際にジャンプしてられる時間
            s32 jptime = (arrivalTime - timedjl);//ボール到着時間?ジャンプまでのダッシュ時間

            const s32 MINJP = 4;
            if (jptime < MINJP)
            {
              jptime = MINJP;
            }

            //再度頂点
            ttx = (tgx + (jvx * jptime));
            tty = (jvy * jptime) - ((PaDt->tgrv * jptime * jptime) / 2);
            ttz = PaDt->tdmz;//★

            //タゲまでの距離
            SetTagDist(PaDt, ttx, tty, ttz);
          }
        }
      }
      else//しゃがみ or ジャンプ
      {
        if (PaDt->tvy == 0)//しゃがみのときはジャンプ初速
        {
          PaDt->Quick_f = TRUE;
          PaDt->Late_f = TRUE;

          //ジャンプ頂点までの時間
          s32 timejtop = (jvy / PaDt->tgrv) + st_.pstMyCh_->Anime.Ani_c;

          //★現在の位置にタゲを更新
          tgx = tgP->st_.pstMyCh_->Zahyou.X;

          ttx = tgx + (jvx * timejtop);
          ttz = PaDt->tdmz;//★

          //とりあえずタゲまでの距離
          SetTagDist(PaDt, ttx, QPATGY, ttz);

          //とりあえず頂点xz位置までの時間
          f32 h = lib_num::Hypot(PaDt->tdx, PaDt->tdz);
          s32 arrivalTime = (s32)(h / paspd);//ボール到着時間
          //実際にジャンプしてられる時間
          s32 jptime = (arrivalTime - st_.pstMyCh_->Anime.Ani_c);//ボール到着時間?ジャンプまでのダッシュ時間
          //再度頂点
          ttx = (tgx + (jvx * jptime));
          tty = (jvy * jptime) - ((PaDt->tgrv * jptime * jptime) / 2);
          ttz = PaDt->tdmz;//★

          //タゲまでの距離
          SetTagDist(PaDt, ttx, tty, ttz);

        }
        else
        {
          //ジャンプに入った時点でアリウープは無し
          PaDt->Quick_f = TRUE;
          tty = tgP->st_.pstMyCh_->Zahyou.Y;

          //タゲまでの距離
          SetTagDist(PaDt, ttx, tty, ttz);
        }
      }
    }


  }
  //パス重力・速度
  void TChCommon::PGrvSpdSet(TPassInfo* PaDt)
  {
    //近距離判定用距離
    s32 dist = (s32)lib_num::Hypot(PaDt->tdx, PaDt->tdz) / XYMAG;

    PaDt->tPaGrv = pmgEO_->mgDt_.dtSet_.GetDtGrv(setPaGrv);

    //アリウープ
    if (PaDt->alleyoop > 0)
    {
      PaDt->tPaSpd = (s32)(lib_num::Hypot(PaDt->tdx, PaDt->tdz) / PaDt->alleyoop);
    }
    else
    {
      //★ダッシュマン
      if (PaDt->tdm_f)
      {
        PaDt->tPaSpd = pmgEO_->mgDt_.dtSet_.GetDtPass(setDMPaSpd);
      }
      else
      {
        PaDt->tPaSpd = (RankLevel(rkPaSpd));

        if (PaDt->tPaLine == pl14)
        {
          PaDt->tPaGrv = pmgEO_->mgDt_.dtSet_.GetDtGrv(setPaGrv14);
          PaDt->tPaSpd += pmgEO_->mgDt_.dtSet_.GetDtPass(setPaSpdAdd14);
        }
        else if (PaDt->tPaLine == pl11) //近すぎるときは遅めで
        {
          const s32 NEARDIST = (pmgEO_->mgDt_.dtSet_.GetDtPass(setNearDistDef));
          //非常に近い
          if ((dist * 2) < NEARDIST)
          {
            PaDt->tPaSpd = (pmgEO_->mgDt_.dtSet_.GetDtPass(setDMPaSpd) / 3);
          }//近い
          else if (dist < NEARDIST)
          {
            PaDt->tPaSpd = (pmgEO_->mgDt_.dtSet_.GetDtPass(setDMPaSpd) / 2);
          }
        }
      }
    }

  }
  //★ダッシュマン移動先への角度
  void TChCommon::DashmanAngle(TPassInfo* PaDt)
  {
    f32 distX = (PaDt->tdx == 0)
      ? (f32)1.0
      : (f32)(PaDt->tdxs * PaDt->tdx);
    f32 distZ = (PaDt->tdz == 0)
      ? (f32)1.0
      : (f32)(PaDt->tdzs * PaDt->tdz);

    f32 Sp = (f32)PaDt->tPaSpd;
    f32 envX = (f32)PaDt->tvx;
    f32 envZ = (f32)PaDt->tvz;

    f32 d = (distZ / distX);
    f32 d2 = (d * d);
    f32 b = (d2 * envX) - (d * envZ);
    f32 pm = sqrt((b * b) + (1 + d2) * ((Sp * Sp) + (2 * envX * envZ * d) - (envZ * envZ) - (d2 * envX * envX)));
    if (distX < 0)
    {
      pm = -pm;
    }

    f32 dd = (distX / distZ);
    f32 dd2 = (dd * dd);
    f32 bb = (dd2 * envZ) - (dd * envX);
    f32 pmm = sqrt((bb * bb) + (1 + dd2) * ((Sp * Sp) + (2 * envX * envZ * dd) - (envX * envX) - (dd2 * envZ * envZ)));
    if (distZ < 0)
    {
      pmm = -pmm;
    }

    f32 sx = ((b + pm) / (1 + d2));
    f32 sz = ((bb + pmm) / (1 + dd2));

    s32 absdst = (s32)abs(distX / (envX - sx));
    s32 RealETA = (s32)lib_num::Max(absdst, 1);

    const s32 MINETA = 5;
    PaDt->PaETA_c = (s32)lib_num::Max(RealETA, MINETA);
    PaDt->tdx = (s32)((sx * RealETA) / PaDt->PaETA_c);
    PaDt->tdz = (s32)((sz * RealETA) / PaDt->PaETA_c);

    s32 tagy = (PaDt->tdys * PaDt->tdy) + ((PaDt->tvy * PaDt->PaETA_c) - (PaDt->tgrv * (PaDt->PaETA_c * PaDt->PaETA_c) / 2));


    //if (tagy < -st_.pstMyCh_->Zahyou.Y) tagy = -st_.pstMyCh_->Zahyou.Y;

    const s32 QPATGY = (pmgEO_->mgDt_.dtSet_.GetDtPass(setQuickPassTagY) * XYMAG) * 10;


    //if ((st_.pstMyCh_->Zahyou.Y + tagy) < 0)
    //if (tagy < QPATGY)


    if (tagy < (-st_.pstMyCh_->Zahyou.Y - QPATGY))
    {
      tagy = -st_.pstMyCh_->Zahyou.Y - QPATGY;//
    }

    PaDt->tdy = ((tagy / PaDt->PaETA_c) + ((PaDt->tPaGrv * PaDt->PaETA_c) / 2));

    //至近距離パスの過剰な速度オーバー
    const s32 NEARETA = 10;
    if (PaDt->PaETA_c < NEARETA)//((PaDt->tdx < abs(XYMAG)) && (PaDt->tdz < abs(XYMAG)))
    {
      PaDt->tdy = lib_num::Min(PaDt->tdy, (s32)Sp);
    }

  }
  //パス角度
  void TChCommon::PAngle(TPassInfo* PaDt)
  {


    f32 tdX = (f32)PaDt->tdx;
    f32 tdY = (f32)PaDt->tdy;
    f32 tdZ = (f32)PaDt->tdz;
    f32 tGrv = (f32)PaDt->tPaGrv;
    f32 tSpd = (f32)PaDt->tPaSpd;
    f32 eX, eY, eZ;
    s32 arrivalTime = 1;//到着時間

    //アリウープ★★
    if (PaDt->alleyoop > 0)
    {
      f32 h = lib_num::Hypot(PaDt->tdx, PaDt->tdz);
      if (h < 1) h = 1;
      PaDt->tdx = (s32)(PaDt->tdxs * PaDt->tPaSpd * PaDt->tdx / h);
      PaDt->tdz = (s32)(PaDt->tdzs * PaDt->tPaSpd * PaDt->tdz / h);
      PaDt->PaETA_c = PaDt->alleyoop;

      //定位置狙う
      PaDt->tdy = (PaDt->tPaGrv * (PaDt->PaETA_c / 2))
        + (((PaDt->tdys * PaDt->tdy)) / PaDt->PaETA_c);
    }
    else if (PaDt->tdm_f && (PaDt->stopDm_f == FALSE)) //★ダッシュマンへの偏差射撃パス
    {
      //アリウープ
      if (PaDt->alleyoop > 0)
      {
        f32 h = lib_num::Hypot(PaDt->tdx, PaDt->tdz);
        PaDt->tdx = (s32)(PaDt->tdxs * PaDt->tPaSpd * PaDt->tdx / h);
        PaDt->tdz = (s32)(PaDt->tdzs * PaDt->tPaSpd * PaDt->tdz / h);
        PaDt->PaETA_c = PaDt->alleyoop;

        //定位置狙う
        PaDt->tdy = (PaDt->tPaGrv * (PaDt->PaETA_c / 2))
          + (((PaDt->tdys * PaDt->tdy)) / PaDt->PaETA_c);
      }
      else//速攻
      {
        //時間経過後のＹ座標狙う
        if (PaDt->Late_f == FALSE)
        {
          DashmanAngle(PaDt);

          //ボール到着予定時間がＺ移動時間を超えるときは取り直し
          TMgChar* tgP = st_.pmgMyTm_->st_.pmgMyCh_[pmgSG_->stBa_.PaTgPNo];

          //着地の方が到達よりも速い
          if ((tgP->st_.pstMyCh_->Zahyou.Y > 0)
            && (tgP->st_.pstMyCh_->LandRest < PaDt->PaETA_c))
          {
            //現Ｚ位置→目標Ｚ
            SetTagDist(PaDt,
              tgP->st_.pstMyCh_->LandX,
              (pmgEO_->mgDt_.dtSet_.GetDtPass(setQuickPassTagY) * XYMAG),
              tgP->st_.pstMyCh_->LandZ);


            tdX = (f32)PaDt->tdx;
            tdY = (f32)PaDt->tdy;
            tdZ = (f32)PaDt->tdz;


            eX = tSpd;
            eZ = (eX * tdZ) / tdX;
            arrivalTime = (s32)(tdX / eX);

            //0にならないように
            if (arrivalTime == 0) arrivalTime = 1;
            eY = ((PaDt->tdys * PaDt->tdy) / arrivalTime) + ((tGrv * arrivalTime) / 2);

            PaDt->tdx = PaDt->tdxs * (s32)eX;
            PaDt->tdy = (s32)eY;// +tPT;
            PaDt->tdz = PaDt->tdzs * (s32)eZ;
            PaDt->PaETA_c = arrivalTime;

            //至近距離パスの過剰な速度オーバー
            const s32 NEARETA = 10;
            if (PaDt->PaETA_c < NEARETA)
            {
              PaDt->tdy = lib_num::Min(PaDt->tdy, (s32)PaDt->tPaSpd);

            }


          }
          else if ((PaDt->tvz_time != NGNUM) && (PaDt->PaETA_c > PaDt->tvz_time))
          {

            //現Ｚ位置→目標Ｚ
            SetTagDist(PaDt,
              tgP->st_.pstMyCh_->Zahyou.X,
              tgP->st_.pstMyCh_->Zahyou.Y + (pmgEO_->mgDt_.dtSet_.GetDtPass(setQuickPassTagY) * XYMAG),
              PaDt->tdmz);
            //Ｚ速度０
            PaDt->tvz = 0;

            //とりなおし
            DashmanAngle(PaDt);
          }
        }
        else
        {

          f32 h = lib_num::Hypot(PaDt->tdx, PaDt->tdz);
          eX = (PaDt->tdxs * PaDt->tPaSpd * PaDt->tdx / h);
          eZ = (PaDt->tdzs * PaDt->tPaSpd * PaDt->tdz / h);
          arrivalTime = (s32)(h / PaDt->tPaSpd);
          //0にならないように
          if (arrivalTime == 0) arrivalTime = 1;

          s32 Gosa = (arrivalTime - PaDt->ArriveTime);//予定時間-実際時間

          //空中一定位置を狙う
          eY = ((PaDt->tdys * PaDt->tdy) / arrivalTime) + ((tGrv * arrivalTime) / 2);

          PaDt->tdx = (s32)eX;
          PaDt->tdy = (s32)eY;
          PaDt->tdz = (s32)eZ;
          PaDt->PaETA_c = arrivalTime;
          PaDt->ArriveTime = arrivalTime;

          //至近距離パスの過剰な速度オーバー
          const s32 NEARETA = 10;
          if (PaDt->PaETA_c < NEARETA)
          {
            PaDt->tdy = lib_num::Min(PaDt->tdy, (s32)PaDt->tPaSpd);
          }
        }
      }
    }
    else
    {

      //縦パスか横パスか
      if (PaDt->tdx > PaDt->tdz)
      {
        eX = tSpd;
        eZ = (eX * tdZ) / tdX;
        arrivalTime = (s32)(tdX / eX);
      }
      else
      {
        eZ = tSpd;
        eX = (eZ * tdX) / tdZ;
        arrivalTime = (s32)(tdZ / eZ);
      }

      //0にならないように
      if (arrivalTime == 0) arrivalTime = 1;

      eY = ((PaDt->tdys * PaDt->tdy) / arrivalTime) + ((tGrv * arrivalTime) / 2);
      ////タゲＹ座標を狙う
      //if (PaDt->tdm_f)//間に合わないダッシュマン
      //{
      //  eY = ((PaDt->tdys * PaDt->tdy) / arrivalTime) + ((tGrv * arrivalTime) / 2);
      //}
      //else if (PaDt->Quick_f)
      //{
      //  eY = ((PaDt->tdys * PaDt->tdy) / arrivalTime) + ((tGrv * arrivalTime) / 2);
      //}
      //else//空中一定位置を狙う
      //{
      //  eY = ((PaDt->tdys * PaDt->tdy) / arrivalTime) + ((tGrv * arrivalTime) / 2);
      //}

      PaDt->tdx = PaDt->tdxs * (s32)eX;
      PaDt->tdy = (s32)eY;// +tPT;
      PaDt->tdz = PaDt->tdzs * (s32)eZ;
      PaDt->PaETA_c = arrivalTime;

      //至近距離パスの過剰な速度オーバー
      const s32 NEARETA = 10;
      if (PaDt->PaETA_c < NEARETA)
      {
        PaDt->tdy = lib_num::Min(PaDt->tdy, (s32)PaDt->tPaSpd);
      }
    }


  }
  //パス
  void TChCommon::SetPass()
  {
    //攻撃時間で手放された
    if (IsBall() == FALSE) return;


    const s32 BALLHOLDY = 16;//ちょい補正
    TPassInfo PaDt;
    PaDt.Init();


    if (IsSelfCtrl())
    {
      //短い押し＝クイック
      PaDt.Quick_f = !(st_.pstMyCh_->TossPass_f);

      if (IsInfield() && (st_.pmgMyTm_->st_.pmgMyCh_[pmgSG_->stBa_.PaTgPNo]->st_.posNo_ <= (s32)dbpoI3))
      {
        //内野間はクイック
        PaDt.Quick_f = TRUE;
      }
    }
    else
    {
      PaDt.Quick_f = !(st_.pmgMyTm_->st_.pstMyTm_->stCOM.toss_f);
    }




    PaDt.tHoldNo = pmgSG_->stBa_.HoldPNo;
    //PaDt.tJPa_f = tJPa_f;

    st_.pmgBa_->baCommon_.SetMtype(bmPass);
    pmgSG_->stBa_.Zahyou.X = st_.pstMyCh_->Zahyou.X;
    pmgSG_->stBa_.Zahyou.Y = st_.pstMyCh_->Zahyou.Y + (BALLHOLDY * XYMAG);
    pmgSG_->stBa_.Zahyou.Z = st_.pstMyCh_->Zahyou.Z;

    GetPassTagDist(&PaDt);
    PGrvSpdSet(&PaDt);
    PAngle(&PaDt);

    //ボールに情報を送る
    pmgSG_->stBa_.Zahyou.dX = PaDt.tdx;
    pmgSG_->stBa_.Zahyou.dY = PaDt.tdy;
    pmgSG_->stBa_.Zahyou.dZ = PaDt.tdz;

    pmgSG_->stBa_.Zahyou.Muki = st_.pstMyCh_->Zahyou.Muki;
    pmgSG_->stBa_.Zahyou.MukiZ = st_.pstMyCh_->Zahyou.MukiZ;

    pmgSG_->stBa_.PaETA_c = PaDt.PaETA_c;
    pmgSG_->stBa_.PaETA = PaDt.PaETA_c;

    pmgSG_->stBa_.NoHit_c = pmgEO_->mgDt_.dtSet_.GetDtPass(setPaHitStartTime);
    pmgSG_->stBa_.PaSelfHit_c = pmgEO_->mgDt_.dtSet_.GetDtShot(setHitStartTime) * 3;
    pmgSG_->stBa_.PaJPa_f = !(PaDt.Quick_f);
    pmgSG_->stBa_.PaAlPa_f = (PaDt.alleyoop > 0);
    pmgSG_->stBa_.PaLine = PaDt.tPaLine;
    pmgSG_->stBa_.PaGrv = PaDt.tPaGrv;

    //シュートとしてのデータ
    pmgSG_->stSh_.Init();//初期化
    pmgSG_->stSh_.idata.spin = 10;

    //カバー出す前に落下地点
    st_.pmgBa_->baCommon_.GetBoundLandPos(pmgSG_->stBa_.PaGrv);

    //カバーマンとりあえず空に★
    st_.pmgMyTm_->st_.pstMyTm_->CvrNo = NGNUM;
    st_.pmgMyTm_->st_.pstMyTm_->Cvr2No = NGNUM;
    //st_.pmgMyTm_->st_.pstMyTm_->Through_f = FALSE;
    pmgSG_->stBa_.MvCtrl_f = FALSE;

    //ボールマンダッシュ予約を解除(ＣＯＭがダッシュマン以外でも走り出す)
    st_.pmgMyTm_->st_.pstMyTm_->DashmanNoBM = NGNUM;

    //ダッシュマン以外へのパス
    if (PaDt.tdm_f == FALSE)
    {
      //敵のパスカッターにカーソル移動
      st_.pmgEnTm_->SetCtrl(pmgSG_->stBa_.PaCtPNo);

      //カバーマン
      const s32 PANOMVCTRLTIME = 12;
      if (pmgSG_->stBa_.PaETA_c > PANOMVCTRLTIME)
      {
        pmgSG_->stBa_.MvCtrl_f = TRUE;//操作権委譲のあるパス
        st_.pmgMyTm_->SeekCover(PaDt.tHoldNo, pmgSG_->stBa_.PichPNo, pmgSG_->stBa_.PaTgPNo, FALSE);
      }

    }
    else//ダッシュマンへのパス
    {
      if (IsInfield() == FALSE)//自分が外野の時は敵のパスカッターにカーソル移動★
      {
        st_.pmgEnTm_->SetCtrl(pmgSG_->stBa_.PaCtPNo);
      }


      //ボールダッシュマン
      if ((IsBMDashOK() && PaDt.Quick_f)
        //|| st_.pmgMyTm_->st_.pstMyTm_->COMDt.setterBMRsvGo_f
        )//敵方向ダッシュが入っていたら予約//トスパスの時はダッシュ継続しない
      {
        st_.pmgMyTm_->st_.pstMyTm_->DashmanNoBM = st_.posNo_;//予約
      }
    }

    CatchSE();
    //st_.pmgBa_->baCommon_.HomingSESet(sePass);

    //★成績//パス数合計
    AddRec(recPassTry_c);
    //RecMsg(AddRec(recPass_c, "パス数");

  }

  //拾える位置関係か
  BOOL TChCommon::IsPickUpPos(BOOL ca_f)
  {
    s32 tD = pmgEO_->mgDt_.dtSet_.GetDtBall(setFAtariDepth2);

    //ジャンプボール上昇中は捕れない
    if ((pmgSG_->stBa_.JumpBall == jbJpball) && (pmgSG_->stBa_.Zahyou.dY > 0)) return FALSE;

    //審判投げ入れボールも相手外野は捕れない
    //内野も取れないように
    if ((pmgSG_->stBa_.JumpBall == jbSide0) && (st_.mysideNo_ == 1)) return FALSE;
    if ((pmgSG_->stBa_.JumpBall == jbSide1) && (st_.mysideNo_ == 0)) return FALSE;

    if (pmgSG_->stBa_.NGGet_f)
    {
      if ((pmgSG_->stBa_.NGGetPNo == st_.posNo_)
        && (pmgSG_->stBa_.NGGetTNo == st_.mysideNo_))
      {
        return FALSE;
      }
    }


    //ボール状態でいろいろ判断
    switch (pmgSG_->stBa_.Motion)
    {
    case bmFree://フリー

      break;

    case bmBound://バウンド

      break;
    case bmPass://パス  パスはここでとらないようにするか
      if (pmgSG_->stBa_.PichTNo == st_.mysideNo_)//味方パス
      {
        return FALSE;
      }
      else//敵パス
      {
        return FALSE;
      }
      break;
    case bmShoot://シュート
      if ((pmgSG_->stBa_.PichTNo == st_.mysideNo_) && ca_f)//味方シュート
      {
        //return FALSE;
      }
      else//敵シュート
      {
        return FALSE;
      }
      break;
    case bmHold://持ち
      if ((pmgSG_->stBa_.HoldTNo == st_.mysideNo_)
        && (pmgSG_->stBa_.HoldPNo != st_.posNo_)
        && ca_f)//味方
      {
        if (st_.pmgMyTm_->GetBallmanNo() != NGNUM)
        {
          //アクション中は取れないように
          if (st_.pmgMyTm_->st_.pmgMyCh_[st_.pmgMyTm_->GetBallmanNo()]->st_.pstMyCh_->Motion.IsMFlags(dbmfAct))
          {
            return FALSE;
          }
        }
        //return FALSE;
      }
      else//敵シュート
      {
        return FALSE;
      }
      break;
      //敵が持っているのはとれない（とれても割と面白い→内野がオート移動してしまう
    default:
      return FALSE;
    }
    //当たり
    return (st_.pstMyCh_->Kurai.IsPile(pmgSG_->stBa_.Atari)
      && (abs(pmgSG_->stBa_.Zahyou.Z - st_.pstMyCh_->Zahyou.Z) < (tD * XYMAG)));
  }

  //勝手に拾う処理
  void TChCommon::AutoPickUp()
  {
    if ((st_.pstMyCh_->Motion.IsMFlags(dbmfAct) == FALSE)
      && IsFreeMotion(FALSE)
      )
    {
      if (IsPickUpPos(FALSE))
      {
        BallGet(FALSE, FALSE);
      }
    }
  }

  BOOL TChCommon::IsShTag()
  {
    return ((pmgSG_->stBa_.ShTgTNo != NGNUM) && (pmgSG_->stBa_.ShTgPNo != NGNUM));
  }

  //タゲ方向を向く
  void TChCommon::LookTg(s32 TgPNo, BOOL Pa_f, BOOL AtLook_f)
  {

    TgPNo = 0;

    TStChar* TgSt = NULL;
    BOOL Notag_f = FALSE;
    s32 fX, fZ;//SHLAG後の自分の位置
    const s32 SHLAG = 10;//約10フレ後に投げる
    BOOL dmPass_f = FALSE;
    BOOL alleyoop_f = FALSE;

    if (Pa_f)
    {
      //タゲがいない
      if ((pmgSG_->stBa_.PaTgTNo == NGNUM) || (pmgSG_->stBa_.PaTgPNo == NGNUM))
      {
        Notag_f = TRUE;
      }
      else//タゲがいる
      {
        dmPass_f = pmgGO_->pmgTm_[pmgSG_->stBa_.PaTgTNo]->st_.pmgMyCh_[pmgSG_->stBa_.PaTgPNo]->IsDashman();
        alleyoop_f = pmgSG_->stBa_.PaAlPa_f;
        TgSt = pmgGO_->pmgTm_[pmgSG_->stBa_.PaTgTNo]->st_.pmgMyCh_[pmgSG_->stBa_.PaTgPNo]->st_.pstMyCh_;
      }
    }
    else
    {
      if (IsShTag() == FALSE)//タゲがいない
      {
        Notag_f = TRUE;
      }
      else//タゲがいる
      {
        TgSt = pmgGO_->pmgTm_[pmgSG_->stBa_.ShTgTNo]->st_.pmgMyCh_[pmgSG_->stBa_.ShTgPNo]->st_.pstMyCh_;
      }
    }
    //先の位置
    fX = st_.pstMyCh_->Zahyou.X + (st_.pstMyCh_->Zahyou.dX * SHLAG);
    fZ = st_.pstMyCh_->Zahyou.Z + (st_.pstMyCh_->Zahyou.dZ * SHLAG);


    //タゲあり
    if (Notag_f == FALSE)
    {
      const s32 NEARDISTX = 24 * XYMAG;
      const s32 NEARDISTZ = 12 * XYMAG;
      BOOL near_f = ((abs(TgSt->Zahyou.X - fX) < NEARDISTX)
        && (abs(TgSt->Zahyou.Z - fZ) < NEARDISTZ));

      if ((IsInfield() == FALSE)
        || Pa_f
        || (near_f == FALSE))
      {
        //オート向き初期化
        AutoMukiInit();

        if (lib_num::Sign(TgSt->Zahyou.X - st_.pstMyCh_->Zahyou.X) == lib_num::Sign(TgSt->Zahyou.X - fX))
        {
          st_.pstMyCh_->Auto.AMuki = (TgSt->Zahyou.X < fX)
            ? maL
            : maR;
        }


        //ダッシュマンパス
        if (dmPass_f)
        {
          st_.pstMyCh_->Auto.AMukiZ = mzaN;
        }
        else
        {
          //BOOL Z_f = ((abs(TgSt->Zahyou.Z - fZ) * 2) > abs(TgSt->Zahyou.X - fX));
          BOOL Z_f = ((abs(TgSt->Zahyou.Z - fZ)) > abs(TgSt->Zahyou.X - fX));//★斜めのとき

          if (TgSt->Zahyou.Z < fZ)
          {
            st_.pstMyCh_->Auto.AMukiZ = (Z_f)
              ? mzaF
              : mzaN;
          }
          else
          {
            st_.pstMyCh_->Auto.AMukiZ = (Z_f)
              ? mzaB
              : mzaN;
          }
        }

        MukiSetAuto();
      }
    }
    else//タゲ無し
    {
      if ((IsInfield() == FALSE)
        || (IsSelfCtrl() == FALSE)
        || (AtLook_f && Notag_f))
      {
        //２，３番外野が真横に投げてしまうのを阻止
        switch (st_.posNo_)
        {
        case (s32)dbpoO2:
          st_.pstMyCh_->Auto.AMukiZ = mzaF;
          MukiSetAuto();
          ShTagSet(FALSE);
          break;
        case (s32)dbpoO3:
          st_.pstMyCh_->Auto.AMukiZ = mzaB;
          MukiSetAuto();
          ShTagSet(FALSE);
          break;
        case (s32)dbpoO4:
          st_.pstMyCh_->Auto.AMuki = (st_.mysideNo_ == 0)
            ? maL
            : maR;
          MukiSetAuto();
          ShTagSet(FALSE);
          break;
        }

        if (Pa_f == FALSE)
        {
          //全方向チェック
          enMukiType lastMuki = st_.pstMyCh_->Zahyou.Muki;
          enMukiZType lastMukiZ = st_.pstMyCh_->Zahyou.MukiZ;

          if (IsShTag()) return;

          //ニュートラルから探す
          st_.pstMyCh_->Auto.AMukiZ = mzaN;
          MukiSetAuto();
          ShTagSet(FALSE);
          if (IsShTag()) return;

          st_.pstMyCh_->Auto.AMukiZ = mzaF;
          MukiSetAuto();
          ShTagSet(FALSE);
          if (IsShTag()) return;

          st_.pstMyCh_->Auto.AMukiZ = mzaB;
          MukiSetAuto();
          ShTagSet(FALSE);
          if (IsShTag()) return;

          //逆向き
          st_.pstMyCh_->Auto.AMukiZ = mzaN;
          st_.pstMyCh_->Auto.AMuki = (st_.pstMyCh_->Zahyou.Muki == mL)
            ? maR
            : maL;
          MukiSetAuto();
          ShTagSet(FALSE);
          if (IsShTag()) return;

          st_.pstMyCh_->Auto.AMukiZ = mzaF;
          MukiSetAuto();
          ShTagSet(FALSE);
          if (IsShTag()) return;

          st_.pstMyCh_->Auto.AMukiZ = mzaB;
          MukiSetAuto();
          ShTagSet(FALSE);
          if (IsShTag()) return;

          //みつからなかった
          lastMuki = st_.pstMyCh_->LastMuki;
          lastMukiZ = st_.pstMyCh_->LastMukiZ;
        }
      }
    }
  }

  //パスタゲにならない★★再確認
  BOOL TChCommon::IsNGPassTag(s32 pos)
  {
    if ((pos == NGNUM) || (pos == st_.posNo_))//自分
    {
      return TRUE;//パス不可
    }
    else if (st_.pmgMyTm_->st_.pmgMyCh_[pos]->IsDashman())//★ダッシュマンはだいじょぶ
      //&& ((st_.pmgMyTm_->st_.pmgMyCh_[pos]->st_.pstMyCh_->Zahyou.dY >= (-XYMAG))
      //  || (st_.pstMyCh_->MirPass_c > 0)))//下降ではない
    {
      return FALSE;
    }
    else
    {

      //空中の人はパスタゲにならないように
      return ((st_.pmgMyTm_->st_.pmgMyCh_[pos]->IsFreeMotion(TRUE) == FALSE)
        || st_.pmgMyTm_->st_.pmgMyCh_[pos]->st_.pstMyCh_->Motion.IsMFlags(dbmfAr)//(IsCheckLandEnPos(pos))
        );
    }
  }

  //ボールマンダッシュが可能
  BOOL TChCommon::IsBMDashOK()
  {
    BOOL res = FALSE;
    //敵方向ダッシュ
    BOOL enmuki_f = (((st_.mysideNo_ == 0) && (st_.pstMyCh_->Zahyou.DsMuki == mR))
      || ((st_.mysideNo_ == 1) && (st_.pstMyCh_->Zahyou.DsMuki == mL)));

    if (st_.pstMyCh_->Motion.IsMFlags(dbmfDs)
      && (st_.pstMyCh_->Motion.IsMFlags(dbmfAr) == FALSE)
      && (enmuki_f))
    {
      res = TRUE;
    }

    return res;
  }

  //パスタゲにならない
  s32 TChCommon::GetDist(s32 pos)
  {
    s32 dx = (st_.pstMyCh_->Zahyou.X - st_.pmgMyTm_->st_.pmgMyCh_[pos]->st_.pstMyCh_->Zahyou.X);
    s32 dz = (st_.pstMyCh_->Zahyou.Z - st_.pmgMyTm_->st_.pmgMyCh_[pos]->st_.pstMyCh_->Zahyou.Z);

    return (s32)(lib_num::Hypot(dx, dz));
  }

  //★パス開始処理
  void TChCommon::Passing(BOOL Jp_f)
  {

    //ギリギリでパスタゲを撮り直す（フリック入力）
    if (mid::midIsTBL())
    {
      PaTagSet();
    }


    //パスタゲがパス出せないとき
    NGPaTagShift();

    //内野の場合後ろ内野内パスなので、
    //相手の方向を向かないと行けない
    //★ミラーダッシュマンのときはそちらを向かない
    //if (st_.pmgMyTm_->st_.pmgMyCh_[pmgSG_->stBa_.PaTgPNo]->IsDashman() == FALSE)
    //{
    LookTg(pmgSG_->stBa_.PaTgPNo, TRUE, FALSE);
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

  void TChCommon::SetBallMukiX()
  {
    if (IsBall()) return;

    s32 distx = 0;

    //自分がタゲでパスが飛んできてるときはそのままの向き自分が投げたときも
    if ((IsInfield() == FALSE)
      && (pmgSG_->stBa_.Motion == bmPass)
      && (pmgSG_->stBa_.PichTNo == st_.mysideNo_)
      && (pmgSG_->stBa_.PichPNo == st_.posNo_))
    {
      return;
    }

    s32 asdf = pmgSG_->stBa_.Zahyou.X;
    s32 asdf2 = st_.pstMyCh_->Zahyou.X;

    if (pmgSG_->stBa_.Zahyou.X < (st_.pstMyCh_->Zahyou.X - distx))
    {
      st_.pstMyCh_->NextAuto.AMuki = maL;
    }
    else if (pmgSG_->stBa_.Zahyou.X > (st_.pstMyCh_->Zahyou.X + distx))
    {
      st_.pstMyCh_->NextAuto.AMuki = maR;
    }
  }

  void TChCommon::SetBallMukiZ()
  {

    if (IsBall()) return;

    //自分がタゲでパスが飛んできてるときはそのままの向き
    //自分が投げたときも
    if ((IsInfield() == FALSE) && (pmgSG_->stBa_.Motion == bmPass))
    {
      if ((pmgSG_->stBa_.PichTNo == st_.mysideNo_)
        && (pmgSG_->stBa_.PichPNo == st_.posNo_))
      {
        return;
      }

      //パスの時はパス先を向く
      switch (pmgSG_->stBa_.PaTgPNo)
      {
      case (s32)dbpoO2: st_.pstMyCh_->NextAuto.AMukiZ = mzaB; break;
      case (s32)dbpoO3: st_.pstMyCh_->NextAuto.AMukiZ = mzaF; break;
      default: st_.pstMyCh_->NextAuto.AMukiZ = mzaN; break;
      }
    }
    else
    {
      const s32 DEFDISTZ = 16 * XYMAG;

      s32 distZ = (IsInfield())
        ? DEFDISTZ//(DEFDISTZ / 2)
        : DEFDISTZ;

      if (pmgSG_->stBa_.Zahyou.Z < st_.pstMyCh_->Zahyou.Z)
      {
        if (pmgSG_->stBa_.Zahyou.Z < (st_.pstMyCh_->Zahyou.Z - distZ))
        {
          st_.pstMyCh_->NextAuto.AMukiZ = mzaF;
        }
        else if (pmgSG_->stBa_.Zahyou.Z > (st_.pstMyCh_->Zahyou.Z - (distZ / 2)))
        {
          st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
        }
        else
        {
          if (st_.pstMyCh_->NextAuto.AMukiZ == mzaB)
          {
            st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
          }
        }
      }
      else
      {
        if (pmgSG_->stBa_.Zahyou.Z > (st_.pstMyCh_->Zahyou.Z + distZ))
        {
          st_.pstMyCh_->NextAuto.AMukiZ = mzaB;
        }
        else if (pmgSG_->stBa_.Zahyou.Z < (st_.pstMyCh_->Zahyou.Z + (distZ / 2)))
        {
          st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
        }
        else
        {

          if (st_.pstMyCh_->NextAuto.AMukiZ == mzaF)
          {
            st_.pstMyCh_->NextAuto.AMukiZ = mzaN;
          }
        }
      }

    }
  }

  void TChCommon::SetCatchMuki()
  {
    //キャッチもぼーる方向向くようにしてみる
    AutoMukiInit();
    SetBallMukiX();
    SetBallMukiZ();
    st_.pstMyCh_->Auto = st_.pstMyCh_->NextAuto;
    MukiSetAuto();

    //        SetMtype(dbmtCa);
    CatchSE();
  }

  //自分で操作
  void TChCommon::SelfCtrl(enATType ATType)
  {
    const s32 MIRWAIT = 4;
    //モーション変更前の向き
    enMukiType lastMuki = st_.pstMyCh_->Zahyou.Muki;
    enMukiZType lastMukiZ = st_.pstMyCh_->Zahyou.MukiZ;
    BOOL LastMukiKeep_f = FALSE;
    BOOL autoPickUp_f = TRUE;
    BOOL atlook_f = MyPad()->IsAutoLook();//自動タゲ向き（ニュートラル）

    //プレビュー中パス禁止
    BOOL pabtn_f = MyPad()->IsPass() && (pmgGO_->pmgRf_->IsPrev() == FALSE);
    BOOL pabtn2_f = MyPad()->IsPass2() && (pmgGO_->pmgRf_->IsPrev() == FALSE);
    BOOL dgbtn_f = MyPad()->IsDodge();
    BOOL dgbtn2_f = MyPad()->IsDodge2();
    BOOL cabtn_f = MyPad()->IsCatch();
    BOOL shbtn_f = MyPad()->IsShot();
    BOOL shbtn2_f = MyPad()->IsShot2();
    BOOL uppos_f = (pabtn_f || dgbtn_f);
    BOOL dnpos_f = (cabtn_f || shbtn_f);


    BOOL ng_f = FALSE;

    //シュート以外の時はきゃっちよけがでないように
    if (st_.pstMyTm_->smart_f_)
    {

      BOOL caok_f = (((pmgSG_->stBa_.Motion == bmShoot) && (pmgSG_->stBa_.PichTNo == st_.ensideNo_))
        || ((pmgSG_->stBa_.Motion == bmPass) && (pmgSG_->stBa_.PichTNo == st_.ensideNo_))
        || ((pmgSG_->stBa_.Motion == bmHold) && pmgSG_->stBa_.shotmotion_f));

      if (caok_f == FALSE)
      {
        //dgbtn_f = FALSE;
        //dgbtn2_f = FALSE;
        //ダッシュがにゅうりょくされてなければ
        cabtn_f = (MyPad()->IsCatch3() //左下短押しリリース
          && (MyPad()->IsDash(IsBall()) == FALSE));//ダッシュにゅうりょく無し
      }

      if (st_.pstMyCh_->shotok_f_ == FALSE)
      {
        if (shbtn2_f == FALSE)
        {
          st_.pstMyCh_->shotok_f_ = TRUE;
        }
        else
        {
          shbtn_f = FALSE;//キャッチ後ジャンプおしっぱで出てしまうのカット
        }
      }

      //ダッシュ中前フリックはシュート
      if (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
      {
        if (st_.pstMyCh_->Zahyou.DsMuki == mL)
        {
          shbtn_f |= MyPad()->IsDsShot(dxL);
        }
        else if (st_.pstMyCh_->Zahyou.DsMuki == mR)
        {
          shbtn_f |= MyPad()->IsDsShot(dxR);
        }
      }

      //スマホ操作ではおしっぱシュートは使わない
      shbtn2_f = FALSE;
    }


#ifdef __K_DEBUG_SHIAI__
    if (kdebug::DebugSystem::GetInstance()->IsEnemyLastOne())
    {
      // 今いるやつを全員殺す
      for (s32 i = 0; i < DBMEMBER_INF; i++)
      {
        pmgGO_->pmgCh_[st_.ensideNo_][i]->st_.pstMyCh_->ANGEL_f = TRUE;
        pmgGO_->pmgCh_[st_.ensideNo_][i]->st_.pstMyCh_->HP = 0;
      }
      pmgGO_->pmgTm_[st_.ensideNo_]->CheckChangePos();
      // 一人を残して全員殺す
      for (s32 i = 0; i < DBMEMBER_INF - 2; i++)
      {
        pmgGO_->pmgCh_[st_.ensideNo_][i]->st_.pstMyCh_->ANGEL_f = TRUE;
        pmgGO_->pmgCh_[st_.ensideNo_][i]->st_.pstMyCh_->HP = 0;
      }
      pmgGO_->pmgTm_[st_.ensideNo_]->CheckChangePos();
    }
#endif


    //チュートリアルのＮＧ行為（押し位置間違いチェック）
    if (pmgEO_->stShiai_.IsTuto() && (st_.mysideNo_ == SIDE0))
    {
      switch (st_.pmgRf_->GetTutoNo())
      {
      case sta_DodgeAct:
        if (dnpos_f)
        {
          st_.pmgRf_->SetTutoNG(st_.pmgRf_->GetTutoNo(), rta_dodge_ngpos);
          ng_f = TRUE;
        }
        break;
      case sta_WalkAct:
        if (uppos_f)
        {
          st_.pmgRf_->SetTutoNG(st_.pmgRf_->GetTutoNo(), rta_walk_ngpos);
          ng_f = TRUE;
        }
        break;
      case sta_JumpAct:
        if (uppos_f)
        {
          st_.pmgRf_->SetTutoNG(st_.pmgRf_->GetTutoNo(), rta_jump_ngpos);
          ng_f = TRUE;
        }
      case sta_CatchAct:
        if (uppos_f)
        {
          st_.pmgRf_->SetTutoNG(st_.pmgRf_->GetTutoNo(), rta_catch_ngpos);
          ng_f = TRUE;
        }
        break;
      case sta_DashAct:
        if (uppos_f)
        {
          st_.pmgRf_->SetTutoNG(st_.pmgRf_->GetTutoNo(), rta_dash_ngpos);
          ng_f = TRUE;
        }
        shbtn_f = FALSE;
        pabtn_f = FALSE;
        break;
      case sta_ShotAct:
        if (uppos_f)
        {
          st_.pmgRf_->SetTutoNG(st_.pmgRf_->GetTutoNo(), rta_shot_ngpos);
          ng_f = TRUE;
        }
        break;
      case sta_GameAct:
        break;
      default:
        ng_f = TRUE;
        break;
      }
    }

    if (ng_f)
    {
      pabtn_f = FALSE;
      pabtn2_f = FALSE;
      dgbtn_f = FALSE;
      dgbtn2_f = FALSE;
      cabtn_f = FALSE;
      shbtn_f = FALSE;
    }

    if (st_.pstMyCh_->Nomove_f)
    {
      st_.pstMyCh_->Nomove_f = FALSE;
      return;
    }

    //シュートとパスは投げる瞬間にターゲットの方向を自動で向くようにする
    LastMukiKeep_f = TRUE;

    BOOL shotok_f = TRUE;

    //スマート
    if (st_.pstMyTm_->smart_f_)
    {
      //内野で立ち、歩きの時はシュートしない
      if (IsInfield()
        && ((st_.pstMyCh_->Motion.Mtype == dbmtSt) || (st_.pstMyCh_->Motion.Mtype == dbmtWk)))
      {
        shotok_f = FALSE;
      }
    }


#ifdef __K_DEBUG_SHIAI__
    kdebug::DebugSystem* pDs = kdebug::DebugSystem::GetInstance();
    bool isAutoShot = FALSE;
    // 自動シュート状態を取得
    s32 step = pDs->GetAutoShootStep();
    if (step == kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_STANDBY)
    {
      // この状態で手元にボールがないのがおかしいので
      // 強制的に手元に戻す
      // なのでこのif文内は例外処理
      if ((pmgSG_->stBa_.Motion != bmHold)
        || (pmgSG_->stBa_.HoldTNo != 0)
        || (pmgSG_->stBa_.HoldPNo != 0))
      {
        // パスできる状態になったら手元に戻す
        if (!IsNGPassTag(0))
        {
          // ボールが手元に帰ってくる処理
          pmgGO_->pmgBa_->baCommon_.ResetRefPos_Prev(FALSE);
        }
        return;
      }
      else
      {
        // 操作キャラをボール持ってるやつに
        st_.pmgMyTm_->SetCtrlBallGet(0);
      }

      // 自動シュート終わりますよフラグが立っていれば投げない
      if (!pDs->IsAutoShootEnd())
      {
        isAutoShot = TRUE;
      }
    }
    else
    {
      // もしもボールを自分が持っていて待っている状態であれば次のシーケンスに進めてしまう
      if (pDs->IsReturnBall())
      {
        // 自動シュート状態を取得
        if (step == kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_WAIT)
        {
          // すでに手に持っているので次のステートに移行する
          if ((pmgSG_->stBa_.Motion == bmHold)
            && (pmgSG_->stBa_.HoldTNo == 0)
            && (pmgSG_->stBa_.HoldPNo == 0))
          {
            pDs->SetAutoShootStep(kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_CHOOSE);
            pDs->SetReturnBallFlg(FALSE);	// フラグを落とす
    }
          // 他人が持ってる
          else
          {
            // パスできる状態になったら手元に戻す
            if (!IsNGPassTag(0))
            {
              // ボールが手元に帰ってくる処理
              pmgGO_->pmgBa_->baCommon_.ResetRefPos_Prev(FALSE);
              return;
            }
          }
  }
}
    }
#endif	// #ifdef __K_DEBUG_SHIAI__


    switch (st_.pstMyCh_->Motion.Mtype)
    {
    case dbmtCr:
      break;
    case dbmtSt:
    case dbmtWk:
    case dbmtDs:
    case dbmtSl:
    {
      switch (ATType)
      {
      case ATA://攻撃/////////////////////////////////
        if (pabtn_f)//
        {
          //内野の場合後ろ内野内パスなので、相手の方向を向かないと行けない
          Passing(FALSE);
        }
#ifdef __K_DEBUG_SHIAI__
        // シュート条件に自動シュートフラグも入れる
        else if ((shbtn_f && shotok_f) || isAutoShot)//シュート入力
#else
        else if (shbtn_f && shotok_f)//シュート入力
#endif	// #ifdef __K_DEBUG_SHIAI__

        {
          //外野２３からＺ軸シュートのとき、一応相手の方向を向く
          LookTg(pmgSG_->stBa_.ShTgPNo, FALSE, atlook_f);// && (st_.pstMyCh_->Motion.Mtype == dbmtSt)

          //ダッシュ方向とシュート方向があっているときは振り返り扱いにしない
          BOOL nortst_f = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
            && (st_.pstMyCh_->Zahyou.DsMuki == st_.pstMyCh_->Zahyou.Muki);

          //内野で向きに変わるときは振り向きシュート
          if (IsInfield()
            && (nortst_f == FALSE)
            && (lastMuki != st_.pstMyCh_->Zahyou.Muki)
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
          kdebug::DebugSystem::GetInstance()->CreateShootDebugLog();
          // 自動シュートフラグを落とす
          if (isAutoShot)
          {
            pDs->SetAutoShootStep(kdebug::AUTO_SHOOT_SYSTEM::ASS_STEP_SHOOT);
          }
#endif	// #ifdef __K_DEBUG_SHIAI__
        }
        else
        {
          if (shbtn2_f)//シュート入力おしっぱ
          {
            st_.pstMyCh_->MirPass_c = 0;
          }

          if (st_.pstMyCh_->MirPass_c > 0)//ミラーパス状態
          {
            if (pabtn2_f)
            {
              if (lib_num::UpToR(&st_.pstMyCh_->MirPass_c, MIRWAIT))
              {
                if (pmgSG_->stBa_.PaTgPNo != NGNUM)
                {
                  if (st_.pmgMyTm_->st_.pmgMyCh_[pmgSG_->stBa_.PaTgPNo]->IsDashman())
                  {
                    Passing(FALSE);
                  }
                  else
                  {
                    st_.pstMyCh_->MirPass_c = 0;
                  }
                }
              }
            }
            else
            {
              st_.pstMyCh_->MirPass_c = 0;
            }
          }

          if (st_.pstMyCh_->MirShot_c > 0)//ミラーシュート状態
          {
            if (shbtn2_f && (pabtn2_f == FALSE))//シュート入力おしっぱ
            {
              if (lib_num::UpToR(&st_.pstMyCh_->MirShot_c, MIRWAIT))
              {
                LookTg(pmgSG_->stBa_.ShTgPNo, FALSE, TRUE);//居ないときはオートで探す

                //ダッシュ方向とシュート方向があっているときは振り返り扱いにしない
                BOOL nortst_f = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
                  && (st_.pstMyCh_->Zahyou.DsMuki == st_.pstMyCh_->Zahyou.Muki);

                if (IsInfield()
                  && (nortst_f == FALSE)
                  && (lastMuki != st_.pstMyCh_->Zahyou.Muki)
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
              st_.pstMyCh_->MirShot_c = 1;
            }
          }
        }
        break;

      case ATD://守備/////////////////////////////////////
        if (dgbtn_f)//避けボタン
        {
          SetMtype(dbmtDg);
          CatchSE();
          if (IsPickUpPos(FALSE)) BallGet(FALSE, FALSE);
        }
        else if (cabtn_f && (st_.pstMyCh_->CatchW_c == 0))//キャッチ入力
        {
          //キャッチもボール方向向くようにしてみる
          SetCatchMuki();
          SetMtype(dbmtCa);
        }
        else if (autoPickUp_f && IsPickUpPos(FALSE))//自動拾い★
        {
          BallGet(FALSE, FALSE);
        }
        break;

      case ATF://拾い
        if ((cabtn_f || dgbtn_f)//キャッチ入力
          && (st_.pstMyCh_->CatchW_c == 0))
        {
          //キャッチもボール方向向くようにしてみる
          SetCatchMuki();
          SetMtype(dbmtCa);
        }
        else if (autoPickUp_f && IsPickUpPos(FALSE))//自動拾い★
        {
          BallGet(FALSE, FALSE);
        }
        break;
      }
    }
    break;
    case dbmtJCa:
      if (IsPickUpPos(TRUE))
      {
        BallGet(FALSE, FALSE);
      }
      break;
    case dbmtCa:
      if (IsPickUpPos(TRUE))
      {
        BallGet(FALSE, FALSE);

        if (IsInfield()
          && (st_.pstMyCh_->Auto.AutoType == dbatFree)
          && (st_.pstMyCh_->Motion.IsMFlags(dbmfDs)))
        {
          if (((st_.pstMyCh_->Zahyou.DsMuki == mL)
            && MyPad()->IsCatchDash(dxL))//キャッチ後ダッシュ継続入力
            || ((st_.pstMyCh_->Zahyou.DsMuki == mR)
            && MyPad()->IsCatchDash(dxR)))
          {
            //継続なので向きセットも歩数リセットもいらない
            SetMtype(dbmtDs);
          }
        }
      }
      break;
    case dbmtDg:
    case dbmtJDg:
      //押しっぱなしで避け続けるようにする
      if (dgbtn2_f
        && (IsBall() == FALSE))
      {
        st_.pstMyCh_->Anime.Ani_c = 0;//こんなんでいいのだろうか
      }
      break;
    case dbmtJUp:
    case dbmtJDn:
//      if ((st_.pstMyCh_->AirAct_f == FALSE)
//        && st_.pstMyCh_->Motion.IsMFlags(dbmfAr)
//        && ((st_.pstMyCh_->Zahyou.dY > 0) || (st_.pstMyCh_->Zahyou.Y >= (JPINVALIDHEIGHT * XYMAG))))
      if (
          ((st_.pstMyCh_->AirAct_f == FALSE)
            && st_.pstMyCh_->Motion.IsMFlags(dbmfAr)
            && ((st_.pstMyCh_->Zahyou.dY > 0) || (st_.pstMyCh_->Zahyou.Y >= (JPINVALIDHEIGHT * XYMAG)))
            )
            ||
            (//マリオネット効果中
            (st_.pstMyCh_->symCtrl_f == TRUE)
            && st_.pstMyCh_->Motion.IsMFlags(dbmfAr)
            )
        )
      {
        if (ATType == ATA)
        {
          if (pabtn_f)
          {
            Passing(TRUE);
          }
          else if (MyPad()->IsJumpShot())//ジャンプシュート入力
          {
            LookTg(pmgSG_->stBa_.ShTgPNo, FALSE, atlook_f);
            SetMtype(dbmtJSh);
          }
          else
          {
            if (shbtn2_f)//シュート入力おしっぱ
            {
              st_.pstMyCh_->MirPass_c = 0;
            }

            if (st_.pstMyCh_->MirPass_c > 0)//ミラーパス状態
            {
              if (pabtn2_f)
              {
                if (lib_num::UpToR(&st_.pstMyCh_->MirPass_c, MIRWAIT))
                {
                  if (pmgSG_->stBa_.PaTgPNo != NGNUM)
                  {
                    if (st_.pmgMyTm_->st_.pmgMyCh_[pmgSG_->stBa_.PaTgPNo]->IsDashman())
                    {
                      Passing(TRUE);
                    }
                    else
                    {
                      st_.pstMyCh_->MirPass_c = 0;
                    }
                  }
                }
              }
              else
              {
                st_.pstMyCh_->MirPass_c = 0;
              }
            }

            if (st_.pstMyCh_->MirShot_c > 0)//ミラーシュート状態
            {
              if (shbtn2_f && (pabtn2_f == FALSE))//シュート入力おしっぱ
              {
                if (lib_num::UpToR(&st_.pstMyCh_->MirShot_c, MIRWAIT))
                {
                  LookTg(pmgSG_->stBa_.ShTgPNo, FALSE, TRUE);
                  SetMtype(dbmtJSh);
                }
              }
              else
              {
                st_.pstMyCh_->MirShot_c = 1;
              }
            }
          }

        }
        else
        {
          if (dgbtn_f)
          {
            SetMtype(dbmtJDg);
            CatchSE();
            if ((st_.pmgMyTm_->st_.pstMyTm_->CtrlNo == pmgSG_->stBa_.PaTgPNo)
              && (pmgSG_->stBa_.Motion == bmPass)
              && (pmgSG_->stBa_.PaTgTNo == st_.mysideNo_)
              && (pmgSG_->stBa_.PaTgPNo <= (s32)dbpoI3))
            {
              st_.pmgMyTm_->SetCtrl(st_.pmgMyTm_->st_.pstMyTm_->CvrNo);
              st_.pmgMyTm_->st_.pmgMyCh_[st_.pmgMyTm_->st_.pstMyTm_->CtrlNo]->st_.pstMyCh_->Nomove_f = TRUE;
              st_.pmgMyTm_->SeekCover(st_.pmgMyTm_->st_.pstMyTm_->CtrlNo, pmgSG_->stBa_.PichPNo, pmgSG_->stBa_.PaTgPNo, TRUE);//新しいカバーマン
            }
          }
          else if (cabtn_f)
          {
            //キャッチもボール方向向くようにしてみる
            SetCatchMuki();
            SetMtype(dbmtJCa);
            //                            CatchSE();
            if (IsPickUpPos(TRUE)) BallGet(FALSE, FALSE);
          }
          else if (autoPickUp_f && IsPickUpPos(FALSE))//自動拾い★
          {
            BallGet(FALSE, FALSE);
          }
        }
      }
      break;
    case dbmtSh://振り向きシュート
      if ((st_.pstMyCh_->Utrun_f == FALSE)
        && IsBall()
        && IsInfield()
        //&& (st_.pstMyCh_->Motion.IsMFlags(dbmfDs) || st_.pstMyCh_->Motion.IsMFlags(dbmfSlip))
        )
      {
        BOOL utrn_f = FALSE;
        if ((st_.pstMyCh_->Zahyou.Muki == mL)
          && MyPad()->IsWalk2(dxR)
          && (st_.mysideNo_ == 0))
        {
          utrn_f = TRUE;
          st_.pstMyCh_->Zahyou.Muki = mR;
          st_.pstMyCh_->Zahyou.MukiZ = mzN;
          st_.pstMyCh_->MukiAgl12 = 1;//1 2 3 4
          SetMukiAgl(FALSE, TRUE, FALSE, FALSE);
        }
        else if ((st_.pstMyCh_->Zahyou.Muki == mR)
          && MyPad()->IsWalk2(dxL)
          && (st_.mysideNo_ == 1))
        {
          utrn_f = TRUE;
          st_.pstMyCh_->Zahyou.Muki = mL;
          st_.pstMyCh_->Zahyou.MukiZ = mzN;
          st_.pstMyCh_->MukiAgl12 = 7;//7 8 9 10
          SetMukiAgl(TRUE, FALSE, FALSE, FALSE);
        }

        if (utrn_f)
        {
          SetMtype(dbmtRtSh);
          //ダッシュに復帰
          if (st_.pstMyCh_->Motion.IsMFlags(dbmfSlip))
          {
            st_.pstMyCh_->Motion.MFlags |= dbmfDs;
          }
        }
      }
      break;
    }

    //モーション変更前の向き
    if (LastMukiKeep_f)
    {
      st_.pstMyCh_->LastMuki = lastMuki;
      st_.pstMyCh_->LastMukiZ = lastMukiZ;
    }
  }

  //ボールが危険球
  BOOL TChCommon::IsDangerBall()
  {
    return FALSE;
    //s32 tdam = lib_num::Percent(pmgSG_->stBa_.ShSt.ShPow, Rank(rkdmDamPer));
    //tdam = st_.pmgBa_->baCommon_.HandiDam(tdam, st_.mysideNo_, st_.pstMyCh_->stPad.MANCharPadNo);//はんで
    //tdam = lib_num::Between(tdam, Rank(rkdmMinDam), Rank(rkdmMaxDam));//1は最低与える
    //
    ////底力が出せる状態か
    //BOOL soko_f = (st_.pstMyCh_->HP >= pmgEO_->mgDt_.dtSet_.GetDtDamage(setSokoCapa));
    //
    //s32 deaddam = (soko_f)
    //? (st_.pstMyCh_->HP + Rank(rkdfSokoHP))
    //: st_.pstMyCh_->HP;
    //
    ////致死ダメージよりもボールの方が強い
    //return (deaddam < tdam);
  }

  //強いボール
  BOOL TChCommon::IsStrongBall()
  {
    return FALSE;
    //const s32 STRPOW = 16;
    //return ((pmgSG_->stBa_.ShSt.ShPow >= STRPOW) || IsDangerBall());
  }

  //デバッグ時成績表示用
  void TChCommon::RecMsg()//s32 num, char* msg
  {
    //std::string s = (st_.mysideNo_ == 0)
    //  ? "★"
    //  : "☆";
    //ConsoleMsg(s + CharNo.Tostd::string() + " " +
    //  pmgEO_->mgDt_.DefCharDt[TeamNo, CharNo].CharName(pscObj_->Eng_f) +
    //  " " + msg + ":" + num.Tostd::string() + "\n");
  }

  s32 TChCommon::ShSptoLv(s32 ShSp)
  {
    if (ShSp < 70) return 1;
    else if (ShSp < 84) return 2;
    else if (ShSp < 100) return 3;
    else if (ShSp < 118) return 4;
    else return 5;
  }

  void TChCommon::SetCOMCatchRec(s32 ShSp)
  {

    for (s32 i = 0; i < (COMCATCHRECNUM - 1); ++i)
    {
      s32 i2 = ((COMCATCHRECNUM - 1) - 1 - i);
      st_.pstMyCh_->stCOM.CatchRec[i2 + 1] = st_.pstMyCh_->stCOM.CatchRec[i2];
    }
    st_.pstMyCh_->stCOM.CatchRec[0] = ShSptoLv(ShSp);
  }

  TChHaveCommon::TChHaveCommon(TGame* pGame, TChCommon* pCommon, s32 tSideNo, s32 tPlayerNo)
    : TChGen(pGame, tSideNo, tPlayerNo), pCommon_(pCommon)
  {
  }
  TChHaveCommon::~TChHaveCommon()
  {
  }
}
