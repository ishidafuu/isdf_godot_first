#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"

namespace db
{
  class TUILayer;

  struct TUGRRaritySt
  {
    BOOL num_f_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGRRarity : public TUGGen
  {
  protected:
    //TUIObj* rare_;
    TUIObj* lvStr_;//
	TUIObj* lvNum_;
    void DoUpdate(BOOL draw_f);
  public:
    TUGRRaritySt stRRarity_;
    //void SetLimY(s32 limu, s32 hhh);
    //コンストラクタ
    explicit TUGRRarity(TUIGenLayer* player);
    virtual ~TUGRRarity(void);

    void MakeGroup(TUIObj* parent);
    void Refresh(s32 rarity);
    void Refresh(s32 rarity, s32 num, BOOL drawzero_f);
    void RefreshInv(s32 rarity);
    void SetLimY(s32 limu, s32 hhh);
  };
 
}
