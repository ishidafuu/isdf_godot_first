namespace db;

public partial class CharaBehavior
{
    private int GetLevelRank(RankLevelType levelType)
    {
        return MasterManager.Instance.RankMaster.GetLevel(levelType, MyState.Level.Level);
    }

    private int GetSpeedRank(RankSpeedType speedType)
    {
        return MasterManager.Instance.RankMaster.GetSpeed(speedType, MyState.Level.Speed);
    }

    private int GetHpRank(RankHpType hpType)
    {
        var hpRateRank = MasterManager.Instance.RankMaster.GetHpRateRank(MyState.Live.Hp, MyState.Live.MaxHp);
        return MasterManager.Instance.RankMaster.GetHp(hpType, hpRateRank);
    }

    private int GetTechRank(RankTechType techType, int targetTech)
    {
        var techRate = MasterManager.Instance.RankMaster.GetTechRateRank(MyState.Level.Tech, targetTech);
        return MasterManager.Instance.RankMaster.GetTech(techType, techRate);
    }

    private int GetPowerRank(RankPowerType powerType, int targetPower)
    {
        var powerRate = MasterManager.Instance.RankMaster.GetPowerRateRank(MyState.Level.Power, targetPower);
        return MasterManager.Instance.RankMaster.GetPower(powerType, powerRate);
    }

    private BaseMotionData GetBaseMotionData()
    {
        return MasterManager.Instance.BaseMotionMaster.Get(MyState.Motion.MotionType);
    }

    private BaseMotionKomaData GetBaseMotionKomaData()
    {
        return GetBaseMotionData().Get(MyState.Motion.MotionKomaNo);
    }

}