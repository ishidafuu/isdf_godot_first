#include "dbUGScPanel.h"
#include "dbUILayer.h"

namespace db
{

  const s32 YOKOKU = 1;
  const s32 OPEN = 2;

  const s32 DROPNUM = 4;
  //エリア 1なし 2ボス 3雑魚 4バス 5空港 6─ 7│ 8┘ 9┐ 10┌ 11└
  const s32 BOSSAREA = 2;
  const s32 ROOTAREA = 6;
  const s32 PORTAREA = 3;
  const s32 PORTAREA2 = 4;

  const s32 SELECTPOSX = 80;
  const s32 SELECTPOSY = -16;

  const s32 BASEX = -160;
  const s32 BASEY = -80;

  const s32 PLBASEX = 190;
  const s32 PLBASEY = 80;
  const s32 PANELDISTX = 104;
  const s32 PANELDISTY = 72;
  const s32 PANELDISTX_Hf = PANELDISTX / 2;
  const s32 PANELDISTY_Hf = PANELDISTY / 2;

  const s32 PLX = -24;
  const s32 CAPX = -48;
  const s32 PLY = 0;
  const s32 CAPY = 0;
  const s32 RTREVY = -3;
  const s32 EATTIME = 90;
  const s32 MVSPD = 12;
  const s32 MVSPD2 = 24;

  //コンストラクタ
  TUGScPanel::TUGScPanel(TUIGenLayer* player) :
    TUGGen(player)//アクションなし
    , hp2_(NULL)
    , textChangeTimer_(0)
    , IsChange_(false)
  {
    stScPanel_.Init();

    panel_ = NULL;
    bossbg_ = NULL;
    noise_ = NULL;
    enemy_ = NULL;
    for (s32 i = 0; i < ROOTNUM; ++i) route_[i] = NULL;
    state_ = NULL;
    hp_ = NULL;
    time_ = NULL;
  }

  TUGScPanel::~TUGScPanel()
  {
  }
  void TUGScPanel::MakeGroup(TUIObj* parent, s32 mapno, s64 lno_m_maptip, PANELVECTOR* panelvec)
  {
    panelvec_ = panelvec;
    stScPanel_.mapNo_ = mapno;
    stScPanel_.lno_m_maptip_ = lno_m_maptip;
    stScPanel_.mid_maptip_ = GetMDt(mdm_maptip_pk);
    stScPanel_.lno_maptip_ = NGNUM;
    for (s32 i = 0; i < pmgEO_->mgCoU_.mdMaptip_.GetLineNum(); ++i)
    {
      if (pmgEO_->mgCoU_.mdMaptip_.GetLineDt(i, mdmaptip_id_m_maptip) == stScPanel_.mid_maptip_)
      {
        stScPanel_.lno_maptip_ = i;//ユーザーデータの方の行番号
        break;
      }
    }

    //stScPanel_.id_maptip_ = lno_m_maptip;
    //stScPanel_.lno_maptip_ = GetDt(mdm_maptip_pk);


    stScPanel_.posX_ = GetMDt(mdm_maptip_posX);//グリッド位置
    stScPanel_.posY_ = GetMDt(mdm_maptip_posY);

    //パネルベース
    //field_ = MakeObj();
    field_->o_SetParent(parent);
    s32 posx = PLBASEX + (stScPanel_.posX_  * PANELDISTX);
    s32 posy = PLBASEY + (stScPanel_.posY_ * PANELDISTY);
    field_->z_SetPos_SafeL(posx, posy);

    s32 area = GetMDt(mdm_maptip_area);// +4;

    //パネル
    panel_ = MakeObj();
    panel_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_map_panel));
    panel_->o_SetParent(field_);
    panel_->g_SetPartNo(area);
    panel_->g_SetCenter(TRUE);
    panel_->g_SetDraw(TRUE);

    //フレーム
    frame_ = MakeObj();
    frame_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_map_panel));
    frame_->o_SetParent(panel_);
    frame_->g_SetPartNo(0);
    frame_->g_SetCenter(TRUE);
    frame_->g_SetDraw(FALSE);



    stScPanel_.roadpanel_f_ = (area >= ROOTAREA);//道パネル
    stScPanel_.bosspanel_f_ = (area == BOSSAREA);//ボスパネル
    stScPanel_.portpanel_f_ = ((area == PORTAREA) || (area == PORTAREA2));//バス停空港パネル
    //道のみパネル
    if (stScPanel_.roadpanel_f_)
    {
      //プライオリティを上げる
      //panel_->g_SetDepth(UIDP_02BGOBJHI);
      panel_->g_SetRevDepthLv(2);
    }
    else
    {

    }

    //ボス背景
    if (stScPanel_.bosspanel_f_)
    {
      std::string name = GetMDtStr(mdm_maptip_name_c64);
      panel_->f_MakeFont(name.c_str(), FC2_normal);
      panel_->f_FontPos(0, 20);
      panel_->f_SetCenter(TRUE);
      s32 courtno = GetMDt(mdm_maptip_id_m_haikei);
      std::string thumbname = "m_" + pmgEO_->mgDt_.dtCourt_.GetStrLine(courtno, enDtCourtStr_filename);
      base::TGrp* bossbg = player_->LoadGrp_NoColKey("map", thumbname.c_str());

      bossbg_ = MakeObj();
      bossbg_->o_SetObjGrp(bossbg);
      bossbg_->o_SetParent(panel_);
      bossbg_->z_SetPos(0, -9);
      bossbg_->g_SetCenter(TRUE);
      bossbg_->g_SetDraw(TRUE);

      //砂嵐
      noise_ = MakeObj();
      noise_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_map_noise));
      noise_->o_SetParent(bossbg_);
      noise_->g_SetCenter(TRUE);
      //noise_->g_SetDraw(FALSE);//とりあえずけしとくマスター参照する
      noise_->a_AddAnivec(0, 2, 4, at_Loop);//砂嵐
      noise_->g_SetDraw(TRUE);
      noise_->a_SetAniNo(0, TRUE);//砂嵐
    }

    //四方向ルート（子の立場から見る）
    for (s32 i2 = 0; i2 < ROOTNUM; ++i2)
    {
      s32 rtype = 0;
      s32 rpno = 0;
      BOOL chboss_f = FALSE;//子がボスか
      BOOL prboss_f = FALSE;//親がボスか
      s32 rx = 0;
      s32 ry = 0;

      switch (i2)
      {
      case 0:
        rtype = GetMDt(mdm_maptip_roadUp);
        rpno = 0;
        ry = -1;
        break;
      case 1:
        rtype = GetMDt(mdm_maptip_roadDown);
        rpno = 2;
        ry = +1;
        break;
      case 2:
        rtype = GetMDt(mdm_maptip_roadLeft);
        rpno = 1;
        rx = -1;
        break;
      case 3:
        rtype = GetMDt(mdm_maptip_roadRight);
        rpno = 3;
        rx = +1;
        break;
      }

      //  道

      s32 posx = GetMDt(mdm_maptip_posX) + rx;
      s32 posy = GetMDt(mdm_maptip_posY) + ry;
      s64 pospk = pmgEO_->mgCoM_.GetMapTipPK(mapno, posx, posy);

      switch (rtype)
      {
      case 2://  親	2
        stScPanel_.route_f_[i2] = TRUE;
        prboss_f = (pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pospk, mdm_maptip_area) == BOSSAREA);
        break;
      case 3://  長子	3
      case 4:
      {
        stScPanel_.route_f_[i2] = FALSE;
        chboss_f = (pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(pospk, mdm_maptip_area) == BOSSAREA);
      }
      break;
      }

      const s32 LONGROAD = 4;

      //自分と次のマスの大きさでルート変える
      if (rpno == 0)
      {
        if (prboss_f == FALSE)
        {
          rpno += LONGROAD;
        }
      }
      else if (rpno == 2)
      {
        if (stScPanel_.bosspanel_f_ == FALSE)
        {
          rpno += LONGROAD;
        }
      }


      //道ありのみ
      if (stScPanel_.route_f_[i2])
      {
        stScPanel_.routeno_[i2] = rpno;
        route_[i2] = MakeObj();
        route_[i2]->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_map_route));
        route_[i2]->o_SetParent(field_);
        route_[i2]->g_SetCenter(TRUE);
        route_[i2]->g_SetPartNo(rpno);
        route_[i2]->g_SetRevDepthLv(1);

        switch (i2)
        {
        case 0: route_[i2]->z_SetPos(0, -PANELDISTY_Hf + RTREVY); break;
        case 1: route_[i2]->z_SetPos(0, +PANELDISTY_Hf + RTREVY); break;
        case 2: route_[i2]->z_SetPos(-PANELDISTX_Hf, +RTREVY); break;
        case 3: route_[i2]->z_SetPos(+PANELDISTX_Hf, +RTREVY); break;
        }
      }
    }

    //道
    if (stScPanel_.roadpanel_f_)
    {

    }
    else//敵
    {
      state_ = MakeObj();
      state_->o_SetParent(panel_);
      state_->f_MakeFont_Mini("ＮＥＷ", FC2_rareB);
      state_->z_SetPos(18, -32);
      state_->g_SetDraw(FALSE);
      newbrink_f_ = TRUE;
      comp_f_ = FALSE;

      time_ = MakeObj();
      time_->o_SetParent(panel_);
      std::string str = "ＴＩＭＥ＿";
      s64 sec = GetMDt(mdm_maptip_shiaiSec);
      s64 min = sec / 60;
      sec = sec % 60;
      if (min < 10)
      {
        // 10分以下は表示
        str += lib_str::IntToStr(min);
        str += "：";
        if (sec < 10)
        {
          str += lib_str::IntToStr(0);
        }
        str += lib_str::IntToStr(sec);
      }
      else
      {
        // 10分以上は？表示
        str += "？：？？";
      }
      //hpstr += lib_str::IntToStr();
      time_->f_MakeFont_Mini(str.c_str(), FC2_mini);
      time_->z_SetPos(-36, 4);

      hp_ = MakeObj();
      hp_->o_SetParent(panel_);
      std::string hpstr = "ＨＰ負";
      hpstr += lib_str::IntToStr(GetMDt(mdm_maptip_staminaMin));
      hp_->f_MakeFont_Mini(hpstr.c_str(), FC2_mini);
      hp_->z_SetPos(18, 4);

      hp2_ = MakeObj();
      hp2_->o_SetParent(panel_);
      hp2_->f_MakeFont_Mini(hpstr.c_str(), FC2_rareB);
      hp2_->z_SetPos(18, 4);
      hp2_->g_SetDraw(FALSE);
      textChangeTimer_ = 60;
      IsChange_ = false;

      enemy_ = MakeObj();
      enemy_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_map_body));
      enemy_->o_SetParent(panel_);//ボス背景に載せる
      enemy_->g_SetRevDepthLv(3);
      enemy_->g_SetCenter(TRUE);
      enemy_->g_SetUnder();
      if (stScPanel_.bosspanel_f_)
      {
        //フレーム
        frame_ = MakeObj();
        frame_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_map_panel));
        frame_->o_SetParent(panel_);
        frame_->g_SetPartNo(0);
        frame_->g_SetCenter(TRUE);
        frame_->g_SetDraw(FALSE);
      }
    }
    stScPanel_.areastate_ = as_None;
    stScPanel_.roadstate_ = as_None;
    ChangeState();
  }

  void TUGScPanel::SetPanelButton(BOOL btn_f)
  {

    if ((stScPanel_.roadpanel_f_ == FALSE)
      && (stScPanel_.areastate_ == as_Open))
    {
      panel_->b_SetBtn(btn_f);
    }
    else
    {
      panel_->b_SetBtn(FALSE);
    }

  }
  void TUGScPanel::SetPanelButtonModal(BOOL modal_f)
  {
    panel_->b_SetModal(modal_f);
  }

  void TUGScPanel::ChangeState()
  {
    switch (stScPanel_.areastate_)
    {
    case as_Open:OpenArea(); break;
    case as_Yokoku:YokokuArea(); break;
    case as_None:NoneArea(); break;
    }
    switch (stScPanel_.roadstate_)
    {
    case as_Open:OpenRoad(); break;
    case as_Yokoku:YokokuRoad(); break;
    case as_None:NoneRoad(); break;
    }
  }

  void TUGScPanel::OpenArea()
  {
    field_->g_SetDraw(TRUE);

    //エリア 1なし 2ボス 3雑魚 4バス 5空港 6─ 7│ 8┘ 9┐ 10┌ 11└
    s32 area = GetMDt(mdm_maptip_area);

    //道のみパネル
    if (stScPanel_.roadpanel_f_)
    {
      const s32 ROADREV = 6;
      const s32 PARENT = 2;//1× 2親 3長 4子
      switch (area)
      {
      case 6: if (GetMDt(mdm_maptip_roadRight) == PARENT) area += ROADREV; break;
      case 7: if (GetMDt(mdm_maptip_roadDown) == PARENT) area += ROADREV; break;
      case 8: if (GetMDt(mdm_maptip_roadUp) == PARENT) area += ROADREV; break;
      case 9: if (GetMDt(mdm_maptip_roadDown) == PARENT) area += ROADREV; break;
      case 10: if (GetMDt(mdm_maptip_roadRight) == PARENT) area += ROADREV; break;
      case 11: if (GetMDt(mdm_maptip_roadUp) == PARENT) area += ROADREV; break;
      }
    }
    else
    {
      time_->g_SetDraw(TRUE);
      panel_->b_SetBtn(TRUE);
      //ボスパネル
      if (stScPanel_.bosspanel_f_)
      {
        noise_->g_SetDraw(FALSE);//砂嵐消す

        s64 mid_team = GetMDt(mdm_maptip_id_m_teamA);
        s64 mid_char = pmgEO_->mgCoM_.GetCharID(mid_team, 0);//キャプテン
        s32 tileno = pmgEO_->mgGrp_.AddBossSt(mid_char, mid_team);//ここでボス画像作成のキューをおくる
        enemy_->o_SetObjGrpTile2(pmgEO_->mgGrp_.pgr_Boss_, tileno);
        enemy_->g_SetMirror(TRUE);
      }
    }

    panel_->g_SetPartNo(area);


  }
  void TUGScPanel::YokokuArea()
  {
    field_->g_SetDraw(TRUE);
    //エリア 1なし 2ボス 3雑魚 4バス 5空港 6─ 7│ 8┘ 9┐ 10┌ 11└
    s32 area = GetMDt(mdm_maptip_area);

    //道のみパネル
    if (stScPanel_.roadpanel_f_)
    {
      const s32 ROADYOKOKU = 12;
      area += ROADYOKOKU;
    }
    else
    {
      //ボスパネル
      if (stScPanel_.bosspanel_f_)
      {

      }
      else
      {
        area = 1;
      }

      newbrink_f_ = FALSE;
      hp_->g_SetDraw(FALSE);
      time_->g_SetDraw(FALSE);
    }

    panel_->g_SetPartNo(area);
  }
  void TUGScPanel::NoneArea()
  {
    field_->g_SetDraw(FALSE);
  }

  void TUGScPanel::OpenRoad()
  {
    //四方向ルート
    for (s32 i2 = 0; i2 < ROOTNUM; ++i2)
    {
      //道ありのみ
      if (stScPanel_.route_f_[i2])
      {
        route_[i2]->g_SetDraw(TRUE);
        route_[i2]->g_SetPartNo(stScPanel_.routeno_[i2]);
        //route_[i2]->g_SetRevDepthLv(1);
      }
    }
  }
  void TUGScPanel::YokokuRoad()
  {
    //四方向ルート
    for (s32 i2 = 0; i2 < ROOTNUM; ++i2)
    {
      const s32 ROADYOKOKU = 8;
      //道ありのみ
      if (stScPanel_.route_f_[i2])
      {
        route_[i2]->g_SetDraw(TRUE);
        route_[i2]->g_SetPartNo(stScPanel_.routeno_[i2] + ROADYOKOKU);
        //route_[i2]->g_SetRevDepthLv(1);
      }
    }
  }
  void TUGScPanel::NoneRoad()
  {
    //四方向ルート
    for (s32 i2 = 0; i2 < ROOTNUM; ++i2)
    {
      //道ありのみ
      if (stScPanel_.route_f_[i2]) route_[i2]->g_SetDraw(FALSE);
    }
  }

  void TUGScPanel::SetClear()
  {
    stScPanel_.clear_f_ = TRUE;

    stScPanel_.areastate_ = as_Open;
    stScPanel_.roadstate_ = as_Open;

    comp_f_ = (GetDtBOOL(mdmaptip_compA_f));
    if (stScPanel_.bosspanel_f_ == FALSE)
    {
      comp_f_ &= (GetDtBOOL(mdmaptip_compB_f) && GetDtBOOL(mdmaptip_compC_f));
    }

    if (comp_f_)
    {
      state_->f_MakeFont_Mini("ＣＯＭＰ", FC2_rareB);
      state_->g_SetDraw(TRUE);
    }
    //NEWけす
    newbrink_f_ = FALSE;
    ChangeState();

  }
  BOOL TUGScPanel::IsClear()
  {
    return (stScPanel_.clear_f_
      || ((stScPanel_.areastate_ || stScPanel_.roadstate_) && stScPanel_.roadpanel_f_));//ロードパネルなら開いたらクリア
  }
  enAreaState TUGScPanel::GetAreaState()
  {
    return stScPanel_.areastate_;
  }

  BOOL TUGScPanel::IsClearParent(s32 posx, s32 posy)
  {
    BOOL res = FALSE;
    for (s32 i = 0; i < panelvec_->size(); ++i)
    {
      TUGScPanel* panel = panelvec_->at(i);
      //enmdm_Maptip::mdm_maptip_posX
      if ((panel->GetMDt(mdm_maptip_posX) == posx)
        && (panel->GetMDt(mdm_maptip_posY) == posy))
      {
        res = panel->IsClear();
        break;
      }
    }
    return res;
  }
  BOOL TUGScPanel::IsClearParent(s64 posNo)
  {
    if (posNo == 0) return TRUE;

    BOOL res = FALSE;
    for (s32 i = 0; i < panelvec_->size(); ++i)
    {
      TUGScPanel* panel = panelvec_->at(i);
      if (panel->GetMDt(mdm_maptip_posNo) == posNo)
      {
        res = panel->IsClear();
        break;
      }
    }
    return res;
  }

  BOOL TUGScPanel::CheckParent()
  {
    BOOL allclear_f = TRUE;
    const s32 PARENT = 2;//1× 2親 3長 4子

    s32 posx = GetMDt(mdm_maptip_posX);
    s32 posy = GetMDt(mdm_maptip_posY);

    //４方向の親と遠親がクリアしてるか確認
    if (GetMDt(mdm_maptip_roadUp) == PARENT) allclear_f &= IsClearParent(posx, posy - 1);
    if (GetMDt(mdm_maptip_roadDown) == PARENT) allclear_f &= IsClearParent(posx, posy + 1);
    if (GetMDt(mdm_maptip_roadLeft) == PARENT)    allclear_f &= IsClearParent(posx - 1, posy);
    if (GetMDt(mdm_maptip_roadRight) == PARENT) allclear_f &= IsClearParent(posx + 1, posy);
    allclear_f &= IsClearParent(GetMDt(mdm_maptip_fp0));
    allclear_f &= IsClearParent(GetMDt(mdm_maptip_fp1));
    allclear_f &= IsClearParent(GetMDt(mdm_maptip_fp2));
    allclear_f &= IsClearParent(GetMDt(mdm_maptip_fp3));
    allclear_f &= IsClearParent(GetMDt(mdm_maptip_fp4));
    allclear_f &= IsClearParent(GetMDt(mdm_maptip_fp5));
    allclear_f &= IsClearParent(GetMDt(mdm_maptip_fp6));
    allclear_f &= IsClearParent(GetMDt(mdm_maptip_fp7));

    //開通
    if (allclear_f)
    {
      stScPanel_.areastate_ = as_Open;
      stScPanel_.roadstate_ = as_Open;
    }
    else
    {
      stScPanel_.areastate_ = (GetMDt(mdm_maptip_ykkarea) > 0) ? as_Yokoku : as_None;
      stScPanel_.roadstate_ = (GetMDt(mdm_maptip_ykkroad) > 0) ? as_Yokoku : as_None;
    }

    //状態ごとの表示変更
    ChangeState();

    return allclear_f;
  }


  s64 TUGScPanel::GetMIDMapTip()
  {
    return stScPanel_.mid_maptip_;
  }
  s64 TUGScPanel::GetMDt(enmdm_Maptip dt)
  {
    return pmgEO_->mgCoM_.mdm_Maptip_.GetLineDt(stScPanel_.lno_m_maptip_, dt);
  }
  std::string TUGScPanel::GetMDtStr(enmdm_MaptipStr dt)
  {
    return pmgEO_->mgCoM_.mdm_Maptip_.GetLineDtStr(stScPanel_.lno_m_maptip_, dt);
  }
  s64 TUGScPanel::GetDt(enmdMaptip dt)
  {
    if (stScPanel_.lno_maptip_ == NGNUM) return 0;

    return pmgEO_->mgCoU_.mdMaptip_.GetLineDt(stScPanel_.lno_maptip_, dt);
  }
  BOOL TUGScPanel::GetDtBOOL(enmdMaptip dt)
  {
    return (GetDt(dt) != 0);
  }
  s32 TUGScPanel::GetLNoMapTip()
  {
    return stScPanel_.lno_maptip_;
  }

  void TUGScPanel::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    if (frame_ != NULL) frame_->Update(draw_f);
    if (panel_ != NULL) panel_->Update(draw_f);
    if (bossbg_ != NULL) bossbg_->Update(draw_f);
    if (noise_ != NULL) noise_->Update(draw_f);
    if (enemy_ != NULL) enemy_->Update(draw_f);
    if (state_ != NULL)
    {
      state_->Update(draw_f);

      if (draw_f == FALSE)
      {
        // NEW点滅
        if (newbrink_f_)
        {
          textChangeTimer_--;
          if (textChangeTimer_ < 0)
          {
            textChangeTimer_ = 60;
            IsChange_ = !IsChange_;
            state_->g_SetDraw(IsChange_);
          }
        }
      }
    }
    if (time_ != NULL) { time_->Update(draw_f); }
    if ((hp_ != NULL) && (hp2_ != NULL))
    {
      hp_->Update(draw_f);
      hp2_->Update(draw_f);

      if (draw_f == FALSE)
      {
        s32 hp = 0;
        s32 needHP = GetMDt(mdm_maptip_staminaMin);
#ifdef __K_DEBUG_USER_DATA__
        hp = kdebug::DebugUDManager::GetInstance()->GetHP();
#endif
        // スタミナが足りてる時は通常カラー
        if (hp >= needHP)
        {
          hp_->g_SetDraw(TRUE);
          hp2_->g_SetDraw(FALSE);
        }
        // 足りない時は色を変える
        else
        {
          // 色変えるだけバージョン
          hp_->g_SetDraw(FALSE);
          hp2_->g_SetDraw(TRUE);
        }
      }

    }
    //    if (hp_ != NULL) hp_->Update(draw_f);
    for (s32 i = 0; i < ROOTNUM; ++i)
    {
      if (route_[i] != NULL) route_[i]->Update(draw_f);
    }

    if (draw_f == FALSE)
    {
      BOOL btn_f = FALSE;

      //道は飛ばす
      if (panel_->b_IsDecide())
      {
        //SetAct(0);
        SetAct(0, stScPanel_.mid_maptip_);
        panel_->g_Flash();
      }
      else if (panel_->b_IsEnter(TRUE))
      {
        panel_->g_Bright();
      }
      else
      {
        panel_->g_EfReset();
      }
    }

  }

}
