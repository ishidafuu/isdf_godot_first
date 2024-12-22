#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcGen.h"
#include "dbUGButton.h"
#include "dbUGRProf.h"

namespace db
{
  class TUGRc2Gen :public TUGRcGen
  {
  protected:
    static const s32 POSXL = 24;
    static const s32 POSXR = 142;
    static const s32 POSY = 8;
    static const s32 SUBY = 3;
    static const s32 RECORDH = 120;
    BOOL prof_f_;
    s32 DoMakeGroupSub();
    void DoSetLimY(s32 limu, s32 hhh);
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();

    virtual void DoMakeGroupSub2(){}
    virtual void DoSetLimY2(s32 limu, s32 hhh){}
    virtual void DoUpdateSub2(BOOL draw_f){}
  public:
    TUGRProf ugrprof_;

    TUGRName ugname_;
    TUGRName ugvalue_;

    std::vector<TUIObj*> text_;
    TUIObj* dttitle_;
    TUIObj* dtvalue_;
    TUIObj* dttitle2_;
    TUIObj* dtvalue2_;

    TUGButton ugbtn_ok_;//ＯＫボタン
    //TUGButton ugbtn_sub_;//特賞表ボタン
    //コンストラクタ
    explicit TUGRc2Gen(TUIGenLayer* player);
    virtual ~TUGRc2Gen(void);
    void UndrawAll();
    //void GenSetValue(const char* value);
    void SetValue(const char* value);
    void SetValue_Star(s32 value);
    void SetValue_Todd(s32 value);
    void SetValue_Huku(s32 value);
    void SetValue_Gacha(s32 value);

    void GenSetData(s32 dtno, const char* dttitle, const char* dtvalue);
    void GenSetText(STRVECTOR textvec);
    void GenSetButton(const char* btntitle);
  };
}
