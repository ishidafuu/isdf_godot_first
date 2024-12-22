#include "dbUGLiNaBox.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"


namespace db
{
  //コンストラクタ
  TUGLiNaBox::TUGLiNaBox(TUIGenLayer* player) :TUGLiGen(player)
    //, uglistend_(player)
  {
    stLiNaBox_.Init();
  }

  TUGLiNaBox::~TUGLiNaBox()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiNaBox::DoClearRecord()
  {
    stLiNaBox_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  void TUGLiNaBox::DoMakeGroupSub()
  {
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frameshop));//ショップの時は変える
  }

  //マスターガチャIDを入れてレコードをすべて作成
  void TUGLiNaBox::Refresh()
  {
    ClearRecord();
    for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_NaBox_.GetLineNum(); ++i)
    {
      s64 mid_nabox = pmgEO_->mgCoM_.mdm_NaBox_.GetLineDt(i, mdm_nabox_pk);
      s32 id_nabox = pmgEO_->mgCoU_.GetNaBox_id(mid_nabox);
      s64 opennum = pmgEO_->mgCoU_.mdNaBox_.GetPKDt(id_nabox, mdnabox_openNum);
      s32 boxsize = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDt(mid_nabox, mdm_nabox_boxSize);

      if (opennum >= boxsize) continue;

      TUGRcNaBox* newrecord = new TUGRcNaBox(player_);
      s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
      newrecord->SetNaBoxDt(mid_nabox);
      records_.push_back(newrecord);
      GenMakeObj_Record(newrecord, rh);
    }


    //チュートクリアしてないとスクロールきかない
    if (pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_NaBox) == FALSE)
    {
      SetSliderDisable(TRUE);
    }
  }

  s64 TUGLiNaBox::GetCommonID()
  {
    return records_[GetActIndex()]->GetCommonID();
  }

  void TUGLiNaBox::DoUpdateSub(BOOL draw_f)
  {

  }

  void TUGLiNaBox::DoActionSub()
  {
  }

}
