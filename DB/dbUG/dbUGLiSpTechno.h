#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcSpTechno.h"
#include "dbUGListEnd.h"
namespace db
{

  struct TUGLiSpTechnoSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };


  //リスト
  class TUGLiSpTechno :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoMakeGroupSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcSpTechno*> records_;
    std::vector<TUGRcNormal*> records2_;
    TUGListEnd uglistend_;
    TUGLiSpTechnoSt stLiSpTechno_;

    //コンストラクタ
    explicit TUGLiSpTechno(TUIGenLayer* player);
    virtual ~TUGLiSpTechno(void);

    void Refresh();
    void ChangeListEnd(BOOL listend_f);
    void SetListEnd(s32 recordidx);
    s64 GetCommonID();

  };
 
}
