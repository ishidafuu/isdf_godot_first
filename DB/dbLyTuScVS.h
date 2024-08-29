#pragma once
#include "dbLyScGen.h"
#include "dbUGVS.h"
namespace db
{

  //VS////////////////////////////////
  class TLyTuScVS :public TLyScGen
  {
  protected:
    std::vector<TUGScVSTeam*> ugteam_;//チーム
    TUGVS ugvs_;//VSマーク

    void MvPage_VS();
    void MvPage_GotoShiai();

    //オーバーロード
    void DoLoad();
    void DoReady();
    void DoUpdate(BOOL draw_f);
  public:
    //コンストラクタ
    explicit TLyTuScVS(TSnScenario* pScene);
    virtual ~TLyTuScVS(void);
  };
 
}
