#include "dbUGTutoMsgGousei.h"


namespace db
{
  const s32 LINENUM = 2;

  STRVECTOR MS_PREVAINTRO =
  {
    "ここでは＿てにいれた＿そざいだまを", "ミックスし＿オリジナルしゅーとを＿つくれます",
    "ためしに＿ミックスで＿つくられた", "オリジナルしゅーとを＿みてみましょう",
  };

  STRVECTOR MS_PREVA =
  {
    "このしゅーとは「サイドにげ１だま」に", "「なかばピシピタッだま」をミックスしました",
  };

  STRVECTOR MS_PREVBINTRO =
  {
    "つぎは＿さきほどの＿しゅーとの", "ミックスを＿ぎゃくのじゅんばんに＿してみます",
  };

  STRVECTOR MS_PREVB =
  {
    "このしゅーとは「なかばピシピタッだま」に", "「サイドにげ１だま」をミックスしました",
  };

  STRVECTOR MS_PREVRINTRO =
  {
    "このように＿まぜるじゅんばんが＿かわると", "ミックスした＿けっかも＿かわります",
    "いろいろな＿ミックスを＿みてみましょう", "おきにいりの＿しゅーとを＿みつけてね",
  };

  STRVECTOR MS_PREVR =
  {
    "いろいろな＿ミックスを＿みてみましょう", "おきにいりの＿しゅーとを＿みつけてね",
  };

  STRVECTOR MS_PREVRYN =
  {
    "このしゅーとで＿いい？＿おーけー！＿なら", "このしゅーとの＿そざいだまを＿プレゼント！",
  };

  STRVECTOR MS_PREVRRESULT =
  {
    "さきほどの＿オリジナルしゅーとは", "この＿そざいだまから＿つくれます",
  };

  STRVECTOR MS_TUTOEND =
  {
    "まったくおなじ＿しゅーとを＿つくりたければ", "プレゼントされたじゅんばんで＿ミックスしてね",
    "レアリティがたかい＿そざいだまは＿たくさん", "ミックスができて＿のうりょくも＿たかいよ",
    "ミックスにつかえるのは＿そざい「だま」だけ", "ミックスずみの＿しゅーとは＿まぜられないよ",
  };

  //コンストラクタ
  TUGTutoMsgGousei::TUGTutoMsgGousei(TUIGenLayer* player) :TUGTutoMsgGen(player)
  {
    stTutoMsg_.Init();
  }

  TUGTutoMsgGousei::~TUGTutoMsgGousei()
  {

  }

  //void TUGTutoMsgGousei::MakeGroup(TUIObj* parent)
  //{
  //  //GenMakeGroup(parent);
  //  //ugserifu_.SetDepth(UIDP_09MSGBOX);
  //}

  void TUGTutoMsgGousei::CheckSkipMsg(void)
  {
    // メッセージスキップ処理
    if (field_->sc_IsCamEnter())
    {
      if (ugserifu_.IsReadLineEnd())
      {
        ugserifu_.NextSerifu();
      }
      else
      {
        ugserifu_.SkipSerifu();
      }
    }
  }

  BOOL TUGTutoMsgGousei::DoNextMsg()
  {
    BOOL res = 0;
    switch (stTutoMsgGen_.msgtype_)
    {
    case 0: res = GenSetMsg(&MS_PREVAINTRO); break;
    case 1: res = GenSetMsg(&MS_PREVA); break;
    case 2: res = GenSetMsg(&MS_PREVBINTRO); break;
    case 3: res = GenSetMsg(&MS_PREVB); break;
    case 4: res = GenSetMsg(&MS_PREVRINTRO); break;
    case 5: res = GenSetMsg(&MS_PREVR); break;
    case 6: res = GenSetMsg(&MS_PREVRYN); break;
    case 7: res = GenSetMsg(&MS_PREVRRESULT); break;
    case 8: res = GenSetMsg(&MS_TUTOEND); break;
    default: break;
    }

    // ここでは
    if (stTutoMsgGen_.msgtype_ == 1 || stTutoMsgGen_.msgtype_ == 3 || stTutoMsgGen_.msgtype_ == 6)
    {
      ugserifu_.SetMsgChangeMode(enMsgChangeMode_Wait2);
    }
    else
    {
      ugserifu_.SetMsgChangeMode(enMsgChangeMode_Touch);
    }

    //オーバーしたときTrue
    return res;
  }

}
