#include "dbUGRcRanking.h"

namespace db
{
  TUGRcRanking::TUGRcRanking(TUIGenLayer* player) : TUGRcNormal(player)
  {
  }
  TUGRcRanking::~TUGRcRanking(void)
  {
  
  }
  void TUGRcRanking::DoMakeGroupSub2()
  {
    basegrp_ = pmgUI_->GetUGGrp(ug_list_record);
    field_->o_SetObjGrp(basegrp_);
  }
  void TUGRcRanking::DoSetLimY2(s32 limu, s32 hhh)
  {
  }
  void TUGRcRanking::DoUpdateSub2(BOOL draw_f)
  {
  }
  //商品データ
  void TUGRcRanking::SetRankingDt(s64 mid_Ranking)
  {
    //ＭＩＤ保存
    SetCommonID(mid_Ranking);
    //ugname_.SetNameIcon();
    ugname_.SetNameRanking(mid_Ranking);
  }

}
