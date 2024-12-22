#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGGen.h"
#include "dbUGChKantoku.h"
namespace db
{
  enum enKantokuSerifu
  {
    ks_catchwait,
    ks_catch,
    ks_shotwait,
    ks_shot,
    ksEND,
  };

  struct TUGShKantokuSt
  {
    BOOL enterKantoku_f_;//キャッチ押され瞬間
    BOOL shotDraw_f_;//キャッチゲージ描画
    BOOL KantokuBarDraw_f_;//キャッチゲージ描画
    s32 KantokuBar_c_;
    s32 KantokuMaru_c_;
    BOOL offence_f_;//キャッチゲージ描画
    void Init()
    {
      Zeromem(this);
    }
  };

  
  //キャッチ
  class TUGShKantoku : public TUGGen
  {
  protected:
    TUGChKantoku ugkantoku_;
    std::vector<TUIObj*> serifu_;

    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGShKantokuSt stShKantoku_;
    //コンストラクタ
    explicit TUGShKantoku(TUIGenLayer* player);
    virtual ~TUGShKantoku(void);
    void MakeGroup(TUIObj* parent);
    void SerifuSelect(enKantokuSerifu sno);
  };
 
}