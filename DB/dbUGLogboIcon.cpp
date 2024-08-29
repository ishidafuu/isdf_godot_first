#include "dbUGLogboIcon.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGLogboIcon::TUGLogboIcon(TUIGenLayer* player) : TUGGen(player)//アクションなし
  {
    stLogboIcon_.Init();

  }

  TUGLogboIcon::~TUGLogboIcon()
  {

  }
  void TUGLogboIcon::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);

    star_ = MakeObj();
    star_->o_SetParent(field_);
    star_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_item));
    star_->g_SetPartNo(12);
    star_->z_SetPos(0, 5);
    star_->g_SetUnder();
    star_->g_SetDraw(FALSE);//デフォはアイコン無し
    star_->g_SetDepth(UIDP_02BGOBJHI);

    sp_ = MakeObj();
    sp_->o_SetParent(field_);
    sp_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_rarity));
    sp_->g_SetPartNo(2);
    sp_->z_SetPos(-20, 0);
    sp_->g_SetUnder();
    sp_->g_SetDraw(FALSE);//デフォはアイコン無し

  }


  //アイコン表示リフレッシュ
  void TUGLogboIcon::Refresh()
  {
    stLogboIcon_.Init();
    time_t logbodate = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_logbo_date);
    time_t logindate = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_login_date);
    struct tm *logbotm = localtime(&logbodate);
    s32 logboyear = logbotm->tm_year;
    s32 logboday = logbotm->tm_yday;

    struct tm *logintm = localtime(&logindate);
    s32 loginyear = logintm->tm_year;
    s32 loginday = logintm->tm_yday;

    //ログイン日がログボ日より１日以上後ろ
    if ((logboday < loginday) || (logboyear < loginyear))
    {
      star_->g_SetDraw(TRUE);
      stLogboIcon_.logbo_f_ = TRUE;
      const s32 SPLOGBO = 10;
      s64 loginnum = pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_loginNum);
	  if (loginnum > 1 && (loginnum % SPLOGBO) == 0)
      {
        sp_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_rarity));
        sp_->g_SetPartNo(9);
        sp_->g_SetDraw(TRUE);
        stLogboIcon_.logbosp_f_ = TRUE;
      }
	  // チーム
	  else if ((loginnum % SPLOGBO) == 5)
      {
        sp_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_icon_rarity));
        sp_->g_SetPartNo(2);
        sp_->g_SetDraw(TRUE);
        stLogboIcon_.logbosp_f_ = TRUE;
      }
    }
    else
    {
      star_->g_SetDraw(FALSE);
      sp_->g_SetDraw(FALSE);
    }

	/*
      if (mid::midIsDXL())
      {
      star_->g_SetDraw(TRUE);
      stLogboIcon_.logbo_f_ = TRUE;
      sp_->g_SetDraw(TRUE);
      stLogboIcon_.logbosp_f_ = TRUE;

      }
	  */
  }
  BOOL TUGLogboIcon::IsLogobo()
  {
    return stLogboIcon_.logbo_f_;
  }
  BOOL TUGLogboIcon::IsLogoboSp()
  {
    return stLogboIcon_.logbosp_f_;
  }

  void TUGLogboIcon::DoUpdate(BOOL draw_f)
  {
    star_->Update(draw_f);
    sp_->Update(draw_f);
  }

}
