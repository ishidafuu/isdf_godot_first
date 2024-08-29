#pragma once
#include "dbStGen.h"
#include "dbDtCom.h"
#include "dbDtComAct.h"
#include "dbStChar.h"

namespace db
{
  enum enInfLineType { iltInf, iltO2, iltO3, iltO4, iltOfO };
  enum enFormationType { ftDfI, ftDf2, ftDf3, ftDf4, ftOfI, ftOfO };
  enum enDodgeType { dgSw, dgJp, dgCr, dgBest };
  enum enShChangeType { scWait, scNone, scPass, scCrv };
  
 
  struct TStTmPosMove
  {
    //敵の重心
    s32 EnCX;
    s32 EnCX3;
    s32 EnCZ;
    s32 EnC_c;
    
    //内野位置取り
    enInfLineType InfLine;
    enFormationType FType;
    s32 DashmanNum;//ダッシュマン人数
    s32 DashmanNum_Auto;//オート動作ダッシュマン人数
    s32 DashmanNum_Run;//走ってるダッシュマン人数
    s32 DashmanNum_Run_NT;//ボールに触ってないダッシュマン人数
    s32 PassAbleNum;//パス可能人数＝ダッシュマン待機人数
    s32 Postman;//現在のポストマン（居ないときはNGNUM）
    s32 DashmanStock[DBMEMBER_INF];//ダッシュマン待機
    
    s32 dmng[DBMEMBER_INF];//ダッシュマンNG理由
    
    void Init()
    {
      Zeromem(this);
    }
  };
  
  struct TStTmPos
  {
    //TSL
	s64 id_kantoku_;
    s32 mid_team_;//チーム番号//デモ用で直設定アリ
    s64 id_team_;
    s32 PosofC[DBMEMBER_ALL];//キャラのポジション PosofC[CharNo] == PosNo
    s32 CharofP[DBMEMBER_ALL];//ポジションのキャラ CharofP[PosNo] == CharNo
    s32 LastPosofC[DBMEMBER_ALL];//キャラのポジション PosofC[CharNo] == PosNo
    s32 LastCharofP[DBMEMBER_ALL];//ポジションのキャラ CharofP[PosNo] == CharNo
    //s32 LastPadPos[DBSIDE];
    BOOL Last_f;
    s32 AutoPos;
    void Init(s64 id_kantoku, s32 mid_team, s64 id_team)
    {
      Zeromem(this);
	  id_kantoku_ = id_kantoku;
      mid_team_ = mid_team;
      id_team_ = id_team;

      for (s32 i = 0; i < DBMEMBER_ALL; ++i)
      {
        PosofC[i] = i;
        CharofP[i] = i;
        LastPosofC[i] = i;
        LastCharofP[i] = i;
      }
    }
    
    //ポジションリセット
    void InitPosition()
    {
      AutoPos = mid::midGetRandMn(0xFFFF);
      for (s32 i = 0; i < DBMEMBER_ALL; ++i)
      {
        PosofC[i] = NGNUM;
        CharofP[i] = NGNUM;
      }
    }
    
    void SaveLastPos()
    {
      for (s32 i = 0; i < DBMEMBER_ALL; ++i)
      {
        LastPosofC[i] = PosofC[i];
        LastCharofP[i] = CharofP[i];
      }
      Last_f = TRUE;
    }
    
    void LoadLastPos()
    {
      for (s32 i = 0; i < DBMEMBER_ALL; ++i)
      {
        PosofC[i] = LastPosofC[i];
        CharofP[i] = LastCharofP[i];
      }
    }
    
    //ポジション入れ替え
    void ChangePos(s32 posA, s32 posB)
    {
      s32 charA = CharofP[posA];
      s32 charB = CharofP[posB];
      
      //ポジション入れ替え
      PosofC[charA] = posB;
      PosofC[charB] = posA;
      CharofP[posA] = charB;
      CharofP[posB] = charA;
    }
  };
  
  
  struct TStTmCOMAct
  {
    //s32 comactNo;//現在のアクションタイプ
    s32 comactPtn[cmaEND];
    BOOL SgOdPa_f;//COMシングルオーダーパスする
    enShChangeType ShCg;//COMシュート変更タイプ
    s32 PaWait;//COMシングルパスウエイト
    s32 ShWait;//COMパワーシュートウエイト
    s32 DShStep;//COMDSh歩数
    s32 JShTime;//COMJShタイミング
    s32 JShTime2;//COMJShタイミング
    s32 DsPos;//CODs開始位置
    BOOL BallTouched_f;//ボール触った
  };
  
  struct TStTmCOMActRnd
  {
    enDodgeType DgType;//避けタイプ
    s32 DgTime;//避け開始時間
  };
  
  struct TStTmCOMRnd
  {
    BOOL DgToCa_f;//COM避けられないシュートはキャッチ
    TStTmCOMActRnd RndAct[DBMEMBER_INF];//個人
  };
  
  struct TStTmCOMCall
  {
    BOOL CallingPa_f[DBMEMBER_ALL];//パスを呼んでる
    BOOL CallingGd_f[DBMEMBER_ALL];//ガードを呼んでる
    BOOL CallingRd_f[DBMEMBER_ALL];//リーダー化
    void Init()
    {
      Zeromem(this);
    }
  };
  
  struct TStTmCOMMain
  {
    TStTmCOMAct actdt[DBMEMBER_ALL];
    s32 comPtn[comEND];
    BOOL Ressei_f;//劣勢
    BOOL Ressei_f2;//じつは劣勢
    s32 FomRev;//フォーメーション前後変動
    BOOL Quick_f;//クイック行動
    BOOL CCaSc_f;//カウンターキャッチ成功
    
    BOOL help_f;//ヘルプガード7
    
    s32 DMWait;//COMDM通常ウエイト
    s32 DMFmWait;//COMDM態勢ウエイト
    s32 DMStWait;//COMDMセッターウエイト
    s32 DMDelay;//COMDMディレイ
    s32 DMPaItv;//COMDMパスインターバル
    s32 ChMakeLoop;//COMチャンスメイクパスループ
    
    s32 BallTakePer;//COMボール拾い行動確率
    BOOL Catch_f;//COMキャッチ行動確率
    
    BOOL DsKeep_f;//３メン、２メンでパス元もダッシュを継続する
    BOOL DsOneItvWait_f;//シングルの時は１インターバル待つ
    
    //サイコロ振った時点で初期化
    s32 comatcSt_f;//ＣＯＭ動作開始（ボール持った人が一旦フリーに鳴るのを待つ）
    
    s32 dmwait_c;//ダッシュマン通常ウエイト
    s32 dmfmwait_c;//ダッシュマン態勢ウエイト
    s32 dmstwait_c;//ダッシュマンセッターウエイト
    s32 dmclwait_c;//ダッシュマンコールウエイト
    s32 dmall_c;//ダッシュマン経過カウンタ
    
    s32 dmstwaitSt_f;//ダッシュマンセッターウエイト開始
    
    BOOL dashman_f;//ダッシュマン作戦
    BOOL dashmanFmWait_f;//ダッシュマン作戦態勢ウエイト中
    BOOL dashmanShOK_f;//ダッシュマン作戦シュートＯＫでた
    //BOOL dashmanStepShOK_f;//ダッシュマン作戦シュートＯＫでた
    BOOL dashStCtrl_f;//操作キャラのダッシュマン予約
    
    s32 dmcallNum;//ダッシュマン発動人数
    s32 dmcalledNum;//ダッシュマン発動した人数
    s32 setterNo;//セッターNo
    
    BOOL dmwaitOK_f;//ダッシュマンウエイトＯＫ
    BOOL callOK_f;//コールＯＫ
    BOOL callNG_f;//コールＮＧ
    BOOL setterOK_f;//セッターに渡った
    BOOL setterBMOKWait_f;//セッターのボールマンダッシュOKでウエイト
    BOOL setterBMRsv_f;//セッターのボールマンダッシュ
    BOOL setterBMRsvGo_f;//セッターのボールマンダッシュＯＫ
    
    s32 dmstop_f;//ダッシュマン作戦停止のタイミング
    s32 tmwait_c;//チーム行動ウエイトカウンタ
    BOOL sgodpass_f;//シングルパスおよびオーダーパス
    s32 shcgpass_c;//パスカウンタ
    s32 chpass_c;//チャンスメイクパスカウンタ
    s32 comReset_f;//COM作戦リセットタイミング
    s32 shAfter_c;//シュートしてからカウンタ(守備用)
    
    BOOL Chance_f;//チャンス行動フラグ
    BOOL vnChance_f;//至近距離チャンス
    BOOL ngChance_f;//ノーガードチャンス
    //BOOL ncChance_f;//ナイスキャッチチャンス
    s32 vnTagNo;//至近距離タゲ番号
    s32 ngTagNo;//ノーガードタゲ番号
    
    void Init()
    {
      Zeromem(this);
    }
    BOOL Isdm()//ダッシュマン作戦中
    {
      return (dashman_f || dashmanFmWait_f);
    }
    
  };
  struct TStTmCOMAttack
  {
    //キャラの手に渡った時点で初期化する系
    s32 dmpawait_c;//ダッシュマンパスインターバル用
    BOOL quick_f;//クイックシュート行動
    BOOL vnnojp_f;//至近距離ジャンプ無しシュート行動
    BOOL sh_f;//シュート行動
    BOOL pa_f;//パス行動
    BOOL jp_f;//ジャンプ行動
    BOOL ds_f;//ダッシュ行動
    BOOL onestep_f;//１歩ダッシュ
    BOOL toss_f;//トスパス
    BOOL dspos_f;//ダッシュ開始位置
    s32 waitmove_c;//ウエイト動作
    BOOL dmpass_f;//ダッシュマンパス
    s32 getstep;//ボール取得時の歩数
    s32 dsTagZ;//ダッシュ目標
    s32 dsTagZ_f;//ダッシュ目標常にサーチ
    
    s32 paTag;//シングルパスタゲ
    
    void Init()
    {
      Zeromem(this);
    }
  };
  
  struct TStTmMaruhi
  {
    s32 CallNum;//呼ぶ人数
    s32 CallDelay;//ディレイ
    s32 DMType;//ダッシュマンタイプ
    s32 Call_c;//呼んだ人数
    s32 Delay_c;//ディレイ用カウンタ
    void Init()
    {
      Zeromem(this);
    }
    
  };
  
  struct TStTeam
  {
    enResultType RType;//試合結果
    TStTmPos PosSt;//試合前決定されたチーム番号やらポジションやら★決定したら上書きする
    TStTmPosMove PosMove;//敵内野重心
    BOOL COMOverTime_f;//ＣＯＭ思考強制シュート
    BOOL RsvResetCOM_f;//ＣＯＭ思考リセット★
    BOOL CallingDMReset_f;//コールによるＣＯＭ思考リセット★
    BOOL CallingRdCg_f;//コールによるリーダチェンジ＆ＣＯＭ思考リセット★
    s32 CtrlNo;//操作中のキャラポジション
    s32 DMCallmanNo;//ダッシュマンを呼んだ人（センターダッシュ用）
    s32 CtrlRsvNo;//操作権予約キャラ
    s32 CvrNo;//パス先に操作権が移ったときカバーにはいるキャラ
    s32 Cvr2No;//カバーマンのカバーマン
    s32 GetNo;//ボール拾いマン
    s32 NoInput_c;//操作権自動切り替え用
    BOOL InputCancel_f;//操作権変わったときなど同フレームの入力を反映しない
    
    s32 DrwHSNo;//表示必殺シュート番号
    s32 DrwHS_c;//表示必殺シュートカウンタ　これらはチームで管理した方がよさそう
    s32 DrwPosX;
    
    s32 BallCrtSideL;
    s32 BallCrtPosL;
    s32 BallCrtSideZ;
    s32 BallCrtPosZ;
    
    s32 DashmanCallNo;//★ダッシュマン予約
    BOOL DashmanAll_f;//全員出発
    s32 DashmanNoBM;//ボールマンからのダッシュマン予約
    enDashmanType DashmanType;//★ダッシュマン予約
    s32 DashmanCall_c;//呼んだ人数
    
    s32 IOChange_c;//内外野交代カウンタ
    s32 UseMotoGaiya_c;//元外野
    s32 Dead_c;//倒された数
    
    s32 TmUeda_c;//チーム上田カウンタ
    
    s32 DbgSerifuPNo;
    s32 DbgSerifuTypeNo;
    
    BOOL ofside_f;//オフェンスサイド
    BOOL noside_f;//サイドナシ
    BOOL randreset_f;//行動確率リセット
    BOOL getJpball_f;//ジャンプボール
    
    TStTmCOMAttack stCOM;//COM思考用★
    TStTmCOMMain  COMDt;//COM思考用★
    TStTmCOMRnd   COMRnd;//COM思考用★
    TStTmCOMCall COMCall;//COM思考用★
    
    BOOL SideMaruhi_f;//丸秘作戦開始
    BOOL MaruhiCall_f;
    
    TStChRecord Rec;
    
    BOOL MANSide_f;//サイド操作
    BOOL MANSidePadNo;//サイド操作
    BOOL OmksSide_f;//おまかせ
    s32 Omakase_c;//チーム用おまかせ連打防止カウンタ
    //s32 Block_c;//ブロックカウンタ
    s32 Encr_c;//エンカレカウンタ
    s32 TutoWait_c;//エンカレカウンタ
    
    BOOL CallingDashman_f;//Calling開始
    BOOL CallingDashman_f2;//Switchでオン
    BOOL CallingDashman_f3;//発動
    s32 CallingNo;
    s32 RdNo;//チームリーダー番号（posNoではなく、charNo）
    s32 comNo_;//リーダーに応じたＣＯＭ番号
    s32 fomNo_;
    s32 LastBallmanNo;
    BOOL NeedTimeRedraw_f;

    //セミオート用追加変数
    BOOL semi_f_;//セミオート
    BOOL semi_order_f_;//セミオートシュートオーダー
    s32 semi_Catch_c_;//キャッチアシストカウンター
    s32 semi_Catching_c_;//キャッチアシストカウンター
    BOOL semi_Shot_f_;//シュート命令
    s32 semi_Shot_c_;//キャッチアシストカウンター
    s32 semi_AI_;
    s32 semi_CaPos_;
    s32 semi_NCaLen_;

    //スマート操作
    BOOL smart_f_;//スマート

    s32 dam_c_;//ＨＰ表示用
    s32 dampos_;//ダメージキャラ

    void Init(s64 id_kantoku, s32 mid_team, s64 id_team)
    {
      Zeromem(this);
      PosSt.Init(id_kantoku, mid_team, id_team);
      PosMove.Init();
    }
    
    void SetPosSt(TStTmPos stpos)
    {
      PosSt = stpos;
    }
    
    s32 GetRdPos()
    {
      return PosSt.PosofC[RdNo];
    }
    
    s32 IsDashmanCall(s32 posNo)
    {
      if (DashmanAll_f)
      {
        for (s32 i = 0; i < DBMEMBER_INF; ++i)
        {
          if (posNo == PosMove.DashmanStock[i])
          {
            return TRUE;
          }
        }
        return FALSE;
      }
      else
      {
        return (posNo == DashmanCallNo);
      }
    }
    
    BOOL IsBMDashmanCall(s32 posNo)
    {
      return (posNo == DashmanNoBM);
    }
    
    BOOL IsDashmanStock(s32 posNo)
    {
      BOOL res = FALSE;
      
      s32 callnum = (COMDt.dmcallNum - COMDt.dmcalledNum);//呼ぶ予定の人数
      
      for (s32 i = 0; i < callnum; ++i)
      {
        if (posNo == PosMove.DashmanStock[i])
        {
          res = TRUE;
        }
      }
      return res;
    }
    
    //ポジションリセット
    void InitPosition()
    {
      PosSt.InitPosition();
    }
  };
  
}

