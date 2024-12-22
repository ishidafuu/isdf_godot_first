#pragma once
#include "dbLyHmGen.h"
#include "dbUGSpMaveko.h"
#include "dbUGButton.h"
#include "dbUGLiMenu.h"
#include "dbUGLiLogGetItem.h"
#include "dbUGLiLogUseItem.h"
#include "dbUGPlateProf.h"
#include "dbUGInfoIcon.h"
namespace db
{


  struct TMavekoSt
  {
    BOOL msgset_f_;
    s32 wait_c_;
    s32 pageNo2_;
    s32 logpage_;
    s64 mid_cpcode_;
    s32 codeketa_;
    s32 endpage_;

    void Init()
    {
      Zeromem(this);

      wait_c_ = 60;
    }
  };


  class TLyHmMaveko :public TLyHmGen
  {
  protected:
    TUGSpMaveko ugsp_maveko_;
    TUGLiMenu uglistMenu_;
    TUGLiMenu uglistCode_;
    TUGLiMenu uglistInvite_;
    TUGLiMenu uglistInviteMember_;
    TUGLiLogGetItem uglistGetLog_;
    TUGLiLogUseItem uglistUseLog_;
    TUGLiMenu uglistDebug_;
    TUGPlateProf ugprof_;
    TUGInfoIcon* uginfo_;

    //TUGInput ugInput_;
    std::string inputcode_;
    std::vector<TUGLiGen*> listvec_;
  
    void DrawList(TUGLiGen* drawlist);
    void ResetObj();
    void SwitchLogPageBtn();
    void RefreshCpCodeList();
    
    void MvPage_Visit();
    void MvPage_Menu();

    void ActListMenu();
    void MvPage_Help();
    void MvPage_Info();
    void MvPage_Code();
    //void MvPage_Code_fr_input();
    //void MvPage_Code_fr_decide();
    //void MvPage_Code_fr_result();
    void MvPage_Code_cp_input();
    void MvPage_Code_cp_result();
    //void MvPage_Invite();
    void MvPage_GetLog();
    void MvPage_GetLogEnd();
    void MvPage_UseLog();

    void MvPage_Circle();
    void MvPage_Push();
    void MvPage_GotoTilte();
    void MvPage_Logout();
    void MvPage_Debug();
    void MvPage_Out();

    void SetEndPage();
    void Post_NewInfo();
    //void Post_FrCode();
    //void Post_FrCodeDecide();
    void Post_CpCode();
    void Post_GetLogReady();
    void Post_GetLog();
    void Post_UseLogReady();
    void Post_UseLog();
    void Post_InviteMember();
    void Post_Debug();

    void Connect_NewInfo();
    void Connect_FrCode();
    void Connect_FrCodeDecide();
    void Connect_CpCode();
    void Connect_GetLogReady();
    void Connect_GetLog();
    void Connect_UseLogReady();
    void Connect_UseLog();
    void Connect_InviteMember();
    void Connect_Debug();
    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoDown();
    void DoChangePage(s32 pageno);
    void DoCheckOut(BOOL rqok_f);
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);
  public:
    TMavekoSt stMaveko_;
    //コンストラクタ
    explicit TLyHmMaveko(TSnHome* pScene);
    virtual ~TLyHmMaveko(void);

  };
 
}
