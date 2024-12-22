#include "dbUGDrGen.h"


namespace db
{
  const s32 DFW = 160;
  const s32 DFH = 160;

  //コンストラクタ
  TUGDrGen::TUGDrGen(TUIGenLayer* player) : TUGGen(player)
  {
    stDrGen_.Init();
  }
  TUGDrGen::~TUGDrGen()
  {

  }
  void TUGDrGen::Ready(s32 dtnum)
  {
    //レコードの数はあくまでオブジェの数なので、データの数とは一致しない

    stDrGen_.dtnum_ = dtnum;
    //共通な物はGenに写すか
    SelectDrumEnd(FALSE);//末端消す
    SelectUra(FALSE);//表表示
    //一回回す
    GenDrumRoll(TRUE, 0);
    DoReady();

    //デフォルトはしない
    //stDrGen_.loop_f_ = TRUE;

    SetLoop(FALSE);

  }
  //ドラムがループするか
  void TUGDrGen::SetLoop(BOOL loop_f)
  {
    stDrGen_.loop_f_ = loop_f;
    if (loop_f)
    {
      field_->sl_LineSwipeHeight(0, 0);
    }
    else
    {
      SetSwipeHeight(0);

      if (stDrGen_.dtnum_ <= 1)
      {
        StopSwipe(TRUE);
      }
      //field_->sl_LineSwipeHeight(0, stDrGen_.DISTY_ * (stDrGen_.dtnum_ - 1));
    }
  }
  void TUGDrGen::SetSwipeHeight(s32 fdtno)
  {
    if (stDrGen_.loop_f_ == FALSE)
    {
      s32 miny = -(fdtno * stDrGen_.DISTY_);
      s32 maxy = (stDrGen_.DISTY_ * (stDrGen_.dtnum_ - 1)) + miny;
      field_->sl_LineSwipeHeight(miny, maxy);
    }
  }

  void TUGDrGen::SetSwipeSize()
  {
    f32 x = 0;
    f32 y = 0;
    //グローバル位置を取得し、そこから実際のスワイプ範囲を求める
    field_->z_GetGlobalPos(&x, &y);
    field_->sl_LineSwipeLimX(x, DFW);
    field_->sl_LineSwipeLimY(-(DFH / 2), DFH);
  }

  //とまってるかどうか
  BOOL TUGDrGen::IsStop()
  {
    return !(stDrGen_.move_f_);
  }
  //フォーカス前面に来てるカードのモデル番号
  s32 TUGDrGen::GetDataNo()
  {
    return stDrGen_.dtno_;
  }
  s32 TUGDrGen::GetLookRecord()
  {
    return stDrGen_.recno_;
  }
  void TUGDrGen::GenMakeGroup(TUIObj* parent, BOOL leftpos_f)
  {
    field_->sl_LineSwipeY(TRUE);//Ｙラインスワイプ

    //継承先でサイズなどが決まったら諸々作成
    field_->o_SetParent(parent);
    s32 posx = (leftpos_f)
      ? -160
      : 8;
    field_->z_SetPos(posx, -mid::midFullDotS_Hf());

    //グローバル位置を取得し、そこから実際のスワイプ範囲を求める
    SetSwipeSize();

  }

  void TUGDrGen::GenAddRecord(TUGGen* pug)
  {
    gen_records_.push_back(pug);
  }
  void TUGDrGen::GenSetUramen(TUGGen* pug)//裏面オブジェ
  {
    gen_ura_ = pug;
    gen_ura_->SetPos(0, (stDrGen_.REVY_ + (stDrGen_.DISTY_ * stDrGen_.LOOKPOS_)));
    gen_ura_->GetField()->g_SetRevDepthLv(DRRECNUM + 1);//最終オブジェ//より一つ上
    stDrGen_.useura_f_ = TRUE;
  }
  void TUGDrGen::GenSetDrumEnd(TUGGen* pug)//決定オブジェ
  {
    gen_drumend_ = pug;
    gen_drumend_->SetPos(0, (stDrGen_.REVY_ + (stDrGen_.DISTY_ * stDrGen_.LOOKPOS_)));
    gen_drumend_->GetField()->g_SetRevDepthLv(DRRECNUM);
    stDrGen_.usedrumend_f_ = TRUE;
  }

  //ドラムのスワイプをオンオフ
  void TUGDrGen::StopSwipe(BOOL stop_f)
  {

    if ((stDrGen_.dtnum_ <= 1) && (stDrGen_.loop_f_ == FALSE))
    {
      stop_f = TRUE;
    }

    field_->sl_StopLineSwipe(stop_f);//スワイプstop
  }

  void TUGDrGen::DrumStop()
  {
    const s32 CARDSPD = 2;
    const s32 CARDSPD2 = 20;
    const s32 MOVEEAT = 30;

    for (s32 i = 0; i < gen_records_.size(); ++i)
    {
      //上部前面のやつは上側がスライドする
      BOOL slide_f = (stDrGen_.TOPFRONT_F_)
        ? (stDrGen_.drrecposno_[i] < stDrGen_.LOOKPOS_)
        : (stDrGen_.drrecposno_[i] > stDrGen_.LOOKPOS_);

      s32 posy = (stDrGen_.drrecposno_[i] * stDrGen_.DISTY_) + stDrGen_.REVY_;

      if (slide_f)
      {
        posy += stDrGen_.SLIDEY_;//カードが見えるように
        gen_records_[i]->GetField()->z_MoveBrk(0, posy, CARDSPD2, MOVEEAT);
      }
      else
      {
        gen_records_[i]->GetField()->z_MoveBrk(0, posy, CARDSPD, MOVEEAT);
      }
    }
  }
  void TUGDrGen::DrumStart()
  {
    const s32 CARDSPD = 2;
    const s32 CARDSPD2 = 15;
    const s32 MOVEEAT = 10;
    for (s32 i = 0; i < gen_records_.size(); ++i)
    {
      s32 posy = (stDrGen_.drrecposno_[i] * stDrGen_.DISTY_) + stDrGen_.REVY_;
      gen_records_[i]->GetField()->z_MoveBrk(0, posy, CARDSPD2, MOVEEAT);
    }

    stDrGen_.start_f_ = TRUE;
  }

  //ドラムを回す
  void TUGDrGen::GenDrumRoll(BOOL first_f, s32 fdtno)
  {
    stDrGen_.roll_f_ = FALSE;
    stDrGen_.stop_f_ = FALSE;
    stDrGen_.start_f_ = FALSE;

    //初期化
    if (first_f)
    {
      stDrGen_.dtno_ = fdtno;
      stDrGen_.recno_ = (fdtno % gen_records_.size());
      stDrGen_.swpos_ = 0;
      stDrGen_.swpos2_ = 0;
      SetSwipeHeight(fdtno);
    }

    //タッチパネルからの情報
    s32 nowpos2 = (field_->sl_LineGetY() / stDrGen_.DISTY_);
    s32 nowpos = field_->sl_LineGetY();

    //移動があった
    if ((nowpos != stDrGen_.swpos_) || first_f)
    {
      //裏表示斬る
      if (stDrGen_.ura_f_) SelectUra(FALSE);
      

      if (stDrGen_.move_f_ == FALSE)
      {
        stDrGen_.move_f_ = TRUE;
        DrumStart();
      }

      BOOL mvno_f = FALSE;

      if (nowpos2 < stDrGen_.swpos2_)//下方向回転
      {
        mvno_f = TRUE;
        s32 loopnum = (stDrGen_.swpos2_ - nowpos2);
        for (s32 i = 0; i < loopnum; ++i)
        {
          if (stDrGen_.loop_f_ == FALSE)
          {
            if (stDrGen_.dtno_ <= 0) break;
          }

          lib_num::DecLoop(&stDrGen_.recno_, 0, (gen_records_.size() - 1));
          lib_num::DecLoop(&stDrGen_.dtno_, 0, (stDrGen_.dtnum_ - 1));
          stDrGen_.roll_f_ = TRUE;
        }
      }
      else if (nowpos2 > stDrGen_.swpos2_)//上方向回転
      {
        mvno_f = TRUE;
        s32 loopnum = (nowpos2 - stDrGen_.swpos2_);
        for (s32 i = 0; i < loopnum; ++i)
        {
          if (stDrGen_.loop_f_ == FALSE)
          {
            if (stDrGen_.dtno_ >= (stDrGen_.dtnum_ - 1)) break;
          }

          lib_num::IncLoop(&stDrGen_.recno_, 0, (gen_records_.size() - 1));
          lib_num::IncLoop(&stDrGen_.dtno_, 0, (stDrGen_.dtnum_ - 1));
          stDrGen_.roll_f_ = TRUE;
        }
      }

      s32 nowpos3 = (field_->sl_LineGetY() % stDrGen_.DISTY_);

      for (s32 i = 0; i < gen_records_.size(); ++i)
      {
        s32 posno = (((i - stDrGen_.recno_ + stDrGen_.LOOKPOS_) + gen_records_.size()) % gen_records_.size());
        // s32 posno = (i - stDrGen_.recno_ + stDrGen_.LOOKPOS_);
        stDrGen_.drrecposno_[i] = posno;

        if (stDrGen_.dtnum_ > 0)
        {
          stDrGen_.drrecdtno_[i] = (stDrGen_.dtno_ - stDrGen_.LOOKPOS_ + posno);// % stDrGen_.dtnum_

          //ループドラム（でふぉ）
          if (stDrGen_.loop_f_)
          {
            stDrGen_.drrecdtno_[i] = (stDrGen_.drrecdtno_[i] + stDrGen_.dtnum_) % stDrGen_.dtnum_;//データ番号を納める
            if (stDrGen_.drrecdtno_[i] < 0)
            {
              stDrGen_.drrecdtno_[i] = (stDrGen_.drrecdtno_[i] + stDrGen_.dtnum_);
            }
          }
          else//ノンループドラム
          {
              //データ番号が範囲外
              if ((stDrGen_.drrecdtno_[i] < 0) || (stDrGen_.drrecdtno_[i] >= stDrGen_.dtnum_))
              {
                stDrGen_.drrecloop_f_[i] = TRUE;
                stDrGen_.drrecdtno_[i] = (stDrGen_.drrecdtno_[i] + (stDrGen_.dtnum_ * gen_records_.size())) % stDrGen_.dtnum_;//データ番号を納める
              }
              else
              {
                stDrGen_.drrecloop_f_[i] = FALSE;
              }

          }
        }
        else
        {
          stDrGen_.drrecdtno_[i] = 0;
        }


        s32 posy = (posno * stDrGen_.DISTY_) - nowpos3 + stDrGen_.REVY_;
        gen_records_[i]->SetPos(0, posy);
      }

    }
    else//移動してないとき
    {
      //今まで移動してた
      if (stDrGen_.move_f_)
      {
        //カウンタ進行
        if ((field_->stSO_.stLineSw_.genswp_f_ == FALSE)//実際にスワイプしてない
          && (field_->stSO_.stLineSw_.swp_f_ == FALSE))//かつ、慣性も掛かってない
        {
          //まるめ
          field_->sl_LineRoundY(stDrGen_.DISTY_);
          //まるめたやつを再取得
          nowpos2 = (field_->sl_LineGetY() / stDrGen_.DISTY_);
          nowpos = field_->sl_LineGetY();

          stDrGen_.move_c_ = 0;
          stDrGen_.move_f_ = FALSE;
          //stDrGen_.loading_f_ = TRUE;
          stDrGen_.stop_f_ = TRUE;
          DrumStop();
        }
      }
      else
      {
        if ((field_->stSO_.stLineSw_.genswp_f_ == TRUE) || (field_->stSO_.stLineSw_.swp_f_ == TRUE))
        {
          stDrGen_.move_f_ = TRUE;
          DrumStart();
        }
      }
    }

    //Ｙ座標による深度変更
    for (s32 i = 0; i < gen_records_.size(); ++i)
    {
      s32 depthlv = (stDrGen_.TOPFRONT_F_)
        ? (gen_records_.size() - stDrGen_.drrecposno_[i])
        : stDrGen_.drrecposno_[i];

      gen_records_[i]->GetField()->g_SetRevDepthLv(depthlv);

      if (i != stDrGen_.recno_) //合成ベースなど使用不可もダークに
      {
        gen_records_[i]->GetField()->g_Dark();
      }
      else
      {
        gen_records_[i]->GetField()->g_EfReset();
      }

      //ノンループの時はー１週目と２週目は表示しない
      if (stDrGen_.loop_f_)
      {
        gen_records_[i]->GetField()->g_SetDraw(TRUE);
      }
      else
      {
        gen_records_[i]->GetField()->g_SetDraw(!stDrGen_.drrecloop_f_[i]);
      }
    }

    //現在位置保存
    stDrGen_.swpos_ = nowpos;
    stDrGen_.swpos2_ = nowpos2;
  }

  void TUGDrGen::DrumRoll_LookDt(s32 dtno)
  {
    GenDrumRoll(TRUE, dtno);
  }
  void TUGDrGen::DrumRoll()
  {
    GenDrumRoll(FALSE, NGNUM);
  }


  //これは継承か
  void TUGDrGen::ShiftOKPos(BOOL toppos_f)
  {

  }

  void TUGDrGen::SelectDrumEnd(BOOL drumend_f)
  {
    stDrGen_.drumend_f_ = drumend_f;
    StopSwipe(drumend_f);//ドラムエンドの時はスワイプオフ
    ShiftOKPos(!drumend_f);
  }

  BOOL TUGDrGen::IsUra()
  {
    return stDrGen_.ura_f_;
  }



  //裏返す
  void TUGDrGen::SelectUra(BOOL ura_f)
  {
    if (stDrGen_.useura_f_ == FALSE) return;
    ura_f = DoSelectUra(ura_f);

    stDrGen_.ura_f_ = ura_f;
    gen_ura_->SetDraw(ura_f);//描画オンオフ
    //StopSwipe(ura_f);//裏の時はスワイプオフ
  }

  void TUGDrGen::DoUpdate(BOOL draw_f)//更新
  {
    //一つもないとき表示しない
    if ((stDrGen_.dtnum_ == 0) && draw_f) return;


    field_->Update(draw_f);
    if (stDrGen_.drumend_f_ && stDrGen_.usedrumend_f_)
    {
      gen_drumend_->Update(draw_f);
    }
    else
    {
      for (s32 i = 0; i < gen_records_.size(); ++i) gen_records_[i]->Update(draw_f);
    }
    if (stDrGen_.useura_f_) gen_ura_->Update(draw_f);
    DoUpdateSub(draw_f);
  }

  void TUGDrGen::SetDontRoll()
  {
    stDrGen_.notroll_f_ = TRUE;
  }

  void TUGDrGen::DoAction()//更新
  {
    if (IsDraw() == FALSE) return;

    stDrGen_.notroll_f_ = FALSE;

    DoActionSub();

    //裏面、決定画面、ボタン押してる間は回さない
    //if (stDrGen_.ura_f_ || stDrGen_.drumend_f_) SetDontRoll();
    if (stDrGen_.drumend_f_) SetDontRoll();


    if (stDrGen_.notroll_f_ == FALSE)
    {
      DrumRoll();

      if (stDrGen_.roll_f_)
      {
        SetAct(enUGDrGenAct_Roll, stDrGen_.dtno_);//回ったタイミング
        DoRoll();
      }
      else if (stDrGen_.stop_f_)
      {
        SetAct(enUGDrGenAct_Stop, stDrGen_.dtno_); //留まったタイミング
        DoStop();//停止カードの読み込み
      }
      else if (stDrGen_.start_f_)
      {
        SetAct(enUGDrGenAct_Start, stDrGen_.dtno_); //動いたタイミング
		DoStart();
      }
    }


  }
  BOOL TUGDrGen::IsActRes(enUGDrumAct res)//返値ゲット
  {
    return (GetActRes() == res);
  }

  BOOL TUGDrGen::IsData()
  {
    return (stDrGen_.dtnum_ > 0);
  }

  void TUGDrGen::SetLRPos(BOOL leftpos_f)
  {
	  s32 posx = (leftpos_f)
		  ? -160
		  : 8;
	  field_->z_SetPos(posx, -mid::midFullDotS_Hf());

	  //グローバル位置を取得し、そこから実際のスワイプ範囲を求める
	  SetSwipeSize();
  }
}
