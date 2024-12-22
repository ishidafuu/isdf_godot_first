#include "dbChAction.h"
#include "dbGame.h"

namespace db
{

  //コンストラクタ
  TChAction::TChAction(TGame* pGame, TChCommon* pCommon, s32 sideNo, s32 charNo)
    : TChHaveCommon(pGame, pCommon, sideNo, charNo)
  {
  }

  TChAction::~TChAction()
  {
  }

  //シュート発動
  void TChAction::COMShootAct()
  {
    //ノーガードは狙わない場合
    BOOL paok_f = (st_.pmgMyTm_->st_.pstMyTm_->COMOverTime_f == FALSE);

    BOOL ngshng_f = FALSE;

    BOOL shtgnone_f = (pmgSG_->stBa_.ShTgPNo == NGNUM);

    if (shtgnone_f == FALSE)
    {
      ngshng_f = ((st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaChanceSh] == 3)
        && (st_.pmgEnTm_->st_.pmgMyCh_[pmgSG_->stBa_.ShTgPNo]->IsNoGuard(TRUE)));
    }

    //セミオートでオーダーでシュート命令が出てないとき
    if (st_.pstMyTm_->semi_f_
      && (st_.pstMyTm_->semi_order_f_)
      && (st_.pstMyTm_->semi_Shot_f_ == FALSE))
    {
      st_.pmgMyTm_->st_.pstMyTm_->stCOM.paTag = (IsInfield())
        ? pCommon_->GetNaiyaPassTag()
        : pCommon_->GetGaiyaPassTag();

      COMPass(FALSE);//シュート切り替えパス
    }
    else if (paok_f && (ngshng_f || shtgnone_f))
    {
      st_.pmgMyTm_->st_.pstMyTm_->stCOM.paTag = (IsInfield())
        ? pCommon_->GetNaiyaPassTag()
        : pCommon_->GetGaiyaPassTag();

      COMPass(FALSE);//シュート切り替えパス
    }
    else
    {
      //タゲ方向向く
      pCommon_->LookTg(pmgSG_->stBa_.ShTgPNo, FALSE, TRUE);//居ないときはオートで探す
      switch (st_.pstMyCh_->Motion.Mtype)
      {
      case dbmtSt:
      case dbmtWk:
      case dbmtDs:
        pCommon_->SetMtype(dbmtSh);
        break;
      case dbmtJUp:
      case dbmtJDn:
        pCommon_->SetMtype(dbmtJSh);
        break;
      default:
        break;
      }
    }
  }

  //ＣＯＭパス
  void TChAction::COMPass(BOOL dmpass_f)
  {
    //パスタゲセット
    pmgSG_->stBa_.PaTgTNo = st_.mysideNo_;

    if (dmpass_f == FALSE)
    {
      //ダッシュマンが居る場合はタゲを急遽取り直す
      if (st_.pmgMyTm_->st_.pstMyTm_->PosMove.DashmanNum > 0)
      {
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.paTag = (IsInfield())
          ? pCommon_->GetNaiyaPassTag()
          : pCommon_->GetGaiyaPassTag();
      }
    }

    pmgSG_->stBa_.PaTgPNo = st_.pmgMyTm_->st_.pstMyTm_->stCOM.paTag;

    //パスタゲがパスを出せない状態とき
    pCommon_->NGPaTagShift();


    //パスカットキャラセット
    pCommon_->PaCtTagSet();
    //タゲの方を向く
    pCommon_->LookTg(pmgSG_->stBa_.PaTgPNo, TRUE, FALSE);

    switch (st_.pstMyCh_->Motion.Mtype)
    {
    case dbmtSt:
    case dbmtWk:
    case dbmtDs:
      pCommon_->SetMtype(dbmtPa);
      break;
    case dbmtJUp:
    case dbmtJDn:
      pCommon_->SetMtype(dbmtJPa);
      break;
    }
  }

  //ＣＯＭのシュートやパス
  void TChAction::COMShoot()
  {
    if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.pa_f)
    {
      COMPass(FALSE);//シュート切り替えパスのみここにくる
    }
    else
    {
      COMShootAct();//シュート発動
    }
  }

  //COMダッシュマンパスタゲセット★★
  s32 TChAction::GetCOMDMPassTag(BOOL nowOKonly_f)
  {

    s32 sltgX[DBMEMBER_INF];
    s32 sltgZ[DBMEMBER_INF];
    s32 sltgXZ[DBMEMBER_INF];
    enDMTag sltg_f[DBMEMBER_INF];
    s32 tgOrd[DBMEMBER_INF];

    BOOL topord_f = TRUE;//最上位オーダーフラグ
    BOOL bottom_f = TRUE;//最後尾フラグ

    s32 sortDt[DBMEMBER_INF];

    BOOL Notag_f = TRUE;//完全にパスタゲが居ない
    BOOL NoOKtag_f = TRUE;//今パスを出せるタゲが居ない

    //パスのタイプ
    enCOMDMPassType patype = (enCOMDMPassType)st_.pmgMyTm_->st_.pstMyTm_->COMDt.comPtn[comDMPaTag];
    //触ってない人だけ
    BOOL NTOnly_f = (st_.pmgMyTm_->st_.pstMyTm_->COMDt.comPtn[comDMPaNum] == 0);

    //優先順位初期化
    for (s32 i = 0; i < DBMEMBER_INF; ++i)
    {
      tgOrd[i] = NGNUM;
    }

    //内野全員との距離を取る
    for (s32 i = 0; i < DBMEMBER_INF; ++i)
    {
      if (i != st_.posNo_)
      {
        //X距離
        sltgX[i] = (st_.pmgMyTm_->st_.pmgMyCh_[i]->GetLeftCrtX() - GetLeftCrtX());//自分より右に居れば＋
        //Z距離
        sltgZ[i] = (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z - st_.pstMyCh_->Zahyou.Z);//自分より上にいれば＋
        //距離
        sltgXZ[i] = (s32)lib_num::Hypot(sltgX[i], sltgZ[i]);
      }
    }

    //ダッシュマンパススピード
    s32 paspd = pmgEO_->mgDt_.dtSet_.GetDtPass(setDMPaSpd);


    //内野全員との角度を取る
    for (s32 i = 0; i < DBMEMBER_INF; ++i)
    {
      if (i == st_.posNo_)//自分
      {
        sltg_f[i] = DMTG_NG;
      }
      else if (st_.pmgMyTm_->st_.pmgMyCh_[i]->IsDashman())//現在ダッシュマン
      {
        const s32 YOYUU = 10;//投げモーションと相手が離れていく分
        s32 reachtime = (sltgXZ[i] / paspd) + YOYUU;


        if ((st_.pmgMyTm_->st_.pmgMyCh_[i]->IsMAN() == FALSE)//マニュアル以外
          && (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->DashmanPaOK_c < reachtime))//おそらくとどかない
        {
          sltg_f[i] = DMTG_NG;
        }
        else
          if (st_.pmgMyTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.dY < 0)//下降に入ってる
          {
            sltg_f[i] = DMTG_NG;
          }
          else if ((NTOnly_f == FALSE)//一人一回ではない
            || (st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[i].BallTouched_f == FALSE))//まだ触ってない
          {

            sltg_f[i] = DMTG_OK;
            NoOKtag_f = FALSE;
          }
          else
          {
            sltg_f[i] = DMTG_NG;
          }
      }
      else if (st_.pmgMyTm_->st_.pstMyTm_->IsDashmanStock(i))//待ちには入っている
      {
        sltg_f[i] = DMTG_WAIT;//ノーアングル扱い
      }
      else
      {
        sltg_f[i] = DMTG_NG;
      }


      if (sltg_f[i] != DMTG_NG)
      {
        if (st_.pmgMyTm_->st_.pmgMyCh_[i]->GetLeftCrtX() < GetLeftCrtX())//後ろに人がいる
        {
          bottom_f = FALSE;//最後尾ではない
        }

        if (st_.posNo_ > i)//上位に人がいる
        {
          topord_f = FALSE;//先頭ではない
        }

        Notag_f = FALSE;
      }


    }

    if (Notag_f)//タゲが居ない
    {
      return NGNUM;
    }
    else
    {

      s32 f = 0;
      for (s32 i = 0; i < DBMEMBER_INF; ++i)
      {
        //向き方向に人なしのとき

        sortDt[i] = 0;//初期化

        if (sltg_f[i] != DMTG_NG)
        {
          //値が少ないほど優先
          switch (patype)
          {
          case cpmUpOrder://一つオーダー上位（上位なら最下位）
            if (topord_f)//上位に人がいない
            {
              sortDt[i] = (-i);//最下位オーダー
            }
            else
            {
              sortDt[i] = (i > st_.posNo_)//タゲの方が前にいる
                ? DBMEMBER_INF
                : (st_.posNo_ - i);//自分より上位で一番近い人
            }
            break;
          case cpmNear://一番近い人
            sortDt[i] = sltgXZ[i];
            break;
          case cpmFar://一番Ｚ軸が遠い人
            sortDt[i] = -(sltgZ[i]);
            break;
          case cpmRear://一つ後ろの人(居なければ一番前の人)
            if (bottom_f)//一番後ろ
            {
              sortDt[i] = -(st_.pmgMyTm_->st_.pmgMyCh_[i]->GetLeftCrtX());//一番前にいる人
            }
            else
            {
              sortDt[i] = (st_.pmgMyTm_->st_.pmgMyCh_[i]->GetLeftCrtX() > GetLeftCrtX())//タゲの方が前にいる
                ? DBCRT_W
                : (GetLeftCrtX() - st_.pmgMyTm_->st_.pmgMyCh_[i]->GetLeftCrtX());//自分より後ろで一番近い人
            }
            break;
          }

          tgOrd[f++] = i;
        }
      }

      //ソート
      for (s32 i = 0; i < (DBMEMBER_INF - 1); ++i)
      {
        for (s32 i2 = 0; i2 < (DBMEMBER_INF - 1); i2++)
        {
          if (i == i2) continue;//同じ

          if ((tgOrd[i] != NGNUM) && (tgOrd[i2] != NGNUM))
          {
            if (sortDt[tgOrd[i]] < sortDt[tgOrd[i2]])//小さい方優先
            {
              s32 tmp;
              tmp = tgOrd[i2];
              tgOrd[i2] = tgOrd[i];
              tgOrd[i] = tmp;
            }
          }
        }
      }

      //ソート１位
      s32 res = tgOrd[0];

      //現状ＯＫなやつしか駄目な場合
      if (nowOKonly_f)
      {
        if (sltg_f[res] != DMTG_OK)
        {
          res = NGNUM;
        }
      }
      return res;
    }
  }

  //ダッシュマンパス
  void TChAction::COMDMPassAct()
  {

    //ジャンプしないダッシュマン
    if (st_.pmgMyTm_->IsNoJpDashman())
    {

      switch (st_.pmgMyTm_->st_.pstMyTm_->COMDt.comPtn[comDMPaNum])
      {
      case 0://一人一回パスの場合
        //全員呼んだ＆触ってない人がいない
        if ((st_.pmgMyTm_->st_.pstMyTm_->COMDt.dmcalledNum == st_.pmgMyTm_->st_.pstMyTm_->COMDt.dmcallNum)
          && (st_.pmgMyTm_->st_.pstMyTm_->PosMove.DashmanNum_Run_NT == 0))
        {
          //歩数きたら投げる
          st_.pmgMyTm_->st_.pstMyTm_->COMDt.dashmanShOK_f = TRUE;
        }
        break;
        //case 1://無限パス
        //  break;
      case 2://パスしない場合
        //歩数きたら投げる
        st_.pmgMyTm_->st_.pstMyTm_->COMDt.dashmanShOK_f = TRUE;
        break;
      }
    }

    //パスしない場合
    if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.comPtn[comDMPaNum] == 2)
    {
      return;
    }


    //シュートタイミング
    BOOL shtiming_f = FALSE;

    //セッター以外の内野ダッシュマンもしくはボールマンダッシュセッター
    BOOL setter_f = (st_.pmgMyTm_->st_.pstMyTm_->COMDt.setterNo == st_.posNo_);

    if (IsInfield()
      && (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))//ダッシュしてる
      && ((setter_f == FALSE) || st_.pmgMyTm_->st_.pstMyTm_->COMDt.setterBMRsvGo_f))
    {
      //現状パスのjpが入っているので
      s32 pwsh = st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaPwShType];

      switch (pwsh)
      {
      case 0://ＤＪＳ
        if ((GetLeftCrtX() > st_.pmgMyTm_->GetAtcLineX(TRUE, TRUE))//攻撃ライン超えた
          && st_.pstMyCh_->Motion.IsMFlags(dbmfAr)//ジャンプ状態
          && (st_.pstMyCh_->Air_c >= st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].JShTime))//予定時間こえてる
        {
          shtiming_f = TRUE;
        }
        break;
      case 1://ＤＳ
        if ((st_.pstMyCh_->Motion.IsMFlags(dbmfAr) == FALSE)
          && ((st_.pstMyCh_->Step_c >= st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].DShStep)//予定歩数超えた
          || (GetLeftCrtX() > st_.pmgMyTm_->GetAtcLineX(FALSE, TRUE))))//攻撃ライン超えた
        {
          shtiming_f = TRUE;
        }
        break;
      case 3://ＪＳ
        if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr)//ジャンプ状態
          && (st_.pstMyCh_->Air_c >= st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].JShTime))//予定時間こえてる
        {
          shtiming_f = TRUE;
        }
        break;
      default://一歩ダッシュＳ
        if ((st_.pstMyCh_->Motion.IsMFlags(dbmfAr) == FALSE)
          && ((st_.pstMyCh_->Step_c >= 1)//予定歩数超えた
          || (GetLeftCrtX() > st_.pmgMyTm_->GetAtcLineX(FALSE, TRUE))))//攻撃ライン超えた
        {
          shtiming_f = TRUE;
        }
        break;
      }
    }
    else
    {
      shtiming_f = TRUE;//外野およびセッターなら無条件でシュートタイミングでいいか
    }


    //パスタゲ(完全ＯＫのみ)
    s32 oktag = GetCOMDMPassTag(TRUE);

    BOOL act_f = FALSE;


    //行動するかどうか//最低数フレ待つ（全員が走り出す前に投げてしまう）
    if ((oktag != NGNUM) && (st_.pmgMyTm_->st_.pstMyTm_->stCOM.dmpawait_c > 1))
    {
      const int WAITSTEP = 2;

      BOOL setterBMPaOK_f = (st_.pmgMyTm_->st_.pstMyTm_->COMDt.setterBMRsvGo_f
        && st_.pstMyCh_->Motion.IsMFlags(dbmfDs)
        && (st_.pstMyCh_->Step_c > WAITSTEP));

      //セッターでなくて、ダッシュしていないキャラ
      BOOL scndSetter_f = (st_.pmgMyTm_->st_.pstMyTm_->COMDt.setterNo != st_.posNo_)
        && (st_.pstMyCh_->Motion.IsMFlags(dbmfDs) == FALSE);

      //セッターダッシュしない、もしくは、セッターダッシュ開始後
      BOOL setterOK_f = ((IsInfield() == FALSE)
        || ((st_.pmgMyTm_->st_.pstMyTm_->COMDt.setterBMRsv_f == FALSE) || setterBMPaOK_f));


      //セカンドセッター
      if (scndSetter_f)
      {
        act_f = TRUE;//無条件ＯＫ
      }
      else if ((setter_f == FALSE) || setterOK_f)
      {
        //外野がパス出せずにジャンプしてしまうのをこれで避けられるか？
        if (IsInfield())
        {
          //内野セッター（もしくはセッターでないのにボールを渡された人）
          BOOL infsetter_f = (st_.pmgMyTm_->st_.pstMyTm_->COMDt.setterNo == st_.posNo_)
            || (st_.pstMyCh_->Motion.IsMFlags(dbmfDs) == FALSE);

          switch (st_.pmgMyTm_->st_.pstMyTm_->COMDt.comPtn[comDMPaTime])
          {
          case 0://パス先が前衛ライン超えたら
            if (st_.pmgMyTm_->st_.pmgMyCh_[oktag]->GetLeftCrtX() > DBCRT_CLXL)
            {
              act_f = TRUE;
            }
            break;
          case 1://パスインターバル後
            if (st_.pmgMyTm_->IsDMPaItvTime(infsetter_f))
            {
              act_f = TRUE;
            }
            break;
          case 2://自分のシュートタイミング★
            act_f = shtiming_f;
            break;
          default://パス先がダッシュ開始したら
            act_f = TRUE;//タゲOKになってる時点で走ってるはず
            break;
          }
        }
        else
        {
          act_f = TRUE;//外野は無条件ＯＫ
        }

      }

      //行動する
      if (act_f)
      {
        //タゲセット
        st_.pmgMyTm_->st_.pstMyTm_->stCOM.paTag = oktag;
        //パスタイプ

        s32 patype = (IsInfield())
          ? st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaPaTypeInf]
          : st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].comactPtn[cmaPaType];

        st_.pmgMyTm_->SetPassType(patype);

        COMPass(TRUE);//パス

        //１回だけパスの場合
        if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.comPtn[comDMPaNum] == 3)
        {
          //シュートOK
          st_.pmgMyTm_->st_.pstMyTm_->COMDt.dashmanShOK_f = TRUE;
        }

      }
    }


    //パスを出さなかった
    if (act_f == FALSE)
    {
      //これ以上は待てないラインを超えた
      if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr))//ジャンプ状態
      {
        //const int LIMTIME = pmgEO_->mgDt_.dtFomation_.GetComDt(cmvJShTime, 0);
        //位置も最低限指定しないとポストマンジャンプの次点で判定されてしまう//パスカットもあるのでダッシュフラグに
        //if ((st_.pstMyCh_->Air_c >= LIMTIME)
        //  && GetLeftCrtX() > st_.pmgMyTm_->GetAtcLineX(TRUE, TRUE)
        //  && st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
        if ((GetLeftCrtX() > st_.pmgMyTm_->GetAtcLineX(TRUE, TRUE))//攻撃ライン超えた
          && (st_.pstMyCh_->Air_c >= st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].JShTime))//予定時間こえてる
        {
          //シュートOK
          st_.pmgMyTm_->st_.pstMyTm_->COMDt.dashmanShOK_f = TRUE;
        }
      }
      else
      {
        //ジャンプするダッシュマン用
        const s32 LIMLINE = st_.pmgMyTm_->GetAtcLineX(FALSE, TRUE);
        if (GetLeftCrtX() > LIMLINE)
        {
          //シュートOK
          st_.pmgMyTm_->st_.pstMyTm_->COMDt.dashmanShOK_f = TRUE;
        }
      }
    }

  }

  void TChAction::COMAction_DM_Sh()//シュート指示が出てる
  {
    //空中攻撃
    if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.jp_f
      || st_.pstMyCh_->Motion.IsMFlags(dbmfAr))
    {
      if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr)//ジャンプ状態
        && (st_.pstMyCh_->Air_c >= st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].JShTime))//予定時間こえてる
      {
        //シュート
        COMShootAct();
      }
    }
    else//地上攻撃
    {
      if ((IsInfield() == FALSE)//外野
        || (st_.pstMyCh_->Step_c >= st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].DShStep)//予定歩数超えた
        || (GetLeftCrtX() > st_.pmgMyTm_->GetAtcLineX(FALSE, TRUE)))//センターライン超えそう
      {
        //シュート
        COMShootAct();
      }
    }
  }
  void TChAction::COMAction_DM_Pa()//パス回し
  {
    //既にメンバー走り出しに入ってる
    if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.callOK_f)
    {
      //ダッシュマンパス（タイミングなどは関数内）
      COMDMPassAct();
    }
    else if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.setterOK_f == FALSE)//セッターにボールが渡ってない
    {
      //空中攻撃
      if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.jp_f
        || st_.pstMyCh_->Motion.IsMFlags(dbmfAr))
      {
        if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr)//ジャンプ状態
          && (st_.pstMyCh_->Air_c >= st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].JShTime))//予定時間こえてる
        {
          COMPass(FALSE);//セッターパス
        }
      }
      else //地上攻撃
      {
        COMPass(FALSE);//セッターパス
      }
    }
  }

  //ＣＯＭ思考行動ダッシュマン★★
  void TChAction::COMAction_DM()
  {
    //ダッシュマン作戦に完全に入ってる必要がある（態勢ウエイトはなにもしない）
    if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.dashman_f == FALSE) return;

    //シュート指示が出てる
    if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.dashmanShOK_f)
    {
      //飛ばないダッシュマン
      if (st_.pmgMyTm_->IsNoJpDashman())
      {
        //歩数過ぎたら投げてイイ
        if ((st_.pstMyCh_->Step_c >= st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].DShStep)
          || (GetLeftCrtX() > st_.pmgMyTm_->GetAtcLineX(FALSE, TRUE)))//ジャンプしないダッシュマンはこのラインが最大限界
        {
          COMAction_DM_Sh();
        }
      }
      else
      {
        COMAction_DM_Sh();
      }
    }
    else//パス回し
    {
      COMAction_DM_Pa();//パス回し
    }

  }

  //ＣＯＭ思考行動通常★★
  void TChAction::COMAction_Std()
  {
    //シュート
    if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.sh_f)
    {
      //空中攻撃
      if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.jp_f
        || st_.pstMyCh_->Motion.IsMFlags(dbmfAr))
      {
        s32 shtiming = (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
          ? st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].JShTime
          : st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].JShTime2;

        if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr)//ジャンプ状態
          && (st_.pstMyCh_->Air_c >= shtiming))//予定時間こえてる
        {
          //内野ダッシュジャンプ攻撃のときは
          if (IsInfield() && st_.pmgMyTm_->st_.pstMyTm_->stCOM.ds_f)
          {
            if (st_.pstMyCh_->LandEnCourt_f)//相手コートに落ちてしまう場合はやむを得ずシュート
            {
              COMShoot();
            }
            else if (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))//ダッシュフラグがたっていたら
            {
              COMShoot();
            }
          }
          else
          {
            COMShoot();//それ以外はシュート
          }
        }
      }
      else if (st_.pmgMyTm_->IsWaitOver())//地上攻撃
      {
        //内野ダッシュ攻撃
        if (IsInfield() && st_.pmgMyTm_->st_.pstMyTm_->stCOM.ds_f)
        {
          if (st_.pstMyCh_->LandEnCourt_f)//相手コートに落ちてしまう場合はやむを得ずシュート
          {
            COMShoot();
          }
          else if //((st_.pmgMyTm_->st_.pstMyTm_->stCOM.quick_f == FALSE)//クイック一時停止
            (st_.pmgMyTm_->st_.pstMyTm_->stCOM.waitmove_c == 1)
          {
            COMShoot();
          }
          //ダッシュ開始位置まで行ったフラグ
          else if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.dspos_f)
          {
            s32 needstep = (st_.pmgMyTm_->st_.pstMyTm_->stCOM.onestep_f)//一歩ＤＳ
              ? 1
              : st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].DShStep;


            if ((st_.pstMyCh_->Step_c >= needstep)//予定歩数超えた
              || (GetLeftCrtX() > st_.pmgMyTm_->GetAtcLineX(FALSE, FALSE)))//攻撃ライン超えた
            {

              if ((st_.pstMyCh_->COMCounter_f == FALSE)
                || (st_.pmgMyTm_->st_.pstMyTm_->stCOM.getstep < st_.pstMyCh_->Step_c))
              {
                COMShoot();
              }
            }
          }
        }
        else
        {
          COMShoot();//それ以外はシュート
        }
      }
    }
    else if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.pa_f)//パス
    {
      //空中攻撃
      if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.jp_f
        || st_.pstMyCh_->Motion.IsMFlags(dbmfAr))
      {
        if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr)//ジャンプ状態
          && (st_.pstMyCh_->Air_c >= st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].JShTime))//予定時間こえてる
        {
          COMPass(FALSE);//パス
        }
      }
      else if (st_.pmgMyTm_->IsWaitOver())//地上攻撃
      {
        COMPass(FALSE);//パス
      }
    }

  }

  //ＣＯＭ思考行動（パス、シュート）★★
  void TChAction::COMAction()
  {
    //アクション中は来ない
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfAct)) return;

    //ボールマンだけ
    if (IsBall() == FALSE) return;

    //セミオート命令シュート
    if (st_.pmgMyTm_->st_.pstMyTm_->semi_Shot_f_)
    {
      COMShootAct();
      st_.pmgMyTm_->st_.pstMyTm_->semi_Shot_f_ = FALSE;
    }
    else if (st_.pmgMyTm_->st_.pstMyTm_->COMOverTime_f)
    {
      //強制シュート
      COMShootAct();
    }
    else
    {
      //ダッシュマン
      if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.Isdm())
      {
        COMAction_DM();
      }
      else//通常行動
      {
        COMAction_Std();
      }
    }
  }

  //キー入力（アクション）
  void TChAction::MCKeyInputAT()
  {
    if (IsShiai() == FALSE) return;

    if (GameEndWait_f()) return;

    if (st_.pstMyCh_->Motion.IsMFlags(dbmfDam)) return;
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfANG)) return;
    if (st_.pstMyCh_->Dead_f) return;

    if ((Kagami_f()) && (st_.pstMyCh_->Auto.AutoType == dbatFree))
    {
      if ((st_.pstMyCh_->Motion.Mtype == dbmtSt)
        || (st_.pstMyCh_->Motion.Mtype == dbmtWk)
        || (st_.pstMyCh_->Motion.Mtype == dbmtDs))
      {
        pCommon_->SetMtype(dbmtKG);//かがみ状態
      }
      return;
    }

    if (st_.pmgMyTm_->st_.pstMyTm_->InputCancel_f)
    {
      st_.pmgMyTm_->st_.pstMyTm_->InputCancel_f = FALSE;
      return;
    }


    enATType ATType;

    if (IsBall())
    {
      ATType = ATA;//攻撃
    }
    else if ((pmgSG_->stBa_.Motion == bmFree)
      || ((pmgSG_->stBa_.Motion == bmHold) && (pmgSG_->stBa_.HoldTNo == st_.mysideNo_))
      || (pmgSG_->stBa_.Motion == bmBound)
      || (pmgSG_->stBa_.Motion == bmReferee)
      || ((pmgSG_->stBa_.Motion == bmPass) && (pmgSG_->stBa_.PichTNo == st_.mysideNo_))
      )
    {
      ATType = ATF;//拾い
    }
    else
    {
      ATType = ATD;//守備
    }

    //敵コート避け
    BOOL ecdgOK_f = FALSE;
    if ((IsInfield() == FALSE) || IsBall())//ボール持ってたらダメかと
    {
      ecdgOK_f = FALSE;
    }
    else if (IsMAN() || st_.pstMyCh_->FreeAct_f)//外野のセンターライン方向押しジャンプが逆飛びになる
    {
      ecdgOK_f = TRUE;
    }
    else if (pmgSG_->stBa_.Motion == bmHold)
    {
      ecdgOK_f = (pmgSG_->stBa_.HoldTNo == st_.ensideNo_);//敵持ち
    }
    else if (pmgSG_->stBa_.Motion == bmShoot)
    {
      ecdgOK_f = (pmgSG_->stBa_.ShTgTNo == st_.mysideNo_);//味方がタゲのシュート
    }
    else
    {
      ecdgOK_f = (pmgSG_->stBa_.TimerCrtSide != st_.mysideNo_);//敵コートに落ちてる
    }


    switch (st_.pstMyCh_->Auto.AutoType)
    {
      //自由
    case dbatFree:
      if (IsSelfCtrl())
      {
        //自分で操作**********
        pCommon_->SelfCtrl(ATType);
      }
      else
      {
        if ((IsBall() == FALSE)
          && (st_.pstMyCh_->Motion.Mtype == dbmtJDg))
        {
          st_.pstMyCh_->Anime.Ani_c = 0;//こうやる必要ないか？
        }

        pCommon_->AutoPickUp();
        //CheckFreeBallPickUp();
      }
      break;

      //ポジショニング
    case dbatDfPos:
      pCommon_->AutoPickUp();
      //CheckFreeBallPickUp();
      break;

      //戻り
    case dbatReturn:
      if (IsSelfCtrl()//←いらないかも
        || (IsCOM() == FALSE))
      {

        BOOL muki_f = ((st_.mysideNo_ == 0) && (st_.pstMyCh_->Zahyou.Muki == mL))
          || ((st_.mysideNo_ == 1) && (st_.pstMyCh_->Zahyou.Muki == mR));

        switch (st_.pstMyCh_->Motion.Mtype)
        {
        case dbmtCr:
          break;
        case dbmtSt:
        case dbmtWk:
        case dbmtDs:
        case dbmtSl://スリップも追加オーバーラインの瞬間なぞのキャッチできてしまう
          if ((st_.pstMyCh_->ECDdg_f == FALSE) && (ecdgOK_f))
          {
            if (MyPad()->IsJump()//ジャンプ入力
              && muki_f
              && (st_.pstMyCh_->ECDjp_f == FALSE))
            {
              pCommon_->JumpSet(FALSE, FALSE, FALSE);//ジャンプ
              //mid::midLog("J0\n");
              st_.pstMyCh_->ECDdg_f = TRUE;//１回だけ
              st_.pstMyCh_->ECDjp_f = TRUE;//１回だけ
            }
            else if (MyPad()->IsDodge())//よけ
            {
              pCommon_->SetMtype(dbmtDg);
              st_.pstMyCh_->ECDdg_f = TRUE;//１回だけ
            }
            //else if (MyPad()->IsCatch())//キャッチ入力
            //{
            //  pCommon_->SetCatchMuki();
            //  pCommon_->SetMtype(dbmtCa);
            //  st_.pstMyCh_->ECDdg_f = TRUE;//１回だけ
            //}

            if (st_.pstMyCh_->ECDdg_f)
            {
              pCommon_->CatchSE();
              st_.pstMyCh_->LastMuki = st_.pstMyCh_->Zahyou.Muki;
              st_.pstMyCh_->LastMukiZ = st_.pstMyCh_->Zahyou.MukiZ;
            }
          }
          break;
        case dbmtSh:
        case dbmtPa:
          if (ecdgOK_f) pCommon_->CanselJump(FALSE);
          break;
        case dbmtCa:
        case dbmtJCa:
          if ((st_.pstMyCh_->ECDjp_f == FALSE) && ecdgOK_f)
          {
            //mid::midLog("J1\n");
            if (pCommon_->CanselJump(FALSE) && muki_f)
            {
              st_.pstMyCh_->ECDjp_f = TRUE;//１回だけ
              st_.pstMyCh_->ECDdg_f = TRUE;//１回だけ
            }
            else
            {
              pCommon_->AutoPickUp();
            }
          }
          //オーバラインは審判息なのでもう不要
          break;
        case dbmtDg:
          if ((st_.pstMyCh_->ECDjp_f == FALSE) && ecdgOK_f)
          {
            //mid::midLog("J2\n");
            if (pCommon_->CanselJump(FALSE) && muki_f)
            {
              st_.pstMyCh_->ECDjp_f = TRUE;//１回だけ
              st_.pstMyCh_->ECDdg_f = TRUE;//１回だけ
            }
            else
            {
              //よけ限界時間
              ++st_.pstMyCh_->ECDdg_c;
              if ((st_.pstMyCh_->ECDdg_c < pmgEO_->mgDt_.dtSet_.GetDtInfield(setEnCourtCrTime))
                && MyPad()->IsDodge2())//押しっぱなしで避け続けるようにする
              {
                st_.pstMyCh_->Anime.Ani_c = 0;
              }
            }
          }

          break;
        }
      }
      break;

      //拾いに行く
    case dbatGet:
      switch (st_.pstMyCh_->Motion.Mtype)
      {
      case dbmtSt:
      case dbmtWk:
      case dbmtDs:
        pCommon_->AutoPickUp();//自動拾い
        break;
      }
      break;

      //よけ
    case dbatDg:
    {
      //COMの的コートオートよけ
      if ((IsSelfCtrl() == FALSE)
        && (pmgSG_->stBa_.Motion == bmShoot)
        && (ecdgOK_f))
      {
        switch (st_.pstMyCh_->Motion.Mtype)
        {
        case dbmtSt:
        case dbmtWk:
        case dbmtDs:
          if (st_.pstMyCh_->ECDdg_f == FALSE)
          {
            pCommon_->SetMtype(dbmtDg);
            pCommon_->CatchSE();
            //st_.pstMyCh_->ECDdg_f = TRUE;//１回だけ
          }
          break;
        case dbmtDg:
          //よけ限界時間
          ++st_.pstMyCh_->ECDdg_c;

          if (st_.pstMyCh_->ECDdg_c < pmgEO_->mgDt_.dtSet_.GetDtInfield(setEnCourtCrTime))
          {
            st_.pstMyCh_->Anime.Ani_c = 0;
          }
          break;
        }
      }
      else
      {
        switch (st_.pstMyCh_->Motion.Mtype)
        {
        case dbmtSt:
        case dbmtWk:
        case dbmtDs:
          pCommon_->SetMtypeReset(dbmtDg);
          pCommon_->CatchSE();
          break;
        case dbmtDg:
          //避け続けるようにする
          st_.pstMyCh_->Anime.Ani_c = 0;
          break;
        }
      }
    }
    break;
    //ＣＯＭキャッチ
    case dbatCPUCa:
      break;
      //ＣＯＭ思考行動★★
    case dbatCPUShPa:
      COMAction();
      break;
    }
  }

  //減速
  void TChAction::AbsorbXZ(BOOL ZWall_f)
  {
    if (ZWall_f == FALSE)
    {
      st_.pstMyCh_->Zahyou.dX = lib_num::Percent(st_.pstMyCh_->Zahyou.dX, pmgEO_->mgDt_.dtSet_.GetDtWall(setXWallAbsdX));
      st_.pstMyCh_->Zahyou.dZ = lib_num::Percent(st_.pstMyCh_->Zahyou.dZ, pmgEO_->mgDt_.dtSet_.GetDtWall(setXWallAbsdZ));
    }
    else
    {
      st_.pstMyCh_->Zahyou.dX = lib_num::Percent(st_.pstMyCh_->Zahyou.dX, pmgEO_->mgDt_.dtSet_.GetDtWall(setZWallAbsdX));
      st_.pstMyCh_->Zahyou.dZ = lib_num::Percent(st_.pstMyCh_->Zahyou.dZ, pmgEO_->mgDt_.dtSet_.GetDtWall(setZWallAbsdZ));
    }
  }
  //壁
  void TChAction::SetWallCrash(BOOL ZWall_f)
  {

    if (st_.pstMyCh_->enshCa_f)
    {
      pCommon_->SetMtype(dbmtFlB);
      //持ってたボール
      if (IsBall())
      {
        st_.pmgBa_->baCommon_.SetMtype(bmBound);
        st_.pmgBa_->baCommon_.ZeroXYZ();
      }
    }
    else
    {
      pCommon_->SetMtype(dbmtARv);
      st_.pstMyCh_->AirAct_f = FALSE;//動けるように
    }


    st_.pstMyCh_->Zahyou.dY = pmgEO_->mgDt_.dtSet_.GetDtWall(setWallBounddY);
    AbsorbXZ(ZWall_f);


  }
  //壁ヒット
  void TChAction::WallAbs(BOOL ZWall_f)
  {
    //14/12/05 BaJudgeに移動。
    //重力がいじられている特殊吹っ飛びのフラグをOFFに
    st_.pstMyCh_->flySpGrav_f = FALSE;
    st_.pstMyCh_->flyLoose_f = FALSE;
    //        st_.pstMyCh_->refHor_f = FALSE;
    //        st_.pstMyCh_->rndWorld_f = FALSE;

    switch (st_.pstMyCh_->Motion.Mtype)
    {
      //転がり中は何もしない
    case dbmtRoF:
    case dbmtRoB:
      return;
    default:
      //反射
      if (ZWall_f == FALSE)
      {
        st_.pstMyCh_->Zahyou.dX = -st_.pstMyCh_->Zahyou.dX;
        st_.pstMyCh_->Zahyou.dZ = 0;
      }
      else
      {//Z壁 反射後吹っ飛びの場合はZ方向を反転
        if (st_.pstMyCh_->refHor_f)//反射後吹っ飛び維持
        {
          st_.pstMyCh_->Zahyou.dZ = (-1) * st_.pstMyCh_->Zahyou.dZ;
        }
        else
        {
          st_.pstMyCh_->Zahyou.dZ = 0;//-st_.pstMyCh_->Zahyou.dZ;
        }
      }

      break;
    }

    switch (st_.pstMyCh_->Motion.Mtype)
    {
      //ふっとび
    case dbmtFlF:
    case dbmtFlB:
      //逆向き
      if (ZWall_f == FALSE)
      {
        st_.pstMyCh_->Zahyou.Muki = (st_.pstMyCh_->Zahyou.Muki == mL)
          ? mR
          : mL;
        pCommon_->SetMtypeReset(st_.pstMyCh_->Motion.Mtype);
      }
      AbsorbXZ(ZWall_f);
      st_.pstMyCh_->Zahyou.dY = pmgEO_->mgDt_.dtSet_.GetDtWall(setWallBounddY);

      break;

    case dbmtCM:
    case dbmtFB:
      SetWallCrash(ZWall_f);
      break;

    case dbmtCr:
      pCommon_->ZeroXYZ();
      break;

    default:

      if (st_.pstMyCh_->AirRev_f)
      {
        AbsorbXZ(ZWall_f);
        st_.pstMyCh_->Zahyou.dY = pmgEO_->mgDt_.dtSet_.GetDtWall(setWallBounddY);
      }
      else
      {
        if (ZWall_f == FALSE)
        {
          if (st_.pstMyCh_->Motion.IsMFlags(dbmfDs)
            && st_.pstMyCh_->Motion.IsMFlags(dbmfAr))
          {
            SetWallCrash(ZWall_f);
          }
          else
          {
            st_.pstMyCh_->Zahyou.dY = 0;
          }
        }
      }
      break;
    }
    //壁に当たったら回転フラグを消す
    st_.pstMyCh_->Roll_c = 0;
  }
  //空中着地点
  void TChAction::GetLandPos(s32 tGrv)
  {
    st_.pstMyCh_->NowGrv = tGrv;//現在の重力
    lib_num::AprTo0(&st_.pstMyCh_->LandRest);

    //計算済み
    if (st_.pstMyCh_->LandSet_f) return;

    s32 t1 = (st_.pstMyCh_->Zahyou.dY / tGrv);//最高点までの時間

    s32 top = (st_.pstMyCh_->Zahyou.dY > 0)//上り
      ? ((st_.pstMyCh_->Zahyou.dY * st_.pstMyCh_->Zahyou.dY) / (2 * tGrv))//最高点
      : (tGrv * (t1 * t1) / 2);//最高点だったところを逆算


    s32 t2 = (s32)(sqrt((f32)(2 * (st_.pstMyCh_->Zahyou.Y + top)) / tGrv));//最高点から地面までの時間

    //着地までの時間
    st_.pstMyCh_->LandRest = (t1 + t2);

    st_.pstMyCh_->LandX = st_.pstMyCh_->Zahyou.X + (st_.pstMyCh_->Zahyou.dX * st_.pstMyCh_->LandRest);
    st_.pstMyCh_->LandZ = st_.pstMyCh_->Zahyou.Z + (st_.pstMyCh_->Zahyou.dZ * st_.pstMyCh_->LandRest);

    //着地点が壁超え
    if (st_.pstMyCh_->LandX > DBCRT_W) { st_.pstMyCh_->LandX = DBCRT_W; }
    if (st_.pstMyCh_->LandX < 0) { st_.pstMyCh_->LandX = 0; }
    if (st_.pstMyCh_->LandZ > DBCRT_WAL) { st_.pstMyCh_->LandZ = DBCRT_WAL; }
    if (st_.pstMyCh_->LandZ < 0) { st_.pstMyCh_->LandZ = 0; }

    s32 LeftLandX = (IsInfield() ^ (st_.mysideNo_ == 0))
      ? DBCRT_CL - (st_.pstMyCh_->LandX - DBCRT_CL)
      : st_.pstMyCh_->LandX;
    //着地点が自コート以外
    if ((LeftLandX > DBCRT_CLI)
      || (LeftLandX < GetSLX())
      || (st_.pstMyCh_->LandZ < DBCRT_FL)
      || (st_.pstMyCh_->LandZ > DBCRT_BL))
    {
      st_.pstMyCh_->LandEnCourt_f = TRUE;
    }
    else
    {
      st_.pstMyCh_->LandEnCourt_f = FALSE;
    }

    st_.pstMyCh_->LandSet_f = TRUE;//計算済みに
  }



  //スリップ
  void TChAction::MvDefSl(s32 frc)
  {
    ++st_.pstMyCh_->Zahyou.Fric_c;
    lib_num::AprTo0(&st_.pstMyCh_->Zahyou.dX, ((st_.pstMyCh_->Zahyou.Fric_c * frc) / XYMAG));
    lib_num::AprTo0(&st_.pstMyCh_->Zahyou.dZ, ((st_.pstMyCh_->Zahyou.Fric_c * frc) / XYMAG));
  }

  void TChAction::MvDefSl_DSFric()
  {
    MvDefSl(pmgEO_->mgDt_.dtSet_.GetDtCourt(setDsStopFric));
    //if (st_.pstMyCh_->DsAcc_c == 0)
    //{
    //  MvDefSl(Rank(rkomDSFric));
    //}
    //else
    //{
    //  MvDefSl(Rank(rkomACCDSFric));
    //}
  }

  //歩き
  void TChAction::MvWk()
  {
    pCommon_->ZeroXYZ();
    //アナログ移動入力
    if (IsSelfCtrl()
      && (MyPad()->IsPadType(enPadType_DXL) == FALSE)
      //&& mid::midIsTBL()//MyPad()の先で判定
      && IsInfield()
      && (st_.pstMyCh_->Auto.AutoType == dbatFree))
    {
      s32 mvspd = pCommon_->GetWkX();
      BOOL vecagl_f = MyPad()->IsWalkVec();//アナログ歩き入力

      if (vecagl_f)
      {
        u32 vecagl = MyPad()->GetWalkVec();
        s32 xper = 0;
        s32 yper = 0;
        const s32 AG4 = 100;
        const s32 AG3 = 92;
        const s32 AG2 = 71;
        const s32 AG1 = 38;
        const s32 AG0 = 0;

        switch (vecagl) {
        case 0: case 8:
          xper = AG0;
          yper = AG4;
          break;
        case 1: case 7: case 9: case 15:
          xper = AG1;
          yper = AG3;
          break;
        case 2: case 6: case 10:case 14:
          xper = AG2;
          yper = AG2;
          break;
        case 3: case 5: case 11: case 13:
          xper = AG3;
          yper = AG1;
          break;
        case 4: case 12:
          xper = AG4;
          yper = AG0;
          break;
        }

        st_.pstMyCh_->Zahyou.dX = lib_num::Percent(mvspd, xper);
        st_.pstMyCh_->Zahyou.dZ = lib_num::Percent(mvspd, yper);
        st_.pstMyCh_->Zahyou.dZ = lib_num::Percent(st_.pstMyCh_->Zahyou.dZ, pmgEO_->mgDt_.dtSet_.GetDtCourt(setWkZPer));


        if (vecagl > 8)
        {
          st_.pstMyCh_->Zahyou.dX = -st_.pstMyCh_->Zahyou.dX;
        }
        if ((vecagl > 4) && (vecagl < 12))
        {
          st_.pstMyCh_->Zahyou.dZ = -st_.pstMyCh_->Zahyou.dZ;
        }

      }
    }
    else
    {
      //X
      if (IsInfield()
        || (st_.pstMyCh_->Auto.AutoType != dbatFree)
        || (st_.posNo_ == (s32)dbpoO2)
        || (st_.posNo_ == (s32)dbpoO3))
      {
#ifdef __K_DEBUG_SHIAI__
		  if (st_.mysideNo_ == 0 && kdebug::DebugSystem::GetInstance()->IsNotWalk())
		  {
			  return;
		  }
#endif	// #ifdef __K_DEBUG_SHIAI__
        if (pCommon_->GetMoveMuki(FALSE) == maL)
        {
          st_.pstMyCh_->Zahyou.dX = -pCommon_->GetWkX();
        }
        else if (pCommon_->GetMoveMuki(FALSE) == maR)
        {
          st_.pstMyCh_->Zahyou.dX = +pCommon_->GetWkX();
        }
      }

      //Z
      if (IsInfield()
        || (st_.pstMyCh_->Auto.AutoType != dbatFree)
        || (st_.posNo_ == (s32)dbpoO4))
      {
#ifdef __K_DEBUG_SHIAI__
		  if (st_.mysideNo_ == 0 && kdebug::DebugSystem::GetInstance()->IsNotWalk())
		  {
			  return;
		  }
#endif	// #ifdef __K_DEBUG_SHIAI__
        if (pCommon_->GetMoveMukiZ(FALSE) == mzaB)
        {
          st_.pstMyCh_->Zahyou.dZ = +pCommon_->GetWkZ();
        }
        else if (pCommon_->GetMoveMukiZ(FALSE) == mzaF)
        {
          st_.pstMyCh_->Zahyou.dZ = -pCommon_->GetWkZ();
        }
      }

      //斜め
      if ((st_.pstMyCh_->Zahyou.dX != 0) && (st_.pstMyCh_->Zahyou.dZ != 0))
      {
        st_.pstMyCh_->Zahyou.dX = lib_num::Sign(st_.pstMyCh_->Zahyou.dX) * pCommon_->GetNWkX();
        st_.pstMyCh_->Zahyou.dZ = lib_num::Sign(st_.pstMyCh_->Zahyou.dZ) * pCommon_->GetNWkZ();
      }
    }

  }
  //ダッシュ
  void TChAction::MvDs()
  {

    //加速カウンタ
    if ((st_.pstMyCh_->Auto.AutoType == dbatReturn)
      || (st_.pstMyCh_->Auto.AutoType == dbatKgReturn)
      || (st_.pstMyCh_->Auto.AutoType == dbatGet)
      || ((st_.pstMyCh_->Auto.AutoType == dbatCPUShPa) && (st_.pmgMyTm_->st_.pstMyTm_->stCOM.quick_f)))
    {
      ++st_.pstMyCh_->DsAcc_c;
    }
    else if (DashAccBtn_f())
    {
      ++st_.pstMyCh_->DsAcc_c;
    }
    else
    {
      lib_num::AprTo0(&st_.pstMyCh_->DsAcc_c);
    }

    s32 dsx = pmgEO_->mgDt_.dtSet_.GetDtCourt(setDashX);


    st_.pstMyCh_->Zahyou.dX = (st_.pstMyCh_->Zahyou.DsMuki == mL)
      ? -dsx
      : +dsx;

    //Z
    if (pCommon_->GetMoveMukiZ(FALSE) != mzaN)
    {
      s32 dsz = pmgEO_->mgDt_.dtSet_.GetDtCourt(setDashZ);
      st_.pstMyCh_->Zahyou.dZ = (pCommon_->GetMoveMukiZ(FALSE) != mzaF)
        ? +dsz
        : -dsz;
    }
    else
    {
      st_.pstMyCh_->Zahyou.dZ = 0;
    }

    //加速
    
    s32 tacc = (st_.pstMyCh_->DsAcc_c *RankSpeed(rkDashAcc));

    //限界加速
    if ((st_.pstMyCh_->NextAuto.AutoType == dbatReturn)
      || (st_.pstMyCh_->NextAuto.AutoType == dbatKgReturn)
      || (tacc > RankSpeed(rkDashMAXAcc)))
    {
      //            if (BackDs_f() == FALSE)//バックダッシュ以外を切ってみる//オートバックダッシュ
      {
        st_.pstMyCh_->DsAcc_c = RankSpeed(rkDashMAXAcc) / RankSpeed(rkDashAcc);
        tacc = RankSpeed(rkDashMAXAcc);
      }
    }

    if (st_.pstMyCh_->DsAcc_c > 0)
    {
      st_.pstMyCh_->Zahyou.dX = (st_.pstMyCh_->Zahyou.dX + (lib_num::Sign(st_.pstMyCh_->Zahyou.dX) * (tacc)));
    }

    ////ぬかるみ★
    //if (pmgEO_->mgStSn_.EtcSS.MudCourt_f())
    //{
    //    const s32 LOSPER = pmgEO_->mgDt_.dtSet_.GetDtCourt(setDsLossPer);
    //    const s32 MADMAX = (100 / LOSPER);
    //    if (st_.pstMyCh_->MadStep_c < MADMAX)
    //    {
    //        s32 nowlosper = LOSPER * st_.pstMyCh_->MadStep_c;
    //        st_.pstMyCh_->Zahyou.dX = lib_num::Percent(st_.pstMyCh_->Zahyou.dX, nowlosper);
    //        st_.pstMyCh_->Zahyou.dZ = lib_num::Percent(st_.pstMyCh_->Zahyou.dZ, nowlosper);
    //    }
    //}
  }
  //ジャンプ
  void TChAction::MvJp()
  {
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr))
    {
      st_.pstMyCh_->Zahyou.dY -= pmgEO_->mgDt_.dtSet_.GetDtGrv(setGRV_Y);//pmgEO_->mgDt_.dtOption_.GetDt(rkopGRV_Y, tGrvLv);
    }
  }

  //しゃがみ
  void TChAction::MvCr()
  {
    const s32 DECFRIC = 2;

    lib_num::AprTo0(&st_.pstMyCh_->JpCr_c);
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
    {
      MvDefSl_DSFric();
    }
    else
    {
      pCommon_->ZeroXYZ();
    }
    
  }
  //吹っ飛び
  void TChAction::MvFl()
  {
    //くっつきではない普通の吹っ飛び
    if (st_.pstMyCh_->Stick_f == FALSE)
    {
      if (lib_num::AprTo0(&st_.pstMyCh_->FlySkip_c))
      {
        s32 grvVer = 0;

        //Y　特殊な重力処理がない場合はデフォの吹っ飛び重力
        if (st_.pstMyCh_->flySpGrav_f == FALSE)
        {
          grvVer = pmgEO_->mgDt_.dtSet_.GetDtGrv(setFlyGrv);//吹っ飛び重力
        }
        //緩吹っ飛び効果の重力
        else if (st_.pstMyCh_->flyLoose_f == TRUE)
        {
          grvVer = pmgEO_->mgDt_.dtSet_.GetDtGrv(setFlyGrvLoose);
        }
        //世界一周効果＆壁跳ね効果の重力
        else if ((st_.pstMyCh_->rndWorld_f == TRUE) || (st_.pstMyCh_->refHor_f == TRUE))
        {
          //壁はねのLvのほうが高ければ壁跳ね効果の重力
          if (st_.pstMyCh_->rndWorldLv < st_.pstMyCh_->refHorLv)
          {
            grvVer = pmgEO_->mgDt_.dtRank_.GetRankDtPow(rkFryHRGrv, st_.pstMyCh_->refHorLv - 1);
          }
          else
          {
            //重力  世界一周効果の重力
            grvVer = pmgEO_->mgDt_.dtRank_.GetRankDtPow(rkFryGrGrv, st_.pstMyCh_->rndWorldLv - 1);
          }
        }

        //重力反映
        st_.pstMyCh_->Zahyou.dY -= grvVer;

        //下降にはいったら無敵消える
        if (//その場ダウン
            (st_.pstMyCh_->noMvDwn_f == FALSE)
            &&//くっつき後
            (st_.pstMyCh_->Sticked_f == FALSE)
            )
        {//その場ダウン×、くっつき後×なら、下降で無敵を消す
            if (st_.pstMyCh_->Zahyou.dY < 0)
            {
                st_.pstMyCh_->Motion.SubMFlags(dbmfMTK);
            }
        }
        else
        {//その場ダウンかくっつき後ならダウンをチェックしてダウンなら無敵をはずす
            if (st_.pstMyCh_->Motion.IsMFlags(dbmfDn))
            {
                st_.pstMyCh_->Motion.SubMFlags(dbmfMTK);
            }
        }

        //吸引なら再度重力反映
//        if (pmgSG_->stSh_.cmn.efEl.suctionLv > 0) st_.pstMyCh_->Zahyou.dY -= grvVer;

        //吸引吹っ飛び効果 ＆ 下降中 ＆ 球は必殺シュート ＆ 球に吸引効果アリ
        //現在のボール位置に向かって徐々に速度を上げながら向かう
        if (
//            (st_.pstMyCh_->Zahyou.dY < 0) 
            (st_.pstMyCh_->flying_c >= 10)
            && 
            (st_.pstMyCh_->suction_f) 
            && 
            (pmgSG_->stBa_.Motion == bmShoot)
            &&
            (pmgSG_->stSh_.cmn.efEl.suctionLv > 0)
            )
        {
            f32 flyX = pmgSG_->stBa_.Zahyou.dX / XYMAG;
            f32 flyZ = pmgSG_->stBa_.Zahyou.dX / XYMAG;
            f32 flyX_a = fabs(flyX);
            f32 flyZ_a = fabs(flyZ);

            //吸引加速度係数が０のとき・・・初期化を行う
            if (st_.pstMyCh_->suctionFlySpdRatio == 0)
            {
            //速度追加Lv初期計算
                //現在の吹っ飛び速度ＸＺをあわせたものをベースの速度とする
//                st_.pstMyCh_->suctionFlySpdBase = fabs(flyX) + fabs(flyZ);
                st_.pstMyCh_->suctionFlySpdBase = pmgSG_->stSh_.phs.spd * 1.5f;
                if (st_.pstMyCh_->suctionFlySpdBase < 3.0f) st_.pstMyCh_->suctionFlySpdBase = 3.0f;
                if (st_.pstMyCh_->suctionLv > 3) st_.pstMyCh_->suctionLv = 3;
                st_.pstMyCh_->suctionFlySpdBase = st_.pstMyCh_->suctionFlySpdBase * ((f32)st_.pstMyCh_->suctionLv + 3.0) / 4.0f;
//                const f32 SUC_UP_RATIO = 5.0f;
//                st_.pstMyCh_->suctionFlySpdRatio = st_.pstMyCh_->suctionLv * SUC_UP_RATIO;
                st_.pstMyCh_->suction_c = 1;
            }
            //方向・速度比
            f32 distX = (pmgSG_->stBa_.Zahyou.X - st_.pstMyCh_->Zahyou.X) / XYMAG;
            f32 distZ = (pmgSG_->stBa_.Zahyou.Z - st_.pstMyCh_->Zahyou.Z) / XYMAG;
            f32 distX_a = abs(distX);
            f32 distZ_a = abs(distZ);
            f32 spdDiv = 1.0f;
            f32 distXZ = distX_a + distZ_a;

            //現在の速度比率で元の速度を分割
            if (distXZ != 0) spdDiv = st_.pstMyCh_->suctionFlySpdBase / distXZ;


            if ((distX_a <= 4) && (st_.pstMyCh_->suctionLockX_f == FALSE))
            {
                st_.pstMyCh_->suctionLockX_f = TRUE;
                st_.pstMyCh_->suctionLockDistX = distX;
            }
            if ((distZ_a <= 4) && (st_.pstMyCh_->suctionLockZ_f == FALSE))
            {
                st_.pstMyCh_->suctionLockZ_f = TRUE;
                st_.pstMyCh_->suctionLockDistZ = distZ;
            }

            //実際の速度(速度ベースを距離XZで分解したもの)×距離×（加速比率×吸引カウント／100）× XYMAG 
            //→／100）× XYMAG は相殺
            st_.pstMyCh_->Zahyou.dX = spdDiv * distX * XYMAG ;
            if (st_.pstMyCh_->suctionLockX_f == TRUE)
            {
                st_.pstMyCh_->Zahyou.X = pmgSG_->stBa_.Zahyou.X - st_.pstMyCh_->suctionLockDistX * XYMAG;
                st_.pstMyCh_->Zahyou.dX = 0;
            }

            if (st_.pstMyCh_->suctionLockZ_f == TRUE)
            {
                st_.pstMyCh_->Zahyou.Z = pmgSG_->stBa_.Zahyou.Z - st_.pstMyCh_->suctionLockDistZ * XYMAG;
                st_.pstMyCh_->Zahyou.dZ = 0;
            }
            st_.pstMyCh_->Zahyou.dZ = spdDiv * distZ * XYMAG;

            st_.pstMyCh_->suction_c = st_.pstMyCh_->suction_c + 1;

        }
      }

      st_.pstMyCh_->flying_c = st_.pstMyCh_->flying_c + 1;

    }
    else//アッパーなどのくっつき
    {
      st_.pstMyCh_->FlySkip = 0;
      st_.pstMyCh_->FlySkip_c = 0;

      //シュート終わりで　終了
      if (pmgSG_->stBa_.Motion != bmShoot) st_.pstMyCh_->Stick_f = FALSE;

      const s32 REVDIST = 1 * XYMAG;
      s32 r;

      //Y
//      st_.pstMyCh_->Zahyou.dY = lib_num::Max(XYMAG, pmgSG_->stBa_.Zahyou.dY);
//      st_.pstMyCh_->Zahyou.dY = pmgSG_->stBa_.Zahyou.dY;
      //地上停止時にY上昇する可能性があるので↑を辞めてボールの１Fr前の座標差を用いる　15/06/14
      st_.pstMyCh_->Zahyou.dY = pmgSG_->stSh_.cmn.prsPY - pmgSG_->stSh_.cmn.pstPY;

      //くっつきの最中なら無敵
      if (st_.pstMyCh_->Sticking_f == TRUE) st_.pstMyCh_->Motion.AddMFlags(dbmfMTK);

      //くっつき中でない場合は真横に吹っ飛ぶ
      if ((st_.pstMyCh_->Stick_f == TRUE) && (st_.pstMyCh_->Sticking_f == FALSE))
      {
          st_.pstMyCh_->Stick_f = FALSE;
          st_.pstMyCh_->Sticked_f = TRUE;
          st_.pstMyCh_->Motion.AddMFlags(dbmfMTK);
      }

      //X
      if (pmgSG_->stBa_.Zahyou.X < (st_.pstMyCh_->Zahyou.X - REVDIST))
      {
        r = -REVDIST;
      }
      else if (pmgSG_->stBa_.Zahyou.X > (st_.pstMyCh_->Zahyou.X + REVDIST))
      {
        r = +REVDIST;
      }
      else
      {
        r = 0;
      }
      st_.pstMyCh_->Zahyou.dX = pmgSG_->stBa_.Zahyou.dX + r;

      //Z
      if (pmgSG_->stBa_.Zahyou.Z < (st_.pstMyCh_->Zahyou.Z - REVDIST))
      {
        r = -REVDIST;
      }
      else if (pmgSG_->stBa_.Zahyou.Z > (st_.pstMyCh_->Zahyou.Z + REVDIST))
      {
        r = +REVDIST;
      }
      else
      {
        r = 0;
      }
      st_.pstMyCh_->Zahyou.dZ = pmgSG_->stBa_.Zahyou.dZ + r;

      //ちうちう系で最大ヒット数を超えていたら強制的に無敵
      if (
          (st_.pstMyCh_->combo_c > st_.pstMyCh_->comboMax) 
          &&
          (st_.pstMyCh_->comboMax > 0)
          )
      { 
          st_.pstMyCh_->Motion.AddMFlags(dbmfMTK);
      }
      else
      {
      //連続ヒットカウントチェック
      switch (st_.pstMyCh_->hitMTime_cd)
      {
          //連続ヒットカウントなし
      case 0:
          //元々コンボなし
          if (st_.pstMyCh_->comboMax <= 0)
          {
              //その場ダウン
              if (st_.pstMyCh_->noMvDwn_f == TRUE)
              {//ダウンで無敵を解く
                  if (st_.pstMyCh_->Motion.IsMFlags(dbmfDn)) st_.pstMyCh_->Motion.SubMFlags(dbmfMTK);
              }
              else//くっつき中
              if (st_.pstMyCh_->Sticking_f == TRUE)
              {
                  st_.pstMyCh_->Motion.AddMFlags(dbmfMTK);
              }
              else//くっつき後
              if (st_.pstMyCh_->Sticked_f == TRUE)
              {//ダウンをチェックしてダウンなら無敵をはずす
                  //地上以下位置 ＆ 下降速度
                  if ((st_.pstMyCh_->Zahyou.Y <= 0) && (st_.pstMyCh_->Zahyou.dY < 0)) 
                      st_.pstMyCh_->Motion.SubMFlags(dbmfMTK);
                  else st_.pstMyCh_->Motion.AddMFlags(dbmfMTK);
              }
              else//基本は、下降で無敵を消す
              {
                  if (st_.pstMyCh_->Zahyou.dY < 0) st_.pstMyCh_->Motion.SubMFlags(dbmfMTK);
              }
          }
          else//コンボアリの場合
          {
//              if ((st_.pstMyCh_->Zahyou.Y <= 0) && (st_.pstMyCh_->Zahyou.dY < 0))
//                  st_.pstMyCh_->Motion.SubMFlags(dbmfMTK);
              if (st_.pstMyCh_->combo_c < st_.pstMyCh_->comboMax)
              {
                  st_.pstMyCh_->Motion.SubMFlags(dbmfMTK);
              }
              //              if (st_.pstMyCh_->Zahyou.Y <= 0) st_.pstMyCh_->Motion.SubMFlags(dbmfMTK);
              //コンボ時間　＆　でくっつき中なら
//              if (st_.pstMyCh_->Sticking_f == TRUE)
//              {
//                  st_.pstMyCh_->Motion.AddMFlags(dbmfMTK);
//              }
          }
          break;
          //連続ヒットカウント中で時間カウント１のとき
      case 1:
          if (st_.pstMyCh_->combo_c < st_.pstMyCh_->comboMax) st_.pstMyCh_->Motion.SubMFlags(dbmfMTK);
//          st_.pstMyCh_->hitMTime_cd = 0;
          //↑dbChCountに移動　15/06/05
          break;
          //連続ヒットカウント中で時間カウント２以上のとき
//      default: st_.pstMyCh_->hitMTime_cd = st_.pstMyCh_->hitMTime_cd - 1; break;
      default: break;//↑dbChCountに移動　15/06/05
      }
      }



    }
  }
  //キャッチモーション
  void TChAction::MvCM()
  {
    const s32 DECFRIC = 2;
    st_.pstMyCh_->Zahyou.dX = lib_num::Percent(st_.pstMyCh_->Zahyou.Fric_c, st_.pstMyCh_->FricX);
    st_.pstMyCh_->Zahyou.dZ = lib_num::Percent(st_.pstMyCh_->Zahyou.Fric_c, st_.pstMyCh_->FricZ);
    lib_num::AprTo0(&st_.pstMyCh_->Zahyou.Fric_c, DECFRIC);
    //MvSl();
  }
  //シュートパス
  void TChAction::MvShPa()
  {
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
    {
      const s32 DEFFRIC = (st_.pstMyCh_->Utrun_f)
        ? 30
        : 50;//100
      MvDefSl(DEFFRIC);
    }
    else
    {
      MvDefSl_DSFric();
    }
  }
  //よけキャッチ
  void TChAction::MvDgCa()
  {
    if (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
    {
      //MvDefSl(Rank(rkomDSFric));
      MvDefSl_DSFric();
      //MvAcSl(Rank(rkomDSFric));
    }
    else
    {
      pCommon_->ZeroXYZ();
    }
  }

  //座標移動
  void TChAction::MoveZahyou()
  {

    //速すぎ防止
    st_.pstMyCh_->Zahyou.dX = lib_num::Between(st_.pstMyCh_->Zahyou.dX, -MAXSPD, MAXSPD);
    st_.pstMyCh_->Zahyou.dY = lib_num::Between(st_.pstMyCh_->Zahyou.dY, -MAXSPD, MAXSPD);
    st_.pstMyCh_->Zahyou.dZ = lib_num::Between(st_.pstMyCh_->Zahyou.dZ, -MAXSPD, MAXSPD);

    //移動量反映
    st_.pstMyCh_->Zahyou.X += st_.pstMyCh_->Zahyou.dX;
    st_.pstMyCh_->Zahyou.Y += st_.pstMyCh_->Zahyou.dY;
    st_.pstMyCh_->Zahyou.Z += st_.pstMyCh_->Zahyou.dZ;

    //シュート中でマリオネット効果中
    if ((st_.pstMyCh_->symCtrl_f == TRUE) && (st_.pstBa_->Motion == bmShoot))
    {
        st_.pstMyCh_->Motion.MFlags = (dbmfAr | dbmfMuki | dbmfAtCa);

        //        pCommon_->SetMtype(dbmtJDn);//ジャンプ状態
        
        const f32 DEC_SPD = 0.5f;

        f32 spdX_fl = DEC_SPD * st_.pstBa_->Zahyou.dX / XYMAG;
        f32 spdY_fl = DEC_SPD * st_.pstBa_->Zahyou.dY / XYMAG;
        f32 spdZ_fl = DEC_SPD * st_.pstBa_->Zahyou.dZ / XYMAG;
        s32 spdX = spdX_fl * XYMAG;
        s32 spdY = spdY_fl * XYMAG;
        s32 spdZ = spdZ_fl * XYMAG;

        //移動量をボールから取得･･･XZは反転
        st_.pstMyCh_->Zahyou.dX = spdX * (-1);
        st_.pstMyCh_->Zahyou.dY = spdY * (-1);
        st_.pstMyCh_->Zahyou.dZ = spdZ * (-1);

        //移動量反映
        st_.pstMyCh_->Zahyou.X += st_.pstMyCh_->Zahyou.dX;
        st_.pstMyCh_->Zahyou.Y += st_.pstMyCh_->Zahyou.dY;
        st_.pstMyCh_->Zahyou.Z += st_.pstMyCh_->Zahyou.dZ;



        //Ｙが１未満なら１にする
        if (st_.pstMyCh_->Zahyou.Y < (3 * XYMAG)) st_.pstMyCh_->Zahyou.Y = 3 * XYMAG;
    }
    else//シュート中以外でマリオネット効果なら
    if (st_.pstMyCh_->symCtrl_f == TRUE)
    {//マリオネット効果を消す
        st_.pstMyCh_->symCtrl_f = FALSE;
        st_.pstMyCh_->Motion.AddMFlags(dbmfAr); //ジャンプ状態
//        pCommon_->SetMtype(dbmtJDn);//ジャンプ状態
    }


    //吹っ飛び中フラグ
    BOOL flying_f = FALSE;
    //吹っ飛びモーション中ならフラグオン
    if ((st_.pstMyCh_->Motion.Mtype == dbmtFlF) || (st_.pstMyCh_->Motion.Mtype == dbmtFlB)) flying_f = TRUE;

    //キャッチスリップ中フラグ
    BOOL slippingCh_f = FALSE;
    //キャッチスリップ中ならフラグオン
    if (st_.pstMyCh_->Motion.Mtype == dbmtCM) slippingCh_f = TRUE;


    if (st_.pstMyCh_->Zahyou.Z < 0)
    {
        st_.pstMyCh_->Zahyou.Z;
    }

    //Ｘ限界
    if ((pmgSG_->stRf_.DBSSt != dbssGohome)//デモ帰宅時は壁抜け
      && (((st_.pstMyCh_->Zahyou.X > DBCRT_W) && (st_.pstMyCh_->Zahyou.dX > 0))
      || ((st_.pstMyCh_->Zahyou.X < 0) && (st_.pstMyCh_->Zahyou.dX < 0))))
    {
      BOOL RtoL_f = (st_.pstMyCh_->Zahyou.X > DBCRT_W);

      //世界一周と壁反射処理　X壁の場合世界一周が優先される
      if (
          (//吹っ飛び状態で、世界一周効果or壁はね効果を持つ
            (flying_f)
            &&
            ((st_.pstMyCh_->Round_f) || (st_.pstMyCh_->refHor_f))
          )
          ||
          (//キャッチスリップ状態で、世界一周効果シュートのキャッチor壁はね効果シュートのキャッチ
          //内野キャラキャッチ限定15/06/29
            (slippingCh_f) 
            &&
            ((st_.pstMyCh_->rndWorldCh_f) || (st_.pstMyCh_->refHorCh_f))
            &&
            (st_.posNo_ < 4)
          )
        )
      {
//        if (st_.pstMyCh_->Round_f)//世界一周
          if (
              ((flying_f) && (st_.pstMyCh_->Round_f))
              ||
              ((slippingCh_f) && (st_.pstMyCh_->rndWorldCh_f))
              )//世界一周
            {
                st_.pstMyCh_->Zahyou.X = (RtoL_f)
                    ? 0 + 1
                    : DBCRT_W - 1;
            }
//        else if (st_.pstMyCh_->refHor_f)//反射後吹っ飛び維持　世界一周がなくて壁反射があった場合は逆方向に吹っ飛ぶ
            else 
            if (
                ((flying_f) && (st_.pstMyCh_->refHor_f))
                ||
                ((slippingCh_f) && (st_.pstMyCh_->refHorCh_f))
                )//反射後吹っ飛び維持　世界一周がなくて壁反射があった場合は逆方向に吹っ飛ぶ
            {
                st_.pstMyCh_->Zahyou.dX = (-1) * st_.pstMyCh_->Zahyou.dX;
                if ((slippingCh_f) && (st_.pstMyCh_->refHorCh_f)) //反射効果シュートをキャッチしているとき
                {
                    st_.pstMyCh_->FricX = (-1) * st_.pstMyCh_->FricX;//保存されている運動量の方向を反転
                }
            }
      }
      else
      {
        st_.pstMyCh_->Zahyou.X = (RtoL_f)
          ? DBCRT_W
          : 0;
        WallAbs(FALSE);//壁反射
      }
    }

    //Ｚ限界
    if ((st_.pstMyCh_->Zahyou.Z > DBCRT_WAL) || (st_.pstMyCh_->Zahyou.Z < 0))
    {
      //世界一周と壁反射処理　X壁の場合世界一周が優先される
      if ((flying_f) && (st_.pstMyCh_->refHor_f))//壁反射吹っ飛び中
      {
        st_.pstMyCh_->Zahyou.dZ = (-1) * st_.pstMyCh_->Zahyou.dZ;
      }
      else
      if ((slippingCh_f) && (st_.pstMyCh_->refHorCh_f))//壁反射吹っ飛び効果シュートをキャッチ中
      {
          st_.pstMyCh_->Zahyou.dZ = (-1) * st_.pstMyCh_->Zahyou.dZ;
          st_.pstMyCh_->FricZ = (-1) * st_.pstMyCh_->FricZ;//保存されている運動量の方向を反転
      }
      else
      {
        BOOL flont_f = (st_.pstMyCh_->Zahyou.Z < 0);
        st_.pstMyCh_->Zahyou.Z = (flont_f)
          ? 0
          : DBCRT_WAL;
        st_.pstMyCh_->Roll_c = 0;
        WallAbs(TRUE);//壁反射
      }
    }

    
    //超天井
    const s32 MAXH = 400 * XYMAG;
    if (st_.pstMyCh_->Zahyou.Y > MAXH)
    {
      st_.pstMyCh_->Zahyou.Y = MAXH;
    }
    else if (st_.pstMyCh_->Zahyou.Y < 0)//着地も一応ここでやるか
    {
      st_.pstMyCh_->JpCr_c = 0;
      st_.pstMyCh_->LandSet_f = FALSE;

      if (st_.pstMyCh_->Motion.IsMFlags(dbmfDam))
      {
        if ((st_.pstMyCh_->Roll_c > 0) && (abs(st_.pstMyCh_->Zahyou.dX) > XYMAG))
        {
          st_.pstMyCh_->Zahyou.dX = lib_num::Sign(st_.pstMyCh_->Zahyou.dX) * (pmgEO_->mgDt_.dtSet_.GetDtCourt(setRollSpeed));
          //st_.pstMyCh_->Zahyou.dY = 0;
          st_.pstMyCh_->Zahyou.dZ = 0;
          if (st_.pstMyCh_->Motion.Mtype == dbmtFlF)
          {
            pCommon_->SetMtype(dbmtRoB);//一週後
          }
          else
          {
            pCommon_->SetMtype(dbmtRoF);//一週後
          }
        }
        else
        {
          //死亡処理
          //敵が全員死んでいるときは死なない
          if ((st_.pstMyCh_->HP <= 0)
            && st_.pmgRf_->IsAngeling()
            && (st_.pmgEnTm_->IsAllDead() == FALSE))
          {
            st_.pstMyCh_->ANGEL_f = TRUE;
            st_.pmgMyTm_->CheckChangePos();
          }
          else
          {
            if ((st_.pstMyCh_->Motion.Mtype == dbmtFlB)
              && (st_.pstMyCh_->Zahyou.dX == 0))//もずおとしのふっとびは転がらない
            {
              pCommon_->SetMtype(dbmtDnB);
            }
            else
            {
              if (st_.pstMyCh_->Motion.Mtype == dbmtFlF)
              {
                pCommon_->SetMtype(dbmtDnF);
              }
              else
              {
                pCommon_->SetMtype(dbmtDnB);
              }
            }

            st_.pstMyCh_->Zahyou.dY = 0;
          }
        }
      }
      else
      {
        //着地してもしゃがんだまま
        if (st_.pstMyCh_->Motion.Mtype == dbmtJDg)
        {
          pCommon_->SetMtype(dbmtDg);
          pCommon_->ZeroXYZ();
        }
        else
        {
          pCommon_->SetMtype(dbmtCr);
          st_.pstMyCh_->JpCr_c = pmgEO_->mgDt_.dtSet_.GetDtJump(setJumpLandTime);
          pCommon_->ZeroXYZ();
        }
      }
      st_.pstMyCh_->Zahyou.Y = 0;
      st_.pstMyCh_->Zahyou.dY = 0;
    }


  }

  //ライン補正
  void TChAction::CheckLine(enCrtLine lineType, s32 line)
  {
    switch (lineType)
    {
    case crtLLine:
      if (GetLeftCrtX() < line)
      {
        SetLeftCrtX(line, lineType);
      }
      break;
    case crtRLine:
      if (GetLeftCrtX() > line)
      {
        SetLeftCrtX(line, lineType);
      }
      break;
    case crtFLine:
      if (st_.pstMyCh_->Zahyou.Z < line)
      {
        st_.pstMyCh_->Zahyou.Z = line;
      }
      break;
    case crtBLine:
      if (st_.pstMyCh_->Zahyou.Z > line)
      {
        st_.pstMyCh_->Zahyou.Z = line;
      }
      break;
    }
  }
  //オーバーライン
  void TChAction::CheckOverLine()
  {
    //自分で操作時、歩いてオーバーラインしない
    switch (st_.pstMyCh_->Motion.Mtype)
    {
    case dbmtSt:
    case dbmtWk:
    case dbmtDs:
    case dbmtSl:
      switch (st_.pstMyCh_->Auto.AutoType)
      {
      case dbatFree:
      case dbatAlleyoop:
      case dbatDfPos:
      case dbatCPUHelp:
      case dbatShCv:
      case dbatCPUShPa:
        switch (st_.posNo_)
        {
        case (s32)dbpoO2:
          st_.pstMyCh_->Zahyou.Z = CRPOS_B_Z;
          CheckLine(crtRLine, DBCRT_CLI);
          CheckLine(crtLLine, CRPOS_B_XL);
          break;
        case (s32)dbpoO3:
          st_.pstMyCh_->Zahyou.Z = CRPOS_F_Z;
          CheckLine(crtRLine, DBCRT_CLI);
          CheckLine(crtLLine, CRPOS_F_XL);
          break;
        case (s32)dbpoO4:
          SetLeftCrtX(GetSLX() - CRPOS_S_X);
          //CheckLine(crtRLine, (GetSLX() - CRPOS_S_X));
          CheckLine(crtBLine, DBCRT_BL);
          CheckLine(crtFLine, DBCRT_FL);
          break;
        default:
          if (st_.pstMyCh_->Auto.AutoType == dbatFree)
          {
            //先にＺ軸補正することで左下カドに行ったときに戻ってしまうのをなくす
            CheckLine(crtBLine, DBCRT_BL);
            CheckLine(crtFLine, DBCRT_FL);
            if (st_.pstMyCh_->Motion.Mtype == dbmtWk)
            {
              CheckLine(crtRLine, DBCRT_CLI);
              CheckLine(crtLLine, GetSLX());
            }
          }
          break;
        }
        break;
      case dbatGet://★内野ボール拾いの時はオーバーラインしないように
        if (IsInfield())
        {
          //先にＺ軸補正することで左下カドに行ったときに戻ってしまうのをなくす
          CheckLine(crtBLine, DBCRT_BL);
          CheckLine(crtFLine, DBCRT_FL);
          if (st_.pstMyCh_->Motion.Mtype == dbmtWk)
          {
            CheckLine(crtRLine, DBCRT_CLI);
            CheckLine(crtLLine, GetSLX());
          }
        }
        break;
      case dbatReturn:
        switch (st_.posNo_)
        {
        case (s32)dbpoO2:
          //下に進んでるとき
          if (st_.pstMyCh_->Zahyou.dZ < 0)
          {
            CheckLine(crtFLine, CRPOS_B_Z);
          }
          break;
        case (s32)dbpoO3:
          //上に進んでるとき
          if (st_.pstMyCh_->Zahyou.dZ > 0)
          {
            CheckLine(crtFLine, CRPOS_F_Z);
          }
          break;
        case (s32)dbpoO4:
          //相手コート側に進んでいるとき
          //if (((st_.mysideNo_ == 0) && (st_.pstMyCh_->Zahyou.dX < 0))
          //  || ((st_.mysideNo_ == 1) && (st_.pstMyCh_->Zahyou.dX > 0)))
          //{
          CheckLine(crtRLine, (GetSLX() - CRPOS_S_X));
          //}
          break;
        }
        break;
      default:
        break;
      }
      break;
    case dbmtJUp:
    case dbmtJDn:
      //４番外野Ｚ軸のみミドルジャンプはエンドラインに沿う
      if ((st_.pstMyCh_->Auto.AutoType == dbatFree)
        && (st_.posNo_ == (s32)dbpoO4)
        && (st_.pstMyCh_->Zahyou.dX == 0))
      {
        SetLeftCrtX(GetSLX() - CRPOS_S_X);
      }
      break;
    default:
      break;
    }
  }

  //着地位置
  void TChAction::SetLand()
  {
    s32 nowGrv = pmgEO_->mgDt_.dtSet_.GetDtGrv(setGRV_Y);//pmgEO_->mgDt_.dtOption_.GetDt(rkopGRV_Y, tGrvLv);

    switch (st_.pstMyCh_->Motion.Mtype)
    {
    case dbmtJUp:
    case dbmtJDn:
    case dbmtJSh:
    case dbmtJPa:
    case dbmtJCa:
    case dbmtJDg:
      GetLandPos(nowGrv);
      break;
    case dbmtFlF:
    case dbmtFlB:
      GetLandPos(pmgEO_->mgDt_.dtSet_.GetDtGrv(setFlyGrv));
      break;
    default:
      //空中以外は現在位置を入れとく
      st_.pstMyCh_->LandX = st_.pstMyCh_->Zahyou.X;
      st_.pstMyCh_->LandZ = st_.pstMyCh_->Zahyou.Z;
      st_.pstMyCh_->LandEnCourt_f = FALSE;
      break;
    }
  }

  //座標移動
  void TChAction::Moving()
  {

    //ヒットストップ
    //if (st_.pstMyCh_->HitStop_f) return;

    if (IsInplay() == FALSE) return;

    switch (st_.pstMyCh_->Motion.Mtype)
    {
      //歩き
    case dbmtWk: MvWk(); break;
      //ダッシュ
    case dbmtDs: MvDs(); break;
      //ジャンプ
    case dbmtJUp:
    case dbmtJDn:
    case dbmtJCM:
    case dbmtJFB:
    case dbmtJSh:
    case dbmtJPa:
    case dbmtJCa:
    case dbmtJDg: MvJp(); break;

      //しゃがみ
    case dbmtCr: MvCr(); break;
      //ふっとび
    case dbmtFlF:
    case dbmtFlB: MvFl(); break;
      //ころがり
    case dbmtRoF:
    case dbmtRoB:
      break;
      //キャッチモーション
    case dbmtCM:
    case dbmtFB:
      MvCM();
      break;
      //スリップ
    case dbmtSl:
      MvDefSl_DSFric();
      break;
      //シュートパス
    case dbmtSh:
    case dbmtPa: MvShPa(); break;
      //よけキャッチ
    case dbmtDg:
    case dbmtCa: MvDgCa(); break;
      //氷床ダウン滑り
    case dbmtDnF:
    case dbmtDnB:
      //if (pmgEO_->mgStSn_.EtcSS.IceCourt_f())
      //{
      //    MvDefSl(pmgEO_->mgDt_.dtSet_.GetDtCourt(setDownBrkIce));
      //}
      //else
    {
      pCommon_->ZeroXYZ();
    }
    break;
    default:
      pCommon_->ZeroXYZ();
      break;
    }


    //吹っ飛びスキップ
    if ((st_.pstMyCh_->Motion.Mtype == dbmtFlF)
      || (st_.pstMyCh_->Motion.Mtype == dbmtFlB))
    {
      if ((st_.pstMyCh_->FlySkip > 0) && (st_.pstMyCh_->FlySkip_c == 0))
      {
        for (s32 i = 0; i < st_.pstMyCh_->FlySkip; ++i)
        {
          //ふっとび
          MvFl();
          //座標移動
          MoveZahyou();
        }
        st_.pstMyCh_->FlySkip = 0;//スキップ終了
      }
    }
    else
    {
      st_.pstMyCh_->FlySkip_c = 0;
    }

    //座標移動
    if (st_.pstMyCh_->FlySkip_c == 0)
    {
      MoveZahyou();
    }

    //オーバーライン補正
    CheckOverLine();

    //着地位置
    SetLand();
  }

  //更新
  void TChAction::UpDate()
  {
    //デモ帰宅
    if ((pmgSG_->stRf_.DBSSt != dbssGohome) && (pmgGO_->pmgRf_->IsFinTuto() == FALSE))
    {
      //キー入力（アクション）
      MCKeyInputAT();
    }
    else
    {
      if (st_.pstMyCh_->HP > 0) pCommon_->SetMtype(dbmtSt);
    }

    //座標移動
    Moving();
  }


}
