#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Connect
  {
    mdm_connect_pk,//通信設定ＩＤ
    mdm_connect_delay,//ディレイ
    mdm_connect_buf,//通信バッファBT
    mdm_connect_senditv,//送信インターバル
    mdm_connect_rollitv,//ロールインターバルBT
    mdm_connect_poscheck_f,//ボール位置チェック
    mdm_connect_staminaMin,//消費スタミナ（分）
    mdm_connect_teamStamina_f,//チームスタミナ消費
  };
  
  const std::string M_CONNECTCLM[] = 
  {
    "pk",//通信設定ＩＤ
    "delay",//ディレイ
    "buf",//通信バッファBT
    "senditv",//送信インターバル
    "rollitv",//ロールインターバルBT
    "poscheck_f",//ボール位置チェック
    "staminaMin",//消費スタミナ（分）
    "teamStamina_f",//チームスタミナ消費
  };
  
  class TMdm_Connect: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Connect();
    virtual ~TMdm_Connect();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Connect dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Connect dtNo);
    s64 GetPKDt(s64 pk, enmdm_Connect dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Connect dtNo);
  };
}
