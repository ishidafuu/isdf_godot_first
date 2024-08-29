#include "dbUGRcHuku.h"
namespace db
{
  TUGRcHuku::TUGRcHuku(TUIGenLayer* player) : TUGRcNormal(player)
  {
  }
  TUGRcHuku::~TUGRcHuku(void)
  {
  
  }
  void TUGRcHuku::DoMakeGroupSub2()
  {
    SetShopRecord();
  }
  void TUGRcHuku::DoSetLimY2(s32 limu, s32 hhh)
  {
  }
  void TUGRcHuku::DoUpdateSub2(BOOL draw_f)
  {
  }

  void TUGRcHuku::SetHukuDt(s64 mid_Huku)
  {
    SetCommonID(mid_Huku);
    std::string name = pmgEO_->mgCoM_.mdm_Huku_.GetPKDtStr(mid_Huku, mdm_huku_name_c64);
    ugname_.SetNameHuku(mid_Huku);
  }

}
