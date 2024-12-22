#include "dbUGFilter.h"


namespace db
{
  const s32 BARW = 512;
  const s32 HEADH = 44;

  //コンストラクタ
  TUGFilter::TUGFilter(TUIGenLayer* player) : TUGGen(player)//アクションなし
  {
    stFilter_.Init();
  }

  TUGFilter::~TUGFilter()
  {

  }
  void TUGFilter::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);

    //ヘッダフッタ間フィルタ
    filter_ = MakeObj();
    filter_->o_SetParent(field_);
    filter_->o_SetObjFilterGrp(BARW, 164, 0xFF, 0, 0, 0);
    filter_->z_SetPos_Full(0, HEADH);
    filter_->g_SetAlp(FALSE, 0x00);//最初見えないように
    filter_->g_SetDepth(UIDP_10ALLFILTER);

    //ハーフフィルタ
    filterhf_ = MakeObj();
    filterhf_->o_SetParent(field_);
    filterhf_->o_SetObjFilterGrp(mid::midFullDotL_Hf(), 164, 0xFF, 0, 0, 0);
    filterhf_->z_SetPos_Full(0, HEADH);
    filterhf_->g_Fade(TRUE);//フィルタ消える
    filterhf_->g_SetAlp(FALSE, 0x00);//最初見えないように
    //filterhf_->g_SetDepth(UIDP_04HEADER);//まだ不確定
    filterhf_->g_SetDepth(UIDP_10ALLFILTER);
  }

  void TUGFilter::OnFilter(s32 lv, BOOL white_f, BOOL autooff_f)
  {
    s32 col = (white_f)
      ? 0xFF
      : 0x00;
    

    switch (lv)
    {
    case 0://ヘッダフッタ間
      filter_->o_SetObjFilterGrp(BARW, 164, 0xFF, col, col, col);
      filter_->z_SetPos_Full(0, HEADH);
      break;
    case 1://ヘッダ画面下
      filter_->o_SetObjFilterGrp(BARW, BARW, 0xFF, col, col, col);
      filter_->z_SetPos_Full(0, HEADH);
      break;
    default://画面上画面下
      filter_->o_SetObjFilterGrp(BARW, BARW, 0xFF, col, col, col);
      filter_->z_SetPos_Full(0, 0);
      break;
    }

    filter_->g_SetAlp(FALSE, 0x00);//最初見えないように
    stFilter_.filter_c_ = 0;
    stFilter_.filterOn_f_ = TRUE;
    stFilter_.eat_ = 10;
    stFilter_.filterMove_f_ = TRUE;
    stFilter_.autooff_f_ = autooff_f;
    stFilter_.filterLv_ = lv;
    //filter_->g_Fade_Ex(!(stFilter_.filterOn_f_), stFilter_.eat_);//フィルタ出てくる
  }
  void TUGFilter::OffFilter(s32 lv, BOOL white_f)
  {
    OnFilter(lv, white_f, FALSE);
    filter_->g_SetAlp(FALSE, 0xFF);//最初出てる
    stFilter_.filterOn_f_ = FALSE;
  }
  void TUGFilter::OnFilterHf(s32 lv, BOOL white_f, BOOL left_f)
  {
    s32 col = (white_f)
      ? 0xFF
      : 0x00;
    s32 posx = (left_f)
      ? 0
      : mid::midFullDotL_Hf();

    switch (lv)
    {
    case 0:
      filterhf_->o_SetObjFilterGrp(mid::midFullDotL_Hf(), 164, 0xFF, col, col, col);
      filterhf_->z_SetPos_Full(posx, HEADH);
      break;
    case 1:
      filterhf_->o_SetObjFilterGrp(mid::midFullDotL_Hf(), BARW, 0xFF, col, col, col);
      filterhf_->z_SetPos_Full(posx, HEADH);
      break;
    default:
      filterhf_->o_SetObjFilterGrp(mid::midFullDotL_Hf(), BARW, 0xFF, col, col, col);
      filterhf_->z_SetPos_Full(posx, 0);
      break;
    }
  }
  void TUGFilter::OffFilterHf()
  {
    filterhf_->g_SetAlp(FALSE, 0x00);//最初見えないように
  }
  void TUGFilter::SetFilterDepth(db::enUIDepth depth)
  {
	  filter_->g_SetDepth(depth);
  }

  void TUGFilter::Flash(BOOL red_f)
  {
    s32 lv = 0;
    s32 col = (red_f)
      ? 0x00
      : 0xFF;
    switch (lv)
    {
    case 0:
      filter_->o_SetObjFilterGrp(BARW, 164, 0xFF, 0xFF, 0xFF, col);
      filter_->z_SetPos_Full(0, HEADH);
      break;
    case 1:
      filter_->o_SetObjFilterGrp(BARW, BARW, 0xFF, 0xFF, 0xFF, col);
      filter_->z_SetPos_Full(0, HEADH);
      break;
    default:
      filter_->o_SetObjFilterGrp(BARW, BARW, 0xFF, 0xFF, 0xFF, col);
      filter_->z_SetPos_Full(0, 0);
      break;
    }
    filter_->g_Fade_Ex(TRUE, 6);
  }

  void TUGFilter::DoUpdate(BOOL draw_f)
  {
    filter_->Update(draw_f);
    filterhf_->Update(draw_f);
  }


  void TUGFilter::DoAction()
  {
    
    if (stFilter_.filterMove_f_)
    {
      ++stFilter_.filter_c_;//関数へ
      if (stFilter_.filter_c_ == 1) filter_->g_Fade_Ex(!(stFilter_.filterOn_f_), stFilter_.eat_);//フィルタ出てくる

      if (stFilter_.filter_c_ > (stFilter_.eat_ + 1))
      {
        stFilter_.filter_c_ = 0;

        if (stFilter_.filterOn_f_)
        {
          SetAct(stFilter_.filterLv_);
          if (stFilter_.autooff_f_)
          {
            stFilter_.filterOn_f_ = FALSE;//自動的にオフ
          }
          else
          {
            stFilter_.filterMove_f_ = FALSE;//暗転したまま止める
          }
        }
        //else
        //{
        //  SetAct(ugfilter_fadein);
        //  stFilter_.filterMove_f_ = FALSE;//フィルタ終了
        //}
      }
    }
  }

}
