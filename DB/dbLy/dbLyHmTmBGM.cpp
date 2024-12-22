#include "dbLyHmTmBGM.h"
#include "dbSnHome.h"

namespace db
{

  enum enPage
  {
    pg_BGM,
  };

  enum enConnect
  {
    cn_SetBGM,
  };

  const s32 INTRO_TIMER_NUM = 90;	// 90フレームだけイントロ再生

  //コンストラクタ
  TLyHmTmBGM::TLyHmTmBGM(TSnHome* pScene) :TLyHmGen(pScene)
      , ugBGM_(this)
  {

  }
  TLyHmTmBGM::~TLyHmTmBGM()
  {
  }
  void TLyHmTmBGM::DoLoad()
  {
      ugBGM_.MakeGroup(field_);
      ugBGM_.SetDraw(FALSE);

#ifdef __K_DEBUG_USER_DATA__
    kdebug::DebugUDManager::GetInstance()->GetAllBGM();
#endif
  }
  void TLyHmTmBGM::DoReady()
  {
      ugBGM_.SetDraw(TRUE);
      ugBGM_.Ready();

      ugbtn_back_->SetFont("もどる");
    ugbtn_back_->SetDraw(TRUE);
    ugbtn_ok_->SetFont("ホームＢＧＭ");
    ugbtn_ok_->SetDraw(FALSE);
  }
  void TLyHmTmBGM::DoDown()
  {
      // 元のBGMに戻す
     ugBGM_.Down();
     pScene_->lyHmTeam_->Ready();
  }

  void TLyHmTmBGM::MvPage_BGM()
  {
      mid_bgm_ = ugBGM_.GetBgmID();

    if (ugbtn_back_->IsAct())//おーけー
    {
      Down();
      return;
    }
    else if (ugbtn_ok_->IsAct() && (mid_bgm_ != NGNUM))//おーけー
    {
        // 持ってる
        if (pmgEO_->mgCoU_.IsBGM(mid_bgm_))
        {
            Post_SetBGM();
        }
        // 持ってない
        else
        {
            // ガチャ屋にとばす
            Down();

            INTVECTOR v;
            v.push_back(footer_id_shop);	// ショップタブ
            v.push_back(2);	// ガチャショップ
            v.push_back(2); // なつかしBOX
            s32 mid_nabox = pmgEO_->mgDt_.dtBGM_.GetDtLine(mid_bgm_, enDtBGM_mid_nabox);
            v.push_back(mid_nabox);
            pmgEO_->stJump_.SetJumpAreaData(v);
            pmgEO_->stJump_.SetFromTab(footer_id_team);
            pScene_->lyHmBar_->JumpFooter(v[0]);
            return;
        }
    }
    else if (ugBGM_.GetActRes() == enUGBgmAct_Select)//BGMりすと
    {
        // 持ってる
        if (pmgEO_->mgCoU_.IsBGM(mid_bgm_))
        {
            BOOL loop_f = (pmgEO_->mgDt_.dtBGM_.GetDtLine(mid_bgm_, enDtBGM_loop_f) != 0);
            ugbtn_ok_->SetFont("ホームＢＧＭ");
            ugbtn_ok_->SetDraw(loop_f);
        }
        // 持ってない
        else
        {
            ugbtn_ok_->SetFont("ガチャでかう");
            ugbtn_ok_->SetDraw(TRUE);
        }
    }
  }

  void TLyHmTmBGM::Post_SetBGM()
  {
    if (mid::midIsDXL())
    {
#ifdef __K_DEBUG_USER_DATA__
        kdebug::DebugUDManager::GetInstance()->SetHomeBGM(mid_bgm_);
#endif
      Connect_SetBGM();
      return;
    }

    StartConnect(cn_SetBGM);
    pmgEO_->mgPo_.poHmTeam_.Ready_setbgm(mid_bgm_);
  }
  void TLyHmTmBGM::Connect_SetBGM()
  {
    //ホームＢＧＭアイコンを差し替え
      ugBGM_.RefreshIcon();
  }
  void TLyHmTmBGM::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_SetBGM: Connect_SetBGM(); break;
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

  void TLyHmTmBGM::DoUpdate(BOOL draw_f)
  {
      ugBGM_.Update(draw_f);

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case pg_BGM:MvPage_BGM(); break;
      }
    }
  }


}
