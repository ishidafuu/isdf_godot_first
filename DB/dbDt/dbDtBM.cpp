#include "dbDtBM.h"

namespace db
{
  TBMData::TBMData(void)
  {
  }
  
  TBMData::~TBMData(void)
  {
  }
  
  //読み込み
  void TBMData::LoadDt(const char *CSVDir, const char *CSVFilename)
  {
    bmdt_.clear();

    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, CSVFilename, FALSE, TRUE);
    
    //空フレームデータ
    BMKomaData empfrdt;
    
    Zeromem(&empfrdt);
    
    BMData empbmdt;
    empbmdt.komanum_ = 0;
    
    s32 bmnum = csv.longvec2_[0].size();

    //技数分確保
    for (s32 i = 0; i < bmnum; ++i)
    {
      bmdt_.push_back(empbmdt);
      //コマ数
      bmdt_[i].komanum_ = csv.longvec2_[0][i];
    }
    
    //行（フレームデータは全て繋がっているため　０行目はヘッダなのでその次から）
    s32 fline = 1;
    
    //技数
    for (s32 i = 0; i < bmnum; ++i)
    {
      ////コマ数
      //bmdt_[i].komanum_ = csv.intvec2_[line][0];
      
      //フレーム数
      for (s32 i2 = 0; i2 < bmdt_[i].komanum_; ++i2)
      {
        //駒追加
        bmdt_[i].komadt_.push_back(empfrdt);
        //１コマ情報
        for (s32 i3 = 0; i3 < bmc_END; ++i3)
        {
          s32 zxcv = csv.longvec2_[fline][i3];
          bmdt_[i].komadt_[i2].dt_[i3] = csv.longvec2_[fline][i3];
        }
        ++fline;
      }
    }
  }
  //コマ数
  s32 TBMData::Getkomanum(s32 bmNo)
  {
    return bmdt_[bmNo].komanum_;
  }
  //取得
  s32 TBMData::GenGetDt(s32 bmNo, s32 komaNo, s32 dtNo)
  {
    return bmdt_[bmNo].komadt_[komaNo].dt_[dtNo];
  }
  s32 TBMData::GetDt(s32 bmNo, s32 komaNo, enBMCommonDt dtNo)
  {
    return GenGetDt(bmNo, komaNo, dtNo);
  }
  //s32 TBMData::GetDt(s32 bmNo, s32 komaNo, enBMEffectDt dtNo)
  //{
  //  return GenGetDt(bmNo, komaNo, dtNo);
  //}
}
