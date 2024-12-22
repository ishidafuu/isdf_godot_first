
#include "dbLyHmTmSelect.h"
#include "dbSnHome.h"
#include "dbUGPrev.h"

namespace db
{
  enum enPage
  {
    pg_Menu,
    pg_ShSet,
    pg_Filter,
    pg_TeamSelect,
    pg_ShSetSelect,
    pg_MemberSelect,
    pg_MemSoubi,
    pg_AllSoubi,
    pg_Syn,
    pg_ShFilter,
    pg_ShSetRename,
    pg_ShPrev,
    pg_ShPrevSyn,
  };


  enum enMenuList
  {
    ml_TeamSelect,
    ml_ShSet1,
    ml_ShSet2,
    ml_ShSet3,
    mlEND,
  };

  enum enTeamSelectMenuList
  {
    tsml_ShSet,
    tsml_Custom,
    tsml_CustomAll,
    tsml_Syn,
    tsmlEND,
  };

  enum enConnect
  {
    cn_Omakase,
    cn_AttachCustom,
    cn_AttachShSet,
    cn_SwitchShSet,
    cn_RenameShSet,
  };

  //コンストラクタ
  TLyHmTmSelect::TLyHmTmSelect(TSnHome* pScene) :TLyHmGen(pScene)
    , ugdrTeam_(this)
    , ugdrShot_(this)
    , uglistMember_(this)
    , uglistSoubi_(this)
    , uglistSoubiAll_(this)
    , uglistMenu_(this)
    , uglistShSetSelect_(this)
    , uglistShSetMenu_(this)
    , uglistShSet_(this)
    , uglistTmFilter_(this)
    , uglistShFilter_(this)
    , uglistSyn_(this)
  {

  }
  TLyHmTmSelect::~TLyHmTmSelect()
  {

  }

  void TLyHmTmSelect::RefreshMenuList()
  {
    uglistMenu_.ClearRecord();
    //メニューレコード
    for (s32 i = 0; i < mlEND; ++i)
    {
      TUGRcNormal* newrecord = (TUGRcNormal*)uglistMenu_.MakeObj_Record();
      //newrecord->SetCommonID(i);
      if (i == 0)
      {
        newrecord->ugname_.SetNameIcon("チームせってい", ugi_Ranking);
      }
      else
      {
        std::string shsetname = pmgEO_->mgCoU_.mdShSet_.GetLineDtStr(i, mdshset_name_c32);
        if (shsetname == "")
        {
          shsetname = "しゅーとセット" + lib_str::IntToStr(i);
        }
        shsetname += "＿へんしゅう";
        newrecord->ugname_.SetNameIcon(shsetname.c_str(), (enUGPIcon)(ugi_ShSet1 + i - 1));
        newrecord->SetCommonID(pmgEO_->mgCoU_.mdShSet_.GetLineDt(i, mdshset_pk));
      }

    }

  }
  void TLyHmTmSelect::DoLoad()
  {
    stSelect_.Init();
    uglistMenu_.MakeGroup(field_, FALSE);
    uglistShSetSelect_.MakeGroup(field_, FALSE);
    uglistShSetMenu_.MakeGroup(field_, FALSE);
    uglistShSet_.MakeGroup(field_, FALSE);
    uglistMember_.MakeGroup(field_, TRUE);
    uglistSoubi_.MakeGroup(field_, FALSE);
    uglistSoubiAll_.MakeGroup(field_, FALSE);
    uglistTmFilter_.MakeGroup(field_, FALSE);
    uglistShFilter_.MakeGroup(field_, FALSE);
    uglistSyn_.MakeGroup(field_, FALSE);

    ugdrTeam_.MakeGroup(field_, TRUE, SIDE0, &uglistTmFilter_);
    ugdrShot_.MakeGroup(field_, TRUE, &uglistShFilter_);

    listvec_.clear();
    listvec_.push_back(&uglistMenu_);
    listvec_.push_back(&uglistShSetSelect_);
    listvec_.push_back(&uglistShSetMenu_);
    listvec_.push_back(&uglistShSet_);
    listvec_.push_back(&uglistMember_);
    listvec_.push_back(&uglistSoubi_);
    listvec_.push_back(&uglistSoubiAll_);
    listvec_.push_back(&uglistTmFilter_);
    listvec_.push_back(&uglistShFilter_);
    listvec_.push_back(&uglistSyn_);

  }

  //入店時の処理
  void TLyHmTmSelect::DoReady()
  {
    stSelect_.Init();
    ugbtn_back_->SetFont("もどる");
    //ugbtn_ok_->SetFont("しゅーとそうび");

    uglistShSetMenu_.Ready(TRUE);
    uglistTmFilter_.Ready(NGNUM);
    ugdrTeam_.ReadyFilter();

    uglistShFilter_.Ready(enShotFilTypeS_All);
    ugdrShot_.ReadyFilter();
  }
  void TLyHmTmSelect::DoDown()
  {
    pScene_->lyHmTeam_->Ready();
  }
  void TLyHmTmSelect::DrawList(TUGLiGen* drawlist)
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
    ResetObj();
    ugbtn_back_->SetDraw(TRUE);
  }
  void TLyHmTmSelect::ResetObj()
  {
    ugdrTeam_.SetDraw(FALSE);
    ugdrTeam_.SelectUra(FALSE);
    ugdrShot_.SetDraw(FALSE);
    ugdrShot_.SelectUra(FALSE);
    ugbtn_prev_->SetDraw(FALSE);
    ugbtn_next_->SetDraw(FALSE);
    ugbtn_mid_->SetDraw(FALSE);
    ugbtn_ok_->SetDraw(FALSE);
  }
  void TLyHmTmSelect::DoChangePage(s32 pageno)
  {
    switch ((enPage)pageno)
    {
    case db::pg_Menu:
      DrawList(&uglistMenu_);
      RefreshMenuList();
      break;
    case db::pg_ShSet:
      DrawList(&uglistShSet_);
      ugdrShot_.SetDraw(TRUE);
      ugbtn_prev_->SetFont("ならびかえ");
      ugbtn_prev_->SetDraw(TRUE);
      ugbtn_next_->SetFont("くわしく");
      ugbtn_next_->SetDraw(TRUE);
      ugbtn_mid_->SetFont("プレビュー");
      ugbtn_mid_->SetDraw(TRUE);
      ugbtn_ok_->SetFont("おーけー！");
      ugbtn_ok_->SetDraw(stSelect_.changed_f_);
      ugbtn_back_->SetFont("もどる");
      break;
    case db::pg_Filter:
      DrawList(&uglistTmFilter_);
      ugdrTeam_.SetDraw(TRUE);
      break;
    case db::pg_TeamSelect:
      DrawList(&uglistShSetMenu_);
      ugbtn_prev_->SetFont("ならびかえ");
      ugbtn_prev_->SetDraw(TRUE);
      ugbtn_next_->SetFont("くわしく");
      ugbtn_next_->SetDraw(TRUE);
      if (swshsetvec_.size() > 0)
      {
        ugbtn_back_->SetFont("おーけー！");
      }
      else
      {
        ugbtn_back_->SetFont("もどる");
      }
      ugdrTeam_.SetDraw(TRUE);
      uglistShSetMenu_.Ready(TRUE);
      //SelectShSet();
      break;
    case db::pg_ShSetSelect:
      DrawList(&uglistShSetSelect_);
      uglistShSetSelect_.SetDraw(TRUE);
      uglistMember_.SetDraw(TRUE);
      ugbtn_back_->SetFont("もどる");
      break;
    case db::pg_MemberSelect:
      DrawList(&uglistSoubi_);
      //ugbtn_mid_->SetFont("いちらん");
      //ugbtn_mid_->SetDraw(TRUE);
      ugbtn_ok_->SetFont("そうび");
      ugbtn_ok_->SetDraw(TRUE);
      ugbtn_back_->SetFont("もどる");
      stSelect_.changed_f_ = FALSE;
      uglistMember_.SetDraw(TRUE);
      uglistSoubi_.Refresh(stSelect_.id_team_, stSelect_.mid_team_, stSelect_.id_shset_, stSelect_.posno_, &shsetvec_, TRUE);
      uglistSoubi_.SwitchEdit(FALSE);
      break;
    case db::pg_MemSoubi:
      DrawList(&uglistSoubi_);
      uglistSoubi_.SwitchEdit(TRUE);
      ugdrShot_.SetDraw(TRUE);
      ugbtn_prev_->SetFont("ならびかえ");
      ugbtn_prev_->SetDraw(TRUE);
      ugbtn_next_->SetFont("くわしく");
      ugbtn_next_->SetDraw(TRUE);
      ugbtn_mid_->SetFont("プレビュー");
      ugbtn_mid_->SetDraw(TRUE);
      ugbtn_ok_->SetFont("おーけー！");
      ugbtn_ok_->SetDraw(stSelect_.changed_f_);
      ResetPrevBtn();
      break;
    case db::pg_AllSoubi:
      DrawList(&uglistSoubiAll_);
      ugdrShot_.SetDraw(TRUE);
      ugbtn_prev_->SetFont("ならびかえ");
      ugbtn_prev_->SetDraw(TRUE);
      ugbtn_next_->SetFont("くわしく");
      ugbtn_next_->SetDraw(TRUE);
      ugbtn_mid_->SetFont("プレビュー");
      ugbtn_mid_->SetDraw(TRUE);
      ugbtn_ok_->SetFont("おーけー！");
      ugbtn_ok_->SetDraw(stSelect_.changed_f_);
      ResetPrevBtn();
      break;
    case db::pg_Syn:
      DrawList(&uglistSyn_);
      ugdrShot_.SetDraw(TRUE);
      ugbtn_prev_->SetFont("プレビュー");
      ugbtn_prev_->SetDraw(TRUE);
      ugbtn_back_->SetFont("もどる");
      break;
    case db::pg_ShFilter:
      DrawList(&uglistShFilter_);
      ugdrShot_.SetDraw(TRUE);
      ugbtn_prev_->SetFont("くわしく");
      ugbtn_prev_->SetDraw(TRUE);
      ugbtn_next_->SetFont("プレビュー");
      ugbtn_next_->SetDraw(TRUE);
      break;
    case db::pg_ShSetRename:
      DrawList(NULL);
      frame_->SetHudInput(SHSETNAMEMAX, FALSE);
      ugbtn_back_->SetDraw(FALSE);
      break;
    case db::pg_ShPrev:
    {
      TPrevParam param;
      param.id_team = stSelect_.id_team_;
      param.id_shot = stSelect_.id_shot_;
			param.nonehavemixbtn_f = TRUE;

      DrawList(NULL);
      SetHudPrev(param);
      ugbtn_back_->SetDraw(FALSE);
    }
    break;
    case db::pg_ShPrevSyn:
    {
      SOZAIDTVECTOR sozaivec = pmgEO_->mgCoU_.GetSozaiDataSyn(stSelect_.id_shot_);
      TPrevParam param;
      param.id_team = stSelect_.id_team_;
      param.char0 = stSelect_.char0_;
      param.char1 = stSelect_.char1_;
      param.char2 = stSelect_.char2_;
      param.char3 = stSelect_.char3_;
      param.char4 = stSelect_.char4_;
      param.nonemixbtn_f = TRUE;
      param.sozaivec = &sozaivec;
			param.nonehavemixbtn_f = TRUE;

      DrawList(NULL);
      SetHudPrev(param);
      //SetHudPrevChangeShot(sozaivec);
      ugbtn_back_->SetDraw(FALSE);
    }
    break;

    }
  }
  //接続完了
  void TLyHmTmSelect::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_Omakase: Connect_Omakase(); break;
        case cn_AttachCustom: Connect_AttachCustom(); break;
        case cn_AttachShSet: Connect_AttachShSet(); break;
        case cn_SwitchShSet: Connect_SwitchShSet(); break;
        case cn_RenameShSet: Connect_RenameShSet(); break;
        }
      }
      else
      {
        //エラーメッセージ
        EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
        ChangePage(pg_Menu);
      }
    }
    else
    {
      //通信エラー
      ChangePage(pg_Menu);
    }
  }


  void TLyHmTmSelect::DoUpdate(BOOL draw_f)
  {
    ugdrTeam_.Update(draw_f);
    ugdrShot_.Update(draw_f);

    uglistMember_.Update(draw_f);
    uglistShSetSelect_.Update(draw_f);
    uglistSoubi_.Update(draw_f);
    uglistSoubiAll_.Update(draw_f);
    uglistMenu_.Update(draw_f);
    uglistShSetMenu_.Update(draw_f);
    uglistShSet_.Update(draw_f);
    uglistTmFilter_.Update(draw_f);
    uglistShFilter_.Update(draw_f);
    uglistSyn_.Update(draw_f);

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case pg_Menu:MvPage_Menu(); break;
      case pg_ShSet:MvPage_ShSet(); break;
      case pg_Filter:MvPage_TmFilter(); break;
      case pg_TeamSelect:MvPage_TeamSelect(); break;
      case pg_ShSetSelect:MvPage_ShSetSelect(); break;
      case pg_MemberSelect:MvPage_MemberSelect(); break;
      case pg_MemSoubi:MvPage_MemSoubi(); break;
      case pg_AllSoubi:MvPage_AllSoubi(); break;
      case pg_Syn:MvPage_Syn(); break;
      case pg_ShFilter:MvPage_ShFilter(); break;
      case pg_ShSetRename:MvPage_ShSetRename(); break;
      case pg_ShPrev:MvPage_Prev(); break;
      case pg_ShPrevSyn:MvPage_PrevSyn(); break;
      }
    }

  }

  //メインメニュー
  void TLyHmTmSelect::MvPage_Menu()
  {
    if (ugbtn_back_->IsAct())
    {
      Down();//もどる
    }
    else if (uglistMenu_.IsAct()) // メンバーきりかえ
    {
      switch ((enMenuList)uglistMenu_.GetActIndex())
      {
      case db::ml_TeamSelect:
        stSelect_.id_team_ = ugdrTeam_.GetIDTeam();
        stSelect_.mid_team_ = ugdrTeam_.GetMIDTeam();
        stSelect_.id_shset_ = pmgEO_->mgCoU_.mdTeam_.GetPKDt(stSelect_.id_team_, mdteam_id_shset);
        stSelect_.posno_ = 0;
        ugdrShot_.SetIDTeam(stSelect_.id_team_);
        //シュートセットマップのクリア
        teamshsetmap_.clear();
        for (s32 i = 0; i < pmgEO_->mgCoU_.mdTeam_.GetLineNum(); ++i)
        {
          teamshsetmap_[pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_pk)] = pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_id_shset);
        }
        //シュートセットマップクリア
        swshsetvec_.clear();
        swteamvec_.clear();
        ChangePage(pg_TeamSelect);
        break;
      default:
        //選択したシュートセット
        stSelect_.id_shset_ = uglistMenu_.GetCommonID();
        stSelect_.slot_ = 0;
        stSelect_.changed_f_ = FALSE;
        uglistShSet_.Refresh(stSelect_.id_shset_, &shsetvec_, TRUE);
        uglistShFilter_.Ready(enShotFilTypeS_All, &shsetvec_);
        ugdrShot_.ReadyFilter();
        ChangePage(pg_ShSet);
        break;
      }
    }
    else if (ugbtn_prev_->IsAct())//ならびかえ
    {
      ChangePage(pg_Filter);
    }
    else if (ugbtn_next_->IsAct())//チーム裏面
    {
      ugdrTeam_.ura_.SetTeamUraDt(ugdrTeam_.GetMIDTeam());
      ugdrTeam_.RevUra();
    }

  }
  //シュートセットをセット
  void TLyHmTmSelect::ShSetAtattch(BOOL dettach_f)
  {
    stSelect_.slot_ = uglistShSet_.GetActIndex();
    stSelect_.id_shot_ = ugdrShot_.GetIDShot();
    s64 idshot = shsetvec_[stSelect_.slot_];
    shsetvec_[stSelect_.slot_] = (dettach_f)
      ? NGID
      : stSelect_.id_shot_;
    uglistShSet_.Refresh(stSelect_.id_shset_, &shsetvec_, FALSE);
    uglistShFilter_.Ready(enShotFilTypeS_All, &shsetvec_);
    ugdrShot_.ReadyFilter();

    if (idshot != 0) ugdrShot_.DrumRoll_IDShot(idshot);
    stSelect_.changed_f_ = TRUE;
    ugbtn_ok_->SetDraw(TRUE);
  }
  //シュートセット選択
  void TLyHmTmSelect::MvPage_ShSet()
  {
    //ドラムがないときくわしくとプレビューけす
    ResetPrevBtn();

    if (frame_->IsHudOK())
    {
      ChangePage(pg_Menu);
    }
    else if (ugbtn_back_->IsAct())//おーけー
    {
      if (stSelect_.changed_f_)
      {
        STRVECTOR strvec = { "へんこうかしょが＿あります", "ほぞんせず＿もとにもどしますか？" };
        SetHudMsg(TRUE, FALSE, &strvec);
      }
      else
      {
        ChangePage(pg_Menu);
      }
    }
    else if (ugbtn_ok_->IsAct())//確定
    {
      if (stSelect_.changed_f_)
      {
        //違うときだけ//POST：装備取り付け
        Post_AttachShSet();
      }
    }
    else if (ugbtn_prev_->IsAct())//ならびかえ
    {
      ChangePage(pg_ShFilter);
      stSelect_.shfilterPage_ = stLy_.lastpageNo_;
    }
    else if (ugbtn_next_->IsAct())//裏面
    {
      stSelect_.id_shot_ = ugdrShot_.GetIDShot();//有効な中の番号
      ugdrShot_.ura_.SetShotUraDt(stSelect_.id_shot_);
      ugdrShot_.RevUra();
    }
    else if (ugbtn_mid_->IsAct())//プレビュー
    {
      stSelect_.id_shot_ = ugdrShot_.GetIDShot();
      ChangePage(pg_ShPrev);
      stLy_.lastpageNo_ = pg_ShSet;
    }
    else if (uglistShSet_.IsActRes(enUGLiAct_SubA))//リネーム
    {
      ChangePage(pg_ShSetRename);
    }
    else if (uglistShSet_.IsActRes(enUGLiAct_SubB))//オート
    {
      s32 i = 0;
      if (uglistShFilter_.id_shotvec_.size() > 0)
      {
        for (auto& item : shsetvec_)
        {
          //空か無効なのに入りっぱなし
          if ((item == NGID) || (pmgEO_->mgCoU_.mdShot_.GetPKDt(item, mdshot_state) == 0))
          {

            item = uglistShFilter_.id_shotvec_[i];
            ++i;
          }
          if (i >= uglistShFilter_.id_shotvec_.size()) break;
        }
      }
      uglistShSet_.Refresh(stSelect_.id_shset_, &shsetvec_, FALSE);
      uglistShFilter_.Ready(enShotFilTypeS_All, &shsetvec_);
      ugdrShot_.ReadyFilter();

      stSelect_.changed_f_ |= (i > 0);
      ugbtn_ok_->SetDraw(stSelect_.changed_f_);

    }
    else if (uglistShSet_.IsActRes(enUGLiAct_SubC))//善消し
    {
      s32 i = 0;
      for (auto& item : shsetvec_)
      {
        if (item != NGID)
        {
          item = NGID;
          ++i;
        }
      }

      for (auto& item : shsetvec_) item = NGID;
      uglistShSet_.Refresh(stSelect_.id_shset_, &shsetvec_, FALSE);
      uglistShFilter_.Ready(enShotFilTypeS_All, &shsetvec_);
      ugdrShot_.ReadyFilter();

      stSelect_.changed_f_ = TRUE;
      ugbtn_ok_->SetDraw(TRUE);
    }
    else if (uglistShSet_.IsActRes(enUGLiAct_RecordBtn))
    {
      ShSetAtattch(TRUE);
    }
    else if (uglistShSet_.IsActRes(enUGLiAct_Record))//shotりすと
    {
      ShSetAtattch(FALSE);
    }
  }

  void TLyHmTmSelect::MvPage_ShSetRename()
  {
    if (frame_->IsHudCancel())
    {
      ChangePage(pg_ShSet);//もどる
    }
    else if (frame_->IsHudOK())//確定ボタンにかえる
    {
      inputname_ = frame_->HudOutputString();
      //POSTリネーム
      Post_RenameShSet();
    }
  }
  //共通チームフィルター
  void TLyHmTmSelect::MvPage_TmFilter()
  {
    if (ugbtn_back_->IsAct())//おーけー
    {
      ChangePage(stLy_.lastpageNo_);
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
  ////チームごとのシュートセット選択
  //void TLyHmTmSelect::SelectShSet()
  //{
  //  stSelect_.posno_ = 0;
  //  stSelect_.id_shset_ = teamshsetmap_[stSelect_.id_team_];
  //  if (stSelect_.id_shset_ == 0)
  //  {
  //    uglistShSetMenu_.SetSelect(pmgEO_->mgCoU_.mdShSet_.GetLineNum());
  //  }
  //  else
  //  {
  //    for (s32 i = 0; i < pmgEO_->mgCoU_.mdShSet_.GetLineNum(); ++i)
  //    {
  //      if (stSelect_.id_shset_ == pmgEO_->mgCoU_.mdShSet_.GetLineDt(i, mdshset_pk))
  //      {
  //        uglistShSetMenu_.SetSelect(i);
  //        break;
  //      }
  //    }
  //  }
  //}


  void TLyHmTmSelect::ClearListSoubi(BOOL init_f)
  {
    shsetvec_.clear();
    for (s32 i = 0; i < SLOTALL; ++i)
    {
      s64 id_shot = pmgEO_->mgCoU_.mdTeam_.GetPKDt(stSelect_.id_team_, (enmdTeam)(mdteam_id_shot00 + i));
      shsetvec_.push_back(id_shot);
    }
    stSelect_.id_shset_ = 0;

    //かすたむ
    uglistSoubi_.Refresh(stSelect_.id_team_, stSelect_.mid_team_, 0, stSelect_.posno_, &shsetvec_, init_f);
    uglistSoubiAll_.Refresh(stSelect_.id_team_, &shsetvec_, init_f);
    uglistSoubi_.SwitchEdit(FALSE);

    uglistShFilter_.Ready(enShotFilTypeS_All, &shsetvec_);//リストに載ったやつをドラムから外す
    ugdrShot_.ReadyFilter();
  }


  void TLyHmTmSelect::ShSetToAll(s32 shsetno)
  {

    s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetLineDt(stSelect_.mid_team_, mdm_team_rarity);
    s32 overlap = pmgEO_->mgCoU_.mdTeam_.GetPKDt(stSelect_.id_team_, mdteam_overlap);

    s32 i = 0;
    for (auto& item : shsetvec_)
    {
      s32 needover = pmgEO_->mgCoM_.mdm_Slot_.GetLineDt(rarity, (enmdm_Slot)(mdm_slot_slot00 + i));//必要なオーバーラップ
      if (needover > overlap)//レベル足りてない
      {
        item = 0;
      }
      else
      {
        item = pmgEO_->mgCoU_.mdShSet_.GetLineDt(shsetno, (enmdShSet)(mdshset_id_shot00 + i));
      }
      ++i;
    }
    uglistSoubiAll_.Refresh(stSelect_.id_team_, &shsetvec_, FALSE);
    uglistShFilter_.Ready(enShotFilTypeS_All, &shsetvec_);//リストに載ったやつをドラムから外す
    ugdrShot_.ReadyFilter();
    stSelect_.changed_f_ = TRUE;
    ugbtn_ok_->SetDraw(TRUE);
  }


  //チーム選択
  void TLyHmTmSelect::MvPage_TeamSelect()
  {
    if (ugbtn_back_->IsAct())
    {
      if (swteamvec_.size() > 0)
      {
        stSelect_.nextPage_ = pg_Menu;
        Post_SwitchShSet();
      }
      else
      {
        ChangePage(pg_Menu);
      }
    }
    else if (uglistShSetMenu_.IsAct())
    {

      stSelect_.id_team_ = ugdrTeam_.GetIDTeam();
      stSelect_.mid_team_ = ugdrTeam_.GetMIDTeam();

      switch (uglistShSetMenu_.GetActIndex())
      {
      case tsml_ShSet:
        ChangePage(pg_ShSetSelect);
        uglistShSetSelect_.Refresh(stSelect_.id_team_, NGNUM, shsetvec_);
        uglistMember_.Refresh(stSelect_.id_team_, stSelect_.mid_team_, FALSE);
        break;
      case tsml_Custom:
        ClearListSoubi(TRUE);
        ChangePage(pg_MemberSelect);
        uglistMember_.Refresh(stSelect_.id_team_, stSelect_.mid_team_, TRUE);
        break;
      case tsml_CustomAll:
        ChangePage(pg_AllSoubi);
        uglistSoubiAll_.Refresh(stSelect_.id_team_, &shsetvec_, TRUE);
        uglistShFilter_.Ready(enShotFilTypeS_All, &shsetvec_);
        ugdrShot_.ReadyFilter();
        break;
      case tsml_Syn:
        ChangePage(pg_Syn);
        uglistSyn_.Refresh(stSelect_.mid_team_);
        stSelect_.id_shot_ = uglistSyn_.GetCommonID_index(0);
        ugdrShot_.ReadyType_Syn(uglistSyn_.GetCommonID_index(0));
        break;
      }
    }
    else if (ugbtn_prev_->IsAct())//ならびかえ
    {
      ChangePage(pg_Filter);
    }
    else if (ugbtn_next_->IsAct())//チーム裏面
    {
      ugdrTeam_.ura_.SetTeamUraDt(ugdrTeam_.GetMIDTeam());
      ugdrTeam_.RevUra();
    }
    else if (ugdrTeam_.IsAct())//読み込みが必要なタイミング
    {
      stSelect_.id_team_ = ugdrTeam_.GetIDTeam();
      stSelect_.mid_team_ = ugdrTeam_.GetMIDTeam();
      //
      //#if defined(__K_DEBUG_HOME_TEAM__) || defined(__K_DEBUG_USER_DATA__)
      //      kdebug::DebugUDManager::GetInstance()->SetLastTeamID(stSelect_.mid_team_);
      //#endif	// #if defined(__K_DEBUG_HOME_TEAM__) || defined(__K_DEBUG_USER_DATA__)
    }
  }

  //シュートセット切り替え
  void TLyHmTmSelect::MvPage_ShSetSelect()
  {
    if (ugbtn_back_->IsAct())//おーけー
    {
      if (stSelect_.id_shset_ != uglistShSetSelect_.GetShSet())
      {
        stSelect_.id_shset_ = uglistShSetSelect_.GetShSet();
        Post_SwitchShSet();
      }
      else
      {
        ChangePage(pg_TeamSelect);
      }

      ////if (uglistShSetSelect_.IsChangedShSet())
      ////{
      ////  stSelect_.id_shset_ = uglistShSetSelect_.GetShSet();
      ////  Post_SwitchShSet();
      ////}
      ////else
      //{
      //  ChangePage(pg_TeamSelect);
      //}
    }
  }


  //メンバー選択
  void TLyHmTmSelect::MvPage_MemberSelect()
  {
    if (ugbtn_back_->IsAct())//おーけー
    {
      ChangePage(pg_TeamSelect);
    }
    //else if (ugbtn_mid_->IsAct())//いちらん
    //{
    //  ChangePage(pg_AllSoubi);
    //  uglistSoubiAll_.Refresh(stSelect_.id_team_, &shsetvec_, FALSE);
    //}
    else if (uglistMember_.IsActRes(enUGLiAct_Record)) // メンバーきりかえ
    {
      //選択したメンバー
      stSelect_.posno_ = uglistMember_.GetActIndex();
      uglistSoubi_.Refresh(stSelect_.id_team_, stSelect_.mid_team_, stSelect_.id_shset_, stSelect_.posno_, &shsetvec_, TRUE);
      uglistSoubi_.SwitchEdit(FALSE);
    }
    //else if (uglistSoubi_.IsActRes(enUGLiAct_Record))//shotりすと
    else if (ugbtn_ok_->IsAct() || uglistSoubi_.IsAct())//shotりすと
    {
      ChangePage(pg_MemSoubi);
      uglistSoubi_.SetSelect(NGNUM);
    }
  }
  //装備関数
  void TLyHmTmSelect::SoubiAtattch(BOOL dettach_f)
  {
    stSelect_.slot_ = uglistSoubi_.records_[uglistSoubi_.GetActIndex()]->GetSlot();
    stSelect_.id_shot_ = ugdrShot_.GetIDShot();
    s64 idshot = uglistSoubi_.records_[uglistSoubi_.GetActIndex()]->GetShotID();
    if (idshot != 0) ugdrShot_.DrumRoll_IDShot(idshot);

    shsetvec_[stSelect_.slot_] = (dettach_f)
      ? NGID
      : stSelect_.id_shot_;

    uglistSoubi_.Refresh(stSelect_.id_team_, stSelect_.mid_team_, 0, stSelect_.posno_, &shsetvec_, FALSE);
    uglistShFilter_.Ready(enShotFilTypeS_All, &shsetvec_);
    ugdrShot_.ReadyFilter();

    if (idshot != 0) ugdrShot_.DrumRoll_IDShot(idshot);
    stSelect_.changed_f_ = TRUE;
    ugbtn_ok_->SetDraw(TRUE);

  }
  //カスタム装備：個別
  void TLyHmTmSelect::MvPage_MemSoubi()
  {
    //ドラムがないときくわしくとプレビューけす
    ResetPrevBtn();

    if (frame_->IsHudOK())
    {
      //装備リスト初期化
      ClearListSoubi(FALSE);
      ChangePage(pg_MemberSelect);
    }
    else if (ugbtn_back_->IsAct())//おーけー
    {
      if (stSelect_.changed_f_)
      {
        STRVECTOR strvec = { "へんこうかしょが＿あります", "ほぞんせず＿もとにもどしますか？" };
        SetHudMsg(TRUE, FALSE, &strvec);
      }
      else
      {
        ChangePage(pg_MemberSelect);
      }
    }
    else if (ugbtn_ok_->IsAct())//確定
    {
      if (stSelect_.changed_f_)
      {
        //違うときだけ//POST：装備取り付け
        stSelect_.nextPage_ = pg_MemberSelect;
        Post_AttachCustom();
      }
    }
    else if (ugbtn_prev_->IsAct())//ならびかえ
    {
      ChangePage(pg_ShFilter);
      stSelect_.shfilterPage_ = stLy_.lastpageNo_;
    }
    else if (ugbtn_next_->IsAct())//裏面
    {
      stSelect_.id_shot_ = ugdrShot_.GetIDShot();//有効な中の番号
      ugdrShot_.ura_.SetShotUraDt(stSelect_.id_shot_);
      ugdrShot_.RevUra();
    }
    else if (ugbtn_mid_->IsAct())//プレビュー
    {
      stSelect_.id_shot_ = ugdrShot_.GetIDShot();//有効な中の番号
      ChangePage(pg_ShPrev);
    }
    else if (uglistSoubi_.IsActRes(enUGLiAct_RecordBtn))
    {
      SoubiAtattch(TRUE);
    }
    else if (uglistSoubi_.IsActRes(enUGLiAct_Record))//shotりすと
    {
      SoubiAtattch(FALSE);
    }
  }


  //シュートセットをセット
  void TLyHmTmSelect::SoubiAllAtattch(BOOL dettach_f)
  {
    stSelect_.slot_ = uglistSoubiAll_.GetActIndex();
    stSelect_.id_shot_ = ugdrShot_.GetIDShot();
    s64 idshot = shsetvec_[stSelect_.slot_];
    shsetvec_[stSelect_.slot_] = (dettach_f)
      ? NGID
      : stSelect_.id_shot_;
    uglistSoubiAll_.Refresh(stSelect_.id_team_, &shsetvec_, FALSE);
    uglistShFilter_.Ready(enShotFilTypeS_All, &shsetvec_);
    ugdrShot_.ReadyFilter();

    if (idshot != 0) ugdrShot_.DrumRoll_IDShot(idshot);
    stSelect_.changed_f_ = TRUE;
    ugbtn_ok_->SetDraw(TRUE);
  }


  //カスタム装備：全員
  void TLyHmTmSelect::MvPage_AllSoubi()
  {
    //ドラムがないときくわしくとプレビューけす
    ResetPrevBtn();

    if (frame_->IsHudOK())
    {
      ClearListSoubi(FALSE);
      ChangePage(pg_TeamSelect);
    }
    else if (ugbtn_back_->IsAct())//おーけー
    {
      if (stSelect_.changed_f_)
      {
        STRVECTOR strvec = { "へんこうかしょが＿あります", "ほぞんせず＿もとにもどしますか？" };
        SetHudMsg(TRUE, FALSE, &strvec);
      }
      else
      {
        ChangePage(pg_TeamSelect);
      }
    }
    else if (ugbtn_ok_->IsAct())//確定
    {
      if (stSelect_.changed_f_)
      {
        //違うときだけ//POST：装備取り付け
        stSelect_.nextPage_ = pg_TeamSelect;
        Post_AttachCustom();
      }
    }
    else if (ugbtn_prev_->IsAct())//ならびかえ
    {
      ChangePage(pg_ShFilter);
      stSelect_.shfilterPage_ = stLy_.lastpageNo_;
    }
    else if (ugbtn_next_->IsAct())//裏面
    {
      stSelect_.id_shot_ = ugdrShot_.GetIDShot();//有効な中の番号
      ugdrShot_.ura_.SetShotUraDt(stSelect_.id_shot_);
      ugdrShot_.RevUra();
    }
    else if (ugbtn_mid_->IsAct())//プレビュー
    {
      stSelect_.id_shot_ = ugdrShot_.GetIDShot();
      ChangePage(pg_ShPrev);
    }
    else if (uglistSoubiAll_.IsActRes(enUGLiAct_SubA))//オート
    {
      ShSetToAll(0);
    }
    else if (uglistSoubiAll_.IsActRes(enUGLiAct_SubB))//set1
    {
      ShSetToAll(1);
    }
    else if (uglistSoubiAll_.IsActRes(enUGLiAct_SubC))//set2
    {
      ShSetToAll(2);
    }
    else if (uglistSoubiAll_.IsActRes(enUGLiAct_SubD))//set3
    {
      ShSetToAll(3);
    }
    else if (uglistSoubiAll_.IsActRes(enUGLiAct_SubE))//善消し
    {
      for (auto& item : shsetvec_) item = NGID;
      uglistSoubiAll_.Refresh(stSelect_.id_team_, &shsetvec_, FALSE);
      uglistShFilter_.Ready(enShotFilTypeS_All, &shsetvec_);
      ugdrShot_.ReadyFilter();

      stSelect_.changed_f_ = TRUE;
      ugbtn_ok_->SetDraw(TRUE);
    }
    else if (uglistSoubiAll_.IsActRes(enUGLiAct_RecordBtn))
    {
      SoubiAllAtattch(TRUE);
    }
    else if (uglistSoubiAll_.IsActRes(enUGLiAct_Record))//shotりすと
    {
      SoubiAllAtattch(FALSE);
    }
  }

  //レシピ表示
  void TLyHmTmSelect::MvPage_Syn()
  {
    if (ugbtn_back_->IsAct())
    {
      ChangePage(pg_TeamSelect);
    }
    else if (ugbtn_prev_->IsAct())//まるひプレビュー
    {
      ChangePage(pg_ShPrevSyn);
    }
    else if (uglistSyn_.IsAct())
    {
      stSelect_.id_shot_ = uglistSyn_.GetCommonID();
      stSelect_.char0_ = uglistSyn_.GetActIndex();
      stSelect_.char1_ = (stSelect_.char0_ == 0)
        ? 1
        : 0;
      stSelect_.char2_ = (stSelect_.char0_ == 0)
        ? 2
        : 0;
      stSelect_.char3_ = (stSelect_.char0_ == 0)
        ? 3
        : 0;
      stSelect_.char4_ = (stSelect_.char0_ == 0)
        ? 4
        : 0;
      ugdrShot_.ReadyType_Syn(stSelect_.id_shot_);
    }
  }

  void TLyHmTmSelect::MvPage_ShFilter()
  {

    if (ugbtn_back_->IsAct())//おーけー
    {
      ChangePage(stSelect_.shfilterPage_);
    }
    else if (ugbtn_prev_->IsAct())//裏面
    {
      stSelect_.id_shot_ = ugdrShot_.GetIDShot();//有効な中の番号
      ugdrShot_.ura_.SetShotUraDt(stSelect_.id_shot_);
      ugdrShot_.RevUra();
    }
    else if (ugbtn_next_->IsAct())//プレビュー
    {
      stSelect_.id_shot_ = ugdrShot_.GetIDShot();
      ChangePage(pg_ShPrev);
    }
    else if (uglistShFilter_.IsActRes2(enUGLiShFilter_Filter))
    {
      ugdrShot_.ReadyFilter();
    }
    else if (uglistShFilter_.IsActRes2(enUGLiShFilter_Sort))
    {
      ugdrShot_.ReadySort();
    }
  }

  void TLyHmTmSelect::MvPage_Prev()
  {
    if (IsHudPrevBack())//おーけー
    {
      ChangeLastPage();
    }
    else if (IsHudPrevMix())
    {
      SetHudPrevMixShot(stSelect_.id_shot_, TRUE);
    }
    else if (IsHudPrevMixClear())
    {
      SetHudPrevMixShot(stSelect_.id_shot_, FALSE);
    }
  }
  void TLyHmTmSelect::MvPage_PrevSyn()
  {
    if (IsHudPrevBack())//おーけー
    {
      ChangeLastPage();
    }
  }

  //POST：おまかせ
  void TLyHmTmSelect::Post_Omakase()
  {
    if (pmgEO_->stSeq_.IsNoSv())
	{
		Connect_Omakase();
	}
	else
	{
		StartConnect(cn_Omakase);
		pmgEO_->mgPo_.poHmTeam_.Ready_attachshset(pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_shset), &shsetvec_);
	}
  }
  //POST：装備取り付け
  void TLyHmTmSelect::Post_AttachCustom()
  {
	  if (pmgEO_->stSeq_.IsNoSv())
	  {
		  Connect_AttachCustom();
	  }
	  else
	  {
		  StartConnect(cn_AttachCustom);
		  pmgEO_->mgPo_.poHmTeam_.Ready_attachcustom(stSelect_.id_team_, &shsetvec_);
	  }
  }

  //POST：装備取り付け
  void TLyHmTmSelect::Post_AttachShSet()
  {
	  if (pmgEO_->stSeq_.IsNoSv())
	  {
		  Connect_AttachShSet();
	  }
	  else
	  {

		  StartConnect(cn_AttachShSet);
		  pmgEO_->mgPo_.poHmTeam_.Ready_attachshset(stSelect_.id_shset_, &shsetvec_);
	  }
  }

  //POST：シュートセット切り替え
  void TLyHmTmSelect::Post_SwitchShSet()
  {
	  if (pmgEO_->stSeq_.IsNoSv())
	  {
		  Connect_SwitchShSet();
	  }
	  else
	  {
		  //StartConnect(cn_SwitchShSet);
		  //pmgEO_->mgPo_.poHmTeam_.Ready_switchshset(&swteamvec_, &swshsetvec_);
		  StartConnect(cn_SwitchShSet);
		  pmgEO_->mgPo_.poHmTeam_.Ready_switchshset(stSelect_.id_team_, stSelect_.id_shset_);
	  }
  }

  //POST：シュートセット名変更
  void TLyHmTmSelect::Post_RenameShSet()
  {
	  if (pmgEO_->stSeq_.IsNoSv())
	  {
		  Connect_RenameShSet();
	  }
	  else
	  {
		  StartConnect(cn_RenameShSet);
		  pmgEO_->mgPo_.poHmTeam_.Ready_renameshset(stSelect_.id_shset_, inputname_);
	  }
  }

  //通信：おまかせ
  void TLyHmTmSelect::Connect_Omakase()
  {
    ChangePage(pg_Menu);
  }
  //通信：装備取り付け
  void TLyHmTmSelect::Connect_AttachCustom()
  {
    ChangePage(stSelect_.nextPage_);
  }
  //通信：シュートセット編集
  void TLyHmTmSelect::Connect_AttachShSet()
  {
    ChangePage(pg_Menu);
  }

  //通信：シュートセット切り替え
  void TLyHmTmSelect::Connect_SwitchShSet()
  {
    swteamvec_.clear();
    swshsetvec_.clear();
    ugbtn_ok_->SetFont("もどる");
    ChangePage(pg_TeamSelect);
    //if (stSelect_.nextPage_ != NGNUM) ChangePage(stSelect_.nextPage_);
  }

  void TLyHmTmSelect::Connect_RenameShSet()
  {
    //ugdrShot_.RefreshSelected();
    ChangePage(pg_ShSet);
  }


  //ドラムがないときくわしくとプレビューけす
  void TLyHmTmSelect::ResetPrevBtn()
  {
    ugbtn_next_->SetDraw(ugdrShot_.IsData());
    ugbtn_prev_->SetDraw(ugdrShot_.IsData());
    ugbtn_mid_->SetDraw(ugdrShot_.IsData());
  }


}
