#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcNormal.h"
namespace db
{
  struct TUGLiTapeSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiTape :public TUGLiGen
  {
  protected:
    void DoMakeGroupSub();
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcNormal*> records_;
    TUGLiTapeSt stLiTape_;

    //コンストラクタ
    explicit TUGLiTape(TUIGenLayer* player);
    virtual ~TUGLiTape(void);

    TUGRcNormal* MakeObj_Record();
    void Refresh(s64 titleid);
	void Refresh(s64 titleid, s64 mid_event);
    void RefreshIcon();
  };
 
}
