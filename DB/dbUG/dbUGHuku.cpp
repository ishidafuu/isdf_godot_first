#include "dbUGHuku.h"

namespace db
{
  const s32 TOPPOSY = 128;
  const s32 FLASHITV = 6;
  const s32 HDLANINUM = 5;
  const s32 HUKUPARTNO = 3;//ガチャ３つの次
  const s32 CUPHP = 7;//ガチャ３つの次
  const s32 GACHACUPOFS = (12 * RARITYNUM);
  enum enFallStep
  {
    fs_None,
    fs_Huku,
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

  enum enHukuBGM
  {
    bgm_shop = 243,
    bgm_Huku = 50,
    bgm_lv1_1 = 52,
    bgm_lv1_2 = 69,
    bgm_lv2_1 = 61,
    bgm_lv2_2 = 56,
    bgm_lv3_1 = 81,
    bgm_lv3_2 = 48,
    bgm_lv4_1 = 284,
    bgm_lv4_2 = 469,
  };

  enum enHukuSE
  {
    se_Hukufall = 10,
    se_Hukubound = 11,
    se_punch = 12,
    se_punchhit = 13,
    se_todo = 14,
    se_kimehit = 15,
    se_tame = 16,
    se_bigbound = 20,
    se_hissatsu = 26,
  };

  enum enRareStep
  {
    rs_Invalid = -1,
    rs_CrankFallInit = 0,
    rs_CrankFall,
    rs_PunchInit,
    rs_Punch,
    rs_CrankMove,
    rs_FlyCapsuleInit,
    rs_FlyCapsule,
  };


  //コンストラクタ
  TUGHuku::TUGHuku(TUIGenLayer* player) : TUGGen(player), ugfilter_(player)//アクションなし
  {
    stHuku_.Init();
  }

  TUGHuku::~TUGHuku()
  {

  }

  void TUGHuku::MakeGroup(TUIObj* parent, TUGChKantoku* ugkantoku, TUGChCaptain* ugcap, TUGChClerk* ugcleak)
  {
    SetParent(parent);
    ugkantoku_ = ugkantoku;
    ugcap_ = ugcap;
    ugcleak_ = ugcleak;

    ugfilter_.MakeGroup(NULL);

    //field_->o_SetObjGrpPart(grppart);
    field_->z_SetPos(0, 0);

#ifdef __HUKUBIKI_USE_BTN__
    btn_ = MakeObj();	// ボタンオブジェ
    btn_->o_SetParent(field_);
    btn_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_pad_btn));
    // ボタンの位置はスマホの画面の大きさによって変わる
    // 右下から～の位置、と定義
    s32 w = mid::midGetDotL();
    s32 h = mid::midGetDotS();
    s32 footerSize = 48;	// フッターの高さ
    btn_->g_SetCenter(TRUE);
    btn_->z_SetPos(w-64, h-64-footerSize);	// 64は自身のアイコンサイズ/2
    btn_->g_SetAlp(TRUE, 128);
    btn_->g_SetDraw(FALSE);
    btn_->g_SetPartNo(0);
#endif	// #ifdef __HUKUBIKI_USE_BTN__

    Huku_ = MakeObj();//ガチャ筐体
    Huku_->o_SetParent(field_);
    Huku_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_gacha));
    Huku_->z_SetPos(8, 0);
    Huku_->g_SetDraw(FALSE);
    Huku_->g_SetPartNo(HUKUPARTNO);


    handle_ = MakeObj();//ハンドル
    handle_->o_SetParent(Huku_);
    handle_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_handle));
    handle_->z_SetPos(44, 52);
    handle_->g_SetPartNo(HUKUPARTNO * HDLANINUM);//レバー
    const s32 LIMY = -60;
    const s32 LIMH = 88;

    crank_ = MakeObj();	// クランク
    crank_->o_SetParent(Huku_);
    crank_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_huku_crank));
    crank_->z_SetPos(8, 8);
    crank_->g_SetPartNo(5);//クランク
    crank_->g_SetLimY(LIMY, LIMH);
    //crank_->g_SetDraw(FALSE);

    Huku_->g_SetLimY(LIMY, LIMH);
    handle_->g_SetLimY(LIMY, LIMH);

    exmark_ = MakeObj();//びっくりマーク


    const s32 ROLLNUM = (12 / 2);//福引きのカプセルは半分周期
    const s32 GACHACUPOFS = (12 * RARITYNUM);

    for (s32 i = 0; i < CAPNUM; ++i)
    {
      capsule_[i].obj_ = MakeObj();//カプセル
      capsule_[i].obj_->o_SetParent(field_);
      capsule_[i].obj_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_cupsel));
      capsule_[i].obj_->g_SetDraw(FALSE);
      capsule_[i].obj_->g_SetLimX(-160, 152);
      ////各レア度ごとの回転アニメーション
      //for (s32 i2 = 0; i2 < RARITYNUM; ++i2)
      //{
      //  INTVECTOR rollani;
      //  const s32 BASENO = OFS + (ROLLNUM * i2);//ガチャカプセル分オフセット
      //  for (s32 i3 = 0; i3 < ROLLNUM; ++i3) rollani.push_back(BASENO + i3);
      //  capsule_[i].obj_->a_AddAnivec(&rollani, 4, at_Loop);
      //}
    }

    // 大玉カプセル演出用ミニカプセル
    capsuleEx_.obj_ = MakeObj();//カプセル
    capsuleEx_.obj_->o_SetParent(field_);
    capsuleEx_.obj_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_cupsel));
    capsuleEx_.obj_->g_SetDraw(FALSE);
    capsuleEx_.obj_->g_SetLimY(LIMY, LIMH);
    ////各レア度ごとの回転アニメーション
    //for (s32 i2 = 0; i2 < RARITYNUM; ++i2)
    //{
    //  INTVECTOR rollani;
    //  const s32 BASENO = OFS + (ROLLNUM * i2);//ガチャカプセル分オフセッ
    //  for (s32 i3 = 0; i3 < ROLLNUM; ++i3) rollani.push_back(BASENO + i3);
    //  capsuleEx_.obj_->a_AddAnivec(&rollani, 4, at_Loop);
    //}

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

  void TUGHuku::SetPattern()//パターン決定
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
      s64 mid_Hukucnt = pmgEO_->mgCoU_.mdlog_ItemGet_.GetLineDt(i, mdlog_itemget_getInfo2);
      //福引きは福引き等級をそのままレアリティにする
      s32 rarity = pmgEO_->mgCoM_.mdm_HukuCnt_.GetPKDt(mid_Hukucnt, mdm_hukucnt_grade);

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

  void TUGHuku::TapField()
  {
    Act_Punch();
  }

  void TUGHuku::Act_FallHuku()//ガチャ落ちてくる命令
  {

    stHuku_.Init();

    stHuku_.garagara_c_ = 0;

    stHuku_.posY_ = TOPPOSY * XYMAG;
    stHuku_.dY_ = 0;
    stHuku_.fallstep_ = fs_Huku;
    stHuku_.punch_c_ = 0;
    stHuku_.cupstop_c_ = 0;
    stHuku_.Hukuout_f_ = FALSE;
    stHuku_.out_c_ = 0;
    stHuku_.panchtype_ = pt_None;
    Huku_->g_SetDraw(TRUE);
    s32 pos = -(stHuku_.posY_ / XYMAG);
    Huku_->g_SetGrpPos(0, pos);
    handle_->g_SetGrpPos(0, pos);
    crank_->g_SetGrpPos(0, pos);
    //パターン決定
    SetPattern();
    pmgEO_->mgNSF_.StopBGM();
    pmgEO_->mgSound_.PlaySYS(se_Hukufall);
#ifdef __HUKUBIKI_USE_BTN__
    btn_->g_SetDraw(TRUE);
#endif // #ifdef __HUKUBIKI_USE_BTN__
  }
  void TUGHuku::Act_Punch()//パンチ入力
  {

    //ハンドルをパンチ
    if (stHuku_.panchOK_f_ && (stHuku_.punch_c_ < pattern_.size()))
    {
#ifdef __HUKUBIKI_USE_BTN__
      // ボタン押したアニメ
      btn_->g_SetPartNo(1);
#endif // #ifdef __HUKUBIKI_USE_BTN__

      ugkantoku_->SetMirror(TRUE);
      stHuku_.panchOK_f_ = FALSE;

      //現在のパターン
      stHuku_.nowrarity_ = rarity_[stHuku_.punch_c_];
      stHuku_.nowpattern_ = pattern_[stHuku_.punch_c_];

#ifdef __K_DEBUG_PAD_MODE__
      stHuku_.nowpattern_ = mid::midGetRandMn(100);
#endif

      stHuku_.nowpattern2_ = (stHuku_.nowpattern_ % 10);

      //通常パンチ
      if (stHuku_.nowpattern_ == 0)
      {
        stHuku_.panchtype_ = pt_Lv0;
        ugkantoku_->SetMType(kmt_Punch);//パンチ
        pmgEO_->mgSound_.PlaySYS(se_punch);
      }
      else//大玉出現パンチ
      {
        stHuku_.panchtype_ = pt_LvN;
        stepR_ = rs_CrankFallInit;
      }

      ++stHuku_.punch_c_;
  }
    //巨大カプセルをパンチ
    else if (stHuku_.bigpanchOK_f_ && (stHuku_.bigcuphp_ > 0))
    {
#ifdef __HUKUBIKI_USE_BTN__
      // ボタン押したアニメ
      btn_->g_SetPartNo(1);
#endif // #ifdef __HUKUBIKI_USE_BTN__

      stHuku_.bigpanchOK_f_ = FALSE;

      //通常パンチ
      --stHuku_.bigcuphp_;
      if (stHuku_.bigcuphp_ == 0)
      {
        stHuku_.panchtype_ = pt_BigCount;
        s32 nextptn = (stHuku_.nowpattern_ / 10);
        stHuku_.panchtype_ = (nextptn == 0)//ここでおわり
          ? pt_BigCount
          : pt_BigNext;//次段階
        stHuku_.nowpattern_ = nextptn;
        stHuku_.nowpattern2_ = (stHuku_.nowpattern_ % 10);

        ugkantoku_->SetMType(kmt_Upper);//アッパー
        //pmgEO_->mgSound_.PlaySYS(se_tame);
        pmgEO_->mgSound_.PlaySYS(se_punch);

        if (stHuku_.panchtype_ == pt_BigNext)
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
        stHuku_.panchtype_ = pt_BigCount;

        ugkantoku_->SetMType(kmt_Punch);//パンチ
        pmgEO_->mgSound_.PlaySYS(se_punch);
      }

}
  }

  void TUGHuku::Mv_FallHuku()//ガチャ落ちてくる
  {
    s32 gposy = 0;
    s32 kposy = 0;
    s32 bposy = 0;

    if ((stHuku_.fallstep_ == fs_Huku)
      || (stHuku_.fallstep_ == fs_kantoku)
      || (stHuku_.fallstep_ == fs_big)
      || (stHuku_.fallstep_ == fs_bigbound)
      )
    {
      s32 GRV = (stHuku_.fallstep_ == fs_Huku)
        ? 7
        : 10;
      if (stHuku_.posY_ > 0)
      {
        stHuku_.posY_ += stHuku_.dY_;
        stHuku_.dY_ -= GRV;
        //着地
        if (stHuku_.posY_ <= 0)
        {
          stHuku_.posY_ = 0;
          stHuku_.dY_ = 0;
          switch (stHuku_.fallstep_)
          {
          case fs_Huku:
            stHuku_.fallstep_ = fs_kantoku;
            stHuku_.posY_ = 1;
            stHuku_.dY_ = 100;
            pmgEO_->mgSound_.PlaySYS(se_Hukubound);
            break;
          case fs_kantoku:
            stHuku_.fallstep_ = fs_end;
            stHuku_.panchOK_f_ = TRUE;
            pmgEO_->mgNSF_.Play(nsf_gacha);//BGM
            break;
          case fs_big:
            stHuku_.fallstep_ = fs_bigbound;
            stHuku_.posY_ = 1;
            stHuku_.dY_ = 150;
            ugkantoku_->SetMirror(FALSE);
            pmgEO_->mgSound_.PlaySYS(se_bigbound);
            break;
          case fs_bigbound:
            stHuku_.fallstep_ = fs_end;
            stHuku_.bigpanchOK_f_ = TRUE;
            pmgEO_->mgSound_.PlaySYS(se_bigbound);
            break;
          }
        }
      }

      s32 posy = -(stHuku_.posY_ / XYMAG);
      switch (stHuku_.fallstep_)
      {
      case fs_Huku:gposy = posy; break;
      case fs_kantoku:kposy = posy; break;
      case fs_big:
      case fs_bigbound:
        bposy = posy;
        break;
      }
    }

    Huku_->g_SetGrpPos(0, gposy);
    handle_->g_SetGrpPos(0, gposy);
    crank_->g_SetGrpPos(0, gposy);
    bigcapsule_->g_SetGrpPos(0, bposy);
    hibi_->g_SetGrpPos(0, bposy);
    ugkantoku_->GetField()->c_SetPos(0, kposy);
    ugcap_->GetField()->c_SetPos(0 + 26, kposy);
    ugcleak_->GetField()->c_SetPos(0, kposy);
  }

  void TUGHuku::SetCapsule(s32 rarity, BOOL bigopen_f)//カプセル出現
  {
    s32 posx = (bigopen_f)
      ? 96
      : 64;

    capsule_[stHuku_.nextcap_].Init();
    capsule_[stHuku_.nextcap_].obj_->g_SetPartNo(GACHACUPOFS + rarity);
    //capsule_[stHuku_.nextcap_].obj_->a_SetAniNo(rarity, TRUE);
    capsule_[stHuku_.nextcap_].obj_->g_SetDraw(TRUE);
    capsule_[stHuku_.nextcap_].obj_->z_SetPos(posx, 52);
    capsule_[stHuku_.nextcap_].obj_->z_MoveSetspd(160, 52, 1);
    lib_num::IncLoop(&stHuku_.nextcap_, 0, (CAPNUM - 1));

    //最後のパンチが終わってそのカプセル
    if (stHuku_.punch_c_ == pattern_.size())
    {
      stHuku_.Hukuout_f_ = TRUE;
      stHuku_.out_c_ = 0;
      pmgEO_->mgNSF_.Play(nsf_shop_huku);//BGM

#ifdef __HUKUBIKI_USE_BTN__
      btn_->g_SetDraw(FALSE);
#endif // #ifdef __HUKUBIKI_USE_BTN__
    }
  }
  void TUGHuku::SetBigCapsule(s32 rarity)//巨大カプセル出現
  {
    bigcapsule_->g_SetPartNo(rarity + RARITYNUM);//ガチャカプセル分オフセット
    bigcapsule_->g_EfReset();
    bigcapsule_->g_SetARGB(FALSE, 0xFF, 0xFF, 0xFF, 0xFF);
    bigcapsule_->g_SetDraw(TRUE);
    hibi_->g_SetDraw(FALSE);
    stHuku_.posY_ = TOPPOSY * XYMAG;
    stHuku_.dY_ = 0;
    stHuku_.fallstep_ = fs_big;
    stHuku_.bigbreak_c_ = 0;

    s32 pos = -(stHuku_.posY_ / XYMAG);
    bigcapsule_->g_SetGrpPos(0, pos);
    hibi_->g_SetGrpPos(0, pos);
    stHuku_.bigpanchOK_f_ = FALSE;

    stHuku_.bigcuprarity_ = rarity;
    stHuku_.bigcuphp_ = CUPHP;//割れるまでの回数
    stHuku_.bigcuphpmax_ = stHuku_.bigcuphp_;
  }
  void TUGHuku::Mv_Punch()//パンチ
  {
    if (stHuku_.panchtype_ == pt_Lv0)
    {
      Mv_PunchN();
    }
    else if (stHuku_.panchtype_ == pt_LvN)
    {
      Mv_PunchR();
    }
  }
  // ノーマル版パンチ
  void TUGHuku::Mv_PunchN()
  {
    //パンチヒットタイミング
    if (ugkantoku_->IsActRes(enUGChGenAct_ActPoint))
    {
      //連続カウンタ
      pmgEO_->mgFont_.OrderDamFontSpriteNocam(stHuku_.punch_c_, -102, -28, (base::enFontCol2)FC2_normal);

      // 筐体アニメの初速セット
      hukuAnimSpd_ = 120;
      hukuAnimInterval_ = 5;
      hukuAnimNo_ = 0;

      ++stHuku_.handle_c_;
      if (stHuku_.nowpattern2_ > 0)//演出時はフラッシュ
      {
        pmgEO_->mgSound_.PlaySYS(se_kimehit);

        stHuku_.flash_c_ = FLASHITV * 1;
        if (stHuku_.nowpattern2_ == 1)
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
    else if (stHuku_.handle_c_ > 0)
    {
      ++stHuku_.handle_c_;
      const s32 HDLANIITV = 2;

      s32 BASENO = HUKUPARTNO * HDLANINUM;

      //一周回った
      if (stHuku_.handle_c_ > (HDLANIITV * HDLANINUM))
      {
        stHuku_.handle_c_ = 0;
        handle_->g_SetPartNo(BASENO);//レバー

        //通常カプセル出現
        SetCapsule(stHuku_.nowrarity_, FALSE);
        stHuku_.panchOK_f_ = TRUE;

        //パンチ終了
        stHuku_.panchtype_ = pt_None;
      }
      else
      {
        s32 hdlanino = ((stHuku_.handle_c_ / HDLANIITV) % HDLANINUM);
        handle_->g_SetPartNo(BASENO + hdlanino);//レバー
      }
    }
  }
  //==================================================================
  // レア版パンチ
  //==================================================================
  void TUGHuku::Mv_PunchR()
  {
    // 演出
    switch (stepR_)
    {
      // 何もしない
    default:
    case rs_Invalid:
      break;

      //-------------------------------------------------------------
      // クランクが手前に落ちてくる演出準備
    case rs_CrankFallInit:
      stHuku_.handle_c_ = 56;

      pmgEO_->mgNSF_.StopBGM();
      ugkantoku_->SetMType(kmt_TameUpper);//アッパー
      pmgEO_->mgSound_.PlaySYS(se_tame);
      pmgEO_->mgSound_.PlaySYS(se_todo);
      //pmgEO_->mgSound_.PlaySYS(se_upper);
      stepR_ = rs_Punch;
      break;

      //-------------------------------------------------------------
      // 監督のアッパーモーション中のアクションポイント
    case rs_Punch:
      //パンチヒットタイミング
      if (ugkantoku_->IsActRes(enUGChGenAct_ActPoint))
      {
        //連続カウンタ
        pmgEO_->mgFont_.OrderDamFontSpriteNocam(stHuku_.punch_c_, -102, -28, (base::enFontCol2)FC2_normal);

        // 筐体アニメの初速セット
        hukuAnimSpd_ = 120;
        hukuAnimInterval_ = 5;
        hukuAnimNo_ = 0;

        stHuku_.handle_c_ = 0;
        if (stHuku_.nowpattern2_ > 0)//演出時はフラッシュ
        {
          pmgEO_->mgSound_.PlaySYS(se_kimehit);

          stHuku_.flash_c_ = FLASHITV * 1;
          if (stHuku_.nowpattern2_ == 1)
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
        stepR_ = rs_CrankMove;
      }
      // クランクが落ちてくる
      else
      {
        stHuku_.handle_c_--;
        if (stHuku_.handle_c_ < 2)
        {
          //stepR_ = rs_PunchInit;
        }
        else
        {
          // 各フレーム数に該当するアニメ番号をセット
          s32 animNo = 0;
          if (stHuku_.handle_c_ > 27)
          {
            animNo = 5;
          }
          else if (stHuku_.handle_c_ > 14)
          {
            animNo = 4;
          }
          else if (stHuku_.handle_c_ > 4)
          {
            animNo = 3;
          }
          else
          {
            animNo = 2;
          }
          crank_->g_SetPartNo(animNo);
        }
      }
      break;

      //-------------------------------------------------------------
      // 回転演出
    case rs_CrankMove:
      ++stHuku_.handle_c_;
      //一周回った
      if (stHuku_.handle_c_ > 20)
      {
        stHuku_.handle_c_ = 0;
        crank_->g_SetPartNo(5);
        stepR_ = rs_FlyCapsuleInit;
      }
      else
      {
        s32 animNo = stHuku_.handle_c_ / 2 + 1;
        if (animNo >= 10){ animNo = 0; }
        crank_->g_SetPartNo(animNo);
      }
      break;

      //-------------------------------------------------------------
      // カプセル飛んでく演出準備
    case rs_FlyCapsuleInit:
      // とりあえずベタ書きで
      capsuleEx_.Init();
      //capsuleEx_.obj_->a_SetAniNo(3, TRUE
      capsuleEx_.obj_->g_SetPartNo(GACHACUPOFS + 3);// , TRUE);
      capsuleEx_.obj_->g_SetDraw(TRUE);
      capsuleEx_.posY_ = 30;
      capsuleEx_.obj_->z_SetPos(50, 30);
      stepR_ = rs_FlyCapsule;
      break;

      //-------------------------------------------------------------
      // カプセル飛んでく演出
    case rs_FlyCapsule:
      capsuleEx_.posY_ -= 4;
      const s32 LIMY = -60;
      if (capsuleEx_.posY_ < LIMY)
      {
        //巨大カプセル出現
        SetBigCapsule(3);//金から
        //演出によるBGM

        //パンチ終了
        stHuku_.panchtype_ = pt_None;

        stepR_ = rs_Invalid;
      }
      else
      {
        capsuleEx_.obj_->z_SetPos(50, capsuleEx_.posY_);
      }
      break;
    }
  }


  void TUGHuku::Mv_BigPunch()//巨大カプセルパンチ
  {
    if ((stHuku_.panchtype_ == pt_BigCount)
      || (stHuku_.panchtype_ == pt_BigNext))
    {
      //パンチヒットタイミング
      if (ugkantoku_->IsActRes(enUGChGenAct_ActPoint))
      {
        stHuku_.bigpanchOK_f_ = TRUE;

        //連続カウンタ
        pmgEO_->mgFont_.OrderDamFontSpriteNocam(stHuku_.bigcuphp_, -62, -38, (base::enFontCol2)FC2_normal);

        //割れる
        if (stHuku_.bigcuphp_ == 0)
        {
          BOOL next_f = (stHuku_.nowpattern2_ != 0);
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
          ++stHuku_.bigbreak_c_;
        }
        else//ひび割れ
        {
          pmgEO_->mgSound_.PlaySYS(se_punchhit);

          if (stHuku_.bigcuphp_ == ((stHuku_.bigcuphpmax_ * 2) / 3))
          {
            hibi_->g_SetPartNo(0);
            hibi_->g_SetDraw(TRUE);
          }
          else if (stHuku_.bigcuphp_ == ((stHuku_.bigcuphpmax_ * 1) / 3))
          {
            hibi_->g_SetPartNo(1);
          }
        }
      }
      else if (stHuku_.bigbreak_c_ > 0)
      {
        ++stHuku_.bigbreak_c_;

        BOOL next_f = (stHuku_.nowpattern2_ != 0);

        if (next_f)
        {
          s32 bgm = (stHuku_.bigcuprarity_ - 3);// *2;
          switch (stHuku_.bigbreak_c_)
          {
          case 2:

            bigcapsule_->g_Flash();
            hibi_->g_Flash();

            stHuku_.fallstep_ = fs_big;
            stHuku_.posY_ = 1;
            stHuku_.dY_ = 200;
            //演出時はフラッシュ
            stHuku_.flash_c_ = FLASHITV * 2;
            if (stHuku_.nowpattern2_ == 1)
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
            stHuku_.bigcuprarity_ += 1;
            bigcapsule_->g_SetPartNo(stHuku_.bigcuprarity_ + RARITYNUM);//ガチャカプセル分オフセット
            hibi_->g_SetDraw(FALSE);
            stHuku_.bigbreak_c_ = 0;
            stHuku_.bigpanchOK_f_ = FALSE;

            stHuku_.bigcuphp_ = (stHuku_.bigcuphpmax_ + CUPHP);//割れるまでの回数
            stHuku_.bigcuphpmax_ = stHuku_.bigcuphp_;
            break;
          }
        }
        else
        {
          //ここでおわり
          switch (stHuku_.bigbreak_c_)
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
            SetCapsule(stHuku_.nowrarity_, TRUE);

            break;
          case 24:
            pmgEO_->mgNSF_.Play(nsf_gacha);//BGM
            ugkantoku_->SetMirror(TRUE);
            stHuku_.panchOK_f_ = TRUE;
            stHuku_.panchtype_ = pt_None;
            break;
          }
        }
      }
    }
  }

  void TUGHuku::Mv_Capsule()//カプセル
  {
    for (s32 i = 0; i < CAPNUM; ++i)
    {
      if (capsule_[i].obj_->z_IsStop())
      {
        capsule_[i].obj_->g_SetDraw(FALSE);
        ++stHuku_.cupstop_c_;

        //if (stHuku_.cupstop_c_ >= pattern_.size())
        //{
        //  SetAct(enUGHukuAct_End);
        //}

        SetAct(enUGHukuAct_CupsuleOut, (stHuku_.cupstop_c_ - 1));
      }
    }
  }
  void TUGHuku::Mv_OutHuku()//ガチャはける
  {
    if (stHuku_.Hukuout_f_)
    {

      ++stHuku_.out_c_;
      if (stHuku_.out_c_ == 60)
      {
        SetAct(enUGHukuAct_End);
      }

      s32 posy = -stHuku_.out_c_ / 2;
      Huku_->g_SetGrpPos(0, posy);
      handle_->g_SetGrpPos(0, posy);
      crank_->g_SetGrpPos(0, posy);
      if (posy < -TOPPOSY)
      {
        stHuku_.Hukuout_f_ = FALSE;
        Huku_->g_SetDraw(FALSE);
      }
    }
  }
  void TUGHuku::Mv_Flash()//フラッシュ
  {
    if (stHuku_.flash_c_ > 0)
    {
      if (stHuku_.flash_c_ % FLASHITV == 0)
      {
        ugfilter_.Flash(stHuku_.nowpattern2_ == 2);
      }
      --stHuku_.flash_c_;
    }
  }

  void TUGHuku::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
#ifdef __HUKUBIKI_USE_BTN__
    btn_->Update(draw_f);	// ボタン
#endif // #ifdef __HUKUBIKI_USE_BTN__
    Huku_->Update(draw_f);//ガチャ筐体
    handle_->Update(draw_f);//レバー
    crank_->Update(draw_f);	// クランク
    exmark_->Update(draw_f);//びっくりマーク
    hibi_->Update(draw_f);//大玉のひび
    for (s32 i = 0; i < CAPNUM; ++i) capsule_[i].obj_->Update(draw_f);//カプセル
    capsuleEx_.obj_->Update(draw_f);
    bigcapsule_->Update(draw_f);//大玉カプセル
    ugfilter_.Update(draw_f);
  }

  void TUGHuku::DoAction()
  {
#ifdef __HUKUBIKI_USE_BTN__
    // ボタン離したアニメ
    btn_->g_SetPartNo(0);
#endif // #ifdef __HUKUBIKI_USE_BTN__

    Mv_FallHuku();
    Mv_Punch();
    Mv_BigPunch();
    Mv_Capsule();
    Mv_OutHuku();
    Mv_Flash();
    //Mv_Crank();
    Mv_Huku();
  }

  void TUGHuku::Mv_Huku(void)
  {
    if (hukuAnimSpd_ > 0)
    {
      hukuAnimSpd_--;
      if (hukuAnimSpd_ != 0)
      {
        if (hukuAnimSpd_ % hukuAnimInterval_ == 0)
        {
          if (hukuAnimNo_ == 0) { hukuAnimNo_ = 1; }
          else{ hukuAnimNo_ = 0; }
          hukuAnimInterval_++;
        }
      }
      else
      {
        hukuAnimNo_ = 0;
      }
      Huku_->g_SetPartNo(HUKUPARTNO + hukuAnimNo_);
    }
  }

  BOOL TUGHuku::IsActRes(enUGHukuAct res)//返値ゲット
  {
    return (GetActRes() == res);
  }

}
