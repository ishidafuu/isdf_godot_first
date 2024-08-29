#include "dbDtTitle.h"

namespace db
{
  TDtTitle::TDtTitle(void)
  {
  }

  TDtTitle::~TDtTitle(void)
  {
  }

  //読み込み
  void TDtTitle::LoadDt(const char *CSVDir, const char *CSVFilename)
  {
    intvec2_.clear();
    strvec2_.clear();

    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, CSVFilename, FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      INTVECTOR intvec;
      intvec2_.push_back(intvec);
      for (s32 i2 = 0; i2 < csv.GetIntSize(0); ++i2) intvec2_.at(i).push_back(csv.GetInt(i, i2));

      //なまえ
      STRVECTOR strvec;
      strvec2_.push_back(strvec);
      for (s32 i2 = 0; i2 < csv.GetStrSize(0); ++i2) strvec2_.at(i).push_back(csv.GetStr(i, i2));
    }
  }

  s32 TDtTitle::GetLineNum()
  {
    return intvec2_.size();
  }
  s32 TDtTitle::PKtoLine(s32 pk)
  {
    s32 res = 0;
    for (s32 i = 0; i < intvec2_.size(); ++i)
    {
      if (intvec2_[i][enDtTitle_pk] == pk)
      {
        res = i;
        break;
      }
    }

    return res;
  }

  std::string TDtTitle::GetStrPK(s32 pk, enDtTitleStr dtNo)
  {
    return strvec2_[PKtoLine(pk)][dtNo];
  }
  std::string TDtTitle::GetStrLine(s32 line, enDtTitleStr dtNo)
  {
    return strvec2_[line][(s32)dtNo];
  }
  s32 TDtTitle::GetDtPK(s32 pk, enDtTitle dtNo)
  {
    return intvec2_[PKtoLine(pk)][dtNo];
  }
  s32 TDtTitle::GetDtLine(s32 line, enDtTitle dtNo)
  {
    return intvec2_[line][(s32)dtNo];
  }
}
