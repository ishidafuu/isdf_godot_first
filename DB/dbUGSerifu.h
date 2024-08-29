#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGGen.h"

namespace db
{
  enum enUGSerifuAct
  {
    enUGSerifuAct_ReadEnd,
    enUGSerifuAct_WaitEnd,
  };
  //ショップメッセージ
  enum enSerifuFrame
  {
    enSerifuFrame_None,
    enSerifuFrame_Normal,
    enSerifuFrame_Message,
    enSerifuFrame_Wide,
  };
  // メッセージの切り替わりタイプ
  enum enMsgChangeMode
  {
    enMsgChangeMode_None = 0,	// 何もしない(切り替えない)
    enMsgChangeMode_Touch,		// タッチによる切り替え
    enMsgChangeMode_Wait,		// ウェイト後自動切り替え
    enMsgChangeMode_Wait2,	// ウェイト後自動切り替え(スキップNG)
  };

  struct TUGSerifuSt
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
  class TUGSerifu : public TUGGen
  {
  public:
    //コンストラクタ
    explicit TUGSerifu(TUIGenLayer* player);
    virtual ~TUGSerifu(void);
    void MakeGroup(TUIObj* parent, s32 linenum, enSerifuFrame frame, enUIDepth depth = UIDP_07BUTTONTOP);//デフォはボタンの上
    void SetSerifu(STRVECTOR strvec);
    void SetEndWait(s32 wait);
    BOOL IsActRes(enUGSerifuAct res);//返値ゲット
    void ChangeFrame(enSerifuFrame frame);

    // ページ切り替えモード
    inline void SetMsgChangeMode(enMsgChangeMode mode){ changeMode_ = mode; }

    // アニメーションスキップ
    BOOL SkipSerifu(void);

    // 次のメッセージへ
    BOOL NextSerifu(void);

    // 現在のページのセリフ表示が全部終わった？
    BOOL IsReadLineEnd(void);

    TUGSerifuSt stSerifu_;
    s32 ENDWAIT;
  protected:
    void DoUpdate(BOOL draw_f);
    void DoAction();
    INTVECTOR itemslen_;//各ラインパーツの文字列長さ
    enMsgChangeMode changeMode_;	// ページ切り替えモード
    TUIObj* frame_;	// メッセージウィンドウのフレームＵＩ
    std::vector<TUIObj*> lines_;//各ラインパーツ
    TUIObj* msgIcon_;	// ページ送り合図アイコン
    s32 msgIconDispTimer_;	// ページ送り合図アイコン点滅タイマー
    BOOL isNext_;
    BOOL isLineEnd_;
  };

}