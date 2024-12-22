#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"
#include "dbUGRIcon.h"

namespace db
{
  class TUILayer;

  struct TUGRCreditSt
  {
    s32 credittype_;
    void Init()
    {
      Zeromem(this);
    }
  };


  class TUGRCredit : public TUGGen
  {
  protected:
    void GenMakeGroup(TUIObj* parent);

    void SetName(std::string name);
    void DoUpdate(BOOL draw_f);
  public:
    TUGRCreditSt stCredit_;
    TUGRIcon ugicon_;
    //コンストラクタ
    explicit TUGRCredit(TUIGenLayer* player);
    virtual ~TUGRCredit(void);

    void MakeGroup_Techno(TUIObj* parent);
    void MakeGroup_Todd(TUIObj* parent);
    void MakeGroup_Star(TUIObj* parent);
    void MakeGroup_Huku(TUIObj* parent);
    void MakeGroup_Gacha(TUIObj* parent);
    void Refresh();
  };

}
