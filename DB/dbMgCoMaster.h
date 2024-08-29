#pragma once
#include "dbMgCoGen.h"

#include "dbMdm_Help.h"
#include "dbMdm_Kiyaku.h"
#include "dbMdm_Client.h"
#include "dbMdm_Timer.h"
#include "dbMdm_Connect.h"
#include "dbMdm_Team.h"
#include "dbMdm_Slot.h"
#include "dbMdm_Supply.h"
#include "dbMdm_ItemPack.h"
#include "dbMdm_ItemPackSet.h"
#include "dbMdm_Rarity.h"
#include "dbMdm_SpSyouhin.h"
#include "dbMdm_BBABuy.h"
#include "dbMdm_Maptip.h"
#include "dbMdm_Drop.h"
#include "dbMdm_Tokkou.h"
#include "dbMdm_TokkouItem.h"
#include "dbMdm_TokkouMag.h"
#include "dbMdm_TokkouItemMag.h"
#include "dbMdm_TokkouSozai.h"
#include "dbMdm_Logbo.h"
#include "dbMdm_LogboSozai.h"
#include "dbMdm_VSBonus.h"
#include "dbMdm_VSDrop.h"
#include "dbMdm_Campaign.h"
#include "dbMdm_Gacha.h"
#include "dbMdm_GachaGrp.h"
#include "dbMdm_GachaCnt.h"
#include "dbMdm_NaBox.h"
#include "dbMdm_NaBoxCnt.h"
#include "dbMdm_Huku.h"
#include "dbMdm_HukuCnt.h"
#include "dbMdm_Event.h"
#include "dbMdm_Tournament.h"
#include "dbMdm_TimeAttack.h"
#include "dbMdm_Ranking.h"
#include "dbMdm_RkReward.h"
#include "dbMdm_Giftbox.h"
#include "dbMdm_Tips.h"
#include "dbMdm_LvUp.h"

namespace db
{
  

		//0	テクノチャージ
		//1	モバコインチャージ
		//2	店
		//3	しゅーとわく
		//4	１００トッドそざいシュート
		//5	チュートリアル
		//6	２００トッドそざいシュート
		//7	１００スターそざいシュート
		//8	３００スターそざいシュート
		//9	１０００スターそざいシュート
		//10	３０００スターそざいシュート
		//11	Ａ負かたち
		//12	Ａ負こうか
		//13	おばばマルひ
		//14	コーチ

  enum enShopType
  {
    enShopType_Techno = 0,
    enShopType_Mobaco = 1,
    enShopType_Supply = 2,
    enShopType_ShotInv = 3,
    enShopType_BuySozai = 4,
    enShopType_BuySozaiVip = 5,
	
	enShopType_Coach = 14, 
    enShopType_Nazo,
  };

  class TMgCoMaster : public TMgCoGen
  {
  protected:
    void DoInitSendPack();
  public:

    TMdm_Help mdm_Help_;//マスターヘルプ
    TMdm_Kiyaku mdm_Kiyaku_;//マスター規約
    TMdm_Client mdm_Client_;//クライアント状態
    TMdm_Timer mdm_Timer_;//マスタータイム管理
    TMdm_Connect mdm_Connect_;//通信設定
    TMdm_Team mdm_Team_;//マスターチーム
    TMdm_Slot mdm_Slot_;//マスタースロットオープン
    TMdm_Supply mdm_Supply_;//マスター消耗品
    TMdm_ItemPack mdm_ItemPack_;//アイテムパック
    TMdm_ItemPackSet mdm_ItemPackSet_;//アイテムパックセット
    TMdm_Rarity mdm_Rarity_;//レアリティ設定
    TMdm_SpSyouhin mdm_SpSyouhin_;//消耗品ショップ＆なぞのみせ＆しゅーとばばあ
    TMdm_BBABuy mdm_BBABuy_;//しゅーとばばあシュート購入メニュー
    TMdm_Maptip mdm_Maptip_;//マスターマップチップ
    TMdm_Drop mdm_Drop_;//マスタードロップ
    TMdm_Tokkou mdm_Tokkou_;//マスター特効チーム
    TMdm_TokkouItem mdm_TokkouItem_;//マスター特効アイテム
    TMdm_TokkouMag mdm_TokkouMag_;//マスター特効チーム倍率
    TMdm_TokkouItemMag mdm_TokkouItemMag_;//マスター特効アイテム倍率
    TMdm_TokkouSozai mdm_TokkouSozai_;//マスター特効素材
    TMdm_Logbo mdm_Logbo_;//ログインボーナス
    TMdm_LogboSozai mdm_LogboSozai_;//ログインボーナス素材
    TMdm_VSBonus mdm_VSBonus_;//対戦ボーナス
    TMdm_VSDrop mdm_VSDrop_;//新通信対戦ボーナス
    TMdm_Campaign mdm_Campaign_;//マスターキャンペーン
    TMdm_Gacha mdm_Gacha_;//マスターガチャ
    TMdm_GachaGrp mdm_GachaGrp_;//マスターガチャ中身グループ
    TMdm_GachaCnt mdm_GachaCnt_;//マスターガチャ中身詳細
    TMdm_NaBox mdm_NaBox_;//マスターなつかしＢＯＸ
    TMdm_NaBoxCnt mdm_NaBoxCnt_;//マスターなつかしＢＯＸ中身詳細
    TMdm_Huku mdm_Huku_;//マスターふくびき
    TMdm_HukuCnt mdm_HukuCnt_;//マスターふくびき中身詳細
    TMdm_Event mdm_Event_;//マスターイベント
    TMdm_Tournament mdm_Tournament_;//マスター勝ち抜き
    TMdm_TimeAttack mdm_TimeAttack_;//マスタータイムアタック
    TMdm_Ranking mdm_Ranking_;//マスターランキング
    TMdm_RkReward mdm_RkReward_;//マスターランキング報酬
    TMdm_Giftbox mdm_Giftbox_;//マスター贈り物ボックス
    TMdm_Tips mdm_Tips_;//Tips表示
	TMdm_LvUp mdm_LvUp_;//キャラLvUP経験値テーブル

    explicit TMgCoMaster(TMgEngineObj* pmgEO);
    virtual ~TMgCoMaster(void);

    void LoadDt();

    //ガチャ内のグループ数（懐かしボックスの枠数）
    s32 GetGachaGrpNum(s64 mid_gacha);
    //商品のアイコン
    s32 GetSpSyouhinIconNo(s64 mid_syouhin);
    //シュートの売却価格
    s32 GetShotSellPrice(INTVECTOR sozairarityvec);

    //位置からＰＫを求める
    s64 GetMapTipPK(s32 mapno, s32 posx, s32 posy);
    s64 GetMapTipPosNo(s32 mapno, s32 posx, s32 posy);

    //チーム内のキャラのＩＤ
    s64 GetCharID(s64 mid_team, s32 memNo);

    //現在開催中のランキング
    LONGVECTOR GetNowRanking(LONGVECTOR nowevent);
    //現在開催中のランキング
    s64 GetEventRanking(LONGVECTOR nowevent, s32 mid_event);

	// 任意のイベントの特効ＢＧＭ取得
	LONGVECTOR2 GetTokkouBgm(s64 mid_event);
	LONGVECTOR GetTokkouBgmByTitle(s64 mid_event, s64 titleid);	// タイトル縛り
	// 任意のイベントの所持している特効ＢＧＭ取得
	LONGVECTOR2 GetHaveTokkouBgm(s64 mid_event);
	LONGVECTOR GetHaveTokkouBgmByTitle(s64 mid_event, s64 titleid);	// タイトル縛り

	// 任意のイベントの対象ＢＧＭ数取得
	s64 GetTokkouBgmNum(s64 mid_event);
	s64 GetTokkouBgmNumByTitle(s64 mid_event, s64 titleid);	// タイトル縛り
	// 任意のイベントの対象ＢＧＭ所持数取得
	s64 GetHaveTokkouBgmNum(s64 mid_event);
	s64 GetHaveTokkouBgmNumByTitle(s64 mid_event, s64 titleid);	// タイトル縛り

	// 任意のイベントの特効背景取得
	LONGVECTOR2 GetTokkouHaikei(s64 mid_event);
	LONGVECTOR GetTokkouHaikeiByTitle(s64 mid_event, s64 titleid); // タイトル縛り
	// 任意のイベントの所持している特効背景取得
	LONGVECTOR2 GetHaveTokkouHaikei(s64 mid_event);
	LONGVECTOR GetHaveTokkouHaikeiByTitle(s64 mid_event, s64 titleid); // タイトル縛り

	// 任意のイベントの対象背景数取得
	s64 GetTokkouHaikeiNum(s64 mid_event);
	s64 GetTokkouHaikeiNumByTitle(s64 mid_event, s64 titleid);	// タイトル縛り
	// 任意のイベントの対象背景所持数取得
	s64 GetHaveTokkouHaikeiNum(s64 mid_event);
	s64 GetHaveTokkouHaikeiNumByTitle(s64 mid_event, s64 titleid);	// タイトル縛り

    void GetBasePowSpin(TSozaiData sozaidt, s32* basepow, s32* basespin, BOOL rank_f);
    void GetRevPowSpin(TSozaiData sozaidt, s32* revpow, s32* revspin);
    s32 GetLightFlag(s32 mid_sozai);
    BOOL IsStarGacha(s64 mid_gacha);
    s32 GetTokkouMag(s32 id_team, s32 mid_event);
    s32 GetTokkouMag(s32 mid_team, s32 tokkouNo, s32 overlap);
    s32 GetTokkouSozaiMag(s32 mid_event, const SOZAIDTVECTOR& sozaivec);
    BOOL IsTimer(s32 mid_timer);
  };
}
