#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcNormal.h"
namespace db
{
  struct TUGLiColSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiCol :public TUGLiGen
  {
  protected:
    void DoMakeGroupSub();
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcNormal*> records_;
    TUGLiColSt stLiCol_;

    //コンストラクタ
    explicit TUGLiCol(TUIGenLayer* player);
    virtual ~TUGLiCol(void);

    TUGRcNormal* MakeObj_Record();
    void Refresh_Hue();
    void Refresh_Lum(s32 hue);
    void Refresh_SkinHue();
    void Refresh_SkinLum(s32 hue);

    s64 GetCommonID();
  };
 
}
