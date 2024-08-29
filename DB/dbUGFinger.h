#pragma once
#include "dbInclude.h"
#include "dbUIObj.h"
#include "dbUGGen.h"

namespace db
{

  struct TUGFingerSt
  {
    s32 ptn_c_;
    s32 ptntime_;
    s32 baseposx_;
    s32 baseposy_;
    s32 motion_;
	s32 submotion_;
	s32 tagNo_;

    void Init()
    {
      Zeromem(this);
    }
  };


  //セリフ
  class TUGFinger : public TUGGen
  {
  public:
    INTVECTOR tagx_;
    INTVECTOR tagy_;
    TUGFingerSt stFinger_;
    TUIObj* hand_;
    BOOL down_f_;
    //コンストラクタ
    explicit TUGFinger(TUIGenLayer* player);
    virtual ~TUGFinger(void);
    void MakeGroup(TUIObj* parent);
    //void SetMotionStay(s32 posx, s32 posy);
    void SetMotionTouch(s32 posx, s32 posy);
    void SetMotionLongTouch(s32 posx, s32 posy);
    void SetMotionGrgr(s32 posx, s32 posy);
    void SetMotionSwipeLR(s32 posx, s32 posy);
    void SetMotionSwipeUDLR(s32 posx, s32 posy);
    void SetMotionFrickU(s32 posx, s32 posy);
    void SetMotionFrickLR(s32 posx, s32 posy);
    void SetMotionFrickUDLR(s32 posx, s32 posy);
	void SetMotionLookPoint(s32 posx, s32 posy);
	void SetMotionMoveLine(s32 sx, s32 sy, s32 tx, s32 ty);
	void ClearMotion(void);
    void SetRevUD(BOOL down_f);
  protected:
    void Move_Touch();
    void Move_LongTouch();
    void Move_Grgr();
    void Move_Swipe();
    void Move_Flick();
	void Move_Point(void);
	void Move_Line(void);

    void Move();
    void GenSetMotion(s32 motion, s32 posx, s32 posy);
    void GenSetTagU();
    void GenSetTagLR();
    void GenSetTagUDLR();
	void GenSetMoveLine(s32 posx, s32 posy);
    void DoUpdate(BOOL draw_f);
    void DoAction();
  };

}