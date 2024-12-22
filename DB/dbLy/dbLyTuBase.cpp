//utf8
#include "dbLyTuBase.h"
#include "dbSnTutorial.h"
//=========================================================================
// 
// チュートリアル：ベースクラス
//
//=========================================================================
namespace db
{
	const char* CAPTAIN_NAME_REPLACE = "$CAP_NAME$";

  //-----------------------------------------------------------------
  // コンストラクタ
  //-----------------------------------------------------------------
  TLyTuBase::TLyTuBase(TSnTutorial* pScene)
    :TLyTuGen(pScene)
    , ugserifu_(this)
    , ugfinger_(this)
    , ugfilter_(this)
    , isTutoEnd_(FALSE)
  {
  }

  //-----------------------------------------------------------------
  // デストラクタ
  //-----------------------------------------------------------------
  TLyTuBase::~TLyTuBase()
  {
    //Freemem(ugfilter_);
  }

  //-----------------------------------------------------------------
  // ロード
  //-----------------------------------------------------------------
  void TLyTuBase::DoLoad()
  {
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuBase::DoReady()
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
  void TLyTuBase::DoChangePage(s32 pageno)
  {
  }

  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuBase::DoUpdate(BOOL draw_f)
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
  void TLyTuBase::SetMsg(const char* text0, const char* text1, const char* text2)
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

	for( s32 i = 0; i < serifuvec_.size(); i++)
	{
		ConvCaptainName(&serifuvec_[i]);
	}
    ugserifu_.SetSerifu(serifuvec_);
  }
  //-----------------------------------------------------------------
  // 
  //-----------------------------------------------------------------
  void TLyTuBase::ConvCaptainName(std::string* s)
  {
	  // 今使っているチームのキャプテン名取得
#ifdef __K_DEBUG_USER_DATA__
	  s64 teamMID = kdebug::DebugUDManager::GetInstance()->GetLastTeamID();
#else
	  s64 teamMID = pmgEO_->mgCoU_.GetLastMIDTeam();
#endif
	  s64 charID = pmgEO_->mgCoM_.GetCharID(teamMID, 0);
	  std::string name = pmgEO_->mgDt_.dtChar_.GetName(charID);

	  std::size_t pos = s->find(CAPTAIN_NAME_REPLACE);
	  if( pos == std::string::npos ) return;
	  std::size_t size = std::string(CAPTAIN_NAME_REPLACE).size();
	  s->replace(pos, size, name);
  }
}
