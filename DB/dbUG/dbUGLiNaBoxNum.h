#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcNormal.h"
#include "dbUGListEnd.h"
namespace db
{

  struct TUGLiNaBoxNumSt
  {
    s64 mid_nabox_;
    void Init()
    {
      Zeromem(this);
      mid_nabox_ = NGNUM;
    }
  };


  //リスト
  class TUGLiNaBoxNum :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoMakeGroupSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcNormal*> records_;
    TUGListEnd uglistend_;
    TUGLiNaBoxNumSt stLiNaBoxNum_;
    //コンストラクタ
    explicit TUGLiNaBoxNum(TUIGenLayer* player);
    virtual ~TUGLiNaBoxNum(void);
    void Refresh(s64 mid_nabox);
    void ChangeListEnd(BOOL listend_f);
    void SetListEnd(BOOL otona_f);
  };
 
}
