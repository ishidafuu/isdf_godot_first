#include "dbMdNaBox.h"
namespace db
{
  TMdNaBox::TMdNaBox():TModelGen("NaBox")
  {
  }
  TMdNaBox::~TMdNaBox(void)
  {
  }
  s64 TMdNaBox::GetLineDt(s64 line, enmdNaBox dtNo)
  {
    return GenGetLineDt(line, NABOXCLM[dtNo].c_str());
  }
  BOOL TMdNaBox::GetLineDtBOOL(s64 line, enmdNaBox dtNo)
  {
    return GenGetLineDtBOOL(line, NABOXCLM[dtNo].c_str());
  }
  s64 TMdNaBox::GetPKDt(s64 pk, enmdNaBox dtNo)
  {
    return GenGetPKDt(pk, NABOXCLM[dtNo].c_str());
  }
  BOOL TMdNaBox::GetPKDtBOOL(s64 pk, enmdNaBox dtNo)
  {
    return GenGetPKDtBOOL(pk, NABOXCLM[dtNo].c_str());
  }
}
