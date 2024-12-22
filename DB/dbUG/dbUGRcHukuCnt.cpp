#include "dbUGRcHukuCnt.h"
namespace db
{
  TUGRcHukuCnt::TUGRcHukuCnt(TUIGenLayer* player) : TUGRcNormal(player)
  {
  }
  TUGRcHukuCnt::~TUGRcHukuCnt(void)
  {
  
  }
  void TUGRcHukuCnt::DoMakeGroupSub2()
  {
    //subget_ = MakeObj();
    //subget_->o_SetParent(field_);
    SetShopRecord();
  }
  void TUGRcHukuCnt::DoSetLimY2(s32 limu, s32 hhh)
  {
    //subget_->g_SetLimY(limu, hhh);
  }
  void TUGRcHukuCnt::DoUpdateSub2(BOOL draw_f)
  {
    //subget_->Update(draw_f);
  }

  void TUGRcHukuCnt::SetHukuCntDt(s32 mid_Hukucnt)
  {
    //ふくびき中身ＩＤ
    SetCommonID(mid_Hukucnt);
    s64 itemtype = pmgEO_->mgCoM_.mdm_HukuCnt_.GetPKDt(mid_Hukucnt, mdm_hukucnt_itemType);
    s64 itemid = pmgEO_->mgCoM_.mdm_HukuCnt_.GetPKDt(mid_Hukucnt, mdm_hukucnt_itemID);
    s64 itemnum = pmgEO_->mgCoM_.mdm_HukuCnt_.GetPKDt(mid_Hukucnt, mdm_hukucnt_itemNum);
    s32 grade = pmgEO_->mgCoM_.mdm_HukuCnt_.GetPKDt(mid_Hukucnt, mdm_hukucnt_grade);
    s32 num = pmgEO_->mgCoM_.mdm_HukuCnt_.GetPKDt(mid_Hukucnt, mdm_hukucnt_num);
    s32 restnum = pmgEO_->mgCoU_.mdw_HukuCnt_.GetPKDt(mid_Hukucnt, mdw_hukucnt_restNum);

    ugname_.SetNameHukuCnt(mid_Hukucnt);

    std::string strno = lib_str::IntToStr(RARITYNUM - grade) + "＿ＴＯＵ";
    SetSubName(strno.c_str());
    std::string strnum = lib_str::IntToStr(restnum) + "／" + lib_str::IntToStr(num);
    SetSubValue(strnum.c_str());
  }

}
