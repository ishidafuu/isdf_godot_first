#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"
#include "dbUGChKantoku.h"
#include "dbUGChCaptain.h"
#include "dbUGChClerk.h"
#include "dbUGFilter.h"

namespace db
{
  enum enUGNaBoxAct
  {
    enUGNaBoxAct_Half,
    enUGNaBoxAct_End,
  };

  class TUILayer;

  struct TUGNaBoxSt
  {
    static const s32 PLATEMAX = 30;
    s32 boxSize_;
    s32 fallstep_;
    s32 posY_;
    s32 dY_;
    BOOL orderOK_f_;
    s32 select_c_;
    BOOL gachaout_f_;
    BOOL ggout_f_;
    s32 out_c_;
    BOOL opened_f_[PLATEMAX];//ナンバー
    s32 lastno_;
    s32 decideno_;
    BOOL otona_f_;
    s32 endwait_c_;
    void Init()
    {
      Zeromem(this);
      lastno_ = NGNUM;
    }
  };

  class TUGNaBox : public TUGGen
  {
  protected:
    static const s32 PLATEMAX = 30;
    s64 mid_nabox_;
    TUIObj* boxgacha_;//ガチャ筐体
    TUIObj* boxnumber_;//ナンバー
    //TUIObj* exmark_;//びっくりマーク
    TUIObj* plate_[PLATEMAX];//ナンバー
    TUGChKantoku* ugkantoku_;//監督
	TUGChCaptain* ugcap_;//キャプテン
    TUGChClerk* ugcleak_;//店員
    TUGFilter ugfilter_;
    INTVECTOR restno_;

    void Mv_FallBoxGacha();//ガチャ落ちてくる
    void Mv_OutBoxGacha();//ガチャはける
    void Mv_OutGG();//ガチャはける
    void Mv_Select();//フラッシュ
    void Mv_EndWait();//間
    void Mv_OtonaSelect();//フラッシュ
    void SetPattern();//パターン決定
    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGNaBoxSt stBoxGacha_;
    //コンストラクタ
    explicit TUGNaBox(TUIGenLayer* player);
    virtual ~TUGNaBox(void);
    void MakeGroup(TUIObj* parent, TUGChKantoku* ugkantoku, TUGChCaptain* ugcap, TUGChClerk* ugcleak);
    BOOL IsActRes(enUGNaBoxAct res);

    void Act_FallNaBox();//ガチャ落ちてくる
    void Act_FallGG();//GG落ちてくる
    void Act_Order();//パンチ入力
    void TapField();

    //あたり番号セット
    void SetNaBoxResult(BOOL otona_f);
    void RefreshBox(s64 mid_nabox);
  };
 
}
