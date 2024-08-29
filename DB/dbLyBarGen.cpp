#include "dbLyBarGen.h"
#include "dbScene.h"
namespace db
{

  enum enPage
  {
    pg_Bar,
    pg_Profile,
    pg_Techno,
    pg_GGZ,
    pg_Filter,
    pg_FilterAll,
    pg_GotoTitle,
    pg_Error,
  };

  enum enHudWait
  {
    hud_Message,
    hud_GGZ,
    hud_GGZQuick,
  };

  enum enConnect
  {
    cn_GGZ,
    cn_GGZQuick,
  };

  //コンストラクタ
  TLyBarGen::TLyBarGen(TScene* pScene, TUIFrame* frame) :TUILayer(pScene, frame)
    , ugfilter_(this)
    , ugheadder_(this)
    , ugfooter_(this)
    , ugshiai_(this)
  {
    stBar_.Init();
    stLy_.barlayer_f_ = TRUE;
  }

  TLyBarGen::~TLyBarGen()
  {

  }
  void TLyBarGen::DoLoad()
  {
    ugfilter_.MakeGroup(field_);
    ugfilter_.OffFilter(2, FALSE);
    ugfooter_.MakeGroup(field_);
    ugheadder_.MakeGroup(field_);
    if (stBar_.shiai_f_) ugshiai_.MakeGroup(field_);
    DoLoadSub();
  }
  void TLyBarGen::SetFilterAll(BOOL gototitle_f)
  {
    stBar_.autooff_f_ = FALSE;
    if (gototitle_f)
    {
      ChangePage(pg_GotoTitle);
    }
    else
    {
      ChangePage(pg_FilterAll);
    }
  }
  void TLyBarGen::SetFilterAll_AutoOff()
  {
    stBar_.autooff_f_ = TRUE;
    ChangePage(pg_FilterAll);
  }
  void TLyBarGen::RefreshHeadder()
  {
    ugheadder_.Refresh(TRUE);
  }
  BOOL TLyBarGen::IsBarMove()
  {
    return stBar_.barmove_f_;
  }
  BOOL TLyBarGen::IsBarAction()
  {
    return (stLy_.pageNo_ != pg_Bar);
  }


  void TLyBarGen::GenOpenHeader()
  {
    stBar_.barmove_f_ = TRUE;
    DoOpenHeader();//レイヤのReadyが入るのでSetFoot_Noneを前に書かない
    ugfooter_.HideBtn(TRUE);
    ugheadder_.SetDisable();
  }
  void TLyBarGen::CloseHeader()
  {
    //SetFoot_Tab();tabとは限らない
    stBar_.barmove_f_ = FALSE;
    DoCloseHeader();
    ChangePage(pg_Bar);
    ugfooter_.HideBtn(FALSE);
    ugheadder_.SetEnable();
  }
  void TLyBarGen::JumpHeader(s32 pageno)
  {
    DoJumpHeader();
    ChangePage(pageno);
  }
  void TLyBarGen::JumpFooter(s32 tabno)
  {
	  ugfilter_.OnFilter(0, FALSE, TRUE);
	  stBar_.nexttabNo_ = tabno;
	  ugfooter_.SetAct(tabno);
	  ugfooter_.tabbtn_[stBar_.tabNo_]->SetSelect(FALSE);
	  ugfooter_.tabbtn_[tabno]->SetSelect(TRUE);
	  ChangePage(pg_Filter);
  }
  void TLyBarGen::OpenTechno()
  {
    DoOpenTechno();
    GenOpenHeader();
  }

  void TLyBarGen::OpenProfile()
  {
    DoOpenProfile();
    GenOpenHeader();
  }
  void TLyBarGen::OpenGGZ()
  {
    GenOpenHeader();

    s32 ggznum = pmgEO_->mgCoU_.GetSupplyNum_GGZ();
    if (ggznum > 0)
    {
      std::string strnum = "もっているかず：";
      strnum += lib_str::IntToStr(ggznum);
      STRVECTOR strvec = { "ぎんぎんぜっとを＿しようします", "よろしいですか？", strnum };
      SetHudMsg(TRUE, FALSE, &strvec);
    }
    else
    {
      SetHudMsgStarSupply(SPL_GGZ, TRUE);
    }
  }
  void TLyBarGen::SetNoneBar(BOOL nonebar_f)
  {
    ugheadder_.SetDraw(!nonebar_f);
    ugfooter_.SetDraw(!nonebar_f);
  }
  void TLyBarGen::SetMakeShiai()
  {
    stBar_.shiai_f_ = TRUE;
  }
  void TLyBarGen::DoChangePage(s32 pageno)
  {
    stBar_.action_f_ = FALSE;
    switch (pageno)
    {
    case db::pg_Bar:
      break;
    case db::pg_Profile:
      OpenProfile();
      break;
    case db::pg_Techno:
      OpenTechno();
      break;
    case db::pg_GGZ:
      OpenGGZ();
      break;
    case db::pg_Filter:
      ugfilter_.OnFilter(0, FALSE, TRUE);
      stBar_.nexttabNo_ = ugfooter_.GetActRes();
      break;
    case db::pg_FilterAll:
      ugfilter_.OnFilter(2, FALSE, stBar_.autooff_f_);
      break;
    case db::pg_GotoTitle:
      ugfilter_.OnFilter(2, FALSE, stBar_.autooff_f_);
      break;
    case db::pg_Error:
      break;
    }
  }

  void TLyBarGen::MvPage_Bar()
  {
    //スタミナバー更新
    if (lib_num::IncLoop(&stBar_.refresh_c_, 0, (30 * 60)))
    {
      ugheadder_.Refresh(FALSE);
    }

    //HUDの返答待ち
    if (ugheadder_.IsAct())//ヘッダアクション
    {
      //ぎんぎんぜっとボタン
      if (ugheadder_.IsActRes(enUGHeadderAct_ggz))
      {
        ChangePage(pg_GGZ);//ぎんぎんぜっと使用
      }
      else if (ugheadder_.IsActRes(enUGHeadderAct_techno))//クレジット
      {
        ChangePage(pg_Techno);
      }
      else if (ugheadder_.IsActRes(enUGHeadderAct_profile))//プロフ
      {
        ChangePage(pg_Profile);
      }
    }
    else if (ugfooter_.IsAct())//フッタアクション
    {
      if (ugfooter_.IsTab())
      {
        if (stBar_.tabNo_ != ugfooter_.GetActRes())
        {
          //ヘッダフッタ間フィルタ
          ugfilter_.OnFilter(0, FALSE, TRUE);
          stBar_.nexttabNo_ = ugfooter_.GetActRes();
          ChangePage(pg_Filter);
        }
      }
      else
      {
        //現状Back以外も受け取る必要になったら増やす
        if (ugfooter_.GetActRes() == 0)
        {
          DoBackBtn();
        }
      }
    }

  }
  void TLyBarGen::MvPage_Profile()
  {
    //ぎんぎんぜっとボタン
    if (ugheadder_.IsActRes(enUGHeadderAct_ggz))
    {
      JumpHeader(pg_GGZ);
    }
    else if (ugheadder_.IsActRes(enUGHeadderAct_techno))//クレジット
    {
      JumpHeader(pg_Techno);
    }
  }
  void TLyBarGen::MvPage_Techno()
  {
    //ぎんぎんぜっとボタン
    if (ugheadder_.IsActRes(enUGHeadderAct_ggz))
    {
      JumpHeader(pg_GGZ);
    }
    else if (ugheadder_.IsActRes(enUGHeadderAct_profile))//プロフ
    {
      JumpHeader(pg_Profile);
    }
  }
  void TLyBarGen::MvPage_GGZ()
  {
    if (IsHudOK())//おーけー
    {
      if (pmgEO_->mgCoU_.GetSupplyNum_GGZ() > 0)
      {
        Post_GGZ();
      }
      else
      {
        Post_GGZQuick();
      }
    }
    else if (IsHudCancel())//きゃんせる
    {
      CloseHeader();
    }
    else if (ugheadder_.IsActRes(enUGHeadderAct_techno))//クレジット
    {
      JumpHeader(pg_Techno);
    }
    else if (ugheadder_.IsActRes(enUGHeadderAct_profile))//プロフ
    {
      JumpHeader(pg_Profile);
    }
  }

  void TLyBarGen::MvPage_Filter()
  {
    if (ugfilter_.IsAct())//フィルタアクション
    {
      DoMoveFilter();
      ChangePage(pg_Bar);//通常営業に戻る
    }
  }
  void TLyBarGen::MvPage_FilterAll()
  {
    if (ugfilter_.IsAct())//フィルタアクション
    {
      DoMoveFilterAll();
      ChangePage(pg_Bar);//通常営業に戻る
    }
  }
  void TLyBarGen::MvPage_GotoTitle()
  {
    if (ugfilter_.IsAct())//フィルタアクション
    {
      //ホームシーン切り替わり予約
      pGenScene_->SetGotoTitle();
    }
  }
  void TLyBarGen::MvPage_Error()
  {
    if (IsHudOK())//おーけー
    {
      CloseHeader();
    }
  }

  //POST：ぎんぎんぜっと
  void TLyBarGen::Post_GGZ()
  {
    StartConnect(cn_GGZ);
    pmgEO_->mgPo_.poHome_.Ready_ggz();
  }
  //POST：ぎんぎんぜっと購入使用
  void TLyBarGen::Post_GGZQuick()
  {
    StartConnect(cn_GGZQuick);
    pmgEO_->mgPo_.poHome_.Ready_ggzquick();
  }

  //接続：ぎんぎんぜっと
  void TLyBarGen::Connect_GGZ()
  {
    //ヘッダリフレッシュ必要であればメッセージ
    RefreshHeadder();
    CloseHeader();
  }
  //接続：ぎんぎんぜっと購入使用
  void TLyBarGen::Connect_GGZQuick()
  {
    //ヘッダリフレッシュ必要であればメッセージ
    RefreshHeadder();
    CloseHeader();
  }

  //接続完了
  void TLyBarGen::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_GGZ: Connect_GGZ(); break;
        case cn_GGZQuick: Connect_GGZQuick(); break;
        }
      }
      else
      {
        //エラーメッセージ
        EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
        //一応ヘッダ・フッタ元に戻す
        ChangePage(pg_Error);
      }
    }
    else
    {
      ChangePage(pg_Error);
    }
  }

  //pg_Bar,
  void TLyBarGen::DoUpdate(BOOL draw_f)
  {
    ugfilter_.Update(draw_f);
    ugheadder_.Update(draw_f);
    ugfooter_.Update(draw_f);
    DoUpdateSub(draw_f);

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case db::pg_Bar: MvPage_Bar(); break;
      case db::pg_Profile: MvPage_Profile(); break;
      case db::pg_Techno: MvPage_Techno(); break;
      case db::pg_GGZ: MvPage_GGZ(); break;
      case db::pg_Filter: MvPage_Filter(); break;
      case db::pg_FilterAll: MvPage_FilterAll(); break;
      case db::pg_GotoTitle: MvPage_GotoTitle(); break;
      case db::pg_Error: MvPage_Error(); break;
      }
    }
  }


}