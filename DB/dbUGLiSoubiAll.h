#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcSoubiAll.h"
namespace db
{

  struct TUGLiSoubiAllSt
  {
    s64 id_team_;
    s32 mid_team_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiSoubiAll :public TUGLiGen
  {
  protected:
    TUGButton* ugbtn_subAuto_;
    TUGButton* ugbtn_subShSet1_;
    TUGButton* ugbtn_subShSet2_;
    TUGButton* ugbtn_subShSet3_;
    TUGButton* ugbtn_subClear_;
    void DoMakeGroupSub();
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcSoubiAll*> records_;

    TUGLiSoubiAllSt stLiSoubiAll_;
    TUGRcSoubiAll* MakeObj_Record();
    void Refresh(s64 id_team, LONGVECTOR* id_shotvec, BOOL init_f);
    void Rename(s32 slot, s64 id_shot);
    //コンストラクタ
    explicit TUGLiSoubiAll(TUIGenLayer* player);
    virtual ~TUGLiSoubiAll(void);

//    BOOL IsActRes(enUGLiSoubiAllAct res);//返値ゲット
  };
 
}
