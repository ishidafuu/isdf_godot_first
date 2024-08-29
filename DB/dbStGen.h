#pragma once
#include "dbInclude.h"

namespace db
{
  
  //向き
  enum enMukiType
  {
    mL = -1,
    mN = 0, //0がデフォ値で入ってるせいか、これなしでXML生成しようとするとエラー
    mR = +1
  };
  
  //奥行き向き
  enum enMukiZType
  {
    mzF,
    mzN,
    mzB
  };
  
  //ダッシュマンタイプ
  enum enDashmanType
  {
    dmtNone,
    dmtStr,//ストレート
    dmtUp,//上ライン
    dmtDw,//下ライン
    dmtAp,//アプローチ
    //dmtCr,//クロス
  };
  
  //コールタイプ
  enum enCallType
  {
    cltNone,
    cltPass,
    cltDash,
  };
  
  
  
  //状態クラス（データ管理のみで基本は被参照。外部への動作はしない）
  struct TZahyouRec
  {
    enMukiType Muki;//向き
    enMukiZType MukiZ;//奥行き向き
    enMukiType DsMuki;//ダッシュ方向
    s32 X;
    s32 Y;
    s32 Z;//座標
    s32 dX;
    s32 dY;
    s32 dZ;//速度
    s32 Fric_c;
    s32 Pri;
    void Init()
    {
      Zeromem(this);
    }
    s32 SigndX()
    {
      if (dX > 0)
      {
        return +1;
      }
      else if (dX < 0)
      {
        return -1;
      }
      else
      {
        return 0;
      }
    }
    s32 SigndZ()
    {
      if (dZ > 0)
      {
        return +1;
      }
      else if (dZ < 0)
      {
        return -1;
      }
      else
      {
        return 0;
      }
    }
  };
}
