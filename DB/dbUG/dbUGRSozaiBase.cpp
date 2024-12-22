#include "dbUGRSozaiBase.h"
#include "dbUILayer.h"

namespace db
{
  const s32 KETA = 4;
  const s32 NBX = -3;// 40;
  const s32 NBY = 12;
  //コンストラクタ
  TUGRSozaiBase::TUGRSozaiBase(TUIGenLayer* player) : TUGGen(player)//アクションなし
  {
    stRSozaiBase_.Init();
  }

  TUGRSozaiBase::~TUGRSozaiBase()
  {

  }
  void TUGRSozaiBase::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);
    field_->g_SetDraw(TRUE);
    //field_->f_SetRightShift();

    sign_ = MakeObj();
    sign_->o_SetParent(field_);
    sign_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_level_num));
    sign_->g_SetDraw(TRUE);

    for (s32 i = 0; i < KETA; ++i)
    {
      TUIObj* newnum = MakeObj();
      baseNumvec_.push_back(newnum);
      newnum->o_SetParent(field_);
      newnum->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_level_num));
      //newnum->z_SetPos((DX * i), 0);
      //newnum->g_SetDraw(TRUE);

	  TUIObj* newnum2 = MakeObj();
	  revNumvec_.push_back(newnum2);
	  newnum2->o_SetParent(field_);
	  newnum2->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_level_num));
    }
  }


  void TUGRSozaiBase::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    if (stRSozaiBase_.num_f_)
    {
      sign_->Update(draw_f);
	  for (s32 i = 0; i < baseNumvec_.size(); ++i)
      {
		  baseNumvec_[i]->Update(draw_f);
		  revNumvec_[i]->Update(draw_f);
      }
    }
  }
  void TUGRSozaiBase::SetLimY(s32 limu, s32 hhh)
  {
    field_->g_SetLimY(limu, hhh);
    sign_->g_SetLimY(limu, hhh);
	for (s32 i = 0; i < baseNumvec_.size(); ++i)
    {
		baseNumvec_[i]->g_SetLimY(limu, hhh);
		revNumvec_[i]->g_SetLimY(limu, hhh);
    }
  }

  void TUGRSozaiBase::Refresh(s32 basedt, s32 rarity)
  {
	  // 数値によって文字を出す仕様から
	  // 数値そのものを表示する仕様に変更
	  if (basedt < 0)
	  {
		  field_->f_MakeFont("負負負負", FC2_rareD + rarity);
		  field_->g_SetDraw(TRUE);
          sign_->g_SetDraw(FALSE);
          for (s32 i = 0; i < baseNumvec_.size(); ++i)
          {
              baseNumvec_[i]->g_SetDraw(FALSE);
              revNumvec_[i]->g_SetDraw(FALSE);
          }
	  }
	  else
	  {
		  std::string str = lib_str::IntToStr(basedt);
		  field_->f_MakeFont(str.c_str(), FC2_rareD + rarity);
		  field_->g_SetDraw(TRUE);
	  }
	  return;


    stRSozaiBase_.num_f_ = FALSE;
    const std::string BSNAME[] = {
      "さいてー心",
      "ちょよわ",
      "よわい",
      "ふつう",
      "つよい",
      "ちょつよ",
      "さいきょう",
    };
	if (basedt < 0)
	{
		assert(basedt < RARITYNUM);
		assert(rarity >= 0);
		assert(rarity < RARITYNUM);

		field_->f_MakeFont("負負負負", FC2_rareD + rarity);
		field_->g_SetDraw(TRUE);
		return;
	}
    assert(basedt >= 0);
    assert(basedt < RARITYNUM);
    assert(rarity >= 0);
    assert(rarity < RARITYNUM);

    field_->f_MakeFont(BSNAME[basedt].c_str(), FC2_rareD + rarity);
    field_->g_SetDraw(TRUE);
  }
  void TUGRSozaiBase::Refresh(s32 basedt, s32 rarity, s32 num, BOOL drawzero_f)
  {
    Refresh(basedt + num, rarity);

	// ベースがマイナス値なのは秘密シュートなので何も表示しない
	if (basedt < 0)
	{
		sign_->g_SetDraw(FALSE);
		for (s32 i = 0; i < baseNumvec_.size(); ++i)
		{
			baseNumvec_[i]->g_SetDraw(FALSE);
			revNumvec_[i]->g_SetDraw(FALSE);
		}
		return;
	}


	stRSozaiBase_.num_f_ = TRUE;

	// 補正値用の座標
	s32 NBX2 = 0;

	// ベース値
	{
		BOOL keta1000_f = (basedt >= 1000);
		BOOL keta100_f = (basedt >= 100);
		BOOL keta10_f = (basedt >= 10);

		//各数字
		baseNumvec_[0]->g_SetPartNo(abs(basedt / 1000));
		baseNumvec_[0]->g_SetDraw(keta1000_f);
		baseNumvec_[1]->g_SetPartNo(abs((basedt / 100) % 10));
		baseNumvec_[1]->g_SetDraw(keta100_f);
		baseNumvec_[2]->g_SetPartNo(abs((basedt / 10) % 10));
		baseNumvec_[2]->g_SetDraw(keta10_f);
		baseNumvec_[3]->g_SetPartNo(abs(basedt % 10));
		baseNumvec_[3]->g_SetDraw(TRUE);

		//ケタずらし
		const s32 DX = 4;
		s32 basex = NBX + DX;
		if (!keta1000_f) basex -= DX;
		if (!keta100_f) basex -= DX;
		if (!keta10_f) basex -= DX;

		for (s32 i = 0; i < KETA; ++i)
		{
			baseNumvec_[i]->z_SetPos(basex + (DX * i), NBY);
		}

		// 補正値の座標決定
		if (keta1000_f) { NBX2 = basex + (DX * KETA); }
		else if (keta100_f) { NBX2 = basex + (DX * KETA - 1); }
		else if (keta10_f) { NBX2 = basex + (DX * KETA - 2); }
		else { NBX2 = basex + (DX * KETA - 3); }

		NBX2 += DX - 2;	// 現在が最後の桁位置なのでその次の文字位置にずらす
						// 今の位置より2ドット左に、との要望
	}

	// 補正値
	{
		BOOL keta1000_f = (num >= 1000);
		BOOL keta100_f = (num >= 100);
		BOOL keta10_f = (num >= 10);

		//各数字
		revNumvec_[0]->g_SetPartNo(abs(num / 1000));
		revNumvec_[0]->g_SetDraw((num >= 1000));
		revNumvec_[1]->g_SetPartNo(abs((num / 100) % 10));
		revNumvec_[1]->g_SetDraw((num >= 100));
		revNumvec_[2]->g_SetPartNo(abs((num / 10) % 10));
		revNumvec_[2]->g_SetDraw((num >= 10));
		revNumvec_[3]->g_SetPartNo(abs(num % 10));
		revNumvec_[3]->g_SetDraw(drawzero_f || (num != 0));

		//ケタずらし
		const s32 DX = 4;
		s32 basex = NBX2 + DX;
		if (!keta1000_f) basex -= DX;
		if (!keta100_f) basex -= DX;
		if (!keta10_f) basex -= DX;

		for (s32 i = 0; i < KETA; ++i)
		{
			revNumvec_[i]->z_SetPos(basex + (DX * i), NBY);
		}
		//符号
		s32 sineno = (num < 0)
			? 11
			: 10;
		sign_->g_SetPartNo(sineno);
		sign_->g_SetDraw((num != 0));
		sign_->z_SetPos(NBX2, NBY - 1);// -7);
	}
  }
  void TUGRSozaiBase::RefreshInv(s32 basedt, s32 rarity)
  {
    Refresh(basedt, rarity);

    stRSozaiBase_.num_f_ = TRUE;

    //各数字
    baseNumvec_[0]->g_SetDraw(FALSE);
	baseNumvec_[1]->g_SetDraw(FALSE);
	baseNumvec_[2]->g_SetDraw(FALSE);
	baseNumvec_[3]->g_SetPartNo(12);//?表示
	baseNumvec_[3]->g_SetDraw(TRUE);

    //ケタずらし
    const s32 DX = 4;
    s32 basex = NBX - (DX * 3);
    for (s32 i = 0; i < KETA; ++i)
    {
		baseNumvec_[i]->z_SetPos(basex + (DX * i), NBY);
    }
    //符号
    s32 sineno = 10;
    sign_->g_SetPartNo(sineno);
    sign_->g_SetDraw(TRUE);
  }
}
