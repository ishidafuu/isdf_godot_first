#include "dbDtShSyn.h"

namespace db
{
  TDtShSyn::TDtShSyn(void)
  {
  }

  TDtShSyn::~TDtShSyn(void)
  {
  }

  //読み込み
  void TDtShSyn::LoadDt(const char *CSVDir, const char *CSVFilename)
  {

    intvec2_.clear();
    strvec_.clear();

    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, CSVFilename, FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      INTVECTOR intvec;
      intvec2_.push_back(intvec);
      for (s32 i2 = 0; i2 < csv.GetIntSize(0); ++i2) intvec2_.at(i).push_back(csv.GetInt(i, i2));

      //なまえ
      strvec_.push_back(csv.GetStr(i, 0));
    }
  }

  void TDtShSyn::ClearOrgShName()
  {
    strvec_OriShName_.clear();
  }
  //オリジナルシュート登録
  s32 TDtShSyn::SetOriShName(const std::string& shname)
  {
    //番号を返す(0はデフォなので１大きい値)
    //s32 res = strvec_OriShName_.size();
    strvec_OriShName_.push_back(shname);
    return  strvec_OriShName_.size();
  }
  std::string TDtShSyn::GetOriShName(s32 sno)
  {
    return strvec_OriShName_[sno - 1];
  }

  std::string TDtShSyn::GetName(s32 id)
  {
    return strvec_[id];
  }
  s32 TDtShSyn::GetIntvec2Size()
  {
    return intvec2_.size();
  }
  s32 TDtShSyn::GetDt(s32 id, enDtShSyn dtNo)
  {
    if (id < 0) id = 0;
    if (id >= intvec2_.size()) id = (intvec2_.size() - 1);

    return intvec2_[id][(s32)dtNo];
  }

}
