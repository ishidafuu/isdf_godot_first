#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGTutoMsgGen.h"


namespace db
{

  struct TUGTutoMsgGouseiSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGTutoMsgGousei : public TUGTutoMsgGen
  {
  protected:
    BOOL DoNextMsg();
  public:
    TUGTutoMsgGouseiSt stTutoMsg_;
    //コンストラクタ
    explicit TUGTutoMsgGousei(TUIGenLayer* player);
    virtual ~TUGTutoMsgGousei(void);
    //void MakeGroup(TUIObj* parent);
    void Ready();

	void CheckSkipMsg(void);
  };
 
}
