#include "dbUGTeamUra.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGTeamUra::TUGTeamUra(TUIGenLayer* player) :TUGGen(player)
    , ugname_(player)
  {
    stTeamUra_.Init();
  }

  TUGTeamUra::~TUGTeamUra()
  {

  }
  void TUGTeamUra::MakeGroup(TUIObj* parent, s32 sideNo)
  {
    //SetParentなかったので位置ずれるようなら確認
    SetParent(parent);

    stTeamUra_.sideNo_ = sideNo;
    //チームカード地
    field_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_team_card));
    
    //キャプテン
    cap_ = MakeObj();
    cap_->o_SetParent(field_);
    cap_->o_SetObjGrPart(player_->pmgEO_->mgGrp_.pgr_IconBody_[stTeamUra_.sideNo_]);
    cap_->z_SetPos(120, 50);
    cap_->g_SetMirror(TRUE);

    //チーム名
    ugname_.MakeGroup(field_);
    ugname_.SetPos(43, 8);

    //限界突破数
    overlapnum_ = MakeObj();
    overlapnum_->o_SetParent(field_);
    overlapnum_->z_SetPos(120, 13);

    //装備シュート数
    shotnum_ = MakeObj();
    shotnum_->o_SetParent(field_);
    shotnum_->z_SetPos(32, 8);

    for (s32 i = 0; i < TEXTLEN; ++i)
    {
      text_[i] = MakeObj();
      text_[i]->o_SetParent(field_);
      text_[i]->z_SetPos(16, 20 + (12 * i));
    }
    
  }

  void TUGTeamUra::SetTeamUraDt(s32 mid_team)
  {
    if (stTeamUra_.mid_team_ == mid_team) return;
    stTeamUra_.mid_team_ = mid_team;
    
    s32 rarity = player_->pmgEO_->mgCoM_.mdm_Team_.GetLineDt(mid_team, mdm_team_rarity);
    s32 droprarity = rarity;//ドロップのレアリティにする
    s32 droprarity2 = rarity + 1;
    if (droprarity2 >= 6)droprarity2 = 6;

    //カードカラー
    field_->g_SetPartNo(RARITYNUM + rarity);

    ugname_.SetNameTeam(mid_team);
    ugname_.SetIcon(FALSE);

    enDtShElmStr fontcol = (enDtShElmStr)(FC2_rareD + rarity);
    text_[0]->f_MakeFont(player_->pmgEO_->mgCoM_.mdm_Team_.GetLineDtStr(mid_team, mdm_team_text0_c64).c_str(), fontcol);
    text_[1]->f_MakeFont(player_->pmgEO_->mgCoM_.mdm_Team_.GetLineDtStr(mid_team, mdm_team_text1_c64).c_str(), fontcol);
    text_[2]->f_MakeFont(player_->pmgEO_->mgCoM_.mdm_Team_.GetLineDtStr(mid_team, mdm_team_text2_c64).c_str(), fontcol);
    text_[3]->f_MakeFont(player_->pmgEO_->mgCoM_.mdm_Team_.GetLineDtStr(mid_team, mdm_team_text3_c64).c_str(), fontcol);
  }

  void TUGTeamUra::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    ugname_.Update(draw_f);
    overlapnum_->Update(draw_f);
    shotnum_->Update(draw_f);
    cap_->Update(draw_f);
    for (s32 i = 0; i < TEXTLEN; ++i) text_[i]->Update(draw_f);
  }

  BOOL TUGTeamUra::IsActRes(enUGTeamUraAct res)//返値ゲット
  {
    return (GetActRes() == res);
  }

}
