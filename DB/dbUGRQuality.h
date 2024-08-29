#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"

namespace db
{
  class TUILayer;

  struct TUGRQualitySt
  {
    BOOL spin_f_;
    BOOL bar_f_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGRQuality : public TUGGen
  {
  protected:
    void DoUpdate(BOOL draw_f);
  public:
    TUGRQualitySt stRQuality_;
    //コンストラクタ
    explicit TUGRQuality(TUIGenLayer* player);
    virtual ~TUGRQuality(void);

    void MakeGroup(TUIObj* parent, BOOL spin_f_, BOOL bar_f);
    void Refresh(s32 quality);
    void Refresh(s32 quality, s32 mid_sozai, s32 mid_rarity);
    //void SetLimY(s32 limu, s32 hhh);
  };
 
}
