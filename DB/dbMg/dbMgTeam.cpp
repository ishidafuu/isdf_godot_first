﻿#include "dbMgTeam.h"
#include "dbGame.h"

namespace db
{
    //チーム管理およびチーム内プレーヤーオブジェクトの管理////////////////////////////////
    //コンストラクタ
    TMgTeam::TMgTeam(TGame* pGame, s32 sideNo)
        :TGameObjGen(pGame)
    {
        st_.Init();
        st_.mysideNo_ = sideNo;
        st_.ensideNo_ = (st_.mysideNo_ == 0) ? 1 : 0;//敵サイド番号
    }

    TMgTeam::~TMgTeam()
    {
    }

    //初期化（ポインタセットし直し、全てのオブジェが生成された後でないとポインタが確定してない）
    void TMgTeam::RefSet()
    {
        //マネージャーポインタ
        st_.pmgMyTm_ = pmgGO_->pmgTm_[st_.mysideNo_];
        st_.pmgEnTm_ = pmgGO_->pmgTm_[st_.ensideNo_];
        st_.pmgBa_ = pmgGO_->pmgBa_;
        st_.pmgRf_ = pmgGO_->pmgRf_;

        //ステータスポインタ
        st_.pstRf_ = &pmgSG_->stRf_;//レフェリーステータス
        st_.pstBa_ = &pmgSG_->stBa_;//ボールステータス
        st_.pstMyTm_ = &pmgSG_->stTm_[st_.mysideNo_];//チームステータス
        st_.pstEnTm_ = &pmgSG_->stTm_[st_.ensideNo_];//チームステータス

    }

    //パッドのセット
    void TMgTeam::SetSidePad()
    {
        st_.pstMyTm_->MANSide_f = pmgEO_->stPad_.IsPadCtrlSide(st_.mysideNo_);
        st_.pstMyTm_->MANSidePadNo = pmgEO_->stPad_.GetPadCtrlSideNo(st_.mysideNo_);
        //全ての状況でサイド側のパッドを持つようにする
        st_.pSidePad_ = pmgGO_->pmgPad_[st_.mysideNo_];
    }

    //ステータス初期化
    void TMgTeam::Ready(BOOL tDemo_f)//チーム設定をシーンから貰うか
    {
        const s32 FIRSTCTRL = (s32)dbpoI3;

		st_.id_kantoku_ = st_.pstMyTm_->PosSt.id_kantoku_;
        st_.mid_team_ = st_.pstMyTm_->PosSt.mid_team_;
        st_.id_team_ = st_.pstMyTm_->PosSt.id_team_;
        st_.id_m_shset_ = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(st_.mid_team_, mdm_team_id_m_shset);
        st_.rarity_ = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(st_.mid_team_, mdm_team_rarity);
        st_.comNo_ = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(st_.mid_team_, mdm_team_com);
        st_.fomNo_ = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(st_.mid_team_, mdm_team_com);//現状ＣＯＭ番号と一致
        st_.droped_f_ = FALSE;
        //選手
        for (s32 i = 0; i < DBMEMBER_ALL; ++i)
        {
            st_.pmgMyCh_[i] = pmgGO_->pmgCh_[st_.mysideNo_][i];
            st_.pstMyCh_[i] = &pmgSG_->stCh_[st_.mysideNo_][i];
        }


        //ここで初期化
        //st_.pstMyTm_->Init();initはしてある
        //st_.pstMyTm_->PosSt.mid_team_ = st_.mid_team_;
        //st_.pstMyTm_->PosSt.id_team_ = st_.id_team_;
        st_.pstMyTm_->CtrlNo = FIRSTCTRL;//最初は一番前衛の人がカーソル
        st_.pstMyTm_->DrwHS_c = NGNUM;
        st_.pstMyTm_->TmUeda_c = 0;
        st_.pstMyTm_->comNo_ = st_.comNo_;//チームに設定されたＣＯＭ番号
        st_.pstMyTm_->fomNo_ = st_.fomNo_;
        SetDefEnCXZ();//デフォ敵位置

        if (tDemo_f == FALSE)
        {
            SetShiaiName();//文字列なのでシーンの移動で消えてしまう
        }

        //パッド情報のセット
        SetSidePad();
    }

    //効果音
    void TMgTeam::SESet(enSEID SENo)
    {
        SESetInt((s32)SENo);
    }
    //効果音
    void TMgTeam::SysSESet(enSYSID SysNo)
    {
        if (st_.pstRf_->NoSE_f == FALSE)
        {
            pmgEO_->mgSound_.PlaySYS(SysNo);//, FALSE);
        }
    }
    //効果音
    void TMgTeam::SESetInt(s32 SENo)
    {
        if (st_.pstRf_->NoSE_f == FALSE)
        {
            pmgEO_->mgSound_.PlaySE(SENo);//, FALSE);
        }
    }
    //サイド位置XDF
    s32 TMgTeam::SideDist(s32 side)
    {
        return (side == 0)
            ? 0
            : 19;
    }
    //ラインYDF
    s32 TMgTeam::LineDist()
    {
        return 2;
    }


    //生存内野人数
    s32 TMgTeam::GetLiveNum()
    {
        s32 livenum = 0;
        for (s32 i = 0; i < DBMEMBER_ALL; ++i)
        {
            if ((st_.pmgMyCh_[i]->IsInfield())
                && ((st_.pmgMyCh_[i]->st_.pstMyCh_->Dead_f == FALSE)))
            {
                livenum++;
            }
        }
        return livenum;
    }
    BOOL TMgTeam::IsLastOne()//一人だけ残ってる
    {
        s32 livenum = 0;
        for (s32 i = 0; i < DBMEMBER_ALL; ++i)
        {
            //if ((st_.pmgMyCh_[i]->IsInfield()) && (st_.pmgMyCh_[i]->st_.pstMyCh_->Dead_f == FALSE))
            if ((st_.pmgMyCh_[i]->IsInfield()) && (st_.pmgMyCh_[i]->st_.pstMyCh_->Live_f))
            {
                livenum++;
            }
        }
        return (livenum == 1); //一人だけ残ってる
    }
    BOOL TMgTeam::IsAllDead()//全員HPゼロ、既に天使か
    {
        s32 deadnum = 0;
        for (s32 i = 0; i < DBMEMBER_ALL; ++i)
        {
            //HPゼロ、既に天使か
            if ((st_.pmgMyCh_[i]->st_.pstMyCh_->HP == 0) || st_.pmgMyCh_[i]->st_.pstMyCh_->ANGEL_f)
            {
                ++deadnum;
            }
        }
        return (deadnum == DBMEMBER_ALL);

        //return (st_.pstMyTm_->Dead_c >= DBMEMBER_ALL);
    }
    BOOL TMgTeam::IsAllNoLive()//全員天使
    {
        s32 livenum = 0;
        for (s32 i = 0; i < DBMEMBER_ALL; ++i)
        {
            if ((st_.pmgMyCh_[i]->IsInfield())
                && ((st_.pmgMyCh_[i]->st_.pstMyCh_->Live_f) || (st_.pmgMyCh_[i]->st_.pstMyCh_->Ueda_c > 0)))
            {
                ++livenum;
            }
        }
        return (livenum == 0);
    }
    BOOL TMgTeam::IsAllNoShTg()//全員ねらえる状態でない
    {
        s32 tagok = 0;
        for (s32 i = 0; i < DBMEMBER_INF; ++i)
        {
            if (st_.pmgMyCh_[i]->IsShTgOK())
            {
                tagok++;
            }
        }
        return (tagok == 0);
    }
    BOOL TMgTeam::IsAllStop()//全員停止
    {
        s32 movenum = 0;
        for (s32 i = 0; i < DBMEMBER_ALL; ++i)
        {
            if (((st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Y != 0) || st_.pmgMyCh_[i]->st_.pstMyCh_->Motion.IsMFlags(dbmfDam))
                && (st_.pmgMyCh_[i]->st_.pstMyCh_->Dead_f == FALSE))
            {
                movenum++;
            }
        }
        return (movenum == 0);
    }

    BOOL TMgTeam::IsNoFly()//全員停止
    {
        for (s32 i = 0; i < DBMEMBER_ALL; ++i)
        {
            if ((st_.pmgMyCh_[i]->st_.pstMyCh_->Motion.Mtype == dbmtFlF)
                || (st_.pmgMyCh_[i]->st_.pstMyCh_->Motion.Mtype == dbmtFlB))
            {
                return FALSE;
            }
        }
        return TRUE;
    }

    //死亡人数
    s32 TMgTeam::DeadNum()
    {
        s32 dnum = 0;
        for (s32 i = 0; i < DBMEMBER_ALL; ++i)
        {
            if ((st_.pmgMyCh_[i]->st_.pstMyCh_->HP == 0) || st_.pmgMyCh_[i]->st_.pstMyCh_->ANGEL_f)
            {
                ++dnum;
            }
        }

        return dnum;
    }

    //内野人数
    s32 TMgTeam::InfNum()
    {
        s32 inf = 0;
        for (s32 i = 0; i < DBMEMBER_INF; ++i)
        {
            if (st_.pmgMyCh_[i]->st_.pstMyCh_->HP > 0)
            {
                ++inf;
            }
        }

        return inf;
    }

    //瀕死人数
    s32 TMgTeam::PinchNum()
    {
        s32 pnum = 0;
        for (s32 i = 0; i < DBMEMBER_INF; ++i)
        {
            if ((st_.pmgMyCh_[i]->st_.pstMyCh_->HP < 16) && (st_.pmgMyCh_[i]->st_.pstMyCh_->MHP_f == FALSE))
            {
                pnum++;
            }
        }
        return pnum;
    }

    //帰宅済みかどうか
    BOOL TMgTeam::IsGohome()
    {
        for (s32 i = 0; i < DBMEMBER_ALL; ++i)
        {
            const s32 FULLBLIND = 6400;
            if ((st_.pmgMyCh_[i]->st_.pstMyCh_->Live_f)
                && ((st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.X < DBCRT_W + FULLBLIND)
                && (st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.X > 0 - FULLBLIND)))
            {
                return FALSE;
            }
        }

        return TRUE;
    }
    //ＣＯＭ操作
    BOOL TMgTeam::IsCOM()
    {
        return (st_.pstMyTm_->semi_f_ //セミオート時のみオート扱い
            || (st_.pstMyTm_->MANSide_f == FALSE)//COMサイド
            );//サイド操作無しか

        //return (st_.pstMyTm_->SideMaruhi_f //丸秘中か
        //        || st_.pstMyTm_->OmksSide_f //おまかせ中か
        //        || (st_.pstMyTm_->MANSide_f == FALSE));//サイド操作無しか
    }

    //マニュアルサイド操作
    BOOL TMgTeam::IsMAN()
    {
        return ((IsCOM() == FALSE));//セミオート時以外
        //return (st_.pstMyTm_->MANSide_f//サイド操作
        //        && (IsCOM() == FALSE));//ＣＯＭアシスト操作以外
    }

    //操作キャラを求める
    void TMgTeam::GetCtrl()
    {
        if (st_.pstMyTm_->CtrlNo == NGNUM)
        {
            SeekCtrl(TRUE);
        }
        else if ((st_.pmgMyCh_[st_.pstMyTm_->CtrlNo]->IsCtrlOK() == FALSE)
            && (st_.pmgMyCh_[st_.pstMyTm_->CtrlNo]->IsMAN() == FALSE))
        {
            //今はとりあえず
            SeekCtrl(TRUE);
        }

        //操作不能
        if (st_.pstMyTm_->CtrlNo == NGNUM) return;

        switch (pmgSG_->stBa_.Motion)
        {
        case bmFree:
            //止まった時点で近いキャラがタゲになる
            break;
        case bmHold:
            //持ったキャラが操作キャラになる
            //もたれたキャラの最初のタゲが操作キャラになる
            break;
        case bmShoot:
            //敵からのシュート（まずシュートされた時点でタゲが操作キャラになる）
            break;
        case bmPass:
            //まだ投げた人に操作権があるとき
            if ((st_.pstMyTm_->CtrlNo == pmgSG_->stBa_.PichPNo)
                && pmgSG_->stBa_.MvCtrl_f//操作権委譲可能なパス
                && (pmgSG_->stBa_.PaTgTNo == st_.mysideNo_)
                && (pmgSG_->stBa_.PaTgPNo <= (s32)dbpoI3))
            {
                //s32 tPACAJP = PACAJPTIME pmgEO_->mgDt_.dtSet_.GetDtGrv(setGRV_PACAJPTIME);//pmgEO_->mgDt_.dtOption_.GetDt(rkopGRV_PACAJPTIME, tGrvLv);

                if (((pmgSG_->stBa_.PaETA_c > PACAJPTIME) || (pmgSG_->stBa_.PaJPa_f == FALSE))
                    && (st_.pmgMyCh_[pmgSG_->stBa_.PaTgPNo]->IsFreeMotion(TRUE))
                    && IsMAN())//★サイド操作の場合
                {
                    if (st_.pSidePad_->IsPaTgCtrl())//操作権委譲入力
                    {
                        SetCtrl(pmgSG_->stBa_.PaTgPNo);
                    }
                }
            }
            break;
        case bmBound:
            //バウンドした瞬間に近いキャラがタゲになる。
            break;

        case bmReferee:

            break;
        }
    }

    //最前に居る内野
    s32 TMgTeam::GetFrontInfPNo()
    {
        s32 res = NGNUM;

        if (st_.pstMyTm_->CtrlNo < DBMEMBER_INF)//内野持ち
        {
            s32 posx = 0;
            for (s32 i = 0; i < DBMEMBER_INF; ++i)
            {
                if (st_.pmgMyCh_[i]->chCommon_.IsFreeMotion(TRUE) || (st_.pstMyTm_->CtrlNo == i))
                {
                    if (posx < st_.pmgMyCh_[i]->GetLeftCrtX())//前方にいる
                    {
                        posx = st_.pmgMyCh_[i]->GetLeftCrtX();
                        res = i;
                    }
                }
            }
        }
        else//外野持ち
        {
            res = st_.pstMyTm_->PosMove.Postman;//ポストマン
        }

        return res;
    }

    //最近に居る外野
    s32 TMgTeam::GetNearGaiyaPNo()
    {
        s32 res = NGNUM;

        if ((st_.pstMyTm_->CtrlNo < DBMEMBER_INF)//内野持ち
            || (st_.pstMyTm_->CtrlNo == dbpoO4))//４番外野
        {
            s32 posx = 0;
            //const s32 GAIYANUM = (DBMEMBER_ALL - DBMEMBER_INF);
            BOOL o2ok_f = (st_.pmgMyCh_[dbpoO2]->chCommon_.IsFreeMotion(TRUE));
            BOOL o3ok_f = (st_.pmgMyCh_[dbpoO3]->chCommon_.IsFreeMotion(TRUE));

            if (o2ok_f && o3ok_f)
            {
                res = (st_.pmgMyCh_[st_.pstMyTm_->CtrlNo]->st_.pstMyCh_->Zahyou.Z > DBCRT_CLZ)
                    ? dbpoO2
                    : dbpoO3;
            }
            else if (o2ok_f)
            {
                res = dbpoO2;
            }
            else if (o3ok_f)
            {
                res = dbpoO3;
            }
        }
        else//23外野持ち
        {
            res = st_.pstMyTm_->CtrlNo;//自分
        }

        return res;
    }

    //最下位オーダーの内野
    s32 TMgTeam::GetBottomInfPNo()
    {
        s32 res = NGNUM;

        //上位から下位に
        for (s32 i = 0; i < DBMEMBER_INF; ++i)
        {
            if (st_.pmgMyCh_[i]->chCommon_.IsFreeMotion(TRUE) || (st_.pstMyTm_->CtrlNo == i))
            {
                res = i;//最後にＯＫ出たキャラ
            }
        }

        return res;
    }

    //パスタイプセット
    void TMgTeam::SetPassType(s32 patype)
    {
        switch (patype)
        {
        case 0://ジャンプトスパス
            st_.pstMyTm_->stCOM.toss_f = TRUE;
            st_.pstMyTm_->stCOM.jp_f = TRUE;
            break;
        case 1://トスパス
            st_.pstMyTm_->stCOM.toss_f = TRUE;
            st_.pstMyTm_->stCOM.jp_f = FALSE;
            break;
        case 2://クイックパス
            st_.pstMyTm_->stCOM.toss_f = FALSE;
            st_.pstMyTm_->stCOM.jp_f = FALSE;
            break;
        default://ジャンプクイックパス
            st_.pstMyTm_->stCOM.toss_f = FALSE;
            st_.pstMyTm_->stCOM.jp_f = TRUE;
            break;
        }
    }

    //クイックシュートタイプセット
    void TMgTeam::SetQShootType(s32 shtype)
    {
        switch (shtype)
        {
        case 0://ＤＪＳ
            st_.pstMyTm_->stCOM.jp_f = TRUE;
            st_.pstMyTm_->stCOM.ds_f = TRUE;
            break;
        case 1://ＤＳ
            st_.pstMyTm_->stCOM.jp_f = FALSE;
            st_.pstMyTm_->stCOM.ds_f = TRUE;
            break;
        case 2://立ちＳ
            st_.pstMyTm_->stCOM.jp_f = FALSE;
            st_.pstMyTm_->stCOM.ds_f = FALSE;
            break;
        default://ＪＳ
            st_.pstMyTm_->stCOM.jp_f = TRUE;
            st_.pstMyTm_->stCOM.ds_f = FALSE;
            break;
        }
        st_.pstMyTm_->stCOM.onestep_f = FALSE;

    }

    //パワーシュートタイプセット
    void TMgTeam::SetPShootType(s32 shtype)
    {
        switch (shtype)
        {
        case 0://ＤＪＳ
            st_.pstMyTm_->stCOM.jp_f = TRUE;
            st_.pstMyTm_->stCOM.ds_f = TRUE;
            st_.pstMyTm_->stCOM.onestep_f = FALSE;
            break;
        case 1://ＤＳ
            st_.pstMyTm_->stCOM.jp_f = FALSE;
            st_.pstMyTm_->stCOM.ds_f = TRUE;
            st_.pstMyTm_->stCOM.onestep_f = FALSE;
            break;
        case 2://一歩ＤＳ
            st_.pstMyTm_->stCOM.jp_f = FALSE;
            st_.pstMyTm_->stCOM.ds_f = TRUE;
            st_.pstMyTm_->stCOM.onestep_f = TRUE;
            break;
        default://一歩ＤＪＳ
            st_.pstMyTm_->stCOM.jp_f = TRUE;
            st_.pstMyTm_->stCOM.ds_f = TRUE;
            st_.pstMyTm_->stCOM.onestep_f = TRUE;
            break;
        }
    }

    //自チームのフォーメーション番号★
    s32 TMgTeam::GetMyFomationNo()
    {
#ifdef __K_DEBUG_SHIAI__
        // ここでフォーメーションの形を変えれる
        // ※いずれでばっぐで好きな値にできるようにする※
        //	return 33;	// ドイツを指定(ディフェンス陣が前にいるから)
        //	return 25;	// おおやまさん用のデバッグ番号？
        return st_.pstMyTm_->comNo_;	// 今は元のチームの値を使う
#else
        if (pmgEO_->stShiai_.IsTuto())
        {
            return 0;//フォーメーションは熱血
        }
        else
        {
            return st_.pstMyTm_->comNo_;
        }
        //return pmgGO_->pmgCh_[st_.mysideNo_][st_.pstMyTm_->RdNo]->MyPDt(clv_Fomation);//とりあえず

#endif	// #ifdef __K_DEBUG_SHIAI__
    }

    s32 TMgTeam::GetBallmanNo()//ボールを持っているキャラ
    {
        s32 res = NGNUM;

        if (pmgSG_->stBa_.HoldTNo == st_.mysideNo_)
        {
            res = pmgSG_->stBa_.HoldPNo;
        }

        return res;
    }

    //自チームの攻撃位置目標Ｘ★
    s32 TMgTeam::GenGetAtcLineX(BOOL jp_f, BOOL dsmn_f)
    {
        if (IsNoJpDashman() && dsmn_f)
        {
            //強ジャンプラインと同じに
            return (pmgEO_->mgDt_.dtSet_.GetDtFormation(setAtcLineX3) * XYMAG);
        }
        else
        {
            endtSet_Formation atcline = (endtSet_Formation)(setAtcLineX + pmgGO_->pmgCh_[st_.mysideNo_][st_.pstMyTm_->RdNo]->MyPDt(clv_AtcLine));
            s32 tgx = pmgEO_->mgDt_.dtSet_.GetDtFormation(atcline);

            //ジャンプ踏切位置は少し後ろに
            if (jp_f)
            {
                const s32 JPLINE = 24;
                tgx += JPLINE;
            }

            return (tgx * XYMAG);
        }
    }

    //自チームの攻撃位置目標Ｘ★
    s32 TMgTeam::GetAtcLineX(BOOL jp_f, BOOL dsmn_f)
    {
        return (DBCRT_CL - GenGetAtcLineX(jp_f, dsmn_f));
    }

    //自チームのダッシュマンジャンプ不可
    BOOL TMgTeam::IsNoJpDashman()
    {
        return (pmgGO_->pmgCh_[st_.mysideNo_][st_.pstMyTm_->RdNo]->MyPDt(clv_DsmnNoJp) != 0);
    }



    BOOL TMgTeam::IsBallCrtMyPos(s32 pos, BOOL wide_f)
    {
        if (wide_f)// && st_.pstMyTm_->COMDt.WideBallPos_f)
        {
            return (((st_.pstMyTm_->BallCrtSideZ == st_.mysideNo_) && (st_.pstMyTm_->BallCrtPosZ == pos))
                || ((st_.pstMyTm_->BallCrtSideL == st_.mysideNo_) && (st_.pstMyTm_->BallCrtPosL == pos)));
        }
        else
        {
            return ((st_.pstMyTm_->BallCrtSideL == st_.mysideNo_) && (st_.pstMyTm_->BallCrtPosL == pos));
        }
    }


    //com★
    s32 TMgTeam::GetCom(enCom com)
    {
        //s32 comno = pmgGO_->pmgCh_[st_.mysideNo_][st_.pstMyTm_->RdNo]->MyPDt(clv_COM);
        if (pmgEO_->stShiai_.IsTuto())
        {
            //チュートリアルは坂宿思考
            return pmgGO_->pmgCommon_->GetComPtn(2, com, st_.mysideNo_);//とりあえず
        }
        else
        {
            return pmgGO_->pmgCommon_->GetComPtn(st_.pstMyTm_->comNo_, com, st_.mysideNo_);//とりあえず
        }
    }


    void TMgTeam::ReaderChange()//直す★
    {
        s32 tmprdno = st_.pstMyTm_->RdNo;
        st_.pstMyTm_->RdNo = CAPNO;
        //if (GetBallmanNo() == NGNUM)
        //{
        //  st_.pstMyTm_->RdNo = CAPNO;
        //}
        //else
        //{
        //  switch (pmgEO_->mgDt_.dtTeam_.GetDt(dbtm_OpeFomChange, st_.teamNo_))
        //  {
        //    case 0: //キャプテン固定
        //      st_.pstMyTm_->RdNo = CAPNO;
        //      break;
        //    case 1: //生存中の最上位
        //    {
        //      s32 tmprdno = st_.pstMyTm_->RdNo;
        //      s32 cno = DBMEMBER_ALL - 1;
        //      for (s32 i = 0; i < DBMEMBER_ALL; ++i)
        //      {
        //        if ((st_.pmgMyCh_[i]->st_.pstMyCh_->HP > 0)
        //            && (st_.pmgMyCh_[i]->st_.pstMyCh_->ANGEL_f == FALSE))
        //        {
        //          if (st_.pstMyTm_->PosSt.CharofP[i] < cno)
        //          {
        //            cno = st_.pstMyTm_->PosSt.CharofP[i];
        //          }
        //        }
        //      }
        //      st_.pstMyTm_->RdNo = cno;
        //    }
        //      break;
        //    case 2://きゃっちまん
        //    case 3://ぼーるまん
        //      st_.pstMyTm_->RdNo = st_.pstMyTm_->PosSt.CharofP[GetBallmanNo()];
        //      break;
        //  }
        //}
        //
        //if (tmprdno != st_.pstMyTm_->RdNo)
        //{
        //  PopupFomChange();
        //}
    }

    s32 TMgTeam::GetComAct(s32 comactNo, enDtComAct cma, BOOL ressei_f, s32 id)
    {
        return pmgGO_->pmgCommon_->GetComActPtn(comactNo, cma, ressei_f, st_.mysideNo_, id);//とりあえず
    }

    //s32 TMgTeam::GetComActNo(s32 posno)
    //{
    //  BOOL useselfact_f = FALSE;
    //  switch (pmgEO_->mgDt_.dtTeam_.GetDt2(dbtm_OpeSelfPlay, st_.teamNo_))
    //  {
    //    case 4: useselfact_f = pmgGO_->pmgCommon_->IsPer100(100); break;//個人ぷれー→
    //    case 3: useselfact_f = pmgGO_->pmgCommon_->IsPer100(75); break;
    //    case 2: useselfact_f = pmgGO_->pmgCommon_->IsPer100(50); break;
    //    case 1: useselfact_f = pmgGO_->pmgCommon_->IsPer100(25); break;//←リーダーにあわせる
    //  }
    //  
    //  s32 useact = (useselfact_f)
    //  ? st_.pstMyTm_->PosSt.CharofP[posno]
    //  : st_.pstMyTm_->RdNo;
    //  
    //  s32 cmano = pmgGO_->pmgCh_[st_.mysideNo_][useact]->MyPDt(clv_COM);
    //  
    //  return cmano;
    //}



    //操作権予約
    void TMgTeam::SetCtrlReserve(s32 tCRNo)
    {
        st_.pstMyTm_->CtrlRsvNo = tCRNo;
    }

    //操作権強制
    void TMgTeam::SetCtrlBallGet(s32 tCNo)
    {
        st_.pstMyTm_->CtrlNo = tCNo;
    }
    //操作権（playerobjから直接呼び出すのはどうだろうか）
    void TMgTeam::SetCtrl(s32 tCNo)
    {

        //指定があったキャラが操作不能状態の時は、
        //近いキャラが代わりに操作可能になる
        //近いキャラが居ない(内野一人の時とか、ダウン中の外野とか)場合は操作不能状態
        if (tCNo == NGNUM) return;

        if (st_.pmgMyCh_[tCNo]->IsCtrlOK())
        {
            st_.pstMyTm_->CtrlNo = tCNo;
        }
        else
        {
            SeekCtrl(TRUE);
        }
    }
    //現状から操作権最適キャラを決める
    s32 TMgTeam::SeekCtrl(BOOL set_f)
    {
        return SeekCtrl(set_f, FALSE);
    }
    s32 TMgTeam::SeekCtrl(BOOL set_f, BOOL landpos_f)
    {
        //各キャラのボールとの距離を測る
        for (s32 i = 0; i < DBMEMBER_ALL; ++i)
        {
            st_.pmgMyCh_[i]->chCommon_.GetBallDist();
        }
        //一番近いキャラ
        s32 nearPNo = NGNUM;


        if (landpos_f)//着地地点から判断
        {
            //内野を検索
            for (s32 i = 0; i < DBMEMBER_INF; ++i)
            {

                if (st_.pmgMyCh_[i]->st_.pstMyCh_->BallLandDist == NGNUM) continue;

                if (nearPNo == NGNUM) //先客なし
                {
                    nearPNo = i;
                }
                else if (st_.pmgMyCh_[i]->st_.pstMyCh_->BallLandDist < st_.pmgMyCh_[nearPNo]->st_.pstMyCh_->BallLandDist)//先客と比較
                {
                    nearPNo = i;
                }
            }
        }
        else
        {
            //内野を検索
            for (s32 i = 0; i < DBMEMBER_INF; ++i)
            {
                if (st_.pmgMyCh_[i]->st_.pstMyCh_->BallDist == NGNUM) continue;

                if (nearPNo == NGNUM) //先客なし
                {
                    nearPNo = i;
                }
                else if (st_.pmgMyCh_[i]->st_.pstMyCh_->BallDist < st_.pmgMyCh_[nearPNo]->st_.pstMyCh_->BallDist)//先客と比較
                {
                    nearPNo = i;
                }
            }
        }

        if (set_f)
        {
            //変わる前のキャラは変わった直後にポジショニングしない★
            //st_.pstMyTm_->PosMove.Inf_f[st_.pstMyTm_->CtrlNo] = FALSE;
            st_.pstMyTm_->CtrlNo = nearPNo;
        }

        return nearPNo;

    }
    //次期カバーマンを設定する
    void TMgTeam::SeekCover(s32 tHoldPNo, s32 tPichPNo, s32 tPaTgPNo, BOOL Cvr2_f)
    {

        st_.pstMyTm_->CvrNo = NGNUM;
        st_.pstMyTm_->Cvr2No = NGNUM;

        //外野からのシュートのみ
        if ((tPichPNo > (s32)dbpoI3) && (tPaTgPNo <= (s32)dbpoI3))
        {
            //各キャラのボールとの距離を測る
            for (s32 i = 0; i < DBMEMBER_ALL; ++i)
            {
                st_.pmgMyCh_[i]->chCommon_.GetBallDist();
            }

            //一番近いキャラ
            s32 nearPNo = NGNUM;

            //内野を検索
            for (s32 i = 0; i < DBMEMBER_INF; ++i)
            {
                if ((st_.pmgMyCh_[i]->st_.pstMyCh_->BallDist == NGNUM)
                    || (i == tPaTgPNo)
                    || (i == tHoldPNo)
                    || (st_.pmgMyCh_[i]->IsFreeMotion(TRUE) == FALSE)
                    || st_.pmgMyCh_[i]->IsMAN())//★専用操作
                {
                    continue;
                }
                //第二カバーマン選出の時は現カバーマンも外れる
                else if (Cvr2_f && ((i == st_.pstMyTm_->CvrNo) || (i == st_.pstMyTm_->CtrlNo)))
                {
                    continue;
                }
                else
                {
                    s32 idist = st_.pmgMyCh_[i]->st_.pstMyCh_->BallLandDist;
                    if (nearPNo != NGNUM)
                    {
                        s32 neard = st_.pmgMyCh_[nearPNo]->st_.pstMyCh_->BallLandDist;
                        if (idist < neard)
                        {
                            nearPNo = i;
                        }
                    }
                    else
                    {
                        nearPNo = i;
                    }

                }

            }

            if (Cvr2_f)
            {
                st_.pstMyTm_->Cvr2No = nearPNo;
            }
            else
            {
                st_.pstMyTm_->CvrNo = nearPNo;
                st_.pstMyTm_->Cvr2No = NGNUM;
            }
        }

    }

    //ボール拾いマン最適者
    void TMgTeam::SeekGetter()
    {
        //各キャラのボールとの距離を測る
        for (s32 i = 0; i < DBMEMBER_ALL; ++i)
        {
            st_.pmgMyCh_[i]->chCommon_.GetBallDist();
        }

        //一番近いキャラ
        s32 nearPNo = NGNUM;

        //内野を検索
        for (s32 i = 0; i < DBMEMBER_INF; ++i)
        {
            if ((st_.pmgMyCh_[i]->st_.pstMyCh_->BallDist == NGNUM)
                || (st_.pmgMyCh_[i]->IsFreeMotion(TRUE) == FALSE))//★専用操作
            {
                continue;
            }


            if (IsCOM())
            {
                //COMの場合は操作権キャラ
                if (st_.pmgMyCh_[i]->IsCtrl() == FALSE) continue;

                nearPNo = i;
            }
            else
            {
                //COM以外は操作権以外の一番近いキャラ
                if (st_.pmgMyCh_[i]->IsCtrl()) continue;


                s32 idist = st_.pmgMyCh_[i]->st_.pstMyCh_->BallLandDist;
                if (nearPNo != NGNUM)
                {
                    s32 neard = st_.pmgMyCh_[nearPNo]->st_.pstMyCh_->BallLandDist;
                    if (idist < neard)
                    {
                        nearPNo = i;
                    }
                }
                else
                {
                    nearPNo = i;
                }
            }
        }

        st_.pstMyTm_->GetNo = nearPNo;
    }


    ////ポジションリセット予約する
    //void TMgTeam::SetPosReset()
    //{
    //  st_.pstMyTm_->PosReset_f = TRUE;
    //}

    void TMgTeam::PopupFomChange()
    {
        std::string fom = pmgEO_->mgDt_.dtFomation_.GetName(GetMyFomationNo()) + "＿ふぉめ";
        pmgGO_->pmgCommon_->PopupMsg(pu_DsMn, fom.c_str(), st_.mysideNo_, 0, 0, 0, FC2_shiai, FALSE);
    }
    //ポジショニング
    void TMgTeam::SetPosLine()
    {
        //敵にボールがもたれているとき（カウンタでインターバル）
        //もしくは強制ポジションリセットが発生した場合

        //どの位置の敵が持っているか
        switch (pmgSG_->stBa_.Motion)
        {
        case bmHold:
            if (pmgSG_->stBa_.HoldTNo == st_.mysideNo_)
            {
                st_.pstMyTm_->PosMove.InfLine = (pmgSG_->stBa_.HoldPNo < DBMEMBER_INF)
                    ? iltInf
                    : iltOfO;
            }
            else
            {
                switch (pmgSG_->stBa_.HoldPNo)
                {
                case (s32)dbpoO2: st_.pstMyTm_->PosMove.InfLine = iltO2; break;
                case (s32)dbpoO3: st_.pstMyTm_->PosMove.InfLine = iltO3; break;
                case (s32)dbpoO4: st_.pstMyTm_->PosMove.InfLine = iltO4; break;
                default: st_.pstMyTm_->PosMove.InfLine = iltInf; break;
                }
            }
            break;
        case bmShoot:
            //敵外野シュートヒット時のポジショニングが敵内野のもの
            if (pmgSG_->stBa_.PichTNo == st_.ensideNo_)
            {
                switch (pmgSG_->stBa_.PichPNo)
                {
                case (s32)dbpoO2: st_.pstMyTm_->PosMove.InfLine = iltO2; break;
                case (s32)dbpoO3: st_.pstMyTm_->PosMove.InfLine = iltO3; break;
                case (s32)dbpoO4: st_.pstMyTm_->PosMove.InfLine = iltO4; break;
                default: st_.pstMyTm_->PosMove.InfLine = iltInf; break;
                }
            }
            else
            {
                st_.pstMyTm_->PosMove.InfLine = iltInf;
            }
            break;
        case bmPass:
            //味方ボール（のときは本当は動いていないはずだが）
            if (pmgSG_->stBa_.PaTgTNo == st_.mysideNo_)
            {
                st_.pstMyTm_->PosMove.InfLine = (pmgSG_->stBa_.PaTgPNo < DBMEMBER_INF)
                    ? iltInf
                    : iltOfO;
            }
            else
            {
                //敵ボールの時はパス先をみて
                switch (pmgSG_->stBa_.PaTgPNo)
                {
                case (s32)dbpoO2: st_.pstMyTm_->PosMove.InfLine = iltO2; break;
                case (s32)dbpoO3: st_.pstMyTm_->PosMove.InfLine = iltO3; break;
                case (s32)dbpoO4: st_.pstMyTm_->PosMove.InfLine = iltO4; break;
                default: st_.pstMyTm_->PosMove.InfLine = iltInf; break;
                }
            }
            break;
        case bmReferee:
            st_.pstMyTm_->PosMove.InfLine = iltInf;
            break;
        default:
            //ジャンプボール
            if (pmgSG_->stBa_.JumpBall == jbJpball)
            {
                st_.pstMyTm_->PosMove.InfLine = iltInf;
            }
            else
            {
                //敵陣地
                if (st_.pstMyTm_->BallCrtSideL != st_.mysideNo_)
                {
                    switch (st_.pstMyTm_->BallCrtPosL)
                    {
                    case (s32)dbpoO2: st_.pstMyTm_->PosMove.InfLine = iltO2; break;
                    case (s32)dbpoO3: st_.pstMyTm_->PosMove.InfLine = iltO3; break;
                    case (s32)dbpoO4: st_.pstMyTm_->PosMove.InfLine = iltO4; break;
                    default: st_.pstMyTm_->PosMove.InfLine = iltInf; break;
                    }
                }
                else//味方陣地
                {
                    //敵外野シュートヒット時のポジショニングが敵内野のもの
                    if (pmgSG_->stBa_.PichTNo == st_.ensideNo_)
                    {
                        switch (pmgSG_->stBa_.PichPNo)
                        {
                        case (s32)dbpoO2: st_.pstMyTm_->PosMove.InfLine = iltO2; break;
                        case (s32)dbpoO3: st_.pstMyTm_->PosMove.InfLine = iltO3; break;
                        case (s32)dbpoO4: st_.pstMyTm_->PosMove.InfLine = iltO4; break;
                        default: st_.pstMyTm_->PosMove.InfLine = iltInf; break;
                        }
                    }
                    else
                    {
                        st_.pstMyTm_->PosMove.InfLine = iltInf;
                    }
                }
            }
            break;
        }



        //それに応じた配置(FTypeはPlObj.moveでも使ってる)
        switch (st_.pstMyTm_->PosMove.InfLine)
        {
        case iltO4: st_.pstMyTm_->PosMove.FType = ftDf4; break;
        case iltO2: st_.pstMyTm_->PosMove.FType = ftDf2; break;
        case iltO3: st_.pstMyTm_->PosMove.FType = ftDf3; break;
        case iltOfO:

            //新しくポストマン決める
            if ((st_.pstMyTm_->PosMove.Postman == NGNUM)//
                || (st_.pstMyTm_->PosMove.FType != ftOfO))//今まで外野攻撃ではなかった
            {
                //初期化
                st_.pstMyTm_->PosMove.Postman = NGNUM;
                s32 secpostNo = NGNUM;
                for (s32 i = 0; i < DBMEMBER_INF; ++i)
                {
                    s32 i2 = (DBMEMBER_INF - 1 - i);//４番から探る

                    //if (i2 == st_.pstMyTm_->CtrlNo) continue;//操作権
                    if (st_.pmgMyCh_[i2]->IsFreeMotion(TRUE) == FALSE) continue;//自由
                    if (st_.pmgMyCh_[i2]->st_.pstMyCh_->LandEnCourt_f) continue;//敵コートに落下しない

                    //第二候補
                    if (secpostNo == NGNUM)
                    {
                        secpostNo = i2;
                    }

                    if (st_.pmgMyCh_[i2]->IsMAN()) continue;//★専用操作

                    st_.pstMyTm_->PosMove.Postman = i2;
                    break;
                }

                if (st_.pstMyTm_->PosMove.Postman == NGNUM)
                {
                    st_.pstMyTm_->PosMove.Postman = secpostNo;
                }

            }

            st_.pstMyTm_->PosMove.FType = ftOfO;

            break;
        default: st_.pstMyTm_->PosMove.FType = ftOfI; break;//内野持ち攻守
        }


        if (st_.pstMyTm_->PosMove.FType != ftOfO) //外野攻撃時以外はポストマンいらない
        {
            st_.pstMyTm_->PosMove.Postman = NGNUM;
        }

    }
    //ダッシュマンストック
    void TMgTeam::SetDashmanStock()
    {
        //ダッシュマンの数
        st_.pstMyTm_->PosMove.DashmanNum = 0;
        st_.pstMyTm_->PosMove.DashmanNum_Auto = 0;
        st_.pstMyTm_->PosMove.DashmanNum_Run = 0;
        st_.pstMyTm_->PosMove.DashmanNum_Run_NT = 0;

        //ダッシュマン順位
        for (s32 i = 0; i < DBMEMBER_INF; ++i)
        {
            //ダッシュマンの人数
            if (st_.pmgMyCh_[i]->IsDashman()
                || (st_.pmgMyCh_[i]->st_.pstMyCh_->DashmanNo != NGNUM)
                )
            {
                ++st_.pstMyTm_->PosMove.DashmanNum;

                //オートのダッシュマンの人数
                if (st_.pmgMyCh_[i]->st_.pstMyCh_->Auto.AutoType == dbatDashman)
                {
                    ++st_.pstMyTm_->PosMove.DashmanNum_Auto;
                }

                //その中でも走っている人
                if (st_.pmgMyCh_[i]->st_.pstMyCh_->Motion.Mtype == dbmtDs)
                {
                    ++st_.pstMyTm_->PosMove.DashmanNum_Run;
                    //その中でもまだ触ってない

                    if (st_.pstMyTm_->COMDt.actdt[i].BallTouched_f == FALSE)
                    {
                        ++st_.pstMyTm_->PosMove.DashmanNum_Run_NT;
                    }
                }
            }

            st_.pstMyTm_->PosMove.DashmanStock[i] = NGNUM;//埋め
        }

        //０番から優先
        s32 f = 0;

        //if ((st_.mysideNo_ == 0) && (st_.pstMyTm_->PosMove.DashmanNum == 0))
        //{
        for (s32 i = 0; i < DBMEMBER_INF; ++i)
        {
            if (i == st_.pstMyTm_->PosMove.Postman)
            {
                st_.pstMyTm_->PosMove.dmng[i] = 1;
                continue;//ポストマンは入らない
            }

            if (i == st_.pstMyTm_->CtrlNo)
            {
                st_.pstMyTm_->PosMove.dmng[i] = 2;
                continue; //操作権キャラは入らない
            }

            if (st_.pmgMyCh_[i]->IsMAN())
            {
                st_.pstMyTm_->PosMove.dmng[i] = 3;
                continue;//マニュアル操作キャラは入らない
            }

            if (st_.pmgMyCh_[i]->st_.pstMyCh_->Motion.IsMFlags(dbmfDmOK) == FALSE)
            {
                st_.pstMyTm_->PosMove.dmng[i] = 4;
                continue;//ダッシュマン可能状態
            }

            if (st_.pmgMyCh_[i]->IsDMOKPos() == FALSE)
            {
                st_.pstMyTm_->PosMove.dmng[i] = 5;
                continue;//コートの中央より後ろ側のみ
            }

            if (st_.pmgMyCh_[i]->st_.pstMyCh_->Motion.IsMFlags(dbmfAr))
            {
                st_.pstMyTm_->PosMove.dmng[i] = 6;
                continue;//地上のみ
            }

            if (st_.pmgMyCh_[i]->IsFreeMotion(TRUE) == FALSE)
            {
                st_.pstMyTm_->PosMove.dmng[i] = 7;
                continue;//自由
            }
            //if (st_.pmgMyCh_[i]->st_.pstMyCh_->LandEnCourt_f) continue;//敵コートに落下しない　そもそも立のみ
            //その人向けのパスが出ているとき
            if ((pmgSG_->stBa_.Motion == bmPass)
                && (pmgSG_->stBa_.PaTgTNo == st_.mysideNo_)
                && (pmgSG_->stBa_.PaTgPNo == i))
            {
                st_.pstMyTm_->PosMove.dmng[i] = 8;
                continue;
            }


            st_.pstMyTm_->PosMove.dmng[i] = 999;
            st_.pstMyTm_->PosMove.DashmanStock[f++] = i;//埋め

        }
        //}
        //else
        //{
        //  for (s32 i = 0; i < DBMEMBER_INF; ++i)
        //  {
        //    if (i == st_.pstMyTm_->PosMove.Postman) continue;//ポストマンは入らない
        //
        //    if (i == st_.pstMyTm_->CtrlNo) continue; //操作権キャラは入らない

        //    if (st_.pmgMyCh_[i]->IsMAN()) continue;//マニュアル操作キャラは入らない

        //    if (st_.pmgMyCh_[i]->st_.pstMyCh_->Motion.IsMFlags(dbmfDmOK) == FALSE) continue;//ダッシュマン可能状態
        //
        //    if (st_.pmgMyCh_[i]->IsDMOKPos() == FALSE) continue;//コートの中央より後ろ側のみ

        //    if (st_.pmgMyCh_[i]->st_.pstMyCh_->Motion.IsMFlags(dbmfAr)) continue;//地上のみ

        //    if (st_.pmgMyCh_[i]->IsFreeMotion(TRUE) == FALSE) continue;//自由
        //    //if (st_.pmgMyCh_[i]->st_.pstMyCh_->LandEnCourt_f) continue;//敵コートに落下しない　そもそも立のみ
        //    //その人向けのパスが出ているとき
        //
        //    if ((pmgSG_->stBa_.Motion == bmPass)
        //      && (pmgSG_->stBa_.PaTgTNo == st_.mysideNo_)
        //      && (pmgSG_->stBa_.PaTgPNo == i))continue;

        //    st_.pstMyTm_->PosMove.DashmanStock[f++] = i;//埋め
        //
        //  }
        //}

        //mid::midLog("ovat\n");

        //呼べる人数
        st_.pstMyTm_->PosMove.PassAbleNum = f;

        //ダッシュマンがいなくなったら作戦リセット
        if (st_.pstMyTm_->PosMove.DashmanNum_Auto == 0)
        {

            //初期化
            st_.pstMyTm_->CallingDashman_f = FALSE;
            if (st_.pstMyTm_->CallingDashman_f3)//スタート後に０になった
            {
                st_.pstMyTm_->CallingDashman_f2 = FALSE;
                st_.pstMyTm_->CallingDashman_f3 = FALSE;
            }
        }
        //ダッシュマンがいなくなったら作戦リセット
        if (st_.pstMyTm_->PosMove.DashmanNum == 0)
        {
            st_.pstMyTm_->DashmanType = dmtNone;
            st_.pstMyTm_->DashmanCall_c = 0;
            st_.pstMyTm_->DashmanNoBM = NGNUM;

            //ダッシュマン番号も空に
            for (s32 i = 0; i < DBMEMBER_INF; ++i)
            {
                if (st_.pmgMyCh_[i]->st_.pstMyCh_->DashmanNo != NGNUM)//埋め)
                {
                    st_.pmgMyCh_[i]->st_.pstMyCh_->DashmanNo = NGNUM;//埋め
                }

                //ボールマンダッシュナシで渡ってしまった場合、ダッシュマン作戦が終わったことになってしまう
            }

        }

    }


    //デフォ敵中心位置
    void TMgTeam::SetDefEnCXZ()
    {
        const s32 DEF23POS = DBCRT_BL + (24 * XYMAG);
        const s32 DEFZPOS = 56 * XYMAG;
        //st_.pstMyTm_->PosMove.EnC_f = TRUE;
        st_.pstMyTm_->PosMove.EnCX = DEF23POS;
        st_.pstMyTm_->PosMove.EnCX3 = DEF23POS;
        st_.pstMyTm_->PosMove.EnCZ = DEFZPOS;

        //はみ出チェック
        CheckEnC();
    }
    //敵内野の重心
    void TMgTeam::GetCOG()
    {
        s32 EL_c = 0;

        s32 EnX[DBMEMBER_INF];
        s32 EnZ[DBMEMBER_INF];
        s32 EnX2[DBMEMBER_INF];
        s32 EnZ2[DBMEMBER_INF];

        //st_.pstMyTm_->PosMove.EnC_f = TRUE;

        for (s32 i = 0; i < DBMEMBER_INF; ++i)
        {
            if (st_.pmgEnTm_->IsLastOne())
            {
                if (st_.pmgEnTm_->st_.pmgMyCh_[i]->IsFreeMotion(FALSE))
                {
                    EnX[EL_c] = st_.pmgEnTm_->st_.pmgMyCh_[i]->GetLeftCrtX();
                    EnZ[EL_c] = st_.pmgEnTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z;
                    EL_c++;
                }
            }
            else
            {
                //生きててダメージ状態ではないうえに戻り中とかでもない
                if (st_.pmgEnTm_->st_.pmgMyCh_[i]->IsFreeMotion(TRUE))
                {
                    EnX[EL_c] = st_.pmgEnTm_->st_.pmgMyCh_[i]->GetLeftCrtX();
                    EnZ[EL_c] = st_.pmgEnTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z;
                    EL_c++;
                }
            }
        }
        switch (EL_c)
        {
        case 4:
            //重心を取る
            //適当
            EnX2[0] = (EnX[0] + EnX[1]) / 2;
            EnX2[1] = (EnX[1] + EnX[2]) / 2;
            EnX2[2] = (EnX[2] + EnX[3]) / 2;
            EnX2[3] = (EnX[3] + EnX[0]) / 2;
            EnZ2[0] = (EnZ[0] + EnZ[1]) / 2;
            EnZ2[1] = (EnZ[1] + EnZ[2]) / 2;
            EnZ2[2] = (EnZ[2] + EnZ[3]) / 2;
            EnZ2[3] = (EnZ[3] + EnZ[0]) / 2;
            st_.pstMyTm_->PosMove.EnCX = (EnX2[0] + EnX2[1] + EnX2[2] + EnX2[3]) / 4;
            st_.pstMyTm_->PosMove.EnCZ = (EnZ2[0] + EnZ2[1] + EnZ2[2] + EnZ2[3]) / 4;
            break;
        case 3:
            EnX2[0] = (EnX[0] + EnX[1]) / 2;
            EnX2[1] = (EnX[1] + EnX[2]) / 2;
            EnX2[2] = (EnX[2] + EnX[0]) / 2;
            EnZ2[0] = (EnZ[0] + EnZ[1]) / 2;
            EnZ2[1] = (EnZ[1] + EnZ[2]) / 2;
            EnZ2[2] = (EnZ[2] + EnZ[0]) / 2;

            st_.pstMyTm_->PosMove.EnCX = (EnX2[0] + EnX2[1] + EnX2[2]) / 3;
            st_.pstMyTm_->PosMove.EnCZ = (EnZ2[0] + EnZ2[1] + EnZ2[2]) / 3;
            break;
        case 2:
            st_.pstMyTm_->PosMove.EnCX = (EnX[0] + EnX[1]) / 2;
            st_.pstMyTm_->PosMove.EnCZ = (EnZ[0] + EnZ[1]) / 2;
            break;
        case 1:
            st_.pstMyTm_->PosMove.EnCX = EnX[0];
            st_.pstMyTm_->PosMove.EnCZ = EnZ[0];
            break;
        default:
            SetDefEnCXZ();
            return;//はみ出しチェックしてあるのでそのまま終了
        }

        //はみ出チェック
        CheckEnC();
    }
    //はみ出チェック
    void TMgTeam::CheckEnC()
    {
        //はみ出てる
        if (st_.pstMyTm_->PosMove.EnCX > DBCRT_CLI)
        {
            st_.pstMyTm_->PosMove.EnCX = DBCRT_CLI;
        }

        //３番外野用
        st_.pstMyTm_->PosMove.EnCX3 = st_.pstMyTm_->PosMove.EnCX;

        const s32 CRPOS_B_XL = DBCRT_SL + (16 * XYMAG);//奥外野Ｘ左限界
        const s32 CRPOS_F_XL = DBCRT_SL;//手前外野Ｘ左限界

        //はみ出てる
        if (st_.pstMyTm_->PosMove.EnCX < CRPOS_B_XL)
        {
            st_.pstMyTm_->PosMove.EnCX = CRPOS_B_XL;
        }
        //はみ出てる
        if (st_.pstMyTm_->PosMove.EnCX3 < CRPOS_F_XL)
        {
            st_.pstMyTm_->PosMove.EnCX3 = CRPOS_F_XL;
        }
        //１Ｐの外野はコート逆転
        if (st_.mysideNo_ == 0)
        {
            st_.pstMyTm_->PosMove.EnCX = DBCRT_W - st_.pstMyTm_->PosMove.EnCX;
            st_.pstMyTm_->PosMove.EnCX3 = DBCRT_W - st_.pstMyTm_->PosMove.EnCX3;
        }
    }


    //ボールの位置（落下地点）
    void TMgTeam::TeamBallPos()
    {

        BOOL infleftcrt_f = FALSE;
        BOOL outleftcrt_f = FALSE;

        const s32 STLW = 2 * XYMAG;
        if (st_.mysideNo_ == 0)
        {
            infleftcrt_f = (pmgSG_->stBa_.LandX < (DBCRT_CL + STLW));
            outleftcrt_f = (pmgSG_->stBa_.LandX < (DBCRT_CL - STLW));
        }
        else
        {
            infleftcrt_f = (pmgSG_->stBa_.LandX < (DBCRT_CL - STLW));
            outleftcrt_f = (pmgSG_->stBa_.LandX < (DBCRT_CL + STLW));
        }

        BOOL infside = (infleftcrt_f)
            ? 0
            : 1;

        s32 gaiyaside = (outleftcrt_f)
            ? 1
            : 0;

        s32 tX = (pmgSG_->stBa_.LandX < DBCRT_CL)
            ? pmgSG_->stBa_.LandX
            : DBCRT_CL - (pmgSG_->stBa_.LandX - DBCRT_CL);


        BOOL st1_f = ((st_.mysideNo_ != infside) && (pmgSG_->stBa_.Motion == bmFree));
        BOOL st2_f = FALSE;
        BOOL st3_f = FALSE;
        BOOL st4_f = FALSE;


        if (st_.mysideNo_ == gaiyaside)
        {
            if ((pmgSG_->stBa_.Motion == bmFree)
                || (pmgSG_->stBa_.Motion == bmBound))
            {
                //強奪
                st2_f = TRUE;
                st3_f = TRUE;
                st4_f = TRUE;
            }
        }


        //s32 st1 = (st1_f)
        //? (pmgEO_->mgDt_.dtSet_.GetDtCOM(setNaiyaSteel) * XYMAG)
        //: 0;

        //s32 st2 = (st2_f)
        //? (st_.pmgMyCh_[(s32)dbpoO2]->Rank(rkomSTEAL_D) * XYMAG)
        //: 0;
        //
        //s32 st3 = (st3_f)
        //? (st_.pmgMyCh_[(s32)dbpoO3]->Rank(rkomSTEAL_D) * XYMAG)
        //: 0;
        //
        //s32 st4 = (st4_f)
        //? (st_.pmgMyCh_[(s32)dbpoO4]->Rank(rkomSTEAL_W) * XYMAG)
        //: 0;

        BOOL gaiya_f = FALSE;


        //自チームボールだと判断するか
        if (pmgSG_->stBa_.LandZ <= (DBCRT_FL))//強奪なし
        {
            st_.pstMyTm_->BallCrtPosL = (s32)dbpoO3;
            gaiya_f = TRUE;
        }
        else if (pmgSG_->stBa_.LandZ >= (DBCRT_BL))//強奪なし
        {
            st_.pstMyTm_->BallCrtPosL = (s32)dbpoO2;
            gaiya_f = TRUE;
        }
        else if (tX < ((DBCRT_SL)+((pmgSG_->stBa_.LandZ - DBCRT_FL) * 10 / DBCRT_SLR)))//強奪なし
        {
            st_.pstMyTm_->BallCrtPosL = (s32)dbpoO4;
            gaiya_f = TRUE;
        }
        else
        {
            st_.pstMyTm_->BallCrtPosL = (s32)dbpoI0;
        }


        st_.pstMyTm_->BallCrtSideL = (gaiya_f)
            ? gaiyaside
            : infside;

    }

    //ボールの位置（実座標）
    void TMgTeam::TeamBallPos2()
    {

        BOOL infleftcrt_f = FALSE;
        BOOL outleftcrt_f = FALSE;

        const s32 STLW = -4 * XYMAG;
        if (st_.mysideNo_ == 0)
        {
            infleftcrt_f = (pmgSG_->stBa_.Zahyou.X < (DBCRT_CL + STLW));
            outleftcrt_f = (pmgSG_->stBa_.Zahyou.X < (DBCRT_CL - STLW));
        }
        else
        {
            infleftcrt_f = (pmgSG_->stBa_.Zahyou.X < (DBCRT_CL - STLW));
            outleftcrt_f = (pmgSG_->stBa_.Zahyou.X < (DBCRT_CL + STLW));
        }

        BOOL infside = (infleftcrt_f)
            ? 0
            : 1;

        s32 gaiyaside = (outleftcrt_f)
            ? 1
            : 0;

        s32 tX = (pmgSG_->stBa_.Zahyou.X < DBCRT_CL)
            ? pmgSG_->stBa_.Zahyou.X
            : DBCRT_CL - (pmgSG_->stBa_.Zahyou.X - DBCRT_CL);



        BOOL gaiya_f = FALSE;


        //自チームボールだと判断するか
        if (pmgSG_->stBa_.Zahyou.Z <= (DBCRT_FL))//強奪なし
        {
            st_.pstMyTm_->BallCrtPosZ = (s32)dbpoO3;
            gaiya_f = TRUE;
        }
        else if (pmgSG_->stBa_.Zahyou.Z >= (DBCRT_BL))//強奪なし
        {
            st_.pstMyTm_->BallCrtPosZ = (s32)dbpoO2;
            gaiya_f = TRUE;
        }
        else if (tX < st_.pmgBa_->baCommon_.GetSLX())//強奪なし
        {
            st_.pstMyTm_->BallCrtPosZ = (s32)dbpoO4;
            gaiya_f = TRUE;
        }
        else
        {
            st_.pstMyTm_->BallCrtPosZ = (s32)dbpoI0;
        }

        ////反転
        //if (revSide_f)
        //{
        //  st_.pstMyTm_->BallCrtSideZ = (st_.pstMyTm_->BallCrtSideZ == 0)
        //    ? 1
        //    : 0;
        //}

        st_.pstMyTm_->BallCrtSideZ = (gaiya_f)
            ? gaiyaside
            : infside;

    }


    //操作権キャラ
    void TMgTeam::NowCtrlNo()
    {
        if (st_.pmgRf_->IsCapOnlyCtrl())
        {
            //もともと０だから大丈夫か
            st_.pstMyTm_->CtrlNo = 0;
        }
        else
        {
            //まず現状から操作できるキャラを決める
            GetCtrl();
        }

    }

    //ポジショニング予定位置計算ポジショニングカウンタ減少
    void TMgTeam::GetEnC_c()
    {
        //ポジショニング予定位置計算
        //ポジショニングカウンタ減少
        if (lib_num::AprTo0(&st_.pstMyTm_->PosMove.EnC_c))
        {
            const s32 GAIYAOFPOSITV = 60;//外野オフェンスポジションリセット間隔(f)

            st_.pstMyTm_->PosMove.EnC_c = (st_.pmgEnTm_->IsLastOne())
                ? (GAIYAOFPOSITV / 4)
                : GAIYAOFPOSITV;

            //ボールがもたれているとき
            if (pmgSG_->stBa_.Motion == bmHold)
            {
                //マイボール
                if (pmgSG_->stBa_.HoldTNo == st_.mysideNo_)
                {
                    GetCOG();
                }
                else//敵ボール
                {
                    SetDefEnCXZ();
                }
            }
        }
    }



    //ポジションカウンタ
    void TMgTeam::SetPosition()
    {

        //現状使ってない

        ////ディフェンスポジションリセット予約
        //if (st_.pstMyTm_->PosReset_f)
        //{
        //  //リセット
        //  st_.pstMyTm_->PosMove.Inf_c = pmgEO_->mgDt_.dtSet_.GetDtFormation(setNaiyaDfPosItv);
        //  //SetDefFormation();//ディフェンス
        //  st_.pstMyTm_->PosReset_f = FALSE;
        //}

    }


    //各内野ポジションの現在行くべき位置★
    void TMgTeam::GetFomposOfInf(s32& getx, s32& getz, s32 posNo)
    {
        s32 line = pmgEO_->mgDt_.dtFomation_.LineOfPos(GetMyFomationNo(), posNo);//衛
        s32 linenum = pmgEO_->mgDt_.dtFomation_.GetDt(GetMyFomationNo(), (enDtFomation)(fomRPos + line));//衛内人数01234
        s32 row = pmgEO_->mgDt_.dtFomation_.RowOfPos(GetMyFomationNo(), posNo);//衛内列
        s32 zwide = pmgEO_->mgDt_.dtFomation_.GetDt(GetMyFomationNo(), fomZWidth);//123
        s32 etc = pmgEO_->mgDt_.dtFomation_.GetDt(GetMyFomationNo(), fomEtc);//01

        //横並び
        //前中後センターラインからのＸ位置
        const s32 XLDIST = 32 * XYMAG;//30
        const s32 XL_FX = 84 * XYMAG;//96
        const s32 XL_MX = (XL_FX + XLDIST);
        const s32 XL_RX = (XL_MX + XLDIST);
        //Ｚ幅
        const s32 XLZWID = 16 * XYMAG;
        const s32 XL_ZW3 = 64 * XYMAG;
        const s32 XL_ZW2 = (XL_ZW3 - XLZWID);
        const s32 XL_ZW1 = (XL_ZW2 - XLZWID);
        //Ｘ幅
        const s32 XL_XW = XLDIST / 3;
        const s32 XL_SITUREV = 8 * XYMAG;//戦況による変動

        //縦並び
        //前中後サイドラインからのＺ位置
        const s32 ZLDIST = 12 * XYMAG;//12
        const s32 ZL_RZ = 72 * XYMAG;//68
        const s32 ZL_MZ = ZL_RZ - ZLDIST;
        const s32 ZL_FZ = ZL_MZ - ZLDIST;
        //Ｚ幅
        const s32 ZLXWID = 24 * XYMAG;//24
        const s32 ZL_XW3 = 104 * XYMAG;//96
        const s32 ZL_XW2 = (ZL_XW3 - ZLXWID);
        const s32 ZL_XW1 = (ZL_XW2 - ZLXWID);
        //Ｘ幅
        const s32 ZL_ZW = ZLDIST / 3;
        const s32 ZL_SITUREV = 3 * XYMAG;//戦況による変動

        const s32 NLWDIV = 3;


        const s32 FNLW = 1;
        const s32 MNLW = 2;
        const s32 RNLW = 3;
        const s32 GRMN = 4;
        const s32 FIJI = 5;

        //フィジーはポジション順
        if (etc == FIJI)
        {
            row = posNo;
        }



        //23外野持ち（縦並び）
        if ((st_.pstMyTm_->PosMove.FType == ftDf2)
            || (st_.pstMyTm_->PosMove.FType == ftDf3))
        {
            s32 zsign = 0;
            //前衛中衛後衛
            if (st_.pstMyTm_->PosMove.FType == ftDf3)
            {
                getz = DBCRT_FL;
                zsign = +1;
                //switch (line)
                //{
                //case 0: getz += ZL_RZ; break;//後衛
                //case 1: getz += ZL_MZ; break;//中衛
                //case 2: getz += ZL_FZ; break;//前衛
                //}
            }
            else
            {
                getz = DBCRT_BL;
                zsign = -1;
                //switch (line)
                //{
                //case 0: getz -= ZL_RZ; break;//後衛
                //case 1: getz -= ZL_MZ; break;//中衛
                //case 2: getz -= ZL_FZ; break;//前衛
                //}
            }


            switch (line)
            {
            case 0: getz += (zsign * ZL_RZ); break;//後衛
            case 1: getz += (zsign * ZL_MZ); break;//中衛
            case 2: getz += (zsign * ZL_FZ); break;//前衛
            }

            //戦況応援によるライン変動
            getz += (zsign * st_.pstMyTm_->COMDt.FomRev * ZL_SITUREV);


            if (linenum >= 2)
            {
                s32 xw = 0;
                switch (zwide)//幅
                {
                case 1: xw = ZL_XW1; break;//狭い
                case 2: xw = ZL_XW2; break;//ふつう
                case 3: xw = ZL_XW3; break;//広い
                }

                //２?２狭い
                if (((line == 0) && (etc == RNLW))//後衛
                    || ((line == 1) && (etc == MNLW))//中衛
                    || ((line == 2) && (etc == FNLW)))//前衛
                {
                    xw /= NLWDIV;
                }

                //間隔
                s32 dist = 0;
                dist = (xw / (linenum - 1));
                getx = DBCRT_CLXL + (xw / 2) - (row * dist);//中心+(幅/2)+(衛内番手 * 幅)

                //ドイツ
                if ((etc == GRMN) && (row == 1))
                {
                    if (st_.pstMyTm_->PosMove.FType == ftDf2)
                    {
                        getz = getz + (ZLDIST);
                    }
                    else
                    {
                        getz = getz - (ZLDIST);
                    }
                }
            }
            else
            {
                getx = DBCRT_CLXL;
            }
        }
        //内野および４番外野（横並び）
        else
        {

            s32 ZUREKAKU = XL_ZW3;
            s32 xsign = 0;

            //前衛中衛後衛
            if (st_.pstMyTm_->PosMove.FType == ftDf4)//敵外野持ち
            {
                getx = DBCRT_SL;
                xsign = +1;

                switch (line)
                {
                case 0:
                    //getx += XL_RX;
                    ZUREKAKU = 64;
                    break;//後衛
                case 1:
                    //getx += XL_MX;
                    ZUREKAKU = 13;
                    break;//中衛
                case 2:
                    //getx += XL_FX;
                    ZUREKAKU = 7;
                    break;//前衛
                }
            }
            else//内野持ち、敵内野持ち、味方外野持ち
            {
                getx = DBCRT_CL;
                xsign = -1;

                switch (line)
                {
                case 0:
                    //getx -= XL_RX;
                    ZUREKAKU = 4;
                    break;//後衛
                case 1:
                    //getx -= XL_MX;
                    ZUREKAKU = 5;
                    break;//中衛
                case 2:
                    //getx -= XL_FX;
                    ZUREKAKU = 7;
                    break;//前衛
                }

            }

            switch (line)
            {
            case 0:
                getx += (xsign * XL_RX);
                break;//後衛
            case 1:
                getx += (xsign * XL_MX);
                break;//中衛
            case 2:
                getx += (xsign * XL_FX);
                break;//前衛
            }


            //戦況応援によるライン変動
            getx += (xsign * st_.pstMyTm_->COMDt.FomRev * XL_SITUREV);


            //フィジー
            if (etc == FIJI)
            {
                ZUREKAKU = -1;
            }

            if (linenum >= 2)
            {
                s32 zw = 0;
                switch (zwide)//幅
                {
                case 1: zw = XL_ZW1; break;//狭い
                case 2: zw = XL_ZW2; break;//ふつう
                case 3: zw = XL_ZW3; break;//広い
                }

                //２?２狭い
                if (((line == 0) && (etc == RNLW))//後衛
                    || ((line == 1) && (etc == MNLW))//中衛
                    || ((line == 2) && (etc == FNLW)))//前衛
                {
                    zw /= NLWDIV;
                }


                //間隔
                s32 dist = 0;
                dist = (zw / 2) - (row * (zw / (linenum - 1)));
                getz = DBCRT_CLZ + dist;//中心+(幅/2)+(衛内番手 * 幅)

                //傾き
                getx += (dist / ZUREKAKU);

                //ドイツ
                if ((etc == GRMN) && (row == 1))
                {
                    if (st_.pstMyTm_->PosMove.FType == ftDf4)
                    {
                        getx = getx - (XLDIST);
                    }
                    else
                    {
                        getx = getx + (XLDIST);
                    }
                }

            }
            else
            {
                getz = DBCRT_CLZ;
            }
        }

    }

    //ダッシュマン指令
    void TMgTeam::OrderDashman()
    {
        //プレビューでは呼べない
        if (pmgGO_->pmgRf_->IsPrev()) return;

        //試合中のみ
        if (st_.pstRf_->DBSSt != dbssShiai) return;

        //操作権無いときには無効
        if (st_.pstMyTm_->CtrlNo == NGNUM) return;

        st_.pstMyTm_->DashmanCallNo = NGNUM;
        st_.pstMyTm_->DashmanAll_f = FALSE;


        BOOL Call_f = FALSE;
        BOOL iosi_f = mid::midIsTBL();
        BOOL All_f = FALSE;
        BOOL test_f = FALSE;

        s32 callRsv = NGNUM;//呼ぶ予定の人
        for (s32 i = 0; i < DBMEMBER_INF; ++i)
        {
            if (st_.pstMyTm_->PosMove.DashmanStock[i] != NGNUM)
            {
                callRsv = st_.pstMyTm_->PosMove.DashmanStock[i];
                break;
            }
        }


        //プレーヤー
        if (IsCOM() == FALSE)
        {
            //コール
            Call_f = st_.pSidePad_->IsDMCall();//ダッシュマンボタン
        }
        else//ＣＯＭのダッシュマン発動★★
        {
            BOOL delayOK_f = IsDMDelayTime();

            //呼ぶタイミング＆シュートフラグがまだ
            if (IsDMDelayTime()
                && st_.pstMyTm_->ofside_f)
            {
                //一回休みフラグが立ってるとき
                if (st_.pstMyTm_->COMDt.DsOneItvWait_f)
                {
                    st_.pstMyTm_->COMDt.DsOneItvWait_f = FALSE;
                    return;
                }

                //ボールマンダッシュをそもそも呼ばない、もしくは、すでにセッターダッシュ開始後
                if ((st_.pstMyTm_->COMDt.setterBMRsv_f == FALSE)
                    || st_.pstMyTm_->COMDt.setterBMRsvGo_f)
                {
                    //呼びたい人数呼んでない
                    if (st_.pstMyTm_->COMDt.dmcalledNum < st_.pstMyTm_->COMDt.dmcallNum)
                    {
                        Call_f = TRUE;
                    }
                }
                else
                {
                    //セッターの走り開始がまだ
                    if (((GetBallmanNo() == st_.pstMyTm_->COMDt.setterNo) || st_.pmgMyCh_[st_.pstMyTm_->COMDt.setterNo]->IsMAN())//セッター操作中
                        && ((callRsv > st_.pstMyTm_->COMDt.setterNo) //セッターより下位キャラが呼ばれてる
                        || (st_.pstMyTm_->COMDt.dmcalledNum >= st_.pstMyTm_->COMDt.dmcallNum)//既に呼びたい人数に到達してる
                        || (callRsv == NGNUM)))
                    {
                        st_.pstMyTm_->COMDt.setterBMRsvGo_f = TRUE;//セッターＢＭＤＯＫ
                    }
                    else
                    {
                        //呼びたい人数呼んでない
                        if (st_.pstMyTm_->COMDt.dmcalledNum < st_.pstMyTm_->COMDt.dmcallNum)
                        {
                            Call_f = TRUE;
                        }
                    }
                }
            }
        }


        //実際に呼ぶ
        if (Call_f)
        {
            BOOL CrsL_f = FALSE;
            BOOL CrsR_f = FALSE;
            BOOL CrsU_f = FALSE;
            BOOL CrsD_f = FALSE;

            BOOL infCrs_f = FALSE;
            BOOL enmCrs_f = FALSE;

            BOOL DMGo_f = FALSE;

            if (st_.pSidePad_ != NULL)
            {
                st_.pSidePad_->GetDMCallCrs(&CrsL_f, &CrsR_f, &CrsU_f, &CrsD_f);

                infCrs_f = (((st_.mysideNo_ == 0) && CrsL_f)
                    || ((st_.mysideNo_ == 1) && CrsR_f));

                enmCrs_f = (((st_.mysideNo_ == 0) && CrsR_f)
                    || ((st_.mysideNo_ == 1) && CrsL_f));
            }


            if (IsCOM()//★サイド操作以外とりあえず
                || ((CrsL_f || CrsR_f || CrsU_f || CrsD_f) == FALSE))//ニュートラル
            {
                //ダッシュスタイル
                switch (st_.pstMyTm_->COMDt.comPtn[comDMType])
                {
                case 0://ワイド
                    if (callRsv != NGNUM)
                    {
                        //広がるように
                        if (st_.pmgMyCh_[callRsv]->st_.pstMyCh_->Zahyou.Z > DBCRT_CLZ)
                        {
                            CrsU_f = TRUE;
                        }
                        else
                        {
                            CrsD_f = TRUE;
                        }
                    }
                    break;
                case 1://アプローチ
                    enmCrs_f = TRUE;
                    break;
                case 2://ストレート
                    infCrs_f = TRUE;
                    break;
                default://クロス
                    if (callRsv != NGNUM)
                    {
                        //狭まるように
                        if (st_.pmgMyCh_[callRsv]->st_.pstMyCh_->Zahyou.Z > DBCRT_CLZ)
                        {
                            CrsD_f = TRUE;
                        }
                        else
                        {
                            CrsU_f = TRUE;
                        }
                    }
                    break;
                }
            }


            s32 alldmNum = 0;
            //s32 stock = 9;
            for (s32 i = 0; i < DBMEMBER_INF; ++i)
            {
                if (st_.pstMyTm_->PosMove.DashmanStock[i] != NGNUM)
                {
                    DMGo_f = TRUE;//一人でも送り出せる
                    //stock = i;
                    ++alldmNum;
                }
            }

            //効果音
            if (DMGo_f)
            {
                SESet(seDmOK);
                //TZahyouRec* pzh = &(st_.pmgMyCh_[st_.pstMyTm_->CtrlNo]->st_.pstMyCh_->Zahyou);
                //s32 fx = pzh->X / XYMAG;
                //s32 fy = DRAWBASELINE
                //  - ((pzh->Y + pzh->Z) / XYMAG)
                //  - DAMPOSY;
                //
                //s32 dt = stock;
                //
                ////カメラ位置
                //const s32 camposX = pmgEO_->mgCamera_.GetRevCamPosX();
                //enFontCol2 col = FC_Red;
                //pmgEO_->mgFont_.OrderDamFontSprite(lib_str::IntToStr(dt).c_str(), fx, fy, col);
            }
            else
            {
                if (IsCOM() == FALSE)//★サイド操作以外とりあえず
                {
                    SESet(seDmNG);
                    //mid::midLog("ng0\n");
                }

                st_.pstMyTm_->COMDt.callNG_f = TRUE;//呼べないときが１回でもあった
            }

            for (s32 i = 0; i < DBMEMBER_INF; ++i)
            {
                if (st_.pstMyTm_->PosMove.DashmanStock[i] != NGNUM)
                {
                    if (All_f)
                    {
                        st_.pstMyTm_->DashmanAll_f = TRUE;//全員
                    }
                    else
                    {
                        st_.pstMyTm_->DashmanCallNo = st_.pstMyTm_->PosMove.DashmanStock[i];
                    }

                    //呼んだ人
                    if (st_.pstMyTm_->DashmanType == dmtNone)
                    {
                        st_.pstMyTm_->DMCallmanNo = st_.pstMyTm_->CtrlNo;
                    }

                    if (st_.pstMyTm_->CallingDashman_f2)
                    {
                        st_.pstMyTm_->CallingDashman_f3 = TRUE;
                    }

                    ////呼ぶカウンタ減らす
                    //lib_num::AprTo0(&st_.pstMyTm_->COMDt.dmcallNum);

                    //作戦が決まってないとき
                    if ((st_.pstMyTm_->DashmanType == dmtNone)
                        //&& (st_.pstMyTm_->CallingDashman_f2 == FALSE)
                        //&& (st_.pstMyTm_->SideMaruhi_f == FALSE)
                        )
                    {

                        //std::string dmname = "";
                        //
                        //if (CrsU_f)
                        //{
                        //  st_.pstMyTm_->DashmanType = dmtUp;
                        //  dmname = "サイド";
                        //}
                        //else if (CrsD_f)
                        //{
                        //  st_.pstMyTm_->DashmanType = dmtDw;
                        //  dmname = "サイド";
                        //}
                        //else if (infCrs_f) //後ろ
                        //{
                        //  st_.pstMyTm_->DashmanType = dmtStr;
                        //  dmname = "ストレート";
                        //}
                        //else if (enmCrs_f)
                        //{
                        //  st_.pstMyTm_->DashmanType = dmtAp;
                        //  dmname = "センター";
                        //}
                        //else
                        //{
                        //  st_.pstMyTm_->DashmanType = dmtStr;
                        //  dmname = "ストレート";
                        //}
                        //
                        //if (IsNoJpDashman())
                        //{
                        //  dmname += "ダッシュ";
                        //}
                        //else
                        //{
                        //  dmname += "ジャンプ";
                        //}
                        //
                        //
                        ////作戦表示//dmname += "だっしゅまん";//ここはチームごとの作戦名を
                        //TZahyouRec* pzh = &(st_.pmgMyCh_[st_.pstMyTm_->DMCallmanNo]->st_.pstMyCh_->Zahyou);
                        //pmgGO_->pmgCommon_->PopupMsg(pu_DsMn, dmname.c_str(), st_.mysideNo_, pzh->X, pzh->Y, pzh->Z, FC2_shiai);



                    }

                    break;
                }

            }
        }
    }

    //内外野入れ替えが発生する人数
    BOOL TMgTeam::IsIOChange()
    {
        //元外野が012
        const s32 OUTNUM = (DBMEMBER_ALL - DBMEMBER_INF);
        return (st_.pstMyTm_->UseMotoGaiya_c < OUTNUM);
    }
    //内外野入れ替え確認
    void TMgTeam::CheckChangePos()
    {

        BOOL change_f = FALSE;

        for (s32 i = 0; i < DBMEMBER_INF; ++i)
        {
            //死亡確認
            if ((st_.pmgMyCh_[i]->st_.pstMyCh_->HP == 0)
                && (st_.pmgMyCh_[i]->st_.pstMyCh_->ANGEL_f)
                && (st_.pmgMyCh_[i]->st_.pstMyCh_->Dead_f == FALSE))
            {


                std::string out = st_.pmgMyCh_[i]->MyName() + "＿アウト";
                pmgGO_->pmgCommon_->PopupMsg(pu_DsMn, out.c_str(),
                    st_.mysideNo_, 0, 0, 0, FC2_shiai, FALSE);


                //倒された数カウンタ増やす
                ++st_.pstMyTm_->Dead_c;

                BOOL IOChange_f = IsIOChange();//★FALSE;//

                //サドンデス決着後は入れ替えしない
                if (st_.pmgRf_->IsSuddunDeath()
                    && (DeadNum() != st_.pmgEnTm_->DeadNum()))
                {
                    IOChange_f = FALSE;
                }

                if (IOChange_f)//交換
                {
                    //天使になる人
                    //天使オブジェクト生成
                    //pmgGO_->pmgCommon_->SetEObj(etomAngel, st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou,
                    //                st_.mysideNo_,st_.pstMyTm_->PosSt.CharofP[i]);

                    //SESet(seDead);//SEならす

                    SetAngel(i);


                    st_.pmgMyCh_[i]->chCommon_.StInit(TRUE);

                    //st_.pmgMyCh_[i]->chCommon_.SetMtype(dbmtJDn);

                    st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Y = 0;
                    st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.X = (st_.mysideNo_ == 0)
                        ? DBCRT_W
                        : 0;
                    st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Muki = (st_.mysideNo_ == 0)
                        ? mL
                        : mR;

                    change_f = TRUE;
                    switch (st_.pstMyTm_->IOChange_c)
                    {
                    case 0:
                        st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z = CRPOS_B_Z;//pmgEO_->mgDt_.dtSet_.GetDtFormation(setPos4_Z) * XYMAG;
                        break;
                    case 1:
                        st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z = CRPOS_F_Z;//pmgEO_->mgDt_.dtSet_.GetDtFormation(setPos5_Z) * XYMAG;
                        break;
                    case 2:
                        st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z = POS6_Z;
                        break;
                    default:
                        //change_f = FALSE;
                        break;
                    }

                    //交換先の外野がボールを持ってる場合
                    const s32 GAIYANUM = 3;
                    s32 changePos = DBMEMBER_INF + (st_.pstMyTm_->IOChange_c % GAIYANUM);
                    if (st_.pmgMyCh_[changePos]->IsBall())
                    {
                        st_.pmgBa_->baCommon_.SetMtype(bmBound);//手放す
                        st_.pmgBa_->baCommon_.ZeroXYZ();

                        pmgSG_->stBa_.NGGet_f = TRUE;
                        pmgSG_->stBa_.NGGetPNo = i;//入れ替え後のpos
                        pmgSG_->stBa_.NGGetTNo = st_.mysideNo_;
                    }

                    //上田カウントが残っている場合
                    if (st_.pstMyTm_->TmUeda_c > 0)
                    {
                        --st_.pstMyTm_->TmUeda_c;//上田カウント減らす
                        st_.pmgMyCh_[i]->st_.pstMyCh_->ANGEL_f = FALSE;//天使の輪を消す
                    }
                    else
                    {
                        ++st_.pstMyTm_->UseMotoGaiya_c;//元外野を使う
                    }


                    //ポジションチェンジ//交換した回数に基づいた位置から
                    pmgSG_->ChangePos(st_.mysideNo_, i, changePos);

                    //入れ替えたポジションのＨＰにリセット
                    //全員初期化の段階でHP入ってるので、現状ここでの差し替えは不要
                    if (st_.pmgRf_->IsSuddunDeath())
                    {
                        st_.pmgMyCh_[i]->st_.pstMyCh_->HP = 1;//サドンデスならＨＰ１に
                    }


                    //交換先の外野がパスタゲの場合、変わった後の人がパスタゲにならないように
                    if ((pmgSG_->stBa_.Motion == bmPass)
                        && (pmgSG_->stBa_.PaTgTNo == st_.mysideNo_)
                        && (pmgSG_->stBa_.PaTgPNo == changePos))
                    {
                        pmgSG_->stBa_.PaTgPNo = i;
                    }

                    ++st_.pstMyTm_->IOChange_c;//交換した回数
                }
                else
                {

                    st_.pmgMyCh_[i]->chCommon_.SetMtype(dbmtANG);
                }

                ////誰か死んだらＣＯＭ思考リセット
                //st_.pstMyTm_->RsvResetCOM_f = TRUE;
            }

        }

        //内外野交代になったとき
        if (change_f && (st_.pstRf_->Demo_f == FALSE))
        {
            //名前
            SetShiaiName();
        }
    }

    void TMgTeam::SetAngel(s32 posNo)
    {
        //天使オブジェクト生成
        pmgGO_->pmgCommon_->SetEObj(etomAngel,
            st_.pmgMyCh_[posNo]->st_.pstMyCh_->Zahyou,
            st_.mysideNo_,
            st_.pstMyTm_->PosSt.CharofP[posNo]);

        //てきキャプテンがアイテムおとす

        if (pmgEO_->stShiai_.IsVSHum() == FALSE &&	// 通信対戦じゃない
            !pmgEO_->stSeq_.IsFirstTuto())		// 初回チュートリアルじゃない
        {
            if ((st_.mysideNo_ == SIDE1)//→チーム
                && (st_.pmgMyCh_[posNo]->st_.objNo_ == 0)//キャプテン
                && (st_.droped_f_ == FALSE)//落としてない
                && (st_.pmgRf_->IsTour() == FALSE)//疑似対人以外
                && (pmgEO_->stShiai_.IsTuto() == FALSE)
                )
            {
                st_.droped_f_ = TRUE;

                pmgGO_->pmgCommon_->SetEObj(etomItem,
                    st_.pmgMyCh_[posNo]->st_.pstMyCh_->Zahyou,
                    st_.mysideNo_,
                    st_.pstMyTm_->PosSt.CharofP[posNo]);

                SysSESet(sysNaBoxOtona);//SEならす
            }
        }


        SESet(seDead);//SEならす
    }
    //サドンデス状態に変更
    void TMgTeam::SuddunDeath()//チーム設定をシーンから貰うか
    {
        for (s32 i = 0; i < DBMEMBER_ALL; ++i)
        {
            if (st_.pmgMyCh_[i]->IsInfield() && (st_.pmgMyCh_[i]->st_.pstMyCh_->HP > 0))
            {
                st_.pmgMyCh_[i]->st_.pstMyCh_->HP = 1;
            }
        }
    }

    ////ヒットストップリセット
    //void TMgTeam::ResetHitStop()
    //{
    //  //for (s32 i = 0; i < DBMEMBER_ALL; ++i)
    //  //{
    //  //  st_.pmgMyCh_[i]->st_.pstMyCh_->HitStop_f = FALSE;
    //  //}
    //}

    //デバッグ用
    void TMgTeam::COMDebugSwitch()
    {
        ////st_.pstMyTm_->COMDt.comPtn[comRessei] = ;//劣勢判断 //体力一定以下いれば	残りが敵未満	ボス退場	内野一人
        ////st_.pstMyTm_->COMDt.comPtn[comBallTake] = ;//ボール奪い //劣勢なら行く	行く	行かない	とれそうなら

        ////st_.pstMyTm_->COMDt.comPtn[comPinch] = ;//ピンチガード //短い	ほぼ同時	長い	普通
        ////st_.pstMyTm_->COMDt.comPtn[comPinchTag] = ;//ピンチガード対象 //対象が瀕死なら	対象がいれば	しない	外野攻撃なら
        //st_.pstMyTm_->COMDt.comPtn[comCounter] = ;//カウンターキャッチ//劣勢ならする] = ;する] = ;しない] = ;劣勢ならしない
        ////st_.pstMyTm_->COMDt.comPtn[comDgType] = ;//ドッジタイプ//サイド避け] = ;ジャンプ避け] = ;膝付き避け] = ;シュート対応避け

        ////st_.pstMyTm_->COMDt.comPtn[comPassCut] = ;//パスカット//劣勢ならする] = ;する] = ;しない] = ;対面パスなら

        //st_.pstMyTm_->COMDt.comPtn[comQDMCallCH] = 1;//DM コール//劣勢ならする] = ;する] = ;しない] = ;劣勢でないなら
        //st_.pstMyTm_->COMDt.comPtn[comQDMCallNC] = 1;//DM コール//劣勢ならする] = ;する] = ;しない] = ;劣勢でないなら
        //st_.pstMyTm_->COMDt.comPtn[comDMCallCH] = 1;//DM コール//劣勢ならする] = ;する] = ;しない] = ;劣勢でないなら
        //st_.pstMyTm_->COMDt.comPtn[comDMCallNC] = 1;//DM コール//劣勢ならする] = ;する] = ;しない] = ;劣勢でないなら
        //st_.pstMyTm_->COMDt.comPtn[comDMSetter] = 0;//OFDM セッター指定//一番前方の内野,内野最下位オーダー,ボス,外野（自分が内野ならば一番近い外野）
        //st_.pstMyTm_->COMDt.comPtn[comDMSetBM] = 0;//DM タイミング//キャッチしたら] = ;速攻] = ;ボールマンジャンプタイミング] = ;ボールマンダッシュと同時
        //
        //st_.pstMyTm_->COMDt.comPtn[comDMNum] = 1;//DM 人数//3人いたら] = ;ALL] = ;1人] = ;2人いたら
        //st_.pstMyTm_->COMDt.comPtn[comDMDelay] = 0;//DM ディレイ//短い,ほぼ同時,長い,普通
        //st_.pstMyTm_->COMDt.comPtn[comDMType] = 0;//ダッシュマンスタイル//ラインダッシュ,サイドダッシュ,ノーマルダッシュ,クロスダッシュ
        //st_.pstMyTm_->COMDt.comPtn[comChance] = 0;//チャンス判断//劣勢なら,する,しない,優勢なら
        //st_.pstMyTm_->COMDt.comPtn[comDMPaNum] = 0;//DM パス回数//一人一回,出来るかぎり,しない,１回
        //st_.pstMyTm_->COMDt.comPtn[comDMPaTag] = 0;//DM パス先//一つオーダー上位,近い人,遠い人,後方の人
        //st_.pstMyTm_->COMDt.comPtn[comDMPaTime] = 3;//DM パスタイミング//パス先が前衛ライン超えたら	パスインターバル後	自分のシュートタイミング	パス先がダッシュ開始したら
        //st_.pstMyTm_->COMDt.comPtn[comDMPaItv] = 1;//DM パスインターバル//短い,最短,長い,普通

        ////st_.pstMyTm_->COMDt.comPtn[comDMPaItv] = ;//DM パスインターバル//短い] = ;最短] = ;長い] = ;普通
        ////st_.pstMyTm_->COMDt.comPtn[comChMake] = ;//チャンスメイク//劣勢なら] = ;する] = ;しない] = ;優勢なら
        ////st_.pstMyTm_->COMDt.comPtn[comChMakePtn] = ;//チャンスメイクスタイル//側面パス] = ;最上位狙い] = ;反時計] = ;対面パス
        ////st_.pstMyTm_->COMDt.comPtn[comChMakeLoop] = ;//チャンスメイクループMAX//少ない] = ;多い] = ;最少] = ;普通
        //st_.pstMyTm_->COMDt.comPtn[comAction] = 1;//アクション//シュート変更パス] = ;//パワーシュート] = ;シングルパス] = ;オーダーパス

        ////st_.pstMyTm_->COMDt.comPtn[comShPaNum] = ;//シュート変更パス回数//少ない] = ;最少] = ;多い] = ;普通
        ////st_.pstMyTm_->COMDt.comPtn[comOdPaTag] = ;//オーダーパス先//上位オーダー] = ;ぱわー＋きあい] = ;ボス] = ;上位ラインナップ
        ////st_.pstMyTm_->COMDt.comPtn[comSgPaTag] = ;//シングルパス先//内野] = ;近接] = ;ランダム] = ;対面
        ////st_.pstMyTm_->COMDt.comPtn[comNrPaTag] = ;//近接パスの狙い//上位オーダーに近い] = ;攻撃力高に近い] = ;ＨＰ少に近い] = ;守備下手に近い
        ////st_.pstMyTm_->COMDt.comPtn[comPaWait] = ;//シングルパスウエイト//短い] = ;最短] = ;長い] = ;普通

        //st_.pstMyTm_->COMDt.comPtn[comPaType] = 2;//パスタイプ//ジャンプトスパス,トスパス,クイックパス,ジャンプクイックパス
        ////st_.pstMyTm_->COMDt.comPtn[comQShType] = ;//クイックシュートタイプ//Ｄシュート] = ;ＤＪシュート] = ;立ちシュート] = ;Ｊシュート
        //st_.pstMyTm_->COMDt.comPtn[comPwShType] = 0;//シュートタイプ//Ｄシュート] = ;ＤＪシュート] = ;立ちシュート] = ;Ｊシュート
        //st_.pstMyTm_->COMDt.comPtn[comShWait] = 0;//パワーウェイト//短い] = ;最短] = ;長い] = ;普通
        //st_.pstMyTm_->COMDt.comPtn[comDShStep] = 1;//Ｄシュート歩数//６（ナイス）] = ;４（必殺）] = ;７歩] = ;５歩
        ////st_.pstMyTm_->COMDt.comPtn[comJpPos] = ;//ＤＪシュート踏切位置//短い] = ;ラインギリギリ] = ;長い] = ;普通
        ////st_.pstMyTm_->COMDt.comPtn[comJShTime] = ;//ＤＪシュートタイミング//頂点（必殺）] = ;早め（ＤＪナイス）] = ;ジャンプしない] = ;下降
        //st_.pstMyTm_->COMDt.comPtn[comDsPos] = 0;//ダッシュ開始位置//必殺位置まで下がる] = ;エンドラインまで下がる] = ;ダッシュ] = ;ナイス位置までさがる

        //st_.pstMyTm_->COMDt.comPtn[comGuard] = 1;//ガード//対象が瀕死なら] = ;対象がいれば] = ;しない] = ;外野攻撃なら
        //st_.pstMyTm_->COMDt.comPtn[comGuardTag] = 1;//ガード対象//上位オーダー] = ;誰でも] = ;ボス] = ;上位ラインナップ
        //st_.pstMyTm_->COMDt.comPtn[comCounter] = 1;//カウンターキャッチ//劣勢ならする,する,しない,劣勢ならしない

        //st_.pstMyTm_->COMDt.comPtn[comChance] = 1;//チャンス判断//劣勢なら,する,しない,優勢なら
        //st_.pstMyTm_->COMDt.comPtn[comDMNum] = 0;//DM 人数//３メン,ALL,1人,２メン

        //st_.pstMyTm_->COMDt.comPtn[comQDMCallCH] = 2;//クイックDM コールチャンス//劣勢ならする,する,しない,劣勢でないなら
        //st_.pstMyTm_->COMDt.comPtn[comQDMCallNC] = 1;//クイックQDM コールノーチャンス//劣勢ならする,する,しない,劣勢でないなら

        //st_.pstMyTm_->COMDt.comPtn[comDMCallCH] = 2;//DM コールチャンス//劣勢ならする,する,しない,劣勢でないなら
        //st_.pstMyTm_->COMDt.comPtn[comDMCallNC] = 2;//DM コールノーチャンス//劣勢ならする,する,しない,劣勢でないなら
        //
        ////OFDM セッター指定//一番前方の内野,内野最下位オーダー,ボス,外野（自分が内野ならば一番近い外野）
        //st_.pstMyTm_->COMDt.comPtn[comDMSetter] = 0;
        ////OFDM セッター＆ボールマン参加//セッタースタート＆ＢＭ参加ＯＫ,セッター関係なし＆ＢＭ参加ＯＫ,セッター関係なし＆ＢＭ参加ＮＧ,セッタースタート＆ＢＭ参加ＮＧ
        //st_.pstMyTm_->COMDt.comPtn[comDMSetBM] = 0;
        ////DM 通常／態勢ウエイト（チャンス時スルー）//１５／６０、０／態勢Ｗナシ、６０／揃うまで、３０／１２０
        //st_.pstMyTm_->COMDt.comPtn[comDMWait] = 0;
        ////DM ダッシュ開始トリガー//０フレウエイト、セッターパスと同時に、６０フレウェイトしてから開始、３０フレウェイトしてから開始
        //st_.pstMyTm_->COMDt.comPtn[comDMTrigger] = 0;
        ////DM ディレイ//短い,ほぼ同時,長い,普通
        //st_.pstMyTm_->COMDt.comPtn[comDMDelay] = 0;
        ////ダッシュマンスタイル//ラインダッシュ,サイドダッシュ,ノーマルダッシュ,クロスダッシュ
        //st_.pstMyTm_->COMDt.comPtn[comDMType] = 0;
        //
        //st_.pstMyTm_->COMDt.comPtn[comDMPaNum] = 0;//DM パス回数//一人一回,出来るかぎり,しない,１回
        //st_.pstMyTm_->COMDt.comPtn[comDMPaTag] = 0;//DM パス先//一つオーダー上位,近い人,遠い人,後方の人
        //st_.pstMyTm_->COMDt.comPtn[comDMPaTime] = 3;//DM パスタイミング//パス先が前衛ライン超えたら	パスインターバル後	自分のシュートタイミング	パス先がダッシュ開始したら
        //st_.pstMyTm_->COMDt.comPtn[comDMPaItv] = 0;//DM パスインターバル//短い,最短,長い,普通
        //st_.pstMyTm_->COMDt.comPtn[comPaType] = 0;//パスタイプ//ジャンプトスパス,トスパス,クイックパス,ジャンプクイックパス
        //st_.pstMyTm_->COMDt.comPtn[comPwShType] = 1;//クイックシュートタイプ//Ｄシュート,ＤＪシュート,立ちシュート,Ｊシュート
        //st_.pstMyTm_->COMDt.comPtn[comQShType] = 0;//クイックシュートタイプ//Ｄシュート,ＤＪシュート,立ちシュート,Ｊシュート
        //st_.pstMyTm_->COMDt.comPtn[comCrvType] = 0;//変化球タイプ//カーブ,タゲ外し,曲げない,トレース

        //st_.pstMyTm_->COMDt.comPtn[comAction] = 2;
        //st_.pstMyTm_->COMDt.comPtn[comShWait] = 2;
    }


    //COMダッシュマン作戦（とりあえずノーマルダッシュマンのみ）
    void TMgTeam::COMDashman()
    {

        //そもそもオフェンスの時のときのみ★キャッチ前も可能であれば出来るように
        if (st_.pstMyTm_->ofside_f == FALSE) return;

        //１人の時も無理
        if (IsLastOne()) return;

        BOOL dmcall_f = FALSE;//そもそも呼ぶか
        BOOL nowcallOK_f = TRUE;//この瞬間に呼べるか
        BOOL waitcallOK_f = TRUE;//待てば呼べるか


        //コールによるリセットの場合はダッシュマン判断
        if (st_.pstMyTm_->CallingDMReset_f)
        {
            dmcall_f = TRUE;
        }
        else
        {
            s32 dmc = (st_.pstMyTm_->COMDt.Chance_f)//チャンスダッシュマンかどうか
                ? st_.pstMyTm_->COMDt.comPtn[comDMCallCH]
                : st_.pstMyTm_->COMDt.comPtn[comDMCallNC];

            //優劣による判断
            switch (dmc)
            {
            case 0://劣勢ならする
                dmcall_f = st_.pstMyTm_->COMDt.Ressei_f;
                break;
            case 1://する
                dmcall_f = TRUE;
                break;
            case 2://しない
                dmcall_f = FALSE;
                break;
            default://劣勢でないなら
                dmcall_f = !(st_.pstMyTm_->COMDt.Ressei_f);
                break;
            }
        }

        //dmcall_f = TRUE;//★テスト

        if (dmcall_f == FALSE) return;

        //セッター
        s32 setterNo = NGNUM;


        //セッター指定
        switch (st_.pstMyTm_->COMDt.comPtn[comDMSetter])
        {
        case 0://一番前方の内野
            setterNo = GetFrontInfPNo();
            break;
        case 1://内野最下位オーダー
            setterNo = GetBottomInfPNo();
            break;
        case 2://ボス
            setterNo = st_.pstMyTm_->GetRdPos();
            //ボスが退場しているときは待っても無理
            if (st_.pmgMyCh_[setterNo]->st_.pstMyCh_->Dead_f
                || st_.pmgMyCh_[setterNo]->st_.pstMyCh_->ANGEL_f)
            {
                waitcallOK_f = FALSE;
            }
            break;
        default://外野（自分が内野ならば一番近い外野）
            setterNo = GetNearGaiyaPNo();
            break;
        }


        //セッターチェック
        BOOL setterCk_f = (st_.pstMyTm_->CallingDMReset_f == FALSE)
            && ((st_.pstMyTm_->COMDt.comPtn[comDMSetBM] == 0) || (st_.pstMyTm_->COMDt.comPtn[comDMSetBM] == 3));

        //セッターチェックする
        if (setterCk_f)
        {
            //セッターが参加できない
            if (setterNo == NGNUM)
            {
                nowcallOK_f = FALSE;
            }
            else
            {
                //操作権キャラ以外がセッター
                if (st_.pstMyTm_->CtrlNo != setterNo)
                {
                    //セッターがフリーモーション
                    nowcallOK_f = (st_.pmgMyCh_[setterNo]->chCommon_.IsFreeMotion(TRUE)
                        && (st_.pmgMyCh_[setterNo]->st_.pstMyCh_->Motion.IsMFlags(dbmfAr) == FALSE));
                }
            }
        }
        else//セッターチェックしない
        {
            setterNo = GetBallmanNo();//マニュアルもいるので、持ってる人に
            //setterNo = st_.pstMyTm_->CtrlNo;//操作キャラがセッターになる
            //dmcall_f = TRUE;
        }



        s32 higher_c = 0;//セッターより上位の人数

        if (setterNo != NGNUM)//セッターの人がいる場合
        {
            for (s32 i = 0; i < DBMEMBER_INF; ++i)
            {
                if (st_.pstMyTm_->PosMove.DashmanStock[i] == NGNUM) continue;
                //セッターより後衛オーダーが居るとき
                if (setterNo > st_.pstMyTm_->PosMove.DashmanStock[i])
                {
                    ++higher_c;
                }
            }
        }

        const s32 DM_3MEN = 3;//３メン
        const s32 DM_2MEN = 2;//２メン
        const s32 DM_1MEN = 1;//シングル


        //セッターボールマンダッシュＯＫ
        BOOL setterBMOK_f = FALSE;

        if (setterNo != NGNUM)
        {
            BOOL dsok_f = (st_.pmgMyCh_[setterNo]->IsMAN()//マニュアル
                || (st_.pmgMyCh_[setterNo]->IsDMOKPos() && (st_.pstMyTm_->PosMove.Postman != setterNo)));//セッターが走れる位置でポストマンではない

            setterBMOK_f = (IsLastOne() == FALSE) //残り１人の時は無理
                && (setterNo != NGNUM)//セッターの人がいる場合
                && (setterNo < DBMEMBER_INF)//内野
                && dsok_f
                && ((st_.pstMyTm_->COMDt.comPtn[comDMSetBM] == 0) || (st_.pstMyTm_->COMDt.comPtn[comDMSetBM] == 1));//セッター走る作戦
        }
        //ダッシュマン参加可能人数
        s32 dmoknum = st_.pstMyTm_->PosMove.PassAbleNum;//現状の待機人数

        //セッターが内野でセッター参加可能で、セッターがＢＭＤ可能位置にいる場合、一人足す
        if (setterBMOK_f)
        {
            ++dmoknum;
        }

        s32 dmcallNum = 0;//コール人数
        s32 usesetBM_f = FALSE;//セッターをボールマンダッシュさせる


        //いけいけのときは２メンとかで１人しか呼べなくても呼ぶ
        if (st_.pstMyTm_->CallingDMReset_f)
        {
            if (st_.pstMyTm_->PosMove.PassAbleNum > 0)//呼べる人が誰かいる
            {
                //ダッシュマン発動人数
                switch (st_.pstMyTm_->COMDt.comPtn[comDMNum])
                {
                case 0://３メン
                    dmcallNum = lib_num::Min(DM_3MEN, st_.pstMyTm_->PosMove.PassAbleNum);
                    break;
                case 1://マックス
                    dmcallNum = st_.pstMyTm_->PosMove.PassAbleNum;
                    break;
                case 2://シングル
                    dmcallNum = DM_1MEN;
                    break;
                case 3://２メン
                    dmcallNum = lib_num::Min(DM_2MEN, st_.pstMyTm_->PosMove.PassAbleNum);
                    break;
                }
                //usesetBM_f = setterBMOK_f;
            }
            else
            {
                nowcallOK_f = FALSE;
                //if (GetLiveNum() < (DM_1MEN + 1))//生きてる人が二人未満
                //{
                //  waitcallOK_f = FALSE;
                //}
            }
        }
        else
        {
            //ダッシュマン発動人数
            switch (st_.pstMyTm_->COMDt.comPtn[comDMNum])
            {
            case 0://３メン
                if (dmoknum >= DM_3MEN)//３人以上
                {
                    dmcallNum = DM_3MEN;

                    if ((DM_3MEN > higher_c) && setterBMOK_f)//上位で３メン足りない
                    {
                        usesetBM_f = TRUE;
                        --dmcallNum;//呼ぶ人減らす
                    }
                }
                else
                {
                    nowcallOK_f = FALSE;
                    if (GetLiveNum() < DM_3MEN)//生きてる人が３人いない
                    {
                        waitcallOK_f = FALSE;
                    }
                }
                break;
            case 1://マックス
                if (st_.pstMyTm_->PosMove.PassAbleNum > 0)//呼べる人が誰かいる
                {
                    dmcallNum = st_.pstMyTm_->PosMove.PassAbleNum;
                    usesetBM_f = setterBMOK_f;
                }
                else
                {
                    nowcallOK_f = FALSE;
                    if (GetLiveNum() < (DM_1MEN + 1))//生きてる人が二人未満
                    {
                        waitcallOK_f = FALSE;
                    }
                }
                break;
            case 2://シングル
                if (dmoknum >= DM_1MEN)// && (higher_c >= DM_1MEN))//自分より上位が居る１人いる
                {
                    dmcallNum = DM_1MEN;
                }
                else
                {
                    nowcallOK_f = FALSE;
                    if (GetLiveNum() < (DM_1MEN + 1))//生きてる人が二人未満
                    {
                        waitcallOK_f = FALSE;
                    }
                }
                break;
            default://２メン
                if (dmoknum >= DM_2MEN)
                {
                    dmcallNum = DM_2MEN;

                    if ((DM_2MEN > higher_c) && setterBMOK_f)//上位で３メン足りない
                    {
                        usesetBM_f = TRUE;
                        --dmcallNum;//呼ぶ人減らす
                    }
                }
                else
                {
                    nowcallOK_f = FALSE;
                    if (GetLiveNum() < DM_2MEN)//生きてる人が２人いない
                    {
                        waitcallOK_f = FALSE;
                    }
                }
                break;
            }
        }


        if (dmcallNum == 0) nowcallOK_f = FALSE;

        //チュートリアルではダッシュマン呼ばない
        if (pmgEO_->stSeq_.IsTutoRetro() || pmgEO_->stSeq_.IsTutoSmart() || pmgEO_->stSeq_.IsTutoAuto()) nowcallOK_f = FALSE;

        //疑似対人でもダッシュマン呼ばない
        if (st_.pmgRf_->IsTour()) nowcallOK_f = FALSE;

        //ここで発動ＯＫＮＧ完全判断
        if (nowcallOK_f)//現状発動出来る(オフェンスタイミングと態勢ウエイト後の２回ここに来る)
        {
            st_.pstMyTm_->COMDt.dashman_f = TRUE;
        }
        else if (waitcallOK_f && (st_.pstMyTm_->CallingDMReset_f == FALSE))//待てば行ける
        {
            st_.pstMyTm_->COMDt.dashmanFmWait_f = TRUE;
        }

        //ダッシュマンではない
        if (st_.pstMyTm_->COMDt.dashman_f == FALSE)
        {
            return;
        }

        //コール人数
        st_.pstMyTm_->COMDt.dmcallNum = dmcallNum;
        //セッター
        st_.pstMyTm_->COMDt.setterNo = setterNo;
        //既にセッターの手元かどうか
        st_.pstMyTm_->COMDt.setterOK_f = (st_.pstMyTm_->COMDt.setterNo == GetBallmanNo());

        //セッターダッシュ可能で待つ
        st_.pstMyTm_->COMDt.setterBMOKWait_f = setterBMOK_f;

        //セッターがボールマンダッシュするかどうか
        st_.pstMyTm_->COMDt.setterBMRsv_f = (setterBMOK_f && usesetBM_f);


        ////セッターがマニュアルでボールマンダッシュ必要なら既にＯＫとする
        //st_.pstMyTm_->COMDt.setterBMRsvGo_f = (st_.pmgMyCh_[setterNo]->IsMAN() && st_.pstMyTm_->COMDt.setterBMRsv_f);

        //態勢ウエイト挟まったときは、その分を通常ウエイトに移行
        if (st_.pstMyTm_->COMDt.dashmanFmWait_f)
        {
            st_.pstMyTm_->COMDt.dmwait_c = st_.pstMyTm_->COMDt.dmfmwait_c;
        }

        //即ＯＫ
        if (st_.pstMyTm_->CallingDMReset_f)
        {
            st_.pstMyTm_->COMDt.callOK_f = TRUE;
        }

    }


    //COMダッシュマン作戦人数再確認
    BOOL TMgTeam::COMDashmanNumRecheck()
    {

        BOOL res = FALSE;
        s32 higher_c = 0;//セッターより上位の人数

        if (st_.pstMyTm_->COMDt.setterNo != NGNUM)//セッターの人がいる場合
        {
            for (s32 i = 0; i < DBMEMBER_INF; ++i)
            {
                if (st_.pstMyTm_->PosMove.DashmanStock[i] == NGNUM) continue;
                //セッターより後衛オーダーが居るとき
                if (st_.pstMyTm_->COMDt.setterNo > st_.pstMyTm_->PosMove.DashmanStock[i])
                {
                    ++higher_c;
                }
            }
        }

        //ダッシュマン参加可能人数
        s32 dmoknum = st_.pstMyTm_->PosMove.PassAbleNum;//現状の待機人数

        //セッターが内野でセッター参加可能で、セッターがＢＭＤ可能位置にいる場合、一人足す
        if (st_.pstMyTm_->COMDt.setterBMOKWait_f)
        {
            ++dmoknum;
        }

        s32 dmcallNum = 0;//コール人数
        s32 usesetBM_f = FALSE;//セッターをボールマンダッシュさせる

        const s32 DM_3MEN = 3;//３メン
        const s32 DM_2MEN = 2;//２メン
        const s32 DM_1MEN = 1;//シングル


        //ダッシュマン発動人数
        switch (st_.pstMyTm_->COMDt.comPtn[comDMNum])
        {
        case 0://３メン
            if (dmoknum >= DM_3MEN)//３人以上
            {
                dmcallNum = DM_3MEN;

                if ((DM_3MEN > higher_c) && st_.pstMyTm_->COMDt.setterBMOKWait_f)//上位で３メン足りない
                {
                    usesetBM_f = TRUE;
                    --dmcallNum;//呼ぶ人減らす
                }
            }
            break;
        case 1://マックス
            if (dmoknum > 0)//呼べる人が誰かいる
            {
                dmcallNum = st_.pstMyTm_->PosMove.PassAbleNum;
                usesetBM_f = st_.pstMyTm_->COMDt.setterBMOKWait_f;
            }
            break;
        case 2://シングル
            if (dmoknum >= DM_1MEN)// && (higher_c >= DM_1MEN))//自分より上位が居る１人いる
            {
                dmcallNum = DM_1MEN;
            }
            break;
        default://２メン
            if (dmoknum >= DM_2MEN)
            {
                dmcallNum = DM_2MEN;

                if ((DM_2MEN > higher_c) && st_.pstMyTm_->COMDt.setterBMOKWait_f)//上位で３メン足りない
                {
                    usesetBM_f = TRUE;
                    --dmcallNum;//呼ぶ人減らす
                }
            }
            break;
        }


        if (dmcallNum > 0)
        {
            //コール人数
            st_.pstMyTm_->COMDt.dmcallNum = dmcallNum;

            //セッターがボールマンダッシュするかどうか
            st_.pstMyTm_->COMDt.setterBMRsv_f = (st_.pstMyTm_->COMDt.setterBMOKWait_f && usesetBM_f);

            //全員さわってないことに
            for (s32 i = 0; i < DBMEMBER_INF; ++i)
            {
                if (i == st_.pstMyTm_->COMDt.setterNo)
                {
                    st_.pstMyTm_->COMDt.actdt[i].BallTouched_f = TRUE;
                }
                else
                {
                    st_.pstMyTm_->COMDt.actdt[i].BallTouched_f = FALSE;
                }
            }

            //コールＯＫ
            res = TRUE;
        }
        else
        {
            st_.pstMyTm_->RsvResetCOM_f = TRUE;
        }


        return res;
    }

    //セミオート切り替え
    void TMgTeam::SemiSwitch()
    {
        //スマート操作
        st_.pstMyTm_->smart_f_ = st_.pSidePad_->IsSmart();

        BOOL semi_f = st_.pSidePad_->IsSemiAuto();
        if (mid::midIsSemiAuto() && st_.pSidePad_->IsSemiAuto())
        {
           
            //セミオート操作
            st_.pstMyTm_->semi_f_ = semi_f;
            //セミオートオーダー
            st_.pstMyTm_->semi_order_f_ = (st_.pstMyTm_->semi_f_ && st_.pSidePad_->IsSemiAutoOrder());
        }
        else
        {
            //セミオート操作
            st_.pstMyTm_->semi_f_ = FALSE;
            //セミオートオーダー
            st_.pstMyTm_->semi_order_f_ = FALSE;
        }


        
        //セミオート用キャッチ補正
        
        if ((st_.pmgMyTm_->st_.pstMyTm_->CtrlNo >= 0)
            && (st_.pmgMyTm_->st_.pstEnTm_->CtrlNo >= 0))
        {
            if ((st_.pmgBa_->st_.pstBa_->ShTgTNo == st_.mysideNo_)
                || (st_.pmgBa_->st_.pstBa_->HoldTNo == st_.ensideNo_))
            {
                TMgChar* pHitman = st_.pmgMyTm_->st_.pmgMyCh_[st_.pmgMyTm_->st_.pstMyTm_->CtrlNo];
                s32 hitman_tech = pHitman->MyStLv(dbst_Tech);

                s32 tecrank = 0;
                if (st_.pmgBa_->st_.pstBa_->Motion == bmShoot)
                {
                    tecrank = pmgEO_->mgDt_.dtRank_.GetRankTech(st_.pmgBa_->st_.pstSh_->idata.sharp, hitman_tech);
                }
                else
                {
                    TMgChar* pShotman = st_.pmgMyTm_->st_.pmgMyCh_[st_.pmgMyTm_->st_.pstEnTm_->CtrlNo];
                    s32 shotman_tech = pShotman->MyStLv(dbst_Tech);
                    tecrank = pmgEO_->mgDt_.dtRank_.GetRankTech(shotman_tech, hitman_tech);
                }
                st_.pstMyTm_->semi_CaPos_ = pmgEO_->mgDt_.dtRank_.GetRankDtTech(rkSemiCatchTime, tecrank);
                st_.pstMyTm_->semi_NCaLen_ = pmgEO_->mgDt_.dtRank_.GetRankDtTech(rkSemiNiceCatchTime, tecrank);
            }
        }
    }

    //セミオートキャッチ★★★★
    void TMgTeam::SemiCatch()
    {

        //キャッチ中
        if (st_.pstMyTm_->semi_Catching_c_ > 0)
        {
            if (lib_num::AprTo0(&(st_.pstMyTm_->semi_Catching_c_)))
            {
                st_.pstMyTm_->semi_Catch_c_ = 0;
                //st_.pstMyTm_->semi_Catch_c_ = 64;//1秒でＯＫか//グラの都合で128の約数に
            }
        }

        if (st_.pstMyTm_->semi_Catching_c_ == 0)
        {
            //セミオートキャッチカウンター減少
            lib_num::AprTo0(&(st_.pstMyTm_->semi_Catch_c_));

            if (st_.pstMyTm_->semi_f_ == FALSE) return;

            if ((st_.pstMyTm_->semi_Catch_c_ == 0)
                && (st_.pstMyTm_->ofside_f == FALSE)//敵チームボール
                && st_.pSidePad_->IsTouch())//タッチ入力（おしっぱst_.pSidePad_->IsBtn2(base::bA)になっていたので瞬間でＯＫかチェック）
            {
                st_.pstMyTm_->semi_Catch_c_ = 64;//1秒でＯＫか//グラの都合で128の約数に
            }
        }

    }

    //セミオートシュート★★★★
    void TMgTeam::SemiShot()
    {
        //カウンター減少
        lib_num::AprTo0(&(st_.pstMyTm_->semi_Shot_c_));

        if (st_.pstMyTm_->ofside_f == TRUE)
        {
            if (st_.pSidePad_->IsTouch())//タッチ入力
            {
                st_.pstMyTm_->semi_Shot_f_ = TRUE;
                st_.pstMyTm_->semi_Shot_c_ = 32;
            }
        }
        else
        {
            st_.pstMyTm_->semi_Shot_f_ = FALSE;
        }
    }

    //セミオートＡＩ★★★★
    void TMgTeam::SemiAI()
    {
        if (mid::midIsSemiAuto())
        {
            if (st_.pstMyTm_->semi_f_ == FALSE)
            {
                st_.pstMyTm_->semi_AI_ = 0;
                //セミオートここでＣＯＭ番号変えられる
                //COM番号保存
                st_.pstMyTm_->comNo_ = pmgGO_->pmgCh_[st_.mysideNo_][st_.pstMyTm_->RdNo]->MyPDt(clv_COM);
            }
            else
            {
              st_.pstMyTm_->semi_AI_ = st_.pSidePad_->GetCmdNo();//作戦入力
              switch (st_.pstMyTm_->semi_AI_)
              {
              case 1:st_.pstMyTm_->comNo_ = 22;  break;//37
              case 2:st_.pstMyTm_->comNo_ = 28;  break;//27
              case 3:st_.pstMyTm_->comNo_ = 35;  break;//32
              //case 4:st_.pstMyTm_->comNo_ = 35;  break;//35
              default:st_.pstMyTm_->comNo_ = 36;  break;
              }
            }
        }
        else
        {
            st_.pstMyTm_->semi_AI_ = 0;
        }
    }

    //シュートチェンジ
    void TMgTeam::Debug_ChangeShot()
    {
        if (pmgGO_->pmgRf_->IsPrev()) return;
        if (mid::midIsDXL())
        {
            s32 mv = 0;
            s32 flyLv = 0;
            s32 res = 0;
            if (st_.pSidePad_->IsDXLBtn(dxFnY)) mv += 100;
            if (st_.pSidePad_->IsDXLBtn(dxFnU)) mv += 10;
            if (st_.pSidePad_->IsDXLBtn(dxFnI)) mv += 1;
            if (st_.pSidePad_->IsDXLBtn(dxFnH)) mv -= 100;
            if (st_.pSidePad_->IsDXLBtn(dxFnJ)) mv -= 10;
            if (st_.pSidePad_->IsDXLBtn(dxFnK)) mv -= 1;
            if (st_.pSidePad_->IsDXLBtn(dxFn0)) flyLv += 1;//吹っ飛びレベル

            //ファイル読み込み
            if (st_.pSidePad_->IsDXLBtn(dxFn9)) pmgEO_->mgDt_.Refresh();

            if (mv != 0)
            {
                //pGame_->mgGO_.pmgBa_->st_.pstSh_->

                for (s32 i = 0; i < DBSIDE; ++i)
                {
                    for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
                    {
                        res = pGame_->mgGO_.pmgCh_[i][i2]->chCommon_.ChangeStShotDebug(mv);
                    }
                }
                //        mid::midSetDbgMsg(4, res, "ChangeShot");
                //mid::midLog("ChangeShot %d\n", res);
            }
            s32 res2 = 0;
            if (flyLv != 0)
            {
                pGame_->mgGO_.pmgBa_->st_.pstBa_->debugFlyRankLv += flyLv;
                if (pGame_->mgGO_.pmgBa_->st_.pstBa_->debugFlyRankLv >= 16) pGame_->mgGO_.pmgBa_->st_.pstBa_->debugFlyRankLv = 0;
                res2 = pGame_->mgGO_.pmgBa_->st_.pstBa_->debugFlyRankLv;
                //        mid::midSetDbgMsg(5, res2, "flyLv");
            }
        }
    }

#ifdef __K_DEBUG_SHIAI__
    //シュートチェンジ
    void TMgTeam::Debug_SetShot(void)
    {
        if (pmgGO_->pmgRf_->IsPrev()) return;
        if (mid::midIsDXL())
        {
            for (s32 i = 0; i < DBSIDE; ++i)
            {
                for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
                {
                    TStChGen* st = &(pGame_->mgGO_.pmgCh_[i][i2]->chCommon_.st_);

                    // ここにデバッグの値を持ってきてshotIdにセットする
                    s32 shotId = kdebug::DebugSystem::GetInstance()->GetShootNo();
                    s32 maxid = pmgEO_->mgDt_.dtShSyn_.GetIntvec2Size() - 1;
                    if (shotId > maxid) shotId = maxid;
                    if (shotId < 0) shotId = 0;
                    //所持合成シュート（仮）
                    for (s32 i = 0; i < STEPNUM; ++i)
                    {
                        st->pstMyCh_->stGousei.set_f_[i] = TRUE;
                        for (s32 i2 = 0; i2 < HSELEMMAX; ++i2)
                        {
                            st->pstMyCh_->stGousei.shot_[i].elem_[i2] = pmgEO_->mgDt_.dtShSyn_.GetDt(shotId, (enDtShSyn)(gs_base + (i2 * 2)));
                            st->pstMyCh_->stGousei.shot_[i].rare_[i2] = pmgEO_->mgDt_.dtShSyn_.GetDt(shotId, (enDtShSyn)(gs_base_rare + (i2 * 2)));
                            st->pstMyCh_->stGousei.shot_[i].shotId = shotId;//デバッグ用データ
                            st->pstMyCh_->stGousei.shot_[i].pow_ = pmgEO_->mgDt_.dtShSyn_.GetDt(shotId, (enDtShSyn)(gs_power));//デバッグ用データ
                            st->pstMyCh_->stGousei.shot_[i].spin_ = pmgEO_->mgDt_.dtShSyn_.GetDt(shotId, (enDtShSyn)(gs_spin));//デバッグ用データ
        }
    }
}
      }
    }
  }
#endif	// #ifdef __K_DEBUG_SHIAI__

    //COM思考サイコロふり直し
    void TMgTeam::COMSwitch()
    {
        //試合中のみ
        if (st_.pstRf_->DBSSt != dbssShiai) return;

        //行動切り替え
        BOOL switch_f = st_.pstMyTm_->MaruhiCall_f;
        BOOL nodsmn_f = FALSE;

        st_.pstMyTm_->CallingDMReset_f = FALSE;

        //ボールがもたれているとき
        if (pmgSG_->stBa_.Motion == bmHold)
        {
            //マイボール
            if (pmgSG_->stBa_.HoldTNo == st_.mysideNo_)
            {

                if (st_.pstMyTm_->noside_f)
                {
                    st_.pstMyTm_->noside_f = FALSE;
                    switch_f = TRUE;
                }

                if (st_.pstMyTm_->ofside_f == FALSE)
                {
                    switch_f = TRUE;
                    st_.pstMyTm_->ofside_f = TRUE;
                    st_.pstMyTm_->COMCall.Init();
                }
            }
            else//敵ボール
            {
                if (st_.pstMyTm_->noside_f)
                {
                    st_.pstMyTm_->noside_f = FALSE;
                    switch_f = TRUE;
                }

                if (st_.pstMyTm_->ofside_f)
                {
                    switch_f = TRUE;
                    st_.pstMyTm_->ofside_f = FALSE;
                    st_.pstMyTm_->COMCall.Init();
                }
            }
            st_.pstMyTm_->randreset_f = TRUE;
        }
        else
        {
            if ((pmgSG_->stBa_.Motion == bmBound)
                || (pmgSG_->stBa_.Motion == bmFree)
                || (pmgSG_->stBa_.Motion == bmReferee)
                || (pmgSG_->stBa_.Motion == bmShoot))
            {
                st_.pstMyTm_->noside_f = TRUE;
            }
            else if ((pmgSG_->stBa_.Motion == bmPass) && (pmgSG_->stBa_.PaTgTNo != st_.mysideNo_))
            {
                if (pmgSG_->stBa_.PaTgPNo >= DBMEMBER_INF)//タゲが外野
                {
                    if (pmgSG_->stBa_.PichPNo < DBMEMBER_INF)//投手が内野
                    {
                        st_.pstMyTm_->noside_f = TRUE;
                    }
                }
                else
                {
                    if (pmgSG_->stBa_.PichPNo >= DBMEMBER_INF)//投手が外野
                    {
                        st_.pstMyTm_->noside_f = TRUE;
                    }
                }
            }

            if (st_.pstMyTm_->randreset_f)
            {
                //通信対戦の時は呼ばないようにしてみる
                if (pmgEO_->stShiai_.IsVSHum() == FALSE)
                {
                    const s32 DGTOCAPER = 50;
                    //st_.pstMyTm_->COMRnd.BallTake_f = pmgGO_->pmgCommon_->GetRandPer(st_.pstMyTm_->COMDt.BallTakePer);//COMボール拾い行動確率
                    st_.pstMyTm_->COMRnd.DgToCa_f = pmgGO_->pmgCommon_->GetRandPer(DGTOCAPER);//回避不能→キャッチシフト

                    for (s32 i = 0; i < DBMEMBER_INF; ++i)
                    {
                        //一部サイコロふり直し
                        st_.pstMyTm_->COMDt.actdt[i].comactPtn[cmaPassCut] //パスカット
                            = GetComAct(st_.pstMyTm_->comNo_, cmaPassCut, st_.pstMyTm_->COMDt.Ressei_f, st_.pmgMyCh_[i]->st_.pstMyCh_->id_char);
                        st_.pstMyTm_->COMDt.actdt[i].comactPtn[cmaCatchJdg] //キャッチ対応
                            = GetComAct(st_.pstMyTm_->comNo_, cmaCatchJdg, st_.pstMyTm_->COMDt.Ressei_f, st_.pmgMyCh_[i]->st_.pstMyCh_->id_char);

                        st_.pstMyTm_->COMRnd.RndAct[i].DgType = (enDodgeType)GetComAct(st_.pstMyTm_->comNo_, cmaDgType, st_.pstMyTm_->COMDt.Ressei_f, st_.pmgMyCh_[i]->st_.pstMyCh_->id_char);
                        st_.pstMyTm_->COMRnd.RndAct[i].DgTime = pmgGO_->pmgCommon_->GetRand(st_.pmgMyCh_[i]->RankLevel(rkDodgeTime));
                    }
                }
                st_.pstMyTm_->randreset_f = FALSE;
            }
        }

        //どちらサイドでもなくなったら丸秘開始はオフ
        if (st_.pstMyTm_->noside_f
            || (st_.pstMyTm_->ofside_f == FALSE))
        {
            st_.pstMyTm_->CallingDashman_f2 = FALSE;
        }

        //Callingによる作戦変更
        if ((switch_f == FALSE)
            && st_.pstMyTm_->CallingDashman_f
            //&& (st_.pstMyTm_->CallingDashman_f2 == FALSE)
            )
        {
            st_.pstMyTm_->CallingDashman_f = FALSE;
            st_.pstMyTm_->CallingDashman_f2 = TRUE;
            st_.pstMyTm_->CallingDashman_f3 = FALSE;
            //ダッシュマン作戦ではないばあい
            if (st_.pstMyTm_->COMDt.dashman_f == FALSE)
            {
                st_.pstMyTm_->CallingDMReset_f = TRUE;
            }
            else//ダッシュマンの時はウエイト削るか
            {
                //まだコールＯＫが出てなければ短縮してまるひ表示
                if (st_.pstMyTm_->COMDt.callOK_f == FALSE)
                {
                    //COMDM通常ウエイト
                    st_.pstMyTm_->COMDt.DMWait = 1;
                    //COMDMセッターウエイト
                    st_.pstMyTm_->COMDt.DMStWait = 1;
                    //セッターを自分に
                    st_.pstMyTm_->COMDt.setterNo = GetBallmanNo();

                    ////まるひさくせん
                    //SESet(seDmOK);
                    //TZahyouRec* pzh = &(st_.pmgMyCh_[GetBallmanNo()]->st_.pstMyCh_->Zahyou);
                    //PopupMsg(pu_Call, pmgEO_->mgDt_.dtScenario_.GetDBShiaiStrDt(mndbMaruhi).c_str(),
                    //  st_.mysideNo_, pzh->X, pzh->Y, pzh->Z, FC_Red);
                }
                else
                {
                    //SESet(seDmNG);//ＮＧ音
                    //mid::midLog("ng5\n");
                }
            }
        }

        ////ボールマンをリーダーに
        //if (pmgEO_->mgDt_.dtTeam_.GetDt2(dbtm_OpeFomChange, st_.teamNo_) == 3)
        //{
        //  if (st_.pstMyTm_->ofside_f && (st_.pstMyTm_->LastBallmanNo != GetBallmanNo()))
        //  {
        //    s32 tmprdno = st_.pstMyTm_->RdNo;
        //    //ダッシュマンが居ないとき
        //    if ((st_.pstMyTm_->PosMove.DashmanNum == 0)
        //        && (st_.pstMyTm_->MaruhiCall_f == FALSE))
        //    {
        //      if (GetBallmanNo() != NGNUM)
        //      {
        //        st_.pstMyTm_->RdNo = st_.pstMyTm_->PosSt.CharofP[GetBallmanNo()];
        //      }
        //    }
        //    if (tmprdno != st_.pstMyTm_->RdNo) PopupFomChange();
        //    st_.pstMyTm_->LastBallmanNo = GetBallmanNo();
        //  }
        //}


        //サイコロふり直し
        if (switch_f
            || st_.pstMyTm_->RsvResetCOM_f
            || st_.pstMyTm_->CallingDMReset_f
            || st_.pstMyTm_->CallingRdCg_f)
        {

            //オフェンスの時にリーダーチェンジ処理
            if (st_.pstMyTm_->ofside_f
                && switch_f
                && (st_.pstMyTm_->CallingRdCg_f == FALSE))
            {
                ReaderChange();
            }

            //まず空に
            st_.pstMyTm_->COMDt.Init();

            //乱数取得
            for (s32 i = 0; i < comEND; ++i)
            {
                st_.pstMyTm_->COMDt.comPtn[i] = GetCom((enCom)i);
            }

            st_.pstMyTm_->COMDt.Ressei_f2 = IsRessei();//実は劣勢
            BOOL ec_f = (st_.pstMyTm_->Encr_c > 0);

            if (st_.pstMyTm_->COMDt.Ressei_f2)// && (ec_f == FALSE))
            {
                st_.pstMyTm_->COMDt.FomRev = +1;//強制的に+1
            }
            if (ec_f)
            {
                st_.pstMyTm_->COMDt.FomRev = -1;//強制的に-1
            }

            //最終的な劣勢判断
            st_.pstMyTm_->COMDt.Ressei_f = (ec_f)
                ? FALSE
                : st_.pstMyTm_->COMDt.Ressei_f2;


            //乱数取得
            for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
            {

                //st_.pstMyTm_->COMDt.actdt[i2].CallPaOK_f = GetCallPaOK(st_.pstMyTm_->comNo_);
                ////個人性格によって、パスしない場合
                //st_.pstMyTm_->COMDt.actdt[i2].CallPaOK_f = (pmgGO_->pmgCommon_->GetRandPer(pmgEO_->mgDt_.dtChar_.GetCoopStop(st_.pmgMyCh_[i2]->st_.id_)) == FALSE);
                //
                //st_.pstMyTm_->COMDt.actdt[i2].comactNo = GetComActNo(i2);
                for (s32 i = 0; i < cmaEND; ++i)
                {
                    st_.pstMyTm_->COMDt.actdt[i2].comactPtn[i]
                        = GetComAct(st_.pstMyTm_->comNo_, (enDtComAct)i, st_.pstMyTm_->COMDt.Ressei_f, st_.pmgMyCh_[i2]->st_.pstMyCh_->id_char);
                }

            }

            //チャンス判断
            ChanceCheck();

            //AI予測シュート
            s32 dai = pmgSG_->stBa_.AIDShDt.GetExp();
            s32 jai = pmgSG_->stBa_.AIJShDt.GetExp();
            s32 rai = pmgGO_->pmgCommon_->GetRand(AISHTYPENUM);

            for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
            {
                st_.pstMyTm_->COMDt.actdt[i2].comactPtn[cmaDShStep] = (dai == NGNUM)
                    ? rai
                    : dai;
                st_.pstMyTm_->COMDt.actdt[i2].comactPtn[cmaJShTime] = (jai == NGNUM)
                    ? rai
                    : jai;
            }

            //カウンターキャッチ成功
            if (st_.pstMyTm_->CtrlNo != NGNUM)
            {
                st_.pstMyTm_->COMDt.CCaSc_f = st_.pmgMyCh_[st_.pstMyTm_->CtrlNo]->st_.pstMyCh_->COMCounter_f;
            }
            else
            {
                st_.pstMyTm_->COMDt.CCaSc_f = FALSE;
            }

            //COMDM通常ウエイト
            st_.pstMyTm_->COMDt.DMWait = (st_.pstMyTm_->CallingDMReset_f)
                ? 1
                : pmgEO_->mgDt_.dtFomation_.GetComDt(cmvDMWait, st_.pstMyTm_->COMDt.comPtn[comDMWait]);
            //COMDM態勢ウエイト
            st_.pstMyTm_->COMDt.DMFmWait = pmgEO_->mgDt_.dtFomation_.GetComDt(cmvDMFmWait, st_.pstMyTm_->COMDt.comPtn[comDMWait]);
            //COMDMセッターウエイト
            st_.pstMyTm_->COMDt.DMStWait = (st_.pstMyTm_->CallingDMReset_f)
                ? 1
                : pmgEO_->mgDt_.dtFomation_.GetComDt(cmvDMStWait, st_.pstMyTm_->COMDt.comPtn[comDMTrigger]);

            //COMDMコールディレイ
            st_.pstMyTm_->COMDt.DMDelay = pmgEO_->mgDt_.dtFomation_.GetComDt(cmvDMDelay, st_.pstMyTm_->COMDt.comPtn[comDMDelay]);
            //COMDMパスインターバル
            st_.pstMyTm_->COMDt.DMPaItv = pmgEO_->mgDt_.dtFomation_.GetComDt(cmvDMPaItv, st_.pstMyTm_->COMDt.comPtn[comDMPaItv]);
            //COMチャンスメイクパスループ
            st_.pstMyTm_->COMDt.ChMakeLoop = pmgEO_->mgDt_.dtFomation_.GetComDt(cmvChMakeLoop, st_.pstMyTm_->COMDt.comPtn[comChMakeLoop]);




            //個人アクション
            for (s32 i = 0; i < DBMEMBER_ALL; ++i)
            {
                //パス回数
                st_.pstMyTm_->COMDt.actdt[i].SgOdPa_f = (st_.pstMyTm_->COMDt.actdt[i].comactPtn[cmaAction] >= 2);

                //シュート変更パス回数（行動後変更するため）
                st_.pstMyTm_->COMDt.actdt[i].ShCg = (st_.pstMyTm_->RsvResetCOM_f == FALSE)
                    ? (enShChangeType)(st_.pstMyTm_->COMDt.actdt[i].comactPtn[cmaShChange])
                    : scNone;

                //COMシングルパスウエイト
                st_.pstMyTm_->COMDt.actdt[i].PaWait
                    = pmgEO_->mgDt_.dtFomation_.GetComDt(cmvSgPaWait, st_.pstMyTm_->COMDt.actdt[i].comactPtn[cmaPaWait]);

                //COMDShウエイト
                st_.pstMyTm_->COMDt.actdt[i].ShWait
                    = pmgEO_->mgDt_.dtFomation_.GetComDt(cmvShWait, st_.pstMyTm_->COMDt.actdt[i].comactPtn[cmaShWait]);

                //COMDSh歩数
                st_.pstMyTm_->COMDt.actdt[i].DShStep
                    = pmgEO_->mgDt_.dtFomation_.GetComDt(cmvDShStep, st_.pstMyTm_->COMDt.actdt[i].comactPtn[cmaDShStep]);

                //COMJShタイミング
                st_.pstMyTm_->COMDt.actdt[i].JShTime
                    = pmgEO_->mgDt_.dtFomation_.GetComDt(cmvJShTime, st_.pstMyTm_->COMDt.actdt[i].comactPtn[cmaJShTime]);

                //COMJShタイミング2
                st_.pstMyTm_->COMDt.actdt[i].JShTime2
                    = pmgEO_->mgDt_.dtFomation_.GetComDt(cmvJShTime2, st_.pstMyTm_->COMDt.actdt[i].comactPtn[cmaJShTime]);

                //COMDs開始位置
                st_.pstMyTm_->COMDt.actdt[i].DsPos
                    = (DBCRT_CLI - (pmgEO_->mgDt_.dtFomation_.GetComDt(cmvDsPos, st_.pstMyTm_->COMDt.actdt[i].comactPtn[cmaDsPos]) * XYMAG));

                if (st_.pstMyTm_->semi_f_ && st_.pstMyTm_->semi_order_f_)
                {
                    st_.pstMyTm_->COMDt.actdt[i].JShTime = 32;
                    st_.pstMyTm_->COMDt.actdt[i].JShTime2 = 32;
                }
            }


            //COMパス元ダッシュ継続
            st_.pstMyTm_->COMDt.DsKeep_f = (st_.pstMyTm_->COMDt.comPtn[comDMPaNum] == 1)//パス無限
                && ((st_.pstMyTm_->COMDt.comPtn[comDMNum] == 0) || (st_.pstMyTm_->COMDt.comPtn[comDMNum] == 3));//３メンと２メン
            ////COMパス元ダッシュ継続
            //st_.pstMyTm_->COMDt.DsKeep_f = ((st_.pstMyTm_->COMDt.comPtn[comDMNum] == 0) || (st_.pstMyTm_->COMDt.comPtn[comDMNum] == 3));//３メンと２メン

            //COMインターバル一つ待つ
            st_.pstMyTm_->COMDt.DsOneItvWait_f = (st_.pstMyTm_->COMDt.comPtn[comDMNum] == 2);//シングルの時はインターバル一つ待つ

            //ヘルプガード
            st_.pstMyTm_->COMDt.help_f = (st_.pstMyTm_->ofside_f == FALSE);//ディフェンスで

            //チャンスメイク回数
            st_.pstMyTm_->COMDt.chpass_c = st_.pstMyTm_->COMDt.ChMakeLoop;


            //リセットしたときはクイック行動
            st_.pstMyTm_->COMDt.Quick_f = st_.pstMyTm_->RsvResetCOM_f;

            //ダッシュマン作戦(リーダーチェンジとリセットとジャンプボールゲットとカウンターキャッチ成功以外)
            if (st_.pstMyTm_->CallingRdCg_f)
            {
                st_.pstMyTm_->COMDt.dashman_f = FALSE;
                st_.pstMyTm_->COMDt.dashmanFmWait_f = FALSE;
            }
            else if (st_.pstMyTm_->CallingDMReset_f)
            {
                COMDashman();
            }
            else if ((st_.pstMyTm_->COMDt.Chance_f == FALSE)
                //&& (st_.pstMyTm_->COMDt.ncChance_f == FALSE)
                && (st_.pstMyTm_->RsvResetCOM_f == FALSE)
                && (st_.pstMyTm_->getJpball_f == FALSE)
                && (st_.pstMyTm_->COMDt.CCaSc_f == FALSE))
            {
                COMDashman();
            }
            //else
            //{
            //作戦リセット時★

            st_.pstMyTm_->COMDt.comReset_f = st_.pstMyTm_->RsvResetCOM_f;
            st_.pstMyTm_->RsvResetCOM_f = FALSE;
            st_.pstMyTm_->getJpball_f = FALSE;
            //}

            st_.pstMyTm_->COMOverTime_f = FALSE;
        }

    }

    //COMカウンタ操作
    void TMgTeam::COMCounter()
    {
        const s32 RESETTIME = 240;
        const s32 SHOTTIME = 240;

        //投げ終わりカウンタを減らす
        lib_num::AprTo0(&st_.pstMyTm_->COMDt.shAfter_c);

        //エンカレカウンタを減らす
        lib_num::AprTo0(&st_.pstMyTm_->Encr_c);

        lib_num::AprTo0(&st_.pstMyTm_->TutoWait_c);


        //COMのみ
        if (IsCOM() == FALSE) return;

        if (st_.pmgRf_->IsReturnEnSide())
        {
            if (st_.pstBa_->Motion != bmHold)
            {
                st_.pstMyTm_->TutoWait_c = 30;
            }

            if (st_.pstMyTm_->TutoWait_c == 0)
            {
                //どっかおかしくなって、6秒かかったら強制シュート
                st_.pstMyTm_->COMDt.comatcSt_f = TRUE;//行動開始
                st_.pstMyTm_->COMOverTime_f = TRUE;
            }
            return;
        }

        if (st_.pstMyTm_->COMDt.comatcSt_f == FALSE)
        {
            if (pmgSG_->stBa_.Motion == bmHold)//ボールがもたれているとき
            {
                //マイボール
                if (pmgSG_->stBa_.HoldTNo == st_.mysideNo_)
                {
                    //持っている人が戻り中（外野強奪）の場合はまだマイボール扱いしない
                    if (st_.pmgMyCh_[pmgSG_->stBa_.HoldPNo]->IsFreeMotion(TRUE))
                    {
                        st_.pstMyTm_->COMDt.comatcSt_f = TRUE;//行動開始
                    }
                }
            }
        }


        //行動開始してない
        if (st_.pstMyTm_->COMDt.comatcSt_f == FALSE) return;

        //ダッシュマン以外
        if (st_.pstMyTm_->COMDt.Isdm() == FALSE)//態勢待ちでもない
        {
            //ＣＯＭ行動カウンタ
            lib_num::IncLoop(&st_.pstMyTm_->COMDt.tmwait_c, 0, 0xFFFF);
            //st_.pstMyTm_->COMDt.dmstop_f = FALSE;

            if (st_.pstMyTm_->COMDt.tmwait_c > SHOTTIME)
            {
                //どっかおかしくなって、6秒かかったら強制シュート
                st_.pstMyTm_->COMOverTime_f = TRUE;
            }
        }
        else//ダッシュマン作戦中（待ち含む）
        {

            //ダッシュマン態勢ウエイト中
            if ((st_.pstMyTm_->COMDt.dashman_f == FALSE)
                && (st_.pstMyTm_->COMDt.dashmanFmWait_f))
            {
                if (st_.pstMyTm_->COMDt.dmfmwait_c <= st_.pstMyTm_->COMDt.DMFmWait)
                {

                    //ダッシュマン作戦を常に確認
                    COMDashman();

                    //結果的にダッシュマン作戦にならなかった
                    if (st_.pstMyTm_->COMDt.dashman_f == FALSE)
                    {
                        //ダッシュマン態勢ウエイト進行
                        if (lib_num::AprToDest(&st_.pstMyTm_->COMDt.dmfmwait_c, st_.pstMyTm_->COMDt.DMFmWait))
                        {
                            st_.pstMyTm_->COMDt.dashmanFmWait_f = FALSE;//待ちでもなくなる
                            st_.pstMyTm_->COMDt.tmwait_c = st_.pstMyTm_->COMDt.dmfmwait_c;//行動カウンタに態勢待ち時間をシフト
                            st_.pstMyTm_->COMDt.dmstop_f = TRUE;
                        }
                    }

                }
            }


            //ダッシュマン作戦決定
            if (st_.pstMyTm_->COMDt.dashman_f)
            {
                //ダッシュマン通常ウエイト
                if (st_.pstMyTm_->COMDt.dmwaitOK_f == FALSE)
                {
                    //ダッシュマン通常ウエイト
                    if (lib_num::AprToDest(&st_.pstMyTm_->COMDt.dmwait_c, st_.pstMyTm_->COMDt.DMWait))
                    {
                        st_.pstMyTm_->COMDt.dmwaitOK_f = TRUE;

                        //セッターウエイトを通常ウエイトから移行
                        if ((st_.pstMyTm_->COMDt.comPtn[comDMTrigger] == 0)
                            || (st_.pstMyTm_->COMDt.comPtn[comDMTrigger] == 1))
                        {
                            st_.pstMyTm_->COMDt.dmstwait_c = st_.pstMyTm_->COMDt.dmwait_c;
                        }
                    }
                }
                else
                {
                    //既にセッターの手元かどうか
                    if (st_.pstMyTm_->COMDt.setterOK_f == FALSE)
                    {
                        if (st_.pstMyTm_->COMDt.setterNo == GetBallmanNo())
                        {
                            st_.pstMyTm_->COMDt.setterOK_f = TRUE;
                        }
                        //セッターへのパスチェックでセッターＯＫ出すのもここでやる
                    }

                    //セッターＯＫ
                    if (st_.pstMyTm_->COMDt.setterOK_f && (st_.pstMyTm_->COMDt.callOK_f == FALSE))
                    {
                        lib_num::AprToDest(&st_.pstMyTm_->COMDt.dmstwait_c, st_.pstMyTm_->COMDt.DMStWait);
                        //セッターウエイトがＯＫ
                        if (st_.pstMyTm_->COMDt.dmstwait_c >= st_.pstMyTm_->COMDt.DMStWait)
                        {
                            //ダッシュマン現状走れる人数を再チェック
                            st_.pstMyTm_->COMDt.callOK_f = COMDashmanNumRecheck();
                        }
                    }

                    //コールＯＫ
                    if (st_.pstMyTm_->COMDt.callOK_f)
                    {
                        //コールカウンタ
                        lib_num::IncLoop(&st_.pstMyTm_->COMDt.dmclwait_c, 0, 0xFFFF);
                        //パスインターバルカウンタ
                        lib_num::IncLoop(&st_.pstMyTm_->stCOM.dmpawait_c, 0, 0xFFFF);
                    }

                    //呼んだ人がいるのにダッシュマンが居ない場合はリセット
                    if ((st_.pstMyTm_->COMDt.dmcalledNum > 0)
                        && (st_.pstMyTm_->PosMove.DashmanNum == 0))
                    {
                        st_.pstMyTm_->RsvResetCOM_f = TRUE;//沼尾かが止まる★
                    }

                    //ダッシュマンパスしない&(呼ぶ人呼んだらor呼べないとき合ったら)ダッシュマン作戦終了
                    if ((st_.pstMyTm_->COMDt.comPtn[comDMPaNum] == 2)
                        && ((st_.pstMyTm_->COMDt.dmcalledNum >= st_.pstMyTm_->COMDt.dmcallNum)
                        || st_.pstMyTm_->COMDt.callNG_f))
                    {
                        st_.pstMyTm_->RsvResetCOM_f = TRUE;
                    }


                    if (lib_num::AprToDest(&st_.pstMyTm_->COMDt.dmall_c, RESETTIME))
                    {
                        //どっかおかしくなって、4秒かかったらＣＯＭ思考リセット
                        st_.pstMyTm_->RsvResetCOM_f = TRUE;
                    }
                }
            }
        }
    }

    //劣勢判断
    BOOL TMgTeam::IsRessei()
    {
        BOOL res = FALSE;
        switch (st_.pstMyTm_->COMDt.comPtn[comRessei])
        {
        case 0://倒され人数で負けてる
            res = (st_.pmgEnTm_->InfNum() < InfNum());//こちらの方が倒された人数が多い
            break;
        case 1://瀕死が居る
            res = (PinchNum() > 0);
            break;
        case 2://ボスが完全退場している→天使で良いのでは
            //res = pmgGO_->pmgCh_[st_.mysideNo_][0]->st_.pstMyCh_->Dead_f;//ボスが居ない
            res = (pmgGO_->pmgCh_[st_.mysideNo_][st_.pstMyTm_->RdNo]->st_.pstMyCh_->Dead_f)
                || (pmgGO_->pmgCh_[st_.mysideNo_][st_.pstMyTm_->RdNo]->st_.pstMyCh_->ANGEL_f);//ボスが居ない
            break;
        default://内野が残り一人
            res = (IsLastOne());//最後の一人状態
            break;
        }

        return res;
    }

    //チャンスパス判断
    BOOL TMgTeam::IsChancePass()
    {
        BOOL res = FALSE;
        //チャンスパス判断
        switch (st_.pstMyTm_->COMDt.comPtn[comChancePa])
        {
        case 0://劣勢ならする
            res = st_.pstMyTm_->COMDt.Ressei_f;
            break;
        case 1://チャンスチェックする
            res = TRUE;
            break;
        case 2://チャンスチェックしない
            res = FALSE;
            break;
        default://劣勢ならしない
            res = !(st_.pstMyTm_->COMDt.Ressei_f);
            break;
        }

        return res;
    }

    //チャンスターゲット
    s32 TMgTeam::ChanceTag()
    {
        s32 res = NGNUM;
        //チャンスチェックする場合、チャンス狙い

        for (s32 i = 0; i < DBMEMBER_INF; ++i)//上から検索かけていく
        {
            //ねらえない
            if (st_.pmgEnTm_->st_.pmgMyCh_[i]->IsShTgOK() == FALSE) continue;

            if ((st_.pmgEnTm_->st_.pmgMyCh_[i]->Kagami_f())//かがみ
                || (st_.pmgEnTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Motion.IsMFlags(dbmfDn)))//ダウン系
            {
                res = i;//チャンスタゲ
                break;
            }
        }

        return res;
    }

    //チャンス判断
    void TMgTeam::ChanceCheck()
    {
        st_.pstMyTm_->COMDt.ngTagNo = NGNUM;
        st_.pstMyTm_->COMDt.vnTagNo = NGNUM;
        //操作権あるとき
        if (st_.pstMyTm_->CtrlNo == NGNUM) return;

        s32 posno = st_.pstMyTm_->CtrlNo;
        TMgChar* pobj = st_.pmgMyCh_[posno];
        TStChar* pst = pobj->st_.pstMyCh_;

        BOOL nca_f = FALSE;// (pst->NiceCounter_c > 0);

        //ガード不能検索
        for (s32 i = 0; i < DBMEMBER_INF; ++i)//上から検索かけていく
        {
            //ねらえない
            if (st_.pmgEnTm_->st_.pmgMyCh_[i]->IsShTgOK() == FALSE) continue;

            if (st_.pmgEnTm_->st_.pmgMyCh_[i]->IsNoGuard(FALSE))//ノーガード
            {
                st_.pstMyTm_->COMDt.ngTagNo = i;//ガード不能敵タゲ
                st_.pstMyTm_->COMDt.ngChance_f = TRUE;
                break;
            }
        }


        //非常に近い人検索
        for (s32 i = 0; i < DBMEMBER_INF; ++i)//上から検索かけていく
        {
            //ねらえない
            if (st_.pmgEnTm_->st_.pmgMyCh_[i]->IsShTgOK() == FALSE) continue;
            //非常に近い
            const s32 VERYNEAR = 32 * XYMAG;
            if ((VERYNEAR > abs(st_.pmgEnTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.X - pst->Zahyou.X))
                && (VERYNEAR > abs(st_.pmgEnTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z - pst->Zahyou.Z)))
            {
                st_.pstMyTm_->COMDt.vnTagNo = i;//非常に近い敵タゲ
                st_.pstMyTm_->COMDt.vnChance_f = TRUE;
                break;
            }
        }



        //チャンスパス＆ノーガードがいる
        if (IsChancePass() && st_.pstMyTm_->COMDt.ngChance_f) st_.pstMyTm_->COMDt.Chance_f = TRUE;

        //チャンスシュート//キャッチ不能ならする	する	しない	キャッチ不能ならしない//★
        switch (st_.pstMyTm_->COMDt.actdt[posno].comactPtn[cmaChanceSh])
        {
        case 0://キャッチ不能ならする
            if (st_.pstMyTm_->COMDt.ngChance_f)
            {
                st_.pstMyTm_->COMDt.Chance_f = TRUE;
            }
            break;
        case 1://する
            if (st_.pstMyTm_->COMDt.ngChance_f || st_.pstMyTm_->COMDt.vnChance_f)
            {
                st_.pstMyTm_->COMDt.Chance_f = TRUE;
            }
            break;
        case 2://しない
            //st_.pstMyTm_->Chance_f = FALSE;
            break;
        default://キャッチ不能ならしない(近い場合のみ)
            if (st_.pstMyTm_->COMDt.vnChance_f && (st_.pstMyTm_->COMDt.ngChance_f == FALSE))
            {
                st_.pstMyTm_->COMDt.Chance_f = TRUE;
            }
            break;
        }

    }

    //待ち時間超えてる
    BOOL TMgTeam::IsWaitOver()
    {
        BOOL res = FALSE;
        if (st_.pmgEnTm_->IsAllNoShTg())//誰もねらえない
        {
            res = FALSE;
        }
        else if (st_.pstMyTm_->stCOM.quick_f)//クイック
        {
            res = TRUE;
        }
        else if (st_.pstMyTm_->stCOM.sh_f)//シュート
        {
            if (st_.pstMyTm_->CtrlNo != NGNUM)
            {
                res = (st_.pstMyTm_->COMDt.tmwait_c > st_.pstMyTm_->COMDt.actdt[st_.pstMyTm_->CtrlNo].ShWait);
            }
            else
            {
                res = (st_.pstMyTm_->COMDt.tmwait_c > st_.pstMyTm_->COMDt.actdt[st_.pstMyTm_->GetRdPos()].ShWait);
            }

        }
        else//パス
        {
            if (st_.pstMyTm_->CtrlNo != NGNUM)
            {
                res = (st_.pstMyTm_->COMDt.tmwait_c > st_.pstMyTm_->COMDt.actdt[st_.pstMyTm_->CtrlNo].PaWait);
            }
            else
            {
                res = (st_.pstMyTm_->COMDt.tmwait_c > st_.pstMyTm_->COMDt.actdt[st_.pstMyTm_->GetRdPos()].PaWait);
            }
        }

        return res;
    }

    //ダッシュマンパスタイミング
    BOOL TMgTeam::IsDMPaItvTime(BOOL infsetter_f)
    {
        //全員呼ぶのが同時でないため、セッターで呼び切れてないときは若干補正
        if (infsetter_f && (st_.pstMyTm_->COMDt.dmcalledNum < st_.pstMyTm_->COMDt.dmcallNum))
        {
            //パスインターバル後
            return (st_.pstMyTm_->stCOM.dmpawait_c > st_.pstMyTm_->COMDt.DMPaItv + (15));
        }
        else
        {
            //パスインターバル後
            return (st_.pstMyTm_->stCOM.dmpawait_c > st_.pstMyTm_->COMDt.DMPaItv);
        }


    }

    //ダッシュマン発動タイミング
    BOOL TMgTeam::IsDMDelayTime()
    {
        //０だと初期化時点でそうなので１にする
        if (st_.pstMyTm_->COMDt.DMDelay == 0)
        {
            return FALSE;
        }
        else
        {
            return ((st_.pstMyTm_->COMDt.dmclwait_c % st_.pstMyTm_->COMDt.DMDelay) == 1);
        }
    }

    //動作
    void TMgTeam::TeamMove()
    {
        //if (st_.mysideNo_ == 0)
        //{
        //  st_.pSidePad_->ppad_->Nowdxpad();
        //  mid::midSetDbgMsg(2, (s32)st_.pSidePad_->ppad_->Nowdxpad(), "tmpad");
        //}

        //操作権キャラ
        NowCtrlNo();

        //ポジショニング予定位置計算ポジショニングカウンタ減少
        GetEnC_c();

        //ポジショニング
        SetPosLine();

        //ダッシュマンストック
        SetDashmanStock();

        //ボールの位置
        TeamBallPos();
        TeamBallPos2();

        //ボール拾いマン検索
        SeekGetter();

        //セミオート
        SemiSwitch();
        //セミオートキャッチ
        SemiCatch();
        //セミオートシュート
        SemiShot();
        //セミオートＡＩ切り替え
        SemiAI();

        //デバッグ用シュート切り替え
        Debug_ChangeShot();

        //COM思考サイコロふり直し（ポストマンが決まってる必要があるのでポジショニングの後）
        COMSwitch();

        //COMのみカウンタ操作
        COMCounter();

        //ダッシュマン指令
        OrderDashman();
    }

    //試合中の名前
    const s32 SIDEDIST = 136;
    const s32 BASEX = -184;
    const s32 BASEY = -104;
    const s32 FACEX = BASEX + 72;
    const s32 NAMEX = FACEX + 11;
    const s32 FACEY = BASEY + 8;
    const s32 NAMEY = FACEY - 8;
    //const s32 HPX = NAMEX + 48;// FACEX + 56;
    //const s32 HPY = NAMEY + 10;// FACEY - 7;
    //const s32 STEPX = NAMEX;
    //const s32 STEPY = FACEY + 2;
    const s32 HPX = NAMEX;// FACEX + 56;
    const s32 HPY = FACEY + 9;
    const s32 STEPX = NAMEX + 48;
    const s32 STEPY = FACEY + -6;

    void TMgTeam::HPDraw(s32 drawcno)
    {
        //for (s32 i = 0; i < DBMEMBER_INF; ++i)
        //{
        //  s32 id = pmgGO_->pmgCh_[st_.mysideNo_][i]->st_.pstMyCh_->id_char;

        //  //天使補正を掛けない
        //  s32 hp = pmgEO_->mgDt_.GetState(dbst_Pow, st_.pmgMyCh_[i]->st_.pstMyCh_->id_char);//0~15

        //  //ＨＰバーｐ
        //  pmgEO_->mgGrp_.pmgEtcGrp_->GraphDrawHP(
        //    HPX + (SIDEDIST * st_.mysideNo_),
        //    HPY + (i * 3),
        //    st_.pmgMyCh_[i]->st_.pstMyCh_->HP,
        //    pmgEO_->mgDt_.dtSet_.GetDtDamage(setHP),
        //    st_.pmgMyCh_[i]->IsCtrl());
        //}


        if (drawcno != NGNUM)
        {
            s32 id = pmgGO_->pmgCh_[st_.mysideNo_][drawcno]->st_.pstMyCh_->id_char;
            //天使補正を掛けない
            s32 hp = pmgEO_->mgDt_.GetState(dbst_Pow, st_.pmgMyCh_[drawcno]->st_.pstMyCh_->id_char);//0~15

            //ＨＰバーｐ
            pmgEO_->mgGrp_.pmgEtcGrp_->GraphDrawHP(
                HPX + (SIDEDIST * st_.mysideNo_),
                HPY,
                st_.pmgMyCh_[drawcno]->st_.pstMyCh_->HP,
                pmgEO_->mgDt_.dtSet_.GetDtDamage(setHP),
                FALSE);
        }

        //for (s32 i = 0; i < DBMEMBER_INF; ++i)
        //{

        //  if (st_.pmgMyCh_[i]->IsCtrl() == FALSE) continue;

        //  s32 id = pmgGO_->pmgCh_[st_.mysideNo_][i]->st_.pstMyCh_->id_char;
        //  //天使補正を掛けない
        //  s32 hp = pmgEO_->mgDt_.GetState(dbst_Pow, st_.pmgMyCh_[i]->st_.pstMyCh_->id_char);//0~15

        //  //ＨＰバーｐ
        //  pmgEO_->mgGrp_.pmgEtcGrp_->GraphDrawHP(
        //    HPX + (SIDEDIST * st_.mysideNo_),
        //    HPY,
        //    st_.pmgMyCh_[i]->st_.pstMyCh_->HP,
        //    pmgEO_->mgDt_.dtSet_.GetDtDamage(setHP),
        //    FALSE);
        //}
    }

    void TMgTeam::FaceDraw(s32 drawcno)
    {
        if (drawcno == NGNUM) return;
        //if (st_.pmgMyTm_->st_.pstMyTm_->CtrlNo == NGNUM) return;
        //選手情報
        //s32 i = st_.pmgMyTm_->st_.pstMyTm_->CtrlNo;
        //s32 charNo = st_.pstMyTm_->PosSt.CharofP[i];
        BOOL mono_f = FALSE;
        BOOL mir_f = (st_.mysideNo_ != 0);
        s32 iconposx = FACEX + (SIDEDIST * st_.mysideNo_);
        s32 posy = GRIDSIZE;

        pmgEO_->mgGrp_.pmgIcGrp_->OrderDrawIconHead(st_.mysideNo_, drawcno, mono_f,
            FACEX + (SIDEDIST * st_.mysideNo_),
            FACEY);
    }

    //試合中の名前
    void TMgTeam::SetShiaiName()
    {
        //選手
        for (s32 i = 0; i < DBMEMBER_ALL; ++i)
        {
            s32 nameposy = (2 + 0);

            s32 wsNo = (DBMEMBER_ALL * st_.mysideNo_) + i;
            s32 wdtNo = (DBMEMBER_ALL * st_.mysideNo_) + st_.pstMyTm_->PosSt.CharofP[i];

            pmgEO_->mgFont_.OrderWordSprite(wsNo, wdtNo,
                NAMEX + (SIDEDIST * st_.mysideNo_),
                NAMEY,
                base::fd_None,
                base::DPT_3BUTTON);

            pmgEO_->mgFont_.FixWordSprite_Draw(wsNo, FALSE);

        }
    }
    void TMgTeam::StepDraw(s32 drawcno, s32 posno)
    {
        if (drawcno == NGNUM) return;
        if (posno == NGNUM) return;


        for (s32 i = 0; i < DBMEMBER_ALL; ++i)
        {
            //if (st_.pmgMyTm_->st_.pstMyTm_->CtrlNo != i) continue;
            s32 id = pmgGO_->pmgCh_[st_.mysideNo_][drawcno]->st_.pstMyCh_->id_char;

            s32 rare[RARITYNUM];
            for (s32 i2 = 0; i2 < STEPNUM; ++i2)
            {
                rare[i2] = (st_.pmgMyCh_[posno]->st_.pstMyCh_->stGousei.set_f_[i2])
                    ? st_.pmgMyCh_[posno]->st_.pstMyCh_->stGousei.shot_[i2].rare_[0]
                    : NGNUM;
            }
            s32 vj = (st_.pmgMyCh_[posno]->st_.pstMyCh_->stGousei.vjset_f_)
                ? st_.pmgMyCh_[posno]->st_.pstMyCh_->stGousei.vjshot_.rare_[0]
                : NGNUM;

            pmgEO_->mgGrp_.pmgEtcGrp_->DrawStep(
                STEPX + (SIDEDIST * st_.mysideNo_),
                STEPY,
                rare[0],
                rare[1],
                rare[2],
                NGNUM,
                rare[4],
                rare[5],
                rare[6],
                vj);
            break;
        }
    }
    void TMgTeam::RedrawShiaiName(s32 drawcno)
    {
        //選手
        for (s32 i = 0; i < DBMEMBER_ALL; ++i)
        {
            s32 wsNo = (DBMEMBER_ALL * st_.mysideNo_) + i;
            BOOL draw_f = (drawcno == i);
            pmgEO_->mgFont_.FixWordSprite_Draw(wsNo, draw_f);
        }
    }
    //描画
    void TMgTeam::Draw()
    {
        if ((st_.pmgRf_->IsNoDamage() == FALSE) && st_.pmgRf_->IsPrev() == FALSE)
        {
            s32 drawpos = st_.pmgMyTm_->st_.pstMyTm_->CtrlNo;//キャラ

            //直近ダメージキャラ優先
            if (st_.pmgMyTm_->st_.pstMyTm_->dam_c_ > 0)
            {
                --st_.pmgMyTm_->st_.pstMyTm_->dam_c_;
                drawpos = st_.pmgMyTm_->st_.pstMyTm_->dampos_;//キャラ
                //drawcno = st_.pmgMyTm_->st_.pstMyTm_->damchar_;// st_.pstMyTm_->PosSt.PosofC[st_.pmgMyTm_->st_.pstMyTm_->damchar_];
            }


            HPDraw(drawpos);//ポジ
            FaceDraw(st_.pstMyTm_->PosSt.CharofP[drawpos]);//キャラ
            StepDraw(st_.pstMyTm_->PosSt.CharofP[drawpos], drawpos);//キャラ
            RedrawShiaiName(drawpos);//ポジ

        }

    }
}