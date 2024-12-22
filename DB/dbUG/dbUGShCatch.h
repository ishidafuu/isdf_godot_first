#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGGen.h"

namespace db
{
  class TUGShKantoku;

  struct TUGShCatchSt
  {
    BOOL enterCatch_f_;//キャッチ押され瞬間
    BOOL shotDraw_f_;//キャッチゲージ描画
    BOOL catchBarDraw_f_;//キャッチゲージ描画
    s32 catchBar_c_;
    s32 catchMaru_c_;
    BOOL offence_f_;//キャッチゲージ描画
    s32 catchPos_;
    s32 ncaLen_;
    void Init()
    {
      Zeromem(this);
    }
  };

  
  //キャッチ
  class TUGShCatch : public TUGGen
  {
  protected:
    TUIObj* catch_;
    TUIObj* barNG_;
    TUIObj* barCa_;
    //TUIObj* barCa2_;
    TUIObj* barNCa_;
    std::vector<TUIObj*> maru_;
    TUGShKantoku* ugshkantoku_;

    void Refresh();
    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGShCatchSt stShCatch_;
    //コンストラクタ
    explicit TUGShCatch(TUIGenLayer* player);
    virtual ~TUGShCatch(void);
    void MakeGroup(TUIObj* parent, TUGShKantoku* ugshkantoku);
  };
 
}