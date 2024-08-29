#include "dbMdre_Ranker.h"
namespace db
{
  TMdre_Ranker::TMdre_Ranker():TModelGen("re_Ranker")
  {
  }
  TMdre_Ranker::~TMdre_Ranker(void)
  {
  }
  s64 TMdre_Ranker::GetLineNo(s64 no, enmdre_Ranker dtNo)
  {
	  s64 ret = NGNUM;
	  for (s32 i = 0; i < model_.intvecmap_[RE_RANKERCLM[dtNo].c_str()].size(); i++)
	  {
		  s64 tmpNo = GetLineDt(i, dtNo);
		  if (no == tmpNo)
		  {
			  ret = i;
			  break;
		  }
	  }
	  return ret;
  }
  s64 TMdre_Ranker::GetLineDt(s64 line, enmdre_Ranker dtNo)
  {
    return GenGetLineDt(line, RE_RANKERCLM[dtNo].c_str());
  }
  BOOL TMdre_Ranker::GetLineDtBOOL(s64 line, enmdre_Ranker dtNo)
  {
    return GenGetLineDtBOOL(line, RE_RANKERCLM[dtNo].c_str());
  }
  std::string TMdre_Ranker::GetLineDtStr(s64 line, enmdre_RankerStr dtNo)
  {
    return GenGetLineDtStr(line, RE_RANKERCLMSTR[dtNo].c_str());
  }
  s64 TMdre_Ranker::GetPKDt(s64 pk, enmdre_Ranker dtNo)
  {
    return GenGetPKDt(pk, RE_RANKERCLM[dtNo].c_str());
  }
  BOOL TMdre_Ranker::GetPKDtBOOL(s64 pk, enmdre_Ranker dtNo)
  {
    return GenGetPKDtBOOL(pk, RE_RANKERCLM[dtNo].c_str());
  }
  std::string TMdre_Ranker::GetPKDtStr(s64 pk, enmdre_RankerStr dtNo)
  {
    return GenGetPKDtStr(pk, RE_RANKERCLMSTR[dtNo].c_str());
  }
}
