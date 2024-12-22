#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdKantoku
  {
    mdkantoku_pk,//監督ＩＤ
    mdkantoku_mbgID,//モバゲーＩＤ
    mdkantoku_haveBStar,//購入スター
    mdkantoku_haveFStar,//無料スター
    mdkantoku_haveTodd,//トッド
    mdkantoku_haveHuku,//福引き券
    mdkantoku_totalStar,//消費したスター
    mdkantoku_login_date,//最終ログイン日時
    mdkantoku_stamina,//スタミナ消費秒
    mdkantoku_id_log_shiai,//試合ログＩＤ
    mdkantoku_svmasterVer,//サーバーマスターバージョン番号
    mdkantoku_clmasterVer,//クライアントマスターバージョン番号
    mdkantoku_create_date,//作成日時
    mdkantoku_updated_date,//更新日時
    mdkantoku_del_f,//削除フラグ
  };
  
  enum enmdKantokuStr
  {
    mdkantoku_sessionid_c40,//セッションＩＤ
  };
  

  const std::string KANTOKUCLM[] = 
  {
    "pk",//監督ＩＤ
    "mbgID",//モバゲーＩＤ
    "haveBStar",//購入スター
    "haveFStar",//無料スター
    "haveTodd",//トッド
    "haveHuku",//福引き券
    "totalStar",//消費したスター
    "login_date",//最終ログイン日時
    "stamina",//スタミナ消費秒
    "id_log_shiai",//試合ログＩＤ
    "svmasterVer",//サーバーマスターバージョン番号
    "clmasterVer",//クライアントマスターバージョン番号
    "create_date",//作成日時
    "updated_date",//更新日時
    "del_f",//削除フラグ
  };
  
  const std::string KANTOKUCLMSTR[] = 
  {
    "sessionid_c40",//セッションＩＤ
  };
  
  class TMdKantoku: public TModelGen
  {
  protected:

  public:
    explicit TMdKantoku();
    virtual ~TMdKantoku();
    s64 GetLineDt(s64 line, enmdKantoku dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdKantoku dtNo);
    std::string GetLineDtStr(s64 line, enmdKantokuStr dtNo);
    s64 GetPKDt(s64 pk, enmdKantoku dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdKantoku dtNo);
    std::string GetPKDtStr(s64 pk, enmdKantokuStr dtNo);
    s64 GetMyDt(enmdKantoku dtNo);
    BOOL GetMyDtBOOL(enmdKantoku dtNo);
    std::string GetMyDtStr(enmdKantokuStr dtNo);
  };
}
