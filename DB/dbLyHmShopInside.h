#pragma once
#include "dbLyHmGen.h"
#include "dbUGLiSpSyouhin.h"
#include "dbUGSpSupply.h"
#include "dbUGBtnTokusyou.h"
namespace db
{
  struct TShopInsideSt
  {
    BOOL msgset_f_;
    s32 wait_c_;
    s32 itemIconNo_;
    s64 mid_supply_;
    void Init()
    {
      Zeromem(this);

      wait_c_ = 60;
    }
  };


  class TLyHmShopInside :public TLyHmGen
  {
  public:
    TShopInsideSt stSI_;

    //コンストラクタ
    explicit TLyHmShopInside(TSnHome* pScene);
    virtual ~TLyHmShopInside(void);

	// メニューの中から開始
	void SetupMenu(void);

  protected:

    void ClearRecords();

    void MvPage_Visit();

    void OutShop();

    void MvPage_Select();
    void MvPage_Decide();
    void MvPage_Out();

    void Post_Buy();
    void Connect_Buy();

    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoDown();
    void DoChangePage(s32 pageno);
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);
    void ActListSyouhin();
    TUGSpSupply ugshop_;
    TUGLiSpSyouhin uglistSyouhin_;
  };
 
}
