#include "dbUGLiSpGacha.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGLiSpGacha::TUGLiSpGacha(TUIGenLayer* player) :TUGLiGen(player), uglistend_(player)
  {
    stLiSpGacha_.Init();
  }

  TUGLiSpGacha::~TUGLiSpGacha()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiSpGacha::DoClearRecord()
  {
    stLiSpGacha_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  void TUGLiSpGacha::DoMakeGroupSub()
  {
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frameshop));
    uglistend_.MakeGroup_ListEnd_Shop(slider_, sliderx_, slidery_);
  }

  void TUGLiSpGacha::Refresh(BOOL todd_f)
  {
    ClearRecord();

    mid::midLog2("pmgEO_->mgCoM_.mdm_Gacha_.GetLineNum() %d",pmgEO_->mgCoM_.mdm_Gacha_.GetLineNum());
    mid::midLog2("todd_f %d",(s32)todd_f);

    mid::midLog2("pmgEO_->mgCoM_.mdm_Timer_.GetLineNum() %d",pmgEO_->mgCoM_.mdm_Timer_.GetLineNum());
    for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_Timer_.GetLineNum(); ++i)
    {
      mid::midLog2("mdm_timer_eventSt_date %d",(s32)pmgEO_->mgCoM_.mdm_Timer_.GetLineDt(i, mdm_timer_eventSt_date));
      mid::midLog2("mdm_timer_pk %d",(s32)pmgEO_->mgCoM_.mdm_Timer_.GetLineDt(i, mdm_timer_pk));
      mid::midLog2("mdm_timer_eventSt_date　pk %d",(s32)pmgEO_->mgCoM_.mdm_Timer_.GetPKDt(i, mdm_timer_eventSt_date));
    }


    for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_Gacha_.GetLineNum(); ++i)
    {
#ifndef __K_DEBUG_ROOT__
      //タイマー切れ
      mid::midLog2("mdm_gacha_id_m_timer %d",(s32)pmgEO_->mgCoM_.mdm_Gacha_.GetLineDt(i, mdm_gacha_id_m_timer));
      if (pmgEO_->mgCoM_.IsTimer(pmgEO_->mgCoM_.mdm_Gacha_.GetLineDt(i, mdm_gacha_id_m_timer)) == FALSE)
        {
          mid::midLog2("IsTimer NG %d",i);
           continue;
        }
#endif

      s64 mid_supply_ken = pmgEO_->mgCoM_.mdm_Gacha_.GetLineDt(i, mdm_gacha_id_m_supply_ken);
      s32 pricetype = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(mid_supply_ken, mdm_supply_priceType);
      mid::midLog2("pricetype %d",pricetype);
      BOOL type_f = FALSE;

      if (todd_f)
      {
        mid::midLog2("enPriceType_Todd %d",(s32)enPriceType_Todd);
        type_f = (pricetype == (s32)enPriceType_Todd);
      }
      else
      {
        s32 type = (s32)enPriceType_Todd;
        mid::midLog2("enPriceType_Star %d",(s32)enPriceType_Star);
        type_f = (pricetype == (s32)enPriceType_Star);
      }

      //このショップの種類
      if (type_f)
      {
        s64 mid_gacha = pmgEO_->mgCoM_.mdm_Gacha_.GetLineDt(i, mdm_gacha_pk);
        mid::midLog2("mid_gacha %d",mid_gacha);

        TUGRcSpSyouhin* newrecord = new TUGRcSpSyouhin(player_);
        s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
        newrecord->SetGachaDt(mid_gacha);
        records_.push_back(newrecord);
        GenMakeObj_Record(newrecord, rh);

        //チュートクリアしてないと一個買いできない
        if (pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_Gatya) == FALSE)
        {
          const s32 SPL_GACHA_TODD10 = 10005;//トッド10連
          if (pmgEO_->mgCoM_.mdm_Gacha_.GetLineDt(i, mdm_gacha_id_m_supply_ken) != SPL_GACHA_TODD10)
          {
#ifndef __K_DEBUG_ROOT__
            newrecord->SetDisable(TRUE);
#endif    
		  }
        }
      }

    }

  }
  void TUGLiSpGacha::DoUpdateSub(BOOL draw_f)
  {
    uglistend_.Update(draw_f);
  }

  void TUGLiSpGacha::ChangeListEnd(BOOL listend_f)
  {
    uglistend_.SetDraw(listend_f);
    for (s32 i = 0; i < records_.size(); ++i) records_[i]->SetDraw(!listend_f);
  }
  void TUGLiSpGacha::SetListEnd(s32 recordidx)
  {
    uglistend_.SetDataGacha(records_[recordidx]->GetCommonID());
  }
  s64 TUGLiSpGacha::GetCommonID()
  {
    return records_[GetActIndex()]->GetCommonID();
  }
}
