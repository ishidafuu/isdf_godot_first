#include "dbUGLiRanker.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGLiRanker::TUGLiRanker(TUIGenLayer* player) :TUGLiGen(player)
    //, uglistend_(player)
  {
    stLiRanker_.Init();
  }

  TUGLiRanker::~TUGLiRanker()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiRanker::DoClearRecord()
  {
    stLiRanker_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);

    records_.clear();
    for (s32 i = 0; i < records2_.size(); ++i) Freemem(records2_[i]);

    records2_.clear();
  }

  void TUGLiRanker::DoMakeGroupSub()
  {

    //ev_icon_.push_back(player_->LoadGrp("event", "ev_icon0"));
    //ev_icon_.push_back(player_->LoadGrp("event", "ev_icon1"));
    //ev_icon_.push_back(player_->LoadGrp("event", "ev_icon2"));
    //ev_icon_.push_back(player_->LoadGrp("event", "ev_icon3"));
  }
  TUGRcNormal* TUGLiRanker::MakeScoreRecord()
  {
    TUGRcNormal* newrecord = new TUGRcNormal(player_);
    s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
    records2_.push_back(newrecord);
    GenMakeObj_Record(newrecord, rh);

    return newrecord;
  }
  void TUGLiRanker::Refresh(BOOL myscore_f, s64 id_ranking)
  {
    ClearRecord();
    stLiRanker_.myscore_f_ = myscore_f;

    for (s32 i = 0; i < pmgEO_->mgCoU_.mdre_Ranker_.GetLineNum(); ++i)
    {
      TUGRcRanker* newrecord = new TUGRcRanker(player_);
      s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
      s64 id_ranker = pmgEO_->mgCoU_.mdre_Ranker_.GetLineDt(i, mdre_ranker_pk);

	  //s32 mid_Event = pmgEO_->mgCoU_.mdlog_Event_.GetLineDt(0, mdlog_event_id_m_event);

	  newrecord->SetRankerDt(id_ranker, i, id_ranking);
      records_.push_back(newrecord);
      GenMakeObj_Record(newrecord, rh);

      //1行目を入れ終わった段階でマイスコア表示必要なとき入れる
      if ((i == 0) && myscore_f && (pmgEO_->mgCoU_.mdlog_Event_.GetLineNum() > 0))
      {
		  s32 scoretype = pmgEO_->mgCoM_.mdm_Ranking_.GetPKDt(id_ranking, mdm_ranking_scoreType);
        if (scoretype > 0)
        {

          switch (scoretype)
          {
          case 1://1勝数
          {
            TUGRcNormal* newrecord = MakeScoreRecord();
            s32 score = pmgEO_->mgCoU_.mdlog_Event_.GetLineDt(0, mdlog_event_win);
            std::string strscore = "しょうりすう";
            newrecord->ugname_.SetNameIcon(strscore.c_str(), ugi_RankBest);
            newrecord->SetValue(lib_str::IntToStr(score).c_str());
          }
          break;
          case 2://2連勝数
          {
            TUGRcNormal* newrecord = MakeScoreRecord();
            s32 wins = pmgEO_->mgCoU_.mdlog_Event_.GetLineDt(0, mdlog_event_wins);
            std::string strwins = "さいだいれんしょう";
            newrecord->ugname_.SetNameIcon(strwins.c_str(), ugi_RankBest);
            newrecord->SetValue(lib_str::IntToStr(wins).c_str());
          }
          {
            TUGRcNormal* newrecord = MakeScoreRecord();
            s32 nowwins = pmgEO_->mgCoU_.mdlog_Event_.GetLineDt(0, mdlog_event_nowwins);
            //ここで挑戦中がみえてるということは０
            if (pmgEO_->mgCoU_.mdlog_Event_.GetLineDt(0, mdlog_event_play_f))
            {
              nowwins = 0;
            }
            std::string strnowwins = "げんざいれんしょう";
            newrecord->ugname_.SetNameIcon(strnowwins.c_str(), ugi_RankNow);
            newrecord->SetValue(lib_str::IntToStr(nowwins).c_str());
          }
          break;
          case 3:// 3最大ダメ
          {
            TUGRcNormal* newrecord = MakeScoreRecord();
            s32 score = pmgEO_->mgCoU_.mdlog_Event_.GetLineDt(0, mdlog_event_maxdam);
            std::string strscore = "さいだいダメージ";
            newrecord->ugname_.SetNameIcon(strscore.c_str(), ugi_RankBest);
            newrecord->SetValue(lib_str::IntToStr(score).c_str());
          }
            break;
          case 4://4累積ダメ
          {
            TUGRcNormal* newrecord = MakeScoreRecord();
            s32 score = pmgEO_->mgCoU_.mdlog_Event_.GetLineDt(0, mdlog_event_totaldam);
            std::string strscore = "ごうけいダメージ";
            newrecord->ugname_.SetNameIcon(strscore.c_str(), ugi_RankBest);
            newrecord->SetValue(lib_str::IntToStr(score).c_str());
          }
            break;
          case 5://5倒した数
          {
            TUGRcNormal* newrecord = MakeScoreRecord();
            s32 score = pmgEO_->mgCoU_.mdlog_Event_.GetLineDt(0, mdlog_event_kill);
            std::string strscore = "たおしたかず";
            newrecord->ugname_.SetNameIcon(strscore.c_str(), ugi_RankBest);
            newrecord->SetValue(lib_str::IntToStr(score).c_str());
          }
            break;
          case 6://6アイテム
          {
            //s32 mid_supply = pmgEO_->mgCoM_.mdm_Ranking_.GetPKDt(mid_ranking, mdm_ranking_id_m_supply);
            //s32 iconno = (mid_supply % 10) % 4;
            TUGRcNormal* newrecord = MakeScoreRecord();
            
            s32 score = pmgEO_->mgCoU_.mdlog_Event_.GetLineDt(0, mdlog_event_itemnum);
			newrecord->ugname_.SetNameRankingItem(id_ranking);
            newrecord->SetValue(lib_str::IntToStr(score).c_str());
          }
            break;
		  case 8://8タイムアタック
		  {
			  TUGRcNormal* newrecord = MakeScoreRecord();
			  s32 score = pmgEO_->mgCoU_.mdlog_Event_.GetLineDt(0, mdlog_event_itemnum);
			  std::string strscore = "しあいじかん";
			  newrecord->ugname_.SetNameIcon(strscore.c_str(), ugi_RankBest);

			  // 99:59以上は　負負：負負　で表示
			  // フレーム数として、１００：００になるような値を初回アクセス時入れるようにするので
			  // 60 * 60 * 100以上のとき判断でよいかとおもいます
			  s64 INVALID_NUM = 60 * 60 * 100;
			  std::string st;
			  if (score >= INVALID_NUM)
			  {
				  st = "負負" + EtcWord(base::ewColon) + "負負" + EtcWord(base::ewColon) + "負負";
			  }
			  else
			  {
				  st = base::MinSecMSecString(score, FALSE);
			  }
			  newrecord->SetValue(st.c_str());
		  }
		  break;
          default:
            break;
          }
        }
      }
    }

    SetZeroPos();
  }
  void TUGLiRanker::DoUpdateSub(BOOL draw_f)
  {
    //uglistend_.Update(draw_f);
  }

  void TUGLiRanker::ChangeListEnd(BOOL listend_f)
  {
    //uglistend_.SetDraw(listend_f);
    for (s32 i = 0; i < records_.size(); ++i) records_[i]->SetDraw(!listend_f);
    for (s32 i = 0; i < records2_.size(); ++i) records2_[i]->SetDraw(!listend_f);
  }
  s64 TUGLiRanker::GetCommonID()
  {
    s64 res = NGNUM;
    if (stLiRanker_.myscore_f_)
    {
      if (GetActIndex() > records2_.size())
      {
        res = records_[GetActIndex() - records2_.size()]->GetCommonID();
      }
    }
    else
    {
      res = records_[GetActIndex()]->GetCommonID();
    }
    return res;
  }
}
