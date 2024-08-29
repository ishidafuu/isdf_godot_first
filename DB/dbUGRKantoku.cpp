#include "dbUGRKantoku.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGRKantoku::TUGRKantoku(TUIGenLayer* player) : TUGGen(player)//アクションなし
    , ugkantoku_(player)
    , ugname_(player)
  {
    stRKantoku_.Init();
    mbgid_ = NULL;
  }

  TUGRKantoku::~TUGRKantoku()
  {

  }
  void TUGRKantoku::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);
    //名前
    ugname_.MakeGroup(field_);
    ugname_.SetPos(0, 0);

    //監督顔アイコン
    ugkantoku_.MakeGroup(field_, TRUE);//enemy
    ugkantoku_.SetMType(kmt_Icon);
    ugkantoku_.SetPos(-14, 12);

    mbgid_ = MakeObj();
    mbgid_->o_SetParent(field_);
    mbgid_->z_SetPos(0, 16);
    mbgid_->g_SetDraw(TRUE);


    //他人監督一覧等の時用に
    //player_->pmgEO_->mgGrp_.pmgcg_kantoku_
    //ではなくマップのボスなど用の監督画像テクスチャを用意し、そこを参照する必要がある
  }

  void TUGRKantoku::GenSetNameCode(s64 mbgid)
  {
    std::string strmbg = "コード：" + player_->pmgEO_->mgCoU_.CnvMBGID(mbgid);
    mbgid_->f_MakeFont(strmbg.c_str(), FC2_normal);
    SetStandIcon();
  }

  void TUGRKantoku::SetName()
  {
    ugname_.SetName(player_->pmgEO_->mgCoU_.mdKantokuSub_.GetMyDtStr(mdkantokusub_name_c32));
    if (mid::midIsDXL())ugname_.SetName("かんとくさん");
  }
  void TUGRKantoku::SetNameCode()
  {
    SetName();
    s64 mbgid = player_->pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_mbgID);
    GenSetNameCode(mbgid);
  }
  void TUGRKantoku::SetNameRanker(s64 id_ranker)
  {
    ugname_.SetName(player_->pmgEO_->mgCoU_.mdre_Ranker_.GetPKDtStr(id_ranker, mdre_ranker_name_c32));
  }
  void TUGRKantoku::SetNameRankerByData(s64 id_ranker)
  {
	  ugname_.SetName(player_->pmgEO_->mgCoU_.rankerData_.GetPKDtStr(id_ranker, mdre_ranker_name_c32));
  }
  void TUGRKantoku::SetNameEnemy(s32 no)
  {
    ugname_.SetName(player_->pmgEO_->mgCoU_.mdre_Enemy_.GetLineDtStr(no, mdre_enemy_name_c32));
  }

  void TUGRKantoku::SetNameCodeRanker(s64 id_ranker)
  {
    SetNameRanker(id_ranker);
    s64 mbgid = player_->pmgEO_->mgCoU_.mdre_Ranker_.GetPKDt(id_ranker, mdre_ranker_mbgID);
    GenSetNameCode(mbgid);
  }
  void TUGRKantoku::SetNameCodeRankerByData(s64 id_ranker)
  {
	  SetNameRanker(id_ranker);
	  s64 mbgid = player_->pmgEO_->mgCoU_.rankerData_.GetPKDt(id_ranker, mdre_ranker_mbgID);
	  GenSetNameCode(mbgid);
  }

  void TUGRKantoku::SetNameCodeEnemy(s32 no)
  {
    SetNameEnemy(no);
    s64 mbgid = player_->pmgEO_->mgCoU_.mdre_Enemy_.GetLineDt(no, mdre_enemy_mbgID);
    GenSetNameCode(mbgid);
  }

  void TUGRKantoku::SetStandIcon()
  {
    ugkantoku_.SetMType(kmt_St);
    ugkantoku_.SetPos(-14, 12 + 16);
  }

  void TUGRKantoku::SetLimY(s32 limu, s32 hhh)
  {
    field_->g_SetLimY(limu, hhh);
    ugname_.GetField()->g_SetLimY(limu, hhh);
    ugkantoku_.GetField()->g_SetLimY(limu, hhh);
    mbgid_->g_SetLimY(limu, hhh);
  }

  void TUGRKantoku::DoUpdate(BOOL draw_f)
  {
    ugname_.Update(draw_f);
    ugkantoku_.Update(draw_f);
    mbgid_->Update(draw_f);
  }

}
