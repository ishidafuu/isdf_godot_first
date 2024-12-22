#include "dbUGInput.h"
//文字リテラルをUTF-8で読み込む
#pragma execution_character_set("utf-8")

namespace db
{
  enum enInputBtn
  {
    ib_Emp,
    ib_Select,
    ib_Space,
    ib_Back,
    ib_dakuh,
    ib_handh,
    ib_dakuk,
    ib_handk,
    ibEND,
  };

  enum enDakuon
  {
    dk_None,
    dk_Daku,
    dk_Handaku,
    dk_Now,
    dkEND,
  };



  //const char HIRA[] = "あいうえおはひふへほっぁ戻かきくけこまみむめもゃぃゝさしすせそやゐゆゑよゅぅ心たちつてとらりるれろょぇ波なにぬねのわをん壱弐ーぉ空";
  //const char HIRAD[] = "あい威えおばびぶべぼっぁ戻がぎぐげごまみむめもゃぃゞざじずぜぞやゐゆゑよゅぅ心だぢづでどらりるれろょぇ波なにぬねのわをん壱弐ーぉ空";
  //const char HIRAH[] = "あいうえおぱぴぷぺぽっぁ戻かきくけこまみむめもゃぃゝさしすせそやゐゆゑよゅぅ心たちつてとらりるれろょぇ波なにぬねのわをん壱弐ーぉ空";
  //const char KATA[] = "アイウエオハヒフヘホッァ戻カキクケコマミムメモャィヽサシスセソヤヰユヱヨュゥ心タチツテトラリルレロョェ波ナニヌネノワヲン参四ーォ空";
  //const char KATAD[] = "アイウエオバビブベボッァ戻ガギグゲゴマミムメモャィヾザジズゼゾヤヰユヱヨュゥ心ダヂヅデドラリルレロョェ波ナニヌネノワヲン参四ーォ空";
  //const char KATAH[] = "アイウエオパピプペポッァ戻カキクケコマミムメモャィヽサシスセソヤヰユヱヨュゥ心タチツテトラリルレロョェ波ナニヌネノワヲン参四ーォ空";
  //const char ALPA[] = "ＡＢＣＤＥａｂｃｄｅＺｚ戻ＦＧＨＩＪｆｇｈｉｊ１２３ＫＬＭＮＯｋｌｍｎｏ４５６ＰＱＲＳＴｐｑｒｓｔ７８９ＵＶＷＸＹｕｖｗｘｙ負０空";
  //const char ALPA_S[] = "ＡＢＣＤＥＺ戻ＦＧＨＩＪ１２３ＫＬＭＮＯ４５６ＰＱＲＳＴ７８９ＵＶＷＸＹ負０空";
  ////  const char SP[]   = "！？＆＃＠著剣心◇葉ⅡⅢ戻※〃・三波▽月音Д＞平北％濁半、。ー△εΛω＜西南東；：，．´｀￣／全≧＋掛÷「」（）”＾下＼積≦負＝＿";
  //const char SP[] = "！？＆＃＠著剣心菱葉ⅡⅢ戻※同・黙波谷星音泥＞平上％濁半、。ー山熊羊鸚＜左下右；：，．鋭重天／全大＋掛割「」（）引曲地＼積小負＝空";
  //const char SP[] = "威ゝゞヽヾ゛゜〃負▼￥「」ー～！？、。…・，．／％：（）臓心↑↓←→著左右ⅡⅢ平＋掛÷＝星＃＆＠※；”´｀＾￣下＼全積＞＜≧≦▽△剣葉◇升月音ДΛεω";
  const char EMP[] = "＿";
  //const char BACK[] = "戻";
  //const char DAKUH[] = "壱";
  //const char HANDH[] = "弐";
  //const char DAKUK[] = "参";
  //const char HANDK[] = "四";

  //コンストラクタ
  TUGInput::TUGInput(TUIGenLayer* player) :TUGGen(player),
    tabradio_(player),
    ugbtn_decide_(player),
    ugbtn_back_(player)
  {
    stInput_.Init();

    INTVECTOR intvec;
    //for (s32 i = 0; i < (dk_Handaku + 1); ++i) itemsword_.push_back(intvec);
    for (s32 i = 0; i < dkEND; ++i) outsword_.push_back(intvec);


  }
  TUGInput::~TUGInput()
  {
    for (s32 i = 0; i < tabbtn_.size(); ++i) Freemem(tabbtn_[i]);
    //破棄
    ClearInputBtn();
    ClearOutBtn();
  }
  void TUGInput::ClearInputBtn()
  {
    //破棄
    for (s32 i = 0; i < keys_.size(); ++i) Freemem(keys_[i]);
    keys_.clear();
  }
  void TUGInput::ClearOutBtn()
  {
    for (s32 i = 0; i < outs_.size(); ++i) Freemem(outs_[i]);
    outs_.clear();
    for (s32 i = 0; i < dkEND; ++i) outsword_[i].clear();
  }
  void TUGInput::MakeGroup(TUIObj* parent)
  {
    stInput_.spacecode_ = base::MojiCodeToInt(EMP[0], EMP[1], EMP[2]);

    SetParent(parent);
    const s32 BARW = 512;

    field_->g_SetCenter(TRUE);//フィルタはフィルタサイズが小さいためセンタリングが正常に効かない
    field_->o_SetObjFilterGrp(BARW, BARW, 0xFF, 0, 0, 0);
    field_->g_SetAlp(FALSE, 0x80);
    field_->g_SetDepth(UIDP_08MSGFILTER);

    ugbtn_decide_.MakeGroup_FtPos(field_, 1, "おーけー");
    ugbtn_back_.MakeGroup_FtPosBack(field_);

    //入力下地
    {
      inputfield_ = MakeObj();
      inputfield_->o_SetParent(field_);
    }

    //出力下地
    {
      outputfieldend_ = MakeObj();
      outputfieldend_->o_SetParent(field_);
      outputfieldend_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_inp_outend));

      outputfield_ = MakeObj();
      outputfield_->o_SetParent(outputfieldend_);
      outputfield_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_inp_out));
      outputfield_->g_SetOvOvj(outputfieldend_);
    }

    //タブ（アルファベットのみのときは不要）
    {
      tabradio_.MakeGroup(inputfield_, 24, 12);
      tabradio_.SetPos(0, 124);

      for (s32 i = 0; i < UGI_TAB; ++i)
      {
        TUGButton* newbtn = new TUGButton(player_);
        newbtn->MakeGroup_Radio(NULL, ug_btn_inputtab, ug_btn_inputtab2);
        newbtn->SetModal(TRUE);
        tabradio_.AddButton(newbtn, 48);
        tabbtn_.push_back(newbtn);
      }

      const s32 TABFPOSX = 0;// 8;
      const s32 TABFPOSY = 0;// 4;
      tabbtn_[0]->SetFont("ひら", FC2_input_tab, TABFPOSX, TABFPOSY);
      tabbtn_[1]->SetFont("カタ", FC2_input_tab, TABFPOSX, TABFPOSY);
      tabbtn_[2]->SetFont("Ａ１", FC2_input_tab, TABFPOSX, TABFPOSY);
      tabbtn_[3]->SetFont("！？", FC2_input_tab, TABFPOSX, TABFPOSY);
    }

    //最初は消しとく
    SetDraw(FALSE);
  }

  void TUGInput::Refresh(s32 outw, BOOL alponly_f)
  {
    INTVECTOR defmcvec;
    defmcvec.clear();
    Refresh(outw, alponly_f, defmcvec);
  }
  void TUGInput::Refresh(s32 outw, BOOL alponly_f, INTVECTOR defmcvec)
  {
    //アウトプットはクリア
    ClearOutput();

    SetDraw(TRUE);

    const s32 INPUTY = -52;
    //いったんクリア


    //入力系に変化有りのときだけ
    if ((stInput_.alponly_f_ != alponly_f) || (stInput_.outw_ == 0))
    {
      ClearInputBtn();
      //入力下地
      if (alponly_f)
      {
        const s32 INPUTX = -100;
        inputfield_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_inp_boad2));//アルファベットオンリーの時は変わる
        inputfield_->z_SetPos(INPUTX, INPUTY);
      }
      else
      {
        const s32 INPUTX = -160;
        inputfield_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_inp_boad));
        inputfield_->z_SetPos(INPUTX, INPUTY);
      }

      //入力文字
      //各レコード
      s32 keynum = (alponly_f)
        ? UGI_ALLWORD_S
        : UGI_ALLWORD;

      for (s32 i = 0; i < keynum; ++i)//アルファベットのみの時は変わる
      {
        TUGInputKey* newkey = new TUGInputKey(player_);//破棄が必要
        newkey->MakeGroup(inputfield_, i, alponly_f);
        newkey->SetModal(TRUE);
        keys_.push_back(newkey);
      }
      //タブ（アルファベットのみのときは不要）
      tabradio_.SetDraw(!alponly_f);
    }

    tabradio_.ResetSelect();//かなに選択タブもどす

    //アウト系クリア
    ClearOutBtn();

    s32 BTNH = 24;
    s32 BTNW = 24;
    BOOL nallow_f = FALSE;
    if (outw >= 14)
    {
      nallow_f = TRUE;
      BTNW = 18;
    }

    //出力下地
    {
      s32 outw2 = BTNW * outw;
      outputfieldend_->z_SetPos(-(outw2 / 2) + outw2, INPUTY - 26);//入力文字数による
      outputfield_->z_SetPos(-outw2, 0);//入力文字数による
      outputfield_->g_SetSrcW(outw2);
    }

    //出力文字
    enUGGrPart outgrp = (nallow_f)
      ? ugp_btn_key2
      : ugp_btn_key;

    for (s32 i = 0; i < outw; ++i)
    {
      TUGButton* newout = new TUGButton(player_);//破棄が必要
      newout->MakeGroup_Radio(outputfield_, outgrp, ib_Emp, ib_Select);
      newout->SetModal(TRUE);
      outs_.push_back(newout);
      s32 nowno = outs_.size() - 1;
      newout->SetPos((nowno * BTNW) + 2 + (BTNW / 2), 2 + (BTNH / 2));

      //初期とりあえずスペースうめ
      if (i < defmcvec.size())
      {
        outsword_[dk_None].push_back(defmcvec[i]);
        outsword_[dk_Daku].push_back(defmcvec[i]);
        outsword_[dk_Handaku].push_back(defmcvec[i]);
        outsword_[dk_Now].push_back(defmcvec[i]);
      }
      else
      {
        outsword_[dk_None].push_back(stInput_.spacecode_);
        outsword_[dk_Daku].push_back(stInput_.spacecode_);
        outsword_[dk_Handaku].push_back(stInput_.spacecode_);
        outsword_[dk_Now].push_back(stInput_.spacecode_);
      }
      outs_[i]->GetField()->f_MakeFontMC(outsword_[dk_Now][i], FC2_input_word);
    }

    stInput_.outw_ = outw;
    stInput_.alponly_f_ = alponly_f;
    stInput_.alllen_ = (alponly_f)
      ? UGI_ALLLINELEN_S
      : UGI_ALLLINELEN;
    stInput_.tabno_ = 0;

  }


  s32 TUGInput::DecideItems()
  {
    for (s32 i = 0; i < keys_.size(); ++i)
    {
      if (keys_[i]->IsDecide()) return i;
    }
    return NGNUM;
  }
  std::string TUGInput::OutputString()
  {
    //アルファベット入力かどうかで１バイト出力判定
    return pmgEO_->mgFont_.CnvMCVecToStr(&outsword_[dk_Now], stInput_.alponly_f_);
  }

  void TUGInput::ClearOutput()
  {
    if (stInput_.outw_ == 0) return;

    for (s32 i = 0; i < stInput_.outw_; ++i)
    {
      outs_[i]->GetField()->f_MakeFontMC(stInput_.spacecode_, FC2_input_word);
      outsword_[dk_None][i] = stInput_.spacecode_;
      outsword_[dk_Daku][i] = stInput_.spacecode_;
      outsword_[dk_Handaku][i] = stInput_.spacecode_;
      outsword_[dk_Now][i] = stInput_.spacecode_;
      outs_[i]->SetSelect(FALSE);
    }
    stInput_.cursor_ = 0;
    outs_[0]->SetSelect(TRUE);
  }

  //文字の入力
  void TUGInput::InputWord(s32 itemNo)
  {
    switch (keys_[itemNo]->btnword_[dk_None])
    {
    case ib_Space:
      outs_[stInput_.cursor_]->GetField()->f_MakeFontMC(stInput_.spacecode_, FC2_input_word);
      outsword_[dk_None][stInput_.cursor_] = stInput_.spacecode_;
      outsword_[dk_Daku][stInput_.cursor_] = stInput_.spacecode_;
      outsword_[dk_Handaku][stInput_.cursor_] = stInput_.spacecode_;
      outsword_[dk_Now][stInput_.cursor_] = stInput_.spacecode_;
      lib_num::AprToDest(&stInput_.cursor_, (stInput_.outw_ - 1));//カーソルの移動
      break;
    case ib_Back:
      // 今の位置にスペース入れておく
      outs_[stInput_.cursor_]->GetField()->f_MakeFontMC(stInput_.spacecode_, FC2_input_word);
      outsword_[dk_None][stInput_.cursor_] = stInput_.spacecode_;
      outsword_[dk_Daku][stInput_.cursor_] = stInput_.spacecode_;
      outsword_[dk_Handaku][stInput_.cursor_] = stInput_.spacecode_;
      outsword_[dk_Now][stInput_.cursor_] = stInput_.spacecode_;
      lib_num::AprTo0(&stInput_.cursor_);
      break;
    case ib_dakuh:
    case ib_dakuk:
    case ib_handh:
    case ib_handk:
    {
      enDakuon dakuon = dk_Daku;
      if ((keys_[itemNo]->btnword_[dk_None] == ib_handh)
        || (keys_[itemNo]->btnword_[dk_None] == ib_handk))
      {
        dakuon = dk_Handaku;
      }

      bool isCursorNext = false;
      s32 pos = stInput_.cursor_;
      //今のカーソル位置がスペース以外は一つ前を濁点
      if ((outsword_[dk_Now][pos] == stInput_.spacecode_) && (stInput_.cursor_ > 0))
      {
        pos = (stInput_.cursor_ - 1);
      }
      // いまカーソルが選んでいるところに濁点つけるなら終わったらカーソル進める
      else
      {
        isCursorNext = true;
      }
      outs_[pos]->GetField()->f_MakeFontMC(outsword_[dakuon][pos], FC2_input_word);
      outsword_[dk_Now][pos] = outsword_[dakuon][pos];
      // カーソル進める
      if (isCursorNext)
      {
        lib_num::AprToDest(&stInput_.cursor_, (stInput_.outw_ - 1));
      }
    }
    break;
    default:
    {
      outs_[stInput_.cursor_]->GetField()->f_MakeFontMC(keys_[itemNo]->btnword_[dk_None], FC2_input_word);
      outsword_[dk_None][stInput_.cursor_] = keys_[itemNo]->btnword_[dk_None];
      outsword_[dk_Daku][stInput_.cursor_] = keys_[itemNo]->btnword_[dk_Daku];
      outsword_[dk_Handaku][stInput_.cursor_] = keys_[itemNo]->btnword_[dk_Handaku];
      outsword_[dk_Now][stInput_.cursor_] = keys_[itemNo]->btnword_[dk_None];

      lib_num::AprToDest(&stInput_.cursor_, (stInput_.outw_ - 1));
    }
    break;
    }

    for (s32 i = 0; i < stInput_.outw_; ++i)
    {
      outs_[i]->SetSelect((i == stInput_.cursor_));
    }
  }

  void TUGInput::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    ugbtn_decide_.Update(draw_f);
    ugbtn_back_.Update(draw_f);
    inputfield_->Update(draw_f);
    outputfield_->Update(draw_f);
    outputfieldend_->Update(draw_f);

    //Addしてないのでここで全て更新する
    tabradio_.Update(draw_f);

    //タブ番号のあってるキーだけ表示
    for (s32 i = 0; i < stInput_.alllen_; ++i)
    {
      s32 itemno = (stInput_.tabno_* stInput_.alllen_) + i;
      keys_[itemno]->Update(draw_f);
    }

    for (s32 i = 0; i < stInput_.outw_; ++i)
    {
      outs_[i]->Update(draw_f);
    }

  }

  void TUGInput::DoAction()
  {

    BOOL act_f = FALSE;

    if (ugbtn_decide_.IsAct())
    {
      SetAct(enUGInputAct_OK);
      act_f = TRUE;
    }
    else if (ugbtn_back_.IsAct())
    {
      SetAct(enUGInputAct_Back);
      act_f = TRUE;
    }

    //ＯＫボタン
    //もどるボタン

    if (act_f == FALSE)
    {
      //キーの入力
      for (s32 i = 0; i < stInput_.alllen_; ++i)
      {
        s32 itemno = (stInput_.tabno_* stInput_.alllen_) + i;
        if (keys_[itemno]->IsAct())
        {
          InputWord(itemno);
          act_f = TRUE;
          break;
        }
      }
    }

    //出力先おされた
    if (act_f == FALSE)
    {
      for (s32 i = 0; i < stInput_.outw_; ++i)
      {
        if (outs_[i]->IsAct())
        {
          stInput_.cursor_ = i;
          for (s32 i = 0; i < stInput_.outw_; ++i)
          {
            outs_[i]->SetSelect((i == stInput_.cursor_));
          }
          act_f = TRUE;
          break;
        }
      }
    }


    if ((act_f == FALSE) && tabradio_.IsAct())
    {
      if (tabradio_.GetActIndex() != stInput_.tabno_)
      {
        stInput_.tabno_ = tabradio_.GetActIndex();
      }
      act_f = TRUE;
    }
  }

  BOOL TUGInput::IsActRes(enUGInputAct res)//返値ゲット
  {
    return (GetActRes() == res);
  }

}
