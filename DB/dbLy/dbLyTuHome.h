//utf8
#pragma once
#include "dbLyTuBase.h"
#include "dbLyHmHome.h"	// WINDOWNUMの為
#include "dbUGFinger.h"
//=========================================================================
// 
// チュートリアル：ホーム画面
//
//=========================================================================
namespace db
{
  class TLyTuHome :public TLyTuBase
  {
    // 基底クラス定義
    typedef TLyTuBase base;

    struct CharaDt
    {
      s32 talkCategory_;	// 喋りのカテゴリー番号
      s32 talkNo_;		// 今喋っているテキスト番号
      void Init(void)
      {
        talkCategory_ = 0;
        talkNo_ = 0;
      }
    };


  public:
    //コンストラクタ
    explicit TLyTuHome(TSnTutorial* pScene);
    virtual ~TLyTuHome(void);

    // 真実メニューからの復帰用
    void ComebackInMamiMenu(void);

    // プロフィールからの復帰用
    void ComebackProfile(void);

    //チュート終了
    void SetTutoEnd();

    // 真実のポインタのゲッター
    inline TUGChGirl* GetMami(void){ return &ugmami_; }
    // ログボアイコンのゲッター
    inline TUGLogboIcon* GetLogbo(void){ return &uglogbo_; }
    // セリフ用ウィンドウのゲッター
    inline TUGSerifu* GetSerifuWindow(void){ return &ugserifu_; }

  protected:

    //オーバーロード
    void DoLoad(void);
    void DoReady(void);
    void DoChangePage(s32 pageno);
    void DoConnect(BOOL rqok_f);
    void DoUpdate(BOOL draw_f);

    void Post_TuHome(void);
    void Connect_TuHome(void);

    TUIObj* floorB_;
    TUIObj* floorF_;
    TUIObj* boardL_;
    TUIObj* boardR_;
    //TUIObj* flower_;
    TUIObj* shadow_[WINDOWNUM];
    TUIObj* window_[WINDOWNUM];

    TUGButton ugbtn_mami_;
    TUGChGirl ugmami_;
    TUGChKantoku ugkantoku_;
    TUGChCaptain ugcap_;
    CharaDt	captainDt_;
    CharaDt mamiDt_;

    TUGLogboIcon uglogbo_;
    //TUGFinger ugfinger_;//よく使うのでBaseにうつしました

    // スペシャルメッセージ用に
    s32 hairCharaNo_;
    s32 faceCharaNo_;
    std::string name_;

    s32 editErrCnt_;

  private:
    s32 subPage_;
    void Init(void);
    void CapTalk(void);

    void MamiMenu(void);
    void InMamiMenu(void);
    void OutMamiMenu(void);

    void EditProfile(void);
    void InEditProfile(void);
    void CheckEditProfile(void);
    void ErrEditProfile(void);
    void SpecialProfileLooks(void);
    void SpecialProfileName(void);
    void OutEditProfile(void);

    void GotoShop(void);
  };

}
