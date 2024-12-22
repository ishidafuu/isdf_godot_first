#pragma once
#include "dbInclude.h"


namespace db
{
  enum enDtShSyn
  {
    gs_shot_syn_mid,
    //gs_shot_syn_name,
    gs_power,
    gs_spin,
    gs_base,
    gs_base_rare,
    gs_slot1,
    gs_slot1_rare,
    gs_slot2,
    gs_slot2_rare,
    gs_slot3,
    gs_slot3_rare,
    gs_slot4,
    gs_slot4_rare,
    gs_slot5,
    gs_slot5_rare,
    gs_slot6,
    gs_slot6_rare,
    gs_slot7,
    gs_slot7_rare,
    gsEND,
  };

  class TDtShSyn
  {
  protected:
    INTVECTOR2 intvec2_;
    STRVECTOR strvec_;

    STRVECTOR strvec_OriShName_;
  public:
    explicit TDtShSyn(void);
    virtual ~TDtShSyn(void);

    //読み込み
    void LoadDt(const char *CSVDir, const char *CSVFilename);

    void ClearOrgShName();
    //オリジナルシュート登録
    s32 SetOriShName(const std::string& shname);
    std::string GetOriShName(s32 sno);

    //取得
    std::string GetName(s32 id);
    s32 GetDt(s32 id, enDtShSyn dtNo);
    s32 GetIntvec2Size();
  };
}
