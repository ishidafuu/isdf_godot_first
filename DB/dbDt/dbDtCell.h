#pragma once
#include "dbInclude.h"

namespace db
{
  enum enDtCell
  {
    ccRefNo,
    ccBX,
    ccBY,//基本位置
    ccFNo,//顔番号
    ccFX,
    ccFY,
    ccIX,
    ccIY,
    ccOvNo,//上書きパーツ
    ccOvX,
    ccOvY,
    ccFR,//顔回転角度
    ccFZ,//顔位置、奥行き(0:手前 1:奥)
    ccIZ,//ボール位置、奥行き(0:手前 1:奥)
    ccOvJ,//ボール位置、奥行き(0:手前 1:奥)
    ccEND,
  };

  class TDtCell
  {
  protected:
    //const s32 CHARCELLMAX = 256;//キャラ絵枚数最大
    INTVECTOR2 dt_;
    
  public:
    explicit TDtCell(void);
    virtual ~TDtCell(void);
    
    //読み込み
    void LoadDt(const char *CSVDir, const char *CSVFilename);
    //取得
    s32 GetDt(s32 bNo, enDtCell dtNo);
  };
  
}
