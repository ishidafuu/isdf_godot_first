#pragma once
#include "dbMgCoGen.h"

#include "dbMdre_Ranker.h"
#include "dbMdre_Enemy.h"
#include "dbMdre_Refresh.h"
#include "dbMdKantoku.h"
#include "dbMdKantokuSub.h"
#include "dbMdShSet.h"
#include "dbMdTeam.h"
#include "dbMdTeamCharLv.h"
#include "dbMdShot.h"
#include "dbMdDock.h"
#include "dbMdSupply.h"
#include "dbMdBGM.h"
#include "dbMdHaikei.h"
#include "dbMdGiftbox.h"
#include "dbMdMaptip.h"
#include "dbMdInvite.h"
#include "dbMdNaBox.h"
#include "dbMdMap.h"
#include "dbMdVSDrop.h"
#include "dbMdlog_BStar.h"
#include "dbMdlog_Event.h"
#include "dbMdlog_ShiaiMap.h"
#include "dbMdlog_ShiaiHum.h"
#include "dbMdlog_ShiaiTour.h"
#include "dbMdlog_ShiaiTmat.h"
#include "dbMdlog_Maruka.h"
#include "dbMdlog_ShotDel.h"
#include "dbMdlog_ItemGet.h"
#include "dbMdlog_SupplyUse.h"
#include "dbMdlog_CpCode.h"
//#include "dbMdlog_CharLvUp.h"
#include "dbMdw_Version.h"
#include "dbMdw_Event.h"
#include "dbMdw_HukuGrp.h"
#include "dbMdw_HukuCnt.h"
#include "dbMdw_log_TousenGacha.h"
#include "dbMdw_log_TousenHuku.h"



namespace db
{

  //消耗品マスターＩＤ開始位置
  const s32 SPL_CREDIT = 0;//お金、スター
  const s32 SPL_FKT = 100;//復活
  const s32 SPL_GGZ = 200;//監督回復
  const s32 SPL_MGK = 300;//チーム回復
  const s32 SPL_GOUSEI = 400;//合成短縮
  const s32 SPL_BARASTAR = 500;//合成ばらし
  const s32 SPL_BARATODD = 501;//合成ばらしトッド
  const s32 SPL_SHOTINV = 600;//シュート枠拡張
  const s32 SPL_DOCKINV = 700;//ドック拡張
  const s32 SPL_FRIENDINV = 800;//フレンド拡張
  const s32 SPL_TEAMLVUP = 900;//チームレベルアップ
  const s32 SPL_GACHA = 10000;//ガチャ券

  struct TSortTeam
  {
    s64 id_team_;
    s32 mid_team_;
    s32 rarity_;
    s64 wait_;
    s32 id_shset_;
    s32 overlap_;
    void Init()
    {
      Zeromem(this);
    }
  };

  typedef std::vector<TSortTeam> SORTTEAMVECTOR;

  struct TSortShot
  {
    BOOL lock_f_;
    s32 basepow_;
    s32 basespin_;
    s32 genre_;
    s32 indvpow_;
    s32 indvspin_;
    s32 mid_sozai_base_;
    s32 qtpow_;
    s32 qtspin_;
    s32 rarity_;
    s32 revpows_;
    s32 revspins_;
    s32 sozaitype_;
    s32 totalpow_;
    s32 totalspin_;
    s32 useslot_;
    s64 id_shot_;
    s64 id_sozai_add_[HSELEMMAX_ADD];
    s64 updated_time_;
    void Init()
    {
      Zeromem(this);
    }
  };

  typedef std::map<s32, TSortShot> SORTSHOTMAP;
  typedef std::vector<TSortShot> SORTSHOTVECTOR;
  typedef std::map<s64, BOOL>  SHLOCKMAP;
  

  enum enTeamFilType
  {
    enTeamFilType_All,
    enTeamFilType_Tokkou,
    enTeamFilType_RarityD,
    enTeamFilType_RarityC,
    enTeamFilType_RarityB,
    enTeamFilType_RarityA,
    enTeamFilType_RarityS,
    enTeamFilType_RaritySS,
//    enTeamFilType_RaritySSS,
    //enTeamFilType_ShSetA,
    //enTeamFilType_ShSetB,
    //enTeamFilType_ShSetC,
    //enTeamFilType_ShSetD,
    //enTeamFilType_ShSetE,
    enTeamFilTypeEND,
  };

  enum enTeamSrtType
  {
    //enTeamSrtType_MID,
    enTeamSrtType_Rarity,
    enTeamSrtType_Wait,
    enTeamSrtType_Lv,
    enTeamSrtType_New,
    enTeamSrtTypeEND,
  };

  //シュートフィルター２（場面ごとに最初にかかる）
  enum enShotFilTypeS
  {
    enShotFilTypeS_All,//全部
    enShotFilTypeS_Baseable,//ベース可能
    enShotFilTypeS_Mixed,//合成（バラ可能）
    enShotFilTypeS_SellD,//売却可能（アンロック）
    enShotFilTypeS_SellC,//売却可能（アンロック）
    enShotFilTypeS_SellB,//売却可能（アンロック）
    enShotFilTypeS_SellA,//売却可能（アンロック）
    enShotFilTypeS_SellALL,//売却可能（アンロック）
    enShotFilTypeS_Under1,//素材1つ以下
    enShotFilTypeS_Under2,//素材2つ以下
    enShotFilTypeS_Under3,//素材3つ以下
    enShotFilTypeS_Under4,//素材4つ以下
    enShotFilTypeS_Under5,//素材5つ以下
    enShotFilTypeS_Under6,//素材6つ以下
    enShotFilTypeS_Under7,//素材7つ以下
  };

  //シュートフィルター
  enum enShotFilType
  {
    enShotFilType_All,
    enShotFilType_RarityD,
    enShotFilType_RarityC,
    enShotFilType_RarityB,
    enShotFilType_RarityA,
    enShotFilType_RarityS,
    enShotFilType_RaritySS,
    enShotFilType_RaritySSS,
    enShotFilType_Locked,
    enShotFilType_Solo,
    enShotFilType_Mixed,
    enShotFilType_FullMixed,
    enShotFilType_Genre0,
    enShotFilType_Genre1,
    enShotFilType_Genre2,
    enShotFilType_Genre3,
    enShotFilType_Genre4,
    enShotFilType_Genre5,
    enShotFilType_Genre6,
    enShotFilType_Genre7,
    enShotFilType_Genre8,
    enShotFilType_Genre9,
    enShotFilType_Genre10,
    enShotFilTypeEND,
  };

  enum enShotSrtType
  {
    //enShotSrtType_MID,
    enShotSrtType_Rarity,
    enShotSrtType_Pow,
    enShotSrtType_Spin,
    enShotSrtType_New,
    enShotSrtTypeEND,
  };

  struct TMgCoUserSt
  {
    BOOL otherkantoku_f_;
    s32 shlocknum_;
    void Init()
    {
      Zeromem(this);
    }
  };


  class TMgCoUser : public TMgCoGen
  {
  protected:
    void DoInitSendPack();

    LONGVECTOR GenGetTeamList(SORTTEAMVECTOR* lnovec);
    SORTTEAMVECTOR GenGetTeamList_FilRarity(SORTTEAMVECTOR* lnovec, s32 rarity);
    SORTTEAMVECTOR GenGetTeamList_FilShSet(SORTTEAMVECTOR* lnovec, s32 shsetNo);
    SORTTEAMVECTOR GenGetTeamList_FilTokkou(SORTTEAMVECTOR* lnovec, s32 mid_event);
    //LONGVECTOR GenGetTeamList_SrtMID(SORTTEAMVECTOR* lnovec, BOOL rev_f);
    LONGVECTOR GenGetTeamList_SrtWait(SORTTEAMVECTOR* lnovec, BOOL rev_f);
    LONGVECTOR GenGetTeamList_SrtRarity(SORTTEAMVECTOR* lnovec, BOOL rev_f);
    LONGVECTOR GenGetTeamList_SrtLv(SORTTEAMVECTOR* lnovec, BOOL rev_f);
    LONGVECTOR GenGetTeamList_SrtNew(SORTTEAMVECTOR* lnovec, BOOL rev_f);
    s32 GenGetNumTeamList_Tokkou(SORTTEAMVECTOR* lnovec, s32 mid_event);
    s32 GenGetNumTeamList_Rarity(SORTTEAMVECTOR* lnovec, s32 rarity);
    s32 GenGetNumTeamList_ShSet(SORTTEAMVECTOR* lnovec, s32 shsetNo);


    SORTSHOTVECTOR GenGetShotList_FilRarity(SORTSHOTVECTOR* lnovec, s32 rarity);
    SORTSHOTVECTOR GenGetShotList_FilGenre(SORTSHOTVECTOR* lnovec, s32 genre);
    SORTSHOTVECTOR GenGetShotList_FilMixed(SORTSHOTVECTOR* lnovec, s32 mixlv);
    SORTSHOTVECTOR GenGetShotList_FilLocked(SORTSHOTVECTOR* lnovec);

    LONGVECTOR GenGetShotList(SORTSHOTVECTOR* lnovec);
    LONGVECTOR GenGetShotList_SrtMID(SORTSHOTVECTOR* lnovec, BOOL rev_f);
    LONGVECTOR GenGetShotList_SrtRarity(SORTSHOTVECTOR* lnovec, BOOL rev_f);
    LONGVECTOR GenGetShotList_SrtTotalPow(SORTSHOTVECTOR* lnovec, BOOL rev_f);
    LONGVECTOR GenGetShotList_SrtBasePow(SORTSHOTVECTOR* lnovec, BOOL rev_f);
    LONGVECTOR GenGetShotList_SrtRevPow(SORTSHOTVECTOR* lnovec, BOOL rev_f);
    LONGVECTOR GenGetShotList_SrtTotalSpin(SORTSHOTVECTOR* lnovec, BOOL rev_f);
    LONGVECTOR GenGetShotList_SrtBaseSpin(SORTSHOTVECTOR* lnovec, BOOL rev_f);
    LONGVECTOR GenGetShotList_SrtRevSpin(SORTSHOTVECTOR* lnovec, BOOL rev_f);
    LONGVECTOR GenGetShotList_SrtNew(SORTSHOTVECTOR* lnovec, BOOL rev_f);

    s32 GenGetNumShotList_Rarity(SORTSHOTVECTOR* lnovec, s32 rarity);
    s32 GenGetNumShotList_Mixed(SORTSHOTVECTOR* lnovec, s32 mixlv);
    s32 GenGetNumShotList_Genre(SORTSHOTVECTOR* lnovec, s32 genre);
    s32 GenGetNumShotList_Locked(SORTSHOTVECTOR* lnovec);
  public:
	  TMdre_Ranker rankerData_;//返信ランキングを元に生成するランキング

    TMdre_Ranker mdre_Ranker_;//返信ランキング
    TMdre_Enemy mdre_Enemy_;//返信ＶＳ敵監督
    TMdre_Refresh mdre_Refresh_;//ホーム更新系
    TMdKantoku mdKantoku_;//監督（通信の度に参照、チェックされるようなもの）
    TMdKantokuSub mdKantokuSub_;//監督データ（主に更新のみされてクライアントに表示だけされてればよいもの）
    TMdShSet mdShSet_;//必殺セット
    TMdTeam mdTeam_;//チーム
	TMdTeamCharLv mdTeamCharLv_;//チーム選手レベル
    TMdShot mdShot_;//シュート（インベントリ）
    TMdDock mdDock_;//合成ドック
    TMdSupply mdSupply_;//消耗品
    TMdBGM mdBGM_;//タイトル（取得情報）
    TMdHaikei mdHaikei_;//背景（取得情報）
    TMdGiftbox mdGiftbox_;//贈り物ＢＯＸ（バッチ廃棄必要）
    TMdMaptip mdMaptip_;//シナリオマップチップ
    TMdInvite mdInvite_;//招待
    TMdNaBox mdNaBox_;//なつかしＢＯＸ
    TMdMap mdMap_;//マップ
    TMdVSDrop mdVSDrop_;//対戦ドロップ
    TMdlog_BStar mdlog_BStar_;//購入スターログ兼管理
    TMdlog_Event mdlog_Event_;//イベントログ兼管理
    TMdlog_ShiaiMap mdlog_ShiaiMap_;//試合ログ兼管理
    TMdlog_ShiaiHum mdlog_ShiaiHum_;//試合ログ対人戦兼管理
    TMdlog_ShiaiTour mdlog_ShiaiTour_;//試合ログ疑似対人兼管理
    TMdlog_ShiaiTmat mdlog_ShiaiTmat_;//試合ログタイムアタック兼管理
    TMdlog_Maruka mdlog_Maruka_;//まるかログ兼管理
    TMdlog_ShotDel mdlog_ShotDel_;//シュート削除ログ
    TMdlog_ItemGet mdlog_ItemGet_;//アイテム取得ログ
    TMdlog_SupplyUse mdlog_SupplyUse_;//消耗品使用ログ
    TMdlog_CpCode mdlog_CpCode_;//キャンペーンコード取得ログ兼管理
	//TMdlog_CharLvUp mdlog_CharLvUp_;//キャラレベルアップログ
    TMdw_Version mdw_Version_;//マスターバージョン管理#キャッシュでよいのでは
    TMdw_Event mdw_Event_;//イベント回数管理
    TMdw_HukuGrp mdw_HukuGrp_;//福引きグループ管理
    TMdw_HukuCnt mdw_HukuCnt_;//福引き中身カウンタ
    TMdw_log_TousenGacha mdw_log_TousenGacha_;//ガチャ当選ログ
    TMdw_log_TousenHuku mdw_log_TousenHuku_;//ふくびき当選ログ

    TMgCoUserSt stCoUser_;
    std::vector<TModelGen*> mdvec_log_;
    SORTSHOTMAP sortshotmap_;
    SHLOCKMAP shlockmap_;

    //TModelGen
    explicit TMgCoUser(TMgEngineObj* pmgEO);
    virtual ~TMgCoUser(void);

    //ログを全て削除
    void CleanLog(void);

    //全て削除(ログアウト時)
    void CleanAll(void);


    void SetOtherKantoku(void);
    //他人監督を全て削除
    void CleanOtherKantoku(void);

    //監督モデルに自分のPK登録
    void SetMyPK();

	//鯖から取得したランカーデータからランキングの形にセット
	void SetupRankerDataByAnimBefore(void);
	void SetupRankerDataByAnimAfter(void);

    //持ってきたランカーデータからアイコン作成
    void MakeChestRanker();  // 14人バージョン
	void MakeChestRankerByData(void); // RANKERNUMバージョン
    //持ってきた他監督データからアイコン作成
    void MakeChestEnemy();

    //モバゲーＩＤの３６進数変換
    std::string CnvMBGID(s64 mbgid);

    BOOL IsShotEnable(s64 id_shot);
	BOOL IsShotGousei(s64 id_shot);

	// シュートステータス取得
	void GetShotTotalPowSpin(s64 mid_team, s32 posNo, s32* pPow, s32* pSpin);	// 装備しているシュートのパワー、スピン合計値取得
	s32 GetTotalSetShotNum(s64 mid_team, s32 posNo);	// 装備しているシュート数取得
	s32 GetOpenShotNum(s64 mid_team, s64 id_team, s64 posNo, s32 overlap);	// 解放されているシュート装備枠数取得

    //有効なシュートインベントリ数
    s32 GetShotNum_Inv();
    s32 GetShotNum_Enable();
    s32 GetShotNum_Souko();
    s32 GetShotNum_Emp();
    //有効なシュートインベントリ内番号からシュートのPK
    s64 GetShotid_Enable(s32 enableno, BOOL sozaionly_f);
    //合成できるベースと素材があるか
    BOOL IsIntoGousei();
    //シュートインベントリの番号
    s32 GetShotInvNo(s64 id_shot);

    //そのシュートを装備しているキャラの番号
    s32 GetShotHaveMember(s64 id_shot, s64 id_team);

    //シュートＩＤとスロットから素材MID
    TSozaiData GetSozaiData(s64 id_shot, s32 slotNo);
    //消耗品MIDから消耗品PK
    s64 GetSupply_id(s64 mid_supply);
    //消耗品の数
    s64 GetSupplyNum(s64 mid_supply);
    //ぎんぎんぜっとの数
    s64 GetSupplyNum_GGZ();
    //みなげんきーの数
    s64 GetSupplyNum_MGK();
    //ふっかつのたねの数
    s64 GetSupplyNum_FKT();
    //ガチャ券枚数
    s64 GetGachaKenNum(s64 mid_gacha);
    BOOL IsGachaKenUse(s64 mid_gacha);
	//コーチチケット枚数(引数にコーチチケットのmid_supplyを渡す)
	s64 GetCoachKenNum(s64 mid_supply);
	BOOL IsCoachKenUse(s64 mid_supply);
    //懐かしボックスID
    s64 GetNaBox_id(s64 mid_nabox);
    //懐かしボックスオープンビット
    BOOL IsNaBoxOpen(s64 id_nabox, s32 no);
    //チュートリアルのオープンビット
    BOOL IsTutoClear(enTutoBitType tutono);
    //ＢＧＭの獲得ビット
    BOOL IsBGM(s32 mid_bgm);
    //背景の獲得ビット
    BOOL IsHaikei(s32 mid_haikei);
    //贈り物ＢＯＸに入ってる数
    LONGVECTOR GetGiftBox();
    //LONGVECTOR GetSozaiKeep();

    //シュートのリスト
    SOZAIDTVECTOR GetSozaiDataAll(s64 id_shot_base);//ついてる素材
    TSozaiData GetSozaiDataLast(s64 id_shot_base);//最後の素材
    SOZAIDTVECTOR GetSozaiDataSyn(s64 defshotid);

    //シュートのリスト
    SORTSHOTVECTOR GetShotVec(enShotFilTypeS filtypeS, LONGVECTOR* ngidvec);
    LONGVECTOR GetShotList_Filter(SORTSHOTVECTOR* shotvec, enShotSrtType srttype, enShotFilType filtype, enShotFilTypeS filtypeS, BOOL rev_f, LONGVECTOR* ngidvec);
    LONGVECTOR GetShotList_Sort(SORTSHOTVECTOR* shotvec, enShotSrtType srttype, enShotFilTypeS filtypeS, BOOL rev_f);
    s32 GetNumShotList(SORTSHOTVECTOR* shotvec, enShotFilType filtype);
    TSortShot* GetSortShot(s64 id_shot);
    SORTSHOTMAP* GetSortShotMap();

    //おまかせシュートidvector
    LONGVECTOR GetShotOmakase();
    void SwitchShLock(s64 id_shot);
    //void ClearShLock();
    LONGVECTOR GetShLockVec();
    s32 GetShLockNum();


    //チームのリスト
    SORTTEAMVECTOR GetTeamVec();
    LONGVECTOR GetTeamList_Filter(SORTTEAMVECTOR* teamvec, enTeamSrtType srttype, enTeamFilType filtype, BOOL rev_f, s32 mid_event);
    LONGVECTOR GetTeamList_Filter(SORTTEAMVECTOR* teamvec, enTeamSrtType srttype, enTeamFilType filtype, BOOL rev_f);
    LONGVECTOR GetTeamList_Sort(SORTTEAMVECTOR* teamvec, enTeamSrtType srttype, BOOL rev_f);
    s32 GetNumTeamList(SORTTEAMVECTOR* teamvec, enTeamFilType filtype, s32 mid_event);

    //チームのレベル
    s64 GetTeamLv(s64 mid_team);

	// キャラレベル
	s64 GetMyTmCharaLv(s64 mid_team, s32 charaPos);
	s64 GetTmCharaLvByKantokuID(s64 id_kantoku, s64 mid_team, s32 charaPos);
	BOOL IsMyTmCharaLvMax(s64 mid_team, s32 charaPos);
	BOOL IsCharaLvMax(s64 lv, s32 rarity);

	// キャラ経験値
	s64 GetMyTmCharaExp(s64 mid_team, s32 charaPos);
	s64 GetTmCharaExpByKantokuID(s64 id_kantoku, s64 mid_team, s32 charaPos);

    //現在開催中のイベント
    LONGVECTOR GetNowEvent();

    //現在結果表示中のイベント
    LONGVECTOR GetResultEvent();

    //イベントの日時
    std::string GetEventDate(s32 mid_event);

    //シュートの売却価格
    s32 GetShotPrice(s64 id_shot);

    //イベントＷＩＤ
    s32 GetEventWID(s32 mid_event, BOOL opencheck_f);

    //イベント期限切れ
    BOOL IsOverTimeEvent(s32 wid_event);

    //シュートsetの名前
    std::string GetShSetName(s32 shsetNo, BOOL set_f);
    //シュートの名前
    std::string GetShotName(s64 id_shot);

    //シュートの空き枠
    s32 GetShotFreeSlotNum(s64 id_shot);

    //最後に使ったチームのマスターＩＤ
    s32 GetLastMIDTeam();

    //カプセルの数
    s32 GetCapsuleNum();

    //特効倍率
    s32 TokkouItemNum(s32 mid_event, enTokkouEffectType effectType);
    //特効倍率
    s32 TokkouItemMag(s32 mid_event, enTokkouEffectType effectType, s32 tokkouNum);

    //操作
    s32 GetCtrl();
    //現在の特効イベント
    LONGVECTOR GetNowTokkouEvent();
  };
}
