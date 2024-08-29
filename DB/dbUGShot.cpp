#include "dbUGShot.h"
#include "dbUILayer.h"
#include "dbUGShotUra.h"
namespace db
{
  //コンストラクタ


  const s32 BX = 49 - 6;
  const s32 SPINDX = 67;
  const s32 BY = 35 - 12;
  const s32 RX = 8;
  const s32 RY = 1;

  //コンストラクタ
  TUGShot::TUGShot(TUIGenLayer* player) :TUGGen(player)
    , ugname_(player)
    , ugname2_(player)
    , uglock_(player)
    , ugbpow_(player)
    , ugbspin_(player)
    , uggenre_(player)
    , ugqtpow_(player)
    , ugqtspin_(player)
  {
    stShot_.Init();
    ura_ = NULL;
  }

  TUGShot::~TUGShot()
  {
    Freemem(ura_);
  }
  void TUGShot::MakeGroup(TUIObj* parent)//, BOOL myeyebtn_f)
  {
    SetParent(parent);
    field_->g_SetDepth(UIDP_02BGOBJHI);
    //stShot_.myeyebtn_f_ = myeyebtn_f;

    //チームカード地
    field_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_shot_card));

    //合成されたボール
    //for (s32 i = 0; i < HSELEMMAX; ++i)
    for (auto& slot : slot_)
    {
      slot = MakeObj();
      slot->o_SetParent(field_);
      slot->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_rarity));
      slot->g_SetRevDepthLv2(10);
      slot->g_SetDraw(FALSE);//けしとく
    }

    ugname_.MakeGroup(field_);
    ugname_.SetPos(35, 8);
    ugname2_.MakeGroup(field_);
    ugname2_.SetPos(35+24, 8+64);

    ugqtpow_.MakeGroup(field_, FALSE, TRUE);
    ugqtpow_.SetPos(129, 5);

    ugqtspin_.MakeGroup(field_, TRUE, TRUE);
    ugqtspin_.SetPos(129, 5 + 8);

    uglock_.MakeGroup(field_);
    uglock_.SetIcon(ugi_ShLock);
    //uglock_.SetPos(26, 12);
    uglock_.SetPos(12, 1);
    uglock_.SetDraw(FALSE);

    //body_ = MakeObj();
    //body_->o_SetParent(field_);
    //body_->o_SetObjGrPart(player_->pmgEO_->mgGrp_.pgr_IconBodySh_);
    //body_->z_SetPos(8, 32);
    //body_->g_SetDraw(FALSE);

    //shotinvno_ = MakeObj();
    //shotinvno_->o_SetParent(field_);
    //shotinvno_->z_SetPos(24, 8);
    //shotinvno_->g_SetDraw(FALSE);
    const s32 GX = 12;
    const s32 GY = 13;
    uggenre_.MakeGroup(field_);
    uggenre_.SetPos(GX, GY);

    ugbpow_.MakeGroup(field_);
    ugbpow_.SetPos(BX, BY);

    ugbspin_.MakeGroup(field_);
    ugbspin_.SetPos(BX + SPINDX, BY);

    //revpower_ = MakeObj();
    //revpower_->o_SetParent(field_);
    //revpower_->z_SetPos(BX + RX, BY + RY);

    //revspin_ = MakeObj();
    //revspin_->o_SetParent(field_);
    //revspin_->z_SetPos(BX + SPINDX + RX, BY + RY);
  }
  void TUGShot::MakeUra()
  {
    if (ura_ == NULL)
    {
      ura_ = new TUGShotUra(player_);
      ura_->MakeGroup(field_);
      ura_->SetDraw(TRUE);
    }
  }
  void TUGShot::SetUra(BOOL ura_f)
  {
    if (ura_ == NULL) return;

    if (ura_f)
    {
      lib_num::IncLoop(&stShot_.urano_, 0, 2);
      ura_->SetUraNo(stShot_.urano_);
    }
    else
    {
      stShot_.urano_ = 0;
    }
  }

  BOOL TUGShot::IsUra()
  {
    return (stShot_.urano_ > 0);
  }
  void TUGShot::ClearSlot()
  {
    for (s32 i = 0; i < HSELEMMAX; ++i)
    {
      slot_[i]->g_SetDraw(FALSE);
    }
  }
  void TUGShot::RefreshTips()
  {
    if (stShot_.id_shot_ == NGNUM) return;

    SOZAIDTVECTOR sozaidtvec = pmgEO_->mgCoU_.GetSozaiDataAll(stShot_.id_shot_);

    ////装備者
    //s32 memno = pmgEO_->mgCoU_.GetShotHaveMember(id_shot, id_team);
    //if (memno != NGNUM)
    //{
    //  body_->g_SetPartNo(memno);
    //  body_->g_SetDraw(TRUE);
    //}
    //else
    //{
    //  body_->g_SetDraw(FALSE);
    //}

    s32 totalrevpow = 0;
    s32 totalrevspin = 0;
    //ベース＋合成されたボール
    for (s32 i = 0; i < sozaidtvec.size(); ++i)
    {
      SetSlot(i, sozaidtvec[i].mid_sozai_, sozaidtvec[i].rarity_, sozaidtvec[0].rarity_);
      //個体値
      s32 revpow = 0;
      s32 revspin = 0;
      pmgEO_->mgCoM_.GetRevPowSpin(sozaidtvec[i], &revpow, &revspin);
      totalrevpow += revpow;
      totalrevspin += revspin;
    }

    //基礎値
    s32 basepow = 0;
    s32 basespin = 0;
    //pmgEO_->mgCoM_.GetBasePowSpin(sozaidtvec[0], &basepow, &basespin, TRUE);
	pmgEO_->mgCoM_.GetBasePowSpin(sozaidtvec[0], &basepow, &basespin, FALSE);

    DrawPowSpin(sozaidtvec[0].rarity_, basepow, basespin, totalrevpow, totalrevspin, TRUE);
  }
  void TUGShot::SetSlot(s32 slotno, s64 mid_sozai, s32 rarity, s32 baserarity)
  {
    slot_[slotno]->g_SetDraw(TRUE);
	if (mid_sozai >= 0)
	{
		// ベースの素材は文字色赤
		if (slotno == 0)
		{
			//s32 fontCol = FC2_rareD + baserarity;
			slot_[slotno]->f_MakeFont_Len(pmgEO_->mgDt_.dtShElm_.GetDtStr(mid_sozai, szs_one).c_str(), FC2_red, 1);
		}
		else
		{
			slot_[slotno]->f_MakeFont_Len(pmgEO_->mgDt_.dtShElm_.GetDtStr(mid_sozai, szs_one).c_str(), FC2_normal, 1);
		}
	}
	else
	{
		slot_[slotno]->f_MakeFont_Len("？", FC2_normal, 1);
	}
    slot_[slotno]->f_FontPos(7, 0);
    slot_[slotno]->g_SetPartNo(ugr_Ball + rarity);

    s32 BX = 40;
    s32 DX = 16;
    switch (baserarity)
    {
    case 0:DX = 48;  break;
    case 1:DX = 24;  break;
    }

    switch (baserarity)
    {
    case 3:case 4: case 5: BX -= 8; break;
    case 6:BX -= 24;  break;
    }
	
	/*
	BCA45678　という順番で
	軌道が現れることが多いので
	現状の
	 0   1   2   3   4   5   6   7
	ベ　１　２　３　４　５　６　７

	から
	１　２　べ　３　４　５　６　７
	という順番で　ボールを　配置するように変更していただけると
	*/
	s32 pos = slotno;
	switch (slotno)
	{
	case 0: // 【ベ】 0→2
		// レアリティＤの時は枠が2つしかないのでその時は1にする
		if (baserarity == 0)
		{
			pos = 1;
		}
		else
		{
			pos = 2;
		}
		break;
	case 1: // 【１】 1→0
		pos = 0;
		break;
	case 2: // 【２】 2→1
		pos = 1;
		break;
	}

    slot_[slotno]->z_SetPos(BX + (pos * DX), 47);//レアリティによって位置が変わる
	//slot_[slotno]->z_SetPos(BX + (slotno * DX), 47);//レアリティによって位置が変わる

  }
  void TUGShot::SetShotDt(s64 id_shot, s64 id_team, BOOL first_f, TSortShot* sortshot)
  {
    //ユーザーシュートＩＤを持ってくることにより
    //そこから名前や既に合成されてるシュートを引き出す
    //合成結果カードに関しては、まだ出来てないため、直に値をうちこんでカードを生成する
    if ((first_f == FALSE) && (stShot_.id_shot_ == id_shot)) return;
    stShot_.id_shot_ = id_shot;
    stShot_.id_team_ = id_team;
    stShot_.tipsmade_f_ = FALSE;

    ClearSlot();
    //素材のＭＩＤとレアリティ
    //TSozaiData sozaidt_base = pmgEO_->mgCoU_.GetSozaiData(stShot_.id_shot_, 0);
    SOZAIDTVECTOR sozaidtvec = pmgEO_->mgCoU_.GetSozaiDataAll(stShot_.id_shot_);
    //カードカラー
    field_->g_SetPartNo(sozaidtvec[0].rarity_);
    //素材名
    ugname_.SetNameShot(stShot_.id_shot_, FALSE);
    ugname_.SetIcon(FALSE);
    uggenre_.SetIconSozaiGenre(sozaidtvec[0].mid_sozai_);
    //サブ名
    ugname2_.SetNameShotSub(stShot_.id_shot_);
    //質
    ugqtpow_.Refresh(sortshot->indvpow_);
    ugqtspin_.Refresh(sortshot->indvspin_);

    //ロックアイコン
    uglock_.SetDraw(sortshot->lock_f_);

    //詳細
    if (first_f) RefreshTips();
  }

  void TUGShot::DrawPowSpin(s32 rarity, s32 basepow, s32 basespin, s32 revpow, s32 revspin, BOOL indv_f)
  {
    if (indv_f)
    {
      ugbpow_.Refresh(basepow, rarity, revpow, TRUE);
      ugbspin_.Refresh(basespin, rarity, revspin, TRUE);
    }
    else
    {
      ugbpow_.Refresh(basepow, rarity);
      ugbspin_.Refresh(basespin, rarity);
    }
    ugbpow_.SetDraw(TRUE);
    ugbspin_.SetDraw(TRUE);
  }

  void TUGShot::SetShotDtSell(s64 id_shot)
  {
    //ユーザーシュートＩＤを持ってくることにより
    //そこから名前や既に合成されてるシュートを引き出す
    //合成結果カードに関しては、まだ出来てないため、直に値をうちこんでカードを生成する
    //if ((first_f == FALSE) && (stShot_.id_shot_ == shotid)) return;
    stShot_.id_shot_ = id_shot;
    ClearSlot();

    //素材のＭＩＤとレアリティ
    SOZAIDTVECTOR sozaidtvec = pmgEO_->mgCoU_.GetSozaiDataAll(stShot_.id_shot_);
    //カードカラー
    field_->g_SetPartNo(sozaidtvec[0].rarity_);
    //素材名
    ugname_.SetNameShot(stShot_.id_shot_, FALSE);
    ugname_.SetIcon(FALSE);
    uggenre_.SetIconSozaiGenre(sozaidtvec[0].mid_sozai_);
    //サブ名
    ugname2_.SetNameShotSub(stShot_.id_shot_);
    //質
    ugqtpow_.Refresh(sozaidtvec[0].indvpow_);
    ugqtspin_.Refresh(sozaidtvec[0].indvspin_);

    stShot_.tipsdraw_f_ = TRUE;

    s32 totalrevpow = 0;
    s32 totalrevspin = 0;
    //ベース＋合成されたボール
    for (s32 i = 0; i < sozaidtvec.size(); ++i)
    {
      SetSlot(i, sozaidtvec[i].mid_sozai_, sozaidtvec[i].rarity_, sozaidtvec[0].rarity_);
      //個体値
      s32 revpow = 0;
      s32 revspin = 0;
      pmgEO_->mgCoM_.GetRevPowSpin(sozaidtvec[i], &revpow, &revspin);
      totalrevpow += revpow;
      totalrevspin += revspin;
    }
    //基礎値
    s32 basepow = 0;
    s32 basespin = 0;
    //pmgEO_->mgCoM_.GetBasePowSpin(sozaidtvec[0], &basepow, &basespin, TRUE);
	pmgEO_->mgCoM_.GetBasePowSpin(sozaidtvec[0], &basepow, &basespin, FALSE);

    DrawPowSpin(sozaidtvec[0].rarity_, basepow, basespin, totalrevpow, totalrevspin, TRUE);

    //裏面にデータ送る
    if (ura_ != NULL) ura_->SetShotUraDt(id_shot);
  }
  s32 TUGShot::SetShotDtGousei(LONGVECTOR* id_shot_mixvec)
  {
    s32 res = 0;
    //ユーザーシュートＩＤを持ってくることにより
    //そこから名前や既に合成されてるシュートを引き出す
    //合成結果カードに関しては、まだ出来てないため、直に値をうちこんでカードを生成する
    //if ((first_f == FALSE) && (stShot_.id_shot_ == shotid)) return;
    stShot_.id_shot_ = id_shot_mixvec->at(0);
    ClearSlot();
    //素材のＭＩＤとレアリティ
    SOZAIDTVECTOR basedtvec = pmgEO_->mgCoU_.GetSozaiDataAll(stShot_.id_shot_);
    //カードカラー
    field_->g_SetPartNo(basedtvec[0].rarity_);
    //素材名
    ugname_.SetNameShot(stShot_.id_shot_, FALSE);
    ugname_.SetIcon(FALSE);
    uggenre_.SetIconSozaiGenre(basedtvec[0].mid_sozai_);
    //サブ名
    ugname2_.SetNameShotSub(stShot_.id_shot_);
    //質
    ugqtpow_.Refresh(basedtvec[0].indvpow_);
    ugqtspin_.Refresh(basedtvec[0].indvspin_);

    uglock_.SetDraw(FALSE);

    stShot_.tipsdraw_f_ = TRUE;

    //shotinvno_->g_SetDraw(FALSE);
    //body_->g_SetDraw(FALSE);

    s32 totalrevpow = 0;
    s32 totalrevspin = 0;
    s32 slotno = 0;

    for (auto& id_shot : *id_shot_mixvec)
    {
      //素材のＭＩＤとレアリティ
      SOZAIDTVECTOR sozaivec = pmgEO_->mgCoU_.GetSozaiDataAll(id_shot);
      for (auto& sozai : sozaivec)
      {
        SetSlot(slotno, sozai.mid_sozai_, sozai.rarity_, basedtvec[0].rarity_);
        //個体値
        s32 revpow = 0;
        s32 revspin = 0;
        pmgEO_->mgCoM_.GetRevPowSpin(sozai, &revpow, &revspin);
        totalrevpow += revpow;
        totalrevspin += revspin;
        ++slotno;
      }
    }

    //基礎値
    s32 basepow = 0;
    s32 basespin = 0;
	//pmgEO_->mgCoM_.GetBasePowSpin(basedtvec[0], &basepow, &basespin, TRUE);
    pmgEO_->mgCoM_.GetBasePowSpin(basedtvec[0], &basepow, &basespin, FALSE);

    DrawPowSpin(basedtvec[0].rarity_, basepow, basespin, totalrevpow, totalrevspin, TRUE);

    //あきスロット
    res = ((basedtvec[0].rarity_ + 2) - slotno);

    //裏面にデータ送る
    if (ura_ != NULL) ura_->SetShotUraDt(id_shot_mixvec);

    return res;
  }

  s32 TUGShot::SetShotDtGousei(SOZAIDTVECTOR id_shot_mixvec)
  {
	  s32 res = 0;
	  //ユーザーシュートＩＤを持ってくることにより
	  //そこから名前や既に合成されてるシュートを引き出す
	  //合成結果カードに関しては、まだ出来てないため、直に値をうちこんでカードを生成する
	  //if ((first_f == FALSE) && (stShot_.id_shot_ == shotid)) return;
	  stShot_.id_shot_ = id_shot_mixvec[0].GetSozaiR();
	  ClearSlot();
	  //カードカラー
	  field_->g_SetPartNo(id_shot_mixvec[0].rarity_);
	  //素材名
	  ugname_.SetNameSozai2(stShot_.id_shot_, FALSE);
	  ugname_.SetIcon(FALSE);
	  uggenre_.SetIconSozaiGenre(id_shot_mixvec[0].mid_sozai_);
	  //サブ名
	  ugname2_.SetNameShotSub(id_shot_mixvec[0]);
	  //質
	  ugqtpow_.Refresh(id_shot_mixvec[0].indvpow_);
	  ugqtspin_.Refresh(id_shot_mixvec[0].indvspin_);

	  uglock_.SetDraw(FALSE);

	  stShot_.tipsdraw_f_ = TRUE;

	  //shotinvno_->g_SetDraw(FALSE);
	  //body_->g_SetDraw(FALSE);

	  s32 totalrevpow = 0;
	  s32 totalrevspin = 0;
	  s32 slotno = 0;

	  for (s32 i = 0; i < id_shot_mixvec.size(); i++)
	  {
		  TSozaiData sozai = id_shot_mixvec[i];
		  SetSlot(slotno, sozai.mid_sozai_, sozai.rarity_, id_shot_mixvec[0].rarity_);
		  //個体値
		  s32 revpow = 0;
		  s32 revspin = 0;
		  pmgEO_->mgCoM_.GetRevPowSpin(sozai, &revpow, &revspin);
		  totalrevpow += revpow;
		  totalrevspin += revspin;
		  ++slotno;
	  }

	  //基礎値
	  s32 basepow = 0;
	  s32 basespin = 0;
	  //pmgEO_->mgCoM_.GetBasePowSpin(id_shot_mixvec[0], &basepow, &basespin, TRUE);
	  pmgEO_->mgCoM_.GetBasePowSpin(id_shot_mixvec[0], &basepow, &basespin, FALSE);

	  DrawPowSpin(id_shot_mixvec[0].rarity_, basepow, basespin, totalrevpow, totalrevspin, TRUE);

	  //あきスロット
	  res = ((id_shot_mixvec[0].rarity_ + 2) - slotno);

	  //裏面にデータ送る
	  if (ura_ != NULL) ura_->SetShotUraDt(id_shot_mixvec);

	  return res;
  }

  void TUGShot::SetShotDtBaraLast(s64 id_shot_base)
  {
    stShot_.id_shot_ = id_shot_base;
    ClearSlot();
    //素材のＭＩＤとレアリティ
    SOZAIDTVECTOR sozaidtvec = pmgEO_->mgCoU_.GetSozaiDataAll(stShot_.id_shot_);
    //カードカラー
    field_->g_SetPartNo(sozaidtvec[0].rarity_);
    //素材名
    ugname_.SetNameShot(stShot_.id_shot_, FALSE);
    ugname_.SetIcon(FALSE);
    uggenre_.SetIconSozaiGenre(sozaidtvec[0].mid_sozai_);
    //サブ名
    ugname2_.SetNameShotSub(stShot_.id_shot_);
    //質
    ugqtpow_.Refresh(sozaidtvec[0].indvpow_);
    ugqtspin_.Refresh(sozaidtvec[0].indvspin_);
    uglock_.SetDraw(FALSE);

    stShot_.tipsdraw_f_ = TRUE;

    //shotinvno_->g_SetDraw(FALSE);
    //body_->g_SetDraw(FALSE);
    for (s32 i = 0; i < HSELEMMAX; ++i) slot_[i]->g_SetDraw(FALSE);
    s32 useslot = pmgEO_->mgCoU_.mdShot_.GetPKDt(id_shot_base, mdshot_useslot);

    s32 totalrevpow = 0;
    s32 totalrevspin = 0;
    //ベース＋合成されたボール
    for (s32 i = 0; i < (sozaidtvec.size() - 1); ++i)
    {
      SetSlot(i, sozaidtvec[i].mid_sozai_, sozaidtvec[i].rarity_, sozaidtvec[0].rarity_);
      //個体値
      s32 revpow = 0;
      s32 revspin = 0;
      pmgEO_->mgCoM_.GetRevPowSpin(sozaidtvec[i], &revpow, &revspin);
      totalrevpow += revpow;
      totalrevspin += revspin;
    }

    //基礎値
    s32 basepow = 0;
    s32 basespin = 0;
    //pmgEO_->mgCoM_.GetBasePowSpin(sozaidtvec[0], &basepow, &basespin, TRUE);
	pmgEO_->mgCoM_.GetBasePowSpin(sozaidtvec[0], &basepow, &basespin, FALSE);

    DrawPowSpin(sozaidtvec[0].rarity_, basepow, basespin, totalrevpow, totalrevspin, TRUE);


    //裏面にデータ送る
    if (ura_ != NULL) ura_->SetShotUraDt(id_shot_base);
  }
  void TUGShot::SetShotDtSozai(TSozaiData sozaidt)
  {
    stShot_.id_shot_ = NGNUM;
    stShot_.id_team_ = NGNUM;
    ClearSlot();
    //カードカラー
    field_->g_SetPartNo(sozaidt.rarity_);
    //素材名
    ugname_.SetNameSozai(sozaidt.mid_sozai_, sozaidt.rarity_, FALSE);
    ugname_.SetIcon(FALSE);
    uggenre_.SetIconSozaiGenre(sozaidt.mid_sozai_);
    //サブ名
    ugname2_.SetNameSozaiSub(sozaidt.mid_sozai_, sozaidt.rarity_);
    //質
    ugqtpow_.Refresh(sozaidt.indvpow_);
    ugqtspin_.Refresh(sozaidt.indvspin_);
    uglock_.SetDraw(FALSE);

    stShot_.tipsdraw_f_ = TRUE;

    //基礎値
    s32 basepow = 0;
    s32 basespin = 0;
    //pmgEO_->mgCoM_.GetBasePowSpin(sozaidt, &basepow, &basespin, TRUE);
	pmgEO_->mgCoM_.GetBasePowSpin(sozaidt, &basepow, &basespin, FALSE);

    BOOL indv_f = FALSE;
    s32 revpow = 0;
    s32 revspin = 0;
    if (sozaidt.indvpow_ > 0)
    {
      pmgEO_->mgCoM_.GetRevPowSpin(sozaidt, &revpow, &revspin);
      indv_f = TRUE;
    }
    DrawPowSpin(sozaidt.rarity_, basepow, basespin, revpow, revspin, indv_f);
    //ベース
    SetSlot(0, sozaidt.mid_sozai_, sozaidt.rarity_, sozaidt.rarity_);

  }

  //表面詳細
  void TUGShot::SetTipsDraw_f(BOOL tipsdraw_f)
  {
    stShot_.tipsdraw_f_ = tipsdraw_f;
	if (stShot_.tipsmade_f_ == FALSE && tipsdraw_f)
    {
      stShot_.tipsmade_f_ = TRUE;
      RefreshTips();
    }
  }
  void TUGShot::SetItemGet(s64 itemid, s64 pow, s64 spin)
  {
    TSozaiData sozaidt;
    sozaidt.Init();
    sozaidt.mid_sozai_ = (itemid / SOZAIMID);
    sozaidt.rarity_ = abs(itemid % SOZAIMID);
    sozaidt.indvpow_ = pow;
    sozaidt.indvspin_ = spin;
    SetShotDtSozai(sozaidt);//とりあえず個体値０
    SetDraw(TRUE);
    if (ura_ != NULL) ura_->SetShotUraDtBuy(itemid);
    //field_->z_SetPos(8, -56);
    //shotinvno_->g_SetDraw(FALSE);
  }
  void TUGShot::SetLRPos(BOOL leftpos_f)
  {
    const s32 POSLX = -160;
    const s32 POSRX = 8;
    //const s32 POSY = -64;
    const s32 POSY = -44;

    s32 posx = (leftpos_f)
      ? POSLX
      : POSRX;

    field_->z_SetPos(posx, POSY);
  }
  void TUGShot::DoUpdate(BOOL draw_f)
  {
    if (stShot_.urano_ > 0)
    {
      if (ura_ != NULL) ura_->Update(draw_f);
    }
    else
    {
      field_->Update(draw_f);
      ugname_.Update(draw_f);
      uglock_.Update(draw_f);
      uggenre_.Update(draw_f);

      ugqtpow_.Update(draw_f);
      ugqtspin_.Update(draw_f);

      if (stShot_.tipsdraw_f_)
      {
        ugbpow_.Update(draw_f);
        ugbspin_.Update(draw_f);
        //revpower_->Update(draw_f);
        //revspin_->Update(draw_f);
        ugname2_.Update(draw_f);
        //body_->Update(draw_f);
        for (s32 i = 0; i < HSELEMMAX; ++i) slot_[i]->Update(draw_f);
      }
    }
  }
  void TUGShot::DoAction()//更新
  {
  }
}
