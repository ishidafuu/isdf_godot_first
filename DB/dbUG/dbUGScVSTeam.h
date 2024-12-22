#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"
#include "dbUGScVSMem.h"
#include "dbUGRName.h"
namespace db
{

  struct TUGScVSTeamSt
  {
    s32 sideno_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGScVSTeam : public TUGGen
  {
  protected:

    void DoUpdate(BOOL draw_f);
  public:
    TUGScVSTeamSt stScVSTeam_;
    TUGRName ugname_;//チーム名
    std::vector<TUGScVSMem*> ugmem_;//メンバー

    //コンストラクタ
    explicit TUGScVSTeam(TUIGenLayer* player);
    virtual ~TUGScVSTeam(void);
    void MakeGroup(TUIObj* parent, s32 sideno);
    void Refresh();
  };
 
}
