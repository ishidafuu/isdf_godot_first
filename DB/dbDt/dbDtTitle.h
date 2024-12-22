#pragma once
#include "dbInclude.h"


namespace db
{
  enum enDtTitle
  {
    enDtTitle_pk,
    enDtTitle_platform,
    enDtTitle_iconno,
    enDtTitle_price,
    enDtTitleEND,
  };
  enum enDtTitleStr
  {
    enDtTitleStr_name,
    enDtTitleStr_shortname,
    enDtTitleStr_type,
    enDtTitleStr_day,
    titsEND,
  };

  class TDtTitle
  {
  protected:
    INTVECTOR2 intvec2_;
    STRVECTOR2 strvec2_;
    s32 PKtoLine(s32 pk);
  public:
    explicit TDtTitle(void);
    virtual ~TDtTitle(void);

    //読み込み
    void LoadDt(const char *CSVDir, const char *CSVFilename);
    //取得
    std::string GetStrPK(s32 pk, enDtTitleStr dtNo);
    std::string GetStrLine(s32 line, enDtTitleStr dtNo);
    s32 GetLineNum();
    s32 GetDtPK(s32 pk, enDtTitle dtNo);
    s32 GetDtLine(s32 line, enDtTitle dtNo);
  };
}
