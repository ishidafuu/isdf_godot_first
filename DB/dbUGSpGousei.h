#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGSerifu.h"
#include "dbUGDock.h"
#include "dbUGChKantoku.h"
#include "dbUGChCaptain.h"

namespace db
{
  //ショップメッセージ
  enum enGouseiMsg
  {
    enGouseiMsg_Visit,//入店時
    enGouseiMsg_Base,//ベース待ち
    enGouseiMsg_Sozai,//素材待ち
    enGouseiMsg_OK,//確認待ち
    enGouseiMsg_Out,//退出時
    enGouseiMsg_Out_Mix,//退出時
    enGouseiMsg_Out_NG,//退出時
    enGouseiMsg_Out_OK,//退出時
    enGouseiMsg_Order,//初注文
    enGouseiMsg_Get,
    enGouseiMsgEND,
  };

  struct TUGSpGouseiSt
  {
    s32 id_shot_base_;
    s32 id_shot_add_;

    enGouseiMsg mtype_;//店員メッセージタイプ
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGSpGousei : public TUGGen
  {
  protected:
    TUGDock* ugdock_;//ポインタ
    TUGChKantoku* ugkantoku_;//ポインタ
	TUGChCaptain* ugcap_;//ポインタ
    STRVECTOR serifuvec_;

    void Mv_Visit();//入店時
    void Mv_Base();
    void Mv_Sozai();
    void Mv_OK();
    void Mv_Out();//退出時
    void Mv_Order();//初注文

    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGSpGouseiSt stGousei_;
    TUGSerifu ugserifu_;//セリフ

    //コンストラクタ
    explicit TUGSpGousei(TUIGenLayer* player);
    virtual ~TUGSpGousei(void);
    void MakeGroup(TUIObj* parent, TUGChKantoku* ugkantoku, TUGChCaptain* ugcap);
    void Ready(TUGDock* ugdock, s32 dockno);
    void Down();

    void GenSetGouseiMsg(const char* text0);
    void GenSetGouseiMsg(const char* text0, const char* text1);
    void SetStopMsg();

    void SetGouseiMsg(enGouseiMsg msgtype);
    void SetBaseID(s32 id_shot_base);
    void SetAddID(s32 id_shot_add);
    void SetSerifuDraw(BOOL draw_f);
  };
 
}
