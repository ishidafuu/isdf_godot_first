#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"

namespace db
{
  class TUILayer;

  enum TUGFilterAct
  {
    ugfilter_fadeout,
    ugfilter_fadeout2,
  };

  struct TUGFilterSt
  {
    BOOL filterMove_f_;
    s32 filter_c_;
    BOOL filterOn_f_;
    s32 eat_;
    BOOL autooff_f_;
    s32 filterLv_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGFilter : public TUGGen
  {
  protected:
    TUIObj* filter_;
    TUIObj* filterhf_;

    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGFilterSt stFilter_;

    //コンストラクタ
    explicit TUGFilter(TUIGenLayer* player);
    virtual ~TUGFilter(void);
    void MakeGroup(TUIObj* parent);

    void OnFilter(s32 lv, BOOL white_f, BOOL autooff_f);
    void OffFilter(s32 lv, BOOL white_f);
    void OnFilterHf(s32 lv, BOOL white_f, BOOL left_f);
    void OffFilterHf();

	void SetFilterDepth(db::enUIDepth depth);
    
    void Flash(BOOL red_f);
  };
 
}
