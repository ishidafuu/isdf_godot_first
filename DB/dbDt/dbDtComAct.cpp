#include "dbDtComAct.h"

namespace db
{
  TDtComAct::TDtComAct(void)
  {
  }

  TDtComAct::~TDtComAct(void)
  {
  }

  //読み込み
  void TDtComAct::LoadDt(const char *CSVDir, const char *CSVFilename_ys, const char *CSVFilename_rs)
  {
    mid::TCSV csv_ys;
    csv_ys.midLoadCSV(CSVDir, CSVFilename_ys, TRUE, TRUE);//16進数
    
    for (s32 i = 0; i < csv_ys.GetLine(); ++i)
    {
      for (s32 i2 = 0; i2 < csv_ys.GetIntSize(0); ++i2)
      {
        dt_ys_[i][i2] = (u8)lib_num::Between255(csv_ys.longvec2_[i][i2]);
      }
    }

    mid::TCSV csv_rs;
    csv_rs.midLoadCSV(CSVDir, CSVFilename_rs, TRUE, TRUE);//16進数

    for (s32 i = 0; i < csv_rs.GetLine(); ++i)
    {
      for (s32 i2 = 0; i2 < csv_rs.GetIntSize(0); ++i2)
      {
        dt_rs_[i][i2] = (u8)lib_num::Between255(csv_rs.longvec2_[i][i2]);
      }
    }

  }

  s32 TDtComAct::GetDt(enDtComAct dtNo, s32 ptn, BOOL ressei_f)
  {
    if (ressei_f)
    {
      return dt_rs_[(s32)dtNo][ptn];
    }
    else
    {
      return dt_ys_[(s32)dtNo][ptn];
    }
    
  }


}
