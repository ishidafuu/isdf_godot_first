#pragma once
#include "dbInclude.h"
//#include "dbMgStScene.h"

namespace db
{
  enum enBallAnimePatt
  { 
    bad_pttID,//パターン番号
    bad_pttGr1Num,//パターン１
    bad_pttGr1Rev,//パターン１反転
    bad_pttGr2Num,//パターン２
    bad_pttGr2Rev,//パターン２反転
    bad_pttGr3Num,//パターン３
    bad_pttGr3Rev,//パターン３反転
    bad_pttGr4Num,//パターン４
    bad_pttGr4Rev,//パターン４反転
    bad_pttLpSrt,//ループ開始パターン数
    bad_pttLpEnd,//ループ終了パターン数
  };

  enum enBallAnimeData
  {
    bad_anmID,//アニメ番号
    bad_anmTyp,//アニメタイプ
    bad_anmSym,//反転しないフラグ＋消しフラグ
    bad_anmFrm,//フレーム数
    bad_anmPtID,//パターン番号
    bad_anmGrhClr,//色替えフラグ
    bad_anmGrId1,//パターン１の画像番号
    bad_anmGrId2,//パターン２の画像番号
    bad_anmGrId3,//パターン３の画像番号
    bad_anmGrId4,//パターン４の画像番号
  };

  enum enBallAnimeSet
  {
    bad_shpId,//形状番号
    bad_shpNormId,//通常時使用アニメ番号
    bad_shpExt1Id,//例外１時使用アニメ番号
    bad_shpExt2Id,//例外２時使用アニメ番号
    bad_shpExAnTp,//特殊アニメタイプ
    bad_shpOFAnId,//特殊アニメ番号
  };

    class TDtBallAnime
    {
    protected:
      void GenLoadDt(const char *CSVDir, const char *CSVFilename, INTVECTOR2* intvec2);
    public:
      INTVECTOR2 anipatt_;
      INTVECTOR2 anidata_;
      INTVECTOR2 aniset_;

      explicit TDtBallAnime(void);
      virtual ~TDtBallAnime(void);
      void LoadDt(const char *CSVDir, const char *CSV_Patt, const char *CSV_Data, const char *CSV_Set);

      s32 GetAniPatt(s32 id, enBallAnimePatt dtNo);
      s32 GetAniData(s32 id, enBallAnimeData dtNo);
      s32 GetAniSet(s32 id, enBallAnimeSet dtNo);
    };
}
