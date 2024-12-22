#include "dbUGLiProb.h"
#include "dbUILayer.h"
#include "dbUGRcProb.h"


namespace db
{
  //コンストラクタ
  TUGLiProb::TUGLiProb(TUIGenLayer* player) :TUGLiGen(player), uglistend_(player)
  {
    stLiProb_.Init();
  }

  TUGLiProb::~TUGLiProb()
  {
    //解放時必ず呼ぶ
    ClearRecord();
  }

  //リストクリア（変数部分的に初期化、レコードとレコードエンド解放）
  void TUGLiProb::DoClearRecord()
  {
    stLiProb_.Init();
    for (s32 i = 0; i < records_.size(); ++i) Freemem(records_[i]);
    records_.clear();
  }

  void TUGLiProb::DoMakeGroupSub()
  {
    field_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_frameshop));//ショップの時は変える
    uglistend_.MakeGroup_ListEnd_Shop(slider_, sliderx_, slidery_);
    uglistend_.SetDraw(FALSE);
  }

  //マスターガチャIDを入れてレコードをすべて作成
  void TUGLiProb::Refresh(s64 mid_gacha)
  {
    ClearRecord();
    stLiProb_.groupNo_ = pmgEO_->mgCoM_.mdm_Gacha_.GetPKDt(mid_gacha, mdm_gacha_lotGrpNoA);
    LONGVECTOR2 probvec;//[0]grade[1]prob

    const s32 PK = 0;//グレード
    const s32 PRB = 1;//確率
    //const s32 CGN = 2;//グループ番号
    //const s32 LINE = 3;//閉め

    s64 proball = 0;
    s32 lastprob = 0;
    for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_GachaGrp_.GetLineNum(); ++i)
    {
      //このガチャのグループ
      if (pmgEO_->mgCoM_.mdm_GachaGrp_.GetLineDt(i, mdm_gachagrp_lotGrpNo) == stLiProb_.groupNo_)
      {

        //タイマー切れ
        if (pmgEO_->mgCoM_.IsTimer(pmgEO_->mgCoM_.mdm_GachaGrp_.GetLineDt(i, mdm_gachagrp_id_m_timer)) == FALSE) continue;

        s32 mid_gachagrp = pmgEO_->mgCoM_.mdm_GachaGrp_.GetLineDt(i, mdm_gachagrp_pk);
        //s32 grade = pmgEO_->mgCoM_.mdm_GachaGrp_.GetLineDt(i, mdm_gachagrp_grade);
        s32 prob = pmgEO_->mgCoM_.mdm_GachaGrp_.GetLineDt(i, mdm_gachagrp_prob);
        //s32 cntGrpNo = pmgEO_->mgCoM_.mdm_GachaGrp_.GetLineDt(i, mdm_gachagrp_cntGrpNo);
        BOOL samegrade_f = FALSE;

        //確率０は表示しない
        if (prob == 0) continue;
        //
        //for (s32 i2 = 0; i2 < probvec.size(); ++i2)
        //{
        //  if ((probvec[i2][GRD] == grade) && (probvec[i2][CLS] == 0))//同じグレード
        //  {
        //    probvec[i2][PRB] += prob;//確率
        //    if (lastprob != i2)
        //    {
        //      probvec[lastprob][CLS] = 1;//一つ前の確率グループを閉じる
        //      lastprob = i2;
        //    }
        //    
        //    proball += prob;//確率分母
        //    samegrade_f = TRUE;
        //    break;
        //  }
        //}

        //if (samegrade_f == FALSE)//見つからなかった
        {
          LONGVECTOR newvec;
          //newvec.push_back(grade);
          newvec.push_back(mid_gachagrp);
          newvec.push_back(prob);
          //newvec.push_back(cntGrpNo);
          //newvec.push_back(i);
          proball += prob;//確率分母
          probvec.push_back(newvec);//新しいグレードと確率を追加

          
        }
      }
    }

    //s32 no = 0;
    //グレードごとのレコード（高ランクから入るので逆順）
    for (s32 i = 0; i < probvec.size(); ++i)
    {
      s32 cntGrpNo = pmgEO_->mgCoM_.mdm_GachaCnt_.GetLineDt(i, mdm_gachacnt_cntGrpNo);

      s32 i2 = probvec.size() - 1 - i;
      TUGRcProb* newrecord = new TUGRcProb(player_);
      s32 rh = newrecord->MakeGroup(slider_, 0, stList_.bottomy_, sliderx_, slidery_);
   
      s64 probper = (probvec[i2][PRB] * 100000);
      probper /= proball;

      newrecord->SetDt(probvec[i2][PK], probper);
      //s32 grade = RARITYNUM - i - 1;
      s32 grade = pmgEO_->mgCoM_.mdm_GachaGrp_.GetPKDt(probvec[i2][PK], mdm_gachagrp_grade);
      newrecord->ugname_.ugicon_.SetIconCup(grade);

      records_.push_back(newrecord);
      GenMakeObj_Record(newrecord, rh);
      //++no;
    }

  }


  void TUGLiProb::DoUpdateSub(BOOL draw_f)
  {
    uglistend_.Update(draw_f);
  }

  void TUGLiProb::DoActionSub()
  {
  }
  s32 TUGLiProb::GetCntGrpNo(s32 recordidx)
  {
    return records_[GetActIndex()]->GetCommonID();
  }
}
