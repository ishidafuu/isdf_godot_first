#include "dbLyScEnemy.h"
#include "dbSnScenario.h"

namespace db
{
  const s32 TMNUM = 3;

  enum enPage
  {
    pg_Enemy,
    pg_GotoTSL,
    pg_Idle,
  };

  enum enConnect
  {
    cn_ready,
  };

  //コンストラクタ
  TLyScEnemy::TLyScEnemy(TSnScenario* pScene)
    :TLyScGen(pScene)
    , pCursor_(NULL)
    , cursorDispTimer_(0)
  {
    for (s32 i = 0; i < TMNUM; ++i) ugenteam_.push_back(new TUGScEnemy(this));
  }

  TLyScEnemy::~TLyScEnemy()
  {
    for (s32 i = 0; i < TMNUM; ++i) Freemem(ugenteam_[i]);
  }

  void TLyScEnemy::DoLoad()
  {
    field_->g_SetDepth(UIDP_02BGOBJHI);//手前

    filter_ = MakeObj();
    filter_->o_SetParent(field_);
    filter_->o_SetObjFilterGrp(mid::midFullDotL_Hf(), mid::midFullDotS(), 0x80, 0, 0, 0);
    filter_->g_SetARGB(TRUE, 0x80, 0x00, 0x00, 0x00);
    filter_->z_SetPos_SafeR(0, 0);

    pCursor_ = MakeObj();
    pCursor_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_finger));
    pCursor_->o_SetParent(field_);
    pCursor_->g_SetDraw(FALSE);
    pCursor_->z_SetPos(-120, -80);

    //チーム情報（ドロップなど）
    for (s32 i = 0; i < TMNUM; ++i)
    {
      ugenteam_[i]->MakeGroup(field_, i);
    }
  }
  void TLyScEnemy::DoResetBtn()
  {
    SetFoot_BtnHd();
    ugbtn_back_->SetFont("もどる");



    if (enablehome_f_)
    {
      ugbtn_mid_->SetFont("ＶＳホーム");
      ugbtn_ok_->SetFont("ＶＳアウェー");
      ugbtn_mid_->SetDraw(TRUE);
    }
    else
    {
      ugbtn_ok_->SetFont("たいけつ！");
      ugbtn_mid_->SetDraw(FALSE);
    }
    
    ugbtn_back_->SetDraw(TRUE);
    ugbtn_ok_->SetDraw(TRUE);
  }
  void TLyScEnemy::RedayEnemy(s64 mid_maptip, s32 lno_maptip)
  {
    Ready();
    //雑魚画像作成用ベクタのクリア
    pmgEO_->mgGrp_.ClearZakoSt();
    for (s32 i = 0; i < TMNUM; ++i)
    {
      ugenteam_[i]->Ready(mid_maptip, lno_maptip);//敵１のときは消すような処理を入れる
    }
    pmgEO_->mgGrp_.MakeZakoTexture();//テクスチャ作成
    ResetBtn();


    enablehome_f_ = ((pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_haikei) != 0)
      && (pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_bgm) != 0)
      && pmgEO_->stSeq_.IsClearTip());
  }

  //-------------------------------------------
  // HPが足りない演出
  //-------------------------------------------
  void TLyScEnemy::SetCheckHP(bool isSet)
  {
    //ugbtn_ok_->SetDraw(!isSet);
    //pCursor_->g_SetDraw(isSet);
    if (isSet)
    {
      ugbtn_ok_->SetFont("ＨＰふそく");
      ugbtn_mid_->SetDraw(FALSE);
    }
    else
    {
      if (enablehome_f_)
      {
        ugbtn_mid_->SetFont("ＶＳホーム");
        ugbtn_ok_->SetFont("ＶＳアウェー");
        ugbtn_mid_->SetDraw(TRUE);
      }
      else
      {
        ugbtn_ok_->SetFont("たいけつ！");
        ugbtn_mid_->SetDraw(FALSE);
      }
    }
    isCheckHP_ = isSet;
  }

  void TLyScEnemy::MvPage_Enemy()
  {
    //キャンセルボタンによって選択番号のマップを中心にするように移動、
    //パネル選択状態に状態遷移

    // HPが足りない状態か
    // 足りなければレイアウトをHPが足りないバージョンにセット
    bool isSet = pScene_->lyMap_->IsCheckHP();
    SetCheckHP(isSet);

    if (ugbtn_ok_->IsAct() || ugbtn_mid_->IsAct())
    {
      if (isCheckHP_)
      {
        cursorDispTimer_ = 60;
        pCursor_->g_SetDraw(TRUE);
      }
      else
      {
        pmgEO_->stSeq_.SetHomeCourt(ugbtn_mid_->IsAct());
        Post_Ready();
      }
    }
  }
  void TLyScEnemy::MvPage_GotoTSL()
  {

  }

  void TLyScEnemy::MvPage_Idle()
  {

  }

  //POST：対決！
  void TLyScEnemy::Post_Ready()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_Ready();//テスト★
    }
    else
    {
      StartConnect(cn_ready);
      pmgEO_->mgPo_.poScenario_.Ready_ready(pmgEO_->stSeq_.GetMidMapTip());
    }
  }
  //接続：敵選択完了
  void TLyScEnemy::Connect_Ready()
  {
    ChangePage(pg_GotoTSL);
    pScene_->lyScBar_->MoveLayer(scl_TSL);
  }

  //接続完了
  void TLyScEnemy::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_ready: Connect_Ready(); break;
        }
      }
      else
      {
        //エラーメッセージ
        EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
      }
    }
    else
    {
      //通信エラー
    }
  }


  //パネルが選択された
  void TLyScEnemy::DoUpdate(BOOL draw_f)
  {
    if ((draw_f == FALSE) && pScene_->lyScBar_->IsBarMove()) return;

    filter_->Update(draw_f);

    if (isCheckHP_)
    {
      if (cursorDispTimer_ < 0)
      {
        pCursor_->g_SetDraw(FALSE);
        cursorDispTimer_ = -1;
      }
      else
      {
        cursorDispTimer_--;
      }
    }
    pCursor_->Update(draw_f);

    //ugbtn_ok_->Update(draw_f);
    for (s32 i = 0; i < TMNUM; ++i) ugenteam_[i]->Update(draw_f);

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case pg_Enemy:MvPage_Enemy(); break;
      case pg_GotoTSL:MvPage_GotoTSL(); break;
      case pg_Idle:MvPage_Idle(); break;
      }
    }
  }

}
