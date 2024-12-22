#include "dbUGBtnVS.h"
#include "dbUILayer.h"
#include "dbMgUI.h"

namespace db
{
  //コンストラクタ
  TUGBtnVS::TUGBtnVS(TUIGenLayer* player) :TUGButton(player)
    , ugname_(player)
  {
    stBtnVS_.Init();
  }
  TUGBtnVS::~TUGBtnVS(void)
  {
  }
  void TUGBtnVS::MakeGroupVS(TUIObj* parent, base::TGrPart* pgrpart)
  {
    s32 partno = 1;

    field_->o_SetObjGrPart(pgrpart);
    field_->g_SetPartNo(partno);
    stBtn_.part_f_ = TRUE;
    stBtn_.partno_ = partno;
    stBtn_.slpartno_ = partno;
    GenMakeGroup(parent, TRUE);
    SetDepth(UIDP_05BUTTON);
    SetFont("つうしんたいせん", FC2_hmshiai_com, 0, -25);

    // スターアイコン
    ugname_.MakeGroup(field_);
    ugname_.SetPos(-55, 20);


    s32 restnum = 0;
    time_t logbodate = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_vsbonus_date);
    time_t logindate = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_login_date);
    struct tm *logbotm = localtime(&logbodate);
    s32 logboyear = logbotm->tm_year;
    s32 logboday = logbotm->tm_yday;
    struct tm *logintm = localtime(&logindate);
    s32 loginyear = logintm->tm_year;
    s32 loginday = logintm->tm_yday;

    s32 VSCAMPNUM = pmgEO_->mgCoM_.mdm_VSBonus_.GetLineDt(1, mdm_vsbonus_bonusNum);


    //ログイン日がVSボ日より１日以上後ろ
    if ((logboday < loginday) || (logboyear < loginyear))
    {
      restnum = VSCAMPNUM;
    }
    else
    {
      restnum = (VSCAMPNUM - pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_vsbonusNum));
    }

    if (restnum < 0) restnum = 0;
    if (restnum == 0)
    {
      ugname_.SetPos(-62, 20);
      ugname_.SetName(pmgEO_->mgCoM_.mdm_VSBonus_.GetLineDtStr(1, mdm_vsbonus_endmsg_c128), TRUE, FC2_hmshiai_com);
      ugname_.SetIcon(FALSE);
    }
    else
    {
      std::string text = pmgEO_->mgCoM_.mdm_VSBonus_.GetLineDtStr(1, mdm_vsbonus_msg_c128);
      text += "＿あと";
      text += lib_str::IntToStr(restnum);
      text += "かい";
      ugname_.SetName(text, TRUE, FC2_hmshiai_com);
      ugname_.ugicon_.SetIcon(
        pmgEO_->mgCoM_.mdm_VSBonus_.GetLineDt(1, mdm_vsbonus_itemType),
        pmgEO_->mgCoM_.mdm_VSBonus_.GetLineDt(1, mdm_vsbonus_itemID), FALSE);
      ugname_.ugicon_.SetPos(-9, 7);
      ugname_.SetIcon(TRUE);
    }


  }

  //更新
  void TUGBtnVS::DoUpdateSub(BOOL draw_f)
  {
    ugname_.Update(draw_f);
  }

}