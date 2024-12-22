#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"
#include "dbUGButton.h"
#include "dbUGRadio.h"
namespace db
{
  enum enAreaState
  {
    as_Open,
    as_Yokoku,
    as_None,
  };
  class TUILayer;
  class TUGScPanel;

  struct TUGScPanelSt
  {
    static const s32 ROOTNUM = 4;
    s32 mapNo_;
    s32 lno_m_maptip_;
    s64 mid_maptip_;
    s32 lno_maptip_;
    BOOL roadpanel_f_;
    BOOL bosspanel_f_;
    BOOL portpanel_f_;
    BOOL route_f_[ROOTNUM];//ルート
    s32 routeno_[ROOTNUM];//ルート
    s32 posX_;
    s32 posY_;

    BOOL clear_f_;
    enAreaState areastate_;
    enAreaState roadstate_;

    void Init()
    {
      Zeromem(this);
    }
  };

  typedef std::vector<TUGScPanel*> PANELVECTOR;

  //リスト
  class TUGScPanel : public TUGGen
  {
  public:
    //コンストラクタ
    explicit TUGScPanel(TUIGenLayer* player);
    virtual ~TUGScPanel(void);
    void MakeGroup(TUIObj* parent, s32 mapno, s64 lno_maptip, PANELVECTOR* panelvec);
    void SetClear();
    BOOL IsClear();
    enAreaState GetAreaState();
    BOOL CheckParent();
    s64 GetMIDMapTip();
    s64 GetMDt(enmdm_Maptip dt);
    std::string GetMDtStr(enmdm_MaptipStr dt);
    s64 GetDt(enmdMaptip dt);
    BOOL GetDtBOOL(enmdMaptip dt);
    s32 GetLNoMapTip();
    void SetPanelButton(BOOL btn_f);
    BOOL IsClearParent(s64 posNo);
    void SetPanelButtonModal(BOOL modal_f);
    TUGScPanelSt stScPanel_;

  protected:
    static const s32 ROOTNUM = 4;
    void DoUpdate(BOOL draw_f);
    void ChangeState();
    void OpenArea();
    void YokokuArea();
    void NoneArea();
    void OpenRoad();
    void YokokuRoad();
    void NoneRoad();
    void SetScPanelPosGen();
    BOOL IsClearParent(s32 posx, s32 posy);
    TUIObj* noise_;//砂嵐
    TUIObj* bossbg_;//ボス背景
    TUIObj* route_[ROOTNUM];//ルート
    TUIObj* enemy_;//敵
    TUIObj* panel_;//各マップパネル画像//なおす
    TUIObj* frame_;
    TUIObj* state_;
	TUIObj* time_;	// 試合時間
    TUIObj* hp_;
    TUIObj* hp2_;			// HP足りない時用の色違い版
    s32 textChangeTimer_;	// 色違いテキストとの切り替えタイマー
    BOOL newbrink_f_;//NEW点滅
    BOOL comp_f_;
    bool IsChange_;			// 切り替えフラグ
    PANELVECTOR* panelvec_;
  };

}
