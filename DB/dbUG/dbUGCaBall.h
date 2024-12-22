#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGChSensyu.h"

namespace db
{

  struct TUGCaBallSt
  {
    s32 itemdraw_c;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGCaBall :public TUGGen
  {
  protected:
	  TUIObj* ball_;
	  TUGChSensyu* mem_[DBSIDE];
	  void DoUpdate(BOOL draw_f);
  public:
    TUGCaBallSt stCB_;
    void MakeGroup(TUGChSensyu* memL, TUGChSensyu* memR, TUIObj* ball);
    //コンストラクタ
    explicit TUGCaBall(TUIGenLayer* player);
    virtual ~TUGCaBall(void);

    
  };
 
}
