﻿#include "dbUGTutoMsgSmart.h"

namespace db
{
  STRVECTOR2 MS_SMART =
  {
    // tmret_Intro,
    {
      "はせべ「スマートモードによる", "そうさをせつめいします",
      "はせべ「かたてで＿そうさできるので", "うまく＿つかいわけてね",
    },
    {//tmret_Dodge,
      "はせべ「まずは＿ボールの＿よけかたです", "",
      "はせべ「がめん＿うえのほうを", "おしっぱなしに＿してね",
    },
    {//tmret_DodgeSub,
      //  rta_dodge_ok,
      //  rta_dodge_ngpos,
      //  rta_dodge_ngearly,
      //  rta_dodge_nglate,
      "はせべ「いいですね心", "",
      "はせべ「もうすこし＿うえのほうを", "タッチしてね",
      "はせべ「よけきるまで＿ゆびを＿はなさないでね", "",
      "はせべ「ボールに＿あたるまえに＿おしてね", "よけきるまで＿おしっぱなしだよ",
    },
    {//tmret_DodgeEnd,
      "はせべ「まぁ＿いいでしょう＿よけるなんて", "ねっけつこうはには＿にあわないわ！",
      "はせべ「これで＿よけは＿おぼえましたね？", "",
    },
    {//tmret_Walk,
      "はせべ「いどうは＿がめんしたのほうを", "グリグリします",
    },
    {//tmret_WalkSub,
      //  rta_walk_ok,
      //  rta_walk_ngpos,
      "はせべ「いいですね心", "",
      "はせべ「がめんしたのほうを", "グリグリしてね",
    },
    {//tmret_WalkEnd,
      "はせべ「まぁ＿いいでしょう", "どっしりかまえていれば＿だいじょうぶ！",
      "はせべ「いどうも＿おぼえましたか？", "",
    },
    {//tmsmt_Jump,
      "はせべ「つぎは＿ジャンプしてみましょう", "",
      "はせべ「がめん＿したのほうを", "おしっぱなしにしてね",
      "はせべ「ジャンプは＿うえにはやく", "はじくように＿うごかしてもＯＫ！",
    },
    {//tmsmt_JumpSub,
      //  rta_jump_ok,
      //  rta_jump_ngpos,
      //  rta_jump_ngdash,
      "はせべ「いいですね心", "",
      "はせべ「がめん＿したのほうを", "おしっぱなしにしてね",
      "はせべ「がめん＿したのほうを", "おしっぱなしにしてね",
    },
    { //tmsmt_JumpEnd,
      "はせべ「まぁ＿いいでしょう", "ひつようなときに＿とべばいいの心",
      "はせべ「ジャンプも＿おぼえましたか？", "",
    },

    {//tmret_Catch,
      "はせべ「つぎは「べんりそうさ」です", "キャッチ・ダッシュ・シュートは＿これでＯＫ心",
      "はせべ「がめんしたを＿タッチすると", "さいてきな＿そうさが＿えらばれるよ",
      "はせべ「まずは＿キャッチしてみましょう", "タイミングよく＿がめんしたを＿おしてね",
    },
    {//tmret_CatchSub,
      //rta_catch_ok,
      //rta_catch_ngpos,
      //rta_catch_ngearly,
      //rta_catch_nglate,
      "はせべ「いいですね心", "",
      "はせべ「キャッチボタンを＿おしてね", "",
      "はせべ「タッチの＿タイミングが＿はやいみたい", "つぎは＿すこし＿ひきつけて＿タッチしてね",
      "はせべ「タッチの＿タイミングが＿おそいみたい", "つぎは＿ボールに＿あたるまえに＿タッチしてね",
    },
    {//tmret_CatchEnd,
      "はせべ「まぁ＿いいでしょう", "おとこなら＿からだでとめれば＿だいじょうぶ！",
      "はせべ「キャッチも＿おぼえましたか？", "",
    },
    {//tmret_Walk,
      "はせべ「つぎは＿ダッシュしてみましょう", "ボールをもったら＿がめんしたを＿タッチ",
      "はせべ「ダッシュの＿ほうこうは", "いいかんじに＿えらばれるよ",
      "はせべ「ゆびを＿さゆうに", "はやく＿うごかすと＿ダッシュだよ",
    },
    {//tmsmt_DashSub,
      //  rta_dash_ok,
      //  rta_dash_ngpos,
      //  rta_dash_ngwalk,
      //  rta_dash_ngjump,
      "はせべ「いいですね心", "",
      "はせべ「がめんの＿したのほうを", "タッチしてね",
      "はせべ「もっと＿すばやく＿フリックしてね", "",
      "はせべ「おしっぱなしは＿ジャンプだよ", "もうすこし＿はやく＿はなしてね",
    },
    {//tmsmt_DashEnd,
      "はせべ「まぁ＿いいでしょう", "こころに＿ゆとりを＿かんじるわ心",
      "はせべ「ダッシュも＿おぼえましたか？", "",
    },
    {//tmret_Shot,
      "はせべ「つぎは＿しゅーとを", "なげてみましょう",
      "はせべ「ボールを＿もったら", "がめんしたを＿タッチ",
      "はせべ「ダッシュを＿はじめたら", "もういちど＿タッチ",
    },
    {//tmret_ShotSub,
      //  rta_shot_ok,
      //  rta_shot_ngpos,
      //  rta_shot_ngovline,
      "はせべ「いいですね心", "ボールが＿ひかるタイミングを＿ねらおう",
      "はせべ「がめんしたを＿おしてね", "",
      "はせべ「ラインオーバーしたら", "あいてボールになるから＿きをつけてね",
      "はせべ「ダッシュジャンプでも", "ひっさつしゅーとが＿なげられるよ",
    },
    { //tmsmt_ShotEnd,
      "はせべ「まぁ＿いいでしょう", "やさしいのね心",
      "はせべ「シュートも＿おぼえましたか？", "",
    },
    {//tmret_Game,
      "はせべ「パスは＿じっせんの＿なかで", "おぼえて＿みましょう",
      "はせべ「がめんうえを＿おすと", "パスができるよ",
      "はせべ「ながくおすと＿たかいパス", "みじかくおすと＿ひくいパス",
      "はせべ「パスしたいほうに＿ゆびを", "すべらせれば＿むきしていできるよ",
    },
  };


  //STRVECTOR2 MS_SMART =
  //{
  //  { //tmsmt_Intro,
  //    "あなたが＿ドッジボールの＿かんとくさんね？", "わたしは＿れいほうのすけばん＿はせべです",
  //    "かんとくとして＿チームを＿しきするために", "まずは＿そうさを＿おぼえましょう心",
  //  },
  //  {//tmsmt_Dodge,
  //    "まずは＿ボールの＿よけかたです", "",
  //    "がめん＿うえのほうを＿おしっぱなしにします", "２きゅう＿よけて＿みましょう",
  //  },
  //  { //tmsmt_DodgeSub,
  //    //  rta_dodge_ok,
  //    //  rta_dodge_ngpos,
  //    //  rta_dodge_ngearly,
  //    //  rta_dodge_nglate,
  //    "いいですね心", "",
  //    "もうすこし＿うえのほうを＿タッチしてね", "",
  //    "よけきるまで＿ゆびを＿はなさないでね", "",
  //    "ボールに＿あたるまえに＿タッチしてね", "がめん＿うえのほうを＿おしっぱなしだよ",
  //  },
  //  {//tmsmt_Walk,
  //    "まぁ＿いいでしょう＿ボールを＿よけるなんて", "ねっけつこうはには＿にあわないわ！",
  //    "つぎは＿コートを＿あるいてみましょう", "がめん＿したのほうを＿グリグリしてみてね",
  //  },
  //  {//tmsmt_WalkSub,
  //    //  rta_walk_ok,
  //    //  rta_walk_ngpos,
  //    "いいですね心", "",
  //    "そのボタンは＿ちがうよ", "",
  //  },
  //  {//tmsmt_Jump,
  //    "まぁ＿いいでしょう", "どっしりかまえていれば＿だいじょうぶ！",
  //    "つぎは＿ジャンプしてみましょう", "",
  //    "がめん＿したのほうを＿おしっぱなしにしてね", "２かい＿ジャンプしてみましょう",
  //  },
  //  {//tmsmt_JumpSub,
  //    //  rta_jump_ok,
  //    //  rta_jump_ngpos,
  //    //  rta_jump_ngdash,
  //    "いいですね心", "",
  //    "がめん＿したのほうを＿おしてね", "",
  //    "もうすこし＿ながく＿がめんを", "おしっぱなしにしてね",
  //  },
  //  {//tmsmt_Catch,
  //    "まぁ＿いいでしょう", "じょうずなひとは＿やみくもに＿とばないわ！",
  //    "つぎは「べんりそうさ」をおぼえましょう", "キャッチ・ダッシュ・シュートは＿これでＯＫ心",
  //    "「べんりそうさ」は＿がめんしたを＿タッチ", "さいてきな＿そうさが＿えらばれるよ",
  //    "まずは＿あいてが＿しゅーとを＿なげてきます", "２きゅう＿キャッチして＿みましょう",
  //    "ボールを＿よーくひきつけて＿タイミングよく", "がめんのしたを＿タッチしてね",
  //  },
  //  {//tmsmt_CatchSub,
  //    //  rta_catch_ok,
  //    //  rta_catch_ngpos,
  //    //  rta_catch_ngearly,
  //    //  rta_catch_nglate,
  //    "いいですね心", "",
  //    "もうすこし＿したのほうを＿タッチしてね", "",
  //    "タッチの＿タイミングが＿はやいみたい", "つぎは＿すこし＿ひきつけて＿タッチしてね",
  //    "タッチの＿タイミングが＿おそいみたい", "つぎは＿ボールに＿あたるまえに＿タッチしてね",
  //    "スワイプに＿なってるよ＿ゆびは＿すべらせず", "ビタッと＿タッチしてね",
  //  },
  //  {//tmsmt_Dash,
  //    "まぁ＿いいでしょう", "おとこなら＿からだでとめれば＿だいじょうぶ！",
  //    "つぎは＿ダッシュしてみましょう", "ボールを＿もったら＿がめんしたを＿タッチ",
  //    "「べんりそうさ」では＿ダッシュのほうこうが", "いいかんじに＿えらばれるよ",
  //  },
  //  {//tmsmt_DashSub,
  //    //  rta_dash_ok,
  //    //  rta_dash_ngpos,
  //    //  rta_dash_ngwalk,
  //    //  rta_dash_ngjump,
  //    "いいですね心", "",
  //    "がめんの＿したのほうを＿フリックしてね", "",
  //    "もっと＿すばやく＿フリックしてね", "",
  //    "タッチおしっぱなしだと＿ジャンプになるよ", "もうすこし＿はやく＿はなしてね",
  //  },
  //  { //tmsmt_Shot,
  //    "まぁ＿いいでしょう", "おとこは＿どしっと＿かまえていればいいわ心",
  //    "つぎは＿しゅーとを＿してみましょう", "",
  //    "ボールを＿もったら＿がめんしたを＿タッチ", "ダッシュを＿はじめたら＿もういちど＿タッチ",
  //    "ダッシュ４ほめは＿ひっさつしゅーとになるよ", "ボールが＿ひかるタイミングを＿ねらおう！",
  //  },
  //  { //tmsmt_ShotSub,
  //    //  rta_shot_ok,
  //    //  rta_shot_ngpos,
  //    //  rta_shot_ngovline,
  //    "いいですね心", "ひっさつしゅーとも＿ねらってみよう",
  //    "もうすこし＿したのほうを＿タッチしてね", "",
  //    "ダッシュちゅうに＿タッチしてね", "ラインオーバーしたら＿あいてボールになるよ",
  //  },
  //  { //tmsmt_Game,
  //    "まあいいでしょう", "",
  //    "がめんを＿フリックや＿スワイプすることで", "より＿こまかいそうさが＿できます",
  //    "では＿じっせんで＿ためしてみましょう", "そうさの＿アドバイスも＿していくね",
  //  },
  //  {//tmsmt_Adv,
  //    "がめんうえ＿ながおしで＿よけ", "（しゅびのとき）",
  //    "がめんした＿グリグリで＿あるき", "",
  //    "がめんした＿ながおしで＿ジャンプ", "",
  //    "がめんした＿うえに＿フリックでも＿ジャンプ", "",
  //    "がめんした＿タッチで＿キャッチ", "（しゅびのとき）",
  //    "がめんした＿タッチで＿ダッシュ", "（ボールもったとき）",
  //    "がめんした＿みぎ・ひだりに＿フリックで", "そのほうこうに＿ダッシュ（ブレーキ）",
  //    "ダッシュちゅう＿がめんした＿タッチで", "シュート（ボールもったとき）",
  //    "がめんうえ＿じょうげさゆう＿フリックで", "ひくいパス（ボールもったとき）",
  //    "がめんうえ＿じょうげさゆう＿スワイプで", "たかいパス（ボールもったとき）",
  //  },
  //};


  //コンストラクタ
  TUGTutoMsgSmart::TUGTutoMsgSmart(TUIGenLayer* player) :TUGTutoMsgGen(player)
  {
  }

  TUGTutoMsgSmart::~TUGTutoMsgSmart()
  {
  }
  BOOL TUGTutoMsgSmart::DoNextMsg()
  {
    BOOL res = GenSetMsg(&MS_SMART[stTutoMsgGen_.msgtype_]);
    //オーバーしたときTrue
    return res;
  }
}
