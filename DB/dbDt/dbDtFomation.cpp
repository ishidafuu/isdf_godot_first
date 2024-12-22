
#include "dbDtFomation.h"

namespace db
{
  TDtFomation::TDtFomation(void)
  {
  }

  TDtFomation::~TDtFomation(void)
  {
  }

  //読み込み
  void TDtFomation::LoadDt(const char *CSVDir, const char *CSVFilename)
  {
    mid::TCSV csv;
    csv.midLoadCSV(CSVDir, CSVFilename, FALSE, FALSE);

    //フォーメーション配置
    for (s32 i = 0; i < DBFOMMAX; ++i)
    {
      name_[i] = csv.strvec2_[i][0];
      for (s32 i2 = 0; i2 < fom_END; ++i2)
      {
        fomdt_[i][i2] = csv.longvec2_[i][i2];
      }
    }
    
    for (s32 i = 0; i < cmvEND; ++i)
    {
      for (s32 i2 = 0; i2 < COMPTN; ++i2)
      {
        comdt_[i][i2] = csv.longvec2_[DBFOMMAX + i][i2];
      }
    }
    ////COMポジション
    //for (s32 i = 0; i < DBCOMPOSMAX; ++i)
    //{
    //  for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
    //  {
    //    posdt_[i][i2] = csv.intvec2_[DBFOMMAX + i][i2];
    //  }
    //}
  }

  std::string TDtFomation::GetName(s32 fomNo)
  {
    return name_[fomNo];
  }

  s32 TDtFomation::GenGetDt(s32 fomNo, s32 dtNo)
  {
    return fomdt_[fomNo][dtNo];
  }
  s32 TDtFomation::GetDt(s32 fomNo, enDtFomation dtNo)
  {
    return GenGetDt(fomNo, dtNo);
  }

  s32 TDtFomation::LineOfPos(s32 fomNo, s32 posNo)//何衛か
  {
    s32 linenum = GetDt(fomNo, fomRPos);//後衛
    if (posNo < linenum) return 0;//後衛

    linenum += GetDt(fomNo, fomMPos);//中衛
    if (posNo < linenum) return 1;//中衛

    return 2;//前衛
  }
  s32 TDtFomation::RowOfPos(s32 fomNo, s32 posNo)//衛の上から何番目か
  {
    s32 rlinenum = GetDt(fomNo, fomRPos);//後衛
    s32 mlinenum = GetDt(fomNo, fomMPos);//中衛
    s32 flinenum = GetDt(fomNo, fomFPos);//前衛

    s32 line = LineOfPos(fomNo, posNo);//衛
    s32 linenum = GenGetDt(fomNo, (fomRPos + line));//衛の人数
    s32 res = 0;

    s32 linerow0 = 0;//その衛の一番若い番号

    switch (line)
    {
    case 0://後衛
      linerow0 = 0;
      break;
    case 1://中衛
      linerow0 = rlinenum;
      break;
    case 2://前衛
      linerow0 = rlinenum + mlinenum;
      break;
    }

    switch (linenum)
    {
    case 1:
      res = linerow0;
      break;
    case 2:
      res = (posNo - linerow0);
      break;
    case 3:
      switch (posNo)
      {
      case 1:res = 0; break;
      case 2:res = 2;break;
      default:res = 1;break;
      }
      //if (posNo == linerow0)
      //{
      //  res = 1;
      //}
      //else if (posNo == (linerow0 + 1))
      //{
      //  res = 0;
      //}
      //else if (posNo == (linerow0 + 2))
      //{
      //  res = 2;
      //}
      break;
    case 4:
      if (posNo == linerow0)
      {
        res = 1;
      }
      else if (posNo == (linerow0 + 1))
      {
        res = 2;
      }
      else if (posNo == (linerow0 + 2))
      {
        res = 0;
      }
      else if (posNo == (linerow0 + 3))
      {
        res = 3;
      }
      break;
    }

    return res;
  }

  s32 TDtFomation::GetComDt(enComValue dtNo, s32 ptnNo)
  {
    return comdt_[(s32)dtNo][ptnNo];
  }
}
