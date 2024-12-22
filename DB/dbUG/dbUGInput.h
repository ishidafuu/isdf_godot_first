#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGLabel.h"
#include "dbUGRadio.h"
#include "dbUGInputKey.h"
//#include "dbLyUGInputGrp.h"

namespace db
{
  enum enUGInputAct
  {
    enUGInputAct_OK,
    enUGInputAct_Back,
  };

  struct TUGInputSt
  {
    s32 spacecode_;
    s32 outw_;
    BOOL alponly_f_;
    s32 alllen_;
    s32 tabno_;
    s32 cursor_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //文字入力
  class TUGInput : public TUGGen
  {
  protected:
    void ClearInputBtn();
    void ClearOutBtn();
    void ClearOutput();
    s32 GetWord(s32 nowno, s32 dakuon);
    s32 DecideItems();
    void InputWord(s32 itemNo);
    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGInputSt stInput_;

    TUIObj* inputfield_;//入力フィールド
    TUIObj* outputfield_;//出力フィールド
    TUIObj* outputfieldend_;//出力
    TUGRadio tabradio_;
    std::vector<TUGButton*> tabbtn_;
    std::vector<TUGInputKey*> keys_;//各文字
    std::vector<TUGButton*> outs_;//各文字
    INTVECTOR2 outsword_;//各文字

    TUGButton ugbtn_decide_;//ＯＫボタン
    TUGButton ugbtn_back_;//もどるボタン

    //コンストラクタ
    explicit TUGInput(TUIGenLayer* player);
    virtual ~TUGInput(void);

    void MakeGroup(TUIObj* parent);
    BOOL IsActRes(enUGInputAct res);
    void Refresh(s32 outw, BOOL alponly_f);
    void Refresh(s32 outw, BOOL alponly_f, INTVECTOR defmcvec);
    std::string OutputString();
  };
 
}
