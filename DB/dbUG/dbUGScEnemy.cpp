#include "dbUGScEnemy.h"


namespace db
{
  const s32 DROPNUM = 4;

  //コンストラクタ
  TUGScEnemy::TUGScEnemy(TUIGenLayer* player) : TUGGen(player)
    , ugradio_drop_(player)
    , ugname_(player)
	, ugBossSerifu_(player)
  {
    stScEnemy_.Init();
    stScEnemy_.posx_ = 0;
    stScEnemy_.posy_ = -(GRIDSIZE * 3);

    for (s32 i2 = 0; i2 < DROPNUM; ++i2) ugbtn_drop_.push_back(new TUGBtnLabel(player));
  }

  TUGScEnemy::~TUGScEnemy()
  {
    for (s32 i2 = 0; i2 < ugbtn_drop_.size(); ++i2) Freemem(ugbtn_drop_[i2]);
  }
  void TUGScEnemy::MakeGroup(TUIObj* parent, s32 enemyNo)
  {
    stScEnemy_.enemyNo_ = enemyNo;

    SetParent(parent);

    const s32 BASEX = 18;
    const s32 BASEY = -64;
    const s32 DISTY = 40;

	field_->z_SetPos(BASEX, BASEY + (enemyNo * DISTY));

    enemy_ = MakeObj();
    ugname_.MakeGroup(field_);

    //敵
    enemy_->o_SetParent(field_);
    enemy_->z_SetPos(0, -4);
    enemy_->g_SetMirror(TRUE);

    ugradio_drop_.MakeGroup(field_, 32 + 12, 12 + 12 );//ラジオボタン
    for (s32 i2 = 0; i2 < DROPNUM; ++i2)
    {
      //ドロップアイコンフレーム
      ugbtn_drop_[i2]->MakeGroup_Radio(field_, ug_btn_icon, ug_btn_icon);//ドロップアイコンの下地用意する
      //ugbtn_drop_[i2]->SetParent(field_);
      ugradio_drop_.AddButton(ugbtn_drop_[i2], 24);
    }

	// とりあえず最大5行のメッセージウィンドウ
	ugBossSerifu_.MakeGroup(field_, 5, enBossSerifuFrame_BigWindow, UIDP_03OBJ);
	ugBossSerifu_.SetPos(0,0);
	ugBossSerifu_.SetDraw(FALSE);
  }

  void TUGScEnemy::Ready(s64 mid_maptip, s32 lno_maptip)
  {
	  // ボスの時以外使わないので消しておく
	  ugBossSerifu_.SetDraw(FALSE);

    const s32 BOSSAREA = 2;
    s32 area = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(mid_maptip, mdm_maptip_area);// +4;

    //１チームか３チームかによって表示や位置を変える
    enmdm_Maptip enteam = mdm_maptip_id_m_teamA;
    enmdMaptip dropbit = mdmaptip_dropbitA;
    enmdMaptip comp = mdmaptip_compA_f;
    switch (stScEnemy_.enemyNo_)
    {
    case 0:
      enteam = mdm_maptip_id_m_teamA;
      dropbit = mdmaptip_dropbitA;
      comp = mdmaptip_compA_f;
      break;
    case 1:
      enteam = mdm_maptip_id_m_teamB;
      dropbit = mdmaptip_dropbitB;
      comp = mdmaptip_compB_f;
      break;
    case 2:
      enteam = mdm_maptip_id_m_teamC; 
      dropbit = mdmaptip_dropbitC;
      comp = mdmaptip_compC_f;
      break;
    }
    

    //mdmaptip_dropbitA

    if(area == BOSSAREA)
	{
		if(stScEnemy_.enemyNo_ > 0)
		{
		  SetDraw(FALSE);
		  return;
		}

		// ボス専用メッセージウィンドウ
		std::string s = "ＢＯＳＳ＿ＭＥＳＳＡＧＥ";
		ugBossSerifu_.SetTitle(s);
		STRVECTOR textvec;
		textvec.push_back(pmgEO_->mgCoM_.mdm_Maptip_.GetPKDtStr(mid_maptip, mdm_maptip_text0_c128));
		textvec.push_back(pmgEO_->mgCoM_.mdm_Maptip_.GetPKDtStr(mid_maptip, mdm_maptip_text1_c128));
		textvec.push_back(pmgEO_->mgCoM_.mdm_Maptip_.GetPKDtStr(mid_maptip, mdm_maptip_text2_c128));
		textvec.push_back(pmgEO_->mgCoM_.mdm_Maptip_.GetPKDtStr(mid_maptip, mdm_maptip_text3_c128));
		//ugBossSerifu_.SetSerifu(textvec);
		ugBossSerifu_.SetSerifuFullOpen(textvec);	// ほんとは喋ってほしいがアニメーションが無いので一気に表示
		ugBossSerifu_.SetPos(-12, 50);
		ugBossSerifu_.SetDraw(TRUE);
	}
    SetDraw(TRUE);

    //キャラＩＤとチームＩＤ
    s64 mid_team = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(mid_maptip, enteam);
    s64 mid_char = pmgEO_->mgCoM_.GetCharID(mid_team, 0);
    s32 tileno = pmgEO_->mgGrp_.AddZakoSt(mid_char, mid_team);//ここでzako画像作成のキューをおくる
    enemy_->o_SetObjGrpTile2(pmgEO_->mgGrp_.pgr_Zako_, tileno);

    s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetLineDt(mid_team,mdm_team_rarity);
    //チーム名
    ugname_.SetNameTeam_Cnt(mid_team);
    ugname_.SetIcon(FALSE);//アイコンは表示しない
    ugname_.SetPos(26, -2);

    for (s32 i2 = 0; i2 < DROPNUM; ++i2)
    {
      //pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(mid_maptip, enteam);

      s64 itemtype = 0;
      s64 itemid = 0;
      s64 itemnum = 0;
      s64 openbit = pmgEO_->mgCoU_.mdMaptip_.GetLineDt(lno_maptip, dropbit);
      BOOL open_f = (((s64)(1 << i2) & openbit) != 0);

      //スペシャルドロップ
      if (i2 == 3)
      {
        enmdm_Maptip enitemtype = mdm_maptip_itemType_sdA;
        enmdm_Maptip enitemid = mdm_maptip_itemID_sdA;
        enmdm_Maptip enitemnum = mdm_maptip_itemNum_sdA;
        switch (stScEnemy_.enemyNo_)
        {
        case 0:
          enitemtype = mdm_maptip_itemType_sdA;
          enitemid = mdm_maptip_itemID_sdA;
          enitemnum = mdm_maptip_itemNum_sdA;
          break;
        case 1:
          enitemtype = mdm_maptip_itemType_sdB;
          enitemid = mdm_maptip_itemID_sdB;
          enitemnum = mdm_maptip_itemNum_sdB;
          break;
        case 2:
          enitemtype = mdm_maptip_itemType_sdC;
          enitemid = mdm_maptip_itemID_sdC;
          enitemnum = mdm_maptip_itemNum_sdC;
          break;
        }
        itemtype = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(mid_maptip, enitemtype);
        itemid = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(mid_maptip, enitemid);
        itemnum = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(mid_maptip, enitemnum);


      }
      else
      {
        enmdm_Drop enitemtype = mdm_drop_itemType_0;
        enmdm_Drop enitemid = mdm_drop_itemID_0;
        enmdm_Drop enitemnum = mdm_drop_itemNum_0;
        switch (i2)
        {
        case 0: 
          enitemtype = mdm_drop_itemType_0;
          enitemid = mdm_drop_itemID_0;
          enitemnum = mdm_drop_itemNum_0;
          break;
        case 1:
          enitemtype = mdm_drop_itemType_1;
          enitemid = mdm_drop_itemID_1;
          enitemnum = mdm_drop_itemNum_1;
          break;
        case 2:
          enitemtype = mdm_drop_itemType_2;
          enitemid = mdm_drop_itemID_2;
          enitemnum = mdm_drop_itemNum_2;
          break;
        }

        enmdm_Maptip endrop = mdm_maptip_id_m_drop_A;
        switch (stScEnemy_.enemyNo_)
        {
        case 0:endrop = mdm_maptip_id_m_drop_A; break;
        case 1:endrop = mdm_maptip_id_m_drop_B; break;
        case 2:endrop = mdm_maptip_id_m_drop_C; break;
        }

        s64 mid_drop = pmgEO_->mgCoM_.mdm_Maptip_.GetPKDt(mid_maptip, endrop);
        itemtype = pmgEO_->mgCoM_.mdm_Drop_.GetPKDt(mid_drop, enitemtype);
        itemid = pmgEO_->mgCoM_.mdm_Drop_.GetPKDt(mid_drop, enitemid);
        itemnum = pmgEO_->mgCoM_.mdm_Drop_.GetPKDt(mid_drop, enitemnum);
      }

      //ドロップアイコン
      if (open_f)
      {
        ugbtn_drop_[i2]->ugricon_.SetIcon(itemtype, itemid, FALSE);
        ugbtn_drop_[i2]->uglabel_.ugname_.SetName(itemtype, itemid, itemnum);
      }
      else
      {
        ugbtn_drop_[i2]->ugricon_.SetIcon(ugi_GiftBox);
        ugbtn_drop_[i2]->uglabel_.ugname_.SetName("");
      }
      ugbtn_drop_[i2]->uglabel_.ugname_.ugicon_.SetDraw(FALSE);
      
    }

  }

  void TUGScEnemy::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    enemy_->Update(draw_f);
    ugname_.Update(draw_f);
    ugradio_drop_.Update(draw_f);
	if(ugBossSerifu_.IsDraw())
	{
		ugBossSerifu_.Update(draw_f);
	}
  }

}
