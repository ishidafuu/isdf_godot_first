#pragma once
#include "dbInclude.h"

namespace db
{
  enum enCom//オプション
  {
    comRessei,//劣勢判断 //体力一定以下いれば	残りが敵未満	ボス退場	内野一人
    
    comHelp,//ヘルプ//対象が瀕死なら,対象がいれば,しない,外野攻撃なら
    comHelpTag,//ピンチガード対象 //上位オーダー,誰でも,ボス,上位ラインナップ
    comGuard,//ガード//対象が瀕死なら,対象がいれば,しない,外野攻撃なら
    comGuardTag,//ガード対象//上位オーダー,誰でも,ボス,上位ラインナップ
    
    comChancePa,//チャンスパス判断//劣勢なら,する,しない,優勢なら
    comDMNum,//DM 人数//３メン,ALL,1人,２メン
    
    comQDMCallCHxxx,//QDM コールチャンス//劣勢ならする,する,しない,劣勢でないなら
    comQDMCallNC,//QDM コールノーチャンス//劣勢ならする,する,しない,劣勢でないなら
    
    comDMCallCH,//DM コールチャンス//劣勢ならする,する,しない,劣勢でないなら
    comDMCallNC,//DM コールノーチャンス//劣勢ならする,する,しない,劣勢でないなら
    comDMSetter,//OFDM セッター指定//一番前方の内野,内野最下位オーダー,ボス,外野（自分が内野ならば一番近い外野）
    comDMSetBM,//OFDM セッター＆ボールマン参加//セッタースタート＆ＢＭ参加ＯＫ,セッター関係なし＆ＢＭ参加ＯＫ,セッター関係なし＆ＢＭ参加ＮＧ,セッタースタート＆ＢＭ参加ＮＧ
    comDMWait,//DM 通常／態勢ウエイト（チャンス時スルー）//１５／６０、０／態勢Ｗナシ、６０／揃うまで、３０／１２０
    
    comDMTrigger,//DM ダッシュ開始トリガー//０フレウエイト、セッターパスと同時に、６０フレウェイトしてから開始、３０フレウェイトしてから開始
    comDMDelay,//DM ディレイ//短い,ほぼ同時,長い,普通
    comDMType,//ダッシュマンスタイル//オープンダッシュ,サイドダッシュ,ノーマルダッシュ,クロスダッシュ
    
    comDMPaNum,//DM パス回数//一人一回,出来るかぎり,しない,１回
    comDMPaTag,//DM パス先//一つオーダー上位,近い人,Ｚ軸遠い人,後方の人
    comDMPaTime,//DM パスタイミング//パス先が前衛ライン超えたら	パスインターバル後	自分のシュートタイミング	パス先がダッシュ開始したら
    comDMPaItv,//DM パスインターバル//短い,最短,長い,普通
    
    comChMake,//チャンスメイク//劣勢なら,する,しない,優勢なら
    comChMakePtn,//チャンスメイクスタイル//側面パス,最上位狙い,反時計,対面パス
    comChMakeLoop,//チャンスメイクループMAX//少ない,多い,最少,普通
    
    comEND,
  };
  
  
  class TdtComData
  {
  protected:
    u8 dt_[comEND][COMPTN];//レベル数
  public:
    explicit TdtComData(void);
    virtual ~TdtComData(void);
    
    //読み込み
    void LoadDt(const char *CSVDir, const char *CSVFilename);
    //取得
    s32 GetDt(enCom dtNo, s32 ptn);//参照CDtの取得
  };
}
