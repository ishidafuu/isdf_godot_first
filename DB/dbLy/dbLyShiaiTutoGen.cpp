#include "dbLyShiaiTutoGen.h"
#include "dbSnShiai.h"
namespace db
{



  //コンストラクタ
  TLyShiaiTutoGen::TLyShiaiTutoGen(TSnShiai* pScene, TUGShiai* ugshiai) :TLyShiaiGen(pScene, ugshiai)
    //, ugtuto_(this)
    , ugfinger_(this)
  {
    stTuto_.Init();
  }

  TLyShiaiTutoGen::~TLyShiaiTutoGen()
  {
  }

  void TLyShiaiTutoGen::DoLoad()
  {
    //ugtuto_.MakeGroup(field_);
    ugfinger_.MakeGroup(field_);
    DoLoadSub();
  }
  void TLyShiaiTutoGen::DoReady()
  {
    stTuto_.Init();
    //ugtuto_.Ready();
    ugfinger_.SetDraw(FALSE);
    ugshiai_->Ready_Tuto(TRUE);//試合関係読み込み

    // ものは試しで
    if (!pmgEO_->stSeq_.IsFirstTuto())
    {
      pmgEO_->mgNSF_.PlayID(pmgEO_->stShiai_.GetMidBGM());
    }

    ugbtn_back_->SetFont("おーけー");
    ugbtn_ok_->SetFont("もいちど！");

    DoReadySub();
  }
  void TLyShiaiTutoGen::DoDown()
  {
  }
  void TLyShiaiTutoGen::DoRestart()
  {
    //ChangePage(stTuto_.lastpg_);
  }

  BOOL TLyShiaiTutoGen::IncActNum(s32 actindex)
  {
    BOOL res = FALSE;
    if (actindex == 0)
    {
      ++stTuto_.okNum_;
      stTuto_.okclear_f_ = TRUE;//OKクリア
      res = TRUE;
    }
    else
    {
      stTuto_.okclear_f_ = FALSE;
    }
    return res;
  }

  void TLyShiaiTutoGen::SetPageData(s32 pageno, enSendTutoAct act, s32 pagetype)
  {
    if (pagetype != tpt_Act)
    {
      ugtuto_->SetMsgClear(pageno, stTuto_.okclear_f_);
    }
    ugshiai_->SendTutoAct(act);
    stTuto_.pagetype_ = pagetype;
    stTuto_.okNum_ = 0;
    stTuto_.ngNum_ = 0;
    stTuto_.okclear_f_ = FALSE;

    BOOL btn_f = (pagetype == tpt_End);
    ugbtn_back_->SetDraw(btn_f);
    ugbtn_ok_->SetDraw(btn_f);
    pScene_->lyShBar_->SetNoneBar(!btn_f);
    ugshiai_->SetHideRetro(btn_f);
  }
  void TLyShiaiTutoGen::MvPage_Info()
  {
    if (ugtuto_->IsAct())//ugtutoからugshiai_に向けてのアクション
    {
      if (ugtuto_->NextMsg())
      {
        ChangePage(stLy_.pageNo_ + 1);
      }
    }
  }
  void TLyShiaiTutoGen::MvPage_Act()
  {
    const s32 CLEARNUM = 1;
    const s32 NGCLEARNUM = 5;

    //ugshiaiからの成功や失敗の受信
    if (ugshiai_->IsActRes(enUGShiaiAct_tuto))
    {
      if (IncActNum(ugshiai_->GetActIndex()))
      {
        if (stTuto_.okNum_ < CLEARNUM)
        {
          ugtuto_->SetMsg(stLy_.pageNo_, 0);//ＯＫメッセージ
        }
        else
        {
          ChangePage(stLy_.pageNo_ + 1);//クリア
        }
      }
      else if (stTuto_.ngwait_c_ == 0)
      {
        if (stTuto_.ngNum_ < NGCLEARNUM)
        {
          stTuto_.ngwait_c_ = 150;
          ++stTuto_.ngNum_;
          ugtuto_->SetMsg(stLy_.pageNo_, ugshiai_->GetActIndex());//NGメッセージ
        }
        else
        {
          ChangePage(stLy_.pageNo_ + 1);//クリア
        }
      }
    }

    lib_num::AprTo0(&stTuto_.ngwait_c_);
  }
  //オーケーチェック
  void TLyShiaiTutoGen::MvPage_End()
  {
    if (ugbtn_back_->IsAct())//おーけー
    {
      ChangePage(stLy_.pageNo_ + 1);
    }
    else if (ugbtn_ok_->IsAct())//もういちど
    {
      ChangePage(stLy_.pageNo_ - 2);
    }
  }

  void TLyShiaiTutoGen::MvPage_Idle()
  {

  }
  void TLyShiaiTutoGen::DoUpdate(BOOL draw_f)
  {
    //field_->Update(draw_f);
    ugshiai_->Update(draw_f);
    ugfinger_.Update(draw_f);
    ugtuto_->Update(draw_f);

    DoUpdateSub(draw_f);
  }
}
