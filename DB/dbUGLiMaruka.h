#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcMaruka.h"
namespace db
{

  struct TUGLiMarukaSt
  {
    s64 id_team_;
    s64 mid_team_;
    s32 posno_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiMaruka :public TUGLiGen
  {
  protected:
    void DoMakeGroupSub();
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcMaruka*> records_;
    TUGButton* ugbtn_subA_;//全選択
    TUGButton* ugbtn_subAC_;//全クリア

    TUGLiMarukaSt stLiMaruka_;
    //コンストラクタ
    explicit TUGLiMaruka(TUIGenLayer* player);
    virtual ~TUGLiMaruka(void);
    TUGRcMaruka* MakeObj_Record();
    void Refresh(s32 pageno, s32 endpage);
    LONGVECTOR GetIDOutSozai();
  };
 
}
