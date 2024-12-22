#include "dbUGSerifu.h"


namespace db
{
  const s32 MSG_ICON_TIMER = 16;	// メッセージ次ページ待ちアイコンの点滅スピード


  //コンストラクタ
  TUGSerifu::TUGSerifu(TUIGenLayer* player) :TUGGen(player)
    , isNext_(FALSE)
    , msgIconDispTimer_(0)
    , changeMode_(enMsgChangeMode_Wait)	// デフォルトはウェイトによる自動切り替え
  {
    stSerifu_.Init();
    ENDWAIT = 40;
  }

  TUGSerifu::~TUGSerifu()
  {

  }

  void TUGSerifu::ChangeFrame(enSerifuFrame frame)
  {
    //frame_ = MakeObj();//点滅が消えるのは、ここで作り直してしまって、点滅の参照が切れてるので
    switch (frame)
    {
    case enSerifuFrame_Normal:frame_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_serifu_frame)); break;
    case enSerifuFrame_Message:frame_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_msg_frame)); break;//GrPart
    case enSerifuFrame_Wide:frame_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_serifu_frame2)); break;
    }
    //frame_ = MakeObj();
    //frame_->o_SetParent(field_);
    //frame_->z_SetPos(0, 0);
  }


  //------------------------------------------------
  // アニメーションスキップ
  //------------------------------------------------
  BOOL TUGSerifu::SkipSerifu(void)
  {
    BOOL ret = FALSE;
	if (changeMode_ == enMsgChangeMode_None
	  || changeMode_ == enMsgChangeMode_Touch
      || changeMode_ == enMsgChangeMode_Wait)
    {
      stSerifu_.readline_ = lines_.size();
      for (s32 i = 0; i < lines_.size(); ++i)
      {
        lines_[i]->f_SetFontDraw(TRUE);
        lines_[i]->f_SetFontDrawLen(0);
      }
      ret = TRUE;
    }
    return ret;
  }
  //------------------------------------------------
  // メッセージスキップ
  //------------------------------------------------
  BOOL TUGSerifu::NextSerifu(void)
  {
    BOOL ret = FALSE;

    if (changeMode_ == enMsgChangeMode_Touch)
    {
      isNext_ = TRUE;
    }
    else if (changeMode_ == enMsgChangeMode_Wait)
    {
      stSerifu_.wait_c_ = 1;
    }
    return ret;
  }

  //------------------------------------------------
  // アニメーションスキップ
  //------------------------------------------------
  BOOL TUGSerifu::IsReadLineEnd(void)
  {
    return !stSerifu_.reading_f_;
  }


  void TUGSerifu::MakeGroup(TUIObj* parent, s32 linenum, enSerifuFrame frame, enUIDepth depth)
  {
    SetParent(parent);

    field_->g_SetDepth(depth);//前面に持ってくる

    frame_ = MakeObj();
    frame_->o_SetParent(field_);
    frame_->z_SetPos(0, 0);
    ChangeFrame(frame);

    for (s32 i = 0; i < linenum; ++i)
    {
      lines_.push_back(MakeObj());
      lines_[i]->o_SetParent(frame_);
      lines_[i]->z_SetPos(6, 6 + (i * 12));
      itemslen_.push_back(0);
    }

    msgIcon_ = MakeObj();
    //msgIcon_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_finger));		// 仮で指アイコン
    msgIcon_->o_SetObjGrp(pmgUI_->GetUGGrp(ug_list_marker));		// 仮アイコン
    msgIcon_->o_SetParent(frame_);
    msgIcon_->g_SetDraw(FALSE);
    msgIcon_->f_SetCenter(TRUE);
    msgIcon_->z_SetPos(frame_->g_GetSize().x_ - 10, frame_->g_GetSize().y_ - 10);
  }
  void TUGSerifu::SetEndWait(s32 wait)
  {
    ENDWAIT = wait;
  }

  void TUGSerifu::SetSerifu(STRVECTOR strvec)
  {
    stSerifu_.Init();
    stSerifu_.reading_f_ = TRUE;
    isNext_ = FALSE;

    for (s32 i = 0; i < lines_.size(); ++i)
    {
      if (i >= strvec.size())
      {
        lines_[i]->f_MakeFont("", FC2_btn_normal);
      }
      else
      {
        lines_[i]->f_MakeFont(strvec[i].c_str(), FC2_btn_normal);
      }

      lines_[i]->f_SetFontDraw(FALSE);
    }
  }


  //セリフ送り
  void TUGSerifu::DoUpdate(BOOL draw_f)
  {
    //field_->Update(draw_f);
    frame_->Update(draw_f);
    for (s32 i = 0; i < lines_.size(); ++i)
    {
      lines_[i]->Update(draw_f);
    }
    msgIcon_->Update(draw_f);
  }

  void TUGSerifu::DoAction()
  {
    const s32 SPD = 4;

    //----------------------------------------------------
    // ページの切り替え
    switch (changeMode_)
    {
      // 切り替えなし
    case enMsgChangeMode_None:
      break;

      // タッチによるページ切り替え
    case enMsgChangeMode_Touch:
      if (isNext_)
      {
        msgIconDispTimer_ = 0;
        msgIcon_->g_SetDraw(FALSE);
        SetAct(enUGSerifuAct_WaitEnd);//終わり
      }
      break;

      // ウェイトによる自動切り替え
    case enMsgChangeMode_Wait:
    case enMsgChangeMode_Wait2:
      if (stSerifu_.wait_c_ > 0)
      {
        --stSerifu_.wait_c_;
        if (stSerifu_.wait_c_ == 0)
        {
          SetAct(enUGSerifuAct_WaitEnd);//終わり
        }
      }
      break;
    }


    //進行
    if (stSerifu_.reading_f_)
    {
      if (stSerifu_.readline_ < lines_.size())
      {
        if (lib_num::AprToDest(&stSerifu_.serifu_c_, lines_[stSerifu_.readline_]->stSO_.stFont_.fontlen_ * SPD))
        {
          stSerifu_.serifu_c_ = 0;
          ++stSerifu_.readline_;
        }
      }
      else
      {
        stSerifu_.reading_f_ = FALSE;
        //stSerifu_.wait_c_ = ENDWAIT;//間を取る
        SetAct(enUGSerifuAct_ReadEnd);//読み終わり

        switch (changeMode_)
        {
          // 切り替えなし
        case enMsgChangeMode_None:
          break;
          // タッチによるページ切り替え
        case enMsgChangeMode_Touch:
        {
          msgIcon_->g_SetDraw(TRUE);
          s32 w = msgIcon_->b_GetAutoSize().width_;
          s32 h = msgIcon_->b_GetAutoSize().height_;
          s32 tmpX = frame_->b_GetAutoSize().width_ - (w + w * 0.5);
          s32 tmpY = frame_->b_GetAutoSize().height_ - (h + h * 0.5);
          msgIcon_->z_SetPos(tmpX, tmpY);
          msgIconDispTimer_ = MSG_ICON_TIMER;
          isNext_ = FALSE;
        }
        break;
        // ウェイトによる自動切り替え
        case enMsgChangeMode_Wait:
        case enMsgChangeMode_Wait2:
          stSerifu_.wait_c_ = ENDWAIT;
          break;
        }
      }
    }

    // メッセージ次ページ待ちアイコン点滅演出
    if (changeMode_ == enMsgChangeMode_Touch
      && !stSerifu_.reading_f_)
    {
      msgIconDispTimer_--;
      if (msgIconDispTimer_ < 0)
      {
        msgIconDispTimer_ = MSG_ICON_TIMER;
        msgIcon_->g_SetDraw(!msgIcon_->IsDraw());
      }
    }
    else
    {
      msgIcon_->g_SetDraw(FALSE);
    }



    //表示量
    for (s32 i = 0; i < lines_.size(); ++i)
    {
      if (i > stSerifu_.readline_)
      {
        lines_[i]->f_SetFontDraw(FALSE);
      }
      else  if (i == stSerifu_.readline_)
      {
        lines_[i]->f_SetFontDraw(TRUE);
        lines_[i]->f_SetFontDrawLen(1 + (stSerifu_.serifu_c_ / SPD));
      }
      else
      {
        lines_[i]->f_SetFontDraw(TRUE);
        lines_[i]->f_SetFontDrawLen(0);
      }
    }
  }
  BOOL TUGSerifu::IsActRes(enUGSerifuAct res)//返値ゲット
  {
    return (GetActRes() == res);
  }



}
