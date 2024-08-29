#include "dbUGShResult.h"
#include "dbUILayer.h"

namespace db
{
  enum enResultText
  {
    enResultText_Title,
    enResultText_MapName,
    enResultText_Kill,
    enResultText_Death,
    enResultText_Time,
    enResultTextEND,
  };

  //コンストラクタ
  TUGShResult::TUGShResult(TUIGenLayer* player) : TUGGen(player)//アクションなし
    //, ugrkantoku_(player)
  {
    stRProf_.Init();
  }

  TUGShResult::~TUGShResult()
  {

  }
  void TUGShResult::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);
    //field_->z_SetPos(-80, 52);
    //ugrkantoku_.MakeGroup(field_);

    //plate_ = MakeObj();
    //plate_->o_SetParent(field_);
    //plate_->o_SetObjGrpPart(pmgUI_->GetUGGrPart(ugp_prof_message));
    //plate_->z_SetPos(-32, 52);
    //plate_->f_MakeFont("かんとくからみんなへ", FC2_normal);
    //plate_->f_FontPos(0,-14);
    //plate_->g_SetDraw(TRUE);

    const s32 BX = -112;
    const s32 BY = -48;
    const s32 DX = 0;
    const s32 DY = 16;

    for (s32 i = 0; i < enResultTextEND; ++i)
    {
      TUIObj* text = MakeObj();
      text->o_SetParent(field_);
      text->z_SetPos(BX + (DX* i), BY + (DY* i));
      text->g_SetDepth(UIDP_09MSGBOX);
      text->g_SetDraw(TRUE);

      texts_.push_back(text);
    }
    texts_[enResultText_Title]->z_SetPos(BX, BY - 8);
    texts_[enResultText_Title]->f_MakeFont("心＿しあいけっか＿心", FC2_normal);

  }

  void TUGShResult::Refresh()
  {
    std::string mapname = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDtStr(pmgEO_->stSeq_.GetMidMapTip(), mdm_maptip_name_c64);
    
    std::string killstr = "たおした＿" + lib_str::IntToStr(pmgEO_->stResult_.GetKillnum()) + "にん";
    std::string deathstr = "やられた＿" + lib_str::IntToStr(pmgEO_->stResult_.GetDeathnum()) + "にん";
	std::string timestr = "タイム＿＿" + base::MinSecMSecString(pmgEO_->stResult_.GetCleattime(), TRUE);
    texts_[enResultText_MapName]->f_MakeFont(mapname.c_str(), FC2_normal);
    texts_[enResultText_Kill]->f_MakeFont(killstr.c_str(), FC2_normal);
    texts_[enResultText_Death]->f_MakeFont(deathstr.c_str(), FC2_normal);
    texts_[enResultText_Time]->f_MakeFont(timestr.c_str(), FC2_normal);
    texts_[enResultText_Time]->g_SetDraw(FALSE);//いったんけす

  }
  void TUGShResult::RefreshTimeAttack(void)
  {
	  std::string mapname = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDtStr(pmgEO_->stSeq_.GetMidMapTip(), mdm_maptip_name_c64);

	  std::string killstr = "たおした＿" + lib_str::IntToStr(pmgEO_->stResult_.GetKillnum()) + "にん";
	  std::string deathstr = "やられた＿" + lib_str::IntToStr(pmgEO_->stResult_.GetDeathnum()) + "にん";
	  std::string timestr = "タイム＿＿" + base::MinSecMSecString(pmgEO_->stResult_.GetCleattime(), FALSE);
	  texts_[enResultText_MapName]->f_MakeFont(mapname.c_str(), FC2_normal);
	  texts_[enResultText_Kill]->f_MakeFont(killstr.c_str(), FC2_normal);
	  texts_[enResultText_Death]->f_MakeFont(deathstr.c_str(), FC2_normal);
	  texts_[enResultText_Time]->f_MakeFont(timestr.c_str(), FC2_normal);
	  texts_[enResultText_Time]->g_SetDraw(TRUE);
  }


  void TUGShResult::DoUpdate(BOOL draw_f)
  {
    for (s32 i = 0; i < enResultTextEND; ++i)
    {
      texts_[i]->Update(draw_f);
    }

  }

}
