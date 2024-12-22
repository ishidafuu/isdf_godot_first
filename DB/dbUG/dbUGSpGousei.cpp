#include "dbUGSpGousei.h"


namespace db
{

  enum enItemDraw
  {
    itd_None,
    itd_Carry,
    itd_Table,
  };


  //コンストラクタ
  TUGSpGousei::TUGSpGousei(TUIGenLayer* player) :TUGGen(player), ugserifu_(player)
  {
    stGousei_.Init();
  }

  TUGSpGousei::~TUGSpGousei()
  {

  }

  void TUGSpGousei::MakeGroup(TUIObj* parent, TUGChKantoku* ugkantoku, TUGChCaptain* ugcap)
  {
    SetParent(parent);
    ugkantoku_ = ugkantoku;
    ugcap_ = ugcap;
    ugserifu_.MakeGroup(field_, 2, enSerifuFrame_Normal, UIDP_03OBJ);//プレビューに重なってしまうので低く
  }

  //入店する前に店員と番号をセット
  void TUGSpGousei::Ready(TUGDock* ugdock, s32 dockno)
  {
    ugdock_ = ugdock;
    ugdock_->ChangeGousei(TRUE);

    //親の交換大丈夫か？
    ugkantoku_->SetParent(ugdock_->GetField());
    ugkantoku_->SetPos(56, 40);
    ugkantoku_->GetField()->g_SetLimX(0, 0);
	ugkantoku_->SetMirror(FALSE);
    ugcap_->SetParent(ugdock_->GetField());
    ugcap_->SetWalkType(cwt_MYWALK);
    ugcap_->SetPos(30, 40);
    ugcap_->SetMType(cmt_St);
    ugcap_->GetField()->g_SetLimX(0, 0);
    //ugkantoku_->SetDraw(FALSE);
    //セリフ位置
    ugserifu_.SetParent(ugdock_->GetField());
    switch (dockno)
    {
    case 0: ugserifu_.SetPos(8, 40 + 80); break;
    case 1: ugserifu_.SetPos(8, 40 + 40); break;
    case 2: ugserifu_.SetPos(8, -80); break;
    case 3: ugserifu_.SetPos(8, -120); break;
    }

    SetGouseiMsg(enGouseiMsg_Visit);
  }
  void TUGSpGousei::Down()
  {
    ugdock_->ChangeGousei(FALSE);
    SetAct(0);//退出アクション
  }

  void TUGSpGousei::GenSetGouseiMsg(const char* text0)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    ugdock_->ugclerk_.SetMType(cmt_Talk);
    ugserifu_.SetSerifu(serifuvec_);
  }
  void TUGSpGousei::GenSetGouseiMsg(const char* text0, const char* text1)
  {
    serifuvec_.clear();
    serifuvec_.push_back(text0);
    serifuvec_.push_back(text1);
    ugdock_->ugclerk_.SetMType(cmt_Talk);
    ugserifu_.SetSerifu(serifuvec_);
  }
  void TUGSpGousei::SetStopMsg()
  {
    ugdock_->ugclerk_.SetMType(cmt_Stand);
  }

  void TUGSpGousei::SetGouseiMsg(enGouseiMsg msgtype)
  {
    stGousei_.mtype_ = msgtype;
    SetSerifuDraw(TRUE);
    //メッセージタイプごとの店員と監督の挙動
    switch (stGousei_.mtype_)
    {
    case enGouseiMsg_Visit: //入店時
      ugdock_->ugdoor_.OpenCloseDoor();//扉開ける
      GenSetGouseiMsg("掛「いらっしゃいませ＿こんにちは");
      break;
    case enGouseiMsg_Base: //ベース待ち
      GenSetGouseiMsg("掛「まずは＿ミックスする", "ベースしゅーとを＿えらんでください");
      break;
    case enGouseiMsg_Sozai: //素材待ち
      GenSetGouseiMsg("掛「つぎに＿ミックスする", "そざいしゅーとを＿えらんでください");
      break;
    case enGouseiMsg_OK: //ＯＫ待ち
      GenSetGouseiMsg("掛「こちらで＿よろしいですか？");
      break;
    case enGouseiMsg_Out: //退出時
      ugdock_->ugdoor_.OpenCloseDoor();//扉開ける
      GenSetGouseiMsg("掛「ありがとうございました心");
      break;
    case enGouseiMsg_Out_Mix://退出時
      ugdock_->ugdoor_.OpenCloseDoor();//扉開ける
      GenSetGouseiMsg("掛「ミックスを＿はじめました", "しばらくのあいだ＿おまちください心");
      break;
    case enGouseiMsg_Out_NG://退出時
      ugdock_->ugdoor_.OpenCloseDoor();//扉開ける
      GenSetGouseiMsg("掛「ミックスを＿とりやめました", "ベース・そざい＿は＿おかえしします");
      break;
    case enGouseiMsg_Out_OK://退出時
      ugdock_->ugdoor_.OpenCloseDoor();//扉開ける
      GenSetGouseiMsg("掛「ミックスを＿かくていしました", "また＿ごりようください心");
      break;
    case enGouseiMsg_Get: //ＯＫ待ち
      GenSetGouseiMsg("掛「おまたせしました＿ミックスを", "かくていしても＿よろしいですか？");
      break;
    }
  }
  //マスターＩＤではなく、自身のシュートＩＤの必要がある
  void TUGSpGousei::SetBaseID(s32 id_shot_base)
  {
    stGousei_.id_shot_base_ = id_shot_base;
    SetGouseiMsg(enGouseiMsg_Sozai);//セリフも一緒に
  }
  void TUGSpGousei::SetAddID(s32 id_shot_add)
  {
    stGousei_.id_shot_add_ = id_shot_add;
    SetGouseiMsg(enGouseiMsg_OK);//セリフも一緒に
  }
  void TUGSpGousei::SetSerifuDraw(BOOL draw_f)
  {
    ugserifu_.SetDraw(draw_f);
  }

  void TUGSpGousei::DoUpdate(BOOL draw_f)
  {
    //進行
    field_->Update(draw_f);
    //inside_->Update(draw_f);

    ugdock_->Update(draw_f);//末端で管理する
    ugkantoku_->Update(draw_f);
    ugcap_->Update(draw_f);
    ugserifu_.Update(draw_f);

    //base_->Update(draw_f);
    //sozai_->Update(draw_f);
  }

  void TUGSpGousei::Mv_Visit()//入店時
  {
    if (ugdock_->ugdoor_.IsActRes(enUGDoorAct_Half))
    {
      //ugkantoku_->GetField()->c_SetDraw(TRUE);
      ugkantoku_->SetDraw(TRUE);
      ugcap_->SetDraw(TRUE);
    }

    if (ugserifu_.IsActRes(enUGSerifuAct_ReadEnd))
    {
      ugdock_->ugclerk_.SetMType(cmt_Stand);
    }

  }
  void TUGSpGousei::Mv_Base()
  {
    if (ugserifu_.IsActRes(enUGSerifuAct_ReadEnd))
    {
      ugdock_->ugclerk_.SetMType(cmt_Stand);
    }

  }
  void TUGSpGousei::Mv_Sozai()
  {
    if (ugserifu_.IsActRes(enUGSerifuAct_ReadEnd))
    {
      ugdock_->ugclerk_.SetMType(cmt_Stand);
    }
  }
  void TUGSpGousei::Mv_OK()
  {
    if (ugserifu_.IsActRes(enUGSerifuAct_ReadEnd))
    {
      ugdock_->ugclerk_.SetMType(cmt_Stand);
    }
  }
  void TUGSpGousei::Mv_Out()//退出時
  {
    if (ugdock_->ugdoor_.IsActRes(enUGDoorAct_Half))
    {
      ugkantoku_->SetDraw(FALSE);
      ugcap_->SetDraw(FALSE);
    }
    if (ugserifu_.IsActRes(enUGSerifuAct_ReadEnd))
    {
      ugdock_->ugclerk_.SetMType(cmt_Stand);
    }
    if (ugserifu_.IsActRes(enUGSerifuAct_WaitEnd))
    {
      Down();
    }
  }
  void TUGSpGousei::Mv_Order()//初注文
  {

  }

  void TUGSpGousei::DoAction()
  {
    //メッセージタイプごとの店員と監督の挙動
    switch (stGousei_.mtype_)
    {
    case enGouseiMsg_Visit: Mv_Visit(); break;//入店時
    case enGouseiMsg_Base: Mv_Base(); break;
    case enGouseiMsg_Sozai: Mv_Sozai(); break;
    case enGouseiMsg_OK: Mv_OK(); break;
    case enGouseiMsg_Out:
    case enGouseiMsg_Out_Mix:
    case enGouseiMsg_Out_NG:
    case enGouseiMsg_Out_OK:
      Mv_Out();
      break;//退出時
    case enGouseiMsg_Order: Mv_Order(); break;//初注文
    }

    // メッセージスキップ処理
    if (field_->sc_IsCamEnter())
    {
      if (ugserifu_.IsReadLineEnd())
      {
        ugserifu_.NextSerifu();
        ugdock_->ugclerk_.SetMType(cmt_Stand);
      }
      else
      {
        ugserifu_.SkipSerifu();
        ugdock_->ugclerk_.SetMType(cmt_Stand);
      }
    }
  }


}
