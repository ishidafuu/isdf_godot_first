#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcSpSyouhin.h"
#include "dbUGListEnd.h"
namespace db
{

  struct TUGLiSpGachaSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };


  //リスト
  class TUGLiSpGacha :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoMakeGroupSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcSpSyouhin*> records_;
    TUGListEnd uglistend_;
    TUGLiSpGachaSt stLiSpGacha_;

    //コンストラクタ
    explicit TUGLiSpGacha(TUIGenLayer* player);
    virtual ~TUGLiSpGacha(void);
    void Refresh(BOOL todd_f);
    void ChangeListEnd(BOOL listend_f);
    void SetListEnd(s32 recordidx);
    s64 GetCommonID();
  };
 
}
