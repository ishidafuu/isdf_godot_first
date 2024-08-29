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
  enum enMarukaMsg
  {
    enMarukaMsg_Visit,//入店時
    enMarukaMsg_ShotRcv,//シュート選択
    enMarukaMsg_OK,//確認待ち
    enMarukaMsg_Thanks,//ありがとう
    enMarukaMsg_Out,//退出時
    enMarukaMsgEND,
  };


  struct TUGSpMarukaSt
  {
    enMarukaMsg mtype_;//店員メッセージタイプ
    void Init()
    {
      Zeromem(this);
    }
  };

  class TUGSpMaruka : public TUGGen
  {
  protected:
    TUGDoor* ugdoor_;//ばばあまどポインタ
    TUGChKantoku* ugkantoku_;//監督ポインタ
	TUGChCaptain* ugcap_;
    TUGChClerk* ugclmaruka_;//まるか兄
    STRVECTOR serifuvec_;

    void GenSetMsg(const char* text0);
    void GenSetMsg(const char* text0, const char* text1);

    void Mv_Default();

    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGSpMarukaSt stMaruka_;
    TUGSerifu ugserifu_;//セリフ

    //コンストラクタ
    explicit TUGSpMaruka(TUIGenLayer* player);
    virtual ~TUGSpMaruka(void);
    void MakeGroup(TUIObj* parent, TUGChKantoku* ugkantoku, TUGChCaptain* ugcap, TUGChClerk* ugclmaruka, TUGDoor* ugdoor);
    void Ready();

    void SetStopMsg();

    void SetMsg(enMarukaMsg msgtype);
  };
 
}
