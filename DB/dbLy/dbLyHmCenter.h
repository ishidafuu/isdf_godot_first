#pragma once
#include "dbLyHmGen.h"
#include "dbUGChKantoku.h"
#include "dbUGChCaptain.h"
#include "dbUGChClerk.h"
#include "dbUGDock.h"
#include "dbUGRSoukoNum.h"
namespace db
{
  struct THmCenterSt
  {
    s32 dockNo_;
    BOOL marukaout_f_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TLyHmCenter :public TLyHmGen
  {
  public:
    //コンストラクタ
    explicit TLyHmCenter(TSnHome* pScene);
    virtual ~TLyHmCenter(void);
    void Idle();
	void SetupCenter(s32 centerNo);
    void RefreshSoukoNum();
    // ドックから出る時に呼んでもらう
    void OutDock(void);

	// まるか運送画面外へ
	void SetOutMaruka(void);

    THmCenterSt stCenter_;
    TUGChKantoku ugkantoku_;
    TUGChCaptain ugcap_;
    TUGChClerk ugcl_bba_;//ばばあ
    TUGDoor ugdoor_bba_;//ばばあまど
    TUGChClerk ugcl_maruka_;//まるか
  protected:
    TUIObj* track_;//トラック
    TUIObj* pole_;//トラック
    TUGButton ugbln_bba_;//ばばあボタン
    TUGButton ugbln_maruka_;//まるかうんそう
    std::vector<TUGDock*> ugdock_;//ドック
    TUGRSoukoNum ugsoukonum_have_;
    TUGRSoukoNum ugsoukonum_inv_;
    s32 subPage_;
    BOOL isLockPage_;	// チェンジページさせないフラグ

    void SetDockWait(BOOL wait_f);
    void RefreshDock();
    void RefreshMaruka();

    void Post_CheckMaruka();
    void Post_AddDock();
    void Post_QuickGousei();
    void Connect_CheckMaruka();
    void Connect_AddDock();
    void Connect_QuickGousei();
    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoChangePage(s32 pageno);
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);

    void MvPage_Walking();
    void MvPage_Selected();
    void MvPage_AddDock();
    void MvPage_QDock();
    void MvPage_InDock();
    void MvPage_OutDock();
    void MvPage_Maruka();
	void MvPage_OutMaruka();
    void MvPage_BBA();
    void MvPage_Error();
    void MvPage_Out();
    void MvPage_Idle();

    
  };

}
