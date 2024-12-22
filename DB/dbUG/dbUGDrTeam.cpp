#include "dbUGDrTeam.h"
#include "dbUGLiTmFilter.h"

namespace db
{

  //コンストラクタ
  TUGDrTeam::TUGDrTeam(TUIGenLayer* player) : TUGDrGen(player),
    ura_(player),
    drumend_(player)
  {
    stDrTeam_.Init();
    id_teamvec_.clear();
  }
  TUGDrTeam::~TUGDrTeam()
  {
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
  }
  void TUGDrTeam::DoReady()
  {
    //チームセレクト用不所持チームドラム
    if (GetRecordNum() == 0) return;

    if (stDrTeam_.cnt_f_)
    {
      //データ読み込み
      for (s32 i = 0; i < mid_teamvec_cnt_.size(); ++i)
      {
        records_[i]->SetTeamDt_Cnt(0, mid_teamvec_cnt_[i], GetEndIDTeam());
      }
      LoadTex(mid_teamvec_cnt_[0], FALSE);
      LoadTex(GetEndIDTeam(), TRUE);
      RefreshRoll(TRUE);
    }
    else
    {
		s64 id_kantoku = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(enmdKantoku::mdkantoku_pk);
      s64 mid_team_end = pmgEO_->mgCoU_.mdTeam_.GetPKDt(GetEndIDTeam(), mdteam_id_m_team);
      //データ読み込み
      for (s32 i = 0; i < records_.size(); ++i)
      {
        records_[i]->SetTeamDt2(id_kantoku, GetIDTeam(i), GetMIDTeam(i), pmgEO_->stSeq_.GetMidEvent(), TRUE, GetEndIDTeam());
      }
      LoadTex(GetMIDTeam(0), FALSE);
      LoadTex(mid_team_end, TRUE);
      RefreshRoll(TRUE);
	  DoStop();
    }
  }
  //BOOL TUGDrTeam::DoSelectUra(BOOL ura_f)
  //{
  //  return ura_f;
  //}
  //チームカード
  void TUGDrTeam::MakeGroup(TUIObj* parent, BOOL leftpos_f, s32 sideno, TUGLiTmFilter* ugliTmfilter)
  {
    ugliTmfilter_ = ugliTmfilter;
    stDrGen_.USEOK_F_ = FALSE;
    stDrGen_.USEEYE_F_ = FALSE;
    stDrGen_.USEETC_F_ = FALSE;

    //レコードサイズなど
    stDrGen_.TOPFRONT_F_ = FALSE;
    stDrGen_.REVY_ = -32;
    stDrGen_.DISTY_ = 24;
    stDrGen_.LOOKPOS_ = 5;
    stDrGen_.SLIDEY_ = 68;
    stDrGen_.CARDH_ = 96;

    stDrGen_.EYEX_ = 38 - 24;
    stDrGen_.EYEY_ = (stDrGen_.REVY_ + (stDrGen_.DISTY_ * stDrGen_.LOOKPOS_));//継承先のデータが必要
    stDrGen_.OKX_ = 136 - 24;
    stDrGen_.OKY_ = (stDrGen_.REVY_ + (stDrGen_.DISTY_ * stDrGen_.LOOKPOS_));
    stDrGen_.ETCX_ = 128;
    stDrGen_.ETCY_ = (stDrGen_.REVY_ + (stDrGen_.DISTY_ * stDrGen_.LOOKPOS_) + stDrGen_.CARDH_ - 16);

    //サイズを決めてから継承元のMakeGroupを呼び出す
    GenMakeGroup(parent, leftpos_f);
    stDrTeam_.sideno_ = sideno;


    for (s32 i = 0; i < DRRECNUM; ++i)
    {
      TUGTeam* newrecord = new TUGTeam(player_);
      newrecord->MakeGroup(field_, sideno);//fieldで大丈夫か？
      records_.push_back(newrecord);
      GenAddRecord(newrecord);
    }

    ura_.MakeGroup(field_, stDrTeam_.sideno_);
    GenSetUramen(&ura_);

    drumend_.MakeGroup(field_, stDrTeam_.sideno_);
    GenSetDrumEnd(&drumend_);

  }
  void TUGDrTeam::ReadyFilter(TUGLiTmFilter* ugliTmfilter)
  {
	  ugliTmfilter_ = ugliTmfilter;
	  ReadyFilter();
  }
  void TUGDrTeam::ReadyFilter()
  {
    stDrTeam_.cnt_f_ = FALSE;
    id_teamvec_ = ugliTmfilter_->id_teamvec_;
    Ready(id_teamvec_.size());
    //ミナゲンキ誤爆対策になるか
    RefreshRoll(FALSE);
  }
  void TUGDrTeam::ReadySort()
  {
    stDrTeam_.cnt_f_ = FALSE;
    id_teamvec_ = ugliTmfilter_->id_teamvec_;
    Ready(id_teamvec_.size());
    //ミナゲンキ誤爆対策になるか
    RefreshRoll(FALSE);
  }
  //チームセレクト用不所持チームドラム
  void TUGDrTeam::Ready_Cnt(INTVECTOR* mid_teamvec)
  {
    stDrTeam_.cnt_f_ = TRUE;
    mid_teamvec_cnt_ = *mid_teamvec;
    Ready(mid_teamvec_cnt_.size());
  }

  //スタミナリフレッシュ
  void TUGDrTeam::RefreshStamina()
  {
    records_[GetLookRecord()]->RefreshStamina();
  }
  void TUGDrTeam::RefreshParam(void)
  {
	  for (s32 i = 0; i < records_.size(); i++)
	  {
		  records_[i]->RefreshParam();
	  }
  }
  void TUGDrTeam::RefreshRoll(BOOL ready_f)
  {
    //チームセレクト用不所持チームドラム
    if (stDrTeam_.cnt_f_)
    {
      for (s32 i = 0; i < records_.size(); ++i)
      {
        //所持してるデータ以上
        if (i >= mid_teamvec_cnt_.size()) break;

        s32 lineno = stDrGen_.drrecdtno_[i];
        s64 mid_team = mid_teamvec_cnt_[lineno];
        records_[i]->SetTeamDt_Cnt(0, mid_team, GetEndIDTeam());
      }
    }
    else
    {
		s64 id_kantoku = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(enmdKantoku::mdkantoku_pk);
      for (s32 i = 0; i < records_.size(); ++i)
      {
        //所持してるデータ以上
        if (i >= id_teamvec_.size()) break;

        s32 lineno = stDrGen_.drrecdtno_[i];
        s64 id_team = id_teamvec_[lineno];// pmgEO_->mgCoU_.mdTeam_.GetLineDt(lineno, mdteam_pk);
        s64 mid_team = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_id_m_team);
        s64 mid_team_end = pmgEO_->mgCoU_.mdTeam_.GetLineDt(id_teamvec_[id_teamvec_.size() - 1], mdteam_id_m_team);
        records_[i]->SetTeamDt2(id_kantoku, id_team, mid_team, pmgEO_->stSeq_.GetMidEvent(), ready_f, GetEndIDTeam());
        records_[i]->RefreshStamina();
      }
    }
  }

  s32 TUGDrTeam::GetRecordNum()
  {
    if (stDrTeam_.cnt_f_)
    {
      return mid_teamvec_cnt_.size();
    }
    else
    {
      return id_teamvec_.size();
    }
  }
  void TUGDrTeam::RevUra()
  {
    SelectUra(!(IsUra()));
  }

  //現在見てる位置のシュートＩＤ
  s64 TUGDrTeam::GetIDTeam()
  {
    if (GetRecordNum() == 0) return 0;

    //チームセレクト用不所持チームドラム
    if (stDrTeam_.cnt_f_)
    {
      return 0;
    }
    else
    {
      return id_teamvec_[GetDataNo()];
    }
  }
  //現在見てる位置のシュートＭＩＤ
  s64 TUGDrTeam::GetMIDTeam()
  {
    //チームセレクト用不所持チームドラム
    if (stDrTeam_.cnt_f_)
    {
      return mid_teamvec_cnt_[GetDataNo()];
    }
    else
    {
      return pmgEO_->mgCoU_.mdTeam_.GetPKDt(GetIDTeam(), mdteam_id_m_team);
    }
  }
  s64 TUGDrTeam::GetMIDTeam(s32 recordno)
  {
    s32 lineno = stDrGen_.drrecdtno_[recordno];

    //チームセレクト用不所持チームドラム
    if (stDrTeam_.cnt_f_)
    {
      return mid_teamvec_cnt_[lineno];
    }
    else
    {
      return pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_teamvec_[lineno], mdteam_id_m_team);
    }
  }
  s64 TUGDrTeam::GetIDTeam(s32 recordno)
  {

    //チームセレクト用不所持チームドラム
    if (stDrTeam_.cnt_f_)
    {
      return 0;
    }
    else
    {
      s32 lineno = stDrGen_.drrecdtno_[recordno];
      return id_teamvec_[lineno];
    }
  }
  s32 TUGDrTeam::GetEndIDTeam()
  {

    //チームセレクト用不所持チームドラム
    if (stDrTeam_.cnt_f_)
    {
      return mid_teamvec_cnt_[mid_teamvec_cnt_.size() - 1];
    }
    else
    {
      return id_teamvec_[id_teamvec_.size() - 1];
    }
  }
  void TUGDrTeam::LoadTex(s64 mid_team, BOOL end_f)
  {
    BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_gb_f);
    for (s32 c = 0; c < DBMEMBER_ALL; ++c)
    {
      s32 charid = pmgEO_->mgCoM_.GetCharID(mid_team, c);

      pmgEO_->mgGrp_.pmgChGrp_[SIDE0][c]->SetCharDt(charid, mid_team, gb_f);
      pmgEO_->mgGrp_.pmgChGrp_[SIDE0][c]->SetGrTile(SIDE0, c);
    }
    //胸部とメンバー全員の立ち絵のテクスチャを読んでる
    pmgEO_->mgGrp_.MakeIconTexture_Chest(SIDE0, FALSE, end_f);
    pmgEO_->mgGrp_.MakeIconTexture_Body(SIDE0, 0, 0, FALSE, end_f);//RcMemberでも共通で使ってる

    //if (end_f == FALSE)
    //{
    //  stDrTeam_.mid_team_lastload_ = mid_team;
    //  pmgEO_->mgGrp_.MakeIconTexture_BodySh();//装備シュート用アイコン
    //}
  }
  void TUGDrTeam::DoStop()
  {
    //同じＩＤの時は読み込みいらない
    s32 mid_team = GetMIDTeam();

    if (stDrTeam_.mid_team_lastload_ == mid_team) return;

    LoadTex(mid_team, FALSE);
	records_[stDrGen_.recno_]->RefreshShoot();
	records_[stDrGen_.recno_]->RefreshCharaLv();

	for (s32 i = 0; i < records_.size(); i++)
	{
		records_[i]->RefreshParam();
		records_[i]->SetDrawParam(TRUE);
	}
  }
  void TUGDrTeam::DoRoll()
  {
    //チームセレクト用不所持チームドラム
    if (GetRecordNum() == 0) return;

    RefreshRoll(FALSE);
  }
  void TUGDrTeam::DoStart()
  {
	  for (s32 i = 0; i < records_.size(); i++)
	  {
		  records_[i]->SetDrawParam(FALSE);
	  }
  }

  void TUGDrTeam::DoUpdateSub(BOOL draw_f)//更新
  {
    if (draw_f == FALSE)
    {
      //スタミナバー更新
      if (lib_num::IncLoop(&stDrTeam_.refresh_c_, 0, (30 * 60)))
      {
        RefreshStamina();
      }
    }

  }
  void TUGDrTeam::DoActionSub()//更新
  {
    //現在LookPosにいるカードの細かい情報表示ＯＮ他ＯＦＦ
    for (s32 i = 0; i < records_.size(); ++i)
    {
      records_[i]->SetTipsDraw_f((stDrGen_.recno_ == i) || (stDrGen_.drrecdtno_[i] == (stDrGen_.dtnum_ - 1)));
    }
  }
  BOOL TUGDrTeam::IsActRes(enUGDrTeamAct res)//返値ゲット
  {
    return (GetActRes() == res);
  }

}
