#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcNormal.h"
namespace db
{

  enum enUGLiTmFilterAct
  {
    enUGLiTmFilter_Filter,
    enUGLiTmFilter_Sort,
  };

  struct TUGLiTmFilterSt
  {
    enTeamFilType filtertype_;
    enTeamSrtType sorttype_;
    s32 mid_event_;
    BOOL rev_f_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiTmFilter :public TUGLiGen
  {
  protected:
    void GetTeamList_Filter(enTeamFilType filtertype);
    void GetTeamList_Sort(enTeamSrtType sorttype);
    void DoMakeGroupSub();
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcNormal*> records_;
    TUGLiTmFilterSt stLiTmFilter_;
    SORTTEAMVECTOR teamvec_;
    LONGVECTOR id_teamvec_;

    //TUGButton* ugbtn_subSortMID_;
    TUGButton* ugbtn_subSortRarity_;
    TUGButton* ugbtn_subSortWait_;
    TUGButton* ugbtn_subSortLv_;
    TUGButton* ugbtn_subSortNew_;

    //コンストラクタ
    explicit TUGLiTmFilter(TUIGenLayer* player);
    virtual ~TUGLiTmFilter(void);

    TUGRcNormal* MakeObj_Record();
    void Ready(s32 mid_event);
    BOOL Refresh();
    //void SetMIDEvent(s32 mid_event);

    BOOL IsActRes2(enUGLiTmFilterAct res);//返値ゲット
  };
 
}
