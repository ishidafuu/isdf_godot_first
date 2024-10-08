﻿#pragma once
#include "dbInclude.h"

namespace db
{
  enum enDtComAct
  {
    cmaAction,//アクション//クイックシュート,//パワーシュート,オーダーパス,シングルパス
    cmaShChange,//シュート変更//ウエイト,しない,シュート変更パス,変化球外し
    cmaOdPaTag,//オーダーパス先//上位オーダー,ぱわー＋きあい,ボス,上位ラインナップ
    cmaSgPaTag,//シングルパス先//内野,側面,対面,近接
    cmaInfPaTag,//内野パスの狙い//最後衛,攻撃力高い人,近い人,最前衛
    cmaNrPaTag,//近接パスの狙い//上位オーダーに近い,攻撃力高に近い,守備下手に近い,ＨＰ少に近い
    cmaPaWait,//シングルパスウエイト//短い,最短,長い,普通
    cmaPaType,//パスタイプ//ジャンプトスパス,トスパス,クイックパス,ジャンプクイックパス
    cmaPaTypeInf,//内野間パスタイプ//ジャンプトスパス,トスパス,クイックパス,ジャンプクイックパス
    cmaQShType,//クイックシュートタイプ//ＤＪシュート,Ｄシュート,立ちシュート,Ｊシュート
    cmaPwShType,//シュートタイプ//ＤＪシュート,Ｄシュート,一歩Ｄシュート,一歩ＤＪシュート
    cmaCrvType,//変化球タイプ//カーブ,タゲ外し,曲げない,トレース
    cmaShWait,//パワーウェイト//短い,最短,長い,普通
    cmaDShStep,//Ｄシュート歩数//６（ナイス）,４（必殺）,５歩,選択
    cmaJShTime,//ＤＪシュートタイミング//早め（ＤＪナイス）,頂点（必殺）,通常,選択
    cmaDsPos,//ダッシュ開始位置//必殺位置まで下がる,ダッシュ,エンドラインまで下がる,ナイス位置までさがる
    cmaDsStyle,//ダッシュスタイル//ブロード,クイック,ストレート,クロス

    cmaBallTake,//ボール奪い //微妙なら行く	行く	行かない	とれそうなら
    cmaCatchJdg,//キャッチ対応確率//ブロック	する	しない	強い球ならばしない
    cmaCounter,//カウンターキャッチ//危険球ならばしない	する	しない	強い球ならばしない
    cmaDgType,//ドッジタイプ//サイド避け,ジャンプ避け,膝付き避け,シュート対応避け

    cmaPassCut,//クイックならする	する	しない	対面＆クイックならする
    cmaChanceSh,//チャンスシュート//キャッチ不能ならする	する	しない	キャッチ不能ならしない//★
    cmaNiceCaSh,//ナイスキャッチシュート//前よりなら	する	しない	内野シュートなら//★
    cmaEND,
  };


  class TDtComAct
  {
  protected:
    u8 dt_ys_[cmaEND][COMPTN];//レベル数
    u8 dt_rs_[cmaEND][COMPTN];//レベル数
  public:
    explicit TDtComAct(void);
    virtual ~TDtComAct(void);

    //読み込み
    void LoadDt(const char *CSVDir, const char *CSVFilename_ys, const char *CSVFilename_rs);
    //取得
    s32 GetDt(enDtComAct dtNo, s32 ptn, BOOL ressei_f);//参照CDtの取得
  };
}
