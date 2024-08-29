#pragma once
#include "dbInclude.h"

//グローバル扱いな変数
namespace db
{
  enum enRequestState
  {
    rqNone,
    rqConnecting,
    rqOK,
    rqNG,
  };


  enum enOSType
  {
	  enOSType_None = 0,	// ＯＳ指定なし
	  enOSType_IOS,			// IOS
	  enOSType_Android,		// Android
  };

  //セッションIDをチェックするか
  enum enSessionCheckType
  {
    enSessionCheckType_Check,//チェックする
    enSessionCheckType_NoCheck,//チェックしない（現在不使用）
    enSessionCheckType_Reset,//チェックしない＆再生成（タイトル画面での通信）
  };

  //アイテムタイプ
  enum enItemType
  {
    enItemType_Team,
    enItemType_Sozai,
    enItemType_Shot,
    enItemType_BGM,
    enItemType_Haikei,
    enItemType_Supply,
    enItemType_Todd,
    enItemType_Star,
    enItemType_Pack,
    enItemType_Huku,
  };

  // スコアタイプ
  enum enScoreType
  {
	  enScoreType_Win = 1,	// 勝利数
	  enScoreType_Wins,		// 連勝数
	  enScoreType_MaxDamage,//最大ダメージ
	  enScoreType_TotalDamage,//累計ダメージ
	  enScoreType_Kill,		// 倒した数
	  enScoreType_Item,		// アイテム所持数
	  enScoreType_VSHum,	// [2015年5月13日 15:02] 石田 裕明: 7が対人戦用のよていだったので
	  enScoreType_Time,		// タイムアタック
  };

  enum enTutoBitType
  {
    enTutoBitType_Kiyaku,		// 初回起動規約表示（送信：ビット番号）
    enTutoBitType_Opening,		// オープニング(実装は未定)（送信：ビット番号）
    enTutoBitType_TmSelect,	// チーム選択（送信：チームＭＩＤ）
    enTutoBitType_GameSousa,		// 操作方法(疑似試合)（送信：チュート試合完了）
    enTutoBitType_Home,			// ホーム画面の説明（送信：ビット番号）
    enTutoBitType_Kantoku,		// 監督の名前、顔ヅラエディット説明（送信：アバターづら替え）
    enTutoBitType_Gatya,		// ガチャの説明（送信：ガチャ引く）
    enTutoBitType_NaBox,		// なつかしＢＯＸ（送信：おとながい）
    enTutoBitType_Gousei,		// シュート合成の説明（送信：素材ＭＩＤ）

    //サブ
    enTutoBitType_SubScenario,		// （概要とＨＰ・スタミナ）（送信：ビット番号）
    enTutoBitType_SubEventOpen,		// イベント解放（送信：ビット番号）
    enTutoBitType_SubGousei2,		// しゅーとミックス（送信：ビット番号）
    enTutoBitType_SubTeam,		// チーム初回（送信：ビット番号）
    enTutoBitType_SubSoubi,		// しゅーとそうび（送信：ビット番号）
    enTutoBitTypeEND,
  };

  enum enOptBitType
  {
    enOptBitType_Smart,		//スマート操作
  };

  enum enInfoType
  {
    //取得種類
    GT_INIT = 0,//初期配布
    GT_SHOP = 10,//ショップ info課金スター info2無料スター
    GT_QUSE = 11,//即使用 info課金スター info2無料スター
    GT_BBA = 12,//しゅーとばばあ infoなし
    GT_QUEST = 20,//クエスト info : マップチップＩＤ
    GT_QUESTCOMP = 21,//コンプ info : マップチップＩＤ
    GT_LOGBO = 30,//ログインボーナス info : ログイン回数
    GT_LOGBOSP = 31,//スペシャルログインボーナス info : ログイン回数
    GT_GIFT = 32,//運営補填 info : 運営補填マスターＩＤ
    GT_RKREWARD = 33,//ランキング報酬 info : ランキングＩＤ info2 : ランキング報酬ＩＤ
    GT_HUKUKEN = 34,//ランキング報酬 info : ランキングＩＤ info2 : ランキング報酬ＩＤ
    GT_OMAKE = 35,//高額スターおまけ info : なし
    GT_INVITED = 40,//招待入力 info : 招待者ＩＤ
    GT_INVITER = 41,//招待特典 info : 招待入力者ＩＤ
    GT_CAMPAIN = 50,//キャンペーン info : キャンペーンＩＤ
    GT_MBGJZN = 51,//キャンペーン info : キャンペーンＩＤ
    GT_GACHA = 60,//ガチャ info : ガチャＩＤ
    GT_NABOX = 61,//なつかしＢＯＸ info : なつかしＢＯＸＩＤ
    GT_HUKU = 62,//ふくびき info : ふくびきＩＤ
    GT_FROMGIFTBOX = 80,//贈り物ボックスからの移動
    GT_VSBONUS = 90,//対戦ボーナス info : 相手監督ＩＤ
    GT_OPENPACK = 100,//アイテムパックの中身
    GT_TUTO = 110,//チュートリアル報酬
    GT_DEBUG = 1000,//デバッグ
  };
  enum enResultType 
  {
    rtREADY = 0, 
    rtPLAY = 1,
    rtWIN = 2,
    rtLOSE = 3,
    rtTIMEUPWIN = 4,
    rtTIMEUPLOSE = 5,
    rtTIMEUPDRAW = 6,
    rtRETIRE = 10,
  };

  enum enTokkouEffectType
  {
    tftNone = 0,
    tftDrop = 1,
    tftTime = 2,
  };

  enum enPriceType
  {
    enPriceType_Techno,	
    enPriceType_Star,
    enPriceType_Todd,
  };

  struct TSozaiData
  {
    s32 mid_sozai_;
    s32 rarity_;
    s32 indvpow_;
    s32 indvspin_;
    void Init()
    {
      Zeromem(this);
    }
    void SetMidSozai(s32 mid_sozai_rarity)
    {
      Zeromem(this);
      mid_sozai_ = (mid_sozai_rarity / 10);
      rarity_ = (mid_sozai_rarity % 10);
    }
	s64 GetSozaiR(void)
	{
		return (mid_sozai_ * 10 + rarity_);
	}
  };
  typedef std::vector<TSozaiData> SOZAIDTVECTOR;

  //シュート
  const s64 MBGIDBASE = 0xFFFFFFFF;
  const s64 MBGIDKETA = 7;
  const s64 SHOTNAMEMAX = 10;
  const s64 SHSETNAMEMAX = 6;
  const s32 RARITYNUM = 7;//レアリティ
  const s32 HSELEMMAX = 8;//最大原料数
  const s32 HSELEMMAX_ADD = (HSELEMMAX - 1);//最大原料数
  const s32 HSATMMAX = 8;//最大素材数
  const s32 STEPNUM = 7;//最大素材数//試合中およびシュート装備などでつかう
  const s32 SOZAIMID = 10;//素材のmidは1の位がレアリティ
  const s32 SLOTALL = 49;//チームやシュートセットのスロット

  const s64 SHSETNUM = 4;//おまかせ＋123（行数を数えると、通信した敵のシュートセットの数まで拾ってしまう）

  //装備品ショット
  const s32 STEPTYPENUM = 6;//素早さＡ～Ｆの６段階
  //ステップが開くレベル（数字は開くレベル）
  const s32 SLOTLV[STEPNUM * STEPTYPENUM] =
  {
    //歩数０１２３４５６
    6, 5, 3, 4, 2, 1, 0,//鈍足
    6, 5, 3, 4, 2, 0, 1,
    6, 5, 2, 4, 0, 1, 3,
    5, 2, 0, 4, 1, 3, 6,
    2, 0, 1, 4, 3, 5, 6,
    0, 1, 2, 4, 3, 5, 6,//俊足（１歩目の開くレベルが低い）
  };

  const s32 DEFSTEP = 3;
  const s32 VJOPENSTEP = 3;
  const s32 VJICONSTEP = 7;
  const s32 SLOTNUM = 7;
  //レベルごとのステップ（数字は歩数）
  const s32 SHOTSTEP[SLOTNUM * STEPTYPENUM] =
  {
    //レベル０１２３４５６
    6, 5, 4, 2, VJICONSTEP, 1, 0,
    5, 6, 4, 2, VJICONSTEP, 1, 0,
    4, 5, 2, 6, VJICONSTEP, 1, 0,
    2, 4, 1, 5, VJICONSTEP, 0, 6,
    1, 2, 0, 4, VJICONSTEP, 5, 6,
    0, 1, 2, 4, VJICONSTEP, 5, 6,
  };

}
