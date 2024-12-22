#include "dbUGRRarity.h"
#include "dbUILayer.h"

namespace db
{
  const s32 NBX = 18;
  const s32 NBY = 6;
  //コンストラクタ
  TUGRRarity::TUGRRarity(TUIGenLayer* player) : TUGGen(player)//アクションなし
  {
    stRRarity_.Init();
  }

  TUGRRarity::~TUGRRarity()
  {

  }
  void TUGRRarity::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);

    //rare_->z_SetPos(RARITYX_TEAM, RARITYY);
    field_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_item));
    field_->g_SetDraw(TRUE);

    lvStr_ = MakeObj();
    lvStr_->o_SetParent(field_);
    lvStr_->f_MakeFont_Mini("ＬＶ", FC2_mini);
    lvStr_->z_SetPos(NBX, NBY);
    lvStr_->g_SetDraw(TRUE);

	lvNum_ = MakeObj();
	lvNum_->o_SetParent(field_);
	lvNum_->f_MakeFont_Mini("１", FC2_mini);
	lvNum_->z_SetPos(NBX, NBY + 7);
	lvNum_->g_SetDraw(TRUE);
  }


  void TUGRRarity::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    if (stRRarity_.num_f_)
    {
      lvStr_->Update(draw_f);
	  lvNum_->Update(draw_f);
    }
  }
  void TUGRRarity::SetLimY(s32 limu, s32 hhh)
  {
    field_->g_SetLimY(limu, hhh);
    lvStr_->g_SetLimY(limu, hhh);
	lvNum_->g_SetLimY(limu, hhh);
  }

  void TUGRRarity::Refresh(s32 rarity)
  {
    stRRarity_.num_f_ = FALSE;
    field_->g_SetPartNo((enUGPIcon)(ugi_RarityD + rarity));
    field_->g_SetDraw(TRUE);
  }
  void TUGRRarity::Refresh(s32 rarity, s32 num, BOOL drawzero_f)
  {
    Refresh(rarity);

    stRRarity_.num_f_ = TRUE;
	if(num < 1)
	{
		lvStr_->g_SetDraw(FALSE);
		lvNum_->g_SetDraw(FALSE);
		return;
	}
	lvStr_->g_SetDraw(TRUE);
	lvNum_->f_MakeFont_Mini(lib_str::IntToStr(num).c_str(), FC2_mini);
	lvNum_->g_SetDraw(TRUE);
  }
  void TUGRRarity::RefreshInv(s32 rarity)
  {
    Refresh(rarity);

    stRRarity_.num_f_ = TRUE;

	lvStr_->g_SetDraw(TRUE);
	lvNum_->f_MakeFont_Mini("？？？", FC2_mini);
	lvNum_->g_SetDraw(TRUE);
  }
}
