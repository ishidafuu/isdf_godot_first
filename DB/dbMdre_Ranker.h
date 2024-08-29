#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdre_Ranker
  {
    mdre_ranker_pk,//ランカーID
    mdre_ranker_id_m_ranking,//マスターランキングID
    mdre_ranker_rank,//順位
    mdre_ranker_score,//スコア
    mdre_ranker_mbgID,//モバゲーＩＤ
    mdre_ranker_avtHair,//髪番号
    mdre_ranker_avtFace,//顔番号
    mdre_ranker_avtHairCol,//髪色
    mdre_ranker_avtSkinCol,//肌色
    mdre_ranker_avtEyeCol,//目色
    mdre_ranker_avtAccCol,//アクセ色
    mdre_ranker_avtClotheCol,//服色
    mdre_ranker_create_date,//作成日時
    mdre_ranker_updated_date,//更新日時
    mdre_ranker_del_f,//削除フラグ
  };
  
  enum enmdre_RankerStr
  {
    mdre_ranker_name_c32,//監督名
    mdre_ranker_prof0_c128,//自己紹介０
    mdre_ranker_prof1_c128,//自己紹介１
    mdre_ranker_prof2_c128,//自己紹介２
  };
  

  const std::string RE_RANKERCLM[] = 
  {
    "pk",//ランカーID
    "id_m_ranking",//マスターランキングID
    "rank",//順位
    "score",//スコア
    "mbgID",//モバゲーＩＤ
    "avtHair",//髪番号
    "avtFace",//顔番号
    "avtHairCol",//髪色
    "avtSkinCol",//肌色
    "avtEyeCol",//目色
    "avtAccCol",//アクセ色
    "avtClotheCol",//服色
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  const std::string RE_RANKERCLMSTR[] = 
  {
    "name_c32",//監督名
    "prof0_c128",//自己紹介０
    "prof1_c128",//自己紹介１
    "prof2_c128",//自己紹介２
  };
  
  class TMdre_Ranker: public TModelGen
  {
  protected:

  public:
    explicit TMdre_Ranker();
    virtual ~TMdre_Ranker();
	s64 GetLineNo(s64 no, enmdre_Ranker dtNo);
	s64 GetLineDt(s64 line, enmdre_Ranker dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdre_Ranker dtNo);
    std::string GetLineDtStr(s64 line, enmdre_RankerStr dtNo);
    s64 GetPKDt(s64 pk, enmdre_Ranker dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdre_Ranker dtNo);
    std::string GetPKDtStr(s64 pk, enmdre_RankerStr dtNo);
  };
}
