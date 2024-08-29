#include "dbDtShType.h"

namespace db
{
  TDtShType::TDtShType(void)
  {
  }

  TDtShType::~TDtShType(void)
  {
  }
  s32 TDtShType::GetLineNum()
  {
    return  intvec2_.size();
  }
  //読み込み
  void TDtShType::LoadDt(const char *CSVDir, const char *CSVFilename)
  {
    intvec2_.clear();

    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, CSVFilename, FALSE, TRUE);//数値のみ
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      INTVECTOR intvec;
      intvec2_.push_back(intvec);
      for (s32 i2 = 0; i2 < csv.GetIntSize(0); ++i2) intvec2_.at(i).push_back(csv.GetInt(i, i2));
    }


  }

  s32 TDtShType::GetDtBaseRank(s32 paratype)
  {
    return intvec2_[paratype][sht_basequality];
  }
  s32 TDtShType::GetDtBasePoint(s32 paratype)
  {
    return intvec2_[paratype][sht_basepoint];
  }
  s32 TDtShType::GetDtRev(s32 paratype, s32 rarity)
  {
    return intvec2_[paratype][sht_revD + rarity];
  }
  s32 TDtShType::GetDtRange(s32 paratype, s32 rarity)
  {
    return intvec2_[paratype][sht_rangeD + rarity];
  }

}
