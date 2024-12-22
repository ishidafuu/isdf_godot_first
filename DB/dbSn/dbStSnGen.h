#pragma once
#include "dbInclude.h"

namespace db
{
  //シーン用ステート基礎
  class TStSnGen
  {
  protected:
    //No増減（限界超えたらループ）
    s32 FixNoLoop(s32 tNo, s32 LengthNum, BOOL inc_f);
    //No増減（増減量指定）
    s32 FixNoLoop(s32 tNo, s32 LengthNum, BOOL inc_f, s32 num);
    
    //No増減（限界超えたらループ）
    void FixNoLoopRef(s32* tNo, s32 LengthNum, BOOL inc_f);
    //No増減
    BOOL FixNo(s32* tNo, s32 MaxNum, BOOL inc_f);
    //No増減（変動したかを返す）
    BOOL FixNoRef(s32* tNo, s32 MaxNum, BOOL inc_f);
  };
 
}
