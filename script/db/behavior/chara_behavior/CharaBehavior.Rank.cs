namespace db;

public partial class CharaBehavior
{
    /// <summary>
    /// レベル→ランク変換
    /// </summary>
    private int GetLevelRank(RankLevelType levelType)
    {
        return MasterManager.Instance.RankMaster.GetLevel(levelType, Level.Level);
    }

    /// <summary>
    /// スピード→ランク変換
    /// </summary>
    private int GetSpeedRank(RankSpeedType speedType)
    {
        return MasterManager.Instance.RankMaster.GetSpeed(speedType, Level.Speed);
    }

    /// <summary>
    /// HP→ランク変換
    /// </summary>
    private int GetHpRank(RankHpType hpType)
    {
        var hpRateRank = MasterManager.Instance.RankMaster.GetHpRateRank(Live.Hp, Live.MaxHp);
        return MasterManager.Instance.RankMaster.GetHp(hpType, hpRateRank);
    }

    /// <summary>
    /// てく比→ランク変換
    /// </summary>
    private int GetTechRank(RankTechType techType, int targetTech)
    {
        var techRate = MasterManager.Instance.RankMaster.GetTechRateRank(Level.Tech, targetTech);
        return MasterManager.Instance.RankMaster.GetTech(techType, techRate);
    }

    /// <summary>
    /// ぱわー比→ランク変換
    /// </summary>
    private int GetPowerRank(RankPowerType powerType, int targetPower)
    {
        var powerRate = MasterManager.Instance.RankMaster.GetPowerRateRank(Level.Power, targetPower);
        return MasterManager.Instance.RankMaster.GetPower(powerType, powerRate);
    }

    /// <summary>
    /// 歩き移動量
    /// </summary>
    private int WalkVelocityX => GetSpeedRank(RankSpeedType.WkX);
    private int WalkVelocityZ => WalkVelocityX * Defines.Percent / GetSettingCourt(SetingCourtType.WkZPer);
    
    /// <summary>
    /// 斜め歩き移動量
    /// </summary>
    private int DiagonalWalkVelocityX => WalkVelocityX * Defines.Percent / GetSettingCourt(SetingCourtType.NWkXPer);
    private int DiagonalWalkVelocityZ => WalkVelocityX * Defines.Percent / GetSettingCourt(SetingCourtType.NWkZPer);

    /// <summary>
    /// 歩きジャンプ移動量
    /// </summary>
    private int WalkJumpVelocityX => GetSpeedRank(RankSpeedType.WkJPX);
    private int WalkJumpVelocityZ => WalkJumpVelocityX * Defines.Percent / GetSettingCourt(SetingCourtType.WkZPer);
    
    /// <summary>
    /// 斜め歩きジャンプ移動量
    /// </summary>
    private int DiagonalWalkJumpVelocityX => WalkJumpVelocityX * Defines.Percent / GetSettingCourt(SetingCourtType.NWkXPer);
    private int DiagonalWalkJumpVelocityZ => WalkJumpVelocityX * Defines.Percent / GetSettingCourt(SetingCourtType.NWkZPer);

}