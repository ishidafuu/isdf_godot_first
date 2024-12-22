#include "dbUGLiMenu.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"

namespace db
{
  //コンストラクタ
  TUGLiMenu::TUGLiMenu(TUIGenLayer* player) :TUGLiGen(player), uglistend_(player)
  {
    stLiMenu_.Init();
  }

  TUGLiMenu::~TUGLiMenu()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiMenu::DoClearRecord()
  {
    stLiMenu_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  void TUGLiMenu::DoMakeGroupSub()
  {
    //if (stLiMenu_.shop_f_)
    //{
    //  uglistend_.MakeGroup_ListEnd_Shop(slider_, sliderx_, slidery_);
    //  field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frameshop));//ショップの時は変える
    //}
    //else
    uglistend_.MakeGroup_ListEnd(slider_, sliderx_, slidery_);
  }

  TUGRcNormal* TUGLiMenu::MakeObj_Record()
  {
    TUGRcNormal* newrecord = new TUGRcNormal(player_);
    s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
    records_.push_back(newrecord);
    GenMakeObj_Record(newrecord, rh);
    return newrecord;
  }
  //レコードとリストエンドを切り替え
  void TUGLiMenu::ChangeListEnd(BOOL listend_f)
  {
    uglistend_.SetDraw(listend_f);
    for (s32 i = 0; i < records_.size(); ++i) records_[i]->SetDraw(!listend_f);
  }
  //void TUGLiMenu::InitShop()
  //{
  //  stLiMenu_.shop_f_ = TRUE;
  //}
  void TUGLiMenu::DoUpdateSub(BOOL draw_f)
  {
    uglistend_.Update(draw_f);
  }

  void TUGLiMenu::DoActionSub()
  {
    //選ばれたレコード色づけ
    //SetSelect();
  }
  s64 TUGLiMenu::GetCommonID()
  {
    return records_[GetActIndex()]->GetCommonID();
  }
}
