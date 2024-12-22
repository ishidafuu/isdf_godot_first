#include "dbLyShiai.h"
#include "dbSnShiai.h"
namespace db
{
  enum enPage
  {
    pg_Play,
    pg_Idle,
  };

  //コンストラクタ
  TLyShiai::TLyShiai(TSnShiai* pScene, TUGShiai* ugshiai) :TLyShiaiGen(pScene, ugshiai)
  {
  }
  
  TLyShiai::~TLyShiai()
  {
  }
  
  void TLyShiai::DoLoad()
  {
    //ugshiai_->MakeGroup(field_);//barが管理
  }
  void TLyShiai::DoReady()
  {
    //チュートと共存するためここにうつす
    ugshiai_->Ready_Shiai();//試合関係読み込み
  }
  void TLyShiai::DoIdle()
  {
    ChangePage(pg_Idle);
  }
  void TLyShiai::DoRestart()
  {
    ChangePage(pg_Play);
  }

  void TLyShiai::MvPage_Play()
  {
    if (ugshiai_->IsAct())
    {
      pmgEO_->stShiai_.SetCtrl(ugshiai_->GetCtrl());
      if (ugshiai_->IsActRes(enUGShiaiAct_pause))//ポーズ
      {
        pScene_->lyShBar_->MoveLayer(shl_Pause);
      }
      else if (ugshiai_->IsActRes(enUGShiaiAct_retry))//リトライ
      {
        pScene_->lyShBar_->MoveLayer(shl_Retry);
      }
      else if (ugshiai_->IsActRes(enUGShiaiAct_result))//結果
      {
        pScene_->lyShBar_->MoveLayer(shl_Result);
      }
    }
  }
  void TLyShiai::MvPage_Idle()
  {

  }
  void TLyShiai::DoUpdate(BOOL draw_f)
  {
    ugshiai_->Update(draw_f);

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case pg_Play:MvPage_Play(); break;
      case pg_Idle:MvPage_Idle(); break;
      }
    }
  }
}
