#pragma once
#include "dbLyHmGen.h"
#include "dbUGSpMaruka.h"
#include "dbUGLiMaruka.h"

namespace db
{
  struct TCMarukaSt
  {
    s32 logpage_;
    s32 endpage_;
    void Init()
    {
      Zeromem(this);
    }
  };


  class TLyHmCMaruka :public TLyHmGen
  {
  protected:
    TUGSpMaruka ugmaruka_;
    TUGLiMaruka uglistMaruka_;
    LONGVECTOR outidvec_;

    //BOOL RefreshPageBtn(BOOL inc_f);

    void MvPage_Visit();
    void MvPage_SelectSozai();
    void MvPage_OutStore();
    void SetEndPage();
    void SwitchLogPageBtn();
    //通信
    void Post_ReadyMaruka();
    void Post_Maruka();
    void Post_OutMaruka();
    void Connect_ReadyMaruka();
    void Connect_Maruka();
    void Connect_OutMaruka();
    //オーバーロード
    void DoChangePage(s32 pageno);
    void DoLoad();
    void DoReady();
    void DoDown();
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);
  public:
    TCMarukaSt stCMaruka_;

    //コンストラクタ
    explicit TLyHmCMaruka(TSnHome* pScene);
    virtual ~TLyHmCMaruka(void);
  };
}
 