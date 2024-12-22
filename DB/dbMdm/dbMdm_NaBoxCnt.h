#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_NaBoxCnt
  {
    mdm_naboxcnt_pk,//なつかしＢＯＸ中身詳細ＩＤ
    mdm_naboxcnt_id_m_nabox,//なつかしＢＯＸＩＤ
    mdm_naboxcnt_boxNo,//ボックス番号
    mdm_naboxcnt_itemType,//アイテムタイプ
    mdm_naboxcnt_itemID,//アイテムID
    mdm_naboxcnt_itemNum,//アイテム個数
  };
  
  const std::string M_NABOXCNTCLM[] = 
  {
    "pk",//なつかしＢＯＸ中身詳細ＩＤ
    "id_m_nabox",//なつかしＢＯＸＩＤ
    "boxNo",//ボックス番号
    "itemType",//アイテムタイプ
    "itemID",//アイテムID
    "itemNum",//アイテム個数
  };
  
  class TMdm_NaBoxCnt: public TModelGen
  {
  protected:

  public:
    explicit TMdm_NaBoxCnt();
    virtual ~TMdm_NaBoxCnt();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_NaBoxCnt dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_NaBoxCnt dtNo);
    s64 GetPKDt(s64 pk, enmdm_NaBoxCnt dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_NaBoxCnt dtNo);
  };
}
