#include "dbUGLiSpHuku.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGLiSpHuku::TUGLiSpHuku(TUIGenLayer* player) :TUGLiGen(player), uglistend_(player)
  {
    stLiSpHuku_.Init();
  }

  TUGLiSpHuku::~TUGLiSpHuku()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiSpHuku::DoClearRecord()
  {
    stLiSpHuku_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  void TUGLiSpHuku::DoMakeGroupSub()
  {
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frameshop));
    uglistend_.MakeGroup_ListEnd_Shop(slider_, sliderx_, slidery_);
  }

  bool CheckPriority_huku(const INTVECTOR& left, const INTVECTOR& right)
  {
    return (left[1] > right[1]);//大きい方が先頭
  }

  void TUGLiSpHuku::Refresh()
  {
    ClearRecord();

    INTVECTOR2 data;
    data.clear();

    // 先にソートする
    for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_Huku_.GetLineNum(); ++i)
    {
#ifndef __K_DEBUG_ROOT__
      //タイマー切れ
      if (pmgEO_->mgCoM_.IsTimer(pmgEO_->mgCoM_.mdm_Huku_.GetLineDt(i, mdm_huku_id_m_timer)) == FALSE) continue;
#endif

      INTVECTOR v;
      v.clear();
      s64 mid_Huku = pmgEO_->mgCoM_.mdm_Huku_.GetLineDt(i, mdm_huku_pk);
      s64 priority = pmgEO_->mgCoM_.mdm_Huku_.GetLineDt(i, mdm_huku_priority);
      v.push_back(mid_Huku);
      v.push_back(priority);
      data.push_back(v);
    }
    std::sort(data.begin(), data.end(), CheckPriority_huku);

    for (s32 i = 0; i < data.size(); ++i)
    {
      // ソートされた順にデータを入れる
      s64 mid_Huku = data[i][0];
      TUGRcHuku* newrecord = new TUGRcHuku(player_);
      s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
      newrecord->SetHukuDt(mid_Huku);
      records_.push_back(newrecord);
      GenMakeObj_Record(newrecord, rh);
    }

  }
  void TUGLiSpHuku::DoUpdateSub(BOOL draw_f)
  {
    uglistend_.Update(draw_f);
  }

  void TUGLiSpHuku::ChangeListEnd(BOOL listend_f)
  {
    uglistend_.SetDraw(listend_f);
    for (s32 i = 0; i < records_.size(); ++i) records_[i]->SetDraw(!listend_f);
  }
  void TUGLiSpHuku::SetListEnd(s32 recordidx)
  {
    uglistend_.SetDataHuku(records_[recordidx]->GetCommonID());
  }
  s64 TUGLiSpHuku::GetCommonID()
  {
    return records_[GetActIndex()]->GetCommonID();
  }
}
