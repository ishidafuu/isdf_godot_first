#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGGen.h"

namespace db
{
  class TUGShKantoku;

  struct TUGShPadSt
  {
    s32 btntype_;
    s32 msgtype_;
    BOOL prev_f_;
    void Init()
    {
      Zeromem(this);
    }
  };

  
  //キャッチ
  class TUGShPad : public TUGGen
  {
  protected:
    TUIObj* dfbal_;
    TUIObj* ofbal_;
    void GenMakeGroup(TUIObj* parent);
    void Refresh();
    void DoUpdate(BOOL draw_f);
  public:
    enum enPadMsgType
    {
      pmt_Normal,
      pmt_TutoOf,
      pmt_TutoDf,
      pmt_Invisible,
    };
    TUGShPadSt stShPad_;
    //コンストラクタ
    explicit TUGShPad(TUIGenLayer* player);
    virtual ~TUGShPad(void);
    void MakeGroup_Shot(TUIObj* parent);
    void MakeGroup_ShotPrev(TUIObj* parent);
    void MakeGroup_Pass(TUIObj* parent);
    void MakeGroup_Jump(TUIObj* parent);
    void MakeGroup_JumpPrev(TUIObj* parent);
    void SetMsgType(enPadMsgType msgtype);
  };
 
}