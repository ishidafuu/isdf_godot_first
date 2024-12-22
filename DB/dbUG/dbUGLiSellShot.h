#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcSellShot.h"
namespace db
{

  enum enUGLiSellShotType
  {
    enUGLiSellShotType_UnderD,
    enUGLiSellShotType_UnderC,
    enUGLiSellShotType_UnderB,
    enUGLiSellShotType_UnderA,
    enUGLiSellShotType_All,
  };



  struct TUGLiSellShotSt
  {
    s64 id_team_;
    s64 mid_team_;
    s32 posno_;
    s32 maxpage_;
    s64 id_shot_last_;
    //s32 pageno_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiSellShot :public TUGLiGen
  {
  protected:
    void DoMakeGroupSub();
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcSellShot*> records_;
    TUGButton* ugbtn_subA_;//全選択
    TUGButton* ugbtn_subAC_;//全クリア

    TUGLiSellShotSt stLiSellShot_;
    TUGRcSellShot* MakeObj_Record();
    //コンストラクタ
    explicit TUGLiSellShot(TUIGenLayer* player);
    virtual ~TUGLiSellShot(void);
    BOOL Refresh(s32 pageno, LONGVECTOR* id_shotvec);
    s32 GetMaxPage();
    LONGVECTOR GetIDSellShot();
    s64 GetIDShotLast();

  };
 
}
