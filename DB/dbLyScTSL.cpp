#include "dbLyScTSL.h"
#include "dbSnScenario.h"

namespace db
{

  enum enPage
  {
    pg_TmSelect,
    pg_Filter,
    pg_Nige,
    pg_MGK,
    pg_ShSetSelect,
    pg_GotoVS,
  };

  enum enConnect
  {
    cn_play,
    cn_mgk,
    cn_SwitchShSet,
  };

  //コンストラクタ
  TLyScTSL::TLyScTSL(TSnScenario* pScene) :TLyScGen(pScene)
    , ugdrTeam_(this)
    , ugencard_(this)
    , uglistTmFilter_(this)
    , pCursor_(NULL)
    , ugtitle_me_(this)
    , ugtitle_en_(this)
    , uglistShSetSelectAll_(this)
    , uglistShSetSelect_(this)
    , uglistMember_(this)
    //, ugbtn_court_(this)
  {
  }

  TLyScTSL::~TLyScTSL()
  {
  }


  void TLyScTSL::RetrunHedder()
  {
    ChangePage(stLy_.pageNo_);
  }

  void TLyScTSL::DoLoad()
  {
    stTSL_.Init();
    //field_ = MakeObj();
    filter_ = MakeObj();
    filter_->z_SetPos_SafeLF(0, 0);
    filter_->o_SetObjFilterGrp(512, 512, 0xc0, 0x00, 0x00, 0x00);

    //ドラム
    uglistTmFilter_.MakeGroup(field_, FALSE);
    ugdrTeam_.MakeGroup(field_, TRUE, SIDE0, &uglistTmFilter_);
    uglistShSetSelectAll_.MakeGroup(field_, FALSE);
    uglistShSetSelect_.MakeGroup(field_, FALSE);
    uglistMember_.MakeGroup(field_, TRUE);
    //    ugdrTeam_.SetDtNum(pmgEO_->mgCoU_.mdTeam_.GetLineNum());

    //敵チームカード
    ugencard_.MakeGroup(field_, SIDE1);
    ugencard_.SetLRPos(FALSE);
    ugencard_.StaminaNone(TRUE);

    ugtitle_me_.MakeGroup(field_, FALSE);
    ugtitle_me_.SetMessage("あなたのチーム");
    ugtitle_me_.SetLRPos(TRUE);
    ugtitle_en_.MakeGroup(field_, FALSE);
    ugtitle_en_.SetMessage("あいてのチーム");
    ugtitle_en_.SetLRPos(FALSE);

    //ugbtn_court_.MakeGroupCourt(field_);
    //ugbtn_court_.SetPos(145, 67);


    //初期チーム読み込み
    LoadLookPosTeam();

    // スタミナ回復先導カーソル
    pCursor_ = MakeObj();
    pCursor_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_finger));
    pCursor_->g_SetPartNo(1);
    //pCursor_->o_SetParent(field_);
    pCursor_->o_SetParent(ugbtn_mgk_->GetField());
    pCursor_->g_SetDraw(FALSE);
    pCursor_->z_SetPos(50, -28);
  }


  void TLyScTSL::DoReady()
  {
    stTSL_.Init();
    //ugdrTeam_.RefreshRoll(TRUE);Readyの中に入れてみた

    uglistTmFilter_.Ready(pmgEO_->stSeq_.GetMidEvent());//イベントＩＤ入れる（特効）
    ugdrTeam_.ReadyFilter();
    ResetBtn();

    for (s32 i = 0; i < pmgEO_->mgCoU_.mdMap_.GetLineNum(); ++i)
    {
      if (pmgEO_->mgCoU_.mdMap_.GetLineDt(i, mdmap_id_m_maptip) == pmgEO_->stSeq_.GetMidMapTip())
      {
        stTSL_.id_map_ = pmgEO_->mgCoU_.mdMap_.GetLineDt(i, mdmap_pk);
        break;
      }
    }

    //敵番号
    stTSL_.enemyno_ = pmgEO_->mgCoU_.mdMap_.GetPKDt(stTSL_.id_map_, mdmap_enemyNo);
    enmdm_Maptip enemyTeam = mdm_maptip_id_m_teamA;
    switch (stTSL_.enemyno_)
    {
    case 0: enemyTeam = mdm_maptip_id_m_teamA; break;
    case 1: enemyTeam = mdm_maptip_id_m_teamB; break;
    case 2:enemyTeam = mdm_maptip_id_m_teamC; break;
    }

	// マップ上の敵なので相手監督IDはNGNUｍ
	stTSL_.id_kantoku_en_ = NGNUM;

    //敵ＭＩＤ
    stTSL_.mid_team_en_ = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), enemyTeam);
    //敵読み込み
	LoadTeamEnemy(stTSL_.id_kantoku_en_, stTSL_.mid_team_en_);
  }
  void TLyScTSL::DoDown()
  {

  }


  void TLyScTSL::DoResetBtn()
  {
    SetFoot_BtnHd();



    ugbtn_next_->SetFont("ならびかえ");
    ugbtn_back_->SetFont("にげる！");
    ugbtn_mid_->SetFont("しゅーと");
    //ugbtn_court_.SetDraw(stTSL_.enablehome_f_);
    ugbtn_ok_->SetFont("しあいかいし");
    ugbtn_ok_->ClearAct();				// これがないとチーム選択画面がスキップされてしまう

    ugbtn_mgk_->Refresh();
    ugbtn_mgk_->SetDraw(TRUE);
    ugbtn_ok_->SetDraw(TRUE);
    ugbtn_back_->SetDraw(TRUE);

    ugbtn_next_->SetDraw(TRUE);
    ugbtn_mid_->SetDraw(TRUE);
    uglistTmFilter_.SetDraw(FALSE);
    uglistShSetSelectAll_.SetDraw(FALSE);
    uglistShSetSelect_.SetDraw(FALSE);
    uglistMember_.SetDraw(FALSE);
  }

  void TLyScTSL::DoChangePage(s32 pageno)
  {
    switch (pageno)
    {
    case db::pg_TmSelect:
      ResetBtn();
      UpdateCheckStamina();
      stTSL_.id_team_ = ugdrTeam_.GetIDTeam();
      stTSL_.mid_team_ = ugdrTeam_.GetMIDTeam();
      break;
    case db::pg_Filter:
      ugbtn_mgk_->SetDraw(FALSE);
      ugbtn_ok_->SetDraw(FALSE);
      ugbtn_mgk_->SetDraw(FALSE);
      ugbtn_next_->SetDraw(FALSE);
      ugbtn_mid_->SetDraw(FALSE);
      uglistTmFilter_.SetDraw(TRUE);
      ugbtn_back_->SetFont("もどる");
      ugbtn_back_->SetDraw(TRUE);
      break;
    case db::pg_Nige:
    {
      SetFoot_Btn();
      STRVECTOR strvec = { "ＨＰは＿しょうひされます", "よろしいですか？" };
      SetHudMsg(TRUE, FALSE, &strvec);
    }
    break;
    case db::pg_MGK:
    {
      SetFoot_Btn();
      stTSL_.mgknum_ = pmgEO_->mgCoU_.GetSupplyNum_MGK();
      if (stTSL_.mgknum_ > 0)
      {
        std::string strnum = "もっているかず：";
        strnum += lib_str::IntToStr(stTSL_.mgknum_);
        STRVECTOR strvec = { "ミナゲンキーを＿しようします", "よろしいですか？", strnum };
        SetHudMsg(TRUE, FALSE, &strvec);
      }
      else
      {
        SetHudMsgStarSupply(SPL_MGK, TRUE);
      }
    }
    break;
    case db::pg_ShSetSelect:
    {
      uglistShSetSelectAll_.SetDraw(TRUE);
      //uglistShSetSelect_.SetDraw(TRUE);
      uglistMember_.SetDraw(TRUE);
      ugbtn_mgk_->SetDraw(FALSE);
      ugbtn_ok_->SetDraw(FALSE);
      ugbtn_mgk_->SetDraw(FALSE);
      ugbtn_next_->SetDraw(FALSE);
      ugbtn_mid_->SetDraw(FALSE);
      ugbtn_back_->SetFont("もどる");
      ugbtn_back_->SetDraw(TRUE);
    }
    break;
    case db::pg_GotoVS:

      break;
    }
  }

  //接続完了
  void TLyScTSL::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_play: Connect_Play(); break;
        case cn_mgk: Connect_MGK(); break;
        case cn_SwitchShSet: Connect_SwitchShSet(); break;
        }
      }
      else
      {
        //エラーメッセージ
        EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
        ChangePage(pg_TmSelect);
      }
    }
    else
    {
      //通信エラー
      ChangePage(pg_TmSelect);
    }
  }

  void TLyScTSL::DoUpdate(BOOL draw_f)
  {

    //if ((draw_f == FALSE) && pScene_->lyScBar_->IsBarMove()) return;
    if (pScene_->lyScBar_->IsBarAction())
    {
      stTSL_.turnbar_f_ = TRUE;
      return;
    }
    else
    {
      if (stTSL_.turnbar_f_)
      {
        stTSL_.turnbar_f_ = FALSE;
        RefreshTeamTexture();
      }
    }

    switch (stLy_.pageNo_)
    {
    case db::pg_GotoVS:
      ugdrTeam_.Update(draw_f);
      ugencard_.Update(draw_f);
      break;
    case db::pg_Filter:
      ugdrTeam_.Update(draw_f);
      uglistTmFilter_.Update(draw_f);
      UpdateCursor(draw_f);
      break;
    case db::pg_ShSetSelect:
      uglistShSetSelectAll_.Update(draw_f);
      uglistShSetSelect_.Update(draw_f);
      uglistMember_.Update(draw_f);
      break;
    default:
      ugdrTeam_.Update(draw_f);
      uglistTmFilter_.Update(draw_f);
      ugencard_.Update(draw_f);
      ugtitle_me_.Update(draw_f);
      ugtitle_en_.Update(draw_f);
      //ugbtn_court_.Update(draw_f);
      UpdateCursor(draw_f);
      break;
    }

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case db::pg_TmSelect:MvPage_TSL(); break;
      case db::pg_Filter:MvPage_TmFilter(); break;
      case db::pg_Nige:MvPage_Nige(); break;
      case db::pg_MGK:MvPage_MGK(); break;
      case db::pg_ShSetSelect:MvPage_ShSetSelect(); break;
      case db::pg_GotoVS:MvPage_GotoVS(); break;
      }
    }

  }


  void TLyScTSL::LoadLookPosTeam()
  {
    //同じＩＤの時は読み込みいらない
    s32 mid_team = pmgEO_->mgCoU_.mdTeam_.GetLineDt(ugdrTeam_.GetDataNo(), mdteam_id_m_team);

    if (stTSL_.mid_team_ == mid_team) return;
    //if (stSelect_.teamid_ == testdt_[ugdrTeam_.GetDataNo()]) return;

	stTSL_.id_kantoku_my_ = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(enmdKantoku::mdkantoku_pk);
    stTSL_.mid_team_ = mid_team;
    stTSL_.id_team_ = pmgEO_->mgCoU_.mdTeam_.GetLineDt(ugdrTeam_.GetDataNo(), mdteam_pk);
    LoadTeamTexture(SIDE0, mid_team);
  }

  //敵チームカード＆情報
  void TLyScTSL::LoadTeamEnemy(s64 id_kantoku, s32 mid_team)
  {
    LoadTeamTexture(SIDE1, mid_team);
	// この関数を使うとチーム名セットのところで
	// 自分のレベルを表示してしまうので使わない
    //ugencard_.SetTeamDt3(NGNUM, mid_team, NGNUM, TRUE);
	const s32 ENEMY_LV = 0;
	ugencard_.SetTeamByCOM(id_kantoku, NGNUM, mid_team, NGNUM, ENEMY_LV);
    ugencard_.SetTipsDraw_f(TRUE);
  }


  //テクスチャロード
  void TLyScTSL::LoadTeamTexture(s32 sideno, s32 mid_team)
  {
    if (mid_team <= 0) return;

    BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_gb_f);
    for (s32 c = 0; c < DBMEMBER_ALL; ++c)
    {
      s32 charid = pmgEO_->mgCoM_.GetCharID(mid_team, c);
      pmgEO_->mgGrp_.pmgChGrp_[sideno][c]->SetCharDt(charid, mid_team, gb_f);
      pmgEO_->mgGrp_.pmgChGrp_[sideno][c]->SetGrTile(sideno, c);
    }
    pmgEO_->mgGrp_.MakeIconTexture_Chest(sideno, FALSE, FALSE);
    pmgEO_->mgGrp_.MakeIconTexture_Body(sideno, 0, 0, TRUE, FALSE);
  }

  //ヘッダにいって返ってきたときのリフレッシュ
  void TLyScTSL::RefreshTeamTexture()
  {
    LoadTeamTexture(SIDE0, pmgEO_->mgCoU_.mdTeam_.GetPKDt(ugdrTeam_.GetIDTeam(), mdteam_id_m_team));
    LoadTeamTexture(SIDE1, stTSL_.mid_team_en_);
  }

  void TLyScTSL::MvPage_TSL()
  {

    UpdateCheckStamina();

    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_Nige);
    }
    else if (ugbtn_next_->IsAct())//ならびかえボタン
    {
      ChangePage(pg_Filter);
    }
    else if (ugbtn_mid_->IsAct())//シュートセットボタン
    {
      ChangePage(pg_ShSetSelect);
      uglistShSetSelectAll_.Refresh(stTSL_.id_team_, NGNUM, shsetvec_);
      uglistMember_.Refresh(stTSL_.id_team_, stTSL_.mid_team_, FALSE);
      stTSL_.shsetmem_f_ = FALSE;
      stTSL_.id_shset_first_ = uglistShSetSelectAll_.GetShSet();
    }
    else if (ugbtn_mgk_->IsAct())//ＭＧＫボタン
    {
      ChangePage(pg_MGK);
    }
    else if (ugbtn_ok_->IsAct())//おーけーボタン|| ugbtn_mid_->IsAct()
    {
      // スタミナ不足
      if (stTSL_.isCheckStamina_)
      {
        stTSL_.cursorDispTimer_ = 60;
        pCursor_->g_SetDraw(TRUE);
        return;
      }

      //stTSL_.homecourt_f_ = ugbtn_court_.IsHome();//ホームコート
      SetEtcSS();
      Post_Play();
    }
    else if (ugdrTeam_.IsActRes(enUGDrTeamAct_Stop))//読み込みが必要なタイミング
    {
      //現在のチームのＩＤ，MID
      stTSL_.id_team_ = ugdrTeam_.GetIDTeam();
      stTSL_.mid_team_ = ugdrTeam_.GetMIDTeam();
    }
  }

  void TLyScTSL::MvPage_TmFilter()
  {
    if (ugbtn_back_->IsAct())//おーけー
    {
      stTSL_.id_team_ = ugdrTeam_.GetIDTeam();
      stTSL_.mid_team_ = ugdrTeam_.GetMIDTeam();
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
  //にげる
  void TLyScTSL::MvPage_Nige()
  {
    if (IsHudOK())//おーけー
    {
      pScene_->lyScBar_->MoveLayer(scl_Map);
    }
    else if (IsHudCancel())//きゃんせる
    {
      ChangePage(pg_TmSelect);
    }
  }
  //みなげんき
  void TLyScTSL::MvPage_MGK()
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
  //シュートセット切り替え
  void TLyScTSL::MvPage_ShSetSelect()
  {

    if (stTSL_.shsetmem_f_)//個別
    {

      if (ugbtn_back_->IsAct())//おーけー
      {
        if (stTSL_.id_shset_first_ != uglistShSetSelect_.GetShSet())
        {
          stTSL_.id_shset_ = uglistShSetSelect_.GetShSet();
          Post_SwitchShSet();
        }
        else
        {
          ChangePage(pg_TmSelect);
        }
      }
      else if (uglistMember_.IsAct())//個別に
      {
        stTSL_.id_shset_ = uglistShSetSelect_.GetShSet();
        if (uglistMember_.IsSelected())
        {
          uglistShSetSelect_.Refresh(stTSL_.id_team_, stTSL_.id_shset_, uglistMember_.GetActIndex(), shsetvec_);
        }
        else
        {
          uglistShSetSelectAll_.Refresh(stTSL_.id_team_, stTSL_.id_shset_, shsetvec_);//全体に戻る
        }
      }
    }
    else//全体
    {
      if (ugbtn_back_->IsAct())//おーけー
      {
        if (stTSL_.id_shset_first_ != uglistShSetSelectAll_.GetShSet())
        {
          stTSL_.id_shset_ = uglistShSetSelectAll_.GetShSet();
          Post_SwitchShSet();
        }
        else
        {
          ChangePage(pg_TmSelect);
        }
      }
      else if (uglistMember_.IsAct())//個別に
      {
        stTSL_.id_shset_ = uglistShSetSelectAll_.GetShSet();
        uglistShSetSelect_.Refresh(stTSL_.id_team_, stTSL_.id_shset_, uglistMember_.GetActIndex(), shsetvec_);
      }
    }

    stTSL_.shsetmem_f_ = uglistMember_.IsSelected();

    uglistShSetSelectAll_.SetDraw(!uglistMember_.IsSelected());
    uglistShSetSelect_.SetDraw(uglistMember_.IsSelected());

  }
  void TLyScTSL::MvPage_GotoVS()
  {

  }

  //POST：開始
  void TLyScTSL::Post_Play()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_Play();
    }
    else
    {
      StartConnect(cn_play);
      pmgEO_->mgPo_.poScenario_.Ready_play(stTSL_.id_team_);
    }
  }
  //POST：ＭＧＫ
  void TLyScTSL::Post_MGK()
  {
    StartConnect(cn_mgk);
    if (stTSL_.mgknum_ == 0)
    {
      pmgEO_->mgPo_.poHmTeam_.Ready_mgkquick(stTSL_.id_team_);
    }
    else
    {
      pmgEO_->mgPo_.poHmTeam_.Ready_mgk(stTSL_.id_team_);
    }
  }
  //POST：シュートセット切り替え
  void TLyScTSL::Post_SwitchShSet()
  {
    StartConnect(cn_SwitchShSet);
    pmgEO_->mgPo_.poHmTeam_.Ready_switchshset(stTSL_.id_team_, stTSL_.id_shset_);
  }
  //接続：敵選択完了
  void TLyScTSL::Connect_Play()
  {
    pScene_->lyScBar_->MoveLayer(scl_VSFilter);
    SetHudEnd();
    ChangePage(pg_GotoVS);
  }
  //接続：ＭＧＫ
  void TLyScTSL::Connect_MGK()
  {
    ugdrTeam_.RefreshStamina();
    ugbtn_mgk_->Refresh();
    SetHudEnd();
    ChangePage(pg_TmSelect);
  }
  //通信：シュートセット切り替え
  void TLyScTSL::Connect_SwitchShSet()
  {
    ugdrTeam_.RefreshStamina();
    ChangePage(pg_TmSelect);
  }
  void TLyScTSL::UpdateCheckStamina(void)
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
      if (!stTSL_.isCheckStamina_)
      {
        stTSL_.isCheckStamina_ = TRUE;
        if (stTSL_.isMGKBtn_ == FALSE)
        {
          ugbtn_ok_->SetFont("ＳＴかいふく");
          stTSL_.isMGKBtn_ = TRUE;
        }
      }
    }
    else
    {
      if (stTSL_.isCheckStamina_)
      {
        stTSL_.isCheckStamina_ = FALSE;
        if (stTSL_.isMGKBtn_)
        {
          ugbtn_ok_->SetFont("しあいかいし");
          stTSL_.isMGKBtn_ = FALSE;
        }
      }
    }

    //ugbtn_ok_->SetDraw(!mgk_f);
    //ugbtn_mgk_->SetDraw(mgk_f);

    //id_prev_team_ = id_team;
    //mid_prev_team_ = mid_team;
  }


  void TLyScTSL::UpdateCursor(BOOL draw_f)
  {
    if (stTSL_.isCheckStamina_)
    {
      if (stTSL_.cursorDispTimer_ < 0)
      {
        pCursor_->g_SetDraw(FALSE);
        stTSL_.cursorDispTimer_ = -1;
      }
      else
      {
        stTSL_.cursorDispTimer_--;
      }
    }
    pCursor_->Update(draw_f);
  }



  //両者情報セット
  void TLyScTSL::SetEtcSS()
  {
    s64 id_map = 0;
    for (s32 i = 0; i < pmgEO_->mgCoU_.mdMap_.GetLineNum(); ++i)
    {
      if (pmgEO_->mgCoU_.mdMap_.GetLineDt(i, mdmap_id_m_maptip) == pmgEO_->stSeq_.GetMidMapTip())
      {
        id_map = pmgEO_->mgCoU_.mdMap_.GetLineDt(i, mdmap_pk);
        break;
      }
    }

    //敵番号
    s32 enemyno = pmgEO_->mgCoU_.mdMap_.GetPKDt(id_map, mdmap_enemyNo);
    enmdm_Maptip enemyTeam = mdm_maptip_id_m_teamA;
    enmdm_Maptip dropPtn = mdm_maptip_id_m_drop_A;
    enmdm_Maptip caper = mdm_maptip_catchA;
    enmdm_Maptip ncaper = mdm_maptip_niceA;
    switch (enemyno)
    {
    case 0:
      enemyTeam = mdm_maptip_id_m_teamA;
      dropPtn = mdm_maptip_id_m_drop_A;
      caper = mdm_maptip_catchA;
      ncaper = mdm_maptip_niceA;
      break;
    case 1:
      enemyTeam = mdm_maptip_id_m_teamB;
      dropPtn = mdm_maptip_id_m_drop_B;
      caper = mdm_maptip_catchB;
      ncaper = mdm_maptip_niceB;
      break;
    case 2:
      enemyTeam = mdm_maptip_id_m_teamC;
      dropPtn = mdm_maptip_id_m_drop_C;
      caper = mdm_maptip_catchC;
      ncaper = mdm_maptip_niceC;
      break;
    }

    //敵ＭＩＤ
    //s32 mid_team_en = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), enemyTeam);
    //LoadTeamEnemy(mid_team_en);//敵読み込み

    //コート//BGMまず敵のデフォ
    s32 mid_court = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(stTSL_.mid_team_en_, mdm_team_id_m_haikei);
    s32 mid_bgm = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(stTSL_.mid_team_en_, mdm_team_id_m_bgm);

    //敵専用BGMがある時はそちらに差し替え
    if (pmgEO_->stSeq_.IsHomeCourt())//ホーム
    {
      mid_court = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_haikei);
      mid_bgm = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_bgm);
    }
    else//マップマスターあるときはさしかえ
    {
      s32 mid_court_map = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), mdm_maptip_id_m_haikei);
      s32 mid_bgm_map = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), mdm_maptip_id_m_bgm);
      if (mid_court_map != 0) mid_court = mid_court_map;
      if (mid_bgm_map != 0) mid_bgm = mid_bgm_map;
    }
    pmgEO_->stResult_.Init();
    pmgEO_->stDrop_.Init();

    BOOL tuto_f = (pmgEO_->stSeq_.GetTutoType() != TStSnSeq::tt_None);

    //特効倍率
    s32 tokkoumag = pmgEO_->mgCoM_.GetTokkouMag(stTSL_.id_team_, pmgEO_->stSeq_.GetMidEvent());

#ifndef __K_MAKE_HIDE_EVENT_SOUND__
	s32 mid_bgm_ready = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(pmgEO_->stSeq_.GetMidEvent(), mdm_event_id_m_bgm_ready);
	s32 mid_bgm_win   = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(pmgEO_->stSeq_.GetMidEvent(), mdm_event_id_m_bgm_win);
	s32 mid_bgm_lose  = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(pmgEO_->stSeq_.GetMidEvent(), mdm_event_id_m_bgm_lose);
	if (mid_bgm_ready == 0) { mid_bgm_ready = nsf_jgl_ready; }
	if (mid_bgm_win   == 0) { mid_bgm_win   = nsf_jgl_win;   }
	if (mid_bgm_lose  == 0) { mid_bgm_lose  = nsf_jgl_lose;  }
#endif

    pmgEO_->stShiai_.SetShiai(
	  stTSL_.id_kantoku_my_,
	  stTSL_.id_kantoku_en_,
      stTSL_.id_team_,
      stTSL_.mid_team_,
      stTSL_.mid_team_en_,
	  FALSE,	// 1Pはプレイヤー
	  TRUE,		// 2PはCOM
      mid_court,
      mid_bgm,
#ifndef __K_MAKE_HIDE_EVENT_SOUND__
	  mid_bgm_ready,
	  mid_bgm_win,
	  mid_bgm_lose,
#endif
      tokkoumag,
      pmgEO_->mgCoU_.GetCtrl(),
      pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), mdm_maptip_shiaiSec),
      pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), caper),
      pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), ncaper),
      tuto_f);


    //ドロップアイテム
    s64 mid_drop = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), dropPtn);
    s32 itemType = 0;
    s32 itemID = 0;
    s32 itemNum = 0;
    s32 dropNo = pmgEO_->mgCoU_.mdMap_.GetPKDt(id_map, mdmap_dropNo);

    if (dropNo == 3)
    {
      enmdm_Maptip droptype = mdm_maptip_itemType_sdA;
      enmdm_Maptip dropid = mdm_maptip_itemID_sdA;
      enmdm_Maptip dropnum = mdm_maptip_itemNum_sdA;
      switch (enemyno)
      {
      case 0:
        droptype = mdm_maptip_itemType_sdA;
        dropid = mdm_maptip_itemID_sdA;
        dropnum = mdm_maptip_itemNum_sdA;
        break;
      case 1:
        droptype = mdm_maptip_itemType_sdB;
        dropid = mdm_maptip_itemID_sdB;
        dropnum = mdm_maptip_itemNum_sdB;
        break;
      case 2:
        droptype = mdm_maptip_itemType_sdC;
        dropid = mdm_maptip_itemID_sdC;
        dropnum = mdm_maptip_itemNum_sdC;
        break;
      }
      itemType = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), droptype);
      itemID = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), dropid);
      itemNum = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pmgEO_->stSeq_.GetMidMapTip(), dropnum);
    }
    else
    {
      enmdm_Drop droptype = mdm_drop_itemType_0;
      enmdm_Drop dropid = mdm_drop_itemID_0;
      enmdm_Drop dropnum = mdm_drop_itemNum_0;
      switch (dropNo)
      {
      case 0:
        droptype = mdm_drop_itemType_0;
        dropid = mdm_drop_itemID_0;
        dropnum = mdm_drop_itemNum_0;
        break;
      case 1:
        droptype = mdm_drop_itemType_1;
        dropid = mdm_drop_itemID_1;
        dropnum = mdm_drop_itemNum_1;
        break;
      case 2:
        droptype = mdm_drop_itemType_2;
        dropid = mdm_drop_itemID_2;
        dropnum = mdm_drop_itemNum_2;
        break;
      }
      itemType = pmgEO_->mgCoM_.mdm_Drop_.GetPKDt(mid_drop, droptype);
      itemID = pmgEO_->mgCoM_.mdm_Drop_.GetPKDt(mid_drop, dropid);
      itemNum = pmgEO_->mgCoM_.mdm_Drop_.GetPKDt(mid_drop, dropnum);
    }

    //特効倍率（サーバー側でも同様の処理を行う）
    s32 tokkouitemnum = pmgEO_->mgCoU_.TokkouItemNum(pmgEO_->stSeq_.GetMidEvent(), tftDrop);
    s32 tokkouitemmag = pmgEO_->mgCoU_.TokkouItemMag(pmgEO_->stSeq_.GetMidEvent(), tftDrop, tokkouitemnum);

    pmgEO_->stDrop_.SetShiai(dropNo, itemType, itemID, itemNum, tokkouitemnum, tokkouitemmag,
      pmgEO_->mgCoM_.mdm_Drop_.GetPKDt(mid_drop, mdm_drop_itemType_Cmp),
      pmgEO_->mgCoM_.mdm_Drop_.GetPKDt(mid_drop, mdm_drop_itemID_Cmp),
      pmgEO_->mgCoM_.mdm_Drop_.GetPKDt(mid_drop, mdm_drop_itemNum_Cmp));

  }
}
