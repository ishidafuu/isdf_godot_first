#include "dbUGLiSpTechno.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGLiSpTechno::TUGLiSpTechno(TUIGenLayer* player) :TUGLiGen(player), uglistend_(player)
  {
    stLiSpTechno_.Init();
  }

  TUGLiSpTechno::~TUGLiSpTechno()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiSpTechno::DoClearRecord()
  {
    stLiSpTechno_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
    for (s32 i = 0; i < records2_.size(); ++i) Freemem(records2_[i]);
    records2_.clear();

  }

  void TUGLiSpTechno::DoMakeGroupSub()
  {
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frameshop));
    uglistend_.MakeGroup_ListEnd_Shop(slider_, sliderx_, slidery_);
  }

  void TUGLiSpTechno::Refresh()//後ほどフラグで分ける
  {
    const s32 TECHNO_STARSHOP = 0;
    const s32 MOBACO_STARSHOP = 1;

    s32 shoptype = (mid::midIsADR())
      ? MOBACO_STARSHOP
      : TECHNO_STARSHOP;

    ClearRecord();

    for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineNum(); ++i)
    {
      if (pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineDt(i, mdm_spsyouhin_shopType) == shoptype)
      {
        s64 mid_spsyouhin = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetLineDt(i, mdm_spsyouhin_pk);
        TUGRcSpTechno* newrecord = new TUGRcSpTechno(player_);
        s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
        newrecord->SetTechnoDt(mid_spsyouhin);
        records_.push_back(newrecord);
        GenMakeObj_Record(newrecord, rh);
      }
    }

    {
      TUGRcNormal* newrecord = new TUGRcNormal(player_);
      s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
      records2_.push_back(newrecord);
      GenMakeObj_Record(newrecord, rh);
      newrecord->ugname_.SetNameIcon("しきん＿けっさいほう", ugi_Info); 
    }
  }
  void TUGLiSpTechno::DoUpdateSub(BOOL draw_f)
  {
    uglistend_.Update(draw_f);
  }

  void TUGLiSpTechno::ChangeListEnd(BOOL listend_f)
  {
    uglistend_.SetDraw(listend_f);
    //for (s32 i = 0; i < records_.size(); ++i) records_[i]->SetDraw(!listend_f);
    //レコードの表示非表示とスライダーの有効無効
    GenSetListEnd(listend_f);
  }
  void TUGLiSpTechno::SetListEnd(s32 recordidx)
  {
    //レコードから商品MIDを取得
    uglistend_.SetDataSpTechno(GetCommonID());
  }
  s64 TUGLiSpTechno::GetCommonID()
  {
    if (GetActIndex() < records_.size())
    {
      return records_[GetActIndex()]->GetCommonID();
    }
    else
    {
      return NGNUM;
    }
  }

}
