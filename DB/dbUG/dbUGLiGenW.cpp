#include "dbUGLiGenW.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"

namespace db
{
  const s32 SLIDERW2 = (SLIDERW * 2);
  const s32 MPOSX = SLIDERW2 + 2;
  const s32 MPOSY = 23;
  const s32 MPOSH = SLIDERH - 8;

  //コンストラクタ
  TUGLiGenW::TUGLiGenW(TUIGenLayer* player) :TUGGen(player)
  {
    stListW_.Init();
    sliderx_ = 0;
    slidery_ = 0;
  }

  TUGLiGenW::~TUGLiGenW()
  {
    //for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
  }

  void TUGLiGenW::ClearRecord()
  {
    //SetZeroPos();
    //ポインタリストをクリアするだけで、解放は行わない
    stListW_.Init();
    genrecords_.clear();
    //継承先で解放
    DoClearRecord();
    //SetZeroPos();
  }

  //位置戻す
  void TUGLiGenW::SetZeroPos()
  {
    slider_->sl_LineSetPos(0, 0);
    SetRecordPos();
  }

  //選ばれたレコード色づけ（必要なら呼び出す）
  void TUGLiGenW::SetSelect()
  {
    if (GetActIndex() != NGNUM)
    {
      for (s32 i = 0; i < genrecords_.size(); ++i)
      {
        genrecords_[i]->SetSelect((i == GetActIndex()));
      }
    }
  }

  void TUGLiGenW::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);
    field_->g_SetDepth(UIDP_05BUTTON);
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_framewide));

    field_->z_SetPos(-148, -84);// player_->HEADH);

    slider_ = MakeObj();
    slider_->o_SetParent(field_);
    slider_->z_SetPos(4, 24);
    slider_->z_GetGlobalPos(&sliderx_, &slidery_);

    marker_ = MakeObj();
    marker_->o_SetParent(field_);
    marker_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_marker));
    marker_->z_SetPos(MPOSX, MPOSY);
    marker_->g_SetOvOvj(slider_);

    DoMakeGroupSub();
  }

  void TUGLiGenW::GenMakeObj_Record(TUGRcGen* newrecord, s32 rh)
  {
    //追加
    genrecords_.push_back(newrecord);

    //末端位置更新
    if ((genrecords_.size() % 2) == 0)
    {
      stListW_.leftx_ = 0;
      stListW_.bottomy_ += rh;
      stListW_.overh_ = (stListW_.bottomy_ + LISTTOPY - SLIDERH);//レコード全部の高さ+下の8ドット黒帯スペース-スライダーの高さ
    }
    else
    {
      stListW_.leftx_ = SLIDERW;
      stListW_.overh_ = (stListW_.bottomy_ + rh + LISTTOPY - SLIDERH);//レコード全部の高さ+下の8ドット黒帯スペース-スライダーの高さ
    }

//    stListW_.overh_ = (stListW_.bottomy_ + rh + LISTTOPY - SLIDERH);//レコード全部の高さ+下の8ドット黒帯スペース-スライダーの高さ
    if (stListW_.overh_ <= 0)
    {
      stListW_.overh_ = 0;
      slider_->sl_LineSwipeY(FALSE);
    }
    else
    {
      //スライドするように
      slider_->sl_LineSwipeY(TRUE);
      slider_->sl_LineSwipeHeight(0, stListW_.overh_);
      slider_->sl_LineSwipeLimX(sliderx_, SLIDERW2);
    }
  }

  void TUGLiGenW::SetRecordPos()
  {
    //スワイプ
    for (s32 i = 0; i < genrecords_.size(); ++i)
    {
      s32 posx = genrecords_[i]->GetPosX();
      s32 posy = genrecords_[i]->GetPosY() - slider_->stSO_.stLineSw_.posY_ + LISTTOPY;

      genrecords_[i]->SetPos(posx, posy);
    }

    s32 mposy = ((slider_->stSO_.stLineSw_.posY_ * MPOSH) / stListW_.overh_);
    marker_->z_SetPos(MPOSX, MPOSY + mposy);

    if (slider_->stSO_.stLineSw_.swp_f_)
    {
      stListW_.marker_c_ = 16;
    }
    else
    {
      lib_num::AprTo0(&stListW_.marker_c_);
    }

    marker_->g_SetDraw((stListW_.marker_c_ > 0));
  }


  void TUGLiGenW::DoActionSub()
  {
    //必要あれば継承先
  }
  void TUGLiGenW::DoUpdateSub(BOOL draw_f)
  {
    //必要あれば継承先
  }
  void TUGLiGenW::DoMakeGroupSub()
  {
    //必要あれば継承先
  }

  void TUGLiGenW::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    slider_->Update(draw_f);
    marker_->Update(draw_f);

    //レコードのアップデートもここで行ってしまう
    for (s32 i = 0; i < genrecords_.size(); ++i) genrecords_[i]->Update(draw_f);

    //継承先呼び出し
    DoUpdateSub(draw_f);

  }

  void TUGLiGenW::DoAction()
  {
    //スワイプ
    SetRecordPos();

    for (s32 i = 0; i < genrecords_.size(); ++i)
    {
      if (genrecords_[i]->IsAct())
      {
        //レコード内ボタン分アクション番号をすすめる
        SetAct(enUGLiAct_Record + genrecords_[i]->GetActRes(), i);//とりあえず０
        //SetActIndex(i);
        //SetAct(enUGLiEND + genrecords_[i]->GetActRes());//とりあえず０
        //SetActIndex(i);
        break;
      }
    }

    DoActionSub();


  }
  BOOL TUGLiGenW::IsActRes(enUGLiAct res)
  {
    return (GetActRes() == res);
  }
}
