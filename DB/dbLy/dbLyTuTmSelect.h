//utf8
#pragma once
#include "dbLyTuBase.h"
#include "dbUGButton.h"
#include "dbUGChGirl.h"
//=========================================================================
// 
// チュートリアル：チーム選択
//
//=========================================================================
namespace db
{
  class TUGChGirl;
  class TUGFilter;
  class TUGTeam;
  class TUGDrTeam;		// チームドラム
  class TUGLiTmFilter;	// チームドラム用フィルター


  enum enGameList
  {
    gl_FC_DODGE = 0,		// 熱血高校ドッジボール部
    gl_FC_SOCCER,			// 熱血高校ドッジボール部サッカー編
    gl_FC_KOUSHINKYOKU,		// ダウンタウン熱血行進曲
    gl_FC_JIDAIGEKI,			// じだいげきいっか
    gl_FC_HOCKEY,			// いけいけ熱血ホッケー部
    gl_FC_KAKUTOU,			// 熱血格闘伝説
    gl_FC_SOCCER_LEAGUE,	// くにおくんの熱血サッカーリーグ
    gl_FC_BASKET,			// 熱血すとりーとバスケット
    gl_GB_SOCCER,			// 熱血高校サッカー部ＧＢ
    gl_GB_DODGE,			// 熱血高校ドッジボール部ＧＢ
    gl_SPC_SPDODGE,			//スーパードッジズ
    gl_Max,
  };
  const s32 TEAM_LIST[gl_Max] =
  {

    252,//0_ねっけつドッジぶ 熱血高校ドッジボール部
    95,//1_ドッジ＆サッカー 熱血高校ドッジボール部サッカー編
    109,//2_ねっけつチーム ダウンタウン熱血行進曲
    253,//3_じだいげきいっか　背景:113 BGM : 115（するが）
    120,//4_ねっけつホッケー いけいけ熱血ホッケー部
    255,//5_くにおりき 背景 166 BGM : 175（ステージ１）
    134,//6_ねっけつＦＣ＿Ｂ くにおくんの熱血サッカーリーグ
    147,//7_ダンクヒーローズ 熱血すとりーとバスケット
    168,//8_ワールドＳＣ１ 熱血高校サッカー部ＧＢ
    170,//9_ねっけつチーム 熱血高校ドッジボール部ＧＢ
    254,//10_スーパードッジズ　背景 : 361 BGM : 389（にほん）
  };
  #define __BG_TABLE_MODE__
  const s32 BG_LIST[gl_Max] =
  {
    46,// ねっけつドッジぶ	
    84,// ドッジ＆サッカー	
    95,	// ねっけつチーム	
    113,	// じだいげきいっか
    153,	// ねっけつホッケー	
    166,// くにおりき
    175,// ねっけつＦＣ＿Ｂ	
    180,// ダンクヒーローズ	
    203,// ワールドＳＣ１	
    207,// ねっけつチーム
    361,	// スーパードッジズ
  };
  #define __BGM_TABLE_MODE__
  const s32 BGM_LIST[gl_Max] =
  {
    33,// ねっけつドッジぶ	
    86,// ドッジ＆サッカー	
    94,// ねっけつチーム	
    115,// じだいげきいっか
    129,// ねっけつホッケー	
    175,// くにおりき１
    191,// ねっけつＦＣ＿Ｂ	
    222,// ダンクヒーローズ	
    254,// ワールドＳＣ１	
    270,// ねっけつチーム
    389,	// スーパードッジズ
  };



  class TLyTuTmSelect :public TLyTuBase
  {
    // 基底クラス定義
    typedef TLyTuBase base;

    struct HasebeDt
    {
      BOOL isReady_;
      BOOL isWalk_;
      s32 enterTimer_;	// 登場までの時間
      s32 talkCategory_;	// 喋りのカテゴリー番号
      s32 talkNo_;		// 今喋っているテキスト番号
      s32 muki_;
      s32 motionNo_;
      void Init(void)
      {
        isReady_ = FALSE;
        isWalk_ = FALSE;
        muki_ = 0;
        motionNo_ = 0;
        talkCategory_ = 0;
        talkNo_ = 0;
      }
    };


  public:
    //コンストラクタ
    explicit TLyTuTmSelect(TSnTutorial* pScene);
    virtual ~TLyTuTmSelect(void);

  protected:
    HasebeDt hasebeDt_;
    TUGChGirl ugHasebe_;
    TUGDrTeam*		ugdrTeam_;			// チームドラム
    TUGLiTmFilter*	uglistTmFilter_;	// チームドラムに必要なフィルター
    TUGTeam*		pUGTeam_;			// 選択したチームカード

    INTVECTOR		teamVec_;
    s32				selectMidTeamID_;

	BOOL			nowChange_;

    s32 nextBG_;
    s32 nextBGM_;

    //オーバーロード
    void DoLoad(void);
    void DoReady(void);
    void DoChangePage(s32 pageno);
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);

    void Post_TuTmSelect(void);
    void Connect_TuTmSelect(void);

  private:
    void Init(void);
    void EnterHasebe(void);
    void Talk(void);
    void TeamSelect(void);
    void TeamSelectCheck(void);


    void ChangeBG(s32 midTeam);
    void ChangeBGM(s32 midTeam);
  };

}
