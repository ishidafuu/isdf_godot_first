#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGSerifu.h"
#include "dbUGDoor.h"
#include "dbUGChKantoku.h"
#include "dbUGChCaptain.h"
#include "dbUGChClerk.h"

namespace db
{
  //ショップメッセージ
  enum enBBAMsg
  {
    enBBAMsg_Visit,//入店時
    enBBAMsg_Menu,//メニュー
    enBBAMsg_ShotInv,//シュートインベントリ
    enBBAMsg_Bara,//合成分解
    enBBAMsg_SellMenu,//シュート売却
    enBBAMsg_Sell,//シュート売却
    enBBAMsg_BuyMenu,//シュート売却
    enBBAMsg_Buy,//シュート売却
    enBBAMsg_OK,//確認待ち
    enBBAMsg_Thanks,//ありがとう
    enBBAMsg_Out,//退出時
    enBBAMsg_Order,//初注文
    enBBAMsg_NoneBara,//ばらせるシュートがない
    enBBAMsg_NoneEmp,//シュートあきがない
    enBBAMsg_NoneSell,//うれるシュートがない
    enBBAMsg_InvMax,//インベントリ限界
    enBBAMsgEND,
  };


  struct TUGBBASt
  {


    enBBAMsg mtype_;//店員メッセージタイプ
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGSpBBA : public TUGGen
  {
  protected:
    TUGDoor* ugdoor_;//ばばあまどポインタ
    TUGChKantoku* ugkantoku_;//監督ポインタ
	TUGChCaptain* ugcap_;//キャプテンポインタ
    TUGChClerk* ugclbba_;//ばばあ
    STRVECTOR serifuvec_;

    //void Mv_Visit();//入店時
    //void Mv_Menu();
    //void Mv_ShotInv();
    //void Mv_Bara();
    //void Mv_Sell();
    //void Mv_OK();
    void Mv_Out();//退出時
    void Mv_Default();//初注文

    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGBBASt stBBA_;
    TUGSerifu ugserifu_;//セリフ

    //コンストラクタ
    explicit TUGSpBBA(TUIGenLayer* player);
    virtual ~TUGSpBBA(void);
    void MakeGroup(TUIObj* parent, TUGChKantoku* ugkantoku, TUGChCaptain* ugcap, TUGChClerk* ugclbba, TUGDoor* ugdoor);
    void Ready();
    void Down();

    void GenSetBBAMsg(const char* text0);
    void GenSetBBAMsg(const char* text0, const char* text1);
    void SetStopMsg();

    void SetBBAMsg(enBBAMsg msgtype);
  };
 
}
