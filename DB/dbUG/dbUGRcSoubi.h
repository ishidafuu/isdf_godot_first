#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGRcNormal.h"
#include "dbUGButton.h"
#include "dbUGBtnSoubi.h"
namespace db
{
  
  enum enUGRcSoubiAct
  {
    enUGRcSoubi_Field,
    enUGRcSoubi_Detach,
    //enUGRcSoubi_Attach,
    //enUGRcSoubi_Eye,
  };

  struct TUGRcSoubiSt
  {
    BOOL soubi_f_;
    BOOL def_f_;
    s64 id_shot_;
    s32 step_;
    s32 slot_;
    void Init()
    {
      Zeromem(this);
      id_shot_ = NGNUM;

    }
  };

  class TUGRcSoubi :public TUGRcNormal
  {
  protected:
    void DoActionSub();
    void DoMakeGroupSub2();
    void DoSetLimY2(s32 limu, s32 hhh);
    void DoUpdateSub2(BOOL draw_f);
  public:
    TUGRcSoubiSt stSoubi_;
    TUGBtnSoubi ugbtn_detach_;
    TUIObj* step_;
    //コンストラクタ
    explicit TUGRcSoubi(TUIGenLayer* player);
    virtual ~TUGRcSoubi(void);
    void SetBaseDt(s32 step, s32 slot);
    void SetBaseDt_ShSet(s32 step, s32 slot);
    void SetOpenLvDt(s64 openlv);
    void SetShotDt(s64 id_shot, BOOL defshot_f);
    void SwitchEdit(BOOL edit_f);
    void SetShotDt_ShSet(s64 id_shot, BOOL defshot_f);
    void ClearShotDt();

    s32 GetSlot();
    s32 GetShotID();

    BOOL IsActRes(enUGRcSoubiAct res);//返値ゲット
  };

}
