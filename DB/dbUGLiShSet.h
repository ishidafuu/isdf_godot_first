#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcShSet.h"
namespace db
{

  struct TUGLiShSetSt
  {
    s64 id_shset_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiShSet :public TUGLiGen
  {
  protected:
    TUGButton* ugbtn_subRename_;
    TUGButton* ugbtn_subAuto_;
    TUGButton* ugbtn_subClear_;
    void DoMakeGroupSub();
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcShSet*> records_;

    TUGLiShSetSt stLiShSet_;
    TUGRcShSet* MakeObj_Record();
    void Refresh(s64 id_shset, LONGVECTOR* id_shotvec, BOOL init_f);
    void Rename(s32 slot, s64 id_shot);
    //コンストラクタ
    explicit TUGLiShSet(TUIGenLayer* player);
    virtual ~TUGLiShSet(void);

//    BOOL IsActRes(enUGLiShSetAct res);//返値ゲット
  };
 
}
