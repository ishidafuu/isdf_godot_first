#pragma once
#include "dbLyHmGen.h"
#include "dbUGChKantoku.h"
#include "dbUGChGirl.h"
#include "dbUGButton.h"
#include "dbUGDoor.h"
#include "dbUGChCaptain.h"
namespace db
{

  struct THmShopSt
  {
    s32 inshopNo_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TLyHmShop :public TLyHmGen
  {
  protected:
    static const s32 SHOPNUM = 3;
    TUIObj* shop_[SHOPNUM];
    TUGChKantoku ugkantoku_;
	TUGChCaptain ugcap_;
    std::vector<TUGChGirl*> ugjk_;
    TUGButton ugbln_item_;
    TUGButton ugbln_gacha_;
    TUGButton ugbln_huku_;

	BOOL isMirror_;
	s32 tipsPersonNo_;

    std::vector<TUGDoor*> ugdoor_;

	s32 subPage_;

    void MvPage_Walking();
    void MvPage_Selected();
    void MvPage_InShop();
    void MvPage_GotoShop();
    void MvPage_OutShop();
	void MvPage_ShowTips(void);
    void MvPage_Idle();

    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoChangePage(s32 pageno);
    void DoUpdate(BOOL draw_f);
  public:
    THmShopSt stHmShop_;

    //コンストラクタ
    explicit TLyHmShop(TSnHome* pScene);
    virtual ~TLyHmShop(void);

	void SetupShop(s32 shopNo);

    void OutShop();
    void Idle();
  };
 
}
