#include "dbUGRcNaBoxCnt.h"
namespace db
{
  TUGRcNaBoxCnt::TUGRcNaBoxCnt(TUIGenLayer* player) : TUGRcNormal(player)
  {
  }
  TUGRcNaBoxCnt::~TUGRcNaBoxCnt(void)
  {
  
  }
  void TUGRcNaBoxCnt::DoMakeGroupSub2()
  {
    subget_ = MakeObj();
    subget_->o_SetParent(field_);
    SetShopRecord();
  }
  void TUGRcNaBoxCnt::DoSetLimY2(s32 limu, s32 hhh)
  {
    subget_->g_SetLimY(limu, hhh);
  }
  void TUGRcNaBoxCnt::DoUpdateSub2(BOOL draw_f)
  {
    subget_->Update(draw_f);
  }

  void TUGRcNaBoxCnt::SetNaBoxCntDt(s64 mid_naboxcnt, s64 id_nabox)
  {
    //懐かし中身ＩＤ
    SetCommonID(mid_naboxcnt);
    s64 itemtype = pmgEO_->mgCoM_.mdm_NaBoxCnt_.GetPKDt(mid_naboxcnt, mdm_naboxcnt_itemType);
    s64 itemid = pmgEO_->mgCoM_.mdm_NaBoxCnt_.GetPKDt(mid_naboxcnt, mdm_naboxcnt_itemID);
    s64 itemnum = pmgEO_->mgCoM_.mdm_NaBoxCnt_.GetPKDt(mid_naboxcnt, mdm_naboxcnt_itemNum);
    s32 boxno = pmgEO_->mgCoM_.mdm_NaBoxCnt_.GetPKDt(mid_naboxcnt, mdm_naboxcnt_boxNo);

    ugname_.SetName(itemtype, itemid, itemnum);

    std::string strno = "ＮＯ．";
    strno += lib_str::IntToStr(boxno + 1);
    SetSubName(strno.c_str(), FC2_normal, FALSE);
    //std::string strnum = "ｘ" + lib_str::IntToStr(itemnum);
    std::string strnum =  lib_str::IntToStr(itemnum);

    switch (itemtype)
    {
    case db::enItemType_Todd:
    case db::enItemType_Star:
    case db::enItemType_BGM:
    case db::enItemType_Haikei:
      break;
    default:
      //SetValue(strnum.c_str());
      break;
    }

    BOOL get_f = pmgEO_->mgCoU_.IsNaBoxOpen(id_nabox, boxno);

    if (get_f)
    {
      subget_->z_SetPos(POSXL + 24, SUBY);
      subget_->f_MakeFont_Mini("ＧＥＴ！", FC2_mini);
      subget_->b_SetBtn(FALSE);
      SetDisable(TRUE);//暗転
    }

    subget_->g_SetDraw(get_f);
  }

}
