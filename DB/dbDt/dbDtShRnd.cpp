#include "dbDtShRnd.h"

namespace db
{
  TDtShRnd::TDtShRnd(void)
  {
  }

  TDtShRnd::~TDtShRnd(void)
  {
  }

  //読み込み
  void TDtShRnd::LoadDt(const char *CSVDir, const char *CSVFilename)
  {

    intvec2_.clear();

    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, CSVFilename, FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      INTVECTOR intvec;
      intvec2_.push_back(intvec);
      for (s32 i2 = 0; i2 < csv.GetIntSize(0); ++i2) intvec2_.at(i).push_back(csv.GetInt(i, i2));
    }
  }

  s32 TDtShRnd::GetIntvec2Size()
  {
    return intvec2_.size();
  }
  s32 TDtShRnd::GetDt(s32 id, enDtShRnd dtNo)
  {
    if (id < 0) id = 0;
    if (id >= intvec2_.size()) id = (intvec2_.size() - 1);

    return intvec2_[id][(s32)dtNo];
  }

}
