#pragma once
#include "dbInclude.h"
#include "dbMgEngineObj.h"
#include "dbMgUI.h"
namespace db
{  
  
  class TUIObj;
  class TUIGenLayer;

  //描画深度
  enum enUIDepth
  {
    UIDP_00BG = 100, //背景(最奥)      小さいほど奥
    UIDP_01BGOBJ = 1000, //背景オブジェ//デフォルト
    UIDP_02BGOBJHI = 10000000, //背景オブジェ
    UIDP_03OBJ = 20000000, //キャラオブジェ
		UIDP_03_5 = 27500000, //
		UIDP_03_6 = 30000000, //
    UIDP_04HEADER = 35000000, //ヘッダ
    UIDP_05BUTTON = 36000000, //ボタン
    UIDP_06BUTTONHI = 37000000, //ボタン
    UIDP_07BUTTONTOP = 38000000, //ボタン
    UIDP_08MSGFILTER = 39000000, 
    UIDP_09MSGBOX = 40000000,
    UIDP_10ALLFILTER = 41000000,
    UIDPNON = 600000000 //表示しない(小さい順にソートなので)
  };

  //描画モード
  enum enMvType
  {
    mvt_Normal,
    mvt_Acc,
    mvt_Brk,
  };
  //座標ステータス
  struct TUIObjZahyouSt
  {
    //座標
    f32 x_;
    f32 y_;
    //速度
    f32 dx_;
    f32 dy_;
    //開始点
    f32 stx_;
    f32 sty_;
    //目的点
    f32 tagx_;
    f32 tagy_;
    //加速
    f32 accx_;
    f32 accy_;
    //到着まで残り時間
    s32 eat_;
    f32 acctime_;

    enMvType mvtype_;
    BOOL stopping_f_;

    void Init()
    {
      Zeromem(this);
    }
  };

  //描画モード
  enum enEffect
  {
    ef_None,
    ef_Flash,
    ef_Flash2,
    ef_Brite,
    ef_Dark,
  };

  //グラフィックステータス
  struct TUIObjGrpSt
  {
    BOOL set_f_;//作成済み
    BOOL draw_f_;//描画するしない
    s32 partno_;//描画するパーツ番号
    BOOL linex_f_;//描画するパーツの並び
    BOOL ani_f_;
    s32 anino_;//現在のアニメーション番号
    s32 rideno_;//載ってる番号（0は背景）

    enUIDepth basedepth_;//描画深さレイヤー（Z）
    s32 revdepthLv_;//補正深度ドラムロールなど
    s32 revdepthLv2_;//補正深度ドラムロールなど
    TUIObj* ovobj_;//深度だけこれの上

    BOOL mirror_f_;//左右反転
    BOOL centerOrigin_f_;//描画の中心を指定
    BOOL under_f_;//画像の下辺を描画座標Ｙに合わせる
    BOOL rightShift_f_;//右詰

    BOOL filter_f_;//フィルター
    mid::TARGB filterargb_;//フィルター色
    s32 filterw_;//フィルター幅
    s32 filterh_;//フィルター高さ

    mid::enDrawMode drawmode_;//描画モード
    mid::TARGB defargb_;//復帰カラー
    mid::TARGB argb_;//カラー
    mid::TARGB argb2_;//カラー2

    //目的色
    mid::TARGB tagargb_;//カラー
    mid::TARGB tagargb2_;//カラー2
    //到着まで残り時間
    s32 alpeat_;
    s32 alpspd_;
    s32 coleat_;
    s32 colspd_;

    s32 sw_;
    s32 sh_;
    //トリミング
    s32 limL_;
    s32 limR_;
    s32 limU_;
    s32 limD_;

    s32 gx_;
    s32 gy_;

    enEffect effect_;//エフェクト状態

    //キャラ系レイヤ
    BOOL charLayer_f_;
    s32 charLayer_;
    void Init()
    {
      Zeromem(this);
      draw_f_ = TRUE;//一応デフォは描画で（こうしないと親が常に不描画になる）
      defargb_.SetDEFCOL();
      argb_.SetDEFCOL();
      argb2_.SetDEFCOL();
      basedepth_ = UIDP_01BGOBJ;//デフォルト
    }
  };
  //フォントステータス
  struct TUIObjFontSt
  {
    BOOL set_f_;//作成済み
    BOOL draw_f_;//描画するしない（タイマーボタンの数値表示を一時的に消すため）
    BOOL centerOrigin_f_;//描画の中心を指定
    BOOL rightShift_f_;//右寄せ
    BOOL mini_f_;//miniフォント
    //ベース座標からの相対位置
    s32 fx_;
    s32 fy_;
    s32 fontlen_;
    s32 drawlen_;

    mid::enDrawMode drawmode_;//描画モード
    mid::TARGB argb_;//カラー
    mid::TARGB argb2_;//カラー2

    //目的色
    mid::TARGB tagargb_;//カラー
    mid::TARGB tagargb2_;//カラー2

    void Init()
    {
      Zeromem(this);
      argb_.SetDEFCOL();
      argb2_.SetDEFCOL();
    }
  };
  //キャラグラフィックステータス
  struct TUIObjCharSt
  {
    BOOL set_f_;//作成済み
    BOOL front_f_;//最前面描画
    BOOL mirror_f_;//左右反転

    s32 cellno_;
    s32 liney_;
    s32 bmpoffsetno_;

    s32 bx_;
    s32 by_;

    s32 cx_;
    s32 cy_;

    void Init()
    {
      Zeromem(this);
    }
  };

  //描画モード
  enum enAnitype
  {
    at_Normal,
    at_Loop,
    at_Door,
    at_Effect,
  };

  //アニメーションステータス
  struct TUIObjAniSt
  {
    BOOL set_f_;//セット
    INTVECTOR anivec_;//全コマ登録
    s32 komano_;//番号
    s32 ani_c_;//カウンタ
    s32 itv_;//インターバル
    enAnitype anitype_;//エフェクト
    void Init()
    {
      set_f_ = FALSE;
      anivec_.clear();
      komano_ = 0;
      ani_c_ = 0;
      itv_ = 0;
      anitype_ = at_Normal;
    }
  };

  //
  enum enTouchState
  {
    ts_None,//さわられてない
    ts_Enter,//指が載った瞬間
    ts_Enter2,//指が載っている
    ts_Cancel,//タッチしたまま離れる（キャンセル）
    ts_Decide,//その場で離れる（ＯＫ）
  };
  //ボタンステータス
  struct TUIObjBtnSt
  {
    BOOL btn_f_;//ボタンとして使うかどうか
    BOOL slideok_f_;
    BOOL autosize_f_;//画像サイズからボタンサイズを持ってくる
    enTouchState touch_;

    //追加サイズ
    s32 adw_;
    s32 adh_;

    //主にリスト用
    s32 limL_;
    s32 limR_;
    s32 limU_;
    s32 limD_;

    BOOL modal_f_;//インプットが聞かないモーダル状態でも聞く（メッセージボックスボタンなど）

    void Init()
    {
      Zeromem(this);
    }
  };
  //スワイプステータス（カメラ操作）
  struct TUIObjCamSwipeSt
  {
    BOOL camswipe_f_;
    s32 posX_;
    s32 posXBegin_;
    s32 posY_;
    s32 posYBegin_;
    s32 left_;
    s32 top_;
    s32 right_;
    s32 bottom_;
    BOOL swlim_f_;
    s32 limL_;
    s32 limR_;
    s32 limU_;
    s32 limD_;

    BOOL swp_f_;
    void Init()
    {
      Zeromem(this);
    }
  };
  //スワイプステータス（ライン操作）
  struct TUIObjLineSwipeSt
  {
    BOOL swipestop_f_;//一時的にスワイプ処理停止

    BOOL linex_f_;
    BOOL liney_f_;
    s32 posX_;
    s32 posXBegin_;
    s32 posY_;
    s32 posYBegin_;
    s32 width_;
    //s32 height_;
    s32 dX_;
    s32 dY_;

    s32 limL_;
    s32 limR_;
    s32 limU_;
    s32 limD_;
    s32 minY_;
    s32 maxY_;


    BOOL genswp_f_;
    BOOL swp_f_;

    void Init()
    {
      Zeromem(this);
    }
  };



  struct TUIObjSt
  {
    static const s32 ANILEN = 8;
    TUIObjZahyouSt stZahyou_;
    TUIObjGrpSt stGrp_;
    TUIObjFontSt stFont_;
    TUIObjCharSt stChar_;
    TUIObjAniSt stAni_[ANILEN];//登録しておける
    TUIObjBtnSt stBtn_;
    TUIObjCamSwipeSt stCamSw_;
    TUIObjLineSwipeSt stLineSw_;
    void Init()
    {
      stZahyou_.Init();
      stGrp_.Init();
      stFont_.Init();
      stChar_.Init();
      stBtn_.Init();
      stCamSw_.Init();
      stLineSw_.Init();
      for (s32 i = 0; i < ANILEN; ++i) stAni_[i].Init();
    }
  };

  //スクリプトオブジェ
  class TUIObj
  {
  protected:
    TMgEngineObj* pmgEO_;
    TMgUI* pmgUI_;
    //TUILayer* player_;
    base::TGrp* pgr_;//グラフィックのポインタ
    base::TGrPart* pgr_part_;//グラフィックのポインタ
    base::TGrTile* pgr_tile_;//グラフィックのポインタ
    base::TGrTile2* pgr_tile2_;//グラフィックのポインタ

    TMgCharGrp* pmgchar_;//キャラグラのポインタ
    TUIObj* parent_;//親オブジェ（に載ってる）
    
    //std::vector<TUIObj*> childlen_;//子オブジェ(ボタン押され判定で使用)
    mid::TBmpObj bmofont_;//フォントbmp
    base::TGrp grfont_;//フォントgrp
    s32 GetDrawDepth();
    
    void DrawFont();//フォント描画
    void DrawFilter();//フィルター描画
    void DrawObj();//オブジェクト描画;
    void DrawChar();//ボディ描画;
  public:
    TUIObjSt stSO_;

    //コンストラクタ
    explicit TUIObj(TMgEngineObj* pmgEO, TMgUI* pmgUI);
    virtual ~TUIObj(void);
    BOOL IsDraw();
    
    //オブジェセッティング****************************************************
    void o_SetParent(TUIObj* parent);//親設定
    void o_SetObjNull();//グラフィックセット
    void o_SetObjGrp(base::TGrp* pgr);//グラフィックセット
    void o_SetObjGrPart(base::TGrPart* pgrpart);//パーツグラフィック
    void o_SetObjGrpTile(base::TGrTile* pgrtile);//タイルグラフィック
    void o_SetObjGrpTile2(base::TGrTile2* pgrtile2, s32 tileno);//タイルグラフィック
    void o_SetObjMgCharGrp(TMgCharGrp* pmgchar);//キャラグラフィック
    void o_SetObjFilterGrp(s32 fw, s32 fh, s32 alp, s32 rrr, s32 ggg, s32 bbb);//フィルター（グラフィックは共通。サイズと基礎αを指定）
    BOOL o_IsSameGrp(base::TGrp* pgr);//比較
    //座標処理****************************************************
    void z_SetPos(s32 x, s32 y);
    void z_SetPos_SafeL(s32 x, s32 y);
    void z_SetPos_SafeLF(s32 x, s32 y);
    void z_SetPos_SafeR(s32 x, s32 y);
    void z_SetPos_Full(s32 x, s32 y);
    void z_SetPos_FullBtm(s32 x, s32 y);
    void z_MoveSetspd(f32 tagx, f32 tagy, f32 spd);
    void z_MoveSetspd_Full(f32 tagx, f32 tagy, f32 spd);
    void z_MoveSetspd_SafeL(f32 tagx, f32 tagy, f32 spd);
    void z_MoveBrk(f32 tagx, f32 tagy, f32 spd, s32 eat);
    void z_MoveBrk_SafeL(f32 tagx, f32 tagy, f32 spd, s32 eat);
    void z_MoveAcc(f32 tagx, f32 tagy, f32 spd, f32 acc);
    void z_MoveAcc_SafeL(f32 tagx, f32 tagy, f32 spd, f32 acc);
	void z_SetStop(void);
    BOOL z_IsStop();
    void z_Moving();
    void z_GetGlobalPos(f32* x, f32* y);
	void z_GetPos(f32* x, f32* y);
    //描画処理****************************************************
    void g_SetDraw(BOOL draw_f);
    void g_SetGrpPos(s32 x, s32 y);
    void g_SetPartNo(s32 partNo);
    void g_SetPartLine(BOOL linex_f);
    void g_SetSrcW(s32 sw);
    void g_SetSrcH(s32 sh);
    TRect g_GetSize();
    void g_SetOvOvj(TUIObj* ovobj);
    void g_SetDepth(enUIDepth depth);
    void g_SetRevDepthLv(s32 revdepthlv);
    void g_SetRevDepthLv2(s32 revdepthlv2);
    void g_SetCenter(BOOL center_f);
    void g_SetUnder();
    void g_SetRightShift();
    void g_SetMirror(BOOL mirror_f);
    void g_SetLimX(s32 liml, s32 www);
    void g_SetLimY(s32 limu, s32 hhh);
    void g_SetDrawmode(mid::enDrawMode drawmode);
    void g_SetARGB(BOOL def_f, s32 aaa, s32 rrr, s32 ggg, s32 bbb);
    void g_SetAlp(BOOL def_f, s32 aaa);
    void g_SetRGB(BOOL def_f, s32 rrr, s32 ggg, s32 bbb);
    void g_ResetDefCol();
    void g_SetCharLayer(s32 chary);
    void g_SetCharHaveDepth(s32 depth);
    void g_EfReset();
    void g_Flash();
    void g_Flash2(u8 rrr, u8 ggg, u8 bbb);
    void g_Bright();
    void g_Dark();
    
    void g_Fade(BOOL out_f);
    void g_Fade_Ex(BOOL out_f, s32 eat);
    void g_GetOverDepth(enUIDepth basedepth, s32* overdepth, TUIObj* ovobj);
    void g_ColMoving();
    void g_AlpMoving();
    void g_ResetBaseDepth(enUIDepth* basedepth, TUIObj* ovobj);
    //フォント処理****************************************************
    //フォントに関してはオブジェごとにテクスチャ管理
    void f_MakeFont(const char* msg, s32 fontcol);
    void f_MakeFont_Ht(const char* msg, s32 fontcol);
    void f_MakeFont_Len(const char* msg, s32 fontcol, s32 maxlen);
    void f_MakeFont_Colnum(const char* msg, s32 fontcol, s32 numcol);
    void f_MakeFont_Ex(const char* msg, u32 draw_fs, s32 fontcol, s32 numcol, s32 maxlen);
    void f_MakeFont_Mini(const char* msg, s32 fontcol);
    void f_MakeFontMC(s32 mojicode, s32 fontcol);
    void f_MakeFontMC_Ex(s32 mojicode, u32 draw_fs, s32 fontcol);
    void f_SetFontDraw(BOOL draw_f);
    void f_SetFontDrawLen(s32 len);
    void f_FontPos(s32 fx, s32 fy);
    void f_FontPosCenter(s32 fx, s32 fy);
    void f_SetCenter(BOOL center_f);
    void f_SetRightShift();
    //void f_SetCenter();
    //キャラ処理****************************************************
    //void c_SetCharDraw(BOOL draw_f);
    void c_SetCellNo(s32 cellno);
    void c_SetMirror(s32 mirror_f);
    void c_SetBaseLine(s32 liney);
    void c_SetPos(s32 cx, s32 cy);
    void c_SetBMPos(s32 bx, s32 by);
    void c_SetBmpOffsetNo(s32 offset);
    s32 c_GetDepth();
    TDtCell* c_GetDtCell();
    //アニメ処理****************************************************
    void a_AddAnivec(s32 stno, s32 edno, s32 itv, enAnitype anitype);
    void a_AddAnivec(INTVECTOR* vec, s32 itv, enAnitype anitype);
    void a_StopAni();
    void a_SetAniNo(s32 anino, BOOL restart_f);//ゼロでアニメ止めて通常絵
    void a_Animation();
    //ボタン処理****************************************************
    void b_SetBtn(BOOL btn_f);
    void b_SetModal(BOOL modal_f);
    void b_SetSlideOK(BOOL slideok_f);
    void b_BtnLimX(s32 liml, s32 www);
    void b_BtnLimY(s32 limu, s32 hhh);
    void b_AddSize(s32 adw, s32 adh);
    void b_Enter();//指が載った
    void b_Enter2();//指が載っている
    void b_Cancel();//タッチしたまま離れる（キャンセル）
    void b_Decide();//その場で離れる（ＯＫ）
    BOOL b_IsDecide();
    BOOL b_IsEnter(BOOL enter2_f);
    TRect b_GetAutoSize();
    BOOL b_IsBaseSwiping();
    BOOL b_IsPile();
    BOOL b_IsBtn();
    void b_Scanning();
    //カメラスワイプ処理****************************************************
    void sc_CamSwipe(BOOL swipe_f);
    void sc_CamSwipeLimX(s32 liml, s32 www);
    void sc_CamSwipeLimY(s32 limu, s32 hhh);
    void sc_CamSwipeData(s32 liml, s32 limu, s32 www, s32 hhh);
    void sc_CamSetPos(s32 posx, s32 posy);//外部からの位置修正
    void sc_CamSetLimit(BOOL lim_f);//一時的にカメラ限界外す
    void sc_CamGenSwipePos(s32 posx, s32 posy);
    void sc_CamSwipePos(s32 swpx, s32 swpy, BOOL kansei_f);
    BOOL sc_IsCamSwipe(s32 dist);
	BOOL sc_IsCamEnter();
	BOOL sc_IsCamEnter2();
    void sc_CamSwiping();
    //ラインスワイプ処理****************************************************
    void sl_LineSwipeX(BOOL swipe_f);
    void sl_LineSwipeY(BOOL swipe_f);
    void sl_LineSwipeLimX(s32 liml, s32 www);
    void sl_LineSwipeLimY(s32 limu, s32 hhh);
    void sl_LineSwipeWidth(s32 www);
    void sl_LineSwipeHeight(s32 minY, s32 maxY);
    void sl_LineSetPos(s32 posx, s32 posy);
    void sl_LineGenSwipePosX(s32 posx);
    void sl_LineGenSwipePosY(s32 posy);
    void sl_LineSwipePosX(s32 swpx, BOOL kansei_f);
    void sl_LineSwipePosY(s32 swpy, BOOL kansei_f);
    void sl_StopLineSwipe(BOOL stop_f);
    void sl_LineSwiping();
    s32 sl_LineGetX();
    s32 sl_LineGetY();
    void sl_LineRoundX(s32 round);
    void sl_LineRoundY(s32 round);
    //更新処理****************************************************
    void Update(BOOL draw_f);//更新
    void Update(BOOL draw_f, BOOL noinput_f);//更新

    void ReloadTex();
    //void Update_NI(BOOL draw_f);//入力処理無し更新
  };
}