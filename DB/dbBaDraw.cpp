#include "dbBaDraw.h"
#include "dbGame.h"

namespace db
{
  
  TBaDraw::TBaDraw(TGame* pGame, TBaCommon* pCommon)
  : TBaHaveCommon(pGame, pCommon)
  {
  }
  
  TBaDraw::~TBaDraw(void)
  {
  }
  
  
  //ボール描画
  void TBaDraw::DrawBall()
  {
  
    if (st_.pstBa_->Motion == bmReferee) return;
    
    if ((pmgSG_->stRf_.DBSSt == dbssResult)
        || (pmgSG_->stRf_.DBSSt == dbssGohome)) return;
    
    //カメラ位置
    s32 camposX = pmgEO_->mgCamera_.GetRevCamPosX();
    s32 camposY = pmgEO_->mgCamera_.GetRevCamPosY();

    s32 baseline = (pmgGO_->pmgRf_->IsPrev())
      ? DRAWBASELINE_PREV
      : DRAWBASELINE;

    s32 SDWDEPTH =   (pmgGO_->pmgRf_->IsPrev())
      ? base::DPT_4OVER
      : base::DPT_3OBJ_SHADOW;
    s32 BALLDEPTH = (pmgGO_->pmgRf_->IsPrev())
      ? base::DPT_5_0FILTER
      : base::DPT_3OBJ;

    if (
        //必殺の場合分裂系による中心球消失フラグがなければ中心球の表示
        ((st_.pstSh_->phs.realBlHide_f == FALSE) && (st_.pstBa_->Motion == bmShoot))
        ||
        //必殺以外では常に表示
        (st_.pstBa_->Motion != bmShoot)
        )
    {
      if (st_.pstSh_->cmn.spdtVnsSdw_f == FALSE)
      {
        s32 ShadowX = (st_.pstBa_->Zahyou.X / XYMAG) + camposX;
        s32 ShadowY = -(st_.pstBa_->Zahyou.Z / XYMAG) + camposY + baseline;
        //描画情報
        mid::TStDraw dst;
        dst.Init();
        dst.centerOrigin_f_ = TRUE;
        //dst.depth_ = base::DPT_3OBJ_SHADOW;
        dst.depth_ = SDWDEPTH;
        dst.argb_.aaa_ = SDWALP;

        //u8 col = (pmgEO_->mgStSn_.EtcSS.Nazo_f)
        //  ? SHADOWCOL_NAZO
        u8 col = SHADOWCOL;

        dst.argb_.rrr_ = col;
        dst.argb_.ggg_ = col;
        dst.argb_.bbb_ = col;
        //影
        pmgEO_->mgGrp_.pmgEtcGrp_->OrderDrawTile(obmt_BallShadow, (s32)ShadowX, (s32)ShadowY, &dst);
      }

      if (st_.pstSh_->cmn.spdtVnsBal_f == FALSE)
      {
        s32 ShadowX = (st_.pstBa_->Zahyou.X / XYMAG) + camposX;
        s32 ShadowY = -(st_.pstBa_->Zahyou.Z / XYMAG) + camposY + baseline;
        //新画像番号
        s32 gNo = st_.pstSh_->cmn.spdtGrhNum;
        const s32 HSCOL = 1;
        const s32 NSCOL = 2;
        s32 bCol = 0;//ノーマル
        if (st_.pstBa_->Motion == bmHold)
        {
          //カウンター
          if (st_.pstBa_->counter_f) bCol = NSCOL;
          //シュートガイド
          //BOOL shguid_f = (pmgEO_->mgStSn_.OptionSS.GetSt(cstmOpShGuide) == 0);
          if (pmgSG_->stCh_[st_.pstBa_->HoldTNo][st_.pstBa_->HoldPNo].HSTiming == hstmSpecial) bCol = HSCOL;
        }
        else if (st_.pstBa_->Motion == bmShoot)
        {
          //新ボールカラー
          if (st_.pstSh_->cmn.spdtRevClr_f) bCol = HSCOL;
        }

        s32 posX = ShadowX;
        s32 posY = ShadowY - (st_.pstBa_->Zahyou.Y / XYMAG) - 7;//形が変わろうとボールの真のサイズは16なので中心表示かつその半分上が描画中心となる

        //描画情報
        mid::TStDraw dst;
        dst.Init();
        //posをいじるのではなくフラグで処理してカット量を反映させる
        dst.centerOrigin_f_ = TRUE;
        //新反転
        dst.mirror_f_ = st_.pstSh_->cmn.spdtRevHor_f;
        dst.depth_ = (BALLDEPTH + st_.pstBa_->Zahyou.Pri - (st_.pstBa_->Zahyou.Z * ZMAG));


        //ボール
        pmgEO_->mgGrp_.pgr_Ball_[bCol]->OrderDrawTile_CS(gNo, posX, posY, &dst);
      }
    }

#ifdef __K_DEBUG_SHIAI__
	// 敵のターゲット位置フリーカーソルモードじゃなければNULLが入ってるはず
	kdebug::DebugSystem* pDs = kdebug::DebugSystem::GetInstance();
	if (pDs->IsFreeTargetPosMode())
	{
		s32 tmpX = pDs->GetFreeTargetPosX();
		s32 tmpY = pDs->GetFreeTargetPosY();
		s32 tmpZ = pDs->GetFreeTargetPosZ();

		s32 ShadowX = tmpX + camposX;
    s32 ShadowY = -tmpZ + camposY + baseline;

		s32 posX = ShadowX;
		s32 posY = ShadowY - tmpY - 8;//形が変わろうとボールの真のサイズは16なので中心表示かつその半分上が描画中心となる


		//描画情報
		mid::TStDraw dst;
		dst.Init();
		dst.centerOrigin_f_ = TRUE;
		//dst.depth_ = base::DPT_3OBJ_SHADOW;
		dst.depth_ = SDWDEPTH;
		dst.argb_.aaa_ = SDWALP;
		
		// 2015/02/16現在ソースの整理によりNazo_fが存在しない為、コメントアウト
		u8 col = /*(pmgEO_->mgStSn_.EtcSS.Nazo_f)
			? SHADOWCOL_NAZO
			: */SHADOWCOL;

		dst.argb_.rrr_ = col;
		dst.argb_.ggg_ = col;
		dst.argb_.bbb_ = col;
		//影
		pmgEO_->mgGrp_.pmgEtcGrp_->OrderDrawTile(obmt_BallShadow, (s32)ShadowX, (s32)ShadowY, &dst);
	}
#endif	// #ifdef __K_DEBUG_SHIAI__

    //分裂
    if (st_.pstBa_->Motion == bmShoot)//必殺中のみ
    {
        for (s32 i = 0; i < SPARATE_MAX; ++i)
        {
            TShpSprt* psprtDt = &st_.pstSh_->phs.sprtDt[i];
            if (psprtDt->use_f == FALSE) continue;

            if (psprtDt->vnsSdw_f == FALSE)
            {

                s32 ShadowX = (psprtDt->posX / XYMAG) + camposX;
                s32 ShadowY = -(psprtDt->posZ / XYMAG) + camposY + baseline;
                //描画情報
                mid::TStDraw dst;
                dst.Init();
                dst.centerOrigin_f_ = TRUE;
                dst.depth_ = SDWDEPTH;
                dst.argb_.aaa_ = SDWALP;

                //u8 col = (pmgEO_->mgStSn_.EtcSS.Nazo_f)
                //    ? SHADOWCOL_NAZO
                u8 col = SHADOWCOL;

                dst.argb_.rrr_ = col;
                dst.argb_.ggg_ = col;
                dst.argb_.bbb_ = col;
                //影
                pmgEO_->mgGrp_.pmgEtcGrp_->OrderDrawTile(obmt_BallShadow, (s32)ShadowX, (s32)ShadowY, &dst);
            }

            if (psprtDt->vnsBal_f == FALSE)
            {
                s32 ShadowX = (psprtDt->posX / XYMAG) + camposX;
                s32 ShadowY = -(psprtDt->posZ / XYMAG) + camposY + baseline;
                //新画像番号
                s32 gNo = psprtDt->grphNo;
                const s32 HSCOL = 1;
                const s32 NSCOL = 2;
                s32 bCol = 0;//ノーマル
                if (st_.pstBa_->Motion == bmHold)
                {
                    //カウンター
                    if (st_.pstBa_->counter_f) bCol = NSCOL;
                    //シュートガイド
                    //BOOL shguid_f = (pmgEO_->mgStSn_.OptionSS.GetSt(cstmOpShGuide) == 0);
                    if (pmgSG_->stCh_[st_.pstBa_->HoldTNo][st_.pstBa_->HoldPNo].HSTiming == hstmSpecial) bCol = HSCOL;
                }
                else if (st_.pstBa_->Motion == bmShoot)
                {
                    //新ボールカラー
                    if (psprtDt->revClr_f) bCol = HSCOL;
                }

                s32 posX = ShadowX;
                s32 posY = ShadowY - (psprtDt->posY / XYMAG) - 7;//形が変わろうとボールの真のサイズは16なので中心表示かつその半分上が描画中心となる

                //描画情報
                mid::TStDraw dst;
                dst.Init();
                //posをいじるのではなくフラグで処理してカット量を反映させる
                dst.centerOrigin_f_ = TRUE;
                //新反転

                dst.mirror_f_ = psprtDt->revHor_f;
                dst.depth_ = (BALLDEPTH + st_.pstBa_->Zahyou.Pri - (psprtDt->posZ * ZMAG));
                //ボール
                pmgEO_->mgGrp_.pgr_Ball_[bCol]->OrderDrawTile_CS(gNo, posX, posY, &dst);
            }
        }
    }


  }
  
  //時間描画
  void TBaDraw::DrawTime()
  {
    ////ボール時間
    //if ((pmgSG_->stRf_.Demo_f == FALSE)
    //    && (pmgEO_->stShiai_.IsTuto() == FALSE))
    //{
    //  //持ち時間
    //  const s32 SECFLAME = 60;

    //  s32 posy = 0;
    //  s32 posx = 0;// pmgEO_->mgDt_.dtScenario_.GetDBShiaiIntDt(mndbiBallTime_XF);
    //  if (st_.pstBa_->TimerSide == 0)
    //  {
    //    
    //  }
    //  else if (st_.pstBa_->TimerSide == 1)
    //  {
    //    //posx += pmgEO_->mgDt_.dtScenario_.GetDBShiaiIntDt(mndbiBallTime_XDF);
    //  }
    //  else
    //  {
    //    pmgEO_->mgFont_.FixFontSprite_Draw((s32)fnsBallTime, (BOOL)FALSE);
    //    
    //    return;
    //  }
    //  
    //  if (st_.pstBa_->DrawTime != ((st_.pstBa_->Timer_c + (SECFLAME - 1)) / SECFLAME))//変わった瞬間だけ
    //  {
    //    //                st_.pstBa_->NeedTimeRedraw_f = FALSE;
    //    std::string st = lib_str::IntToStr(((st_.pstBa_->Timer_c - 1) / SECFLAME) + 1);
    //    pmgEO_->mgFont_.OrderFontSpriteGrid(
    //                                        (s32)fnsBallTime,
    //                                        st.c_str(),
    //                                        posx,
    //                                        posy,
    //                                        base::FC2_normal,
    //                                        base::fd_None,
    //                                        base::DPT_4OVER_FONT
    //                                        );
    //    
    //  }
    //}
    
  }
  
  
  void TBaDraw::Update()
  {
    
    //ボール描画
    DrawBall();
    
    //時間描画
    //DrawTime();
  }
  
  
}
