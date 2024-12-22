#include "dbUGScVSTeam.h"


namespace db
{
  const s32 DROPNUM = 4;

  //コンストラクタ
  TUGScVSTeam::TUGScVSTeam(TUIGenLayer* player) : TUGGen(player)
    , ugname_(player)
  {
    stScVSTeam_.Init();
    for (s32 i = 0; i < DBMEMBER_ALL; ++i) ugmem_.push_back(new TUGScVSMem(player));
  }

  TUGScVSTeam::~TUGScVSTeam()
  {
    for (s32 i = 0; i < ugmem_.size(); ++i) Freemem(ugmem_[i]);
  }
  void TUGScVSTeam::MakeGroup(TUIObj* parent, s32 sideno)
  {
    stScVSTeam_.sideno_ = sideno;
    //フィールド
    SetParent(parent);

    if (sideno == SIDE1)
    {
      field_->o_SetObjFilterGrp(512, 512, 0x80, 0x00, 0x00, 0x00);
      field_->z_SetPos_SafeR(0, -48);
    }
    else
    {
      field_->z_SetPos_SafeL(0, -48);
    }
    ugname_.MakeGroup(field_);
    ugname_.SetPos(40, 28);

    for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
    {
      ugmem_[i2]->MakeGroup(parent, sideno, i2);//チームフィールドに載せない方がやりやすいかも
    }
  }
  void TUGScVSTeam::Refresh()
  { 
	s32 id_team = pmgEO_->stShiai_.GetIDTeam(stScVSTeam_.sideno_);
    s32 mid_team = pmgEO_->stShiai_.GetMidTeam(stScVSTeam_.sideno_);
    s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetLineDt(mid_team, mdm_team_rarity);
	BOOL isCOM = pmgEO_->stShiai_.IsCOM(stScVSTeam_.sideno_);

	s32 lv = 0;
	// 通信対戦時
	if (pmgEO_->stShiai_.IsVSHum())
	{
		s32 myside = mid::midGetMyIdx();

		// 自分
		if (myside == stScVSTeam_.sideno_)
		{
			lv = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);
		}
		// 相手
		else
		{
			lv = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);
		}
	}
	else
	{
		// 自分
		if (SIDE0 == stScVSTeam_.sideno_)
		{
			lv = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);
		}
		// 勝ち抜きイベント時の相手チーム
		else if (pmgEO_->stShiai_.IsTour() && SIDE1 == stScVSTeam_.sideno_)
		{
			lv = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);
		}
		// COM
		else
		{
			// 1のまま
			lv = 1;
		}
	}

	// COMはレベル表示しない
	if(isCOM)
	{
		ugname_.SetNameTeam(mid_team, NGNUM);
	}
	else
	{
		ugname_.SetNameTeam(mid_team, lv);
	}

    //チーム名
    ugname_.SetIcon(FALSE);
    //std::string name = pmgEO_->mgDt_.dtTeam_.GetTeamName(mid_team);
    //name_->f_MakeFont(name.c_str(), (FC2_rareD + rarity));
    //レアリティ
    //team_[i].rare_->g_SetPartNo(rarity);
    for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
    {
      ugmem_[i2]->Refresh();
    }
  }

  void TUGScVSTeam::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    ugname_.Update(draw_f);
    //rare_->Update(draw_f, noinput_f);
    for (s32 i = 0; i < ugmem_.size(); ++i) ugmem_[i]->Update(draw_f);
  }

}
