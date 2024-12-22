#pragma once
#include "dbInclude.h"
#include "dbUGDrGen.h"
#include "dbUGHaikei.h"
//#include "dbUGHaikeiUra.h"
namespace db
{

  enum enUGDrHaikeiAct
  {
    enUGDrHaikeiAct_Roll,
    enUGDrHaikeiAct_Stop,
    enUGDrHaikeiAct_Start,
    enUGDrHaikeiAct_OK,
    enUGDrHaikeiAct_Eye,
    enUGDrHaikeiAct_Sub,
    enUGDrHaikeiAct_Sub2,
    enUGDrHaikeiAct_Sub3,
  };

  struct TDrHaikeiSt
  {
    s32 sideno_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //ドラム
  class TUGDrHaikei :public TUGDrGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);//更新
    void DoActionSub();
    void DoStop();
    void DoRoll();
	void DoStart(){};
    void DoReady();
  public:
    TDrHaikeiSt stDrHaikei_;
    std::vector<TUGHaikei*> records_;

    //コンストラクタ
    explicit TUGDrHaikei(TUIGenLayer* player);
    virtual ~TUGDrHaikei(void);
    void MakeGroup(TUIObj* parentr, BOOL leftpos_f, base::TGrPart* grfilm);
//    void Ready();

    BOOL IsActRes(enUGDrHaikeiAct res);
  };
 
}
