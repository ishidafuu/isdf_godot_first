#include "dbUGInputKey.h"
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



  const char HIRA[] = "あいうえおはひふへほっぁ戻かきくけこまみむめもゃぃゝさしすせそやゐゆゑよゅぅ心たちつてとらりるれろょぇ波なにぬねのわをん壱弐ーぉ空";
  const char HIRAD[] = "あい威えおばびぶべぼっぁ戻がぎぐげごまみむめもゃぃゞざじずぜぞやゐゆゑよゅぅ心だぢづでどらりるれろょぇ波なにぬねのわをん壱弐ーぉ空";
  const char HIRAH[] = "あいうえおぱぴぷぺぽっぁ戻かきくけこまみむめもゃぃゝさしすせそやゐゆゑよゅぅ心たちつてとらりるれろょぇ波なにぬねのわをん壱弐ーぉ空";
  const char KATA[] = "アイウエオハヒフヘホッァ戻カキクケコマミムメモャィヽサシスセソヤヰユヱヨュゥ心タチツテトラリルレロョェ波ナニヌネノワヲン参四ーォ空";
  const char KATAD[] = "アイヴエオバビブベボッァ戻ガギグゲゴマミムメモャィヾザジズゼゾヤヰユヱヨュゥ心ダヂヅデドラリルレロョェ波ナニヌネノワヲン参四ーォ空";
  const char KATAH[] = "アイウエオパピプペポッァ戻カキクケコマミムメモャィヽサシスセソヤヰユヱヨュゥ心タチツテトラリルレロョェ波ナニヌネノワヲン参四ーォ空";
  const char ALPA[] = "ＡＢＣＤＥａｂｃｄｅＺｚ戻ＦＧＨＩＪｆｇｈｉｊ１２３ＫＬＭＮＯｋｌｍｎｏ４５６ＰＱＲＳＴｐｑｒｓｔ７８９ＵＶＷＸＹｕｖｗｘｙ負０空";
  const char ALPA_S[] = "ＡＢＣＤＥ戻戻戻ＦＧＨＩＪ１２３ＫＬＭＮＯ４５６ＰＱＲＳＴ７８９ＵＶＷＸＹＺ０空";
  const char SP[] = "！？＆＃＠著剣心菱葉ⅡⅢ戻※同・黙波谷星音泥＞平上％゛゜、。ー山熊羊鸚＜左下右；：，．鋭重天／全大＋掛割「」（）引曲地＼積小負＝空";
  const char KARA[] = "空";
  const char BACK[] = "戻";
  const char DAKUH[] = "壱";
  const char HANDH[] = "弐";
  const char DAKUK[] = "参";
  const char HANDK[] = "四";



  //コンストラクタ
  TUGInputKey::TUGInputKey(TUIGenLayer* player) :TUGGen(player),
    tooltip_(player)
  {
    tooltip_f_ = FALSE;
  }
  TUGInputKey::~TUGInputKey()
  {
  }
  s32 TUGInputKey::GetWord(s32 nowno, s32 dakuon)
  {
    s32 nowwordno = (nowno % UGI_ALLLINELEN);
    char word[4];
    word[base::WordByte()] = '\0';

    const char* WDLIST;

    if (nowno < UGI_ALLLINELEN)
    {
      switch (dakuon)
      {
      case dk_Daku: WDLIST = HIRAD; break;
      case dk_Handaku:WDLIST = HIRAH; break;
      default: WDLIST = HIRA; break;
      }
    }
    else if (nowno < (UGI_ALLLINELEN * 2))
    {
      switch (dakuon)
      {
      case dk_Daku:WDLIST = KATAD; break;
      case dk_Handaku:WDLIST = KATAH; break;
      default: WDLIST = KATA; break;
      }
    }
    else if (nowno < (UGI_ALLLINELEN * 3))
    {
      WDLIST = ALPA;
    }
    else if (nowno < (UGI_ALLLINELEN * 4))
    {
      WDLIST = SP;
    }

    for (s32 i = 0; i < base::WordByte(); ++i) word[i] = WDLIST[nowwordno * base::WordByte() + i];

    //入れた文字を保存しておく
    s32 wordint = base::MojiCodeToInt(word[0], word[1], word[2]);

    if ((word[0] == KARA[0]) && (word[1] == KARA[1]) && (word[2] == KARA[2])) wordint = ib_Space;
    if ((word[0] == BACK[0]) && (word[1] == BACK[1]) && (word[2] == BACK[2])) wordint = ib_Back;
    if ((word[0] == DAKUH[0]) && (word[1] == DAKUH[1]) && (word[2] == DAKUH[2])) wordint = ib_dakuh;
    if ((word[0] == HANDH[0]) && (word[1] == HANDH[1]) && (word[2] == HANDH[2])) wordint = ib_handh;
    if ((word[0] == DAKUK[0]) && (word[1] == DAKUK[1]) && (word[2] == DAKUK[2])) wordint = ib_dakuk;
    if ((word[0] == HANDK[0]) && (word[1] == HANDK[1]) && (word[2] == HANDK[2])) wordint = ib_handk;

    return wordint;
  }

  //アルファベットオンリー
  s32 TUGInputKey::GetWordAlp(s32 nowno)
  {
    s32 nowwordno = (nowno % UGI_ALLLINELEN_S);
    char word[4];
    word[base::WordByte()] = '\0';

    for (s32 i = 0; i < base::WordByte(); ++i) word[i] = ALPA_S[nowwordno * base::WordByte() + i];

    //入れた文字を保存しておく
    s32 wordint = base::MojiCodeToInt(word[0], word[1], word[2]);

    if ((word[0] == KARA[0]) && (word[1] == KARA[1]) && (word[2] == KARA[2])) wordint = ib_Space;
    if ((word[0] == BACK[0]) && (word[1] == BACK[1]) && (word[2] == BACK[2])) wordint = ib_Back;


    return wordint;
  }

  BOOL TUGInputKey::IsDecide()
  {
    BOOL res = FALSE;
    if (field_ != NULL) res = field_->b_IsDecide();
    return res;
  }

  void TUGInputKey::MakeGroup(TUIObj* parent, s32 nowno, BOOL alponly_f)
  {
    //field_ = MakeObj();
    SetParent(parent);
    //field_->o_SetParent(parent);
    field_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_btn_key));
    field_->b_SetBtn(TRUE);
    field_->b_SetModal(TRUE);

    //入れた文字を保存しておく
    s32 mojicode = (alponly_f)
      ? GetWordAlp(nowno)
      : GetWord(nowno, dk_None);

    //だくおんとはんだくおんの文字コードを取得してベクタにいれる
    btnword_[dk_None] = mojicode;
    if (alponly_f == FALSE)
    {
      btnword_[dk_Daku] = (GetWord(nowno, dk_Daku));
      btnword_[dk_Handaku] = (GetWord(nowno, dk_Handaku));
    }


    field_->f_MakeFontMC(mojicode, FC2_input_word);
    field_->f_FontPos(7, 4);

    const s32 BTNW = 24;
    s32 posno = 0;
    s32 posx = 0;
    s32 posy = 0;
    s32 kugiri = 0;
    if (alponly_f)
    {
      posno = (nowno % UGI_ALLLINELEN_S);
      posx = ((posno  % UGI_LINELEN_S) * BTNW) + 2;
      posy = ((posno / UGI_LINELEN_S) * BTNW) + 4;
      kugiri = UGI_LINELEN_S;
    }
    else
    {
      posno = (nowno % UGI_ALLLINELEN);
      posx = ((posno  % UGI_LINELEN) * BTNW) + 2;
      posy = ((posno / UGI_LINELEN) * BTNW) + 4;
      kugiri = UGI_LINELEN;
    }



    if ((posno  % kugiri) < 5) posx += 0;
    else if ((posno  % kugiri) < 10) posx += 2;
    else posx += 4;
    field_->z_SetPos(posx, posy);


    if (mojicode < ibEND)
    {
      field_->g_SetPartNo(mojicode);
      tooltip_f_ = FALSE;
    }
    else
    {
      tooltip_f_ = TRUE;
      tooltip_.MakeGroup(field_);
      tooltip_.SetFontMC(mojicode);
    }
  }

  void TUGInputKey::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);

    if (tooltip_f_)
    {
      if (field_->b_IsEnter(TRUE)) tooltip_.Update(draw_f);
    }
  }

  void TUGInputKey::DoAction()
  {
    //決定
    if (field_->b_IsDecide())
    {
      SetAct(0);//値はinput側で管理
    }
  }


}
