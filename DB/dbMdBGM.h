#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdBGM
  {
    mdbgm_pk,//ＢＧＭＩＤ
    mdbgm_id_kantoku,//保持監督ＩＤ
    mdbgm_bgmBit00,//ＢＧＭ獲得ビット0
    mdbgm_bgmBit01,//ＢＧＭ獲得ビット1
    mdbgm_bgmBit02,//ＢＧＭ獲得ビット2
    mdbgm_bgmBit03,//ＢＧＭ獲得ビット3
    mdbgm_bgmBit04,//ＢＧＭ獲得ビット4
    mdbgm_bgmBit05,//ＢＧＭ獲得ビット5
    mdbgm_bgmBit06,//ＢＧＭ獲得ビット6
    mdbgm_bgmBit07,//ＢＧＭ獲得ビット7
    mdbgm_bgmBit08,//ＢＧＭ獲得ビット8
    mdbgm_bgmBit09,//ＢＧＭ獲得ビット9
    mdbgm_bgmBit10,//ＢＧＭ獲得ビット10
    mdbgm_bgmBit11,//ＢＧＭ獲得ビット11
    mdbgm_bgmBit12,//ＢＧＭ獲得ビット12
    mdbgm_bgmBit13,//ＢＧＭ獲得ビット13
    mdbgm_bgmBit14,//ＢＧＭ獲得ビット14
    mdbgm_bgmBit15,//ＢＧＭ獲得ビット15
    mdbgm_bgmBit16,//ＢＧＭ獲得ビット16
    mdbgm_bgmBit17,//ＢＧＭ獲得ビット17
    mdbgm_bgmBit18,//ＢＧＭ獲得ビット18
    mdbgm_bgmBit19,//ＢＧＭ獲得ビット19
    mdbgm_bgmBit20,//ＢＧＭ獲得ビット20
    mdbgm_bgmBit21,//ＢＧＭ獲得ビット21
    mdbgm_bgmBit22,//ＢＧＭ獲得ビット22
    mdbgm_bgmBit23,//ＢＧＭ獲得ビット23
    mdbgm_bgmBit24,//ＢＧＭ獲得ビット24
    mdbgm_bgmBit25,//ＢＧＭ獲得ビット25
    mdbgm_bgmBit26,//ＢＧＭ獲得ビット26
    mdbgm_bgmBit27,//ＢＧＭ獲得ビット27
    mdbgm_bgmBit28,//ＢＧＭ獲得ビット28
    mdbgm_bgmBit29,//ＢＧＭ獲得ビット29
    mdbgm_create_date,//作成日時
    mdbgm_updated_date,//更新日時
    mdbgm_del_f,//削除フラグ
  };
  
  const std::string BGMCLM[] = 
  {
    "pk",//ＢＧＭＩＤ
    "id_kantoku",//保持監督ＩＤ
    "bgmBit00",//ＢＧＭ獲得ビット0
    "bgmBit01",//ＢＧＭ獲得ビット1
    "bgmBit02",//ＢＧＭ獲得ビット2
    "bgmBit03",//ＢＧＭ獲得ビット3
    "bgmBit04",//ＢＧＭ獲得ビット4
    "bgmBit05",//ＢＧＭ獲得ビット5
    "bgmBit06",//ＢＧＭ獲得ビット6
    "bgmBit07",//ＢＧＭ獲得ビット7
    "bgmBit08",//ＢＧＭ獲得ビット8
    "bgmBit09",//ＢＧＭ獲得ビット9
    "bgmBit10",//ＢＧＭ獲得ビット10
    "bgmBit11",//ＢＧＭ獲得ビット11
    "bgmBit12",//ＢＧＭ獲得ビット12
    "bgmBit13",//ＢＧＭ獲得ビット13
    "bgmBit14",//ＢＧＭ獲得ビット14
    "bgmBit15",//ＢＧＭ獲得ビット15
    "bgmBit16",//ＢＧＭ獲得ビット16
    "bgmBit17",//ＢＧＭ獲得ビット17
    "bgmBit18",//ＢＧＭ獲得ビット18
    "bgmBit19",//ＢＧＭ獲得ビット19
    "bgmBit20",//ＢＧＭ獲得ビット20
    "bgmBit21",//ＢＧＭ獲得ビット21
    "bgmBit22",//ＢＧＭ獲得ビット22
    "bgmBit23",//ＢＧＭ獲得ビット23
    "bgmBit24",//ＢＧＭ獲得ビット24
    "bgmBit25",//ＢＧＭ獲得ビット25
    "bgmBit26",//ＢＧＭ獲得ビット26
    "bgmBit27",//ＢＧＭ獲得ビット27
    "bgmBit28",//ＢＧＭ獲得ビット28
    "bgmBit29",//ＢＧＭ獲得ビット29
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdBGM: public TModelGen
  {
  protected:

  public:
    explicit TMdBGM();
    virtual ~TMdBGM();
    s64 GetLineDt(s64 line, enmdBGM dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdBGM dtNo);
    s64 GetPKDt(s64 pk, enmdBGM dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdBGM dtNo);
  };
}
