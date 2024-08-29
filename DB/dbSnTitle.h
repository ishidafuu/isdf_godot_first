#pragma once
#include "dbScene.h"
#include "dbLyList_Title.h"

namespace db
{  
  //タイトル///////////////////////////
  class TSnTitle : public TScene
  {
  public:
    TUIFrame* frame_;
    TLyTiGrp* tiGrp_;
    TLyTiTitle* lyTitle_;

    explicit TSnTitle(TGame* pGame, TMgUI* pMgUI, s32 myScene);
    virtual ~TSnTitle(void);

    void GoToHome();
  protected:
    //BOOL setlayer_f_;
    //BOOL goto_f_;

    void MvPgTitle();
    //継承
    virtual void DoSetLayer();
    virtual void DoFreeLayer();
    virtual void DoReady();
    virtual void DoUpdate();//継承先の更新内容はこれをオーバーロード
    virtual void DoDraw();//継承先の描画内容はこれをオーバーロード
    virtual void DoGotoTitleFilter();
  };
}
