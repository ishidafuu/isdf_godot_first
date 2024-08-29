#include "dbUGFinger.h"


namespace db
{
  //ショップメッセージ
  enum enFingerMotion
  {
    enFingerMotion_None,
    enFingerMotion_Touch,
    enFingerMotion_LongTouch,
    enFingerMotion_Grgr,
    enFingerMotion_Swipe,
    enFingerMotion_Flick,

	enFingerMotion_Point,
	enFingerMotion_MoveLine,
  };

  const s32 MOVE = 40;
  //コンストラクタ
  TUGFinger::TUGFinger(TUIGenLayer* player) :TUGGen(player)
  {
    stFinger_.Init();
    down_f_ = FALSE;
    //ENDWAIT = 40;
  }

  TUGFinger::~TUGFinger()
  {

  }

  void TUGFinger::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);
    field_->f_FontPos(0, -20);
    field_->g_SetDepth(UIDP_09MSGBOX);
    hand_ = MakeObj();
    hand_->o_SetParent(field_);
    hand_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_finger));

    //field_->z_SetPos(0, -(GRIDSIZE * 3));
  }

  void TUGFinger::GenSetMotion(s32 motion, s32 posx, s32 posy)
  {
    SetDraw(TRUE);
    hand_->g_EfReset();
    hand_->g_SetDraw(TRUE);
    std::string text = "";
    stFinger_.Init();
    stFinger_.motion_ = motion;
	stFinger_.submotion_ = 0;
	stFinger_.ptntime_ = 0;
	stFinger_.tagNo_ = 0;
    stFinger_.baseposx_ = posx;
    stFinger_.baseposy_ = posy;
    field_->z_SetPos(posx, posy);
    hand_->z_SetPos(0, 0);
    switch (motion)
    {
    case db::enFingerMotion_None:
      break;
    case db::enFingerMotion_Touch:
      text = "タッチ！";
      break;
    case db::enFingerMotion_LongTouch:
      text = "ながおし";
      break;
    case db::enFingerMotion_Grgr:
      text = "グリグリ";
      break;
    case db::enFingerMotion_Swipe:
      text = "スワイプ";
      break;
    case db::enFingerMotion_Flick:
      text = "フリック！";
      break;
	case db::enFingerMotion_Point:
	case db::enFingerMotion_MoveLine:
	  break;
    default:
      break;
    }

    field_->f_MakeFont(text.c_str(), FC2_normal);
    Move();
  }
  void  TUGFinger::GenSetTagU()
  {
    tagx_.clear();
    tagy_.clear();
    tagy_.push_back(-MOVE);
  }
  void  TUGFinger::GenSetTagLR()
  {
    tagx_.clear();
    tagx_.push_back(+MOVE);
    tagx_.push_back(-MOVE);
    tagy_.clear();
  }
  void  TUGFinger::GenSetTagUDLR()
  {
    tagx_.clear();
    tagx_.push_back(0);
    tagx_.push_back(0);
    tagx_.push_back(+MOVE);
    tagx_.push_back(-MOVE);
    tagy_.clear();
    tagy_.push_back(-MOVE);
    tagy_.push_back(+MOVE);
    tagy_.push_back(0);
    tagy_.push_back(0);
  }
  void TUGFinger::GenSetMoveLine(s32 posx, s32 posy)
  {
	  tagx_.clear();
	  tagx_.push_back(posx);
	  tagy_.clear();
	  tagy_.push_back(posy);
  }
  //void TUGFinger::SetMotionStay(s32 posx, s32 posy)
  //{
  //  GenSetMotion(enFingerMotion_None, posx, posy);
  //}
  void TUGFinger::SetMotionTouch(s32 posx, s32 posy)
  {
    GenSetMotion(enFingerMotion_Touch, posx, posy);
  }
  void TUGFinger::SetMotionLongTouch(s32 posx, s32 posy)
  {
    GenSetMotion(enFingerMotion_LongTouch, posx, posy);
  }
  void TUGFinger::SetMotionGrgr(s32 posx, s32 posy)
  {
    GenSetMotion(enFingerMotion_Grgr, posx, posy);
  }
  void TUGFinger::SetMotionSwipeLR(s32 posx, s32 posy)
  {
    GenSetTagLR();
    GenSetMotion(enFingerMotion_Swipe, posx, posy);
  }

  void TUGFinger::SetMotionSwipeUDLR(s32 posx, s32 posy)
  {
    GenSetTagUDLR();
    GenSetMotion(enFingerMotion_Swipe, posx, posy);
  }
  void TUGFinger::SetMotionFrickU(s32 posx, s32 posy)
  {
    GenSetTagU();
    GenSetMotion(enFingerMotion_Flick, posx, posy);
  }
  void TUGFinger::SetMotionFrickLR(s32 posx, s32 posy)
  {
    GenSetTagLR();
    GenSetMotion(enFingerMotion_Flick, posx, posy);
  }
  void TUGFinger::SetMotionFrickUDLR(s32 posx, s32 posy)
  {
    GenSetTagUDLR();
    GenSetMotion(enFingerMotion_Flick, posx, posy);
  }
  void TUGFinger::SetMotionLookPoint(s32 posx, s32 posy)
  {
	  GenSetMotion(enFingerMotion_Point, posx, posy);
  }
  void TUGFinger::SetMotionMoveLine(s32 sx, s32 sy, s32 tx, s32 ty)
  {
	  GenSetMoveLine(tx, ty);
	  GenSetMotion(enFingerMotion_MoveLine, sx, sy);
  }
  void TUGFinger::ClearMotion(void)
  {
	  tagx_.clear();
	  tagy_.clear();
	  GenSetMotion(enFingerMotion_None, 0, 0);
  }

  void TUGFinger::SetRevUD(BOOL down_f)
  {
    if (hand_)
    {
      s32 partno = (down_f)
        ? 1
        : 0;
      hand_->g_SetPartNo(partno);
      down_f_ = down_f;
    }
  }
  void TUGFinger::Move_Touch()
  {
    const s32 ITV = 60;
    s32 now_c = (stFinger_.ptn_c_ % ITV);
    switch (now_c)
    {
    case 0:
      hand_->z_SetPos(0, 0);
      if (down_f_)
      {
        hand_->z_MoveSetspd(-8, +8, 1);
      }
      else
      {
        hand_->z_MoveSetspd(-8, -8, 1);
      }
      break;
    case 10:
      field_->f_SetFontDraw(TRUE);
      hand_->z_MoveSetspd(0, 0, 1);
      break;
    case 30:
      field_->f_SetFontDraw(FALSE);
      break;
    }
  }
  void TUGFinger::Move_LongTouch()
  {
    const s32 ITV = 90;
    s32 now_c = (stFinger_.ptn_c_ % ITV);
    switch (now_c)
    {
    case 0:
      hand_->z_SetPos(0, 0);
      if (down_f_)
      {
        hand_->z_MoveSetspd(-8, +8, 1);
      }
      else
      {
        hand_->z_MoveSetspd(-8, -8, 1);
      }
      break;
    case 10:
      field_->f_SetFontDraw(TRUE);
      break;
    case 40:
      hand_->z_MoveSetspd(0, 0, 1);
      break;
    case 60:
      field_->f_SetFontDraw(FALSE);
      break;
    }
  }
  void TUGFinger::Move_Grgr()
  {
    const s32 ITV = 80;
    s32 now_c = (stFinger_.ptn_c_ % ITV);
    switch (now_c)
    {
    case 0:
      field_->f_SetFontDraw(TRUE);
      hand_->z_SetPos(0, 0);
      hand_->z_MoveSetspd(24, 24, 1);
      break;
    case 20:
      hand_->z_MoveSetspd(24, 0, 1);
      break;
    case 40:
      hand_->z_MoveSetspd(0, 24, 1);
      break;
    case 60:
      hand_->z_MoveSetspd(0, 0, 1);
      break;
    }
  }
  void TUGFinger::Move_Swipe()
  {
    const s32 ITV = 60;
    s32 now_c = (stFinger_.ptn_c_ % ITV);

    s32 tx = 0;
    s32 ty = 0;
    s32 ptn_c2 = (stFinger_.ptn_c_ / ITV);
    if (tagx_.size() > 0) tx = tagx_.at(ptn_c2 % tagx_.size());
    if (tagy_.size() > 0) ty = tagy_.at(ptn_c2 % tagy_.size());

    switch (now_c)
    {
    case 0:
      field_->f_SetFontDraw(TRUE);
      hand_->z_SetPos(0, 0);
      break;
    case 10:
      hand_->z_MoveSetspd(tx, ty, 1);
      break;
    case 30:
      break;
    }
  }
  void TUGFinger::Move_Flick()
  {
    const s32 ITV = 60;
    s32 now_c = (stFinger_.ptn_c_ % ITV);

    s32 tx = 0;
    s32 ty = 0;
    s32 ptn_c2 = (stFinger_.ptn_c_ / ITV);
    if (tagx_.size() > 0) tx = tagx_.at(ptn_c2 % tagx_.size()) * 2;
    if (tagy_.size() > 0) ty = tagy_.at(ptn_c2 % tagy_.size()) * 2;


    switch (now_c)
    {
    case 0:

      hand_->g_SetDraw(TRUE);
      hand_->g_EfReset();
      hand_->g_SetAlp(TRUE, 0xFF);
      hand_->z_SetPos(0, 0);

      break;
    case 10:
      field_->f_SetFontDraw(TRUE);
      hand_->z_MoveSetspd(tx, ty, 4);
      break;
    case 20:
      hand_->g_Fade_Ex(TRUE, 10);
      break;

    case 40:
      //hand_->z_MoveSetspd(stFinger_.tagx_ + 8, stFinger_.tagy_ + 8, 2);
      field_->f_SetFontDraw(FALSE);
      break;
    }
  }

  //----------------------------------------------------------------
  // 
  //----------------------------------------------------------------
  void TUGFinger::Move_Point(void)
  {
	  const f32 PI = 3.1415f;// π

	  f32 posY = sin(PI * 2 / 60 * stFinger_.ptn_c_) * 5.0f;
	  field_->z_SetPos(stFinger_.baseposx_, stFinger_.baseposy_ + posY);
  }

  //----------------------------------------------------------------
  // tagx_のサイズとtagy_のサイズはあわせること！
  //----------------------------------------------------------------
  void TUGFinger::Move_Line(void)
  {
	  const s32 ITV = 30;	// 止める時間

	  s32 tx = 0;
	  s32 ty = 0;
	  switch (stFinger_.submotion_)
	  {
      // 一定時間とめる
	  case 0:
		  stFinger_.ptntime_++;
		  // とまったら次へ
		  if (ITV < stFinger_.ptntime_)
		  {
			  if( tagx_.size() <= stFinger_.tagNo_ && tagy_.size() <= stFinger_.tagNo_ )
			  {
				  // 全てのターゲットにいったので初期に戻る
				  stFinger_.tagNo_ = 0;
				  tx = stFinger_.baseposx_;
				  ty = stFinger_.baseposy_;
			  }
			  else
			  {
				  tx = tagx_[stFinger_.tagNo_];
				  ty = tagy_[stFinger_.tagNo_];
				  stFinger_.tagNo_++;
			  }

			  // 座標系が違うのでfieldを使う
			  //hand_->z_MoveSetspd(tx, ty, 1);
			  field_->z_MoveSetspd(tx, ty, 2);
			  stFinger_.submotion_ = 1;
		  }
		  break;
	  // ついたらそこで一定時間とめる
	  case 1:
		  //if( hand_->z_IsStop())
		  if (field_->z_IsStop())
		  {
			  stFinger_.ptntime_ = 0;
			  stFinger_.submotion_ = 0;
		  }
		  break;
	  }
  }


  void TUGFinger::Move()
  {


    switch (stFinger_.motion_)
    {
    case db::enFingerMotion_None: break;
    case db::enFingerMotion_Touch: Move_Touch(); break;
    case db::enFingerMotion_LongTouch:Move_LongTouch(); break;
    case db::enFingerMotion_Grgr: Move_Grgr(); break;
    case db::enFingerMotion_Swipe: Move_Swipe(); break;
    case db::enFingerMotion_Flick:Move_Flick(); break;
	case db::enFingerMotion_Point:Move_Point(); break;
	case db::enFingerMotion_MoveLine:Move_Line(); break;
    default:
      break;
    }

    ++stFinger_.ptn_c_;
  }

  void TUGFinger::DoUpdate(BOOL draw_f)
  {
    field_->Update(draw_f);
    hand_->Update(draw_f);
  }
  void TUGFinger::DoAction()
  {
    Move();
  }
}
