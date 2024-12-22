#include "dbUGButton.h"
#include "dbMgUI.h"

namespace db
{


  //コンストラクタ
  TUGButton::TUGButton(TUIGenLayer* player) :TUGGen(player)
    , ugricon_(player)
  {
    stBtn_.Init();
//    field_->f_MakeFont("著Ｍｉｌｌｉｏｎ＿Ｃｏ．，Ｌｔｄ＿／＿Ｍａｒｖｅｌｏｕｓ＿Ｉｎｃ．", FC2_btn_normal);
//    lock_ = NULL;
  }

  TUGButton::~TUGButton(void)
  {
  }
  void TUGButton::GenMakeGroup(TUIObj* parent, BOOL center_f)
  {
    SetParent(parent);
    ugricon_.MakeGroup(field_);

    if (stBtn_.part_f_ == FALSE)
    {
      field_->o_SetObjGrp(stBtn_.basegrp_);//画像セット
    }
    field_->g_SetDepth(UIDP_05BUTTON);//手前
    field_->b_SetBtn(TRUE);//ボタン化

    if (center_f)
    {
      field_->g_SetCenter(TRUE);//中心
      field_->f_SetCenter(TRUE);//文字中心
    }

    DoMakeGroupSub(parent);
  }
  void TUGButton::MakeGroup(TUIObj* parent, base::TGrp* basegrp)
  {
    //直接選択
    stBtn_.basegrp_ = basegrp;
    stBtn_.basegrp2_ = basegrp;
    GenMakeGroup(parent, TRUE);
  }
  void TUGButton::MakeGroup(TUIObj* parent, enUGGrp basegrp)
  {
    //UGグラから選択
    stBtn_.basegrp_ = pmgUI_->GetUGGrp(basegrp);
    stBtn_.basegrp2_ = pmgUI_->GetUGGrp(basegrp);
    GenMakeGroup(parent, TRUE);
  }
  void TUGButton::MakeGroup(TUIObj* parent, enUGGrp basegrp, enUGGrp basegrp2)
  {
    //UGグラから選択
    stBtn_.basegrp_ = pmgUI_->GetUGGrp(basegrp);
    stBtn_.basegrp2_ = pmgUI_->GetUGGrp(basegrp2);
    GenMakeGroup(parent, TRUE);
  }
  void TUGButton::MakeGroup(TUIObj* parent, enUGGrp basegrp, BOOL center_f)
  {
    stBtn_.basegrp_ = pmgUI_->GetUGGrp(basegrp);
    stBtn_.basegrp2_ = pmgUI_->GetUGGrp(basegrp);
    GenMakeGroup(parent, center_f);
  }
  void TUGButton::MakeGroup(TUIObj* parent, enUGGrPart basegrp, s32 partno)
  {
    //UGグラから選択
    stBtn_.enterAct_f_ = TRUE;
    field_->o_SetObjGrPart(pmgUI_->GetUGGrPart(basegrp));
    field_->g_SetPartNo(partno);
    stBtn_.part_f_ = TRUE;
    stBtn_.partno_ = partno;
    stBtn_.slpartno_ = partno;
    GenMakeGroup(parent, TRUE);
  }
  void TUGButton::MakeGroup_Otoiawase(TUIObj* parent)
  {
    MakeGroup(parent, ug_btn_oto);
    field_->z_SetPos_Full(328, 223);
  }
  void TUGButton::MakeGroup_Headder(TUIObj* parent, enUGGrp basegrp, enUGGrp basegrp2)
  {
    //UGグラから選択
    stBtn_.basegrp_ = pmgUI_->GetUGGrp(basegrp);
    stBtn_.basegrp2_ = pmgUI_->GetUGGrp(basegrp2);
    GenMakeGroup(parent, FALSE);
    //field_->b_SetBtn(FALSE);//いったんけす
  }
  void TUGButton::MakeGroup_Footer(TUIObj* parent, s32 pos)
  {
    stBtn_.basegrp_ = pmgUI_->GetUGGrp(ug_btn_footer);
    stBtn_.basegrp2_ = pmgUI_->GetUGGrp(ug_btn_footer2);
    GenMakeGroup(parent, TRUE);
    SetPos(352 - (TABDX * pos), 15);//223
    //プレビューのフィルタより上に出す必要があるのでプライオリティアップとモーダル有効
    SetDepth(UIDP_09MSGBOX);
    GetField()->b_SetModal(TRUE);
    //UIDP_07BUTTONTOP
  }
    void TUGButton::MakeGroup_FooterTokusyou(TUIObj* parent)
    {
      stBtn_.basegrp_ = pmgUI_->GetUGGrp(ug_btn_foottsh);
      stBtn_.basegrp2_ = pmgUI_->GetUGGrp(ug_btn_foottsh);
      GenMakeGroup(parent, TRUE);
      SetPos(352 - (TABDX * 4) + 32, 15);//223
      SetDepth(UIDP_07BUTTONTOP);
      //SetDepth(UIDP_04HEADER);
    }

  void TUGButton::MakeGroup_FtPosBack(TUIObj* parent)
  {
    MakeGroup_FtPos(parent, 0, "もどる");
  }
	/* 2015/07/02 Koike Added */
	void TUGButton::MakeGroup_FtPosCancel(TUIObj* parent)
	{
		MakeGroup_FtPos(parent, 0, "ミックスやめる");
	}
  void TUGButton::MakeGroup_FtPos(TUIObj* parent, s32 pos, const char* msg)
  {
    stBtn_.basegrp_ = pmgUI_->GetUGGrp(ug_btn_footer);
    stBtn_.basegrp2_ = pmgUI_->GetUGGrp(ug_btn_footer2);
    GenMakeGroup(parent, TRUE);
    field_->z_SetPos_Full(352 - (TABDX * pos), 223);
    SetFont(msg, FC2_btn_normal);
    SetDepth(UIDP_09MSGBOX);
    GetField()->b_SetModal(TRUE);
  }
  void TUGButton::MakeGroup_FtPosTokusyou(TUIObj* parent)
  {
    stBtn_.basegrp_ = pmgUI_->GetUGGrp(ug_btn_foottsh);
    stBtn_.basegrp2_ = pmgUI_->GetUGGrp(ug_btn_foottsh);
    GenMakeGroup(parent, TRUE);
    field_->z_SetPos_Full(352 - (TABDX * 4) + 32, 223);//223
    SetDepth(UIDP_09MSGBOX);
    GetField()->b_SetModal(TRUE);
  }

  void TUGButton::MakeGroup_Tokusyou(TUIObj* parent)
  {
    stBtn_.basegrp_ = pmgUI_->GetUGGrp(ug_btn_foottsh);
    stBtn_.basegrp2_ = pmgUI_->GetUGGrp(ug_btn_foottsh);
    GenMakeGroup(parent, TRUE);
    field_->z_SetPos_Full(128, 223);
    SetDepth(UIDP_09MSGBOX);
    GetField()->b_SetModal(TRUE);
  }

  void TUGButton::MakeGroup_Balloon(TUIObj* parent, const char* msg)
  {
    MakeGroup(parent, ug_btn_balloon);
    SetFont(msg, FC2_btn_normal);
    GetField()->g_SetAlp(TRUE, 0xc0);
  }
  void TUGButton::MakeGroup_Radio(TUIObj* parent, base::TGrp* basegrp, base::TGrp* basegrp2)
  {
    //直接選択
    stBtn_.enterAct_f_ = TRUE;
    stBtn_.basegrp_ = basegrp;
    stBtn_.basegrp2_ = basegrp2;
    GenMakeGroup(parent, TRUE);
  }
  void TUGButton::MakeGroup_Radio(TUIObj* parent, enUGGrp basegrp, enUGGrp basegrp2)
  {
    //UGグラから選択
    stBtn_.enterAct_f_ = TRUE;
    stBtn_.basegrp_ = pmgUI_->GetUGGrp(basegrp);
    stBtn_.basegrp2_ = pmgUI_->GetUGGrp(basegrp2);
    GenMakeGroup(parent, TRUE);
  }
  void TUGButton::MakeGroup_Radio(TUIObj* parent, enUGGrPart basegrp, s32 partno, s32 slpartno)
  {
    //UGグラから選択
    stBtn_.enterAct_f_ = TRUE;
    field_->o_SetObjGrPart(pmgUI_->GetUGGrPart(basegrp));
    stBtn_.part_f_ = TRUE;
    stBtn_.partno_ = partno;
    stBtn_.slpartno_ = slpartno;
    GenMakeGroup(parent, TRUE);
  }
  //押しっぱなし入力系ボタン
  void TUGButton::SetEnterAct(BOOL enteract_f)
  {
    stBtn_.enterAct_f_ = enteract_f;
  }

  //文字列セット
  void TUGButton::SetFont(const char* msg)
  {
    field_->f_MakeFont(msg, FC2_btn_normal);
  }
  void TUGButton::SetFont(const char* msg, s32 fontcol)
  {
    field_->f_MakeFont(msg, fontcol);
  }
  void TUGButton::SetFont(const char* msg, s32 fontcol, s32 fx, s32 fy)
  {
    field_->f_MakeFont(msg, fontcol);
    field_->f_FontPos(fx, fy);
  }
  //位置セット
  void TUGButton::SetPos(s32 posx, s32 posy)
  {
    field_->z_SetPos(posx, posy);
  }

  //選択画像／通常画像切り替え（フィールドおよびトップ）
  void TUGButton::SetSelect(BOOL select_f)
  {
    if (stBtn_.part_f_)
    {
      if (select_f) field_->g_SetPartNo(stBtn_.slpartno_);
      else field_->g_SetPartNo(stBtn_.partno_);
    }
    else
    {
      if (select_f) field_->o_SetObjGrp(stBtn_.basegrp2_);
      else field_->o_SetObjGrp(stBtn_.basegrp_);
    }
  }
  void TUGButton::SetDisable(BOOL disable_f)
  {
    stBtn_.disable_f_ = disable_f;
    if (disable_f)
    {
      field_->g_Dark();//とりあえずここで暗く
    }
    else
    {
      field_->g_EfReset();//とりあえずここで戻す
    }

  }


  //更新
  void TUGButton::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    ugricon_.Update(draw_f);

    DoUpdateSub(draw_f);
  }
  //アクション
  void TUGButton::DoAction()
  {
    //有効かどうか
    if (stBtn_.disable_f_) return;

    if (field_->b_IsBtn())
    {
      //ボタンおされ
      if (stBtn_.enterAct_f_)
      {
        if (field_->b_IsEnter(TRUE))
        {
          SetAct(0);//自分の子番号を渡してやる//やっぱし親が番号管理
          field_->g_Bright();//とりあえずここで明るく
        }
        else
        {
          field_->g_EfReset();//とりあえずここで戻す
        }
      }
      else//ボタン決定（リリースタイミング）
      {
        if (field_->b_IsDecide())
        {
          SetAct(0);//自分の子番号を渡してやる//やっぱし親が番号管理
          //とりあえずここでフラッシュ
          field_->g_Flash();
        }
        else if (field_->b_IsEnter(TRUE))
        {
          field_->g_Bright();//とりあえずここで明るく
        }
        else
        {
          field_->g_EfReset();//とりあえずここで戻す
        }
      }

      DoActionSub();
    }
  }
}
