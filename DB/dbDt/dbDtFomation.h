#pragma once
#include "dbInclude.h"

namespace db
{
  enum enDtFomation
  {
    fomRPos,//後衛
    fomMPos,//中衛
    fomFPos,//前衛
    fomZWidth,//奥行幅
    fomEtc,//特殊
    fom_END,
  };

  const s32 FOMWIDETYPE = 4;//広がり４種類

  enum enComValue//COM思考用値
  {
    cmvDMWait,//COMDM通常ウエイト
    cmvDMFmWait,//COMDM態勢ウエイト
    cmvDMStWait,//COMDMセッターウエイト
    cmvDMDelay,//COMDMディレイ
    cmvDMBMItv,//COMDMボールマンダッシュインターバル
    cmvDMPaItv,//COMDMパスインターバル
    cmvChMakeLoop,//COMチャンスメイクパスループ
    cmvShPaNum,//COMシュート変更パス
    cmvSgPaWait,//COMシングルパスウエイト
    cmvShWait,//COMパワーシュートウエイト
    cmvDShStep,//COMDSh歩数
    cmvJShTime,//COMJShタイミング
    cmvJShTime2,//COMJShタイミング
    cmvDsPos,//COMダッシュ開始位置
    cmvRandPer,//COM確率（怒高確率）
    cmvEND,
  };

  class TDtFomation
  {
  protected:
    
  public:
    std::string name_[DBFOMMAX];
    s32 fomdt_[DBFOMMAX][fom_END];
    s32 comdt_[cmvEND][COMPTN];//分岐数

    explicit TDtFomation(void);
    virtual ~TDtFomation(void);

    void LoadDt(const char *CSVDir, const char *CSVFilename);

    std::string GetName(s32 fomNo);
    s32 GenGetDt(s32 fomNo, s32 dtNo);
    s32 GetDt( s32 fomNo ,enDtFomation dtNo);
    s32 LineOfPos(s32 fomNo, s32 posNo);//何衛か012
    s32 RowOfPos(s32 fomNo, s32 posNo);//衛の上から何番目か
    s32 GetComDt(enComValue dtNo, s32 ptnNo);//参照CDtの取得
  };
}
