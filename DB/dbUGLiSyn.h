#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcSyn.h"
namespace db
{

  struct TUGLiSynSt
  {
    s64 mid_team_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiSyn :public TUGLiGen
  {
  public:
    //コンストラクタ
    explicit TUGLiSyn(TUIGenLayer* player);
    virtual ~TUGLiSyn(void);
    void Refresh(s64 mid_team);
    s64 GetCommonID();
    s64 GetCommonID_index(s32 index);
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
    std::vector<TUGRcSyn*> records_;
    TUGLiSynSt stLiSyn_;
    TUGRcSyn* MakeObj_Record();
  };
 
}
