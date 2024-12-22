#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"

namespace db
{
  class TUILayer;

  struct TUGInputTipSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGInputTip : public TUGGen
  {
  protected:
    void SetInputTipPosGen();
    void DoUpdate(BOOL draw_f);
  public:
    TUGInputTipSt stInputTip_;
    void MakeGroup(TUIObj* parent);
    void SetFont(std::string msg);
    void SetFontMC(s32 mojicode);

    //コンストラクタ
    explicit TUGInputTip(TUIGenLayer* player);
    virtual ~TUGInputTip(void);
  };
 
}
