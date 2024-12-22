#include "dbUGShKantoku.h"
#include "dbUIGenLayer.h"
#include "dbGame.h"


namespace db
{
  //コンストラクタ
  TUGShKantoku::TUGShKantoku(TUIGenLayer* player) :TUGGen(player)
    , ugkantoku_(player)
  {
    stShKantoku_.Init();
  }

  TUGShKantoku::~TUGShKantoku()
  {

  }

  void TUGShKantoku::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);

    field_->g_SetDepth(UIDP_05BUTTON);
    SetPos(-160, 76);
    //SetPos(-160, 160 - 84);

    //監督背景
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_game_kantokubg));
    field_->g_SetPartNo(0);//0番画像
    field_->g_SetDepth(UIDP_05BUTTON);
    field_->g_SetAlp(TRUE, 200);


    //監督顔アイコン
    ugkantoku_.MakeGroup(field_,FALSE);
    ugkantoku_.SetMType(kmt_Icon);
    ugkantoku_.SetPos(16, 20);
    ugkantoku_.SetDraw(TRUE);

    for (s32 i = 0; i < ksEND; ++i)
    {
      TUIObj* newserifu = MakeObj();
      serifu_.push_back(newserifu);
      newserifu->o_SetParent(field_);
      newserifu->z_SetPos(30, 5);
      newserifu->g_SetDraw(FALSE);
    }
    serifu_[ks_catchwait]->f_MakeFont("がめんタップでキャッチしよう", FC2_normal);//0番画像
    serifu_[ks_catch]->f_MakeFont("キャッチ！", FC2_normal);//0番画像
    serifu_[ks_shotwait]->f_MakeFont("がめんタップでシュートしよう", FC2_normal);//0番画像
    serifu_[ks_shot]->f_MakeFont("シュート！", FC2_normal);//0番画像
  }


  void TUGShKantoku::SerifuSelect(enKantokuSerifu sno)
  {
    for (s32 i = 0; i < ksEND; ++i)
    {
      serifu_[i]->g_SetDraw((i == sno));
    }

    if ((sno == ks_catch) || (sno == ks_shot))
    {
      ugkantoku_.SetMType(kmt_Icon);
    }
    else
    {
      ugkantoku_.SetMType(kmt_Icon);
    }

  }


  void TUGShKantoku::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    ugkantoku_.Update(draw_f);
    for (s32 i = 0; i < serifu_.size(); ++i) serifu_[i]->Update(draw_f);
  }

  void TUGShKantoku::DoAction()
  {
  }
}
