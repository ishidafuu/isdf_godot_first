#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcNormal.h"
#include "dbUGRName.h"
#include "dbUGButton.h"

namespace db
{

  enum enUGRcGiftBoxAct
  {
    enUGRcGiftBox_Field,
    enUGRcGiftBox_Eye,
  };

  struct TUGGiftBoxSt
  {
    s64 id_giftbox_;
    void Init()
    {
      Zeromem(this);
      id_giftbox_ = NGNUM;
    }
  };

  class TUGRcGiftBox :public TUGRcNormal
  {
  protected:
    //s64 mid_gachacnt_;
    void DoMakeGroupSub2();
    void DoSetLimY2(s32 limu, s32 hhh);
    void DoUpdateSub2(BOOL draw_f);
    void DoActionSub();
  public:
    TUGGiftBoxSt stGiftBox_;
    //TUGButton ugbtn_eye_;
    //コンストラクタ
    explicit TUGRcGiftBox(TUIGenLayer* player);
    virtual ~TUGRcGiftBox(void);
    void SetGiftBoxDt(s64 id_giftbox);
  };
}
