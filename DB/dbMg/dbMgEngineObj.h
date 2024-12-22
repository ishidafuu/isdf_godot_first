#pragma once
#include "dbInclude.h"
#include "dbMgData.h"
#include "dbMgGrp.h"
//#include "dbMgStScene.h"
#include "dbMgCoMaster.h"
#include "dbMgCoUser.h"
#include "dbMgCoInfo.h"
//#include "dbMgCoWorld.h"
#include "dbMgCoZip.h"

#include "dbMgPost.h"
#include "dbMgNSF.h"

#include "dbStSnPad.h"
#include "dbStSnDrop.h"
#include "dbStSnResult.h"
#include "dbStSnShiai.h"
#include "dbStSnSeq.h"
#include "dbStSnJump.h"


namespace db
{
  //TMgEngineObjBaseを継承
  class TMgEngineObj :public base::TMgEngineObjBase
  {
  public:
    explicit TMgEngineObj(void);
    virtual ~TMgEngineObj(void);
    //読み込み
    virtual void DoLoadData();
    virtual void DoLoadGrp();
    //準備
    virtual void DoReady();
    //更新
    virtual void DoUpdate();

    //データ
    TMgData mgDt_;
    //グラフィック
    TMgGrp mgGrp_;
    //チェックアウト
    TMgCoUser mgCoU_;
    TMgCoMaster mgCoM_;
    TMgCoInfo mgCoInfo_;
    TMgCoZip mgCoClmaster_;
    TMgCoZip mgCoSvmaster_;
    TMgCoZip mgCoBannerBmp_;
    TMgCoZip mgCoEventBmp_;
    //POST
    TMgPost mgPo_;
    TMgNSF mgNSF_;

    //シーンステータス
    //TMgStScene mgStSn_;
    TStSnPad stPad_;
    TStSnDrop stDrop_;
    TStSnResult stResult_;
    TStSnShiai stShiai_;
    TStSnSeq stSeq_; 
    TStSnJump stJump_;

	void RefreshStateTbl(void);	// ステータステーブル更新
	void RefreshStateTbl(s64 mid_team);	// ステータステーブル更新
	s64 GetTopTeamByMasterID(void);	// 最強チーム取得
	s64 GetTopTeamByUserID(void);	// 最強チーム取得
	s64 CalcSt(s64 mid_team, s64 id_team, s64 charaid, s32 type, s32 lv, s32 tmLv = NGNUM);
	s64 GetSt(s64 id_team, s64 charaPos, s32 type);
  private:
	  void ClearStData(void);
	  f64 GetNowBaseSt(s64 charaid, s64 charaLv, s64 teamLv, s64 teamRarity);		// 以下の３つを足した値
	  f64 GetBaseSt(s64 charaid);		// 基礎総合値
	  f64 GetTeamLvSt(s64 charaid, s64 teamRarity, s64 teamLv);		// チームレベル値
	  f64 GetCharaLvSt(s64 lv);				// キャラレベル値

	  // すでに計算済みのステータスデータ
	  std::map<s64, LONGVECTOR2> stData_;
	  s64 id_team_top_;		// 最強チーム(ユーザーID)
	  s64 mid_team_top_;	// 最強チーム(マスターID)
  };
}
