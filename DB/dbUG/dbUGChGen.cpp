#include "dbUGChGen.h"


namespace db
{
  //コンストラクタ
  TUGChGen::TUGChGen(TUIGenLayer* player) : TUGGen(player)
  {
    stChar_.Init();
    itemobj_ = NULL;
  }

  TUGChGen::~TUGChGen()
  {

  }
  //管理元から呼び出される。TMgCharGrpを完成させておく、
  void TUGChGen::GenMakeGroup(TUIObj* parent, TMgCharGrp* pmgchar, TBMData* bmdt, s32 bmpoffset)
  {
    SetParent(parent);
    field_->o_SetObjMgCharGrp(pmgchar);
    field_->c_SetBmpOffsetNo(bmpoffset);
    field_->g_SetDepth(UIDP_03OBJ);
    //chobj_->c_SetBaseLine(80);//てきとう
    bmdt_ = bmdt;
  }

  //監督のＢＭＤＴ
  s32 TUGChGen::NowKoma(enBMCommonDt dtNo)
  {
    return bmdt_->GetDt(stChar_.mt_.mtype_, stChar_.mt_.frameNo_, dtNo);
  }
  BOOL TUGChGen::NowKomaBOOL(enBMCommonDt dtNo)
  {
    return (NowKoma(dtNo) != 0);
  }

  //最終コマか
  BOOL TUGChGen::IsLastKoma()
  {
    return (bmdt_->Getkomanum(stChar_.mt_.mtype_) == (stChar_.mt_.frameNo_ + 1));
  }
  //次のフレーム
  void TUGChGen::NextFrame()
  {
    //ラスコマ
    if (IsLastKoma())//(NowKoma(bmc_Last_f != 0)
    {
      SetAct(enUGChGenAct_MotihonEnd);
    }
    else//次コマ
    {
      ++stChar_.mt_.frameNo_;
      SetFrameData(FALSE);
    }
  }
  ////フレーム情報ＳＥ
  //void TChCommon::SettingSE()
  //{
  //  const s32 DEFANISE = 1;
  //  const s32 NOANISE = 0;
  //  s32 anise = NowKoma(bmc_se);

  //  switch (anise)
  //  {
  //  case NOANISE:
  //  case DEFANISE:
  //    break;
  //  default:
  //    SESetInt(anise);
  //    break;
  //  }
  //}
  void TUGChGen::SetGenMType(s32 mtype)
  {
    stChar_.mt_.Init();
    stChar_.mt_.mtype_ = mtype;
    SetFrameData(FALSE);
  }

  //フレーム情報のセット
  void TUGChGen::SetFrameData(BOOL InLoop_f)
  {
    //ループ始点の場合
    if (InLoop_f == FALSE)
    {
      switch (NowKoma(bmc_loopSt))
      {
      case (s32)bmlp_St:
        stChar_.mt_.loop_c_ = NowKoma(bmc_loopNum);
        stChar_.mt_.loopStNo_ = stChar_.mt_.frameNo_;
        break;

      case (s32)bmlp_StEd:
        stChar_.mt_.loop_c_ = NowKoma(bmc_loopNum);
        break;
      }
      //SettingSE();
    }
    else
    {
      if (NowKomaBOOL(bmc_seloop_f))
      {
        //SettingSE();
      }
    }

    //初期化
    stChar_.mt_.ani_c_ = 0;
    stChar_.mt_.actP_f_ = NowKomaBOOL(bmc_ActP_f);

    //アクションポイント
    if (stChar_.mt_.actP_f_)
    {
      SetAct(enUGChGenAct_ActPoint);
    }

  }
  //モーションを進める
  void TUGChGen::IncMotion(BOOL goto_f)//goto_f:強制的に進める
  {
    //アニメーションカウンタ増加
    ++stChar_.mt_.ani_c_;

    //フレーム終了の時間がきましたもしくは強制次フレーム
    if ((stChar_.mt_.ani_c_ >= NowKoma(bmc_DefFrm))
      || (goto_f))
    {

      switch (NowKoma(bmc_loopSt))
      {
        //ループ終了
      case (s32)bmlp_Ed:

        //カウンタ
        --stChar_.mt_.loop_c_;

        //ループ終了
        if (stChar_.mt_.loop_c_ == 0)
        {
          NextFrame();
        }
        else//ループ中
        {
          //ループ始点まで戻る
          stChar_.mt_.frameNo_ = stChar_.mt_.loopStNo_;
          SetFrameData(TRUE);

          if (stChar_.mt_.loop_c_ <= NGNUM)
          {
            stChar_.mt_.loop_c_ = 0;
          }
        }
        break;
        //同コマループ
      case (s32)bmlp_StEd:
        //カウンタ
        --stChar_.mt_.loop_c_;

        //ループ終了
        if (stChar_.mt_.loop_c_ == 0)
        {
          NextFrame();
        }
        else if (stChar_.mt_.loop_c_ <= NGNUM)
        {
          stChar_.mt_.loop_c_ = 0;
        }
        break;

        //非ループ
      default:
        NextFrame();
        break;
      }
    }
  }
  //モーションカウンタ増加
  void TUGChGen::Count()
  {
    //モーションカウンタ増やす
    ++stChar_.mt_.m_c_;
    IncMotion(FALSE);
    //現在のセル番号
    field_->c_SetCellNo(NowKoma(bmc_bmpNo));
    //BMの位置補正
    field_->c_SetBMPos(NowKoma(bmc_posX), NowKoma(bmc_posY));
  }
  void TUGChGen::SetMirror(BOOL mirror_f)
  {
    stChar_.mirror_f_ = mirror_f;
  }

  //所持アイテム
  void TUGChGen::SetItem(TUIObj* itemobj)
  {
    itemobj_ = itemobj;
    itemobj_->g_SetCenter(TRUE);
  }
  void TUGChGen::ReleaseItem()
  {
    itemobj_ = NULL;
  }
  BOOL TUGChGen::IsItem()
  {
    return (itemobj_ != NULL);
  }
  BOOL TUGChGen::IsMirror(void)
  {
	  return stChar_.mirror_f_;
  }
  void TUGChGen::SetItemPos()
  {
    if (IsItem())
    {
      TDtCell* ccdt = field_->c_GetDtCell();

      const s32 TXREV = 16;//体の中心から
      s32 cellNo = NowKoma(bmc_bmpNo);
      s32 bx = ccdt->GetDt(cellNo, ccBX) + NowKoma(bmc_posX);
      s32 by = ccdt->GetDt(cellNo, ccBY) + NowKoma(bmc_posY);
      //持たれ位置補正
      s32 tx = NowKoma(bmc_posX) + bx;// -TXREV;
      s32 ty = NowKoma(bmc_posY) + by - TXREV;// +TIPSIZE8;
      //プライオリティ
      s32 tz = (ccdt->GetDt(cellNo, ccIZ) == 0)
        ? PRVLAYRE_BALL
        : -1;
      s32 tlx = ccdt->GetDt(cellNo, ccIX) + tx;
      s32 tly = ccdt->GetDt(cellNo, ccIY) + ty;
      tlx = (stChar_.mirror_f_)
        ? (field_->stSO_.stZahyou_.x_ - (tlx))
        : (field_->stSO_.stZahyou_.x_ + (tlx));
      tly = ((field_->stSO_.stZahyou_.y_ + tly));

      // 持たれ時のキャラ移動補正
      tlx += field_->stSO_.stChar_.cx_;
      tly += field_->stSO_.stChar_.cy_;

      itemobj_->z_SetPos(tlx, tly);
      itemobj_->g_SetCharHaveDepth(field_->c_GetDepth() + tz);
    }
  }

  void TUGChGen::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    if (itemobj_ != NULL)  itemobj_->Update(draw_f);
  }
  void TUGChGen::DoAction()
  {
    //カウンタ進める
    field_->c_SetMirror(stChar_.mirror_f_);
    Count();

    //所持アイテムの位置とプライオリティ
    SetItemPos();

    if (field_->z_IsStop())
    {
      SetAct(enUGChGenAct_MoveStop);

      //止まったときに予約した方向を向く
      switch (stChar_.muki_)
      {
      case cmk_Left: SetMirror(TRUE); break;
      case cmk_Right: SetMirror(FALSE); break;
      }
      stChar_.muki_ = cmk_None;
    }


    DoActionSub();
  }
  BOOL TUGChGen::IsActRes(enUGChGenAct res)//返値ゲット
  {
    return (GetActRes() == res);
  }

}
