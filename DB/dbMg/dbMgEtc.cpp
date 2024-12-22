#include "dbMgEtc.h"
#include "dbGame.h"

namespace db
{

  //コンストラクタ
  TMgEtc::TMgEtc(TGame* pGame, s32 etcNo)
    : TGameObjGen(pGame), etcNo_(etcNo)
  {
    pstMyEtc_ = &pmgSG_->stEtc_[etcNo];
  }

  TMgEtc::~TMgEtc()
  {
  }

  //移動
  void TMgEtc::Update()
  {
    switch (pstMyEtc_->MType)
    {
    case etomNone://なにもしない
      return;
    case etomAngel://天使
    {
      UpdateAngel();
    }
    break;
    case etomItem:
    {
      UpdateItem();
    }
    break;
    case etomItemMv:
    {
      UpdateItemMv();
    }
    break;
    case etomItemStop:
    {
      //特に更新なし
      //UpdateItemMv();
    }
    break;
    }
    pstMyEtc_->Zahyou.X += pstMyEtc_->Zahyou.dX;
    pstMyEtc_->Zahyou.Y += pstMyEtc_->Zahyou.dY;
    pstMyEtc_->Zahyou.Z += pstMyEtc_->Zahyou.dZ;
  }
  void TMgEtc::Draw()
  {
    s32 camX = pmgEO_->mgCamera_.GetRevCamPosX();
    s32 camY = pmgEO_->mgCamera_.GetRevCamPosY();
    s32 baseline = (pmgGO_->pmgRf_->IsPrev())
      ? DRAWBASELINE_PREV
      : DRAWBASELINE;

    switch (pstMyEtc_->MType)
    {
    case etomNone://なにもしない
      return;
    case etomAngel://天使
    {
      DrawAngel(camX, camY, baseline);
    }
    break;
    case etomItem:
    case etomItemMv:
    case etomItemStop:
    {
      DrawItem(camX, camY, baseline);
    }
    break;
    }
  }

  void TMgEtc::SetMType(enEtcObjMotion tMType)
  {
    pstMyEtc_->MType = tMType;
    pstMyEtc_->M_c = 0;

    switch (pstMyEtc_->MType)
    {
    case etomItem:
      SetMtypeItem();
      break;
    case etomItemMv:
      SetMtypeItemMv();
      break;
    }
  }

  void TMgEtc::SetMtypeItem()
  {
    s32 iconno = obmt_Todd;
    
    s64 itemid = pmgEO_->stDrop_.GetItemID(FALSE);
    switch ((enItemType)pmgEO_->stDrop_.GetItemType(FALSE))
    {
    case db::enItemType_Team:
      iconno = (obmt_TeamD + pmgEO_->mgCoM_.mdm_Team_.GetLineDt(itemid, mdm_team_rarity));
      break;
    case db::enItemType_Sozai:
      iconno = (obmt_SozaiD + (itemid % SOZAIMID));
      break;
    case db::enItemType_Shot:
      break;
    case db::enItemType_BGM:
      break;
    case db::enItemType_Haikei:
      break;
    case db::enItemType_Supply:
      iconno = obmt_Event0 + (itemid % 4);
      break;
    case db::enItemType_Todd:
      iconno = obmt_Todd;
      break;
    case db::enItemType_Star:
      break;
    case db::enItemType_Pack:
      break;
    default:
      break;
    }
    pstMyEtc_->AniNo = iconno;
    pstMyEtc_->Zahyou.dX = 0;
    pstMyEtc_->Zahyou.dZ = 0;
  }
  void TMgEtc::SetMtypeItemMv()
  {

    //s32 eat = 60;
    s32 stx = pstMyEtc_->Zahyou.X;
    s32 sty = pstMyEtc_->Zahyou.Y;
    s32 stz = pstMyEtc_->Zahyou.Z;
    s32 tagx = ((DBCRT_CL - 24 * XYMAG));
    s32 tagy = 0;
    s32 tagz = (DBCRT_WAL);
    s32 spd = 8 * XYMAG;

    f32 lenx = (tagx - stx);
    f32 leny = (tagy - sty);
    f32 lenz = (tagz - stz);
    f32 h = lib_num::Hypot(lenx, leny);
    f32 h2 = lib_num::Hypot(h, lenz);
    if (h2 == 0.0f)
    {
      pstMyEtc_->Zahyou.dX = 0;
      pstMyEtc_->Zahyou.dY = 0;
      pstMyEtc_->Zahyou.dZ = 0;
      return;
    }

    pstMyEtc_->Zahyou.dX = (lenx / h2) * spd;
    pstMyEtc_->Zahyou.dY = (leny / h2) * spd;
    pstMyEtc_->Zahyou.dZ = (lenz / h2) * spd;

    s32 eat = (h2 / spd);
    if (eat < 1) eat = 1;

    pstMyEtc_->stMove_.Init(90, stx, sty, stz, tagx, tagy, tagz);

  }
  BOOL TMgEtc::SetNewMType(enEtcObjMotion tMType, TZahyouRec tZahyou, s32 tSideNo, s32 tCharNo)
  {
    if (pstMyEtc_->MType == etomNone)
    {
      pstMyEtc_->Zahyou = tZahyou;
      pstMyEtc_->SideNo = tSideNo;
      pstMyEtc_->CharNo = tCharNo;
      SetMType(tMType);
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }



  void TMgEtc::DrawItem(s32 camX, s32 camY, s32 baseline)
  {
    s32 ShadowX = (pstMyEtc_->Zahyou.X / XYMAG) + camX;//カメラ位置反映
    s32 ShadowY = -(pstMyEtc_->Zahyou.Z / XYMAG) + camY + baseline;

    s32 posX = ShadowX;

    const s32 ESIZE = 8;
    s32 posY = ShadowY
      - (pstMyEtc_->Zahyou.Y / XYMAG) - ESIZE;

    mid::TStDraw dst;
    dst.Init();

    dst.centerOrigin_f_ = TRUE;
    dst.depth_ = (base::DPT_3OBJ - pstMyEtc_->Zahyou.Z * ZMAG);
    pmgEO_->mgGrp_.pmgEtcGrp_->OrderDrawTile(pstMyEtc_->AniNo, posX, posY, &dst);


    //描画情報
    //mid::TStDraw dst;
    dst.Init();
    dst.centerOrigin_f_ = TRUE;
    dst.depth_ = base::DPT_3OBJ_SHADOW;
    dst.argb_.aaa_ = SDWALP;

    //u8 col = (pmgEO_->mgStSn_.EtcSS.Nazo_f)
    //  ? SHADOWCOL_NAZO
    //  : SHADOWCOL;
    u8 col = SHADOWCOL;
    dst.argb_.rrr_ = col;
    dst.argb_.ggg_ = col;
    dst.argb_.bbb_ = col;

    //影
    pmgEO_->mgGrp_.pmgEtcGrp_->OrderDrawTile(obmt_BallShadow, (s32)ShadowX, (s32)ShadowY, &dst);
  }

  void TMgEtc::DrawAngel(s32 camX, s32 camY, s32 baseline)
  {
    const s32 CHARW = 32;
    const s32 CHARW_hf = CHARW / 2;
    const s32 CHARH = 32;

    //カメラ位置反映
    s32 ShadowX = (pstMyEtc_->Zahyou.X / XYMAG) + camX;
    s32 ShadowY = -(pstMyEtc_->Zahyou.Z / XYMAG) + camY + baseline;
    s32 posX = ShadowX - CHARW_hf;
    s32 posY = ShadowY
      - (pstMyEtc_->Zahyou.Y / XYMAG)
      - CHARH;

    s32 angelNo = pmgEO_->mgDt_.dtBM_.GetDt(dbmnANG, 0, bmc_bmpNo);
    //キャラ描画情報
    TCharDraw cdst;
    cdst.Init();
    cdst.depth_ = (base::DPT_3OBJ - pstMyEtc_->Zahyou.Z);
    //cdst.shTg_c_ = NGNUM;
    cdst.cellNo_ = angelNo;
    pmgEO_->mgGrp_.pmgChGrp_[pstMyEtc_->SideNo][pstMyEtc_->CharNo]->OrderDraw(angelNo, posX, posY, &cdst);
  }

  void TMgEtc::UpdateAngel()
  {
    const s32 ANGCOUNT = 600;
    const s32 LRITV = 90;
    const s32 LRITV_H = LRITV / 2;
    const s32 LRITV_Q = LRITV / 4;
    const s32 EX = 50;
    const s32 EY = 75;
    pstMyEtc_->Zahyou.dX = (((pstMyEtc_->M_c + LRITV_Q) % LRITV) < LRITV_H)
      ? +EX
      : -EX;

    pstMyEtc_->Zahyou.dY = EY;
    pstMyEtc_->Zahyou.dZ = 0;

    if (lib_num::UpToR(&pstMyEtc_->M_c, ANGCOUNT))
    {
      SetMType(etomNone);
    }
  }

  void TMgEtc::UpdateItem()
  {
    const s32 ITEMCOUNT = 120;
    pstMyEtc_->Zahyou.dY -= 10;
    if (pstMyEtc_->Zahyou.Y < 0)
    {
      pstMyEtc_->Zahyou.dY = 200;
    }

    //審判本に移動
    if (lib_num::UpToR(&pstMyEtc_->M_c, ITEMCOUNT))
    {
      SetMType(etomItemMv);
    }
  }
  void TMgEtc::BrkItemMv(s32 tagpos, s32& nowpos, s32& dpos)
  {
    const s32 RCLEN = XYMAG;//これ以内に入ったら到着にしてがたつきを防ぐ
    s32 len = (tagpos - nowpos);
    s32 acc = 0;
    BOOL rcx_f = FALSE;
    if (tagpos > nowpos)
    {
      if (len <= +RCLEN) rcx_f = TRUE;
    }
    else
    {
      if (len >= -RCLEN) rcx_f = TRUE;
    }

    if (rcx_f)
    {
      nowpos = tagpos;
      dpos = 0;
    }
    else
    {
      if (len != 0) acc = -(dpos * dpos) / (2 * len);
    }

    dpos += acc;

  }

  void TMgEtc::UpdateItemMv()
  {
    BOOL stop_f = FALSE;

    //残り時間減らす
    if (pstMyEtc_->stMove_.eat_ > 0)
    {
      //帳尻を合わせる
      if (lib_num::AprTo0(&pstMyEtc_->stMove_.eat_))
      {
        pstMyEtc_->Zahyou.X = pstMyEtc_->stMove_.tagx_;
        pstMyEtc_->Zahyou.Y = pstMyEtc_->stMove_.tagy_;
        pstMyEtc_->Zahyou.Z = pstMyEtc_->stMove_.tagz_;
        pstMyEtc_->Zahyou.dX = 0;
        pstMyEtc_->Zahyou.dY = 0;
        pstMyEtc_->Zahyou.dZ = 0;
        SetMType(etomItemStop);
      }
      else
      {
        BrkItemMv(pstMyEtc_->stMove_.tagx_, pstMyEtc_->Zahyou.X, pstMyEtc_->Zahyou.dX);
        BrkItemMv(pstMyEtc_->stMove_.tagy_, pstMyEtc_->Zahyou.Y, pstMyEtc_->Zahyou.dY);
        BrkItemMv(pstMyEtc_->stMove_.tagz_, pstMyEtc_->Zahyou.Z, pstMyEtc_->Zahyou.dZ);
      }
    }
  }
}
