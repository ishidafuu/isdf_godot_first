#include "dbMdGiftbox.h"
namespace db
{
  TMdGiftbox::TMdGiftbox():TModelGen("Giftbox")
  {
  }
  TMdGiftbox::~TMdGiftbox(void)
  {
  }
  s64 TMdGiftbox::GetLineDt(s64 line, enmdGiftbox dtNo)
  {
    return GenGetLineDt(line, GIFTBOXCLM[dtNo].c_str());
  }
  BOOL TMdGiftbox::GetLineDtBOOL(s64 line, enmdGiftbox dtNo)
  {
    return GenGetLineDtBOOL(line, GIFTBOXCLM[dtNo].c_str());
  }
  s64 TMdGiftbox::GetPKDt(s64 pk, enmdGiftbox dtNo)
  {
    return GenGetPKDt(pk, GIFTBOXCLM[dtNo].c_str());
  }
  BOOL TMdGiftbox::GetPKDtBOOL(s64 pk, enmdGiftbox dtNo)
  {
    return GenGetPKDtBOOL(pk, GIFTBOXCLM[dtNo].c_str());
  }
}
