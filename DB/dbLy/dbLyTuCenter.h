//utf8
#pragma once
#include "dbLyTuBase.h"
#include "dbUGChKantoku.h"
#include "dbUGChCaptain.h"
#include "dbUGChClerk.h"
#include "dbUGDock.h"
#include "dbUGRSoukoNum.h"
//=========================================================================
// 
// チュートリアル：ホーム画面
//
//=========================================================================
namespace db
{
  struct TTuCenterSt
  {
    s32 dockNo_;
    void Init()
    {
      Zeromem(this);
    }
  };


  class TLyTuCenter :public TLyTuBase
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
    explicit TLyTuCenter(TSnTutorial* pScene);
    virtual ~TLyTuCenter(void);

    // 真実メニューからの復帰用
    void ComebackInMamiMenu(void);

    // プロフィールからの復帰用
    void ComebackProfile(void);

    // セリフ用ウィンドウのゲッター
    inline TUGSerifu* GetSerifuWindow(void){ return &ugserifu_; }
    ////がちゃ屋から出るとき呼ばれる
    //void OutCenter();
    void Idle();
    void RefreshSoukoNum();
    // ドックから出る時に呼んでもらう
    void OutDock(void);
    void SetTutoEnd();

    TUGChKantoku ugkantoku_;
    TUGChCaptain ugcap_;
  protected:
    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoChangePage(s32 pageno);
    //void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);

    void SetDockWait(BOOL wait_f);
    void RefreshDock();
    void Post_AddDock();
    void Post_QuickGousei();
    void Connect_AddDock();
    void Connect_QuickGousei();
    //void MvPage_Init(void);
    //void MvPage_CapTalk(void);
    //void MvPage_Walking();
    //void MvPage_Selected();
    //void MvPage_InCenter();
    //void MvPage_GotoCenter();
    //void MvPage_OutCenter();
    //void MvPage_CapTalk2(void);
    //void MvPage_Idle();
    //void MvPage_GotoCenter(void);

    void MvPage_Init(void);
    void MvPage_CapTalk(void);
    void MvPage_Walking();
    void MvPage_Selected();
    void MvPage_InDock();
    void MvPage_OutDock();
	void MvPage_OutDock2(void);
    void MvPage_CapTalk2(void);
    void MvPage_GotoHome(void);

    CharaDt	captainDt_;
    s32 subPage_;
    TTuCenterSt stCenter_;

    //TUGChClerk ugcl_bba_;//ばばあ
    TUGDoor ugdoor_bba_;//ばばあまど
    TUGChClerk ugcl_maruka_;//まるか
    TUIObj* track_;//トラック
    TUIObj* pole_;//トラック
    TUGButton ugbln_bba_;//ばばあボタン
    TUGButton ugbln_maruka_;//まるかうんそう
    std::vector<TUGDock*> ugdock_;//ドック
    TUGRSoukoNum ugsoukonum_have_;
    TUGRSoukoNum ugsoukonum_inv_;
    BOOL isLockPage_;	// チェンジページさせないフラグ

  };

}
