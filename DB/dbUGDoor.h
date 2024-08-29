#pragma once
#include "dbInclude.h"
#include "dbUGGen.h"

namespace db
{
  enum enUGDoorAct
  {
    enUGDoorAct_Half,
    enUGDoorAct_End,
  };


  class TUILayer;

  struct TUGDoorSt
  {
    s32 door_c_;
    BOOL move_f_;
    BOOL half_f_;

    s32 HALFTIME;
    s32 ENDTIME;
    void Init()
    {
      Zeromem(this);
    }
  };

  //リスト
  class TUGDoor : public TUGGen
  {
  protected:
    //TUIObj* door_;

    void GenMakeGroup(TUIObj* parent, base::TGrPart* grppart, s32 posx, s32 posy);
    void SetAnime(s32 num,s32 itv);

    void DoUpdate(BOOL draw_f);
    void DoAction();
  public:
    TUGDoorSt stDoor_;

    //コンストラクタ
    explicit TUGDoor(TUIGenLayer* player);
    virtual ~TUGDoor(void);
    void MakeGroup(TUIObj* parent, base::TGrPart* grppart, s32 posx, s32 posy);
    void MakeGroup_shutter(TUIObj* parent, base::TGrPart* grppart, s32 posx, s32 posy);
    void OpenCloseDoor();
    void OpenDoor();
    void CloseDoor();


    BOOL IsActRes(enUGDoorAct res);
  };
 
}
