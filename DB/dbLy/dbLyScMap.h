#pragma once
#include "dbLyScGen.h"

namespace db
{

  struct TMapSt
  {
    s32 lastmaptip_;
    
    s32 nowmapNo_;
    s32 selectMapNo_;
    s32 left_;
    s32 top_;
    s32 bottom_;
    s32 right_;
    BOOL paused_f_;
    s32 startpos_;

    s32 lastmapvecno_;
    s64 nextmapvecno_;
    BOOL lasthome_f_;
    s32 next_c_;

    void Init()
    {
      Zeromem(this);
    }
  };


  class TLyScMap :public TLyScGen
  {
  public:
    //コンストラクタ
    explicit TLyScMap(TSnScenario* pScene);
    virtual ~TLyScMap(void);
    void SetScrollSize();
    void Return();
    BOOL IsCheckHP(void);
  protected:
    //オーバーロード
    void DoLoad();
    void DoResetBtn();
    void DoReady();
    void DoDown();
    void DoChangePage(s32 pageno);
    void DoUpdate(BOOL draw_f);

    void ClearPanelVec();
    void OpenMap(s32 startpos);
    void OpenChild(s64 posNo, BOOL fc_f);
    void OpenChildXY(s32 posx, s32 posy);
    void OpenChildlen(TUGScPanel* parent);
    void Pause(BOOL pause_f);

    void MvPage_Visit();
    void MvPage_Comp();
    void MvPage_SelectPanel();
    void MvPage_NextPanel();
    void MvPage_Enemy();
    void MvPage_BackSelect();
    void NextMapTip(s32 standpos);
    PANELVECTOR ugpanelvec_;//各マップマス（ふえるたび確保）
    TUGChKantoku ugkantoku_;
    TUGChCaptain ugcap_;
    TUIObj* map_;
    TUIObj* camera_;
    TMapSt stMap_;
    
  };

}
