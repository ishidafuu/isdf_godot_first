#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Tournament
  {
    mdm_tournament_pk,//勝ち抜きＩＤ
    mdm_tournament_round,//回戦
    mdm_tournament_probD,//Ｄ率
    mdm_tournament_probC,//Ｃ率
    mdm_tournament_probB,//Ｂ率
    mdm_tournament_probA,//Ａ率
    mdm_tournament_probS,//Ｓ率
    mdm_tournament_probSS,//ＳＳ率
    mdm_tournament_probSSS,//ＳＳＳ率
  };
  
  const std::string M_TOURNAMENTCLM[] = 
  {
    "pk",//勝ち抜きＩＤ
    "round",//回戦
    "probD",//Ｄ率
    "probC",//Ｃ率
    "probB",//Ｂ率
    "probA",//Ａ率
    "probS",//Ｓ率
    "probSS",//ＳＳ率
    "probSSS",//ＳＳＳ率
  };
  
  class TMdm_Tournament: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Tournament();
    virtual ~TMdm_Tournament();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Tournament dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Tournament dtNo);
    s64 GetPKDt(s64 pk, enmdm_Tournament dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Tournament dtNo);
  };
}
