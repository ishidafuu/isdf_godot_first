#pragma once
#include "dbInclude.h"
#include "dbUGDrGen.h"
#include "dbUGShot.h"
#include "dbUGShotUra.h"
namespace db
{
  struct TDrShotSt
  {
    s64 id_team_;
    s32 drtype_;
    s32 urano_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGLiShFilter;

  //ドラム
  class TUGDrShot :public TUGDrGen
  {
  protected:
    BOOL IsNoneVec();
    void RefreshRecords(BOOL first_f);
    void DoUpdateSub(BOOL draw_f);//更新
    void DoActionSub();
    void DoStop();
    void DoRoll();
	void DoStart(){};
    void DoReady();
    BOOL DoSelectUra(BOOL ura_f);
  public:
    TDrShotSt stDrShot_;
    std::vector<TUGShot*> records_;
    LONGVECTOR id_shotvec_;
    SOZAIDTVECTOR sozaidtvec_bara_;
    TUGShotUra ura_;
    TUGShot drumend_;
    TUGLiShFilter* uglistShFilter_;

    //コンストラクタ
    explicit TUGDrShot(TUIGenLayer* player);
    virtual ~TUGDrShot(void);
    void MakeGroup(TUIObj* parentr, BOOL leftpos_f, TUGLiShFilter* uglistShFilter);
    void ReadyFilter();
    void ReadySort();

    void ReadyType_BaraLast(s64 id_shot_base);//ばらし結果
    void ReadyType_BaraAll(s64 id_shot_base);//ばらし結果
    void ReadyType_Syn(s64 defshotid);//固有シュートレシピ
    void RefreshSelected();

    void DrumRoll_IDShot(s64 id_shot);
    s64 GetIDShot();
    void SetIDTeam(s64 id_team);
    void RevUra();
  };
 
}
