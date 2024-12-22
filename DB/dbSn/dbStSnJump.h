#pragma once
#include "dbStSnGen.h"

namespace db
{
  //ShiaiTypeステート
  class TStSnJump : TStSnGen
  {
  public:
    //コンストラクタ
    explicit TStSnJump();
    virtual ~TStSnJump();
    void ClearJumpAreaData(void)
	{
		fromTab_ = NGNUM;
		jumpAreaData_.clear();
	}
    void SetJumpAreaData(INTVECTOR data){ jumpAreaData_ = data; }
    s32 GetJumpTabNo(void);			// 0:ホームのどのタブか
    s32 GetJumpCategoryNo(void);	// 1:そのタブ内のどの場所か
    INTVECTOR GetJumpData(void);	// 2～:1の場所の中に入ってからの階層位置

	void SetFromTab(s32 no) { fromTab_ = no; }
	s32  GetFromTab(void) { return fromTab_; }
  protected:
    INTVECTOR jumpAreaData_;	// 特殊遷移先
	s32 fromTab_;
  };

}
