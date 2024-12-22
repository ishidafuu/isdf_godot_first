#include "dbUIObj.h"
//#include "dbUIGenLayer.h"
namespace db
{

  const s32 OVERDEPTH = 200;
  const s32 REVDEPTHMAG = 1000;
  const s32 OVERDEPTHOBJ = 10;
  const s32 FONTDEPTH = 100;
  //const s32 CHARDEPTH = base::DPT_3OBJ;
  //const s32 FRONTDEPTH = base::DPT_4OVER_ICO;
  //const s32 FRONTDEPTHADD = 10000000;
  const s32 CNTTOHEAD = 84;//中心からヘッダまで
  const s32 CNTTOSAFEX = 160;
  const s32 FONTYREV = -2;//0,0を指定して芯を食うように
  const s32 FONTMAXLEN = 64;


  const s32 MAXALP = 0xFF;
  //コンストラクタ
  TUIObj::TUIObj(TMgEngineObj* pmgEO, TMgUI* pmgUI) :
    pmgEO_(pmgEO),
    pmgUI_(pmgUI),
    grfont_(&pmgEO->mgOrder_, &bmofont_)
  {
    stSO_.Init();
    bmofont_.Init();
    pgr_ = NULL;
    pgr_part_ = NULL;
    pgr_tile_ = NULL;
    pmgchar_ = NULL;
    parent_ = NULL;
    pmgUI->AddUIObj(this);//再構築用にフォントアドレスを登録
  }

  TUIObj::~TUIObj(void)
  {
    bmofont_.DeleteTex();
    //シーン移動待ちでなければリストからはずす
    pmgUI_->RemoveUIObj(this);
  }

  //オブジェセッティング****************************************************

  //親オブジェ
  void TUIObj::o_SetParent(TUIObj* parent)
  {
    //UGにAddされた後に確定させる場合、作成時、NULLを渡しておけるようにする
    if (parent == NULL) return;

    //親オブジェに自分を子オブジェとして追加
    //解放される可能性があるので親から子は管理しない
    //ＵＧ系で管理するようにする
    //parent->childlen_.push_back(this);
    parent_ = parent;
    stSO_.stGrp_.limU_ = parent->stSO_.stGrp_.limU_;
    stSO_.stGrp_.limD_ = parent->stSO_.stGrp_.limD_;
    stSO_.stGrp_.limL_ = parent->stSO_.stGrp_.limL_;
    stSO_.stGrp_.limR_ = parent->stSO_.stGrp_.limR_;


    //キャラレイヤ系であればコピー
    if (parent_->stSO_.stGrp_.charLayer_f_)
    {
      stSO_.stGrp_.charLayer_f_ = TRUE;
      stSO_.stGrp_.charLayer_ = parent_->stSO_.stGrp_.charLayer_ + XYMAG;
    }
  }
  //オブジェのグラフィックをセット
  void TUIObj::o_SetObjNull()
  {
    pgr_ = NULL;
    pgr_part_ = NULL;//上書きが効くように
    pgr_tile_ = NULL;
    pgr_tile2_ = NULL;
    stSO_.stGrp_.set_f_ = FALSE;
    //stSO_.stGrp_.draw_f_ = TRUE;
    //stSO_.stGrp_.depth_ = base::DPT_3OBJ;
  }
  //オブジェのグラフィックをセット
  void TUIObj::o_SetObjGrp(base::TGrp* pgr)
  {
    pgr_ = pgr;
    pgr_part_ = NULL;//上書きが効くように
    pgr_tile_ = NULL;
    pgr_tile2_ = NULL;
    stSO_.stGrp_.set_f_ = TRUE;
    stSO_.stGrp_.draw_f_ = TRUE;
    //stSO_.stGrp_.depth_ = base::DPT_3OBJ;
  }
  //パーツグラフィックをセット
  void TUIObj::o_SetObjGrPart(base::TGrPart* pgrpart)
  {
    o_SetObjGrp(pgrpart);
    pgr_part_ = pgrpart;
    pgr_tile_ = NULL;
    pgr_tile2_ = NULL;
  }
  //タイルグラフィックをセット
  void TUIObj::o_SetObjGrpTile(base::TGrTile* pgrtile)
  {
    o_SetObjGrp(pgrtile);
    pgr_tile_ = pgrtile;
    pgr_part_ = NULL;//上書きが効くように
    pgr_tile2_ = NULL;
  }
  void TUIObj::o_SetObjGrpTile2(base::TGrTile2* pgrtile2, s32 tileno)//タイルグラフィック
  {
    o_SetObjGrp(pgrtile2);
    pgr_tile2_ = pgrtile2;
    pgr_tile_ = NULL;//上書きが効くように
    pgr_part_ = NULL;//上書きが効くように
    g_SetPartNo(tileno);//番号ものこの時点で保存
  }
  //キャラグラフィックをセット
  void TUIObj::o_SetObjMgCharGrp(TMgCharGrp* pmgchar)
  {
    stSO_.stChar_.set_f_ = TRUE;
    stSO_.stGrp_.draw_f_ = TRUE;
    pmgchar_ = pmgchar;
  }
  //フィルターをセット
  void TUIObj::o_SetObjFilterGrp(s32 fw, s32 fh, s32 alp, s32 rrr, s32 ggg, s32 bbb)
  {
    o_SetObjGrp(pmgUI_->pgr_UIFilter_);
    stSO_.stGrp_.filter_f_ = TRUE;
    stSO_.stGrp_.filterargb_.SetCOL(alp, rrr, ggg, bbb);
    stSO_.stGrp_.filterw_ = fw;
    stSO_.stGrp_.filterh_ = fh;
    g_SetARGB(TRUE, alp, rrr, ggg, bbb);
  }
  BOOL TUIObj::o_IsSameGrp(base::TGrp* pgr)//比較
  {
    return (pgr_ == pgr);
  }
  //座標処理****************************************************
  void TUIObj::z_SetPos(s32 x, s32 y)//ローカルポジション
  {
    stSO_.stZahyou_.Init();//動きを止める
    stSO_.stZahyou_.x_ = x;
    stSO_.stZahyou_.y_ = y;
  }
  void TUIObj::z_SetPos_SafeL(s32 x, s32 y)
  {
    z_SetPos(x - CNTTOSAFEX, y - CNTTOHEAD);
  }
  void TUIObj::z_SetPos_SafeLF(s32 x, s32 y)
  {
    z_SetPos(x - mid::midFullDotL_Hf(), y - CNTTOHEAD);
  }
  void TUIObj::z_SetPos_SafeR(s32 x, s32 y)
  {
    z_SetPos(x, y - CNTTOHEAD);
  }
  void TUIObj::z_SetPos_Full(s32 x, s32 y)
  {
    //224,128
    z_SetPos(x - mid::midFullDotL_Hf(), y - mid::midFullDotS_Hf());
  }
  void TUIObj::z_SetPos_FullBtm(s32 x, s32 y)
  {
    z_SetPos(x - mid::midFullDotL_Hf(), y + mid::midFullDotS_Hf());
  }
  void TUIObj::z_MoveSetspd_SafeL(f32 tagx, f32 tagy, f32 spd)
  {
    z_MoveSetspd(tagx - CNTTOSAFEX, tagy - CNTTOHEAD, spd);
  }
  void TUIObj::z_MoveSetspd(f32 tagx, f32 tagy, f32 spd)
  {
    //初速
    stSO_.stZahyou_.eat_ = 1;
    stSO_.stZahyou_.tagx_ = tagx;
    stSO_.stZahyou_.tagy_ = tagy;
    f32 lenx = (tagx - stSO_.stZahyou_.x_);
    f32 leny = (tagy - stSO_.stZahyou_.y_);
    f32 h = lib_num::Hypot(lenx, leny);
    if (h == 0.0f)
    {
      stSO_.stZahyou_.dx_ = 0;
      stSO_.stZahyou_.dy_ = 0;
      return;
    }
    stSO_.stZahyou_.dx_ = (lenx / h) * spd;
    stSO_.stZahyou_.dy_ = (leny / h) * spd;
    if (spd > 0) stSO_.stZahyou_.eat_ = (h / spd);

    if (stSO_.stZahyou_.eat_ < 1) stSO_.stZahyou_.eat_ = 1;

    stSO_.stZahyou_.mvtype_ = mvt_Normal;
    stSO_.stZahyou_.stopping_f_ = FALSE;
  }
  void TUIObj::z_MoveSetspd_Full(f32 tagx, f32 tagy, f32 spd)
  {
    z_MoveSetspd((tagx - mid::midFullDotL_Hf()), (tagy - mid::midFullDotS_Hf()), spd);
  }
  void TUIObj::z_MoveBrk_SafeL(f32 tagx, f32 tagy, f32 spd, s32 eat)
  {
    z_MoveBrk(tagx - CNTTOSAFEX, tagy - CNTTOHEAD, spd, eat);
  }
  void TUIObj::z_MoveBrk(f32 tagx, f32 tagy, f32 spd, s32 eat)
  {
    stSO_.stZahyou_.eat_ = eat;
    stSO_.stZahyou_.stx_ = stSO_.stZahyou_.x_;
    stSO_.stZahyou_.sty_ = stSO_.stZahyou_.y_;
    stSO_.stZahyou_.tagx_ = tagx;
    stSO_.stZahyou_.tagy_ = tagy;
    stSO_.stZahyou_.mvtype_ = mvt_Brk;
    //初速
    z_MoveSetspd(tagx, tagy, spd);
  }
  void TUIObj::z_MoveAcc_SafeL(f32 tagx, f32 tagy, f32 spd, f32 acc)
  {
    z_MoveAcc(tagx - CNTTOSAFEX, tagy - CNTTOHEAD, spd, acc);
  }
  void TUIObj::z_MoveAcc(f32 tagx, f32 tagy, f32 spd, f32 acc)
  {
    //初速
    stSO_.stZahyou_.tagx_ = tagx;
    stSO_.stZahyou_.tagy_ = tagy;
    z_MoveSetspd(tagx, tagy, spd);
    f32 lenx = (tagx - stSO_.stZahyou_.x_);
    f32 leny = (tagy - stSO_.stZahyou_.y_);
    f32 h = lib_num::Hypot(lenx, leny);
    if (h == 0.0f) return;
    stSO_.stZahyou_.accx_ = (lenx / h) * acc;
    stSO_.stZahyou_.accy_ = (leny / h) * acc;
    stSO_.stZahyou_.mvtype_ = mvt_Acc;

  }
  void TUIObj::z_SetStop()
  {
    //初速
    stSO_.stZahyou_.eat_ = 0;
    stSO_.stZahyou_.tagx_ = 0;
    stSO_.stZahyou_.tagy_ = 0;
    stSO_.stZahyou_.dx_ = 0;
    stSO_.stZahyou_.dy_ = 0;
    stSO_.stZahyou_.mvtype_ = mvt_Normal;
    stSO_.stZahyou_.stopping_f_ = TRUE;
  }
  BOOL TUIObj::z_IsStop()
  {
    //留まった瞬間
    return stSO_.stZahyou_.stopping_f_;
  }
  void TUIObj::z_Moving()
  {
    //ブレーキ
    stSO_.stZahyou_.stopping_f_ = FALSE;

    //残り時間減らす
    if (stSO_.stZahyou_.eat_ > 0)
    {
      //帳尻を合わせる
      if (lib_num::AprTo0(&stSO_.stZahyou_.eat_))
      {
        z_SetPos(stSO_.stZahyou_.tagx_, stSO_.stZahyou_.tagy_);//速度もゼロに
        stSO_.stZahyou_.stopping_f_ = TRUE;//留まった瞬間のみ
      }
      else
      {
        if ((stSO_.stZahyou_.mvtype_ == mvt_Acc)
          || (stSO_.stZahyou_.mvtype_ == mvt_Brk))
        {
          const f32 RCLEN = 1.0;//これ以内に入ったら到着にしてがたつきを防ぐ
          f32 lenx = (stSO_.stZahyou_.tagx_ - stSO_.stZahyou_.x_);
          BOOL rcx_f = FALSE;
          if (stSO_.stZahyou_.tagx_ > stSO_.stZahyou_.stx_)
          {
            if (lenx <= +RCLEN) rcx_f = TRUE;
          }
          else
          {
            if (lenx >= -RCLEN) rcx_f = TRUE;
          }

          if (rcx_f)
          {
            stSO_.stZahyou_.x_ = stSO_.stZahyou_.tagx_;
            stSO_.stZahyou_.dx_ = 0.0;
            stSO_.stZahyou_.accx_ = 0.0;
          }
          else
          {
            if (lenx != 0.0) stSO_.stZahyou_.accx_ = -(stSO_.stZahyou_.dx_ * stSO_.stZahyou_.dx_) / (2 * lenx);
          }

          f32 leny = (stSO_.stZahyou_.tagy_ - stSO_.stZahyou_.y_);
          BOOL rcy_f = FALSE;
          if (stSO_.stZahyou_.tagy_ > stSO_.stZahyou_.sty_)
          {
            if (leny <= +RCLEN) rcy_f = TRUE;
          }
          else
          {
            if (leny >= -RCLEN) rcy_f = TRUE;
          }

          if (rcy_f)
          {
            stSO_.stZahyou_.y_ = stSO_.stZahyou_.tagy_;
            stSO_.stZahyou_.dy_ = 0.0;
            stSO_.stZahyou_.accy_ = 0.0;
          }
          else
          {
            if (leny != 0.0)  stSO_.stZahyou_.accy_ = -(stSO_.stZahyou_.dy_ * stSO_.stZahyou_.dy_) / (2 * leny);
          }
        }
      }
    }

    //座標移動
    stSO_.stZahyou_.dx_ += stSO_.stZahyou_.accx_;
    stSO_.stZahyou_.dy_ += stSO_.stZahyou_.accy_;
    stSO_.stZahyou_.x_ += stSO_.stZahyou_.dx_;
    stSO_.stZahyou_.y_ += stSO_.stZahyou_.dy_;



    //加速終了はみだしチェック
    if (stSO_.stZahyou_.mvtype_ == mvt_Acc)
    {
      BOOL rcx_f = TRUE;
      if (stSO_.stZahyou_.accx_ > 0.0)
      {
        rcx_f = (stSO_.stZahyou_.x_ >= stSO_.stZahyou_.tagx_);
      }
      else if (stSO_.stZahyou_.accy_ < 0.0)
      {
        rcx_f = (stSO_.stZahyou_.x_ <= stSO_.stZahyou_.tagx_);
      }


      BOOL rcy_f = TRUE;
      if (stSO_.stZahyou_.accy_ > 0.0)
      {
        rcy_f = (stSO_.stZahyou_.y_ >= stSO_.stZahyou_.tagy_);
      }
      else if (stSO_.stZahyou_.accy_ < 0.0)
      {
        rcy_f = (stSO_.stZahyou_.y_ <= stSO_.stZahyou_.tagy_);
      }

      if (rcx_f && rcy_f)
      {
        z_SetPos(stSO_.stZahyou_.tagx_, stSO_.stZahyou_.tagy_);//速度もゼロに
      }
      else
      {
        if (rcx_f)
        {
          stSO_.stZahyou_.x_ = stSO_.stZahyou_.tagx_;
          stSO_.stZahyou_.accx_ = 0.0;
          stSO_.stZahyou_.dx_ = 0.0;
        }
        if (rcy_f)
        {
          stSO_.stZahyou_.y_ = stSO_.stZahyou_.tagy_;
          stSO_.stZahyou_.accy_ = 0.0;
          stSO_.stZahyou_.dy_ = 0.0;
        }
      }
    }

  }
  void TUIObj::z_GetGlobalPos(f32* x, f32* y)
  {
    *x += stSO_.stZahyou_.x_;
    *y += stSO_.stZahyou_.y_;
    //カメラスワイプ（スワイプの有効無効が途中で変わることがあるため、関わらず反映するように）
    //if (stSO_.stCamSw_.camswipe_f_)
    {
      *x -= stSO_.stCamSw_.posX_;
      *y -= stSO_.stCamSw_.posY_;
    }
    //親がある場合は親の座標もプラスする
    if (parent_ != NULL) parent_->z_GetGlobalPos(x, y);
  }
  void TUIObj::z_GetPos(f32* x, f32* y)
  {
    *x = stSO_.stZahyou_.x_;
    *y = stSO_.stZahyou_.y_;
  }
  //描画処理****************************************************
  void TUIObj::g_SetDraw(BOOL draw_f)
  {
    stSO_.stGrp_.draw_f_ = draw_f;
  }
  void TUIObj::g_SetGrpPos(s32 x, s32 y)
  {
    stSO_.stGrp_.gx_ = x;
    stSO_.stGrp_.gy_ = y;
  }
  void TUIObj::g_SetPartNo(s32 partNo)
  {
    stSO_.stGrp_.partno_ = partNo;
  }
  void TUIObj::g_SetPartLine(BOOL linex_f)
  {
    stSO_.stGrp_.linex_f_ = linex_f;
  }
  void TUIObj::g_SetSrcW(s32 sw)
  {
    stSO_.stGrp_.sw_ = sw;
  }
  void TUIObj::g_SetSrcH(s32 sh)
  {
    stSO_.stGrp_.sh_ = sh;
  }
  TRect TUIObj::g_GetSize()
  {
    TRect res;
    res.Bounds(0, 0, 0, 0);

    if (pmgchar_ != NULL)
    {
      res.Bounds(-16, -32, 32, 32);
    }
    else if (pgr_ == NULL)
    {
      res.Bounds(0, 0, 0, 0);
    }
    else if (pgr_tile_ != NULL)//タイル
    {
      s32 ptno = stSO_.stGrp_.partno_;
      res.Bounds(0, 0, pgr_tile_->GetTileW(ptno), pgr_tile_->GetTileH(ptno));
    }
    else if (pgr_tile2_ != NULL)//タイル2
    {
      s32 ptno = stSO_.stGrp_.partno_;
      res.Bounds(0, 0, pgr_tile2_->GetTileW(ptno), pgr_tile2_->GetTileH(ptno));
    }
    else if (pgr_part_ != NULL)//パーツ
    {
      res.Bounds(0, 0, pgr_part_->GetPartW(), pgr_part_->GetPartH());
    }
    else//通常（パーツ番号無視）
    {
      res.Bounds(0, 0, pgr_->GetWidth(), pgr_->GetHeight());
    }
    return res;
  }
  void TUIObj::g_SetOvOvj(TUIObj* ovobj)
  {
    stSO_.stGrp_.ovobj_ = ovobj;
  }
  void TUIObj::g_SetDepth(enUIDepth depth)
  {
    stSO_.stGrp_.basedepth_ = depth;//基礎値
  }
  void TUIObj::g_SetRevDepthLv(s32 revdepthLv)
  {
    stSO_.stGrp_.revdepthLv_ = revdepthLv;//補正値Lv(倍率を後ほど掛ける)
  }
  void TUIObj::g_SetRevDepthLv2(s32 revdepthLv2)
  {
    stSO_.stGrp_.revdepthLv2_ = revdepthLv2;//補正値Lv2(倍率をかけない直指定)
  }
  void TUIObj::g_SetCenter(BOOL center_f)
  {
    stSO_.stGrp_.centerOrigin_f_ = center_f;
  }
  void TUIObj::g_SetUnder()
  {
    stSO_.stGrp_.under_f_ = TRUE;
  }
  void TUIObj::g_SetRightShift()
  {
    stSO_.stGrp_.rightShift_f_ = TRUE;
  }
  void TUIObj::g_SetMirror(BOOL mirror_f)
  {
    stSO_.stGrp_.mirror_f_ = mirror_f;
  }
  void TUIObj::g_SetLimX(s32 liml, s32 www)
  {
    stSO_.stGrp_.limL_ = liml;
    stSO_.stGrp_.limR_ = liml + www;
  }
  void TUIObj::g_SetLimY(s32 limu, s32 hhh)
  {
    stSO_.stGrp_.limU_ = limu;
    stSO_.stGrp_.limD_ = limu + hhh;
  }
  void TUIObj::g_SetDrawmode(mid::enDrawMode drawmode)
  {
    ////描画モード
    //enum enDrawMode
    //{
    //  drm_Nml,
    //  drm_Add,
    //  drm_ColFil,
    //  drm_Neg,
    //  drm_Def,
    //};
    stSO_.stGrp_.drawmode_ = drawmode;
  }
  void TUIObj::g_SetARGB(BOOL def_f, s32 aaa, s32 rrr, s32 ggg, s32 bbb)
  {
    stSO_.stGrp_.argb_.aaa_ = aaa;
    stSO_.stGrp_.argb_.rrr_ = rrr;
    stSO_.stGrp_.argb_.ggg_ = ggg;
    stSO_.stGrp_.argb_.bbb_ = bbb;
    if (def_f) stSO_.stGrp_.defargb_ = stSO_.stGrp_.argb_;
  }
  void TUIObj::g_SetAlp(BOOL def_f, s32 aaa)
  {
    stSO_.stGrp_.argb_.aaa_ = aaa;
    if (def_f) stSO_.stGrp_.defargb_ = stSO_.stGrp_.argb_;
  }
  void TUIObj::g_SetRGB(BOOL def_f, s32 rrr, s32 ggg, s32 bbb)
  {
    stSO_.stGrp_.argb_.rrr_ = rrr;
    stSO_.stGrp_.argb_.ggg_ = ggg;
    stSO_.stGrp_.argb_.bbb_ = bbb;
    if (def_f) stSO_.stGrp_.defargb_ = stSO_.stGrp_.argb_;
  }
  void TUIObj::g_ResetDefCol()
  {
    stSO_.stGrp_.argb_ = stSO_.stGrp_.defargb_;
  }
  void TUIObj::g_SetCharLayer(s32 chary)
  {
    //キャラ系レイヤ
    stSO_.stGrp_.charLayer_f_ = TRUE;
    stSO_.stGrp_.charLayer_ = (UIDP_03OBJ + (chary * OVERDEPTH));
  }
  void TUIObj::g_SetCharHaveDepth(s32 depth)
  {
    //キャラの所持による直接深度指定
    stSO_.stGrp_.charLayer_f_ = TRUE;
    stSO_.stGrp_.charLayer_ = depth;
  }
  void TUIObj::g_EfReset()
  {
    stSO_.stGrp_.drawmode_ = mid::drm_Nml;
    stSO_.stGrp_.argb_ = stSO_.stGrp_.defargb_;
    stSO_.stGrp_.argb2_.SetDEFCOL();
    stSO_.stGrp_.coleat_ = 0;
    stSO_.stGrp_.effect_ = ef_None;
    stSO_.stGrp_.alpspd_ = 0;
  }
  void TUIObj::g_Flash()
  {
    if (stSO_.stGrp_.effect_ == ef_Flash) return;
    g_EfReset();
    stSO_.stGrp_.drawmode_ = mid::drm_ColFil;
    stSO_.stGrp_.coleat_ = 6;
    stSO_.stGrp_.colspd_ = (s32)(0xFF / stSO_.stGrp_.coleat_);
    stSO_.stGrp_.effect_ = ef_Flash;
  }
  void TUIObj::g_Flash2(u8 rrr, u8 ggg, u8 bbb)
  {
    if (stSO_.stGrp_.effect_ == ef_Flash2) return;
    g_EfReset();
    stSO_.stGrp_.drawmode_ = mid::drm_ColFil;
    stSO_.stGrp_.coleat_ = 6;
    stSO_.stGrp_.colspd_ = (s32)(0xFF / stSO_.stGrp_.coleat_);
    stSO_.stGrp_.argb2_.SetCOL(0xFF, rrr, ggg, bbb);
    stSO_.stGrp_.effect_ = ef_Flash2;
  }
  void TUIObj::g_Bright()
  {
    if (stSO_.stGrp_.effect_ == ef_Brite) return;
    g_EfReset();
    stSO_.stGrp_.drawmode_ = mid::drm_ColFil;
    stSO_.stGrp_.argb2_.aaa_ = 0x50;
    stSO_.stGrp_.effect_ = ef_Brite;
  }
  void TUIObj::g_Dark()
  {
    if (stSO_.stGrp_.effect_ == ef_Dark) return;
    g_EfReset();
    stSO_.stGrp_.drawmode_ = mid::drm_Nml;
    stSO_.stGrp_.coleat_ = 6;
    stSO_.stGrp_.colspd_ = (s32)(0x80 / stSO_.stGrp_.coleat_);
    stSO_.stGrp_.effect_ = ef_Dark;
  }
  void TUIObj::g_Fade(BOOL out_f)
  {
    g_Fade_Ex(out_f, 20);
  }
  void TUIObj::g_Fade_Ex(BOOL out_f, s32 eat)
  {
    stSO_.stGrp_.alpeat_ = eat;
    stSO_.stGrp_.alpspd_ = (stSO_.stGrp_.filter_f_)
      ? (s32)(stSO_.stGrp_.filterargb_.aaa_ / stSO_.stGrp_.alpeat_)//フィルタは初期設定α値を基本α値とする
      : (s32)(MAXALP / stSO_.stGrp_.alpeat_);
    stSO_.stGrp_.draw_f_ = TRUE;
    if (out_f)
    {
      stSO_.stGrp_.alpspd_ = -stSO_.stGrp_.alpspd_;
    }
    else
    {
      stSO_.stGrp_.argb_.aaa_ = 0;
    }
  }
  void TUIObj::g_GetOverDepth(enUIDepth basedepth, s32* overdepth, TUIObj* ovobj)
  {
    //上乗りオブジェがある
    if (ovobj != NULL)
    {
      *overdepth += OVERDEPTHOBJ;//ovobjが居る度1ずつ上がっていく
      ovobj->g_GetOverDepth(ovobj->stSO_.stGrp_.basedepth_, overdepth, ovobj->stSO_.stGrp_.ovobj_);
    }
    else
    {
      //同じ深度ならプラスする
      if (basedepth == stSO_.stGrp_.basedepth_)
      {
        if (basedepth == UIDP_03OBJ)
        {
          *overdepth += OVERDEPTHOBJ;//親が居る度1ずつ上がっていく
        }
        else
        {
          *overdepth += OVERDEPTH + (stSO_.stGrp_.revdepthLv_ * REVDEPTHMAG) + stSO_.stGrp_.revdepthLv2_;//親が居る度1000ずつ上がっていく
        }

      }

      //親がある場合は親の深度もプラスする
      if (parent_ != NULL) parent_->g_GetOverDepth(basedepth, overdepth, ovobj);
    }
  }
  void TUIObj::g_ColMoving()
  {
    switch (stSO_.stGrp_.effect_)
    {
    case ef_Flash:
    case ef_Flash2:
      if (stSO_.stGrp_.coleat_ > 0)
      {
        if (lib_num::AprTo0(&stSO_.stGrp_.coleat_))
        {
          g_EfReset();
        }
        else
        {
          //          lib_num::AprTo0((s32*)&stSO_.stGrp_.argb2_.aaa_, stSO_.stGrp_.colspd_);
          lib_num::AprTo0((s32*)&stSO_.stGrp_.argb2_.aaa_, stSO_.stGrp_.colspd_);
        }
      }
      break;
    case ef_Brite:

      break;
    case ef_Dark:
      if (stSO_.stGrp_.coleat_ > 0)
      {
        if (lib_num::AprTo0(&stSO_.stGrp_.coleat_))
        {
          g_SetRGB(FALSE, 0x80, 0x80, 0x80);
        }
        else
        {
          stSO_.stGrp_.argb_.rrr_ -= stSO_.stGrp_.colspd_;
          stSO_.stGrp_.argb_.ggg_ -= stSO_.stGrp_.colspd_;
          stSO_.stGrp_.argb_.bbb_ -= stSO_.stGrp_.colspd_;
        }
      }
      break;
    }
  }
  void TUIObj::g_AlpMoving()
  {
    if (stSO_.stGrp_.alpspd_ == 0) return;

    s32 alp = (stSO_.stGrp_.argb_.aaa_ + stSO_.stGrp_.alpspd_);


    s32 endalp = (stSO_.stGrp_.filter_f_)
      ? stSO_.stGrp_.filterargb_.aaa_
      : MAXALP;

    if (alp >= endalp)
    {
      stSO_.stGrp_.argb_.aaa_ = endalp;
      stSO_.stGrp_.alpspd_ = 0;
    }
    else if (alp <= 0x00)
    {
      stSO_.stGrp_.argb_.aaa_ = 0;
      stSO_.stGrp_.alpspd_ = 0;
      stSO_.stGrp_.draw_f_ = FALSE;
    }
    else
    {
      stSO_.stGrp_.argb_.aaa_ = alp;
    }

  }
  void TUIObj::g_ResetBaseDepth(enUIDepth* basedepth, TUIObj* ovobj)
  {
    //上乗りオブジェがある
    if (ovobj != NULL)
    {
      if (*basedepth < ovobj->stSO_.stGrp_.basedepth_)
      {
        *basedepth = ovobj->stSO_.stGrp_.basedepth_;
      }
      ovobj->g_ResetBaseDepth(basedepth, ovobj->stSO_.stGrp_.ovobj_);
    }
    else if (parent_ != NULL)//親がある
    {
      if (*basedepth < parent_->stSO_.stGrp_.basedepth_)
      {
        *basedepth = parent_->stSO_.stGrp_.basedepth_;
      }
      parent_->g_ResetBaseDepth(basedepth, parent_->stSO_.stGrp_.ovobj_);
    }
  }
  //フォント処理****************************************************
  void TUIObj::f_MakeFont(const char* msg, s32 fontcol)
  {
    f_MakeFont_Ex(msg, base::fd_None, fontcol, fontcol, 0);
    //grfont_にはすでに渡されてるので大丈夫なはず
  }
  void TUIObj::f_MakeFont_Ht(const char* msg, s32 fontcol)
  {
    std::string h_msg_h = "心＿";
    h_msg_h += msg;
    h_msg_h += "＿心";
    f_MakeFont_Ex(h_msg_h.c_str(), base::fd_None, fontcol, fontcol, 0);
  }
  void TUIObj::f_MakeFont_Len(const char* msg, s32 fontcol, s32 maxlen)
  {
    f_MakeFont_Ex(msg, base::fd_None, fontcol, fontcol, maxlen);
    //grfont_にはすでに渡されてるので大丈夫なはず
  }
  void TUIObj::f_MakeFont_Colnum(const char* msg, s32 fontcol, s32 numcol)
  {
    f_MakeFont_Ex(msg, base::fd_ColNum, fontcol, numcol, 0);
  }
  //void TUIObj::f_MakeFont_Colsp(const char* msg, s32 fx, s32 fy, s32 fontcol, s32 spcol)
  //{
  //  f_MakeFont_Ex(msg, fx, fy, base::fd_ColSP, fontcol, spcol);
  //}
  void TUIObj::f_MakeFont_Ex(const char* msg, u32 draw_fs, s32 fontcol, s32 numcol, s32 maxlen)
  {
    bmofont_.DeleteTex();
    bmofont_.Init();
    stSO_.stFont_.fontlen_ = pmgEO_->mgFont_.MakeUIFontTexture_Ex(&bmofont_, msg, draw_fs, (base::enFontCol2)fontcol, (base::enFontCol2)numcol, maxlen);
    stSO_.stFont_.drawlen_ = stSO_.stFont_.fontlen_;
    stSO_.stFont_.set_f_ = TRUE;
    stSO_.stFont_.draw_f_ = TRUE;
    stSO_.stFont_.mini_f_ = ((draw_fs & base::fd_Mini) != 0);
    //stSO_.stGrp_.draw_f_ = TRUE;//描画フラグは立てなくてよい気がする
  }
  void TUIObj::f_MakeFont_Mini(const char* msg, s32 fontcol)
  {
    f_MakeFont_Ex(msg, base::fd_Mini | base::fd_Pro, fontcol, fontcol, 0);
  }
  void TUIObj::f_MakeFontMC(s32 mojicode, s32 fontcol)
  {
    f_MakeFontMC_Ex(mojicode, base::fd_None, fontcol);
  }
  void TUIObj::f_MakeFontMC_Ex(s32 mojicode, u32 draw_fs, s32 fontcol)
  {
    bmofont_.DeleteTex();
    bmofont_.Init();
    stSO_.stFont_.fontlen_ = pmgEO_->mgFont_.MakeUIFontTextureMC(&bmofont_, mojicode, draw_fs, (base::enFontCol2)fontcol);
    stSO_.stFont_.drawlen_ = stSO_.stFont_.fontlen_;
    stSO_.stFont_.set_f_ = TRUE;
    stSO_.stFont_.draw_f_ = TRUE;
    stSO_.stFont_.mini_f_ = ((draw_fs & base::fd_Mini) != 0);
    stSO_.stGrp_.draw_f_ = TRUE;

    //f_FontPos(fx, fy);
  }

  void TUIObj::f_SetFontDraw(BOOL draw_f)
  {
    stSO_.stFont_.draw_f_ = draw_f;
  }
  void TUIObj::f_SetFontDrawLen(s32 len)
  {

    if ((len > stSO_.stFont_.fontlen_) || (len <= 0))
    {
      stSO_.stFont_.drawlen_ = stSO_.stFont_.fontlen_;
    }
    else
    {
      stSO_.stFont_.drawlen_ = len;
    }
  }
  void TUIObj::f_FontPos(s32 fx, s32 fy)
  {
    stSO_.stFont_.fx_ = fx;
    stSO_.stFont_.fy_ = fy;
  }
  void TUIObj::f_FontPosCenter(s32 fx, s32 fy)
  {
    TRect br = b_GetAutoSize();
    stSO_.stFont_.fx_ = (br.width_ / 2) + fx;
    stSO_.stFont_.fy_ = (br.height_ / 2) + fy;
  }
  void TUIObj::f_SetCenter(BOOL center_f)
  {
    stSO_.stFont_.centerOrigin_f_ = center_f;
  }
  void TUIObj::f_SetRightShift()
  {
    stSO_.stFont_.rightShift_f_ = TRUE;
  }


  //キャラ処理****************************************************
  //void TUIObj::c_SetDraw(BOOL draw_f)
  //{
  //  stSO_.stChar_.draw_f_ = draw_f;
  //}
  void TUIObj::c_SetCellNo(s32 cellno)
  {
    stSO_.stChar_.cellno_ = cellno;
  }
  void TUIObj::c_SetMirror(s32 mirror_f)
  {
    stSO_.stChar_.mirror_f_ = mirror_f;
  }
  void TUIObj::c_SetBaseLine(s32 liney)
  {
    stSO_.stChar_.liney_ = liney;
  }
  void TUIObj::c_SetPos(s32 x, s32 y)//, s32 z)
  {
    stSO_.stChar_.cx_ = x;
    stSO_.stChar_.cy_ = y;
  }
  void TUIObj::c_SetBMPos(s32 bx, s32 by)
  {
    stSO_.stChar_.bx_ = bx;
    stSO_.stChar_.by_ = by;
  }
  void TUIObj::c_SetBmpOffsetNo(s32 offset)
  {
    stSO_.stChar_.bmpoffsetno_ = offset;
  }
  s32 TUIObj::c_GetDepth()
  {
    return GetDrawDepth();
  }
  TDtCell* TUIObj::c_GetDtCell()
  {
    return pmgchar_->ccdt_;
  }

  //アニメ処理****************************************************
  void TUIObj::a_AddAnivec(s32 stno, s32 edno, s32 itv, enAnitype anitype)
  {

    for (s32 i = 0; i < TUIObjSt::ANILEN; ++i)
    {
      if (stSO_.stAni_[i].set_f_ == FALSE)
      {
        stSO_.stAni_[i].set_f_ = TRUE;
        for (s32 i2 = 0; i2 < (edno - stno + 1); ++i2)
        {
          stSO_.stAni_[i].anivec_.push_back(stno + i2);
        }
        stSO_.stAni_[i].itv_ = itv;
        stSO_.stAni_[i].anitype_ = anitype;
        break;
      }
    }
  }
  void TUIObj::a_AddAnivec(INTVECTOR* vec, s32 itv, enAnitype anitype)
  {
    for (s32 i = 0; i < TUIObjSt::ANILEN; ++i)
    {
      if (stSO_.stAni_[i].set_f_) continue;
      stSO_.stAni_[i].set_f_ = TRUE;
      for (s32 i2 = 0; i2 < vec->size(); ++i2)
      {
        stSO_.stAni_[i].anivec_.push_back(vec->at(i2));
      }
      stSO_.stAni_[i].itv_ = itv;
      stSO_.stAni_[i].anitype_ = anitype;
      break;
    }
  }
  void TUIObj::a_StopAni()
  {
    stSO_.stGrp_.ani_f_ = FALSE;
  }
  void TUIObj::a_SetAniNo(s32 anino, BOOL restart_f)
  {
    if (restart_f || (stSO_.stGrp_.ani_f_ == FALSE) || (stSO_.stGrp_.anino_ != anino))
    {
      stSO_.stGrp_.ani_f_ = TRUE;
      stSO_.stGrp_.anino_ = anino;
      stSO_.stAni_[anino].ani_c_ = 0;
      stSO_.stAni_[anino].komano_ = 0;
    }
  }
  void TUIObj::a_Animation()
  {
    if (stSO_.stGrp_.ani_f_ == FALSE) return;
    TUIObjAniSt* anist = &stSO_.stAni_[stSO_.stGrp_.anino_];

    if (lib_num::UpToR(&anist->ani_c_, anist->itv_))
    {
      anist->ani_c_ = 0;
      //終点
      if (lib_num::UpToR(&anist->komano_, anist->anivec_.size()))
      {
        switch (anist->anitype_)
        {
        case at_Normal:
          anist->komano_ = (anist->anivec_.size() - 1);
          break;
        case at_Loop:
          anist->komano_ = 0;//再スタート
          break;
        case  at_Door:
          anist->komano_ = 0;//再スタート
          stSO_.stGrp_.ani_f_ = FALSE;
          break;
        case at_Effect:
          stSO_.stGrp_.ani_f_ = FALSE;
          g_SetDraw(FALSE);
          break;
        }
      }
    }
  }
  //ボタン処理****************************************************
  void TUIObj::b_SetBtn(BOOL btn_f)//, BOOL xor_f)
  {
    stSO_.stBtn_.btn_f_ = btn_f;
    //ボタンを無理矢理切ったときはタッチしてない状態に戻す
    if (btn_f == FALSE)
    {
      stSO_.stBtn_.touch_ = ts_None;
      //g_EfReset();
    }
  }
  void TUIObj::b_SetModal(BOOL modal_f)
  {
    stSO_.stBtn_.modal_f_ = modal_f;
  }
  void TUIObj::b_SetSlideOK(BOOL slideok_f)
  {
    stSO_.stBtn_.slideok_f_ = slideok_f;
  }
  void TUIObj::b_BtnLimX(s32 liml, s32 www)
  {
    stSO_.stBtn_.limL_ = liml;
    stSO_.stBtn_.limR_ = liml + www;
  }
  void TUIObj::b_BtnLimY(s32 limu, s32 hhh)
  {
    stSO_.stBtn_.limU_ = limu;
    stSO_.stBtn_.limD_ = limu + hhh;
  }
  void TUIObj::b_AddSize(s32 adw, s32 adh)
  {
    stSO_.stBtn_.adw_ = adw;
    stSO_.stBtn_.adh_ = adh;
  }
  void TUIObj::b_Enter()//指が載った
  {
    stSO_.stBtn_.touch_ = ts_Enter;
    //g_Bright();
  }
  void TUIObj::b_Enter2()//指が載っている
  {
    stSO_.stBtn_.touch_ = ts_Enter2;
    //g_Bright();
  }
  void TUIObj::b_Cancel()//タッチしたまま離れる（キャンセル）
  {
    stSO_.stBtn_.touch_ = ts_Cancel;
    //g_EfReset();

  }
  void TUIObj::b_Decide()//その場で離れる（ＯＫ）
  {
    stSO_.stBtn_.touch_ = ts_Decide;
    //g_Flash();
  }
  BOOL TUIObj::b_IsDecide()
  {
    return (stSO_.stBtn_.touch_ == ts_Decide);
  }
  BOOL TUIObj::b_IsEnter(BOOL enter2_f)
  {
    if (enter2_f)
    {
      return ((stSO_.stBtn_.touch_ == ts_Enter) || (stSO_.stBtn_.touch_ == ts_Enter2));
    }
    else
    {
      return (stSO_.stBtn_.touch_ == ts_Enter);
    }
  }
  TRect TUIObj::b_GetAutoSize()
  {
    TRect res = g_GetSize();

    res.width_ += stSO_.stBtn_.adw_;
    res.height_ += stSO_.stBtn_.adh_;

    if (stSO_.stGrp_.centerOrigin_f_)
    {
      res.x_ = -(res.width_ / 2);
      res.y_ = -(res.height_ / 2);
    }
    if (stSO_.stGrp_.under_f_)
    {
      res.y_ = -res.height_;
    }

    return res;
  }
  BOOL TUIObj::b_IsBaseSwiping()
  {
    //先祖辿ってスワイプあるかチェック
    if (parent_ == NULL)
    {
      return FALSE;
    }
    else
    {
      //スワイプ処理が発生していたら
      if (parent_->stSO_.stCamSw_.swp_f_ || parent_->stSO_.stLineSw_.swp_f_)
      {
        return TRUE;
      }
      else
      {
        return parent_->b_IsBaseSwiping();
      }
    }
  }
  BOOL TUIObj::b_IsPile()
  {
    BOOL res = FALSE;
    ////子にボタンが居て押されていたら親は押されない
    //for (s32 i = 0; i < childlen_.size(); ++i)
    //{
    //  if (childlen_[i]->b_IsPile()) return res;
    //}


    if (stSO_.stBtn_.btn_f_)
    {
      mid::TStScreen* nowstsc = &(pmgEO_->mgInput_.nowstsc_);
      TRect br = b_GetAutoSize();

      br.width_ -= 1;
      br.height_ -= 1;
      //センタリングの時とそうでないときのタッチ判定の違いを確認する★

      f32 posX = 0.0;
      f32 posY = 0.0;
      z_GetGlobalPos(&posX, &posY);
      br.x_ += posX;
      br.y_ += posY;
      //ボタン内が押された
      BOOL pile_f = (br.IsPile(nowstsc->dotx_, nowstsc->doty_));

      if (pile_f)
      {
        if ((stSO_.stBtn_.limL_ != 0) || (stSO_.stBtn_.limR_ != 0))
        {
          if ((nowstsc->dotx_ < stSO_.stBtn_.limL_)
            || (nowstsc->dotx_ > stSO_.stBtn_.limR_))
          {
            pile_f = FALSE;
          }
        }

        if ((stSO_.stBtn_.limU_ != 0) || (stSO_.stBtn_.limD_ != 0))
        {
          if ((nowstsc->doty_ < stSO_.stBtn_.limU_)
            || (nowstsc->doty_ > stSO_.stBtn_.limD_))
          {
            pile_f = FALSE;
          }
        }
      }

      res = pile_f;
    }

    return res;
  }
  BOOL TUIObj::b_IsBtn()
  {
    return stSO_.stBtn_.btn_f_;
  }
  void TUIObj::b_Scanning()
  {
    //押されチェック
    BOOL pile_f = FALSE;

    //すでにアクション発生済み
    if (pmgUI_->IsAction())
    {
      pile_f = FALSE;
    }
    else if (pmgUI_->IsModal() == FALSE)//モーダル状態以外もしくはモーダル無視オブジェ
    {
      pile_f = (b_IsPile() && IsDraw());
    }
    else
    {
      pile_f = (b_IsPile() && IsDraw() && stSO_.stBtn_.modal_f_);
    }

    mid::TStScreen* nowstsc = &(pmgEO_->mgInput_.nowstsc_);

    if ((nowstsc->touchtype_ == mid::tct_Enter)
      //|| (nowstsc->touchtype_ == mid::tct_Enter2)
      || (nowstsc->touchtype_ == mid::tct_Exit))
    {
      if (pile_f && stSO_.stBtn_.btn_f_) pmgUI_->SetAction(); //アクション発生
    }

    //スライド入力受付
    if (stSO_.stBtn_.slideok_f_)
    {
      switch (stSO_.stBtn_.touch_)
      {
      case ts_None:
        if (pile_f)
        {
          if ((nowstsc->touchtype_ == mid::tct_Enter) || (nowstsc->touchtype_ == mid::tct_Enter2))
          {
            b_Enter2();//押された瞬間
          }
        }
        break;
      case ts_Enter:
      case ts_Enter2:
        if ((nowstsc->touchtype_ == mid::tct_Exit) || (pile_f == FALSE))
        {
          b_Cancel();//キャンセル
        }
        break;
      default:
        stSO_.stBtn_.touch_ = ts_None;//Noneにもどる
        break;
      }
    }
    else//通常タッチ入力受付
    {
      switch (stSO_.stBtn_.touch_)
      {
      case ts_None:
        if (pile_f)
        {
          if (nowstsc->touchtype_ == mid::tct_Enter) b_Enter();//押された瞬間
        }
        break;
      case ts_Enter:
      case ts_Enter2:
        //親スワイプ
        if (b_IsBaseSwiping())
        {
          b_Cancel();//キャンセル
        }
        else if (pile_f)
        {
          if (nowstsc->touchtype_ == mid::tct_Exit)
          {
            b_Decide();//決定
          }
          else
          {
            b_Enter2();//押され継続に変化
          }
        }
        else
        {
          b_Cancel();//キャンセル
        }
        break;
      case ts_Cancel:
        //離れたら
        if (nowstsc->touchtype_ == mid::tct_None)
        {
          stSO_.stBtn_.touch_ = ts_None;//Noneにもどる
        }
        break;
      case ts_Decide:
        stSO_.stBtn_.touch_ = ts_None;//Noneにもどる
        break;
      }
    }
  }

  //カメラスワイプ処理****************************************************
  void TUIObj::sc_CamSwipe(BOOL swipe_f)
  {
    stSO_.stCamSw_.camswipe_f_ = swipe_f;
    mid::TStScreen* nowstsc = &(pmgEO_->mgInput_.nowstsc_);
    nowstsc->Restart();
  }
  void TUIObj::sc_CamSwipeLimX(s32 liml, s32 www)
  {
    stSO_.stCamSw_.limL_ = liml;
    stSO_.stCamSw_.limR_ = liml + www;
  }
  void TUIObj::sc_CamSwipeLimY(s32 limu, s32 hhh)
  {
    stSO_.stCamSw_.limU_ = limu;
    stSO_.stCamSw_.limD_ = limu + hhh;
  }
  void TUIObj::sc_CamSwipeData(s32 left, s32 top, s32 right, s32 bottom)
  {
    stSO_.stCamSw_.left_ = left;
    stSO_.stCamSw_.top_ = top;
    stSO_.stCamSw_.right_ = right;
    stSO_.stCamSw_.bottom_ = bottom;
    stSO_.stCamSw_.swlim_f_ = TRUE;
  }
  void TUIObj::sc_CamSetPos(s32 posx, s32 posy)
  {
    stSO_.stCamSw_.posX_ = posx;//直接代入限界無し
    stSO_.stCamSw_.posY_ = posy;//直接代入
    stSO_.stCamSw_.posXBegin_ = posx;//初期位置更新
    stSO_.stCamSw_.posYBegin_ = posy;
  }
  void TUIObj::sc_CamSetLimit(BOOL lim_f)//一時的にカメラ限界外す
  {
    stSO_.stCamSw_.swlim_f_ = lim_f;
  }
  //カメラ位置の移動
  void TUIObj::sc_CamGenSwipePos(s32 posx, s32 posy)
  {
    if (stSO_.stCamSw_.swlim_f_ == FALSE)
    {
      stSO_.stCamSw_.posX_ = posx;//直接代入限界無し
      stSO_.stCamSw_.posY_ = posy;//直接代入
    }
    else
    {
      stSO_.stCamSw_.posX_ = lib_num::Between(posx, stSO_.stCamSw_.left_, stSO_.stCamSw_.right_);
      stSO_.stCamSw_.posY_ = lib_num::Between(posy, stSO_.stCamSw_.top_, stSO_.stCamSw_.bottom_);
    }
  }
  void TUIObj::sc_CamSwipePos(s32 swpx, s32 swpy, BOOL kansei_f)
  {
    if (kansei_f)//慣性
    {
      sc_CamGenSwipePos((stSO_.stCamSw_.posX_ + swpx), (stSO_.stCamSw_.posY_ + swpy));
      stSO_.stCamSw_.posXBegin_ = stSO_.stCamSw_.posX_;//初期位置更新
      stSO_.stCamSw_.posYBegin_ = stSO_.stCamSw_.posY_;
    }
    else//（指位置）
    {
      sc_CamGenSwipePos((stSO_.stCamSw_.posXBegin_ + swpx), (stSO_.stCamSw_.posYBegin_ + swpy));
    }
  }
  BOOL TUIObj::sc_IsCamSwipe(s32 dist)//閾値
  {
    mid::TStScreen* nowstsc = &(pmgEO_->mgInput_.nowstsc_);

    s32 mvdist = (s32)lib_num::Hypot((nowstsc->swpbx_ - nowstsc->swpx_), (nowstsc->swpby_ - nowstsc->swpy_));
    return (mvdist >= dist);
    //return (nowstsc->swp_f_);
  }

  BOOL TUIObj::sc_IsCamEnter()
  {
    mid::TStScreen* nowstsc = &(pmgEO_->mgInput_.nowstsc_);
    return (nowstsc->touchtype_ == mid::tct_Enter);
  }
  BOOL TUIObj::sc_IsCamEnter2()
  {
    mid::TStScreen* nowstsc = &(pmgEO_->mgInput_.nowstsc_);
    return (nowstsc->touchtype_ == mid::tct_Enter2);
  }
  void TUIObj::sc_CamSwiping()
  {
    stSO_.stCamSw_.swp_f_ = FALSE;

    //見えないときは聞かない
    if (IsDraw() == FALSE) return;

    if (stSO_.stCamSw_.camswipe_f_ == FALSE) return;

    //とりあえず全画面スワイプ受付

    mid::TStScreen* nowstsc = &(pmgEO_->mgInput_.nowstsc_);
    //範囲外
    if ((stSO_.stCamSw_.limL_ != 0) || (stSO_.stCamSw_.limR_ != 0))
    {
      if ((nowstsc->dotx_ < stSO_.stCamSw_.limL_)
        || (nowstsc->dotx_ > stSO_.stCamSw_.limR_))
      {
        return;
      }
    }

    if ((stSO_.stCamSw_.limU_ != 0) || (stSO_.stCamSw_.limD_ != 0))
    {
      if ((nowstsc->doty_ < stSO_.stCamSw_.limU_)
        || (nowstsc->doty_ > stSO_.stCamSw_.limD_))
      {
        return;
      }
    }



    if (nowstsc->swp_f_)
    {
      s32 swpx = (s32)((nowstsc->swpbx_ - nowstsc->swpx_) / mid::midGetMag());
      s32 swpy = (s32)((nowstsc->swpby_ - nowstsc->swpy_) / mid::midGetMag());
      sc_CamSwipePos(swpx, swpy, FALSE);

      if ((swpx != 0) || (swpy != 0)) stSO_.stCamSw_.swp_f_ = TRUE;
    }
    else
    {
      sc_CamSwipePos(nowstsc->swpdx_, nowstsc->swpdy_, TRUE);

      if ((nowstsc->swpdx_ != 0) || (nowstsc->swpdy_ != 0)) stSO_.stCamSw_.swp_f_ = TRUE;
    }
  }

  //ラインスワイプ処理****************************************************
  void TUIObj::sl_LineSwipeX(BOOL swipe_f)
  {
    stSO_.stLineSw_.linex_f_ = swipe_f;
  }
  void TUIObj::sl_LineSwipeY(BOOL swipe_f)
  {
    stSO_.stLineSw_.liney_f_ = swipe_f;
  }
  void TUIObj::sl_LineSwipeLimX(s32 liml, s32 www)
  {
    stSO_.stLineSw_.limL_ = liml;
    stSO_.stLineSw_.limR_ = liml + www;
  }
  void TUIObj::sl_LineSwipeLimY(s32 limu, s32 hhh)
  {
    stSO_.stLineSw_.limU_ = limu;
    stSO_.stLineSw_.limD_ = limu + hhh;
  }
  void TUIObj::sl_LineSwipeWidth(s32 www)
  {
    stSO_.stLineSw_.width_ = www;
  }
  void TUIObj::sl_LineSwipeHeight(s32 minY, s32 maxY)
  {
    stSO_.stLineSw_.minY_ = minY;
    stSO_.stLineSw_.maxY_ = maxY;
    stSO_.stLineSw_.posY_ = 0;
    stSO_.stLineSw_.posYBegin_ = 0;
  }
  void TUIObj::sl_LineSetPos(s32 posx, s32 posy)
  {
    stSO_.stLineSw_.posX_ = posx;//直接代入限界無し
    stSO_.stLineSw_.posY_ = posy;//直接代入
    stSO_.stLineSw_.posXBegin_ = posx;//初期位置更新
    stSO_.stLineSw_.posYBegin_ = posy;
  }
  void TUIObj::sl_LineGenSwipePosX(s32 posx)
  {
    if (stSO_.stLineSw_.width_ == 0)
    {
      stSO_.stLineSw_.posX_ = posx;
    }
    else
    {
      stSO_.stLineSw_.posX_ = lib_num::Between(posx, 0, stSO_.stLineSw_.width_);
    }

  }
  void TUIObj::sl_LineGenSwipePosY(s32 posy)
  {
    if ((stSO_.stLineSw_.minY_ == 0) && (stSO_.stLineSw_.maxY_ == 0))
    {
      stSO_.stLineSw_.posY_ = posy;
    }
    else
    {
      stSO_.stLineSw_.posY_ = lib_num::Between(posy, stSO_.stLineSw_.minY_, stSO_.stLineSw_.maxY_);
    }

  }
  void TUIObj::sl_LineSwipePosX(s32 swpx, BOOL kansei_f)
  {
    if (kansei_f)//慣性
    {
      sl_LineGenSwipePosX(stSO_.stLineSw_.posX_ + swpx);
      stSO_.stLineSw_.posXBegin_ = stSO_.stLineSw_.posX_;//初期位置更新
    }
    else//（指位置）
    {
      sl_LineGenSwipePosX((stSO_.stLineSw_.posXBegin_ + swpx));
    }
  }
  void TUIObj::sl_LineSwipePosY(s32 swpy, BOOL kansei_f)
  {
    if (kansei_f)//慣性
    {
      sl_LineGenSwipePosY(stSO_.stLineSw_.posY_ + swpy);
      stSO_.stLineSw_.posYBegin_ = stSO_.stLineSw_.posY_;
    }
    else//（指位置）
    {
      sl_LineGenSwipePosY(stSO_.stLineSw_.posYBegin_ + swpy);
    }
  }
  void TUIObj::sl_StopLineSwipe(BOOL stop_f)
  {
    stSO_.stLineSw_.swipestop_f_ = stop_f;
  }

  void TUIObj::sl_LineSwiping()
  {


    stSO_.stLineSw_.swp_f_ = FALSE;
    stSO_.stLineSw_.genswp_f_ = FALSE;

    //見えないときは聞かない
    if (IsDraw() == FALSE) return;

    //一時停止中
    if (stSO_.stLineSw_.swipestop_f_) return;

    ////一時的にスワイプ停止
    //if (stSO_.stLineSw_.swipestop_f_) return;

    //とりあえず全画面スワイプ受付
    mid::TStScreen* nowstsc = &(pmgEO_->mgInput_.nowstsc_);

    //範囲外
    if ((stSO_.stLineSw_.limL_ != 0) || (stSO_.stLineSw_.limR_ != 0))
    {
      if ((nowstsc->dotx_ < stSO_.stLineSw_.limL_)
        || (nowstsc->dotx_ > stSO_.stLineSw_.limR_))
      {
        return;
      }
    }

    if ((stSO_.stLineSw_.limU_ != 0) || (stSO_.stLineSw_.limD_ != 0))
    {
      if ((nowstsc->doty_ < stSO_.stLineSw_.limU_)
        || (nowstsc->doty_ > stSO_.stLineSw_.limD_))
      {
        return;
      }
    }

    //スワイプ自体はしているか
    stSO_.stLineSw_.genswp_f_ = nowstsc->swp_f_;

    if (stSO_.stLineSw_.linex_f_)
    {
      if (nowstsc->swp_f_)
      {
        s32 swpx = (s32)((nowstsc->swpbx_ - nowstsc->swpx_) / mid::midGetMag());
        sl_LineSwipePosX(swpx, FALSE);

        if (swpx != 0) stSO_.stLineSw_.swp_f_ = TRUE;
      }
      else//慣性
      {
        sl_LineSwipePosX(nowstsc->swpdx_, TRUE);
        if (nowstsc->swpdx_ != 0) stSO_.stLineSw_.swp_f_ = TRUE;
      }
    }

    if (stSO_.stLineSw_.liney_f_)
    {
      if (nowstsc->swp_f_)
      {
        s32 swpy = (s32)((nowstsc->swpby_ - nowstsc->swpy_) / mid::midGetMag());
        sl_LineSwipePosY(swpy, FALSE);

        if (swpy != 0)
        {
          stSO_.stLineSw_.swp_f_ = TRUE;
        }
      }
      else//慣性
      {
        sl_LineSwipePosY(nowstsc->swpdy_, TRUE);
        if (nowstsc->swpdy_ != 0)
        {
          stSO_.stLineSw_.swp_f_ = TRUE;
        }
      }
    }
  }
  s32 TUIObj::sl_LineGetX()
  {
    return stSO_.stLineSw_.posX_;
  }
  s32 TUIObj::sl_LineGetY()
  {
    return stSO_.stLineSw_.posY_;
  }
  void TUIObj::sl_LineRoundX(s32 round)
  {
    stSO_.stLineSw_.posX_ = ((stSO_.stLineSw_.posX_ / round) * round);
    stSO_.stLineSw_.posXBegin_ = stSO_.stLineSw_.posX_;
  }
  void TUIObj::sl_LineRoundY(s32 round)
  {
    stSO_.stLineSw_.posY_ = ((stSO_.stLineSw_.posY_ / round) * round);
    stSO_.stLineSw_.posYBegin_ = stSO_.stLineSw_.posY_;
  }
  //更新処理****************************************************
  s32 TUIObj::GetDrawDepth()
  {
    s32 res = stSO_.stGrp_.basedepth_;
    if (stSO_.stGrp_.charLayer_f_)
    {
      res = stSO_.stGrp_.charLayer_;//所持の直接指定
    }
    else
    {
      if (stSO_.stGrp_.basedepth_ == UIDP_03OBJ)
      {

        if (parent_->stSO_.stGrp_.basedepth_ < UIDP_03OBJ)
        {
          res += (stSO_.stZahyou_.y_ * OVERDEPTH);
        }
        else
        {
          s32 ovdepth = 0;
          g_GetOverDepth(stSO_.stGrp_.basedepth_, &ovdepth, stSO_.stGrp_.ovobj_);
          res += ovdepth;
        }
      }
      else
      {
        s32 ovdepth = 0;
        g_GetOverDepth(stSO_.stGrp_.basedepth_, &ovdepth, stSO_.stGrp_.ovobj_);
        res += ovdepth;

      }
    }

    res += pmgUI_->GetDepth();

    return res;
  }

  BOOL TUIObj::IsDraw()
  {
    if (stSO_.stGrp_.draw_f_ == FALSE) return FALSE;

    if (parent_ != NULL)
    {
      return parent_->IsDraw();
    }
    else
    {
      return stSO_.stGrp_.draw_f_;
    }
  }
  void TUIObj::DrawFont()//フォント描画
  {
    //フォント描画
    //stSO_.stGrp_.draw_f_は画像、フォント共通
    if ((IsDraw() && stSO_.stFont_.draw_f_ && stSO_.stFont_.set_f_) == FALSE) return;
    //if ((stSO_.stGrp_.draw_f_ && stSO_.stFont_.set_f_) == FALSE) return;
    //描画情報
    mid::TStDraw dst;
    dst.Init();
    dst.centerOrigin_f_ = stSO_.stFont_.centerOrigin_f_;//中央寄せはオブジェとリンクでいいか//やっぱばらす
    dst.rightShift_f_ = stSO_.stFont_.rightShift_f_;//中央寄せはオブジェとリンクでいいか//やっぱばらす

    dst.depth_ = GetDrawDepth() + FONTDEPTH;

    f32 posX = stSO_.stFont_.fx_;
    f32 posY = stSO_.stFont_.fy_ + FONTYREV;//補正
    //表示限界
    dst.limL_ = stSO_.stGrp_.limL_;
    dst.limR_ = stSO_.stGrp_.limR_;
    dst.limU_ = stSO_.stGrp_.limU_;
    dst.limD_ = stSO_.stGrp_.limD_;

    if (stSO_.stFont_.centerOrigin_f_) posY += 1;

    z_GetGlobalPos(&posX, &posY);

    s32 fontw = (stSO_.stFont_.mini_f_)
      ? (GRIDSIZE - 4)
      : GRIDSIZE;

    //(stSO_.stFont_.drawlen_ * GRIDSIZE)+1にするとテクスチャサイズが奇数になるのでフォントががたつく
    s32 www = (stSO_.stFont_.drawlen_ * fontw);

    if (stSO_.stFont_.drawlen_ >= stSO_.stFont_.fontlen_) www += 2;

    if (stSO_.stFont_.mini_f_) www += 4;//miniのWのはばが広いので切れる

    TRect rect;
    rect.Bounds(0, 0, www, grfont_.GetHeight());
    grfont_.OrderDrawRect(rect, posX, posY, &dst);

  }
  void TUIObj::DrawFilter()//フィルター描画
  {
    //透明の時は飛ばす
    if (stSO_.stGrp_.argb_.aaa_ == 0) return;


    //描画情報
    mid::TStDraw dst;
    dst.Init();
    dst.noneTexture_f_ = TRUE;
    dst.destW_ = stSO_.stGrp_.filterw_;
    dst.destH_ = stSO_.stGrp_.filterh_;

    dst.argb_ = stSO_.stGrp_.argb_;
    dst.centerOrigin_f_ = FALSE;// stSO_.stGrp_.centerOrigin_f_;//中央寄せはオブジェとリンクでいいか

    dst.depth_ = GetDrawDepth();



    f32 posX = 0.0;
    f32 posY = 0.0;
    z_GetGlobalPos(&posX, &posY);

    //フィルターはテクスチャサイズが描画サイズと違うので、centerOrigin_f_指定ではなく補正済み左上で疑似中心
    if (stSO_.stGrp_.centerOrigin_f_)
    {
      posX -= (stSO_.stGrp_.filterw_ / 2);
      posY -= (stSO_.stGrp_.filterh_ / 2);
    }

    //表示限界
    dst.limL_ = stSO_.stGrp_.limL_;
    dst.limR_ = stSO_.stGrp_.limR_;
    dst.limU_ = stSO_.stGrp_.limU_;
    dst.limD_ = stSO_.stGrp_.limD_;

    posX += stSO_.stGrp_.gx_;
    posY += stSO_.stGrp_.gy_;

    if (mid::midIsNotOpenGL())
    {
      pgr_->OrderDrawAll(posX, posY, &dst);
    }
    else
    {
      pgr_->OrderDraw(0, 0, stSO_.stGrp_.filterw_, stSO_.stGrp_.filterh_, posX, posY, &dst);
    }
  }
  void TUIObj::DrawObj()//オブジェクト描画
  {
    if (pgr_ == NULL) return;

    //描画情報
    mid::TStDraw dst;
    dst.Init();

    dst.depth_ = GetDrawDepth();

    dst.argb_ = stSO_.stGrp_.argb_;
    dst.argb2_ = stSO_.stGrp_.argb2_;
    dst.centerOrigin_f_ = stSO_.stGrp_.centerOrigin_f_;
    dst.under_f_ = stSO_.stGrp_.under_f_;
    dst.rightShift_f_ = stSO_.stGrp_.rightShift_f_;
    dst.drawmode_ = stSO_.stGrp_.drawmode_;
    dst.mirror_f_ = stSO_.stGrp_.mirror_f_;

    f32 posX = 0.0;
    f32 posY = 0.0;
    z_GetGlobalPos(&posX, &posY);

    //表示限界
    dst.limL_ = stSO_.stGrp_.limL_;
    dst.limR_ = stSO_.stGrp_.limR_;
    dst.limU_ = stSO_.stGrp_.limU_;
    dst.limD_ = stSO_.stGrp_.limD_;

    posX += stSO_.stGrp_.gx_;
    posY += stSO_.stGrp_.gy_;

    base::TGrPart* ptpgr = (base::TGrPart*)pgr_;

    s32 ptno = stSO_.stGrp_.partno_;
    if (stSO_.stGrp_.ani_f_)
    {
      TUIObjAniSt* anist = &stSO_.stAni_[stSO_.stGrp_.anino_];
      ptno = anist->anivec_.at(anist->komano_);
    }



    if (pgr_tile_ != NULL)//タイル
    {
      pgr_tile_->OrderDrawTile_CS(ptno, posX, posY, &dst);
    }
    else if (pgr_tile2_ != NULL)//タイル
    {
      pgr_tile2_->OrderDrawTile(ptno, posX, posY, &dst);
    }
    else if (pgr_part_ != NULL)//パーツ
    {
      s32 sw = pgr_part_->GetPartW();
      s32 sh = pgr_part_->GetPartH();
      if (stSO_.stGrp_.sw_ != 0) sw = stSO_.stGrp_.sw_;
      if (stSO_.stGrp_.sh_ != 0) sh = stSO_.stGrp_.sh_;

      if (stSO_.stGrp_.linex_f_)
      {
        pgr_part_->OrderDrawPtX_Ex(ptno, sw, sh, posX, posY, &dst);
      }
      else
      {
        pgr_part_->OrderDrawPtY_Ex(ptno, sw, sh, posX, posY, &dst);
      }
    }
    else//通常（パーツ番号無視）
    {
      s32 sw = pgr_->GetWidth();
      s32 sh = pgr_->GetHeight();
      if (stSO_.stGrp_.sw_ != 0) sw = stSO_.stGrp_.sw_;
      if (stSO_.stGrp_.sh_ != 0) sh = stSO_.stGrp_.sh_;

      pgr_->OrderDraw(0, 0, sw, sh, posX, posY, &dst);
    }
  }
  void TUIObj::DrawChar()//キャラ描画
  {

    if ((IsDraw() && stSO_.stChar_.set_f_) == FALSE) return;
    //if ((stSO_.stChar_.draw_f_ && stSO_.stChar_.draw_f_ && stSO_.stChar_.set_f_) == FALSE) return;

    //描画情報
    f32 posX = 0.0;
    f32 posY = 0.0;
    z_GetGlobalPos(&posX, &posY);

    posX += (stSO_.stChar_.cx_ + stSO_.stChar_.bx_);
    posY += (stSO_.stChar_.cy_ + stSO_.stChar_.by_);


    //キャラ描画情報
    TCharDraw cdst;
    cdst.Init();
    cdst.depth_ = GetDrawDepth();
    cdst.mirror_f_ = stSO_.stChar_.mirror_f_;
    cdst.cellNo_ = stSO_.stChar_.cellno_;

    cdst.drawmode_ = stSO_.stGrp_.drawmode_;
    cdst.argb_ = stSO_.stGrp_.argb_;
    cdst.argb2_ = stSO_.stGrp_.argb2_;

    cdst.limL_ = stSO_.stGrp_.limL_;
    cdst.limR_ = stSO_.stGrp_.limR_;
    cdst.limU_ = stSO_.stGrp_.limU_;
    cdst.limD_ = stSO_.stGrp_.limD_;


    //上書きパーツ

    if (pmgchar_->ccdt_ != NULL)
    {
      if (pmgchar_->ccdt_->GetDt(stSO_.stChar_.cellno_, ccOvNo) != 0)
      {
        cdst.ov_f_ = TRUE;
      }
    }


    s32 bmpNo = cdst.cellNo_ + stSO_.stChar_.bmpoffsetno_;

    if (pmgchar_->ccdt_ != NULL)
    {
      if (pmgchar_->ccdt_->GetDt(stSO_.stChar_.cellno_, ccRefNo) != 0)
      {
        bmpNo = pmgchar_->ccdt_->GetDt(stSO_.stChar_.cellno_, ccRefNo);
      }
    }

    //キャラ
    pmgchar_->OrderDraw(bmpNo, posX, posY, &cdst);
    //影
    //pmgEO_->mgGrp_.pmgChGrp_[st_.mysideNo_][st_.charNo_]->OrderShadow(ShadowX, ShadowY, dbly2_DB, pmgEO_->mgStSn_.EtcSS.Nazo_f);
  }
  void TUIObj::Update(BOOL draw_f, BOOL noinput_f)
  {

    if (draw_f)//描画処理
    {

      //フォント描画
      DrawFont();

      //キャラ描画
      DrawChar();

      //オブジェ描画
      if ((IsDraw() && stSO_.stGrp_.set_f_) == FALSE) return;

      if (stSO_.stGrp_.filter_f_)
      {
        //フィルター描画
        DrawFilter();
      }
      else
      {
        DrawObj();//オブジェクト描画
      }

    }
    else//更新処理
    {
      //通信中はボタンフラグがあるやつすべて消す
      //      if ((pmgEO_->mgPo_.Connecting() == rqConnecting) && stSO_.stBtn_.btn_f_) return;
      //      if (pmgEO_->mgPo_.Connecting() == rqConnecting) return;//処理を行ってしまうのでダメt
      if (mid::midGetRequestState() == mid::rqConnecting) return;
      //基礎深度の更新
      enUIDepth depth = stSO_.stGrp_.basedepth_;
      g_ResetBaseDepth(&depth, stSO_.stGrp_.ovobj_);
      stSO_.stGrp_.basedepth_ = depth;

      //座標移動
      z_Moving();
      //色変化
      g_ColMoving();
      g_AlpMoving();
      //アニメーション
      a_Animation();

      //入力//描画してないやつは入力も受け付けないで大丈夫か
      if (noinput_f == FALSE)
      {
        //モーダル状態以外もしくはモーダル無視オブジェ
        if ((pmgUI_->IsModal() == FALSE) || stSO_.stBtn_.modal_f_)
        {
          //カメラスワイプ
          sc_CamSwiping();
          //ラインスワイプ
          sl_LineSwiping();
        }
        //ボタン受付
        b_Scanning();
      }
    }
  }
  void TUIObj::Update(BOOL draw_f)//更新
  {
    Update(draw_f, FALSE);
  }

  void TUIObj::ReloadTex()
  {
    u32 lasthdl = bmofont_.GetHdl();
    mid::TFontSpriteSt* fst  = pmgEO_->mgFont_.GetFontSpriteSt(lasthdl);
    if (fst != NULL)
    {
      pmgEO_->mgFont_.GenMakeUIFontTexture(&bmofont_, fst);
      //ハンドルをキーにしたポインタ管理
//      pmgEO_->mgFont_.SetFontSpriteSt(lasthdl, NULL);
      pmgEO_->mgFont_.SetFontSpriteSt(bmofont_.GetHdl(), fst);
    }
  }
}


