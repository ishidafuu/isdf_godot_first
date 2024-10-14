namespace db;

public partial class TeamBehavior
{

    public void CallGenerateAngel(int orderIndex)
    {
        // st_.pmgMyTm_->SetAngel(st_.posNo_);
    }

    public void CallCheckChangePos()
    {
    }

    public void CallHoldBall(int orderIndex)
    {
        // //チーム
        // st_.pmgMyTm_->st_.pstMyTm_->getJpball_f = (pmgSG_->stBa_.JumpBall == jbJpball);
        // st_.pmgMyTm_->st_.pstMyTm_->COMCall.CallingPa_f[st_.posNo_] = FALSE; //パス要求もクリア
        // //ボール触った
        // st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].BallTouched_f = TRUE;
    }

    //
    //     //内外野入れ替えが発生する人数
    // BOOL TMgTeam::IsIOChange()
    // {
    //     //元外野が012
    //     const s32 OUTNUM = (DBMEMBER_ALL - DBMEMBER_INF);
    //     return (st_.pstMyTm_->UseMotoGaiya_c < OUTNUM);
    // }
    // //内外野入れ替え確認
    // void TMgTeam::CheckChangePos()
    // {
    //
    //     BOOL change_f = FALSE;
    //
    //     for (s32 i = 0; i < DBMEMBER_INF; ++i)
    //     {
    //         //死亡確認
    //         if ((st_.pmgMyCh_[i]->st_.pstMyCh_->HP == 0)
    //             && (st_.pmgMyCh_[i]->st_.pstMyCh_->ANGEL_f)
    //             && (st_.pmgMyCh_[i]->st_.pstMyCh_->Dead_f == FALSE))
    //         {
    //
    //
    //             std::string out = st_.pmgMyCh_[i]->MyName() + "＿アウト";
    //             pmgGO_->pmgCommon_->PopupMsg(pu_DsMn, out.c_str(),
    //                 st_.mysideNo_, 0, 0, 0, FC2_shiai, FALSE);
    //
    //
    //             //倒された数カウンタ増やす
    //             ++st_.pstMyTm_->Dead_c;
    //
    //             BOOL IOChange_f = IsIOChange();//★FALSE;//
    //
    //             //サドンデス決着後は入れ替えしない
    //             if (st_.pmgRf_->IsSuddunDeath()
    //                 && (DeadNum() != st_.pmgEnTm_->DeadNum()))
    //             {
    //                 IOChange_f = FALSE;
    //             }
    //
    //             if (IOChange_f)//交換
    //             {
    //                 //天使になる人
    //                 //天使オブジェクト生成
    //                 //pmgGO_->pmgCommon_->SetEObj(etomAngel, st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou,
    //                 //                st_.mysideNo_,st_.pstMyTm_->PosSt.CharofP[i]);
    //
    //                 //SESet(seDead);//SEならす
    //
    //                 SetAngel(i);
    //
    //
    //                 st_.pmgMyCh_[i]->chCommon_.StInit(TRUE);
    //
    //                 //st_.pmgMyCh_[i]->chCommon_.SetMtype(dbmtJDn);
    //
    //                 st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Y = 0;
    //                 st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.X = (st_.mysideNo_ == 0)
    //                     ? DBCRT_W
    //                     : 0;
    //                 st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Muki = (st_.mysideNo_ == 0)
    //                     ? mL
    //                     : mR;
    //
    //                 change_f = TRUE;
    //                 switch (st_.pstMyTm_->IOChange_c)
    //                 {
    //                 case 0:
    //                     st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z = CRPOS_B_Z;//pmgEO_->mgDt_.dtSet_.GetDtFormation(setPos4_Z) * XYMAG;
    //                     break;
    //                 case 1:
    //                     st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z = CRPOS_F_Z;//pmgEO_->mgDt_.dtSet_.GetDtFormation(setPos5_Z) * XYMAG;
    //                     break;
    //                 case 2:
    //                     st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z = POS6_Z;
    //                     break;
    //                 default:
    //                     //change_f = FALSE;
    //                     break;
    //                 }
    //
    //                 //交換先の外野がボールを持ってる場合
    //                 const s32 GAIYANUM = 3;
    //                 s32 changePos = DBMEMBER_INF + (st_.pstMyTm_->IOChange_c % GAIYANUM);
    //                 if (st_.pmgMyCh_[changePos]->IsBall())
    //                 {
    //                     st_.pmgBa_->baCommon_.SetMtype(bmBound);//手放す
    //                     st_.pmgBa_->baCommon_.ZeroXYZ();
    //
    //                     pmgSG_->stBa_.NGGet_f = TRUE;
    //                     pmgSG_->stBa_.NGGetPNo = i;//入れ替え後のpos
    //                     pmgSG_->stBa_.NGGetTNo = st_.mysideNo_;
    //                 }
    //
    //                 //上田カウントが残っている場合
    //                 if (st_.pstMyTm_->TmUeda_c > 0)
    //                 {
    //                     --st_.pstMyTm_->TmUeda_c;//上田カウント減らす
    //                     st_.pmgMyCh_[i]->st_.pstMyCh_->ANGEL_f = FALSE;//天使の輪を消す
    //                 }
    //                 else
    //                 {
    //                     ++st_.pstMyTm_->UseMotoGaiya_c;//元外野を使う
    //                 }
    //
    //
    //                 //ポジションチェンジ//交換した回数に基づいた位置から
    //                 pmgSG_->ChangePos(st_.mysideNo_, i, changePos);
    //
    //                 //入れ替えたポジションのＨＰにリセット
    //                 //全員初期化の段階でHP入ってるので、現状ここでの差し替えは不要
    //                 if (st_.pmgRf_->IsSuddunDeath())
    //                 {
    //                     st_.pmgMyCh_[i]->st_.pstMyCh_->HP = 1;//サドンデスならＨＰ１に
    //                 }
    //
    //
    //                 //交換先の外野がパスタゲの場合、変わった後の人がパスタゲにならないように
    //                 if ((pmgSG_->stBa_.Motion == bmPass)
    //                     && (pmgSG_->stBa_.PaTgTNo == st_.mysideNo_)
    //                     && (pmgSG_->stBa_.PaTgPNo == changePos))
    //                 {
    //                     pmgSG_->stBa_.PaTgPNo = i;
    //                 }
    //
    //                 ++st_.pstMyTm_->IOChange_c;//交換した回数
    //             }
    //             else
    //             {
    //
    //                 st_.pmgMyCh_[i]->chCommon_.SetMtype(dbmtANG);
    //             }
    //
    //             ////誰か死んだらＣＯＭ思考リセット
    //             //st_.pstMyTm_->RsvResetCOM_f = TRUE;
    //         }
    //
    //     }
    //
    //     //内外野交代になったとき
    //     if (change_f && (st_.pstRf_->Demo_f == FALSE))
    //     {
    //         //名前
    //         SetShiaiName();
    //     }
    // }
}