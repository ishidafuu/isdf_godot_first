#pragma once
#include "dbBaCommon.h"
#include "dbDtRank.h"
namespace db
{
  class TMgChar;

  //判定////////////////////////////////
  class TBaJudge : public TBaHaveCommon
  {
  protected:
    BOOL IsHitCheck(s32 tNo, s32 pNo, BOOL myteamPass_f);
    BOOL IsHitCheck2(s32 tNo, s32 pNo, BOOL myteamPass_f);
    BOOL IsHitCheckFr(s32 tNo, s32 pNo);
    void OSShoot();
    s32 SetShootAgl8(BOOL firstdXZ_f);
    BOOL IsCheckCaAgl(TMgChar* pHitman, s32 tShAgl);
    enJudgeType ShootHit(TMgChar* pHitman);
    void HitReflex(TMgChar* pHitman, BOOL ZShoot_f);

    void HitReactionAI(TMgChar* pHitman, enJudgeType JdType);

    void HitReactionCatch(TMgChar* pHitman, enJudgeType JdType);
    BOOL HitReactionHit(TMgChar* pHitman, enJudgeType JdType);
    void HitReactionBound(TMgChar* pHitman, enJudgeType JdType);
    void HitReactionTuto(TMgChar* pHitman, enJudgeType JdType, BOOL dam_f);
    void HitReactionDamage(TMgChar* pHitman, enJudgeType JdType);
    void HitReaction(TMgChar* pHitman, enJudgeType JdType);
    void GetFlyEffData(TMgChar* pHitman , BOOL flyLooseOff_f);
    void SetFlyEffData(s32 startNo, s32 rankDt);
    void SetFlyEffDataSp(s32 startNo, s32 rankDt, BOOL spdX_f, BOOL grav_f);
    s32 GetFlyRankData(TMgChar* pHitman);
    BOOL CheckBrainHit(TMgChar* pHitman);
    s32 GetBrainHitLv();

    s32 GetRankDtLevel(enRankLevel dtNo);
    s32 GetRankDtTech(enRankTech dtNo);
    s32 GetRankDtPow(enRankPow dtNo);
    s32 GetRankDtPow2(enRankPow dtNo,s32 rankDt);
    s32 GetRankDtHP(enRankHP dtNo);
    struct TStHit
    {
      f32 flyAngle;
      s32 flySpdY;
      s32 flySpdX;
      BOOL effFly_f;//特殊吹っ飛びフラグ
      BOOL flySpGrav_f;//重力が関わる特殊吹っ飛びフラグ

      s32 shrank_;//投げ手のランク
      s32 tecrank_;//技量比ランク
      s32 powrank_;//威力比ランク
      s32 hprank_;//のこりＨＰランク
      s32 dam_;
      s32 cadam_;
      BOOL sp_f_;//必殺シュート
      BOOL enshot_f_;////敵のシュート
      BOOL kezuridead_f_;//削りで死ぬ状態
      void Init() { Zeromem(this); }
    };
    TStHit stHit_;
  public:

    //コンストラクタ
    explicit TBaJudge(TGame* pGame, TBaCommon* pCommon);
    virtual ~TBaJudge(void);

    //更新
    void UpDate();

  };


}
