#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGButton.h"
//#include "dbUGLabel.h"
namespace db
{
  struct TRadioSt
  {
    s32 baseX_;
    s32 baseY_;
    BOOL enter2_f_;//押された
    BOOL radio_f_;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGRadio : public TUGGen
  {
  protected:
    std::vector<TUGButton*> tabbtn_;
    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TRadioSt stRadio_;
    //コンストラクタ
    explicit TUGRadio(TUIGenLayer* player);
    virtual ~TUGRadio(void);

    void MakeGroup(TUIObj* parent, s32 baseX, s32 baseY);
    void AddButton(TUGButton* ugbtn, s32 distX);//ボタンは詳細を決める必要があるので、外部で作って持たせる
    void ResetSelect();
    void SetSelect(s32 tabno);
    void SetRadio(BOOL radio_f);
		TUGButton* GetSelect(s32 tabno);
		void Clear();
  };
 
}
