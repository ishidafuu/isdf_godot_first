#include "dbUGShotUra.h"
#include "dbUILayer.h"

namespace db
{
  const s32 TEXTNUM = 4;
  //コンストラクタ
  TUGShotUra::TUGShotUra(TUIGenLayer* player) :TUGGen(player)
  {
    stShotUra_.Init();
  }

  TUGShotUra::~TUGShotUra()
  {
    for (s32 i = 0; i < uggenre_.size(); ++i) Freemem(uggenre_[i]);
    for (s32 i = 0; i < ugpow_.size(); ++i) Freemem(ugpow_[i]);
    for (s32 i = 0; i < ugspin_.size(); ++i) Freemem(ugspin_[i]);
  }
  void TUGShotUra::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);

    field_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_shot_card2));
    field_->g_SetDepth(UIDP_02BGOBJHI);
    for (s32 i = 0; i < HSELEMMAX_ADD; ++i)
    {
      slotname_[i] = MakeObj();
      slotname_[i]->o_SetParent(field_);
      slotname_[i]->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_shot_miniball));
    }

    const s32 QBX = 88;
    const s32 QBY = -1;
    const s32 QDX = 28;
    for (s32 i = 0; i < HSELEMMAX_ADD; ++i)
    {
      TUGRQuality* newqt = new TUGRQuality(player_);
      ugpow_.push_back(newqt);
      newqt->MakeGroup(slotname_[i], FALSE, FALSE);
      newqt->SetPos(QBX, QBY);
    }

    for (s32 i = 0; i < HSELEMMAX_ADD; ++i)
    {
      TUGRQuality* newqt = new TUGRQuality(player_);
      ugspin_.push_back(newqt);
      newqt->MakeGroup(slotname_[i], TRUE, FALSE);
      newqt->SetPos(QBX + QDX, QBY);
    }

    for (s32 i = 0; i < TEXTNUM; ++i)
    {
      text_[i] = MakeObj();
      text_[i]->o_SetParent(field_);
    }

    const s32 BX = 22;
    const s32 BY = 80;
    const s32 DX = 15;
    const s32 DX2 = 3;
    for (s32 i = 0; i < GENRENUM; ++i)
    {
      TUGRIcon* newicon = new TUGRIcon(player_);
      newicon->MakeGroup(field_);
      s32 posx = BX + (i * DX);
      if (i >3) posx += DX2;
      newicon->SetPos(posx, BY);

      uggenre_.push_back(newicon);
    }

    basename_ = MakeObj();
    basename_->o_SetParent(field_);
  }

  void TUGShotUra::ClearShotName()
  {
    for (s32 i = 0; i < HSELEMMAX_ADD; ++i)slotname_[i]->g_SetDraw(FALSE);
    for (s32 i = 0; i < TEXTNUM; ++i) text_[i]->g_SetDraw(FALSE);
    basename_->g_SetDraw(FALSE);
  }

  void TUGShotUra::SetSlot(s32 slotno, const TSozaiData& sozai, s32 baserarity)
  {
    ugpow_[slotno]->Refresh(sozai.indvpow_, sozai.mid_sozai_, sozai.rarity_);
    ugspin_[slotno]->Refresh(sozai.indvspin_, sozai.mid_sozai_, sozai.rarity_);

    enDtShElmStr fontcol = (enDtShElmStr)(FC2_rareD + sozai.rarity_);
    slotname_[slotno]->g_SetDraw(TRUE);
    slotname_[slotno]->f_MakeFont(pmgEO_->mgDt_.dtShElm_.GetDtStr(sozai.mid_sozai_, szs_name).c_str(), fontcol);
    slotname_[slotno]->f_FontPos(8, -4);
    slotname_[slotno]->g_SetPartNo(sozai.rarity_);

    s32 BY = 8;
    s32 DY = 12;
    switch (baserarity)
    {
    case 0: BY = 44; DY = 0; break;
    case 1: BY = 32; DY = 24; break;
    case 2: BY = 20; DY = 24; break;
    case 3: BY = 26; break;
    case 4: BY = 20; break;
    case 5: BY = 8; break;
    case 6: BY = 8; break;
    }

    slotname_[slotno]->z_SetPos(4, BY + 1 + (slotno * DY));//レアリティによって位置が変わる
  }
  void TUGShotUra::GenUraDt(TSozaiData& sozaidt)
  {
    
    //partpos = (sozaidt.rarity_ + RARITYNUM);7
    s32 BX = 8;
    s32 BY = 8-3;
    s32 DY = 12;
    
    enDtShElmStr fontcol = (enDtShElmStr)(FC2_rareD + sozaidt.rarity_);

    basename_->f_MakeFont(pmgEO_->mgDt_.dtShElm_.GetDtStr(sozaidt.mid_sozai_, szs_name).c_str(), fontcol);
    basename_->z_SetPos(BX + 24, BY);
    basename_->g_SetDraw(TRUE);
    //basename_->f_FontPos(32, 8-4);
    STRVECTOR textvec = pmgEO_->mgDt_.dtShElm_.GetText(sozaidt.mid_sozai_);

    for (s32 i = 0; i < textvec.size(); ++i)
    {
      text_[i]->g_SetDraw(TRUE);
      text_[i]->f_MakeFont(textvec[i].c_str(), fontcol);
      //text_[i]->f_FontPos(8, -4);
      text_[i]->z_SetPos(BX, BY + ((i + 1) * DY));
    }
  }
  void TUGShotUra::SetShotUraDt(s32 id_shot)
  {
    ClearShotName();
    SOZAIDTVECTOR sozaidtvec = pmgEO_->mgCoU_.GetSozaiDataAll(id_shot);
    stShotUra_.rarity_ = sozaidtvec[0].rarity_;
    stShotUra_.mixed_f_ = (pmgEO_->mgCoU_.mdShot_.GetPKDt(id_shot, mdshot_useslot) > 0);
    TSozaiData sozaidt = pmgEO_->mgCoU_.GetSozaiData(id_shot, 0);
    GenUraDt(sozaidt);

    //合成されたボール
    for (s32 i = 0; i < sozaidtvec.size(); ++i)
    {
      if (i != 0) SetSlot((i - 1), sozaidtvec[i], sozaidtvec[0].rarity_);
    }
    for (s32 i = 0; i < uggenre_.size(); ++i)
    {
      uggenre_[i]->SetIconSozaiGenreSub(sozaidtvec[0].mid_sozai_, i);
    }
  }

  void TUGShotUra::SetShotUraDt(LONGVECTOR* id_shotvec)
  {
    ClearShotName();
    s32 mid_sozai = pmgEO_->mgCoU_.mdShot_.GetPKDt(id_shotvec->at(0), mdshot_id_m_sozai0);
    stShotUra_.rarity_ = (mid_sozai % SOZAIMID);
    stShotUra_.mixed_f_ = (id_shotvec->size() > 1);
    TSozaiData sozaidt = pmgEO_->mgCoU_.GetSozaiData(id_shotvec->at(0), 0);
    GenUraDt(sozaidt);

    s32 slotno = 0;
    for (auto& id_shot : *id_shotvec)
    {
      SOZAIDTVECTOR sozaivec = pmgEO_->mgCoU_.GetSozaiDataAll(id_shot);
      for (auto& sozai : sozaivec)
      {
        if (slotno != 0) SetSlot(slotno - 1, sozai, stShotUra_.rarity_);
        ++slotno;
      }
    }

    for (s32 i = 0; i < uggenre_.size(); ++i)
    {
      uggenre_[i]->SetIconSozaiGenreSub(mid_sozai / SOZAIMID, i);
    }

  }
  void TUGShotUra::SetShotUraDt(SOZAIDTVECTOR id_shotvec)
  {
	  ClearShotName();
	  s32 mid_sozai = id_shotvec[0].GetSozaiR();
	  stShotUra_.rarity_ = (mid_sozai % SOZAIMID);
	  stShotUra_.mixed_f_ = (id_shotvec.size() > 1);
	  TSozaiData sozaidt = id_shotvec[0];
	  GenUraDt(sozaidt);

	  s32 slotno = 0;


		for (auto& sozai : id_shotvec)
		{
			if (slotno != 0) SetSlot(slotno - 1, sozai, stShotUra_.rarity_);
			++slotno;
		}

	  for (s32 i = 0; i < uggenre_.size(); ++i)
	  {
		  uggenre_[i]->SetIconSozaiGenreSub(mid_sozai / SOZAIMID, i);
	  }

  }
  void TUGShotUra::SetShotUraDtBuy(s32 mid_shot)
  {
    ClearShotName();
    stShotUra_.rarity_ = abs(mid_shot % SOZAIMID);
    stShotUra_.mixed_f_ = FALSE;
    TSozaiData sozaidt;
    sozaidt.Init();
    sozaidt.mid_sozai_ = abs(mid_shot / SOZAIMID);
    sozaidt.rarity_ = abs(mid_shot % SOZAIMID);
    sozaidt.indvpow_ = NGNUM;
    sozaidt.indvspin_ = NGNUM;

    GenUraDt(sozaidt);

    ////合成されたボール
    //for (s32 i = 0; i < sozaidtvec.size(); ++i)
    //{
    //  if (i != 0) SetSlot((i - 1), sozaidtvec[i], sozaidtvec[0].rarity_);
    //}
    for (s32 i = 0; i < uggenre_.size(); ++i)
    {
      uggenre_[i]->SetIconSozaiGenreSub(sozaidt.mid_sozai_, i);
    }
  }

  void TUGShotUra::SetUraNo(s32 urano)
  {
    stShotUra_.urano_ = urano;
    s32 partpos = stShotUra_.rarity_;
    if (urano == 1) partpos += RARITYNUM;//せつめい
    field_->g_SetPartNo(partpos);
  }

  void TUGShotUra::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    switch (stShotUra_.urano_)
    {
    case 0:
      //表示しない
      break;
    case 1:
      basename_->Update(draw_f);
      for (s32 i = 0; i < TEXTNUM; ++i) text_[i]->Update(draw_f);
      for (auto& item : uggenre_) item->Update(draw_f);
      break;
    case 2:
      for (s32 i = 0; i < HSELEMMAX_ADD; ++i) slotname_[i]->Update(draw_f);
      for (auto& item : ugpow_) item->Update(draw_f);
      for (auto& item : ugspin_) item->Update(draw_f);
      break;
    }
  }
  void TUGShotUra::DoAction()//更新
  {

  }


}
