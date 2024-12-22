#include "dbDtShElm.h"

namespace db
{
  TDtShElm::TDtShElm(void)
  {
    intvec_genre_.push_back(0);
    strvec_genre_.push_back("ぶんれつ");
    intvec_genre_.push_back(100);
    strvec_genre_.push_back("いろかえ");
    intvec_genre_.push_back(200);
    strvec_genre_.push_back("かたちかえ");
    intvec_genre_.push_back(1000);
    strvec_genre_.push_back("ぜんごなみ");
    intvec_genre_.push_back(1100);
    strvec_genre_.push_back("たてなみ");
    intvec_genre_.push_back(1200);
    strvec_genre_.push_back("サイドなみ");
    intvec_genre_.push_back(2000);
    strvec_genre_.push_back("そくど");
    intvec_genre_.push_back(2100);
    strvec_genre_.push_back("ふっとび");
    intvec_genre_.push_back(3000);
    strvec_genre_.push_back("きょりへんか");
    intvec_genre_.push_back(4000);
    strvec_genre_.push_back("とくしゅ");
    intvec_genre_.push_back(5000);
    strvec_genre_.push_back("らっかきどう");


    intvec_genre8_.push_back(0);
    intvec_genre8_.push_back(100);
    //intvec_genre_.push_back(200);
    intvec_genre8_.push_back(1000);
    //intvec_genre_.push_back(1100);
    //intvec_genre_.push_back(1200);
    intvec_genre8_.push_back(2000);
    intvec_genre8_.push_back(2100);
    intvec_genre8_.push_back(3000);
    intvec_genre8_.push_back(4000);
    intvec_genre8_.push_back(5000);

  }
  TDtShElm::~TDtShElm(void)
  {
  }

  //ジャンル////////////////////////////////////////
  s32 TDtShElm::GetGenreNum()
  {
    return intvec_genre_.size();
}
  std::string TDtShElm::GetGenreNameLine(s32 no)
  {
    return strvec_genre_[no];
  }
  s32 TDtShElm::GetGenreID(s32 no)
  {
    return intvec_genre_[no];
  }
  //ジャンル選別
  s32 TDtShElm::GetGenre(s32 pk)
  {
    s32 res = 0;
    s32 stpos = 0;
    for (s32 i = 0; i < intvec_genre_.size(); ++i)
    {
      if ((i + 1) == intvec_genre_.size())
      {
        res = i;
        break;
      }
      else if (pk < intvec_genre_[i + 1])
      {
        res = i;
        break;
      }
    }
    return res;
  }
  //ジャンル選別'(8type)
  s32 TDtShElm::GetGenre8(s32 pk)
  {
    s32 res = 0;
    s32 stpos = 0;
    for (s32 i = 0; i < intvec_genre8_.size(); ++i)
    {
      if ((i + 1) == intvec_genre8_.size())
      {
        res = i;
        break;
      }
      else if (pk < intvec_genre8_[i + 1])
      {
        res = i;
        break;
      }
    }
    return res;
  }
  //ジャンル名
  std::string TDtShElm::GetGenreNamePK(s32 pk)
  {
    return strvec_genre_[GetGenre(pk)];
  }

  //素材////////////////////////////////////////

  //読み込み
  void TDtShElm::LoadDt(const char *CSVDir, const char *CSVFilename)
  {
    intvec2_.clear();
    strvec2_.clear();

    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, CSVFilename, FALSE, FALSE);
    for (s32 i = 0; i < csv.GetLine(); ++i)
    {
      INTVECTOR intvec;
      STRVECTOR strvec;
      for (s32 i2 = 0; i2 < csv.GetIntSize(0); ++i2) intvec.push_back(csv.GetInt(i, i2));
      for (s32 i2 = 0; i2 < csv.GetStrSize(0); ++i2) strvec.push_back(csv.GetStr(i, i2));
      s32 pk = intvec[0];//最初の要素がＰＫのはず
      intvec2_[pk] = intvec;
      strvec2_[pk] = strvec;
    }
  }


  STRVECTOR TDtShElm::GetText(s32 pk)
  {
    STRVECTOR res;
    res.push_back(strvec2_[pk][szs_text0]);
    res.push_back(strvec2_[pk][szs_text1]);
    res.push_back(strvec2_[pk][szs_text2]);
    res.push_back(strvec2_[pk][szs_text3]);
    return res;
  }
  s32 TDtShElm::GetDt(s32 pk, enDtShElm dtNo)
  {
    return intvec2_[pk][(s32)dtNo];
  }
  std::string TDtShElm::GetDtStr(s32 pk, enDtShElmStr dtNo)
  {
    return strvec2_[pk][dtNo];
  }
  
  //属性ランプフラグ
  s32 TDtShElm::GetLightFlag(s32 pk)
  {
    s32 res = 0;
    const s32 LIGHTNUM = 8;
    for (s32 i = 0; i < LIGHTNUM; ++i)
    {
      if (GetDt(pk, (enDtShElm)(sz_light1 + i)) != 0)
      {
        res |= (1 << i);
      }
    }
    return res;
  }

  BOOL TDtShElm::IsLightFlag(s32 pk, s32 genreno)
  {
    return (GetDt(pk, (enDtShElm)(sz_light1 + genreno)) != 0);
  }

#if defined(__K_DEBUG_SHIAI__) || defined(__K_DEBUG_USER_DATA__)
  s32 TDtShElm::GetSize(void)
  {
    s32 ret = intvec2_.size();
    return ret;
  }

  s32 TDtShElm::GetKey(s32 no)
  {
    s32 ret = -1;
    s32 cnt = 0;
    for (PKINTVECMAP::iterator itr = intvec2_.begin(); itr != intvec2_.end(); itr++, cnt++)
    {
      if (cnt == no)
      {
        ret = itr->first;
        break;
      }
    }
    return ret;
  }
#endif	// #if defined(__K_DEBUG_SHIAI__) || defined(__K_DEBUG_USER_DATA__)
}
