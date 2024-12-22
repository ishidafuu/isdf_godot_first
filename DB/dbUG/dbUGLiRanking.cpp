#include "dbUGLiRanking.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGLiRanking::TUGLiRanking(TUIGenLayer* player) :TUGLiGen(player)
    //, uglistend_(player)
  {
    stLiRanking_.Init();
  }

  TUGLiRanking::~TUGLiRanking()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiRanking::DoClearRecord()
  {
    stLiRanking_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
    for (s32 i = 0; i < records2_.size(); ++i) Freemem(records2_[i]);
    records2_.clear();

	helpCate_.clear();
  }

  void TUGLiRanking::DoMakeGroupSub()
  {
    //field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frameshop));
    //uglistend_.MakeGroup_ListEnd_Shop(slider_, sliderx_, slidery_);
  }

  void TUGLiRanking::RefreshAllEvent(BOOL result_f)
  {
    ClearRecord();
    //現在のランキング

    LONGVECTOR rankingvec = (result_f)
      ? pmgEO_->mgCoM_.GetNowRanking(pmgEO_->mgCoU_.GetResultEvent())  //結果
      : pmgEO_->mgCoM_.GetNowRanking(pmgEO_->mgCoU_.GetNowEvent());
    for (s32 i = 0; i < rankingvec.size(); ++i)
    {
      TUGRcRanking* newrecord = new TUGRcRanking(player_);
      s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
      newrecord->SetRankingDt(rankingvec[i]);
      records_.push_back(newrecord);
      GenMakeObj_Record(newrecord, rh);
    }
  }


  void TUGLiRanking::RefreshEvent(s32 mid_event)
  {
    ClearRecord();
    //現在のランキング
    LONGVECTOR rankingvec;

    //ランキングをさらう
    for (s32 i2 = 0; i2 < pmgEO_->mgCoM_.mdm_Ranking_.GetLineNum(); ++i2)
    {
      if (pmgEO_->mgCoM_.mdm_Ranking_.GetLineDt(i2, mdm_ranking_id_m_event) == mid_event)
      {
        s64 mid_ranking = pmgEO_->mgCoM_.mdm_Ranking_.GetLineDt(i2, mdm_ranking_pk);
        rankingvec.push_back(mid_ranking);
      }
    }

    for (s32 i = 0; i < rankingvec.size(); ++i)
    {
      TUGRcRanking* newrecord = new TUGRcRanking(player_);
      s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
      newrecord->SetRankingDt(rankingvec[i]);
      records_.push_back(newrecord);
      GenMakeObj_Record(newrecord, rh);
    }

    s32 helpNo = 0;
    //for (s32 i = 0; i < rankingvec.size(); ++i)
    //s32 mid_event = pmgEO_->mgCoM_.mdm_Ranking_.GetPKDt(rankingvec[i], mdm_ranking_id_m_event);
    s32 helpType = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(mid_event, mdm_event_helpType);

    if (helpType != 0)
    {
      for (s32 i2 = 0; i2 < pmgEO_->mgCoM_.mdm_Help_.GetLineNum(); ++i2)
      {
        if (pmgEO_->mgCoM_.mdm_Help_.GetLineDt(i2, mdm_help_helpType) == helpType)
        {
          std::string eventname = pmgEO_->mgCoM_.mdm_Help_.GetLineDtStr(i2, mdm_help_title_c64);
          TUGRcNormal* newrecord = new TUGRcNormal(player_);
          s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
          newrecord->ugname_.SetNameIcon(eventname.c_str(), ugi_Nazo);
          newrecord->SetCommonID(helpType);
          records2_.push_back(newrecord);
          GenMakeObj_Record(newrecord, rh);

		  helpCate_.push_back(pmgEO_->mgCoM_.mdm_Help_.GetLineDt(i2, mdm_help_category));

          ++helpNo;
        }
      }
    }


  }

  void TUGLiRanking::DoUpdateSub(BOOL draw_f)
  {
    //uglistend_.Update(draw_f);
  }

  void TUGLiRanking::ChangeListEnd(BOOL listend_f)
  {
    //uglistend_.SetDraw(listend_f);
    for (s32 i = 0; i < records_.size(); ++i) records_[i]->SetDraw(!listend_f);
    for (s32 i = 0; i < records2_.size(); ++i) records2_[i]->SetDraw(!listend_f);
  }
  //   void TUGLiRanking::SetListEnd(s32 recordidx)
  //   {
  //     //レコードから商品MIDを取得
  //     uglistend_.SetDataRanking(GetCommonID(recordidx));
  //   }
  s64 TUGLiRanking::GetCommonID()
  {
    if (GetActIndex() < records_.size())
    {
      return records_[GetActIndex()]->GetCommonID();
    }
    else
    {
      return records2_[GetActIndex() - records_.size()]->GetCommonID();
    }
  }
  s64 TUGLiRanking::GetHelpCategory()
  {
	  if (GetActIndex() < records_.size())
	  {
		  return NGNUM;
	  }
	  else
	  {
		  return helpCate_[GetActIndex() - records_.size()];
	  }
  }
}
