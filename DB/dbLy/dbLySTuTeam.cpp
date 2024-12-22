//utf8
#include "dbSnHome.h"
#include "dbLySTuTeam.h"
#include "dbUGFinger.h"
#include "dbUGSerifu.h"
//=========================================================================
// 
// サブチュートリアル：チームタブ初選択時の説明
//
//=========================================================================
namespace db
{

	// チュートリアル用メッセージ
	const s32 TUTO_TEAM_MSG_MAX = 4;
	const char* TUTO_TEAM_MSG[TUTO_TEAM_MSG_MAX][3] =
	{
		{
			"掛「チームでは＿ＢＧＭ・はいけいの＿えつらん",
			"チームごとの＿しゅーとそうび＿をおこなえます",
		},
		{
			"掛「なつかしＢＯＸガチャ＿で＿かくとくできる",
			"ＢＧＭ・はいけいは＿ホームに＿せっていでき",
		},
		{
			"掛「ホームにすると＿たいせんステージや",
			"クリアずみのステージを＿へんこう＿できます",
		},
		{
			//★指さし（BGM）
			"掛「まずは＿ＢＧＭで＿さきほど＿かくとくした",
			"ＢＧＭを＿きいてみては＿どうでしょうか？",
		},
	};

	enum enPage
	{
		pg_Default,
		pg_End,
	};

  enum enConnect
  {
    cn_stutoteam,
  };

  //-----------------------------------------------------------------
  // コンストラクタ
  //-----------------------------------------------------------------
//  TLySTuTeam::TLySTuTeam(TSnTutorial* pScene)
//   :TLySTuBase(pScene)
	TLySTuTeam::TLySTuTeam(TSnHome* pScene)
	: TLySTuBase(pScene)

	, serifuNo_(0)
  {
  }

  //-----------------------------------------------------------------
  // デストラクタ
  //-----------------------------------------------------------------
  TLySTuTeam::~TLySTuTeam()
  {
  }

  //-----------------------------------------------------------------
  // ロード
  //-----------------------------------------------------------------
  void TLySTuTeam::DoLoad()
  {
	  Base::DoLoad();
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLySTuTeam::DoReady()
  {
	  // 基底クラスのDoReadyをコール
	  Base::DoReady();

	  ugserifu_.SetMsgChangeMode(enMsgChangeMode_Touch);

	  // メッセージ行数 = 3;
	  s32 msgLine = 3;
	  serifuNo_ = 0;
	  SetMsg(TUTO_TEAM_MSG[serifuNo_][0], TUTO_TEAM_MSG[serifuNo_][1], TUTO_TEAM_MSG[serifuNo_][2]);
	  ugserifu_.SetDraw(TRUE);
	  serifuNo_++;

	  pScene_->lyHmBar_->SetDrawFinger(FALSE);
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLySTuTeam::DoChangePage(s32 pageno)
  {
  }

  void TLySTuTeam::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_stutoteam: Connect_STuTeam(); break;
        }
      }
      else
      {
        //エラーメッセージ
        EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
      }
    }
    else
    {
      //通信エラー
    }
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLySTuTeam::Post_STuTeam(void)
  {
	  if (pmgEO_->stSeq_.IsNoSv())
	  {
		  Connect_STuTeam();
	  }
	  else
	  {
      StartConnect(cn_stutoteam);
      pmgEO_->mgPo_.poTuto_.Ready_SetBit(enTutoBitType_SubTeam);
	  }
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLySTuTeam::Connect_STuTeam(void)
  {
	  SetHudEnd();
#ifdef __K_DEBUG_USER_DATA__
    kdebug::DebugUDManager::GetInstance()->SetTutoEnd(enTutoBitType_SubTeam);
#endif
	  // 自身も終わりを知らせる
	  isTutoEnd_ = TRUE;
	  ChangePage(pg_End);
	  pScene_->pmgUI_->SetModal(FALSE);
    SetHeadderModal(TRUE);//ヘッダーがモーダル状態でも聴くのをもどす

	pScene_->lyHmBar_->SetDrawFinger(TRUE);
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLySTuTeam::DoUpdate(BOOL draw_f)
  {
	  // 基底クラスのDoUpdateをコール
	  Base::DoUpdate(draw_f);

	  switch (stLy_.pageNo_)
	  {
		  case pg_End:
			  MvPage_End();
			  break;

		  case pg_Default:
		  default:
			  MvPage_Default();
			  break;
	  }
  }

  void TLySTuTeam::MvPage_Default(void)
  {
	  // メッセージの次へが押された
	  if (ugserifu_.GetActRes() == enUGSerifuAct_WaitEnd)
	  {
		  // チュートリアル終了
		  if (serifuNo_ >= TUTO_TEAM_MSG_MAX)
		  {
			  Post_STuTeam();
			  ugserifu_.SetDraw(FALSE);
			  ugfinger_.SetDraw(FALSE);
			  return;
		  }

		  // ボタンの説明メッセージ
		  if (serifuNo_ == 3)
		  {
			  //ugfinger_.SetRevUD(TRUE);
			  ugfinger_.SetPos_Full(232 + 32+5, 58 + 16+5);
			  ugfinger_.SetDraw(TRUE);
		  }

		  // 次のセリフセット
		  // メッセージ行数 = 3;
		  SetMsg(TUTO_TEAM_MSG[serifuNo_][0], TUTO_TEAM_MSG[serifuNo_][1], TUTO_TEAM_MSG[serifuNo_][2]);
		  serifuNo_++;
	  }

	  // メッセージスキップ処理
	  if (field_->sc_IsCamEnter())
	  {
		  if (ugserifu_.IsReadLineEnd())
		  {
			  ugserifu_.NextSerifu();
		  }
		  else
		  {
			  ugserifu_.SkipSerifu();
		  }
	  }

  }
  void TLySTuTeam::MvPage_End(void)
  {
  }
}
