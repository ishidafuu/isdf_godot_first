#pragma once
#include "dbStReferee.h"
#include "dbStTeam.h"
#include "dbStChar.h"
#include "dbStBall.h"
#include "dbStShot.h"
#include "dbStEtc.h"

namespace db
{
  
  struct TMgStGame
  {
    //全てのステータス
    TStReferee stRf_;//レフェリーステータス
    TStBall stBa_;//ボールステータス
    TStShot stSh_;//シュートステータス
    TStEtc stEtc_[DBETCOBJ];//小物ステータス
    TStTeam stTm_[DBSIDE];//チームステータス
    TStChar stCh_[DBSIDE][DBMEMBER_ALL];//プレーヤーステータス（ポジションと添え字一致）
    
    
    //ポジション入れ替え
    void ChangePos(s32 sideNo, s32 posA, s32 posB)
    {
      //ポジション番号入れ替え
      stTm_[sideNo].PosSt.ChangePos(posA, posB);
      
      TStChar tmpPSt = stCh_[sideNo][posA];
      
      //TStChManPad tmpPadStA = stCh_[sideNo][posA].stPad;
      //TStChManPad tmpPadStB = stCh_[sideNo][posB].stPad;
      
      stCh_[sideNo][posA] = stCh_[sideNo][posB];
      stCh_[sideNo][posB] = tmpPSt;
    }
    
    //ステータス初期化
    void Init(s64 id_kantoku0, s32 mid_team0, s64 id_team0, s64 id_kantoku1, s32 mid_team1, s64 id_team1)
    {
      stRf_.Init();
      stBa_.Init();
      stSh_.Init();
      
      stTm_[0].Init(id_kantoku0, mid_team0, id_team0);
      stTm_[1].Init(id_kantoku1, mid_team1, id_team1);

      for (s32 i = 0; i < DBSIDE; ++i)
      {
        for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
        {
          stCh_[i][i2].Init();
        }
      }
      
      for (s32 i = 0; i < DBETCOBJ; ++i)
      {
        stEtc_[i].Init();
      }
    }

    //リトライ
    void Retry(s64 id_kantoku0, s32 mid_team0, s64 id_team0)
    {
      stRf_.Init();
      stBa_.Init();
      stSh_.Init();

      stTm_[0].Init(id_kantoku0, mid_team0, id_team0);
      //stTm_[1].Init(mid_team1, id_team1);

      //for (s32 i = 0; i < DBSIDE; ++i)
      {
        for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
        {
          stCh_[SIDE0][i2].Init();
        }
      }

      for (s32 i = 0; i < DBETCOBJ; ++i)
      {
        stEtc_[i].Init();
      }
    }
  };
  
}
