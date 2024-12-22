#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"
#include "dbUGChKantoku.h"
#include "dbUGRName.h"

namespace db
{
  class TUILayer;

  struct TUGRKantokuSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGRKantoku : public TUGGen
  {
  protected:
    TUGRName ugname_;
    TUGChKantoku ugkantoku_;
    TUIObj* mbgid_;
    void GenSetNameCode(s64 mbgid);
    void DoUpdate(BOOL draw_f);
  public:
    TUGRKantokuSt stRKantoku_;
    //コンストラクタ
    explicit TUGRKantoku(TUIGenLayer* player);
    virtual ~TUGRKantoku(void);

    void MakeGroup(TUIObj* parent);
    void SetName();
    void SetNameCode();
    void SetNameRanker(s64 id_ranker);
	void SetNameRankerByData(s64 id_ranker);
    void SetNameEnemy(s32 no);
    void SetNameCodeRanker(s64 id_ranker);
	void SetNameCodeRankerByData(s64 id_ranker);
    void SetNameCodeEnemy(s32 no);
    void SetStandIcon();
    void SetLimY(s32 limu, s32 hhh);
  };
 
}
