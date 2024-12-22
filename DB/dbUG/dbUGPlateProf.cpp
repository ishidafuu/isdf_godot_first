#include "dbUGPlateProf.h"
namespace db
{

  //コンストラクタ
  TUGPlateProf::TUGPlateProf(TUIGenLayer* player) :TUGGen(player)
    , ugrprof_(player)
  {

  }

  TUGPlateProf::~TUGPlateProf()
  {

  }

  void TUGPlateProf::MakeGroup(TUIObj* parent, BOOL left_f)
  {
    SetParent(parent);
    if (left_f)
    {
      field_->z_SetPos(-156, -56);
    }
    else
    {
      field_->z_SetPos(0, -56);
    }
    
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_panelavatar));
    field_->g_SetDepth(UIDP_06BUTTONHI);

    static const s32 POSX = 35;
    static const s32 POSY = 8;

    ugrprof_.MakeGroup(field_);
    ugrprof_.SetPos(POSX, POSY);

    SetDraw(FALSE);
  }

  void TUGPlateProf::SetProfMe()
  {
    ugrprof_.SetProfMe();
    SetDraw(TRUE);
    pmgEO_->mgGrp_.SetProf_Me();
  }
  void TUGPlateProf::SetProfRanker(s64 id_ranker)
  {
    ugrprof_.SetProfRanker(id_ranker);
    SetDraw(TRUE);
    pmgEO_->mgGrp_.SetProf_Ranker(id_ranker);
  }
  void TUGPlateProf::SetProfEnemy(s32 no)
  {
    ugrprof_.SetProfEnemy(no);
    SetDraw(TRUE);
    pmgEO_->mgGrp_.SetProf_Enemy(no);
  }
  void TUGPlateProf::SetProfFriend()
  {
    ugrprof_.SetProfFriend();
    SetDraw(TRUE);
    pmgEO_->mgGrp_.SetProf_Enemy(0);
  }
  void TUGPlateProf::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    ugrprof_.Update(draw_f);
  }

  void TUGPlateProf::DoAction()
  {

  }
}
