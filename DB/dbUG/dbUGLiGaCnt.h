#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcNormal.h"
#include "dbUGListEnd.h"
namespace db
{

  struct TUGLiGaCntSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiGaCnt :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
    void DoMakeGroupSub();
  public:
    std::vector<TUGRcNormal*> records_;
    TUGLiGaCntSt stLiGaCnt_;
    TUGListEnd uglistend_;

    //コンストラクタ
    explicit TUGLiGaCnt(TUIGenLayer* player);
    virtual ~TUGLiGaCnt(void);

    void Refresh(s32 cntgrpNo);
    void ChangeListEnd(BOOL listend_f);
    void SetListEnd(s32 recordidx);
    s64 GetCommonID();
  };
 
}
