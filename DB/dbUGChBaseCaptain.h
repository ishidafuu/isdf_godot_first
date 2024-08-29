#pragma once
#include "dbUGChGen.h"
namespace db
{
  // 前方参照
  class TUGChSensyu;
  class TUGChBaseKantoku;

  //モーション
  enum enChCaptainMotionType
  {
    cmt_St,//立ち
    cmt_Wk,//歩き
    cmt_Ds,//走り
    cmt_Jp,//ジャンプ
    cmt_Cr,//着地
    cmt_Kamae,//構え
    cmt_hasBallWk,	// ボール持ち歩き
    cmt_Shot,//シュート
    cmt_Catching,//キャッチ中
    cmt_Catch,//キャッチ
    cmt_Cassette,//カセット
    cmt_Hdphone,//ヘッドホン
    cmt_Pause,//ポーズ
    cmt_PauseFly,//吹っ飛び
    cmt_PauseDown,//ダウン
    cmt_Rev,//起き上がり
    cmt_Icon,//アイコン

    cmt_RabbitJump,		// ウサギ跳び
    cmt_RabbitJumpTired,	// うさぎ跳び疲れ
    cmt_RabbitJumpLook,	// うさぎ跳び疲れ目線
    cmt_Banzai,			// バンザイ
    cmt_Angel,			// 天使
    cmt_Back,				// 後ろ向き
    cmt_FaceDown,			// うつぶせ
    cmt_DamageProfile,	// ダメージ横顔
    cmt_BlindProfile,		// 目つぶり横顔
    cmt_Anger,			// 怒り

    cmt_NONE = -1,
  };

  // 移動タイプ
  enum enChCaptainWalkType
  {
    cwt_FOLLOW = 0,	// 監督の足跡を追いかける
    cwt_MYWALK,		// 自分で監督の横に移動する
  };

  struct TUGChBaseCaptainSt
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
  class TUGChBaseCaptain : public TUGChGen
  {
  public:

    //コンストラクタ
    explicit TUGChBaseCaptain(TUIGenLayer* player);
    virtual ~TUGChBaseCaptain(void);

	void MakeGroup(TUIObj* parent, TMgCharGrp* pmgcg);
    void SetMType(enChCaptainMotionType mtype);

    void z_MoveWork(s32 moveno, f32 tagx, f32 tagy, f32 spd);
    void z_MoveWork(s32 moveno, f32 tagx, f32 tagy, f32 spd, enUGChGenMuki muki);
    void z_MoveWork_Full(s32 moveno, f32 tagx, f32 tagy, f32 spd);
    void z_MoveWork_Full(s32 moveno, f32 tagx, f32 tagy, f32 spd, enUGChGenMuki muki);
    void z_MoveWork_Full(s32 moveno, f32 tagx, f32 tagy, f32 spd, f32 lookx);

    void SetWalkType(enChCaptainWalkType type);

    void SetKantoku(TUGChBaseKantoku* kantoku);
    void UpdateMemPos(void);

	void SetFollowPos(s32 pos);

  protected:
	  void DoActionSub();

	  // ここにかんとくを持たせてしまう
	  TUGChBaseKantoku* ugKantoku_;
	  // かんとくの歩いた座標
	  std::vector<std::vector<f32>> memPos;
	  BOOL isWalk_;	//歩いてるフラグ
	  enChCaptainWalkType type_;

	  s32 cpPos_;	// 監督にフォローする際の使う配列位置
  };

}
