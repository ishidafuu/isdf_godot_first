#include "dbStSnOption.h"
#include "dbMgData.h"

namespace db
{
  //オプションステート*********************************************
  
  //コンストラクタ
  TStSnOption::TStSnOption()
  {
    Init();
  }
  TStSnOption::~TStSnOption()
  {
  }
  
  //初期化
  void TStSnOption::Init()
  {
    Zeromem(this);
  }
  
  //Lv増減
  BOOL TStSnOption::FixLvRef(s32* tLv, BOOL inc_f)
  {
    if (inc_f)
    {
      if (*tLv < (s32)dblLv4)
      {
        *tLv += 1;
        return TRUE;
      }
      else
      {
        return FALSE;
      }
    }
    else
    {
      if (*tLv > (s32)dblLv0)
      {
        *tLv -= 1;
        return TRUE;
      }
      else
      {
        return FALSE;
      }
    }
  }
 

  //値増減（直接レベル設定）
  void TStSnOption::FixOptStDirect(enDtCustom cstm, enDBLv setLv)
  {
    States[(s32)cstm] = (s32)setLv;
  }
  //値の取得
  s32 TStSnOption::GetSt(enDtCustom cstm)
  {
    
    switch (cstm)
    {
    case cstmOpShSpd:
      return DEFOPTLV;
      case cstm_END:
        return 0;
      case cstmOpBGMVol:
      case cstmOpSEVol:
        return (States[(s32)cstm] * 5);
      default:
        return States[(s32)cstm];
    }
  }
  
  enDtShot TStSnOption::GetShSp()
  {
    if (GetSt(cstmOpShSpd) < 2)
    {
      return shdtSpeedSlw;
    }
    else if (GetSt(cstmOpShSpd) > 2)
    {
      return shdtSpeedHi;
    }
    else
    {
      return shdtSpeedNml;
    }
  }
  
  //値の取得（そのまま返す）保存用
  s32 TStSnOption::StDirectGet(enDtCustom cstm)
  {
    return States[(s32)cstm];
  }
  //値の代入（そのまま入れる）読込用
  void TStSnOption::StDirectSet(enDtCustom cstm, s32 dt)
  {
    States[(s32)cstm] = dt;
  }

  //バッファに保存
  void TStSnOption::SaveBuf(u8* pbuf)
  {
    //Optionのぶんの1KB
    mid::midSaveBuf(pbuf, 1, States, (sizeof(s32) * SaveLen));
  }
  //バッファから読み込み
  void TStSnOption::LoadBuf(u8* pbuf)
  {
    mid::midLoadBuf(pbuf, States, (sizeof(s32) * SaveLen));
  }
 
}
