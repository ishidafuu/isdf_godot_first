#pragma once
#include "dbInclude.h"

namespace db
{
  class TDtChar
  {
  protected:
    INTVECTOR2 intvec2_;
    STRVECTOR2 strvec2_;

  public:
    explicit TDtChar(void);
    virtual ~TDtChar(void);
    
    //読み込み
    void LoadDt(const char *CSVDir, const char *CSVFilename);

    //データ取得(現状indexとidが同値になっているのでＭＩＤを探索しないでも良い
    s32 GetLineNum();
    s32 GetLooks(enCharLooks dtNo, s32 id);
    s32 GetLooks_ZuraFile(s32 id);
    s32 GetLooks_ZuraNo(s32 id);
    s32 GetLooks_FaceFile(s32 id);
    s32 GetLooks_FaceNo(s32 id);

    s32 GetLv(enCharLv dtNo, s32 id);
    std::string GetName(s32 id);
    std::string GetSerifu(s32 id);
    //BOOL IsSpecial(s32 id, enCharSp_fs flag);

  };
}
