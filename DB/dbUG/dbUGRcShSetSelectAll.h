#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcNormal.h"
#include "dbUGButton.h"
#include "dbUGBtnSoubi.h"
namespace db
{
  

  struct TUGRcShSetSelectAllSt
  {
    BOOL ShSetSelect_f_;
    BOOL def_f_;
    s64 id_shot_;
    s64 mid_team_;
    s32 slot_;
    void Init()
    {
      Zeromem(this);
      id_shot_ = NGNUM;

    }
  };

  class TUGRcShSetSelectAll :public TUGRcNormal
  {
  public:
    //コンストラクタ
    explicit TUGRcShSetSelectAll(TUIGenLayer* player);
    virtual ~TUGRcShSetSelectAll(void);
    void SetBaseDt(s32 slot, s32 mid_team);
    void SetShotDt(s64 id_shot);
    void SetOpenLvDt(s64 openlv);
    //s32 GetSlot();
    //s32 GetShotID();
    //BOOL IsActRes(enUGRcShSetSelectAct res);//返値ゲット
  protected:
    void DoActionSub();
    void DoMakeGroupSub2();
    void DoSetLimY2(s32 limu, s32 hhh);
    void DoUpdateSub2(BOOL draw_f);
    void SetShotDt(s64 id_shot, s32 mid_team);
    TUGRcShSetSelectAllSt stShSetSelect_;
    TUIObj* step_;
    TUIObj* chest_;
    TUIObj* slot_;
  };

}
