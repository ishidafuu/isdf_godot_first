#include "dbStSnDrop.h"

namespace db
{


  //Dropステート******************************************************
  TStSnDrop::TStSnDrop()
  {
    Init();
  }

  TStSnDrop::~TStSnDrop()
  {
  }

  void TStSnDrop::SetVSHum(BOOL drop_f, s32 itemType, s64 itemID, s32 itemNum)
  {
    Init();
    drop_f_ = drop_f;

    itemType_ = itemType;
    itemID_ = itemID;
    itemNum_ = itemNum;
  }


  void TStSnDrop::SetShiai(s32 dropNo, s32 itemType, s64 itemID, s32 itemNum, s32 tokkouNum, s32 tokkouMag, s32 itemType_cmp, s64 itemID_cmp, s32 itemNum_cmp)
  {
    Init();
    dropNo_ = dropNo;
    itemType_ = itemType;
    itemID_ = itemID;
    itemNum_ = itemNum;
    itemType_cmp_ = itemType_cmp;
    itemID_cmp_ = itemID_cmp;
    itemNum_cmp_ = itemNum_cmp;
    tokkouNum_ = tokkouNum;
    tokkouMag_ = tokkouMag;
  }
  void TStSnDrop::SetEventShiai(BOOL drop_f, s32 itemType, s64 itemID, s32 itemNum)
  {
	  Init();
	  drop_f_ = drop_f;

	  itemType_ = itemType;
	  itemID_ = itemID;
	  itemNum_ = itemNum;
  }

  BOOL TStSnDrop::PopCmp()
  {
    BOOL res = cmp_f_;
    cmp_f_ = FALSE;
    return res;
  }

}
