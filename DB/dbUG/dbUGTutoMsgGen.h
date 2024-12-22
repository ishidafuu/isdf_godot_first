#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGSerifu.h"
#include "dbUGChGirl.h"


namespace db
{


  struct TUGTutoMsgGenSt
  {
    s32 msgtype_;//店員メッセージタイプ
    s32 msgno_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGTutoMsgGen : public TUGGen
  {
  protected:
    STRVECTOR serifuvec_;
    BOOL GenSetMsg(STRVECTOR* msg);
   
    void Mv_Default();
    void DoUpdate(BOOL draw_f);
    void DoAction();

    virtual BOOL DoNextMsg() = 0;
  public:
    TUGTutoMsgGenSt stTutoMsgGen_;
    TUGSerifu ugserifu_;//セリフ

    //コンストラクタ
    explicit TUGTutoMsgGen(TUIGenLayer* player);
    virtual ~TUGTutoMsgGen(void);
    void MakeGroup(TUIObj* parent);
    void Ready();
    BOOL NextMsg();
    void IncMsg();
    void FirstMsg();
    void SameMsg();
    void SetMsgClear(s32 msgtype, BOOL ok_f);
    void SetMsg(s32 msgtype, s32 msgno);
    void ChangeFrame(BOOL wide_f);
    s32 GetMsgNo();
    void SetSerifuEndWait(s32 wait);
    void CheckSkipMsg(void);
  };
 
}
