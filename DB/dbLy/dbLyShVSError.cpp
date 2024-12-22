#include "dbLyShVSError.h"
#include "dbSnShiai.h"

namespace db
{

  enum enPage
  {
    pg_VSError,
    //pg_Idle,
  };

  enum enConnect
  {
    cn_vserror,
  };

  //コンストラクタ
  TLyShVSError::TLyShVSError(TSnShiai* pScene) 
    :TLyShGen(pScene)
  {
  }
  
  TLyShVSError::~TLyShVSError()
  {
  }

  void TLyShVSError::DoLoad()
  {
    field_->g_SetDepth(UIDP_02BGOBJHI);//手前

    filter_ = MakeObj();
    filter_->o_SetParent(field_);
    filter_->o_SetObjFilterGrp(mid::midFullDotL(), mid::midFullDotS(), 0x80, 0, 0, 0);
    filter_->g_SetARGB(TRUE, 0x80, 0x00, 0x00, 0x00);
    filter_->g_SetCenter(TRUE);
  }
  void TLyShVSError::DoReady()
  {
    //接続エラーメッセージ
    SetHudGKErrorMsg();
  }
  void TLyShVSError::DoDown()
  {
  }
  void TLyShVSError::DoChangePage(s32 pageno)
  {

  }
  void TLyShVSError::MvPage_Error()
  {
    if  (IsHudOK())
    {
      Post_VSError();
    }
  }
  //POST
  void TLyShVSError::Post_VSError()
  {
    if (pmgEO_->stSeq_.IsNoSv())
    {
      Connect_VSError();
    }
    else
    {
      StartConnect(cn_vserror);
      pmgEO_->mgPo_.poHmShiai_.Ready_vshumretire(mid::midGetSyncErrorCode() + (mid::midGetGKErrorCode() *10),
                                                 mid::midGetNGNum(),
                                                 mid::midGetMyBM(),
                                                 mid::midGetEnBM(),
                                                 pmgEO_->stShiai_.GetCtrl());//win:0 lose : 1 draw : 2
    }
  }
  //接続
  void TLyShVSError::Connect_VSError()
  {
    Down();
    pScene_->lyShBar_->MoveLayer(shl_Home);
    SetHudEnd();
  }
  //接続完了
  void TLyShVSError::DoConnect(BOOL rqok_f)
  {
    if (rqok_f)//ＯＫのみ
    {
      //code0
      if (pmgEO_->mgPo_.IsCodeZero())
      {
        switch (stLy_.connect_)
        {
        case cn_vserror: Connect_VSError(); break;
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


  void TLyShVSError::DoUpdate(BOOL draw_f)
  {
    filter_->Update(draw_f);

    if (draw_f == FALSE)
    {
      switch (stLy_.pageNo_)
      {
      case pg_VSError:MvPage_Error(); break;
      }
    }
  }
}
