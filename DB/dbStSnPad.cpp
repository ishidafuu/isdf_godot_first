#include "dbStSnPad.h"

namespace db
{

  //Etcステート******************************************************
  TStSnPad::TStSnPad()
  {
    Init();
  }

  TStSnPad::~TStSnPad()
  {
  }
  void TStSnPad::SetPadCOM(s32 padNo)
  {
    use_f_[padNo] = FALSE;
    sideNo_[padNo] = 0;
  }
  void TStSnPad::SetPadSide(s32 padNo, s32 sideNo)
  {
    use_f_[padNo] = TRUE;
    sideNo_[padNo] = sideNo;
  }
  //サイド操作
  BOOL TStSnPad::IsPadCtrlSide(s32 sideNo)
  {
    BOOL res = FALSE;
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      if ((sideNo_[i] == sideNo) && (use_f_[i]))
      {
        res = TRUE;
        break;
      }
    }
    return res;
  }
  //サイド操作のパッド番号
  s32 TStSnPad::GetPadCtrlSideNo(s32 sideNo)
  {
    s32 res = NGNUM;
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      if (sideNo_[i] == sideNo)
      {
        res = i;
        break;
      }
    }
    return res;
  }
}
