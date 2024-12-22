#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGInputTip.h"
#include "dbUGRadio.h"
//#include "dbLyUGInputGrp.h"

namespace db
{
  const s32 UGI_DAKUON = 4;


  const s32 UGI_TAB = 4;
  const s32 UGI_LINELEN = 13;
  const s32 UGI_LINENUM = 5;
  const s32 UGI_ALLLINELEN = UGI_LINELEN * UGI_LINENUM;
  const s32 UGI_ALLWORD = UGI_ALLLINELEN * UGI_TAB;

  //ユーザーコードなど少なくてタブないやつ
  const s32 UGI_LINELEN_S = 8;
  const s32 UGI_ALLLINELEN_S = UGI_LINELEN_S * UGI_LINENUM;
  const s32 UGI_ALLWORD_S = UGI_ALLLINELEN_S;


  //各入力ボタン
  class TUGInputKey : public TUGGen
  {
  protected:
    s32 GetWord(s32 nowno, s32 dakuon);
    s32 GetWordAlp(s32 nowno);
    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    BOOL tooltip_f_;
    TUGInputTip tooltip_;//チップラベルはここで生成
    s32 btnword_[UGI_DAKUON];//各文字
    //コンストラクタ
    explicit TUGInputKey(TUIGenLayer* player);
    virtual ~TUGInputKey(void);
    
    BOOL IsDecide();
    void MakeGroup(TUIObj* parent, s32 nowno, BOOL alponly_f);

  };

 
}
