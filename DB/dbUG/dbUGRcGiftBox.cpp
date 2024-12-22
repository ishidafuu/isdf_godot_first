#include "dbUGRcGiftBox.h"
namespace db
{
  TUGRcGiftBox::TUGRcGiftBox(TUIGenLayer* player) : TUGRcNormal(player)
  {
    //自前アクション
    SetPlate(TRUE);
    stGiftBox_.Init();
  }
  TUGRcGiftBox::~TUGRcGiftBox(void)
  {

  }
  void TUGRcGiftBox::DoMakeGroupSub2()
  {
  }
  void TUGRcGiftBox::DoSetLimY2(s32 limu, s32 hhh)
  {

  }
  void TUGRcGiftBox::DoUpdateSub2(BOOL draw_f)
  {
  }
  void TUGRcGiftBox::DoActionSub()
  {
    if (field_->b_IsDecide())//レコード
    {
      SetSelect(!(IsSelect()));
      SetAct(enUGRcGiftBox_Field);//
      field_->g_Flash();//とりあえずここで明るく
    }
    else if (field_->b_IsEnter(TRUE))
    {
      field_->g_Bright();//とりあえずここで明るく
    }
    else
    {
      field_->g_EfReset();//とりあえずここで戻す
    }
  }

  void TUGRcGiftBox::SetGiftBoxDt(s64 id_giftbox)
  {
    //おくりものＢＯＸＩＤ保存
    SetCommonID(id_giftbox);
    stGiftBox_.id_giftbox_ = id_giftbox;
    //受取期限がある
    s64 keepdate = pmgEO_->mgCoU_.mdGiftbox_.GetPKDt(id_giftbox, mdgiftbox_limit_date);
    std::string datestr = base::UnixtimeToDateString(keepdate, TRUE);
    //SubValue
    SetSubValue(datestr.c_str());
    ugname_.SetNameGiftBox(id_giftbox);
  }

}
