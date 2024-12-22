#pragma once
#include "dbUGChGen.h"
namespace db
{

  //モーション
  enum enChSensyuMotionType
  {
    bmt_St,//立ち
    bmt_Wk,//歩き
    bmt_Ds,//走り
    bmt_Jp,//ジャンプ
    bmt_Cr,//着地
    bmt_Kamae,//構え
    bmt_hasBallWk,	// ボール持ち歩き
    bmt_Shot,//シュート
    bmt_Catching,//キャッチ中
    bmt_Catch,//キャッチ
    bmt_Cassette,//カセット
    bmt_Hdphone,//ヘッドホン
    bmt_Pause,//ポーズ
    bmt_PauseFly,//吹っ飛び
    bmt_PauseDown,//ダウン
    bmt_Rev,//起き上がり
    bmt_Icon,//アイコン

    bmt_RabbitJump,		// ウサギ跳び
    bmt_RabbitJumpTired,	// うさぎ跳び疲れ
    bmt_RabbitJumpLook,	// うさぎ跳び疲れ目線
    bmt_Banzai,			// バンザイ
    bmt_Angel,			// 天使
    bmt_Back,				// 後ろ向き
    bmt_FaceDown,			// うつぶせ
    bmt_DamageProfile,	// ダメージ横顔
    bmt_BlindProfile,		// 目つぶり横顔
    bmt_Anger,			// 怒り

    bmt_NONE = -1,
  };

  struct TUGChSensyuSt
  {
    s32 y_;
    s32 dy_;
    s32 x_;
    s32 dx_;

    void Init()
    {
      Zeromem(this);
    }
  };


  //リスト
  class TUGChSensyu : public TUGChGen
  {
  protected:
    void DoActionSub();
  public:
    TUGChSensyuSt stChSensyu_;
    //コンストラクタ
    explicit TUGChSensyu(TUIGenLayer* player);
    virtual ~TUGChSensyu(void);

    void MakeGroup(TUIObj* parent, s32 uino);
    void SetMType(enChSensyuMotionType mtype);

    void Init(void);

    struct runParam
    {
      s32 startX;
      s32 startY;
      s32 distX;
      s32 distY;
      f32 frame;
      f32 cnt;

      runParam()
      {
        startX = 0;
        startY = 0;
        distX = 0;
        distY = 0;
        frame = 0.0f;
        cnt = 0.0f;
      };
    }rp;
    bool isSleep_;
    s32 sleepCnt_;
    enChSensyuMotionType sleepAfterSt_;
    enChSensyuMotionType nextSt_;
    BOOL nextMirr_;
    s32 ballManType_;
    void SetNextSt(enChSensyuMotionType st, BOOL isMirror, s32 ballManType = -1);

    // 走り
    bool isRunning_;
    void SetRun(s32 sx, s32 sy, s32 ex, s32 ey, f32 frame);
    void Running(void);

    // 反動
    bool isReaction_;
    void SetReaction(s32 rdx);
    void Reaction(void);

    // ボール持って歩く
    bool isPosReset_;
    void SetPosReset(void);
    void PosResetWalk(void);

    // うさぎ跳び
    bool isRabbitJump_;
    s32 rabbitJumpNum_;		// やった回数
    void SetRabbitJump(s32 fdx, s32 fdy);
    void RabbitJump(void);

    // うさぎ跳び疲れ
    bool isRabbitJumpTire_;
    void SetRabbitJumpTire(void);
    void RabbitJumpTire(void);

    // うさぎ跳び疲れこっち見る
    void SetRabbitJumpLook(void);

    bool isFly_;

    void SetFly(s32 fdx, s32 fdy);
    void Gravity();
  };

}