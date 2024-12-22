#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGGen.h"

namespace db
{
  enum enUGBossSerifuAct
  {
    enUGBossSerifuAct_ReadEnd,
    enUGBossSerifuAct_WaitEnd,
  };
  //ショップメッセージ
  enum enBossSerifuFrame
  {
    enBossSerifuFrame_None,
    enBossSerifuFrame_Normal,
    enBossSerifuFrame_Message,
    enBossSerifuFrame_Wide,
    enBossSerifuFrame_BigWindow,	
  };
  // メッセージの切り替わりタイプ
  enum enBossMsgChangeMode
  {
    enBossMsgChangeMode_None = 0,	// 何もしない(切り替えない)
    enBossMsgChangeMode_Touch,		// タッチによる切り替え
    enBossMsgChangeMode_Wait,		// ウェイト後自動切り替え
    enBossMsgChangeMode_Wait2,	// ウェイト後自動切り替え(スキップNG)
  };

  struct TUGBossSerifuSt
  {
    s32 serifu_c_;
    s32 readline_;
    BOOL reading_f_;
    s32 wait_c_;

    void Init()
    {
      Zeromem(this);
    }
  };


  //セリフ
  class TUGBossSerifu : public TUGGen
  {
  public:
    //コンストラクタ
    explicit TUGBossSerifu(TUIGenLayer* player);
    virtual ~TUGBossSerifu(void);
    void MakeGroup(TUIObj* parent, s32 linenum, enBossSerifuFrame frame, enUIDepth depth = UIDP_07BUTTONTOP);//デフォはボタンの上
    void SetSerifu(STRVECTOR strvec);
	void SetSerifuFullOpen(STRVECTOR strvec);	// 最初から全部テキストが出てる
	void SetTitle(std::string tile);
    void SetEndWait(s32 wait);
    BOOL IsActRes(enUGBossSerifuAct res);//返値ゲット
    void ChangeFrame(enBossSerifuFrame frame);

    // ページ切り替えモード
    inline void SetMsgChangeMode(enBossMsgChangeMode mode){ changeMode_ = mode; }

    // アニメーションスキップ
    BOOL SkipSerifu(void);

    // 次のメッセージへ
    BOOL NextSerifu(void);

    // 現在のページのセリフ表示が全部終わった？
    BOOL IsReadLineEnd(void);

    TUGBossSerifuSt stSerifu_;
    s32 ENDWAIT;
  protected:
    void DoUpdate(BOOL draw_f);
    void DoAction();
	TUIObj* title_;
    INTVECTOR itemslen_;//各ラインパーツの文字列長さ
    enBossMsgChangeMode changeMode_;	// ページ切り替えモード
    TUIObj* frame_;	// メッセージウィンドウのフレームＵＩ
    std::vector<TUIObj*> lines_;//各ラインパーツ
    TUIObj* msgIcon_;	// ページ送り合図アイコン
    s32 msgIconDispTimer_;	// ページ送り合図アイコン点滅タイマー
    BOOL isNext_;
    BOOL isLineEnd_;
  };

}