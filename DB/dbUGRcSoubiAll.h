#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcNormal.h"
#include "dbUGButton.h"
#include "dbUGBtnSoubi.h"
namespace db
{

  enum enUGRcSoubiAllAct
  {
    enUGRcSoubiAll_Field,
    enUGRcSoubiAll_Detach,
    //enUGRcSoubiAll_Attach,
    //enUGRcSoubiAll_Eye,
  };

  struct TUGRcSoubiAllSt
  {
    BOOL SoubiAll_f_;
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

  class TUGRcSoubiAll :public TUGRcNormal
  {
  public:
    //コンストラクタ
    explicit TUGRcSoubiAll(TUIGenLayer* player);
    virtual ~TUGRcSoubiAll(void);
    void SetBaseDt(s32 slot, s32 mid_team);
    void SetShotDt(s64 id_shot, s32 mid_team);
    void SetOpenLvDt(s64 openlv);
    void ClearShotDt();
    s32 GetSlot();
    s32 GetShotID();
    BOOL IsActRes(enUGRcSoubiAllAct res);//返値ゲット
  protected:
    void DoActionSub();
    void DoMakeGroupSub2();
    void DoSetLimY2(s32 limu, s32 hhh);
    void DoUpdateSub2(BOOL draw_f);
    TUGRcSoubiAllSt stSoubiAll_;
    TUGBtnSoubi ugbtn_detach_;
    TUIObj* slot_;
    TUIObj* chest_;
    TUIObj* step_;

  };

}
