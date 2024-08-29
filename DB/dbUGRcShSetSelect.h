#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcNormal.h"
#include "dbUGButton.h"
#include "dbUGRIcon.h"
//#include "dbUGBtnShSetSelect.h"
namespace db
{
  
  enum enUGRcShSetSelectAct
  {
    enUGRcShSetSelect_Field,
    enUGRcShSetSelect_Detach,
    //enUGRcShSetSelect_Attach,
    //enUGRcShSetSelect_Eye,
  };

  struct TUGRcShSetSelectSt
  {
    BOOL ShSetSelect_f_;
    BOOL def_f_;
    s64 id_shot_;
    s32 step_;
    s32 slot_;
    void Init()
    {
      Zeromem(this);
      id_shot_ = NGNUM;

    }
  };

  class TUGRcShSetSelect :public TUGRcNormal
  {
  public:

    explicit TUGRcShSetSelect(TUIGenLayer* player);
    virtual ~TUGRcShSetSelect(void);
    //void SetBaseDt(s32 step, s32 slot);
    void SetBaseDt_ShSet(s32 step, s32 slot);
    void SetOpenLvDt(s64 openlv);
    //void SetShotDt(s64 id_shot, BOOL defshot_f);
    //void SwitchEdit(BOOL edit_f);
    void SetShotDt_ShSet(s64 id_shot, BOOL defshot_f);
    void ClearShotDt();

    //s32 GetSlot();
    //s32 GetShotID();
  protected:
    void DoActionSub();
    void DoMakeGroupSub2();
    void DoSetLimY2(s32 limu, s32 hhh);
    void DoUpdateSub2(BOOL draw_f);
    TUGRcShSetSelectSt stShSetSelect_;
    TUIObj* step_;
    TUGRIcon ugball_;
    //TUIObj* slot_;
    //BOOL IsActRes(enUGRcShSetSelectAct res);//返値ゲット
  };

}
