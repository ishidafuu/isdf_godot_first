#include "dbMgCoGuild.h"

namespace db
{
  TMgCoGuild::TMgCoGuild(void) :TMgCoGen("co_guild")
  {

    mdvec_.clear();
    mdvec_.push_back(&mdGuild_);//ギルド
    mdvec_.push_back(&mdGuildbbs_);//ギルド掲示板

    models_.clear();
    for (s32 i = 0; i < mdvec_.size(); ++i)
    {
      models_.push_back(&mdvec_[i]->model_);
    }
  }

  TMgCoGuild::~TMgCoGuild(void)
  {

  }

  void TMgCoGuild::DoInitSendPack()
  {
  
  }

}
