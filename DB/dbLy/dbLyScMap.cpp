#include "dbLyScMap.h"
#include "dbSnScenario.h"

const s32 TESTPANELNUM = 4;

namespace db
{
  const s32 SELECTPOSX = 80;
  const s32 SELECTPOSY = -16;
  const s32 BASEX = -160;
  const s32 BASEY = -80;

  const s32 PLBASEX = 30;
  const s32 PLBASEY = -4;
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



  const u32 STARTPOS = 0x1;
  const u32 RETROTUTO = 0x2;
  const u32 SMARTTUTO = 0x4;
  const u32 AUTOTUTO = 0x8;

  const s32 CHILD = 3;//1× 2親 3長 4子

  enum enDepth
  {
    dpPanel,
    dpBosspanel,
    dpBridge,
    dpRoad,
    dpEnemy,
    dpPlayer,
    dpOKBtn,
  };

  enum enPage
  {
    pg_Visit,
    pg_Comp,
    pg_NextPanel,
    pg_SelectPanel,
    pg_Enemy,
    pg_BackSelect,
  };


  //コンストラクタ
  TLyScMap::TLyScMap(TSnScenario* pScene) :TLyScGen(pScene)
    , ugkantoku_(this)
    , ugcap_(this)
  {
    stMap_.Init();
  }

  TLyScMap::~TLyScMap()
  {
    ClearPanelVec();

    //ボス画像クリア
    //    pmgEO_->mgGrp_.ClearBossGrp();
    pmgEO_->mgGrp_.ClearBossSt();
  }

  void TLyScMap::DoLoad()
  {
    //field_->sc_CamSwipe(TRUE);//スワイプオン

    //スワイプ用オブジェ
    map_ = MakeObj();
    map_->o_SetParent(field_);
    map_->sc_CamSwipe(TRUE);//スワイプオン
    //カメラ位置用オブジェ
    camera_ = MakeObj();
    ugkantoku_.MakeGroup(map_, FALSE);
    ugkantoku_.SetPos(PLBASEX + PLX, PLBASEY);
    ugkantoku_.SetMType(kmt_Ds);

    //ugcap_.SetupMgCharGrp();
    //ugcap_.MakeGroup(map_);
    //ugcap_.SetPos(PLBASEX + PLX - 26, PLBASEY);
    //ugkantoku_.SetCaptain(&ugcap_);
  }
  void TLyScMap::DoResetBtn()
  {
    SetFoot_BtnHd();
    ugbtn_back_->SetFont("もどる");
    ugbtn_back_->SetDraw(TRUE);
  }
  void TLyScMap::DoReady()
  {
    if (pmgEO_->stSeq_.GetMidEvent() == 0)
    {
      stMap_.nowmapNo_ = 1;//通常マップは１から
    }
    else
    {
      stMap_.nowmapNo_ = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(pmgEO_->stSeq_.GetMidEvent(), mdm_event_mapNo);//イベントマップは指定番号から
    }

    stMap_.nextmapvecno_ = NGNUM;
    stMap_.lasthome_f_ = (pScene_->GetLastScene() == snHome); //pmgEO_->stSeq_.PopFromHome(); //ホームから来たフラグをたたむ


    ClearPanelVec();

    //ボス画像作成用ベクタのクリア
    pmgEO_->mgGrp_.ClearBossSt();

    //マップパネル
    //パネル追加（マスターデータから追加するパネルの情報をもってくる）ここでは初期化のみ
    //とりあえずテストで全行
    BOOL startpos_f = FALSE;
    s32 startpos = 0;
    s32 standpos = NGNUM;

    //最後にクリアしたマップチップ
    s64 lastmaptip = 0;
    if (stMap_.lasthome_f_ == FALSE)
    {
      for (s32 i = 0; i < pmgEO_->mgCoU_.mdMap_.GetLineNum(); ++i)
      {
        if (pmgEO_->mgCoU_.mdMap_.GetLineDt(i, mdmap_mapNo) == stMap_.nowmapNo_)
        {
          lastmaptip = pmgEO_->mgCoU_.mdMap_.GetLineDt(i, mdmap_id_m_maptip);
          break;
        }
      }
    }

    for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_Maptip_.GetLineNum(); ++i)
    {
      //マップが違う
      if (pmgEO_->mgCoM_.mdm_Maptip_.GetLineDt(i, mdm_maptip_mapNo) != stMap_.nowmapNo_) continue;

      TUGScPanel* newpanel = new TUGScPanel(this);

      newpanel->MakeGroup(map_, stMap_.nowmapNo_, i, &ugpanelvec_);//行番号で保存

      //スタート位置//ホームから来たときはデフォ位置

      BOOL startpos_f = lib_num::IsFlags(newpanel->GetMDt(mdm_maptip_start_f), STARTPOS);
      if (startpos_f) startpos = ugpanelvec_.size();

      if (stMap_.lasthome_f_ == FALSE)
      {
        //試合後は最終選択位置
        if (newpanel->GetMIDMapTip() == lastmaptip) standpos = ugpanelvec_.size();
      }

      ugpanelvec_.push_back(newpanel);
    }

    //現状を反映したマップの開放
    OpenMap(startpos);

    //初期位置
    if (standpos == NGNUM) standpos = startpos;
    s32 posx = ugpanelvec_[standpos]->GetMDt(mdm_maptip_posX)* PANELDISTX;
    s32 posy = ugpanelvec_[standpos]->GetMDt(mdm_maptip_posY)* PANELDISTY;

    //異動先
    if (pmgEO_->stResult_.GetVsResult() == rtWIN)
    {
      NextMapTip(standpos);
    }

    ugkantoku_.SetPos(posx + PLBASEX + PLX, posy + PLBASEY);
    camera_->z_SetPos(posx, posy);
    map_->sc_CamSetPos(posx, posy);
    //スクロール画面サイズ
    SetScrollSize();

    //押せるものはボタンになる
    for (s32 i = 0; i < ugpanelvec_.size(); ++i)
    {
      ugpanelvec_[i]->SetPanelButton(TRUE);
    }

    pmgEO_->mgGrp_.MakeBossTexture();//テクスチャ作成


    SetFoot_BtnHd();
    ResetBtn();
  }
  void TLyScMap::DoDown()
  {
  }
  void TLyScMap::ClearPanelVec()
  {
    for (s32 i = 0; i < ugpanelvec_.size(); ++i) Freemem(ugpanelvec_[i]);
    ClearGrp();//読み込んだパネル画像pgrvec_をクリア
  }
  void TLyScMap::SetScrollSize()
  {
    BOOL zeropos_f = TRUE;
    //マップパネル
    for (s32 i = 0; i < ugpanelvec_.size(); ++i)
    {
      //まだ空いてないのもチェック★
      if (ugpanelvec_[i]->GetAreaState() == as_None)continue;

      s32 posx = ugpanelvec_[i]->GetMDt(mdm_maptip_posX);
      s32 posy = ugpanelvec_[i]->GetMDt(mdm_maptip_posY);
      if (zeropos_f || (posx < stMap_.left_)) stMap_.left_ = posx;
      if (zeropos_f || (posx > stMap_.right_)) stMap_.right_ = posx;
      if (zeropos_f || (posy < stMap_.top_)) stMap_.top_ = posy;
      if (zeropos_f || (posy > stMap_.bottom_)) stMap_.bottom_ = posy;

      zeropos_f = FALSE;
    }

    map_->sc_CamSwipeData(
      (stMap_.left_ * PANELDISTX) - PANELDISTX,
      (stMap_.top_ * PANELDISTY) - PANELDISTY,
      (stMap_.right_ * PANELDISTX) + PANELDISTX,// + BASEX - 8,
      (stMap_.bottom_ * PANELDISTY) + PANELDISTY// + BASEY - PANELDISTY- PANELDISTY);//広がるたび範囲拡大する
      );
  }

  void TLyScMap::OpenChild(s64 posNo, BOOL fc_f)
  {

    if (fc_f && (posNo == 0)) return; //これがあるとpos0を開くことが出来ない 遠子は0が空なので

    for (s32 i2 = 0; i2 < ugpanelvec_.size(); ++i2)
    {
      //if (ugpanelvec_[i2]->GetIDMapTip() == mid_maptip)
      if (ugpanelvec_[i2]->GetMDt(mdm_maptip_posNo) == posNo)
      {
        //子がクリアしてたらそこでストップ。この子のチェックで以降を行う
        if (ugpanelvec_[i2]->IsClear()) break;

        //開いた場合その子もチェック
        if (ugpanelvec_[i2]->CheckParent())
        {
          OpenChildlen(ugpanelvec_[i2]);
        }

        break; //posNoは一つしかないのでこれはブレイクして良いかと
      }
    }
  }
  void TLyScMap::OpenChildXY(s32 posx, s32 posy)
  {
    OpenChild(pmgEO_->mgCoM_.GetMapTipPosNo(stMap_.nowmapNo_, posx, posy), FALSE);
  }
  void TLyScMap::OpenChildlen(TUGScPanel* parent)
  {
    s32 posx = parent->GetMDt(mdm_maptip_posX);
    s32 posy = parent->GetMDt(mdm_maptip_posY);

    //４方向の子と遠子に命令を送る
    if (parent->GetMDt(mdm_maptip_roadUp) >= CHILD)      OpenChildXY(posx, posy - 1);
    if (parent->GetMDt(mdm_maptip_roadDown) >= CHILD) OpenChildXY(posx, posy + 1);
    if (parent->GetMDt(mdm_maptip_roadLeft) >= CHILD)    OpenChildXY(posx - 1, posy);
    if (parent->GetMDt(mdm_maptip_roadRight) >= CHILD) OpenChildXY(posx + 1, posy);
    OpenChild(parent->GetMDt(mdm_maptip_fc0), TRUE);
    OpenChild(parent->GetMDt(mdm_maptip_fc1), TRUE);
    OpenChild(parent->GetMDt(mdm_maptip_fc2), TRUE);
    OpenChild(parent->GetMDt(mdm_maptip_fc3), TRUE);
    OpenChild(parent->GetMDt(mdm_maptip_fc4), TRUE);
    OpenChild(parent->GetMDt(mdm_maptip_fc5), TRUE);
    OpenChild(parent->GetMDt(mdm_maptip_fc6), TRUE);
    OpenChild(parent->GetMDt(mdm_maptip_fc7), TRUE);
  }
  void TLyScMap::NextMapTip(s32 standpos)
  {


    //立ち位置が設定されてない（試合後ではない）
    if (standpos == NGNUM) return;

    TUGScPanel& panel = *ugpanelvec_[standpos];

    s32 posx = panel.GetMDt(mdm_maptip_posX);
    s32 posy = panel.GetMDt(mdm_maptip_posY);
    s32 cposx = posx;
    s32 cposy = posy;
    //長兄の位置
    if (panel.GetMDt(mdm_maptip_roadUp) == CHILD) --cposy;
    if (panel.GetMDt(mdm_maptip_roadDown) == CHILD) ++cposy;
    if (panel.GetMDt(mdm_maptip_roadLeft) == CHILD) --cposx;
    if (panel.GetMDt(mdm_maptip_roadRight) == CHILD) ++cposx;

    s32 mid_maptip_child = pmgEO_->mgCoM_.GetMapTipPK(stMap_.nowmapNo_, cposx, cposy);
    s32 i = 0;
    for (auto& item: ugpanelvec_)
    {
      if (item->GetMIDMapTip() == mid_maptip_child)
      {
        //親がすべてクリアされてる
        BOOL allclear_f = TRUE;
        allclear_f &= item->IsClearParent(item->GetMDt(mdm_maptip_fp0));
        allclear_f &= item->IsClearParent(item->GetMDt(mdm_maptip_fp1));
        allclear_f &= item->IsClearParent(item->GetMDt(mdm_maptip_fp2));
        allclear_f &= item->IsClearParent(item->GetMDt(mdm_maptip_fp3));
        allclear_f &= item->IsClearParent(item->GetMDt(mdm_maptip_fp4));
        allclear_f &= item->IsClearParent(item->GetMDt(mdm_maptip_fp5));
        allclear_f &= item->IsClearParent(item->GetMDt(mdm_maptip_fp6));
        allclear_f &= item->IsClearParent(item->GetMDt(mdm_maptip_fp7));
        //それ自体はクリアされてない
        if (allclear_f && (item->IsClear() == FALSE))
        {
          stMap_.nextmapvecno_ = i;
        }
        break;
      }
      ++i;
    }


    ////テスト
    //for (s32 i = 0; i < ugpanelvec_.size(); ++i)
    //{
    //  if (ugpanelvec_[i]->GetMIDMapTip() == mid_maptip_child)
    //  {
    //    stMap_.nextmapvecno_ = i;
    //    break;
    //  }
    //}
  }
  void TLyScMap::OpenMap(s32 startpos)
  {

    //１行目はスタート地点としてオープン
    //OpenChild(ugpanelvec_[startpos]->GetMDt(mdm_maptip_posNo), TRUE);
    //子がクリアしてたらそこでストップ。この子のチェックで以降を行う
    if (ugpanelvec_[startpos]->IsClear() == FALSE)
    {
      //開いた場合その子もチェック
      if (ugpanelvec_[startpos]->CheckParent())
      {
        OpenChildlen(ugpanelvec_[startpos]);
      }
    }

    //クリアチェック
    for (s32 i = 0; i < ugpanelvec_.size(); ++i)
    {
      s64 mid_maptip = ugpanelvec_[i]->GetMIDMapTip();
      //クリアチェック
      for (s32 i2 = 0; i2 < pmgEO_->mgCoU_.mdMaptip_.GetLineNum(); ++i2)
      {
        if (pmgEO_->mgCoU_.mdMaptip_.GetLineDt(i2, mdmaptip_id_m_maptip) == mid_maptip)
        {
          ugpanelvec_[i]->SetClear();
          break;
        }
      }
    }

    //クリアチェック
    if (pmgEO_->stSeq_.IsNoSv())
    {
      for (s32 i = 0; i < ugpanelvec_.size(); ++i)
      {
        s64 mid_maptip = ugpanelvec_[i]->GetMIDMapTip();
        //クリアチェック
        ugpanelvec_[i]->SetClear();

      }
    }

    //オープンチェック
    for (s32 i = 0; i < ugpanelvec_.size(); ++i)
    {
      //クリアのみ
      if (ugpanelvec_[i]->IsClear() == FALSE) continue;
      //      OpenSelf(ugpanelvec_[i]);
      OpenChildlen(ugpanelvec_[i]);
    }
  }
  void TLyScMap::Return()
  {
    ChangePage(pg_BackSelect);
  }

  BOOL TLyScMap::IsCheckHP(void)
  {
    BOOL ret = FALSE;

    s32 hp = 0;
    s32 needHP = ugpanelvec_[stMap_.selectMapNo_]->GetMDt(mdm_maptip_staminaMin);
#ifdef __K_DEBUG_USER_DATA__
    // デバッグモード時デバッグ用データから取得
    hp = kdebug::DebugUDManager::GetInstance()->GetHP();
#else
    // 本番環境ではサーバから監督のHP取得
    const s32 MINTOSEC = 60;
    const s32 MAXSTA = 100;

    s64 nowtime = mid::midNowTime();
    s64 endtime = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_login_date);
    s64 spendtime = (nowtime - endtime);
    if (spendtime < 0) spendtime = 0;

    s64 nowstamina = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_stamina) - spendtime;
    if (nowstamina < 0) nowstamina = 0;

    hp = MAXSTA - ((nowstamina + (MINTOSEC - 1)) / MINTOSEC);
#endif
    if (hp < needHP)
    {
      ret = TRUE;
    }
    return ret;
  }

  void TLyScMap::Pause(BOOL pause_f)
  {
    if (stMap_.paused_f_)
    {
      //map_->sc_CamSwipe(FALSE);
    }
    else
    {
      if (stMap_.paused_f_ != pause_f)
      {
        switch (stLy_.pageNo_)
        {
        case db::pg_Enemy: break;
        default:
          //map_->sc_CamSwipe(TRUE);
          break;
        }
      }
    }
    stMap_.paused_f_ = pause_f;
  }
  void TLyScMap::DoChangePage(s32 pageno)
  {
    switch (pageno)
    {
    case db::pg_Visit:
      break;
    case db::pg_Comp:
      SetFoot_None();
      SetHudItemView_Drop(
        pmgEO_->stDrop_.GetItemType(TRUE),
        pmgEO_->stDrop_.GetItemID(TRUE),
        pmgEO_->stDrop_.GetItemNum(TRUE),
        NGNUM,
        NGNUM,
        TRUE);
      break;
    case db::pg_SelectPanel:
      ResetBtn();
      break;
    case db::pg_Enemy:
      break;
    case db::pg_BackSelect:
      break;
    default:
      break;
    }
  }

  void TLyScMap::MvPage_Visit()
  {
    if (pmgEO_->stDrop_.PopCmp())//コンプフラグここでたたむ
    {
      ChangePage(pg_Comp);
    }
    else
    {
      if ((stMap_.lasthome_f_ == FALSE) && (stMap_.nextmapvecno_ != NGNUM))
      {
        map_->sc_CamSetPos(
          (ugkantoku_.GetField()->stSO_.stZahyou_.x_),
          (ugkantoku_.GetField()->stSO_.stZahyou_.y_));

        if (lib_num::UpToR(&stMap_.next_c_, 30))
        {
          const s32 MVSPDNEXT = 3;
          const s32 EATNEXT = 120;
          ChangePage(pg_NextPanel);
          s32 posx = ugpanelvec_[stMap_.nextmapvecno_]->stScPanel_.posX_* PANELDISTX;
          s32 posy = ugpanelvec_[stMap_.nextmapvecno_]->stScPanel_.posY_* PANELDISTY;
          ugkantoku_.GetField()->z_MoveBrk(posx + PLBASEX + PLX, posy + PLBASEY, MVSPDNEXT, EATNEXT);
          if (posx < ugkantoku_.GetField()->stSO_.stZahyou_.x_) ugkantoku_.SetMirror(TRUE);//ムーンウォーク対応

          for (s32 i = 0; i < ugpanelvec_.size(); ++i)
          {
            ugpanelvec_[i]->SetPanelButton(FALSE);
          }
          map_->sc_CamSwipe(FALSE);//スワイプ切る
        }
        
      }
      else
      {
        ChangePage(pg_SelectPanel);
      }
    }
  }
  void TLyScMap::MvPage_Comp()
  {
    if (IsHudItemView())
    {
      ChangePage(pg_SelectPanel);
    }
  }
  void TLyScMap::MvPage_NextPanel()
  {
    //マップスクロール＆プレート選択
    if (ugkantoku_.GetField()->stSO_.stZahyou_.eat_ == 0)
    {
      ugkantoku_.SetMType(kmt_Ds);
      ugkantoku_.SetMirror(FALSE);//ムーンウォーク対応

      map_->sc_CamSwipe(TRUE);
      for (s32 i = 0; i < ugpanelvec_.size(); ++i)
      {
        ugpanelvec_[i]->SetPanelButton(TRUE);//道のみ以外はボタンにもどる
      }
      ChangePage(pg_SelectPanel);
    }
    map_->sc_CamSetPos(
      (ugkantoku_.GetField()->stSO_.stZahyou_.x_),
      (ugkantoku_.GetField()->stSO_.stZahyou_.y_));

  }
  void TLyScMap::MvPage_SelectPanel()
  {
    for (s32 i = 0; i < ugpanelvec_.size(); ++i)
    {
      //道は飛ばす
      if (ugpanelvec_[i]->stScPanel_.roadpanel_f_) continue;

      //パネル選択
      if (ugpanelvec_[i]->IsAct())
      {
        //時間切れ
        if (pmgEO_->mgCoU_.IsOverTimeEvent(pmgEO_->stSeq_.GetWidEvent()))
        {
          STRVECTOR strvec = { "イベントの＿じかんぎれです", "またきてね" };
          SetHudMsg(TRUE, FALSE, &strvec);
          break;
        }

        stMap_.selectMapNo_ = i;

        BOOL retrotuto_f = lib_num::IsFlags(ugpanelvec_[i]->GetMDt(mdm_maptip_start_f), RETROTUTO);
        BOOL smarttuto_f = lib_num::IsFlags(ugpanelvec_[i]->GetMDt(mdm_maptip_start_f), SMARTTUTO);
		BOOL autotuto_f = lib_num::IsFlags(ugpanelvec_[i]->GetMDt(mdm_maptip_start_f), AUTOTUTO);

        TStSnSeq::enTutoType tutotype = TStSnSeq::tt_None;
        if (retrotuto_f)
        {
          tutotype = TStSnSeq::tt_Retro;//レトロチュートリアル
        }
        else if (smarttuto_f)
        {
          tutotype = TStSnSeq::tt_Smart;//スマホチュートリアル
        }
		else if (autotuto_f)
		{
			tutotype = TStSnSeq::tt_Auto;//オートチュートリアル
		}

        //マップチップ準備
        pmgEO_->stSeq_.ReadyMapTip(ugpanelvec_[i]->GetMIDMapTip(), ugpanelvec_[i]->GetLNoMapTip(), ugpanelvec_[i]->IsClear(), tutotype);


        s32 posx = ugpanelvec_[i]->stScPanel_.posX_* PANELDISTX;
        s32 posy = ugpanelvec_[i]->stScPanel_.posY_* PANELDISTY;
        ugkantoku_.GetField()->z_MoveBrk(posx + PLBASEX + PLX, posy + PLBASEY, MVSPD, EATTIME);
        camera_->z_SetPos(map_->stSO_.stCamSw_.posX_, map_->stSO_.stCamSw_.posY_);
        camera_->z_MoveBrk(posx + SELECTPOSX, posy + SELECTPOSY, MVSPD, EATTIME);
        for (s32 i = 0; i < ugpanelvec_.size(); ++i)
        {
          ugpanelvec_[i]->SetPanelButton(FALSE);
        }
        map_->sc_CamSwipe(FALSE);//スワイプ切る
        ChangePage(pg_Enemy);

        pScene_->lyScBar_->MoveLayer(scl_Enemy);
        break;
      }
    }
  }
  void TLyScMap::MvPage_Enemy()
  {
    //マップスクロール＆プレート選択
    if (ugkantoku_.GetField()->stSO_.stZahyou_.eat_ == 0)
    {
      ugkantoku_.SetMType(kmt_St);
    }
    map_->sc_CamSetPos(
      (camera_->stSO_.stZahyou_.x_),
      (camera_->stSO_.stZahyou_.y_));
  }

  void TLyScMap::MvPage_BackSelect()
  {
    ugkantoku_.SetMType(kmt_Ds);
    map_->z_MoveBrk(0, 0, MVSPD, EATTIME);//左にずらしたところから戻る
    map_->sc_CamSwipe(TRUE);
    for (s32 i = 0; i < ugpanelvec_.size(); ++i)
    {
      ugpanelvec_[i]->SetPanelButton(TRUE);//道のみ以外はボタンにもどる
    }
    ChangePage(pg_SelectPanel);//無条件でマップ選択にもどる
  }

  void TLyScMap::DoUpdate(BOOL draw_f)
  {
    //ヘッダ動作時はポーズ
    Pause(pScene_->lyScBar_->IsBarMove());
    //描画の時以外はポーズ時抜ける
    if ((draw_f == FALSE) && stMap_.paused_f_) return;

    map_->Update(draw_f);

    switch (stLy_.pageNo_)
    {
    case db::pg_BackSelect:
      ugkantoku_.Update(draw_f);
      //ugcap_.Update(draw_f);
      for (s32 i = 0; i < ugpanelvec_.size(); ++i) ugpanelvec_[i]->Update(draw_f);
      break;
    default:
      ugkantoku_.Update(draw_f);
      //ugcap_.Update(draw_f);
      camera_->Update(draw_f);
      for (s32 i = 0; i < ugpanelvec_.size(); ++i) ugpanelvec_[i]->Update(draw_f);
      break;
    }

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case db::pg_Visit:MvPage_Visit(); break;
      case db::pg_Comp:MvPage_Comp();  break;
      case db::pg_NextPanel:MvPage_NextPanel();  break;
      case db::pg_SelectPanel:MvPage_SelectPanel();  break;
      case db::pg_Enemy:MvPage_Enemy(); break;
      case db::pg_BackSelect:MvPage_BackSelect(); break;
      }
    }
  }
}
