#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGSerifu.h"
#include "dbUGChGirl.h"


namespace db
{
  //ショップメッセージ
  enum enMvkMsgType
  {
    mvm_Visit,
    mvm_Menu,
    mvm_Info,
    mvm_Code,
    mvm_FrCode,
    mvm_CodeResult,
    mvm_Invite,
    mvm_GetLog,
    mvm_UseLog,
    mvm_Circle,
    mvm_Mail,
    mvm_Push,
    mvm_Logout,
    mvm_Debug,
    mvm_Out,
  };


  struct TUGSpMavekoSt
  {
    enMvkMsgType mtype_;//店員メッセージタイプ
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGSpMaveko : public TUGGen
  {
  protected:
    TUIObj* inside_;
    TUGChGirl* uggirl_;
    

    TUIObj* logbostar_;
    TUIObj* logbosp_;

    STRVECTOR serifuvec_;

    void GenSetMsg(const char* text0);
    void GenSetMsg(const char* text0, const char* text1);
    void GenSetMsg(const char* text0, const char* text1, const char* text2);

    void Mv_Default();

    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGSpMavekoSt stMvk_;
    TUGSerifu ugserifu_;//セリフ

    //コンストラクタ
    explicit TUGSpMaveko(TUIGenLayer* player);
    virtual ~TUGSpMaveko(void);
    void MakeGroup(TUGChGirl* uggirl);
    void SetStopMsg();
    void SetMsg(enMvkMsgType msgtype);
  };
 
}
