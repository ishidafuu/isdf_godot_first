#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcTousen.h"
#include "dbUGListEnd.h"
namespace db
{

  struct TUGLiTousenSt
  {
    //s64 mid_gacha_;
    void Init()
    {
      Zeromem(this);
      //mid_gacha_ = NGNUM;
    }
  };


  //リスト
  class TUGLiTousen :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoMakeGroupSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcTousen*> records_;
    TUGListEnd uglistend_;
    TUGLiTousenSt stLiTousen_;
    //コンストラクタ
    explicit TUGLiTousen(TUIGenLayer* player);
    virtual ~TUGLiTousen(void);
    void Refresh_Gacha();
    void Refresh_Huku();
    void ChangeListEnd(BOOL listend_f);
    void SetListEnd(s32 recordidx);
    s64 GetCommonID();
  };
 
}
