#pragma once
#include "dbStSnGen.h"

namespace db
{
  class TMgData;
  enum enDBLv
  {
    dblLv0,
    dblLv1,
    dblLv2,
    dblLv3,
    dblLv4
  };

  //オプションステート
  class TStSnOption : TStSnGen
  {
  protected:
    
    //Lv増減
    BOOL FixLvRef(s32* tLv, BOOL inc_f);
    
  public:
    
    //オプションステート
    s32 States[cstm_END];
    
    //初期化
    void Init();
    
    //コンストラクタ
    explicit TStSnOption();
    virtual ~TStSnOption(void);
    
    //値増減
    //BOOL FixOptSt(TMgData* pmgDt, enDtCustom cstm, BOOL inc_f, s32 ulst);
    //値増減（直接レベル設定）
    void FixOptStDirect(enDtCustom cstm, enDBLv setLv);
    //値の取得
    s32 GetSt(enDtCustom cstm);
    
    //値の取得（そのまま返す）保存用
    s32  StDirectGet(enDtCustom cstm);
    //値の代入（そのまま入れる）読込用
    void StDirectSet(enDtCustom cstm,s32  dt);
    
    //シュート速度に応じたデータ
    enDtShot GetShSp();
    
    //バッファに保存
    void SaveBuf(u8* pbuf);
    //バッファから読み込み
    void LoadBuf(u8* pbuf);
  };
  
  
}
