﻿using System;
using isdf;

namespace db;

public partial class CharaBehavior
{
    /// <summary>
    /// //ＣＯＭ思考行動（パス、シュート）★★
    /// </summary>
    private void ComAction()
    {
        //アクション中は来ない
        //ボールマンだけ
        if (Motion.HasFlag(CharaMotionFlag.Act)
            || IsBallHolder == false)
        {
            return;
        }

        if (MyTeam.SemiAuto.SemiShotF
            || MyTeam.MainState.ComOverTimeF)
        {
            COMShootAct();
            // MyTeam.SemiAuto.SemiShotF = false;
        }
        else if (MyTeam.AiMain.Isdm)
        {
            COMAction_DM();
        }
        else
        {
            COMAction_Std();
        }
    }

    void COMShootAct()
    {
        //ノーガードは狙わない場合
        bool paok_f = (MyTeam.MainState.ComOverTimeF == false);

        bool ngshng_f = false;

        bool shtgnone_f = BallState.ShotTargetOrder == OrderIndexType.Disabled;

        if (shtgnone_f == false)
        {
            ngshng_f = MyTeam.PatternState.GetActionPattern(MyOrderIndex, ComActionType.cmaChanceSh) == ActionGroupType.Type3PosiPassive
                       && CharaBehaviorManager.Instance.GetChara(EnemySideIndex, BallState.ShotTargetOrder).IsNoGuard(true);
        }

        //セミオートでオーダーでシュート命令が出てないとき
        bool semiPass_f = MyTeam.SemiAuto.SemiF
                          && MyTeam.SemiAuto.SemiOrderF
                          && MyTeam.SemiAuto.SemiShotF == false;

        bool pass = semiPass_f || (paok_f && (ngshng_f || shtgnone_f));

        if (pass)
        {
            // MyComState.AttackState.PaTag = MyOrder.IsInfield
            //     ? GetNaiyaPassTag()
            //     : GetGaiyaPassTag();

            COMPass(false); //シュート切り替えパス
        }
        else
        {
            //タゲ方向向く
            //居ないときはオートで探す
            LookTg(BallState.ShotTargetOrder, false, true);
            switch (Motion.MotionType)
            {
                case CharaMotionType.St or CharaMotionType.Wk or CharaMotionType.Ds:
                    SetMotionType(CharaMotionType.Sh);
                    break;
                case CharaMotionType.JUp or CharaMotionType.JDn:
                    SetMotionType(CharaMotionType.JSh);
                    break;
            }
        }
    }

    //ＣＯＭパス
    void COMPass(bool dmpass_f)
    {
        var paTag = Order.IsInfield
            ? GetNaiyaPassTag()
            : GetGaiyaPassTag();

        //パスタゲがパス出せないとき
        SetPassTarget(paTag);

        //パスカットキャラセット
        PaCtTagSet(paTag);
        //タゲの方を向く
        LookTg(BallState.ShotTargetOrder, true, false);

        switch (Motion.MotionType)
        {
            case CharaMotionType.St or CharaMotionType.Wk or CharaMotionType.Ds:
                SetMotionType(CharaMotionType.Pa);
                break;
            case CharaMotionType.JUp or CharaMotionType.JDn:
                SetMotionType(CharaMotionType.JPa);
                break;
        }
    }

    void COMAction_DM()
    {
        //ダッシュマン作戦に完全に入ってる必要がある（態勢ウエイトはなにもしない）
        if (MyTeam.AiMain.DashmanF == false)
        {
            return;
        }

        //シュート指示が出てる
        if (MyTeam.AiMain.DashmanShOkF == false)
        {
            //飛ばないダッシュマン
            if (MyTeam.IsNoJpDashman())
            {
                //歩数過ぎたら投げてイイ
                if (Shoot.StepValue >= MyTeamAiAction.DShStep
                    || LeftCourtX > MyTeam.GetAtcLineX(false, true)) //ジャンプしないダッシュマンはこのラインが最大限界
                {
                    COMAction_DM_Sh();
                }
            }
            else
            {
                COMAction_DM_Sh();
            }
        }
        else //パス回し
        {
            COMAction_DM_Pa(); //パス回し
        }
    }

    void COMAction_DM_Sh() //シュート指示が出てる
    {
        //空中攻撃
        if (MyTeam.AiAttack.JpF
            || Motion.HasFlag(CharaMotionFlag.Ar))
        {
          
            if (Motion.HasFlag(CharaMotionFlag.Ar) //ジャンプ状態
                && (Air.AirCountValue >= MyTeamAiAction.JShTime)) //予定時間こえてる
            {
                //シュート
                COMShootAct();
            }
        }
        else //地上攻撃
        {
            
            if ((Order.IsInfield == false) //外野
                || (st_.pstMyCh_->Step_c >= st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].DShStep) //予定歩数超えた
                || (GetLeftCrtX() > st_.pmgMyTm_->GetAtcLineX(false, TRUE))) //センターライン超えそう
            {
                //シュート
                COMShootAct();
            }
        }
    }

    void COMAction_DM_Pa() //パス回し
    {
        //既にメンバー走り出しに入ってる
        if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.callOK_f)
        {
            //ダッシュマンパス（タイミングなどは関数内）
            COMDMPassAct();
        }
        else if (st_.pmgMyTm_->st_.pstMyTm_->COMDt.setterOK_f == false) //セッターにボールが渡ってない
        {
            //空中攻撃
            if (st_.pmgMyTm_->st_.pstMyTm_->stCOM.jp_f
                || st_.pstMyCh_->Motion.IsMFlags(dbmfAr))
            {
                if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr) //ジャンプ状態
                    && (st_.pstMyCh_->Air_c >= st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].JShTime)) //予定時間こえてる
                {
                    COMPass(false); //セッターパス
                }
            }
            else //地上攻撃
            {
                COMPass(false); //セッターパス
            }
        }
    }

    void COMAction_Std()
    {
    }

}
