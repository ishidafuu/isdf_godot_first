#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGGen.h"
#include "dbUGRName.h"
#include "dbUGRTimer.h"
#include "dbUGRBlink.h"
#include "dbUGTokkou.h"
#include "dbUGShSet.h"

namespace db
{
  struct TTeamSt
  {
    s32 sideNo_;
	s64 id_kantoku_;
    s32 id_team_;
    s32 mid_team_;
    BOOL tipsdraw_f_;
    BOOL nonedraw_f_;
    void Init()
    {
      Zeromem(this);
      mid_team_ = NGNUM;
    }
  };

  //リスト
  class TUGTeam : public TUGGen
  {
  public:
    //コンストラクタ
    explicit TUGTeam(TUIGenLayer* player);
    virtual ~TUGTeam(void);
    void MakeGroup(TUIObj* parent, s32 sideNo);
	void SetTeamByCOM(s64 id_kantoku, s64 id_team, s64 mid_team, s64 mid_event, s32 tmLv);	// COMチーム表示用
	void SetTeamByVSHum(s64 id_kantoku, s64 id_team, s64 mid_team, s64 mid_event, s32 tmLv); // 通信対戦、かちぬきの対戦相手カード
    void SetTeamDt3(s64 id_kantoku, s64 id_team, s64 mid_team, s64 mid_event, BOOL ready_f);	// チームカード単体表示用
	void SetTeamDt2(s64 id_kantoku, s64 id_team, s64 mid_team, s64 mid_event, BOOL ready_f, s32 id_team_end); // チームカードドラムリスト表示用
    void SetTeamDt_Get(s64 id_team, s64 mid_team, s32 lv);
    void SetTeamDt_Cnt(s64 id_team, s32 mid_team, s32 mid_team_end);
    void SetTipsDraw_f(BOOL tipsdraw_f);
    void RefreshStamina();
	void RefreshShoot(void);
	void RefreshParam(void);
	void RefreshCharaLv(void);
	void SetDrawParam(BOOL isDraw);
    void MakeIconTexture();
    void SetDrEnd(BOOL end_f);
    void SetItemGet(s64 mid_team, s32 lv, BOOL leftpos_f);
	void SetTeamView(s64 id_kantoku, s64 mid_team, BOOL leftpos_f);
    void SetItemCnt(s64 mid_team, BOOL leftpos_f);
    void SetLRPos(BOOL leftpos_f);
	void SetLRPosByGet(BOOL leftpos_f);
    void StaminaNone(BOOL none_f);
	void SetParam(s64 id_kantoku, s64 mid_team, s64 id_team, BOOL isLvNone, s32 tmLv);
	void SetParamTBL(s64 id_kantoku, s64 mid_team, s64 id_team, BOOL isLvNone, s32 tmLv);
	void SetCharaLv(s64 id_kantoku, s64 mid_team, s64 id_team);
  protected:
    void GenSetTeamDt(s64 id_kantoku, s64 id_team, s64 mid_team, BOOL ready_f);
    void DoUpdate(BOOL draw_f);
    //void DoAction();
    TTeamSt stTeam_;
    TUIObj* cap_;
	TUIObj* capLv_;
    TUIObj* mem_[DBMEMBER_ALL];
	TUIObj* memLv_[DBMEMBER_ALL];
    TUGRName ugname_;
    TUGRTimer ugtimer_;
    TUGRTimer ugtimer2_;
    TUGRBlink uggetstate_;
    TUGTokkou ugtokkou_;
	TUIObj* tokkouTxt_;
	TUGShSet ugshset_;

	//TUIObj* uiStrST_;	// 固定文字「ST」(チームカードに直接描きこまれたのでコメントアウト)
	TUIObj* uiTotalTmSt_; // チームの総ステータス

    //TUIObj* teamid_;
    TUIObj* staminafield_;
    TUIObj* stamina_;
    TUIObj* waittime_;

	s64 mid_prev_team_;
  };
 
}
