#pragma once
#include "dbUGChGen.h"
namespace db
{

  //モーション
  enum enGirlMotionType
  {
    gmt_St,//立ち
    gmt_Wk,//歩き
    gmt_StTalk,//立ち話
    gmt_WkTalk,//歩き話
  };

  struct TUGChGirlSt
  {
    s32 stx_;
    s32 edx_;
    s32 liney_;
    BOOL isFrameOut_;
    BOOL wonder_f_;
    void Init()
    {
      Zeromem(this);
    }
  };


  //リスト
  class TUGChGirl : public TUGChGen
  {
  protected:
    void DoActionSub();

  public:
    TUGChGirlSt stGirl_;

    //コンストラクタ
    explicit TUGChGirl(TUIGenLayer* player);
    virtual ~TUGChGirl(void);
    void MakeGroup(TUIObj* parent, s32 girlNo, BOOL wonder_f);
    void SetMType(enGirlMotionType mtype);
    void SetWander(s32 stx, s32 edx, s32 liney);
    void z_MoveWork(s32 moveno, f32 tagx, f32 tagy, f32 spd, enUGChGenMuki muki);
    void NextWander();
  };

}
