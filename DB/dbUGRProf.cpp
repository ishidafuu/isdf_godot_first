#include "dbUGRProf.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGRProf::TUGRProf(TUIGenLayer* player) : TUGGen(player)//アクションなし
    , ugrkantoku_(player)
  {
    stRProf_.Init();
  }

  TUGRProf::~TUGRProf()
  {

  }
  void TUGRProf::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);
    ugrkantoku_.MakeGroup(field_);

    plate_ = MakeObj();
    plate_->o_SetParent(field_);
    plate_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_prof_message));
    plate_->z_SetPos(-32, 52);
    plate_->f_MakeFont("かんとくからみんなへ", FC2_normal);
    plate_->f_FontPos(0, -14);
    plate_->g_SetDraw(TRUE);

    text0_ = MakeObj();
    text0_->o_SetParent(plate_);
    text0_->z_SetPos(24, 6);
    text0_->f_MakeFont("じこしょうかい１", FC2_normal);
    text0_->g_SetDraw(TRUE);
    text1_ = MakeObj();
    text1_->o_SetParent(plate_);
    text1_->z_SetPos(24, 22);
    text1_->f_MakeFont("じこしょうかい２", FC2_normal);
    text1_->g_SetDraw(TRUE);
    text2_ = MakeObj();
    text2_->o_SetParent(plate_);
    text2_->z_SetPos(24, 38);
    text2_->f_MakeFont("じこしょうかい３", FC2_normal);
    text2_->g_SetDraw(TRUE);

  }

  void TUGRProf::SetProfMe()
  {
    ugrkantoku_.SetNameCode();
    text0_->f_MakeFont(player_->pmgEO_->mgCoU_.mdKantokuSub_.GetMyDtStr(mdkantokusub_prof0_c128).c_str(), FC2_normal);
    text1_->f_MakeFont(player_->pmgEO_->mgCoU_.mdKantokuSub_.GetMyDtStr(mdkantokusub_prof1_c128).c_str(), FC2_normal);
    text2_->f_MakeFont(player_->pmgEO_->mgCoU_.mdKantokuSub_.GetMyDtStr(mdkantokusub_prof2_c128).c_str(), FC2_normal);
  }

  void TUGRProf::SetProfRanker(s64 id_ranker)
  {
    ugrkantoku_.SetNameCodeRanker(id_ranker);
    text0_->f_MakeFont(player_->pmgEO_->mgCoU_.mdre_Ranker_.GetPKDtStr(id_ranker, mdre_ranker_prof0_c128).c_str(), FC2_normal);
    text1_->f_MakeFont(player_->pmgEO_->mgCoU_.mdre_Ranker_.GetPKDtStr(id_ranker, mdre_ranker_prof1_c128).c_str(), FC2_normal);
    text2_->f_MakeFont(player_->pmgEO_->mgCoU_.mdre_Ranker_.GetPKDtStr(id_ranker, mdre_ranker_prof2_c128).c_str(), FC2_normal);
  }
  void TUGRProf::SetProfRankerByData(s64 id_ranker)
  {
	  ugrkantoku_.SetNameCodeRanker(id_ranker);
	  text0_->f_MakeFont(player_->pmgEO_->mgCoU_.rankerData_.GetPKDtStr(id_ranker, mdre_ranker_prof0_c128).c_str(), FC2_normal);
	  text1_->f_MakeFont(player_->pmgEO_->mgCoU_.rankerData_.GetPKDtStr(id_ranker, mdre_ranker_prof1_c128).c_str(), FC2_normal);
	  text2_->f_MakeFont(player_->pmgEO_->mgCoU_.rankerData_.GetPKDtStr(id_ranker, mdre_ranker_prof2_c128).c_str(), FC2_normal);
  }

  void TUGRProf::SetProfEnemy(s32 no)
  {
    ugrkantoku_.SetNameCodeEnemy(no);
    text0_->f_MakeFont(player_->pmgEO_->mgCoU_.mdre_Enemy_.GetLineDtStr(no, mdre_enemy_prof0_c128).c_str(), FC2_normal);
    text1_->f_MakeFont(player_->pmgEO_->mgCoU_.mdre_Enemy_.GetLineDtStr(no, mdre_enemy_prof1_c128).c_str(), FC2_normal);
    text2_->f_MakeFont(player_->pmgEO_->mgCoU_.mdre_Enemy_.GetLineDtStr(no, mdre_enemy_prof2_c128).c_str(), FC2_normal);
  }
  void TUGRProf::SetProfFriend()
  {
    s64 id_friend = player_->pmgEO_->mgCoU_.mdre_Enemy_.GetLineDt(0, mdre_enemy_pk);
    ugrkantoku_.SetNameCodeEnemy(id_friend);
    text0_->f_MakeFont(player_->pmgEO_->mgCoU_.mdre_Enemy_.GetLineDtStr(0, mdre_enemy_prof0_c128).c_str(), FC2_normal);
    text1_->f_MakeFont(player_->pmgEO_->mgCoU_.mdre_Enemy_.GetLineDtStr(0, mdre_enemy_prof1_c128).c_str(), FC2_normal);
    text2_->f_MakeFont(player_->pmgEO_->mgCoU_.mdre_Enemy_.GetLineDtStr(0, mdre_enemy_prof2_c128).c_str(), FC2_normal);
  }

  void TUGRProf::DoUpdate(BOOL draw_f)
  {
    ugrkantoku_.Update(draw_f);
    plate_->Update(draw_f);
    text0_->Update(draw_f);
    text1_->Update(draw_f);
    text2_->Update(draw_f);
  }

}
