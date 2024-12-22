#pragma once
#include "dbInclude.h"
#include "dbUGRCredit.h"
#include "dbUGRText.h"
namespace db
{

  //リスト
  class TUGPlateCredit :public TUGGen
  {
  public:
    TUGRCredit ugrTechno_;
    TUGRCredit ugrStar_;
    TUGRCredit ugrTodd_;
    //TUGRCredit ugrGacha_;
    TUGRCredit ugrHuku_;
    TUGRText ugrText_;

    //コンストラクタ
    explicit TUGPlateCredit(TUIGenLayer* player);
    virtual ~TUGPlateCredit(void);

    void MakeGroup(TUIObj* parent);
    void Refresh();
  protected:
    void DoUpdate(BOOL draw_f);
    void DoAction();
  };
 
}
