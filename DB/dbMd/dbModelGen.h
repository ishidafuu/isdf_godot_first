#pragma once
#include "dbInclude.h"


namespace db
{
  //基礎
  class TModelGen
  {
  protected:
    //取得
    s64 pk_;
    s32 myline_;//PKの行取得
    s32 GetPKLine(s64 pk);

    s64 GenGetLineDt(s32 line, const char* key);
    BOOL GenGetLineDtBOOL(s32 line, const char* key);
    std::string GenGetLineDtStr(s32 line, const char* key);

    s64 GenGetPKDt(s64 pk, const char* key);
    BOOL GenGetPKDtBOOL(s64 pk, const char* key);
    std::string GenGetPKDtStr(s64 pk, const char* key);

    //じぶんのデータ取得
    s64 GenGetMyDt(const char* key);
    BOOL GenGetMyDtBOOL(const char* key);
    std::string GenGetMyDtStr(const char* key);

    virtual void DoLoadDt(const char *CSVDir);
  public:
    MODELMAP model_;

    explicit TModelGen(const char* name);
    virtual ~TModelGen(void);
    void Init();
    void SetMyPK(s64 pk);
    s32 GetLineNum();
    void LoadDt(const char *CSVDir);

  };
}
