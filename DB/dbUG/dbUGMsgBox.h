#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGButton.h"
#include "dbUGRName.h"
namespace db
{
 
  enum enUGMsgBoxAct
  {
    enUGMsgBoxAct_OK,
    enUGMsgBoxAct_NG,
  };

  struct TUGMsgSt
  {
    BOOL okbtn_f_;
    BOOL title_f_;
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGMsgBox :public TUGGen
  {
  public:
    //コンストラクタ
    explicit TUGMsgBox(TUIGenLayer* player);
    virtual ~TUGMsgBox(void);
    void MakeGroup(TUIObj* parent);
    void SetData(STRVECTOR* strvec, BOOL ngbtn_f);
    void SetUseStar(STRVECTOR* strvec, s32 num);
    void SetUseTodd(STRVECTOR* strvec, s32 num, BOOL get_f);
    BOOL IsActRes(enUGMsgBoxAct res);//返値ゲット
  protected:
    void DoUpdate(BOOL draw_f);
    void DoAction();
    TUGMsgSt stMsg_;
    TUGButton ugbtn_ok_;//ＯＫボタン
    TUGButton ugbtn_ng_;//NGボタン
    TUIObj* panel_;
    std::vector<TUIObj*> text_;
    TUGRName ugcredit_;
  };
 
}
