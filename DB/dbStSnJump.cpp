#include "dbStSnJump.h"

namespace db
{
  //ShiaiTypeステート******************************************************
  TStSnJump::TStSnJump()
  {
 
  }

  TStSnJump::~TStSnJump()
  {
  }

  // 0:ホームのどのタブか
  s32 TStSnJump::GetJumpTabNo(void)
  {
	  return jumpAreaData_.empty() ? NGNUM : jumpAreaData_[0];
  }
  // 1:そのタブ内のどの場所か
  s32 TStSnJump::GetJumpCategoryNo(void)
  {
	  if (jumpAreaData_.empty()) { return NGNUM; }			// 空っぽ(指定無し)
	  else if (jumpAreaData_.size() == 1){ return NGNUM; }	// 空っぽ(指定無し)
	  return  jumpAreaData_[1];
  }
  // 2～:1の場所の中に入ってからの階層位置
  INTVECTOR TStSnJump::GetJumpData(void)
  {
	  INTVECTOR v;
	  for (s32 i = 0; i < jumpAreaData_.size(); i++)
	  {
		  // 0,1は上の関数を使って取得するのでいらない
		  if (i == 0 || i == 1) { continue; }
		  v.push_back(jumpAreaData_[i]);
	  }
	  return v;
  }
}
