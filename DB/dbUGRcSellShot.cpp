#include "dbUGRcSellShot.h"
namespace db
{
  TUGRcSellShot::TUGRcSellShot(TUIGenLayer* player) : TUGRcNormal(player)
    //ugbtn_eye_(player)
  {
    //自前アクション
    SetPlate(TRUE);
  }
  TUGRcSellShot::~TUGRcSellShot(void)
  {
  
  }
  void TUGRcSellShot::DoMakeGroupSub2()
  {
  }
  void TUGRcSellShot::DoSetLimY2(s32 limu, s32 hhh)
  {
  }
  void TUGRcSellShot::DoUpdateSub2(BOOL draw_f)
  {
  }
  void TUGRcSellShot::DoActionSub()
  {
    if (field_->b_IsDecide())//レコード
    {
      SetSelect(!(IsSelect()));
      SetAct(enUGRcSellShot_Field);//
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
  void TUGRcSellShot::SetShotDt(s64 id_shot)
  {

    if (mid::midIsDXL())
    {

      ugname_.SetNameShot(id_shot, TRUE);
      //価格
      SetValue(lib_str::IntToStr(999).c_str());
      stSellShot_.id_shot_ = id_shot;
    }
    else
    {
      //素材ＩＤ保存
      SetCommonID(id_shot);
      stSellShot_.id_shot_ = id_shot;
      s32 sellprice = pmgEO_->mgCoU_.GetShotPrice(id_shot);
      ugname_.SetNameShot(id_shot, TRUE);
      SetValue_Todd(sellprice);
    }

  }

}
