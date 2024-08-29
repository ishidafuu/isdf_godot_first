#include "dbLyHmEvent.h"
#include "dbSnHome.h"

namespace db
{
  enum enPage
  {
	pg_Init,
    pg_Tuto,
	pg_Event_None,
    pg_Main,
    pg_InEvent,
    pg_Idle,
  };

  enum enConnect
  {
    cn_Omakase,
    //cn_Event,
  };

  const char* TITLE_TEXT = "おすすめのイベント";

  //コンストラクタ
  TLyHmEvent::TLyHmEvent(TSnHome* pScene) :TLyHmGen(pScene)
    , uglist_event_(this)
	, ugevbtn_(this)
	, ugrmsg_ev_(this)
	, ugtitle_ev_(this)
  {
  }

  TLyHmEvent::~TLyHmEvent()
  {
    for (s32 i = 0; i < uglock_.size(); ++i) Freemem(uglock_[i]);
  }

  void TLyHmEvent::SetupEvent(s32 eventNo)
  {
	  if (eventNo > 0)
	  {
		  s32 mid_event = eventNo;
		  if (uglist_event_.records_.size() != 0)
		  {
			  s32 eventWID = pmgEO_->mgCoU_.GetEventWID(mid_event, TRUE);
			  if (eventWID != 0)
			  {
				  pmgEO_->stSeq_.SetIDEvent(mid_event, eventWID);//イベントID
				  pScene_->lyHmEvEvent_->Ready();
				  ChangePage(pg_InEvent);
			  }
		  }
	  }
	  // ジャンプが終わったらジャンプデータはクリアしておく
	    pmgEO_->stJump_.ClearJumpAreaData();
  }

  void TLyHmEvent::DoLoad()
  {
	  ugtitle_ev_.MakeGroup(field_, FALSE);
	  ugtitle_ev_.SetMessage(TITLE_TEXT);
	  ugtitle_ev_.SetLRPos(TRUE);

	  ugevbtn_.MakeGroupEvent(field_, hmgrp_->hmshiai_next_);
	  ugevbtn_.SetPos(-85, -18);

	  ugrmsg_ev_.MakeGroup_Mini(field_, FALSE);
	  ugrmsg_ev_.SetPos(-161, 24);
	  ugrmsg_ev_.SetDraw(TRUE);

    uglist_event_.MakeGroup(field_, FALSE);

  }
  void TLyHmEvent::DoReady()
  {
    SetFoot_Tab();
    uglist_event_.Refresh();
  }

  void TLyHmEvent::Idle()
  {
    ChangePage(pg_Idle);
  }

  void TLyHmEvent::MvPage_Init(void)
  {
	  BOOL isEnd = IsSubEventOpen();
	  // チュートリアルがまだおわっていない
	  if (!isEnd)
	  {
		  ChangePage(pg_Tuto);
		  return;
	  }

	  // イベントの数によって表示をかえる
	  s32 eventNum = uglist_event_.records_.size();

	  // イベントなし
	  if (eventNum == 0)
	  {
		  // 全部非表示
		  uglist_event_.SetDraw(FALSE);
		  ugtitle_ev_.SetDraw(FALSE);
		  ugevbtn_.SetDraw(FALSE);

		  // イベントないのでメッセージウィンドウだけ出す(予定)
		  STRVECTOR s = { "","イベントは＿かいさい＿していません" };
		  ugrmsg_ev_.SetData(&s);
		  ugrmsg_ev_.SetPos(-76, -30);
		  ugrmsg_ev_.SetDraw(TRUE);
		  ChangePage(pg_Event_None);
	  }
	  // イベント１個だけ
	  else if (eventNum == 1)
	  {
		  // イベントリストだけ非表示
		  uglist_event_.SetDraw(FALSE);
		  ugtitle_ev_.SetDraw(TRUE);
		  ugevbtn_.SetDraw(TRUE);

		  ugtitle_ev_.SetPos(0, -70);

		  // １個だけ表示のUI
		s64 mid_event = uglist_event_.records_[0]->GetMIDEvent();
		ugevbtn_.SetEvent(mid_event);
		ugevbtn_.SetPos(0, -18);

		STRVECTOR v;
		v.clear();
		s32 msgNum = mdm_event_ostext2_c64 - mdm_event_ostext0_c64 + 1;
		for (s32 i = 0; i < msgNum; i++)
		{
			v.push_back(pmgEO_->mgCoM_.mdm_Event_.GetPKDtStr(mid_event, (enmdm_EventStr)(mdm_event_ostext0_c64 + i)));
		}
		ugrmsg_ev_.SetData(&v);
		ugrmsg_ev_.SetPos(-76, 24);
		ChangePage(pg_Main);
	  }
	  // イベント２個以上(通常)
	  else
	  {
		  // 全部表示
		  uglist_event_.SetDraw(TRUE);
		  ugtitle_ev_.SetDraw(TRUE);
		  ugevbtn_.SetDraw(TRUE);
		  ugrmsg_ev_.SetDraw(TRUE);

		  // とりあえず0番目をセット
		  s64 favoriteEvent = uglist_event_.records_[0]->GetMIDEvent();
		  s64 maxPriority = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(favoriteEvent, mdm_event_priority);
		  for (s32 i = 0; i < eventNum; i++)
		  {
			  s64 event = uglist_event_.records_[i]->GetMIDEvent();
			  s64 prio = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(event, mdm_event_priority);

			  if (prio >= maxPriority)
			  {
				  maxPriority = prio;
				  favoriteEvent = event;
			  }
		  }

		  STRVECTOR v;
		  v.clear();
		  s32 msgNum = mdm_event_ostext2_c64 - mdm_event_ostext0_c64 + 1;
		  for (s32 i = 0; i < msgNum; i++)
		  {
			  v.push_back(pmgEO_->mgCoM_.mdm_Event_.GetPKDtStr(favoriteEvent, (enmdm_EventStr)(mdm_event_ostext0_c64 + i)));
		  }
		  // 同じものを表示しないようにリストをリフレッシュ
		  uglist_event_.Refresh(favoriteEvent);

		  ugevbtn_.SetEvent(favoriteEvent);
		  ugrmsg_ev_.SetData(&v);

		  ugtitle_ev_.SetLRPos(TRUE);
		  ugevbtn_.SetPos(-85, -18);
		  ugrmsg_ev_.SetPos(-161, 24);

		  ChangePage(pg_Main);
	  }
  }

  void TLyHmEvent::MvPage_Tuto(void)
  {
    BOOL isEnd = IsSubEventOpen();


    // ここでのチュートリアルが終わった時用
    if (pScene_->lySTuShiai_ && pScene_->lySTuShiai_->IsTutoEnd())
    {
      // 初回のはじめて試合タブ開いた時のチュートリアルが終わっただけなら
      // シナリオマップのボタンだけを押せるようにする
      if (IsSubEventOpen() == FALSE)
      {
        pScene_->lyHmBar_->SetDrawFinger(FALSE);
        // ロックをかける
        pScene_->pmgUI_->SetModal(TRUE);

        //ヘッダとフッタきかないように
        frame_->ugheadder_->SetDisable();
        frame_->ugfooter_->SetDisable();

        // これだけ押せるように
        for (auto& item : uglist_event_.records_)
        {
          item->SetDisable(TRUE);
        }

        for (s32 i = 0; i < uglist_event_.records_.size(); ++i)
        {
          TUGLock* newlock = new TUGLock(this);
          newlock->MakeGroup(uglist_event_.records_[i]->GetField());
          newlock->SetPos(72, 16);
          newlock->SetDepth(UIDP_06BUTTONHI);
          uglock_.push_back(newlock);
        }

      }

      isEnd = TRUE;
    }

    if (isEnd)
    {
      ChangePage(pg_Main);
    }
  }
  void TLyHmEvent::MvPage_EventNone()
  {

  }
  void TLyHmEvent::MvPage_Main()
  {

    if (pmgEO_->stSeq_.PopNeedOmakase())//おまかせ更新フラグチェック＆たたみ
    {
      shsetvec_ = pmgEO_->mgCoU_.GetShotOmakase();
      Post_Omakase();
    }
    else if (uglist_event_.IsAct())//イベントマップ
    {
      pmgEO_->stSeq_.SetIDEvent(uglist_event_.GetMIDEvent(), pmgEO_->mgCoU_.GetEventWID(uglist_event_.GetMIDEvent(), FALSE));//イベントID
      //pmgEO_->mgStSn_.EtcSS.SetEventID(uglist_event_.GetMIDEvent(), pmgEO_->mgCoU_.GetEventWID(uglist_event_.GetMIDEvent()));//イベントID
      pScene_->lyHmEvEvent_->Ready();
      ChangePage(pg_InEvent);
    }
	else if (ugevbtn_.IsAct())
	{
		s64 mid_event = ugevbtn_.stBtnEvent_.mid_event_;
		s32 eventWID = pmgEO_->mgCoU_.GetEventWID(mid_event, TRUE);
		pmgEO_->stSeq_.SetIDEvent(mid_event, eventWID);//イベントID
		pScene_->lyHmEvEvent_->Ready();
		ChangePage(pg_InEvent);
	}
  }
  void TLyHmEvent::MvPage_InEvent()
  {

  }
  void TLyHmEvent::MvPage_Idle()
  {

  }

  //POST：おまかせ
  void TLyHmEvent::Post_Omakase()
  {
    StartConnect(cn_Omakase);
    pmgEO_->mgPo_.poHmTeam_.Ready_attachshset(pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_shset), &shsetvec_);
  }


  //通信：おまかせ
  void TLyHmEvent::Connect_Omakase()
  {
    //ChangePage(pg_Menu);
  }
  ////通信：イベント更新
  //void TLyHmEvent::Connect_Event()
  //{
  //  //ChangePage(pg_Menu);
  //  uglist_event_.Refresh();
  //}

  //接続完了
  void TLyHmEvent::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_Omakase: Connect_Omakase(); break;
        //case cn_Event: Connect_Event(); break;
        }
      }
      else
      {
        //エラーメッセージ
        EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
        //ChangePage(pg_Error);
      }
    }
    else
    {
      //ChangePage(pg_Error);
    }
  }

  void TLyHmEvent::DoUpdate(BOOL draw_f)
  {
    switch (stLy_.pageNo_)
    {
			case db::pg_Init:
			case db::pg_InEvent:
			case db::pg_Idle:
      break;

    default:
      for (s32 i = 0; i < uglock_.size(); ++i) uglock_[i]->Update(draw_f);
      uglist_event_.Update(draw_f);
			ugtitle_ev_.Update(draw_f);
			ugevbtn_.Update(draw_f);
			ugrmsg_ev_.Update(draw_f);
      //ugicon_.Update(draw_f);
      break;
    }

    if (draw_f == FALSE)
    {
      if (pScene_->lyHmBar_->IsBarAction()) return;
      switch (stLy_.pageNo_)
      {
				case db::pg_Init:MvPage_Init(); break;
				case db::pg_Tuto:MvPage_Tuto(); break;
				case db::pg_Event_None:MvPage_EventNone(); break;
				case db::pg_Main:MvPage_Main(); break;
				case db::pg_InEvent:MvPage_InEvent(); break;
				case db::pg_Idle:MvPage_Idle(); break;
      }
    }
  }

  BOOL TLyHmEvent::IsSubEventOpen()
  {
    BOOL res = FALSE;
#ifdef __K_DEBUG_USER_DATA__
    if (kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_SubEventOpen))	// チュートリアルが全て終了している
#else
    if (pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_SubEventOpen))
#endif
    {
      res = TRUE;
    }
    return res;
  }

}
