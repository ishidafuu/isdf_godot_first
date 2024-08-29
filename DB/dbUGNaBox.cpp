#include "dbUGNaBox.h"

namespace db
{
  const s32 TOPPOSY = 128;
  const s32 FLASHITV = 6;

  enum enFallStep
  {
    fs_None,
    fs_gacha,
    fs_kantoku,
    fs_end,
    fs_gg,
  };

  enum enBoxGachaBGM
  {
    bgm_shop = 243,
    bgm_boxgacha = 10,
  };
  enum enBoxGachaSE
  {
    se_gachafall = 10,
    se_gachabound = 11,
    se_select = 21,
    se_decide = 22,
    se_zawa = 23,
    se_hue = 24,
    se_otonadecide = 25,
    se_hissatsu = 26,
  };



  //コンストラクタ
  TUGNaBox::TUGNaBox(TUIGenLayer* player) : TUGGen(player), ugfilter_(player)//アクションなし
  {
    stBoxGacha_.Init();
  }

  TUGNaBox::~TUGNaBox()
  {

  }

  void TUGNaBox::MakeGroup(TUIObj* parent, TUGChKantoku* ugkantoku, TUGChCaptain* ugcap, TUGChClerk* ugcleak)
  {
    SetParent(parent);
    ugkantoku_ = ugkantoku;
	ugcap_ = ugcap;
    ugcleak_ = ugcleak;

    ugfilter_.MakeGroup(NULL);

    //field_->o_SetObjGrpPart(grppart);
    field_->z_SetPos(0, 0);

    boxgacha_ = MakeObj();//ガチャ筐体
    boxgacha_->o_SetParent(field_);
    boxgacha_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_box));
    boxgacha_->z_SetPos(0, 0);
    //boxgacha_->g_SetRevDepthLv(1);
    boxgacha_->g_SetDraw(FALSE);

    boxnumber_ = MakeObj();
    boxnumber_->o_SetParent(boxgacha_);
    boxnumber_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_boxnum));
    boxnumber_->z_SetPos(1, 5);
    boxnumber_->g_SetRevDepthLv(1);
    //boxSize_->g_SetDraw(FALSE);

    const s32 LIMY = -60;
    const s32 LIMH = 88;
    ugcleak_->GetField()->g_SetLimY(LIMY, LIMH);
    boxgacha_->g_SetLimY(LIMY, LIMH);
    boxnumber_->g_SetLimY(LIMY, LIMH);

    const s32 BX = 2;
    const s32 BY = 6;
    for (s32 i = 0; i < PLATEMAX; ++i)
    {
      plate_[i] = MakeObj();
      plate_[i]->o_SetParent(boxgacha_);
      plate_[i]->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_box_part));
      s32 posx = BX + ((i % 5) * 12);
      s32 posy = BY + ((i / 5) * 10);
      plate_[i]->z_SetPos(posx, posy);
      //plate_[i]->g_SetDraw(FALSE);
    }
  }


  void TUGNaBox::TapField()
  {
    Act_Order();
  }

  //抽選通信入る前にリフレッシュをかける
  void TUGNaBox::RefreshBox(s64 mid_nabox)
  {
    stBoxGacha_.Init();
    restno_.clear();
    mid_nabox_ = mid_nabox;

    s64 id_nabox = 0;
    stBoxGacha_.boxSize_ = pmgEO_->mgCoM_.mdm_NaBox_.GetPKDt(mid_nabox, mdm_nabox_boxSize);

    for (s32 i = 0; i < pmgEO_->mgCoU_.mdNaBox_.GetLineNum(); ++i)
    {
      if (pmgEO_->mgCoU_.mdNaBox_.GetLineDt(i, mdnabox_id_m_nabox) == mid_nabox)
      {
        id_nabox = pmgEO_->mgCoU_.mdNaBox_.GetLineDt(i, mdnabox_pk);
        for (s32 i = 0; i < stBoxGacha_.boxSize_; ++i)
        {
          stBoxGacha_.opened_f_[i] = pmgEO_->mgCoU_.IsNaBoxOpen(id_nabox, i);
        }
      }
    }

    for (s32 i = 0; i < PLATEMAX; ++i)
    {
//      if (i % 3 == 0) stBoxGacha_.opened_f_[i] = TRUE;

      if (i >= stBoxGacha_.boxSize_)
      {
        plate_[i]->g_SetPartNo(3);//封鎖
        plate_[i]->g_SetRevDepthLv(2);
      }
      else
      {
        if (stBoxGacha_.opened_f_[i])
        {
          plate_[i]->g_SetPartNo(2);//既にOPEN
        }
        else
        {
          plate_[i]->g_SetPartNo(0);//残り
          restno_.push_back(i);//残り番号
        }

        plate_[i]->g_SetRevDepthLv(0);
      }

    }
  }
  //あたり番号セット
  void TUGNaBox::SetNaBoxResult(BOOL otona_f)
  {
    //ここで初期化OKか
    stBoxGacha_.Init();
    s64 id_nabox = pmgEO_->mgCoU_.GetNaBox_id(mid_nabox_);
    stBoxGacha_.decideno_ = pmgEO_->mgCoU_.mdNaBox_.GetPKDt(id_nabox, mdnabox_lastNo);
    stBoxGacha_.otona_f_ = otona_f;
  }


  void TUGNaBox::Act_FallNaBox()//ガチャ落ちてくる命令
  {
    stBoxGacha_.posY_ = TOPPOSY * XYMAG;
    stBoxGacha_.dY_ = 0;
    stBoxGacha_.fallstep_ = fs_gacha;
    boxgacha_->g_SetDraw(TRUE);
    s32 pos = -(stBoxGacha_.posY_ / XYMAG);
    boxgacha_->g_SetGrpPos(0, pos);
    boxnumber_->g_SetGrpPos(0, pos);
    for (s32 i = 0; i < PLATEMAX; ++i) plate_[i]->g_SetGrpPos(0, pos);
    pmgEO_->mgNSF_.StopBGM();
    stBoxGacha_.select_c_ = 0;
    stBoxGacha_.endwait_c_ = 0;

    pmgEO_->mgSound_.PlaySYS(se_gachafall);

  }
  void TUGNaBox::Act_FallGG()//GG落ちてくる
  {
    stBoxGacha_.posY_ = TOPPOSY * XYMAG;
    stBoxGacha_.dY_ = 0;
    stBoxGacha_.fallstep_ = fs_gg;
    ugcleak_->SetDraw(TRUE);
    s32 pos = -(stBoxGacha_.posY_ / XYMAG);
    ugcleak_->GetField()->c_SetPos(0, pos);
  }

  //スタートさせる
  void TUGNaBox::Act_Order()
  {
    if (stBoxGacha_.orderOK_f_)
    {
      ugkantoku_->SetMirror(TRUE);
      stBoxGacha_.orderOK_f_ = FALSE;
      ugkantoku_->SetMType(kmt_ShopOrd);//オーダー

      if (stBoxGacha_.otona_f_)
      {
        stBoxGacha_.select_c_ = (restno_.size() * 8);
      }
      else
      {
        stBoxGacha_.select_c_ = 256;
      }
    }
  }

  void TUGNaBox::Mv_FallBoxGacha()//ガチャ落ちてくる
  {
    s32 gposy = 0;
    s32 kposy = 0;
    s32 ggposy = 0;

    if ((stBoxGacha_.fallstep_ == fs_gacha)
      || (stBoxGacha_.fallstep_ == fs_kantoku)
      || (stBoxGacha_.fallstep_ == fs_gg)
      )
    {
      s32 GRV = (stBoxGacha_.fallstep_ == fs_gacha)
        ? 7
        : 10;
      if (stBoxGacha_.posY_ > 0)
      {
        stBoxGacha_.posY_ += stBoxGacha_.dY_;
        stBoxGacha_.dY_ -= GRV;
        //着地
        if (stBoxGacha_.posY_ <= 0)
        {
          stBoxGacha_.posY_ = 0;
          stBoxGacha_.dY_ = 0;
          switch (stBoxGacha_.fallstep_)
          {
          case fs_gacha:
            stBoxGacha_.fallstep_ = fs_kantoku;
            stBoxGacha_.ggout_f_ = TRUE;
            stBoxGacha_.out_c_ = 0;
            stBoxGacha_.posY_ = 1;
            stBoxGacha_.dY_ = 100;
            pmgEO_->mgSound_.PlaySYS(se_gachabound);
            break;
          case fs_kantoku:
            stBoxGacha_.fallstep_ = fs_end;
            stBoxGacha_.orderOK_f_ = TRUE;
            pmgEO_->mgNSF_.Play(nsf_nabox);//BGM
            break;
          case fs_gg:
            stBoxGacha_.fallstep_ = fs_end;
            break;
          }
        }
      }

      s32 posy = -(stBoxGacha_.posY_ / XYMAG);
      switch (stBoxGacha_.fallstep_)
      {
      case fs_gacha:gposy = posy; break;
      case fs_kantoku:kposy = posy; break;
      case fs_gg:ggposy = posy; break;
      }
    }

    if (stBoxGacha_.gachaout_f_ == FALSE)
    {
      boxgacha_->g_SetGrpPos(0, gposy);
      boxnumber_->g_SetGrpPos(0, gposy);
      for (s32 i = 0; i < PLATEMAX; ++i) plate_[i]->g_SetGrpPos(0, gposy);
    }


    ugkantoku_->GetField()->c_SetPos(0, kposy);
	ugcap_->GetField()->c_SetPos(0, kposy);
    if (stBoxGacha_.fallstep_ == fs_gg)
    {
      ugcleak_->GetField()->c_SetPos(0, ggposy);
    }
    
  }



  void TUGNaBox::Mv_OutBoxGacha()//ガチャはける
  {
    if (stBoxGacha_.gachaout_f_)
    {
      ++stBoxGacha_.out_c_;
      s32 posy = -stBoxGacha_.out_c_ / 2;
      boxgacha_->g_SetGrpPos(0, posy);
      boxnumber_->g_SetGrpPos(0, posy);
      for (s32 i = 0; i < PLATEMAX; ++i) plate_[i]->g_SetGrpPos(0, posy);

      if (posy < -TOPPOSY)
      {
        stBoxGacha_.gachaout_f_ = FALSE;
        boxgacha_->g_SetDraw(FALSE);
      }
    }
  }
  void TUGNaBox::Mv_OutGG()
  {
    if (stBoxGacha_.ggout_f_)
    {
      ++stBoxGacha_.out_c_;
      s32 posy = -stBoxGacha_.out_c_ * 2;
      ugcleak_->GetField()->c_SetPos(0, posy);
      if (posy < -TOPPOSY)
      {
        ugcleak_->SetDraw(FALSE);
        stBoxGacha_.ggout_f_ = FALSE;
      }
    }
  }

  //番号点滅
  void TUGNaBox::Mv_Select()
  {
    if (stBoxGacha_.otona_f_) return;

    if (stBoxGacha_.select_c_ == 1)
    {
      pmgEO_->mgSound_.PlaySYS(se_decide);
      plate_[stBoxGacha_.lastno_]->g_SetPartNo(0);
      plate_[stBoxGacha_.decideno_]->g_SetPartNo(1);//FLASH

      //確定
      --stBoxGacha_.select_c_;
      stBoxGacha_.endwait_c_ = 60;
    }
    else if (stBoxGacha_.select_c_ > 0)
    {
      BOOL flash_f = FALSE;
      if (stBoxGacha_.select_c_ <= 64)
      {
        if (stBoxGacha_.select_c_ % 32 == 0)
        {
          flash_f = TRUE;
        }
      }
      else if (stBoxGacha_.select_c_ <= 128)
      {
        if (stBoxGacha_.select_c_ % 16 == 0)
        {
          flash_f = TRUE;
        }
      }
      else if (stBoxGacha_.select_c_ <= 192)
      {
        if (stBoxGacha_.select_c_ % 8 == 0)
        {
          flash_f = TRUE;
        }
      }
      else
      {
        if (stBoxGacha_.select_c_ % 4 == 0)
        {
          flash_f = TRUE;
        }
      }

      if (flash_f)
      {
        pmgEO_->mgSound_.PlaySYS(se_select);

        s32 flashidx = mid::midGetRandMn(restno_.size());
        s32 flashno = restno_[flashidx];
        if (stBoxGacha_.lastno_ != NGNUM)
        {
          plate_[stBoxGacha_.lastno_]->g_SetPartNo(0);//既にOPEN
        }

        plate_[flashno]->g_SetPartNo(1);//FLASH
        stBoxGacha_.lastno_ = flashno;
      }

      --stBoxGacha_.select_c_;
    }
  }
  //番号点滅（大人買い）
  void TUGNaBox::Mv_OtonaSelect()
  {
    if (stBoxGacha_.otona_f_ == FALSE) return;

    if (stBoxGacha_.select_c_ == 1)
    {
      pmgEO_->mgSound_.PlaySYS(se_otonadecide);//確定
      stBoxGacha_.endwait_c_ = 60;
      --stBoxGacha_.select_c_;
    }
    else  if (stBoxGacha_.select_c_ > 0)
    {
      if (stBoxGacha_.select_c_ % 8 == 0)
      {
        pmgEO_->mgSound_.PlaySYS(se_select);
        s32 flashno = restno_[stBoxGacha_.lastno_ + 1];
        plate_[flashno]->g_SetPartNo(1);
        ++stBoxGacha_.lastno_;
      }

      --stBoxGacha_.select_c_;
    }
  }
  void TUGNaBox::Mv_EndWait()//間
  {
    if (stBoxGacha_.endwait_c_ > 0)
    {
      if (stBoxGacha_.endwait_c_ == 20)
      {
        Act_FallGG();//GG落ちてくる
        stBoxGacha_.out_c_ = 0;
        stBoxGacha_.gachaout_f_ = TRUE;
        SetAct(enUGNaBoxAct_End);
      }
      else if (stBoxGacha_.endwait_c_ == 1)
      {
        pmgEO_->mgSound_.PlaySYS(se_hue);
        pmgEO_->mgSound_.PlaySYS(se_zawa);
      }
      //if (stBoxGacha_.endwait_c_ == 10)
      //{
      //  stBoxGacha_.gachaout_f_ = TRUE;
      //}
      --stBoxGacha_.endwait_c_;
    }
  }

  void TUGNaBox::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    boxgacha_->Update(draw_f);//ガチャ筐体
    boxnumber_->Update(draw_f);//レバー
    for (s32 i = 0; i < PLATEMAX; ++i) plate_[i]->Update(draw_f);
    //ugkantoku_->Update(draw_f);//監督
    //ugcleak_->Update(draw_f);//店員
    ugfilter_.Update(draw_f);
    
  }

  void TUGNaBox::DoAction()
  {
    Mv_FallBoxGacha();
    Mv_OutBoxGacha();
    Mv_OutGG();
    Mv_Select();
    Mv_OtonaSelect();
    Mv_EndWait();
  }

  BOOL TUGNaBox::IsActRes(enUGNaBoxAct res)//返値ゲット
  {
    return (GetActRes() == res);
  }

}
