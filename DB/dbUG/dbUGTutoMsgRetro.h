#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGTutoMsgGen.h"


namespace db
{
  class TUGTutoMsgRetro : public TUGTutoMsgGen
  {
  protected:
    BOOL DoNextMsg();
  public:
    //コンストラクタ
    explicit TUGTutoMsgRetro(TUIGenLayer* player);
    virtual ~TUGTutoMsgRetro(void);
  };
 
}
