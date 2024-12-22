#include "dbUGRcGen.h"

namespace db
{
  TUGRcGen::TUGRcGen(TUIGenLayer* player) : TUGGen(player)
  {
    stRec_.Init();

  }
  TUGRcGen::~TUGRcGen(void)
  {

  }
  s32 TUGRcGen::MakeGroup(TUIObj* parent, s32 posx, s32 posy, s32 limx, s32 limy)
  {
    stRec_.posx_ = posx;
    stRec_.posy_ = posy;
    SetParent(parent);
    field_->b_SetBtn(TRUE);
    field_->z_SetPos(posx, posy);
    //field_->b_BtnLimX(limx, SLIDERW);//左右の無効はいらないか
    field_->b_BtnLimY(limy, SLIDERH);
    s32 res = DoMakeGroupSub();

    //継承先が出来てから高さ制限呼ぶ
    SetLimY(limy, SLIDERH);

    return res;
  }
  void TUGRcGen::SetLimY(s32 limu, s32 hhh)
  {
    field_->g_SetLimY(limu, hhh);
    for (s32 i = 0; i < items_.size(); ++i) items_[i]->g_SetLimY(limu, hhh);
    //継承先で必要なら
    DoSetLimY(limu, hhh);
  }
  s32 TUGRcGen::GetPosX()
  {
    return stRec_.posx_;
  }
  s32 TUGRcGen::GetPosY()
  {
    return stRec_.posy_;
  }
  //選択画像／通常画像切り替え（フィールドおよびトップ）
  void TUGRcGen::SetSelect(BOOL select_f)
  {
    if (select_f) field_->o_SetObjGrp(basegrp2_);
    else field_->o_SetObjGrp(basegrp_);

    stRec_.selected_f_ = select_f;
  }
  BOOL TUGRcGen::IsSelect()
  {
    return stRec_.selected_f_;
  }
  //押せるかどうか
  void TUGRcGen::SetPlate(BOOL plate_f)
  {
    stRec_.plate_f_ = plate_f;
  }
  void TUGRcGen::SetDisable(BOOL disable_f)
  {
    stRec_.disable_f_ = disable_f;
    if (disable_f)
    {
      field_->g_Dark();//とりあえずここで暗く
    }
    else
    {
      field_->g_EfReset();//とりあえずここで戻す
    }

  }

  void TUGRcGen::DoUpdate(BOOL draw_f)
  {
    //継承先で必要なら
    DoUpdateSub(draw_f);

    for (s32 i = 0; i < items_.size(); ++i) items_[i]->Update(draw_f);
    field_->Update(draw_f);//最後にフィールド更新で、レコード上のボタンの処理を優先できるか
  }

  void TUGRcGen::DoAction()
  {
    //有効かどうか
    if (stRec_.disable_f_) return;

    //継承先の処理を優先
    DoActionSub();

    //継承先でアクションがなければ
    if (IsAct() == FALSE)
    {
      //押せるかどうか
      if (stRec_.plate_f_ == FALSE)
      {
        if (field_->b_IsDecide())//レコード
        {
          SetAct(0);//
          field_->g_Flash();//とりあえずここで明るく
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
    }


  }
}
