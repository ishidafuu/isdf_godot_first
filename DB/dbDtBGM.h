#pragma once
#include "dbInclude.h"


namespace db
{
  enum enDtBGM
  {
    enDtBGM_pk,
    enDtBGM_titleid,
    enDtBGM_nsftype,
    enDtBGM_nsftrack,
    enDtBGM_titletrack,
    enDtBGM_loop_f,
    enDtBGM_mid_nabox,
    enDtBGMEND,
  };
  enum enDtBGMStr
  {
    enDtBGMStr_name,
    enDtBGMStr_filename,
    enDtBGMStrEND,
  };

  class TDtBGM
  {
  protected:
    INTVECTOR2 intvec2_;
    STRVECTOR2 strvec2_;
    s32 PKtoLine(s32 pk);
  public:
    explicit TDtBGM(void);
    virtual ~TDtBGM(void);

    //読み込み
    void LoadDt(const char *CSVDir, const char *CSVFilename);
    //取得
    std::string GetStrPK(s32 pk, enDtBGMStr dtNo);
    std::string GetStrLine(s32 line, enDtBGMStr dtNo);
    s32 GetLineNum();
    s32 GetDtPK(s32 pk, enDtBGM dtNo);
    s32 GetDtLine(s32 line, enDtBGM dtNo);
  };
}
