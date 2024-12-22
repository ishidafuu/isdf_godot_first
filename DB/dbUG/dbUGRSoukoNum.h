#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"
#include "dbUGRKantoku.h"

namespace db
{
  class TUILayer;

  struct TUGRSoukoNumSt
  {
    BOOL inv_f_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGRSoukoNum : public TUGGen
  {
  protected:
    std::vector<TUIObj*> num_;//各ラインパーツ
    void DoUpdate(BOOL draw_f);
  public:
    TUGRSoukoNumSt stRSoukoNum_;
    //void SetLimY(s32 limu, s32 hhh);
    //コンストラクタ
    explicit TUGRSoukoNum(TUIGenLayer* player);
    virtual ~TUGRSoukoNum(void);

    void MakeGroup(TUIObj* parent, BOOL inv_f);
    void Refresh();
  };
 
}
