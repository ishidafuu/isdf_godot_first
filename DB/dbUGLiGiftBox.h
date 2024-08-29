#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcGiftBox.h"
namespace db
{

  struct TUGLiGiftBoxSt
  {
    s64 id_team_;
    s64 mid_team_;
    s32 posno_;
    //s32 pageno_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiGiftBox :public TUGLiGen
  {
  protected:
    void DoMakeGroupSub();
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcGiftBox*> records_;
    TUGButton* ugbtn_subA_;//全選択
    TUGButton* ugbtn_subAC_;//全クリア

    TUGLiGiftBoxSt stLiGiftBox_;
    TUGRcGiftBox* MakeObj_Record();
    void Refresh(s32 pageno);
    s32 GetMaxPage();
    LONGVECTOR GetIDOpen();



    //コンストラクタ
    explicit TUGLiGiftBox(TUIGenLayer* player);
    virtual ~TUGLiGiftBox(void);

  };
 
}
