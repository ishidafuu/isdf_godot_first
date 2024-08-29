#include "dbDtRank.h"

namespace db
{
    TDtRank::TDtRank(void)
    {
    }

    TDtRank::~TDtRank(void)
    {
    }

    //読み込み
    void TDtRank::LoadDt(const char *CSVDir, const char *CSVFilename)
    {
        mid::TCSV csv;
        csv.midLoadCSV(CSVDir, CSVFilename, FALSE, TRUE);

        for (s32 i = 0; i < csv.longvec2_.size(); ++i)
        {
            
            //範囲オーバー
            if (i >= enbtRankDataEND) break;

            for (s32 i2 = 0; i2 < csv.longvec2_[0].size(); ++i2)
            {

                dt_[i][i2] = csv.longvec2_[i][i2];
            }
        }
    }

    s32 TDtRank::GenGetRankDt(s32 dtNo, s32 rank)
    {
        rank = lib_num::Between(rank, 0, (RANKNUM - 1));
        return dt_[dtNo][rank];
    }
    s32 TDtRank::GetRankDtLevel(enRankLevel dtNo, s32 rank)
    {
        return GenGetRankDt((s32)dtNo, rank);
    }
    s32 TDtRank::GetRankDtSpeed(enRankSpeed dtNo, s32 rank)
    {
        return GenGetRankDt((s32)dtNo, rank);
    }
    s32 TDtRank::GetRankDtHP(enRankHP dtNo, s32 rank)
    {
        return GenGetRankDt((s32)dtNo, rank);
    }
    s32 TDtRank::GetRankDtTech(enRankTech dtNo, s32 rank)
    {
        return GenGetRankDt((s32)dtNo, rank);
    }
    s32 TDtRank::GetRankDtPow(enRankPow dtNo, s32 rank)
    {
        return GenGetRankDt((s32)dtNo, rank);
    }

    s32 TDtRank::GetRankTech(s32 shtech, s32 hitmantech)
    {
        s32 res = 0;
        s32 techrt = (shtech* XYMAG) / hitmantech;
        for (s32 i = 0; i < RANKNUM; ++i)
        {
            if (techrt <= GenGetRankDt(rkTechtoRank, i))
            {
                res = i;
                break;
            }
        }
        return res;
    }
    s32 TDtRank::GetRankPow(s32 shpow, s32 hitmanpow)
    {
        s32 res = 0;
        s32 powrt = (shpow* XYMAG) / hitmanpow;
        for (s32 i = 0; i < RANKNUM; ++i)
        {
            if (powrt <= GenGetRankDt(rkPowtoRank, i))
            {
                res = i;
                break;
            }
        }
        return res;
    }
    s32 TDtRank::GetRankHP(s32 hp, s32 maxhp)
    {
        s32 res = 0;
        s32 hpper = (hp * 100) / maxhp;
        for (s32 i = 0; i < RANKNUM; ++i)
        {
            if (hpper <= GenGetRankDt(rkHPtoRank, i))
            {
                res = i;
                break;
            }
        }
        return res;
    }
}
