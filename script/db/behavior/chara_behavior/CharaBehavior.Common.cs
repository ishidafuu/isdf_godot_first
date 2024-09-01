namespace db;

public partial class CharaBehavior
{

    /// <summary>
    /// サイド操作権を渡せるキャラか
    /// </summary>
    private bool CanControl()
    {
        // 死亡していない、ダメージ中でない、手動操作中でない
        return MyState.Live.IsDead == false
               && MyState.Motion.HasFlag(CharaMotionFlag.Dam) == false
               && MyState.Pad.IsManualControl == false;
    }

    /// <summary>
    /// ボールとの距離を計算
    /// </summary>
    private void UpdateBallDistance()
    {
        var ballDist = 0;
        var ballLandDist = 0;
        var ballLandLineDist = 0;

        // 操作可能キャラのみ
        if (CanControl())
        {
            ballDist = MyState.Coordinate.DistanceXZ(BallState.Coordinate);
            ballLandDist = ballDist;
            ballLandLineDist = ballDist;

            // 着地予定のみ
            if (BallState.MotionType is BallMotionType.Bound or BallMotionType.Pass)
            {
                ballLandDist = MyState.Coordinate.DistanceXZ(BallState.LandX, BallState.LandZ);

                // ライン超える場合のみ
                ballLandLineDist = BallState.LandLine
                    ? MyState.Coordinate.DistanceXZ(BallState.LandXLine, BallState.LandZLine)
                    : ballLandDist;
            }
        }

        MyState.Distance.Set(ballDist, ballLandDist, ballLandLineDist);
    }

    /// <summary>
    /// ボール持ちかどうか
    /// </summary>
    private bool IsBallHolder()
    {
        return BallState.IsBallHolder(MySideIndex, OrderIndex);
    }

    /// <summary>
    /// 自分がシュートターゲット
    /// </summary>
    private bool IsShotTarget()
    {
        return BallState.IsShotTarget(MySideIndex, OrderIndex);
    }

    /// <summary>
    /// 自分がパスターゲット
    /// </summary>
    private bool IsPassTarget()
    {
        return BallState.IsPassTarget(MySideIndex, OrderIndex);
    }

    /// <summary>
    /// パス待ち状態
    /// </summary>
    private bool IsPassWait()
    {
        return IsPassTarget()
               && IsBallHolder() == false
               && MyState.Pad.IsManualControl;
        // && IsShiai() //試合中チェック
    }

    private int GetLevelRank(RankLevelType levelType)
    {
        return DataManager.Instance.RankData.GetLevel(levelType, MyState.Level.Level);
    }

    private int GetSpeedRank(RankSpeedType speedType)
    {
        return DataManager.Instance.RankData.GetSpeed(speedType, MyState.Level.Speed);
    }

    private int GetHpRank(RankHpType hpType)
    {
        var hpRateRank = DataManager.Instance.RankData.GetHpRateRank(MyState.Live.Hp, MyState.Live.MaxHp);
        return DataManager.Instance.RankData.GetHp(hpType, hpRateRank);
    }

    private int GetTechRank(RankTechType techType, int targetTech)
    {
        var techRate = DataManager.Instance.RankData.GetTechRateRank(MyState.Level.Tech, targetTech);
        return DataManager.Instance.RankData.GetTech(techType, techRate);
    }

    private int GetPowerRank(RankPowerType powerType, int targetPower)
    {
        var powerRate = DataManager.Instance.RankData.GetPowerRateRank(MyState.Level.Power, targetPower);
        return DataManager.Instance.RankData.GetPower(powerType, powerRate);
    }
}