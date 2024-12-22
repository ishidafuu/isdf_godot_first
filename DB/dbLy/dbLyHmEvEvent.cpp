#include "dbLyHmEvEvent.h"
#include "dbSnHome.h"
#include "dbUGPrev.h"
namespace db
{

  enum enPage
  {
    pg_Visit,
    pg_Menu,
    pg_Help,
    pg_Ranker,
    pg_MyRank,
    pg_tourReady,
    pg_Match,
    pg_TmSelect,
    pg_ShSetSelect,
    pg_Filter,
    pg_MGK,
    pg_Nige,
    pg_GotoVS,
    pg_VS,
    pg_timeAttackReady,
    pg_timeAttackMatch,
    pg_timeAttackNige,
    pg_GotoShiai,

    pg_reward,
    pg_nowreward,
    pg_nextreward,
    pg_selectreward,
    pg_prevreward,

    pg_TokkouInfo,
    pg_TokkouBGM,
    pg_TokkouHaikei,

    pg_Out,
    pg_Error,
  };


  enum enConnect
  {
    cn_EventTourReady,
    cn_EventTourPlay,
    cn_EventTourRetire,
    cn_Ranker,
    cn_MyRank,
    cn_MGK,
    cn_SwitchShSet,
    cn_TimeAttackReady,
    cn_TimeAttackPlay,
    cn_TimeAttackRetire,
    cn_RewardRanker,
  };

  enum enEventTYpe
  {
    evt_Map,
    evt_Tour,
    evt_VSHum,
    evt_TimeAttack,
  };

  //コンストラクタ
  TLyHmEvEvent::TLyHmEvEvent(TSnHome* pScene)
    :TLyHmGen(pScene)
    , ugevbtn_(this)
    , ugprof_(this)
    , ugrmsg_ev_(this)
    , uglistRankMenu_(this)
    , uglistRanker_(this)
    , uglistMyRank_(this)
    , uglistTmFilter_(this)
    , ugdrTeam_(this)
    , ugTeam_(this)
    , ugencard_(this)
    , ugvs_(this)
    , ugtitle_me_(this)
    , ugtitle_en_(this)
    , uglistShSetSelect_(this)
    , uglistShSetSelectAll_(this)
    , uglistMember_(this)
    , ugNextReward_(this)
    , ugNowReward_(this)
    , ugLiRkReward_(this)
    , ugRkReward_(this)
    , ugTokkouBGM_(this)
    , ugTokkouInfo_(this)
    , ugTokkouHaikei_(this)
  {
    for (s32 i = 0; i < DBSIDE; ++i) ugvsteam_.push_back(new TUGScVSTeam(this));
    stEvent_.Init();
  }

  TLyHmEvEvent::~TLyHmEvEvent()
  {
    for (s32 i = 0; i < ugvsteam_.size(); ++i) Freemem(ugvsteam_[i]);
  }

  void TLyHmEvEvent::DoLoad()
  {
    ugprof_.MakeGroup(field_, TRUE);
    ugprof_.SetDraw(FALSE);

    ugevbtn_.MakeGroupEvent(field_, hmgrp_->hmshiai_next_);
    ugevbtn_.SetPos(-80, -40);

    ugrmsg_ev_.MakeGroup(field_, TRUE);
    ugrmsg_ev_.SetPos(-156, 0);


    uglistRankMenu_.MakeGroup(field_, FALSE);
    uglistRankMenu_.SetDepth(UIDP_07BUTTONTOP);

    uglistRanker_.MakeGroup(field_, FALSE);
    uglistRanker_.SetDepth(UIDP_07BUTTONTOP);

    uglistMyRank_.MakeGroup(field_, FALSE);
    uglistMyRank_.SetDepth(UIDP_07BUTTONTOP);

    uglistTmFilter_.MakeGroup(field_, FALSE);

    uglistShSetSelect_.MakeGroup(field_, FALSE);
    uglistShSetSelectAll_.MakeGroup(field_, FALSE);
    uglistMember_.MakeGroup(field_, TRUE);


    ugdrTeam_.MakeGroup(field_, TRUE, SIDE0, &uglistTmFilter_);
    ugdrTeam_.SetDraw(FALSE);
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      ugvsteam_[i]->MakeGroup(field_, i);
    }

    ugTeam_.MakeGroup(field_, SIDE0);
    ugTeam_.SetLRPos(TRUE);
    ugTeam_.SetDraw(FALSE);

    //敵チームカード
    ugencard_.MakeGroup(field_, SIDE1);
    ugencard_.SetLRPos(FALSE);
    ugencard_.StaminaNone(TRUE);

    //VS
    ugvs_.MakeGroup(field_);

    listvec_.clear();
    listvec_.push_back(&uglistRankMenu_);
    listvec_.push_back(&uglistRanker_);
    listvec_.push_back(&uglistMyRank_);
    listvec_.push_back(&uglistTmFilter_);

    ugtitle_me_.MakeGroup(field_, FALSE);
    ugtitle_me_.SetMessage("あなたのチーム");
    ugtitle_me_.SetLRPos(TRUE);
    ugtitle_en_.MakeGroup(field_, FALSE);
    ugtitle_en_.SetMessage("あいてのチーム");
    ugtitle_en_.SetLRPos(FALSE);
    ugtitle_me_.SetDraw(FALSE);
    ugtitle_en_.SetDraw(FALSE);

    // スタミナ回復先導カーソル
    pCursor_ = MakeObj();
    pCursor_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_finger));
    pCursor_->g_SetPartNo(1);
    //pCursor_->o_SetParent(field_);
    pCursor_->o_SetParent(ugbtn_mgk_->GetField());
    pCursor_->g_SetDraw(FALSE);
    pCursor_->z_SetPos(50, -28);


    // 次の報酬
    ugNextReward_.Init();
    ugNextReward_.SetPos(-150, -80);
    ugNextReward_.SetDraw(FALSE);

    // 今の報酬
    ugNowReward_.Init();
    ugNowReward_.SetPos(-150, 0);
    ugNowReward_.SetDraw(FALSE);

    // 報酬リストを選んだ時に
    // フェードとタイトルバーを表示するためだけに必要
    // 時間がないのでしかたない
    ugRkReward_.Init();
    ugRkReward_.SetPos(0, 0);
    ugRkReward_.SetDraw(FALSE);

    ugLiRkReward_.MakeGroup(field_, FALSE);

    ugTokkouBGM_.MakeGroup(field_);
    ugTokkouBGM_.SetDraw(FALSE);

    ugTokkouInfo_.MakeGroup(field_);
    ugTokkouInfo_.SetDraw(FALSE);

    ugTokkouHaikei_.MakeGroup(field_);
    ugTokkouHaikei_.SetDraw(FALSE);
  }

  void TLyHmEvEvent::DoReady()
  {
    pScene_->lyHmBar_->SwitchInside(TRUE);


    stEvent_.Init();


    s32 eventtype = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(pmgEO_->stSeq_.GetMidEvent(), mdm_event_eventType);
    if (eventtype == evt_TimeAttack)
    {
        uglistTmFilter_.Ready(pmgEO_->stSeq_.GetMidEvent());
    }
    else
    {
        uglistTmFilter_.Ready(NGNUM);
    }
    ugdrTeam_.ReadyFilter();

    DrawList(NULL);
    ugbtn_back_->SetFont("もどる");
    ugbtn_ok_->SetFont("たいけつ！");
    ugbtn_next_->SetFont("ならびかえ");
    ugbtn_mid_->SetFont("しゅーと");
    ugbtn_back_->SetDraw(TRUE);
    ugbtn_ok_->SetDraw(FALSE);
    ugbtn_mgk_->Refresh();

    //pmgEO_->stSeq_.GetMidEvent();

    ugevbtn_.SetEvent(pmgEO_->stSeq_.GetMidEvent());
    ugrmsg_ev_.SetEvent(pmgEO_->stSeq_.GetMidEvent());
    uglistRankMenu_.RefreshEvent(pmgEO_->stSeq_.GetMidEvent());


#ifdef __K_DEBUG_USER_DATA__
    mid_Ranking_ = 12;
#else
    s64 mid_event = pmgEO_->stSeq_.GetMidEvent();
    mid_Ranking_ = NGNUM;
    for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_Ranking_.GetLineNum(); i++)
    {
        // 該当のイベントではない
        if (pmgEO_->mgCoM_.mdm_Ranking_.GetLineDt(i, enmdm_Ranking::mdm_ranking_id_m_event) != mid_event){ continue; }
        mid_Ranking_ = pmgEO_->mgCoM_.mdm_Ranking_.GetLineDt(i, enmdm_Ranking::mdm_ranking_pk);
        break;
    }
#endif

    // 変数も初期化しておく
    prevPage_ = NGNUM;
    nowRank_ = NGNUM;
    nowRewardRank_ = NGNUM;
    nextRewardRank_ = NGNUM;
  }

  void TLyHmEvEvent::DoDown()
  {
    pScene_->lyHmBar_->SwitchInside(FALSE);
    pScene_->lyHmEvent_->Ready();
  }


  void TLyHmEvEvent::DoChangePage(s32 pageno)
  {
    switch ((enPage)pageno)
    {
    case db::pg_Visit:
      break;
    case db::pg_Menu:
      DrawList(&uglistRankMenu_);
      frame_->ugheadder_->SetEnable();//ヘッダ有効
      ugtitle_me_.SetDraw(FALSE);
      ugtitle_en_.SetDraw(FALSE);

      pmgEO_->mgCoU_.CleanOtherKantoku();//他人情報削除（取得して逆送してきたときにここで消す）
      ugevbtn_.SetDraw(TRUE);
      ugevbtn_.Refresh();
      ugbtn_back_->SetFont("もどる");
      ugbtn_ok_->SetFont("たいけつ！");
      ugrmsg_ev_.SetDraw(TRUE);
      break;
    case db::pg_Help:
      mid::midModalPanel_HelpSingle(&pmgEO_->mgCoM_.mdm_Help_.model_, stEvent_.helpType_, stEvent_.helpNo_);
      break;
    case db::pg_Ranker:
      DrawList(&uglistRanker_);
      frame_->ugheadder_->SetDisable();//ヘッダ無効
      pmgEO_->mgCoU_.MakeChestRanker();//ランカーアイコン作成

      if (stEvent_.rankingpage_ == 0)
      {
        uglistRanker_.Refresh(TRUE, mid_Ranking_);
        ugprof_.SetProfMe();
      }
      else
      {
          uglistRanker_.Refresh(FALSE, mid_Ranking_);
      }
      SwitchRankerPageBtn();
      break;
    case db::pg_MyRank:
      DrawList(&uglistMyRank_);
      pmgEO_->mgCoU_.MakeChestRanker();//ランカーアイコン作成
      uglistMyRank_.Refresh(FALSE, mid_Ranking_);
      ugprof_.SetProfMe();
      break;
    case db::pg_tourReady:
      DrawList(NULL);
      ugbtn_back_->SetDraw(FALSE);//戻るボタン消す
      break;
    case db::pg_Match:
      DrawList(NULL);
      ugtitle_me_.SetMessage("あいてのプロフィール");
      ugtitle_me_.SetDraw(TRUE);
      ugtitle_en_.SetDraw(TRUE);
      frame_->ugheadder_->SetDisable();//ヘッダ無効
      ugbtn_back_->SetDraw(TRUE);
      ugbtn_back_->SetFont("たいけつ！");
      ugbtn_next_->SetFont("ならびかえ");
      ugprof_.SetProfEnemy(0);
      //敵チーム
	  stEvent_.id_kantoku_en_ = pmgEO_->mgCoU_.mdre_Enemy_.GetLineDt(0, mdre_enemy_id_kantoku);
      stEvent_.id_team_en_ = pmgEO_->mgCoU_.mdre_Enemy_.GetLineDt(0, mdre_enemy_id_Team);
      stEvent_.mid_team_en_ = pmgEO_->mgCoU_.mdTeam_.GetPKDt(stEvent_.id_team_en_, mdteam_id_m_team);
      stEvent_.mid_bgm_en_ = pmgEO_->mgCoU_.mdre_Enemy_.GetLineDt(0, mdre_enemy_id_m_bgm);
      stEvent_.mid_court_en_ = pmgEO_->mgCoU_.mdre_Enemy_.GetLineDt(0, mdre_enemy_id_m_haikei);

	  LoadTeamEnemy(stEvent_.id_kantoku_en_, stEvent_.mid_team_en_);
      //ugencard_.SetTeamDt3(stEvent_.id_team_en_, stEvent_.mid_team_en_, NGNUM, TRUE);
	  ugencard_.SetTeamDt2(stEvent_.id_kantoku_en_, stEvent_.id_team_en_, stEvent_.mid_team_en_, NGNUM, TRUE, FALSE);
	  ugencard_.SetCharaLv(stEvent_.id_kantoku_en_, stEvent_.mid_team_en_, stEvent_.id_team_en_);
	  ugencard_.SetParam(stEvent_.id_kantoku_en_, stEvent_.mid_team_en_, stEvent_.id_team_en_, FALSE, NGNUM);
      ugencard_.SetTipsDraw_f(TRUE);
      ugencard_.SetDraw(TRUE);
      ugencard_.StaminaNone(TRUE);

      stEvent_.id_team_dr_ = ugdrTeam_.GetIDTeam();
      stEvent_.mid_team_dr_ = ugdrTeam_.GetMIDTeam();
      stEvent_.id_team_my_ = stEvent_.id_team_dr_;//自分のチーム
      //ugbtn_ok_->SetDraw(FALSE);//もう押せない
      LoadTeamTexture(SIDE0, pmgEO_->mgCoU_.mdTeam_.GetPKDt(stEvent_.id_team_my_, mdteam_id_m_team));

      //ugbtn_ok_->SetDraw(TRUE);
      //ugbtn_ok_->SetFont("たいけつ！");
      break;
    case db::pg_TmSelect:
      DrawList(NULL);
      frame_->ugheadder_->SetEnable();
      ugtitle_me_.SetMessage("あなたのチーム");
      ugtitle_me_.SetDraw(TRUE);
      ugdrTeam_.SetDraw(TRUE);
      ugencard_.SetDraw(TRUE);
      ugbtn_ok_->SetDraw(TRUE);
      ugbtn_ok_->SetFont("しあいかいし");
      ugbtn_back_->SetFont("にげる！");
      ugbtn_next_->SetDraw(TRUE);//ならびかえ
      ugbtn_mid_->SetDraw(TRUE);//しゅーと
      ugbtn_mgk_->SetDraw(TRUE);//皆元気

      stEvent_.id_team_dr_ = ugdrTeam_.GetIDTeam();
      stEvent_.mid_team_dr_ = ugdrTeam_.GetMIDTeam();
      break;
    case db::pg_ShSetSelect:
      DrawList(&uglistShSetSelectAll_);
      uglistShSetSelect_.SetDraw(TRUE);
      uglistMember_.SetDraw(TRUE);
      ugbtn_back_->SetFont("もどる");
      ugbtn_back_->SetDraw(TRUE);
      break;
    case db::pg_Filter:
      DrawList(&uglistTmFilter_);
      ugdrTeam_.SetDraw(TRUE);
      ugbtn_back_->SetFont("もどる");
      ugbtn_back_->SetDraw(TRUE);
      break;
    case db::pg_MGK:
    {
      //DrawList(NULL);
      //SetFoot_Btn();
      frame_->ugheadder_->SetDisable();
      stEvent_.mgknum_ = pmgEO_->mgCoU_.GetSupplyNum_MGK();
      if (stEvent_.mgknum_ > 0)
      {
        std::string strnum = "もっているかず：";
        strnum += lib_str::IntToStr(stEvent_.mgknum_);
        STRVECTOR strvec = { "ミナゲンキーを＿しようします", "よろしいですか？", strnum };
        SetHudMsg(TRUE, FALSE, &strvec);
      }
      else
      {
        SetHudMsgStarSupply(SPL_MGK, TRUE);
      }
    }
    break;
    case db::pg_Nige:
    {
      //SetFoot_Btn();
      frame_->ugheadder_->SetDisable();
      STRVECTOR strvec = { "かんとくＨＰは＿しょうひされ", "れんしょうも＿とぎれます", "よろしいですか？" };
      SetHudMsg(TRUE, FALSE, &strvec);
    }
    break;
    case db::pg_GotoVS:
      pScene_->lyHmBar_->SetFilterAll(FALSE);//フィルタかける
      break;
    case db::pg_VS:
      SetEtcSS();
      pScene_->lyHmBar_->SetNoneBar(TRUE);
      break;
    case db::pg_timeAttackReady:
      DrawList(NULL);
      ugbtn_back_->SetDraw(FALSE);//戻るボタン消す
      break;
    case db::pg_timeAttackMatch:
      DrawList(NULL);
      //ugtitle_me_.SetMessage("あいてのプロフィール");
      //ugtitle_me_.SetDraw(TRUE);
      ugtitle_en_.SetDraw(TRUE);
      frame_->ugheadder_->SetDisable();//ヘッダ無効
      ugbtn_back_->SetDraw(TRUE);
      ugbtn_back_->SetFont("たいけつ！");
      ugbtn_next_->SetFont("ならびかえ");

      //-----------------------------------------------------------------------------------------------
      //敵チーム
      // ここを専用テーブルから引っ張ってくる
      {
        s64 pk = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(pmgEO_->stSeq_.GetMidEvent(), enmdm_Event::mdm_event_pk);
        s64 teamid = NGNUM;
        s32 bgmid = NGNUM;
        s32 courtid = NGNUM;
        for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_TimeAttack_.GetLineNum(); i++)
        {
          s32 tmpPk = pmgEO_->mgCoM_.mdm_TimeAttack_.GetLineDt(i, enmdm_TimeAttack::mdm_timeattack_id_m_event);
          if (tmpPk == pk)
          {
            teamid = pmgEO_->mgCoM_.mdm_TimeAttack_.GetLineDt(i, enmdm_TimeAttack::mdm_timeattack_id_m_team);
            bgmid = pmgEO_->mgCoM_.mdm_TimeAttack_.GetLineDt(i, enmdm_TimeAttack::mdm_timeattack_id_m_bgm);
            courtid = pmgEO_->mgCoM_.mdm_TimeAttack_.GetLineDt(i, enmdm_TimeAttack::mdm_timeattack_id_m_haikei);
            break;
          }
        }
		stEvent_.id_kantoku_en_ = NGNUM;	// 他プレイヤーではないのでNGNUM入れておく
        stEvent_.id_team_en_ = teamid;
        stEvent_.mid_team_en_ = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(stEvent_.id_team_en_, enmdm_Team::mdm_team_pk);
        stEvent_.mid_bgm_en_ = bgmid;
        stEvent_.mid_court_en_ = courtid;
      }
      //-----------------------------------------------------------------------------------------------

	  LoadTeamEnemy(stEvent_.id_kantoku_en_, stEvent_.mid_team_en_);
	  ugencard_.SetTeamDt3(stEvent_.id_kantoku_en_,stEvent_.id_team_en_, stEvent_.mid_team_en_, NGNUM, TRUE);
      ugencard_.SetTipsDraw_f(TRUE);
      ugencard_.SetDraw(TRUE);
      ugencard_.StaminaNone(TRUE);

      stEvent_.id_team_dr_ = ugdrTeam_.GetIDTeam();
      stEvent_.mid_team_dr_ = ugdrTeam_.GetMIDTeam();
      stEvent_.id_team_my_ = stEvent_.id_team_dr_;//自分のチーム
      //ugbtn_ok_->SetDraw(FALSE);//もう押せない
      LoadTeamTexture(SIDE0, pmgEO_->mgCoU_.mdTeam_.GetPKDt(stEvent_.id_team_my_, mdteam_id_m_team));

      //ugbtn_ok_->SetDraw(TRUE);
      //ugbtn_ok_->SetFont("たいけつ！");
      break;
      break;
    case db::pg_timeAttackNige:
    {
      //SetFoot_Btn();
      frame_->ugheadder_->SetDisable();
      STRVECTOR strvec = { "かんとくＨＰは＿しょうひされたままですが", "ほんとうに＿よろしいですか？" };
      SetHudMsg(TRUE, FALSE, &strvec);
    }
    break;
    case pg_reward:
        DrawList(NULL);
        frame_->ugheadder_->SetDisable();//ヘッダ無効
        ugbtn_ok_->SetFont("たいけつ！");
        ugNowReward_.SetDraw(TRUE);
        ugNextReward_.SetDraw(TRUE);
        ugLiRkReward_.SetDraw(TRUE);
        break;
    case pg_nowreward:
        ugbtn_ok_->SetFont("プレビュー");
        break;
    case pg_nextreward:
        ugbtn_ok_->SetFont("プレビュー");
        break;
    case db::pg_selectreward:
        ugbtn_ok_->SetFont("プレビュー");
        break;
    case pg_prevreward:
    {
        ugbtn_ok_->SetDraw(FALSE);
        SetHudDraw(FALSE);
        //backBtn_.SetDraw(TRUE);

        SOZAIDTVECTOR v;
        v.clear();
        TSozaiData sozai;
        // ここにシュートの素材ベースを決める処理を書く
        sozai.SetMidSozai(mid_sozai_);
        v.push_back(sozai);

        TPrevParam param;
        param.nonebackbtn_f = FALSE;
        param.nonemixbtn_f = FALSE;
        param.nonehavemixbtn_f = TRUE;
        param.sozaivec = &v;
        SetHudPrev(param);
    }
    break;
    case pg_TokkouInfo:
        DrawList(NULL);
        ugTokkouInfo_.Refresh(pmgEO_->stSeq_.GetMidEvent());
        ugTokkouInfo_.SetDraw(TRUE);
        break;
    case pg_TokkouBGM:
        DrawList(NULL);
        ugTokkouBGM_.SetDraw(TRUE);
        ugTokkouBGM_.Ready(pmgEO_->stSeq_.GetMidEvent());
        ugbtn_ok_->SetFont("ガチャでかう");
        ugbtn_ok_->SetDraw(FALSE);
        frame_->ugheadder_->SetDisable();//ヘッダ無効
        break;
    case pg_TokkouHaikei:
        DrawList(NULL);
        ugTokkouHaikei_.SetDraw(TRUE);
        ugTokkouHaikei_.Ready(pmgEO_->stSeq_.GetMidEvent());
        ugbtn_ok_->SetFont("ガチャでかう");
        ugbtn_ok_->SetDraw(FALSE);
        frame_->ugheadder_->SetDisable();//ヘッダ無効
        break;
    case db::pg_GotoShiai:
      pScene_->lyHmBar_->SetFilterAll(FALSE);
      pScene_->SetNextVSShiai();
      break;
    case db::pg_Error:
      //とりあえず切断
      DrawList(NULL);
      SetHudVSWaitEnd();
      break;
    }
  }

  //接続完了
  void TLyHmEvEvent::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_EventTourReady: Connect_TourReady(); break;
        case cn_EventTourPlay: Connect_TourPlay(); break;
        case cn_EventTourRetire: Connect_TourRetire(); break;
        case cn_Ranker: Connect_Ranker(); break;
        case cn_MyRank: Connect_MyRank(); break;
        case cn_MGK: Connect_MGK(); break;
        case cn_SwitchShSet: Connect_SwitchShSet(); break;
        case cn_TimeAttackReady: Connect_TimeAttackReady(); break;
        case cn_TimeAttackPlay: Connect_TimeAttackPlay(); break;
        case cn_TimeAttackRetire: Connect_TimeAttackRetire(); break;
        case cn_RewardRanker: Connect_RewardRanker(); break;
        }
      }
      else
      {
        switch (stLy_.connect_)
        {
        case cn_MGK:
        case cn_SwitchShSet:
        case cn_EventTourPlay:
        case cn_EventTourRetire:
        case cn_TimeAttackPlay:
        case cn_TimeAttackRetire:
          ChangePage(pg_TmSelect);
          break;
        default:
          ChangePage(pg_Menu);
          break;
        }
        //エラーメッセージ(ここでstLy_.connect_のクリアが入ってしまう)
        EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());

      }
    }
    else
    {
      switch (stLy_.connect_)
      {
      case cn_MGK:
      case cn_SwitchShSet:
      case cn_EventTourPlay:
      case cn_EventTourRetire:
      case cn_TimeAttackPlay:
      case cn_TimeAttackRetire:
        ChangePage(pg_TmSelect);
        break;
      default:
        ChangePage(pg_Menu);
        break;
      }
      ////通信エラー
      //SetHudEnd();
      //ChangePage(pg_Menu);

    }
  }


  void TLyHmEvEvent::DoUpdate(BOOL draw_f)
  {
    if (pScene_->lyHmBar_->IsBarAction())
    {
      stEvent_.turnbar_f_ = TRUE;
      return;
    }
    else
    {
      if (stEvent_.turnbar_f_)
      {
        stEvent_.turnbar_f_ = FALSE;
        RefreshTeamTexture();
      }
    }

    switch (stLy_.pageNo_)
    {
    case db::pg_Error:
      //なにもださない
      break;
    case db::pg_GotoShiai:
    case db::pg_VS:
      //Event画面
      ugvs_.Update(draw_f);
      for (s32 i = 0; i < ugvsteam_.size(); ++i) ugvsteam_[i]->Update(draw_f);
      break;
    case db::pg_Filter:
      ugdrTeam_.Update(draw_f);
      uglistTmFilter_.Update(draw_f);
      break;
    case db::pg_ShSetSelect:
      uglistShSetSelect_.Update(draw_f);
      uglistShSetSelectAll_.Update(draw_f);
      uglistMember_.Update(draw_f);
      break;
    case db::pg_reward:
    case db::pg_nowreward:
    case db::pg_nextreward:
    case db::pg_selectreward:
        ugNowReward_.Update(draw_f);
        ugNextReward_.Update(draw_f);
        ugLiRkReward_.Update(draw_f);
        ugRkReward_.Update(draw_f);
        break;
    case db::pg_TokkouInfo:
        ugTokkouInfo_.Update(draw_f);
        break;
    case db::pg_TokkouBGM:
        ugTokkouBGM_.Update(draw_f);
        break;
    case db::pg_TokkouHaikei:
        ugTokkouHaikei_.Update(draw_f);
        break;
    default:
      ugprof_.Update(draw_f);
      ugevbtn_.Update(draw_f);
      ugdrTeam_.Update(draw_f);
      uglistTmFilter_.Update(draw_f);
      ugTeam_.Update(draw_f);
      ugrmsg_ev_.Update(draw_f);
      uglistRankMenu_.Update(draw_f);
      uglistRanker_.Update(draw_f);
      uglistMyRank_.Update(draw_f);
      ugencard_.Update(draw_f);
      ugtitle_me_.Update(draw_f);
      ugtitle_en_.Update(draw_f);
      UpdateCursor(draw_f);
      break;
    }


    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case db::pg_Visit:MvPage_Visit(); break;
      case db::pg_Menu:MvPage_Menu(); break;
      case db::pg_Help:MvPage_Help(); break;
      case db::pg_Ranker:MvPage_Ranker(); break;
      case db::pg_MyRank:MvPage_MyRank(); break;
      case db::pg_tourReady:MvPage_TourReady(); break;
      case db::pg_Match:MvPage_Match(); break;
      case db::pg_TmSelect:MvPage_TmSelect(); break;
      case db::pg_ShSetSelect:MvPage_ShSetSelect(); break;
      case db::pg_Filter:MvPage_TmFilter(); break;
      case db::pg_MGK:MvPage_MGK(); break;
      case db::pg_Nige:MvPage_Nige(); break;
      case db::pg_GotoVS:MvPage_GotoVS(); break;
      case db::pg_VS:MvPage_VS(); break;
      case db::pg_GotoShiai:MvPage_GotoShiai(); break;
      case db::pg_timeAttackReady: MvPage_TimeAttackReady(); break;
      case db::pg_timeAttackMatch: MvPage_TimeAttackMatch(); break;
      case db::pg_timeAttackNige:MvPage_TimeAttackNige(); break;
      case db::pg_reward:MvPage_Reward(); break;
      case db::pg_nowreward:MvPage_NowReward(); break;
      case db::pg_nextreward:MvPage_NextReward(); break;
      case db::pg_selectreward:MvPage_SelectReward(); break;
      case db::pg_prevreward:MvPage_PreviewReward(); break;
      case db::pg_TokkouInfo:MvPage_TokkouInfo(); break;
      case db::pg_TokkouBGM:MvPage_TokkouBGM(); break;
      case db::pg_TokkouHaikei:MvPage_TokkouHaikei(); break;
      case db::pg_Out:MvPage_Out(); break;
      case db::pg_Error:MvPage_Error(); break;
      }
    }
  }


  void TLyHmEvEvent::DrawList(TUGLiGen* drawlist)
  {
    for (s32 i = 0; i < listvec_.size(); ++i)
    {
      listvec_[i]->SetDraw(FALSE);
    }
    if (drawlist != NULL)
    {
      drawlist->SetDraw(TRUE);
      //drawlist->SetZeroPos();
    }
    ugbtn_back_->SetDraw(TRUE);
    ugbtn_prev_->SetDraw(FALSE);//まえ
    ugbtn_next_->SetDraw(FALSE);//つぎ
    ugbtn_mid_->SetDraw(FALSE);//しゅーと
    ugbtn_mgk_->SetDraw(FALSE);//皆元気
    ugbtn_ok_->SetDraw(FALSE);
    ugprof_.SetDraw(FALSE);
    ugLiRkReward_.SetDraw(FALSE);
    ResetObj();
  }
  void TLyHmEvEvent::ResetObj()
  {
    ugevbtn_.SetDraw(FALSE);
    ugdrTeam_.SetDraw(FALSE);
    ugTeam_.SetDraw(FALSE);
    ugencard_.SetDraw(FALSE);
    ugrmsg_ev_.SetDraw(FALSE);
    ugNowReward_.SetDraw(FALSE);
    ugNextReward_.SetDraw(FALSE);
    ugRkReward_.SetDraw(FALSE);
    ugTokkouBGM_.SetDraw(FALSE);
    ugTokkouInfo_.SetDraw(FALSE);
    ugTokkouHaikei_.SetDraw(FALSE);
  }

  //敵チームカード＆情報
  void TLyHmEvEvent::LoadTeamEnemy(s64 id_kantoku, s32 mid_team)
  {
    LoadTeamTexture(SIDE1, mid_team);
    ugencard_.SetTeamDt3(id_kantoku, NGNUM, mid_team, NGNUM, TRUE);
    ugencard_.SetTipsDraw_f(TRUE);
  }

  //テクスチャロード
  void TLyHmEvEvent::LoadTeamTexture(s32 sideno, s32 mid_team)
  {
    if (mid_team <= 0) return;

    BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_gb_f);
    for (s32 c = 0; c < DBMEMBER_ALL; ++c)
    {
      s32 charid = pmgEO_->mgCoM_.GetCharID(mid_team, c);
      pmgEO_->mgGrp_.pmgChGrp_[sideno][c]->SetCharDt(charid, mid_team, gb_f);
      pmgEO_->mgGrp_.pmgChGrp_[sideno][c]->SetGrTile(SIDE1, c);
    }
    pmgEO_->mgGrp_.MakeIconTexture_Chest(sideno, FALSE, FALSE);
    pmgEO_->mgGrp_.MakeIconTexture_Body(sideno, 0, 0, FALSE, FALSE);

  }

  //ヘッダにいって返ってきたときのリフレッシュ
  void TLyHmEvEvent::RefreshTeamTexture()
  {
    if ((stLy_.pageNo_ == pg_TmSelect)
      || (stLy_.pageNo_ == pg_Filter))
    {
      LoadTeamTexture(SIDE0, pmgEO_->mgCoU_.mdTeam_.GetPKDt(ugdrTeam_.GetIDTeam(), mdteam_id_m_team));
      LoadTeamTexture(SIDE1, stEvent_.mid_team_en_);
    }
  }

  //両者情報セット
  void TLyHmEvEvent::SetEtcSS()
  {
    pmgEO_->stResult_.Init();
    pmgEO_->stDrop_.Init();
    pmgEO_->stSeq_.ReadyEvent();

    s32 eventtype = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(pmgEO_->stSeq_.GetMidEvent(), mdm_event_eventType);
    if (eventtype == evt_TimeAttack)
    {
      s64 pk = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(pmgEO_->stSeq_.GetMidEvent(), enmdm_Event::mdm_event_pk);
      s64 shiaisec = NGNUM;
#ifndef __K_MAKE_HIDE_TIME_ATTACK_ENEMY_CATCH__
	  s32 caper = pmgEO_->mgCoM_.mdm_Client_.GetLineDt(0, mdm_client_caper);
	  s32 ncaper = pmgEO_->mgCoM_.mdm_Client_.GetLineDt(0, mdm_client_ncaper);
#endif
      //ドロップアイテム
      s32 itemType = 0;
      s32 itemID = 0;
      s32 itemNum = 0;
      for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_TimeAttack_.GetLineNum(); i++)
      {
        s32 tmpPk = pmgEO_->mgCoM_.mdm_TimeAttack_.GetLineDt(i, enmdm_TimeAttack::mdm_timeattack_id_m_event);
        if (tmpPk == pk)
        {
          shiaisec = pmgEO_->mgCoM_.mdm_TimeAttack_.GetLineDt(i, enmdm_TimeAttack::mdm_timeattack_shiaiSec);
#ifndef __K_MAKE_HIDE_TIME_ATTACK_ENEMY_CATCH__
		  caper = pmgEO_->mgCoM_.mdm_TimeAttack_.GetLineDt(i, enmdm_TimeAttack::mdm_timeattack_catch);
		  ncaper = pmgEO_->mgCoM_.mdm_TimeAttack_.GetLineDt(i, enmdm_TimeAttack::mdm_timeattack_nice);
#endif
          itemType = pmgEO_->mgCoM_.mdm_TimeAttack_.GetLineDt(i, enmdm_TimeAttack::mdm_timeattack_itemType);
          itemID = pmgEO_->mgCoM_.mdm_TimeAttack_.GetLineDt(i, enmdm_TimeAttack::mdm_timeattack_itemID);
          itemNum = pmgEO_->mgCoM_.mdm_TimeAttack_.GetLineDt(i, enmdm_TimeAttack::mdm_timeattack_itemNum);
          break;
        }
      }

#ifndef __K_MAKE_HIDE_EVENT_SOUND__
	  s32 mid_bgm_ready = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(pmgEO_->stSeq_.GetMidEvent(), mdm_event_id_m_bgm_ready);
	  s32 mid_bgm_win = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(pmgEO_->stSeq_.GetMidEvent(), mdm_event_id_m_bgm_win);
	  s32 mid_bgm_lose = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(pmgEO_->stSeq_.GetMidEvent(), mdm_event_id_m_bgm_lose);
	  if (mid_bgm_ready == 0) { mid_bgm_ready = nsf_jgl_ready; }
	  if (mid_bgm_win == 0) { mid_bgm_win = nsf_jgl_win; }
	  if (mid_bgm_lose == 0) { mid_bgm_lose = nsf_jgl_lose; }
#endif

      //特効倍率
      s32 tokkoumag = pmgEO_->mgCoM_.GetTokkouMag(stEvent_.id_team_my_, pmgEO_->stSeq_.GetMidEvent());

      pmgEO_->stShiai_.SetTimeAttack(
		pmgEO_->mgCoU_.mdKantoku_.GetMyDt(enmdKantoku::mdkantoku_pk),
		NGNUM,	// タイムアタックなので相手監督IDはNGNUM
        stEvent_.id_team_my_,
        stEvent_.id_team_en_,
        pmgEO_->mgCoU_.mdTeam_.GetPKDt(stEvent_.id_team_my_, mdteam_id_m_team),
        stEvent_.mid_team_en_,
        stEvent_.mid_court_en_,
        stEvent_.mid_bgm_en_,
#ifndef __K_MAKE_HIDE_EVENT_SOUND__
		mid_bgm_ready,
		mid_bgm_win,
		mid_bgm_lose,
		//nsf_jgl_ready,
		//nsf_jgl_win,
		//nsf_jgl_lose,
#endif
        tokkoumag,
        pmgEO_->mgCoU_.GetCtrl(),
        shiaisec,
#ifndef __K_MAKE_HIDE_TIME_ATTACK_ENEMY_CATCH__
        caper,
        ncaper
#else
		pmgEO_->mgCoM_.mdm_Client_.GetLineDt(0, mdm_client_caper),
		pmgEO_->mgCoM_.mdm_Client_.GetLineDt(0, mdm_client_ncaper)
#endif
        );

      pmgEO_->stDrop_.SetEventShiai(TRUE, itemType, itemID, itemNum);


      // ここでイベントフラグを立てておく
      pmgEO_->stSeq_.SetEventTimeAttack(TRUE);
    }
    else
    {
#ifndef __K_MAKE_HIDE_EVENT_SOUND__
		s32 mid_bgm_ready = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(pmgEO_->stSeq_.GetMidEvent(), mdm_event_id_m_bgm_ready);
		s32 mid_bgm_win = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(pmgEO_->stSeq_.GetMidEvent(), mdm_event_id_m_bgm_win);
		s32 mid_bgm_lose = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(pmgEO_->stSeq_.GetMidEvent(), mdm_event_id_m_bgm_lose);
		if (mid_bgm_ready == 0) { mid_bgm_ready = nsf_jgl_ready; }
		if (mid_bgm_win == 0) { mid_bgm_win = nsf_jgl_win; }
		if (mid_bgm_lose == 0) { mid_bgm_lose = nsf_jgl_lose; }
#endif
      pmgEO_->stShiai_.SetTour(
		pmgEO_->mgCoU_.mdKantoku_.GetMyDt(enmdKantoku::mdkantoku_pk),
		stEvent_.id_kantoku_en_,
        stEvent_.id_team_my_,
        stEvent_.id_team_en_,
        pmgEO_->mgCoU_.mdTeam_.GetPKDt(stEvent_.id_team_my_, mdteam_id_m_team),
        stEvent_.mid_team_en_,
        stEvent_.mid_court_en_,
        stEvent_.mid_bgm_en_,
#ifndef __K_MAKE_HIDE_EVENT_SOUND__
		mid_bgm_ready,
		mid_bgm_win,
		mid_bgm_lose,
		//nsf_jgl_ready,
		//nsf_jgl_win,
		//nsf_jgl_lose,
#endif
        pmgEO_->mgCoU_.GetCtrl(),
        599,
        pmgEO_->mgCoM_.mdm_Client_.GetLineDt(0, mdm_client_caper),
        pmgEO_->mgCoM_.mdm_Client_.GetLineDt(0, mdm_client_ncaper)
        );
    }

    for (s32 i = 0; i < ugvsteam_.size(); ++i) ugvsteam_[i]->Refresh();
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      s32 mid_team = pmgEO_->stShiai_.GetMidTeam(i);
      BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_gb_f);
      for (s32 c = 0; c < DBMEMBER_ALL; ++c)
      {
        s32 charid = pmgEO_->mgCoM_.GetCharID(mid_team, c);
        pmgEO_->mgGrp_.pmgChGrp_[i][c]->SetCharDt(charid, mid_team, gb_f);
        pmgEO_->mgGrp_.pmgChGrp_[i][c]->SetGrTile(i, c);
      }
      pmgEO_->mgGrp_.MakeIconTexture_Body(i, 1, 1, FALSE, FALSE);

	  if(eventtype == evt_Tour)
	  {
		  // ここでイベントフラグを立てておく
		  pmgEO_->stSeq_.SetEventTour(TRUE);
	  }
    }
  }

  void TLyHmEvEvent::MvPage_Visit()
  {
    ChangePage(pg_Menu);
  }
  //対戦メニュー
  void TLyHmEvEvent::MvPage_Menu()
  {
    if (ugbtn_back_->IsAct())
    {
      Down();
    }
    else if (ugevbtn_.IsAct())
    {
      s32 eventtype = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(pmgEO_->stSeq_.GetMidEvent(), mdm_event_eventType);

      switch (eventtype)
      {
      case evt_Map://マップ形式イベント
        pScene_->lyHmBar_->SetFilterAll(FALSE);
        pScene_->SetNextScenario();
#ifndef __K_MAKE_HIDE_EVENT_SOUND__
		pmgEO_->stSeq_.SetMapMidBGM(pmgEO_->mgCoM_.mdm_Event_.GetPKDt(pmgEO_->stSeq_.GetMidEvent(), mdm_event_id_m_bgm_map));
#endif
        break;
      case evt_Tour://疑似対人形式イベント
      {
        STRVECTOR msg{ "かんとくＨＰが＿負２０＿されます", "かちぬきドッジボールを＿はじめますか？" };
        SetHudMsg(TRUE, FALSE, &msg);
        ChangePage(pg_tourReady);
      }
      break;
      case evt_VSHum://対人イベント
      {
        STRVECTOR msg{ "かんとくＨＰが＿負２０＿されます", "つうしんイベントドッジを＿はじめますか？" };
        SetHudMsg(TRUE, FALSE, &msg);
        //ChangePage();
      }
      break;
      case evt_TimeAttack:
      {
        // ここで今回選んだイベント
        s64 pk = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(pmgEO_->stSeq_.GetMidEvent(), enmdm_Event::mdm_event_pk);
        s32 needHP = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(pmgEO_->stSeq_.GetMidEvent(), enmdm_Event::mdm_event_staminaMin);
        STRVECTOR msg{ "かんとくＨＰが＿負" + lib_str::IntToStr(needHP) + "＿されます", "タイムアタックを＿はじめますか？" };
        SetHudMsg(TRUE, FALSE, &msg);
        ChangePage(pg_timeAttackReady);
      }
      break;
      }
    }
    else if (uglistRankMenu_.IsAct())
    {
      if (uglistRankMenu_.GetActIndex() < uglistRankMenu_.records_.size())
      {
        stEvent_.mid_ranking_ = uglistRankMenu_.GetCommonID();
        stEvent_.rankingpage_ = 0;
        Post_Ranker();
      }
      else
      {
          enum HelpCategory
          {
              hc_REWARD_ID = 1,
              hc_TOKKOU_INFO,
          };
          s64 category = uglistRankMenu_.GetHelpCategory();

#ifdef __K_DEBUG_ROOT__
          if (category != hc_REWARD_ID)
          {
              category = hc_TOKKOU_INFO;
          }
#endif
          switch (category)
          {
          //-----------------------------------------------
          // 報酬ボタン
          case hc_REWARD_ID:
              {
                  stEvent_.mid_ranking_ = mid_Ranking_;
                  //ChangePage(pg_reward);
                  Post_RewardRanker();
                  return;
              }
              break;
          //-----------------------------------------------
          // 特効詳細ボタン
          case hc_TOKKOU_INFO:
              {
                  ChangePage(pg_TokkouInfo);
                  return;
              }
              break;
          }

        stEvent_.helpType_ = uglistRankMenu_.GetCommonID();
        stEvent_.helpNo_ = (uglistRankMenu_.GetActIndex() - uglistRankMenu_.records_.size());
        ChangePage(pg_Help);
      }


      ////ランカー表示接続できたらページ移動
      //ChangePage(pg_Ranker);
    }
  }
  //ヘルプ
  void TLyHmEvEvent::MvPage_Help()
  {
    if (mid::midIsDrawModalPanel() == FALSE)
    {
      ChangePage(pg_Menu);
    }
  }
  //ランキング関係
  void TLyHmEvEvent::MvPage_Ranker()
  {
    if (ugbtn_back_->IsAct())
    {
      //自分に表示を戻す
      //RefreshProf();
      //ランクメニューに戻る
      ChangePage(pg_Menu);
    }
    else if (uglistRanker_.IsAct())
    {
      //一行目が押されたら自分付近ランク表示、それ以外は上位ランク者表示
      if ((stEvent_.rankingpage_ == 0) && (uglistRanker_.GetActIndex() == 0))
      {
        Post_MyRank();
      }
      else if (uglistRanker_.GetCommonID() != NGNUM)
      {
        //表示書き換え//変数更新（テクスチャ読み込み）
        ugprof_.SetProfRanker(uglistRanker_.GetCommonID());
      }
    }
    else if (ugbtn_prev_->IsAct() && (stEvent_.rankingpage_ > 0))
    {
      --stEvent_.rankingpage_;
      if (stEvent_.rankingpage_ == 0)
      {
        Post_Ranker();
      }
      else
      {
        Post_RankerPage();
      }
    }
    else if ((ugbtn_next_->IsAct() && (stEvent_.rankingpage_ < 9)))
    {
      ++stEvent_.rankingpage_;
      Post_RankerPage();
    }

  }
  void TLyHmEvEvent::SwitchRankerPageBtn()
  {
    ugbtn_prev_->SetFont("まえのページ");
    ugbtn_next_->SetFont("つぎのページ");
    ugbtn_prev_->SetDraw((stEvent_.rankingpage_ > 0));
    ugbtn_next_->SetDraw((stEvent_.rankingpage_ < 9));
  }
  void TLyHmEvEvent::MvPage_MyRank()
  {
    if (ugbtn_back_->IsAct())
    {
      //ランカー表示
      Post_Ranker();
    }
    else if (uglistMyRank_.IsAct())
    {
      //表示書き換え//変数更新（テクスチャ読み込み）
      ugprof_.SetProfRanker(uglistMyRank_.GetCommonID());
    }
  }

  //疑似対人レディ  
  void TLyHmEvEvent::MvPage_TourReady()
  {
    if (IsHudOK())//おーけー
    {
      Post_TourReady();
    }
    else if (IsHudCancel())//きゃんせる
    {
      ChangePage(pg_Menu);
    }
  }
  //疑似対人相手表示
  void TLyHmEvEvent::MvPage_Match()
  {
    //ugprof_.SetDraw(TRUE);
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_TmSelect);
    }
  }
  //疑似対人チームセレクト
  void TLyHmEvEvent::MvPage_TmSelect()
  {
    UpdateCheckStamina();

    s32 eventtype = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(pmgEO_->stSeq_.GetMidEvent(), mdm_event_eventType);

    //ドラムアクション
    if (ugbtn_back_->IsAct())
    {
      if (eventtype == evt_TimeAttack)
      {
        ChangePage(pg_timeAttackNige);
      }
      else
      {
        ChangePage(pg_Nige);
      }
    }
    else if (ugbtn_ok_->IsAct())
    {

      // スタミナ不足
      if (stEvent_.isCheckStamina_)
      {
        stEvent_.cursorDispTimer_ = 60;
        pCursor_->g_SetDraw(TRUE);
        return;
      }

      stEvent_.id_team_dr_ = ugdrTeam_.GetIDTeam();
      stEvent_.mid_team_dr_ = ugdrTeam_.GetMIDTeam();
      stEvent_.id_team_my_ = stEvent_.id_team_dr_;//自分のチーム
      //ugbtn_ok_->SetDraw(FALSE);//もう押せない

      s64 mid_team = pmgEO_->mgCoU_.mdTeam_.GetPKDt(stEvent_.id_team_my_, mdteam_id_m_team);
      LoadTeamTexture(SIDE0, mid_team);

	  s64 id_kantoku = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(enmdKantoku::mdkantoku_pk);
      ugTeam_.SetTeamDt3(id_kantoku, stEvent_.id_team_my_, mid_team, NGNUM, TRUE);
      ugTeam_.SetTipsDraw_f(TRUE);
      ugTeam_.SetDraw(TRUE);

      ugdrTeam_.SetDraw(FALSE);

      if (eventtype == evt_TimeAttack)
      {
        //ChangePage(pg_GotoVS);
        Post_TimeAttackPlay();
      }
      else
      {
        Post_TourPlay();
      }
    }
    else if (ugbtn_next_->IsAct())
    {
      ChangePage(pg_Filter);
    }
    else if (ugbtn_mid_->IsAct())
    {
      ChangePage(pg_ShSetSelect);
      uglistShSetSelectAll_.Refresh(stEvent_.id_team_dr_, NGNUM, shsetvec_);
      uglistMember_.Refresh(stEvent_.id_team_dr_, stEvent_.mid_team_dr_, FALSE);
      stEvent_.shsetmem_f_ = FALSE;
      stEvent_.id_shset_first_ = uglistShSetSelectAll_.GetShSet();
    }
    else if (ugbtn_mgk_->IsAct())//ＭＧＫボタン
    {
      ChangePage(pg_MGK);
    }
    //else if (ugdrTeam_.IsActRes(enUGDrTeamAct_Roll))//ドラム回転＆カウンタ処理
    //{
    //  //ugdrTeam_.RefreshRoll(FALSE);
    //}
    else if (ugdrTeam_.IsActRes(enUGDrTeamAct_Stop))//読み込みが必要なタイミング
    {
      //現在のチームのＩＤ，MID
      stEvent_.id_team_dr_ = ugdrTeam_.GetIDTeam();
      stEvent_.mid_team_dr_ = ugdrTeam_.GetMIDTeam();
      //キャプテンとメンバーの画像をロード
      //LoadLookPosTeam();
    }
  }
  //スタミナチェック
  void TLyHmEvEvent::UpdateCheckStamina(void)
  {
    ugdrTeam_.stDrGen_.recno_;
    s64 id_team = ugdrTeam_.GetIDTeam();
    s64 mid_team = ugdrTeam_.GetMIDTeam();

    if (id_team == NGNUM) { return; }				//敵
    /*
    if( id_prev_team_ == id_team ) { return; }		// 前回と同じ
    if (mid_prev_team_ == mid_team) { return; }		// なので更新の必要なし
    */

    const s32 MINTOSEC = 60;
    //const s32 MAXSTA = 100;

    s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_rarity);
    s32 waittime = pmgEO_->mgCoM_.mdm_Rarity_.GetLineDt(rarity, mdm_rarity_staminaLim);

    s64 nowwait = waittime;

    s64 nowtime = mid::midNowTime();
    s64 endtime = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_useSec);
    nowwait = (nowtime - endtime) / MINTOSEC;
    if (nowwait > waittime) nowwait = waittime;

    // 満タンじゃない
    BOOL mgk_f = (nowwait != waittime);
    if (mgk_f)
    {
      if (!stEvent_.isCheckStamina_)
      {
        stEvent_.isCheckStamina_ = TRUE;
        if (stEvent_.isMGKBtn_ == FALSE)
        {
          ugbtn_ok_->SetFont("ＳＴかいふく");
          stEvent_.isMGKBtn_ = TRUE;
        }
      }
    }
    else
    {
      if (stEvent_.isCheckStamina_)
      {
        stEvent_.isCheckStamina_ = FALSE;
        if (stEvent_.isMGKBtn_)
        {
          ugbtn_ok_->SetFont("しあいかいし");
          stEvent_.isMGKBtn_ = FALSE;
        }
      }
    }
  }


  //シュートセット切り替え
  void TLyHmEvEvent::MvPage_ShSetSelect()
  {

    if (stEvent_.shsetmem_f_)//個別
    {

      if (ugbtn_back_->IsAct())//おーけー
      {
        if (stEvent_.id_shset_first_ != uglistShSetSelect_.GetShSet())
        {
          stEvent_.id_shset_ = uglistShSetSelect_.GetShSet();
          Post_SwitchShSet();
        }
        else
        {
          ChangePage(pg_TmSelect);
        }
      }
      else if (uglistMember_.IsAct())//個別に
      {
        stEvent_.id_shset_ = uglistShSetSelect_.GetShSet();
        if (uglistMember_.IsSelected())
        {
          uglistShSetSelect_.Refresh(stEvent_.id_team_dr_, stEvent_.id_shset_, uglistMember_.GetActIndex(), shsetvec_);
        }
        else
        {
          uglistShSetSelectAll_.Refresh(stEvent_.id_team_dr_, stEvent_.id_shset_, shsetvec_);//全体に戻る
        }
      }
    }
    else//全体
    {
      if (ugbtn_back_->IsAct())//おーけー
      {
        if (stEvent_.id_shset_first_ != uglistShSetSelectAll_.GetShSet())
        {
          stEvent_.id_shset_ = uglistShSetSelectAll_.GetShSet();
          Post_SwitchShSet();
        }
        else
        {
          ChangePage(pg_TmSelect);
        }
      }
      else if (uglistMember_.IsAct())//個別に
      {
        stEvent_.id_shset_ = uglistShSetSelectAll_.GetShSet();
        uglistShSetSelect_.Refresh(stEvent_.id_team_dr_, stEvent_.id_shset_, uglistMember_.GetActIndex(), shsetvec_);
      }
    }

    stEvent_.shsetmem_f_ = uglistMember_.IsSelected();

    uglistShSetSelectAll_.SetDraw(!uglistMember_.IsSelected());
    uglistShSetSelect_.SetDraw(uglistMember_.IsSelected());

  }

  //チームフィルター
  void TLyHmEvEvent::MvPage_TmFilter()
  {
    if (ugbtn_back_->IsAct())//おーけー
    {
      ChangePage(pg_TmSelect);
    }
    else if (uglistTmFilter_.IsActRes2(enUGLiTmFilter_Filter))
    {
      ugdrTeam_.ReadyFilter();
    }
    else if (uglistTmFilter_.IsActRes2(enUGLiTmFilter_Sort))
    {
      ugdrTeam_.ReadySort();
    }
  }
  //みなげんき
  void TLyHmEvEvent::MvPage_MGK()
  {
    if (IsHudOK())//おーけー
    {
      //POST：合成ＮＧ
      Post_MGK();
    }
    else if (IsHudCancel())//きゃんせる
    {
      ChangePage(pg_TmSelect);
    }
  }
  //にげる
  void TLyHmEvEvent::MvPage_Nige()
  {
    if (IsHudOK())//おーけー
    {
      Post_TourRetire();
    }
    else if (IsHudCancel())//きゃんせる
    {
      ChangePage(pg_TmSelect);
    }
  }
  //ＶＳへ
  void TLyHmEvEvent::MvPage_GotoVS()
  {
  }
  //ＶＳ
  void TLyHmEvEvent::MvPage_VS()
  {
    if (field_->sc_IsCamEnter()) //さわったら即
    {
      ChangePage(pg_GotoShiai);
    }
  }
  //タイムアタックのレディ  
  void TLyHmEvEvent::MvPage_TimeAttackReady(void)
  {
    if (IsHudOK())//おーけー
    {
      //ChangePage(pg_timeAttackMatch);
      Post_TimeAttackReady();
    }
    else if (IsHudCancel())//きゃんせる
    {
      ChangePage(pg_Menu);
    }
  }

  // タイムアタックのマッチ
  void TLyHmEvEvent::MvPage_TimeAttackMatch(void)
  {
    //if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_TmSelect);
    }
  }
  //タイムアタックのにげる
  void TLyHmEvEvent::MvPage_TimeAttackNige()
  {
    if (IsHudOK())//おーけー
    {
      Post_TimeAttackRetire();
    }
    else if (IsHudCancel())//きゃんせる
    {
      ChangePage(pg_TmSelect);
    }
  }

  void TLyHmEvEvent::MvPage_Reward(void)
  {
    if (ugbtn_back_->IsAct())
    {
        ChangePage(pg_Menu);
    }
    else if (ugNowReward_.IsAct())
    {
        // 報酬をアイテムビューワーにセット
        s32 rank = nowRank_;
        SetHudItemView_Reward(stEvent_.mid_ranking_, rank, FALSE, FALSE, FALSE);

        // フェードセット
        ugNowReward_.SetFade();

        ChangePage(pg_nowreward);
    }
    else if (ugNextReward_.IsAct())
    {
        // 報酬をアイテムビューワーにセット
        s32 rank = nextRewardRank_;
        SetHudItemView_Reward(stEvent_.mid_ranking_, rank, FALSE, FALSE, FALSE);

        // フェードセット
        ugNextReward_.SetFade();

        ChangePage(pg_nextreward);
    }
    else if (ugLiRkReward_.IsAct())
    {
        s64 pk = ugLiRkReward_.GetCommonID();
        s32 top = pmgEO_->mgCoM_.mdm_RkReward_.GetPKDt(pk, enmdm_RkReward::mdm_rkreward_top);
        stEvent_.mid_ranking_ = pmgEO_->mgCoM_.mdm_RkReward_.GetPKDt(pk, enmdm_RkReward::mdm_rkreward_id_m_ranking);
        SetHudItemView_Reward(stEvent_.mid_ranking_, top, FALSE, FALSE, FALSE);

        // フェードセット
        ugRkReward_.SetFade();

        ChangePage(pg_selectreward);
    }
  }

  //--------------------------------------------------------------------------------------------
  // 
  //--------------------------------------------------------------------------------------------
  void TLyHmEvEvent::MvPage_NowReward()
  {
      if (IsHudItemView())
      {
          ugNowReward_.OutFade();
          ChangePage(pg_reward);
      }

      // 選択したものが素材
      if (GetHudItemView().type_ == enItemType_Sozai)
      {
          ugbtn_ok_->SetDraw(TRUE);
      }
      else
      {
          ugbtn_ok_->SetDraw(FALSE);
      }

      if (ugbtn_ok_->IsAct())
      {
          ugNowReward_.SetFadeHide(FALSE);

          prevPage_ = pg_nowreward;
          mid_sozai_ = GetHudItemView().id_;
          ChangePage(pg_prevreward);
      }
  }
  //--------------------------------------------------------------------------------------------
  // 
  //--------------------------------------------------------------------------------------------
  void TLyHmEvEvent::MvPage_NextReward()
  {
      if (IsHudItemView())
      {
          ugNextReward_.OutFade();
          ChangePage(pg_reward);
      }

      // 選択したものが素材
      if (GetHudItemView().type_ == enItemType_Sozai)
      {
          ugbtn_ok_->SetDraw(TRUE);
      }
      else
      {
          ugbtn_ok_->SetDraw(FALSE);
      }

      if (ugbtn_ok_->IsAct())
      {
          ugNextReward_.SetFadeHide(FALSE);

          prevPage_ = pg_nextreward;
          mid_sozai_ = GetHudItemView().id_;
          ChangePage(pg_prevreward);
      }
  }
  //--------------------------------------------------------------------------------------------
  // 
  //--------------------------------------------------------------------------------------------
  void TLyHmEvEvent::MvPage_SelectReward()
  {
      if (IsHudItemView())
      {
          ugRkReward_.OutFade();
          ChangePage(pg_reward);
      }

      // 選択したものが素材
      if (GetHudItemView().type_ == enItemType_Sozai)
      {
          ugbtn_ok_->SetDraw(TRUE);
      }
      else
      {
          ugbtn_ok_->SetDraw(FALSE);
      }

      if (ugbtn_ok_->IsAct())
      {
          ugNextReward_.SetFadeHide(FALSE);

          prevPage_ = pg_selectreward;
          mid_sozai_ = GetHudItemView().id_;
          ChangePage(pg_prevreward);
      }
  }
  //--------------------------------------------------------------------------------------------
  // 
  //--------------------------------------------------------------------------------------------
  void TLyHmEvEvent::MvPage_PreviewReward(void)
  {
      //if (backBtn_.IsAct())
      if (IsHudPrevBack())
      {
          SetPrevEnd();
          ugbtn_back_->SetDraw(FALSE);

          if (prevPage_ == pg_nowreward)
          {
              ugNowReward_.SetFadeHide(TRUE);
          }
          else if (prevPage_ == pg_nextreward)
          {
              ugNextReward_.SetFadeHide(TRUE);
          }
          else if (prevPage_ == pg_selectreward)
          {
              ugRkReward_.SetFadeHide(TRUE);
          }

          SetHudDraw(TRUE);

          ChangePage(prevPage_);
          prevPage_ = NGNUM;
      }
      else if (IsHudPrevMix())//ランダムボタン
      {
          SetHudPrevMixSozai(mid_sozai_, TRUE);
      }
      else if (IsHudPrevMixClear())
      {
          SetHudPrevMixSozai(mid_sozai_, FALSE);

      }
  }
  //--------------------------------------------------------------------------------------------
  // 
  //--------------------------------------------------------------------------------------------
  void TLyHmEvEvent::MvPage_TokkouInfo(void)
  {
      if (ugbtn_back_->IsAct())
      {
      //戻る前に画像を読み直す
      ugevbtn_.SetEvent(pmgEO_->stSeq_.GetMidEvent());
          ChangePage(pg_Menu);
          return;
      }
      else if (ugTokkouInfo_.IsAct())
      {
          if (ugTokkouInfo_.GetActRes() == TIA_Select_BGM)
          {
              ChangePage(pg_TokkouBGM);
          }
          else if (ugTokkouInfo_.GetActRes() == TIA_Select_Haikei)
          {
              ChangePage(pg_TokkouHaikei);
          }
      }
  }

  //--------------------------------------------------------------------------------------------
  // 
  //--------------------------------------------------------------------------------------------
  void TLyHmEvEvent::MvPage_TokkouBGM(void)
  {
      s64 mid_bgm = ugTokkouBGM_.GetBgmID();

      if (ugbtn_back_->IsAct())
      {
          ugTokkouBGM_.Down();
          frame_->ugheadder_->SetEnable();
          ugbtn_ok_->SetDraw(FALSE);
          ChangePage(pg_TokkouInfo);
          return;
      }
      else if (ugTokkouBGM_.IsAct())
      {

      }
      else if (ugbtn_ok_->IsAct() && (mid_bgm != NGNUM))//おーけー
      {
          // 持ってない
          if (!pmgEO_->mgCoU_.IsBGM(mid_bgm))
          {
              // ガチャ屋にとばす
              Down();

              INTVECTOR v;
              v.push_back(footer_id_shop);	// ショップタブ
              v.push_back(2);	// ガチャショップ
              v.push_back(2); // なつかしBOX
              s32 mid_nabox = pmgEO_->mgDt_.dtBGM_.GetDtLine(mid_bgm, enDtBGM_mid_nabox);
              v.push_back(mid_nabox);
              pmgEO_->stJump_.SetJumpAreaData(v);
              pmgEO_->stJump_.SetFromTab(footer_id_team);
              pScene_->lyHmBar_->JumpFooter(v[0]);
              return;
          }
      }
      
      // 持ってない
      if (mid_bgm != NGNUM && !pmgEO_->mgCoU_.IsBGM(mid_bgm))
      {
          ugbtn_ok_->SetFont("ガチャでかう");
          ugbtn_ok_->SetDraw(TRUE);
      }
      else
      {
          ugbtn_ok_->SetDraw(FALSE);
      }
  }

  //--------------------------------------------------------------------------------------------
  // 
  //--------------------------------------------------------------------------------------------
  void TLyHmEvEvent::MvPage_TokkouHaikei(void)
  {
      s64 mid_haikei = ugTokkouHaikei_.GetHaikeiID();

      if (ugbtn_back_->IsAct())
      {
          ugTokkouHaikei_.Down();
          frame_->ugheadder_->SetEnable();
          ugbtn_ok_->SetDraw(FALSE);
          ChangePage(pg_TokkouInfo);
          return;
      }
      else if (ugTokkouHaikei_.IsAct())
      {

      }
      else if (ugbtn_ok_->IsAct() && (mid_haikei != NGNUM))//おーけー
      {
          // 持ってない
          if (!pmgEO_->mgCoU_.IsHaikei(mid_haikei))
          {
              // ガチャ屋にとばす
              Down();

              // ここに飛び先データを取ってきて入れる
              INTVECTOR v;
              v.push_back(footer_id_shop);	// ショップタブ
              v.push_back(2);	// ガチャショップ
              v.push_back(2); // なつかしBOX
              s32 mid_nabox = pmgEO_->mgDt_.dtCourt_.GetDtLine(mid_haikei, enDtCourt_mid_nabox);
              v.push_back(mid_nabox);
              pmgEO_->stJump_.SetJumpAreaData(v);
              pmgEO_->stJump_.SetFromTab(footer_id_team);
              pScene_->lyHmBar_->JumpFooter(v[0]);
              return;
          }
      }

      // 持ってない
      if (mid_haikei != NGNUM && !pmgEO_->mgCoU_.IsHaikei(mid_haikei))
      {
          ugbtn_ok_->SetFont("ガチャでかう");
          ugbtn_ok_->SetDraw(TRUE);
      }
      else
      {
          ugbtn_ok_->SetDraw(FALSE);
      }
  }

  //退出
  void TLyHmEvEvent::MvPage_Out()
  {
    Down();
  }
  //試合へ
  void TLyHmEvEvent::MvPage_GotoShiai()
  {
  }
  //通信エラー
  void TLyHmEvEvent::MvPage_Error()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Menu);
    }
  }


  //POSTEvent敵情報取得
  void TLyHmEvEvent::Post_TourReady()
  {
    StartConnect(cn_EventTourReady);

    pmgEO_->mgPo_.poHmShiai_.Ready_tourready(pmgEO_->stSeq_.GetMidEvent());
  }
  //POSTEvent選択チーム
  void TLyHmEvEvent::Post_TourPlay()
  {
    StartConnect(cn_EventTourPlay);
    pmgEO_->mgPo_.poHmShiai_.Ready_tourplay(stEvent_.id_team_my_);
    //POSTEvent結果
  }
  //POSTリタイヤ（にげる）
  void TLyHmEvEvent::Post_TourRetire()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_TourRetire();
    }
    else
    {
      StartConnect(cn_EventTourRetire);
      pmgEO_->mgPo_.poHmShiai_.Ready_tourretire();
    }
  }
  //POST上位ランカー
  void TLyHmEvEvent::Post_Ranker()
  {
    StartConnect(cn_Ranker);
    //通信はページの更新と変わらない
    pmgEO_->mgPo_.poHome_.Ready_ranker(stEvent_.mid_ranking_, FALSE);
    if (pmgEO_->stSeq_.IsNoSv()) Connect_Ranker();
  }
  //POSTランカーpage
  void TLyHmEvEvent::Post_RankerPage()
  {
    StartConnect(cn_Ranker);
    //通信はページの更新と変わらない
    pmgEO_->mgPo_.poHome_.Ready_rankerpage(stEvent_.mid_ranking_, FALSE, stEvent_.rankingpage_);
    if (pmgEO_->stSeq_.IsNoSv()) Connect_Ranker();
  }
  //POST自分順位
  void TLyHmEvEvent::Post_MyRank()
  {
    StartConnect(cn_MyRank);
    //通信はページの更新と変わらない
    pmgEO_->mgPo_.poHome_.Ready_myrank(stEvent_.mid_ranking_, FALSE);
    if (pmgEO_->stSeq_.IsNoSv()) Connect_MyRank();
  }
  //POST：ＭＧＫ
  void TLyHmEvEvent::Post_MGK()
  {
    StartConnect(cn_MGK);
    if (stEvent_.mgknum_ == 0)
    {
      pmgEO_->mgPo_.poHmTeam_.Ready_mgkquick(stEvent_.id_team_dr_);
    }
    else
    {
      pmgEO_->mgPo_.poHmTeam_.Ready_mgk(stEvent_.id_team_dr_);
    }
  }

  //POST：シュートセット切り替え
  void TLyHmEvEvent::Post_SwitchShSet()
  {
    StartConnect(cn_SwitchShSet);
    pmgEO_->mgPo_.poHmTeam_.Ready_switchshset(stEvent_.id_team_dr_, stEvent_.id_shset_);
  }

  //POSTEvent敵情報取得
  void TLyHmEvEvent::Post_TimeAttackReady(void)
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_TimeAttackReady();
    }
    else
    {
      StartConnect(cn_TimeAttackReady);
      pmgEO_->mgPo_.poHmShiai_.Ready_tmatready(pmgEO_->stSeq_.GetMidEvent());
    }
  }
  //POSTEvent選択チーム
  void TLyHmEvEvent::Post_TimeAttackPlay(void)
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_TimeAttackPlay();
    }
    else
    {
      StartConnect(cn_TimeAttackPlay);
      pmgEO_->mgPo_.poHmShiai_.Ready_tmatplay(stEvent_.id_team_my_);
    }
  }
  //POSTリタイヤ（にげる）
  void TLyHmEvEvent::Post_TimeAttackRetire()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_TimeAttackRetire();
    }
    else
    {
      StartConnect(cn_TimeAttackRetire);
      pmgEO_->mgPo_.poHmShiai_.Ready_tmatretire();
    }
  }
  //POSTランキング報酬用ポスト
  void TLyHmEvEvent::Post_RewardRanker()
  {
      if (pmgEO_->stSeq_.IsNoSv())
      {
          Connect_RewardRanker();
      }
      else
      {
          StartConnect(cn_RewardRanker);
          //通信はページの更新と変わらない
          pmgEO_->mgPo_.poHome_.Ready_ranker(stEvent_.mid_ranking_, FALSE);
      }
  }

  void TLyHmEvEvent::Connect_TourReady()
  {
    pmgEO_->mgCoU_.SetOtherKantoku();
    frame_->ugheadder_->Refresh(FALSE);//HPリフレッシュ
    ChangePage(pg_Match);
  }
  void TLyHmEvEvent::Connect_TourPlay()
  {
    ChangePage(pg_GotoVS);
  }
  void TLyHmEvEvent::Connect_TourRetire()
  {
    //監督画像が共用なので元に戻す
    ugprof_.SetProfMe();
    ChangePage(pg_Menu);
  }
  void TLyHmEvEvent::Connect_Ranker()
  {
    ChangePage(pg_Ranker);
  }
  void TLyHmEvEvent::Connect_RankerPage()
  {
    ChangePage(pg_Ranker);
  }
  void TLyHmEvEvent::Connect_MyRank()
  {
    ChangePage(pg_MyRank);
  }
  //接続：ＭＧＫ
  void TLyHmEvEvent::Connect_MGK()
  {
    ugdrTeam_.RefreshStamina();
    ugbtn_mgk_->Refresh();
    SetHudEnd();
    ChangePage(pg_TmSelect);
  }
  //通信：シュートセット切り替え
  void TLyHmEvEvent::Connect_SwitchShSet()
  {
    ugdrTeam_.RefreshStamina();
    ChangePage(pg_TmSelect);
  }

  void TLyHmEvEvent::Connect_TimeAttackReady(void)
  {
    frame_->ugheadder_->Refresh(FALSE);//HPリフレッシュ
    ChangePage(pg_timeAttackMatch);
  }
  void TLyHmEvEvent::Connect_TimeAttackPlay(void)
  {
    ChangePage(pg_GotoVS);
  }
  void TLyHmEvEvent::Connect_TimeAttackRetire()
  {
    //監督画像が共用なので元に戻す
    ugprof_.SetProfMe();
    ChangePage(pg_Menu);
  }
  void TLyHmEvEvent::Connect_RewardRanker()
  {
      // 0番に必ず自分のデータが入っている
      nowRank_ = pmgEO_->mgCoU_.mdre_Ranker_.GetLineDt(0, enmdre_Ranker::mdre_ranker_rank);

#ifdef __K_DEBUG_USER_DATA__
      // イベントの種類
      nowRank_ = 3;
#endif

      nowRewardRank_ = NGNUM;
      nextRewardRank_ = NGNUM;

      // 今回のイベントの報酬をまとめる
      INTVECTOR rkRewardVec;
      rkRewardVec.clear();
      for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_RkReward_.GetLineNum(); i++)
      {
          // 該当のイベントではない
          if (pmgEO_->mgCoM_.mdm_RkReward_.GetLineDt(i, enmdm_RkReward::mdm_rkreward_id_m_ranking) != mid_Ranking_) { continue; }
          rkRewardVec.push_back(pmgEO_->mgCoM_.mdm_RkReward_.GetLineDt(i, enmdm_RkReward::mdm_rkreward_pk));
      }

      ugLiRkReward_.ClearRecord();
      for (s32 i = 0; i < rkRewardVec.size(); i++)
      {
          s64 pk = rkRewardVec[i];

          // 該当の順位ではない
          s32 top = pmgEO_->mgCoM_.mdm_RkReward_.GetPKDt(pk, enmdm_RkReward::mdm_rkreward_top);
          s32 bottom = pmgEO_->mgCoM_.mdm_RkReward_.GetPKDt(pk, enmdm_RkReward::mdm_rkreward_bottom);

          // 今の報酬の番号
          if (nowRank_ >= top && bottom >= nowRank_)
          {
              // 自分の報酬ランク
              nowRewardRank_ = bottom;

              // 1位だったときは特にセットしない
              if (i != 0)
              {
                  // 今の報酬の次の報酬
                  s64 nextPK = rkRewardVec[i - 1];
                  nextRewardRank_ = pmgEO_->mgCoM_.mdm_RkReward_.GetPKDt(nextPK, enmdm_RkReward::mdm_rkreward_bottom);
              }
          }

          TUGRcNormal* newrecord = ugLiRkReward_.MakeObj_Record();
          std::string str = "";
          // その順位専用報酬
          if (top == bottom)
          {
              str = lib_str::IntToStr(top) + "い＿ほうしゅう";
          }
          // 桁数4桁同士までは「○○～○○い」という表示
          else if ((top / 10000) == 0 && (bottom / 10000) == 0)
          {
              str = lib_str::IntToStr(top) + "＿波＿" + lib_str::IntToStr(bottom) + "い";
          }
          // 桁数5桁以上なら「○○い　いか」という表示
          else
          {
              str = lib_str::IntToStr(top) + "い＿いか";
          }
          newrecord->SetNameIcon(str.c_str(), enUGPIcon::ugi_GiftBox);
          newrecord->SetCommonID(pk);
      }

      // 今のランクに報酬がない
      if (nowRewardRank_ == NGNUM)
      {
          ugNowReward_.SetRankOutData();

          // 今のランクが分からないと次がどれか分からないので
          // もしランキングに報酬が存在するのであれば
          // 一番後ろのデータを次のデータとしてセットしておく
          if (rkRewardVec.size())
          {
              s64 nextPK = rkRewardVec[rkRewardVec.size() - 1];
              nextRewardRank_ = pmgEO_->mgCoM_.mdm_RkReward_.GetPKDt(nextPK, enmdm_RkReward::mdm_rkreward_bottom);
          }
      }
      // あるときはそのまま今のランクをセット
      else
      {
          ugNowReward_.SetNowData(nowRank_);
      }

      // 次のランクの報酬がない
      if (nextRewardRank_ == NGNUM)
      {
          ugNextReward_.SetNextData(nowRank_, nextRewardRank_);
      }
      // あるときはその順位差を渡す
      else
      {
          ugNextReward_.SetNextData(nowRank_, nowRank_ - nextRewardRank_);
      }

      ugRkReward_.SetEtcData();

    ChangePage(pg_reward);
  }
  
  //*******************

  BOOL TLyHmEvEvent::IsGotoVS()
  {
    return (stLy_.pageNo_ == pg_GotoVS);
  }
  void TLyHmEvEvent::SetVS()
  {
    ChangePage(pg_VS);
  }
  void TLyHmEvEvent::UpdateCursor(BOOL draw_f) // スタミナ回復先導カーソル更新
  {
    if (stEvent_.isCheckStamina_)
    {
      if (stEvent_.cursorDispTimer_ < 0)
      {
        pCursor_->g_SetDraw(FALSE);
        stEvent_.cursorDispTimer_ = -1;
      }
      else
      {
        stEvent_.cursorDispTimer_--;
      }
    }
    pCursor_->Update(draw_f);
  }

}

