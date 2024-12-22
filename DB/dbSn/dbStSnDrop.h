#pragma once
#include "dbStSnGen.h"

namespace db
{
  //Dropステート
  class TStSnDrop : TStSnGen
  {
  public:
    //コンストラクタ
    explicit TStSnDrop();
    virtual ~TStSnDrop();
    void Init(){ Zeromem(this); };
    void SetVSHum(BOOL drop_f, s32 itemType, s64 itemID, s32 itemNum);
    void SetShiai(s32 dropNo, s32 itemType, s64 itemID, s32 itemNum, s32 tokkouNum, s32 tokkouMag, s32 itemType_cmp, s64 itemID_cmp, s32 itemNum_cmp);
	void SetEventShiai(BOOL drop_f, s32 itemType, s64 itemID, s32 itemNum);
    BOOL IsDrop(){ return drop_f_; };
    void SetDrop(){ drop_f_ = TRUE; };
    s32 GetItemType(BOOL cmp_f){ return cmp_f ? itemType_cmp_ : itemType_; };
    s32 GetItemID(BOOL cmp_f){ return cmp_f ? itemID_cmp_ : itemID_; };
    s32 GetItemNum(BOOL cmp_f){ return cmp_f ? itemNum_cmp_ : itemNum_; };
    s32 GetTokkouNum(){ return tokkouNum_; };
    s32 GetTokkouMag(){ return tokkouMag_; };
    void PushCmp(){ cmp_f_ = TRUE; };
    BOOL PopCmp();
  protected:
    //ドロップアイテム
    s32 dropNo_;
    s32 itemType_;
    s64 itemID_;
    s32 itemNum_;
    s32 itemType_cmp_;
    s64 itemID_cmp_;
    s32 itemNum_cmp_;
    s32 tokkouNum_;
    s32 tokkouMag_;
    BOOL cmp_f_;
    BOOL drop_f_;
  };

}
