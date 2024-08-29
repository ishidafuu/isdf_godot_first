#include "dbUGRadio.h"

namespace db
{

  //コンストラクタ
  TUGRadio::TUGRadio(TUIGenLayer* player) :TUGGen(player)//スイッチ
  {
    stRadio_.Init();
    tabbtn_.clear();
    stRadio_.radio_f_ = TRUE;
  }

  TUGRadio::~TUGRadio()
  {
  }

  void TUGRadio::MakeGroup(TUIObj* parent, s32 baseX, s32 baseY)
  {
    field_->o_SetParent(parent);
    field_->g_SetDepth(UIDP_05BUTTON);
    stRadio_.baseX_ = baseX;
    stRadio_.baseY_ = baseY;
  }

  void TUGRadio::AddButton(TUGButton* ugbtn, s32 distX)
  {
    ugbtn->SetParent(field_);
    ugbtn->SetPos(stRadio_.baseX_ + (tabbtn_.size() * distX), stRadio_.baseY_);
    tabbtn_.push_back(ugbtn);

    if (tabbtn_.size() == 1)
    {
      ugbtn->SetSelect(TRUE);
    }
  }
  void TUGRadio::ResetSelect()
  {
    SetSelect(0);
  }
  void TUGRadio::SetSelect(s32 tabno)
  {
    for (s32 i = 0; i < tabbtn_.size(); ++i)
    {
      tabbtn_[i]->SetSelect((i == tabno));
    }
  }
  void TUGRadio::SetRadio(BOOL radio_f)
  {
    stRadio_.radio_f_ = radio_f;
  }

	TUGButton* TUGRadio::GetSelect(s32 tabno)
	{
        if (tabbtn_.at(tabno) != NULL)
        {
            return tabbtn_[tabno];
        }
        else
        {
            s32 asdf = 0;
            return NULL;
        }
	}

  void TUGRadio::DoUpdate(BOOL draw_f)
  {
    for (s32 i = 0; i < tabbtn_.size(); ++i)
    {
      tabbtn_[i]->Update(draw_f);
    }

  }

	void TUGRadio::Clear()
	{
		tabbtn_.clear();
	}

  //チェック
  void TUGRadio::DoAction()
  {
    s32 actbtn = NGNUM;
    for (s32 i = 0; i < tabbtn_.size(); ++i)
    {
      if (tabbtn_[i]->IsAct())
      {
        SetAct(0, i);
        actbtn = i;
        break;
      }
    }

    //選ばれたレコード色づけ
    if (stRadio_.radio_f_)
    {
      if (actbtn != NGNUM)
      {
        for (s32 i = 0; i < tabbtn_.size(); ++i)
        {
          tabbtn_[i]->SetSelect((i == actbtn));
        }
      }
    }

  }

}
