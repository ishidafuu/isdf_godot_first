#include "dbDtSetting.h"

namespace db
{
  TDtSetting::TDtSetting(void)
  {
  }

  TDtSetting::~TDtSetting(void)
  {
  }

  //読み込み
  void TDtSetting::LoadDt(const char *CSVDir, const char *CSVFilename)
  {
    //dt_->clear();
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, CSVFilename, FALSE, TRUE);
    
    //s32の0番だけ
    //for (s32 i = 0; i < endbSetDtEND; ++i)
    //{
    //  if (i < csv.longvec2_.size())

    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
        //範囲オーバー
        if (i >= endbSetDtEND) break;

      //for (s32 i2 = 0; i2 < csv.GetIntSize(0); ++i2)
      {
        dt_[i] = csv.longvec2_[i][0];
      }
    }
  }



  s32 TDtSetting::GenGetDt(s32 dtNo)
  {
    return dt_[dtNo];
  }
  s32 TDtSetting::GetDtGrv(endtSet_Grv dtNo)
  {
    return GenGetDt(dtNo);
  }
  s32 TDtSetting::GetDtDamage(endtSet_Damage dtNo)
  {
    return GenGetDt(dtNo);
  }
  s32 TDtSetting::GetDtJump(endtSet_Jump dtNo)
  {
    return GenGetDt(dtNo);
  }
  s32 TDtSetting::GetDtCourt(endtSet_Court dtNo)
  {
    return GenGetDt(dtNo);
  }
  s32 TDtSetting::GetDtBall(endtSet_Ball dtNo)
  {
    return GenGetDt(dtNo);
  }
  s32 TDtSetting::GetDtWall(endtSet_Wall dtNo)
  {
    return GenGetDt(dtNo);
  }
  s32 TDtSetting::GetDtInfield(endtSet_Infield dtNo)
  {
    return GenGetDt(dtNo);
  }
  s32 TDtSetting::GetDtCatch(endtSet_Catch dtNo)
  {
    return GenGetDt(dtNo);
  }
  s32 TDtSetting::GetDtShot(endtSet_Shot dtNo)
  {
    return GenGetDt(dtNo);
  }
  s32 TDtSetting::GetDtPass(endtSet_Pass dtNo)
  {
    return GenGetDt(dtNo);
  }
  s32 TDtSetting::GetDtFormation(endtSet_Formation dtNo)
  {
    return GenGetDt(dtNo);
  }
  s32 TDtSetting::GetDtCOM(endtSet_COM dtNo)
  {
    return GenGetDt(dtNo);
  }
  s32 TDtSetting::GetDtInput(endtSet_Input dtNo)
  {
    return GenGetDt(dtNo);
  }
}
