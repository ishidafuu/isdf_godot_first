#include "dbUGBtnEvent.h"
#include "dbUILayer.h"
#include "dbMgUI.h"

namespace db
{
  //コンストラクタ
  TUGBtnEvent::TUGBtnEvent(TUIGenLayer* player) :TUGButton(player)
  {
    stBtnEvent_.Init();
    banner_ = NULL;
  }
  TUGBtnEvent::~TUGBtnEvent(void)
  {
  }
  void TUGBtnEvent::MakeGroupEvent(TUIObj* parent, base::TGrPart* pgrpart)
  {
    const s32 EVENTPARTNO = 3;
    field_->o_SetObjGrPart(pgrpart);
    field_->g_SetPartNo(EVENTPARTNO);
    stBtn_.part_f_ = TRUE;
    stBtn_.partno_ = EVENTPARTNO;
    stBtn_.slpartno_ = EVENTPARTNO;


    GenMakeGroup(parent, TRUE);
    SetDepth(UIDP_05BUTTON);
    //field_->b_SetModal(TRUE);

    banner_ = MakeObj();
    banner_->o_SetParent(field_);
    banner_->g_SetCenter(TRUE);
  }
  //チームマスターから情報を引いてくる
  void TUGBtnEvent::SetEvent(s64 mid_event)
  {
    //レイヤの画像をクリアしてるので、使い所によってはバッティングする
    //player_->ClearGrp();//pgrvec_をクリア
    stBtnEvent_.mid_event_ = mid_event;
    for (s32 i = 0; i < pmgEO_->mgCoU_.mdw_Event_.GetLineNum(); ++i)
    {
      if (pmgEO_->mgCoU_.mdw_Event_.GetLineDt(i, mdw_event_id_m_event) == stBtnEvent_.mid_event_)
      {
        stBtnEvent_.wid_event_ = pmgEO_->mgCoU_.mdw_Event_.GetLineDt(i, mdw_event_pk);
        break;
      }
    }

    const s32 EVKETA = 5;
    s32 bmpno = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(mid_event, mdm_event_bmpNo);
    std::string bannername = "ev_" + lib_str::IntToStr1b(bmpno, EVKETA);//数値→1byte文字列
    base::TGrp* evbanner = player_->LoadGrp("event", bannername.c_str());//pgrvec_の中に入る
    banner_->o_SetObjGrp(evbanner);
    std::string evname = pmgEO_->mgCoM_.mdm_Event_.GetPKDtStr(mid_event, mdm_event_name_c64);
    const s32 MAXLEN = 17;
    SetFont(evname.c_str(), FC2_hmshiai_com, 0, -25);

    MvTimer();
    banner_->f_FontPos(0, 29);
    banner_->f_SetCenter(TRUE);
  }

  void TUGBtnEvent::Refresh()
  {
    s64 nowtime = mid::midNowTime();
    s64 endtime = pmgEO_->mgCoU_.mdw_Event_.GetPKDt(stBtnEvent_.wid_event_, mdw_event_end_date);
    //残り時間
    if (nowtime < endtime)
    {
      //ugbtn_.SetFont("たんしゅく", FC2_btn_normal);
      std::string datestr = "あと";
      if ((endtime - nowtime) < 60)
      {
        datestr += "すこし";
      }
      else
      {
        datestr += base::UnixtimeToDDHHMMSSString(endtime - nowtime);
      }

      stBtnEvent_.resttime_ = ((endtime - nowtime) / 60);
      banner_->f_MakeFont_Colnum(datestr.c_str(), FC2_btn_normal, FC2_rareB);

    }
    else
    {
      banner_->f_MakeFont("おしまい", FC2_btn_normal);

      //おしまいを書いた時点で更新が必要
      pmgEO_->stSeq_.PushNeedEvent();
    }
  }
  //チームマスターから情報を引いてくる
  void TUGBtnEvent::MvTimer()
  {
    if ((stBtnEvent_.mv_c_ % (60 * 1)) == 0)
    {
      Refresh();
    }

    ++stBtnEvent_.mv_c_;
  }

  //更新
  void TUGBtnEvent::DoUpdateSub(BOOL draw_f)
  {
    banner_->Update(draw_f);

#ifndef __K_DEBUG_ROOT__
    MvTimer();
#endif
  }

}