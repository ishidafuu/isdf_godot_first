#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGTutoMsgGen.h"

namespace db
{
  class TUGTutoMsgSmart : public TUGTutoMsgGen
  {
  protected:
    BOOL DoNextMsg();
  public:
    //コンストラクタ
    explicit TUGTutoMsgSmart(TUIGenLayer* player);
    virtual ~TUGTutoMsgSmart(void);
  };
 
}
