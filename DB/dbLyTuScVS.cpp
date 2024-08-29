#include "dbLyTuScVS.h"
#include "dbSnScenario.h"

namespace db
{
  enum enPage
  {
    pg_VS,
    pg_GotoShiai,
  };

  //コンストラクタ
  TLyTuScVS::TLyTuScVS(TSnScenario* pScene)
    :TLyScGen(pScene)
    , ugvs_(this)
  {
    for (s32 i = 0; i < DBSIDE; ++i) ugteam_.push_back(new TUGScVSTeam(this));
  }
  
  TLyTuScVS::~TLyTuScVS()
  {
    for (s32 i = 0; i < ugteam_.size(); ++i) Freemem(ugteam_[i]);
  }

  void TLyTuScVS::DoLoad()
  {
    for (s32 i = 0; i < DBSIDE; ++i) ugteam_[i]->MakeGroup(field_, i);
    
    //VS
    ugvs_.MakeGroup(field_);
  }


  void TLyTuScVS::DoReady()
  {

    //ＶＳ画面の両チーム情報をセット
    for (s32 i = 0; i < ugteam_.size(); ++i)
    {
      ugteam_[i]->Refresh();
    }

    //ＶＳ画面用の画像を読み込む
    for (s32 i = 0; i < DBSIDE; ++i)
    {
      s32 mid_team = pmgEO_->stShiai_.GetMidTeam(i);
      BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_gb_f);
      for (s32 c = 0; c < DBMEMBER_ALL; ++c)
      {
        s32 charid = pmgEO_->mgCoM_.GetCharID(mid_team, c);
        pmgEO_->mgGrp_.pmgChGrp_[i][c]->SetCharDt(charid, mid_team, gb_f);
        pmgEO_->mgGrp_.pmgChGrp_[i][c]->SetGrTile(i, c);
      }
      pmgEO_->mgGrp_.MakeIconTexture_Body(i, 1, 1, FALSE, FALSE);
    }
  }

  void TLyTuScVS::MvPage_VS()
  {
    if (field_->sc_IsCamEnter()) //さわったら即
    {
      ChangePage(pg_GotoShiai);
      pScene_->lyTuScBar_->MoveLayer(tscl_Shiai);// SetFilter_GoShiai();//->stLy_.lyphase_ = 3;//shiaiへ
    }
  }
  void TLyTuScVS::MvPage_GotoShiai()
  {

  }

  void TLyTuScVS::DoUpdate(BOOL draw_f)
  {
    ugvs_.Update(draw_f);
    for (s32 i = 0; i < ugteam_.size(); ++i) ugteam_[i]->Update(draw_f);

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case pg_VS:MvPage_VS(); break;
      case pg_GotoShiai:MvPage_GotoShiai(); break;
      }
    }
  }

}
