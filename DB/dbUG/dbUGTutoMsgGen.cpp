#include "dbUGTutoMsgGen.h"


namespace db
{
  const s32 LINENUM = 2;

  //コンストラクタ
  TUGTutoMsgGen::TUGTutoMsgGen(TUIGenLayer* player) :TUGGen(player)
    , ugserifu_(player)
  {
    stTutoMsgGen_.Init();
  }

  TUGTutoMsgGen::~TUGTutoMsgGen()
  {

  }
  void TUGTutoMsgGen::Ready()
  {
    stTutoMsgGen_.Init();
  }

  void TUGTutoMsgGen::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);
    ugserifu_.MakeGroup(field_, LINENUM, enSerifuFrame_Wide);
    ugserifu_.SetEndWait(120);
    ugserifu_.SetDepth(UIDP_02BGOBJHI);
    ugserifu_.SetPos(-96, -80);
  }


  BOOL TUGTutoMsgGen::GenSetMsg(STRVECTOR* msg)
  {
    BOOL res = FALSE;
    s32 idx = (stTutoMsgGen_.msgno_ * LINENUM);

    if (idx < msg->size())
    {
      serifuvec_.clear();
      serifuvec_.push_back(msg->at(idx + 0));
      serifuvec_.push_back(msg->at(idx + 1));
      ugserifu_.SetSerifu(serifuvec_);
    }
    else
    {
      //オーバー
      res = TRUE;
    }

    return res;
  }
  void TUGTutoMsgGen::SetMsgClear(s32 msgtype, BOOL ok_f)
  {
    stTutoMsgGen_.msgtype_ = msgtype;
    stTutoMsgGen_.msgno_ = (ok_f)
      ? 0//ＯＫの時は１番から
      : -1;//０番にはＮＧから来たときのメッセージを入れる。
    NextMsg();//一つ数値が上がる
  }
  void TUGTutoMsgGen::SetMsg(s32 msgtype, s32 msgno)
  {
    stTutoMsgGen_.msgtype_ = msgtype;
    stTutoMsgGen_.msgno_ = (msgno - 1);//一つ数値が上がるので下げとく
    NextMsg();
  }
  //ＯＫＮＧなく単純に一つ進める
  void TUGTutoMsgGen::IncMsg()
  {
    stTutoMsgGen_.msgtype_ += 1;
    stTutoMsgGen_.msgno_ = -1;//一つ数値が上がるので下げとく
    NextMsg();
  }
  //ＯＫＮＧなく最初の一つ
  void TUGTutoMsgGen::FirstMsg()
  {
    Ready();
    stTutoMsgGen_.msgtype_ = 0;
    stTutoMsgGen_.msgno_ = -1;//一つ数値が上がるので下げとく
    NextMsg();
  }
  //くりかえし
  void TUGTutoMsgGen::SameMsg()
  {
    //stTutoMsgGen_.msgtype_ += 1;
    stTutoMsgGen_.msgno_ = -1;//一つ数値が上がるので下げとく
    NextMsg();
  }
  BOOL TUGTutoMsgGen::NextMsg()
  {
    BOOL res = FALSE;

    ++stTutoMsgGen_.msgno_;
    res = DoNextMsg();
    //オーバーしたときTrue
    return res;
  }
  void TUGTutoMsgGen::ChangeFrame(BOOL wide_f)
  {
    if (wide_f)
    {
      ugserifu_.ChangeFrame(enSerifuFrame_Wide);
      ugserifu_.SetPos(-96, -80);
    }
    else
    {
      ugserifu_.ChangeFrame(enSerifuFrame_Normal);
      ugserifu_.SetPos(-160, -80);
    }
  }

  s32 TUGTutoMsgGen::GetMsgNo()
  {
    return stTutoMsgGen_.msgno_;
  }
  void TUGTutoMsgGen::SetSerifuEndWait(s32 endwait)
  {
    ugserifu_.SetEndWait(endwait);
  }


  void TUGTutoMsgGen::CheckSkipMsg(void)
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

  void TUGTutoMsgGen::DoUpdate(BOOL draw_f)
  {
    //進行
    ugserifu_.Update(draw_f);
  }

  void TUGTutoMsgGen::Mv_Default()//入店時
  {
    if (ugserifu_.IsActRes(enUGSerifuAct_ReadEnd))//セリフ読み終わり
    {

    }
    else if (ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))//セリフウエイト
    {
      SetAct(0);//アクション
    }
  }

  void TUGTutoMsgGen::DoAction()
  {
    //メッセージタイプごとの店員と監督の挙動
    Mv_Default();
  }
}
