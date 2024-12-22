#include "dbMgCommon.h"
#include "dbGame.h"

namespace db
{
  //コンストラクタ
  TMgCommon::TMgCommon(TGame* pGame)
  : TGameObjGen(pGame)
  {
  }
  
  TMgCommon::~TMgCommon()
  {
  }
  
  void TMgCommon::MakeCharNameTex()
  {
    //単語テクスチャ作成
    pmgEO_->mgFont_.InitWords(base::fd_None, base::FC2_shiai);

    //ステータス名作成
    for (s32 i2 = 0; i2 < DBSIDE; ++i2)
    {
      s32 mid_team = pGame_->mgSG_.stTm_[i2].PosSt.mid_team_;

      //選手
      for (s32 i = 0; i < DBMEMBER_ALL; ++i)
      {
        s32 charid = pmgEO_->mgCoM_.GetCharID(mid_team, i);
        std::string charName = pmgEO_->mgDt_.dtChar_.GetName(charid);
        s32 charwdt = (DBMEMBER_ALL * i2) + i;
        pmgEO_->mgFont_.SetWords(charwdt, charName.c_str());
      }

    }
    pmgEO_->mgFont_.MakeWordTexture();
  }
  //ポップアップ
  void TMgCommon::PopupMsg(enPopUp putype, const char* msg, s32 sideNo, s32 posX, s32 posY, s32 posZ, enFontCol2 col, BOOL dam_f)
  {
	  PopupMsgDelay(putype, msg, sideNo, posX, posY, posZ, col, 0, dam_f);
  }
  //ポップアップ
  void TMgCommon::PopupMsgDelay(enPopUp putype, const char* msg, s32 sideNo, s32 posX, s32 posY, s32 posZ, enFontCol2 col, s32 delay, BOOL dam_f)
  {
    //デモの時はポップアップださない
    if (pmgSG_->stRf_.Demo_f) return;
    
    //現状パワーも表示しない
    if (putype == pu_Pow) return;
    
    //ぽっぷあっぷ
    BOOL popup_f = FALSE;
    
    popup_f = (putype == pu_Act)
    || (putype == pu_Pow)
    || (putype == pu_Damage)
    || (putype == pu_Call);
    
    BOOL cutin_f = (putype == pu_Cutin);

    //ポップアップ
    if (popup_f && (cutin_f == FALSE))
    {
      
      s32 baseline = (pmgGO_->pmgRf_->IsPrev())
        ? DRAWBASELINE_PREV
        : DRAWBASELINE;

      s32 posx = posX / XYMAG;
      s32 posy = baseline
      - ((posY + posZ) / XYMAG)
      - DAMPOSY;

      pmgEO_->mgFont_.OrderDamFontSprite(msg, posx, posy, (base::enFontCol2)col, FALSE, delay);
    }
    else if ((sideNo != NGNUM) || cutin_f)//スライドイン
    {
      s32 posx = (sideNo == SIDE0)
        ? (GRIDSIZE * 6) - 1 - mid::midGetDotL_Hf()
        : -(GRIDSIZE * 6) + 1 + mid::midGetDotL_Hf();

      const s32 SLINPOSY = -64;

      pmgEO_->mgFont_.OrderSlideFont(msg, posx, SLINPOSY, (base::enFontCol2)col, sideNo, delay);

    }
  }
  void TMgCommon::PopupMsg(s32 msg, s32 posX, s32 posY, s32 posZ, enFontCol2 col)
  {
    //デモの時はポップアップださない
    if (pmgSG_->stRf_.Demo_f) return;
    
    s32 fx = posX / XYMAG;
    s32 baseline = (pmgGO_->pmgRf_->IsPrev())
      ? DRAWBASELINE_PREV
      : DRAWBASELINE;

    s32 fy = baseline
    - ((posY + posZ) / XYMAG)
    - DAMPOSY;

    pmgEO_->mgFont_.OrderDamFontSprite(msg, fx, fy, (base::enFontCol2)col);
    //pmgEO_->mgFont_.OrderDamFontSprite(msg, fx, fy, (base::enFontCol2)col);
  }
  //ポップアップ
  void TMgCommon::ResetPopupMsg(void)
  {
	  pmgEO_->mgFont_.ResetAllFontSprite();
  }
  
  //小物オブジェを天使とかにする
  void TMgCommon::SetEObj(enEtcObjMotion tMType,TZahyouRec tZahyou, s32 tSideNo, s32 tCharNo)
  {
    for (s32 i = 0; i < DBETCOBJ; ++i)
    {
      if (pmgGO_->pmgEtc_[i]->SetNewMType(tMType, tZahyou, tSideNo, tCharNo))
      {
        break;
      }
    }
  }
  
  //COM思考確率タイプ
  BOOL TMgCommon::GetRandPer(s32 perptn)
  {
    return (perptn > GetRand(100));
  }
  //COMAct思考
  s32 TMgCommon::GetComActPtn(s32 comType, enDtComAct cmaDt, BOOL ressei_f, s32 sideNo, s32 id)
  {
    s32 ptndt[COMPTN];
    for (s32 i = 0; i < COMPTN; ++i)
    {
      ptndt[i] = pmgEO_->mgDt_.dtComAct_[comType].GetDt(cmaDt, i, ressei_f);
    }
    
    
    s32 base = 0xFF;//初期分母255
    //const s32 LV1 = 1;
    //const s32 LV2 = 2;
    //
    //
    ////pmgEO_->mgDt_.dtTeam_.GetDt(dbtm_Ope0,);cmaAction sh01pa23
    //switch (cmaDt)
    //{
    //    case cmaAction://ope0
    //        switch (pmgEO_->mgDt_.dtTeam_.GetSideDt(dbtm_OpePa, sideNo))
    //    {
    //        case 4: base += PtnDtRev(ptndt, LV2, 2, 3); break;//パス→
    //        case 3: base += PtnDtRev(ptndt, LV1, 2, 3); break;
    //        case 1: base += PtnDtRev(ptndt, LV1, 0, 1); break;
    //        case 0: base += PtnDtRev(ptndt, LV2, 0, 1); break;//←シュート
    //    }
    //        break;
    //    case cmaCatchJdg://ope1//comCatchJdg dg023 ca1
    //        switch (pmgEO_->mgDt_.dtTeam_.GetSideDt(dbtm_OpeCa, sideNo))
    //    {
    //        case 4: base += PtnDtRev(ptndt, LV2, 0, 1); break;//キャッチ→
    //        case 3: base += PtnDtRev(ptndt, LV1, 0, 1); break;
    //        case 1: base += PtnDtRev(ptndt, LV1, 2, 3); break;
    //        case 0: base += PtnDtRev(ptndt, LV2, 2, 3); break;//←しない
    //    }
    //        break;
    //}
    
    
    ////個人補正★
    //for (s32 i = 0; i < COMPTN; ++i)
    //{
    //  s32 rev = pmgEO_->mgDt_.dtChar_.GetActRev(id, i);
    //  
    //  //そのまま引いて分母も引く
    //  if (ptndt[i] >= rev)
    //  {
    //    ptndt[i] -= rev;
    //    base -= rev;
    //  }
    //  else
    //  {
    //    base -= (rev - ptndt[i]);//差分
    //    ptndt[i] = 0;
    //  }
    //}
    
    
    //0~254
    s32 sai255 = GetRand(base);
    
    if (sai255 < ptndt[0])
    {
      return 0;
    }
    else if (sai255 < (ptndt[0] + ptndt[1]))
    {
      return 1;
    }
    else if (sai255 < (ptndt[0] + ptndt[1] + ptndt[2]))
    {
      return 2;
    }
    else
    {
      return 3;
    }
  }
  BOOL TMgCommon::IsPer100(s32 border)
  {
    //確率
    const s32 PER100 = 100;
    return (mid::midGetRandGm(PER100) < border);
  }
  
  s32 TMgCommon::GetRand(s32 len)
  {
    if (len == 0)
    {
      return 0;
    }
    else
    {
      return mid::midGetRandGm(len);
    }
  }
  
  s32 TMgCommon::PtnDtRev(s32 ptndt[], s32 lv, s32 adNo)
  {
    s32 rev = pmgEO_->mgDt_.dtSet_.GetDtCOM(setOpeRev) * lv;
    ptndt[adNo] += (rev * 3);
    return (rev * 3);
  }
  s32 TMgCommon::PtnDtRev(s32 ptndt[], s32 lv, s32 adNo, s32 adNo2)
  {
    s32 rev = pmgEO_->mgDt_.dtSet_.GetDtCOM(setOpeRev) * lv;
    ptndt[adNo] += (rev * 2);
    ptndt[adNo2] += (rev * 2);
    return (rev * 4);
  }
  s32 TMgCommon::PtnDtRev(s32 ptndt[], s32 lv, s32 adNo, s32 adNo2, s32 adNo3)
  {
    s32 rev = pmgEO_->mgDt_.dtSet_.GetDtCOM(setOpeRev) * lv;
    ptndt[adNo] += rev;
    ptndt[adNo2] += rev;
    ptndt[adNo3] += rev;
    return (rev * 3);
  }
  
  //COM思考
  s32 TMgCommon::GetComPtn(s32 comType, enCom comDt, s32 sideNo)
  {
    s32 ptndt[COMPTN];
    for (s32 i = 0; i < COMPTN; ++i)
    {
      ptndt[i] = pmgEO_->mgDt_.dtCom_[comType].GetDt(comDt, i);
    }
    

    //セミオートここで補正掛けられる
    //作戦補正
    s32 base = 0xFF;//初期分母255
    //const s32 LV1 = 1;
    //const s32 LV2 = 2;
    //
    //switch (comDt)
    //{
    //    case comDMCallCH://ope4//comDMCallNC no2 go013
    //    case comDMCallNC://ope4//comDMCallNC no2 go013
    //        switch (pmgEO_->mgDt_.dtTeam_.GetDt2(dbtm_OpeDmCall, sideNo))
    //    {
    //        case 4: base += PtnDtRev(ptndt, LV2, 0, 1, 3); break;//ダッシュマン→
    //        case 3: base += PtnDtRev(ptndt, LV1, 0, 1, 3); break;
    //        case 1: base += PtnDtRev(ptndt, LV1, 2); break;
    //        case 0: base += PtnDtRev(ptndt, LV2, 2); break;//←よばない
    //    }
    //        break;
    //    case comDMNum://ope5//comDMNum all1 sgl2
    //        switch (pmgEO_->mgDt_.dtTeam_.GetSideDt(dbtm_OpeDmNum, sideNo))
    //    {
    //        case 4: base += PtnDtRev(ptndt, LV2, 1); break;//全員呼ぶ→
    //        case 3: base += PtnDtRev(ptndt, LV1, 1); break;
    //        case 1: base += PtnDtRev(ptndt, LV1, 2); break;
    //        case 0: base += PtnDtRev(ptndt, LV2, 2); break;//←シングル
    //    }
    //        break;
    //    case comDMPaNum://ope6//comDMPaNum no2 pa013
    //        switch (pmgEO_->mgDt_.dtTeam_.GetSideDt(dbtm_OpeDmPa, sideNo))
    //    {
    //        case 4: base += PtnDtRev(ptndt, LV2, 0, 1, 3); break;//ダッシュマンパス→
    //        case 3: base += PtnDtRev(ptndt, LV1, 0, 1, 3); break;
    //        case 1: base += PtnDtRev(ptndt, LV1, 2); break;
    //        case 0: base += PtnDtRev(ptndt, LV2, 2); break;//←しない
    //    }
    //        break;
    //}
    
    
    
    //0~254
    s32 sai255 = GetRand(base);//ゲーム用乱数
    
    if (sai255 < ptndt[0])
    {
      return 0;
    }
    else if (sai255 < (ptndt[0] + ptndt[1]))
    {
      return 1;
    }
    else if (sai255 < (ptndt[0] + ptndt[1] + ptndt[2]))
    {
      return 2;
    }
    else
    {
      return 3;
    }
  }
  
  
}
