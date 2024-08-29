#include "dbUGDoor.h"


namespace db
{
  enum TUGDoorAct
  {
    ugDoor_AutoClose,
    ugDoor_Open,
    ugDoor_Close,
  };

  //コンストラクタ
  TUGDoor::TUGDoor(TUIGenLayer* player) : TUGGen(player)//アクションなし
  {
    stDoor_.Init();
  }

  TUGDoor::~TUGDoor()
  {

  }

  void TUGDoor::SetAnime(s32 num, s32 itv)
  {
    INTVECTOR anivec_oc;//開閉
    INTVECTOR anivec_o;//開く
    INTVECTOR anivec_c;//閉じる

    const s32 OPENSTOP = 2;

    for (s32 i = 0; i < num; ++i)
    {
      if (i == 0) continue;
      anivec_oc.push_back(i);
      anivec_o.push_back(i);
    }

    for (s32 i = 0; i < OPENSTOP; ++i) anivec_oc.push_back(num - 1);

    for (s32 i = 0; i < num; ++i)
    {
      if (i == 0) continue;
      s32 i2 = (num - 1 - i);
      anivec_oc.push_back(i2);
      anivec_c.push_back(i2);
    }
    
    field_->a_AddAnivec(&anivec_oc, itv, at_Door);//開閉
    field_->a_AddAnivec(&anivec_o, itv, at_Normal);//開く
    field_->a_AddAnivec(&anivec_c, itv, at_Normal);//閉じる
    stDoor_.HALFTIME = itv * num;
    stDoor_.ENDTIME = itv * ((num * 2) + OPENSTOP);
  }

  void TUGDoor::GenMakeGroup(TUIObj* parent, base::TGrPart* grppart, s32 posx, s32 posy)
  {
    SetParent(parent);
    field_->o_SetObjGrPart(grppart);
    field_->z_SetPos(posx, posy);
  }

  void TUGDoor::MakeGroup(TUIObj* parent, base::TGrPart* grppart, s32 posx, s32 posy)
  {
    GenMakeGroup(parent, grppart, posx, posy);
    const s32 ANINUM = 4;
    const s32 ANIITV = 4;
    SetAnime(ANINUM, ANIITV);
  }

  void TUGDoor::MakeGroup_shutter(TUIObj* parent, base::TGrPart* grppart, s32 posx, s32 posy)
  {
    GenMakeGroup(parent, grppart, posx, posy);
    const s32 ANINUM = 6;
    const s32 ANIITV = 8;
    SetAnime(ANINUM, ANIITV);
  }

  void TUGDoor::OpenCloseDoor()
  {
      //ドア開く
      stDoor_.door_c_ = 0;
      stDoor_.move_f_ = TRUE;
      stDoor_.half_f_ = FALSE;
      field_->a_SetAniNo(ugDoor_AutoClose, TRUE);
  }
  void TUGDoor::OpenDoor()
  {
    //ドア開く
    stDoor_.door_c_ = 0;
    stDoor_.move_f_ = TRUE;
    stDoor_.half_f_ = TRUE;
    field_->a_SetAniNo(ugDoor_Open, TRUE);
  }
  void TUGDoor::CloseDoor()
  {
    //ドア開く
    stDoor_.door_c_ = 0;
    stDoor_.move_f_ = TRUE;
    stDoor_.half_f_ = TRUE;
    field_->a_SetAniNo(ugDoor_Close, TRUE);
  }

  void TUGDoor::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
  }


  void TUGDoor::DoAction()
  {

    if (stDoor_.move_f_)
    {
      ++stDoor_.door_c_;

      if (stDoor_.door_c_ == stDoor_.HALFTIME)
      {
        SetAct(enUGDoorAct_Half);
        if (stDoor_.half_f_) stDoor_.move_f_ = FALSE;
      }
      else if (stDoor_.door_c_ > stDoor_.ENDTIME)
      {
        SetAct(enUGDoorAct_End);
        stDoor_.move_f_ = FALSE;
      }
    }
  }

  BOOL TUGDoor::IsActRes(enUGDoorAct res)//返値ゲット
  {
    return (GetActRes() == res);
  }

}
