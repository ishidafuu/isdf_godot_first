#pragma once
#include "dbInclude.h"
#include "dbUGRProf.h"
namespace db
{

  //リスト
  class TUGPlateProf :public TUGGen
  {
  protected:
    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGRProf ugrprof_;

    //コンストラクタ
    explicit TUGPlateProf(TUIGenLayer* player);
    virtual ~TUGPlateProf(void);

    void MakeGroup(TUIObj* parent, BOOL left_f);

    void SetProfMe();
    void SetProfRanker(s64 id_ranker);
    void SetProfEnemy(s32 no);
    void SetProfFriend();
  };
 
}
