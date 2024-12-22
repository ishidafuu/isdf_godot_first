#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcHuku.h"
#include "dbUGListEnd.h"
namespace db
{

  struct TUGLiSpHukuSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };


  //リスト
  class TUGLiSpHuku :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoMakeGroupSub();
    void DoClearRecord();

  public:
    std::vector<TUGRcHuku*> records_;
    TUGListEnd uglistend_;
    TUGLiSpHukuSt stLiSpHuku_;

    //コンストラクタ
    explicit TUGLiSpHuku(TUIGenLayer* player);
    virtual ~TUGLiSpHuku(void);
    void Refresh();
    void ChangeListEnd(BOOL listend_f);
    void SetListEnd(s32 recordidx);
    s64 GetCommonID();
  };
 
}
