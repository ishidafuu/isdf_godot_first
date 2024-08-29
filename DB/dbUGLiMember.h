#pragma once
#include "dbInclude.h"
#include "dbUGLiGen.h"
#include "dbUGRcMember.h"
namespace db
{
  struct TUGLiMemberSt
  {
    s64 id_team_;
    s32 mid_team_;
    BOOL soubi_f_;
    s32 selectedNo_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGLiMember :public TUGLiGen
  {
  public:
    //コンストラクタ
    explicit TUGLiMember(TUIGenLayer* player);
    virtual ~TUGLiMember(void);
    TUGRcMember* MakeObj_Record();
    void Refresh(s64 id_team, s64 mid_team, BOOL soubi_f);
    //void RefreshShSet(s32 shsetNo);
    BOOL IsSelected();
    std::vector<TUGRcMember*> records_;
    TUGLiMemberSt stLiMember_;
  protected:
    void DoMakeGroupSub();
    void DoUpdateSub(BOOL draw_f);
    void DoActionSub();
    void DoClearRecord();
  };
 
}
