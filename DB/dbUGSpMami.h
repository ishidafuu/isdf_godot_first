#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGSerifu.h"
#include "dbUGChGirl.h"


namespace db
{
  //チュートメッセージ
  enum enMamiMsgType
  {
    mmm_Visit,
	
	mmm_Logbo,
	mmm_LogboSpYokoku,
	mmm_LogboSp,
	mmm_LogboResult,

	mmm_LogboShot,
	mmm_LogboShot2,
	mmm_LogboShotPrev,
	mmm_LogboShotPrev2,
	mmm_LogboShotResult,
	mmm_LogboShotResult2,
	mmm_LogboSpShot,
	mmm_LogboSpShotYokoku,

    mmm_Menu,
    mmm_GiftBox,
    mmm_Out,
  };


  struct TUGSpMamiSt
  {
    enMamiMsgType mtype_;//店員メッセージタイプ
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGSpMami : public TUGGen
  {
  protected:
    //TUIObj* inside_;
    TUGChGirl* uggirl_;
    

    //TUIObj* logbostar_;
    //TUIObj* logbosp_;

    STRVECTOR serifuvec_;

    void GenSetMsg(const char* text0);
    void GenSetMsg(const char* text0, const char* text1);
    void GenSetMsg(const char* text0, const char* text1, const char* text2);

    void Mv_Default();

    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGSpMamiSt stMami_;
    TUGSerifu ugserifu_;//セリフ

    //コンストラクタ
    explicit TUGSpMami(TUIGenLayer* player);
    virtual ~TUGSpMami(void);
    void MakeGroup(TUGChGirl* uggirl);
    void SetStopMsg();
    void SetMsg(enMamiMsgType msgtype, s64 number = -1);
  };
 
}
