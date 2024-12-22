#pragma once
#include "dbLyHmGen.h"
#include "dbUGSpBBA.h"
#include "dbUGLiShopMenu.h"
#include "dbUGLiSpSyouhin.h"
#include "dbUGLiSellShot.h"
#include "dbUGLiBuySozaiType.h"
#include "dbUGDrShot.h"
#include "dbUGShot.h"
#include "dbUGBtnTokusyou.h"
#include "dbUGLiShFilter.h"
#include "dbUGLiBBABuyMenu.h"
#include "dbUGLiSpBBABuy.h"

namespace db
{
  struct TCBBASt
  {
    s32 listpageNo_;
    s64 id_shot_;
    s64 mid_sozai_;
    s64 mid_spsyouhin_;
    s32 mid_bbabuy_;
    enUGLiSellShotType sellfilter_;
    //BOOL baraall_f_;
    BOOL barastar_f_;

    s32 shfilterPage_;
    s32 nextPage_;
    void Init()
    {
      Zeromem(this);
    }
  };


  class TLyHmCBBA :public TLyHmGen
  {
  public:
    TCBBASt stCBBA_;
    
    //コンストラクタ
    explicit TLyHmCBBA(TSnHome* pScene);
    virtual ~TLyHmCBBA(void);
  protected:
    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoDown();
    void DoChangePage(s32 pageno);

    void BaraPriceType();

    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);
    void MvPage_Visit();
    void MvPage_Menu();
    void MvPage_ShEdit();
    void MvPage_RenameShot();
    void MvPage_ShotInv();
    void MvPage_ShotInvEnd();
    void MvPage_BaraMenu();
    void MvPage_Bara();
    void MvPage_ShFilter();
    void MvPage_ShPrev();
    void MvPage_BaraEnd();
    void MvPage_SellMenu();
    void MvPage_SellShot();

    void RefreshSellBtn(BOOL init_f);

    void MvPage_BuySozaiMenu();

    void BuyPriceType();

    void MvPage_BuySozaiType();
    void MvPage_BuySozai();
    void MvPage_BuyShPrev();
    void MvPage_Out();
    //通信
    void Post_LockShot();
    void Post_RenameShot();
    void Post_AddShotInv();
    //void Post_BaraLast();
    void Post_BaraAll();
    void Post_SellShot();
    void Post_BuySozai();
    void Connect_RenameShot();
    void Connect_LockShot();
    void Connect_AddShotInv();
    //void Connect_BaraLast();
    void Connect_BaraAll();
    void Connect_SellShot();
    void Connect_BuySozai();

    void ActListMenu();
    BOOL RefreshPageBtn(BOOL init_f, s32 mv);
    void DrawList(TUGLiGen* drawlist);
    void ResetObj();
    BOOL CheckPostLockShot(s32 nextpage);
    void ResetPrevBtn();

    TUGSpBBA ugbba_;
    TUGDrShot ugdrShot_;
    TUGLiShopMenu uglistMenu_;
    TUGLiShopMenu uglistBaraMenu_;
    TUGLiShopMenu uglistSellMenu_;
    TUGLiBBABuyMenu uglistBuySozaiMenu_;
    TUGLiSpSyouhin uglistShotInv_;
    TUGLiSpBBABuy uglistBuySozai_;
    TUGLiSellShot uglistSellShot_;
    TUGLiBuySozaiType uglistBuySozaiType_;
    TUGLiShFilter uglistShFilter_;//シュートフィルタ
    TUGShot ugshot_;
    LONGVECTOR sellidvec_;
    SOZAIDTVECTOR buyprevvec_;
    std::vector<TUGLiGen*> listvec_;
    std::string inputname_;
    LONGVECTOR shsetvec_;
  };
 
}
