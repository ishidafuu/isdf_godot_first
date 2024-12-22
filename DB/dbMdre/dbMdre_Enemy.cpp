#include "dbMdre_Enemy.h"
namespace db
{
  TMdre_Enemy::TMdre_Enemy():TModelGen("re_Enemy")
  {
  }
  TMdre_Enemy::~TMdre_Enemy(void)
  {
  }
  s64 TMdre_Enemy::GetLineDt(s64 line, enmdre_Enemy dtNo)
  {
    return GenGetLineDt(line, RE_ENEMYCLM[dtNo].c_str());
  }
  BOOL TMdre_Enemy::GetLineDtBOOL(s64 line, enmdre_Enemy dtNo)
  {
    return GenGetLineDtBOOL(line, RE_ENEMYCLM[dtNo].c_str());
  }
  std::string TMdre_Enemy::GetLineDtStr(s64 line, enmdre_EnemyStr dtNo)
  {
    return GenGetLineDtStr(line, RE_ENEMYCLMSTR[dtNo].c_str());
  }
  s64 TMdre_Enemy::GetPKDt(s64 pk, enmdre_Enemy dtNo)
  {
    return GenGetPKDt(pk, RE_ENEMYCLM[dtNo].c_str());
  }
  BOOL TMdre_Enemy::GetPKDtBOOL(s64 pk, enmdre_Enemy dtNo)
  {
    return GenGetPKDtBOOL(pk, RE_ENEMYCLM[dtNo].c_str());
  }
  std::string TMdre_Enemy::GetPKDtStr(s64 pk, enmdre_EnemyStr dtNo)
  {
    return GenGetPKDtStr(pk, RE_ENEMYCLMSTR[dtNo].c_str());
  }
}
