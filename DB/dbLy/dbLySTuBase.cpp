//utf8
#include "dbLySTuBase.h"
#include "dbSnTutorial.h"
//=========================================================================
// 
// チュートリアル：ベースクラス
//
//=========================================================================
namespace db
{

  //-----------------------------------------------------------------
  // コンストラクタ
  //-----------------------------------------------------------------
 // TLySTuBase::TLySTuBase(TSnTutorial* pScene)
 //	:TLyTuGen(pScene)
	TLySTuBase::TLySTuBase(TSnHome* pScene)
    :TLyHmGen(pScene)
    , ugserifu_(this)
    , ugfinger_(this)
    , ugfilter_(this)
    , isTutoEnd_(FALSE)
  {
  }

  //-----------------------------------------------------------------
  // デストラクタ
  //-----------------------------------------------------------------
  TLySTuBase::~TLySTuBase()
  {
    //Freemem(ugfilter_);
  }

  //-----------------------------------------------------------------
  // ロード
  //-----------------------------------------------------------------
  void TLySTuBase::DoLoad()
  {
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLySTuBase::DoReady()
  {
    // セリフウィンドウ生成
    //ugserifu_ = new TUGSerifu(this);
    ugserifu_.MakeGroup(field_, 2, enSerifuFrame_Wide);
    ugserifu_.SetParent(field_);
    ugserifu_.SetPos(-90, 30);
    ugserifu_.SetDraw(FALSE);
    ugserifu_.SetMsgChangeMode(enMsgChangeMode_Touch);

    // フェード生成
    //ugfilter_ = new TUGFilter(this);
    ugfilter_.MakeGroup(field_);
    ugfilter_.SetDraw(FALSE);

    // チュートリアル用指
    ugfinger_.MakeGroup(field_);
    ugfinger_.SetDepth(UIDP_09MSGBOX);
    ugfinger_.SetDraw(FALSE);
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLySTuBase::DoChangePage(s32 pageno)
  {
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLySTuBase::DoUpdate(BOOL draw_f)
  {
    ugserifu_.Update(draw_f);
    ugfilter_.Update(draw_f);
    ugfinger_.Update(draw_f);

    // メッセージスキップ処理
    if (field_->sc_IsCamEnter())
    {
      if (ugserifu_.IsReadLineEnd())
      {
        ugserifu_.NextSerifu();
      }
      else
      {
        ugserifu_.SkipSerifu();
      }
    }
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLySTuBase::SetMsg(const char* text0, const char* text1, const char* text2)
  {
    std::string strNone = "";

    serifuvec_.clear();
    serifuvec_.push_back(text0);
    if (text1 && strNone.compare(text1) != 0)
    {
      serifuvec_.push_back(text1);
    }
    if (text2 && strNone.compare(text2) != 0)
    {
      serifuvec_.push_back(text2);
    }
    ugserifu_.SetSerifu(serifuvec_);
  }
}
