#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"
namespace db
{
  struct TUGScVSMemSt
  {
    s32 sideno_;
    s32 memno_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGScVSMem : public TUGGen
  {
  protected:
    TUIObj* body_;//ボディ
    TUIObj* no_;//No
    TUIObj* name_;//キャラ名
    //TUIObj* hp_;//HPバー
	TUIObj* lv_;//個人レベル

    void DoUpdate(BOOL draw_f);
  public:
    TUGScVSMemSt stScVSMem_;

    //コンストラクタ
    explicit TUGScVSMem(TUIGenLayer* player);
    virtual ~TUGScVSMem(void);
    void MakeGroup(TUIObj* parent, s32 sideno, s32 memno);
    void Refresh();
  };
 
}
