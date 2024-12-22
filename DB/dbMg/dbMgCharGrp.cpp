#include "dbMgCharGrp.h"
#include "dbMgEngineObj.h"


namespace db
{
  TStGrChar::TStGrChar(TMgEngineObj* pmgEO) :pmgEO_(pmgEO)
  {
    //Init();
    mid_char_face_ = 0;
    mid_char_hair_ = 0;
    mid_team_ = 0;
    //pal_.i;//dxパレット
    skincol_ = 0;
  };
  void TStGrChar::SetCharDt(s64 mid_char, s64 mid_team, BOOL gb_f)
  {
    mid_char_face_ = mid_char;
    mid_char_hair_ = mid_char;
    mid_team_ = mid_team;
    skincol_ = pmgEO_->mgDt_.dtChar_.GetLooks(clk_SkinCol, mid_char);
    base::TCharPal cpal;
    cpal.Init();
    cpal.gb_f_ = gb_f;
    //cpal.gb_f_ = TRUE;
    cpal.SetEye(pmgEO_->mgDt_.dtChar_.GetLooks(clk_EyesCol, mid_char));
    cpal.SetSkin(pmgEO_->mgDt_.dtChar_.GetLooks(clk_SkinCol, mid_char));
    cpal.SetHair(pmgEO_->mgDt_.dtChar_.GetLooks(clk_HairCol, mid_char));
    cpal.SetAcc(pmgEO_->mgDt_.dtChar_.GetLooks(clk_AcceCol, mid_char));
    cpal.SetWear(pmgEO_->mgCoM_.mdm_Team_.GetLineDt(mid_team, mdm_team_shirCol));
    cpal.SetPants(pmgEO_->mgCoM_.mdm_Team_.GetLineDt(mid_team, mdm_team_pantCol));
    cpal.SetSide(pmgEO_->mgCoM_.mdm_Team_.GetLineDt(mid_team, mdm_team_sideLineCol));
    cpal.SetUnder(pmgEO_->mgCoM_.mdm_Team_.GetLineDt(mid_team, mdm_team_underLineCol));
    cpal.SetShoe(pmgEO_->mgCoM_.mdm_Team_.GetLineDt(mid_team, mdm_team_shoeCol));
    //cpal.SetClothe(clothecol);
    //stGC_.pal_を書き換え
    pmgEO_->dtPal_.SetPal(pal_, &cpal);
  }

  void TStGrChar::SetCharDt(s32 hair, s32 face, s32 haircol, s32 skincol, s32 eyecol, s32 acccol, s32 clothecol, BOOL gb_f)
  {
    mid_char_face_ = face;
    mid_char_hair_ = hair;
    skincol_ = skincol;
    base::TCharPal cpal;
    cpal.Init();
    cpal.gb_f_ = gb_f;
    cpal.SetSkin(skincol);
    cpal.SetHair(haircol);
    cpal.SetEye(eyecol);
    cpal.SetAcc(acccol);
    cpal.SetClothe(clothecol);
    //stGC_.pal_を書き換え
    pmgEO_->dtPal_.SetPal(pal_, &cpal);
  }
  void TStGrChar::SetCharDt(s32 hair, s32 face, s32 haircol, s32 skincol, s32 eyecol, s32 acccol, s32 wearcol, s32 pantscol, s32 sidecol, s32 undercol, s32 shoecol, BOOL gb_f)
  {
	  mid_char_face_ = face;
	  mid_char_hair_ = hair;
	  skincol_ = skincol;
	  base::TCharPal cpal;
	  cpal.Init();
	  cpal.gb_f_ = gb_f;
	  cpal.SetSkin(skincol);
	  cpal.SetHair(haircol);
	  cpal.SetEye(eyecol);
	  cpal.SetAcc(acccol);
	  cpal.SetWear(wearcol);
	  cpal.SetPants(pantscol);
	  cpal.SetSide(sidecol);
	  cpal.SetUnder(undercol);
	  cpal.SetShoe(shoecol);
	  //stGC_.pal_を書き換え
	  pmgEO_->dtPal_.SetPal(pal_, &cpal);
  }

//  s32 TStGrChar::GetLooks(enCharLooks dtNo)
//  {
//    return pmgEO_->mgDt_.dtChar_.GetLooks(dtNo, mid_char_face_);//一応faceを基礎idにしておく
//  }
  s32 TStGrChar::GetLooks_ZuraFile()
  {
    return pmgEO_->mgDt_.dtChar_.GetLooks_ZuraFile(mid_char_hair_);
  }
  s32 TStGrChar::GetLooks_ZuraNo()
  {
    return pmgEO_->mgDt_.dtChar_.GetLooks_ZuraNo(mid_char_hair_);
  }
  s32 TStGrChar::GetLooks_FaceFile()
  {
    return pmgEO_->mgDt_.dtChar_.GetLooks_FaceFile(mid_char_face_);
  }
  s32 TStGrChar::GetLooks_FaceNo()
  {
    return pmgEO_->mgDt_.dtChar_.GetLooks_FaceNo(mid_char_face_);
  }

  //コンストラクタ
  TMgCharGrp::TMgCharGrp(TMgEngineObj* pmgEO)
    : pmgEO_(pmgEO), stGC_(pmgEO)
  {
    body_ = NULL;
    face_ = NULL;
    hair_ = NULL;
    etc_ = NULL;
    ccdt_ = NULL;
    
  }
  
  //開放
  TMgCharGrp::~TMgCharGrp()
  {
  }
  
  //キャラ番号設定(TLUTもへんこう)
  void TMgCharGrp::SetCharDt(s64 mid_char, s64 mid_team, BOOL gb_f)
  {
    stGC_.SetCharDt(mid_char, mid_team, gb_f);
  }
  void TMgCharGrp::SetCharDt(s32 hair, s32 face, s32 haircol, s32 skincol, s32 eyecol, s32 acccol, s32 clothecol, BOOL gb_f)
  {
    stGC_.SetCharDt(hair, face, haircol, skincol, eyecol, acccol, clothecol, gb_f);
  }
  void TMgCharGrp::SetCharDt(s32 hair, s32 face, s32 haircol, s32 skincol, s32 eyecol, s32 acccol, s32 wearcol, s32 pantscol, s32 sidecol, s32 undercol, s32 shoecol, BOOL gb_f)
  {
	  stGC_.SetCharDt(hair, face, haircol, skincol, eyecol, acccol, wearcol, pantscol, sidecol, undercol, shoecol, gb_f);
  }
  void TMgCharGrp::SetMyKantokuDt()
  {
    BOOL gb_f = FALSE;
    stGC_.SetCharDt(
      pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_avtHair),
      pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_avtFace),
      pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_avtHairCol),
      pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_avtSkinCol),
      pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_avtEyeCol),
      pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_avtAccCol),
      pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_avtClotheCol),
      gb_f);
  }

  void TMgCharGrp::SetRankerDt(s64 id_ranker)
  {
    BOOL gb_f = FALSE;
    stGC_.SetCharDt(
      pmgEO_->mgCoU_.mdre_Ranker_.GetPKDt(id_ranker, mdre_ranker_avtHair),
      pmgEO_->mgCoU_.mdre_Ranker_.GetPKDt(id_ranker, mdre_ranker_avtFace),
      pmgEO_->mgCoU_.mdre_Ranker_.GetPKDt(id_ranker, mdre_ranker_avtHairCol),
      pmgEO_->mgCoU_.mdre_Ranker_.GetPKDt(id_ranker, mdre_ranker_avtSkinCol),
      pmgEO_->mgCoU_.mdre_Ranker_.GetPKDt(id_ranker, mdre_ranker_avtEyeCol),
      pmgEO_->mgCoU_.mdre_Ranker_.GetPKDt(id_ranker, mdre_ranker_avtAccCol),
      pmgEO_->mgCoU_.mdre_Ranker_.GetPKDt(id_ranker, mdre_ranker_avtClotheCol),
      gb_f);
  }
  void TMgCharGrp::SetRankerDtByData(s64 id_ranker)
  {
	  BOOL gb_f = FALSE;
	  stGC_.SetCharDt(
		  pmgEO_->mgCoU_.rankerData_.GetPKDt(id_ranker, mdre_ranker_avtHair),
		  pmgEO_->mgCoU_.rankerData_.GetPKDt(id_ranker, mdre_ranker_avtFace),
		  pmgEO_->mgCoU_.rankerData_.GetPKDt(id_ranker, mdre_ranker_avtHairCol),
		  pmgEO_->mgCoU_.rankerData_.GetPKDt(id_ranker, mdre_ranker_avtSkinCol),
		  pmgEO_->mgCoU_.rankerData_.GetPKDt(id_ranker, mdre_ranker_avtEyeCol),
		  pmgEO_->mgCoU_.rankerData_.GetPKDt(id_ranker, mdre_ranker_avtAccCol),
		  pmgEO_->mgCoU_.rankerData_.GetPKDt(id_ranker, mdre_ranker_avtClotheCol),
		  gb_f);
  }
  void TMgCharGrp::SetEnemyDt(s32 no)
  {
    BOOL gb_f = FALSE;
    stGC_.SetCharDt(
      pmgEO_->mgCoU_.mdre_Enemy_.GetLineDt(no, mdre_enemy_avtHair),
      pmgEO_->mgCoU_.mdre_Enemy_.GetLineDt(no, mdre_enemy_avtFace),
      pmgEO_->mgCoU_.mdre_Enemy_.GetLineDt(no, mdre_enemy_avtHairCol),
      pmgEO_->mgCoU_.mdre_Enemy_.GetLineDt(no, mdre_enemy_avtSkinCol),
      pmgEO_->mgCoU_.mdre_Enemy_.GetLineDt(no, mdre_enemy_avtEyeCol),
      pmgEO_->mgCoU_.mdre_Enemy_.GetLineDt(no, mdre_enemy_avtAccCol),
      pmgEO_->mgCoU_.mdre_Enemy_.GetLineDt(no, mdre_enemy_avtClotheCol),
      gb_f);
  }
  
  //角度計算
  mid::enRoll TMgCharGrp::AddRoll(mid::enRoll r, s32 addr)
  {
    s32 rint = (r + mid::rollEND + addr) % (mid::rollEND);
    
    mid::enRoll res = mid::roll_0;
    
    switch (rint)
    {
      case mid::roll_90:  res = mid::roll_90;  break;
      case mid::roll_180: res = mid::roll_180; break;
      case mid::roll_270: res = mid::roll_270; break;
    }
    
    return res;
  }
  
  //画像全体を回転させたときの描画座標
  void TMgCharGrp::RotetPos(s32* posX, s32* posY, BOOL mirror_f, mid::enRoll r)
  {
    //左右反転の時はＸ位置逆
    if (mirror_f)
    {
      *posX = -(*posX);
    }
    
    s32 rx = *posX;
    s32 ry = *posY;
    
    switch (r)
    {
      case mid::roll_90:
        rx = -(*posY);
        ry = (*posX);
        break;
      case mid::roll_180:
        rx = -(*posX);
        ry = -(*posY);
        break;
      case mid::roll_270:
        rx = (*posY);
        ry = -(*posX);
        break;
    }
    
    *posX = rx;
    *posY = ry;
  }
  
  //一部を中心に画像を回転させる（ずらを中心にかおとあごを回転）
  void TMgCharGrp::RotetPart(s32* posX, s32* posY, s32 cposX, s32 cposY, mid::enRoll r)
  {
    s32 tx = (*posX - cposX);
    s32 ty = (*posY - cposY);
    
    s32 rx = tx;
    s32 ry = ty;
    
    switch (r)
    {
      case mid::roll_90:
        rx = -ty;
        ry = tx;
        break;
      case mid::roll_180:
        rx = -tx;
        ry = -ty;
        break;
      case mid::roll_270:
        rx = ty;
        ry = -tx;
        break;
    }
    *posX = (rx + cposX);
    *posY = (ry + cposY);
    
  }
  
//  s32 TMgCharGrp::GetLooks(enCharLooks dtNo)
//  {
//    return stGC_.GetLooks(dtNo);
//  }

  s32 TMgCharGrp::GetLooks_ZuraFile()
  {
    return stGC_.GetLooks_ZuraFile();
  }
  s32 TMgCharGrp::GetLooks_ZuraNo()
  {
    return stGC_.GetLooks_ZuraNo();
  }
  s32 TMgCharGrp::GetLooks_FaceFile()
  {
    return stGC_.GetLooks_FaceFile();
  }
  s32 TMgCharGrp::GetLooks_FaceNo()
  {
    return stGC_.GetLooks_FaceNo();
  }

  //キャラ番号設定
  //店員、女子高生
  void TMgCharGrp::SetGrTile(base::TGrTile* body)
  {
    body_ = body; 
    face_ = NULL; 
    hair_ = NULL;
    etc_ = NULL;
    ccdt_ = NULL;
  }
  //ゲーム内キャラ
  void TMgCharGrp::SetGrTile(s32 sideno, s32 charno)
  {
    body_ = pmgEO_->mgGrp_.pgr_Body_[sideno][charno];
    face_ = pmgEO_->mgGrp_.pgr_Face_[sideno][charno];
    hair_ = pmgEO_->mgGrp_.pgr_Hair_[sideno][charno];
    etc_ = pmgEO_->mgGrp_.pmgEtcGrp_;
    ccdt_ = &pmgEO_->mgDt_.dtCell_;
  }
  //ＵＩキャラ
  void TMgCharGrp::SetGrTile(base::TGrTile* body, base::TGrTile2* face, base::TGrTile2* hair, TDtCell* ccdt)
  {
    body_ = body;
    face_ = face;
    hair_ = hair;
    //etc_ = etc;
    ccdt_ = ccdt;
  }

  
  //描画オーダー
  void TMgCharGrp::OrderDraw(u16 bodyNo, s32 dx, s32 dy, TCharDraw* pcdst)
  {

    if (ccdt_ != NULL)
    {
      s32 bx = (pcdst->mirror_f_)
        ? -ccdt_->GetDt(pcdst->cellNo_, ccBX)
        : +ccdt_->GetDt(pcdst->cellNo_, ccBX);
      s32 by = ccdt_->GetDt(pcdst->cellNo_, ccBY);

      dx += bx;
      dy += by;
    }


    s32 lll = body_->GetCutL(bodyNo);
    s32 rrr = body_->GetCutR(bodyNo);
    s32 ddd = body_->GetCutD(bodyNo);
    s32 revx = ((lll - rrr) / 2);
    s32 dx2 = (pcdst->mirror_f_)
      ? dx - revx
      : dx + revx;
    s32 dy2 = dy - ddd;
    s32 BODYDEPTH = (pcdst->depth_ + PRVLAYRE_BODY);


    //描画情報
    mid::TStDraw dst;
    dst.Init();
    dst.mirror_f_ = pcdst->mirror_f_;
    dst.centerOrigin_f_ = TRUE;
    dst.under_f_ = TRUE;
    dst.roll_ = pcdst->roll_;
    dst.depth_ = BODYDEPTH;
    dst.drawmode_ = pcdst->drawmode_;
    dst.argb_ = pcdst->argb_;
    dst.argb2_ = pcdst->argb2_;

    dst.limL_ = pcdst->limL_;
    dst.limR_ = pcdst->limR_;
    dst.limU_ = pcdst->limU_;
    dst.limD_ = pcdst->limD_;

    ////タゲフラッシュ
    //if ((pcdst->shTg_c_ != NGNUM) && (pcdst->shTg_c_ > 0))
    //{
    //  dst.drawmode_ = mid::drm_ColFil;
    //  
    //  const s32 FLSITV = 20;
    //  const s32 FLSITVHf = FLSITV / 10;//強く光るように見える
    //  const s32 FLSBRT = (pcdst->shTg_c_ < FLSITV)//最初だけ強いフラッシュ
    //  ? 24
    //  : 6;
    //  s32 count = (pcdst->shTg_c_ % FLSITV);//0~19

    //  s32 col = (count < FLSITVHf)
    //  ? (count * FLSBRT)
    //  : ((FLSITV - count) * FLSBRT);
    //  dst.argb2_.aaa_ = lib_num::Between255(col);
    //}
    
    //ボディ
    body_->OrderDrawTile(bodyNo, dx2, dy2, &dst);
    
    if (ccdt_ != NULL)
    {
      //上書きパーツ
      if (pcdst->ov_f_)
      {
        s32 ovX = ccdt_->GetDt(pcdst->cellNo_, ccOvX);
        s32 ovY = ccdt_->GetDt(pcdst->cellNo_, ccOvY);
        RotetPos(&ovX, &ovY, pcdst->mirror_f_, pcdst->roll_);

        s32 ovNo = ccdt_->GetDt(pcdst->cellNo_, ccOvNo);

        s32 lov = body_->GetCutL(ovNo);
        s32 rov = body_->GetCutR(ovNo);
        s32 dov = body_->GetCutD(ovNo);
        s32 revxov = ((lov - rov) / 2);
        s32 dxov = (pcdst->mirror_f_)
          ? dx - revxov
          : dx + revxov;
        s32 dyov = dy - dov;

        dst.depth_ = BODYDEPTH + PRVLAYRE_OV;

        body_->OrderDrawTile(ovNo, dxov + ovX, dyov + ovY, &dst);

      }

      s32 fno = ccdt_->GetDt(pcdst->cellNo_, ccFNo);

      if (pcdst->cadam_c_ > 10)
      {
        if (fno == 0) fno = 9;
        else if (fno == 1) fno = 8;
      }
      else if (pcdst->cadam_c_ > 0)
      {
        if (fno == 0) fno = 9;
        else if (fno == 1) fno = 7;
      }
      //fno = 7;
      //顔
      if (fno >= 0)//-1が非表示
      {

        //レイヤー
        const s32 LYBACK = 5;
        const s32 LYMID = 10;
        const s32 LYFRONT = 15;
        s32 lykao = 0;
        s32 lyago = 0;
        s32 lyzura = 0;
        s32 lyang = 0;
        //描画順 （プラスが奥、マイナスが手前）
        switch (ccdt_->GetDt(pcdst->cellNo_, ccFZ))
        {
        case 0://顔が奥、髪だけ手前
          lykao = -1;
          lyzura = +1;
          break;
        case 1://顔が手前
          lykao = +1;
          lyzura = +2;
          break;
        default://顔髪ともに奥
          lykao = -2;
          lyzura = -1;
          break;
        }
        lyang = lykao - 1;//天使の輪は髪の上やっぱ顔の下

        //顔の回転
        s32 addr = ccdt_->GetDt(pcdst->cellNo_, ccFR);
        if (pcdst->mirror_f_) addr = -addr;
        //全体の回転と顔の回転を足した回転位置
        mid::enRoll faceroll = AddRoll(pcdst->roll_, addr);
        s32 fcX = ccdt_->GetDt(pcdst->cellNo_, ccFX);
        s32 fcY = ccdt_->GetDt(pcdst->cellNo_, ccFY) - 32 + FACEH;
        RotetPos(&fcX, &fcY, pcdst->mirror_f_, pcdst->roll_);

        s32 kaoX = fcX;
        s32 kaoY = fcY;
        kaoY -= (FACEH / 2);
        s32 zuraX = fcX;
        s32 zuraY = fcY;
        const s32 ZURARY = -1;
        zuraY += ZURARY;
        const s32 ANGY = -9;
        s32 angX = fcX;
        s32 angY = fcY;
        angY += ANGY;

        RotetPart(&zuraX, &zuraY, fcX, fcY, faceroll);
        RotetPart(&angX, &angY, fcX, fcY, faceroll);
        RotetPart(&kaoX, &kaoY, fcX, fcY, faceroll);


        dst.roll_ = faceroll;
        dst.under_f_ = FALSE;
        dst.depth_ = BODYDEPTH + lykao;
        face_->OrderDrawTile(fno, dx + kaoX, dy + kaoY, &dst);

        const s32 ZURA_OF_FACE[] = { 0, 1, 2, 5, 5, 4, 3, 1, 1, 0, 0 };
        s32 hno = ZURA_OF_FACE[fno];
        dst.depth_ = BODYDEPTH + lyzura;
        hair_->OrderDrawTile(hno, dx + zuraX, dy + zuraY, &dst);
      }

      ////天使の輪
      //if (pcdst->ang_f_)
      //{
      //  dst.depth_ = (BODYDEPTH + lyang);
      //  const s32 ANGNO = 110;
      //  body_->OrderDrawTile(ANGNO, dx + angX, dy + angY, &dst);
      //}
    }
  }
  
  //影描画
  void TMgCharGrp::OrderShadow(s32 posX, s32 posY, BOOL nazo_f, BOOL prev_f)
  {
    //描画情報
    mid::TStDraw dst;
    dst.Init();
    dst.centerOrigin_f_ = TRUE;
    //dst.depth_ = base::DPT_3OBJ_SHADOW;
    //プレビューのプライオリティ
    dst.depth_ = (prev_f)
      ? base::DPT_4OVER
      : base::DPT_3OBJ_SHADOW;

    dst.argb_.aaa_ = SDWALP;
    
    dst.argb_.rrr_ = dst.argb_.ggg_ = dst.argb_.bbb_ = (nazo_f)
    ? SHADOWCOL_NAZO
    : SHADOWCOL;
    //影
    etc_->OrderDrawTile(obmt_CharShadow, posX, posY - 2, &dst);
  } 
}
