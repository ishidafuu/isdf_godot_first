#include "dbMgGrp.h"
#include "dbMgEngineObj.h"

namespace db
{

  const s32 TEX1024 = 1024;
  const s32 TEX256 = 256;
  const s32 TEXPTSIZE = 256;//区画サイズ
  const s32 TEXPTNUM = (TEX1024 / TEXPTSIZE);//区画一辺の数

  const s32 TEXPTSIZE2 = TEXPTSIZE / 2;//区画サイズ
  const s32 TEXPTNUM2 = 2;//区画一辺の数


  TMgGrp::TMgGrp(TMgEngineObj* pmgEO)
    : pmgEO_(pmgEO)//, pdtPal_(&pmgEO->dtPal_), pmgOrder_(&pmgEO->mgOrder_)
  {
    mid::midLog("TMgGrp::TMgGrp");
    stMG_.Init();
  }

  TMgGrp::~TMgGrp()
  {

    Freemem(pgr_Game_);

    //キャラ
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i3 = 0; i3 < DBMEMBER_ALL; ++i3)
      {
        Freemem(pgr_Body_[i][i3]);
        Freemem(pgr_Face_[i][i3]);
        Freemem(pgr_Hair_[i][i3]);
        Freemem(pgr_IconHead_[i][i3]);
        Freemem(pmgChGrp_[i][i3]);

      }
      Freemem(pgr_IconChest_[i]);
      Freemem(pgr_IconBody_[i]);
      Freemem(pgr_IconChestSub_[i]);
      Freemem(pgr_IconBodySub_[i]);
    }
    for (s32 i = 0; i < RANKERNUM; i++)
    {
      Freemem(pmgChRankerGrp_);
      Freemem(pgr_IconRankerChest_[i]);
      Freemem(pgr_IconRankerChestSub_[i]);
    }
    Freemem(pgr_IconBodySh_);

    Freemem(pmgIcGrp_);//アイコン
    Freemem(pmgEtcGrp_);//アイテム
    //Freemem(pmgEtcGrp_);//アイテム

    //for (s32 i3 = 0; i3 < ICON_FACENUM; ++i3) Freemem(pgr_IconFace_[i3]);

    //for (s32 i3 = 0; i3 < ICON_HAIRNUM; ++i3) Freemem(pgr_IconHair_[i3]);

    for (s32 i = 0; i < BALLSPNUM; ++i) Freemem(pgr_Ball_[i]);//ボール

    //for (s32 i = 0; i < ICON_MOUSENUM; ++i) Freemem(pgr_IconMouse_[i]);

    Freemem(pgr_Court_);//コート
    Freemem(pgr_Referee_);//レフェリー

    Freemem(pgr_Girl_);
    Freemem(pgr_Clerk_);
    Freemem(pgr_Kantoku_);
    Freemem(pgr_KantokuFace_);
    Freemem(pgr_KantokuHair_);

    Freemem(pgr_Captain_);
    Freemem(pgr_CaptainFace_);
    Freemem(pgr_CaptainHair_);

    for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
    {
      Freemem(pgr_Sensyu_[i2]);
      Freemem(pgr_SensyuFace_[i2]);
      Freemem(pgr_SensyuHair_[i2]);
    }

    Freemem(pgr_Enemy_);
    Freemem(pgr_EnemyFace_);
    Freemem(pgr_EnemyHair_);

    for (s32 i = 0; i < PLAYERNUM; i++)
    {
      Freemem(pgr_Enemies_[i]);
      Freemem(pgr_EnemiesFace_[i]);
      Freemem(pgr_EnemiesHair_[i]);

      Freemem(pgr_Captains_[i]);
      Freemem(pgr_CaptainsFace_[i]);
      Freemem(pgr_CaptainsHair_[i]);
    }

    for (std::map<s32, base::TGrTile*>::iterator itr = pgr_Person_.begin(); itr != pgr_Person_.end(); itr++){ Freemem(itr->second); }
    for (std::map<s32, base::TGrTile2*>::iterator itr = pgr_PersonFace_.begin(); itr != pgr_PersonFace_.end(); itr++){ Freemem(itr->second); }
    for (std::map<s32, base::TGrTile2*>::iterator itr = pgr_PersonHair_.begin(); itr != pgr_PersonHair_.end(); itr++){ Freemem(itr->second); }
    pgr_Person_.clear();
    pgr_PersonFace_.clear();
    pgr_PersonHair_.clear();

    Freemem(pgr_Boss_);
    Freemem(pgr_Zako_);
    ClearBossSt();
    ClearZakoSt();

    //キャラグラフィックマネージャー
    Freemem(pmgcg_kantoku_);
    Freemem(pmgcg_captain_);
    for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
    {
      Freemem(pmgcg_sensyu_[i2]);
    }
    Freemem(pmgcg_clerk_);
    Freemem(pmgcg_girl_);
    Freemem(pmgcg_enemy_);
    for (s32 i = 0; i < PLAYERNUM; i++){ Freemem(pmgcg_enemies_[i]); }
    for (s32 i = 0; i < PLAYERNUM; i++){ Freemem(pmgcg_captains_[i]); }
    for (std::map<s32, TMgCharGrp*>::iterator itr = pmgcg_person_.begin(); itr != pmgcg_person_.end(); itr++){ Freemem(itr->second); }
    pmgcg_person_.clear();
  }

  //ボス描画情報クリア
  void TMgGrp::ClearBossSt()
  {
    for (s32 i = 0; i < stBoss_.size(); ++i) Freemem(stBoss_[i]);
    stBoss_.clear();
  }
  //ボス描画情報追加
  s32 TMgGrp::AddBossSt(s64 mid_char, s64 mid_team)
  {
    s32 res = stBoss_.size();
    TStGrChar* newboss = new TStGrChar(pmgEO_);
    BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_gb_f);
    newboss->SetCharDt(mid_char, mid_team, gb_f);
    stBoss_.push_back(newboss);

    return res;//保存した行番号
  }


  //雑魚描画情報クリア
  void TMgGrp::ClearZakoSt()
  {
    for (s32 i = 0; i < stZako_.size(); ++i) Freemem(stZako_[i]);
    stZako_.clear();
  }
  //雑魚描画情報追加
  s32 TMgGrp::AddZakoSt(s64 mid_char, s64 mid_team)
  {
    s32 res = stZako_.size();
    TStGrChar* newzako = new TStGrChar(pmgEO_);
    BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_gb_f);
    newzako->SetCharDt(mid_char, mid_team, gb_f);
    stZako_.push_back(newzako);

    return res;//保存した行番号
  }


  //画像読み込み&管理クラス作成
  void TMgGrp::LoadBmpCntn()
  {
    base::TMgOrder* pmgOrder = &(pmgEO_->mgOrder_);

    //画像の読み込み＆HLS基礎パレット作成
    cntn_.LoadBmpAll();

    //統合テクスチャ原本
    pgr_Game_ = new base::TGrp(&(pmgEO_->mgOrder_), &(cntn_.cntnBO_.bmo_Game_));

    //新かおづら
    for (s32 i2 = 0; i2 < DBSIDE; ++i2)
    {
      for (s32 i3 = 0; i3 < DBMEMBER_ALL; ++i3)
      {
        //統合テクスチャ      
        pgr_Body_[i2][i3] = new base::TGrTile(pmgOrder, &(cntn_.cntnBO_.bmo_Game_), BMP_BODY);//タイル読み込み
        pgr_Face_[i2][i3] = new base::TGrTile2(pmgOrder, &(cntn_.cntnBO_.bmo_Game_));
        pgr_Hair_[i2][i3] = new base::TGrTile2(pmgOrder, &(cntn_.cntnBO_.bmo_Game_));
        pgr_IconHead_[i2][i3] = new base::TGrTile2(pmgOrder, &(cntn_.cntnBO_.bmo_Game_));
      }

      pgr_IconChest_[i2] = new base::TGrPart(pmgOrder, &(cntn_.cntnBO_.bmo_IconChest_[i2]), CHESTW, CHESTH);
      pgr_IconBody_[i2] = new base::TGrPart(pmgOrder, &(cntn_.cntnBO_.bmo_IconBody_[i2]), BODYW, BODYH);
      pgr_IconChestSub_[i2] = new base::TGrPart(pmgOrder, &(cntn_.cntnBO_.bmo_IconChestSub_[i2]), CHESTW, CHESTH);
      pgr_IconBodySub_[i2] = new base::TGrPart(pmgOrder, &(cntn_.cntnBO_.bmo_IconBodySub_[i2]), BODYW, BODYH);
    }
    for (s32 i = 0; i < RANKERNUM; i++)
    {
      pgr_IconRankerChest_[i] = new base::TGrPart(pmgOrder, &(cntn_.cntnBO_.bmo_IconRankerChest_[i]), CHESTW, CHESTH);
      pgr_IconRankerChestSub_[i] = new base::TGrPart(pmgOrder, &(cntn_.cntnBO_.bmo_IconRankerChestSub_[i]), CHESTW, CHESTH);
    }
    pgr_IconBodySh_ = new base::TGrPart(pmgOrder, &(cntn_.cntnBO_.bmo_IconBodySh_), BODYW, BODYW);//32x32

    pgr_Boss_ = new base::TGrTile2(pmgOrder, &(cntn_.cntnBO_.bmo_Boss_));
    pgr_Zako_ = new base::TGrTile2(pmgOrder, &(cntn_.cntnBO_.bmo_Zako_));

    //コート
    pgr_Court_ = new base::TGrBG(pmgOrder, &(cntn_.cntnBO_.bmo_Game_), &pmgEO_->mgCamera_);
    //ボール
    for (s32 i = 0; i < BALLSPNUM; ++i)
    {
      pgr_Ball_[i] = new base::TGrTile(pmgOrder, &(cntn_.cntnBO_.bmo_Game_), BMP_BALL);
    }
    //レフェリー
    pgr_Referee_ = new base::TGrTile(pmgOrder, &(cntn_.cntnBO_.bmo_Game_), BMP_REFEREE);

    //アイテム描画マネージャー
    pmgEtcGrp_ = new TMgEtcGrp(pmgOrder, &(cntn_.cntnBO_.bmo_Game_), BMP_ETC);//タイル読み込み
    //アイテム描画マネージャー
    //pmgEtcGrp_ = new TMgEtcGrp(pmgOrder, &(cntn_.cntnBO_.bmo_Game_), BMP_Etc);//タイル読み込み

//    //アイコン
//    for (s32 i3 = 0; i3 < ICON_FACENUM; ++i3)
//    {
//      pgr_IconFace_[i3] = new base::TGrPart(pmgOrder, &(cntn_.cntnBO_.bmo_IconFace_[i3]), FACEW, FACEH);
//    }
//    for (s32 i3 = 0; i3 < ICON_HAIRNUM; ++i3)
//    {
//      pgr_IconHair_[i3] = new base::TGrPart(pmgOrder, &(cntn_.cntnBO_.bmo_IconHair_[i3]), HAIRW, HAIRH);
//    }
//    for (s32 i3 = 0; i3 < ICON_MOUSENUM; ++i3)
//    {
//      pgr_IconMouse_[i3] = new base::TGrPart(pmgOrder, &(cntn_.cntnBO_.bmo_IconMouse_[i3]), MOUSEW, MOUSEH);
//    }

    //おみせやさん
    pgr_Clerk_ = new base::TGrTile(pmgOrder, &(cntn_.cntnBO_.bmo_Clerk_), BMP_CLERK);
    //女子高生
    pgr_Girl_ = new base::TGrTile(pmgOrder, &(cntn_.cntnBO_.bmo_Girl_), BMP_GIRL);

    pgr_Kantoku_ = new base::TGrTile(pmgOrder, &(cntn_.cntnBO_.bmo_Kantoku_), BMP_KANTOKU);
    pgr_KantokuFace_ = new base::TGrTile2(pmgOrder, &(cntn_.cntnBO_.bmo_KantokuFace_));
    pgr_KantokuHair_ = new base::TGrTile2(pmgOrder, &(cntn_.cntnBO_.bmo_KantokuHair_));

    pgr_Captain_ = new base::TGrTile(pmgOrder, &(cntn_.cntnBO_.bmo_Captain_), BMP_UIBODY);
    pgr_CaptainFace_ = new base::TGrTile2(pmgOrder, &(cntn_.cntnBO_.bmo_CaptainFace_));
    pgr_CaptainHair_ = new base::TGrTile2(pmgOrder, &(cntn_.cntnBO_.bmo_CaptainHair_));

    for (s32 i3 = 0; i3 < DBMEMBER_ALL; ++i3)
    {
      pgr_Sensyu_[i3] = new base::TGrTile(pmgOrder, &(cntn_.cntnBO_.bmo_Sensyu_[i3]), BMP_UIBODY);
      pgr_SensyuFace_[i3] = new base::TGrTile2(pmgOrder, &(cntn_.cntnBO_.bmo_SensyuFace_[i3]));
      pgr_SensyuHair_[i3] = new base::TGrTile2(pmgOrder, &(cntn_.cntnBO_.bmo_SensyuHair_[i3]));
    }

    pgr_Enemy_ = new base::TGrTile(pmgOrder, &(cntn_.cntnBO_.bmo_Enemy_), BMP_KANTOKU);
    pgr_EnemyFace_ = new base::TGrTile2(pmgOrder, &(cntn_.cntnBO_.bmo_EnemyFace_));
    pgr_EnemyHair_ = new base::TGrTile2(pmgOrder, &(cntn_.cntnBO_.bmo_EnemyHair_));

    for (s32 i = 0; i < PLAYERNUM; i++)
    {
      pgr_Enemies_[i] = new base::TGrTile(pmgOrder, &(cntn_.cntnBO_.bmo_Enemies_[i]), BMP_KANTOKU);
      pgr_EnemiesFace_[i] = new base::TGrTile2(pmgOrder, &(cntn_.cntnBO_.bmo_EnemiesFace_[i]));
      pgr_EnemiesHair_[i] = new base::TGrTile2(pmgOrder, &(cntn_.cntnBO_.bmo_EnemiesHair_[i]));

      pgr_Captains_[i] = new base::TGrTile(pmgOrder, &(cntn_.cntnBO_.bmo_Captains_[i]), BMP_UIBODY);
      pgr_CaptainsFace_[i] = new base::TGrTile2(pmgOrder, &(cntn_.cntnBO_.bmo_CaptainsFace_[i]));
      pgr_CaptainsHair_[i] = new base::TGrTile2(pmgOrder, &(cntn_.cntnBO_.bmo_CaptainsHair_[i]));
    }

    //キャラ描画マネージャー
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
      {
        pmgChGrp_[i][i2] = new TMgCharGrp(pmgEO_);
      }

    }
    for (s32 i = 0; i < RANKERNUM; i++)
    {
      pmgChRankerGrp_[i] = new TMgCharGrp(pmgEO_);
    }
    //アイコン描画マネージャー
    pmgIcGrp_ = new TMgIconGrp(pmgEO_);

    //キャラグラフィックマネージャー
    pmgcg_kantoku_ = new  TMgCharGrp(pmgEO_);//一人
    pmgcg_captain_ = new TMgCharGrp(pmgEO_);
    for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
    {
      pmgcg_sensyu_[i2] = new  TMgCharGrp(pmgEO_);
    }
    pmgcg_clerk_ = new  TMgCharGrp(pmgEO_);//１枚に全員分入ってるので人数分不要
    pmgcg_girl_ = new  TMgCharGrp(pmgEO_);
    //店と女子高生はここでセットで現状大丈夫
    pmgcg_clerk_->SetGrTile(pgr_Clerk_);
    pmgcg_girl_->SetGrTile(pgr_Girl_);
    pmgcg_enemy_ = new  TMgCharGrp(pmgEO_);//一人
    for (s32 i = 0; i < PLAYERNUM; i++)
    {
      pmgcg_enemies_[i] = new TMgCharGrp(pmgEO_);	// 複数
      pmgcg_captains_[i] = new TMgCharGrp(pmgEO_);	// 複数
    }
  }

  //準備
  void TMgGrp::Ready()
  {
    //全かおづらアイコン作成
    //MakeIconTexture();
  }

  void TMgGrp::LoadCourt(s64 mid_court)
  {
    std::string courtname = "c_" + pmgEO_->mgDt_.dtCourt_.GetStrPK(mid_court, enDtCourtStr_filename);
    cntn_.LoadCourt(courtname.c_str());
  }

  //全アイコン作成
  void TMgGrp::MakeIconTexture()
  {
    //かおづら
    MakeIconTexture_Kaozura();
    //顎ファイル番号
    MakeIconTexture_Ago();
  }

  //かおづら
  void TMgGrp::MakeIconTexture_Kaozura()
  {
    lib_rgb::RGBPAL pal;
    mid::TParamBltBufFromPng bbfp;
    //顔
    const s32 TEXSIZE128 = 128;
    const s32 DFXNUM = (TEXSIZE128 / FACEW);
    const s32 DFYNUM = (TEXSIZE128 / FACEH);
    const s32 SFYNUM = cntn_.cntnPng_.png_Face_[0].height_ / FACEH;
    const s32 DHXNUM = (TEXSIZE128 / HAIRW);
    const s32 DHYNUM = (TEXSIZE128 / HAIRH);
    const s32 SHYNUM = cntn_.cntnPng_.png_Hair_[0].height_ / HAIRH;

    void* facebuf[ICON_FACENUM];
    for (s32 i = 0; i < ICON_FACENUM; ++i)
    {
      facebuf[i] = mid::midAlloc2(TEXSIZE128 * TEXSIZE128 * mid::TEXFMT24);
    }

    void* hairbuf[ICON_HAIRNUM];
    for (s32 i = 0; i < ICON_HAIRNUM; ++i)
    {
      hairbuf[i] = mid::midAlloc2(TEXSIZE128 * TEXSIZE128 * mid::TEXFMT24);
    }

    base::TCharPal cpal;
    cpal.Init();
    //pmgEO_->dtPal_.SetPal(pal, skinHue, hairHue, hairLum, acceHue, acceLum, eyesHue, eyesLum);

    for (s32 i = 0; i < pmgEO_->mgDt_.dtChar_.GetLineNum(); ++i)
    {
      //パレットに違いがある
      if ((cpal.skincol_.col_ != pmgEO_->mgDt_.dtChar_.GetLooks(clk_SkinCol, i))
        || (cpal.haircol_.col_ != pmgEO_->mgDt_.dtChar_.GetLooks(clk_HairCol, i))
        || (cpal.acccol_.col_ != pmgEO_->mgDt_.dtChar_.GetLooks(clk_AcceCol, i))
        || (cpal.eyecol_.col_ != pmgEO_->mgDt_.dtChar_.GetLooks(clk_EyesCol, i))
        )
      {
        cpal.SetEye(pmgEO_->mgDt_.dtChar_.GetLooks(clk_EyesCol, i));
        cpal.SetSkin(pmgEO_->mgDt_.dtChar_.GetLooks(clk_SkinCol, i));
        cpal.SetHair(pmgEO_->mgDt_.dtChar_.GetLooks(clk_HairCol, i));
        cpal.SetAcc(pmgEO_->mgDt_.dtChar_.GetLooks(clk_AcceCol, i));
        pmgEO_->dtPal_.SetPal(pal, &cpal);
      }

      //顔ファイル番号
      {
        //★ここでＩＤからの顔番号を反映させる必要はない
        //全ての顔および髪のアイコンを作成して、
        //描画の段階でＩＤを観て組み合わせる
        //下の試合中アイコンはそれを行う。
        const s32 ffileNo = pmgEO_->mgDt_.dtChar_.GetLooks_FaceFile(i);
        const s32 facesy = (pmgEO_->mgDt_.dtChar_.GetLooks_FaceNo(i)) * FACEH;
        const s32 dbufNo = (i / (DFXNUM * DFYNUM));//出力先
        s32 dposy = (((i / DFXNUM) * FACEH) % TEXSIZE128);
        s32 dposx = ((i % DFXNUM) * FACEW);

        bbfp.Init();
        bbfp.SetSrcParam(&cntn_.cntnPng_.png_Face_[ffileNo], 0, facesy, FACEW, FACEH);
        bbfp.SetDestParam(facebuf[dbufNo], dposx, dposy, TEXSIZE128, TEXSIZE128);
        mid::midBltBufFromPngData(pal, bbfp);
      }

      //髪ファイル番号
      {
        const s32 hfileNo = pmgEO_->mgDt_.dtChar_.GetLooks_ZuraFile(i);
        const s32 hairsy = (pmgEO_->mgDt_.dtChar_.GetLooks_ZuraNo(i)) * HAIRH;
        const s32 dbufNo = (i / (DHXNUM * DHYNUM));//出力先
        s32 dposy = (((i / DHXNUM) * HAIRH) % TEXSIZE128);
        s32 dposx = ((i % DHXNUM) * HAIRW);
        bbfp.Init();
        bbfp.SetSrcParam(&cntn_.cntnPng_.png_Hair_[hfileNo], 0, hairsy, HAIRW, HAIRH);
        bbfp.SetDestParam(hairbuf[dbufNo], dposx, dposy, TEXSIZE128, TEXSIZE128);
        mid::midBltBufFromPngData(pal, bbfp);
      }
    }



    //for (s32 i = 0; i < ICON_FACENUM; ++i)
    //{

    //  //テクスチャ生成し、ハンドル渡す
    //  mid::midMakeTexture24(pgr_IconFace_[i]->pbmo_, facebuf[i], TEXSIZE128, TEXSIZE128);

    //  //バッファ開放
    //  mid::midRelease(facebuf[i]);
    //}

    //for (s32 i = 0; i < ICON_HAIRNUM; ++i)
    //{

    //  //テクスチャ生成し、ハンドル渡す
    //  mid::midMakeTexture24(pgr_IconHair_[i]->pbmo_, hairbuf[i], TEXSIZE128, TEXSIZE128);

    //  //バッファ開放
    //  mid::midRelease(hairbuf[i]);
    //}
  }

  //顎ファイル番号
  void TMgGrp::MakeIconTexture_Ago()
  {
    lib_rgb::RGBPAL pal;
    mid::TParamBltBufFromPng bbfp;

    //const s32 MOUSENUM = base::SKIN_HUENUM + 1;
    const s32 MSW = cntn_.cntnPng_.png_Mouse_.width_;
    const s32 MSH = cntn_.cntnPng_.png_Mouse_.height_;
    const s32 MDW = lib_num::ConvPow2(MSW);
    const s32 MDH = lib_num::ConvPow2(MSH);
    void* mousebuf[ICON_MOUSENUM];

    base::TCharPal cpal;
    cpal.Init();

    //肌の色相の数だけ作る
    for (s32 i = 0; i < ICON_MOUSENUM; ++i)
    {
      mousebuf[i] = mid::midAlloc2(MDW * MDH * mid::TEXFMT24);
    }

    //肌の色相の数だけ作る
    for (s32 i = 0; i < ICON_MOUSENUM; ++i)
    {
      s32 col = ((i / 3) * 10) + (i % 3);
      cpal.SetSkin(i);
      pmgEO_->dtPal_.SetPal(pal, &cpal);

      bbfp.Init();
      bbfp.SetSrcParam(&cntn_.cntnPng_.png_Mouse_, 0, 0, MSW, MSH);
      bbfp.SetDestParam(mousebuf[i], 0, 0, MDW, MDH);
      mid::midBltBufFromPngData(pal, bbfp);
    }

    for (s32 i = 0; i < ICON_MOUSENUM; ++i)
    {

      //テクスチャ生成し、ハンドル渡す
      //mid::midMakeTexture24(pgr_IconMouse_[i]->pbmo_, MDW, MDH, mousebuf[i], MSW, MSH);
      //バッファ開放
      mid::midRelease(mousebuf[i]);
    }
  }


  //UI用チーム情報作成（pmgChGrp_にチーム情報いれておく）
  void TMgGrp::MakeIconTexture_Chest(s32 sideno, BOOL seihuku_f, BOOL sub_f)
  {
    s32 subno = (sub_f) ? 1 : 0;
    stMG_.stchest_.seihuku_f[sideno][subno] = seihuku_f;

    const s32 TEXH = 256;//区画サイズ
    const s32 TEXW = 32;//区画サイズ
    //バッファメモリ
    void* texbuf = mid::midAlloc2((TEXW * TEXH)* mid::TEXFMT24);//0クリア必要

    //区画１行に入る数
    const s32 YMAX = TEXH / CHESTH;

    const s32 HREVX = 0;//胸パーツに合う顔の位置
    const s32 HREVY = 3;//胸パーツに合う顔の位置
    const s32 FREVX = 4;//胸パーツに合う顔の位置
    const s32 FREVY = 4;//胸パーツに合う顔の位置

    s32 SY = (seihuku_f)
      ? CHESTH
      : 0;

    for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
    {
      //づら
      const s32 hairsx = 0;
      const s32 hfileNo = pmgChGrp_[sideno][i2]->GetLooks_ZuraFile();
      const s32 hairsy = pmgChGrp_[sideno][i2]->GetLooks_ZuraNo() * HAIRH;
      const s32 ffileNo = pmgChGrp_[sideno][i2]->GetLooks_FaceFile();
      const s32 facesx = 0;
      const s32 facesy = (pmgChGrp_[sideno][i2]->GetLooks_FaceNo() % XYMAG) * FACEH;

      //胸
      s32 cdx = ((i2 / YMAX) * CHESTW);
      s32 cdy = ((i2 % YMAX) * CHESTH);

      //髪の毛
      s32 hdx = cdx + HREVX;
      s32 hdy = cdy + HREVY;


      //顔
      s32 fdx = cdx + FREVX;
      s32 fdy = cdy + FREVY;


      mid::TParamBltBufFromPng bbfp;

      //胸
      bbfp.Init();
      bbfp.SetSrcParam(&cntn_.cntnPng_.png_Chest_, 0, SY, CHESTW, CHESTH);
      bbfp.SetDestParam(texbuf, cdx, cdy, TEXW, TEXH);
      mid::midBltBufFromPngData(pmgChGrp_[sideno][i2]->stGC_.pal_, bbfp);


      //顔
      bbfp.Init();
      bbfp.SetSrcParam(&cntn_.cntnPng_.png_Face_[ffileNo], facesx, facesy, FACEW, FACEH);
      bbfp.SetDestParam(texbuf, fdx, fdy, TEXW, TEXH);
      bbfp.colkey_f_ = TRUE;
      mid::midBltBufFromPngData(pmgChGrp_[sideno][i2]->stGC_.pal_, bbfp);


      //ずら
      bbfp.Init();
      bbfp.SetSrcParam(&cntn_.cntnPng_.png_Hair_[hfileNo], hairsx, hairsy, HAIRW, HAIRH);
      bbfp.SetDestParam(texbuf, hdx, hdy, TEXW, TEXH);
      bbfp.colkey_f_ = TRUE;
      mid::midBltBufFromPngData(pmgChGrp_[sideno][i2]->stGC_.pal_, bbfp);
    }

    //テクスチャ生成し、ハンドル渡す
    if (sub_f)
    {
      mid::midMakeTexture24(&cntn_.cntnBO_.bmo_IconChestSub_[sideno], texbuf, TEXW, TEXH);
    }
    else
    {
      mid::midMakeTexture24(&cntn_.cntnBO_.bmo_IconChest_[sideno], texbuf, TEXW, TEXH);
    }

    //バッファ開放
    mid::midRelease(texbuf);
  }

  void TMgGrp::MakeIconTexture_BodyCap(s32 sideno, s32 capbmpno)
  {
    MakeIconTexture_Body(sideno, capbmpno, 0, TRUE, FALSE);
  }
  //UI用立ち絵アイコン
  void TMgGrp::MakeIconTexture_Body(s32 sideno, s32 capbmpno, s32 membmpno, BOOL caponly_f, BOOL sub_f)
  {
    s32 subno = (sub_f) ? 1 : 0;
    stMG_.stbody_.caponly_f[sideno][subno] = caponly_f;
    stMG_.stbody_.capbmpno[sideno][subno] = capbmpno;
    stMG_.stbody_.membmpno[sideno][subno] = membmpno;



    s32 TEXH = 512;//区画サイズ
    s32 TEXW = 32;//区画サイズ
    s32 num = DBMEMBER_ALL;
    if (caponly_f)
    {
      TEXH = 64;
      num = 1;
    }
    //バッファメモリ
    void* texbuf = mid::midAlloc2((TEXW * TEXH)* mid::TEXFMT24);//0クリア必要

    //区画１行に入る数
    const s32 YMAX = TEXH / BODYH;
    const s32 HREVX = 4;//ボディパーツに合う顔の位置
    const s32 HREVY = 7;//ボディパーツに合う顔の位置
    const s32 FREVX = 8;//ボディパーツに合う顔の位置
    const s32 FREVY = 8;//ボディパーツに合う顔の位置

    //づら
    for (s32 i2 = 0; i2 < num; ++i2)
    {
      const s32 hairsx = 0;
      const s32 hfileNo = pmgChGrp_[sideno][i2]->GetLooks_ZuraFile();
      const s32 hairsy = pmgChGrp_[sideno][i2]->GetLooks_ZuraNo() * HAIRH;
      const s32 ffileNo = pmgChGrp_[sideno][i2]->GetLooks_FaceFile();
      const s32 facesx = 0;
      const s32 facesy = pmgChGrp_[sideno][i2]->GetLooks_FaceNo() * FACEH;
      const s32 bodysy = (i2 == 0)
        ? (capbmpno * BODYH)
        : (membmpno * BODYH);

      //body
      s32 bdx = ((i2 / YMAX) * BODYW);
      s32 bdy = ((i2 % YMAX) * BODYH);

      //髪の毛
      s32 hdx = bdx + HREVX;
      s32 hdy = bdy + HREVY;


      //顔
      s32 fdx = bdx + FREVX;
      s32 fdy = bdy + FREVY;


      mid::TParamBltBufFromPng bbfp;

      //ボディ
      bbfp.Init();
      bbfp.SetSrcParam(&cntn_.cntnPng_.png_Cap_, 0, bodysy, BODYW, BODYH);
      bbfp.SetDestParam(texbuf, bdx, bdy, TEXW, TEXH);
      mid::midBltBufFromPngData(pmgChGrp_[sideno][i2]->stGC_.pal_, bbfp);


      //顔
      bbfp.Init();
      bbfp.SetSrcParam(&cntn_.cntnPng_.png_Face_[ffileNo], facesx, facesy, FACEW, FACEH);
      bbfp.SetDestParam(texbuf, fdx, fdy, TEXW, TEXH);
      bbfp.colkey_f_ = TRUE;
      mid::midBltBufFromPngData(pmgChGrp_[sideno][i2]->stGC_.pal_, bbfp);


      //ずら
      bbfp.Init();
      bbfp.SetSrcParam(&cntn_.cntnPng_.png_Hair_[hfileNo], hairsx, hairsy, HAIRW, HAIRH);
      bbfp.SetDestParam(texbuf, hdx, hdy, TEXW, TEXH);
      bbfp.colkey_f_ = TRUE;
      mid::midBltBufFromPngData(pmgChGrp_[sideno][i2]->stGC_.pal_, bbfp);

    }

    //テクスチャ生成し、ハンドル渡す
    //テクスチャ生成し、ハンドル渡す
    if (sub_f)
    {
      mid::midMakeTexture24(&cntn_.cntnBO_.bmo_IconBodySub_[sideno], texbuf, TEXW, TEXH);
    }
    else
    {
      mid::midMakeTexture24(&cntn_.cntnBO_.bmo_IconBody_[sideno], texbuf, TEXW, TEXH);
    }

    //バッファ開放
    mid::midRelease(texbuf);
  }
  //UIシュートカード用キャラアイコン
  void TMgGrp::MakeIconTexture_BodySh()
  {
    s32 TEXH = 256;//区画サイズ
    s32 TEXW = 32;//区画サイズ
    s32 SHOTH = 32;
    s32 num = DBMEMBER_ALL;
    //バッファメモリ
    void* texbuf = mid::midAlloc2((TEXW * TEXH)* mid::TEXFMT24);//0クリア必要

    //区画１行に入る数
    const s32 YMAX = TEXH / SHOTH;
    const s32 HREVX = 9;//ボディパーツに合う顔の位置
    const s32 HREVY = 2;//ボディパーツに合う顔の位置
    const s32 FREVX = 13;//ボディパーツに合う顔の位置
    const s32 FREVY = 3;//ボディパーツに合う顔の位置

    //づら
    for (s32 i2 = 0; i2 < num; ++i2)
    {
      const s32 hairsx = 0;
      const s32 hfileNo = pmgChGrp_[SIDE0][i2]->GetLooks_ZuraFile();
      const s32 hairsy = pmgChGrp_[SIDE0][i2]->GetLooks_ZuraNo() * HAIRH;
      const s32 ffileNo = pmgChGrp_[SIDE0][i2]->GetLooks_FaceFile();
      const s32 facesx = 0;
      const s32 facesy = pmgChGrp_[SIDE0][i2]->GetLooks_FaceNo() * FACEH;

      //body
      s32 bdx = ((i2 / YMAX) * BODYW);
      s32 bdy = ((i2 % YMAX) * SHOTH);

      //髪の毛
      s32 hdx = bdx + HREVX;
      s32 hdy = bdy + HREVY;


      //顔
      s32 fdx = bdx + FREVX;
      s32 fdy = bdy + FREVY;


      mid::TParamBltBufFromPng bbfp;

      //ボディ
      bbfp.Init();
      bbfp.SetSrcParam(&cntn_.cntnPng_.png_Shot_, 0, 0, BODYW, SHOTH);
      bbfp.SetDestParam(texbuf, bdx, bdy, TEXW, TEXH);
      mid::midBltBufFromPngData(pmgChGrp_[SIDE0][i2]->stGC_.pal_, bbfp);


      //顔
      bbfp.Init();
      bbfp.SetSrcParam(&cntn_.cntnPng_.png_Face_[ffileNo], facesx, facesy, FACEW, FACEH);
      bbfp.SetDestParam(texbuf, fdx, fdy, TEXW, TEXH);
      bbfp.colkey_f_ = TRUE;
      mid::midBltBufFromPngData(pmgChGrp_[SIDE0][i2]->stGC_.pal_, bbfp);


      //ずら
      bbfp.Init();
      bbfp.SetSrcParam(&cntn_.cntnPng_.png_Hair_[hfileNo], hairsx, hairsy, HAIRW, HAIRH);
      bbfp.SetDestParam(texbuf, hdx, hdy, TEXW, TEXH);
      bbfp.colkey_f_ = TRUE;
      mid::midBltBufFromPngData(pmgChGrp_[SIDE0][i2]->stGC_.pal_, bbfp);

    }

    //テクスチャ生成し、ハンドル渡す
    mid::midMakeTexture24(&cntn_.cntnBO_.bmo_IconBodySh_, texbuf, TEXW, TEXH);
    //バッファ開放
    mid::midRelease(texbuf);
  }

  //UI用チーム情報作成（pmgChGrp_にチーム情報いれておく）
  void TMgGrp::MakeIconRankerTexture_Chest(s32 no, BOOL seihuku_f, BOOL sub_f)
  {
    const s32 TEXH = 32;//区画サイズ
    const s32 TEXW = 32;// 256;//区画サイズ
    //バッファメモリ
    void* texbuf = mid::midAlloc2((TEXW * TEXH)* mid::TEXFMT24);//0クリア必要

    //区画１行に入る数
    const s32 YMAX = 1;// TEXH / CHESTH;

    const s32 HREVX = 0;//胸パーツに合う顔の位置
    const s32 HREVY = 3;//胸パーツに合う顔の位置
    const s32 FREVX = 4;//胸パーツに合う顔の位置
    const s32 FREVY = 4;//胸パーツに合う顔の位置

    s32 SY = (seihuku_f)
      ? CHESTH
      : 0;

    //づら
    const s32 hairsx = 0;
    const s32 hfileNo = pmgChRankerGrp_[no]->GetLooks_ZuraFile();
    const s32 hairsy = pmgChRankerGrp_[no]->GetLooks_ZuraNo() * HAIRH;
    const s32 ffileNo = pmgChRankerGrp_[no]->GetLooks_FaceFile();
    const s32 facesx = 0;
    const s32 facesy = (pmgChRankerGrp_[no]->GetLooks_FaceNo() % XYMAG) * FACEH;

    //胸
    s32 cdx = 0;// ((no / YMAX) * CHESTW);
    s32 cdy = 0;// ((no % YMAX) * CHESTH);

    //髪の毛
    s32 hdx = cdx + HREVX;
    s32 hdy = cdy + HREVY;


    //顔
    s32 fdx = cdx + FREVX;
    s32 fdy = cdy + FREVY;


    mid::TParamBltBufFromPng bbfp;

    //胸
    bbfp.Init();
    bbfp.SetSrcParam(&cntn_.cntnPng_.png_Chest_, 0, SY, CHESTW, CHESTH);
    bbfp.SetDestParam(texbuf, cdx, cdy, TEXW, TEXH);
    mid::midBltBufFromPngData(pmgChRankerGrp_[no]->stGC_.pal_, bbfp);


    //顔
    bbfp.Init();
    bbfp.SetSrcParam(&cntn_.cntnPng_.png_Face_[ffileNo], facesx, facesy, FACEW, FACEH);
    bbfp.SetDestParam(texbuf, fdx, fdy, TEXW, TEXH);
    bbfp.colkey_f_ = TRUE;
    mid::midBltBufFromPngData(pmgChRankerGrp_[no]->stGC_.pal_, bbfp);


    //ずら
    bbfp.Init();
    bbfp.SetSrcParam(&cntn_.cntnPng_.png_Hair_[hfileNo], hairsx, hairsy, HAIRW, HAIRH);
    bbfp.SetDestParam(texbuf, hdx, hdy, TEXW, TEXH);
    bbfp.colkey_f_ = TRUE;
    mid::midBltBufFromPngData(pmgChRankerGrp_[no]->stGC_.pal_, bbfp);

    //テクスチャ生成し、ハンドル渡す
    if (sub_f)
    {
      mid::midMakeTexture24(&cntn_.cntnBO_.bmo_IconRankerChestSub_[no], texbuf, TEXW, TEXH);
    }
    else
    {
      mid::midMakeTexture24(&cntn_.cntnBO_.bmo_IconRankerChest_[no], texbuf, TEXW, TEXH);
    }

    //バッファ開放
    mid::midRelease(texbuf);
  }


  //監督体テクスチャ
  void TMgGrp::MakeKantokuTexture_Body(TMgCharGrp* mgcg, mid::TBmpObj* pbmpobj)
  {
    s32 BODYTEX = 128;

    //バッファメモリ
    void* texbuf = mid::midAlloc2((BODYTEX * BODYTEX) * mid::TEXFMT24);//0クリア必要
    //作成
    mid::TParamBltBufFromPng bbfp;
    bbfp.Init();
    bbfp.SetSrcParam(&cntn_.cntnPng_.png_Kantoku_, 0, 0, cntn_.cntnPng_.png_Kantoku_.width_, cntn_.cntnPng_.png_Kantoku_.height_);
    bbfp.SetDestParam(texbuf, 0, 0, BODYTEX, BODYTEX);
    mid::midBltBufFromPngData(mgcg->stGC_.pal_, bbfp);

    //テクスチャ生成し、ハンドル渡す
    mid::midMakeTexture24(pbmpobj, texbuf, BODYTEX, BODYTEX);

    //バッファ開放
    mid::midRelease(texbuf);
  }
  //Captainテクスチャ
  void TMgGrp::MakeCaptainTexture_Body(TMgCharGrp* mgcg)
  {
    //解放
    //    cntn_.cntnBO_.bmo_Sensyu_[uino].DeleteTex();

    s32 UIBH = 128;
    s32 UIBW = 256;
    //s32 UIBW = 256;
    //s32 UIBH = 512;

    //バッファメモリ
    void* texbuf = mid::midAlloc2((UIBW * UIBH) * mid::TEXFMT24);//0クリア必要
    //作成
    mid::TParamBltBufFromPng bbfp;
    bbfp.Init();
    bbfp.SetSrcParam(&cntn_.cntnPng_.png_Captain_, 0, 0, cntn_.cntnPng_.png_Captain_.width_, cntn_.cntnPng_.png_Captain_.height_);
    bbfp.SetDestParam(texbuf, 0, 0, UIBW, UIBH);
    mid::midBltBufFromPngData(mgcg->stGC_.pal_, bbfp);

    //u8* texss = (u8*)texbuf;
    //for (s32 i = 0; i < UIBW * UIBH * mid::TEXFMT24; ++i)
    //{
    //  texss[i] = 0xaa;
    //}


    //テクスチャ生成し、ハンドル渡す
    mid::midMakeTexture24(&cntn_.cntnBO_.bmo_Captain_, texbuf, UIBW, UIBH);
    //バッファ開放
    mid::midRelease(texbuf);
  }
  void TMgGrp::MakeCaptainsTexture_Body(TMgCharGrp* mgcg, s32 uino)
  {
    //解放
    //    cntn_.cntnBO_.bmo_Sensyu_[uino].DeleteTex();

    s32 UIBH = 128;
    s32 UIBW = 256;
    //s32 UIBW = 256;
    //s32 UIBH = 512;

    //バッファメモリ
    void* texbuf = mid::midAlloc2((UIBW * UIBH) * mid::TEXFMT24);//0クリア必要
    //作成
    mid::TParamBltBufFromPng bbfp;
    bbfp.Init();
    bbfp.SetSrcParam(&cntn_.cntnPng_.png_Captain_, 0, 0, cntn_.cntnPng_.png_Captain_.width_, cntn_.cntnPng_.png_Captain_.height_);
    bbfp.SetDestParam(texbuf, 0, 0, UIBW, UIBH);
    mid::midBltBufFromPngData(mgcg->stGC_.pal_, bbfp);

    //u8* texss = (u8*)texbuf;
    //for (s32 i = 0; i < UIBW * UIBH * mid::TEXFMT24; ++i)
    //{
    //  texss[i] = 0xaa;
    //}


    //テクスチャ生成し、ハンドル渡す
    mid::midMakeTexture24(&cntn_.cntnBO_.bmo_Captains_[uino], texbuf, UIBW, UIBH);
    //バッファ開放
    mid::midRelease(texbuf);
  }

  //Sensyuテクスチャ
  void TMgGrp::MakeSensyuTexture_Body(TMgCharGrp* mgcg, s32 uino)
  {
    //解放
    //    cntn_.cntnBO_.bmo_Sensyu_[uino].DeleteTex();

    s32 UIBH = 128;
    s32 UIBW = 256;
    //s32 UIBW = 256;
    //s32 UIBH = 512;

    //バッファメモリ
    void* texbuf = mid::midAlloc2((UIBW * UIBH) * mid::TEXFMT24);//0クリア必要
    //作成
    mid::TParamBltBufFromPng bbfp;
    bbfp.Init();
    bbfp.SetSrcParam(&cntn_.cntnPng_.png_Sensyu_, 0, 0, cntn_.cntnPng_.png_Sensyu_.width_, cntn_.cntnPng_.png_Sensyu_.height_);
    bbfp.SetDestParam(texbuf, 0, 0, UIBW, UIBH);
    mid::midBltBufFromPngData(mgcg->stGC_.pal_, bbfp);

    //u8* texss = (u8*)texbuf;
    //for (s32 i = 0; i < UIBW * UIBH * mid::TEXFMT24; ++i)
    //{
    //  texss[i] = 0xaa;
    //}


    //テクスチャ生成し、ハンドル渡す
    mid::midMakeTexture24(&cntn_.cntnBO_.bmo_Sensyu_[uino], texbuf, UIBW, UIBH);
    //バッファ開放
    mid::midRelease(texbuf);
  }

  // Tipsキャラテクスチャ
  void TMgGrp::MakePersonTexture_Body(TMgCharGrp* mgcg, s32 uino)
  {
    //解放
    //    cntn_.cntnBO_.bmo_Sensyu_[uino].DeleteTex();

    s32 UIBH = 128;
    s32 UIBW = 256;
    //s32 UIBW = 256;
    //s32 UIBH = 512;

    //バッファメモリ
    void* texbuf = mid::midAlloc2((UIBW * UIBH) * mid::TEXFMT24);//0クリア必要
    //作成
    mid::TParamBltBufFromPng bbfp;
    bbfp.Init();
    bbfp.SetSrcParam(&cntn_.cntnPng_.png_Person_, 0, 0, cntn_.cntnPng_.png_Person_.width_, cntn_.cntnPng_.png_Person_.height_);
    bbfp.SetDestParam(texbuf, 0, 0, UIBW, UIBH);
    mid::midBltBufFromPngData(mgcg->stGC_.pal_, bbfp);

    //u8* texss = (u8*)texbuf;
    //for (s32 i = 0; i < UIBW * UIBH * mid::TEXFMT24; ++i)
    //{
    //  texss[i] = 0xaa;
    //}


    //テクスチャ生成し、ハンドル渡す
    mid::midMakeTexture24(cntn_.cntnBO_.bmo_Person_[uino], texbuf, UIBW, UIBH);
    //バッファ開放
    mid::midRelease(texbuf);
  }

  void TMgGrp::MakeUITexture_Face(TMgCharGrp* mgcg, mid::TBmpObj* bmoface, base::TGrTile2* face)
  {
    //    bmoface->DeleteTex();


    s32 ffile = mgcg->stGC_.GetLooks_FaceFile();
    s32 fno = mgcg->stGC_.GetLooks_FaceNo();
    s32 FW = 256;
    s32 FH = 8;

    //バッファメモリ
    void* texbuf = mid::midAlloc2((FW * FH) * mid::TEXFMT24);//0クリア必要

    //作成
    mid::TParamBltBufFromPng bbfp;
    bbfp.Init();
    bbfp.SetSrcParam(&cntn_.cntnPng_.png_Face_[ffile], 0, (fno * FH), cntn_.cntnPng_.png_Face_[ffile].width_, FH);
    bbfp.SetDestParam(texbuf, 0, 0, FW, FH);
    mid::midBltBufFromPngData(mgcg->stGC_.pal_, bbfp);

    //タイル追加
    s32 fnum = cntn_.cntnPng_.png_Face_[ffile].width_ / FACEW;
    for (s32 i = 0; i < fnum; ++i)
    {
      TRect rect = TRect((i * FACEW), 0, FACEW, FACEH);
      face->AddTile(rect);
    }

    //u8* texss = (u8*)texbuf;
    //for (s32 i = 0; i < FW * FH * mid::TEXFMT24; ++i)
    //{
    //  texss[i] = 0x80;
    //}

    //テクスチャ生成し、ハンドル渡す
    mid::midMakeTexture24(bmoface, texbuf, FW, FH);
    //バッファ開放
    mid::midRelease(texbuf);
  }
  void TMgGrp::MakeUITexture_Hair(TMgCharGrp* mgcg, mid::TBmpObj* bmohair, base::TGrTile2* hair)
  {
    //    bmohair->DeleteTex();

    s32 hfile = mgcg->stGC_.GetLooks_ZuraFile();
    s32 hno = mgcg->stGC_.GetLooks_ZuraNo();
    s32 HW = 256;
    s32 HH = 16;

    //バッファメモリ
    void* texbuf = mid::midAlloc2((HW * HH) * mid::TEXFMT24);//0クリア必要
    //作成
    mid::TParamBltBufFromPng bbfp;
    bbfp.Init();
    bbfp.SetSrcParam(&cntn_.cntnPng_.png_Hair_[hfile], 0, (hno * HH), cntn_.cntnPng_.png_Hair_[hfile].width_, HH);
    bbfp.SetDestParam(texbuf, 0, 0, HW, HH);
    mid::midBltBufFromPngData(mgcg->stGC_.pal_, bbfp);

    //タイル追加
    s32 hnum = cntn_.cntnPng_.png_Hair_[hfile].width_ / HAIRW;
    for (s32 i = 0; i < hnum; ++i)
    {
      TRect rect = TRect((i * HAIRW), 0, HAIRW, HAIRH);
      hair->AddTile(rect);
    }

    //u8* texss = (u8*)texbuf;
    //for (s32 i = 0; i < HW * HH; ++i)
    //{
    //  texss[i] = 0x40;
    //}

    //テクスチャ生成し、ハンドル渡す
    mid::midMakeTexture24(bmohair, texbuf, HW, HH);
    //バッファ開放
    mid::midRelease(texbuf);
  }

  //監督テクスチャ作成
  void TMgGrp::MakeKantokuTexture()
  {
    MakeKantokuTexture_Body(pmgcg_kantoku_, &cntn_.cntnBO_.bmo_Kantoku_);

    MakeUITexture_Face(pmgcg_kantoku_, &cntn_.cntnBO_.bmo_KantokuFace_, pgr_KantokuFace_);
    MakeUITexture_Hair(pmgcg_kantoku_, &cntn_.cntnBO_.bmo_KantokuHair_, pgr_KantokuHair_);
    pmgcg_kantoku_->SetGrTile(pgr_Kantoku_, pgr_KantokuFace_, pgr_KantokuHair_, &pmgEO_->mgDt_.dtCell_kantoku_);
  }
  //キャプテンテクスチャ作成
  void TMgGrp::MakeCaptainTexture()
  {
    MakeCaptainTexture_Body(pmgcg_captain_);
    MakeUITexture_Face(pmgcg_captain_, &cntn_.cntnBO_.bmo_CaptainFace_, pgr_CaptainFace_);
    MakeUITexture_Hair(pmgcg_captain_, &cntn_.cntnBO_.bmo_CaptainHair_, pgr_CaptainHair_);
    pmgcg_captain_->SetGrTile(pgr_Captain_, pgr_CaptainFace_, pgr_CaptainHair_, &pmgEO_->mgDt_.dtCell_sensyu_); //dtCell_captain_
  }

  //ＵＩbodyテクスチャ作成
  void TMgGrp::MakeSensyuTexture(s32 uino)
  {
    MakeSensyuTexture_Body(pmgcg_sensyu_[uino], uino);
    MakeUITexture_Face(pmgcg_sensyu_[uino], &cntn_.cntnBO_.bmo_SensyuFace_[uino], pgr_SensyuFace_[uino]);
    MakeUITexture_Hair(pmgcg_sensyu_[uino], &cntn_.cntnBO_.bmo_SensyuHair_[uino], pgr_SensyuHair_[uino]);
    pmgcg_sensyu_[uino]->SetGrTile(pgr_Sensyu_[uino], pgr_SensyuFace_[uino], pgr_SensyuHair_[uino], &pmgEO_->mgDt_.dtCell_sensyu_);
  }
  //敵テクスチャ作成
  void TMgGrp::MakeEnemyTexture()
  {
    MakeKantokuTexture_Body(pmgcg_enemy_, &cntn_.cntnBO_.bmo_Enemy_);
    MakeUITexture_Face(pmgcg_enemy_, &cntn_.cntnBO_.bmo_EnemyFace_, pgr_EnemyFace_);
    MakeUITexture_Hair(pmgcg_enemy_, &cntn_.cntnBO_.bmo_EnemyHair_, pgr_EnemyHair_);
    pmgcg_enemy_->SetGrTile(pgr_Enemy_, pgr_EnemyFace_, pgr_EnemyHair_, &pmgEO_->mgDt_.dtCell_kantoku_);
  }
  //敵テクスチャ作成
  void TMgGrp::MakeEnemiesTexture(s32 uino)
  {
    MakeKantokuTexture_Body(pmgcg_enemies_[uino], &cntn_.cntnBO_.bmo_Enemies_[uino]);
    MakeUITexture_Face(pmgcg_enemies_[uino], &cntn_.cntnBO_.bmo_EnemiesFace_[uino], pgr_EnemiesFace_[uino]);
    MakeUITexture_Hair(pmgcg_enemies_[uino], &cntn_.cntnBO_.bmo_EnemiesHair_[uino], pgr_EnemiesHair_[uino]);
    pmgcg_enemies_[uino]->SetGrTile(pgr_Enemies_[uino], pgr_EnemiesFace_[uino], pgr_EnemiesHair_[uino], &pmgEO_->mgDt_.dtCell_kantoku_);
  }
  void TMgGrp::MakeCaptainsTexture(s32 uino)
  {
    MakeCaptainsTexture_Body(pmgcg_captains_[uino], uino);
    MakeUITexture_Face(pmgcg_captains_[uino], &cntn_.cntnBO_.bmo_CaptainsFace_[uino], pgr_CaptainsFace_[uino]);
    MakeUITexture_Hair(pmgcg_captains_[uino], &cntn_.cntnBO_.bmo_CaptainsHair_[uino], pgr_CaptainsHair_[uino]);
    pmgcg_captains_[uino]->SetGrTile(pgr_Captains_[uino], pgr_CaptainsFace_[uino], pgr_CaptainsHair_[uino], &pmgEO_->mgDt_.dtCell_sensyu_); //dtCell_captain_
  }
  //Tipsテクスチャ作成
  void TMgGrp::MakePersonTexture(s32 key)
  {
    // まだ存在しない
    if (pgr_Person_.count(key) == 0)
    {
      // 新規作成
      cntn_.cntnBO_.AddPerson(key);
      base::TMgOrder* pmgOrder = &(pmgEO_->mgOrder_);
      pgr_Person_[key] = new base::TGrTile(pmgOrder, cntn_.cntnBO_.bmo_Person_[key], BMP_PERSON);
      pgr_PersonFace_[key] = new base::TGrTile2(pmgOrder, cntn_.cntnBO_.bmo_PersonFace_[key]);
      pgr_PersonHair_[key] = new base::TGrTile2(pmgOrder, cntn_.cntnBO_.bmo_PersonHair_[key]);
    }

    MakePersonTexture_Body(pmgcg_person_[key], key);
    MakeUITexture_Face(pmgcg_person_[key], cntn_.cntnBO_.bmo_PersonFace_[key], pgr_PersonFace_[key]);
    MakeUITexture_Hair(pmgcg_person_[key], cntn_.cntnBO_.bmo_PersonHair_[key], pgr_PersonHair_[key]);
    pmgcg_person_[key]->SetGrTile(pgr_Person_[key], pgr_PersonFace_[key], pgr_PersonHair_[key], &pmgEO_->mgDt_.dtCell_person_);
  }


  //ボステクスチャ作成
  void TMgGrp::MakeBossTexture()
  {
    //    ClearBossGrp();
    pgr_Boss_->ClearTile();

    s32 TEXH = 256;//区画サイズ 6
    s32 TEXW = 128;//区画サイズ4
    if (stBoss_.size() > 24) TEXW = 256;
    if (stBoss_.size() > 48) TEXW = 512;
    //s32 num = DBMEMBER_ALL;

    //バッファメモリ
    void* texbuf = mid::midAlloc2((TEXW * TEXH)* mid::TEXFMT24);//0クリア必要

    //区画１行に入る数
    const s32 YMAX = TEXH / BODYH;
    const s32 HREVX = 4;//ボディパーツに合う顔の位置
    const s32 HREVY = 7;//ボディパーツに合う顔の位置
    const s32 FREVX = 8;//ボディパーツに合う顔の位置
    const s32 FREVY = 8;//ボディパーツに合う顔の位置

    //づら
    for (s32 i2 = 0; i2 < stBoss_.size(); ++i2)
    {
      const s32 hairsx = 0;

      const s32 hfileNo = stBoss_[i2]->GetLooks_ZuraFile();
      const s32 hairsy = stBoss_[i2]->GetLooks_ZuraNo() * HAIRH;
      const s32 ffileNo = stBoss_[i2]->GetLooks_FaceFile();
      const s32 facesx = 0;
      const s32 facesy = stBoss_[i2]->GetLooks_FaceNo() * FACEH;
      const s32 bodysy = 0;

      //body
      s32 bdx = ((i2 / YMAX) * BODYW);
      s32 bdy = ((i2 % YMAX) * BODYH);

      //髪の毛
      s32 hdx = bdx + HREVX;
      s32 hdy = bdy + HREVY;
      //顔
      s32 fdx = bdx + FREVX;
      s32 fdy = bdy + FREVY;


      mid::TParamBltBufFromPng bbfp;

      //ボディ
      bbfp.Init();
      bbfp.SetSrcParam(&cntn_.cntnPng_.png_Cap_, 0, bodysy, BODYW, BODYH);
      bbfp.SetDestParam(texbuf, bdx, bdy, TEXW, TEXH);
      mid::midBltBufFromPngData(stBoss_[i2]->pal_, bbfp);


      //顔
      bbfp.Init();
      bbfp.SetSrcParam(&cntn_.cntnPng_.png_Face_[ffileNo], facesx, facesy, FACEW, FACEH);
      bbfp.SetDestParam(texbuf, fdx, fdy, TEXW, TEXH);
      bbfp.colkey_f_ = TRUE;
      mid::midBltBufFromPngData(stBoss_[i2]->pal_, bbfp);


      //ずら
      bbfp.Init();
      bbfp.SetSrcParam(&cntn_.cntnPng_.png_Hair_[hfileNo], hairsx, hairsy, HAIRW, HAIRH);
      bbfp.SetDestParam(texbuf, hdx, hdy, TEXW, TEXH);
      bbfp.colkey_f_ = TRUE;
      mid::midBltBufFromPngData(stBoss_[i2]->pal_, bbfp);

      //タイル追加
      TRect rect = TRect(bdx, bdy, BODYW, BODYH);
      pgr_Boss_->AddTile(rect);
    }

    //テクスチャ生成し、ハンドル渡す
    mid::midMakeTexture24(&cntn_.cntnBO_.bmo_Boss_, texbuf, TEXW, TEXH);
    //バッファ開放
    mid::midRelease(texbuf);
  }


  //雑魚テクスチャ作成
  void TMgGrp::MakeZakoTexture()
  {
    //    ClearZakoGrp();
    pgr_Zako_->ClearTile();

    s32 TEXH = 128;
    s32 TEXW = 32;

    //バッファメモリ
    void* texbuf = mid::midAlloc2((TEXW * TEXH)* mid::TEXFMT24);//0クリア必要

    //区画１行に入る数
    const s32 YMAX = TEXH / BODYH;
    const s32 HREVX = 4;//ボディパーツに合う髪の位置
    const s32 HREVY = 7;//ボディパーツに合う髪の位置
    const s32 FREVX = 8;//ボディパーツに合う顔の位置
    const s32 FREVY = 8;//ボディパーツに合う顔の位置

    //づら
    for (s32 i2 = 0; i2 < stZako_.size(); ++i2)
    {
      const s32 hairsx = 0;

      const s32 hfileNo = stZako_[i2]->GetLooks_ZuraFile();
      const s32 hairsy = stZako_[i2]->GetLooks_ZuraNo() * HAIRH;
      const s32 ffileNo = stZako_[i2]->GetLooks_FaceFile();
      const s32 facesx = 0;
      const s32 facesy = stZako_[i2]->GetLooks_FaceNo() * FACEH;
      const s32 bodysy = 0;

      //body
      s32 bdx = ((i2 / YMAX) * BODYW);
      s32 bdy = ((i2 % YMAX) * BODYH);

      //髪の毛
      s32 hdx = bdx + HREVX;
      s32 hdy = bdy + HREVY;

      //顔
      s32 fdx = bdx + FREVX;
      s32 fdy = bdy + FREVY;

      mid::TParamBltBufFromPng bbfp;

      //ボディ
      bbfp.Init();
      bbfp.SetSrcParam(&cntn_.cntnPng_.png_Cap_, 0, bodysy, BODYW, BODYH);
      bbfp.SetDestParam(texbuf, bdx, bdy, TEXW, TEXH);
      mid::midBltBufFromPngData(stZako_[i2]->pal_, bbfp);


      //顔
      bbfp.Init();
      bbfp.SetSrcParam(&cntn_.cntnPng_.png_Face_[ffileNo], facesx, facesy, FACEW, FACEH);
      bbfp.SetDestParam(texbuf, fdx, fdy, TEXW, TEXH);
      bbfp.colkey_f_ = TRUE;
      mid::midBltBufFromPngData(stZako_[i2]->pal_, bbfp);


      //ずら
      bbfp.Init();
      bbfp.SetSrcParam(&cntn_.cntnPng_.png_Hair_[hfileNo], hairsx, hairsy, HAIRW, HAIRH);
      bbfp.SetDestParam(texbuf, hdx, hdy, TEXW, TEXH);
      bbfp.colkey_f_ = TRUE;
      mid::midBltBufFromPngData(stZako_[i2]->pal_, bbfp);

      //タイル追加
      TRect rect = TRect(bdx, bdy, BODYW, BODYH);
      pgr_Zako_->AddTile(rect);
    }

    //テクスチャ生成し、ハンドル渡す
    mid::midMakeTexture24(&cntn_.cntnBO_.bmo_Zako_, texbuf, TEXW, TEXH);
    //バッファ開放
    mid::midRelease(texbuf);
  }

  //統合テクスチャ作成
  void TMgGrp::MakeGameTexture(s32 line_r, s32 line_g, s32 line_b, s32 line_a)
  {
    //バッファメモリ
    void* texbuf = mid::midAlloc2((TEX1024 * TEX1024) * mid::TEXFMT24);//0クリア必要
    s32 texpos = 0;
    s32 texpos2 = 0;

    //コート
    MakeTexture_Court(texbuf, texpos, line_r, line_g, line_b, line_a);

    //ボディ
    MakeTexture_Body(texbuf, texpos);

    //かおづら
    MakeTexture_Head(texbuf, texpos);

    //ボール
    for (s32 i = 0; i < BALLSPNUM; ++i)
    {
      MakeTexture_Ball(texbuf, texpos, texpos2, i);
    }


    //レフェリー
    MakeTexture_Tex2(texbuf, texpos, texpos2, &cntn_.cntnPng_.png_Referee_, pgr_Referee_);

    //その他
    MakeTexture_Tex2(texbuf, texpos, texpos2, &cntn_.cntnPng_.png_Etc_, pmgEtcGrp_);
    //その他
    //MakeTexture_Tex2(texbuf, texpos, texpos2, &cntn_.cntnPng_.png_Etc_, pmgEtcGrp_);

    //テクスチャ生成し、ハンドル渡す
    mid::midMakeTexture24(&cntn_.cntnBO_.bmo_Game_, texbuf, TEX1024, TEX1024);
    //バッファ開放
    mid::midRelease(texbuf);
  }

  //テクスチャ作成プレビュー用
  void TMgGrp::MakeGameTexture_Prev()
  {
    s32 TEXW = (TEX1024 / 2);
    //if (pmgChGrp_[SIDE0][0]->stGC_.skincol_ == pmgChGrp_[SIDE0][1]->stGC_.skincol_)
    //{
    //  //TEXW = (TEX1024 / 4);確保エラーでる
    //}

    //バッファメモリ（コートは読まないので高さは半分で済む）
    void* texbuf = mid::midAlloc2((TEX1024 * TEXW) * mid::TEXFMT24);//0クリア必要
    s32 texpos = 0;
    s32 texpos2 = 0;

    //ボディ
    MakeTexture_Body(texbuf, texpos);
    //MakeTexture_BodyPrev(texbuf, texpos);
    //かおづら
    MakeTexture_Head(texbuf, texpos);
    //MakeTexture_HeadPrev(texbuf, texpos, texpos2);


    //ボール
    for (s32 i = 0; i < (BALLSPNUM - 1); ++i)//ナイスシュートはいらない
    {
      MakeTexture_Ball(texbuf, texpos, texpos2, i);
    }

    //その他
    MakeTexture_Tex2(texbuf, texpos, texpos2, &cntn_.cntnPng_.png_Etc_, pmgEtcGrp_);
    //その他
    //MakeTexture_Tex2(texbuf, texpos, texpos2, &cntn_.cntnPng_.png_Etc_, pmgEtcGrp_);

    //テクスチャ生成し、ハンドル渡す
    mid::midMakeTexture24(&cntn_.cntnBO_.bmo_Game_, texbuf, TEX1024, TEXW);
    //バッファ開放
    mid::midRelease(texbuf);
  }

  //コートテクスチャ（コートのラインもここで描く）
  void TMgGrp::MakeTexture_Court(void* texbuf, s32& texpos, s32 line_r, s32 line_g, s32 line_b, s32 line_a)
  {
    const s32 COURTLINE_Y = 136;

    //重なりがない
    s32 posx = ((texpos % TEXPTNUM) * TEXPTSIZE);
    s32 posy = ((texpos / TEXPTNUM) * TEXPTSIZE);


    ////バッファメモリ
    //
    //void* courtbuf = mid::midAlloc2((TEX1024 * TEX1024) * mid::TEXFMT24);//0クリア必要
    //s32 texpos = 0;
    //s32 texpos2 = 0;

    //作成
    mid::TParamBltBufFromPng bbfp;
    bbfp.Init();
    bbfp.SetSrcParam(&cntn_.cntnPng_.png_Court_, 0, 0, cntn_.cntnPng_.png_Court_.width_, cntn_.cntnPng_.png_Court_.height_);
    bbfp.SetDestParam(texbuf, posx, posy, TEX1024, TEX1024);
    mid::midBltBufFromPngData(cntn_.cntnPng_.png_Court_.pal_, bbfp);

    lib_rgb::RGBPAL linepal;
    for (s32 i = 0; i < lib_rgb::PALNUM; ++i)
    {
      linepal[i] = cntn_.cntnPng_.png_Court_.pal_[i];
    }

    linepal[1].SetCol(line_r, line_g, line_b);

    s32 linex = posx + (cntn_.cntnPng_.png_Court_.width_ / 2) - (cntn_.cntnPng_.png_Line_.width_ / 2);
    s32 liney = posy + COURTLINE_Y;
    bbfp.SetSrcParam(&cntn_.cntnPng_.png_Line_, 0, 0, cntn_.cntnPng_.png_Line_.width_, cntn_.cntnPng_.png_Line_.height_);
    bbfp.SetDestParam(texbuf, linex, liney, TEX1024, TEX1024);
    bbfp.colkey_f_ = TRUE;//コートはカラーキーいらないか
    bbfp.alp_f_ = TRUE;
    bbfp.srcalp_ = line_a;
    mid::midBltBufFromPngData(linepal, bbfp);

    //参照オフセット
    pgr_Court_->SetOfset(posx, posy, cntn_.cntnPng_.png_Court_.width_, cntn_.cntnPng_.png_Court_.height_);
    //次の区画
    //コートは２区画使う
    texpos += 2;

  }

  //体テクスチャ
  void TMgGrp::MakeTexture_Body(void* texbuf, s32& texpos)
  {
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
      {

        s32 sameskin = NGNUM;

        for (s32 i3 = 0; i3 < i2; ++i3)
        {
          //自分より前に同じ肌の色がいる
          if (pmgChGrp_[i][i2]->stGC_.skincol_ == pmgChGrp_[i][i3]->stGC_.skincol_)
          {
            sameskin = i3;
            break;
          }
        }

        s32 posx = 0;
        s32 posy = 0;

        //重なりがない
        if (sameskin == NGNUM)
        {
          posx = ((texpos % TEXPTNUM) * TEXPTSIZE);
          posy = ((texpos / TEXPTNUM) * TEXPTSIZE);

          //作成
          mid::TParamBltBufFromPng bbfp;
          bbfp.Init();
          bbfp.SetSrcParam(&cntn_.cntnPng_.png_Body_, 0, 0, cntn_.cntnPng_.png_Body_.width_, cntn_.cntnPng_.png_Body_.height_);
          bbfp.SetDestParam(texbuf, posx, posy, TEX1024, TEX1024);
          mid::midBltBufFromPngData(pmgChGrp_[i][i2]->stGC_.pal_, bbfp);

          //次の区画
          ++texpos;
        }
        else
        {
          posx = pgr_Body_[i][sameskin]->GetOfsX();
          posy = pgr_Body_[i][sameskin]->GetOfsY();
        }

        //参照オフセット
        pgr_Body_[i][i2]->SetOfset(posx, posy, TEXPTSIZE, TEXPTSIZE);
      }
    }
  }

  void TMgGrp::MakeTexture_BodyPrev(void* texbuf, s32& texpos)
  {
    //左サイドのみ

    for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
    {
      s32 sameskin = NGNUM;
      for (s32 i3 = 0; i3 < i2; ++i3)
      {
        //自分より前に同じ肌の色がいる
        if (pmgChGrp_[SIDE0][i2]->stGC_.skincol_ == pmgChGrp_[SIDE0][i3]->stGC_.skincol_)
        {
          sameskin = i3;
          break;
        }
      }

      s32 posx = 0;
      s32 posy = 0;

      //重なりがない
      if (sameskin == NGNUM)
      {
        posx = ((texpos % TEXPTNUM) * TEXPTSIZE);
        posy = ((texpos / TEXPTNUM) * TEXPTSIZE);
        //作成
        mid::TParamBltBufFromPng bbfp;
        bbfp.Init();
        bbfp.SetSrcParam(&cntn_.cntnPng_.png_Body_, 0, 0, cntn_.cntnPng_.png_Body_.width_, cntn_.cntnPng_.png_Body_.height_);
        bbfp.SetDestParam(texbuf, posx, posy, TEX1024, TEX1024);
        mid::midBltBufFromPngData(pmgChGrp_[SIDE0][i2]->stGC_.pal_, bbfp);
        //次の区画
        ++texpos;
      }
      else
      {
        posx = pgr_Body_[SIDE0][sameskin]->GetOfsX();
        posy = pgr_Body_[SIDE0][sameskin]->GetOfsY();
      }
      //参照オフセット
      pgr_Body_[SIDE0][i2]->SetOfset(posx, posy, TEXPTSIZE, TEXPTSIZE);
      pgr_Body_[SIDE1][i2]->SetOfset(posx, posy, TEXPTSIZE, TEXPTSIZE);
    }

  }

  //顔
  void TMgGrp::MakeTexture_Head(void* texbuf, s32& texpos)
  {
    //とりあえず現状、７人２チーム分のかおづらのみ。区画を超える可能性出たら修正。
    const s32 BASEX = ((texpos % TEXPTNUM) * TEXPTSIZE);
    const s32 BASEY = ((texpos / TEXPTNUM) * TEXPTSIZE);


    //区画を進める
    ++texpos;

    s32 cpos = 0;

    //試合中用頭アイコン
    {
      //区画１行に入る数
      const s32 HYMAX = TEXPTSIZE / CHESTH;
      const s32 HREVX = 0;//胸パーツに合う顔の位置
      const s32 HREVY = 3;//胸パーツに合う顔の位置
      const s32 FREVX = 4;//胸パーツに合う顔の位置
      const s32 FREVY = 4;//胸パーツに合う顔の位置

      for (s32 i = 0; i < DBSIDE; ++i)
      {
        for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
        {
          //づら
          pgr_IconHead_[i][i2]->ClearTile();
          const s32 hairsx = 0;
          const s32 hfileNo = pmgChGrp_[i][i2]->GetLooks_ZuraFile();
          const s32 hairsy = pmgChGrp_[i][i2]->GetLooks_ZuraNo() * HAIRH;
          const s32 ffileNo = pmgChGrp_[i][i2]->GetLooks_FaceFile();
          const s32 facesx = 0;
          const s32 facesy = pmgChGrp_[i][i2]->GetLooks_FaceNo() * FACEH;
          const s32 mousesx = 0;
          const s32 mousesy = 0;

          //胸
          s32 cdx = BASEX + ((cpos / HYMAX) * CHESTW);
          s32 cdy = BASEY + ((cpos % HYMAX) * CHESTH);

          //髪の毛
          s32 hdx = cdx + HREVX;
          s32 hdy = cdy + HREVY;
          //顔
          s32 fdx = cdx + FREVX;
          s32 fdy = cdy + FREVY;

          mid::TParamBltBufFromPng bbfp;

          //胸
          bbfp.Init();
          bbfp.SetSrcParam(&cntn_.cntnPng_.png_Chest_, 0, 0, CHESTW, CHESTH);
          bbfp.SetDestParam(texbuf, cdx, cdy, TEX1024, TEX1024);
          mid::midBltBufFromPngData(pmgChGrp_[i][i2]->stGC_.pal_, bbfp);

          //顔
          bbfp.Init();
          bbfp.SetSrcParam(&cntn_.cntnPng_.png_Face_[ffileNo], facesx, facesy, FACEW, FACEH);
          bbfp.SetDestParam(texbuf, fdx, fdy, TEX1024, TEX1024);
          bbfp.colkey_f_ = TRUE;
          mid::midBltBufFromPngData(pmgChGrp_[i][i2]->stGC_.pal_, bbfp);

          //ずら
          bbfp.Init();
          bbfp.SetSrcParam(&cntn_.cntnPng_.png_Hair_[hfileNo], hairsx, hairsy, HAIRW, HAIRH);
          bbfp.SetDestParam(texbuf, hdx, hdy, TEX1024, TEX1024);
          bbfp.colkey_f_ = TRUE;
          mid::midBltBufFromPngData(pmgChGrp_[i][i2]->stGC_.pal_, bbfp);

          ++cpos;

          //タイル追加
          TRect rect = TRect(cdx, cdy, CHESTW, CHESTH);
          pgr_IconHead_[i][i2]->AddTile(rect);
        }
      }
    }


    s32 hpos = ((cpos * CHESTH) / HAIRH) + 2;
    s32 endhx = 0;
    const s32 HYMAX = TEXPTSIZE / HAIRH;
    //づらのほうがサイズが大きいので先に
    {
      //区画１行に入る数
      const s32 HYNUM = cntn_.cntnPng_.png_Hair_[0].height_ / HAIRH;
      const s32 HXNUM = cntn_.cntnPng_.png_Hair_[0].width_ / HAIRW;

      for (s32 i = 0; i < DBSIDE; ++i)
      {
        for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
        {
          //づら

          pgr_Hair_[i][i2]->ClearTile();

          const s32 hfileNo = pmgChGrp_[i][i2]->GetLooks_ZuraFile();
          const s32 hairsy = pmgChGrp_[i][i2]->GetLooks_ZuraNo() * HAIRH;

          for (s32 i3 = 0; i3 < HXNUM; ++i3)
          {
            const s32 hairsx = (i3 * HAIRW);

            s32 hdx = BASEX + ((hpos / HYMAX) * HAIRW);
            s32 hdy = BASEY + ((hpos % HYMAX) * HAIRH);
            //作成
            mid::TParamBltBufFromPng bbfp;
            bbfp.Init();
            bbfp.SetSrcParam(&cntn_.cntnPng_.png_Hair_[hfileNo], hairsx, hairsy, HAIRW, HAIRH);
            bbfp.SetDestParam(texbuf, hdx, hdy, TEX1024, TEX1024);
            mid::midBltBufFromPngData(pmgChGrp_[i][i2]->stGC_.pal_, bbfp);

            ++hpos;
            endhx = hdx + HAIRW;

            //タイル追加
            TRect rect = TRect(hdx, hdy, HAIRW, HAIRH);
            pgr_Hair_[i][i2]->AddTile(rect);
          }
        }
      }
    }

    s32 fpos = 0;// (hpos * 2);

    //s32 endfx = 0;
    //かお
    {
      //区画１行に入る数
      const s32 FYMAX = TEXPTSIZE / FACEH;

      const s32 FYNUM = cntn_.cntnPng_.png_Face_[0].height_ / FACEH;
      const s32 FXNUM = cntn_.cntnPng_.png_Face_[0].width_ / FACEW;

      for (s32 i = 0; i < DBSIDE; ++i)
      {
        for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
        {
          pgr_Face_[i][i2]->ClearTile();

          const s32 ffileNo = pmgChGrp_[i][i2]->GetLooks_FaceFile();
          const s32 facesy = pmgChGrp_[i][i2]->GetLooks_FaceNo() * FACEH;

          for (s32 i3 = 0; i3 < FXNUM; ++i3)
          {
            const s32 facesx = (i3 * FACEW);

            s32 fdx = endhx + ((fpos / FYMAX) * FACEW);
            s32 fdy = BASEY + ((fpos % FYMAX) * FACEH);
            //作成
            mid::TParamBltBufFromPng bbfp;
            bbfp.Init();
            bbfp.SetSrcParam(&cntn_.cntnPng_.png_Face_[ffileNo], facesx, facesy, FACEW, FACEH);
            bbfp.SetDestParam(texbuf, fdx, fdy, TEX1024, TEX1024);
            mid::midBltBufFromPngData(pmgChGrp_[i][i2]->stGC_.pal_, bbfp);

            ++fpos;
            //endfx = fdx + FACEW;

            //タイル追加
            TRect rect = TRect(fdx, fdy, FACEW, FACEH);
            pgr_Face_[i][i2]->AddTile(rect);
          }
        }
      }
    }

  }

  //プレビュー用かおづら
  void TMgGrp::MakeTexture_HeadPrev(void* texbuf, s32& texpos, s32& texpos2)
  {
    //とりあえず現状、７人２チーム分のかおづらのみ。区画を超える可能性出たら修正。
    const s32 BASEX = ((texpos % TEXPTNUM) * TEXPTSIZE);
    const s32 BASEY = ((texpos / TEXPTNUM) * TEXPTSIZE);
    //const s32 FREVX = 4;//胸パーツに合う顔の位置
    //const s32 FREVY = 3;//胸パーツに合う顔の位置
    //小区画を二つ進める
    texpos2 += 2;
    //大区画を進める
    if (texpos2 >= 4)
    {
      ++texpos;
      texpos2 = 0;
    }
    s32 hpos = 0;
    const s32 MEM0 = 0;

    s32 endhx = 0;
    //づらのほうがサイズが大きいので先に
    {
      //区画１行に入る数
      const s32 HYMAX = TEXPTSIZE / HAIRH;
      const s32 HYNUM = cntn_.cntnPng_.png_Hair_[0].height_ / HAIRH;
      const s32 HXNUM = cntn_.cntnPng_.png_Hair_[0].width_ / HAIRW;
      for (s32 i = 0; i < DBSIDE; ++i)
      {

        //づら
        pgr_Hair_[i][MEM0]->ClearTile();
        const s32 hfileNo = pmgChGrp_[i][MEM0]->GetLooks_ZuraFile();//左サイドのキャプテン副キャプテン
        const s32 hairsy = pmgChGrp_[i][MEM0]->GetLooks_ZuraNo() * HAIRH;

        for (s32 i3 = 0; i3 < HXNUM; ++i3)
        {
          const s32 hairsx = (i3 * HAIRW);

          s32 hdx = BASEX + ((hpos / HYMAX) * HAIRW);
          s32 hdy = BASEY + ((hpos % HYMAX) * HAIRH);
          //作成
          mid::TParamBltBufFromPng bbfp;
          bbfp.Init();
          bbfp.SetSrcParam(&cntn_.cntnPng_.png_Hair_[hfileNo], hairsx, hairsy, HAIRW, HAIRH);
          bbfp.SetDestParam(texbuf, hdx, hdy, TEX1024, TEX1024);
          mid::midBltBufFromPngData(pmgChGrp_[i][MEM0]->stGC_.pal_, bbfp);

          ++hpos;
          endhx = hdx + HAIRW;
          //タイル追加
          TRect rect = TRect(hdx, hdy, HAIRW, HAIRH);
          pgr_Hair_[i][MEM0]->AddTile(rect);
        }

      }
    }


    s32 fpos = 0;

    //s32 endfx = 0;
    //かお
    {
      //区画１行に入る数
      const s32 FYMAX = TEXPTSIZE / FACEH;

      const s32 FYNUM = cntn_.cntnPng_.png_Face_[0].height_ / FACEH;
      const s32 FXNUM = cntn_.cntnPng_.png_Face_[0].width_ / FACEW;

      for (s32 i = 0; i < DBSIDE; ++i)
      {

        pgr_Face_[i][MEM0]->ClearTile();

        const s32 ffileNo = pmgChGrp_[i][MEM0]->GetLooks_FaceFile();
        const s32 facesy = pmgChGrp_[i][MEM0]->GetLooks_FaceNo() * FACEH;

        for (s32 i3 = 0; i3 < FXNUM; ++i3)
        {
          const s32 facesx = (i3 * FACEW);

          s32 fdx = endhx + ((fpos / FYMAX) * FACEW);
          s32 fdy = BASEY + ((fpos % FYMAX) * FACEH);
          //作成
          mid::TParamBltBufFromPng bbfp;
          bbfp.Init();
          bbfp.SetSrcParam(&cntn_.cntnPng_.png_Face_[ffileNo], facesx, facesy, FACEW, FACEH);
          bbfp.SetDestParam(texbuf, fdx, fdy, TEX1024, TEX1024);
          mid::midBltBufFromPngData(pmgChGrp_[i][MEM0]->stGC_.pal_, bbfp);

          ++fpos;
          //endfx = fdx + FACEW;

          //タイル追加
          TRect rect = TRect(fdx, fdy, FACEW, FACEH);
          pgr_Face_[i][MEM0]->AddTile(rect);
        }

      }
    }

  }

  //void MakeTexture_Ball(void* texbuf, s32& texpos, s32& texpos2);
  //128x128tex
  void TMgGrp::MakeTexture_Tex2(void* texbuf, s32& texpos, s32& texpos2, mid::TPng* ppng, base::TGrp* pgrp)
  {
    const s32 BASEX = ((texpos % TEXPTNUM) * TEXPTSIZE) + ((texpos2 / TEXPTNUM2) * TEXPTSIZE2);
    const s32 BASEY = ((texpos / TEXPTNUM) * TEXPTSIZE) + ((texpos2 % TEXPTNUM2) * TEXPTSIZE2);

    //小区画を進める
    ++texpos2;
    //大区画を進める
    if (texpos2 >= 4)
    {
      ++texpos;
      texpos2 = 0;
    }

    //作成
    mid::TParamBltBufFromPng bbfp;
    bbfp.Init();
    bbfp.SetSrcParam(ppng, 0, 0, TEXPTSIZE2, TEXPTSIZE2);
    bbfp.SetDestParam(texbuf, BASEX, BASEY, TEX1024, TEX1024);
    mid::midBltBufFromPngData(ppng->pal_, bbfp);

    //参照オフセット
    pgrp->SetOfset(BASEX, BASEY, TEXPTSIZE2, TEXPTSIZE2);
  }

  //ボールテクスチャ
  void TMgGrp::MakeTexture_Ball(void* texbuf, s32& texpos, s32& texpos2, s32 ballcol)
  {
    const s32 BASEX = ((texpos % TEXPTNUM) * TEXPTSIZE) + ((texpos2 / TEXPTNUM2) * TEXPTSIZE2);
    const s32 BASEY = ((texpos / TEXPTNUM) * TEXPTSIZE) + ((texpos2 % TEXPTNUM2) * TEXPTSIZE2);

    ////小区画を進める
    //++texpos2;
    ////大区画を進める
    //if (texpos2 >= 4)
    //{
    //  ++texpos;
    //  texpos2 = 0;
    //}
    ++texpos;
    texpos2 = 0;

    lib_rgb::RGBPAL pal;

    const s32 SHIFTCOL = (ballcol * 16);

    for (s32 i = 0; i < lib_rgb::PALNUM; ++i)
    {
      s32 i2 = (i + SHIFTCOL);
      if (i2 > 0xFF) i2 = 0;

      pal[i] = cntn_.cntnPng_.png_Ball_.pal_[i2];

    }
    pal[0] = cntn_.cntnPng_.png_Ball_.pal_[0];


    //作成
    mid::TParamBltBufFromPng bbfp;
    bbfp.Init();
    bbfp.SetSrcParam(&cntn_.cntnPng_.png_Ball_, 0, 0, TEXPTSIZE, TEXPTSIZE);
    bbfp.SetDestParam(texbuf, BASEX, BASEY, TEX1024, TEX1024);
    mid::midBltBufFromPngData(pal, bbfp);

    //参照オフセット
    pgr_Ball_[ballcol]->SetOfset(BASEX, BASEY, TEXPTSIZE, TEXPTSIZE);
  }


  void TMgGrp::GenSetKantoku()
  {
    MakeKantokuTexture();

    pmgcg_kantoku_->SetGrTile(
      pgr_Kantoku_,
      pgr_KantokuFace_,
      pgr_KantokuHair_,
      &pmgEO_->mgDt_.dtCell_kantoku_);
  }
  void TMgGrp::GenSetEnemy()
  {
    mid::midLog("MakeEnemyTexture();");
    MakeEnemyTexture();

    pmgcg_enemy_->SetGrTile(
      pgr_Enemy_,
      pgr_EnemyFace_,
      pgr_EnemyHair_,
      &pmgEO_->mgDt_.dtCell_kantoku_);
  }

  void TMgGrp::SetKantoku()
  {
    pmgcg_kantoku_->SetMyKantokuDt();
    GenSetKantoku();
  }
  void TMgGrp::SetProf_Me()
  {
    pmgcg_enemy_->SetMyKantokuDt();
    GenSetEnemy();
  }
  void TMgGrp::SetProf_Ranker(s64 id_ranker)
  {
    pmgcg_enemy_->SetRankerDt(id_ranker);
    GenSetEnemy();
  }
  void TMgGrp::SetProf_Enemy(s32 no)
  {
    pmgcg_enemy_->SetEnemyDt(no);
    GenSetEnemy();
  }

  void TMgGrp::ReloadTex(enSceneName scenename)
  {
    cntn_.ReloadTex();
    //MakeIconTexture();

    //監督テクスチャ作成
    MakeKantokuTexture();
    MakeCaptainTexture();

    for (s32 i = 0; i < DBMEMBER_ALL; ++i) MakeSensyuTexture(i);
    MakeEnemyTexture();
    for (s32 i = 0; i < PLAYERNUM; ++i)MakeEnemiesTexture(i);
    for (s32 i = 0; i < PLAYERNUM; ++i)MakeCaptainsTexture(i);


//    return;

    BOOL body_f = TRUE;

    switch (scenename)
    {
    case db::snTitle:
      body_f = FALSE;
      break;
    case db::snHome:
      MakeGameTexture_Prev();
      for (s32 i = 0; i < DBSIDE; ++i)
      for (auto item : pgr_Person_) pmgEO_->mgGrp_.MakePersonTexture(item.first);
      break;
    case db::snScenario:
      MakeBossTexture();
      MakeZakoTexture();
      break;
    case db::snShiai:
      pmgEO_->mgFont_.MakeWordTexture();
      MakeGameTexture(stMG_.stgame_.line_r, stMG_.stgame_.line_g, stMG_.stgame_.line_b, stMG_.stgame_.line_a);
      break;
    case db::snTutorial:
      break;
    }

    if (body_f)
    {
      for (s32 i = 0; i < DBSIDE; ++i)
      {
        for (s32 i2 = 0; i2 < SUBNUM; ++i2)
        {
          pmgEO_->mgGrp_.MakeIconTexture_Chest(i, stMG_.stchest_.seihuku_f[i][i2], (i2 == 1));
          pmgEO_->mgGrp_.MakeIconTexture_Body(i, stMG_.stbody_.capbmpno[i][i2], stMG_.stbody_.membmpno[i][i2], stMG_.stbody_.caponly_f[i][i2], (i2 == 1));
        }
      }
    }



  }

}
