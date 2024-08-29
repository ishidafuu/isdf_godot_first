#include "dbUGChSensyu.h"
#include "dbUILayer.h"

namespace db
{
  //コンストラクタ
  TUGChSensyu::TUGChSensyu(TUIGenLayer* player) :TUGChGen(player)
  {
    stChSensyu_.Init();
  }

  TUGChSensyu::~TUGChSensyu()
  {

  }
  void TUGChSensyu::MakeGroup(TUIObj* parent, s32 uino)
  {
    GenMakeGroup(parent, player_->pmgEO_->mgGrp_.pmgcg_sensyu_[uino], &player_->pmgEO_->mgDt_.dtBM_sensyu_, 0);
  }

  void TUGChSensyu::SetMType(enChSensyuMotionType mtype)
  {
    if (mtype == bmt_NONE) { return; }
    SetGenMType(mtype);
  }


  void TUGChSensyu::Init(void)
  {
    stChSensyu_.Init();
    stChSensyu_.dx_ = 0;
    stChSensyu_.x_ = 0;
    stChSensyu_.dy_ = 0;
    stChSensyu_.y_ = 0;
    ballManType_ = -1;
    isSleep_ = FALSE;
    isReaction_ = FALSE;
    isPosReset_ = FALSE;
    isFly_ = FALSE;
    sleepAfterSt_ = bmt_St;
    sleepCnt_ = -1;
    isRabbitJump_ = FALSE;
    rabbitJumpNum_ = 0;
  }

  void TUGChSensyu::SetNextSt(enChSensyuMotionType st, BOOL isMirror, s32 ballManType)
  {
    nextSt_ = st;
    nextMirr_ = isMirror;
    ballManType_ = ballManType;
  }

  //--------------------------------------------------------
  // 渡したフレーム数かけてスタート位置から目的位置に移動する
  //--------------------------------------------------------
  void TUGChSensyu::SetRun(s32 sx, s32 sy, s32 ex, s32 ey, f32 frame)
  {
    rp.startX = sx;
    rp.startY = sy;
    rp.distX = ex - sx;
    rp.distY = ey - sy;
    rp.frame = frame;
    rp.cnt = 0;

    SetMType(bmt_St);
    SetPos_Full(sx, sy);

    // オフセットは全てリセット
    stChSensyu_.x_ = 0;
    stChSensyu_.dx_ = 0;
    stChSensyu_.y_ = 0;
    stChSensyu_.dy_ = 0;

    // チームカード閉じた後すぐに走るよりは
    // ちょっとだけ全員が立っている姿を見せるほうがイイカンジ
    sleepCnt_ = 16;
    sleepAfterSt_ = bmt_Ds;
    isSleep_ = TRUE;

    isRunning_ = TRUE;
    if (IsItem())
    {
      itemobj_->g_SetDraw(FALSE);
    }
  }
  void TUGChSensyu::Running(void)
  {
    // 走り終わってる or そもそも走ってないなら何もしない
    if (!isRunning_ || stChar_.mt_.mtype_ != bmt_Ds){ return; }

    rp.cnt++;
    f32 ratio = rp.cnt / rp.frame;

    s32 tx = rp.startX + rp.distX * ratio;
    s32 ty = rp.startY + rp.distY * ratio;
    SetPos_Full(tx, ty);

    // txの+-でキャラ向きが決まる
    if (tx >= 0)
    {
      SetMirror(FALSE);
    }
    else
    {
      SetMirror(TRUE);
    }

    if (rp.cnt >= rp.frame)
    {
      isRunning_ = FALSE;
      rp.cnt = rp.frame;

      // 次の状態に変化
      SetMirror(nextMirr_);
      SetMType(nextSt_);
      if (IsItem())
      {
        itemobj_->g_SetDraw(TRUE);
      }
    }
  }


  // 反動
  void TUGChSensyu::SetReaction(s32 rdx)
  {
    SetGenMType(bmt_Catching);
    stChSensyu_.dx_ = rdx * XYMAG;
    stChSensyu_.x_ = (ballManType_ == 0) ? stChSensyu_.dx_ : -stChSensyu_.dx_;
    isReaction_ = TRUE;
  }
  void TUGChSensyu::Reaction(void)
  {
    s32 SUB = 10;
    if (isReaction_)
    {
      // キャッチボールするヤツ左
      if (ballManType_ == 0)
      {
        stChSensyu_.x_ -= stChSensyu_.dx_;
      }
      // キャッチボールするヤツ右
      else
      {
        stChSensyu_.x_ += stChSensyu_.dx_;
      }

      stChSensyu_.dx_ -= SUB;
      if (stChSensyu_.dx_ <= 0)
      {

        SetGenMType(bmt_Catch);
        //stChSensyu_.x_ = 0;
        stChSensyu_.dx_ = 0;
        isReaction_ = FALSE;
      }
    }
  }

  // 反動後の位置リセット
  void TUGChSensyu::SetPosReset(void)
  {
    rp.startX = stChSensyu_.x_;
    rp.startY = stChSensyu_.y_;
    rp.distX = 0 - stChSensyu_.x_;
    rp.distY = 0 - stChSensyu_.y_;
    rp.frame = 60.0f;		// ここは定義してるアニメのフレーム数なので
    rp.cnt = 0;

    SetMType(bmt_hasBallWk);

    isPosReset_ = TRUE;
  }

  void TUGChSensyu::PosResetWalk(void)
  {
    // 走り終わってる or そもそも走ってないなら何もしない
    if (!isPosReset_){ return; }

    rp.cnt++;
    f32 ratio = rp.cnt / rp.frame;

    stChSensyu_.x_ = rp.startX + rp.distX * ratio;
    stChSensyu_.y_ = rp.startY + rp.distY * ratio;

    if (rp.cnt >= rp.frame)
    {
      isPosReset_ = FALSE;
      sleepAfterSt_ = bmt_NONE;
      sleepCnt_ = 16;
      isSleep_ = TRUE;
      SetMType(bmt_Kamae);
    }
  }

  // うさぎ跳び
  void TUGChSensyu::SetRabbitJump(s32 fdx, s32 fdy)
  {
    rabbitJumpNum_++;

    stChSensyu_.dx_ = fdx * XYMAG;
    stChSensyu_.dy_ = fdy * XYMAG;
    stChSensyu_.y_ = stChSensyu_.dy_;

    isRabbitJump_ = TRUE;
  }
  void TUGChSensyu::RabbitJump()
  {
    s32 GRV = 8;
    if (isRabbitJump_)
    {
      if (stChar_.mirror_f_)
      {
        stChSensyu_.x_ -= stChSensyu_.dx_;
      }
      else
      {
        stChSensyu_.x_ += stChSensyu_.dx_;
      }
      stChSensyu_.y_ += stChSensyu_.dy_;
      stChSensyu_.dy_ -= GRV;
      if (stChSensyu_.y_ <= 0)
      {
        stChSensyu_.dx_ = 0;
        stChSensyu_.y_ = 0;
        stChSensyu_.dy_ = 0;
        isRabbitJump_ = FALSE;
      }
    }
  }


  // うさぎ跳び疲れ
  void TUGChSensyu::SetRabbitJumpTire(void)
  {
    SetMType(bmt_RabbitJumpTired);
    field_->b_SetBtn(TRUE);
  }
  void TUGChSensyu::RabbitJumpTire(void)
  {
    // キャラをタッチしたか
    if (field_->b_IsEnter(FALSE))
    {
      SetRabbitJumpLook();
      field_->b_SetBtn(FALSE);
	  pmgEO_->mgSound_.PlaySYS(2);	// なめんなよって流してみる
    }
  }

  // うさぎ跳び疲れこっち見る
  void TUGChSensyu::SetRabbitJumpLook(void)
  {
    SetMType(bmt_RabbitJumpLook);
    // ここでなめんなよ！って音声入れたい！！
  }

  void TUGChSensyu::SetFly(s32 fdx, s32 fdy)
  {
    SetGenMType(bmt_PauseFly);
    stChSensyu_.dy_ = fdy * XYMAG;
    stChSensyu_.y_ = stChSensyu_.dy_;
    field_->g_Flash();
    isFly_ = TRUE;
  }
  void TUGChSensyu::Gravity()
  {
    s32 GRV = 8;
    if (isFly_)
    {
      stChSensyu_.y_ += stChSensyu_.dy_;
      stChSensyu_.dy_ -= GRV;
      if (stChSensyu_.y_ <= 0)
      {
        SetGenMType(bmt_PauseDown);
        stChSensyu_.y_ = 0;
        stChSensyu_.dy_ = 0;
        isFly_ = FALSE;
      }
    }
  }

  void TUGChSensyu::DoActionSub()
  {

    if (isSleep_)
    {
      sleepCnt_--;
      if (sleepCnt_ < 0)
      {
        SetMType(sleepAfterSt_);
        sleepAfterSt_ = bmt_St;
        isSleep_ = FALSE;
        sleepCnt_ = -1;
        return;
      }
    }
    // 走る処理
    Running();
    // 反動
    Reaction();
    // 反動後の位置リセット
    PosResetWalk();
    // うさぎ跳び
    RabbitJump();
    // うさぎ跳び疲れ
    RabbitJumpTire();

    //重力
    Gravity();

    field_->c_SetPos((stChSensyu_.x_ / XYMAG), -(stChSensyu_.y_ / XYMAG));
    if (IsAct())
    {
      switch (GetActRes())
      {
      case enUGChGenAct_ActPoint:
        if (stChar_.mt_.mtype_ == bmt_Pause) { SetFly(0, 1); }
        if (stChar_.mt_.mtype_ == bmt_RabbitJump) { SetRabbitJump(1, 1); }
        break;
      case enUGChGenAct_MotihonEnd:
        switch (stChar_.mt_.mtype_)
        {
        case bmt_PauseDown:
          SetMType(bmt_Pause);
          break;
        case bmt_Catch:
          SetPosReset();
          break;
        case bmt_Shot:
          SetMType(bmt_Kamae);
          break;
        case bmt_Kamae:
          break;
        case bmt_RabbitJump:
          // 方向反転
          if (rabbitJumpNum_ == 2 || rabbitJumpNum_ == 6)
          {
            if (!isSleep_)
            {
              SetMirror(!stChar_.mirror_f_);
              sleepAfterSt_ = bmt_RabbitJump;
              sleepCnt_ = 16;
              isSleep_ = TRUE;
            }
          }
          // 真ん中に戻ったので休憩
          else if (rabbitJumpNum_ == 8)
          {
            if (!isSleep_)
            {
              SetRabbitJumpTire();
              rabbitJumpNum_ = 0;
            }
          }
          // もう一度うさぎ跳び
          else
          {
            if (!isSleep_)
            {
              sleepAfterSt_ = bmt_RabbitJump;
              sleepCnt_ = 16;
              isSleep_ = TRUE;
            }
          }
          break;
        case bmt_RabbitJumpTired:
          field_->b_SetBtn(FALSE);
          SetMType(bmt_RabbitJump);
          break;
        case bmt_RabbitJumpLook:
          SetMType(bmt_RabbitJump);
          break;
        default:
          SetMType(bmt_St);
          break;
        }
        break;
      }
    }
  }


}
