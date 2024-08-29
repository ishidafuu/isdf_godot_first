#pragma once
#include "dbMgCoGen.h"
#include "dbMdGuild.h"
#include "dbMdGuildbbs.h"

namespace db
{
  class TMgCoGuild : public TMgCoGen
  {
  protected:
    void DoInitSendPack();
  public:
    TMdGuild mdGuild_;//ギルド
    TMdGuildbbs mdGuildbbs_;//ギルド掲示板

    //TModelGen
    explicit TMgCoGuild(void);
    virtual ~TMgCoGuild(void);
  };
}
