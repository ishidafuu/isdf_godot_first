#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"
#include "dbUGChKantoku.h"
#include "dbUGChCaptain.h"
#include "dbUGChClerk.h"
#include "dbUGFilter.h"

//#define __HUKUBIKI_USE_BTN__		// ふくびき内で画面タッチ誘導用のボタンを表示させる

namespace db
{
  enum enUGHukuAct
  {
    enUGHukuAct_Half,
    enUGHukuAct_End,
    enUGHukuAct_CupsuleOut,
  };

  enum enUGHukuType
  {
    enUGHukuType_HukuEtc,
    enUGHukuType_HukuStar,
    enUGHukuType_HukuTodd,
    enUGHukuType_Hukubiki,
  };



  class TUILayer;

  struct THukuCapsule
  {
    s32 move_f_;
    s32 posX_;
	s32 posY_;
    TUIObj* obj_;//カプセル
    void Init()
    {
      move_f_ = FALSE;
      posX_ = 0;
	  posY_ = 0;
    }
  };

  struct TUGHukuSt
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

    BOOL Hukuout_f_;
    s32 out_c_;

    s32 flash_c_;

    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGHuku : public TUGGen
  {
  protected:
    static const s32 CAPNUM = 8;

    TUIObj* Huku_;//ガチャ筐体
	TUIObj* crank_;//がらがらクランク
	TUIObj* handle_;//レバー
    TUIObj* exmark_;//びっくりマーク
    TUIObj* hibi_;//大玉のひび
#ifdef __HUKUBIKI_USE_BTN__
	TUIObj* btn_;	// ボタン
#endif    
    TUIObj* bigcapsule_;//大玉カプセル
    TUGChKantoku* ugkantoku_;//監督
	TUGChCaptain* ugcap_;//キャプテン
    TUGChClerk* ugcleak_;//店員
    TUGFilter ugfilter_;
    THukuCapsule capsule_[CAPNUM];
	THukuCapsule capsuleEx_;	// 大玉カプセル演出前用
    //enUGHukuType Hukutype_;

    void SetCapsule(s32 rarity, BOOL bigopen_f);//カプセル
    void SetBigCapsule(s32 rarity);//巨大カプセル
    void Mv_FallHuku();//ガチャ落ちてくる
    void Mv_Punch();//パンチ
	void Mv_PunchN();	// ノーマル版
	void Mv_PunchR();	// レア版
    void Mv_BigPunch();//パンチ
    void Mv_Capsule();//カプセル
    void Mv_OutHuku();//ガチャはける
    void Mv_Flash();//フラッシュ
    void SetPattern();//パターン決定
    void DoUpdate(BOOL draw_f);
    void DoAction();

	// レア演出ステップ
	s32 stepR_;


	void Mv_Huku(void);	// 筐体のアニメーション
	s32 hukuAnimSpd_;
	s32 hukuAnimInterval_;
	s32 hukuAnimNo_;

  public:
    TUGHukuSt stHuku_;
    INTVECTOR pattern_;
    INTVECTOR rarity_;
    //コンストラクタ
    explicit TUGHuku(TUIGenLayer* player);
    virtual ~TUGHuku(void);
    void MakeGroup(TUIObj* parent, TUGChKantoku* ugkantoku, TUGChCaptain* ugcap, TUGChClerk* ugcleak);

    BOOL IsActRes(enUGHukuAct res);

    void Act_FallHuku();//ガチャ落ちてくる
    void Act_Punch();//パンチ入力
    void TapField();
  };
 
}
