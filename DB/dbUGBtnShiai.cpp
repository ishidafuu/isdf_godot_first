#include "dbUGBtnShiai.h"
#include "dbUILayer.h"
#include "dbMgUI.h"

namespace db
{
  //コンストラクタ
  TUGBtnShiai::TUGBtnShiai(TUIGenLayer* player) :TUGButton(player)
  {
    stBtnShiai_.Init();
    banner_ = NULL;
  }
  TUGBtnShiai::~TUGBtnShiai(void)
  {
  }
  void TUGBtnShiai::MakeGroupShiai(TUIObj* parent, base::TGrPart* pgrpart, s32 partno, const char* msg)
  {
    
    //stBtn_.enterAct_f_ = TRUE;
    field_->o_SetObjGrPart(pgrpart);
    field_->g_SetPartNo(partno);
    stBtn_.part_f_ = TRUE;
    stBtn_.partno_ = partno;
    stBtn_.slpartno_ = partno;
    GenMakeGroup(parent, TRUE);
    SetDepth(UIDP_05BUTTON);
    SetFont(msg, FC2_hmshiai_com, 0, -25);

    banner_ = MakeObj();
    banner_->o_SetParent(field_);
    banner_->g_SetCenter(TRUE);
  }
  void TUGBtnShiai::SetBanner(base::TGrPart* pgrpart, s32 partno, const char* msg)
  {
    banner_->o_SetObjGrPart(pgrpart);
    banner_->g_SetPartNo(partno);
    SetBannerMsg(msg);
  }
  void TUGBtnShiai::SetBannerMsg(const char* msg)
  {
    banner_->f_MakeFont(msg, FC2_hmshiai_com);
    banner_->f_FontPos(0, 26);
    banner_->f_SetCenter(TRUE);
  }

  //チームマスターから情報を引いてくる
  void TUGBtnShiai::SetEvent(s64 mid_event)
  {
    field_->g_SetPartNo(3);
    //レイヤの画像をクリアしてるので、使い所によってはバッティングする
    //player_->ClearGrp();//pgrvec_をクリア
    s32 bmpno = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(mid_event, mdm_event_bmpNo);
    std::string bannername = "ev_" + lib_str::IntToStr1b(bmpno, 5);//数値→1byte文字列
    base::TGrp* evbanner = player_->LoadGrp("event", bannername.c_str());//pgrvec_の中に入る
    banner_->o_SetObjGrp(evbanner);
    std::string evname = pmgEO_->mgCoM_.mdm_Event_.GetPKDtStr(mid_event, mdm_event_name_c64);
    const s32 MAXLEN = 17;
    SetFont(evname.c_str(), FC2_hmshiai_com, 0, -25);
    //name_->f_MakeFont_Len(evname.c_str(), FC2_mini, MAXLEN);
    //name_->z_SetPos(8, 8);
    //nokori_->z_SetPos(8, 32);
  }


  //更新
  void TUGBtnShiai::DoUpdateSub(BOOL draw_f)
  {
    banner_->Update(draw_f);
  }

}