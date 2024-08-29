#include "dbUGLiGen.h"
#include "dbUILayer.h"
#include "dbUGRcGen.h"
#include "dbUGRcNormal.h"
namespace db
{
  const s32 MPOSX = 146;
  const s32 MPOSY = 23;
  const s32 MPOSH = SLIDERH - 8;

  const s32 DFW = 160;
  const s32 DFH = 160;

  //コンストラクタ
  TUGLiGen::TUGLiGen(TUIGenLayer* player) :TUGGen(player)
    , ugradio_sub_(player)
    , ugrname_(player)
  {
    stList_.Init();
    sliderx_ = 0;
    slidery_ = 0;
  }

  TUGLiGen::~TUGLiGen()
  {
    //サブボタン解放
    for (s32 i = 0; i < ugbtn_sub_.size(); ++i) Freemem(ugbtn_sub_[i]);
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiGen::ClearRecord()
  {
    //ポインタリストをクリアするだけで、解放は行わない
    stList_.Init();
    genrecords_.clear();
    //継承先で解放
    DoClearRecord();
  }

  //位置戻す
  void TUGLiGen::SetZeroPos()
  {
    slider_->sl_LineSetPos(0, 0);
    SetRecordPos();
  }

  //選ばれたレコード色づけ（必要なら呼び出す）選ばれた以外の奴は消えるので、複数選択にはつかえない
  void TUGLiGen::SetSelect()
  {
    if (GetActIndex() != NGNUM)
    {
      for (s32 i = 0; i < genrecords_.size(); ++i)
      {
        genrecords_[i]->SetSelect((i == GetActIndex()));
      }
    }
  }
  void TUGLiGen::SetSliderDisable(BOOL disable_f)
  {
    if (disable_f)
    {
      //stList_.overh_ = 0;
      slider_->sl_LineSwipeY(FALSE);//Ｙラインスワイプ
      stList_.slide_f_ = FALSE;
    }
    else
    {
      //スライドするように
      slider_->sl_LineSwipeY(TRUE);//Ｙラインスワイプ
      slider_->sl_LineSwipeHeight(0, stList_.overh_);
	  slider_->sl_LineSwipeLimX(sliderx_, SLIDERW);
    }
  }


  void TUGLiGen::SetLRPos(BOOL leftpos_f)
  {
    s32 posx = (leftpos_f)
      ? 64
      : 232;
	field_->z_SetPos_Full(posx, 44);
  }
  void TUGLiGen::SetSelect(s32 idx)
  {
    for (s32 i = 0; i < genrecords_.size(); ++i)
    {
      genrecords_[i]->SetSelect((i == idx));
    }
  }
  s32 TUGLiGen::GetSelect()
  {
    s32 res = 0;
    for (s32 i = 0; i < genrecords_.size(); ++i)
    {
      if (genrecords_[i]->IsSelect())
      {
        res = i;
        break;;
      }
    }
    return res;
  }
  INTVECTOR TUGLiGen::GetSelectedRec()
  {
    INTVECTOR res;
    res.clear();
    for (s32 i = 0; i < genrecords_.size(); ++i)
    {
      if (genrecords_[i]->IsSelect()) res.push_back(i);
    }

    return res;
  }
  s32 TUGLiGen::GetSelectNum()
  {
    s32 res = 0;
    for (s32 i = 0; i < genrecords_.size(); ++i)
    {
      if (genrecords_[i]->IsSelect()) ++res;
    }

    return res;
  }
  void TUGLiGen::GenSetListEnd(BOOL listend_f)
  {
    slider_->sl_LineSwipeY(stList_.slide_f_ && (listend_f == FALSE));//Ｙラインスワイプ必要有れば戻す
    for (s32 i = 0; i < genrecords_.size(); ++i) genrecords_[i]->SetDraw(!listend_f);

  }

  void TUGLiGen::MakeGroup(TUIObj* parent, BOOL leftpos_f)
  {
    SetParent(parent);
    field_->g_SetDepth(UIDP_05BUTTON);//リストはボタンの高さ
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frame));//ショップの時は変える
    SetLRPos(leftpos_f);

    slider_ = MakeObj();
    slider_->o_SetParent(field_);
    //slider_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_marker));
    slider_->z_SetPos(4, 24);
    slider_->z_GetGlobalPos(&sliderx_, &slidery_);

    marker_ = MakeObj();
    marker_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_marker));
    marker_->o_SetParent(field_);
    marker_->z_SetPos(MPOSX, MPOSY);
    marker_->g_SetOvOvj(slider_);

    ugradio_sub_.MakeGroup(field_, 12, 12);

    ugrname_.MakeGroup(field_);
    ugrname_.SetRightShift();
    ugrname_.SetPos(144, 8);
    ugrname_.SetDraw(FALSE);
    //ugradio_sub_.SetPos(8, 8);

    DoMakeGroupSub();
    //リストは全て最初消しとく
    SetDraw(FALSE);
  }
  TUGBtnLabel* TUGLiGen::MakeSubBtn(const std::string& label, enUGPIcon iconno)
  {
    TUGBtnLabel* newbtn = new TUGBtnLabel(player_);
    newbtn->MakeGroup(field_, ug_btn_icon, ug_btn_icon2);
    //ugbtn_sub_.push_back(newbtn);
    const s32 BX = 12;
    const s32 BY = 12;
    const s32 DX = 24;
    newbtn->SetPos(BX + (DX * (ugbtn_sub_.size() - 1)), BY);
    newbtn->uglabel_.ugname_.SetName(label);
    newbtn->ugricon_.SetIcon(iconno);
    //newbtn->uglabel_.GetField()->g_SetDepth(UIDP_09MSGBOX);//最前面に
    newbtn->SetDraw(TRUE);

    //ラジオボタンに追加
    ugradio_sub_.AddButton(newbtn, DX);
    return newbtn;
  }

  void TUGLiGen::GenMakeObj_Record(TUGRcGen* newrecord, s32 rh)
  {
    //追加
    genrecords_.push_back(newrecord);

    //末端位置更新
    stList_.bottomy_ += rh;
    stList_.overh_ = (stList_.bottomy_ + (LISTTOPY * 2) - SLIDERH);//レコード全部の高さ+下の8ドット黒帯スペース-スライダーの高さ
    if (stList_.overh_ <= 0)
    {
      stList_.overh_ = 0;
      slider_->sl_LineSwipeY(FALSE);//Ｙラインスワイプ
      stList_.slide_f_ = FALSE;
    }
    else
    {
      //スライドするように
      slider_->sl_LineSwipeY(TRUE);//Ｙラインスワイプ
      slider_->sl_LineSwipeHeight(0, stList_.overh_);
      slider_->sl_LineSwipeLimX(sliderx_, SLIDERW);
	  stList_.slide_f_ = TRUE;
    }
  }

  void TUGLiGen::SetRecordPos()
  {
    //スワイプ
    for (s32 i = 0; i < genrecords_.size(); ++i)
    {
      s32 posy = genrecords_[i]->GetPosY() - slider_->stSO_.stLineSw_.posY_ + LISTTOPY;
      genrecords_[i]->SetPos(0, posy);
    }

    s32 mposy = ((slider_->stSO_.stLineSw_.posY_ * MPOSH) / stList_.overh_);
    marker_->z_SetPos(MPOSX, MPOSY + mposy);

    if (slider_->stSO_.stLineSw_.swp_f_)
    {
      stList_.marker_c_ = 16;
    }
    else
    {
      lib_num::AprTo0(&stList_.marker_c_);
    }

    marker_->g_SetDraw((stList_.marker_c_ > 0));
  }
  void TUGLiGen::SetNamePage(s32 nowpage, s32 maxpage)
  {
    ugrname_.SetPage(nowpage, maxpage);
    ugrname_.SetDraw(TRUE);
  }


  void TUGLiGen::DoActionSub()
  {
    //必要あれば継承先
  }
  void TUGLiGen::DoUpdateSub(BOOL draw_f)
  {
    //必要あれば継承先
  }
  void TUGLiGen::DoMakeGroupSub()
  {
    //必要あれば継承先
  }

  void TUGLiGen::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    slider_->Update(draw_f);
    marker_->Update(draw_f);

    ugradio_sub_.Update(draw_f);
    ugrname_.Update(draw_f);

    //レコードのアップデートもここで行ってしまう
    for (s32 i = 0; i < genrecords_.size(); ++i) genrecords_[i]->Update(draw_f);

    //継承先呼び出し
    DoUpdateSub(draw_f);

  }

  void TUGLiGen::DoAction()
  {
    //スワイプ
    SetRecordPos();

    for (s32 i = 0; i < genrecords_.size(); ++i)
    {
      if (genrecords_[i]->IsAct())
      {
        //レコード内ボタン分アクション番号をすすめる。レコードの行数はインデックス
        SetAct(enUGLiAct_Record + genrecords_[i]->GetActRes(), i);//とりあえず０
        //SetActIndex(i);
        break;
      }
    }

    //サブボタン
    if (IsAct() == FALSE)
    {
      if (ugradio_sub_.IsAct())
      {
        SetAct((enUGLiAct_SubA + ugradio_sub_.GetActIndex()), 0);
      }
    }

    DoActionSub();
  }

  BOOL TUGLiGen::IsActRes(enUGLiAct res)
  {
    return (GetActRes() == res);
  }
}
