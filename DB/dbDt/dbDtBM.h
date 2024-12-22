#pragma once
#include "dbInclude.h"

namespace db
{

  //ループ設定
  enum enBMLoopSt
  {
    bmlp_None,//無関係
    bmlp_St,//開始位置
    bmlp_Ed,//終了位置
    bmlp_StEd,//同コマループ
  };

  //BMグラフィック
  enum enBMCommonDt
  {
    bmc_bmpNo,//絵番号
    bmc_posX,//Ｘ位置
    bmc_posY,//Ｙ位置
    bmc_se,//SE
    bmc_loopNum,//ループ回数
    bmc_DefFrm,//デフォフレーム数
    bmc_loopSt,//ループ設定
    bmc_seloop_f,//ループ時SEも鳴らし直す
    bmc_ActP_f,//アクションポイント
    bmc_END,
  };


  struct BMKomaData
  {
    s32 dt_[bmc_END];
  };

  struct BMData
  {
    s32 komanum_;
    std::vector<BMKomaData> komadt_;
  };

  class TBMData
  {
  protected:
    std::vector<BMData> bmdt_;
  public:
    explicit TBMData(void);
    virtual ~TBMData(void);

    //読み込み
    void LoadDt(const char *CSVDir, const char *CSVFilename);
    //取得
    s32 Getkomanum(s32 bmNo);
  //取得
    s32 GenGetDt(s32 bmNo, s32 komaNo, s32 dtNo);
    s32 GetDt(s32 bmNo, s32 komaNo, enBMCommonDt dtNo);
    //s32 GetDt(s32 bmNo, s32 komaNo, enBMEffectDt dtNo);

  };
}
