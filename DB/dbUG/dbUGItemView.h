#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGButton.h"
#include "dbUGLiItemGet.h"
#include "dbUGLiItemGetW.h"
#include "dbUGLiItemSet.h"
#include "dbUGLiItemSetW.h"
#include "dbUGTeam.h"
#include "dbUGShot.h"
#include "dbUGHaikei.h"
#include "dbUGPlateItem.h"
#include "dbUGTitle.h"
#include "dbUGLiSozai.h"
#include "dbUGTmStatePlate.h"

namespace db
{
  struct TUGItemViewSt
  {
    //BOOL okbtn_f_;
    s32 viewtype_;
    BOOL openlist_f_;
    BOOL leftpos_f_;
    void Init()
    {
      Zeromem(this);
    }
  };

  struct TUGItemViewParam
  {
	  enItemType type_;
	  s64 id_;
	  s64 num_;
	  void Init()
	  {
		  Zeromem(this);
	  }
	  void Clear()
	  {
		  type_ = (enItemType)NGNUM;
		  id_ = NGNUM;
		  num_ = NGNUM;
	  }
  };

  class TUGItemView :public TUGGen
  {
  protected:
    void UndrawSingleObj();
    void UndrawListFilter();
    void SetLRPos(BOOL leftpos_f);
    void SingleRefresh(enItemType itemtype, s64 itemid, s64 itemnum, s32 tokkouNum, s32 tokkouMag, BOOL get_f, s32 etcnum, BOOL leftpos_f);
	void SingleRefreshByListSet(enItemType itemtype, s64 itemid, s64 itemnum, s32 tokkouNum, s32 tokkouMag, BOOL get_f, s32 etcnum, BOOL leftpos_f); // 色々と都合良いようにする
    void DoUpdate(BOOL draw_f);
    void DoAction();
	BOOL isDrawFilter_;
  public:
    TUGButton ugbtn_back_;//ＯＫボタン
	TUGButton ugbtn_intro_;	// イントロボタン
    TUGLiItemGet ugliget_;
    TUGLiItemGetW ugligetw_;
	TUGLiItemSet ugliset_;
	TUGLiItemSetW uglisetw_;
    //TUIObj* title_;
    TUGItemViewSt stIV_;
    TUGTitle ugtitle_;

	TUGLiSozai uglisozai_;

    TUGTeam ugteam_;
	TUGTmStatePlate ugTmStPlate_;
    TUGShot ugshot_;
    TUGHaikei ughaikei_;
    TUGPlateItem ugitem_;
     
    std::map<s32, s32> gachalv_;
	
	TUGItemViewParam viewItemParam_;

    //コンストラクタ
    explicit TUGItemView(TUIGenLayer* player);
    virtual ~TUGItemView(void);
    void MakeGroup(TUIObj* parent);
    void ClearGachaLv();
    s32 IncGachaLv(s32 id_team);
    void SetDrawFilter(BOOL draw_f);

	void Refresh_Reward(s64 mid_ranking, s32 rank, BOOL msgok_f, BOOL leftpos_f, BOOL filter_f);
	void Refresh_GetLog(BOOL msgok_f, BOOL leftpos_f, BOOL filter_f);
    void Refresh_GetLogSingle(s32 logno, BOOL msgok_f, BOOL leftpos_f);
    void Refresh_GachaCnt(s64 mid_gachacnt);
    void Refresh_GachaGet(s32 logno);
    void Refresh_NaBoxCnt(s64 mid_naboxcnt);
    void Refresh_HukuCnt(s64 mid_hukucnt);
    void Refresh_View(s64 itemType, s64 itemID, s64 itemNum, BOOL msgok_f, BOOL leftpos_f);
	void Refresh_Shot(SOZAIDTVECTOR sozai, BOOL leftpos_f);
	void Refresh_SozaiList(SOZAIDTVECTOR sozai, BOOL leftpos_f);
    void Refresh_Drop(s64 itemType, s64 itemID, s64 itemNum, s32 tokkouNum, s32 tokkouMag, BOOL comp_f);

	TUGItemViewParam GetSelectViewItem(void);
  };
 
}
