#include "dbUGRcProb.h"
namespace db
{
  TUGRcProb::TUGRcProb(TUIGenLayer* player) : TUGRcNormal(player)
  {
  }
  TUGRcProb::~TUGRcProb(void)
  {
  
  }
  void TUGRcProb::DoMakeGroupSub2()
  {
    SetShopRecord();
  }
  void TUGRcProb::DoSetLimY2(s32 limu, s32 hhh)
  {

  }
  void TUGRcProb::DoUpdateSub2(BOOL draw_f)
  {

  }

  void TUGRcProb::SetDt(s32 mid_GachaGrp, s32 prob)
  {
    //const std::string GRADE[] =
    //{
    //  "Ｄランク", 
    //  "Ｃランク", 
    //  "Ｂランク", 
    //  "Ａランク", 
    //  "Ｓランク", 
    //  "ＳＳランク", 
    //  "ＳＳＳランク",
    //};

    //コンテンツグループナンバー保存
    //s32 grade = pmgEO_->mgCoM_.mdm_GachaGrp_.GetPKDt(probvec[i2][PK], mdm_gachagrp_grade);
    s32 cntgrpno = pmgEO_->mgCoM_.mdm_GachaGrp_.GetPKDt(mid_GachaGrp, mdm_gachagrp_cntGrpNo);
    SetCommonID(cntgrpno);
    //SubName
    //SetSubName("ＩＤ");

    //Name
    ugname_.SetName(pmgEO_->mgCoM_.mdm_GachaGrp_.GetPKDtStr(mid_GachaGrp, mdm_gachagrp_name_c64).c_str());

    //SubValue
    //SetSubValue(datestr.c_str());

    //Value
    s32 probper = prob / 1000;
    s32 probcnm = prob % 1000;
    std::string strprob = lib_str::IntToStr(probper) + "．" + lib_str::IntToStr(probcnm, 3) + "％";

    SetValue(strprob.c_str());
  }

}
