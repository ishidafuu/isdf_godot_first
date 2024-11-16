namespace db;

public partial class TeamBehavior
{
    //自チームのダッシュマンジャンプ不可
    public bool IsNoJpDashman()
    {
        return false;
        // return (pmgGO_->pmgCh_[st_.mysideNo_][st_.pstMyTm_->RdNo]->MyPDt(clv_DsmnNoJp) != 0);
    }


    //自チームの攻撃位置目標Ｘ★
    private int GenGetAtcLineX(bool jp_f, bool dsmn_f)
    {
        if (IsNoJpDashman() && dsmn_f)
        {
            //強ジャンプラインと同じに
            return GetSettingFormation(SettingFormationType.AtcLineX3) * Defines.Percent;
        }
        // 仮
        int tgx = GetSettingFormation(SettingFormationType.AtcLineX) + CaptainChara.Level.AttackLine;

        //ジャンプ踏切位置は少し後ろに
        if (jp_f)
        {
            const int JPLINE = 24;
            tgx += JPLINE;
        }

        return tgx * Defines.Percent;
    }


    //自チームの攻撃位置目標Ｘ★
    public int GetAtcLineX(bool jp_f, bool dsmn_f)
    {
        return Defines.DBCRT_CL - GenGetAtcLineX(jp_f, dsmn_f);
    }


    /// <summary>
    /// 全員アウト
    /// </summary>
    public bool IsAllOut
    {
        get
        {
            foreach (var mySideChara in MySideOrders)
            {
                if (mySideChara.Composite.IsOut == false)
                {
                    return false;
                }
            }

            return true;
        }
    }


    public bool IsCom => SemiAuto.SemiF || Main.ManSideF == false;


    //待ち時間超えてる
    public bool IsWaitOver()
    {
        bool res = false;
        if ( EnemyTeam.IsAllNoShTg()) //誰もねらえない
        {
            res = false;
        }
        else if (MyTeamAiState.Main.QuickF) //クイック
        {
            res = true;
        }
        else if (MyTeamAiState.Attack.ShF) //シュート
        {
            if (MyTeamState.MainState.ControlOrderIndex != OrderIndexType.Disabled)
            {
                res = MyTeamAiState.Main.TmwaitC > MyTeamAiState.Action[(int)MyTeamState.MainState.ControlOrderIndex].ShWait;
            }
            else
            {
                res = MyTeamAiState.Main.TmwaitC > MyTeamAiState.Action[0].ShWait;
            }
        }
        else //パス
        {
            var action = MyTeamState.MainState.ControlOrderIndex != OrderIndexType.Disabled
                ? MyTeamAiState.Action[(int)MyTeamState.MainState.ControlOrderIndex]
                : MyTeamAiState.Action[0];
            
            if (MyTeamAiState.Attack.ShF) 
            {
                res = MyTeamAiState.Main.TmwaitC > action.ShWait;
            }
            else
            {
                res = MyTeamAiState.Main.TmwaitC > action.PaWait;
            }
        }

        return res;
    }


    public bool IsAllNoShTg() //全員ねらえる状態でない
    {
        int tagok = 0;

        for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
        {
            if (MySideOrders[i].Composite.IsEnableShootTarget)
            {
                tagok++;
            }
        }
        return tagok == 0;
    }
//
//     
//     #include "dbMgTeam.h"
// #include "dbGame.h"
//
// namespace db
// {
//     //チーム管理およびチーム内プレーヤーオブジェクトの管理////////////////////////////////
//     //コンストラクタ
//     TMgTeam::TMgTeam(TGame* pGame, int sideNo)
//         :TGameObjGen(pGame)
//     {
//         st_.Init();
//         st_.mysideNo_ = sideNo;
//         st_.ensideNo_ = (st_.mysideNo_ == 0) ? 1 : 0;//敵サイド番号
//     }
//
//     TMgTeam::~TMgTeam()
//     {
//     }
//
//     //初期化（ポインタセットし直し、全てのオブジェが生成された後でないとポインタが確定してない）
//     void TMgTeam::RefSet()
//     {
//         //マネージャーポインタ
//         st_.pmgMyTm_ = pmgGO_->pmgTm_[st_.mysideNo_];
//         st_.pmgEnTm_ = pmgGO_->pmgTm_[st_.ensideNo_];
//         st_.pmgBa_ = pmgGO_->pmgBa_;
//         st_.pmgRf_ = pmgGO_->pmgRf_;
//
//         //ステータスポインタ
//         st_.pstRf_ = &pmgSG_->stRf_;//レフェリーステータス
//         st_.pstBa_ = &pmgSG_->stBa_;//ボールステータス
//         st_.pstMyTm_ = &pmgSG_->stTm_[st_.mysideNo_];//チームステータス
//         st_.pstEnTm_ = &pmgSG_->stTm_[st_.ensideNo_];//チームステータス
//
//     }
//
//     //パッドのセット
//     void TMgTeam::SetSidePad()
//     {
//         st_.pstMyTm_->MANSide_f = pmgEO_->stPad_.IsPadCtrlSide(st_.mysideNo_);
//         st_.pstMyTm_->MANSidePadNo = pmgEO_->stPad_.GetPadCtrlSideNo(st_.mysideNo_);
//         //全ての状況でサイド側のパッドを持つようにする
//         st_.pSidePad_ = pmgGO_->pmgPad_[st_.mysideNo_];
//     }
//
//     //ステータス初期化
//     void TMgTeam::Ready(BOOL tDemo_f)//チーム設定をシーンから貰うか
//     {
//         const int FIRSTCTRL = (int)dbpoI3;
//
// 		st_.id_kantoku_ = st_.pstMyTm_->PosSt.id_kantoku_;
//         st_.mid_team_ = st_.pstMyTm_->PosSt.mid_team_;
//         st_.id_team_ = st_.pstMyTm_->PosSt.id_team_;
//         st_.id_m_shset_ = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(st_.mid_team_, mdm_team_id_m_shset);
//         st_.rarity_ = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(st_.mid_team_, mdm_team_rarity);
//         st_.comNo_ = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(st_.mid_team_, mdm_team_com);
//         st_.fomNo_ = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(st_.mid_team_, mdm_team_com);//現状ＣＯＭ番号と一致
//         st_.droped_f_ = FALSE;
//         //選手
//         for (int i = 0; i < DBMEMBER_ALL; ++i)
//         {
//             st_.pmgMyCh_[i] = pmgGO_->pmgCh_[st_.mysideNo_][i];
//             st_.pstMyCh_[i] = &pmgSG_->stCh_[st_.mysideNo_][i];
//         }
//
//
//         //ここで初期化
//         //st_.pstMyTm_->Init();initはしてある
//         //st_.pstMyTm_->PosSt.mid_team_ = st_.mid_team_;
//         //st_.pstMyTm_->PosSt.id_team_ = st_.id_team_;
//         MyTeamState.MainState.ControlOrderIndex = FIRSTCTRL;//最初は一番前衛の人がカーソル
//         st_.pstMyTm_->DrwHS_c = Defines.NGNUM;
//         st_.pstMyTm_->TmUeda_c = 0;
//         st_.pstMyTm_->comNo_ = st_.comNo_;//チームに設定されたＣＯＭ番号
//         st_.pstMyTm_->fomNo_ = st_.fomNo_;
//         SetDefEnCXZ();//デフォ敵位置
//
//         if (tDemo_f == FALSE)
//         {
//             SetShiaiName();//文字列なのでシーンの移動で消えてしまう
//         }
//
//         //パッド情報のセット
//         SetSidePad();
//     }
//
//     //効果音
//     void TMgTeam::SESet(enSEID SENo)
//     {
//         SESetInt((int)SENo);
//     }
//     //効果音
//     void TMgTeam::SysSESet(enSYSID SysNo)
//     {
//         if (st_.pstRf_->NoSE_f == FALSE)
//         {
//             pmgEO_->mgSound_.PlaySYS(SysNo);//, FALSE);
//         }
//     }
//     //効果音
//     void TMgTeam::SESetInt(int SENo)
//     {
//         if (st_.pstRf_->NoSE_f == FALSE)
//         {
//             pmgEO_->mgSound_.PlaySE(SENo);//, FALSE);
//         }
//     }
//     //サイド位置XDF
//     int TMgTeam::SideDist(int side)
//     {
//         return (side == 0)
//             ? 0
//             : 19;
//     }
//     //ラインYDF
//     int TMgTeam::LineDist()
//     {
//         return 2;
//     }
//
//
//     //生存内野人数
//     int TMgTeam::GetLiveNum()
//     {
//         int livenum = 0;
//         for (int i = 0; i < DBMEMBER_ALL; ++i)
//         {
//             if ((st_.pmgMyCh_[i]->IsInfield())
//                 && ((st_.pmgMyCh_[i]->st_.pstMyCh_->Dead_f == FALSE)))
//             {
//                 livenum++;
//             }
//         }
//         return livenum;
//     }
//     BOOL TMgTeam::IsLastOne()//一人だけ残ってる
//     {
//         int livenum = 0;
//         for (int i = 0; i < DBMEMBER_ALL; ++i)
//         {
//             //if ((st_.pmgMyCh_[i]->IsInfield()) && (st_.pmgMyCh_[i]->st_.pstMyCh_->Dead_f == FALSE))
//             if ((st_.pmgMyCh_[i]->IsInfield()) && (st_.pmgMyCh_[i]->st_.pstMyCh_->Live_f))
//             {
//                 livenum++;
//             }
//         }
//         return (livenum == 1); //一人だけ残ってる
//     }
//     BOOL TMgTeam::IsAllDead()//全員HPゼロ、既に天使か
//     {
//         int deadnum = 0;
//         for (int i = 0; i < DBMEMBER_ALL; ++i)
//         {
//             //HPゼロ、既に天使か
//             if ((st_.pmgMyCh_[i]->st_.pstMyCh_->HP == 0) || st_.pmgMyCh_[i]->st_.pstMyCh_->ANGEL_f)
//             {
//                 ++deadnum;
//             }
//         }
//         return (deadnum == DBMEMBER_ALL);
//
//         //return (st_.pstMyTm_->Dead_c >= DBMEMBER_ALL);
//     }
//     BOOL TMgTeam::IsAllNoLive()//全員天使
//     {
//         int livenum = 0;
//         for (int i = 0; i < DBMEMBER_ALL; ++i)
//         {
//             if ((st_.pmgMyCh_[i]->IsInfield())
//                 && ((st_.pmgMyCh_[i]->st_.pstMyCh_->Live_f) || (st_.pmgMyCh_[i]->st_.pstMyCh_->Ueda_c > 0)))
//             {
//                 ++livenum;
//             }
//         }
//         return (livenum == 0);
//     }

//     BOOL TMgTeam::IsAllStop()//全員停止
//     {
//         int movenum = 0;
//         for (int i = 0; i < DBMEMBER_ALL; ++i)
//         {
//             if (((st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Y != 0) || st_.pmgMyCh_[i]->st_.pstMyCh_->Motion.IsMFlags(dbmfDam))
//                 && (st_.pmgMyCh_[i]->st_.pstMyCh_->Dead_f == FALSE))
//             {
//                 movenum++;
//             }
//         }
//         return (movenum == 0);
//     }
//
//     BOOL TMgTeam::IsNoFly()//全員停止
//     {
//         for (int i = 0; i < DBMEMBER_ALL; ++i)
//         {
//             if ((st_.pmgMyCh_[i]->st_.pstMyCh_->Motion.Mtype == dbmtFlF)
//                 || (st_.pmgMyCh_[i]->st_.pstMyCh_->Motion.Mtype == dbmtFlB))
//             {
//                 return FALSE;
//             }
//         }
//         return TRUE;
//     }
//
//     //死亡人数
//     int TMgTeam::DeadNum()
//     {
//         int dnum = 0;
//         for (int i = 0; i < DBMEMBER_ALL; ++i)
//         {
//             if ((st_.pmgMyCh_[i]->st_.pstMyCh_->HP == 0) || st_.pmgMyCh_[i]->st_.pstMyCh_->ANGEL_f)
//             {
//                 ++dnum;
//             }
//         }
//
//         return dnum;
//     }
//
//     //内野人数
//     int TMgTeam::InfNum()
//     {
//         int inf = 0;
//         for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//         {
//             if (st_.pmgMyCh_[i]->st_.pstMyCh_->HP > 0)
//             {
//                 ++inf;
//             }
//         }
//
//         return inf;
//     }
//
//     //瀕死人数
//     int TMgTeam::PinchNum()
//     {
//         int pnum = 0;
//         for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//         {
//             if ((st_.pmgMyCh_[i]->st_.pstMyCh_->HP < 16) && (st_.pmgMyCh_[i]->st_.pstMyCh_->MHP_f == FALSE))
//             {
//                 pnum++;
//             }
//         }
//         return pnum;
//     }
//
//     //帰宅済みかどうか
//     BOOL TMgTeam::IsGohome()
//     {
//         for (int i = 0; i < DBMEMBER_ALL; ++i)
//         {
//             const int FULLBLIND = 6400;
//             if ((st_.pmgMyCh_[i]->st_.pstMyCh_->Live_f)
//                 && ((st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.X < DBCRT_W + FULLBLIND)
//                 && (st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.X > 0 - FULLBLIND)))
//             {
//                 return FALSE;
//             }
//         }
//
//         return TRUE;
//     }
//     //ＣＯＭ操作
//     BOOL TMgTeam::IsCOM()
//     {
//         return (st_.pstMyTm_->semi_f_ //セミオート時のみオート扱い
//             || (st_.pstMyTm_->MANSide_f == FALSE)//COMサイド
//             );//サイド操作無しか
//
//         //return (st_.pstMyTm_->SideMaruhi_f //丸秘中か
//         //        || st_.pstMyTm_->OmksSide_f //おまかせ中か
//         //        || (st_.pstMyTm_->MANSide_f == FALSE));//サイド操作無しか
//     }
//
//     //マニュアルサイド操作
//     BOOL TMgTeam::IsMAN()
//     {
//         return ((IsCOM() == FALSE));//セミオート時以外
//         //return (st_.pstMyTm_->MANSide_f//サイド操作
//         //        && (IsCOM() == FALSE));//ＣＯＭアシスト操作以外
//     }
//
//     //操作キャラを求める
//     void TMgTeam::GetCtrl()
//     {
//         if (MyTeamState.MainState.ControlOrderIndex == Defines.NGNUM)
//         {
//             SeekCtrl(TRUE);
//         }
//         else if ((st_.pmgMyCh_[MyTeamState.MainState.ControlOrderIndex]->IsCtrlOK() == FALSE)
//             && (st_.pmgMyCh_[MyTeamState.MainState.ControlOrderIndex]->IsMAN() == FALSE))
//         {
//             //今はとりあえず
//             SeekCtrl(TRUE);
//         }
//
//         //操作不能
//         if (MyTeamState.MainState.ControlOrderIndex == Defines.NGNUM) return;
//
//         switch (pmgSG_->stBa_.Motion)
//         {
//         case bmFree:
//             //止まった時点で近いキャラがタゲになる
//             break;
//         case bmHold:
//             //持ったキャラが操作キャラになる
//             //もたれたキャラの最初のタゲが操作キャラになる
//             break;
//         case bmShoot:
//             //敵からのシュート（まずシュートされた時点でタゲが操作キャラになる）
//             break;
//         case bmPass:
//             //まだ投げた人に操作権があるとき
//             if ((MyTeamState.MainState.ControlOrderIndex == pmgSG_->stBa_.PichPNo)
//                 && pmgSG_->stBa_.MvCtrl_f//操作権委譲可能なパス
//                 && (pmgSG_->stBa_.PaTgTNo == st_.mysideNo_)
//                 && (pmgSG_->stBa_.PaTgPNo <= (int)dbpoI3))
//             {
//                 //int tPACAJP = PACAJPTIME pmgEO_->mgDt_.dtSet_.GetDtGrv(setGRV_PACAJPTIME);//pmgEO_->mgDt_.dtOption_.GetDt(rkopGRV_PACAJPTIME, tGrvLv);
//
//                 if (((pmgSG_->stBa_.PaETA_c > PACAJPTIME) || (pmgSG_->stBa_.PaJPa_f == FALSE))
//                     && (st_.pmgMyCh_[pmgSG_->stBa_.PaTgPNo]->IsFreeMotion(TRUE))
//                     && IsMAN())//★サイド操作の場合
//                 {
//                     if (st_.pSidePad_->IsPaTgCtrl())//操作権委譲入力
//                     {
//                         SetCtrl(pmgSG_->stBa_.PaTgPNo);
//                     }
//                 }
//             }
//             break;
//         case bmBound:
//             //バウンドした瞬間に近いキャラがタゲになる。
//             break;
//
//         case bmReferee:
//
//             break;
//         }
//     }
//
//     //最前に居る内野
//     int TMgTeam::GetFrontInfPNo()
//     {
//         int res = Defines.NGNUM;
//
//         if (MyTeamState.MainState.ControlOrderIndex < Defines.DBMEMBER_INF)//内野持ち
//         {
//             int posx = 0;
//             for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//             {
//                 if (st_.pmgMyCh_[i]->chCommon_.IsFreeMotion(TRUE) || (MyTeamState.MainState.ControlOrderIndex == i))
//                 {
//                     if (posx < st_.pmgMyCh_[i]->GetLeftCrtX())//前方にいる
//                     {
//                         posx = st_.pmgMyCh_[i]->GetLeftCrtX();
//                         res = i;
//                     }
//                 }
//             }
//         }
//         else//外野持ち
//         {
//             res = st_.pstMyTm_->PosMove.Postman;//ポストマン
//         }
//
//         return res;
//     }
//
//     //最近に居る外野
//     int TMgTeam::GetNearGaiyaPNo()
//     {
//         int res = Defines.NGNUM;
//
//         if ((MyTeamState.MainState.ControlOrderIndex < Defines.DBMEMBER_INF)//内野持ち
//             || (MyTeamState.MainState.ControlOrderIndex == dbpoO4))//４番外野
//         {
//             int posx = 0;
//             //const int GAIYANUM = (DBMEMBER_ALL - Defines.DBMEMBER_INF);
//             BOOL o2ok_f = (st_.pmgMyCh_[dbpoO2]->chCommon_.IsFreeMotion(TRUE));
//             BOOL o3ok_f = (st_.pmgMyCh_[dbpoO3]->chCommon_.IsFreeMotion(TRUE));
//
//             if (o2ok_f && o3ok_f)
//             {
//                 res = (st_.pmgMyCh_[MyTeamState.MainState.ControlOrderIndex]->st_.pstMyCh_->Zahyou.Z > DBCRT_CLZ)
//                     ? dbpoO2
//                     : dbpoO3;
//             }
//             else if (o2ok_f)
//             {
//                 res = dbpoO2;
//             }
//             else if (o3ok_f)
//             {
//                 res = dbpoO3;
//             }
//         }
//         else//23外野持ち
//         {
//             res = MyTeamState.MainState.ControlOrderIndex;//自分
//         }
//
//         return res;
//     }
//
//     //最下位オーダーの内野
//     int TMgTeam::GetBottomInfPNo()
//     {
//         int res = Defines.NGNUM;
//
//         //上位から下位に
//         for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//         {
//             if (st_.pmgMyCh_[i]->chCommon_.IsFreeMotion(TRUE) || (MyTeamState.MainState.ControlOrderIndex == i))
//             {
//                 res = i;//最後にＯＫ出たキャラ
//             }
//         }
//
//         return res;
//     }
//
//     //パスタイプセット
//     void TMgTeam::SetPassType(int patype)
//     {
//         switch (patype)
//         {
//         case 0://ジャンプトスパス
//             MyTeamAiState.Main.toss_f = TRUE;
//             MyTeamAiState.Main.jp_f = TRUE;
//             break;
//         case 1://トスパス
//             MyTeamAiState.Main.toss_f = TRUE;
//             MyTeamAiState.Main.jp_f = FALSE;
//             break;
//         case 2://クイックパス
//             MyTeamAiState.Main.toss_f = FALSE;
//             MyTeamAiState.Main.jp_f = FALSE;
//             break;
//         default://ジャンプクイックパス
//             MyTeamAiState.Main.toss_f = FALSE;
//             MyTeamAiState.Main.jp_f = TRUE;
//             break;
//         }
//     }
//
//     //クイックシュートタイプセット
//     void TMgTeam::SetQShootType(int shtype)
//     {
//         switch (shtype)
//         {
//         case 0://ＤＪＳ
//             MyTeamAiState.Main.jp_f = TRUE;
//             MyTeamAiState.Main.ds_f = TRUE;
//             break;
//         case 1://ＤＳ
//             MyTeamAiState.Main.jp_f = FALSE;
//             MyTeamAiState.Main.ds_f = TRUE;
//             break;
//         case 2://立ちＳ
//             MyTeamAiState.Main.jp_f = FALSE;
//             MyTeamAiState.Main.ds_f = FALSE;
//             break;
//         default://ＪＳ
//             MyTeamAiState.Main.jp_f = TRUE;
//             MyTeamAiState.Main.ds_f = FALSE;
//             break;
//         }
//         MyTeamAiState.Main.onestep_f = FALSE;
//
//     }
//
//     //パワーシュートタイプセット
//     void TMgTeam::SetPShootType(int shtype)
//     {
//         switch (shtype)
//         {
//         case 0://ＤＪＳ
//             MyTeamAiState.Main.jp_f = TRUE;
//             MyTeamAiState.Main.ds_f = TRUE;
//             MyTeamAiState.Main.onestep_f = FALSE;
//             break;
//         case 1://ＤＳ
//             MyTeamAiState.Main.jp_f = FALSE;
//             MyTeamAiState.Main.ds_f = TRUE;
//             MyTeamAiState.Main.onestep_f = FALSE;
//             break;
//         case 2://一歩ＤＳ
//             MyTeamAiState.Main.jp_f = FALSE;
//             MyTeamAiState.Main.ds_f = TRUE;
//             MyTeamAiState.Main.onestep_f = TRUE;
//             break;
//         default://一歩ＤＪＳ
//             MyTeamAiState.Main.jp_f = TRUE;
//             MyTeamAiState.Main.ds_f = TRUE;
//             MyTeamAiState.Main.onestep_f = TRUE;
//             break;
//         }
//     }
//
//     //自チームのフォーメーション番号★
//     int TMgTeam::GetMyFomationNo()
//     {
// #ifdef __K_DEBUG_SHIAI__
//         // ここでフォーメーションの形を変えれる
//         // ※いずれでばっぐで好きな値にできるようにする※
//         //	return 33;	// ドイツを指定(ディフェンス陣が前にいるから)
//         //	return 25;	// おおやまさん用のデバッグ番号？
//         return st_.pstMyTm_->comNo_;	// 今は元のチームの値を使う
// #else
//         if (pmgEO_->stShiai_.IsTuto())
//         {
//             return 0;//フォーメーションは熱血
//         }
//         else
//         {
//             return st_.pstMyTm_->comNo_;
//         }
//         //return pmgGO_->pmgCh_[st_.mysideNo_][st_.pstMyTm_->RdNo]->MyPDt(clv_Fomation);//とりあえず
//
// #endif	// #ifdef __K_DEBUG_SHIAI__
//     }
//
//     int TMgTeam::GetBallmanNo()//ボールを持っているキャラ
//     {
//         int res = Defines.NGNUM;
//
//         if (pmgSG_->stBa_.HoldTNo == st_.mysideNo_)
//         {
//             res = pmgSG_->stBa_.HoldPNo;
//         }
//
//         return res;
//     }
//
//     //自チームの攻撃位置目標Ｘ★
//     int TMgTeam::GenGetAtcLineX(BOOL jp_f, BOOL dsmn_f)
//     {
//         if (IsNoJpDashman() && dsmn_f)
//         {
//             //強ジャンプラインと同じに
//             return (pmgEO_->mgDt_.dtSet_.GetDtFormation(setAtcLineX3) * XYMAG);
//         }
//         else
//         {
//             endtSet_Formation atcline = (endtSet_Formation)(setAtcLineX + pmgGO_->pmgCh_[st_.mysideNo_][st_.pstMyTm_->RdNo]->MyPDt(clv_AtcLine));
//             int tgx = pmgEO_->mgDt_.dtSet_.GetDtFormation(atcline);
//
//             //ジャンプ踏切位置は少し後ろに
//             if (jp_f)
//             {
//                 const int JPLINE = 24;
//                 tgx += JPLINE;
//             }
//
//             return (tgx * XYMAG);
//         }
//     }
//
//     //自チームの攻撃位置目標Ｘ★
//     int TMgTeam::GetAtcLineX(BOOL jp_f, BOOL dsmn_f)
//     {
//         return (DBCRT_CL - GenGetAtcLineX(jp_f, dsmn_f));
//     }
//
//     //自チームのダッシュマンジャンプ不可
//     BOOL TMgTeam::IsNoJpDashman()
//     {
//         return (pmgGO_->pmgCh_[st_.mysideNo_][st_.pstMyTm_->RdNo]->MyPDt(clv_DsmnNoJp) != 0);
//     }
//
//
//
//     BOOL TMgTeam::IsBallCrtMyPos(int pos, BOOL wide_f)
//     {
//         if (wide_f)// && MyTeamAiState.Main.WideBallPos_f)
//         {
//             return (((st_.pstMyTm_->BallCrtSideZ == st_.mysideNo_) && (st_.pstMyTm_->BallCrtPosZ == pos))
//                 || ((st_.pstMyTm_->BallCrtSideL == st_.mysideNo_) && (st_.pstMyTm_->BallCrtPosL == pos)));
//         }
//         else
//         {
//             return ((st_.pstMyTm_->BallCrtSideL == st_.mysideNo_) && (st_.pstMyTm_->BallCrtPosL == pos));
//         }
//     }
//
//
//     //com★
//     int TMgTeam::GetCom(enCom com)
//     {
//         //int comno = pmgGO_->pmgCh_[st_.mysideNo_][st_.pstMyTm_->RdNo]->MyPDt(clv_COM);
//         if (pmgEO_->stShiai_.IsTuto())
//         {
//             //チュートリアルは坂宿思考
//             return pmgGO_->pmgCommon_->GetComPtn(2, com, st_.mysideNo_);//とりあえず
//         }
//         else
//         {
//             return pmgGO_->pmgCommon_->GetComPtn(st_.pstMyTm_->comNo_, com, st_.mysideNo_);//とりあえず
//         }
//     }
//
//
//     void TMgTeam::ReaderChange()//直す★
//     {
//         int tmprdno = st_.pstMyTm_->RdNo;
//         st_.pstMyTm_->RdNo = CAPNO;
//         //if (GetBallmanNo() == Defines.NGNUM)
//         //{
//         //  st_.pstMyTm_->RdNo = CAPNO;
//         //}
//         //else
//         //{
//         //  switch (pmgEO_->mgDt_.dtTeam_.GetDt(dbtm_OpeFomChange, st_.teamNo_))
//         //  {
//         //    case 0: //キャプテン固定
//         //      st_.pstMyTm_->RdNo = CAPNO;
//         //      break;
//         //    case 1: //生存中の最上位
//         //    {
//         //      int tmprdno = st_.pstMyTm_->RdNo;
//         //      int cno = DBMEMBER_ALL - 1;
//         //      for (int i = 0; i < DBMEMBER_ALL; ++i)
//         //      {
//         //        if ((st_.pmgMyCh_[i]->st_.pstMyCh_->HP > 0)
//         //            && (st_.pmgMyCh_[i]->st_.pstMyCh_->ANGEL_f == FALSE))
//         //        {
//         //          if (st_.pstMyTm_->PosSt.CharofP[i] < cno)
//         //          {
//         //            cno = st_.pstMyTm_->PosSt.CharofP[i];
//         //          }
//         //        }
//         //      }
//         //      st_.pstMyTm_->RdNo = cno;
//         //    }
//         //      break;
//         //    case 2://きゃっちまん
//         //    case 3://ぼーるまん
//         //      st_.pstMyTm_->RdNo = st_.pstMyTm_->PosSt.CharofP[GetBallmanNo()];
//         //      break;
//         //  }
//         //}
//         //
//         //if (tmprdno != st_.pstMyTm_->RdNo)
//         //{
//         //  PopupFomChange();
//         //}
//     }
//
//     int TMgTeam::GetComAct(int comactNo, enDtComAct cma, BOOL ressei_f, int id)
//     {
//         return pmgGO_->pmgCommon_->GetComActPtn(comactNo, cma, ressei_f, st_.mysideNo_, id);//とりあえず
//     }
//
//     //int TMgTeam::GetComActNo(int posno)
//     //{
//     //  BOOL useselfact_f = FALSE;
//     //  switch (pmgEO_->mgDt_.dtTeam_.GetDt2(dbtm_OpeSelfPlay, st_.teamNo_))
//     //  {
//     //    case 4: useselfact_f = pmgGO_->pmgCommon_->IsPer100(100); break;//個人ぷれー→
//     //    case 3: useselfact_f = pmgGO_->pmgCommon_->IsPer100(75); break;
//     //    case 2: useselfact_f = pmgGO_->pmgCommon_->IsPer100(50); break;
//     //    case 1: useselfact_f = pmgGO_->pmgCommon_->IsPer100(25); break;//←リーダーにあわせる
//     //  }
//     //  
//     //  int useact = (useselfact_f)
//     //  ? st_.pstMyTm_->PosSt.CharofP[posno]
//     //  : st_.pstMyTm_->RdNo;
//     //  
//     //  int cmano = pmgGO_->pmgCh_[st_.mysideNo_][useact]->MyPDt(clv_COM);
//     //  
//     //  return cmano;
//     //}
//
//
//
//     //操作権予約
//     void TMgTeam::SetCtrlReserve(int tCRNo)
//     {
//         st_.pstMyTm_->CtrlRsvNo = tCRNo;
//     }
//
//     //操作権強制
//     void TMgTeam::SetCtrlBallGet(int tCNo)
//     {
//         MyTeamState.MainState.ControlOrderIndex = tCNo;
//     }
//     //操作権（playerobjから直接呼び出すのはどうだろうか）
//     void TMgTeam::SetCtrl(int tCNo)
//     {
//
//         //指定があったキャラが操作不能状態の時は、
//         //近いキャラが代わりに操作可能になる
//         //近いキャラが居ない(内野一人の時とか、ダウン中の外野とか)場合は操作不能状態
//         if (tCNo == Defines.NGNUM) return;
//
//         if (st_.pmgMyCh_[tCNo]->IsCtrlOK())
//         {
//             MyTeamState.MainState.ControlOrderIndex = tCNo;
//         }
//         else
//         {
//             SeekCtrl(TRUE);
//         }
//     }
//     //現状から操作権最適キャラを決める
//     int TMgTeam::SeekCtrl(BOOL set_f)
//     {
//         return SeekCtrl(set_f, FALSE);
//     }
//     int TMgTeam::SeekCtrl(BOOL set_f, BOOL landpos_f)
//     {
//         //各キャラのボールとの距離を測る
//         for (int i = 0; i < DBMEMBER_ALL; ++i)
//         {
//             st_.pmgMyCh_[i]->chCommon_.GetBallDist();
//         }
//         //一番近いキャラ
//         int nearPNo = Defines.NGNUM;
//
//
//         if (landpos_f)//着地地点から判断
//         {
//             //内野を検索
//             for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//             {
//
//                 if (st_.pmgMyCh_[i]->st_.pstMyCh_->BallLandDist == Defines.NGNUM) continue;
//
//                 if (nearPNo == Defines.NGNUM) //先客なし
//                 {
//                     nearPNo = i;
//                 }
//                 else if (st_.pmgMyCh_[i]->st_.pstMyCh_->BallLandDist < st_.pmgMyCh_[nearPNo]->st_.pstMyCh_->BallLandDist)//先客と比較
//                 {
//                     nearPNo = i;
//                 }
//             }
//         }
//         else
//         {
//             //内野を検索
//             for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//             {
//                 if (st_.pmgMyCh_[i]->st_.pstMyCh_->BallDist == Defines.NGNUM) continue;
//
//                 if (nearPNo == Defines.NGNUM) //先客なし
//                 {
//                     nearPNo = i;
//                 }
//                 else if (st_.pmgMyCh_[i]->st_.pstMyCh_->BallDist < st_.pmgMyCh_[nearPNo]->st_.pstMyCh_->BallDist)//先客と比較
//                 {
//                     nearPNo = i;
//                 }
//             }
//         }
//
//         if (set_f)
//         {
//             //変わる前のキャラは変わった直後にポジショニングしない★
//             //st_.pstMyTm_->PosMove.Inf_f[MyTeamState.MainState.ControlOrderIndex] = FALSE;
//             MyTeamState.MainState.ControlOrderIndex = nearPNo;
//         }
//
//         return nearPNo;
//
//     }
//     //次期カバーマンを設定する
//     void TMgTeam::SeekCover(int tHoldPNo, int tPichPNo, int tPaTgPNo, BOOL Cvr2_f)
//     {
//
//         st_.pstMyTm_->CvrNo = Defines.NGNUM;
//         st_.pstMyTm_->Cvr2No = Defines.NGNUM;
//
//         //外野からのシュートのみ
//         if ((tPichPNo > (int)dbpoI3) && (tPaTgPNo <= (int)dbpoI3))
//         {
//             //各キャラのボールとの距離を測る
//             for (int i = 0; i < DBMEMBER_ALL; ++i)
//             {
//                 st_.pmgMyCh_[i]->chCommon_.GetBallDist();
//             }
//
//             //一番近いキャラ
//             int nearPNo = Defines.NGNUM;
//
//             //内野を検索
//             for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//             {
//                 if ((st_.pmgMyCh_[i]->st_.pstMyCh_->BallDist == Defines.NGNUM)
//                     || (i == tPaTgPNo)
//                     || (i == tHoldPNo)
//                     || (st_.pmgMyCh_[i]->IsFreeMotion(TRUE) == FALSE)
//                     || st_.pmgMyCh_[i]->IsMAN())//★専用操作
//                 {
//                     continue;
//                 }
//                 //第二カバーマン選出の時は現カバーマンも外れる
//                 else if (Cvr2_f && ((i == st_.pstMyTm_->CvrNo) || (i == MyTeamState.MainState.ControlOrderIndex)))
//                 {
//                     continue;
//                 }
//                 else
//                 {
//                     int idist = st_.pmgMyCh_[i]->st_.pstMyCh_->BallLandDist;
//                     if (nearPNo != Defines.NGNUM)
//                     {
//                         int neard = st_.pmgMyCh_[nearPNo]->st_.pstMyCh_->BallLandDist;
//                         if (idist < neard)
//                         {
//                             nearPNo = i;
//                         }
//                     }
//                     else
//                     {
//                         nearPNo = i;
//                     }
//
//                 }
//
//             }
//
//             if (Cvr2_f)
//             {
//                 st_.pstMyTm_->Cvr2No = nearPNo;
//             }
//             else
//             {
//                 st_.pstMyTm_->CvrNo = nearPNo;
//                 st_.pstMyTm_->Cvr2No = Defines.NGNUM;
//             }
//         }
//
//     }
//
//     //ボール拾いマン最適者
//     void TMgTeam::SeekGetter()
//     {
//         //各キャラのボールとの距離を測る
//         for (int i = 0; i < DBMEMBER_ALL; ++i)
//         {
//             st_.pmgMyCh_[i]->chCommon_.GetBallDist();
//         }
//
//         //一番近いキャラ
//         int nearPNo = Defines.NGNUM;
//
//         //内野を検索
//         for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//         {
//             if ((st_.pmgMyCh_[i]->st_.pstMyCh_->BallDist == Defines.NGNUM)
//                 || (st_.pmgMyCh_[i]->IsFreeMotion(TRUE) == FALSE))//★専用操作
//             {
//                 continue;
//             }
//
//
//             if (IsCOM())
//             {
//                 //COMの場合は操作権キャラ
//                 if (st_.pmgMyCh_[i]->IsCtrl() == FALSE) continue;
//
//                 nearPNo = i;
//             }
//             else
//             {
//                 //COM以外は操作権以外の一番近いキャラ
//                 if (st_.pmgMyCh_[i]->IsCtrl()) continue;
//
//
//                 int idist = st_.pmgMyCh_[i]->st_.pstMyCh_->BallLandDist;
//                 if (nearPNo != Defines.NGNUM)
//                 {
//                     int neard = st_.pmgMyCh_[nearPNo]->st_.pstMyCh_->BallLandDist;
//                     if (idist < neard)
//                     {
//                         nearPNo = i;
//                     }
//                 }
//                 else
//                 {
//                     nearPNo = i;
//                 }
//             }
//         }
//
//         st_.pstMyTm_->GetNo = nearPNo;
//     }
//
//
//     ////ポジションリセット予約する
//     //void TMgTeam::SetPosReset()
//     //{
//     //  st_.pstMyTm_->PosReset_f = TRUE;
//     //}
//
//     void TMgTeam::PopupFomChange()
//     {
//         std::string fom = pmgEO_->mgDt_.dtFomation_.GetName(GetMyFomationNo()) + "＿ふぉめ";
//         pmgGO_->pmgCommon_->PopupMsg(pu_DsMn, fom.c_str(), st_.mysideNo_, 0, 0, 0, FC2_shiai, FALSE);
//     }
//     //ポジショニング
//     void TMgTeam::SetPosLine()
//     {
//         //敵にボールがもたれているとき（カウンタでインターバル）
//         //もしくは強制ポジションリセットが発生した場合
//
//         //どの位置の敵が持っているか
//         switch (pmgSG_->stBa_.Motion)
//         {
//         case bmHold:
//             if (pmgSG_->stBa_.HoldTNo == st_.mysideNo_)
//             {
//                 st_.pstMyTm_->PosMove.InfLine = (pmgSG_->stBa_.HoldPNo < Defines.DBMEMBER_INF)
//                     ? iltInf
//                     : iltOfO;
//             }
//             else
//             {
//                 switch (pmgSG_->stBa_.HoldPNo)
//                 {
//                 case (int)dbpoO2: st_.pstMyTm_->PosMove.InfLine = iltO2; break;
//                 case (int)dbpoO3: st_.pstMyTm_->PosMove.InfLine = iltO3; break;
//                 case (int)dbpoO4: st_.pstMyTm_->PosMove.InfLine = iltO4; break;
//                 default: st_.pstMyTm_->PosMove.InfLine = iltInf; break;
//                 }
//             }
//             break;
//         case bmShoot:
//             //敵外野シュートヒット時のポジショニングが敵内野のもの
//             if (pmgSG_->stBa_.PichTNo == st_.ensideNo_)
//             {
//                 switch (pmgSG_->stBa_.PichPNo)
//                 {
//                 case (int)dbpoO2: st_.pstMyTm_->PosMove.InfLine = iltO2; break;
//                 case (int)dbpoO3: st_.pstMyTm_->PosMove.InfLine = iltO3; break;
//                 case (int)dbpoO4: st_.pstMyTm_->PosMove.InfLine = iltO4; break;
//                 default: st_.pstMyTm_->PosMove.InfLine = iltInf; break;
//                 }
//             }
//             else
//             {
//                 st_.pstMyTm_->PosMove.InfLine = iltInf;
//             }
//             break;
//         case bmPass:
//             //味方ボール（のときは本当は動いていないはずだが）
//             if (pmgSG_->stBa_.PaTgTNo == st_.mysideNo_)
//             {
//                 st_.pstMyTm_->PosMove.InfLine = (pmgSG_->stBa_.PaTgPNo < Defines.DBMEMBER_INF)
//                     ? iltInf
//                     : iltOfO;
//             }
//             else
//             {
//                 //敵ボールの時はパス先をみて
//                 switch (pmgSG_->stBa_.PaTgPNo)
//                 {
//                 case (int)dbpoO2: st_.pstMyTm_->PosMove.InfLine = iltO2; break;
//                 case (int)dbpoO3: st_.pstMyTm_->PosMove.InfLine = iltO3; break;
//                 case (int)dbpoO4: st_.pstMyTm_->PosMove.InfLine = iltO4; break;
//                 default: st_.pstMyTm_->PosMove.InfLine = iltInf; break;
//                 }
//             }
//             break;
//         case bmReferee:
//             st_.pstMyTm_->PosMove.InfLine = iltInf;
//             break;
//         default:
//             //ジャンプボール
//             if (pmgSG_->stBa_.JumpBall == jbJpball)
//             {
//                 st_.pstMyTm_->PosMove.InfLine = iltInf;
//             }
//             else
//             {
//                 //敵陣地
//                 if (st_.pstMyTm_->BallCrtSideL != st_.mysideNo_)
//                 {
//                     switch (st_.pstMyTm_->BallCrtPosL)
//                     {
//                     case (int)dbpoO2: st_.pstMyTm_->PosMove.InfLine = iltO2; break;
//                     case (int)dbpoO3: st_.pstMyTm_->PosMove.InfLine = iltO3; break;
//                     case (int)dbpoO4: st_.pstMyTm_->PosMove.InfLine = iltO4; break;
//                     default: st_.pstMyTm_->PosMove.InfLine = iltInf; break;
//                     }
//                 }
//                 else//味方陣地
//                 {
//                     //敵外野シュートヒット時のポジショニングが敵内野のもの
//                     if (pmgSG_->stBa_.PichTNo == st_.ensideNo_)
//                     {
//                         switch (pmgSG_->stBa_.PichPNo)
//                         {
//                         case (int)dbpoO2: st_.pstMyTm_->PosMove.InfLine = iltO2; break;
//                         case (int)dbpoO3: st_.pstMyTm_->PosMove.InfLine = iltO3; break;
//                         case (int)dbpoO4: st_.pstMyTm_->PosMove.InfLine = iltO4; break;
//                         default: st_.pstMyTm_->PosMove.InfLine = iltInf; break;
//                         }
//                     }
//                     else
//                     {
//                         st_.pstMyTm_->PosMove.InfLine = iltInf;
//                     }
//                 }
//             }
//             break;
//         }
//
//
//
//         //それに応じた配置(FTypeはPlObj.moveでも使ってる)
//         switch (st_.pstMyTm_->PosMove.InfLine)
//         {
//         case iltO4: st_.pstMyTm_->PosMove.FType = ftDf4; break;
//         case iltO2: st_.pstMyTm_->PosMove.FType = ftDf2; break;
//         case iltO3: st_.pstMyTm_->PosMove.FType = ftDf3; break;
//         case iltOfO:
//
//             //新しくポストマン決める
//             if ((st_.pstMyTm_->PosMove.Postman == Defines.NGNUM)//
//                 || (st_.pstMyTm_->PosMove.FType != ftOfO))//今まで外野攻撃ではなかった
//             {
//                 //初期化
//                 st_.pstMyTm_->PosMove.Postman = Defines.NGNUM;
//                 int secpostNo = Defines.NGNUM;
//                 for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//                 {
//                     int i2 = (Defines.DBMEMBER_INF - 1 - i);//４番から探る
//
//                     //if (i2 == MyTeamState.MainState.ControlOrderIndex) continue;//操作権
//                     if (st_.pmgMyCh_[i2]->IsFreeMotion(TRUE) == FALSE) continue;//自由
//                     if (st_.pmgMyCh_[i2]->st_.pstMyCh_->LandEnCourt_f) continue;//敵コートに落下しない
//
//                     //第二候補
//                     if (secpostNo == Defines.NGNUM)
//                     {
//                         secpostNo = i2;
//                     }
//
//                     if (st_.pmgMyCh_[i2]->IsMAN()) continue;//★専用操作
//
//                     st_.pstMyTm_->PosMove.Postman = i2;
//                     break;
//                 }
//
//                 if (st_.pstMyTm_->PosMove.Postman == Defines.NGNUM)
//                 {
//                     st_.pstMyTm_->PosMove.Postman = secpostNo;
//                 }
//
//             }
//
//             st_.pstMyTm_->PosMove.FType = ftOfO;
//
//             break;
//         default: st_.pstMyTm_->PosMove.FType = ftOfI; break;//内野持ち攻守
//         }
//
//
//         if (st_.pstMyTm_->PosMove.FType != ftOfO) //外野攻撃時以外はポストマンいらない
//         {
//             st_.pstMyTm_->PosMove.Postman = Defines.NGNUM;
//         }
//
//     }
//     //ダッシュマンストック
//     void TMgTeam::SetDashmanStock()
//     {
//         //ダッシュマンの数
//         st_.pstMyTm_->PosMove.DashmanNum = 0;
//         st_.pstMyTm_->PosMove.DashmanNum_Auto = 0;
//         st_.pstMyTm_->PosMove.DashmanNum_Run = 0;
//         st_.pstMyTm_->PosMove.DashmanNum_Run_NT = 0;
//
//         //ダッシュマン順位
//         for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//         {
//             //ダッシュマンの人数
//             if (st_.pmgMyCh_[i]->IsDashman()
//                 || (st_.pmgMyCh_[i]->st_.pstMyCh_->DashmanNo != Defines.NGNUM)
//                 )
//             {
//                 ++st_.pstMyTm_->PosMove.DashmanNum;
//
//                 //オートのダッシュマンの人数
//                 if (st_.pmgMyCh_[i]->st_.pstMyCh_->Auto.AutoType == dbatDashman)
//                 {
//                     ++st_.pstMyTm_->PosMove.DashmanNum_Auto;
//                 }
//
//                 //その中でも走っている人
//                 if (st_.pmgMyCh_[i]->st_.pstMyCh_->Motion.Mtype == dbmtDs)
//                 {
//                     ++st_.pstMyTm_->PosMove.DashmanNum_Run;
//                     //その中でもまだ触ってない
//
//                     if (MyTeamAiState.Action[i].BallTouched_f == FALSE)
//                     {
//                         ++st_.pstMyTm_->PosMove.DashmanNum_Run_NT;
//                     }
//                 }
//             }
//
//             st_.pstMyTm_->PosMove.DashmanStock[i] = Defines.NGNUM;//埋め
//         }
//
//         //０番から優先
//         int f = 0;
//
//         //if ((st_.mysideNo_ == 0) && (st_.pstMyTm_->PosMove.DashmanNum == 0))
//         //{
//         for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//         {
//             if (i == st_.pstMyTm_->PosMove.Postman)
//             {
//                 st_.pstMyTm_->PosMove.dmng[i] = 1;
//                 continue;//ポストマンは入らない
//             }
//
//             if (i == MyTeamState.MainState.ControlOrderIndex)
//             {
//                 st_.pstMyTm_->PosMove.dmng[i] = 2;
//                 continue; //操作権キャラは入らない
//             }
//
//             if (st_.pmgMyCh_[i]->IsMAN())
//             {
//                 st_.pstMyTm_->PosMove.dmng[i] = 3;
//                 continue;//マニュアル操作キャラは入らない
//             }
//
//             if (st_.pmgMyCh_[i]->st_.pstMyCh_->Motion.IsMFlags(dbmfDmOK) == FALSE)
//             {
//                 st_.pstMyTm_->PosMove.dmng[i] = 4;
//                 continue;//ダッシュマン可能状態
//             }
//
//             if (st_.pmgMyCh_[i]->IsDMOKPos() == FALSE)
//             {
//                 st_.pstMyTm_->PosMove.dmng[i] = 5;
//                 continue;//コートの中央より後ろ側のみ
//             }
//
//             if (st_.pmgMyCh_[i]->st_.pstMyCh_->Motion.IsMFlags(dbmfAr))
//             {
//                 st_.pstMyTm_->PosMove.dmng[i] = 6;
//                 continue;//地上のみ
//             }
//
//             if (st_.pmgMyCh_[i]->IsFreeMotion(TRUE) == FALSE)
//             {
//                 st_.pstMyTm_->PosMove.dmng[i] = 7;
//                 continue;//自由
//             }
//             //if (st_.pmgMyCh_[i]->st_.pstMyCh_->LandEnCourt_f) continue;//敵コートに落下しない　そもそも立のみ
//             //その人向けのパスが出ているとき
//             if ((pmgSG_->stBa_.Motion == bmPass)
//                 && (pmgSG_->stBa_.PaTgTNo == st_.mysideNo_)
//                 && (pmgSG_->stBa_.PaTgPNo == i))
//             {
//                 st_.pstMyTm_->PosMove.dmng[i] = 8;
//                 continue;
//             }
//
//
//             st_.pstMyTm_->PosMove.dmng[i] = 999;
//             st_.pstMyTm_->PosMove.DashmanStock[f++] = i;//埋め
//
//         }
//         //}
//         //else
//         //{
//         //  for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//         //  {
//         //    if (i == st_.pstMyTm_->PosMove.Postman) continue;//ポストマンは入らない
//         //
//         //    if (i == MyTeamState.MainState.ControlOrderIndex) continue; //操作権キャラは入らない
//
//         //    if (st_.pmgMyCh_[i]->IsMAN()) continue;//マニュアル操作キャラは入らない
//
//         //    if (st_.pmgMyCh_[i]->st_.pstMyCh_->Motion.IsMFlags(dbmfDmOK) == FALSE) continue;//ダッシュマン可能状態
//         //
//         //    if (st_.pmgMyCh_[i]->IsDMOKPos() == FALSE) continue;//コートの中央より後ろ側のみ
//
//         //    if (st_.pmgMyCh_[i]->st_.pstMyCh_->Motion.IsMFlags(dbmfAr)) continue;//地上のみ
//
//         //    if (st_.pmgMyCh_[i]->IsFreeMotion(TRUE) == FALSE) continue;//自由
//         //    //if (st_.pmgMyCh_[i]->st_.pstMyCh_->LandEnCourt_f) continue;//敵コートに落下しない　そもそも立のみ
//         //    //その人向けのパスが出ているとき
//         //
//         //    if ((pmgSG_->stBa_.Motion == bmPass)
//         //      && (pmgSG_->stBa_.PaTgTNo == st_.mysideNo_)
//         //      && (pmgSG_->stBa_.PaTgPNo == i))continue;
//
//         //    st_.pstMyTm_->PosMove.DashmanStock[f++] = i;//埋め
//         //
//         //  }
//         //}
//
//         //mid::midLog("ovat\n");
//
//         //呼べる人数
//         st_.pstMyTm_->PosMove.PassAbleNum = f;
//
//         //ダッシュマンがいなくなったら作戦リセット
//         if (st_.pstMyTm_->PosMove.DashmanNum_Auto == 0)
//         {
//
//             //初期化
//             st_.pstMyTm_->CallingDashman_f = FALSE;
//             if (st_.pstMyTm_->CallingDashman_f3)//スタート後に０になった
//             {
//                 st_.pstMyTm_->CallingDashman_f2 = FALSE;
//                 st_.pstMyTm_->CallingDashman_f3 = FALSE;
//             }
//         }
//         //ダッシュマンがいなくなったら作戦リセット
//         if (st_.pstMyTm_->PosMove.DashmanNum == 0)
//         {
//             st_.pstMyTm_->DashmanType = dmtNone;
//             st_.pstMyTm_->DashmanCall_c = 0;
//             st_.pstMyTm_->DashmanNoBM = Defines.NGNUM;
//
//             //ダッシュマン番号も空に
//             for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//             {
//                 if (st_.pmgMyCh_[i]->st_.pstMyCh_->DashmanNo != Defines.NGNUM)//埋め)
//                 {
//                     st_.pmgMyCh_[i]->st_.pstMyCh_->DashmanNo = Defines.NGNUM;//埋め
//                 }
//
//                 //ボールマンダッシュナシで渡ってしまった場合、ダッシュマン作戦が終わったことになってしまう
//             }
//
//         }
//
//     }
//
//
//     //デフォ敵中心位置
//     void TMgTeam::SetDefEnCXZ()
//     {
//         const int DEF23POS = DBCRT_BL + (24 * XYMAG);
//         const int DEFZPOS = 56 * XYMAG;
//         //st_.pstMyTm_->PosMove.EnC_f = TRUE;
//         st_.pstMyTm_->PosMove.EnCX = DEF23POS;
//         st_.pstMyTm_->PosMove.EnCX3 = DEF23POS;
//         st_.pstMyTm_->PosMove.EnCZ = DEFZPOS;
//
//         //はみ出チェック
//         CheckEnC();
//     }
//     //敵内野の重心
//     void TMgTeam::GetCOG()
//     {
//         int EL_c = 0;
//
//         int EnX[Defines.DBMEMBER_INF];
//         int EnZ[Defines.DBMEMBER_INF];
//         int EnX2[Defines.DBMEMBER_INF];
//         int EnZ2[Defines.DBMEMBER_INF];
//
//         //st_.pstMyTm_->PosMove.EnC_f = TRUE;
//
//         for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//         {
//             if (st_.pmgEnTm_->IsLastOne())
//             {
//                 if (st_.pmgEnTm_->st_.pmgMyCh_[i]->IsFreeMotion(FALSE))
//                 {
//                     EnX[EL_c] = st_.pmgEnTm_->st_.pmgMyCh_[i]->GetLeftCrtX();
//                     EnZ[EL_c] = st_.pmgEnTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z;
//                     EL_c++;
//                 }
//             }
//             else
//             {
//                 //生きててダメージ状態ではないうえに戻り中とかでもない
//                 if (st_.pmgEnTm_->st_.pmgMyCh_[i]->IsFreeMotion(TRUE))
//                 {
//                     EnX[EL_c] = st_.pmgEnTm_->st_.pmgMyCh_[i]->GetLeftCrtX();
//                     EnZ[EL_c] = st_.pmgEnTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z;
//                     EL_c++;
//                 }
//             }
//         }
//         switch (EL_c)
//         {
//         case 4:
//             //重心を取る
//             //適当
//             EnX2[0] = (EnX[0] + EnX[1]) / 2;
//             EnX2[1] = (EnX[1] + EnX[2]) / 2;
//             EnX2[2] = (EnX[2] + EnX[3]) / 2;
//             EnX2[3] = (EnX[3] + EnX[0]) / 2;
//             EnZ2[0] = (EnZ[0] + EnZ[1]) / 2;
//             EnZ2[1] = (EnZ[1] + EnZ[2]) / 2;
//             EnZ2[2] = (EnZ[2] + EnZ[3]) / 2;
//             EnZ2[3] = (EnZ[3] + EnZ[0]) / 2;
//             st_.pstMyTm_->PosMove.EnCX = (EnX2[0] + EnX2[1] + EnX2[2] + EnX2[3]) / 4;
//             st_.pstMyTm_->PosMove.EnCZ = (EnZ2[0] + EnZ2[1] + EnZ2[2] + EnZ2[3]) / 4;
//             break;
//         case 3:
//             EnX2[0] = (EnX[0] + EnX[1]) / 2;
//             EnX2[1] = (EnX[1] + EnX[2]) / 2;
//             EnX2[2] = (EnX[2] + EnX[0]) / 2;
//             EnZ2[0] = (EnZ[0] + EnZ[1]) / 2;
//             EnZ2[1] = (EnZ[1] + EnZ[2]) / 2;
//             EnZ2[2] = (EnZ[2] + EnZ[0]) / 2;
//
//             st_.pstMyTm_->PosMove.EnCX = (EnX2[0] + EnX2[1] + EnX2[2]) / 3;
//             st_.pstMyTm_->PosMove.EnCZ = (EnZ2[0] + EnZ2[1] + EnZ2[2]) / 3;
//             break;
//         case 2:
//             st_.pstMyTm_->PosMove.EnCX = (EnX[0] + EnX[1]) / 2;
//             st_.pstMyTm_->PosMove.EnCZ = (EnZ[0] + EnZ[1]) / 2;
//             break;
//         case 1:
//             st_.pstMyTm_->PosMove.EnCX = EnX[0];
//             st_.pstMyTm_->PosMove.EnCZ = EnZ[0];
//             break;
//         default:
//             SetDefEnCXZ();
//             return;//はみ出しチェックしてあるのでそのまま終了
//         }
//
//         //はみ出チェック
//         CheckEnC();
//     }
//     //はみ出チェック
//     void TMgTeam::CheckEnC()
//     {
//         //はみ出てる
//         if (st_.pstMyTm_->PosMove.EnCX > DBCRT_CLI)
//         {
//             st_.pstMyTm_->PosMove.EnCX = DBCRT_CLI;
//         }
//
//         //３番外野用
//         st_.pstMyTm_->PosMove.EnCX3 = st_.pstMyTm_->PosMove.EnCX;
//
//         const int CRPOS_B_XL = DBCRT_SL + (16 * XYMAG);//奥外野Ｘ左限界
//         const int CRPOS_F_XL = DBCRT_SL;//手前外野Ｘ左限界
//
//         //はみ出てる
//         if (st_.pstMyTm_->PosMove.EnCX < CRPOS_B_XL)
//         {
//             st_.pstMyTm_->PosMove.EnCX = CRPOS_B_XL;
//         }
//         //はみ出てる
//         if (st_.pstMyTm_->PosMove.EnCX3 < CRPOS_F_XL)
//         {
//             st_.pstMyTm_->PosMove.EnCX3 = CRPOS_F_XL;
//         }
//         //１Ｐの外野はコート逆転
//         if (st_.mysideNo_ == 0)
//         {
//             st_.pstMyTm_->PosMove.EnCX = DBCRT_W - st_.pstMyTm_->PosMove.EnCX;
//             st_.pstMyTm_->PosMove.EnCX3 = DBCRT_W - st_.pstMyTm_->PosMove.EnCX3;
//         }
//     }
//
//
//     //ボールの位置（落下地点）
//     void TMgTeam::TeamBallPos()
//     {
//
//         BOOL infleftcrt_f = FALSE;
//         BOOL outleftcrt_f = FALSE;
//
//         const int STLW = 2 * XYMAG;
//         if (st_.mysideNo_ == 0)
//         {
//             infleftcrt_f = (pmgSG_->stBa_.LandX < (DBCRT_CL + STLW));
//             outleftcrt_f = (pmgSG_->stBa_.LandX < (DBCRT_CL - STLW));
//         }
//         else
//         {
//             infleftcrt_f = (pmgSG_->stBa_.LandX < (DBCRT_CL - STLW));
//             outleftcrt_f = (pmgSG_->stBa_.LandX < (DBCRT_CL + STLW));
//         }
//
//         BOOL infside = (infleftcrt_f)
//             ? 0
//             : 1;
//
//         int gaiyaside = (outleftcrt_f)
//             ? 1
//             : 0;
//
//         int tX = (pmgSG_->stBa_.LandX < DBCRT_CL)
//             ? pmgSG_->stBa_.LandX
//             : DBCRT_CL - (pmgSG_->stBa_.LandX - DBCRT_CL);
//
//
//         BOOL st1_f = ((st_.mysideNo_ != infside) && (pmgSG_->stBa_.Motion == bmFree));
//         BOOL st2_f = FALSE;
//         BOOL st3_f = FALSE;
//         BOOL st4_f = FALSE;
//
//
//         if (st_.mysideNo_ == gaiyaside)
//         {
//             if ((pmgSG_->stBa_.Motion == bmFree)
//                 || (pmgSG_->stBa_.Motion == bmBound))
//             {
//                 //強奪
//                 st2_f = TRUE;
//                 st3_f = TRUE;
//                 st4_f = TRUE;
//             }
//         }
//
//
//         //int st1 = (st1_f)
//         //? (pmgEO_->mgDt_.dtSet_.GetDtCOM(setNaiyaSteel) * XYMAG)
//         //: 0;
//
//         //int st2 = (st2_f)
//         //? (st_.pmgMyCh_[(int)dbpoO2]->Rank(rkomSTEAL_D) * XYMAG)
//         //: 0;
//         //
//         //int st3 = (st3_f)
//         //? (st_.pmgMyCh_[(int)dbpoO3]->Rank(rkomSTEAL_D) * XYMAG)
//         //: 0;
//         //
//         //int st4 = (st4_f)
//         //? (st_.pmgMyCh_[(int)dbpoO4]->Rank(rkomSTEAL_W) * XYMAG)
//         //: 0;
//
//         BOOL gaiya_f = FALSE;
//
//
//         //自チームボールだと判断するか
//         if (pmgSG_->stBa_.LandZ <= (DBCRT_FL))//強奪なし
//         {
//             st_.pstMyTm_->BallCrtPosL = (int)dbpoO3;
//             gaiya_f = TRUE;
//         }
//         else if (pmgSG_->stBa_.LandZ >= (DBCRT_BL))//強奪なし
//         {
//             st_.pstMyTm_->BallCrtPosL = (int)dbpoO2;
//             gaiya_f = TRUE;
//         }
//         else if (tX < ((DBCRT_SL)+((pmgSG_->stBa_.LandZ - DBCRT_FL) * 10 / DBCRT_SLR)))//強奪なし
//         {
//             st_.pstMyTm_->BallCrtPosL = (int)dbpoO4;
//             gaiya_f = TRUE;
//         }
//         else
//         {
//             st_.pstMyTm_->BallCrtPosL = (int)dbpoI0;
//         }
//
//
//         st_.pstMyTm_->BallCrtSideL = (gaiya_f)
//             ? gaiyaside
//             : infside;
//
//     }
//
//     //ボールの位置（実座標）
//     void TMgTeam::TeamBallPos2()
//     {
//
//         BOOL infleftcrt_f = FALSE;
//         BOOL outleftcrt_f = FALSE;
//
//         const int STLW = -4 * XYMAG;
//         if (st_.mysideNo_ == 0)
//         {
//             infleftcrt_f = (pmgSG_->stBa_.Zahyou.X < (DBCRT_CL + STLW));
//             outleftcrt_f = (pmgSG_->stBa_.Zahyou.X < (DBCRT_CL - STLW));
//         }
//         else
//         {
//             infleftcrt_f = (pmgSG_->stBa_.Zahyou.X < (DBCRT_CL - STLW));
//             outleftcrt_f = (pmgSG_->stBa_.Zahyou.X < (DBCRT_CL + STLW));
//         }
//
//         BOOL infside = (infleftcrt_f)
//             ? 0
//             : 1;
//
//         int gaiyaside = (outleftcrt_f)
//             ? 1
//             : 0;
//
//         int tX = (pmgSG_->stBa_.Zahyou.X < DBCRT_CL)
//             ? pmgSG_->stBa_.Zahyou.X
//             : DBCRT_CL - (pmgSG_->stBa_.Zahyou.X - DBCRT_CL);
//
//
//
//         BOOL gaiya_f = FALSE;
//
//
//         //自チームボールだと判断するか
//         if (pmgSG_->stBa_.Zahyou.Z <= (DBCRT_FL))//強奪なし
//         {
//             st_.pstMyTm_->BallCrtPosZ = (int)dbpoO3;
//             gaiya_f = TRUE;
//         }
//         else if (pmgSG_->stBa_.Zahyou.Z >= (DBCRT_BL))//強奪なし
//         {
//             st_.pstMyTm_->BallCrtPosZ = (int)dbpoO2;
//             gaiya_f = TRUE;
//         }
//         else if (tX < st_.pmgBa_->baCommon_.GetSLX())//強奪なし
//         {
//             st_.pstMyTm_->BallCrtPosZ = (int)dbpoO4;
//             gaiya_f = TRUE;
//         }
//         else
//         {
//             st_.pstMyTm_->BallCrtPosZ = (int)dbpoI0;
//         }
//
//         ////反転
//         //if (revSide_f)
//         //{
//         //  st_.pstMyTm_->BallCrtSideZ = (st_.pstMyTm_->BallCrtSideZ == 0)
//         //    ? 1
//         //    : 0;
//         //}
//
//         st_.pstMyTm_->BallCrtSideZ = (gaiya_f)
//             ? gaiyaside
//             : infside;
//
//     }
//
//
//     //操作権キャラ
//     void TMgTeam::NowCtrlNo()
//     {
//         if (st_.pmgRf_->IsCapOnlyCtrl())
//         {
//             //もともと０だから大丈夫か
//             MyTeamState.MainState.ControlOrderIndex = 0;
//         }
//         else
//         {
//             //まず現状から操作できるキャラを決める
//             GetCtrl();
//         }
//
//     }
//
//     //ポジショニング予定位置計算ポジショニングカウンタ減少
//     void TMgTeam::GetEnC_c()
//     {
//         //ポジショニング予定位置計算
//         //ポジショニングカウンタ減少
//         if (lib_num::AprTo0(&st_.pstMyTm_->PosMove.EnC_c))
//         {
//             const int GAIYAOFPOSITV = 60;//外野オフェンスポジションリセット間隔(f)
//
//             st_.pstMyTm_->PosMove.EnC_c = (st_.pmgEnTm_->IsLastOne())
//                 ? (GAIYAOFPOSITV / 4)
//                 : GAIYAOFPOSITV;
//
//             //ボールがもたれているとき
//             if (pmgSG_->stBa_.Motion == bmHold)
//             {
//                 //マイボール
//                 if (pmgSG_->stBa_.HoldTNo == st_.mysideNo_)
//                 {
//                     GetCOG();
//                 }
//                 else//敵ボール
//                 {
//                     SetDefEnCXZ();
//                 }
//             }
//         }
//     }
//
//
//
//     //ポジションカウンタ
//     void TMgTeam::SetPosition()
//     {
//
//         //現状使ってない
//
//         ////ディフェンスポジションリセット予約
//         //if (st_.pstMyTm_->PosReset_f)
//         //{
//         //  //リセット
//         //  st_.pstMyTm_->PosMove.Inf_c = pmgEO_->mgDt_.dtSet_.GetDtFormation(setNaiyaDfPosItv);
//         //  //SetDefFormation();//ディフェンス
//         //  st_.pstMyTm_->PosReset_f = FALSE;
//         //}
//
//     }
//
//
//     //各内野ポジションの現在行くべき位置★
//     void TMgTeam::GetFomposOfInf(int& getx, int& getz, int posNo)
//     {
//         int line = pmgEO_->mgDt_.dtFomation_.LineOfPos(GetMyFomationNo(), posNo);//衛
//         int linenum = pmgEO_->mgDt_.dtFomation_.GetDt(GetMyFomationNo(), (enDtFomation)(fomRPos + line));//衛内人数01234
//         int row = pmgEO_->mgDt_.dtFomation_.RowOfPos(GetMyFomationNo(), posNo);//衛内列
//         int zwide = pmgEO_->mgDt_.dtFomation_.GetDt(GetMyFomationNo(), fomZWidth);//123
//         int etc = pmgEO_->mgDt_.dtFomation_.GetDt(GetMyFomationNo(), fomEtc);//01
//
//         //横並び
//         //前中後センターラインからのＸ位置
//         const int XLDIST = 32 * XYMAG;//30
//         const int XL_FX = 84 * XYMAG;//96
//         const int XL_MX = (XL_FX + XLDIST);
//         const int XL_RX = (XL_MX + XLDIST);
//         //Ｚ幅
//         const int XLZWID = 16 * XYMAG;
//         const int XL_ZW3 = 64 * XYMAG;
//         const int XL_ZW2 = (XL_ZW3 - XLZWID);
//         const int XL_ZW1 = (XL_ZW2 - XLZWID);
//         //Ｘ幅
//         const int XL_XW = XLDIST / 3;
//         const int XL_SITUREV = 8 * XYMAG;//戦況による変動
//
//         //縦並び
//         //前中後サイドラインからのＺ位置
//         const int ZLDIST = 12 * XYMAG;//12
//         const int ZL_RZ = 72 * XYMAG;//68
//         const int ZL_MZ = ZL_RZ - ZLDIST;
//         const int ZL_FZ = ZL_MZ - ZLDIST;
//         //Ｚ幅
//         const int ZLXWID = 24 * XYMAG;//24
//         const int ZL_XW3 = 104 * XYMAG;//96
//         const int ZL_XW2 = (ZL_XW3 - ZLXWID);
//         const int ZL_XW1 = (ZL_XW2 - ZLXWID);
//         //Ｘ幅
//         const int ZL_ZW = ZLDIST / 3;
//         const int ZL_SITUREV = 3 * XYMAG;//戦況による変動
//
//         const int NLWDIV = 3;
//
//
//         const int FNLW = 1;
//         const int MNLW = 2;
//         const int RNLW = 3;
//         const int GRMN = 4;
//         const int FIJI = 5;
//
//         //フィジーはポジション順
//         if (etc == FIJI)
//         {
//             row = posNo;
//         }
//
//
//
//         //23外野持ち（縦並び）
//         if ((st_.pstMyTm_->PosMove.FType == ftDf2)
//             || (st_.pstMyTm_->PosMove.FType == ftDf3))
//         {
//             int zsign = 0;
//             //前衛中衛後衛
//             if (st_.pstMyTm_->PosMove.FType == ftDf3)
//             {
//                 getz = DBCRT_FL;
//                 zsign = +1;
//                 //switch (line)
//                 //{
//                 //case 0: getz += ZL_RZ; break;//後衛
//                 //case 1: getz += ZL_MZ; break;//中衛
//                 //case 2: getz += ZL_FZ; break;//前衛
//                 //}
//             }
//             else
//             {
//                 getz = DBCRT_BL;
//                 zsign = -1;
//                 //switch (line)
//                 //{
//                 //case 0: getz -= ZL_RZ; break;//後衛
//                 //case 1: getz -= ZL_MZ; break;//中衛
//                 //case 2: getz -= ZL_FZ; break;//前衛
//                 //}
//             }
//
//
//             switch (line)
//             {
//             case 0: getz += (zsign * ZL_RZ); break;//後衛
//             case 1: getz += (zsign * ZL_MZ); break;//中衛
//             case 2: getz += (zsign * ZL_FZ); break;//前衛
//             }
//
//             //戦況応援によるライン変動
//             getz += (zsign * MyTeamAiState.Main.FomRev * ZL_SITUREV);
//
//
//             if (linenum >= 2)
//             {
//                 int xw = 0;
//                 switch (zwide)//幅
//                 {
//                 case 1: xw = ZL_XW1; break;//狭い
//                 case 2: xw = ZL_XW2; break;//ふつう
//                 case 3: xw = ZL_XW3; break;//広い
//                 }
//
//                 //２?２狭い
//                 if (((line == 0) && (etc == RNLW))//後衛
//                     || ((line == 1) && (etc == MNLW))//中衛
//                     || ((line == 2) && (etc == FNLW)))//前衛
//                 {
//                     xw /= NLWDIV;
//                 }
//
//                 //間隔
//                 int dist = 0;
//                 dist = (xw / (linenum - 1));
//                 getx = DBCRT_CLXL + (xw / 2) - (row * dist);//中心+(幅/2)+(衛内番手 * 幅)
//
//                 //ドイツ
//                 if ((etc == GRMN) && (row == 1))
//                 {
//                     if (st_.pstMyTm_->PosMove.FType == ftDf2)
//                     {
//                         getz = getz + (ZLDIST);
//                     }
//                     else
//                     {
//                         getz = getz - (ZLDIST);
//                     }
//                 }
//             }
//             else
//             {
//                 getx = DBCRT_CLXL;
//             }
//         }
//         //内野および４番外野（横並び）
//         else
//         {
//
//             int ZUREKAKU = XL_ZW3;
//             int xsign = 0;
//
//             //前衛中衛後衛
//             if (st_.pstMyTm_->PosMove.FType == ftDf4)//敵外野持ち
//             {
//                 getx = DBCRT_SL;
//                 xsign = +1;
//
//                 switch (line)
//                 {
//                 case 0:
//                     //getx += XL_RX;
//                     ZUREKAKU = 64;
//                     break;//後衛
//                 case 1:
//                     //getx += XL_MX;
//                     ZUREKAKU = 13;
//                     break;//中衛
//                 case 2:
//                     //getx += XL_FX;
//                     ZUREKAKU = 7;
//                     break;//前衛
//                 }
//             }
//             else//内野持ち、敵内野持ち、味方外野持ち
//             {
//                 getx = DBCRT_CL;
//                 xsign = -1;
//
//                 switch (line)
//                 {
//                 case 0:
//                     //getx -= XL_RX;
//                     ZUREKAKU = 4;
//                     break;//後衛
//                 case 1:
//                     //getx -= XL_MX;
//                     ZUREKAKU = 5;
//                     break;//中衛
//                 case 2:
//                     //getx -= XL_FX;
//                     ZUREKAKU = 7;
//                     break;//前衛
//                 }
//
//             }
//
//             switch (line)
//             {
//             case 0:
//                 getx += (xsign * XL_RX);
//                 break;//後衛
//             case 1:
//                 getx += (xsign * XL_MX);
//                 break;//中衛
//             case 2:
//                 getx += (xsign * XL_FX);
//                 break;//前衛
//             }
//
//
//             //戦況応援によるライン変動
//             getx += (xsign * MyTeamAiState.Main.FomRev * XL_SITUREV);
//
//
//             //フィジー
//             if (etc == FIJI)
//             {
//                 ZUREKAKU = -1;
//             }
//
//             if (linenum >= 2)
//             {
//                 int zw = 0;
//                 switch (zwide)//幅
//                 {
//                 case 1: zw = XL_ZW1; break;//狭い
//                 case 2: zw = XL_ZW2; break;//ふつう
//                 case 3: zw = XL_ZW3; break;//広い
//                 }
//
//                 //２?２狭い
//                 if (((line == 0) && (etc == RNLW))//後衛
//                     || ((line == 1) && (etc == MNLW))//中衛
//                     || ((line == 2) && (etc == FNLW)))//前衛
//                 {
//                     zw /= NLWDIV;
//                 }
//
//
//                 //間隔
//                 int dist = 0;
//                 dist = (zw / 2) - (row * (zw / (linenum - 1)));
//                 getz = DBCRT_CLZ + dist;//中心+(幅/2)+(衛内番手 * 幅)
//
//                 //傾き
//                 getx += (dist / ZUREKAKU);
//
//                 //ドイツ
//                 if ((etc == GRMN) && (row == 1))
//                 {
//                     if (st_.pstMyTm_->PosMove.FType == ftDf4)
//                     {
//                         getx = getx - (XLDIST);
//                     }
//                     else
//                     {
//                         getx = getx + (XLDIST);
//                     }
//                 }
//
//             }
//             else
//             {
//                 getz = DBCRT_CLZ;
//             }
//         }
//
//     }
//
//     //ダッシュマン指令
//     void TMgTeam::OrderDashman()
//     {
//         //プレビューでは呼べない
//         if (pmgGO_->pmgRf_->IsPrev()) return;
//
//         //試合中のみ
//         if (st_.pstRf_->DBSSt != dbssShiai) return;
//
//         //操作権無いときには無効
//         if (MyTeamState.MainState.ControlOrderIndex == Defines.NGNUM) return;
//
//         st_.pstMyTm_->DashmanCallNo = Defines.NGNUM;
//         st_.pstMyTm_->DashmanAll_f = FALSE;
//
//
//         BOOL Call_f = FALSE;
//         BOOL iosi_f = mid::midIsTBL();
//         BOOL All_f = FALSE;
//         BOOL test_f = FALSE;
//
//         int callRsv = Defines.NGNUM;//呼ぶ予定の人
//         for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//         {
//             if (st_.pstMyTm_->PosMove.DashmanStock[i] != Defines.NGNUM)
//             {
//                 callRsv = st_.pstMyTm_->PosMove.DashmanStock[i];
//                 break;
//             }
//         }
//
//
//         //プレーヤー
//         if (IsCOM() == FALSE)
//         {
//             //コール
//             Call_f = st_.pSidePad_->IsDMCall();//ダッシュマンボタン
//         }
//         else//ＣＯＭのダッシュマン発動★★
//         {
//             BOOL delayOK_f = IsDMDelayTime();
//
//             //呼ぶタイミング＆シュートフラグがまだ
//             if (IsDMDelayTime()
//                 && st_.pstMyTm_->ofside_f)
//             {
//                 //一回休みフラグが立ってるとき
//                 if (MyTeamAiState.Main.DsOneItvWait_f)
//                 {
//                     MyTeamAiState.Main.DsOneItvWait_f = FALSE;
//                     return;
//                 }
//
//                 //ボールマンダッシュをそもそも呼ばない、もしくは、すでにセッターダッシュ開始後
//                 if ((MyTeamAiState.Main.setterBMRsv_f == FALSE)
//                     || MyTeamAiState.Main.setterBMRsvGo_f)
//                 {
//                     //呼びたい人数呼んでない
//                     if (MyTeamAiState.Main.dmcalledNum < MyTeamAiState.Main.dmcallNum)
//                     {
//                         Call_f = TRUE;
//                     }
//                 }
//                 else
//                 {
//                     //セッターの走り開始がまだ
//                     if (((GetBallmanNo() == MyTeamAiState.Main.setterNo) || st_.pmgMyCh_[MyTeamAiState.Main.setterNo]->IsMAN())//セッター操作中
//                         && ((callRsv > MyTeamAiState.Main.setterNo) //セッターより下位キャラが呼ばれてる
//                         || (MyTeamAiState.Main.dmcalledNum >= MyTeamAiState.Main.dmcallNum)//既に呼びたい人数に到達してる
//                         || (callRsv == Defines.NGNUM)))
//                     {
//                         MyTeamAiState.Main.setterBMRsvGo_f = TRUE;//セッターＢＭＤＯＫ
//                     }
//                     else
//                     {
//                         //呼びたい人数呼んでない
//                         if (MyTeamAiState.Main.dmcalledNum < MyTeamAiState.Main.dmcallNum)
//                         {
//                             Call_f = TRUE;
//                         }
//                     }
//                 }
//             }
//         }
//
//
//         //実際に呼ぶ
//         if (Call_f)
//         {
//             BOOL CrsL_f = FALSE;
//             BOOL CrsR_f = FALSE;
//             BOOL CrsU_f = FALSE;
//             BOOL CrsD_f = FALSE;
//
//             BOOL infCrs_f = FALSE;
//             BOOL enmCrs_f = FALSE;
//
//             BOOL DMGo_f = FALSE;
//
//             if (st_.pSidePad_ != NULL)
//             {
//                 st_.pSidePad_->GetDMCallCrs(&CrsL_f, &CrsR_f, &CrsU_f, &CrsD_f);
//
//                 infCrs_f = (((st_.mysideNo_ == 0) && CrsL_f)
//                     || ((st_.mysideNo_ == 1) && CrsR_f));
//
//                 enmCrs_f = (((st_.mysideNo_ == 0) && CrsR_f)
//                     || ((st_.mysideNo_ == 1) && CrsL_f));
//             }
//
//
//             if (IsCOM()//★サイド操作以外とりあえず
//                 || ((CrsL_f || CrsR_f || CrsU_f || CrsD_f) == FALSE))//ニュートラル
//             {
//                 //ダッシュスタイル
//                 switch (MyTeamAiState.Main.comPtn[comDMType])
//                 {
//                 case 0://ワイド
//                     if (callRsv != Defines.NGNUM)
//                     {
//                         //広がるように
//                         if (st_.pmgMyCh_[callRsv]->st_.pstMyCh_->Zahyou.Z > DBCRT_CLZ)
//                         {
//                             CrsU_f = TRUE;
//                         }
//                         else
//                         {
//                             CrsD_f = TRUE;
//                         }
//                     }
//                     break;
//                 case 1://アプローチ
//                     enmCrs_f = TRUE;
//                     break;
//                 case 2://ストレート
//                     infCrs_f = TRUE;
//                     break;
//                 default://クロス
//                     if (callRsv != Defines.NGNUM)
//                     {
//                         //狭まるように
//                         if (st_.pmgMyCh_[callRsv]->st_.pstMyCh_->Zahyou.Z > DBCRT_CLZ)
//                         {
//                             CrsD_f = TRUE;
//                         }
//                         else
//                         {
//                             CrsU_f = TRUE;
//                         }
//                     }
//                     break;
//                 }
//             }
//
//
//             int alldmNum = 0;
//             //int stock = 9;
//             for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//             {
//                 if (st_.pstMyTm_->PosMove.DashmanStock[i] != Defines.NGNUM)
//                 {
//                     DMGo_f = TRUE;//一人でも送り出せる
//                     //stock = i;
//                     ++alldmNum;
//                 }
//             }
//
//             //効果音
//             if (DMGo_f)
//             {
//                 SESet(seDmOK);
//                 //TZahyouRec* pzh = &(st_.pmgMyCh_[MyTeamState.MainState.ControlOrderIndex]->st_.pstMyCh_->Zahyou);
//                 //int fx = pzh->X / XYMAG;
//                 //int fy = DRAWBASELINE
//                 //  - ((pzh->Y + pzh->Z) / XYMAG)
//                 //  - DAMPOSY;
//                 //
//                 //int dt = stock;
//                 //
//                 ////カメラ位置
//                 //const int camposX = pmgEO_->mgCamera_.GetRevCamPosX();
//                 //enFontCol2 col = FC_Red;
//                 //pmgEO_->mgFont_.OrderDamFontSprite(lib_str::IntToStr(dt).c_str(), fx, fy, col);
//             }
//             else
//             {
//                 if (IsCOM() == FALSE)//★サイド操作以外とりあえず
//                 {
//                     SESet(seDmNG);
//                     //mid::midLog("ng0\n");
//                 }
//
//                 MyTeamAiState.Main.callNG_f = TRUE;//呼べないときが１回でもあった
//             }
//
//             for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//             {
//                 if (st_.pstMyTm_->PosMove.DashmanStock[i] != Defines.NGNUM)
//                 {
//                     if (All_f)
//                     {
//                         st_.pstMyTm_->DashmanAll_f = TRUE;//全員
//                     }
//                     else
//                     {
//                         st_.pstMyTm_->DashmanCallNo = st_.pstMyTm_->PosMove.DashmanStock[i];
//                     }
//
//                     //呼んだ人
//                     if (st_.pstMyTm_->DashmanType == dmtNone)
//                     {
//                         st_.pstMyTm_->DMCallmanNo = MyTeamState.MainState.ControlOrderIndex;
//                     }
//
//                     if (st_.pstMyTm_->CallingDashman_f2)
//                     {
//                         st_.pstMyTm_->CallingDashman_f3 = TRUE;
//                     }
//
//                     ////呼ぶカウンタ減らす
//                     //lib_num::AprTo0(&MyTeamAiState.Main.dmcallNum);
//
//                     //作戦が決まってないとき
//                     if ((st_.pstMyTm_->DashmanType == dmtNone)
//                         //&& (st_.pstMyTm_->CallingDashman_f2 == FALSE)
//                         //&& (st_.pstMyTm_->SideMaruhi_f == FALSE)
//                         )
//                     {
//
//                         //std::string dmname = "";
//                         //
//                         //if (CrsU_f)
//                         //{
//                         //  st_.pstMyTm_->DashmanType = dmtUp;
//                         //  dmname = "サイド";
//                         //}
//                         //else if (CrsD_f)
//                         //{
//                         //  st_.pstMyTm_->DashmanType = dmtDw;
//                         //  dmname = "サイド";
//                         //}
//                         //else if (infCrs_f) //後ろ
//                         //{
//                         //  st_.pstMyTm_->DashmanType = dmtStr;
//                         //  dmname = "ストレート";
//                         //}
//                         //else if (enmCrs_f)
//                         //{
//                         //  st_.pstMyTm_->DashmanType = dmtAp;
//                         //  dmname = "センター";
//                         //}
//                         //else
//                         //{
//                         //  st_.pstMyTm_->DashmanType = dmtStr;
//                         //  dmname = "ストレート";
//                         //}
//                         //
//                         //if (IsNoJpDashman())
//                         //{
//                         //  dmname += "ダッシュ";
//                         //}
//                         //else
//                         //{
//                         //  dmname += "ジャンプ";
//                         //}
//                         //
//                         //
//                         ////作戦表示//dmname += "だっしゅまん";//ここはチームごとの作戦名を
//                         //TZahyouRec* pzh = &(st_.pmgMyCh_[st_.pstMyTm_->DMCallmanNo]->st_.pstMyCh_->Zahyou);
//                         //pmgGO_->pmgCommon_->PopupMsg(pu_DsMn, dmname.c_str(), st_.mysideNo_, pzh->X, pzh->Y, pzh->Z, FC2_shiai);
//
//
//
//                     }
//
//                     break;
//                 }
//
//             }
//         }
//     }
//
//     //内外野入れ替えが発生する人数
//     BOOL TMgTeam::IsIOChange()
//     {
//         //元外野が012
//         const int OUTNUM = (DBMEMBER_ALL - Defines.DBMEMBER_INF);
//         return (st_.pstMyTm_->UseMotoGaiya_c < OUTNUM);
//     }
//     //内外野入れ替え確認
//     void TMgTeam::CheckChangePos()
//     {
//
//         BOOL change_f = FALSE;
//
//         for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//         {
//             //死亡確認
//             if ((st_.pmgMyCh_[i]->st_.pstMyCh_->HP == 0)
//                 && (st_.pmgMyCh_[i]->st_.pstMyCh_->ANGEL_f)
//                 && (st_.pmgMyCh_[i]->st_.pstMyCh_->Dead_f == FALSE))
//             {
//
//
//                 std::string out = st_.pmgMyCh_[i]->MyName() + "＿アウト";
//                 pmgGO_->pmgCommon_->PopupMsg(pu_DsMn, out.c_str(),
//                     st_.mysideNo_, 0, 0, 0, FC2_shiai, FALSE);
//
//
//                 //倒された数カウンタ増やす
//                 ++st_.pstMyTm_->Dead_c;
//
//                 BOOL IOChange_f = IsIOChange();//★FALSE;//
//
//                 //サドンデス決着後は入れ替えしない
//                 if (st_.pmgRf_->IsSuddunDeath()
//                     && (DeadNum() != st_.pmgEnTm_->DeadNum()))
//                 {
//                     IOChange_f = FALSE;
//                 }
//
//                 if (IOChange_f)//交換
//                 {
//                     //天使になる人
//                     //天使オブジェクト生成
//                     //pmgGO_->pmgCommon_->SetEObj(etomAngel, st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou,
//                     //                st_.mysideNo_,st_.pstMyTm_->PosSt.CharofP[i]);
//
//                     //SESet(seDead);//SEならす
//
//                     SetAngel(i);
//
//
//                     st_.pmgMyCh_[i]->chCommon_.StInit(TRUE);
//
//                     //st_.pmgMyCh_[i]->chCommon_.SetMtype(dbmtJDn);
//
//                     st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Y = 0;
//                     st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.X = (st_.mysideNo_ == 0)
//                         ? DBCRT_W
//                         : 0;
//                     st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Muki = (st_.mysideNo_ == 0)
//                         ? mL
//                         : mR;
//
//                     change_f = TRUE;
//                     switch (st_.pstMyTm_->IOChange_c)
//                     {
//                     case 0:
//                         st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z = CRPOS_B_Z;//pmgEO_->mgDt_.dtSet_.GetDtFormation(setPos4_Z) * XYMAG;
//                         break;
//                     case 1:
//                         st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z = CRPOS_F_Z;//pmgEO_->mgDt_.dtSet_.GetDtFormation(setPos5_Z) * XYMAG;
//                         break;
//                     case 2:
//                         st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z = POS6_Z;
//                         break;
//                     default:
//                         //change_f = FALSE;
//                         break;
//                     }
//
//                     //交換先の外野がボールを持ってる場合
//                     const int GAIYANUM = 3;
//                     int changePos = Defines.DBMEMBER_INF + (st_.pstMyTm_->IOChange_c % GAIYANUM);
//                     if (st_.pmgMyCh_[changePos]->IsBall())
//                     {
//                         st_.pmgBa_->baCommon_.SetMtype(bmBound);//手放す
//                         st_.pmgBa_->baCommon_.ZeroXYZ();
//
//                         pmgSG_->stBa_.NGGet_f = TRUE;
//                         pmgSG_->stBa_.NGGetPNo = i;//入れ替え後のpos
//                         pmgSG_->stBa_.NGGetTNo = st_.mysideNo_;
//                     }
//
//                     //上田カウントが残っている場合
//                     if (st_.pstMyTm_->TmUeda_c > 0)
//                     {
//                         --st_.pstMyTm_->TmUeda_c;//上田カウント減らす
//                         st_.pmgMyCh_[i]->st_.pstMyCh_->ANGEL_f = FALSE;//天使の輪を消す
//                     }
//                     else
//                     {
//                         ++st_.pstMyTm_->UseMotoGaiya_c;//元外野を使う
//                     }
//
//
//                     //ポジションチェンジ//交換した回数に基づいた位置から
//                     pmgSG_->ChangePos(st_.mysideNo_, i, changePos);
//
//                     //入れ替えたポジションのＨＰにリセット
//                     //全員初期化の段階でHP入ってるので、現状ここでの差し替えは不要
//                     if (st_.pmgRf_->IsSuddunDeath())
//                     {
//                         st_.pmgMyCh_[i]->st_.pstMyCh_->HP = 1;//サドンデスならＨＰ１に
//                     }
//
//
//                     //交換先の外野がパスタゲの場合、変わった後の人がパスタゲにならないように
//                     if ((pmgSG_->stBa_.Motion == bmPass)
//                         && (pmgSG_->stBa_.PaTgTNo == st_.mysideNo_)
//                         && (pmgSG_->stBa_.PaTgPNo == changePos))
//                     {
//                         pmgSG_->stBa_.PaTgPNo = i;
//                     }
//
//                     ++st_.pstMyTm_->IOChange_c;//交換した回数
//                 }
//                 else
//                 {
//
//                     st_.pmgMyCh_[i]->chCommon_.SetMtype(dbmtANG);
//                 }
//
//                 ////誰か死んだらＣＯＭ思考リセット
//                 //st_.pstMyTm_->RsvResetCOM_f = TRUE;
//             }
//
//         }
//
//         //内外野交代になったとき
//         if (change_f && (st_.pstRf_->Demo_f == FALSE))
//         {
//             //名前
//             SetShiaiName();
//         }
//     }
//
//     void TMgTeam::SetAngel(int posNo)
//     {
//         //天使オブジェクト生成
//         pmgGO_->pmgCommon_->SetEObj(etomAngel,
//             st_.pmgMyCh_[posNo]->st_.pstMyCh_->Zahyou,
//             st_.mysideNo_,
//             st_.pstMyTm_->PosSt.CharofP[posNo]);
//
//         //てきキャプテンがアイテムおとす
//
//         if (pmgEO_->stShiai_.IsVSHum() == FALSE &&	// 通信対戦じゃない
//             !pmgEO_->stSeq_.IsFirstTuto())		// 初回チュートリアルじゃない
//         {
//             if ((st_.mysideNo_ == SIDE1)//→チーム
//                 && (st_.pmgMyCh_[posNo]->st_.objNo_ == 0)//キャプテン
//                 && (st_.droped_f_ == FALSE)//落としてない
//                 && (st_.pmgRf_->IsTour() == FALSE)//疑似対人以外
//                 && (pmgEO_->stShiai_.IsTuto() == FALSE)
//                 )
//             {
//                 st_.droped_f_ = TRUE;
//
//                 pmgGO_->pmgCommon_->SetEObj(etomItem,
//                     st_.pmgMyCh_[posNo]->st_.pstMyCh_->Zahyou,
//                     st_.mysideNo_,
//                     st_.pstMyTm_->PosSt.CharofP[posNo]);
//
//                 SysSESet(sysNaBoxOtona);//SEならす
//             }
//         }
//
//
//         SESet(seDead);//SEならす
//     }
//     //サドンデス状態に変更
//     void TMgTeam::SuddunDeath()//チーム設定をシーンから貰うか
//     {
//         for (int i = 0; i < DBMEMBER_ALL; ++i)
//         {
//             if (st_.pmgMyCh_[i]->IsInfield() && (st_.pmgMyCh_[i]->st_.pstMyCh_->HP > 0))
//             {
//                 st_.pmgMyCh_[i]->st_.pstMyCh_->HP = 1;
//             }
//         }
//     }
//
//     ////ヒットストップリセット
//     //void TMgTeam::ResetHitStop()
//     //{
//     //  //for (int i = 0; i < DBMEMBER_ALL; ++i)
//     //  //{
//     //  //  st_.pmgMyCh_[i]->st_.pstMyCh_->HitStop_f = FALSE;
//     //  //}
//     //}
//
//     //デバッグ用
//     void TMgTeam::COMDebugSwitch()
//     {
//         ////MyTeamAiState.Main.comPtn[comRessei] = ;//劣勢判断 //体力一定以下いれば	残りが敵未満	ボス退場	内野一人
//         ////MyTeamAiState.Main.comPtn[comBallTake] = ;//ボール奪い //劣勢なら行く	行く	行かない	とれそうなら
//
//         ////MyTeamAiState.Main.comPtn[comPinch] = ;//ピンチガード //短い	ほぼ同時	長い	普通
//         ////MyTeamAiState.Main.comPtn[comPinchTag] = ;//ピンチガード対象 //対象が瀕死なら	対象がいれば	しない	外野攻撃なら
//         //MyTeamAiState.Main.comPtn[comCounter] = ;//カウンターキャッチ//劣勢ならする] = ;する] = ;しない] = ;劣勢ならしない
//         ////MyTeamAiState.Main.comPtn[comDgType] = ;//ドッジタイプ//サイド避け] = ;ジャンプ避け] = ;膝付き避け] = ;シュート対応避け
//
//         ////MyTeamAiState.Main.comPtn[comPassCut] = ;//パスカット//劣勢ならする] = ;する] = ;しない] = ;対面パスなら
//
//         //MyTeamAiState.Main.comPtn[comQDMCallCH] = 1;//DM コール//劣勢ならする] = ;する] = ;しない] = ;劣勢でないなら
//         //MyTeamAiState.Main.comPtn[comQDMCallNC] = 1;//DM コール//劣勢ならする] = ;する] = ;しない] = ;劣勢でないなら
//         //MyTeamAiState.Main.comPtn[comDMCallCH] = 1;//DM コール//劣勢ならする] = ;する] = ;しない] = ;劣勢でないなら
//         //MyTeamAiState.Main.comPtn[comDMCallNC] = 1;//DM コール//劣勢ならする] = ;する] = ;しない] = ;劣勢でないなら
//         //MyTeamAiState.Main.comPtn[comDMSetter] = 0;//OFDM セッター指定//一番前方の内野,内野最下位オーダー,ボス,外野（自分が内野ならば一番近い外野）
//         //MyTeamAiState.Main.comPtn[comDMSetBM] = 0;//DM タイミング//キャッチしたら] = ;速攻] = ;ボールマンジャンプタイミング] = ;ボールマンダッシュと同時
//         //
//         //MyTeamAiState.Main.comPtn[comDMNum] = 1;//DM 人数//3人いたら] = ;ALL] = ;1人] = ;2人いたら
//         //MyTeamAiState.Main.comPtn[comDMDelay] = 0;//DM ディレイ//短い,ほぼ同時,長い,普通
//         //MyTeamAiState.Main.comPtn[comDMType] = 0;//ダッシュマンスタイル//ラインダッシュ,サイドダッシュ,ノーマルダッシュ,クロスダッシュ
//         //MyTeamAiState.Main.comPtn[comChance] = 0;//チャンス判断//劣勢なら,する,しない,優勢なら
//         //MyTeamAiState.Main.comPtn[comDMPaNum] = 0;//DM パス回数//一人一回,出来るかぎり,しない,１回
//         //MyTeamAiState.Main.comPtn[comDMPaTag] = 0;//DM パス先//一つオーダー上位,近い人,遠い人,後方の人
//         //MyTeamAiState.Main.comPtn[comDMPaTime] = 3;//DM パスタイミング//パス先が前衛ライン超えたら	パスインターバル後	自分のシュートタイミング	パス先がダッシュ開始したら
//         //MyTeamAiState.Main.comPtn[comDMPaItv] = 1;//DM パスインターバル//短い,最短,長い,普通
//
//         ////MyTeamAiState.Main.comPtn[comDMPaItv] = ;//DM パスインターバル//短い] = ;最短] = ;長い] = ;普通
//         ////MyTeamAiState.Main.comPtn[comChMake] = ;//チャンスメイク//劣勢なら] = ;する] = ;しない] = ;優勢なら
//         ////MyTeamAiState.Main.comPtn[comChMakePtn] = ;//チャンスメイクスタイル//側面パス] = ;最上位狙い] = ;反時計] = ;対面パス
//         ////MyTeamAiState.Main.comPtn[comChMakeLoop] = ;//チャンスメイクループMAX//少ない] = ;多い] = ;最少] = ;普通
//         //MyTeamAiState.Main.comPtn[comAction] = 1;//アクション//シュート変更パス] = ;//パワーシュート] = ;シングルパス] = ;オーダーパス
//
//         ////MyTeamAiState.Main.comPtn[comShPaNum] = ;//シュート変更パス回数//少ない] = ;最少] = ;多い] = ;普通
//         ////MyTeamAiState.Main.comPtn[comOdPaTag] = ;//オーダーパス先//上位オーダー] = ;ぱわー＋きあい] = ;ボス] = ;上位ラインナップ
//         ////MyTeamAiState.Main.comPtn[comSgPaTag] = ;//シングルパス先//内野] = ;近接] = ;ランダム] = ;対面
//         ////MyTeamAiState.Main.comPtn[comNrPaTag] = ;//近接パスの狙い//上位オーダーに近い] = ;攻撃力高に近い] = ;ＨＰ少に近い] = ;守備下手に近い
//         ////MyTeamAiState.Main.comPtn[comPaWait] = ;//シングルパスウエイト//短い] = ;最短] = ;長い] = ;普通
//
//         //MyTeamAiState.Main.comPtn[comPaType] = 2;//パスタイプ//ジャンプトスパス,トスパス,クイックパス,ジャンプクイックパス
//         ////MyTeamAiState.Main.comPtn[comQShType] = ;//クイックシュートタイプ//Ｄシュート] = ;ＤＪシュート] = ;立ちシュート] = ;Ｊシュート
//         //MyTeamAiState.Main.comPtn[comPwShType] = 0;//シュートタイプ//Ｄシュート] = ;ＤＪシュート] = ;立ちシュート] = ;Ｊシュート
//         //MyTeamAiState.Main.comPtn[comShWait] = 0;//パワーウェイト//短い] = ;最短] = ;長い] = ;普通
//         //MyTeamAiState.Main.comPtn[comDShStep] = 1;//Ｄシュート歩数//６（ナイス）] = ;４（必殺）] = ;７歩] = ;５歩
//         ////MyTeamAiState.Main.comPtn[comJpPos] = ;//ＤＪシュート踏切位置//短い] = ;ラインギリギリ] = ;長い] = ;普通
//         ////MyTeamAiState.Main.comPtn[comJShTime] = ;//ＤＪシュートタイミング//頂点（必殺）] = ;早め（ＤＪナイス）] = ;ジャンプしない] = ;下降
//         //MyTeamAiState.Main.comPtn[comDsPos] = 0;//ダッシュ開始位置//必殺位置まで下がる] = ;エンドラインまで下がる] = ;ダッシュ] = ;ナイス位置までさがる
//
//         //MyTeamAiState.Main.comPtn[comGuard] = 1;//ガード//対象が瀕死なら] = ;対象がいれば] = ;しない] = ;外野攻撃なら
//         //MyTeamAiState.Main.comPtn[comGuardTag] = 1;//ガード対象//上位オーダー] = ;誰でも] = ;ボス] = ;上位ラインナップ
//         //MyTeamAiState.Main.comPtn[comCounter] = 1;//カウンターキャッチ//劣勢ならする,する,しない,劣勢ならしない
//
//         //MyTeamAiState.Main.comPtn[comChance] = 1;//チャンス判断//劣勢なら,する,しない,優勢なら
//         //MyTeamAiState.Main.comPtn[comDMNum] = 0;//DM 人数//３メン,ALL,1人,２メン
//
//         //MyTeamAiState.Main.comPtn[comQDMCallCH] = 2;//クイックDM コールチャンス//劣勢ならする,する,しない,劣勢でないなら
//         //MyTeamAiState.Main.comPtn[comQDMCallNC] = 1;//クイックQDM コールノーチャンス//劣勢ならする,する,しない,劣勢でないなら
//
//         //MyTeamAiState.Main.comPtn[comDMCallCH] = 2;//DM コールチャンス//劣勢ならする,する,しない,劣勢でないなら
//         //MyTeamAiState.Main.comPtn[comDMCallNC] = 2;//DM コールノーチャンス//劣勢ならする,する,しない,劣勢でないなら
//         //
//         ////OFDM セッター指定//一番前方の内野,内野最下位オーダー,ボス,外野（自分が内野ならば一番近い外野）
//         //MyTeamAiState.Main.comPtn[comDMSetter] = 0;
//         ////OFDM セッター＆ボールマン参加//セッタースタート＆ＢＭ参加ＯＫ,セッター関係なし＆ＢＭ参加ＯＫ,セッター関係なし＆ＢＭ参加ＮＧ,セッタースタート＆ＢＭ参加ＮＧ
//         //MyTeamAiState.Main.comPtn[comDMSetBM] = 0;
//         ////DM 通常／態勢ウエイト（チャンス時スルー）//１５／６０、０／態勢Ｗナシ、６０／揃うまで、３０／１２０
//         //MyTeamAiState.Main.comPtn[comDMWait] = 0;
//         ////DM ダッシュ開始トリガー//０フレウエイト、セッターパスと同時に、６０フレウェイトしてから開始、３０フレウェイトしてから開始
//         //MyTeamAiState.Main.comPtn[comDMTrigger] = 0;
//         ////DM ディレイ//短い,ほぼ同時,長い,普通
//         //MyTeamAiState.Main.comPtn[comDMDelay] = 0;
//         ////ダッシュマンスタイル//ラインダッシュ,サイドダッシュ,ノーマルダッシュ,クロスダッシュ
//         //MyTeamAiState.Main.comPtn[comDMType] = 0;
//         //
//         //MyTeamAiState.Main.comPtn[comDMPaNum] = 0;//DM パス回数//一人一回,出来るかぎり,しない,１回
//         //MyTeamAiState.Main.comPtn[comDMPaTag] = 0;//DM パス先//一つオーダー上位,近い人,遠い人,後方の人
//         //MyTeamAiState.Main.comPtn[comDMPaTime] = 3;//DM パスタイミング//パス先が前衛ライン超えたら	パスインターバル後	自分のシュートタイミング	パス先がダッシュ開始したら
//         //MyTeamAiState.Main.comPtn[comDMPaItv] = 0;//DM パスインターバル//短い,最短,長い,普通
//         //MyTeamAiState.Main.comPtn[comPaType] = 0;//パスタイプ//ジャンプトスパス,トスパス,クイックパス,ジャンプクイックパス
//         //MyTeamAiState.Main.comPtn[comPwShType] = 1;//クイックシュートタイプ//Ｄシュート,ＤＪシュート,立ちシュート,Ｊシュート
//         //MyTeamAiState.Main.comPtn[comQShType] = 0;//クイックシュートタイプ//Ｄシュート,ＤＪシュート,立ちシュート,Ｊシュート
//         //MyTeamAiState.Main.comPtn[comCrvType] = 0;//変化球タイプ//カーブ,タゲ外し,曲げない,トレース
//
//         //MyTeamAiState.Main.comPtn[comAction] = 2;
//         //MyTeamAiState.Main.comPtn[comShWait] = 2;
//     }
//
//
//     //COMダッシュマン作戦（とりあえずノーマルダッシュマンのみ）
//     void TMgTeam::COMDashman()
//     {
//
//         //そもそもオフェンスの時のときのみ★キャッチ前も可能であれば出来るように
//         if (st_.pstMyTm_->ofside_f == FALSE) return;
//
//         //１人の時も無理
//         if (IsLastOne()) return;
//
//         BOOL dmcall_f = FALSE;//そもそも呼ぶか
//         BOOL nowcallOK_f = TRUE;//この瞬間に呼べるか
//         BOOL waitcallOK_f = TRUE;//待てば呼べるか
//
//
//         //コールによるリセットの場合はダッシュマン判断
//         if (st_.pstMyTm_->CallingDMReset_f)
//         {
//             dmcall_f = TRUE;
//         }
//         else
//         {
//             int dmc = (MyTeamAiState.Main.Chance_f)//チャンスダッシュマンかどうか
//                 ? MyTeamAiState.Main.comPtn[comDMCallCH]
//                 : MyTeamAiState.Main.comPtn[comDMCallNC];
//
//             //優劣による判断
//             switch (dmc)
//             {
//             case 0://劣勢ならする
//                 dmcall_f = MyTeamAiState.Main.Ressei_f;
//                 break;
//             case 1://する
//                 dmcall_f = TRUE;
//                 break;
//             case 2://しない
//                 dmcall_f = FALSE;
//                 break;
//             default://劣勢でないなら
//                 dmcall_f = !(MyTeamAiState.Main.Ressei_f);
//                 break;
//             }
//         }
//
//         //dmcall_f = TRUE;//★テスト
//
//         if (dmcall_f == FALSE) return;
//
//         //セッター
//         int setterNo = Defines.NGNUM;
//
//
//         //セッター指定
//         switch (MyTeamAiState.Main.comPtn[comDMSetter])
//         {
//         case 0://一番前方の内野
//             setterNo = GetFrontInfPNo();
//             break;
//         case 1://内野最下位オーダー
//             setterNo = GetBottomInfPNo();
//             break;
//         case 2://ボス
//             setterNo = st_.pstMyTm_->GetRdPos();
//             //ボスが退場しているときは待っても無理
//             if (st_.pmgMyCh_[setterNo]->st_.pstMyCh_->Dead_f
//                 || st_.pmgMyCh_[setterNo]->st_.pstMyCh_->ANGEL_f)
//             {
//                 waitcallOK_f = FALSE;
//             }
//             break;
//         default://外野（自分が内野ならば一番近い外野）
//             setterNo = GetNearGaiyaPNo();
//             break;
//         }
//
//
//         //セッターチェック
//         BOOL setterCk_f = (st_.pstMyTm_->CallingDMReset_f == FALSE)
//             && ((MyTeamAiState.Main.comPtn[comDMSetBM] == 0) || (MyTeamAiState.Main.comPtn[comDMSetBM] == 3));
//
//         //セッターチェックする
//         if (setterCk_f)
//         {
//             //セッターが参加できない
//             if (setterNo == Defines.NGNUM)
//             {
//                 nowcallOK_f = FALSE;
//             }
//             else
//             {
//                 //操作権キャラ以外がセッター
//                 if (MyTeamState.MainState.ControlOrderIndex != setterNo)
//                 {
//                     //セッターがフリーモーション
//                     nowcallOK_f = (st_.pmgMyCh_[setterNo]->chCommon_.IsFreeMotion(TRUE)
//                         && (st_.pmgMyCh_[setterNo]->st_.pstMyCh_->Motion.IsMFlags(dbmfAr) == FALSE));
//                 }
//             }
//         }
//         else//セッターチェックしない
//         {
//             setterNo = GetBallmanNo();//マニュアルもいるので、持ってる人に
//             //setterNo = MyTeamState.MainState.ControlOrderIndex;//操作キャラがセッターになる
//             //dmcall_f = TRUE;
//         }
//
//
//
//         int higher_c = 0;//セッターより上位の人数
//
//         if (setterNo != Defines.NGNUM)//セッターの人がいる場合
//         {
//             for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//             {
//                 if (st_.pstMyTm_->PosMove.DashmanStock[i] == Defines.NGNUM) continue;
//                 //セッターより後衛オーダーが居るとき
//                 if (setterNo > st_.pstMyTm_->PosMove.DashmanStock[i])
//                 {
//                     ++higher_c;
//                 }
//             }
//         }
//
//         const int DM_3MEN = 3;//３メン
//         const int DM_2MEN = 2;//２メン
//         const int DM_1MEN = 1;//シングル
//
//
//         //セッターボールマンダッシュＯＫ
//         BOOL setterBMOK_f = FALSE;
//
//         if (setterNo != Defines.NGNUM)
//         {
//             BOOL dsok_f = (st_.pmgMyCh_[setterNo]->IsMAN()//マニュアル
//                 || (st_.pmgMyCh_[setterNo]->IsDMOKPos() && (st_.pstMyTm_->PosMove.Postman != setterNo)));//セッターが走れる位置でポストマンではない
//
//             setterBMOK_f = (IsLastOne() == FALSE) //残り１人の時は無理
//                 && (setterNo != Defines.NGNUM)//セッターの人がいる場合
//                 && (setterNo < Defines.DBMEMBER_INF)//内野
//                 && dsok_f
//                 && ((MyTeamAiState.Main.comPtn[comDMSetBM] == 0) || (MyTeamAiState.Main.comPtn[comDMSetBM] == 1));//セッター走る作戦
//         }
//         //ダッシュマン参加可能人数
//         int dmoknum = st_.pstMyTm_->PosMove.PassAbleNum;//現状の待機人数
//
//         //セッターが内野でセッター参加可能で、セッターがＢＭＤ可能位置にいる場合、一人足す
//         if (setterBMOK_f)
//         {
//             ++dmoknum;
//         }
//
//         int dmcallNum = 0;//コール人数
//         int usesetBM_f = FALSE;//セッターをボールマンダッシュさせる
//
//
//         //いけいけのときは２メンとかで１人しか呼べなくても呼ぶ
//         if (st_.pstMyTm_->CallingDMReset_f)
//         {
//             if (st_.pstMyTm_->PosMove.PassAbleNum > 0)//呼べる人が誰かいる
//             {
//                 //ダッシュマン発動人数
//                 switch (MyTeamAiState.Main.comPtn[comDMNum])
//                 {
//                 case 0://３メン
//                     dmcallNum = lib_num::Min(DM_3MEN, st_.pstMyTm_->PosMove.PassAbleNum);
//                     break;
//                 case 1://マックス
//                     dmcallNum = st_.pstMyTm_->PosMove.PassAbleNum;
//                     break;
//                 case 2://シングル
//                     dmcallNum = DM_1MEN;
//                     break;
//                 case 3://２メン
//                     dmcallNum = lib_num::Min(DM_2MEN, st_.pstMyTm_->PosMove.PassAbleNum);
//                     break;
//                 }
//                 //usesetBM_f = setterBMOK_f;
//             }
//             else
//             {
//                 nowcallOK_f = FALSE;
//                 //if (GetLiveNum() < (DM_1MEN + 1))//生きてる人が二人未満
//                 //{
//                 //  waitcallOK_f = FALSE;
//                 //}
//             }
//         }
//         else
//         {
//             //ダッシュマン発動人数
//             switch (MyTeamAiState.Main.comPtn[comDMNum])
//             {
//             case 0://３メン
//                 if (dmoknum >= DM_3MEN)//３人以上
//                 {
//                     dmcallNum = DM_3MEN;
//
//                     if ((DM_3MEN > higher_c) && setterBMOK_f)//上位で３メン足りない
//                     {
//                         usesetBM_f = TRUE;
//                         --dmcallNum;//呼ぶ人減らす
//                     }
//                 }
//                 else
//                 {
//                     nowcallOK_f = FALSE;
//                     if (GetLiveNum() < DM_3MEN)//生きてる人が３人いない
//                     {
//                         waitcallOK_f = FALSE;
//                     }
//                 }
//                 break;
//             case 1://マックス
//                 if (st_.pstMyTm_->PosMove.PassAbleNum > 0)//呼べる人が誰かいる
//                 {
//                     dmcallNum = st_.pstMyTm_->PosMove.PassAbleNum;
//                     usesetBM_f = setterBMOK_f;
//                 }
//                 else
//                 {
//                     nowcallOK_f = FALSE;
//                     if (GetLiveNum() < (DM_1MEN + 1))//生きてる人が二人未満
//                     {
//                         waitcallOK_f = FALSE;
//                     }
//                 }
//                 break;
//             case 2://シングル
//                 if (dmoknum >= DM_1MEN)// && (higher_c >= DM_1MEN))//自分より上位が居る１人いる
//                 {
//                     dmcallNum = DM_1MEN;
//                 }
//                 else
//                 {
//                     nowcallOK_f = FALSE;
//                     if (GetLiveNum() < (DM_1MEN + 1))//生きてる人が二人未満
//                     {
//                         waitcallOK_f = FALSE;
//                     }
//                 }
//                 break;
//             default://２メン
//                 if (dmoknum >= DM_2MEN)
//                 {
//                     dmcallNum = DM_2MEN;
//
//                     if ((DM_2MEN > higher_c) && setterBMOK_f)//上位で３メン足りない
//                     {
//                         usesetBM_f = TRUE;
//                         --dmcallNum;//呼ぶ人減らす
//                     }
//                 }
//                 else
//                 {
//                     nowcallOK_f = FALSE;
//                     if (GetLiveNum() < DM_2MEN)//生きてる人が２人いない
//                     {
//                         waitcallOK_f = FALSE;
//                     }
//                 }
//                 break;
//             }
//         }
//
//
//         if (dmcallNum == 0) nowcallOK_f = FALSE;
//
//         //チュートリアルではダッシュマン呼ばない
//         if (pmgEO_->stSeq_.IsTutoRetro() || pmgEO_->stSeq_.IsTutoSmart() || pmgEO_->stSeq_.IsTutoAuto()) nowcallOK_f = FALSE;
//
//         //疑似対人でもダッシュマン呼ばない
//         if (st_.pmgRf_->IsTour()) nowcallOK_f = FALSE;
//
//         //ここで発動ＯＫＮＧ完全判断
//         if (nowcallOK_f)//現状発動出来る(オフェンスタイミングと態勢ウエイト後の２回ここに来る)
//         {
//             MyTeamAiState.Main.dashman_f = TRUE;
//         }
//         else if (waitcallOK_f && (st_.pstMyTm_->CallingDMReset_f == FALSE))//待てば行ける
//         {
//             MyTeamAiState.Main.dashmanFmWait_f = TRUE;
//         }
//
//         //ダッシュマンではない
//         if (MyTeamAiState.Main.dashman_f == FALSE)
//         {
//             return;
//         }
//
//         //コール人数
//         MyTeamAiState.Main.dmcallNum = dmcallNum;
//         //セッター
//         MyTeamAiState.Main.setterNo = setterNo;
//         //既にセッターの手元かどうか
//         MyTeamAiState.Main.setterOK_f = (MyTeamAiState.Main.setterNo == GetBallmanNo());
//
//         //セッターダッシュ可能で待つ
//         MyTeamAiState.Main.setterBMOKWait_f = setterBMOK_f;
//
//         //セッターがボールマンダッシュするかどうか
//         MyTeamAiState.Main.setterBMRsv_f = (setterBMOK_f && usesetBM_f);
//
//
//         ////セッターがマニュアルでボールマンダッシュ必要なら既にＯＫとする
//         //MyTeamAiState.Main.setterBMRsvGo_f = (st_.pmgMyCh_[setterNo]->IsMAN() && MyTeamAiState.Main.setterBMRsv_f);
//
//         //態勢ウエイト挟まったときは、その分を通常ウエイトに移行
//         if (MyTeamAiState.Main.dashmanFmWait_f)
//         {
//             MyTeamAiState.Main.dmwait_c = MyTeamAiState.Main.dmfmwait_c;
//         }
//
//         //即ＯＫ
//         if (st_.pstMyTm_->CallingDMReset_f)
//         {
//             MyTeamAiState.Main.callOK_f = TRUE;
//         }
//
//     }
//
//
//     //COMダッシュマン作戦人数再確認
//     BOOL TMgTeam::COMDashmanNumRecheck()
//     {
//
//         BOOL res = FALSE;
//         int higher_c = 0;//セッターより上位の人数
//
//         if (MyTeamAiState.Main.setterNo != Defines.NGNUM)//セッターの人がいる場合
//         {
//             for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//             {
//                 if (st_.pstMyTm_->PosMove.DashmanStock[i] == Defines.NGNUM) continue;
//                 //セッターより後衛オーダーが居るとき
//                 if (MyTeamAiState.Main.setterNo > st_.pstMyTm_->PosMove.DashmanStock[i])
//                 {
//                     ++higher_c;
//                 }
//             }
//         }
//
//         //ダッシュマン参加可能人数
//         int dmoknum = st_.pstMyTm_->PosMove.PassAbleNum;//現状の待機人数
//
//         //セッターが内野でセッター参加可能で、セッターがＢＭＤ可能位置にいる場合、一人足す
//         if (MyTeamAiState.Main.setterBMOKWait_f)
//         {
//             ++dmoknum;
//         }
//
//         int dmcallNum = 0;//コール人数
//         int usesetBM_f = FALSE;//セッターをボールマンダッシュさせる
//
//         const int DM_3MEN = 3;//３メン
//         const int DM_2MEN = 2;//２メン
//         const int DM_1MEN = 1;//シングル
//
//
//         //ダッシュマン発動人数
//         switch (MyTeamAiState.Main.comPtn[comDMNum])
//         {
//         case 0://３メン
//             if (dmoknum >= DM_3MEN)//３人以上
//             {
//                 dmcallNum = DM_3MEN;
//
//                 if ((DM_3MEN > higher_c) && MyTeamAiState.Main.setterBMOKWait_f)//上位で３メン足りない
//                 {
//                     usesetBM_f = TRUE;
//                     --dmcallNum;//呼ぶ人減らす
//                 }
//             }
//             break;
//         case 1://マックス
//             if (dmoknum > 0)//呼べる人が誰かいる
//             {
//                 dmcallNum = st_.pstMyTm_->PosMove.PassAbleNum;
//                 usesetBM_f = MyTeamAiState.Main.setterBMOKWait_f;
//             }
//             break;
//         case 2://シングル
//             if (dmoknum >= DM_1MEN)// && (higher_c >= DM_1MEN))//自分より上位が居る１人いる
//             {
//                 dmcallNum = DM_1MEN;
//             }
//             break;
//         default://２メン
//             if (dmoknum >= DM_2MEN)
//             {
//                 dmcallNum = DM_2MEN;
//
//                 if ((DM_2MEN > higher_c) && MyTeamAiState.Main.setterBMOKWait_f)//上位で３メン足りない
//                 {
//                     usesetBM_f = TRUE;
//                     --dmcallNum;//呼ぶ人減らす
//                 }
//             }
//             break;
//         }
//
//
//         if (dmcallNum > 0)
//         {
//             //コール人数
//             MyTeamAiState.Main.dmcallNum = dmcallNum;
//
//             //セッターがボールマンダッシュするかどうか
//             MyTeamAiState.Main.setterBMRsv_f = (MyTeamAiState.Main.setterBMOKWait_f && usesetBM_f);
//
//             //全員さわってないことに
//             for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//             {
//                 if (i == MyTeamAiState.Main.setterNo)
//                 {
//                     MyTeamAiState.Action[i].BallTouched_f = TRUE;
//                 }
//                 else
//                 {
//                     MyTeamAiState.Action[i].BallTouched_f = FALSE;
//                 }
//             }
//
//             //コールＯＫ
//             res = TRUE;
//         }
//         else
//         {
//             st_.pstMyTm_->RsvResetCOM_f = TRUE;
//         }
//
//
//         return res;
//     }
//
//     //セミオート切り替え
//     void TMgTeam::SemiSwitch()
//     {
//         //スマート操作
//         st_.pstMyTm_->smart_f_ = st_.pSidePad_->IsSmart();
//
//         BOOL semi_f = st_.pSidePad_->IsSemiAuto();
//         if (mid::midIsSemiAuto() && st_.pSidePad_->IsSemiAuto())
//         {
//            
//             //セミオート操作
//             st_.pstMyTm_->semi_f_ = semi_f;
//             //セミオートオーダー
//             st_.pstMyTm_->semi_order_f_ = (st_.pstMyTm_->semi_f_ && st_.pSidePad_->IsSemiAutoOrder());
//         }
//         else
//         {
//             //セミオート操作
//             st_.pstMyTm_->semi_f_ = FALSE;
//             //セミオートオーダー
//             st_.pstMyTm_->semi_order_f_ = FALSE;
//         }
//
//
//         
//         //セミオート用キャッチ補正
//         
//         if ((st_.pmgMyTm_->MyTeamState.MainState.ControlOrderIndex >= 0)
//             && (st_.pmgMyTm_->st_.pstEnTm_->CtrlNo >= 0))
//         {
//             if ((st_.pmgBa_->st_.pstBa_->ShTgTNo == st_.mysideNo_)
//                 || (st_.pmgBa_->st_.pstBa_->HoldTNo == st_.ensideNo_))
//             {
//                 TMgChar* pHitman = st_.pmgMyTm_->st_.pmgMyCh_[st_.pmgMyTm_->MyTeamState.MainState.ControlOrderIndex];
//                 int hitman_tech = pHitman->MyStLv(dbst_Tech);
//
//                 int tecrank = 0;
//                 if (st_.pmgBa_->st_.pstBa_->Motion == bmShoot)
//                 {
//                     tecrank = pmgEO_->mgDt_.dtRank_.GetRankTech(st_.pmgBa_->st_.pstSh_->idata.sharp, hitman_tech);
//                 }
//                 else
//                 {
//                     TMgChar* pShotman = st_.pmgMyTm_->st_.pmgMyCh_[st_.pmgMyTm_->st_.pstEnTm_->CtrlNo];
//                     int shotman_tech = pShotman->MyStLv(dbst_Tech);
//                     tecrank = pmgEO_->mgDt_.dtRank_.GetRankTech(shotman_tech, hitman_tech);
//                 }
//                 st_.pstMyTm_->semi_CaPos_ = pmgEO_->mgDt_.dtRank_.GetRankDtTech(rkSemiCatchTime, tecrank);
//                 st_.pstMyTm_->semi_NCaLen_ = pmgEO_->mgDt_.dtRank_.GetRankDtTech(rkSemiNiceCatchTime, tecrank);
//             }
//         }
//     }
//
//     //セミオートキャッチ★★★★
//     void TMgTeam::SemiCatch()
//     {
//
//         //キャッチ中
//         if (st_.pstMyTm_->semi_Catching_c_ > 0)
//         {
//             if (lib_num::AprTo0(&(st_.pstMyTm_->semi_Catching_c_)))
//             {
//                 st_.pstMyTm_->semi_Catch_c_ = 0;
//                 //st_.pstMyTm_->semi_Catch_c_ = 64;//1秒でＯＫか//グラの都合で128の約数に
//             }
//         }
//
//         if (st_.pstMyTm_->semi_Catching_c_ == 0)
//         {
//             //セミオートキャッチカウンター減少
//             lib_num::AprTo0(&(st_.pstMyTm_->semi_Catch_c_));
//
//             if (st_.pstMyTm_->semi_f_ == FALSE) return;
//
//             if ((st_.pstMyTm_->semi_Catch_c_ == 0)
//                 && (st_.pstMyTm_->ofside_f == FALSE)//敵チームボール
//                 && st_.pSidePad_->IsTouch())//タッチ入力（おしっぱst_.pSidePad_->IsBtn2(base::bA)になっていたので瞬間でＯＫかチェック）
//             {
//                 st_.pstMyTm_->semi_Catch_c_ = 64;//1秒でＯＫか//グラの都合で128の約数に
//             }
//         }
//
//     }
//
//     //セミオートシュート★★★★
//     void TMgTeam::SemiShot()
//     {
//         //カウンター減少
//         lib_num::AprTo0(&(st_.pstMyTm_->semi_Shot_c_));
//
//         if (st_.pstMyTm_->ofside_f == TRUE)
//         {
//             if (st_.pSidePad_->IsTouch())//タッチ入力
//             {
//                 st_.pstMyTm_->semi_Shot_f_ = TRUE;
//                 st_.pstMyTm_->semi_Shot_c_ = 32;
//             }
//         }
//         else
//         {
//             st_.pstMyTm_->semi_Shot_f_ = FALSE;
//         }
//     }
//
//     //セミオートＡＩ★★★★
//     void TMgTeam::SemiAI()
//     {
//         if (mid::midIsSemiAuto())
//         {
//             if (st_.pstMyTm_->semi_f_ == FALSE)
//             {
//                 st_.pstMyTm_->semi_AI_ = 0;
//                 //セミオートここでＣＯＭ番号変えられる
//                 //COM番号保存
//                 st_.pstMyTm_->comNo_ = pmgGO_->pmgCh_[st_.mysideNo_][st_.pstMyTm_->RdNo]->MyPDt(clv_COM);
//             }
//             else
//             {
//               st_.pstMyTm_->semi_AI_ = st_.pSidePad_->GetCmdNo();//作戦入力
//               switch (st_.pstMyTm_->semi_AI_)
//               {
//               case 1:st_.pstMyTm_->comNo_ = 22;  break;//37
//               case 2:st_.pstMyTm_->comNo_ = 28;  break;//27
//               case 3:st_.pstMyTm_->comNo_ = 35;  break;//32
//               //case 4:st_.pstMyTm_->comNo_ = 35;  break;//35
//               default:st_.pstMyTm_->comNo_ = 36;  break;
//               }
//             }
//         }
//         else
//         {
//             st_.pstMyTm_->semi_AI_ = 0;
//         }
//     }
//
//     //シュートチェンジ
//     void TMgTeam::Debug_ChangeShot()
//     {
//         if (pmgGO_->pmgRf_->IsPrev()) return;
//         if (mid::midIsDXL())
//         {
//             int mv = 0;
//             int flyLv = 0;
//             int res = 0;
//             if (st_.pSidePad_->IsDXLBtn(dxFnY)) mv += 100;
//             if (st_.pSidePad_->IsDXLBtn(dxFnU)) mv += 10;
//             if (st_.pSidePad_->IsDXLBtn(dxFnI)) mv += 1;
//             if (st_.pSidePad_->IsDXLBtn(dxFnH)) mv -= 100;
//             if (st_.pSidePad_->IsDXLBtn(dxFnJ)) mv -= 10;
//             if (st_.pSidePad_->IsDXLBtn(dxFnK)) mv -= 1;
//             if (st_.pSidePad_->IsDXLBtn(dxFn0)) flyLv += 1;//吹っ飛びレベル
//
//             //ファイル読み込み
//             if (st_.pSidePad_->IsDXLBtn(dxFn9)) pmgEO_->mgDt_.Refresh();
//
//             if (mv != 0)
//             {
//                 //pGame_->mgGO_.pmgBa_->st_.pstSh_->
//
//                 for (int i = 0; i < DBSIDE; ++i)
//                 {
//                     for (int i2 = 0; i2 < DBMEMBER_ALL; ++i2)
//                     {
//                         res = pGame_->mgGO_.pmgCh_[i][i2]->chCommon_.ChangeStShotDebug(mv);
//                     }
//                 }
//                 //        mid::midSetDbgMsg(4, res, "ChangeShot");
//                 //mid::midLog("ChangeShot %d\n", res);
//             }
//             int res2 = 0;
//             if (flyLv != 0)
//             {
//                 pGame_->mgGO_.pmgBa_->st_.pstBa_->debugFlyRankLv += flyLv;
//                 if (pGame_->mgGO_.pmgBa_->st_.pstBa_->debugFlyRankLv >= 16) pGame_->mgGO_.pmgBa_->st_.pstBa_->debugFlyRankLv = 0;
//                 res2 = pGame_->mgGO_.pmgBa_->st_.pstBa_->debugFlyRankLv;
//                 //        mid::midSetDbgMsg(5, res2, "flyLv");
//             }
//         }
//     }
//
// #ifdef __K_DEBUG_SHIAI__
//     //シュートチェンジ
//     void TMgTeam::Debug_SetShot(void)
//     {
//         if (pmgGO_->pmgRf_->IsPrev()) return;
//         if (mid::midIsDXL())
//         {
//             for (int i = 0; i < DBSIDE; ++i)
//             {
//                 for (int i2 = 0; i2 < DBMEMBER_ALL; ++i2)
//                 {
//                     TStChGen* st = &(pGame_->mgGO_.pmgCh_[i][i2]->chCommon_.st_);
//
//                     // ここにデバッグの値を持ってきてshotIdにセットする
//                     int shotId = kdebug::DebugSystem::GetInstance()->GetShootNo();
//                     int maxid = pmgEO_->mgDt_.dtShSyn_.GetIntvec2Size() - 1;
//                     if (shotId > maxid) shotId = maxid;
//                     if (shotId < 0) shotId = 0;
//                     //所持合成シュート（仮）
//                     for (int i = 0; i < STEPNUM; ++i)
//                     {
//                         st->pstMyCh_->stGousei.set_f_[i] = TRUE;
//                         for (int i2 = 0; i2 < HSELEMMAX; ++i2)
//                         {
//                             st->pstMyCh_->stGousei.shot_[i].elem_[i2] = pmgEO_->mgDt_.dtShSyn_.GetDt(shotId, (enDtShSyn)(gs_base + (i2 * 2)));
//                             st->pstMyCh_->stGousei.shot_[i].rare_[i2] = pmgEO_->mgDt_.dtShSyn_.GetDt(shotId, (enDtShSyn)(gs_base_rare + (i2 * 2)));
//                             st->pstMyCh_->stGousei.shot_[i].shotId = shotId;//デバッグ用データ
//                             st->pstMyCh_->stGousei.shot_[i].pow_ = pmgEO_->mgDt_.dtShSyn_.GetDt(shotId, (enDtShSyn)(gs_power));//デバッグ用データ
//                             st->pstMyCh_->stGousei.shot_[i].spin_ = pmgEO_->mgDt_.dtShSyn_.GetDt(shotId, (enDtShSyn)(gs_spin));//デバッグ用データ
//         }
//     }
// }
//       }
//     }
//   }
// #endif	// #ifdef __K_DEBUG_SHIAI__
//
//     //COM思考サイコロふり直し
//     void TMgTeam::COMSwitch()
//     {
//         //試合中のみ
//         if (st_.pstRf_->DBSSt != dbssShiai) return;
//
//         //行動切り替え
//         BOOL switch_f = st_.pstMyTm_->MaruhiCall_f;
//         BOOL nodsmn_f = FALSE;
//
//         st_.pstMyTm_->CallingDMReset_f = FALSE;
//
//         //ボールがもたれているとき
//         if (pmgSG_->stBa_.Motion == bmHold)
//         {
//             //マイボール
//             if (pmgSG_->stBa_.HoldTNo == st_.mysideNo_)
//             {
//
//                 if (st_.pstMyTm_->noside_f)
//                 {
//                     st_.pstMyTm_->noside_f = FALSE;
//                     switch_f = TRUE;
//                 }
//
//                 if (st_.pstMyTm_->ofside_f == FALSE)
//                 {
//                     switch_f = TRUE;
//                     st_.pstMyTm_->ofside_f = TRUE;
//                     st_.pstMyTm_->COMCall.Init();
//                 }
//             }
//             else//敵ボール
//             {
//                 if (st_.pstMyTm_->noside_f)
//                 {
//                     st_.pstMyTm_->noside_f = FALSE;
//                     switch_f = TRUE;
//                 }
//
//                 if (st_.pstMyTm_->ofside_f)
//                 {
//                     switch_f = TRUE;
//                     st_.pstMyTm_->ofside_f = FALSE;
//                     st_.pstMyTm_->COMCall.Init();
//                 }
//             }
//             st_.pstMyTm_->randreset_f = TRUE;
//         }
//         else
//         {
//             if ((pmgSG_->stBa_.Motion == bmBound)
//                 || (pmgSG_->stBa_.Motion == bmFree)
//                 || (pmgSG_->stBa_.Motion == bmReferee)
//                 || (pmgSG_->stBa_.Motion == bmShoot))
//             {
//                 st_.pstMyTm_->noside_f = TRUE;
//             }
//             else if ((pmgSG_->stBa_.Motion == bmPass) && (pmgSG_->stBa_.PaTgTNo != st_.mysideNo_))
//             {
//                 if (pmgSG_->stBa_.PaTgPNo >= Defines.DBMEMBER_INF)//タゲが外野
//                 {
//                     if (pmgSG_->stBa_.PichPNo < Defines.DBMEMBER_INF)//投手が内野
//                     {
//                         st_.pstMyTm_->noside_f = TRUE;
//                     }
//                 }
//                 else
//                 {
//                     if (pmgSG_->stBa_.PichPNo >= Defines.DBMEMBER_INF)//投手が外野
//                     {
//                         st_.pstMyTm_->noside_f = TRUE;
//                     }
//                 }
//             }
//
//             if (st_.pstMyTm_->randreset_f)
//             {
//                 //通信対戦の時は呼ばないようにしてみる
//                 if (pmgEO_->stShiai_.IsVSHum() == FALSE)
//                 {
//                     const int DGTOCAPER = 50;
//                     //st_.pstMyTm_->COMRnd.BallTake_f = pmgGO_->pmgCommon_->GetRandPer(MyTeamAiState.Main.BallTakePer);//COMボール拾い行動確率
//                     st_.pstMyTm_->COMRnd.DgToCa_f = pmgGO_->pmgCommon_->GetRandPer(DGTOCAPER);//回避不能→キャッチシフト
//
//                     for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//                     {
//                         //一部サイコロふり直し
//                         MyTeamAiState.Action[i].comactPtn[cmaPassCut] //パスカット
//                             = GetComAct(st_.pstMyTm_->comNo_, cmaPassCut, MyTeamAiState.Main.Ressei_f, st_.pmgMyCh_[i]->st_.pstMyCh_->id_char);
//                         MyTeamAiState.Action[i].comactPtn[cmaCatchJdg] //キャッチ対応
//                             = GetComAct(st_.pstMyTm_->comNo_, cmaCatchJdg, MyTeamAiState.Main.Ressei_f, st_.pmgMyCh_[i]->st_.pstMyCh_->id_char);
//
//                         st_.pstMyTm_->COMRnd.RndAct[i].DgType = (enDodgeType)GetComAct(st_.pstMyTm_->comNo_, cmaDgType, MyTeamAiState.Main.Ressei_f, st_.pmgMyCh_[i]->st_.pstMyCh_->id_char);
//                         st_.pstMyTm_->COMRnd.RndAct[i].DgTime = pmgGO_->pmgCommon_->GetRand(st_.pmgMyCh_[i]->RankLevel(rkDodgeTime));
//                     }
//                 }
//                 st_.pstMyTm_->randreset_f = FALSE;
//             }
//         }
//
//         //どちらサイドでもなくなったら丸秘開始はオフ
//         if (st_.pstMyTm_->noside_f
//             || (st_.pstMyTm_->ofside_f == FALSE))
//         {
//             st_.pstMyTm_->CallingDashman_f2 = FALSE;
//         }
//
//         //Callingによる作戦変更
//         if ((switch_f == FALSE)
//             && st_.pstMyTm_->CallingDashman_f
//             //&& (st_.pstMyTm_->CallingDashman_f2 == FALSE)
//             )
//         {
//             st_.pstMyTm_->CallingDashman_f = FALSE;
//             st_.pstMyTm_->CallingDashman_f2 = TRUE;
//             st_.pstMyTm_->CallingDashman_f3 = FALSE;
//             //ダッシュマン作戦ではないばあい
//             if (MyTeamAiState.Main.dashman_f == FALSE)
//             {
//                 st_.pstMyTm_->CallingDMReset_f = TRUE;
//             }
//             else//ダッシュマンの時はウエイト削るか
//             {
//                 //まだコールＯＫが出てなければ短縮してまるひ表示
//                 if (MyTeamAiState.Main.callOK_f == FALSE)
//                 {
//                     //COMDM通常ウエイト
//                     MyTeamAiState.Main.DMWait = 1;
//                     //COMDMセッターウエイト
//                     MyTeamAiState.Main.DMStWait = 1;
//                     //セッターを自分に
//                     MyTeamAiState.Main.setterNo = GetBallmanNo();
//
//                     ////まるひさくせん
//                     //SESet(seDmOK);
//                     //TZahyouRec* pzh = &(st_.pmgMyCh_[GetBallmanNo()]->st_.pstMyCh_->Zahyou);
//                     //PopupMsg(pu_Call, pmgEO_->mgDt_.dtScenario_.GetDBShiaiStrDt(mndbMaruhi).c_str(),
//                     //  st_.mysideNo_, pzh->X, pzh->Y, pzh->Z, FC_Red);
//                 }
//                 else
//                 {
//                     //SESet(seDmNG);//ＮＧ音
//                     //mid::midLog("ng5\n");
//                 }
//             }
//         }
//
//         ////ボールマンをリーダーに
//         //if (pmgEO_->mgDt_.dtTeam_.GetDt2(dbtm_OpeFomChange, st_.teamNo_) == 3)
//         //{
//         //  if (st_.pstMyTm_->ofside_f && (st_.pstMyTm_->LastBallmanNo != GetBallmanNo()))
//         //  {
//         //    int tmprdno = st_.pstMyTm_->RdNo;
//         //    //ダッシュマンが居ないとき
//         //    if ((st_.pstMyTm_->PosMove.DashmanNum == 0)
//         //        && (st_.pstMyTm_->MaruhiCall_f == FALSE))
//         //    {
//         //      if (GetBallmanNo() != Defines.NGNUM)
//         //      {
//         //        st_.pstMyTm_->RdNo = st_.pstMyTm_->PosSt.CharofP[GetBallmanNo()];
//         //      }
//         //    }
//         //    if (tmprdno != st_.pstMyTm_->RdNo) PopupFomChange();
//         //    st_.pstMyTm_->LastBallmanNo = GetBallmanNo();
//         //  }
//         //}
//
//
//         //サイコロふり直し
//         if (switch_f
//             || st_.pstMyTm_->RsvResetCOM_f
//             || st_.pstMyTm_->CallingDMReset_f
//             || st_.pstMyTm_->CallingRdCg_f)
//         {
//
//             //オフェンスの時にリーダーチェンジ処理
//             if (st_.pstMyTm_->ofside_f
//                 && switch_f
//                 && (st_.pstMyTm_->CallingRdCg_f == FALSE))
//             {
//                 ReaderChange();
//             }
//
//             //まず空に
//             MyTeamAiState.Main.Init();
//
//             //乱数取得
//             for (int i = 0; i < comEND; ++i)
//             {
//                 MyTeamAiState.Main.comPtn[i] = GetCom((enCom)i);
//             }
//
//             MyTeamAiState.Main.Ressei_f2 = IsRessei();//実は劣勢
//             BOOL ec_f = (st_.pstMyTm_->Encr_c > 0);
//
//             if (MyTeamAiState.Main.Ressei_f2)// && (ec_f == FALSE))
//             {
//                 MyTeamAiState.Main.FomRev = +1;//強制的に+1
//             }
//             if (ec_f)
//             {
//                 MyTeamAiState.Main.FomRev = -1;//強制的に-1
//             }
//
//             //最終的な劣勢判断
//             MyTeamAiState.Main.Ressei_f = (ec_f)
//                 ? FALSE
//                 : MyTeamAiState.Main.Ressei_f2;
//
//
//             //乱数取得
//             for (int i2 = 0; i2 < DBMEMBER_ALL; ++i2)
//             {
//
//                 //MyTeamAiState.Action[i2].CallPaOK_f = GetCallPaOK(st_.pstMyTm_->comNo_);
//                 ////個人性格によって、パスしない場合
//                 //MyTeamAiState.Action[i2].CallPaOK_f = (pmgGO_->pmgCommon_->GetRandPer(pmgEO_->mgDt_.dtChar_.GetCoopStop(st_.pmgMyCh_[i2]->st_.id_)) == FALSE);
//                 //
//                 //MyTeamAiState.Action[i2].comactNo = GetComActNo(i2);
//                 for (int i = 0; i < cmaEND; ++i)
//                 {
//                     MyTeamAiState.Action[i2].comactPtn[i]
//                         = GetComAct(st_.pstMyTm_->comNo_, (enDtComAct)i, MyTeamAiState.Main.Ressei_f, st_.pmgMyCh_[i2]->st_.pstMyCh_->id_char);
//                 }
//
//             }
//
//             //チャンス判断
//             ChanceCheck();
//
//             //AI予測シュート
//             int dai = pmgSG_->stBa_.AIDShDt.GetExp();
//             int jai = pmgSG_->stBa_.AIJShDt.GetExp();
//             int rai = pmgGO_->pmgCommon_->GetRand(AISHTYPENUM);
//
//             for (int i2 = 0; i2 < DBMEMBER_ALL; ++i2)
//             {
//                 MyTeamAiState.Action[i2].comactPtn[cmaDShStep] = (dai == Defines.NGNUM)
//                     ? rai
//                     : dai;
//                 MyTeamAiState.Action[i2].comactPtn[cmaJShTime] = (jai == Defines.NGNUM)
//                     ? rai
//                     : jai;
//             }
//
//             //カウンターキャッチ成功
//             if (MyTeamState.MainState.ControlOrderIndex != OrderIndexType.Disabled)
//             {
//                 MyTeamAiState.Main.CCaSc_f = st_.pmgMyCh_[MyTeamState.MainState.ControlOrderIndex]->st_.pstMyCh_->COMCounter_f;
//             }
//             else
//             {
//                 MyTeamAiState.Main.CCaSc_f = FALSE;
//             }
//
//             //COMDM通常ウエイト
//             MyTeamAiState.Main.DMWait = (st_.pstMyTm_->CallingDMReset_f)
//                 ? 1
//                 : pmgEO_->mgDt_.dtFomation_.GetComDt(cmvDMWait, MyTeamAiState.Main.comPtn[comDMWait]);
//             //COMDM態勢ウエイト
//             MyTeamAiState.Main.DMFmWait = pmgEO_->mgDt_.dtFomation_.GetComDt(cmvDMFmWait, MyTeamAiState.Main.comPtn[comDMWait]);
//             //COMDMセッターウエイト
//             MyTeamAiState.Main.DMStWait = (st_.pstMyTm_->CallingDMReset_f)
//                 ? 1
//                 : pmgEO_->mgDt_.dtFomation_.GetComDt(cmvDMStWait, MyTeamAiState.Main.comPtn[comDMTrigger]);
//
//             //COMDMコールディレイ
//             MyTeamAiState.Main.DMDelay = pmgEO_->mgDt_.dtFomation_.GetComDt(cmvDMDelay, MyTeamAiState.Main.comPtn[comDMDelay]);
//             //COMDMパスインターバル
//             MyTeamAiState.Main.DMPaItv = pmgEO_->mgDt_.dtFomation_.GetComDt(cmvDMPaItv, MyTeamAiState.Main.comPtn[comDMPaItv]);
//             //COMチャンスメイクパスループ
//             MyTeamAiState.Main.ChMakeLoop = pmgEO_->mgDt_.dtFomation_.GetComDt(cmvChMakeLoop, MyTeamAiState.Main.comPtn[comChMakeLoop]);
//
//
//
//
//             //個人アクション
//             for (int i = 0; i < DBMEMBER_ALL; ++i)
//             {
//                 //パス回数
//                 MyTeamAiState.Action[i].SgOdPa_f = (MyTeamAiState.Action[i].comactPtn[cmaAction] >= 2);
//
//                 //シュート変更パス回数（行動後変更するため）
//                 MyTeamAiState.Action[i].ShCg = (st_.pstMyTm_->RsvResetCOM_f == FALSE)
//                     ? (enShChangeType)(MyTeamAiState.Action[i].comactPtn[cmaShChange])
//                     : scNone;
//
//                 //COMシングルパスウエイト
//                 MyTeamAiState.Action[i].PaWait
//                     = pmgEO_->mgDt_.dtFomation_.GetComDt(cmvSgPaWait, MyTeamAiState.Action[i].comactPtn[cmaPaWait]);
//
//                 //COMDShウエイト
//                 MyTeamAiState.Action[i].ShWait
//                     = pmgEO_->mgDt_.dtFomation_.GetComDt(cmvShWait, MyTeamAiState.Action[i].comactPtn[cmaShWait]);
//
//                 //COMDSh歩数
//                 MyTeamAiState.Action[i].DShStep
//                     = pmgEO_->mgDt_.dtFomation_.GetComDt(cmvDShStep, MyTeamAiState.Action[i].comactPtn[cmaDShStep]);
//
//                 //COMJShタイミング
//                 MyTeamAiState.Action[i].JShTime
//                     = pmgEO_->mgDt_.dtFomation_.GetComDt(cmvJShTime, MyTeamAiState.Action[i].comactPtn[cmaJShTime]);
//
//                 //COMJShタイミング2
//                 MyTeamAiState.Action[i].JShTime2
//                     = pmgEO_->mgDt_.dtFomation_.GetComDt(cmvJShTime2, MyTeamAiState.Action[i].comactPtn[cmaJShTime]);
//
//                 //COMDs開始位置
//                 MyTeamAiState.Action[i].DsPos
//                     = (DBCRT_CLI - (pmgEO_->mgDt_.dtFomation_.GetComDt(cmvDsPos, MyTeamAiState.Action[i].comactPtn[cmaDsPos]) * XYMAG));
//
//                 if (st_.pstMyTm_->semi_f_ && st_.pstMyTm_->semi_order_f_)
//                 {
//                     MyTeamAiState.Action[i].JShTime = 32;
//                     MyTeamAiState.Action[i].JShTime2 = 32;
//                 }
//             }
//
//
//             //COMパス元ダッシュ継続
//             MyTeamAiState.Main.DsKeep_f = (MyTeamAiState.Main.comPtn[comDMPaNum] == 1)//パス無限
//                 && ((MyTeamAiState.Main.comPtn[comDMNum] == 0) || (MyTeamAiState.Main.comPtn[comDMNum] == 3));//３メンと２メン
//             ////COMパス元ダッシュ継続
//             //MyTeamAiState.Main.DsKeep_f = ((MyTeamAiState.Main.comPtn[comDMNum] == 0) || (MyTeamAiState.Main.comPtn[comDMNum] == 3));//３メンと２メン
//
//             //COMインターバル一つ待つ
//             MyTeamAiState.Main.DsOneItvWait_f = (MyTeamAiState.Main.comPtn[comDMNum] == 2);//シングルの時はインターバル一つ待つ
//
//             //ヘルプガード
//             MyTeamAiState.Main.help_f = (st_.pstMyTm_->ofside_f == FALSE);//ディフェンスで
//
//             //チャンスメイク回数
//             MyTeamAiState.Main.chpass_c = MyTeamAiState.Main.ChMakeLoop;
//
//
//             //リセットしたときはクイック行動
//             MyTeamAiState.Main.Quick_f = st_.pstMyTm_->RsvResetCOM_f;
//
//             //ダッシュマン作戦(リーダーチェンジとリセットとジャンプボールゲットとカウンターキャッチ成功以外)
//             if (st_.pstMyTm_->CallingRdCg_f)
//             {
//                 MyTeamAiState.Main.dashman_f = FALSE;
//                 MyTeamAiState.Main.dashmanFmWait_f = FALSE;
//             }
//             else if (st_.pstMyTm_->CallingDMReset_f)
//             {
//                 COMDashman();
//             }
//             else if ((MyTeamAiState.Main.Chance_f == FALSE)
//                 //&& (MyTeamAiState.Main.ncChance_f == FALSE)
//                 && (st_.pstMyTm_->RsvResetCOM_f == FALSE)
//                 && (st_.pstMyTm_->getJpball_f == FALSE)
//                 && (MyTeamAiState.Main.CCaSc_f == FALSE))
//             {
//                 COMDashman();
//             }
//             //else
//             //{
//             //作戦リセット時★
//
//             MyTeamAiState.Main.comReset_f = st_.pstMyTm_->RsvResetCOM_f;
//             st_.pstMyTm_->RsvResetCOM_f = FALSE;
//             st_.pstMyTm_->getJpball_f = FALSE;
//             //}
//
//             st_.pstMyTm_->COMOverTime_f = FALSE;
//         }
//
//     }
//
//     //COMカウンタ操作
//     void TMgTeam::COMCounter()
//     {
//         const int RESETTIME = 240;
//         const int SHOTTIME = 240;
//
//         //投げ終わりカウンタを減らす
//         lib_num::AprTo0(&MyTeamAiState.Main.shAfter_c);
//
//         //エンカレカウンタを減らす
//         lib_num::AprTo0(&st_.pstMyTm_->Encr_c);
//
//         lib_num::AprTo0(&st_.pstMyTm_->TutoWait_c);
//
//
//         //COMのみ
//         if (IsCOM() == FALSE) return;
//
//         if (st_.pmgRf_->IsReturnEnSide())
//         {
//             if (st_.pstBa_->Motion != bmHold)
//             {
//                 st_.pstMyTm_->TutoWait_c = 30;
//             }
//
//             if (st_.pstMyTm_->TutoWait_c == 0)
//             {
//                 //どっかおかしくなって、6秒かかったら強制シュート
//                 MyTeamAiState.Main.comatcSt_f = TRUE;//行動開始
//                 st_.pstMyTm_->COMOverTime_f = TRUE;
//             }
//             return;
//         }
//
//         if (MyTeamAiState.Main.comatcSt_f == FALSE)
//         {
//             if (pmgSG_->stBa_.Motion == bmHold)//ボールがもたれているとき
//             {
//                 //マイボール
//                 if (pmgSG_->stBa_.HoldTNo == st_.mysideNo_)
//                 {
//                     //持っている人が戻り中（外野強奪）の場合はまだマイボール扱いしない
//                     if (st_.pmgMyCh_[pmgSG_->stBa_.HoldPNo]->IsFreeMotion(TRUE))
//                     {
//                         MyTeamAiState.Main.comatcSt_f = TRUE;//行動開始
//                     }
//                 }
//             }
//         }
//
//
//         //行動開始してない
//         if (MyTeamAiState.Main.comatcSt_f == FALSE) return;
//
//         //ダッシュマン以外
//         if (MyTeamAiState.Main.Isdm() == FALSE)//態勢待ちでもない
//         {
//             //ＣＯＭ行動カウンタ
//             lib_num::IncLoop(&MyTeamAiState.Main.TmwaitC, 0, 0xFFFF);
//             //MyTeamAiState.Main.dmstop_f = FALSE;
//
//             if (MyTeamAiState.Main.TmwaitC > SHOTTIME)
//             {
//                 //どっかおかしくなって、6秒かかったら強制シュート
//                 st_.pstMyTm_->COMOverTime_f = TRUE;
//             }
//         }
//         else//ダッシュマン作戦中（待ち含む）
//         {
//
//             //ダッシュマン態勢ウエイト中
//             if ((MyTeamAiState.Main.dashman_f == FALSE)
//                 && (MyTeamAiState.Main.dashmanFmWait_f))
//             {
//                 if (MyTeamAiState.Main.dmfmwait_c <= MyTeamAiState.Main.DMFmWait)
//                 {
//
//                     //ダッシュマン作戦を常に確認
//                     COMDashman();
//
//                     //結果的にダッシュマン作戦にならなかった
//                     if (MyTeamAiState.Main.dashman_f == FALSE)
//                     {
//                         //ダッシュマン態勢ウエイト進行
//                         if (lib_num::AprToDest(&MyTeamAiState.Main.dmfmwait_c, MyTeamAiState.Main.DMFmWait))
//                         {
//                             MyTeamAiState.Main.dashmanFmWait_f = FALSE;//待ちでもなくなる
//                             MyTeamAiState.Main.TmwaitC = MyTeamAiState.Main.dmfmwait_c;//行動カウンタに態勢待ち時間をシフト
//                             MyTeamAiState.Main.dmstop_f = TRUE;
//                         }
//                     }
//
//                 }
//             }
//
//
//             //ダッシュマン作戦決定
//             if (MyTeamAiState.Main.dashman_f)
//             {
//                 //ダッシュマン通常ウエイト
//                 if (MyTeamAiState.Main.dmwaitOK_f == FALSE)
//                 {
//                     //ダッシュマン通常ウエイト
//                     if (lib_num::AprToDest(&MyTeamAiState.Main.dmwait_c, MyTeamAiState.Main.DMWait))
//                     {
//                         MyTeamAiState.Main.dmwaitOK_f = TRUE;
//
//                         //セッターウエイトを通常ウエイトから移行
//                         if ((MyTeamAiState.Main.comPtn[comDMTrigger] == 0)
//                             || (MyTeamAiState.Main.comPtn[comDMTrigger] == 1))
//                         {
//                             MyTeamAiState.Main.dmstwait_c = MyTeamAiState.Main.dmwait_c;
//                         }
//                     }
//                 }
//                 else
//                 {
//                     //既にセッターの手元かどうか
//                     if (MyTeamAiState.Main.setterOK_f == FALSE)
//                     {
//                         if (MyTeamAiState.Main.setterNo == GetBallmanNo())
//                         {
//                             MyTeamAiState.Main.setterOK_f = TRUE;
//                         }
//                         //セッターへのパスチェックでセッターＯＫ出すのもここでやる
//                     }
//
//                     //セッターＯＫ
//                     if (MyTeamAiState.Main.setterOK_f && (MyTeamAiState.Main.callOK_f == FALSE))
//                     {
//                         lib_num::AprToDest(&MyTeamAiState.Main.dmstwait_c, MyTeamAiState.Main.DMStWait);
//                         //セッターウエイトがＯＫ
//                         if (MyTeamAiState.Main.dmstwait_c >= MyTeamAiState.Main.DMStWait)
//                         {
//                             //ダッシュマン現状走れる人数を再チェック
//                             MyTeamAiState.Main.callOK_f = COMDashmanNumRecheck();
//                         }
//                     }
//
//                     //コールＯＫ
//                     if (MyTeamAiState.Main.callOK_f)
//                     {
//                         //コールカウンタ
//                         lib_num::IncLoop(&MyTeamAiState.Main.dmclwait_c, 0, 0xFFFF);
//                         //パスインターバルカウンタ
//                         lib_num::IncLoop(&MyTeamAiState.Main.dmpawait_c, 0, 0xFFFF);
//                     }
//
//                     //呼んだ人がいるのにダッシュマンが居ない場合はリセット
//                     if ((MyTeamAiState.Main.dmcalledNum > 0)
//                         && (st_.pstMyTm_->PosMove.DashmanNum == 0))
//                     {
//                         st_.pstMyTm_->RsvResetCOM_f = TRUE;//沼尾かが止まる★
//                     }
//
//                     //ダッシュマンパスしない&(呼ぶ人呼んだらor呼べないとき合ったら)ダッシュマン作戦終了
//                     if ((MyTeamAiState.Main.comPtn[comDMPaNum] == 2)
//                         && ((MyTeamAiState.Main.dmcalledNum >= MyTeamAiState.Main.dmcallNum)
//                         || MyTeamAiState.Main.callNG_f))
//                     {
//                         st_.pstMyTm_->RsvResetCOM_f = TRUE;
//                     }
//
//
//                     if (lib_num::AprToDest(&MyTeamAiState.Main.dmall_c, RESETTIME))
//                     {
//                         //どっかおかしくなって、4秒かかったらＣＯＭ思考リセット
//                         st_.pstMyTm_->RsvResetCOM_f = TRUE;
//                     }
//                 }
//             }
//         }
//     }
//
//     //劣勢判断
//     BOOL TMgTeam::IsRessei()
//     {
//         BOOL res = FALSE;
//         switch (MyTeamAiState.Main.comPtn[comRessei])
//         {
//         case 0://倒され人数で負けてる
//             res = (st_.pmgEnTm_->InfNum() < InfNum());//こちらの方が倒された人数が多い
//             break;
//         case 1://瀕死が居る
//             res = (PinchNum() > 0);
//             break;
//         case 2://ボスが完全退場している→天使で良いのでは
//             //res = pmgGO_->pmgCh_[st_.mysideNo_][0]->st_.pstMyCh_->Dead_f;//ボスが居ない
//             res = (pmgGO_->pmgCh_[st_.mysideNo_][st_.pstMyTm_->RdNo]->st_.pstMyCh_->Dead_f)
//                 || (pmgGO_->pmgCh_[st_.mysideNo_][st_.pstMyTm_->RdNo]->st_.pstMyCh_->ANGEL_f);//ボスが居ない
//             break;
//         default://内野が残り一人
//             res = (IsLastOne());//最後の一人状態
//             break;
//         }
//
//         return res;
//     }
//
//     //チャンスパス判断
//     BOOL TMgTeam::IsChancePass()
//     {
//         BOOL res = FALSE;
//         //チャンスパス判断
//         switch (MyTeamAiState.Main.comPtn[comChancePa])
//         {
//         case 0://劣勢ならする
//             res = MyTeamAiState.Main.Ressei_f;
//             break;
//         case 1://チャンスチェックする
//             res = TRUE;
//             break;
//         case 2://チャンスチェックしない
//             res = FALSE;
//             break;
//         default://劣勢ならしない
//             res = !(MyTeamAiState.Main.Ressei_f);
//             break;
//         }
//
//         return res;
//     }
//
//     //チャンスターゲット
//     int TMgTeam::ChanceTag()
//     {
//         int res = Defines.NGNUM;
//         //チャンスチェックする場合、チャンス狙い
//
//         for (int i = 0; i < Defines.DBMEMBER_INF; ++i)//上から検索かけていく
//         {
//             //ねらえない
//             if (st_.pmgEnTm_->MySideOrders[i].Composite.IsEnableShootTarget == FALSE) continue;
//
//             if ((st_.pmgEnTm_->st_.pmgMyCh_[i]->Kagami_f())//かがみ
//                 || (st_.pmgEnTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Motion.IsMFlags(dbmfDn)))//ダウン系
//             {
//                 res = i;//チャンスタゲ
//                 break;
//             }
//         }
//
//         return res;
//     }
//
//     //チャンス判断
//     void TMgTeam::ChanceCheck()
//     {
//         MyTeamAiState.Main.ngTagNo = Defines.NGNUM;
//         MyTeamAiState.Main.vnTagNo = Defines.NGNUM;
//         //操作権あるとき
//         if (MyTeamState.MainState.ControlOrderIndex == Defines.NGNUM) return;
//
//         int posno = MyTeamState.MainState.ControlOrderIndex;
//         TMgChar* pobj = st_.pmgMyCh_[posno];
//         TStChar* pst = pobj->st_.pstMyCh_;
//
//         BOOL nca_f = FALSE;// (pst->NiceCounter_c > 0);
//
//         //ガード不能検索
//         for (int i = 0; i < Defines.DBMEMBER_INF; ++i)//上から検索かけていく
//         {
//             //ねらえない
//             if (st_.pmgEnTm_->MySideOrders[i].Composite.IsEnableShootTarget == FALSE) continue;
//
//             if (st_.pmgEnTm_->st_.pmgMyCh_[i]->IsNoGuard(FALSE))//ノーガード
//             {
//                 MyTeamAiState.Main.ngTagNo = i;//ガード不能敵タゲ
//                 MyTeamAiState.Main.ngChance_f = TRUE;
//                 break;
//             }
//         }
//
//
//         //非常に近い人検索
//         for (int i = 0; i < Defines.DBMEMBER_INF; ++i)//上から検索かけていく
//         {
//             //ねらえない
//             if (st_.pmgEnTm_->MySideOrders[i].Composite.IsEnableShootTarget == FALSE) continue;
//             //非常に近い
//             const int VERYNEAR = 32 * XYMAG;
//             if ((VERYNEAR > abs(st_.pmgEnTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.X - pst->Zahyou.X))
//                 && (VERYNEAR > abs(st_.pmgEnTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z - pst->Zahyou.Z)))
//             {
//                 MyTeamAiState.Main.vnTagNo = i;//非常に近い敵タゲ
//                 MyTeamAiState.Main.vnChance_f = TRUE;
//                 break;
//             }
//         }
//
//
//
//         //チャンスパス＆ノーガードがいる
//         if (IsChancePass() && MyTeamAiState.Main.ngChance_f) MyTeamAiState.Main.Chance_f = TRUE;
//
//         //チャンスシュート//キャッチ不能ならする	する	しない	キャッチ不能ならしない//★
//         switch (MyTeamAiState.Action[posno].comactPtn[cmaChanceSh])
//         {
//         case 0://キャッチ不能ならする
//             if (MyTeamAiState.Main.ngChance_f)
//             {
//                 MyTeamAiState.Main.Chance_f = TRUE;
//             }
//             break;
//         case 1://する
//             if (MyTeamAiState.Main.ngChance_f || MyTeamAiState.Main.vnChance_f)
//             {
//                 MyTeamAiState.Main.Chance_f = TRUE;
//             }
//             break;
//         case 2://しない
//             //st_.pstMyTm_->Chance_f = FALSE;
//             break;
//         default://キャッチ不能ならしない(近い場合のみ)
//             if (MyTeamAiState.Main.vnChance_f && (MyTeamAiState.Main.ngChance_f == FALSE))
//             {
//                 MyTeamAiState.Main.Chance_f = TRUE;
//             }
//             break;
//         }
//
//     }
//
//     //待ち時間超えてる
//     BOOL TMgTeam::IsWaitOver()
//     {
//         BOOL res = FALSE;
//         if (st_.pmgEnTm_->IsAllNoShTg())//誰もねらえない
//         {
//             res = FALSE;
//         }
//         else if (MyTeamAiState.Main.quick_f)//クイック
//         {
//             res = TRUE;
//         }
//         else if (MyTeamAiState.Main.sh_f)//シュート
//         {
//             if (MyTeamState.MainState.ControlOrderIndex != OrderIndexType.Disabled)
//             {
//                 res = (MyTeamAiState.Main.TmwaitC > MyTeamAiState.Action[MyTeamState.MainState.ControlOrderIndex].ShWait);
//             }
//             else
//             {
//                 res = (MyTeamAiState.Main.TmwaitC > MyTeamAiState.Action[st_.pstMyTm_->GetRdPos()].ShWait);
//             }
//
//         }
//         else//パス
//         {
//             if (MyTeamState.MainState.ControlOrderIndex != OrderIndexType.Disabled)
//             {
//                 res = (MyTeamAiState.Main.TmwaitC > MyTeamAiState.Action[MyTeamState.MainState.ControlOrderIndex].PaWait);
//             }
//             else
//             {
//                 res = (MyTeamAiState.Main.TmwaitC > MyTeamAiState.Action[st_.pstMyTm_->GetRdPos()].PaWait);
//             }
//         }
//
//         return res;
//     }
//
//     //ダッシュマンパスタイミング
//     BOOL TMgTeam::IsDMPaItvTime(BOOL infsetter_f)
//     {
//         //全員呼ぶのが同時でないため、セッターで呼び切れてないときは若干補正
//         if (infsetter_f && (MyTeamAiState.Main.dmcalledNum < MyTeamAiState.Main.dmcallNum))
//         {
//             //パスインターバル後
//             return (MyTeamAiState.Main.dmpawait_c > MyTeamAiState.Main.DMPaItv + (15));
//         }
//         else
//         {
//             //パスインターバル後
//             return (MyTeamAiState.Main.dmpawait_c > MyTeamAiState.Main.DMPaItv);
//         }
//
//
//     }
//
//     //ダッシュマン発動タイミング
//     BOOL TMgTeam::IsDMDelayTime()
//     {
//         //０だと初期化時点でそうなので１にする
//         if (MyTeamAiState.Main.DMDelay == 0)
//         {
//             return FALSE;
//         }
//         else
//         {
//             return ((MyTeamAiState.Main.dmclwait_c % MyTeamAiState.Main.DMDelay) == 1);
//         }
//     }
//
//     //動作
//     void TMgTeam::TeamMove()
//     {
//         //if (st_.mysideNo_ == 0)
//         //{
//         //  st_.pSidePad_->ppad_->Nowdxpad();
//         //  mid::midSetDbgMsg(2, (int)st_.pSidePad_->ppad_->Nowdxpad(), "tmpad");
//         //}
//
//         //操作権キャラ
//         NowCtrlNo();
//
//         //ポジショニング予定位置計算ポジショニングカウンタ減少
//         GetEnC_c();
//
//         //ポジショニング
//         SetPosLine();
//
//         //ダッシュマンストック
//         SetDashmanStock();
//
//         //ボールの位置
//         TeamBallPos();
//         TeamBallPos2();
//
//         //ボール拾いマン検索
//         SeekGetter();
//
//         //セミオート
//         SemiSwitch();
//         //セミオートキャッチ
//         SemiCatch();
//         //セミオートシュート
//         SemiShot();
//         //セミオートＡＩ切り替え
//         SemiAI();
//
//         //デバッグ用シュート切り替え
//         Debug_ChangeShot();
//
//         //COM思考サイコロふり直し（ポストマンが決まってる必要があるのでポジショニングの後）
//         COMSwitch();
//
//         //COMのみカウンタ操作
//         COMCounter();
//
//         //ダッシュマン指令
//         OrderDashman();
//     }
//
//     //試合中の名前
//     const int SIDEDIST = 136;
//     const int BASEX = -184;
//     const int BASEY = -104;
//     const int FACEX = BASEX + 72;
//     const int NAMEX = FACEX + 11;
//     const int FACEY = BASEY + 8;
//     const int NAMEY = FACEY - 8;
//     //const int HPX = NAMEX + 48;// FACEX + 56;
//     //const int HPY = NAMEY + 10;// FACEY - 7;
//     //const int STEPX = NAMEX;
//     //const int STEPY = FACEY + 2;
//     const int HPX = NAMEX;// FACEX + 56;
//     const int HPY = FACEY + 9;
//     const int STEPX = NAMEX + 48;
//     const int STEPY = FACEY + -6;
//
//     void TMgTeam::HPDraw(int drawcno)
//     {
//         //for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//         //{
//         //  int id = pmgGO_->pmgCh_[st_.mysideNo_][i]->st_.pstMyCh_->id_char;
//
//         //  //天使補正を掛けない
//         //  int hp = pmgEO_->mgDt_.GetState(dbst_Pow, st_.pmgMyCh_[i]->st_.pstMyCh_->id_char);//0~15
//
//         //  //ＨＰバーｐ
//         //  pmgEO_->mgGrp_.pmgEtcGrp_->GraphDrawHP(
//         //    HPX + (SIDEDIST * st_.mysideNo_),
//         //    HPY + (i * 3),
//         //    st_.pmgMyCh_[i]->st_.pstMyCh_->HP,
//         //    pmgEO_->mgDt_.dtSet_.GetDtDamage(setHP),
//         //    st_.pmgMyCh_[i]->IsCtrl());
//         //}
//
//
//         if (drawcno != Defines.NGNUM)
//         {
//             int id = pmgGO_->pmgCh_[st_.mysideNo_][drawcno]->st_.pstMyCh_->id_char;
//             //天使補正を掛けない
//             int hp = pmgEO_->mgDt_.GetState(dbst_Pow, st_.pmgMyCh_[drawcno]->st_.pstMyCh_->id_char);//0~15
//
//             //ＨＰバーｐ
//             pmgEO_->mgGrp_.pmgEtcGrp_->GraphDrawHP(
//                 HPX + (SIDEDIST * st_.mysideNo_),
//                 HPY,
//                 st_.pmgMyCh_[drawcno]->st_.pstMyCh_->HP,
//                 pmgEO_->mgDt_.dtSet_.GetDtDamage(setHP),
//                 FALSE);
//         }
//
//         //for (int i = 0; i < Defines.DBMEMBER_INF; ++i)
//         //{
//
//         //  if (st_.pmgMyCh_[i]->IsCtrl() == FALSE) continue;
//
//         //  int id = pmgGO_->pmgCh_[st_.mysideNo_][i]->st_.pstMyCh_->id_char;
//         //  //天使補正を掛けない
//         //  int hp = pmgEO_->mgDt_.GetState(dbst_Pow, st_.pmgMyCh_[i]->st_.pstMyCh_->id_char);//0~15
//
//         //  //ＨＰバーｐ
//         //  pmgEO_->mgGrp_.pmgEtcGrp_->GraphDrawHP(
//         //    HPX + (SIDEDIST * st_.mysideNo_),
//         //    HPY,
//         //    st_.pmgMyCh_[i]->st_.pstMyCh_->HP,
//         //    pmgEO_->mgDt_.dtSet_.GetDtDamage(setHP),
//         //    FALSE);
//         //}
//     }
//
//     void TMgTeam::FaceDraw(int drawcno)
//     {
//         if (drawcno == Defines.NGNUM) return;
//         //if (st_.pmgMyTm_->MyTeamState.MainState.ControlOrderIndex == Defines.NGNUM) return;
//         //選手情報
//         //int i = st_.pmgMyTm_->MyTeamState.MainState.ControlOrderIndex;
//         //int charNo = st_.pstMyTm_->PosSt.CharofP[i];
//         BOOL mono_f = FALSE;
//         BOOL mir_f = (st_.mysideNo_ != 0);
//         int iconposx = FACEX + (SIDEDIST * st_.mysideNo_);
//         int posy = GRIDSIZE;
//
//         pmgEO_->mgGrp_.pmgIcGrp_->OrderDrawIconHead(st_.mysideNo_, drawcno, mono_f,
//             FACEX + (SIDEDIST * st_.mysideNo_),
//             FACEY);
//     }
//
//     //試合中の名前
//     void TMgTeam::SetShiaiName()
//     {
//         //選手
//         for (int i = 0; i < DBMEMBER_ALL; ++i)
//         {
//             int nameposy = (2 + 0);
//
//             int wsNo = (DBMEMBER_ALL * st_.mysideNo_) + i;
//             int wdtNo = (DBMEMBER_ALL * st_.mysideNo_) + st_.pstMyTm_->PosSt.CharofP[i];
//
//             pmgEO_->mgFont_.OrderWordSprite(wsNo, wdtNo,
//                 NAMEX + (SIDEDIST * st_.mysideNo_),
//                 NAMEY,
//                 base::fd_None,
//                 base::DPT_3BUTTON);
//
//             pmgEO_->mgFont_.FixWordSprite_Draw(wsNo, FALSE);
//
//         }
//     }
//     void TMgTeam::StepDraw(int drawcno, int posno)
//     {
//         if (drawcno == Defines.NGNUM) return;
//         if (posno == Defines.NGNUM) return;
//
//
//         for (int i = 0; i < DBMEMBER_ALL; ++i)
//         {
//             //if (st_.pmgMyTm_->MyTeamState.MainState.ControlOrderIndex != i) continue;
//             int id = pmgGO_->pmgCh_[st_.mysideNo_][drawcno]->st_.pstMyCh_->id_char;
//
//             int rare[RARITYNUM];
//             for (int i2 = 0; i2 < STEPNUM; ++i2)
//             {
//                 rare[i2] = (st_.pmgMyCh_[posno]->st_.pstMyCh_->stGousei.set_f_[i2])
//                     ? st_.pmgMyCh_[posno]->st_.pstMyCh_->stGousei.shot_[i2].rare_[0]
//                     : Defines.NGNUM;
//             }
//             int vj = (st_.pmgMyCh_[posno]->st_.pstMyCh_->stGousei.vjset_f_)
//                 ? st_.pmgMyCh_[posno]->st_.pstMyCh_->stGousei.vjshot_.rare_[0]
//                 : Defines.NGNUM;
//
//             pmgEO_->mgGrp_.pmgEtcGrp_->DrawStep(
//                 STEPX + (SIDEDIST * st_.mysideNo_),
//                 STEPY,
//                 rare[0],
//                 rare[1],
//                 rare[2],
//                 Defines.NGNUM,
//                 rare[4],
//                 rare[5],
//                 rare[6],
//                 vj);
//             break;
//         }
//     }
//     void TMgTeam::RedrawShiaiName(int drawcno)
//     {
//         //選手
//         for (int i = 0; i < DBMEMBER_ALL; ++i)
//         {
//             int wsNo = (DBMEMBER_ALL * st_.mysideNo_) + i;
//             BOOL draw_f = (drawcno == i);
//             pmgEO_->mgFont_.FixWordSprite_Draw(wsNo, draw_f);
//         }
//     }
//     //描画
//     void TMgTeam::Draw()
//     {
//         if ((st_.pmgRf_->IsNoDamage() == FALSE) && st_.pmgRf_->IsPrev() == FALSE)
//         {
//             int drawpos = st_.pmgMyTm_->MyTeamState.MainState.ControlOrderIndex;//キャラ
//
//             //直近ダメージキャラ優先
//             if (st_.pmgMyTm_->st_.pstMyTm_->dam_c_ > 0)
//             {
//                 --st_.pmgMyTm_->st_.pstMyTm_->dam_c_;
//                 drawpos = st_.pmgMyTm_->st_.pstMyTm_->dampos_;//キャラ
//                 //drawcno = st_.pmgMyTm_->st_.pstMyTm_->damchar_;// st_.pstMyTm_->PosSt.PosofC[st_.pmgMyTm_->st_.pstMyTm_->damchar_];
//             }
//
//
//             HPDraw(drawpos);//ポジ
//             FaceDraw(st_.pstMyTm_->PosSt.CharofP[drawpos]);//キャラ
//             StepDraw(st_.pstMyTm_->PosSt.CharofP[drawpos], drawpos);//キャラ
//             RedrawShiaiName(drawpos);//ポジ
//
//         }
//
//     }
// }
//
}
