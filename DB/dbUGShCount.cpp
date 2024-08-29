#include "dbUGShCount.h"
#include "dbUIGenLayer.h"
#include "dbGame.h"


namespace db
{
  const s32 LINENUM = 2;

  //コンストラクタ
  TUGShCount::TUGShCount(TUIGenLayer* player) :TUGGen(player)
  {
    stShCount_.Init();
  }

  TUGShCount::~TUGShCount()
  {

  }

  void TUGShCount::MakeGroup(TUIObj* parent)
  {
    SetParent(parent);

    field_->g_SetDepth(UIDP_05BUTTON);
    SetPos(0, -100);

    killcount_ = MakeObj();
    killcount_->o_SetParent(field_);
    killcount_->f_SetCenter(TRUE);
    ballcount_ = MakeObj();
    ballcount_->o_SetParent(field_);
    ballcount_->f_FontPos(0, 10);
    ballcount_->f_SetCenter(TRUE);
  }

  void TUGShCount::Refresh()
  {

    //スコア
    {
      s32 s0num = DBMEMBER_ALL - player_->pGame_->mgGO_.pmgTm_[SIDE0]->DeadNum();
      s32 s1num = DBMEMBER_ALL - player_->pGame_->mgGO_.pmgTm_[SIDE1]->DeadNum();
      BOOL needrefresh_f = ((stShCount_.s0num_ != s0num) || (stShCount_.s1num_ != s1num));
      if (needrefresh_f)
      {
        stShCount_.s0num_ = s0num;
        stShCount_.s1num_ = s1num;
        std::string st = lib_str::IntToStr(s0num) + "負" + lib_str::IntToStr(s1num);
        killcount_->f_MakeFont(st.c_str(), FC2_btn_normal);
      }
    }

    //ボールカウント
    {
      s32 SEC = 60;
      s32 bcount = ((player_->pGame_->mgGO_.pmgBa_->st_.pstBa_->Timer_c + (SEC - 1)) / SEC);
      BOOL needrefresh_f = (stShCount_.bcount_ != bcount);//変わった瞬間だけ
     
      if (needrefresh_f)
      {
        stShCount_.bcount_ = bcount;
        std::string st = lib_str::IntToStr(bcount);
        ballcount_->f_MakeFont(st.c_str(), FC2_btn_normal);
      }
    }
  }

  void TUGShCount::DoUpdate(BOOL draw_f)
  {
    Refresh();
    field_->Update(draw_f);
    killcount_->Update(draw_f);
    ballcount_->Update(draw_f);
  }

  void TUGShCount::DoAction()
  {
  }
}
