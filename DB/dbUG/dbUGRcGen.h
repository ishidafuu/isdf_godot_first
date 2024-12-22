#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGGen.h"
#include "dbUGRName.h"
namespace db
{
  const s32 SLIDERW = 144;
  const s32 SLIDERH = 136;
  const s32 LISTTOPY = 8;


  struct TUGRcGenSt
  {
    s32 posx_;//スライダー上の位置Ｙ
    s32 posy_;//スライダー上の位置Ｙ
    BOOL plate_f_;
    BOOL selected_f_;
    BOOL disable_f_;
    void Init()
    {
      Zeromem(this);
    }
  };


  class TUGRcGen :public TUGGen
  {
  protected:
    void DoUpdate(BOOL draw_f);
    void DoAction();
    virtual void DoSetLimY(s32 limu, s32 hhh){}
    virtual void DoUpdateSub(BOOL draw_f){}
    virtual void DoActionSub(){}
    virtual s32 DoMakeGroupSub() = 0;//必須
    
  public:
    TUGRcGenSt stRec_;
    std::vector<TUIObj*> items_;

    //コンストラクタ
    explicit TUGRcGen(TUIGenLayer* player);
    virtual ~TUGRcGen(void);
    s32 MakeGroup(TUIObj* parent, s32 posx, s32 posy, s32 limx, s32 limy);//wideもあるのでposxもいる

    void SetLimY(s32 limu, s32 hhh);
    void SetSelect(BOOL select_f);
    BOOL IsSelect();
    void SetPlate(BOOL plate_f);
    void SetDisable(BOOL disable_f);
    s32 GetPosX();
    s32 GetPosY();
  };
}
