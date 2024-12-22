#pragma once
#include "dbInclude.h"

namespace db
{
  //ボタン種類
  enum enBtnType
  {
    dxbNone = mid::dxNone,
    dxA = mid::dxA,
    dxB = mid::dxB,
    dxX = mid::dxX,
    dxY = mid::dxY,
    dxLB = mid::dxLB,
    dxRB = mid::dxRB,
    dxST = mid::dxST,
    dxSL = mid::dxSL,
    dxFnY = mid::dxFnY,
    dxFnU = mid::dxFnU,
    dxFnI = mid::dxFnI,
    dxFnH = mid::dxFnH,
    dxFnJ = mid::dxFnJ,
    dxFnK = mid::dxFnK,
    dxFn0 = mid::dxFn0,
    dxFn1 = mid::dxFn1,
    dxFn2 = mid::dxFn2,
    dxFn3 = mid::dxFn3,
    dxFn4 = mid::dxFn4,
    dxFn5 = mid::dxFn5,
    dxFn6 = mid::dxFn6,
    dxFn7 = mid::dxFn7,
    dxFn8 = mid::dxFn8,
    dxFn9 = mid::dxFn9,

  };

  //十字種類
  enum enCrsType
  {
    dxcNone = mid::dxNone,
    dxD = mid::dxD,
    dxL = mid::dxL,
    dxR = mid::dxR,
    dxU = mid::dxU,
  };

  enum enTBLBitType
  {
    tbTL_1 = mid::tbTL_1,
    tbTL_2 = mid::tbTL_2,
    tbTL_4 = mid::tbTL_4,
    tbTL_8 = mid::tbTL_8,
    tbTL_a = mid::tbTL_a,//angle
    tbTL_b = mid::tbTL_b,//btn
    tbTL_r = mid::tbTL_r,//release
    tbBL_1 = mid::tbBL_1,
    tbBL_2 = mid::tbBL_2,
    tbBL_4 = mid::tbBL_4,
    tbBL_8 = mid::tbBL_8,
    tbBL_a = mid::tbBL_a,
    tbBL_b = mid::tbBL_b,
    tbBL_r = mid::tbBL_r,
    tbTR_1 = mid::tbTR_1,
    tbTR_2 = mid::tbTR_2,
    tbTR_4 = mid::tbTR_4,
    tbTR_8 = mid::tbTR_8,
    tbTR_a = mid::tbTR_a,
    tbTR_b = mid::tbTR_b,
    tbTR_r = mid::tbTR_r,
    tbBR_1 = mid::tbBR_1,
    tbBR_2 = mid::tbBR_2,
    tbBR_4 = mid::tbBR_4,
    tbBR_8 = mid::tbBR_8,
    tbBR_a = mid::tbBR_a,
    tbBR_b = mid::tbBR_b,
    tbBR_r = mid::tbBR_r,
  };

  const u32 AGLBIT = (tbTL_1 | tbTL_2 | tbTL_4 | tbTL_8);
  const s32 BITSHIFT_BTM = mid::BITSHIFT_BTM;
  const s32 BITSHIFT_RIGHT = mid::BITSHIFT_RIGHT;

  enum enShiftBitType
  {
    sftA = mid::sftA,
    sftB = mid::sftB,
    sftC = mid::sftC,
    sftD = mid::sftD,
  };

  enum enUIBitType
  {
    uiNone = 0x0000,
    uiTouch = (1 << 0),
    uiCmdA = (1 << 1),
    uiCmdB = (1 << 2),
    uiCmdC = (1 << 3),
    uiCmdD = (1 << 4),
    uiOrdBtn = (1 << 5),
  };

}
