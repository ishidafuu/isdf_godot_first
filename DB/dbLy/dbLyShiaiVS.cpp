#include "dbLyShiaiVS.h"
#include "dbSnShiai.h"
namespace db
{
  enum enPage
  {
    pg_Play,
    pg_Idle,
  };

  //コンストラクタ
  TLyShiaiVS::TLyShiaiVS(TSnShiai* pScene, TUGShiai* ugshiai) :TLyShiaiGen(pScene, ugshiai)
  {
    disconnect_f_ = FALSE;
  }

  TLyShiaiVS::~TLyShiaiVS()
  {
  }

  void TLyShiaiVS::DoLoad()
  {
    //ugshiai_->MakeGroup(field_);//barが管理
  }
  void TLyShiaiVS::DoReady()
  {
    //チュートと共存するためここにうつす
    ugshiai_->Ready_VS();//試合関係読み込み
    disconnect_f_ = FALSE;
    pmgEO_->stShiai_.SetDisConnect(disconnect_f_);
  }
  void TLyShiaiVS::DoRestart()
  {
    ChangePage(pg_Play);
  }
  void TLyShiaiVS::DoIdle()
  {
    ChangePage(pg_Idle);
  }
  void TLyShiaiVS::MvPage_Play()
  {
    if (disconnect_f_ == FALSE)
    {
      if (mid::midIsDisconnect())//エラーコード
      {
        mid::midSetDisconnect(FALSE);//エラーの時は切断する
        ugshiai_->Dissconnect();
        disconnect_f_ = TRUE;
        pmgEO_->stShiai_.SetDisConnect(disconnect_f_);
      }
      //pScene_->lyShBar_->MoveLayer(shl_VSError);
    }

    if (ugshiai_->IsAct())
    {
      pmgEO_->stShiai_.SetCtrl(ugshiai_->GetCtrl());
      if (ugshiai_->IsActRes(enUGShiaiAct_pause))//ポーズ
      {
        pScene_->lyShBar_->MoveLayer(shl_Pause);
      }
      else if (ugshiai_->IsActRes(enUGShiaiAct_result))//結果
      {
        pScene_->lyShBar_->MoveLayer(shl_Result);
      }
    }
  }
  void TLyShiaiVS::MvPage_Idle()
  {

  }
  void TLyShiaiVS::DoUpdate(BOOL draw_f)
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
