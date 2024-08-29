#include "dbUGRSoukoNum.h"
#include "dbUILayer.h"

namespace db
{
  const s32 KETA = 4;

  //コンストラクタ
  TUGRSoukoNum::TUGRSoukoNum(TUIGenLayer* player) : TUGGen(player)//アクションなし
  {
    stRSoukoNum_.Init();
  }

  TUGRSoukoNum::~TUGRSoukoNum()
  {

  }
  void TUGRSoukoNum::MakeGroup(TUIObj* parent, BOOL inv_f)
  {
    SetParent(parent);
    stRSoukoNum_.inv_f_ = inv_f;

    for (s32 i = 0; i < KETA; ++i)
    {
      TUIObj* newnum = MakeObj();
      num_.push_back(newnum);
      newnum->o_SetParent(field_);
      newnum->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_souko_num));
      newnum->z_SetPos((8 * i), 0);
      newnum->g_SetDraw(TRUE);
    }
  }


  void TUGRSoukoNum::DoUpdate(BOOL draw_f)
  {
    for (s32 i = 0; i < num_.size(); ++i)
    {
      num_[i]->Update(draw_f);
    }
  }

  void TUGRSoukoNum::Refresh()
  {
    s32 num = 0;
    if (stRSoukoNum_.inv_f_)
    {
      num = pmgEO_->mgCoU_.GetShotNum_Inv();
    }
    else
    {
      num = pmgEO_->mgCoU_.GetShotNum_Souko();
    }

    num_[0]->g_SetPartNo(num / 1000);
    num_[1]->g_SetPartNo((num / 100) % 10);
    num_[2]->g_SetPartNo((num / 10) % 10);
    num_[3]->g_SetPartNo(num % 10);
  }

}
