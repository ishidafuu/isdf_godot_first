#include "dbLyScGrp.h"
#include "dbSnScenario.h"

namespace db
{
  //コンストラクタ
  TLyScGrp::TLyScGrp(TSnScenario* pScene) :TUILayer(pScene, pScene->frame_)
  {
  }

  TLyScGrp::~TLyScGrp()
  {
  }

  void TLyScGrp::DoLoad()
  {
      //vs_vs_ = LoadGrpPart("vs", "vs_vs", 64, 64);
      //vs_vsset_ = LoadGrpPart("vs", "vs_vsset", 56, 40);
      //vs_body_ = LoadGrpPart("vs", "vs_body", 32, 32);
      //vs_hp_ = LoadGrpPart("vs", "vs_hp", 64, 8);
  }
}
