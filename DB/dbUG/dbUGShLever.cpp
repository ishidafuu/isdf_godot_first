#include "dbUGShLever.h"
#include "dbUIGenLayer.h"
#include "dbGame.h"

namespace db
{

  //コンストラクタ
  TUGShLever::TUGShLever(TUIGenLayer* player) :TUGGen(player)
  {
    stShLever_.Init();
  }

  TUGShLever::~TUGShLever()
  {
  }
  void TUGShLever::MakeGroup(TUIObj* parent, BOOL prev_f)
  {
    SetParent(parent);
    field_->g_SetDepth(UIDP_07BUTTONTOP);
    field_->g_SetCenter(TRUE);
    field_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_pad_lever));
    field_->g_SetAlp(TRUE, 128);

    if (prev_f == FALSE) 
    {
      bal_ = MakeObj();
      bal_->g_SetCenter(TRUE);
      bal_->f_SetCenter(TRUE);
      bal_->o_SetParent(field_);
      bal_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_btn_balloon));
      bal_->z_SetPos(0, 32);
      bal_->g_SetAlp(TRUE, 160);
      bal_->f_MakeFont("いどう", FC2_normal);
      SetPos(-136, 64);
    }
    else
    {
      SetPos(-136, 64 - 24);
      bal_ = NULL;
    }
    
  }



  void TUGShLever::Refresh()
  {
    TMgPad* pmgPad = player_->pGame_->mgGO_.pmgPad_[mid::midGetMyIdx()];
    field_->g_SetPartNo(pmgPad->GetLeverNo());
  }

  void TUGShLever::DoUpdate(BOOL draw_f)
  {
    Refresh();
    field_->Update(draw_f);
    if (bal_ != NULL) bal_->Update(draw_f);
  }

  //void TUGShLever::DoAction()
  //{
  //}
}
