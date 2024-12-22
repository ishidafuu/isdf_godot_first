#include "dbStSnGen.h"

namespace db
{
  //No増減（限界超えたらループ）
  s32 TStSnGen::FixNoLoop(s32 tNo, s32 LengthNum, BOOL inc_f)
  {
    if (inc_f)
    {
      tNo++;
      if (tNo >= LengthNum) { tNo = 0; }
    }
    else
    {
      tNo--;
      if (tNo < 0) { tNo = (LengthNum - 1); }
    }
    return tNo;
  }
  //No増減（増減量指定）
  s32 TStSnGen::FixNoLoop(s32 tNo, s32 LengthNum, BOOL inc_f, s32 num)
  {
    for (s32 i = 0; i < num; ++i)
    {
      tNo = FixNoLoop(tNo, LengthNum, inc_f);
    }
    return tNo;
  }
  //No増減（限界超えたらループ）
  void TStSnGen::FixNoLoopRef(s32* tNo, s32 LengthNum, BOOL inc_f)
  {
    if (inc_f)
    {
      *tNo += 1;
      if (*tNo >= LengthNum) { *tNo = 0; }
    }
    else
    {
      *tNo -= 1;
      if (*tNo < 0) { *tNo = (LengthNum - 1); }
    }
  }
  //No増減
  BOOL TStSnGen::FixNo(s32* tNo, s32 MaxNum, BOOL inc_f)
  {
    BOOL res = FALSE;
    if (inc_f)
    {
      if (*tNo < (MaxNum - 1))
      {
        *tNo += 1;
        res = TRUE;
      }
    }
    else
    {
      if (*tNo > 0)
      {
        *tNo -= 1;
        res = TRUE;
      }
    }
    return res;
  }
  //No増減（変動したかを返す）
  BOOL TStSnGen::FixNoRef(s32* tNo, s32 MaxNum, BOOL inc_f)
  {
    if (inc_f)
    {
      if (*tNo < MaxNum)
      {
        *tNo += 1;
        return TRUE;
      }
      else
      {
        return FALSE;
      }
    }
    else
    {
      if (*tNo > 0)
      {
        *tNo -= 1;
        return TRUE;
      }
      else
      {
        return FALSE;
      }
    }
  }
  
}
