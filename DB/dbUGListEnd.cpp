#include "dbUGListEnd.h"
namespace db
{


  //コンストラクタ
  TUGListEnd::TUGListEnd(TUIGenLayer* player) :TUGRc2Gen(player)
  {
  }

  TUGListEnd::~TUGListEnd()
  {

  }

  s32 TUGListEnd::MakeGroup_ListEnd(TUIObj* parent, s32 limx, s32 limy)
  {
    MakeGroup(parent, 0, 8, limx, limy);
    basegrp_ = pmgUI_->GetUGGrp(ug_list_panel);
    basegrp2_ = pmgUI_->GetUGGrp(ug_list_panel);
    field_->o_SetObjGrp(basegrp_);

    SetDraw(FALSE);//リストエンドはけしとく
    return 0;
  }
  s32 TUGListEnd::MakeGroup_ListEnd_Shop(TUIObj* parent, s32 limx, s32 limy)
  {
    MakeGroup(parent, 0, 8, limx, limy);
    basegrp_ = pmgUI_->GetUGGrp(ug_list_panelshop);
    basegrp2_ = pmgUI_->GetUGGrp(ug_list_panelshop);
    field_->o_SetObjGrp(basegrp_);
    SetDraw(FALSE);//リストエンドはけしとく
    return 0;
  }

  //消耗品ＭＩＤ
  void TUGListEnd::GenSetDataSupply(s64 mid_supply)
  {
    UndrawAll();
    ugname_.SetNameSupply(mid_supply, 1);
    ugname_.SetDraw(TRUE);
    s32 value = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(mid_supply, mdm_supply_price);
    std::string valuestr = lib_str::IntToStr(value);
    SetValue(valuestr.c_str());

    s32 dtvalue = pmgEO_->mgCoU_.GetSupplyNum(mid_supply);
    std::string dtvaluestr = lib_str::IntToStr(dtvalue);

    STRVECTOR textvec;
    textvec.push_back(pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(mid_supply, mdm_supply_text0_c64));
    textvec.push_back(pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(mid_supply, mdm_supply_text1_c64));
    textvec.push_back(pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(mid_supply, mdm_supply_text2_c64));

    GenSetData(0, "もっているかず", dtvaluestr.c_str());
    GenSetText(textvec);
  }
  //消耗品ＭＩＤ
  void TUGListEnd::GenSetDataShotInv(s64 mid_supply)
  {
    UndrawAll();
    ugname_.SetNameSupply(mid_supply, 1);
    ugname_.SetDraw(TRUE);
    s32 value = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(mid_supply, mdm_supply_price);
    std::string valuestr = lib_str::IntToStr(value);
    SetValue(valuestr.c_str());

    s32 dtvalue = pmgEO_->mgCoU_.mdShot_.GetLineNum();
    std::string dtvaluestr = lib_str::IntToStr(dtvalue);

    GenSetData(0, "シュートわく", dtvaluestr.c_str());
    STRVECTOR textvec;
    textvec.push_back(pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(mid_supply, mdm_supply_text0_c64));
    textvec.push_back(pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(mid_supply, mdm_supply_text1_c64));
    textvec.push_back(pmgEO_->mgCoM_.mdm_Supply_.GetPKDtStr(mid_supply, mdm_supply_text2_c64));
    GenSetText(textvec);
  }

  //アイテムグループＭＩＤ
  void TUGListEnd::GenSetDataItemPack(s64 mid_ItemPack)
  {
    UndrawAll();
    ugname_.SetNameItemPack(mid_ItemPack, 1);
    ugname_.SetDraw(TRUE);
    s32 value = pmgEO_->mgCoM_.mdm_ItemPack_.GetPKDt(mid_ItemPack, mdm_itempack_price);
    std::string valuestr = lib_str::IntToStr(value);
    SetValue(valuestr.c_str());
    STRVECTOR textvec;
    textvec.push_back(pmgEO_->mgCoM_.mdm_ItemPack_.GetPKDtStr(mid_ItemPack, mdm_itempack_text0_c64));
    textvec.push_back(pmgEO_->mgCoM_.mdm_ItemPack_.GetPKDtStr(mid_ItemPack, mdm_itempack_text1_c64));
    textvec.push_back(pmgEO_->mgCoM_.mdm_ItemPack_.GetPKDtStr(mid_ItemPack, mdm_itempack_text2_c64));
    GenSetText(textvec);
  }
  //商品ＭＩＤ
  void TUGListEnd::SetDataSpSyouhin(s64 mid_spsyouhin)
  {
    UndrawAll();
    s32 itemtype = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_spsyouhin, mdm_spsyouhin_itemType);
    s32 itemid = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_spsyouhin, mdm_spsyouhin_itemID);
    s32 itemnum = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_spsyouhin, mdm_spsyouhin_itemNum);
    switch (itemtype)
    {
    case enItemType_Supply:
      if ((itemid / 100) == (SPL_SHOTINV / 100))
      {
        GenSetDataShotInv(itemid);
      }
      else
      {
        GenSetDataSupply(itemid);
      }
      break;
    case enItemType_Pack: GenSetDataItemPack(itemid); break;
    }

    GenSetButton("かう");
  }
  //所持スター
  void TUGListEnd::SetDataSpTechno(s64 mid_spsyouhin)
  {
    UndrawAll();
    s32 itemtype = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_spsyouhin, mdm_spsyouhin_itemType);
    s32 itemid = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_spsyouhin, mdm_spsyouhin_itemID);
    s32 itemnum = pmgEO_->mgCoM_.mdm_SpSyouhin_.GetPKDt(mid_spsyouhin, mdm_spsyouhin_itemNum);

    ugname_.SetNameSupply(itemid, 1);
    ugname_.SetDraw(TRUE);
    s32 value = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(itemid, mdm_supply_price);
    std::string valuestr = lib_str::IntToStr(value);
    s32 allster = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(itemid, mdm_supply_effect);
    s32 fstar = (allster - value);
    STRVECTOR textvec;
    textvec.push_back("かきん＿：" + lib_str::IntToStr(value) + "スター");
    textvec.push_back("むかきん：" + lib_str::IntToStr(fstar) + "スター");
    textvec.push_back("ごうけい：" + lib_str::IntToStr(allster) + "スター");
    GenSetText(textvec);

    s32 havebstar = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_haveBStar);
    s32 havefstar = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_haveFStar);
    std::string habestr = lib_str::IntToStr(havebstar + havefstar);
    GenSetData(0, "しょじスター", habestr.c_str());
    GenSetButton("かう");
  }

  //ガチャＭＩＤ
  void TUGListEnd::SetDataGacha(s64 mid_gacha)
  {
    UndrawAll();
    ugname_.SetNameGacha(mid_gacha);
    ugname_.SetDraw(TRUE);

    s64 mid_supply_ken = pmgEO_->mgCoM_.mdm_Gacha_.GetPKDt(mid_gacha, mdm_gacha_id_m_supply_ken);
    s32 value = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(mid_supply_ken, mdm_supply_price);
    s32 kennum = pmgEO_->mgCoM_.mdm_Gacha_.GetPKDt(mid_gacha, mdm_gacha_kenNum);

    if (pmgEO_->mgCoU_.IsGachaKenUse(mid_gacha))
    {
      //std::string valuestr = lib_str::IntToStr(kennum);
      SetValue_Gacha(kennum);
      GenSetButton("ガチャけんつかう");
    }
    else
    {
      //std::string valuestr = lib_str::IntToStr(value);
      if (pmgEO_->mgCoM_.IsStarGacha(mid_gacha))
      {
        SetValue_Star(value);
      }
      else
      {
        SetValue_Todd(value);
      }
      GenSetButton("ひく");
    }

    STRVECTOR textvec;
    textvec.push_back(pmgEO_->mgCoM_.mdm_Gacha_.GetPKDtStr(mid_gacha, mdm_gacha_text0_c64));
    textvec.push_back(pmgEO_->mgCoM_.mdm_Gacha_.GetPKDtStr(mid_gacha, mdm_gacha_text1_c64));
    textvec.push_back(pmgEO_->mgCoM_.mdm_Gacha_.GetPKDtStr(mid_gacha, mdm_gacha_text2_c64));
    textvec.push_back(pmgEO_->mgCoM_.mdm_Gacha_.GetPKDtStr(mid_gacha, mdm_gacha_text3_c64));
    GenSetText(textvec);

  }
  //ガチャ中身ＭＩＤ
  void TUGListEnd::SetDataGachaCnt(s64 mid_gachacnt)
  {
    UndrawAll();
    s32 itemtype = pmgEO_->mgCoM_.mdm_GachaCnt_.GetPKDt(mid_gachacnt, mdm_gachacnt_itemType);
    s32 itemid = pmgEO_->mgCoM_.mdm_GachaCnt_.GetPKDt(mid_gachacnt, mdm_gachacnt_itemID);
    s32 itemnum = pmgEO_->mgCoM_.mdm_GachaCnt_.GetPKDt(mid_gachacnt, mdm_gachacnt_itemNum);
    switch (itemtype)
    {
    case enItemType_Supply: GenSetDataSupply(itemid); break;
    case enItemType_Pack: GenSetDataItemPack(itemid); break;
    }
    //ボタン無し
  }

  //なつかしＢＯＸ
  void TUGListEnd::SetDataNaBox(s64 mid_nabox, BOOL otona_f)
  {
    UndrawAll();
    std::string name = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDtStr(mid_nabox, mdm_nabox_name_c64);
    s32 iconno = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDt(mid_nabox, mdm_nabox_iconNo);

    s32 id_nabox = pmgEO_->mgCoU_.GetNaBox_id(mid_nabox);
    s32 opennum = pmgEO_->mgCoU_.mdNaBox_.GetPKDt(id_nabox, mdnabox_openNum);
    s32 boxsize = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDt(mid_nabox, mdm_nabox_boxSize);
    s32 restnum = (boxsize - opennum);

    s64 mid_ken = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDt(mid_nabox, mdm_nabox_id_m_supply_ken);
    s32 kennum = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDt(mid_nabox, mdm_nabox_kenNum);
    s32 price = (pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(mid_ken, mdm_supply_price) * kennum);


    if (otona_f == FALSE)
    {
      ugname_.SetNameNaBox(mid_nabox);
      ugname_.SetDraw(TRUE);
      //std::string valuestr = lib_str::IntToStr(price);
      //GenSetValue(valuestr.c_str());
      SetValue_Star(price);
      STRVECTOR textvec;
      textvec.push_back(pmgEO_->mgCoM_.mdm_NaBox_.GetPKDtStr(mid_nabox, mdm_nabox_text0_c64));
      textvec.push_back(pmgEO_->mgCoM_.mdm_NaBox_.GetPKDtStr(mid_nabox, mdm_nabox_text1_c64));
      textvec.push_back(pmgEO_->mgCoM_.mdm_NaBox_.GetPKDtStr(mid_nabox, mdm_nabox_text2_c64));
      textvec.push_back(pmgEO_->mgCoM_.mdm_NaBox_.GetPKDtStr(mid_nabox, mdm_nabox_text3_c64));
      GenSetText(textvec);
      GenSetData(0, "", "");
      GenSetButton("ひく");
    }
    else
    {
      std::string strrest = "のこり＿" + lib_str::IntToStr(restnum) + "／" + lib_str::IntToStr(boxsize);
      ugname_.SetName("おとながい");
      ugname_.ugicon_.SetIconNaBox(mid_nabox);
      ugname_.SetDraw(TRUE);
      //std::string valuestr = lib_str::IntToStr(price * restnum);
      //GenSetValue(valuestr.c_str());
      SetValue_Star(price * restnum);
      STRVECTOR textvec;
      textvec.push_back("このＢＯＸに＿のこっている");
      textvec.push_back("アイテムを＿すべてかくとく");
      GenSetText(textvec);
      GenSetData(0, "", strrest.c_str());
      GenSetButton("ひく");

    }
  }
  //なつかし中身ＭＩＤ
  void TUGListEnd::SetDataNaBoxCnt(s64 mid_naboxcnt)
  {
    UndrawAll();
    s32 itemtype = pmgEO_->mgCoM_.mdm_NaBoxCnt_.GetPKDt(mid_naboxcnt, mdm_naboxcnt_itemType);
    s32 itemid = pmgEO_->mgCoM_.mdm_NaBoxCnt_.GetPKDt(mid_naboxcnt, mdm_naboxcnt_itemID);
    s32 itemnum = pmgEO_->mgCoM_.mdm_NaBoxCnt_.GetPKDt(mid_naboxcnt, mdm_naboxcnt_itemNum);
    switch (itemtype)
    {
    case enItemType_Supply: GenSetDataSupply(itemid); break;
    case enItemType_Pack: GenSetDataItemPack(itemid); break;
    }
    //ボタン無し
  }

  //福引きＭＩＤ
  void TUGListEnd::SetDataHuku(s64 mid_Huku)
  {
    UndrawAll();
    ugname_.SetNameHuku(mid_Huku);
    ugname_.SetDraw(TRUE);

    s64 kennum = pmgEO_->mgCoM_.mdm_Huku_.GetPKDt(mid_Huku, mdm_huku_kenNum);
    //s32 value = pmgEO_->mgCoM_.mdm_Supply_.GetPKDt(mid_supply_ken, mdm_supply_price);
    //std::string valuestr = lib_str::IntToStr(kennum);
    //GenSetValue(valuestr.c_str());
    SetValue_Huku(kennum);
    STRVECTOR textvec;
    textvec.push_back(pmgEO_->mgCoM_.mdm_Huku_.GetPKDtStr(mid_Huku, mdm_huku_text0_c64));
    textvec.push_back(pmgEO_->mgCoM_.mdm_Huku_.GetPKDtStr(mid_Huku, mdm_huku_text1_c64));
    textvec.push_back(pmgEO_->mgCoM_.mdm_Huku_.GetPKDtStr(mid_Huku, mdm_huku_text2_c64));
    textvec.push_back(pmgEO_->mgCoM_.mdm_Huku_.GetPKDtStr(mid_Huku, mdm_huku_text3_c64));
    GenSetText(textvec);
    GenSetButton("ひく");
  }
  //福引き中身ＭＩＤ
  void TUGListEnd::SetDataHukuCnt(s64 mid_Hukucnt)
  {
    UndrawAll();
    s32 itemtype = pmgEO_->mgCoM_.mdm_HukuCnt_.GetPKDt(mid_Hukucnt, mdm_hukucnt_itemType);
    s32 itemid = pmgEO_->mgCoM_.mdm_HukuCnt_.GetPKDt(mid_Hukucnt, mdm_hukucnt_itemID);
    s32 itemnum = pmgEO_->mgCoM_.mdm_HukuCnt_.GetPKDt(mid_Hukucnt, mdm_hukucnt_itemNum);
    switch (itemtype)
    {
    case enItemType_Supply: GenSetDataSupply(itemid); break;
    case enItemType_Pack: GenSetDataItemPack(itemid); break;
    }
    //ボタン無し
  }



  //獲得情報
  void TUGListEnd::SetInfoMsg(s32 gettype, s64 getinfo, s64 getinfo2)
  {
    UndrawAll();
    STRVECTOR textvec;

    //#取得種類
    switch (gettype)
    {
    case db::GT_INIT://      GT_INIT = 0#初期配布
      textvec.push_back("しょきはいふ");
      break;
    case db::GT_SHOP://      GT_SHOP = 10#ショップ infoなし
      textvec.push_back("ショップこうにゅう");
      break;
    case db::GT_QUSE://      GT_QUSE = 11#即使用
      textvec.push_back("そくじしよう");
      break;
    case db::GT_BBA://      GT_BBA = 13#しゅーとばばあ infoなし
      textvec.push_back("しゅーとおばば");
      break;
    case db::GT_QUEST://      GT_QUEST = 20#クエスト info : マップチップＩＤ
      textvec.push_back("シナリオドロップ");
      textvec.push_back(pmgEO_->mgCoM_.mdm_Maptip_.GetPKDtStr(getinfo, mdm_maptip_name_c64));
      break;
    case db::GT_QUESTCOMP://      GT_QUESTCOMP = 21#コンプ　info : マップチップＩＤ
      textvec.push_back("シナリオドロップコンプ");
      textvec.push_back(pmgEO_->mgCoM_.mdm_Maptip_.GetPKDtStr(getinfo, mdm_maptip_name_c64));
      break;
    case db::GT_LOGBO://      GT_LOGBO = 30#ログインボーナス　info : ログイン回数
      textvec.push_back("ログインボーナス");
      textvec.push_back("ログイン" + lib_str::IntToStr(getinfo) + "かいめ");
      break;
    case db::GT_LOGBOSP://      GT_LOGBO = 30#ログインボーナス　info : ログイン回数
      textvec.push_back("とくべつログインボーナス");
      textvec.push_back("ログイン" + lib_str::IntToStr(getinfo) + "かいめ");
      break;
    case db::GT_GIFT://      GT_GIFT = 31#運営補填　info : 運営補填マスターＩＤ
      textvec.push_back("うんえいおくりもの");
      textvec.push_back(pmgEO_->mgCoM_.mdm_Giftbox_.GetPKDtStr(getinfo, mdm_giftbox_name_c64));
      break;
    case db::GT_RKREWARD://ランキング報酬 info : ランキングＩＤ info2 : ランキング報酬ＩＤ
      textvec.push_back("ランキングほうしゅう");
      textvec.push_back(pmgEO_->mgCoM_.mdm_Ranking_.GetPKDtStr(getinfo, mdm_ranking_name_c64));
      break;
    case db::GT_INVITED://      GT_INVITED = 40#招待入力　info : 招待者ＩＤ
      textvec.push_back("しょうたいにゅうりょく");
      textvec.push_back("しょうたいかんとくＩＤ");
      textvec.push_back(lib_str::IntTo36code((MBGIDBASE - getinfo), MBGIDKETA));//ＩＤは36変換
      break;
    case db::GT_INVITER://      GT_INVITER = 41#招待特典　info : 招待入力者ＩＤ
      textvec.push_back("しょうたいとくてん");
      textvec.push_back("しょうたいかんとくＩＤ");
      textvec.push_back(lib_str::IntTo36code((MBGIDBASE - getinfo), MBGIDKETA));//ＩＤは36変換
      break;
    case db::GT_CAMPAIN://      GT_CAMPAIN = 50#キャンペーン　info : キャンペーンＩＤ
      textvec.push_back("キャンペーンコード");
      textvec.push_back(pmgEO_->mgCoM_.mdm_Campaign_.GetPKDtStr(getinfo, mdm_campaign_name_c64));
      break;
    case db::GT_MBGJZN://      GT_CAMPAIN = 50#キャンペーン　info : キャンペーンＩＤ
      textvec.push_back("モバゲーじぜんとうろく");
      //textvec.push_back(pmgEO_->mgCoM_.mdm_Campaign_.GetPKDtStr(getinfo, mdm_campaign_name_c64);
      break;
    case db::GT_GACHA://      GT_GACHA = 60#ガチャ　info : ガチャＩＤ
      textvec.push_back("ガチャ");
      textvec.push_back(pmgEO_->mgCoM_.mdm_Gacha_.GetPKDtStr(getinfo, mdm_gacha_name_c64));
      break;
    case db::GT_NABOX://      GT_NABOX = 61#なつかしＢＯＸ　info : なつかしＢＯＸＩＤ
      textvec.push_back("なつかしＢＯＸ");
      textvec.push_back(pmgEO_->mgCoM_.mdm_NaBox_.GetPKDtStr(getinfo, mdm_nabox_name_c64));
      break;
    case db::GT_HUKU://      GT_NABOX = 62#なつかしＢＯＸ　info : なつかしＢＯＸＩＤ
      textvec.push_back("ふくびき");
      textvec.push_back(pmgEO_->mgCoM_.mdm_Huku_.GetPKDtStr(getinfo, mdm_huku_name_c64));
      break;
    case db::GT_FROMGIFTBOX://      GT_FROMGIFTBOX = 80#贈り物ボックスからの移動
      textvec.push_back("おくりものＢＯＸ");
      break;
    case db::GT_VSBONUS://      GT_FROMGIFTBOX = 80#贈り物ボックスからの移動
      textvec.push_back("ＶＳほうしゅう");
      break;
    case db::GT_OPENPACK://      GT_FROMGIFTBOX = 80#贈り物ボックスからの移動
      textvec.push_back("アイテムパック");
      break;
    case db::GT_TUTO://      GT_FROMGIFTBOX = 80#贈り物ボックスからの移動
      textvec.push_back("チュートボーナス");
      break;
    case db::GT_DEBUG://      GT_DEBUG = 100#デバッグ infoなし
      textvec.push_back("デバッグ");
      break;
    default:
      break;
    }

    GenSetText(textvec);

  }

  //獲得ログ
  void TUGListEnd::SetDataGetLog(s64 id_itemget)
  {
    UndrawAll();
    s32 itemtype = pmgEO_->mgCoU_.mdlog_ItemGet_.GetPKDt(id_itemget, mdlog_itemget_itemType);
    s32 itemid = pmgEO_->mgCoU_.mdlog_ItemGet_.GetPKDt(id_itemget, mdlog_itemget_itemID);
    s32 itemnum = pmgEO_->mgCoU_.mdlog_ItemGet_.GetPKDt(id_itemget, mdlog_itemget_itemNum);
    s64 getdate = pmgEO_->mgCoU_.mdlog_ItemGet_.GetPKDt(id_itemget, mdlog_itemget_create_date);

    ugname_.SetName(itemtype, itemid, itemnum);
    ugname_.SetDraw(TRUE);
    std::string datestr = base::UnixtimeToDateString(getdate, TRUE);
    GenSetData(0, "", datestr.c_str());
  }
  //スター購入ログ
  void TUGListEnd::SetDataLogBuyStar(s64 id_bstar)
  {
    //s32 itemtype = pmgEO_->mgCoU_.mdlog_ItemGet_.GetPKDt(id_itemget, mdlog_itemget_itemType);
    //s32 itemid = pmgEO_->mgCoU_.mdlog_ItemGet_.GetPKDt(id_itemget, mdlog_itemget_itemID);
    //s32 itemnum = pmgEO_->mgCoU_.mdlog_ItemGet_.GetPKDt(id_itemget, mdlog_itemget_itemNum);
    //s64 getdate = pmgEO_->mgCoU_.mdlog_ItemGet_.GetPKDt(id_itemget, mdlog_itemget_create_date);

    //ugname_.SetName(itemtype, itemid, itemnum);
    //std::string datestr = base::UnixtimeToDateString(getdate, TRUE);
    //GenSetData(0, "", datestr.c_str());
  }

  void TUGListEnd::SetDataProf(s64 id_kantoku)
  {
    UndrawAll();
    ugrprof_.SetProfMe();
    ugrprof_.SetPos(36, 8);
    prof_f_ = TRUE;
  }
  void TUGListEnd::SetDataInfoStar()
  {
    UndrawAll();
    s32 bstar = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_haveBStar);
    s32 fstar = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_haveFStar);
    //s32 techno = mid::midResBalance();
    s32 hukumade = (XYMAG - (pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_totalStar) % XYMAG));
    //std::string bstarstr = "かきん＿：" + lib_str::IntToStr(bstar) + "スター";
    //std::string fstarstr = "むかきん：" + lib_str::IntToStr(fstar) + "スター";
    //std::string technostr = "テクノ＿：" + lib_str::IntToStr(techno) + "テクノ";
    //std::string hukustr = "ふくびきけんまで＿あと：" + lib_str::IntToStr(techno) + "スター";
    ugname_.SetNameStar(bstar + fstar);
    ugname_.SetDraw(TRUE);

    STRVECTOR textvec;
    textvec.push_back("かきん＿：" + lib_str::IntToStr(bstar) + "スター");
    textvec.push_back("むかきん：" + lib_str::IntToStr(fstar) + "スター");
    //textvec.push_back("テクノ＿：" + lib_str::IntToStr(techno) + "テクノ");
    //textvec.push_back("あと" + lib_str::IntToStr(hukumade) + "スター＿つかうと");
    //textvec.push_back("ふくびきけん＿１まいプレゼント！");
    GenSetText(textvec);
  }


  BOOL TUGListEnd::IsActRes(enUGListEndAct res)
  {
    return (GetActRes() == res);
  }
}
