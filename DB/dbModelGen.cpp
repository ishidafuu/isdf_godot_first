#include "dbModelGen.h"

namespace db
{

  TModelGen::TModelGen(const char* name)
  {
    model_.Clear();
    model_.name_ = name;
    myline_ = NGNUM;
    pk_ = NGNUM;
  }

  TModelGen::~TModelGen(void)
  {
  }

  void TModelGen::Init()
  {
  }
  void TModelGen::SetMyPK(s64 pk)
  {
    pk_ = pk;
    myline_ = GetPKLine(pk_);
  }

  s32 TModelGen::GetLineNum()
  {
    return model_.intvecmap_["pk"].size();
  }
  void TModelGen::LoadDt(const char *CSVDir)
  {
    model_.Clear();
    DoLoadDt(CSVDir);
    model_.MappingPK(FALSE);//PKマッピング
  }

  void TModelGen::DoLoadDt(const char *CSVDir)
  {
    //マスターのみ継承先で定義
  }

  //プライマリキーＩＤを渡して行番号を返す
  s32 TModelGen::GetPKLine(s64 pk)
  {
    //"pk"キーが見つからない（取得してない場合もあるので必要）
    if (model_.pkmap_.find(pk) == model_.pkmap_.end()) return NGNUM;
    return model_.pkmap_[pk];
  }

  s64 TModelGen::GenGetLineDt(s32 line, const char* key)
  {
    s64 res = 0;
    if (line < model_.intvecmap_[key].size()) res = model_.intvecmap_[key].at(line);
    return res;
  }
  BOOL TModelGen::GenGetLineDtBOOL(s32 line, const char* key)
  {
    return (GenGetLineDt(line, key) != 0);
  }
  std::string TModelGen::GenGetLineDtStr(s32 line, const char* key)
  {
    std::string res = EMPTYSTR;
    if (line < model_.strvecmap_[key].size()) res = model_.strvecmap_[key].at(line);
    return res;
  }

  //データ取得
  s64 TModelGen::GenGetPKDt(s64 pk, const char* key)
  {
    s64 res = 0;
    s32 line = GetPKLine(pk);//PKの行取得
    if (line != NGNUM) res = model_.intvecmap_[key].at(line);
    return res;
  }
  //データ取得ＢＯＯＬ
  BOOL TModelGen::GenGetPKDtBOOL(s64 pk, const char* key)
  {
    return (GenGetPKDt(pk, key) != 0);
  }

  //データ取得ＳＴＲ
  std::string TModelGen::GenGetPKDtStr(s64 pk, const char* key)
  {
    std::string res = EMPTYSTR;
    s32 line = GetPKLine(pk);//PKの行取得
    if (line != NGNUM) res = model_.strvecmap_[key].at(line);
    return res;
  }


  //データ取得
  s64 TModelGen::GenGetMyDt(const char* key)
  {
    s64 res = 0;
    if (myline_ != NGNUM) res = model_.intvecmap_[key].at(myline_);
    return res;
  }
  //データ取得ＢＯＯＬ
  BOOL TModelGen::GenGetMyDtBOOL(const char* key)
  {
    return (GenGetMyDt(key) != 0);
  }

  //データ取得ＳＴＲ
  std::string TModelGen::GenGetMyDtStr(const char* key)
  {
    std::string res = EMPTYSTR;
    if (myline_ != NGNUM) res = model_.strvecmap_[key].at(myline_);
    return res;
  }

}
