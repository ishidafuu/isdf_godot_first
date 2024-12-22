#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcSpSyouhin.h"
#include "dbUGListEnd.h"
namespace db
{

  struct TUGLiSpSyouhinSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };


  //リスト
  class TUGLiSpSyouhin :public TUGLiGen
  {
  protected:
    void DoActionSub();
    void DoUpdateSub(BOOL draw_f);
    void DoMakeGroupSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcSpSyouhin*> records_;
    TUGListEnd uglistend_;
    TUGLiSpSyouhinSt stLiSpSyouhin_;

    //コンストラクタ
    explicit TUGLiSpSyouhin(TUIGenLayer* player);
    virtual ~TUGLiSpSyouhin(void);

    void Refresh(enShopType shoptype);
    void RefreshBuySozai(s32 typest, s32 typeed, BOOL vip_f);
    void ChangeListEnd(BOOL listend_f);
    void SetListEnd(s32 recordidx);
    s64 GetCommonID();

  };
 
}
