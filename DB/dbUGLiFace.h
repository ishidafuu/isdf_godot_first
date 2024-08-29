#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcNormal.h"
namespace db
{
  struct TUGLiFaceSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiFace :public TUGLiGen
  {
  protected:
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcNormal*> records_;

    TUGLiFaceSt stLiFace_;

    //コンストラクタ
    explicit TUGLiFace(TUIGenLayer* player);
    virtual ~TUGLiFace(void);
    TUGRcNormal* MakeObj_Record();
    void RefreshFace(INTVECTOR dtvec);
    void RefreshCol(INTVECTOR dtvec, BOOL skin_f);
    void RefreshCol2(INTVECTOR dtvec, BOOL skin_f);
    s64 GetCommonID();
  };
 
}
