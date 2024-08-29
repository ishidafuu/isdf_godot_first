#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"
#include "dbUGChKantoku.h"
#include "dbUGChCaptain.h"
#include "dbUGChClerk.h"
#include "dbUGChCaptain.h"
#include "dbUGFilter.h"
#include "dbUGTeam.h"
//#define __GACHA_USE_BTN__		// ガチャ内で画面タッチ誘導用のボタンを表示させる
namespace db
{
  enum enUGGachaAct
  {
    enUGGachaAct_Half,
    enUGGachaAct_End,
    enUGGachaAct_CupsuleOut,
  };

  enum enUGGachaType
  {
    enUGGachaType_GachaEtc,
    enUGGachaType_GachaStar,
    enUGGachaType_GachaTodd,
    enUGGachaType_Hukubiki,
  };



  class TUILayer;

  struct TCapsule
  {
    s32 move_f_;
    s32 posX_;
    TUIObj* obj_;//カプセル
    void Init()
    {
      move_f_ = FALSE;
      posX_ = 0;
    }
  };

  struct TUGGachaSt
  {
    static const s32 CAPNUM = 8;
    s32 garagara_c_;

    s32 fallstep_;
    s32 posY_;
    s32 dY_;

    s32 punch_c_;

    BOOL panchOK_f_;
    BOOL bigpanchOK_f_;
    s32 panchtype_;
    s32 nowrarity_;
    s32 nowpattern_;
    s32 nowpattern2_;

    s32 handle_c_;
    s32 nextcap_;
    s32 bigcuprarity_;
    s32 bigcuphp_;
    s32 bigcuphpmax_;
    s32 bigbreak_c_;

    s32 cupstop_c_;

    BOOL gachaout_f_;
    s32 out_c_;

    s32 flash_c_;

    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGGacha : public TUGGen
  {
  protected:
    static const s32 CAPNUM = 8;

    TUIObj* gacha_;//ガチャ筐体
    //TUIObj* crank_;//がらがらクランク
    TUIObj* handle_;//レバー
    TUIObj* exmark_;//びっくりマーク
    TUIObj* hibi_;//大玉のひび
#ifdef __GACHA_USE_BTN__
	TUIObj* btn_;	// ボタン
#endif // #define __GACHA_USE_BTN__
    
    TUIObj* bigcapsule_;//大玉カプセル
    TUGChKantoku* ugkantoku_;//監督
	TUGChCaptain* ugcap_;//キャプテン
    TUGChClerk* ugcleak_;//店員
    TUGFilter ugfilter_;
    TCapsule capsule_[CAPNUM];
    enUGGachaType gachatype_;

    void SetCapsule(s32 rarity, BOOL bigopen_f);//カプセル
    void SetBigCapsule(s32 rarity);//巨大カプセル
    void Mv_FallGacha();//ガチャ落ちてくる
    void Mv_Punch();//パンチ
    void Mv_BigPunch();//パンチ
    void Mv_Capsule();//カプセル
    void Mv_OutGacha();//ガチャはける
    void Mv_Flash();//フラッシュ
    void SetPattern();//パターン決定
    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGGachaSt stGacha_;
    INTVECTOR pattern_;
    INTVECTOR rarity_;
    //コンストラクタ
    explicit TUGGacha(TUIGenLayer* player);
    virtual ~TUGGacha(void);
    void MakeGroup(TUIObj* parent, TUGChKantoku* ugkantoku, TUGChCaptain* ugcap, TUGChClerk* ugcleak);

    BOOL IsActRes(enUGGachaAct res);
    void SetGachaType(enUGGachaType gachatype);//ガチャタイプ

    void Act_FallGacha();//ガチャ落ちてくる
    void Act_Punch();//パンチ入力
    void TapField();
  };
 
}
