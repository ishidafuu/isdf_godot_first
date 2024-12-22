#include "dbDtBallAnime.h"

namespace db
{
    TDtBallAnime::TDtBallAnime(void)
    {
    }
    
    TDtBallAnime::~TDtBallAnime(void)
    {
    }

    void TDtBallAnime::GenLoadDt(const char *CSVDir, const char *CSVFilename, INTVECTOR2* intvec2)
    {
      intvec2->clear();

      mid::TCSV csv;
      csv.midLoadCSV(CSVDir, CSVFilename, FALSE, TRUE);//NUMONLY
      for (s32 i = 0; i < csv.GetLine(); ++i)
      {
        INTVECTOR intvec;
        intvec2->push_back(intvec);
        for (s32 i2 = 0; i2 < csv.GetIntSize(0); ++i2) intvec2->at(i).push_back(csv.GetInt(i, i2));
      }
    }
    //読み込み（追記対応）
    void TDtBallAnime::LoadDt(const char *CSVDir, const char *CSV_Patt, const char *CSV_Data, const char *CSV_Set)
    {
      GenLoadDt(CSVDir, CSV_Patt, &anipatt_);
      GenLoadDt(CSVDir, CSV_Data, &anidata_);
      GenLoadDt(CSVDir, CSV_Set, &aniset_);
    }

    s32 TDtBallAnime::GetAniPatt(s32 id, enBallAnimePatt dtNo)
    {
      return anipatt_[id][dtNo];
    }
    s32 TDtBallAnime::GetAniData(s32 id, enBallAnimeData dtNo)
    {
      return anidata_[id][dtNo];
    }
    s32 TDtBallAnime::GetAniSet(s32 id, enBallAnimeSet dtNo)
    {
      return aniset_[id][dtNo];
    }



}
