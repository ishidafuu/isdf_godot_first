#include "dbUGTitle.h"


namespace db
{
  //コンストラクタ
  TUGTitle::TUGTitle(TUIGenLayer* player) :TUGGen(player)
  {
  }

  TUGTitle::~TUGTitle()
  {

  }

  void TUGTitle::MakeGroup(TUIObj* parent, BOOL ovfilter_f)
  {
    SetParent(parent);
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_itemget_title));
    field_->g_SetCenter(TRUE);
    field_->f_SetCenter(TRUE);
    if (ovfilter_f)
    {
      field_->g_SetDepth(UIDP_09MSGBOX);
    }
    else
    {
      field_->g_SetDepth(UIDP_06BUTTONHI);
    }
  }
  void TUGTitle::MakeGroup_Mini(TUIObj* parent, BOOL ovfilter_f)
  {
	  SetParent(parent);
	  field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_itemget_title_mini));
	  field_->g_SetCenter(TRUE);
	  field_->f_SetCenter(TRUE);
	  if (ovfilter_f)
	  {
		  field_->g_SetDepth(UIDP_09MSGBOX);
	  }
	  else
	  {
		  field_->g_SetDepth(UIDP_06BUTTONHI);
	  }
  }
  void TUGTitle::SetMessage(const std::string& msg)
  {
    field_->f_MakeFont(msg.c_str(), FC2_normal);
  }
  void TUGTitle::SetLRPos(BOOL leftpos_f)
  {
    if (leftpos_f)
    {
      field_->z_SetPos_Full(232 - 76 - 16, 44 + 12);
    }
    else
    {
      field_->z_SetPos_Full(232 + 76, 44 + 12);
    }
  }
	void TUGTitle::SetLRPos_BRank(BOOL leftpos_f)
	{
		if (leftpos_f)
		{
			field_->z_SetPos_Full(222, 35);
		}
		else
		{
			field_->z_SetPos_Full(222, 35);
		}
	}

  void TUGTitle::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
  }

  void TUGTitle::DoAction()
  {
  }
}
