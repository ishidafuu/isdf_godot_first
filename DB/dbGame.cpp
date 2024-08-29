#include "dbGame.h"


namespace db
{
  //コンストラクタ
  TGame::TGame(TMgEngineObj* pmgEO)
    : TGameBase(pmgEO, (u32)sizeof(mgSG_)),
    mgGO_(this),
    pmgEO_(pmgEO)
  {
    //真ステート保存情報
    mid::midInitGameSt(&mgSG_, (u32)sizeof(mgSG_));
  }

  TGame::~TGame()
  {

  }

  //レフェリーの持つ次シーン
  enSceneName TGame::GetNextScene()
  {
    return mgSG_.stRf_.NextScene;
  }

  BOOL TGame::IsWin()
  {
    return (mgSG_.stRf_.WinSide == SIDE0);
  }
  BOOL TGame::IsGameSet()
  {
    return (mgSG_.stRf_.GameSet_f);
  }
  BOOL TGame::IsTimeOver()
  {
    return (mgSG_.stRf_.LimitTime == 0);
  }

  //チュート用結果取得
  s32 TGame::GetTutoAct()
  {
    if (mgSG_.stRf_.TutoSend_f)
    {
      return mgSG_.stRf_.TutoSend;
    }
    else
    {
      return NGNUM;
    }
  }
  //プレビュー用結果取得
  BOOL TGame::IsPrevReturn()
  {
    BOOL res = FALSE;
    if (mgSG_.stRf_.prevreturn_f)
    {
      mgSG_.stRf_.prevreturn_f = FALSE;//とりあえずここでクリア
      res = TRUE;
    }
    return res;
  }
  //UGTutoからの情報受け取り
  void  TGame::SetTutoAct(s32 tutoact)
  {
    mgSG_.stRf_.TutoRcv = tutoact;
  }
  void  TGame::SetTutoGameAct()
  {
    mgSG_.stRf_.TutoGameAct_f = TRUE;//実戦開始
  }

  //初期化
  void TGame::DoReady()
  {
  }

  //各シーンごとのゲームオブジェの準備
  void TGame::TexRerease()
  {
  }

  void TGame::Ready_Title()
  {

  }

  void TGame::Ready_Shiai()
  {
    //mid,base関連（ループカウンタなど）初期化
    TGameBase::Ready();

    //パッド初期化
    pmgEO_->stPad_.Init();
    pmgEO_->stPad_.SetPadSide(0, 0);//ここでセットしておくことでマニュアル操作になる

    //コートロード

    pmgEO_->mgGrp_.LoadCourt(pmgEO_->stShiai_.GetMidCourt());

    //ステータス初期化
	mgSG_.Init(pmgEO_->stShiai_.GetKantokuID(SIDE0),
	  pmgEO_->stShiai_.GetMidTeam(SIDE0),
      pmgEO_->stShiai_.GetIDTeam(SIDE0),
	  pmgEO_->stShiai_.GetKantokuID(SIDE1),
	  pmgEO_->stShiai_.GetMidTeam(SIDE1),
      pmgEO_->stShiai_.GetIDTeam(SIDE1));//チームのＩＤをセット

    BOOL gbcourt_f = FALSE;//とりあえずＧＢコートによるすべてＧＢカラーは無くす
    //BOOL gbcourt_f = (pmgEO_->mgDt_.dtCourt_.GetDtPK(pmgEO_->stShiai_.GetMidCourt(), enDtCourt_gb) != 0);
    //キャラデータセット
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      s32 mid_team = pmgEO_->stShiai_.GetMidTeam(i);
      BOOL gb_f = (pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_gb_f) || gbcourt_f);
      for (s32 c = 0; c < DBMEMBER_ALL; ++c)
      {
        s32 charid = pmgEO_->mgCoM_.GetCharID(mid_team, c);
        //s32 id = pmgEO_->mgDt_.dtTeam_.GetCharID(mid_team, c);//てきとう
        pmgEO_->mgGrp_.pmgChGrp_[i][c]->SetCharDt(charid, mid_team, gb_f);
        pmgEO_->mgGrp_.pmgChGrp_[i][c]->SetGrTile(i, c);
      }
    }

    //ゲームオブジェ初期化
    if (pmgEO_->stShiai_.IsTour())
    {
      mgGO_.Ready(goShiaiTour);
    }
    else
    {
      mgGO_.Ready(goShiai);
    }

    //統合テクスチャ作成
    s32 rrr = pmgEO_->mgDt_.dtCourt_.GetDtPK(pmgEO_->stShiai_.GetMidCourt(), enDtCourt_line_r);
    s32 ggg = pmgEO_->mgDt_.dtCourt_.GetDtPK(pmgEO_->stShiai_.GetMidCourt(), enDtCourt_line_g);
    s32 bbb = pmgEO_->mgDt_.dtCourt_.GetDtPK(pmgEO_->stShiai_.GetMidCourt(), enDtCourt_line_b);
    s32 aaa = pmgEO_->mgDt_.dtCourt_.GetDtPK(pmgEO_->stShiai_.GetMidCourt(), enDtCourt_line_a);
    pmgEO_->mgGrp_.MakeGameTexture(rrr, ggg, bbb, aaa);//ラインカラーを引いてくる

    //キャラ名テクスチャ作成
    mgGO_.pmgCommon_->MakeCharNameTex();
  }

  void TGame::Ready_Prev(BOOL needLoad_f, s64 mid_team, s64 id_team, s32 char0, s32 char1, s32 char2, s32 char3, s32 char4)
  {
    //mid,base関連（ループカウンタなど）初期化
    TGameBase::Ready();

    //パッド初期化
    pmgEO_->stPad_.Init();
    pmgEO_->stPad_.SetPadSide(0, 0);//ここでセットしておくことでマニュアル操作になる

    //コートロード
    //pmgEO_->mgGrp_.LoadCourt(0, mid::ldm_Def);

    //デフォ
    if (char0 == char1)
    {
      char0 = 0;
      char1 = 1;
      char2 = 2;
      char3 = 3;
      char4 = 4;
    }

	s64 id_kantoku = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(enmdKantoku::mdkantoku_pk);
    //ステータス初期化（同一チーム
    mgSG_.Init(id_kantoku, mid_team, id_team, id_kantoku, mid_team, id_team);//チームのＩＤをセット
    BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_gb_f);
    //キャラデータセット(自チームキャプテン副キャプテン
    pmgEO_->mgGrp_.pmgChGrp_[SIDE0][0]->SetCharDt(pmgEO_->mgCoM_.GetCharID(mid_team, char0), mid_team, gb_f);
    pmgEO_->mgGrp_.pmgChGrp_[SIDE1][0]->SetCharDt(pmgEO_->mgCoM_.GetCharID(mid_team, char1), mid_team, gb_f);
    s32 side1num = 1;
    if (char2 > 0)
    {
      side1num = 4;
      pmgEO_->mgGrp_.pmgChGrp_[SIDE1][1]->SetCharDt(pmgEO_->mgCoM_.GetCharID(mid_team, char2), mid_team, gb_f);
      pmgEO_->mgGrp_.pmgChGrp_[SIDE1][2]->SetCharDt(pmgEO_->mgCoM_.GetCharID(mid_team, char3), mid_team, gb_f);
      pmgEO_->mgGrp_.pmgChGrp_[SIDE1][3]->SetCharDt(pmgEO_->mgCoM_.GetCharID(mid_team, char4), mid_team, gb_f);
    }

    for (s32 i = 0; i < DBSIDE; ++i)
    {
      for (s32 i2 = 0; i2 < DBMEMBER_INF; ++i2)
      {
        if (i2 >= side1num) break;
        pmgEO_->mgGrp_.pmgChGrp_[i][i2]->SetGrTile(i, i2);
        if (i == SIDE0) break;//左は一人
      }
    }

    

    //ゲームオブジェ初期化
    mgGO_.Ready(goPrev);//プレビュー

    //統合テクスチャ作成（チームが変わってないときは読む必要なし）
    if (needLoad_f) pmgEO_->mgGrp_.MakeGameTexture_Prev();//プレビュー

    //キャラ名テクスチャ作成
    //mgGO_.pmgCommon_->MakeCharNameTex();
  }

  //チュートリアル
  void TGame::Ready_Tuto(BOOL retro_f)
  {
    //mid,base関連（ループカウンタなど）初期化
    TGameBase::Ready();

    //パッド初期化
    pmgEO_->stPad_.Init();
    pmgEO_->stPad_.SetPadSide(0, 0);//ここでセットしておくことでマニュアル操作になる

    //コートロード
    pmgEO_->mgGrp_.LoadCourt(pmgEO_->stShiai_.GetMidCourt());
    //ステータス初期化
	mgSG_.Init(pmgEO_->stShiai_.GetKantokuID(SIDE0),
		pmgEO_->stShiai_.GetMidTeam(SIDE0),
      pmgEO_->stShiai_.GetIDTeam(SIDE0),
	  pmgEO_->stShiai_.GetKantokuID(SIDE1),
	  pmgEO_->stShiai_.GetMidTeam(SIDE1),
      pmgEO_->stShiai_.GetIDTeam(SIDE1));//チームのＩＤをセット


    //キャラデータセット
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      s32 mid_team = pmgEO_->stShiai_.GetMidTeam(i);
      BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_gb_f);
      for (s32 c = 0; c < DBMEMBER_ALL; ++c)
      {
        s32 charid = pmgEO_->mgCoM_.GetCharID(mid_team, c);
        //s32 id = pmgEO_->mgDt_.dtTeam_.GetCharID(mid_team, c);//てきとう
        pmgEO_->mgGrp_.pmgChGrp_[i][c]->SetCharDt(charid, mid_team, gb_f);
        pmgEO_->mgGrp_.pmgChGrp_[i][c]->SetGrTile(i, c);
      }
    }

    //ゲームオブジェ初期化//チュートリアル
    enGOType rtype;
    if (pmgEO_->stSeq_.IsFirstTuto())
    {
      rtype = goTutoFirst;
    }
	else if(pmgEO_->stSeq_.IsTutoAuto())
	{
		rtype = goTutoAuto;
	}
    else
    {
      rtype = (retro_f)
        ? goTutoRetro
        : goTutoSmart;
    }
    mgGO_.Ready(rtype);


    //統合テクスチャ作成
    pmgEO_->mgGrp_.MakeGameTexture(0xFF, 0xFF, 0xFF, 0xc0);

    //キャラ名テクスチャ作成
    mgGO_.pmgCommon_->MakeCharNameTex();
  }

  void TGame::Ready_VS()
  {
    //mid,base関連（ループカウンタなど）初期化
    TGameBase::Ready();

    //パッド初期化

    //対戦なので両者
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      pmgEO_->stPad_.SetPadSide(i, i);//ここでセットしておくことでマニュアル操作になる
    }

    //コートロード
    pmgEO_->mgGrp_.LoadCourt(pmgEO_->stShiai_.GetMidCourt());

    //ステータス初期化
	mgSG_.Init(pmgEO_->stShiai_.GetKantokuID(SIDE0),
		pmgEO_->stShiai_.GetMidTeam(SIDE0),
      pmgEO_->stShiai_.GetIDTeam(SIDE0),
	  pmgEO_->stShiai_.GetKantokuID(SIDE1),
	  pmgEO_->stShiai_.GetMidTeam(SIDE1),
      pmgEO_->stShiai_.GetIDTeam(SIDE1));//チームのＩＤをセット


    //キャラデータセット
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      s32 mid_team = pmgEO_->stShiai_.GetMidTeam(i);
      BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_gb_f);
      for (s32 c = 0; c < DBMEMBER_ALL; ++c)
      {
        s32 charid = pmgEO_->mgCoM_.GetCharID(mid_team, c);
        //s32 id = pmgEO_->mgDt_.dtTeam_.GetCharID(mid_team, c);//てきとう
        pmgEO_->mgGrp_.pmgChGrp_[i][c]->SetCharDt(charid, mid_team, gb_f);
        pmgEO_->mgGrp_.pmgChGrp_[i][c]->SetGrTile(i, c);
      }
    }

    //ゲームオブジェ初期化
    mgGO_.Ready(goShiai);

    //統合テクスチャ作成
    s32 rrr = pmgEO_->mgDt_.dtCourt_.GetDtPK(pmgEO_->stShiai_.GetMidCourt(), enDtCourt_line_r);
    s32 ggg = pmgEO_->mgDt_.dtCourt_.GetDtPK(pmgEO_->stShiai_.GetMidCourt(), enDtCourt_line_g);
    s32 bbb = pmgEO_->mgDt_.dtCourt_.GetDtPK(pmgEO_->stShiai_.GetMidCourt(), enDtCourt_line_b);
    s32 aaa = pmgEO_->mgDt_.dtCourt_.GetDtPK(pmgEO_->stShiai_.GetMidCourt(), enDtCourt_line_a);
    pmgEO_->mgGrp_.MakeGameTexture(rrr, ggg, bbb, aaa);//ラインカラーを引いてくる

    //キャラ名テクスチャ作成
    mgGO_.pmgCommon_->MakeCharNameTex();
  }

  //復活
  void TGame::Retry()
  {
    //mid,base関連（ループカウンタなど）初期化
    //TGameBase::Ready();
    //ステータス初期化
	  mgSG_.Retry(pmgEO_->stShiai_.GetKantokuID(SIDE0),
		pmgEO_->stShiai_.GetMidTeam(SIDE0),
      pmgEO_->stShiai_.GetIDTeam(SIDE0));

    //ゲームオブジェ初期化
    mgGO_.Retry();
  }
  //切断
  void TGame::Dissconnect()
  {
    pmgEO_->stPad_.SetPadCOM(mid::midGetPeerIdx());//ここでセットしておくことでマニュアル操作になる

    mgGO_.Dissconnect();
  }
  //更新
  void TGame::DoUpdate()
  {
    mgGO_.Update();

  }
  //描画
  void TGame::DoDraw()
  {
    mgGO_.Draw();
  }
}
