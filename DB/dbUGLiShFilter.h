#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcNormal.h"
namespace db
{

  enum enUGLiShFilterAct
  {
    enUGLiShFilter_Filter,
    enUGLiShFilter_Sort,
  };

  struct TUGLiShFilterSt
  {
    enShotFilType filtertype_;
    enShotFilTypeS filtertypeS_;
    enShotSrtType sorttype_;
    BOOL rev_f_;
    //s64 id_shot_ng_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiShFilter :public TUGLiGen
  {
  protected:
    void GetShotList_Filter(enShotFilType filtertype, enShotFilTypeS filtertypeS);
    void GetShotList_Sort(enShotSrtType sorttype);
    void DoMakeGroupSub();
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcNormal*> records_;
    TUGLiShFilterSt stLiShFilter_;
    SORTSHOTVECTOR shotvec_;
    LONGVECTOR id_shotvec_;
    LONGVECTOR ngidvec_;

    //TUGButton* ugbtn_subSortMID_;
    TUGButton* ugbtn_subSortRarity_;
    TUGButton* ugbtn_subSortPow_;
    TUGButton* ugbtn_subSortSpin_;
    TUGButton* ugbtn_subSortNew_;

    //コンストラクタ
    explicit TUGLiShFilter(TUIGenLayer* player);
    virtual ~TUGLiShFilter(void);

    TUGRcNormal* MakeObj_Record();
    BOOL Ready(enShotFilTypeS filtypeS);
    BOOL Ready(enShotFilTypeS filtypeS, LONGVECTOR* ngidvec);
    BOOL Refresh(enShotFilTypeS filtypeS, BOOL from_ready);
    BOOL IsActRes2(enUGLiShFilterAct res);//返値ゲット
    BOOL IsHaveMixed();
		void HaveFilter(enShotFilType filtertype, enShotFilTypeS filtertypeS);
  };
 
}
