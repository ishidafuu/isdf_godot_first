#include "dbUGDock.h"


namespace db
{

  enum enItemDraw
  {
    itd_None,
    itd_Carry,
    itd_Table,
  };


  //コンストラクタ
  TUGDock::TUGDock(TUIGenLayer* player) :TUGGen(player), ugclerk_(player), ugshutter_(player), ugdoor_(player), ugbtn_(player)
  {
    stDock_.Init();
  }

  TUGDock::~TUGDock()
  {

  }

  void TUGDock::MakeGroup(TUIObj* parent, base::TGrPart* grpshutter, base::TGrPart* grpdoor, s32 dockno)
  {
    stDock_.dockno_ = dockno;

    SetParent(parent);
    field_->z_SetPos_Full(224, 48 + (dockno * 40));

    //ball_ = MakeObj();
    //ball_->o_SetParent(field_);
    //ball_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_rarity));
    //ball_->z_SetPos(88, 25);
    enClerkType ctype = (enClerkType)(ct_Dock0 + dockno);
    ugclerk_.MakeGroup(field_, ctype);
    ugclerk_.SetPos(83, 40);
    ugclerk_.SetMirror(TRUE);
    ugclerk_.SetDepth(UIDP_01BGOBJ);

    ugshutter_.MakeGroup_shutter(field_, grpshutter, 8, 4);
    ugshutter_.SetDepth(UIDP_02BGOBJHI);
    ugdoor_.MakeGroup(field_, grpdoor, 24, 4);//ドア
    //既に解放されてたら表示しない

    ugbtn_.MakeGroup(field_, ug_btn_ok);
    ugbtn_.SetPos(89+34, 10+12);
    ugbtn_.SetDraw(FALSE);
  }
  void TUGDock::ChangeGousei(BOOL gousei_f)
  {
    ugbtn_.SetDraw(!gousei_f);
  }
  s32 TUGDock::GetDockNo()
  {
    return stDock_.dockno_;
  }
  s64 TUGDock::GetIDDock()
  {
    return stDock_.id_dock_;
  }

  void TUGDock::SetEnable(s64 id_dock, BOOL add_f)
  {
    ugbtn_.SetDraw(TRUE);
    stDock_.mv_c_ = 0;
    stDock_.id_dock_ = id_dock;
    stDock_.state_ = enDockState_Empty;

    if (add_f)//オープン
    {
      //ugbtn_.SetDraw(FALSE);
      ugbtn_.SetFont("ミックス", FC2_btn_normal);
      ugbtn_.SetDraw(TRUE);
      ugshutter_.OpenDoor();
    }
    else//すでにオープン
    {
      ugbtn_.SetDraw(TRUE);
      ugshutter_.SetDraw(FALSE);      
      //空き
      BOOL emp_f = (pmgEO_->mgCoU_.mdDock_.GetPKDt(stDock_.id_dock_, mddock_state) == 0);

      if (emp_f)
      {
        stDock_.state_ = enDockState_Empty;
        ugbtn_.SetFont("ミックス", FC2_btn_normal);
      }
      else
      {
        Mv_Gousei();
      }
    }
  }
  void TUGDock::SetDisable(BOOL nextopen_f)
  {
    stDock_.state_ = (nextopen_f)
      ? enDockState_NextOpen
      : enDockState_Close;

    if (stDock_.state_ == enDockState_NextOpen)
    {
      ugbtn_.SetFont("かいほう", FC2_btn_normal);
      ugbtn_.SetDraw(TRUE);
    }
    else
    {
      ugbtn_.SetDraw(FALSE);
    }
  }
  //完了
  void TUGDock::SetFinish()
  {
    ugbtn_.SetFont("うけとる", FC2_btn_normal);
    stDock_.resttime_ = 0;
    stDock_.state_ = enDockState_Finish;
    //ugbtn_.SetFontSub("", FC2_btn_normal);
  }

  //空状態
  void TUGDock::Mv_Empty()
  {
    if (pmgEO_->mgCoU_.mdDock_.GetPKDt(stDock_.id_dock_, mddock_state) != 0)
    {
      SetEnable(stDock_.id_dock_, FALSE);
    }
  }
  //合成中状態
  void TUGDock::Mv_Gousei()
  {
    if ((stDock_.mv_c_ % (60 * 5)) == 0)
    {
      s64 nowtime = mid::midNowTime();
      s64 endtime = pmgEO_->mgCoU_.mdDock_.GetPKDt(stDock_.id_dock_, mddock_end_date);
      //終了時間を過ぎている
      stDock_.state_ = (nowtime > endtime)
        ? enDockState_Finish
        : enDockState_Gousei;
      //残り時間
      if (stDock_.state_ == enDockState_Gousei)
      {
        //ugbtn_.SetFont("たんしゅく", FC2_btn_normal);
        std::string datestr = "あと";
        if ((endtime - nowtime) < 60)
        {
          datestr += "すこし";
        }
        else
        {
          datestr += base::UnixtimeToHHMMSSString(endtime - nowtime, FALSE);
        }
        

        stDock_.resttime_ = ((endtime - nowtime) / 60);
        ugbtn_.SetFont(datestr.c_str(), FC2_btn_normal);
      }
      else
      {
        SetFinish();
      }
    }

  }
  //合成完了
  void TUGDock::Mv_Finish()
  {
    if (pmgEO_->mgCoU_.mdDock_.GetPKDt(stDock_.id_dock_, mddock_state) == 0)
    {
      SetEnable(stDock_.id_dock_, FALSE);
    }
  }
  //未解放（次にあく）
  void TUGDock::Mv_NextOpen()
  {

  }
  //未解放
  void TUGDock::Mv_Close()
  {
  
  }
  //解放アニメーション
  void TUGDock::Mv_Opening()
  {
    if (ugshutter_.IsActRes(enUGDoorAct_Half))
    {
      ugshutter_.SetDraw(FALSE);
      stDock_.state_ = enDockState_Empty;
    }
  }
  void TUGDock::MoveState()
  {
    ++stDock_.mv_c_;

    switch (stDock_.state_)
    {
    case enDockState_Empty: Mv_Empty(); break;
    case enDockState_Gousei: Mv_Gousei(); break;
    case enDockState_Finish: Mv_Finish(); break;
    case enDockState_NextOpen:Mv_NextOpen(); break;
    case enDockState_Close: Mv_Close(); break;
    //case enDockState_Opening:Mv_Opening(); break;
    }
  }
  void TUGDock::SetWait(BOOL wait_f)
  {
    stDock_.wait_f_ = wait_f;
  }
  enDockState TUGDock::GetDockState()
  {
    return stDock_.state_;
  }
  void TUGDock::DoUpdate(BOOL draw_f)
  {
    //進行
    field_->Update(draw_f);
    //ball_->Update(draw_f);
    ugshutter_.Update(draw_f);
    ugdoor_.Update(draw_f);
    if (stDock_.wait_f_ == FALSE)
    {
      ugclerk_.Update(draw_f);
      ugbtn_.Update(draw_f);
    }

  }

  void TUGDock::DoAction()
  {
    if (ugbtn_.IsAct())
    {
      SetAct(0);
    }
  }

}
