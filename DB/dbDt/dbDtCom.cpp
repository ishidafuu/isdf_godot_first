#include "dbDtCom.h"

namespace db
{
  TdtComData::TdtComData(void)
  {
  }
  
  TdtComData::~TdtComData(void)
  {
  }
  
  //読み込み
  void TdtComData::LoadDt(const char *CSVDir, const char *CSVFilename)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, CSVFilename, TRUE, TRUE);//16進数
    


    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      for (s32 i2 = 0; i2 < csv.GetIntSize(0); ++i2)
      {
        dt_[i][i2] = (u8)lib_num::Between255(csv.longvec2_[i][i2]);
      }
    }
  }
  
  s32 TdtComData::GetDt(enCom dtNo, s32 ptn)
  {
    return dt_[(s32)dtNo][ptn];
  }
}
