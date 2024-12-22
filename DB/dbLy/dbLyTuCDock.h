#pragma once
#include "dbLyTuBase.h"
#include "dbUGSpGousei.h"
#include "dbUGDock.h"
#include "dbUGDrShot.h"
#include "dbUGLiShFilter.h"
#include "dbUGTutoMsgGousei.h"

namespace db
{

  struct TCTutoSt
  {
    BOOL prevhit_f_;
    BOOL prevretry_f_;
    s32 prevr_c_;
    BOOL prevfirstback_f_;//手もと戻りでＯＫフラグにしてしまったので２回手元に戻ったらにする
    void Init()
    {
      Zeromem(this);
    }
  };

  class TLyTuCDock :public TLyTuBase
  {
    // 基底クラス定義
    typedef TLyTuBase base;
    struct CharaDt
    {
      s32 talkCategory_;	// 喋りのカテゴリー番号
      s32 talkNo_;		// 今喋っているテキスト番号
      void Init(void)
      {
        talkCategory_ = 0;
        talkNo_ = 0;
      }
    };
  public:
    //コンストラクタ
    explicit TLyTuCDock(TSnTutorial* pScene);
    virtual ~TLyTuCDock(void);
    void ReadyDock(TUGDock* ugdock, s32 dockno);

  protected:
    void DoChangePage(s32 pageno);
    void DoLoad();
    void DoReady();
    void DoDown();
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);

    void ResetSozaiVec();

    void ResetObj();
    void SwitchLongMsg(BOOL longmsg_f);
    BOOL SetDockMsg(const STRVECTOR2& msg, s32 line);
    void MvPage_Visit();
    void MvPage_PrevA();
    void MvPage_PrevB();
    void MvPage_PrevC();
    void MvPage_PrevD();
    void MvPage_PrevRIntro();
    void MvPage_PrevR();
    void MvPage_PrevRResult();
    void MvPage_Last();
    void MvPage_Out();
    void MvPage_Error();

    void Post_TutoGousei();
    void Connect_TutoGousei();
    TUGShot ugshot_;
    TUGChKantoku* ugkantoku_;//ポインタ
    TUGChCaptain* ugcap_;//ポインタ
    TUGDock* ugdock_;//ポインタ
    LONGVECTOR id_shot_mixvec_;
    INTVECTOR sozaivec_add_;
    INTVECTOR syouhinvec_add_;
    INTVECTOR sozaivec_base_;
    INTVECTOR syouhinvec_base_;
    INTVECTOR spsyouhinvec_;
    SOZAIDTVECTOR prevsozaivec_;

    

    TCTutoSt stCTuto_;
    CharaDt	dockDt_;
  };
}
