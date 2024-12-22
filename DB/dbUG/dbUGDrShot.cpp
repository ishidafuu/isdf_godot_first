#include "dbUGDrShot.h"
#include "dbUGLiShFilter.h"

namespace db
{

  enum enDrumType
  {
    drt_Shot,
    drt_BaraLast,
    drt_BaraAll,
  };
  
  //コンストラクタ
  TUGDrShot::TUGDrShot(TUIGenLayer* player) : TUGDrGen(player),
  ura_(player),
  drumend_(player)
  {
    stDrShot_.Init();
    id_shotvec_.clear();
    //id_shotvec_ = pmgEO_->mgCoU_.GetShotList_All();
   
  }
  TUGDrShot::~TUGDrShot()
  {
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
  }
  //ドラム準備
  void TUGDrShot::DoReady()
  {
    //各レコードの情報書き換え
    RefreshRecords(TRUE);
    
    //停止位置カード読み込み
    DoStop();
  }
  BOOL TUGDrShot::DoSelectUra(BOOL ura_f)
  {
    if (ura_f)
    {
      lib_num::IncLoop(&stDrShot_.urano_, 0, 2);
    }
    else
    {
      stDrShot_.urano_ = 0;
    }
    ura_.SetUraNo(stDrShot_.urano_);

    return (stDrShot_.urano_ > 0);
  }

  //シュートカード
  void TUGDrShot::MakeGroup(TUIObj* parent, BOOL leftpos_f, TUGLiShFilter* uglistShFilter)
  {
    uglistShFilter_ = uglistShFilter;
    stDrGen_.USEOK_F_ = FALSE;
    stDrGen_.USEEYE_F_ = FALSE;
    
    stDrGen_.TOPFRONT_F_ = FALSE;
    stDrGen_.REVY_ = -40;
    stDrGen_.DISTY_ = 24;
    stDrGen_.LOOKPOS_ = 5;
    stDrGen_.SLIDEY_ = 68;
    stDrGen_.CARDH_ = 96;
    
    stDrGen_.EYEX_ = 38 - 24;
    stDrGen_.EYEY_ = (stDrGen_.REVY_ + (stDrGen_.DISTY_ * stDrGen_.LOOKPOS_));//継承先のデータが必要
    stDrGen_.OKX_ = 136 - 24;
    stDrGen_.OKY_ = (stDrGen_.REVY_ + (stDrGen_.DISTY_ * stDrGen_.LOOKPOS_));
    
    //サイズを決めてから継承元のMakeGroupを呼び出す
    GenMakeGroup(parent, leftpos_f);
    
    for (s32 i = 0; i < DRRECNUM; ++i)
    {
      TUGShot* newrecord = new TUGShot(player_);
      newrecord->MakeGroup(field_);//fieldで大丈夫か？
      records_.push_back(newrecord);
      GenAddRecord(newrecord);
    }
    
    ura_.MakeGroup(field_);
    GenSetUramen(&ura_);
    
    drumend_.MakeGroup(field_);
    GenSetDrumEnd(&drumend_);
  }
  
  void TUGDrShot::ReadyFilter()
  {
    id_shotvec_.clear();
    sozaidtvec_bara_.clear();
    id_shotvec_ = uglistShFilter_->id_shotvec_;
    stDrShot_.drtype_ = drt_Shot;
    Ready(id_shotvec_.size());
  }
  void TUGDrShot::ReadySort()
  {
    id_shotvec_.clear();
    sozaidtvec_bara_.clear();
    id_shotvec_ = uglistShFilter_->id_shotvec_;
    stDrShot_.drtype_ = drt_Shot;
    Ready(id_shotvec_.size());
  }

  void TUGDrShot::ReadyType_BaraLast(s64 id_shot_base)
  {
    id_shotvec_.clear();
    sozaidtvec_bara_.clear();
    id_shotvec_.push_back(id_shot_base);
    sozaidtvec_bara_.push_back(pmgEO_->mgCoU_.GetSozaiDataLast(id_shot_base));
    stDrShot_.drtype_ = drt_BaraLast;
    Ready(id_shotvec_.size() + sozaidtvec_bara_.size());
  }
  void TUGDrShot::ReadyType_BaraAll(s64 id_shot_base)
  {
    id_shotvec_.clear();
    sozaidtvec_bara_.clear();
    sozaidtvec_bara_ = pmgEO_->mgCoU_.GetSozaiDataAll(id_shot_base);
    stDrShot_.drtype_ = drt_BaraAll;
    Ready(sozaidtvec_bara_.size());
  }
  void TUGDrShot::ReadyType_Syn(s64 defshotid)//固有シュートレシピ
  {
    id_shotvec_.clear();
    sozaidtvec_bara_.clear();
    sozaidtvec_bara_ = pmgEO_->mgCoU_.GetSozaiDataSyn(defshotid); 
    stDrShot_.drtype_ = drt_BaraAll;
    Ready(sozaidtvec_bara_.size());
  }
  void TUGDrShot::RefreshSelected()
  {
    records_[GetLookRecord()]->SetShotDt(GetIDShot(), stDrShot_.id_team_, TRUE, pmgEO_->mgCoU_.GetSortShot(GetIDShot()));
  }

  BOOL TUGDrShot::IsNoneVec()
  {
    return ((id_shotvec_.size() == 0) && (sozaidtvec_bara_.size() == 0));
  }
  //各レコードの情報書き換え
  void TUGDrShot::RefreshRecords(BOOL first_f)
  {
    if (IsNoneVec()) return;

    switch (stDrShot_.drtype_)
    {
    case drt_Shot:
    {
      for (s32 i = 0; i < records_.size(); ++i)
      {
        //所持してるデータ以上
        if (i >= id_shotvec_.size()) break;

        s32 lineno = stDrGen_.drrecdtno_[i];
        s64 id_shot = id_shotvec_[lineno];
        records_[i]->SetShotDt(id_shot, stDrShot_.id_team_, first_f, pmgEO_->mgCoU_.GetSortShot(id_shot));
      }
    }
      break;
    case drt_BaraLast:
    {
      records_[0]->SetShotDtBaraLast(id_shotvec_[0]);
      records_[1]->SetShotDtSozai(sozaidtvec_bara_[0]);
    }
      break;
    case drt_BaraAll:
    {
      for (s32 i = 0; i < sozaidtvec_bara_.size(); ++i)
      {
        records_[i]->SetShotDtSozai(sozaidtvec_bara_[i]);
      }
    }
      break;
    }
  }
  
  //現在見てる位置のシュートＩＤ
  s64 TUGDrShot::GetIDShot()
  {
    if (IsNoneVec()) return 0;

    return id_shotvec_[GetDataNo()];//有効な中の番号
  }
  void TUGDrShot::SetIDTeam(s64 id_team)
  {
    stDrShot_.id_team_ = id_team;
  }
  void TUGDrShot::RevUra()
  {
    SelectUra(TRUE);
  }
  //停止位置カード読み込み
  void TUGDrShot::DoStop()
  {
    //現在LookPosにいるカードの細かい情報表示ＯＮ他ＯＦＦ
    for (s32 i = 0; i < records_.size(); ++i)
    {
      records_[i]->SetTipsDraw_f((stDrGen_.recno_ == i) || (stDrGen_.drrecdtno_[i] == (stDrGen_.dtnum_ - 1)));
    }
  }
  void TUGDrShot::DoRoll()
  {
    //各レコードの情報書き換え
    RefreshRecords(FALSE);
  }

  void TUGDrShot::DrumRoll_IDShot(s64 id_shot)
  {
    s32 index = 0;
    for (s32 i = 0; i < id_shotvec_.size(); ++i)
    {
      if (id_shotvec_[i] == id_shot)
      {
        index = i;
        break;
      }
    }

    DrumRoll_LookDt(index);

    SelectUra(FALSE);//表にする
    
    //各レコードの情報書き換え
    RefreshRecords(FALSE);
    
    //停止位置カード読み込み
    DoStop();
  }
  
  void TUGDrShot::DoUpdateSub(BOOL draw_f)//更新
  {
    //現在LookPosにいるカードの細かい情報表示ＯＮ他ＯＦＦ
    for (s32 i = 0; i < records_.size(); ++i)
    {
      //末尾は常にチェック
      if ((stDrGen_.drrecdtno_[i] == (stDrGen_.dtnum_ - 1)))
      {
        records_[i]->SetTipsDraw_f(TRUE);
      }
      //records_[i]->SetTipsDraw_f((stDrGen_.recno_ == i) || (stDrGen_.drrecdtno_[i] == (stDrGen_.dtnum_ - 1)));
    }
  }
  void TUGDrShot::DoActionSub()//更新
  {
  }

}
