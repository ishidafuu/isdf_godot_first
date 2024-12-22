#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcSoubi.h"
namespace db
{

  struct TUGLiSoubiSt
  {
    s64 id_team_;
    s64 mid_team_;
    s32 posno_;
    s64 id_shset_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiSoubi :public TUGLiGen
  {
  protected:
    void Refresh_Custom(s64 id_team, s64 mid_team, s64 id_shset, s32 posno, BOOL init_f, LONGVECTOR* id_shotvec);
    void Refresh_Custom_mc(s64 id_team, s64 mid_team, s64 id_shset, s32 posno, BOOL init_f, LONGVECTOR* id_shotvec);
    void Refresh_ShSet(s64 id_team, s64 mid_team, s64 id_shset, s32 posno, BOOL init_f, LONGVECTOR* id_shotvec);
    void Refresh_ShSet_mc(s64 id_team, s64 mid_team, s64 id_shset, s32 posno, BOOL init_f, LONGVECTOR* id_shotvec);
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
  public:
    std::vector<TUGRcSoubi*> records_;

    TUGLiSoubiSt stLiSoubi_;
    TUGRcSoubi* MakeObj_Record();
    //コンストラクタ
    explicit TUGLiSoubi(TUIGenLayer* player);
    virtual ~TUGLiSoubi(void);
    void Refresh(s64 id_team, s64 mid_team, s64 id_shset, s32 posno, LONGVECTOR* id_shotvec, BOOL init_f);
    void SwitchEdit(BOOL edit_f);
  };
 
}
