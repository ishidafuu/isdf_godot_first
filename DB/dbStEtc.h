#pragma once
#include "dbStGen.h"

namespace db
{
  //オブジェステータス************************************

  //オブジェ状態
  enum enEtcObjMotion
  {
    etomNone,
    etomAngel,
    etomItem,
    etomItemMv,
    etomItemStop,
  };

  struct TStEtcMv
  {
    s32 eat_;
    s32 stx_;
    s32 sty_;
    s32 stz_;
    s32 tagx_;
    s32 tagy_;
    s32 tagz_;

    void Init(s32 eat, s32 stx, s32 sty, s32 stz, s32 tagx, s32 tagy, s32 tagz)
    {
      Zeromem(this);
      eat_ = eat;
      stx_ = stx;
      sty_ = sty;
      stz_ = stz;
      tagx_ = tagx;
      tagy_ = tagy;
      tagz_ = tagz;
    }
  };

  struct TStEtc
  {
    TZahyouRec Zahyou;
    TStEtcMv stMove_;

    enEtcObjMotion MType;
    s32 M_c;
    s32 AniNo;
    s32 SideNo;
    s32 CharNo;

    void Init()
    {
      Zeromem(this);
    }
  };



  //セリフステータス************************************
  const s32 SERIFUMAX = 128;

  struct TStSerifu
  {
    s32 LineNo;
    char SerifuStr[SERIFUMAX];
    s32 Speak_c;//プレーヤーからのインクリメント
    s32 End_c;
    s32 CharNo;

    void Init()
    {
      Zeromem(this);
      CharNo = NGNUM;
      //      SerifuStr[0];
    }

    //空ライン
    BOOL IsEmp()
    {
      return (SerifuStr[0] == '\0');
    }
  };




}
