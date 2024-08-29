#pragma once
#include "dbLyTiGen.h"

namespace db
{

  struct TTitleSt
  {
    BOOL postinit_f_;
    s32 connectStep_;
    BOOL filter_f_;
    BOOL retry_f_;
    BOOL reloadclm_f_;
    s64 connect_;//接続先
    s64 re_clmver_;
    s64 re_svmver_;
    s64 re_bnver_;
    s64 re_evver_;
    BOOL cdn_f_;
    BOOL review_f_;
    BOOL waitdialog_f_;
    void Init()
    {
      Zeromem(this);
    }
  };


  class TLyTiTitle :public TLyTiGen
  {
  protected:
    //TUIObj* field_;//フィールド
    TUIObj* copywrite_;//コピーライト表示
    TUIObj* cver_;//クライアントビルドVer
    TUIObj* clmver_;//CLマスターVer
    TUIObj* svmver_;//SVマスターVer
    TUGFilter ugfilter_;//バー以下を隠すフィルタ
    TUGButton ugbtn_dev_;
    TUGButton ugbtn_devdbg_;
    TUGButton ugbtn_start_;
    //TUGButton ugbtn_kuniodb_;
    TUGButton ugbtn_stg_;//wsgisv
    //TUGButton ugbtn_test2_;//debugsv
    //TUGButton ugbtn_test1dbg_;//nosv
    //TUGButton ugbtn_test2dbg_;//nosv
    TUGButton ugbtn_oto_;//nosv
    //TUGHud ughud_;//debugsv
    TTitleSt stTitle_;

    void MvPage_VerCheck();
    void MvPage_MbgLogin();
    void MvPage_MbgAuth();
    void MvPage_Title();
    void MvPage_Kiyaku();
    void MvPage_KiyakuYN();
    void MvPage_CoInfo();
    void MvPage_CoClmaster();
    void MvPage_CoSvmaster();
    void MvPage_CoBannerBmp();
    void MvPage_CoEventBmp();
    void MvPage_CoUser();
    void MvPage_CoWorld();
    void MvPage_Retry();
    void MvPage_GotoItunes();

    void Connect_VerCheck();
    void Connect_Kiyaku();

    void CheckOut_CoClmaster();
    void CheckOut_CoSvmaster();
    void CheckOut_CoBannerBmp();
    void CheckOut_CoEventBmp();
    void CheckOut_CoInfo();
    void CheckOut_CoUser();
    void CheckOut_CoWorld();

    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoDown();
    void DoChangePage(s32 pageno);
    void DoConnect(BOOL rqok_f);
    void DoCheckOut(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TLyTiTitle(TSnTitle* pScene);
    virtual ~TLyTiTitle(void);
    void SetClmVerFont();
    void SetSvmVerFont();
  };
 
}
