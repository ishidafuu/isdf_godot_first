#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcShSetSelectAll.h"
namespace db
{

  struct TUGLiShSetSelectAllSt
  {
    s64 id_team_;
    s32 mid_team_;
    s32 rarity_;
    s32 overlap_;
    s32 shsetNo_;
    s64 id_shset_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiShSetSelectAll :public TUGLiGen
  {
  public:
    //コンストラクタ
    explicit TUGLiShSetSelectAll(TUIGenLayer* player);
    virtual ~TUGLiShSetSelectAll(void);
    void Refresh(s64 id_team, s64 id_shset, LONGVECTOR& id_shotvec);
    void Refresh2(s32 shsetNo);
    TUGRcShSetSelectAll* MakeObj_Record();
    s64 GetShSet();

    std::vector<TUGRcShSetSelectAll*> records_;
    LONGVECTOR* id_shotvec_;
    TUGLiShSetSelectAllSt stLiShSetSelect_;
  protected:
    void DoMakeGroupSub();
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
    TUGButton* ugbtn_subAuto_;
    TUGButton* ugbtn_subShSet1_;
    TUGButton* ugbtn_subShSet2_;
    TUGButton* ugbtn_subShSet3_;
    TUGButton* ugbtn_subCustom_;

    //void GenRefresh();

  };
 
}
