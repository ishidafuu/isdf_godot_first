#include "dbLyTiGrp.h"
#include "dbSnTitle.h"

namespace db
{
  //コンストラクタ
  TLyTiGrp::TLyTiGrp(TSnTitle* pScene) :TUILayer(pScene, pScene->frame_)
  {

  }

  TLyTiGrp::~TLyTiGrp()
  {

  }

  void TLyTiGrp::DoLoad()
  {
    title_bg_ = LoadGrp("bg", "title_bg");//オブジェ
    //okbtn_ = LoadGrpPart("btn", "btn_ok", 72, 24);//OKボタン
  }
  void TLyTiGrp::DoReady()
  {

  }
  void TLyTiGrp::DoDown()
  {

  }
  void TLyTiGrp::DoChangePage(s32 pageno)
  {

  }
  void TLyTiGrp::DoUpdate(BOOL draw_f)
  {

  }
}
