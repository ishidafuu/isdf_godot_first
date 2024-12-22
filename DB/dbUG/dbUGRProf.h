#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"
#include "dbUGRKantoku.h"

namespace db
{
  class TUILayer;

  struct TUGRProfSt
  {
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGRProf : public TUGGen
  {
  protected:
    TUGRKantoku ugrkantoku_;
    TUIObj* plate_;
    TUIObj* text0_;
    TUIObj* text1_;
    TUIObj* text2_;
    void DoUpdate(BOOL draw_f);
  public:
    TUGRProfSt stRProf_;
    //void SetLimY(s32 limu, s32 hhh);
    //コンストラクタ
    explicit TUGRProf(TUIGenLayer* player);
    virtual ~TUGRProf(void);

    void MakeGroup(TUIObj* parent);
    void SetProfMe();
    void SetProfRanker(s64 id_ranker);
	void SetProfRankerByData(s64 id_ranker);
    void SetProfEnemy(s32 no);
    void SetProfFriend();
  };
 
}
