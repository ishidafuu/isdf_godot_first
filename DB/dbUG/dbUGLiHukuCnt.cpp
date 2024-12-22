#include "dbUGLiHukuCnt.h"
#include "dbUILayer.h"
#include "dbUGRcNormal.h"


namespace db
{
  //コンストラクタ
  TUGLiHukuCnt::TUGLiHukuCnt(TUIGenLayer* player) :TUGLiGen(player)
    //, uglistend_(player)
  {
    stLiHukuCnt_.Init();
  }

  TUGLiHukuCnt::~TUGLiHukuCnt()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiHukuCnt::DoClearRecord()
  {
    stLiHukuCnt_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  void TUGLiHukuCnt::DoMakeGroupSub()
  {
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frameshop));//ショップの時は変える
  }

  //マスターガチャIDを入れてレコードをすべて作成
  void TUGLiHukuCnt::Refresh(s64 mid_hukugrp)
  {
    ClearRecord();

    for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_HukuCnt_.GetLineNum(); ++i)
    {
      if (pmgEO_->mgCoM_.mdm_HukuCnt_.GetLineDt(i, mdm_hukucnt_id_w_hukugrp) == mid_hukugrp)
      {
        s64 mid_Hukucnt = pmgEO_->mgCoM_.mdm_HukuCnt_.GetLineDt(i, mdm_hukucnt_pk);
        s32 grade = pmgEO_->mgCoM_.mdm_HukuCnt_.GetLineDt(i, mdm_hukucnt_grade);
        TUGRcHukuCnt* newrecord = new TUGRcHukuCnt(player_);
        s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
        newrecord->SetHukuCntDt(mid_Hukucnt);//mid_HukucntをCommonに保存
        records_.push_back(newrecord);
        GenMakeObj_Record(newrecord, rh);
      }
    }
    
  }

  s64 TUGLiHukuCnt::GetCommonID()
  {
    return records_[GetActIndex()]->GetCommonID();
  }


  void TUGLiHukuCnt::DoUpdateSub(BOOL draw_f)
  {
    //uglistend_.Update(draw_f);
  }

  void TUGLiHukuCnt::DoActionSub()
  {
  }

}
