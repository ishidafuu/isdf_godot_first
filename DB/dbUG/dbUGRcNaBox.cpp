#include "dbUGRcNaBox.h"
namespace db
{
  TUGRcNaBox::TUGRcNaBox(TUIGenLayer* player) : TUGRcNormal(player)
  {
  }
  TUGRcNaBox::~TUGRcNaBox(void)
  {
  
  }
  void TUGRcNaBox::DoMakeGroupSub2()
  {
    SetShopRecord();
  }
  void TUGRcNaBox::DoSetLimY2(s32 limu, s32 hhh)
  {
  }
  void TUGRcNaBox::DoUpdateSub2(BOOL draw_f)
  {
  }

  void TUGRcNaBox::SetNaBoxDt(s64 mid_nabox)
  {
    SetCommonID(mid_nabox);
    std::string name = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDtStr(mid_nabox, mdm_nabox_name_c64);
    s32 id_nabox = pmgEO_->mgCoU_.GetNaBox_id(mid_nabox);
    s64 opennum = pmgEO_->mgCoU_.mdNaBox_.GetPKDt(id_nabox, mdnabox_openNum);
    s32 boxsize = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDt(mid_nabox, mdm_nabox_boxSize);

    s32 restnum = (boxsize - opennum);
    //Name
    ugname_.SetNameNaBox(mid_nabox);

//    SetSubValue("ＮＵＭ");

    std::string numstr = lib_str::IntToStr(restnum) + "／" + lib_str::IntToStr(boxsize);
    SetSubValue(numstr.c_str());

    BOOL disable_f = (restnum == 0);
#ifndef __K_DEBUG_ROOT__
    //チュートクリアしてないと無料以外買えない
    if (pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_NaBox) == FALSE)
    {
      s32 mid_ken = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDt(mid_nabox, mdm_nabox_id_m_supply_ken);
      s32 price = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(mid_ken, mdm_supply_price);
      disable_f = (price != 0);
    }
#else
	s32 mid_ken = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDt(mid_nabox, mdm_nabox_id_m_supply_ken);
	s32 price = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(mid_ken, mdm_supply_price);
	disable_f = FALSE;
#endif

    SetDisable(disable_f);
  }

}
