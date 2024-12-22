#include "dbUGCaBall.h"

namespace db
{

  //コンストラクタ
  TUGCaBall::TUGCaBall(TUIGenLayer* player) :TUGGen(player)
  {
    stCB_.Init();
  }

  TUGCaBall::~TUGCaBall()
  {
  }

  void TUGCaBall::MakeGroup(TUGChSensyu* memL, TUGChSensyu* memR, TUIObj* ball)
  {
    ball_ = ball;
    mem_[0] = memL;
    mem_[1] = memR;
    mem_[0]->SetItem(ball_);

	// 最初の1回がなぜか玉表示されてしまうので
	// これで消してしまう
	ball_->g_SetDraw(FALSE);
  }

  void TUGCaBall::DoUpdate(BOOL draw_f)
  {
    mem_[0]->Update(draw_f);
    mem_[1]->Update(draw_f);

	// 二人とも走り終えていればボール表示する
	BOOL isBallDraw = FALSE;
	if (!mem_[0]->isRunning_ && !mem_[1]->isRunning_)
	{
		ball_->g_SetDraw(TRUE);
		isBallDraw = draw_f;
	}
	else
	{
		isBallDraw = FALSE;
	}
	ball_->Update(isBallDraw);

    if (draw_f == FALSE)
    {
		if (mem_[0]->stChar_.mt_.actP_f_ &&
			mem_[0]->stChar_.mt_.mtype_ == bmt_Shot &&
			mem_[1]->stChar_.mt_.mtype_ != bmt_Catching )
      {
        mem_[0]->ReleaseItem();
		mem_[1]->SetReaction(2);
        mem_[1]->SetItem(ball_);
      }
		if (mem_[1]->stChar_.mt_.actP_f_ &&
			mem_[1]->stChar_.mt_.mtype_ == bmt_Shot &&
			mem_[0]->stChar_.mt_.mtype_ != bmt_Catching)
		{
        mem_[1]->ReleaseItem();
		mem_[0]->SetReaction(2);
        mem_[0]->SetItem(ball_);
      }

      for (s32 i = 0; i < DBSIDE; ++i)
      {
        if (mem_[i]->IsItem() 
          && (mem_[i]->stChar_.mt_.mtype_ == bmt_Kamae)
		  && !mem_[i]->isSleep_)
        {
          mem_[i]->SetMType(bmt_Shot);
        }
      }
    }
  }
}
