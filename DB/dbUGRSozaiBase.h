#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"

namespace db
{
  class TUILayer;

  struct TUGRSozaiBaseSt
  {
    BOOL num_f_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGRSozaiBase : public TUGGen
  {
  protected:
    TUIObj* sign_;//符号
    std::vector<TUIObj*> baseNumvec_;//各ラインパーツ(base部分)
	std::vector<TUIObj*> revNumvec_;//各ラインパーツ(rev部分)
    void DoUpdate(BOOL draw_f);
  public:
    TUGRSozaiBaseSt stRSozaiBase_;
    //void SetLimY(s32 limu, s32 hhh);
    //コンストラクタ
    explicit TUGRSozaiBase(TUIGenLayer* player);
    virtual ~TUGRSozaiBase(void);

    void MakeGroup(TUIObj* parent);
    void Refresh(s32 basedt, s32 rarity);
    void Refresh(s32 basedt, s32 rarity, s32 num, BOOL drawzero_f);
    void RefreshInv(s32 basedt, s32 rarity);
    void SetLimY(s32 limu, s32 hhh);
  };
 
}
