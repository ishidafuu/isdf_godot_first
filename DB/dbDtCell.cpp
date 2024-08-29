
#include "dbDtCell.h"


namespace db
{

  const s32 CHARCELLMAX = 256;//キャラ絵枚数最大

  TDtCell::TDtCell(void)
  {
    
  }
  
  TDtCell::~TDtCell(void)
  {
  }
  
  //読み込み
  void TDtCell::LoadDt(const char *CSVDir, const char *CSVFilename)
  {
    dt_.clear();

    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, CSVFilename, FALSE, TRUE);
    
    
    for (s32 i = 0; i < csv.longvec2_.size(); ++i)
    //for (s32 i = 0; i < CHARCELLMAX; ++i)
    {
      if (i >= csv.longvec2_.size()) break;
      INTVECTOR newvec;
      dt_.push_back(newvec);
      for (s32 i2 = 0; i2 < ccEND; ++i2)
      {  
        dt_[i].push_back(csv.longvec2_[i][i2]);
      }
    }
  }
  
  s32 TDtCell::GetDt(s32 bNo, enDtCell dtNo)
  {
    return dt_[bNo][dtNo];
  }
  
}
