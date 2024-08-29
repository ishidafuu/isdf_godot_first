#include "dbLyShiaiTutoRetro.h"
#include "dbSnShiai.h"
namespace db
{
  enum enPage
  {
    pg_Intro,
    pg_Dodge,
    pg_DodgeAct,
    pg_DodgeEnd,
    pg_Catch,
    pg_CatchAct,
    pg_CatchEnd,
    pg_Walk,
    pg_WalkAct,
    pg_WalkEnd,
    pg_Dash,
    pg_DashAct,
    pg_DashEnd,
    pg_Jump,
    pg_JumpAct,
    pg_JumpEnd,
    pg_Shot,
    pg_ShotAct,
    pg_ShotEnd,
    pg_Game,
    pg_GameAct,
    pg_Fin,
    pg_Idle,
  };

  //コンストラクタ
  TLyShiaiTutoRetro::TLyShiaiTutoRetro(TSnShiai* pScene, TUGShiai* ugshiai) :TLyShiaiTutoGen(pScene, ugshiai)
  {
  }

  TLyShiaiTutoRetro::~TLyShiaiTutoRetro()
  {
    Freemem(ugtuto_);
  }

  void TLyShiaiTutoRetro::DoLoadSub()
  {
    ugtuto_ = new TUGTutoMsgRetro(this);
    ugtuto_->MakeGroup(field_);
  }
  void TLyShiaiTutoRetro::DoReadySub()
  {
    ugtuto_->Ready();
  }

  //ページ切り替え
  void TLyShiaiTutoRetro::DoChangePage(s32 pageno)
  {
    ugfinger_.SetDraw(FALSE);

    switch (pageno)
    {
    case db::pg_Intro:
      SetPageData(pageno, sta_Intro, tpt_Info);
      ugshiai_->ugbtn_time_.SetDraw(TRUE);
      break;
    case db::pg_Dodge:SetPageData(pageno, sta_Dodge, tpt_Info); break;
    case db::pg_DodgeAct:SetPageData(pageno, sta_DodgeAct, tpt_Act); break;
    case db::pg_DodgeEnd:SetPageData(pageno, sta_Dodge, tpt_End); break;
    case db::pg_Catch:SetPageData(pageno, sta_Catch, tpt_Info); break;
    case db::pg_CatchAct:SetPageData(pageno, sta_CatchAct, tpt_Act); break;
    case db::pg_CatchEnd:SetPageData(pageno, sta_Catch, tpt_End); break;
    case db::pg_Walk:SetPageData(pageno, sta_Walk, tpt_Info); break;
    case db::pg_WalkAct:SetPageData(pageno, sta_WalkAct, tpt_Act); break;
    case db::pg_WalkEnd:SetPageData(pageno, sta_Walk, tpt_End); break;
    case db::pg_Dash:SetPageData(pageno, sta_Dash, tpt_Info); break;
    case db::pg_DashAct:SetPageData(pageno, sta_DashAct, tpt_Act); break;
    case db::pg_DashEnd:SetPageData(pageno, sta_Dash, tpt_End); break;
    case db::pg_Jump:SetPageData(pageno, sta_Jump, tpt_Info); break;
    case db::pg_JumpAct:SetPageData(pageno, sta_JumpAct, tpt_Act); break;
    case db::pg_JumpEnd:SetPageData(pageno, sta_Jump, tpt_End); break;
    case db::pg_Shot:SetPageData(pageno, sta_Shot, tpt_Info); break;
    case db::pg_ShotAct:SetPageData(pageno, sta_ShotAct, tpt_Act); break;
    case db::pg_ShotEnd:SetPageData(pageno, sta_Shot, tpt_End); break;
    case db::pg_Game:SetPageData(pageno, sta_Game, tpt_Info); break;
    case db::pg_GameAct:SetPageData(pageno, sta_GameAct, tpt_Act); break;
    case db::pg_Fin:SetPageData(pageno, sta_Fin, tpt_Info); break;
    }

    const s32 FYB = 64 + 8;
    const s32 FYL = 64 - 8;
    const s32 FYJ = -40;

    const s32 FXA = 136 + 8;
    const s32 FXB = 56 + 8;
    const s32 FXL = -136 - 8;
    const s32 FXJ = -136;

    switch (pageno)
    {
    case db::pg_DodgeAct:ugfinger_.SetMotionLongTouch(FXA, FYB); break;
    case db::pg_CatchAct:ugfinger_.SetMotionTouch(FXB, FYB); break;
    case db::pg_WalkAct:ugfinger_.SetMotionGrgr(FXL, FYL); break;
    case db::pg_DashAct:ugfinger_.SetMotionFrickLR(FXL, FYL); break;
    case db::pg_JumpAct:ugfinger_.SetMotionTouch(FXJ, FYJ); break;
    case db::pg_ShotAct:ugfinger_.SetMotionTouch(FXB, FYB); break;
    case db::pg_GameAct:
      //ugshiai_をノーマルに戻す
      ugshiai_->SendTutoGameAct(pScene_->pmgEO_->stSeq_.IsFirstTuto() == FALSE);
      ugtuto_->SetDraw(FALSE);
      if (pScene_->pmgEO_->stSeq_.IsFirstTuto() == FALSE)
      {
        stTuto_.normal_f_ = TRUE;
        pmgEO_->stShiai_.SetTutoToNormal();//stShiai_をノーマルに戻す
      }
      break;
    case db::pg_Fin:
      ugtuto_->SetDraw(TRUE);
      ugtuto_->ugserifu_.SetDepth(UIDP_09MSGBOX);
      ugshiai_->ugbtn_time_.SetDraw(FALSE);
      break;
    }

    switch (pageno)
    {
    case db::pg_Dodge: ugshiai_->SetRetroStep(1, TUGShPad::enPadMsgType::pmt_TutoDf); break;
    case db::pg_Catch: ugshiai_->SetRetroStep(2, TUGShPad::enPadMsgType::pmt_TutoDf); break;
    case db::pg_Walk: ugshiai_->SetRetroStep(3, TUGShPad::enPadMsgType::pmt_TutoDf); break;
    case db::pg_Jump: ugshiai_->SetRetroStep(4, TUGShPad::enPadMsgType::pmt_TutoDf); break;
    case db::pg_Shot: ugshiai_->SetRetroStep(5, TUGShPad::enPadMsgType::pmt_TutoOf); break;
    case db::pg_GameAct: ugshiai_->SetRetroStep(6, TUGShPad::enPadMsgType::pmt_Normal); break;
      //case db::pg_GameAct: ugshiai_->SetRetroStep(5, TRUE); break;
    }
  }

  void TLyShiaiTutoRetro::DoUpdateSub(BOOL draw_f)
  {

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case db::pg_GameAct:
        if (stTuto_.normal_f_)
        {
          MvPage_GameAct_Normal();
        }
        else
        {
          MvPage_GameAct();
        }
        break;
      case db::pg_Fin: MvPage_Fin(); break;
      case db::pg_Idle: MvPage_Idle(); break;
      default:
        switch (stTuto_.pagetype_)
        {
        case tpt_Info:MvPage_Info(); break;
        case tpt_Act:MvPage_Act(); break;
        case tpt_End:MvPage_End(); break;
        }

        if (ugshiai_->IsActRes(enUGShiaiAct_pause))
        {
          if (stLy_.pageNo_ < pg_GameAct)
          {
            ChangePage(pg_GameAct);
          }
          else
          {
            pScene_->lyShBar_->MoveLayer(shl_Result);
          }
        }
        break;
      }

      if (stLy_.pageNo_ != db::pg_GameAct
        && stLy_.pageNo_ != db::pg_Idle)
      {
        ugtuto_->CheckSkipMsg();
      }
    }
  }

  //実戦開始
  void TLyShiaiTutoRetro::MvPage_GameAct()
  {
    if (ugtuto_->IsAct())//ugtutoからugshiai_に向けてのアクション
    {
      if (ugtuto_->NextMsg()) ugtuto_->SameMsg();
    }
    else if (ugshiai_->IsAct())
    {
      if (ugshiai_->IsActRes(enUGShiaiAct_pause))
      {
        ChangePage(pg_Fin);
      }
      else if (ugshiai_->IsActRes(enUGShiaiAct_gameset))//試合終了
      {
        ChangePage(pg_Fin);
      }
      else if (ugshiai_->IsActRes(enUGShiaiAct_result))//結果
      {
        pScene_->lyShBar_->MoveLayer(shl_Result);
        ChangePage(pg_Idle);
      }
    }
  }
  void TLyShiaiTutoRetro::MvPage_GameAct_Normal()
  {
    if (ugshiai_->IsActRes(enUGShiaiAct_pause))//ポーズ
    {
      pScene_->lyShBar_->MoveLayer(shl_Pause);
    }
    else if (ugshiai_->IsActRes(enUGShiaiAct_retry))//リトライ
    {
      pScene_->lyShBar_->MoveLayer(shl_Retry);
    }
    else if (ugshiai_->IsActRes(enUGShiaiAct_result))//結果
    {
      pScene_->lyShBar_->MoveLayer(shl_Result);
      ChangePage(pg_Idle);
    }
  }
  void TLyShiaiTutoRetro::MvPage_Fin()
  {
    if (ugtuto_->IsAct())//ugtutoからugshiai_に向けてのアクション
    {
      if (ugtuto_->NextMsg())
      {
        pScene_->lyShBar_->MoveLayer(shl_Result);
        ChangePage(pg_Idle);
      }
    }
  }

}
