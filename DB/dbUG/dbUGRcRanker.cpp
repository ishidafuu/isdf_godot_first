#include "dbUGRcRanker.h"

namespace db
{
  TUGRcRanker::TUGRcRanker(TUIGenLayer* player) : TUGRcNormal(player)
  {
  }
  TUGRcRanker::~TUGRcRanker(void)
  {
  
  }
  void TUGRcRanker::DoMakeGroupSub2()
  {
    basegrp_ = pmgUI_->GetUGGrp(ug_list_record);
    field_->o_SetObjGrp(basegrp_);
  }
  void TUGRcRanker::DoSetLimY2(s32 limu, s32 hhh)
  {
  }
  void TUGRcRanker::DoUpdateSub2(BOOL draw_f)
  {
  }
  //商品データ
  void TUGRcRanker::SetRankerDt(s64 id_ranker, s32 lineno, s32 id_ranking)
  {
    SetCommonID(id_ranker);
    ugname_.SetNameRanker(id_ranker, lineno);

    s32 rank = pmgEO_->mgCoU_.mdre_Ranker_.GetPKDt(id_ranker, mdre_ranker_rank);
    std::string rankstr = "ＲＡＮＫ＿" + lib_str::IntToStr(rank);
    SetSubName(rankstr.c_str());

	s32 scoretype = pmgEO_->mgCoM_.mdm_Ranking_.GetPKDt(id_ranking, mdm_ranking_scoreType);

	s32 score = pmgEO_->mgCoU_.mdre_Ranker_.GetPKDt(id_ranker, mdre_ranker_score);
	std::string scoreStr = "";
	if (scoretype == 8)	// タイムアタック
	{
		// 99:59以上は　負負：負負　で表示
		// フレーム数として、１００：００になるような値を初回アクセス時入れるようにするので
		// 60 * 60 * 100以上のとき判断でよいかとおもいます
		s64 INVALID_NUM = 60 * 60 * 100;
		if (score >= INVALID_NUM)
		{
			scoreStr = "負負" + EtcWord(base::ewColon) + "負負" + EtcWord(base::ewColon) + "負負";
		}
		else
		{
			scoreStr = base::MinSecMSecString(score, FALSE);
		}
	}
	else
	{
		scoreStr = lib_str::IntToStr(score);
	}
	SetValue(scoreStr.c_str());

    s64 createdate = pmgEO_->mgCoU_.mdre_Ranker_.GetPKDt(id_ranker, mdre_ranker_create_date);
    std::string datestr = base::UnixtimeToDateString(createdate, TRUE);
    //SubValue
    SetSubValue(datestr.c_str());
  }
}
