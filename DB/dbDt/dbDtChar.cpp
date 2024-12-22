
#include "dbDtChar.h"


namespace db
{
  const s32 FILEDIV = 100;

  TDtChar::TDtChar(void)
  {

  }
  
  TDtChar::~TDtChar(void)
  {
  }
  
  //読み込み(追記対応)全チーム
  void TDtChar::LoadDt(const char *CSVDir, const char *CSVFilename)
  {
    intvec2_.clear();
    strvec2_.clear();

    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, CSVFilename, FALSE, FALSE);

    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      INTVECTOR intvec;
      STRVECTOR strvec;
      intvec2_.push_back(intvec);
      strvec2_.push_back(strvec);

      for (s32 i2 = 0; i2 < clv_END; ++i2)
      {
        intvec2_[i].push_back(csv.GetInt(i, i2));
      }
      for (s32 i2 = 0; i2 < 2; ++i2)//現状名前,台詞
      {
        strvec2_[i].push_back(csv.GetStr(i, i2));
      }
    }
  }
  
  s32 TDtChar::GetLineNum()
  {
    return  intvec2_.size();
  }
  //データ取得(現状indexとidが同値になっているのでＭＩＤを探索しないでも良い
  s32 TDtChar::GetLooks(enCharLooks dtNo, s32 id)
  {
    return  intvec2_[id][dtNo];
  }

  s32 TDtChar::GetLooks_ZuraFile(s32 id)
  {
    return GetLooks(clk_ZuraNo, id) / FILEDIV;
  }
  s32 TDtChar::GetLooks_ZuraNo(s32 id)
  {
    return GetLooks(clk_ZuraNo, id) % FILEDIV;
  }
  s32 TDtChar::GetLooks_FaceFile(s32 id)
  {
    return GetLooks(clk_FaceNo, id) / FILEDIV;
  }
  s32 TDtChar::GetLooks_FaceNo(s32 id)
  {
    return GetLooks(clk_FaceNo, id) % FILEDIV;
  }


  s32 TDtChar::GetLv(enCharLv dtNo, s32 id)
  {
    return  intvec2_[id][dtNo];
  }
  std::string TDtChar::GetName(s32 id)
  {
    return  strvec2_[id][0];
  }
  std::string TDtChar::GetSerifu(s32 id)
  {
    return  strvec2_[id][1];
  }
  //BOOL TDtChar::IsSpecial(s32 id, enCharSp_fs flag)
  //{
  //  return ((intvec2_[id][clv_Special] & flag) != 0);
  //}
}
