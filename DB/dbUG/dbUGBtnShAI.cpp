#include "dbUGBtnShAI.h"
#include "dbMgUI.h"
#include "dbUIGenLayer.h"
#include "dbGame.h"

namespace db
{
  const s32 AINUM = 4;

  //コンストラクタ
  TUGShAI::TUGShAI(TUIGenLayer* player) :TUGGen(player)
    , ugairadio_(player)
  {
    stBtnShAI_.Init();
  }
  TUGShAI::~TUGShAI(void)
  {
    for (s32 i = 0; i < ugaibtn_.size(); ++i) Freemem(ugaibtn_[i]);
  }
  void TUGShAI::MakeGroup(TUIObj* parent)//, TUGShKantoku* ugshkantoku)
  {
    //ugshkantoku_ = ugshkantoku;

    SetParent(parent);

    field_->g_SetDepth(UIDP_05BUTTON);
    SetPos(24, 88);

    //AI選択ラジオ
    ugairadio_.MakeGroup(field_, 0, 0);

    for (s32 i = 0; i < AINUM; ++i)
    {
      TUGButton* newbtn = new TUGButton(player_);
      newbtn->MakeGroup_Radio(field_, ug_game_ai, ug_game_ai2);
      ugairadio_.AddButton(newbtn, 40);//ugairadio_.btns_[i]->base_->z_SetPos_SafeL(SAKUSENX + (40 * i), KANTOKUY);
      newbtn->GetField()->g_SetAlp(TRUE, 200);
      newbtn->SetDepth(UIDP_05BUTTON);
      ugaibtn_.push_back(newbtn);
    }
    s32 fx = 4;
    s32 fy = 2;//高さあってないかも
    ugaibtn_[0]->SetFont("いけいけ", FC2_btn_normal);
    ugaibtn_[1]->SetFont("ほんろう", FC2_btn_normal);
    ugaibtn_[2]->SetFont("らぐびい", FC2_btn_normal);
    ugaibtn_[3]->SetFont("ぼすぱす", FC2_btn_normal);

  }


  //文字列セット
  void TUGShAI::Refresh()
  {

  }
  void TUGShAI::SetDisable(BOOL disable_f)
  {
    if (disable_f)
    {
      field_->g_Dark();
      field_->b_SetBtn(FALSE);
    }
  }
  void TUGShAI::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    ugairadio_.Update(draw_f);
  }

  void TUGShAI::DoAction()
  {
      if (ugairadio_.IsAct())
      {
          SetAct(ugairadio_.GetActIndex());
      }

 
  }
}