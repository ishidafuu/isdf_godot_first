#pragma once
#include "dbInclude.h"


namespace db
{
  enum enDtShRnd
  {
    gs_shot_rnd_mid,
  };

  class TDtShRnd
  {
  protected:
    INTVECTOR2 intvec2_;

  public:
	  explicit TDtShRnd(void);
	  virtual ~TDtShRnd(void);

    //読み込み
    void LoadDt(const char *CSVDir, const char *CSVFilename);

    //取得
	s32 GetDt(s32 id, enDtShRnd dtNo);
    s32 GetIntvec2Size();
  };
}
