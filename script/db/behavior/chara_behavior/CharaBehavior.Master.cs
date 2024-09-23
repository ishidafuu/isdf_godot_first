namespace db;

public partial class CharaBehavior
{
    /// <summary>
    /// レベル→ランク変換
    /// </summary>
    private int GetLevelRank(RankLevelType levelType)
    {
        return MasterManager.Instance.RankMaster.GetLevel(levelType, MyState.Level.Level);
    }

    /// <summary>
    /// スピード→ランク変換
    /// </summary>
    private int GetSpeedRank(RankSpeedType speedType)
    {
        return MasterManager.Instance.RankMaster.GetSpeed(speedType, MyState.Level.Speed);
    }

    /// <summary>
    /// HP→ランク変換
    /// </summary>
    private int GetHpRank(RankHpType hpType)
    {
        var hpRateRank = MasterManager.Instance.RankMaster.GetHpRateRank(MyState.Live.Hp, MyState.Live.MaxHp);
        return MasterManager.Instance.RankMaster.GetHp(hpType, hpRateRank);
    }

    /// <summary>
    /// てく比→ランク変換
    /// </summary>
    private int GetTechRank(RankTechType techType, int targetTech)
    {
        var techRate = MasterManager.Instance.RankMaster.GetTechRateRank(MyState.Level.Tech, targetTech);
        return MasterManager.Instance.RankMaster.GetTech(techType, techRate);
    }

    /// <summary>
    /// ぱわー比→ランク変換
    /// </summary>
    private int GetPowerRank(RankPowerType powerType, int targetPower)
    {
        var powerRate = MasterManager.Instance.RankMaster.GetPowerRateRank(MyState.Level.Power, targetPower);
        return MasterManager.Instance.RankMaster.GetPower(powerType, powerRate);
    }
}