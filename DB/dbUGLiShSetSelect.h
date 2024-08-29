#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcShSetSelect.h"
namespace db
{

  struct TUGLiShSetSelectSt
  {
    s64 id_team_;
    s32 mid_team_;
    s32 rarity_;
    s32 overlap_;
    s32 shsetNo_;
    s64 id_shset_;
    s64 posNo_;
    void Init()
    {
      Zeromem(this);
    }
  };
  //リスト
  class TUGLiShSetSelect :public TUGLiGen
  {
  public:
    //コンストラクタ
    explicit TUGLiShSetSelect(TUIGenLayer* player);
    virtual ~TUGLiShSetSelect(void);
    void Refresh(s64 id_team, s64 id_shset, s32 posno, LONGVECTOR& id_shotvec);
    void Refresh2(s32 shsetNo);
    s64 GetShSet();
  protected:
    void DoMakeGroupSub();
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
    void GenRefresh();
    void SetShotVec();
    TUGRcShSetSelect* MakeObj_Record();
    std::vector<TUGRcShSetSelect*> records_;
    LONGVECTOR* id_shotvec_;
    TUGLiShSetSelectSt stLiShSetSelect_;
    TUGButton* ugbtn_subAuto_;
    TUGButton* ugbtn_subShSet1_;
    TUGButton* ugbtn_subShSet2_;
    TUGButton* ugbtn_subShSet3_;
    TUGButton* ugbtn_subCustom_;
  };
 
}
