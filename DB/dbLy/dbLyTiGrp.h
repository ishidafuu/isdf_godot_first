#pragma once
#include "dbInclude.h"
#include "dbUILayer.h"

namespace db
{

  class TSnTitle;

  //画像ローダー////////////////////////////////
  class TLyTiGrp :public TUILayer//自動開放読み込みするためTUILayerを継承
  {
  protected:
    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoDown();
    void DoChangePage(s32 pageno);
    void DoUpdate(BOOL draw_f);
  public:
    base::TGrp* title_bg_;
    base::TGrPart* okbtn_;
    //コンストラクタ
    explicit TLyTiGrp(TSnTitle* pScene);
    virtual ~TLyTiGrp(void);


  };

}
