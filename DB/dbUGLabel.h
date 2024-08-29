#pragma once
#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"
#include "dbUGRName.h"

namespace db
{
  class TUILayer;

  struct TUGLabelSt
  {
    s32 www_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLabel : public TUGGen
  {
  protected:
    TUGLabelSt stLabel_;
    TUIObj* labelend_;
    void DoUpdate(BOOL draw_f);
  public:
    TUGRName ugname_;

    //コンストラクタ
    explicit TUGLabel(TUIGenLayer* player);
    virtual ~TUGLabel(void);
    void MakeGroup(TUIObj* parent);
    void Refresh();//ugname_に文字列を入れた後呼び出す
  };
 
}
