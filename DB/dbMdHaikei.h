#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdHaikei
  {
    mdhaikei_pk,//背景ＩＤ
    mdhaikei_id_kantoku,//保持監督ＩＤ
    mdhaikei_haikeiBit00,//背景獲得ビット0
    mdhaikei_haikeiBit01,//背景獲得ビット1
    mdhaikei_haikeiBit02,//背景獲得ビット2
    mdhaikei_haikeiBit03,//背景獲得ビット3
    mdhaikei_haikeiBit04,//背景獲得ビット4
    mdhaikei_haikeiBit05,//背景獲得ビット5
    mdhaikei_haikeiBit06,//背景獲得ビット6
    mdhaikei_haikeiBit07,//背景獲得ビット7
    mdhaikei_haikeiBit08,//背景獲得ビット8
    mdhaikei_haikeiBit09,//背景獲得ビット9
    mdhaikei_haikeiBit10,//背景獲得ビット10
    mdhaikei_haikeiBit11,//背景獲得ビット11
    mdhaikei_haikeiBit12,//背景獲得ビット12
    mdhaikei_haikeiBit13,//背景獲得ビット13
    mdhaikei_haikeiBit14,//背景獲得ビット14
    mdhaikei_haikeiBit15,//背景獲得ビット15
    mdhaikei_haikeiBit16,//背景獲得ビット16
    mdhaikei_haikeiBit17,//背景獲得ビット17
    mdhaikei_haikeiBit18,//背景獲得ビット18
    mdhaikei_haikeiBit19,//背景獲得ビット19
    mdhaikei_haikeiBit20,//背景獲得ビット20
    mdhaikei_haikeiBit21,//背景獲得ビット21
    mdhaikei_haikeiBit22,//背景獲得ビット22
    mdhaikei_haikeiBit23,//背景獲得ビット23
    mdhaikei_haikeiBit24,//背景獲得ビット24
    mdhaikei_haikeiBit25,//背景獲得ビット25
    mdhaikei_haikeiBit26,//背景獲得ビット26
    mdhaikei_haikeiBit27,//背景獲得ビット27
    mdhaikei_haikeiBit28,//背景獲得ビット28
    mdhaikei_haikeiBit29,//背景獲得ビット29
    mdhaikei_create_date,//作成日時
    mdhaikei_updated_date,//更新日時
    mdhaikei_del_f,//削除フラグ
  };
  
  const std::string HAIKEICLM[] = 
  {
    "pk",//背景ＩＤ
    "id_kantoku",//保持監督ＩＤ
    "haikeiBit00",//背景獲得ビット0
    "haikeiBit01",//背景獲得ビット1
    "haikeiBit02",//背景獲得ビット2
    "haikeiBit03",//背景獲得ビット3
    "haikeiBit04",//背景獲得ビット4
    "haikeiBit05",//背景獲得ビット5
    "haikeiBit06",//背景獲得ビット6
    "haikeiBit07",//背景獲得ビット7
    "haikeiBit08",//背景獲得ビット8
    "haikeiBit09",//背景獲得ビット9
    "haikeiBit10",//背景獲得ビット10
    "haikeiBit11",//背景獲得ビット11
    "haikeiBit12",//背景獲得ビット12
    "haikeiBit13",//背景獲得ビット13
    "haikeiBit14",//背景獲得ビット14
    "haikeiBit15",//背景獲得ビット15
    "haikeiBit16",//背景獲得ビット16
    "haikeiBit17",//背景獲得ビット17
    "haikeiBit18",//背景獲得ビット18
    "haikeiBit19",//背景獲得ビット19
    "haikeiBit20",//背景獲得ビット20
    "haikeiBit21",//背景獲得ビット21
    "haikeiBit22",//背景獲得ビット22
    "haikeiBit23",//背景獲得ビット23
    "haikeiBit24",//背景獲得ビット24
    "haikeiBit25",//背景獲得ビット25
    "haikeiBit26",//背景獲得ビット26
    "haikeiBit27",//背景獲得ビット27
    "haikeiBit28",//背景獲得ビット28
    "haikeiBit29",//背景獲得ビット29
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdHaikei: public TModelGen
  {
  protected:

  public:
    explicit TMdHaikei();
    virtual ~TMdHaikei();
    s64 GetLineDt(s64 line, enmdHaikei dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdHaikei dtNo);
    s64 GetPKDt(s64 pk, enmdHaikei dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdHaikei dtNo);
  };
}
