#include "dbUGRcLogStar.h"
namespace db
{
  TUGRcLogStar::TUGRcLogStar(TUIGenLayer* player) : TUGRcNormal(player)
  {
  }
  TUGRcLogStar::~TUGRcLogStar(void)
  {
  
  }
  void TUGRcLogStar::DoMakeGroupSub2()
  {
    SetShopRecord();
  }
  void TUGRcLogStar::DoSetLimY2(s32 limu, s32 hhh)
  {
  }
  void TUGRcLogStar::DoUpdateSub2(BOOL draw_f)
  {
  }
  //スター購入ログ
  void TUGRcLogStar::SetLogBuyStarDt(s64 id_log_bstar)
  {
    SetCommonID(id_log_bstar);

    s32 Starnum = pmgEO_->mgCoU_.mdlog_BStar_.GetPKDt(id_log_bstar, mdlog_bstar_buyNum);
    ugname_.SetNameStar(Starnum);

    s64 createdate = pmgEO_->mgCoU_.mdlog_BStar_.GetPKDt(id_log_bstar, mdlog_bstar_create_date);
    std::string datestr = base::UnixtimeToDateString(createdate, TRUE);
    SetSubName(datestr.c_str());

    SetSubValue("ＺＡＮＤＡＫＡ");

    s32 zandaka = pmgEO_->mgCoU_.mdlog_BStar_.GetPKDt(id_log_bstar, mdlog_bstar_zandaka);
    std::string zandakastr = lib_str::IntToStr(zandaka);
    SetValue(zandakastr.c_str());
  }
  //スター消費ログ
  void TUGRcLogStar::SetLogUseStarDt(s64 id_itemget, BOOL bstar_f, BOOL fstar_f)
  {
    SetCommonID(id_itemget);

    s32 itemtype = pmgEO_->mgCoU_.mdlog_ItemGet_.GetPKDt(id_itemget, mdlog_itemget_itemType);
    s64 itemid = pmgEO_->mgCoU_.mdlog_ItemGet_.GetPKDt(id_itemget, mdlog_itemget_itemID);
    s32 itemnum = pmgEO_->mgCoU_.mdlog_ItemGet_.GetPKDt(id_itemget, mdlog_itemget_itemNum);
    ugname_.SetName(itemtype, itemid, itemnum);

    s64 createdate = pmgEO_->mgCoU_.mdlog_ItemGet_.GetPKDt(id_itemget, mdlog_itemget_create_date);
    std::string datestr = base::UnixtimeToDateString(createdate, TRUE);
    SetSubName(datestr.c_str());

    s32 usebstar = (bstar_f)
      ? pmgEO_->mgCoU_.mdlog_ItemGet_.GetPKDt(id_itemget, mdlog_itemget_getInfo)
      : 0;
    s32 usefstar = (fstar_f)
      ? pmgEO_->mgCoU_.mdlog_ItemGet_.GetPKDt(id_itemget, mdlog_itemget_getInfo2)
      : 0;
    std::string numstr = lib_str::IntToStr(usebstar + usefstar);
    SetValue(numstr.c_str());
  }
}
