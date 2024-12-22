#pragma once
#include "dbModelGen.h"

namespace db
{

  enum enmdm_Maptip
  {
    mdm_maptip_pk,//マスターマップチップＩＤ
    mdm_maptip_id_m_event,//イベントＩＤ
    mdm_maptip_mapNo,//マップ番号
    mdm_maptip_posNo,//ローカル位置番号
    mdm_maptip_staminaMin,//消費スタミナ（分）
    mdm_maptip_shiaiSec,//試合時間（秒）
    mdm_maptip_prob_B,//敵Ｂ出現万分率
    mdm_maptip_prob_C,//敵Ｃ出現万分率
    mdm_maptip_start_f,//スタートポジション
    mdm_maptip_warpNo,//ワープゾーン番号
    mdm_maptip_posX,//Ｘ座標
    mdm_maptip_posY,//Ｙ座標
    mdm_maptip_id_m_haikei,//背景ＩＤ
    mdm_maptip_id_m_bgm,//ＢＧＭＩＤ
    mdm_maptip_roadUp,//上ルート 1× 2親 3長 4子
    mdm_maptip_roadDown,//下ルート
    mdm_maptip_roadLeft,//左ルート
    mdm_maptip_roadRight,//右ルート
    mdm_maptip_area,//エリア 1なし 2ボス 3雑魚 4バス 5空港 6─ 7│ 8┘ 9┐ 10┌ 11└
    mdm_maptip_ykkarea,//予告エリア 1隠し 2表示
    mdm_maptip_ykkroad,//予告ルート 1隠し 2表示
    mdm_maptip_bossOpen,//ボス公開 1？顔 2表示
    mdm_maptip_fc0,//確認遠子０
    mdm_maptip_fc1,//確認遠子１
    mdm_maptip_fc2,//確認遠子２
    mdm_maptip_fc3,//確認遠子３
    mdm_maptip_fc4,//確認遠子４
    mdm_maptip_fc5,//確認遠子５
    mdm_maptip_fc6,//確認遠子６
    mdm_maptip_fc7,//確認遠子７
    mdm_maptip_fp0,//確認遠親０
    mdm_maptip_fp1,//確認遠親１
    mdm_maptip_fp2,//確認遠親２
    mdm_maptip_fp3,//確認遠親３
    mdm_maptip_fp4,//確認遠親４
    mdm_maptip_fp5,//確認遠親５
    mdm_maptip_fp6,//確認遠親６
    mdm_maptip_fp7,//確認遠親７
    mdm_maptip_id_m_teamA,//敵ＡチームＩＤ
    mdm_maptip_catchA,//敵Ａキャッチ率（百分率）
    mdm_maptip_niceA,//敵Ａナイスキャッチ率（百分率）
    mdm_maptip_id_m_drop_A,//敵ＡドロップパターンＩＤ
    mdm_maptip_prob_sdA,//敵Ａ固有ドロップ率（万分率）
    mdm_maptip_itemType_sdA,//敵Ａ固有アイテムタイプ
    mdm_maptip_itemID_sdA,//敵Ａ固有アイテムＩＤ
    mdm_maptip_itemNum_sdA,//敵Ａ固有アイテム個数
    mdm_maptip_id_m_teamB,//敵ＢチームＩＤ
    mdm_maptip_catchB,//敵Ｂキャッチ率（百分率）
    mdm_maptip_niceB,//敵Ｂナイスキャッチ率（百分率）
    mdm_maptip_id_m_drop_B,//敵ＢドロップパターンＩＤ
    mdm_maptip_prob_sdB,//敵Ｂ固有ドロップ率（万分率）
    mdm_maptip_itemType_sdB,//敵Ｂ固有アイテムタイプ
    mdm_maptip_itemID_sdB,//敵Ｂ固有アイテムＩＤ
    mdm_maptip_itemNum_sdB,//敵Ｂ固有アイテム個数
    mdm_maptip_id_m_teamC,//敵ＣチームＩＤ
    mdm_maptip_catchC,//敵Ｃキャッチ率（百分率）
    mdm_maptip_niceC,//敵Ｃナイスキャッチ率（百分率）
    mdm_maptip_id_m_drop_C,//敵ＣドロップパターンＩＤ
    mdm_maptip_prob_sdC,//敵Ｃ固有ドロップ率（万分率）
    mdm_maptip_itemType_sdC,//敵Ｃ固有アイテムタイプ
    mdm_maptip_itemID_sdC,//敵Ｃ固有アイテムＩＤ
    mdm_maptip_itemNum_sdC,//敵Ｃ固有アイテム個数
  };
  
  enum enmdm_MaptipStr
  {
    mdm_maptip_name_c64,//マップチップの名前
    mdm_maptip_text0_c128,//ボステキスト０
    mdm_maptip_text1_c128,//ボステキスト１
    mdm_maptip_text2_c128,//ボステキスト２
    mdm_maptip_text3_c128,//ボステキスト３
  };
  

  const std::string M_MAPTIPCLM[] = 
  {
    "pk",//マスターマップチップＩＤ
    "id_m_event",//イベントＩＤ
    "mapNo",//マップ番号
    "posNo",//ローカル位置番号
    "staminaMin",//消費スタミナ（分）
    "shiaiSec",//試合時間（秒）
    "prob_B",//敵Ｂ出現万分率
    "prob_C",//敵Ｃ出現万分率
    "start_f",//スタートポジション
    "warpNo",//ワープゾーン番号
    "posX",//Ｘ座標
    "posY",//Ｙ座標
    "id_m_haikei",//背景ＩＤ
    "id_m_bgm",//ＢＧＭＩＤ
    "roadUp",//上ルート 1× 2親 3長 4子
    "roadDown",//下ルート
    "roadLeft",//左ルート
    "roadRight",//右ルート
    "area",//エリア 1なし 2ボス 3雑魚 4バス 5空港 6─ 7│ 8┘ 9┐ 10┌ 11└
    "ykkarea",//予告エリア 1隠し 2表示
    "ykkroad",//予告ルート 1隠し 2表示
    "bossOpen",//ボス公開 1？顔 2表示
    "fc0",//確認遠子０
    "fc1",//確認遠子１
    "fc2",//確認遠子２
    "fc3",//確認遠子３
    "fc4",//確認遠子４
    "fc5",//確認遠子５
    "fc6",//確認遠子６
    "fc7",//確認遠子７
    "fp0",//確認遠親０
    "fp1",//確認遠親１
    "fp2",//確認遠親２
    "fp3",//確認遠親３
    "fp4",//確認遠親４
    "fp5",//確認遠親５
    "fp6",//確認遠親６
    "fp7",//確認遠親７
    "id_m_teamA",//敵ＡチームＩＤ
    "catchA",//敵Ａキャッチ率（百分率）
    "niceA",//敵Ａナイスキャッチ率（百分率）
    "id_m_drop_A",//敵ＡドロップパターンＩＤ
    "prob_sdA",//敵Ａ固有ドロップ率（万分率）
    "itemType_sdA",//敵Ａ固有アイテムタイプ
    "itemID_sdA",//敵Ａ固有アイテムＩＤ
    "itemNum_sdA",//敵Ａ固有アイテム個数
    "id_m_teamB",//敵ＢチームＩＤ
    "catchB",//敵Ｂキャッチ率（百分率）
    "niceB",//敵Ｂナイスキャッチ率（百分率）
    "id_m_drop_B",//敵ＢドロップパターンＩＤ
    "prob_sdB",//敵Ｂ固有ドロップ率（万分率）
    "itemType_sdB",//敵Ｂ固有アイテムタイプ
    "itemID_sdB",//敵Ｂ固有アイテムＩＤ
    "itemNum_sdB",//敵Ｂ固有アイテム個数
    "id_m_teamC",//敵ＣチームＩＤ
    "catchC",//敵Ｃキャッチ率（百分率）
    "niceC",//敵Ｃナイスキャッチ率（百分率）
    "id_m_drop_C",//敵ＣドロップパターンＩＤ
    "prob_sdC",//敵Ｃ固有ドロップ率（万分率）
    "itemType_sdC",//敵Ｃ固有アイテムタイプ
    "itemID_sdC",//敵Ｃ固有アイテムＩＤ
    "itemNum_sdC",//敵Ｃ固有アイテム個数
  };
  
  const std::string M_MAPTIPCLMSTR[] = 
  {
    "name_c64",//マップチップの名前
    "text0_c128",//ボステキスト０
    "text1_c128",//ボステキスト１
    "text2_c128",//ボステキスト２
    "text3_c128",//ボステキスト３
  };
  
  class TMdm_Maptip: public TModelGen
  {
  protected:

  public:
    explicit TMdm_Maptip();
    virtual ~TMdm_Maptip();
    void DoLoadDt(const char *CSVDir);
    s64 GetLineDt(s64 line, enmdm_Maptip dtNo);
    BOOL GetLineDtBOOL(s64 line, enmdm_Maptip dtNo);
    std::string GetLineDtStr(s64 line, enmdm_MaptipStr dtNo);
    s64 GetPKDt(s64 pk, enmdm_Maptip dtNo);
    BOOL GetPKDtBOOL(s64 pk, enmdm_Maptip dtNo);
    std::string GetPKDtStr(s64 pk, enmdm_MaptipStr dtNo);
  };
}
