#include "dbUGFooter.h"
//#include "dbUILayer.h"

namespace db
{
  const s32 TABNUM = 6;

  //コンストラクタ
  TUGFooter::TUGFooter(TUIGenLayer* player) :TUGGen(player)
    , ugradio_(player)
    , ugbtn_back_(player)
    , ugbtn_ok_(player)
    , ugbtn_mid_(player)
    , ugbtn_prev_(player)
    , ugbtn_next_(player)
    , ugbtn_tsh_(player)
    , ugbtn_mgk_(player)
    , ugbtn_fkt_(player)
  {
    stFooter_.Init();
  }
  TUGFooter::~TUGFooter()
  {
    for (s32 i = 0; i < tabbtn_.size(); ++i)
    {
      Freemem(tabbtn_[i]);
    }
  }

  void TUGFooter::MakeGroup(TUIObj* parent)
  {
    const s32 BARW = 512;
    const s32 FOOTH = 48;
    SetParent(parent);

    field_->o_SetObjFilterGrp(BARW, FOOTH, 0xFF, 0, 0, 0);
    field_->z_SetPos_FullBtm(0, -FOOTH);
    field_->g_SetDepth(UIDP_04HEADER);

    ugbtn_back_.MakeGroup_Footer(field_, 0);
    ugbtn_ok_.MakeGroup_Footer(field_, 1);
    ugbtn_mid_.MakeGroup_Footer(field_, 2);
    ugbtn_next_.MakeGroup_Footer(field_, 3);
    ugbtn_prev_.MakeGroup_Footer(field_, 4);
    ugbtn_tsh_.MakeGroup_FooterTokusyou(field_);
    ugbtn_mgk_.MakeGroupSupply_MGK(field_);
    ugbtn_fkt_.MakeGroupSupply_FKT(field_);

    lybtn_.push_back(&ugbtn_back_);
    lybtn_.push_back(&ugbtn_ok_);
    lybtn_.push_back(&ugbtn_mid_);
    lybtn_.push_back(&ugbtn_next_);
    lybtn_.push_back(&ugbtn_prev_);
    lybtn_.push_back(&ugbtn_tsh_);
    lybtn_.push_back(&ugbtn_mgk_);
    lybtn_.push_back(&ugbtn_fkt_);

    ugradio_.MakeGroup(field_, 88, 15);
    //フッタボタン
    const s32 TABDX = 54;
    for (s32 i = 0; i < TABNUM; ++i)
    {
      TUGButton* newbtn = new TUGButton(player_);
      newbtn->MakeGroup_Radio(NULL, ug_btn_footer_type2, ug_btn_footer2_type2);
      std::string tmnm;
      switch (i)
      {
	  default:
      case footer_id_home:tmnm = "ホーム"; break;
      case footer_id_shiai:tmnm = "しあい"; break;
      case footer_id_team:tmnm = "チーム"; break;
      case footer_id_center:tmnm = "しゅーと"; break;
      case footer_id_shop:tmnm = "ショップ"; break;
	  case footer_id_event:tmnm = "イベント"; break;
      }
      newbtn->SetFont(tmnm.c_str(), FC2_btn_normal);//FC2_btn_mark
      ugradio_.AddButton(newbtn, TABDX);
      tabbtn_.push_back(newbtn);
    }

	// 位置修正
	for (s32 i = footer_id_team; i < TABNUM; i++)
	{
		s32 tabNo = i;
		if (i != footer_id_event)
		{
			tabNo += 1;
		}
		else
		{
			tabNo = footer_id_team;
		}
		tabbtn_[i]->SetPos(88 + (tabNo * TABDX), 15);
	}

    //ボタンは消しておく
    UndrawBtn();
  }
  BOOL TUGFooter::IsTab()
  {
    return (stFooter_.state_ == foot_Tab);
  }
  void TUGFooter::SetEnable()
  {
    stFooter_.state_ = stFooter_.laststate_;
  }
  void TUGFooter::SetDisable()
  {
    SetFoot_None();
  }

  void TUGFooter::UndrawBtn()
  {
    for (s32 i = 0; i < lybtn_.size(); ++i)
    {
      lybtn_.at(i)->SetDraw(FALSE);
    }
  }

  void TUGFooter::SetFoot_Tab()
  {
    stFooter_.state_ = foot_Tab;
    stFooter_.laststate_ = foot_Tab;
    ugradio_.SetDraw(TRUE);
    UndrawBtn();
  }
  void TUGFooter::SetFoot_Btn()
  {
    stFooter_.state_ = foot_Btn;
    stFooter_.laststate_ = foot_Btn;
    ugradio_.SetDraw(FALSE);
    UndrawBtn();//使う前に全部消す
  }
  void TUGFooter::SetFoot_None()
  {
    stFooter_.state_ = foot_None;
    ugradio_.SetDraw(FALSE);
    UndrawBtn();
  }

  void TUGFooter::HideBtn(BOOL hide_f)
  {
    stFooter_.hide_f_ = hide_f;
  }
  BOOL TUGFooter::IsHideBtn()
  {
    return stFooter_.hide_f_;
  }

  void TUGFooter::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    if (stFooter_.hide_f_)//隠してるときもアクションはクリア
    {
      ugradio_.ClearAct();
      for (s32 i = 0; i < lybtn_.size(); ++i) lybtn_.at(i)->ClearAct();
    }
    else
    {
      ugradio_.Update(draw_f);
      for (s32 i = 0; i < lybtn_.size(); ++i)  lybtn_.at(i)->Update(draw_f);
    }
  }

  void TUGFooter::DoAction()
  {
    switch (stFooter_.state_)
    {
    case foot_Tab:
      if (ugradio_.IsAct()) SetAct(ugradio_.GetActIndex());
      break;
    case foot_Btn:
      for (s32 i = 0; i < lybtn_.size(); ++i)
      {
        if (lybtn_.at(i)->IsAct()) SetAct(i);
      }
      break;
    }
  }
}
