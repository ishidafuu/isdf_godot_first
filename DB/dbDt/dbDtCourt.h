#pragma once
#include "dbInclude.h"


namespace db
{
  enum enDtCourt
  {
    enDtCourt_pk,
    enDtCourt_titleid,
    enDtCourt_slip,
    enDtCourt_slow,
    enDtCourt_bound,
    enDtCourt_grv,
    enDtCourt_dam,
    enDtCourt_gb,
    enDtCourt_line_r,
    enDtCourt_line_g,
    enDtCourt_line_b,
    enDtCourt_line_a,
    enDtCourt_mid_nabox,
    enDtCourtEND,
  };
  enum enDtCourtStr
  {
    enDtCourtStr_name,
    enDtCourtStr_filename,
	enDtCourtStr_no,
    enDtCourtStrEND,
  };

  class TDtCourt
  {
  protected:
    INTVECTOR2 intvec2_;
    STRVECTOR2 strvec2_;
    s32 PKtoLine(s32 pk);
  public:
    explicit TDtCourt(void);
    virtual ~TDtCourt(void);

    //読み込み
    void LoadDt(const char *CSVDir, const char *CSVFilename);
    //取得
    std::string GetStrPK(s32 pk, enDtCourtStr dtNo);
    std::string GetStrLine(s32 line, enDtCourtStr dtNo);
    s32 GetLineNum();
    s32 GetDtPK(s32 pk, enDtCourt dtNo);
    s32 GetDtLine(s32 line, enDtCourt dtNo);
  };
}
