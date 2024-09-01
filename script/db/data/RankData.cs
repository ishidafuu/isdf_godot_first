using System;

namespace db;

public class RankData
{
    public static int AllRankLength => Enum.GetValues<RankLevelType>().Length
                                       + Enum.GetValues<RankSpeedType>().Length
                                       + Enum.GetValues<RankHpType>().Length
                                       + Enum.GetValues<RankTechType>().Length
                                       + Enum.GetValues<RankPowType>().Length;

    public int[][] Sheet { get; private set; }

    public RankData()
    {
        var allRankLength = AllRankLength;
        Sheet = new int[allRankLength][];

        for (var i = 0; i < allRankLength; i++)
        {
            Sheet[i] = new int[Defines.RANKNUM];
        }
    }

    public void Load()
    {
    }

    /// <summary>
    /// レベルランク値取得
    /// </summary>
    public int GetLevel(RankLevelType levelType, int rank)
    {
        var index = (int)levelType;
        return Sheet[index][rank];
    }

    /// <summary>
    /// スピードランク値取得
    /// </summary>
    public int GetSpeed(RankSpeedType speedType, int rank)
    {
        var index = Enum.GetValues<RankLevelType>().Length
                    + (int)speedType;
        return Sheet[index][rank];
    }

    /// <summary>
    /// HPランク値取得
    /// </summary>
    public int GetHp(RankHpType hpType, int rank)
    {
        var index = Enum.GetValues<RankLevelType>().Length
                    + Enum.GetValues<RankSpeedType>().Length
                    + (int)hpType;
        return Sheet[index][rank];
    }

    /// <summary>
    /// てくランク値取得
    /// </summary>
    public int GetTech(RankTechType techType, int rank)
    {
        var index = Enum.GetValues<RankLevelType>().Length
                    + Enum.GetValues<RankSpeedType>().Length
                    + Enum.GetValues<RankHpType>().Length
                    + (int)techType;
        return Sheet[index][rank];
    }

    /// <summary>
    /// ぱわーランク値取得
    /// </summary>
    public int GetPower(RankPowType powType, int rank)
    {
        var index = Enum.GetValues<RankLevelType>().Length
                    + Enum.GetValues<RankSpeedType>().Length
                    + Enum.GetValues<RankHpType>().Length
                    + Enum.GetValues<RankTechType>().Length
                    + (int)powType;
        return Sheet[index][rank];
    }

    /// <summary>
    /// てく比ランク取得
    /// </summary>
    public int GetTechRateRank(int tech, int targetTech)
    {
        var rate = tech * Defines.Percent / targetTech;
        for (var i = 0; i < Defines.RANKNUM; i++)
        {
            if (rate <= GetTech(RankTechType.TechtoRank, i))
            {
                return i;
            }
        }

        return Defines.RANKNUM - 1;
    }

    /// <summary>
    /// ぱわー比ランク取得
    /// </summary>
    public int GetPowerRateRank(int pow, int targetPower)
    {
        var rate = pow * Defines.Percent / targetPower;
        for (var i = 0; i < Defines.RANKNUM; i++)
        {
            if (rate <= GetPower(RankPowType.PowtoRank, i))
            {
                return i;
            }
        }

        return Defines.RANKNUM - 1;
    }

    /// <summary>
    /// HP割合ランク取得
    /// </summary>
    public int GetHpRateRank(int hp, int maxHp)
    {
        var rate = hp * Defines.Percent / maxHp;
        for (var i = 0; i < Defines.RANKNUM; i++)
        {
            if (rate <= GetHp(RankHpType.HPtoRank, i))
            {
                return i;
            }
        }

        return Defines.RANKNUM - 1;
    }

//     //読み込み
//     void LoadDt(const char *CSVDir, const char *CSVFilename_ys, const char *CSVFilename_rs);
//     //取得
//     s32 GetDt(enDtComAct dtNo, s32 ptn, BOOL ressei_f);//参照CDtの取得
}