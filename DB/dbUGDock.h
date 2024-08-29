#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGDoor.h"
#include "dbUGButton.h"
#include "dbUGChClerk.h"

namespace db
{
  enum enDockState
  {
    enDockState_Empty,
    enDockState_Gousei,
    enDockState_Finish,
    enDockState_NextOpen,
    enDockState_Close,
    //enDockState_Opening,
  };


  struct TUGDockSt
  {
    enDockState state_;
    s32 dockno_;
    s64 id_dock_;
    s32 mv_c_;
    BOOL wait_f_;

    s64 resttime_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGDock : public TUGGen
  {
  public:
    //コンストラクタ
    explicit TUGDock(TUIGenLayer* player);
    virtual ~TUGDock(void);
    void MakeGroup(TUIObj* parent, base::TGrPart* grpshutter, base::TGrPart* grpdoor, s32 dockno);
    void ChangeGousei(BOOL gousei_f);
    s32 GetDockNo();
    s64 GetIDDock();
    void SetEnable(s64 id_dock, BOOL add_f);
    void SetDisable(BOOL nextopen_f);
    void SetFinish();
    void MoveState();
    void SetWait(BOOL wait_f);
    enDockState GetDockState();

    TUGButton ugbtn_;//ボタン
    TUGChClerk ugclerk_;//店員
    TUGDoor ugdoor_;//ドア
    TUGDockSt stDock_;

  protected:
    void DoUpdate(BOOL draw_f);
    void DoAction();

    void Mv_Empty();
    void Mv_Gousei();
    void Mv_Finish();
    void Mv_NextOpen();
    void Mv_Close();
    void Mv_Opening();
    //TUIObj* ball_;//ボール
    TUGDoor ugshutter_;//シャッター

  };
 
}
