#pragma once
#include "dbInclude.h"
#include "dbUGDrGen.h"
#include "dbUGTeam.h"
#include "dbUGTeamUra.h"


namespace db
{

  enum enUGDrTeamAct
  {
    enUGDrTeamAct_Roll,
    enUGDrTeamAct_Stop,
    enUGDrTeamAct_Start,
  };

  struct TDrTeamSt
  {
    BOOL cnt_f_;
    s32 sideno_;
    s64 mid_team_lastload_;
    s32 refresh_c_;
    void Init()
    {
      Zeromem(this);
      mid_team_lastload_ = NGNUM;
    }
  };

  class TUGLiTmFilter;

  //ドラム
  class TUGDrTeam :public TUGDrGen
  {
  public:
    //コンストラクタ
    explicit TUGDrTeam(TUIGenLayer* player);
    virtual ~TUGDrTeam(void);
    void MakeGroup(TUIObj* parent, BOOL leftpos_f, s32 sideno, TUGLiTmFilter* ugliTmfilter);
    BOOL IsActRes(enUGDrTeamAct res);
    s64 GetMIDTeam();
    s64 GetIDTeam();
    void RefreshStamina();
	void RefreshParam(void);
    void RefreshRoll(BOOL ready_f);
    void ReadyFilter();
	void ReadyFilter(TUGLiTmFilter* ugliTmfilter);
    void ReadySort();
    void Ready_Cnt(INTVECTOR* mid_teamvec);
    s32 GetRecordNum();
    void RevUra();

    TDrTeamSt stDrTeam_;
    std::vector<TUGTeam*> records_;
    LONGVECTOR id_teamvec_;
    INTVECTOR mid_teamvec_cnt_;//コンテンツ表示用（自分所持ではない）
    TUGTeamUra ura_;
    TUGTeam drumend_;
    TUGLiTmFilter* ugliTmfilter_;
  protected:
    s64 GetMIDTeam(s32 recordno);
    s64 GetIDTeam(s32 recordno);
    s32 GetEndIDTeam();
    void LoadTex(s64 mid_team, BOOL end_f);
    void DoUpdateSub(BOOL draw_f);//更新
    void DoActionSub();
    void DoStop();
    void DoRoll();
	void DoStart();
    void DoReady();
  };
 
}
