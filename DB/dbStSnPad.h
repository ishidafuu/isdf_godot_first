#pragma once
#include "dbStSnGen.h"
#include "dbStChar.h"

namespace db
{
  //Etcステート
  class TStSnPad : TStSnGen
  {
  public:
    //コンストラクタ
    explicit TStSnPad();
    virtual ~TStSnPad();
    void Init(){ Zeromem(this); };
    void SetPadSide(s32 padNo, s32 sideNo);
    void SetPadCOM(s32 padNo);
    BOOL IsPadCtrlSide(s32 sideNo);
    s32 GetPadCtrlSideNo(s32 sideNo);
  protected:
    BOOL use_f_[DBSIDE];
    s32 sideNo_[DBSIDE];
  };

}
