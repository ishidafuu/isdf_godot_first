#include "dbMdlog_CharLvUp.h"
namespace db
{
  TMdlog_CharLvUp::TMdlog_CharLvUp():TModelGen("log_CharLvUp")
  {
  }
  TMdlog_CharLvUp::~TMdlog_CharLvUp(void)
  {
  }
  s64 TMdlog_CharLvUp::GetLineDt(s64 line, enmdlog_CharLvUp dtNo)
  {
    return GenGetLineDt(line, LOG_CHARLVUPCLM[dtNo].c_str());
  }
  BOOL TMdlog_CharLvUp::GetLineDtBOOL(s64 line, enmdlog_CharLvUp dtNo)
  {
    return GenGetLineDtBOOL(line, LOG_CHARLVUPCLM[dtNo].c_str());
  }
  s64 TMdlog_CharLvUp::GetPKDt(s64 pk, enmdlog_CharLvUp dtNo)
  {
    return GenGetPKDt(pk, LOG_CHARLVUPCLM[dtNo].c_str());
  }
  BOOL TMdlog_CharLvUp::GetPKDtBOOL(s64 pk, enmdlog_CharLvUp dtNo)
  {
    return GenGetPKDtBOOL(pk, LOG_CHARLVUPCLM[dtNo].c_str());
  }
}
