#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"
//#include "dbUGRKantoku.h"

namespace db
{
  class TUILayer;

  struct TUGShResultSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGShResult : public TUGGen
  {
  protected:
    //TUGRKantoku ugrkantoku_;
    //TUIObj* plate_;
    std::vector<TUIObj*> texts_;
    void DoUpdate(BOOL draw_f);
  public:
    TUGShResultSt stRProf_;
    void MakeGroup(TUIObj* parent);
    void Refresh();
	void RefreshTimeAttack(void);
    //コンストラクタ
    explicit TUGShResult(TUIGenLayer* player);
    virtual ~TUGShResult(void);
  };
 
}
