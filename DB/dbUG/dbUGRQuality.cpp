#include "dbUGRQuality.h"
#include "dbUILayer.h"

namespace db
{
  const s32 KETA = 4;
  const s32 NBX = 18;
  const s32 NBY = 10;
  //コンストラクタ
  TUGRQuality::TUGRQuality(TUIGenLayer* player) : TUGGen(player)//アクションなし
  {
    stRQuality_.Init();
  }

  TUGRQuality::~TUGRQuality()
  {

  }
  void TUGRQuality::MakeGroup(TUIObj* parent, BOOL spin_f, BOOL bar_f)
  {
    SetParent(parent);
    stRQuality_.spin_f_ = spin_f;
    stRQuality_.bar_f_ = bar_f;
    //rare_->z_SetPos(QualityX_TEAM, QualityY);
    if (bar_f)
    {
      field_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_quality_bar));
    }
    else
    {
      field_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_quality_icon));
    }
    field_->g_SetDraw(TRUE);
    field_->f_FontPos(9, +4);
    //field_->f_MakeFont("てすと",FC2_mini);

  }


  void TUGRQuality::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
  }
  //void TUGRQuality::SetLimY(s32 limu, s32 hhh)
  //{
  //  field_->g_SetLimY(limu, hhh);
  //  sign_->g_SetLimY(limu, hhh);
  //  for (s32 i = 0; i < numvec_.size(); ++i)
  //  {
  //    numvec_[i]->g_SetLimY(limu, hhh);
  //  }
  //}

  void TUGRQuality::Refresh(s32 quality)
  {
    if (quality == NGNUM)
    {
      if (stRQuality_.spin_f_)
      {
        field_->g_SetDraw(FALSE);
        return;
      }
      else
      {
        quality = 10;//11番目に？
        field_->g_SetGrpPos(2,5);
      }
    }
    else
    {
      field_->g_SetGrpPos(0, 0);
    }

    if (stRQuality_.bar_f_)
    {
      field_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_quality_bar));
    }
    else
    {
      field_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_quality_icon));
    }

    s32 pno = (stRQuality_.spin_f_)
      ? quality + 11
      : quality;
    field_->g_SetPartNo(pno);
    field_->g_SetDraw(TRUE);

   
  }
  void TUGRQuality::Refresh(s32 quality, s32 mid_sozai, s32 mid_rarity)
  {
    Refresh(quality);

    const s32 INDVRANGE = 10;

    s32 qtype = (stRQuality_.spin_f_)
      ? pmgEO_->mgDt_.dtShElm_.GetDt(mid_sozai, sz_para_type_spn)
      : pmgEO_->mgDt_.dtShElm_.GetDt(mid_sozai, sz_para_type_pow);
    //s32 powtype = pmgEO_->mgDt_.dtShElm_.GetDt(mid_sozai, sz_para_type_pow);
    s32 rev = pmgEO_->mgDt_.dtShType_.GetDtRev(qtype, mid_rarity);
    s32 range = pmgEO_->mgDt_.dtShType_.GetDtRange(qtype, mid_rarity);
    s32 revnum = (rev + ((range * quality) / INDVRANGE));

    field_->f_MakeFont_Mini(lib_str::IntToSignStr(revnum).c_str(), (FC2_rareD + mid_rarity));

  }

}
