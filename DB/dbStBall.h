#pragma once
#include "dbStGen.h"
#include "dbStShot.h"
namespace db
{
  
  enum enAIShType { aisNS, aisHS, aisNml, aisNml2, ais_END };
  
  const s32 AISHTYPENUM = ais_END;
  const s32 RULENUM = (AISHTYPENUM * AISHTYPENUM * AISHTYPENUM);//4*4*4
  
  const s32 OMKSITV = 45;
  
  struct TStAIRule
  {
    enAIShType prvSh;//前の攻撃
    enAIShType prvSh2;//２つ前の攻撃
    enAIShType nextSh;//次の攻撃
    
    BOOL match_f;//マッチした
    s32 weight;//重さ
  };
  
  //当たりやすそうなシュートを推測する用構造体
  struct TStShootAI
  {
    TStAIRule Rules[RULENUM];
    s32 nowRuleNo;//今回のルール
    enAIShType ShMem[3];//攻撃種類保存(今回＋前回＋前々回)
    void Init()
    {
      Zeromem(this);
      
      for (s32 i = 0; i < RULENUM; ++i)
      {
        if (i < (AISHTYPENUM * AISHTYPENUM))
        {
          Rules[i].prvSh = aisNml;
        }
        else if (i < (AISHTYPENUM * AISHTYPENUM * 2))
        {
          Rules[i].prvSh = aisNml2;
        }
        else if (i < (AISHTYPENUM * AISHTYPENUM * 3))
        {
          Rules[i].prvSh = aisNS;
        }
        else
        {
          Rules[i].prvSh = aisHS;
        }
        
        s32 prv2 = (i % (AISHTYPENUM * AISHTYPENUM));
        if (prv2 < AISHTYPENUM)
        {
          Rules[i].prvSh2 = aisNml;
        }
        else if (prv2 < (AISHTYPENUM * 2))
        {
          Rules[i].prvSh2 = aisNml2;
        }
        else if (prv2 < (AISHTYPENUM * 3))
        {
          Rules[i].prvSh2 = aisNS;
        }
        else
        {
          Rules[i].prvSh2 = aisHS;
        }
        
        s32 next = (i % AISHTYPENUM);
        if (next == 0)
        {
          Rules[i].nextSh = aisNml;
        }
        else if (next == 1)
        {
          Rules[i].nextSh = aisNml2;
        }
        else if (next == 2)
        {
          Rules[i].nextSh = aisNS;
        }
        else
        {
          Rules[i].nextSh = aisHS;
        }
      }
    }
    void SetSh(s32 nowsh)
    {
      ShMem[2] = ShMem[1];
      ShMem[1] = ShMem[0];
      ShMem[0] = (enAIShType)nowsh;
      
      for (s32 i = 0; i < RULENUM; ++i)
      {
        if ((Rules[i].prvSh2 == ShMem[2])
            && (Rules[i].prvSh == ShMem[1])
            && (Rules[i].nextSh == ShMem[0]))
        {
          nowRuleNo = i;
          break;
        }
      }
    }
    void SetResult(BOOL hit_f)
    {
      if (hit_f)
      {
        ++Rules[nowRuleNo].weight;
      }
      else
      {
        --Rules[nowRuleNo].weight;
      }
    }
    s32 GetExp()
    {
      s32 res = NGNUM;
      s32 maxwt = 0;
      s32 minwt = 0;
      
      s32 find_c = 0;
      
      for (s32 i = 0; i < RULENUM; ++i)
      {
        if ((Rules[i].prvSh == ShMem[0])
            && (Rules[i].prvSh2 == ShMem[1]))
        {
          
          //ウエイトが高い奴
          if (maxwt <= Rules[i].weight)
          {
            res = Rules[i].nextSh;
            maxwt = Rules[i].weight;
          }
          
          //低い奴も
          if (minwt > Rules[i].weight)
          {
            minwt = Rules[i].weight;
          }
          ++find_c;
          
          if (find_c >= AISHTYPENUM) break;
        }
      }
      
      //全く同じだったら見つからなかったことに
      if (maxwt == minwt) res = NGNUM;
      
      return res;
    }
  };
  
  //ボール状態
  enum enBallMotion
  {
    bmFree,
    bmHold,
    bmShoot,
    bmPass,
    bmBound,
    bmReferee
  };
  
  enum enPassLine
  {
    plNml,
    pl23,
    pl14,
    pl11,
    plKyo
  };
  
  enum enJumpBall
  {
    jbNormal,
    jbJpball,
    jbSide0,
    jbSide1
  };
  
 
  const s32 BUFPOSMAX = 16;
  
  struct TStBall
  {
    BOOL Invs_f;
    BOOL LandLine_f;//またぐよ
    BOOL LandSet_f;
    BOOL Land_f;//落ちる予定
    BOOL LowBound_f;
    BOOL MvCtrl_f;
    BOOL OvLine_f;
    BOOL PaAlPa_f;
    BOOL PaJPa_f;
    BOOL ShootFirstBound_f;//シュート着地１回目のバウンド
    BOOL Start_f;
    BOOL counter_f;
    BOOL shotmotion_f;
    TRect Atari2;
    TRect Atari;
    TRect AtariFr;
    TRect ComCaAtari;
    TStShootAI AIDShDt;
    TStShootAI AIJShDt;
    TZahyouRec Zahyou;
    enBallMotion Motion;
    enJumpBall JumpBall;
    enMukiZType PaNMuki;
    enPassLine PaLine;
    s32 AniNo;
    s32 Atari2X;
    s32 Atari2Y;
    s32 Atari2Z;
    s32 AtariD2;
    s32 AtariD;
    s32 AtariDFr;
    s32 AtariZ2;
    s32 AtariZ;
    s32 AtariZFr;
    s32 Bound_c;//バウンド回数
    s32 BufSpin_c[BUFPOSMAX];
    s32 BufX[BUFPOSMAX];
    s32 BufY[BUFPOSMAX];
    s32 BufZ[BUFPOSMAX];//座標
    s32 CameraPosX2;
    s32 CameraPosX;
    s32 CameraPosdX;
    s32 ComCaAtariZ;
    s32 CounterPow;//カウンターパワー
    s32 CrvX;
    s32 CrvY;
    s32 CrvZ;
    s32 EnemTNo;
    s32 HitPNo;
    s32 HitTNo;
    s32 HoldPNo;
    s32 HoldTNo;
    s32 LandX;//落下予想地点
    s32 LandXLine;
    s32 LandZ;
    s32 LandZLine;
    s32 NGGetPNo;//拾えない人の番号（外野入れ替え用）
    s32 NGGetTNo;//拾えない人の番号（外野入れ替え用）
    s32 NGGet_f;//拾えない人の番号（外野入れ替え用）
    s32 NoHit_c;
    s32 PaCtPNo;
    s32 PaCtTNo;
    s32 PaETA;
    s32 PaETA_c;
    s32 PaGrv;
    s32 PaSelfHit_c;
    s32 PaTgPNo;
    s32 PaTgTNo;
    s32 PichPNo;
    s32 PichTNo;
    s32 RbwTNo;
    s32 ShTgPNo;
    s32 ShTgTNo;
    s32 TimerAutoReturn_c;
    s32 TimerCrtPos;
    s32 TimerCrtSide;
    s32 TimerSide;//これも審判クラス
    s32 Timer_c2;
    s32 Timer_c3;
    s32 Timer_c;
    s32 debugFlyRankLv;

    void Init()
    {
      Zeromem(this);
    }
  };
}
