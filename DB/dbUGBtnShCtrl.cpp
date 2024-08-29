#include "dbUGBtnShCtrl.h"
#include "dbMgUI.h"
#include "dbUIGenLayer.h"
#include "dbGame.h"

namespace db
{
  enum enCtrlType
  {
    ct_Retro,
    ct_Smart,
    ct_Auto,
    ct_Tech,
	ct_Debug,
  };

  //コンストラクタ
  TUGBtnShCtrl::TUGBtnShCtrl(TUIGenLayer* player) :TUGButton(player)
  {
    stBtnShCtrl_.Init();
  }
  TUGBtnShCtrl::~TUGBtnShCtrl(void)
  {
  }
  void TUGBtnShCtrl::MakeGroupShCtrl(TUIObj* parent)
  {
    GenMakeGroup(parent, TRUE);//センタリングする
    SetPos(-160+12, -108+12);
    SetDepth(UIDP_05BUTTON);
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_btn_icon));
    field_->b_SetModal(TRUE);
    field_->g_SetPartNo(0);//0番画像
    ugricon_.SetIcon(ugi_CtrlRetro);
  }

  void TUGBtnShCtrl::NextCtrl()
  {
    if (IsAct() == FALSE) return;

    //switch (stBtnShCtrl_.ctrltype_)
    //{
    //case ct_Smart: stBtnShCtrl_.ctrltype_ = ct_Retro; break;
    //case ct_Retro: stBtnShCtrl_.ctrltype_ = ct_Tech; break;
    //case ct_Auto: stBtnShCtrl_.ctrltype_ = ct_Smart; break;
    //case ct_Tech:stBtnShCtrl_.ctrltype_ = ct_Auto; break;
    //}


    if (mid::midIsSemiAuto())
    {
        // デバッグ用
        if (mid::midIsDebug())
        {
            switch (stBtnShCtrl_.ctrltype_)
            {
            case ct_Retro:
                stBtnShCtrl_.ctrltype_ = ct_Smart;
                ugricon_.SetIcon(ugi_CtrlSmart);
                break;
            case ct_Smart:
                stBtnShCtrl_.ctrltype_ = ct_Auto;
                ugricon_.SetIcon(ugi_ShSetAuto);
                break;
            case ct_Auto:
                stBtnShCtrl_.ctrltype_ = ct_Debug;
                ugricon_.SetIcon(ugi_Ball);
                break;
            case ct_Debug:
                stBtnShCtrl_.ctrltype_ = ct_Retro;
                ugricon_.SetIcon(ugi_CtrlRetro);
                break;
            }
            //アイコン切り替え
            field_->g_SetPartNo(stBtnShCtrl_.ctrltype_);
            return;
        }

        //---------------------------------------------------------


        switch (stBtnShCtrl_.ctrltype_)
        {
        case ct_Retro:
            stBtnShCtrl_.ctrltype_ = ct_Smart;
            ugricon_.SetIcon(ugi_CtrlSmart);
            break;
        case ct_Smart:
            stBtnShCtrl_.ctrltype_ = ct_Auto;
            ugricon_.SetIcon(ugi_ShSetAuto);
            break;
        case ct_Auto:
            stBtnShCtrl_.ctrltype_ = ct_Retro;
            ugricon_.SetIcon(ugi_CtrlRetro);
            break;
        }
    }
    else
    {
        // デバッグ用
        if (mid::midIsDebug())
        {
        	switch (stBtnShCtrl_.ctrltype_)
        	{
        	case ct_Retro:
        		stBtnShCtrl_.ctrltype_ = ct_Smart;
        		ugricon_.SetIcon(ugi_CtrlSmart);
        		break;
        	case ct_Smart:
        		stBtnShCtrl_.ctrltype_ = ct_Debug;
        		ugricon_.SetIcon(ugi_Ball);
        		break;

        	case ct_Debug:
        		stBtnShCtrl_.ctrltype_ = ct_Retro;
        		ugricon_.SetIcon(ugi_CtrlRetro);
        		break;
        	}
        	//アイコン切り替え
        	field_->g_SetPartNo(stBtnShCtrl_.ctrltype_);
        	return;
        }

        //---------------------------------------------------------


        switch (stBtnShCtrl_.ctrltype_)
        {
        case ct_Retro:
            stBtnShCtrl_.ctrltype_ = ct_Smart;
            ugricon_.SetIcon(ugi_CtrlSmart);
            break;
        case ct_Smart:
            stBtnShCtrl_.ctrltype_ = ct_Retro;
            ugricon_.SetIcon(ugi_CtrlRetro);
            break;
        }
    }

	//---------------------------------------------------------



    //アイコン切り替え
    field_->g_SetPartNo(stBtnShCtrl_.ctrltype_);
  }
  BOOL TUGBtnShCtrl::IsSmart()
  {
    return (stBtnShCtrl_.ctrltype_ == ct_Smart);
  }
  BOOL TUGBtnShCtrl::IsRetro()
  {
    return (stBtnShCtrl_.ctrltype_ == ct_Retro);
  }
  BOOL TUGBtnShCtrl::IsAuto()
  {
    return (stBtnShCtrl_.ctrltype_ == ct_Auto);
  }
  BOOL TUGBtnShCtrl::IsTech()
  {
    return (stBtnShCtrl_.ctrltype_ == ct_Tech);
  }
  BOOL TUGBtnShCtrl::IsDebug()
  {
	  return (stBtnShCtrl_.ctrltype_ == ct_Debug);
  }
  void TUGBtnShCtrl::SetSmart()
  {
    stBtnShCtrl_.ctrltype_ = ct_Smart;
    ugricon_.SetIcon(ugi_CtrlSmart);
    field_->g_SetPartNo(stBtnShCtrl_.ctrltype_);
  }
  void TUGBtnShCtrl::SetRetro()
  {
    stBtnShCtrl_.ctrltype_ = ct_Retro;
    ugricon_.SetIcon(ugi_CtrlRetro);
    field_->g_SetPartNo(stBtnShCtrl_.ctrltype_);
  }
  void TUGBtnShCtrl::SetDebug(void)
  {
	  stBtnShCtrl_.ctrltype_ = ct_Debug;
	  ugricon_.SetIcon(ugi_Ball);
	  field_->g_SetPartNo(stBtnShCtrl_.ctrltype_);
  }
  s32 TUGBtnShCtrl::GetCtrl()
  {
    return stBtnShCtrl_.ctrltype_;
  }
  void TUGBtnShCtrl::SetDisable(BOOL disable_f)
  {
    if (disable_f)
    {
      field_->g_Dark();
      field_->b_SetBtn(FALSE);
    }
  }

  //更新
  void TUGBtnShCtrl::DoUpdateSub(BOOL draw_f)
  {

  }

}