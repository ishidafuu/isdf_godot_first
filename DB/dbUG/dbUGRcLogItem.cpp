#include "dbUGRcLogItem.h"
namespace db
{
  TUGRcLogItem::TUGRcLogItem(TUIGenLayer* player) : TUGRcNormal(player)
  {
  }
  TUGRcLogItem::~TUGRcLogItem(void)
  {
  
  }
  void TUGRcLogItem::DoMakeGroupSub2()
  {
    SetShopRecord();
  }
  void TUGRcLogItem::DoSetLimY2(s32 limu, s32 hhh)
  {
  }
  void TUGRcLogItem::DoUpdateSub2(BOOL draw_f)
  {
  }
  void TUGRcLogItem::SetLogGetItemDt(s64 id_itemget)
  {
    SetCommonID(id_itemget);

    s32 itemtype = pmgEO_->mgCoU_.mdlog_ItemGet_.GetPKDt(id_itemget, mdlog_itemget_itemType);
    s64 itemid = pmgEO_->mgCoU_.mdlog_ItemGet_.GetPKDt(id_itemget, mdlog_itemget_itemID);
    s32 itemnum = pmgEO_->mgCoU_.mdlog_ItemGet_.GetPKDt(id_itemget, mdlog_itemget_itemNum);
    ugname_.SetName(itemtype, itemid, itemnum);

    s64 createdate = pmgEO_->mgCoU_.mdlog_ItemGet_.GetPKDt(id_itemget, mdlog_itemget_create_date);
    std::string datestr = base::UnixtimeToDateString(createdate, TRUE);
    //SubValue
    SetSubValue(datestr.c_str());

    //std::string numstr = lib_str::IntToStr(restnum) + "／" + lib_str::IntToStr(boxsize);
    //SetValue(numstr.c_str());
  }
  void TUGRcLogItem::SetLogUseItemDt(s64 id_supplyuse)
  {
    SetCommonID(id_supplyuse);
    s64 itemid = pmgEO_->mgCoU_.mdlog_SupplyUse_.GetPKDt(id_supplyuse, mdlog_supplyuse_id_m_supply);
    ugname_.SetNameSupply(itemid, 0);

    s64 createdate = pmgEO_->mgCoU_.mdlog_SupplyUse_.GetPKDt(id_supplyuse, mdlog_supplyuse_create_date);
    std::string datestr = base::UnixtimeToDateString(createdate, TRUE);
    //SubValue
    SetSubValue(datestr.c_str());

    s32 itemnum = pmgEO_->mgCoU_.mdlog_SupplyUse_.GetPKDt(id_supplyuse, mdlog_supplyuse_useNum);
    std::string numstr = lib_str::IntToStr(itemnum);
    SetValue(numstr.c_str());
  }
}
