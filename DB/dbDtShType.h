#pragma once
#include "dbInclude.h"


namespace db
{
  enum enDtShType
  {
    sht_basequality,
    sht_basepoint,
    sht_revD,
    sht_revC,
    sht_revB,
    sht_revA,
    sht_revS,
    sht_revSS,
    sht_revSSS,
    sht_rangeD,
    sht_rangeC,
    sht_rangeB,
    sht_rangeA,
    sht_rangeS,
    sht_rangeSS,
    sht_rangeSSS,
  };

  class TDtShType
  {
  protected:
    INTVECTOR2 intvec2_;
  public:
    explicit TDtShType(void);
    virtual ~TDtShType(void);

    //読み込み
    void LoadDt(const char *CSVDir, const char *CSVFilename);
    s32 GetDtBaseRank(s32 paratype);
    s32 GetDtBasePoint(s32 paratype);
    s32 GetDtRev(s32 paratype, s32 rarity);
    s32 GetDtRange(s32 paratype, s32 rarity);
    s32 GetLineNum();
  };
}
