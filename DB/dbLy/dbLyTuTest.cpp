//utf8
#include "dbLyTuTest.h"
#include "dbSnTutorial.h"
#include "dbUGSerifu.h"
#include "dbUGChGirl.h"
//=========================================================================
// 
// テストクラス
//
//=========================================================================
namespace db
{
  enum enPage
  {
    pg_None,
    pg_InitTalk1,
    pg_Talk1,
    pg_InitTeamSelect,
    pg_TeamSelect,
    pg_InitTalk2,
    pg_Talk2,
    pg_InitFade,
    pg_Fade,
    pg_InitShiai,
    pg_Shiai,
  };


  enum enHaikei
  {
    hk_DOGE = 0,
    hk_HOCKEY,
    hk_BIKKURI,
    hk_JIDAIGEKI,
    hk_MAX,
  };
  const s32 haikeiNoTbl[hk_MAX] =
  {
    0,
    1,
    2,
    3,
  };

  //-----------------------------------------------------------------
  // コンストラクタ
  //-----------------------------------------------------------------
  TLyTuTest::TLyTuTest(TSnTutorial* pScene)
    :TLyTuGen(pScene)
    , pUIObj_(NULL)
    , ugbtn_(this)
    , haikeiNo_(hk_DOGE)
    , isTutorialEnd_(FALSE)
    , ugHasebe_(NULL)
    , ugserifu_(NULL)
    , ugfilter_(NULL)
  {
  }

  //-----------------------------------------------------------------
  // デストラクタ
  //-----------------------------------------------------------------
  TLyTuTest::~TLyTuTest()
  {
    Freemem(ugfilter_);
    Freemem(ugHasebe_);
    Freemem(ugserifu_);
  }

  //-----------------------------------------------------------------
  // ロード
  //-----------------------------------------------------------------
  void TLyTuTest::DoLoad()
  {
    pUIObj_ = MakeObj();
    //pUIObj_->o_SetObjGrPart();
    pUIObj_->g_SetPartNo(1);
    pUIObj_->g_SetCenter(TRUE);
    pUIObj_->o_SetParent(field_);

    hasebeDt_.Init();
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTest::DoReady()
  {
    //SetFoot_Tab();
    ugHasebe_ = new TUGChGirl(this);
    ugHasebe_->MakeGroup(field_, 11, FALSE);
    ugHasebe_->SetPos(0, 0);
    ugHasebe_->SetMirror(TRUE);
    ugHasebe_->SetMType(gmt_St);

    ugserifu_ = new TUGSerifu(this);
    ugserifu_->MakeGroup(field_, 2, enSerifuFrame_Wide);
    ugserifu_->SetParent(field_);
    ugserifu_->SetPos(-50, 30);
    ugserifu_->SetDraw(FALSE);
    ugserifu_->SetMsgChangeMode(enMsgChangeMode_Touch);

    ugfilter_ = new TUGFilter(this);
    ugfilter_->MakeGroup(field_);
    ugfilter_->SetDraw(FALSE);
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTest::DoChangePage(s32 pageno)
  {
    switch (pageno)
    {
    case db::pg_InitTalk1:			InitTalk1();	break;
    case db::pg_InitTeamSelect:	InitTeamSelect();	break;
    case db::pg_InitTalk2:			InitTalk2();	break;
    case db::pg_InitFade:			InitFade();		break;
    case db::pg_InitShiai:			InitShiai();	break;
    case db::pg_None:
    default:
      break;
    }
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTest::DoUpdate(BOOL draw_f)
  {
    pUIObj_->Update(draw_f);
    ugbtn_.Update(draw_f);

    switch (stLy_.pageNo_)
    {
    case pg_None:
      DoChangePage(pg_InitTeamSelect);
      break;

    case pg_Talk1:
      Talk1();
      break;

    case pg_TeamSelect:
      TeamSelect();
      break;

    case pg_Talk2:
      Talk2();
      break;

    case pg_Fade:
      Fade();
      break;

    case pg_Shiai:
      Shiai();
      break;

    default:
      break;
    }

    ugHasebe_->SetDraw(hasebeDt_.isReady_);
    ugHasebe_->Update(draw_f);
    ugserifu_->Update(draw_f);
    ugfilter_->Update(draw_f);

    // メッセージスキップ処理
    if (field_->sc_IsCamEnter())
    {
      if (ugserifu_->IsReadLineEnd())
      {
        ugserifu_->NextSerifu();
      }
      else
      {
        ugserifu_->SkipSerifu();
      }
    }
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTest::InitTalk1(void)
  {
    SetMsg("どんがばちょ", "ぶひぶは");
    ugserifu_->SetDraw(TRUE);
    stLy_.pageNo_ = pg_Talk1;
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTest::Talk1(void)
  {
    if (ugserifu_->IsActRes(enUGSerifuAct_ReadEnd))
    {
    }
    if (ugserifu_->IsActRes(enUGSerifuAct_WaitEnd))
    {
      ugserifu_->SetDraw(FALSE);
      DoChangePage(pg_InitTeamSelect);
    }
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTest::InitTeamSelect(void)
  {
    stLy_.pageNo_ = pg_TeamSelect;
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTest::TeamSelect(void)
  {
    //kdebug::DebugController* pCntrl = kdebug::DebugController::GetInstance();
    //if (pCntrl->IsTriggerButton(XINPUT_BUTTON_LEFT_STICK_UP))
    //{
    //}
    //else if (pCntrl->IsTriggerButton(XINPUT_BUTTON_LEFT_STICK_DOWN))
    //{
    //}
    //else if (pCntrl->IsTriggerButton(XINPUT_BUTTON_DPAD_LEFT))
    //{
    //	haikeiNo_--;
    //	//pmgUI_->LoadCourt(haikeiNoTbl[haikeiNo_]);//背景
    //	pmgUI_->LoadCourt(haikeiNo_);//背景
    //}
    //else if (pCntrl->IsTriggerButton(XINPUT_BUTTON_DPAD_RIGHT))
    //{
    //	haikeiNo_++;
    //	//pmgUI_->LoadCourt(haikeiNoTbl[haikeiNo_]);//背景
    //	pmgUI_->LoadCourt(haikeiNo_);//背景
    //}
    //else if (pCntrl->IsTriggerButton(XINPUT_BUTTON_A))
    //{
    //	hasebeDt_.isReady_ = TRUE;
    //	SetMsg("ほげほげ", "ふがふが");
    //	ugserifu_->SetDraw(TRUE);
    //}
    //else if (pCntrl->IsTriggerButton(XINPUT_BUTTON_B))
    //{
    //	hasebeDt_.isReady_ = FALSE;
    //	ugserifu_->SetDraw(FALSE);
    //}
    //else if (pCntrl->IsTriggerButton(XINPUT_BUTTON_X))
    //{
    //	hasebeDt_.isReady_ = FALSE;
    //	ugserifu_->SetDraw(FALSE);
    //	DoChangePage(pg_InitTalk2);
    //}
    //else if (pCntrl->IsTriggerButton(XINPUT_BUTTON_Y))
    //{
    //	DoChangePage(pg_InitFade);
    //}
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTest::InitTalk2(void)
  {
    SetMsg("とらひげ", "かいぞくだん");
    ugserifu_->SetDraw(TRUE);
    stLy_.pageNo_ = pg_Talk2;
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTest::Talk2(void)
  {
    if (ugserifu_->IsActRes(enUGSerifuAct_ReadEnd))
    {
    }
    if (ugserifu_->IsActRes(enUGSerifuAct_WaitEnd))
    {
      ugserifu_->SetDraw(FALSE);
      DoChangePage(pg_InitTalk1);
    }
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTest::InitFade(void)
  {
    ugfilter_->SetDraw(TRUE);
    //ugfilter_->OffFilter(2, FALSE);
    ugfilter_->OnFilter(ugfilter_fadeout2, FALSE, FALSE);
    stLy_.pageNo_ = pg_Fade;
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTest::Fade(void)
  {
    if (ugfilter_->GetActRes() == ugfilter_fadeout2)
    {
      //ugfilter_->SetDraw(FALSE);
      DoChangePage(pg_InitShiai);
    }
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTest::InitShiai(void)
  {
    //pmgEO_->mgStSn_.EtcSS.SetShiaiType_TutoSmart();//チュートリアル
    isTutorialEnd_ = TRUE;
    stLy_.pageNo_ = pg_Shiai;
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTest::Shiai(void)
  {
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuTest::SetMsg(const char* text0, const char* text1)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    serifuvec_.push_back(text1);
    ugserifu_->SetSerifu(serifuvec_);
  }
}
