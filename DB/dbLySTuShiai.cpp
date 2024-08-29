//utf8
#include "dbLySTuShiai.h"
#include "dbSnHome.h"
#include "dbUGSerifu.h"
//=========================================================================
// 
// サブチュートリアル：試合タブ選択時の説明
//
//=========================================================================
namespace db
{
	const s32 TUTO_SHIAI_MSG1_MAX = 2;
	const char* TUTO_SHIAI_MSG1[TUTO_SHIAI_MSG1_MAX][3] =
	{
		{
			"掛「まずモードの＿せんたくを＿して",
			"ドッジボールのしあいを＿はじめます",
		},
		{
			"掛「こんかいは＿きほんモードの",
			"シナリオマップを＿えらんでください",
		},
	};

	// チュートリアル用メッセージ
	const s32 TUTO_SHIAI_MSG2_MAX = 5;
	const char* TUTO_SHIAI_MSG2[TUTO_SHIAI_MSG2_MAX][3] =
	{
		{
			"掛「つうしんたいせん＿と",
			"イベントが＿かいほうしました",
		},
		{
			"掛「イベントは＿きかんげんてい",
			"ステージで＿あそぶことが＿できます",
		},
		{
			"掛「レアアイテムを＿ランキングや",
			"ドロップで＿かくとくできるかも！？",
		},
		{
			"掛「つうしんたいせん＿は",
			"ブルートゥースや＿Ｗｉ負Ｆｉで",
		},
		{
			"掛「ほかのプレーヤーと",
			"たいせんすることが＿できます",
		},
	};

	enum enPage
	{
		pg_Init,
		pg_SubTuto1,
		pg_SubTuto2,
		pg_End,
	};

  enum enConnect
  {
    cn_stutoevent,
  };


  //-----------------------------------------------------------------
  // コンストラクタ
  //-----------------------------------------------------------------
//  TLySTuShiai::TLySTuShiai(TSnTutorial* pScene)
//   :TLySTuBase(pScene)
	TLySTuShiai::TLySTuShiai(TSnHome* pScene)
	: TLySTuBase(pScene)

	, serifuNo_(0)
  {
  }

  //-----------------------------------------------------------------
  // デストラクタ
  //-----------------------------------------------------------------
  TLySTuShiai::~TLySTuShiai()
  {
  }

  //-----------------------------------------------------------------
  // ロード
  //-----------------------------------------------------------------
  void TLySTuShiai::DoLoad()
  {
	  Base::DoLoad();
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLySTuShiai::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_stutoevent: Connect_STuShiai(); break;
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
  void TLySTuShiai::Post_STuShiai(void)
  {
	  if (pmgEO_->stSeq_.IsNoSv())
	  {
		  Connect_STuShiai();
	  }
	  else
	  {
		  // ここに送信処理を書く
      StartConnect(cn_stutoevent);
      pmgEO_->mgPo_.poTuto_.Ready_SetBit(enTutoBitType_SubEventOpen);
	  }
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLySTuShiai::Connect_STuShiai(void)
  {
	  SetHudEnd();
#ifdef __K_DEBUG_USER_DATA__
	  kdebug::DebugUDManager::GetInstance()->SetTutoEnd(enTutoBitType_SubEventOpen);
#endif
	  // 自身も終わりを知らせる
	  isTutoEnd_ = TRUE;
	  field_->g_SetDraw(FALSE);
	  ChangePage(pg_End);
	  pScene_->pmgUI_->SetModal(FALSE);
    SetHeadderModal(TRUE);//ヘッダーがモーダル状態でも聴くのをもどす

	pScene_->lyHmBar_->ugfooter_.tabbtn_[enFooterTabID::footer_id_event]->SetDisable(FALSE);
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLySTuShiai::DoUpdate(BOOL draw_f)
  {
	  // 基底クラスのDoUpdateをコール
	  Base::DoUpdate(draw_f);

	  switch (stLy_.pageNo_)
	  {
	  case pg_Init:
		  MvPage_Init();
		  break;
	  case pg_SubTuto1:
		  MvPage_SubTuto1();
		  break;
	  case pg_SubTuto2:
		  MvPage_SubTuto2();
		  break;
	  
	  default:
		  case pg_End:
			  MvPage_End();
			  break;
	  }
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLySTuShiai::MvPage_Init(void)
  {
	  // 指差しを消す
	  pScene_->lyHmBar_->SetDrawFinger(FALSE);

	  // ugserfu_のデフォレイヤーはUIDP_05BUTTON
	  ugserifu_.SetDepth(UIDP_09MSGBOX);
	  ugserifu_.SetMsgChangeMode(enMsgChangeMode_Touch);

	  // メッセージ行数 = 3;
	  s32 msgLine = 3;
	  serifuNo_ = 0;
	  ugserifu_.SetDraw(TRUE);
	  ugserifu_.SetPos_Full(mid::midGetDotL() - 30, mid::midGetDotS() + 30);
	  ugserifu_.ChangeFrame(enSerifuFrame_Normal);

	  // サブチュートリアルのシナリオマップを終えていない
#ifdef __K_DEBUG_USER_DATA__
	  if (!kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_SubScenario))
#else
	  if (!pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_SubScenario))
#endif
	  {
		  // シナリオマップ以外は選択不可
		  pScene_->lyHmShiai_->SetScenarioOnly(TRUE);

		  SetMsg(TUTO_SHIAI_MSG1[serifuNo_][0], TUTO_SHIAI_MSG1[serifuNo_][1], TUTO_SHIAI_MSG1[serifuNo_][2]);
		  
		  // 自身を半透明で黒塗りしてフェード替わりにする
		  const s32 BARW = 512;
		  field_->z_SetPos_Full(0, 0);
		  field_->o_SetObjFilterGrp(BARW, BARW, 0xFF, 0, 0, 0);
		  field_->g_SetAlp(FALSE, 0x80);
		  field_->g_SetDepth(UIDP_08MSGFILTER);
		  ChangePage(pg_SubTuto1);
	  }
	  // 終えている
	  else
	  {
		  SetMsg(TUTO_SHIAI_MSG2[serifuNo_][0], TUTO_SHIAI_MSG2[serifuNo_][1], TUTO_SHIAI_MSG2[serifuNo_][2]);

		  // 自身を半透明で黒塗りしてフェード替わりにする
		  const s32 BARW = 512;
		  field_->z_SetPos_Full(0, 0);
		  field_->o_SetObjFilterGrp(BARW, BARW, 0xFF, 0, 0, 0);
		  field_->g_SetAlp(FALSE, 0x80);
		  field_->g_SetDepth(UIDP_08MSGFILTER);
		  ChangePage(pg_SubTuto2);
	  }
	  serifuNo_++;
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
	void TLySTuShiai::MvPage_SubTuto1(void)
	{
		// メッセージの次へが押された
		if (ugserifu_.GetActRes() == enUGSerifuAct_WaitEnd)
		{
			// チュートリアル終了
			if (serifuNo_ >= TUTO_SHIAI_MSG1_MAX)
			{
				ugserifu_.SetDraw(FALSE);
			
				//ゆびだす
				ugfinger_.SetMotionLookPoint(200, 100);
				ugfinger_.SetDraw(TRUE);
				
				field_->o_SetObjNull();
				//field_->g_SetDraw(FALSE);
				isTutoEnd_ = TRUE;
				ChangePage(pg_End);
				return;
			}

			// 次のセリフセット
			// メッセージ行数 = 3;
			SetMsg(TUTO_SHIAI_MSG1[serifuNo_][0], TUTO_SHIAI_MSG1[serifuNo_][1], TUTO_SHIAI_MSG1[serifuNo_][2]);
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

	//-----------------------------------------------------------------
	// 
	//-----------------------------------------------------------------
  void TLySTuShiai::MvPage_SubTuto2(void)
  {
	  // メッセージの次へが押された
	  if (ugserifu_.GetActRes() == enUGSerifuAct_WaitEnd)
	  {
		  // チュートリアル終了
		  if (serifuNo_ >= TUTO_SHIAI_MSG2_MAX)
		  {
			  Post_STuShiai();
			  ugserifu_.SetDraw(FALSE);
			  return;
		  }

		  // 次のセリフセット
		  // メッセージ行数 = 3;
		  SetMsg(TUTO_SHIAI_MSG2[serifuNo_][0], TUTO_SHIAI_MSG2[serifuNo_][1], TUTO_SHIAI_MSG2[serifuNo_][2]);
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
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLySTuShiai::MvPage_End(void)
  {
  }
}
