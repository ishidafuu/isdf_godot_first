#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdNaBox
  {
    mdnabox_pk,//なつかしＢＯＸＩＤ
    mdnabox_id_kantoku,//監督ＩＤ
    mdnabox_id_m_nabox,//なつかしＢＯＸマスターＩＤ
    mdnabox_openNum,//オープン個数
    mdnabox_openBit,//オープンビット
    mdnabox_lastNo,//最後に開けた番号
    mdnabox_create_date,//作成日時
    mdnabox_updated_date,//更新日時
    mdnabox_del_f,//削除フラグ
  };
  
  const std::string NABOXCLM[] = 
  {
    "pk",//なつかしＢＯＸＩＤ
    "id_kantoku",//監督ＩＤ
    "id_m_nabox",//なつかしＢＯＸマスターＩＤ
    "openNum",//オープン個数
    "openBit",//オープンビット
    "lastNo",//最後に開けた番号
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdNaBox: public TModelGen
  {
  protected:

  public:
    explicit TMdNaBox();
    virtual ~TMdNaBox();
    s64 GetLineDt(s64 line, enmdNaBox dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdNaBox dtNo);
    s64 GetPKDt(s64 pk, enmdNaBox dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdNaBox dtNo);
  };
}
