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

    public int Get(RankLevelType levelType, int rank)
    {
        var index = (int)levelType;
        return Sheet[index][rank];
    }

    public int Get(RankSpeedType speedType, int rank)
    {
        var index = Enum.GetValues<RankLevelType>().Length
                    + (int)speedType;
        return Sheet[index][rank];
    }

    public int Get(RankHpType hpType, int rank)
    {
        var index = Enum.GetValues<RankLevelType>().Length
                    + Enum.GetValues<RankSpeedType>().Length
                    + (int)hpType;
        return Sheet[index][rank];
    }

    public int Get(RankTechType techType, int rank)
    {
        var index = Enum.GetValues<RankLevelType>().Length
                    + Enum.GetValues<RankSpeedType>().Length
                    + Enum.GetValues<RankHpType>().Length
                    + (int)techType;
        return Sheet[index][rank];
    }

    public int Get(RankPowType powType, int rank)
    {
        var index = Enum.GetValues<RankLevelType>().Length
                    + Enum.GetValues<RankSpeedType>().Length
                    + Enum.GetValues<RankHpType>().Length
                    + Enum.GetValues<RankTechType>().Length
                    + (int)powType;
        return Sheet[index][rank];
    }

    public int GetTechRank(int tech, int targetTech)
    {
        var techRate = tech * Defines.Percent / targetTech;
        for (var i = 0; i < Defines.RANKNUM; i++)
        {
            if (techRate <= Get(RankTechType.TechtoRank, i))
            {
                return i;
            }
        }

        return Defines.RANKNUM - 1;
    }

    public int GetPowRank(int pow, int targetPow)
    {
        var techRate = pow * Defines.Percent / targetPow;
        for (var i = 0; i < Defines.RANKNUM; i++)
        {
            if (techRate <= Get(RankPowType.PowtoRank, i))
            {
                return i;
            }
        }

        return Defines.RANKNUM - 1;
    }

    public int GetHpRank(int hp, int maxHp)
    {
        var techRate = hp * Defines.Percent / maxHp;
        for (var i = 0; i < Defines.RANKNUM; i++)
        {
            if (techRate <= Get(RankHpType.HPtoRank, i))
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