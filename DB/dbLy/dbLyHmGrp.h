#pragma once
#include "dbInclude.h"
#include "dbUILayer.h"

namespace db
{
  class TSnHome;

  //画像ローダー////////////////////////////////
  class TLyHmGrp :public TUILayer//自動開放読み込みするためTUILayerを継承
  {
  protected:

    //オーバーロード
    void DoLoad();
  public:
    //base::TGrp* bar_obj_;
    base::TGrPart* home_bg_;
    //base::TGrp* home_board_;
    //base::TGrp* home_challenge_;
    //base::TGrp* home_flower_;
    base::TGrp* home_shadow_;
    //base::TGrPart* home_tv_;
    base::TGrp* home_window_;

    base::TGrp* team_banner_;
    base::TGrPart* team_bcolors_;
    base::TGrPart* team_boombox_;
    base::TGrp* team_camera_;
    base::TGrp* team_cardside_;
    base::TGrPart* team_ball_;
    base::TGrp* team_vol_;
    base::TGrp* team_volbtn_;

    base::TGrp* stage_picture_;

    //base::TGrPart* hmshiai_event_;
    base::TGrPart* hmshiai_next_;
    //base::TGrPart* hmshiai_select_;
    base::TGrPart* hmshiai_banner_;
    //base::TGrp* hmshiai_lock_;
	base::TGrPart* hmshiai_menu_wifi_;
	base::TGrPart* hmshiai_menu_bt_;

    //base::TGrPart* icon_etc_;
    //base::TGrPart* icon_rare_;
    //base::TGrPart* icon_ball_;

    base::TGrPart* center_shutter_;
    base::TGrPart* center_bbadoor_;
    base::TGrPart* center_dockdoor_;
    base::TGrp* center_track_;
    base::TGrp* center_pole_;

    base::TGrPart* shop_door_[3];
    base::TGrp* shop_front_[3];

    //コンストラクタ
    explicit TLyHmGrp(TSnHome* pScene);
    virtual ~TLyHmGrp(void);

  };

}
