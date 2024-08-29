#include "dbUGLiBBABuyMenu.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"


namespace db
{
  //コンストラクタ
  TUGLiBBABuyMenu::TUGLiBBABuyMenu(TUIGenLayer* player) :TUGLiMenu(player)
  {
  }

  TUGLiBBABuyMenu::~TUGLiBBABuyMenu()
  {
  }


  void TUGLiBBABuyMenu::DoMakeGroupSub()
  {
    uglistend_.MakeGroup_ListEnd_Shop(slider_, sliderx_, slidery_);
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frameshop));//ショップの時は変える
  }
  TUGRcNormal* TUGLiBBABuyMenu::MakeObj_Record()
  {
    TUGRcNormal* newrecord = TUGLiMenu::MakeObj_Record();
    newrecord->SetShopRecord();
    return newrecord;
  }

  //マスターガチャIDを入れてレコードをすべて作成
  void TUGLiBBABuyMenu::Refresh()
  {
    ClearRecord();
    for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_BBABuy_.GetLineNum(); ++i)
    {
      TUGRcNormal* newrecord = MakeObj_Record();
      std::string name = pmgEO_->mgCoM_.mdm_BBABuy_.GetLineDtStr(i, mdm_bbabuy_text_c64);
      newrecord->ugname_.SetNameIcon(name, ugi_Ball); 
      newrecord->SetCommonID(pmgEO_->mgCoM_.mdm_BBABuy_.GetLineDt(i, mdm_bbabuy_pk));//PKを保存
    }


  }
}
