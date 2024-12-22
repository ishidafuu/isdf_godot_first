#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcNormal.h"
#include "dbUGButton.h"
#include "dbUGBtnSoubi.h"
namespace db
{
  
  enum enUGRcShSetAct
  {
    enUGRcShSet_Field,
    enUGRcShSet_Detach,
    //enUGRcShSet_Attach,
    //enUGRcShSet_Eye,
  };

  struct TUGRcShSetSt
  {
    BOOL ShSet_f_;
    BOOL def_f_;
    s64 id_shot_;
    //s32 step_;
    s32 slot_;
    void Init()
    {
      Zeromem(this);
      id_shot_ = NGNUM;

    }
  };

  class TUGRcShSet :public TUGRcNormal
  {
  protected:
    void DoActionSub();
    void DoMakeGroupSub2();
    void DoSetLimY2(s32 limu, s32 hhh);
    void DoUpdateSub2(BOOL draw_f);
  public:
    TUGRcShSetSt stShSet_;
    TUGBtnSoubi ugbtn_detach_;
    TUIObj* slot_;
    //コンストラクタ
    explicit TUGRcShSet(TUIGenLayer* player);
    virtual ~TUGRcShSet(void);
    void SetBaseDt(s32 slot);
    void SetShotDt(s64 id_shot);
    
    void ClearShotDt();

    s32 GetSlot();
    s32 GetShotID();

    BOOL IsActRes(enUGRcShSetAct res);//返値ゲット
  };


  //class TUGRcShSet :public TUGRcNormal
  //{
  //protected:
  //  void DoUpdateSub(BOOL draw_f);
  //  s32 DoMakeGroupSub();
  //  void DoActionSub();
  //  void DoSetLimY(s32 limu, s32 hhh);
  //public:
  //  
  //  TUIObj* name_;
  //  TUIObj* rare_;
  //  TUIObj* step_;
  //  TUGBtnShSet ugbtn_detach_;
  //  //TUGButton ugbtn_eye_;

  //  //コンストラクタ
  //  explicit TUGRcShSet(TUIGenLayer* player);
  //  virtual ~TUGRcShSet(void);
  //  void SetBaseDt(s32 step);
  //  void SetShotDt(s64 shotid, BOOL defshot_f);
  //  void ClearShotDt();

  //  s32 GetStep();
  //  s32 GetShotID();

  //  BOOL IsActRes(enUGRcShSetAct res);//返値ゲット
  //};
}
