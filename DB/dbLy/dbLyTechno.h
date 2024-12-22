#pragma once
#include "dbUILayer.h"
#include "dbUGLiMenu.h"
#include "dbUGLiSpTechno.h"
#include "dbUGLiLogBuyStar.h"
#include "dbUGLiLogUseStar.h"
#include "dbUGBtnTokusyou.h"
#include "dbUGPlateCredit.h"
namespace db
{

  struct TTechnoSt
  {
    s32 pageno_;
    s32 logpage_;
    BOOL bstar_f_;
    BOOL fstar_f_;
    s64 mid_supply_;
    s64 techno_;
    s32 endpage_;
    s32 wait_c_;
    s32 wait_c2_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TLyBarGen;

  class TLyTechno :public TUILayer
  {
  protected:
    TLyBarGen* lybar_;
    TTechnoSt stTechno_;
    TUGLiMenu uglistMenu_;
    TUGLiSpTechno uglistSpTechno_;
    TUGLiLogBuyStar uglistBuyLog_;
    TUGLiLogUseStar uglistUseLog_;
    TUGButton ugbtn_back_tec_;
    TUGButton ugbtn_ok_tec_;
    //TUGButton ugbtn_mid_tec_;
    TUGButton ugbtn_next_tec_;
    TUGButton ugbtn_prev_tec_;
    TUGBtnTokusyou ugbtn_tsh_tec_;
    std::vector<TUGLiGen*> listvec_;
    std::string tid_;

    TUGPlateCredit ugallcredit_;

    void DrawList(TUGLiGen* drawlist);
    void ResetObj();
    void SwitchLogPageBtn();
    void DoChangePage(s32 pageno);

    void MvPage_Menu();
    void MvPage_Info();
    void MvPage_BuyTechno();
    void MvPage_BuyStar();
    void MvPage_BuyStarOK();
    void MvPage_BuyStarTransaction();
    void MvPage_BuyStarClose();
    void MvPage_BuyStarCancel();
    void MvPage_BuyStarResult();
    void MvPage_BuyLog();
    void MvPage_BuyLogEnd();
    void MvPage_UseLog();
    void MvPage_Help();
    
    void SetEndPage();

    void Post_GetBalance_BuyStarReady();
    void Post_GetBalance_BuyTechnoResult();
    void Post_GetBalance_BuyStarResult();
    void Post_BankCreate();
    void Post_Transaction();
    void Post_BankClose();
    void Post_BankCancel();
    void Post_BuyLogReady();
    void Post_BuyLog();
    void Post_UseLogReady();
    void Post_UseLog();

    void Connect_GetBalance_BuyStarReady();
    void Connect_GetBalance_BuyTechnoResult();
    void Connect_GetBalance_BuyStarResult();
    void Connect_BankCreate();
    void Connect_Transaction();
    void Connect_BankClose();
    void Connect_BankCancel();
    void Connect_BuyLogReady();
    void Connect_BuyLog();
    void Connect_UseLogReady();
    void Connect_UseLog();
    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoDown();
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TLyTechno(TScene* pScene, TUIFrame* frame, TLyBarGen* lybar);
    virtual ~TLyTechno(void);

  };
 
}
