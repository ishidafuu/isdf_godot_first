#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdMaptip
  {
    mdmaptip_pk,//マップチップＩＤ
    mdmaptip_id_kantoku,//保持監督ＩＤ
    mdmaptip_id_m_maptip,//マスターマップチップＩＤ
    mdmaptip_dropbitA,//敵Ａドロップビット
    mdmaptip_dropbitB,//敵Ｂドロップビット
    mdmaptip_dropbitC,//敵Ｃドロップビット
    mdmaptip_compA_f,//コンプフラグ
    mdmaptip_compB_f,//コンプフラグ
    mdmaptip_compC_f,//コンプフラグ
    mdmaptip_winNum,//クリア回数
    mdmaptip_create_date,//作成日時
    mdmaptip_updated_date,//更新日時
    mdmaptip_del_f,//削除フラグ
  };
  
  const std::string MAPTIPCLM[] = 
  {
    "pk",//マップチップＩＤ
    "id_kantoku",//保持監督ＩＤ
    "id_m_maptip",//マスターマップチップＩＤ
    "dropbitA",//敵Ａドロップビット
    "dropbitB",//敵Ｂドロップビット
    "dropbitC",//敵Ｃドロップビット
    "compA_f",//コンプフラグ
    "compB_f",//コンプフラグ
    "compC_f",//コンプフラグ
    "winNum",//クリア回数
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  class TMdMaptip: public TModelGen
  {
  protected:

  public:
    explicit TMdMaptip();
    virtual ~TMdMaptip();
    s64 GetLineDt(s64 line, enmdMaptip dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdMaptip dtNo);
    s64 GetPKDt(s64 pk, enmdMaptip dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdMaptip dtNo);
  };
}
