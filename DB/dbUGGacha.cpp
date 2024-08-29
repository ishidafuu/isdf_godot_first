#include "dbUGGacha.h"
#include "dbUILayer.h"

namespace db
{
  const s32 TOPPOSY = 128;
  const s32 FLASHITV = 6;
  const s32 HDLANINUM = 5;
  const s32 CUPHP = 7;
  enum enFallStep
  {
    fs_None,
    fs_gacha,
    fs_kantoku,
    fs_big,
    fs_bigbound,
    fs_end,
  };

  enum enPunchType
  {
    pt_None,
    pt_Lv0,
    pt_LvN,
    pt_BigCount,
    pt_BigNext,
  };

  enum enGachaBGM
  {
    bgm_shop = 243,
    bgm_gacha = 50,
    bgm_lv1_1 = 52,
    bgm_lv1_2 = 69,
    bgm_lv2_1 = 61,
    bgm_lv2_2 = 56,
    bgm_lv3_1 = 81,
    bgm_lv3_2 = 48,
    bgm_lv4_1 = 284,
    bgm_lv4_2 = 469,
  };

  enum enGachaSE
  {
    se_gachafall = 10,
    se_gachabound = 11,
    se_punch = 12,
    se_punchhit = 13,
    se_todo = 14,
    se_kimehit = 15,
    se_tame = 16,
    se_bigbound = 20,
    se_hissatsu = 26,
  };


  //コンストラクタ
  TUGGacha::TUGGacha(TUIGenLayer* player) : TUGGen(player)
    , ugfilter_(player)
  {
    stGacha_.Init();
  }

  TUGGacha::~TUGGacha()
  {

  }

  void TUGGacha::MakeGroup(TUIObj* parent, TUGChKantoku* ugkantoku, TUGChCaptain* ugcap, TUGChClerk* ugcleak)
  {
    SetParent(parent);
    ugkantoku_ = ugkantoku;
    ugcap_ = ugcap;
    ugcleak_ = ugcleak;

    ugfilter_.MakeGroup(NULL);

    //field_->o_SetObjGrpPart(grppart);
    field_->z_SetPos(0, 0);

#ifdef __GACHA_USE_BTN__
    btn_ = MakeObj();	// ボタンオブジェ
    btn_->o_SetParent(field_);
    btn_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_pad_btn));
    // ボタンの位置はスマホの画面の大きさによって変わる
    // 右下から～の位置、と定義
    s32 w = mid::midGetDotL();
    s32 h = mid::midGetDotS();
    s32 footerSize = 48;	// フッターの高さ
    btn_->g_SetCenter(TRUE);
    btn_->z_SetPos(w - 64, h - 64 - footerSize);	// 64は自身のアイコンサイズ/2
    btn_->g_SetAlp(TRUE, 128);
    btn_->g_SetDraw(FALSE);
    btn_->g_SetPartNo(0);
#endif // #define __GACHA_USE_BTN__

    gacha_ = MakeObj();//ガチャ筐体
    gacha_->o_SetParent(field_);
    gacha_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_gacha));
    gacha_->z_SetPos(8, 0);
    gacha_->g_SetDraw(FALSE);

    handle_ = MakeObj();//ハンドル
    handle_->o_SetParent(gacha_);
    handle_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_handle));
    handle_->z_SetPos(44, 52);

    const s32 LIMY = -60;
    const s32 LIMH = 88;

    gacha_->g_SetLimY(LIMY, LIMH);
    handle_->g_SetLimY(LIMY, LIMH);

    exmark_ = MakeObj();//びっくりマーク

    for (s32 i = 0; i < CAPNUM; ++i)
    {
      capsule_[i].obj_ = MakeObj();//カプセル
      capsule_[i].obj_->o_SetParent(field_);
      capsule_[i].obj_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_cupsel));
      capsule_[i].obj_->g_SetDraw(FALSE);
      capsule_[i].obj_->g_SetLimX(-160, 152);
      //各レア度ごとの回転アニメーション
      for (s32 i2 = 0; i2 < RARITYNUM; ++i2)
      {
        INTVECTOR rollani;
        const s32 ROLLNUM = 12;
        const s32 BASENO = (ROLLNUM * i2);
        for (s32 i3 = 0; i3 < ROLLNUM; ++i3) rollani.push_back(BASENO + i3);
        capsule_[i].obj_->a_AddAnivec(&rollani, 4, at_Loop);
      }

    }

    bigcapsule_ = MakeObj();//大玉カプセル
    bigcapsule_->o_SetParent(field_);
    bigcapsule_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_cupselbig));
    bigcapsule_->g_SetDraw(FALSE);
    bigcapsule_->z_SetPos(88, 32);

    hibi_ = MakeObj();//ひび
    hibi_->o_SetParent(bigcapsule_);
    hibi_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_hibi));
    hibi_->g_SetDraw(FALSE);
    hibi_->z_SetPos(0, 7);

    bigcapsule_->g_SetLimY(LIMY, LIMH);
    hibi_->g_SetLimY(LIMY, LIMH);
  }


  void TUGGacha::SetGachaType(enUGGachaType gachatype)//ガチャタイプ
  {
    //enUGGachaType_GachaTodd
    gachatype_ = gachatype;
    gacha_->g_SetPartNo(gachatype);
    handle_->g_SetPartNo(gachatype * HDLANINUM);//レバー
    stGacha_.garagara_c_ = 0;

  }
  void TUGGacha::SetPattern()//パターン決定
  {

    pattern_.clear();
    rarity_.clear();
    //ログから持ってくる（ガチャを引く前にCoLogをクリアする）
    s32 num = pmgEO_->mgCoU_.GetCapsuleNum();

    if (pmgEO_->stSeq_.IsNoSv()) num = 1;

#ifdef __K_DEBUG_USER_DATA__
    num = kdebug::DebugUDManager::GetInstance()->gatyaNum_;
#endif

    for (s32 i = 0; i < num; ++i)
    {
      s32 rarity = 0;
      //チュートリアルのトッドガチャはレアリティC固定
      if (pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(i, mdlog_itemget_getType) == GT_TUTO)
      {
        rarity = 1;
      }
      else
      {
        s64 mid_gachacnt = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(i, mdlog_itemget_getInfo2);
        s32 itemtype = pmgEO_->mgCoM_.mdm_GachaCnt_.GetPKDt(mid_gachacnt, mdm_gachacnt_itemType);
        s32 itemid = pmgEO_->mgCoM_.mdm_GachaCnt_.GetPKDt(mid_gachacnt, mdm_gachacnt_itemID);
        s32 itemnum = pmgEO_->mgCoM_.mdm_GachaCnt_.GetPKDt(mid_gachacnt, mdm_gachacnt_itemNum);
        switch (itemtype)
        {
        case db::enItemType_Team:
          rarity = pmgEO_->mgCoM_.mdm_Team_.GetLineDt(itemid, mdm_team_rarity);
          break;
        case db::enItemType_Sozai:
          rarity = (itemid % SOZAIMID);
          break;
        case db::enItemType_Shot:
          break;
        case db::enItemType_BGM:
          break;
        case db::enItemType_Haikei:
          break;
        case db::enItemType_Supply:
          break;
        case db::enItemType_Todd:
          break;
        case db::enItemType_Star:
          break;
        case db::enItemType_Pack:
          break;
        case db::enItemType_Huku:
          break;
        default:
          break;
        }
      }


      //レアリティ保存
      rarity_.push_back(rarity);

      s32 ptn = 0;
      //アイテムごとのレア度（演出レベル）を取得
      s32 lv = (rarity - 2); //3以上
      s32 keta = 1;
      for (s32 i2 = 0; i2 < lv; ++i2)
      {
        s32 tmpptn = (mid::midGetRandMn(100) < 80)
          ? 2
          : 1;

        //もう上がない
        if ((lv <= (i2 + 1)) && (mid::midGetRandMn(100) < 80))
        {
          tmpptn = 1;
        }

        ptn += (keta * tmpptn);
        keta *= 10;//次の桁へ
      }
      //パターン保存
      pattern_.push_back(ptn);
    }
  }

  void TUGGacha::TapField()
  {
    Act_Punch();
  }

  void TUGGacha::Act_FallGacha()//ガチャ落ちてくる命令
  {
    stGacha_.Init();
    stGacha_.posY_ = TOPPOSY * XYMAG;
    stGacha_.dY_ = 0;
    stGacha_.fallstep_ = fs_gacha;
    stGacha_.punch_c_ = 0;
    stGacha_.cupstop_c_ = 0;
    stGacha_.gachaout_f_ = FALSE;
    stGacha_.out_c_ = 0;
    stGacha_.panchtype_ = pt_None;
    gacha_->g_SetDraw(TRUE);
    s32 pos = -(stGacha_.posY_ / XYMAG);
    gacha_->g_SetGrpPos(0, pos);
    handle_->g_SetGrpPos(0, pos);
    //パターン決定
    SetPattern();
    pmgEO_->mgNSF_.StopBGM();
    pmgEO_->mgSound_.PlaySYS(se_gachafall);
#ifdef __GACHA_USE_BTN__
    btn_->g_SetDraw(TRUE);
#endif // #define __GACHA_USE_BTN__
  }
  void TUGGacha::Act_Punch()//パンチ入力
  {

    //ハンドルをパンチ
    if (stGacha_.panchOK_f_ && (stGacha_.punch_c_ < pattern_.size()))
    {
#ifdef __GACHA_USE_BTN__
      // ボタン押したアニメ
      btn_->g_SetPartNo(1);
#endif // #define __GACHA_USE_BTN__

      ugkantoku_->SetMirror(TRUE);
      stGacha_.panchOK_f_ = FALSE;

      //現在のパターン
      stGacha_.nowrarity_ = rarity_[stGacha_.punch_c_];
      stGacha_.nowpattern_ = pattern_[stGacha_.punch_c_];
      stGacha_.nowpattern2_ = (stGacha_.nowpattern_ % 10);

      //通常パンチ
      if (stGacha_.nowpattern_ == 0)
      {
        stGacha_.panchtype_ = pt_Lv0;
        ugkantoku_->SetMType(kmt_Punch);//パンチ
        pmgEO_->mgSound_.PlaySYS(se_punch);
      }
      else//大玉出現パンチ
      {

        pmgEO_->mgNSF_.StopBGM();
        stGacha_.panchtype_ = pt_LvN;
        ugkantoku_->SetMType(kmt_TameUpper);//アッパー
        pmgEO_->mgSound_.PlaySYS(se_tame);
        pmgEO_->mgSound_.PlaySYS(se_todo);
        //pmgEO_->mgSound_.PlaySYS(se_upper);
      }

      ++stGacha_.punch_c_;
    }
    //巨大カプセルをパンチ
    else if (stGacha_.bigpanchOK_f_ && (stGacha_.bigcuphp_ > 0))
    {
      stGacha_.bigpanchOK_f_ = FALSE;

#ifdef __GACHA_USE_BTN__
      // ボタン押したアニメ
      btn_->g_SetPartNo(1);
#endif // #define __GACHA_USE_BTN__

      //通常パンチ
      --stGacha_.bigcuphp_;
      if (stGacha_.bigcuphp_ == 0)
      {
        stGacha_.panchtype_ = pt_BigCount;
        s32 nextptn = (stGacha_.nowpattern_ / 10);
        stGacha_.panchtype_ = (nextptn == 0)//ここでおわり
          ? pt_BigCount
          : pt_BigNext;//次段階
        stGacha_.nowpattern_ = nextptn;
        stGacha_.nowpattern2_ = (stGacha_.nowpattern_ % 10);

        ugkantoku_->SetMType(kmt_Upper);//アッパー
        //pmgEO_->mgSound_.PlaySYS(se_tame);
        pmgEO_->mgSound_.PlaySYS(se_punch);

        if (stGacha_.panchtype_ == pt_BigNext)
        {
          pmgEO_->mgNSF_.StopBGM();
          ugkantoku_->SetMType(kmt_TameUpper);//アッパー
          pmgEO_->mgSound_.PlaySYS(se_tame);
        }
        else
        {
          ugkantoku_->SetMType(kmt_Upper);//アッパー
          pmgEO_->mgSound_.PlaySYS(se_punch);
        }
      }
      else
      {
        stGacha_.panchtype_ = pt_BigCount;

        ugkantoku_->SetMType(kmt_Punch);//パンチ
        pmgEO_->mgSound_.PlaySYS(se_punch);
      }

    }
  }

  void TUGGacha::Mv_FallGacha()//ガチャ落ちてくる
  {
    s32 gposy = 0;
    s32 kposy = 0;
    s32 bposy = 0;

    if ((stGacha_.fallstep_ == fs_gacha)
      || (stGacha_.fallstep_ == fs_kantoku)
      || (stGacha_.fallstep_ == fs_big)
      || (stGacha_.fallstep_ == fs_bigbound)
      )
    {
      s32 GRV = (stGacha_.fallstep_ == fs_gacha)
        ? 7
        : 10;
      if (stGacha_.posY_ > 0)
      {
        stGacha_.posY_ += stGacha_.dY_;
        stGacha_.dY_ -= GRV;
        //着地
        if (stGacha_.posY_ <= 0)
        {
          stGacha_.posY_ = 0;
          stGacha_.dY_ = 0;
          switch (stGacha_.fallstep_)
          {
          case fs_gacha:
            stGacha_.fallstep_ = fs_kantoku;
            stGacha_.posY_ = 1;
            stGacha_.dY_ = 100;
            pmgEO_->mgSound_.PlaySYS(se_gachabound);
            break;
          case fs_kantoku:
            stGacha_.fallstep_ = fs_end;
            stGacha_.panchOK_f_ = TRUE;
            pmgEO_->mgNSF_.Play(nsf_gacha);//BGM
            break;
          case fs_big:
            stGacha_.fallstep_ = fs_bigbound;
            stGacha_.posY_ = 1;
            stGacha_.dY_ = 150;
            ugkantoku_->SetMirror(FALSE);
            pmgEO_->mgSound_.PlaySYS(se_bigbound);
            break;
          case fs_bigbound:
            stGacha_.fallstep_ = fs_end;
            stGacha_.bigpanchOK_f_ = TRUE;
            pmgEO_->mgSound_.PlaySYS(se_bigbound);
            break;
          }
        }
      }

      s32 posy = -(stGacha_.posY_ / XYMAG);
      switch (stGacha_.fallstep_)
      {
      case fs_gacha:gposy = posy; break;
      case fs_kantoku:kposy = posy; break;
      case fs_big:
      case fs_bigbound:
        bposy = posy;
        break;
      }
    }

    gacha_->g_SetGrpPos(0, gposy);
    handle_->g_SetGrpPos(0, gposy);
    bigcapsule_->g_SetGrpPos(0, bposy);
    hibi_->g_SetGrpPos(0, bposy);
    ugkantoku_->GetField()->c_SetPos(0, kposy);
    ugcleak_->GetField()->c_SetPos(0, kposy);
	ugcap_->GetField()->c_SetPos(0, kposy);
  }

  void TUGGacha::SetCapsule(s32 rarity, BOOL bigopen_f)//カプセル出現
  {
    s32 posx = (bigopen_f)
      ? 96
      : 64;

    capsule_[stGacha_.nextcap_].Init();
    capsule_[stGacha_.nextcap_].obj_->a_SetAniNo(rarity, TRUE);
    capsule_[stGacha_.nextcap_].obj_->g_SetDraw(TRUE);
    capsule_[stGacha_.nextcap_].obj_->z_SetPos(posx, 52);
    capsule_[stGacha_.nextcap_].obj_->z_MoveSetspd(160, 52, 1);
    lib_num::IncLoop(&stGacha_.nextcap_, 0, (CAPNUM - 1));

    //最後のパンチが終わってそのカプセル
    if (stGacha_.punch_c_ == pattern_.size())
    {
      stGacha_.gachaout_f_ = TRUE;
      stGacha_.out_c_ = 0;
      pmgEO_->mgNSF_.Play(nsf_shop_gatya);//BGM
#ifdef __GACHA_USE_BTN__
      btn_->g_SetDraw(FALSE);
#endif // #define __GACHA_USE_BTN__
    }
  }
  void TUGGacha::SetBigCapsule(s32 rarity)//巨大カプセル出現
  {
    bigcapsule_->g_SetPartNo(rarity);
    bigcapsule_->g_EfReset();
    bigcapsule_->g_SetARGB(FALSE, 0xFF, 0xFF, 0xFF, 0xFF);
    bigcapsule_->g_SetDraw(TRUE);
    hibi_->g_SetDraw(FALSE);
    stGacha_.posY_ = TOPPOSY * XYMAG;
    stGacha_.dY_ = 0;
    stGacha_.fallstep_ = fs_big;
    stGacha_.bigbreak_c_ = 0;

    s32 pos = -(stGacha_.posY_ / XYMAG);
    bigcapsule_->g_SetGrpPos(0, pos);
    hibi_->g_SetGrpPos(0, pos);
    stGacha_.bigpanchOK_f_ = FALSE;

    stGacha_.bigcuprarity_ = rarity;
    stGacha_.bigcuphp_ = CUPHP;//割れるまでの回数
    stGacha_.bigcuphpmax_ = stGacha_.bigcuphp_;
  }
  void TUGGacha::Mv_Punch()//パンチ
  {
    if ((stGacha_.panchtype_ == pt_Lv0)
      || (stGacha_.panchtype_ == pt_LvN))
    {
      //パンチヒットタイミング
      if (ugkantoku_->IsActRes(enUGChGenAct_ActPoint))
      {
        //連続カウンタ
        pmgEO_->mgFont_.OrderDamFontSpriteNocam(stGacha_.punch_c_, -102, -28, (base::enFontCol2)FC2_normal);

        ++stGacha_.handle_c_;
        if (stGacha_.nowpattern2_ > 0)//演出時はフラッシュ
        {
          pmgEO_->mgSound_.PlaySYS(se_kimehit);

          stGacha_.flash_c_ = FLASHITV * 1;
          if (stGacha_.nowpattern2_ == 1)
          {
            pmgEO_->mgNSF_.Play(nsf_ga_lv1_1);//BGM
          }
          else
          {
            pmgEO_->mgNSF_.Play(nsf_ga_lv1_2);//BGM
          }
        }
        else
        {
          pmgEO_->mgSound_.PlaySYS(se_punchhit);
        }
      }
      else if (stGacha_.handle_c_ > 0)
      {
        ++stGacha_.handle_c_;
        const s32 HDLANIITV = 2;

        s32 BASENO = gachatype_ * HDLANINUM;

        //一周回った
        if (stGacha_.handle_c_ > (HDLANIITV * HDLANINUM))
        {
          stGacha_.handle_c_ = 0;
          handle_->g_SetPartNo(BASENO);//レバー
          switch (stGacha_.panchtype_)
          {
          case pt_Lv0:
            //通常カプセル出現
            SetCapsule(stGacha_.nowrarity_, FALSE);
            stGacha_.panchOK_f_ = TRUE;
            break;
          case pt_LvN:
            //巨大カプセル出現
            SetBigCapsule(3);//金から
            //演出によるBGM
            break;

          }
          //パンチ終了
          stGacha_.panchtype_ = pt_None;
        }
        else
        {
          s32 hdlanino = ((stGacha_.handle_c_ / HDLANIITV) % HDLANINUM);
          handle_->g_SetPartNo(BASENO + hdlanino);//レバー
        }
      }
    }
  }
  void TUGGacha::Mv_BigPunch()//巨大カプセルパンチ
  {
    if ((stGacha_.panchtype_ == pt_BigCount)
      || (stGacha_.panchtype_ == pt_BigNext))
    {
      //パンチヒットタイミング
      if (ugkantoku_->IsActRes(enUGChGenAct_ActPoint))
      {
        stGacha_.bigpanchOK_f_ = TRUE;

        //連続カウンタ
        pmgEO_->mgFont_.OrderDamFontSpriteNocam(stGacha_.bigcuphp_, -62, -38, (base::enFontCol2)FC2_normal);

        //割れる
        if (stGacha_.bigcuphp_ == 0)
        {
          BOOL next_f = (stGacha_.nowpattern2_ != 0);
          if (next_f)
          {
            pmgEO_->mgSound_.PlaySYS(se_hissatsu);
            pmgEO_->mgSound_.PlaySYS(se_kimehit);
          }
          else
          {
            pmgEO_->mgSound_.PlaySYS(se_kimehit);
          }
          //pmgEO_->mgSound_.PlaySYS(se_punchhit);
          ++stGacha_.bigbreak_c_;
        }
        else//ひび割れ
        {
          pmgEO_->mgSound_.PlaySYS(se_punchhit);

          if (stGacha_.bigcuphp_ == ((stGacha_.bigcuphpmax_ * 2) / 3))
          {
            hibi_->g_SetPartNo(0);
            hibi_->g_SetDraw(TRUE);
          }
          else if (stGacha_.bigcuphp_ == ((stGacha_.bigcuphpmax_ * 1) / 3))
          {
            hibi_->g_SetPartNo(1);
          }
        }
      }
      else if (stGacha_.bigbreak_c_ > 0)
      {
        ++stGacha_.bigbreak_c_;

        BOOL next_f = (stGacha_.nowpattern2_ != 0);

        if (next_f)
        {
          s32 bgm = (stGacha_.bigcuprarity_ - 3);// *2;
          switch (stGacha_.bigbreak_c_)
          {
          case 2:

            bigcapsule_->g_Flash();
            hibi_->g_Flash();

            stGacha_.fallstep_ = fs_big;
            stGacha_.posY_ = 1;
            stGacha_.dY_ = 200;
            //演出時はフラッシュ
            stGacha_.flash_c_ = FLASHITV * 2;
            if (stGacha_.nowpattern2_ == 1)
            {
              switch (bgm)
              {
              case 0:pmgEO_->mgNSF_.Play((enNSF)(nsf_ga_lv2_1)); break;
              case 1:pmgEO_->mgNSF_.Play((enNSF)(nsf_ga_lv3_1)); break;
              case 2:pmgEO_->mgNSF_.Play((enNSF)(nsf_ga_lv4_1)); break;
              }
            }
            else
            {
              switch (bgm)
              {
              case 0:pmgEO_->mgNSF_.Play((enNSF)(nsf_ga_lv2_2)); break;
              case 1:pmgEO_->mgNSF_.Play((enNSF)(nsf_ga_lv3_2)); break;
              case 2:pmgEO_->mgNSF_.Play((enNSF)(nsf_ga_lv4_2)); break;
              }
            }
            break;
          case 8:
            stGacha_.bigcuprarity_ += 1;
            bigcapsule_->g_SetPartNo(stGacha_.bigcuprarity_);
            hibi_->g_SetDraw(FALSE);
            stGacha_.bigbreak_c_ = 0;
            stGacha_.bigpanchOK_f_ = FALSE;

            stGacha_.bigcuphp_ = (stGacha_.bigcuphpmax_ + CUPHP);//割れるまでの回数
            stGacha_.bigcuphpmax_ = stGacha_.bigcuphp_;
            break;
          }
        }
        else
        {
          //ここでおわり
          switch (stGacha_.bigbreak_c_)
          {
          case 2:
            bigcapsule_->g_Flash();
            hibi_->g_Flash();
            break;
          case 8:
            bigcapsule_->g_SetARGB(FALSE, 0xFF, 0xFF, 0xFF, 0xFF);
            hibi_->g_SetDraw(FALSE);
            break;
          case 16:
            pmgEO_->mgNSF_.StopBGM();//BGM
            bigcapsule_->g_Fade(TRUE);
            SetCapsule(stGacha_.nowrarity_, TRUE);

            break;
          case 24:
            pmgEO_->mgNSF_.Play(nsf_gacha);//BGM
            ugkantoku_->SetMirror(TRUE);
            stGacha_.panchOK_f_ = TRUE;
            stGacha_.panchtype_ = pt_None;
            break;
          }
        }
      }
    }
  }

  void TUGGacha::Mv_Capsule()//カプセル
  {
    for (s32 i = 0; i < CAPNUM; ++i)
    {
      //カプセルの停止
      if (capsule_[i].obj_->z_IsStop())
      {
        capsule_[i].obj_->g_SetDraw(FALSE);
        ++stGacha_.cupstop_c_;

        //if (stGacha_.cupstop_c_ >= pattern_.size())
        //{
        //  SetAct(enUGGachaAct_End);
        //}
        //else

        SetAct(enUGGachaAct_CupsuleOut, (stGacha_.cupstop_c_ - 1));

      }
    }
  }
  void TUGGacha::Mv_OutGacha()//ガチャはける
  {
    if (stGacha_.gachaout_f_)
    {

      ++stGacha_.out_c_;

	  if (stGacha_.out_c_ == 180)
	  {
		  SetAct(enUGGachaAct_End);
	  }

      s32 posy = -stGacha_.out_c_ / 3;
      gacha_->g_SetGrpPos(0, posy);
      handle_->g_SetGrpPos(0, posy);

      if (posy < -TOPPOSY)
      {
        stGacha_.gachaout_f_ = FALSE;
        gacha_->g_SetDraw(FALSE);
      }
    }
  }
  void TUGGacha::Mv_Flash()//フラッシュ
  {
    if (stGacha_.flash_c_ > 0)
    {
      if (stGacha_.flash_c_ % FLASHITV == 0)
      {
        ugfilter_.Flash(stGacha_.nowpattern2_ == 2);
      }
      --stGacha_.flash_c_;
    }
  }

  void TUGGacha::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
#ifdef __GACHA_USE_BTN__
    btn_->Update(draw_f);	// ボタン
#endif // #define __GACHA_USE_BTN__
    gacha_->Update(draw_f);//ガチャ筐体
    handle_->Update(draw_f);//レバー
    exmark_->Update(draw_f);//びっくりマーク
    hibi_->Update(draw_f);//大玉のひび
    for (s32 i = 0; i < CAPNUM; ++i) capsule_[i].obj_->Update(draw_f);//カプセル
    bigcapsule_->Update(draw_f);//大玉カプセル
    ugfilter_.Update(draw_f);
  }

  void TUGGacha::DoAction()
  {
#ifdef __GACHA_USE_BTN__
    // ボタン離したアニメ
    btn_->g_SetPartNo(0);
#endif // #define __GACHA_USE_BTN__

    Mv_FallGacha();
    Mv_Punch();
    Mv_BigPunch();
    Mv_Capsule();
    Mv_OutGacha();
    Mv_Flash();
  }

  BOOL TUGGacha::IsActRes(enUGGachaAct res)//返値ゲット
  {
    return (GetActRes() == res);
  }

  }
