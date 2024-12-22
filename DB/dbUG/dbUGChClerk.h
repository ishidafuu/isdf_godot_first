#pragma once
#include "dbUGChGen.h"
namespace db
{
  enum enClerkType
  {
    ct_Supply,
    ct_Maruka,
    ct_Dock0,
    ct_Dock1,
    ct_Dock2,
    ct_Dock3,
    ct_BBA,
    ct_Gacha,
    ct_Nazo,
    ct_Hukubiki,
  };
  //モーション
  enum enClerkMotionType
  {
    cmt_Stand,//立ち
    cmt_Talk,//話す
    cmt_Kubi,//首を傾ける
    cmt_Walk,//来る
    cmt_Leave,//離れる
    cmt_Carry,//運んでくる
    cmt_Hand,//渡す
    cmt_Bow,//礼
  };

  //リスト
  class TUGChClerk : public TUGChGen
  {
  protected:
    void DoActionSub();
  public:
    //コンストラクタ
    explicit TUGChClerk(TUIGenLayer* player);
    virtual ~TUGChClerk(void);

    void MakeGroup(TUIObj* parent, enClerkType ctype);
    void SetMType(enClerkMotionType mtype);
	void z_MoveWork(s32 moveno, f32 tagx, f32 tagy, f32 spd, enUGChGenMuki muki);
  };
 
}
