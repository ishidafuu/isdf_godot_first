using System;
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

        if (MyTeamState.SemiAutoState.SemiShotF
            || MyTeamState.MainState.ComOverTimeF)
        {
            COMShootAct();
            MyTeamState.SemiAutoState.SemiShotF = false;
        }
        else if (MyComState.MainState.Isdm())
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
        bool paok_f = (MyTeamState.MainState.ComOverTimeF == false);

        bool ngshng_f = false;

        bool shtgnone_f = BallState.ShotTargetOrder == OrderIndexType.Disabled;

        if (shtgnone_f == false)
        {
            ngshng_f = MyComState.PatternState.GetActionPattern(MyOrderIndex, ComActionType.cmaChanceSh) == ActionGroupType.Type3PosiPassive
                       && CharaBehaviorManager.Instance.GetChara(EnemySideIndex, BallState.ShotTargetOrder).IsNoGuard(true);
        }

        //セミオートでオーダーでシュート命令が出てないとき
        bool semiPass_f = MyTeamState.SemiAutoState.SemiF
                          && MyTeamState.SemiAutoState.SemiOrderF
                          && MyTeamState.SemiAutoState.SemiShotF == false;

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
        if (MyComState.MainState.DashmanF == false)
        {
            return;
        }

        //シュート指示が出てる
        if (MyComState.MainState.DashmanShOkF == false)
        {
            //飛ばないダッシュマン
            if (MyTeamBehavior.IsNoJpDashman())
            {
                //歩数過ぎたら投げてイイ
                if ((st_.pstMyCh_->Step_c >= st_.pmgMyTm_->st_.pstMyTm_->COMDt.actdt[st_.posNo_].DShStep)
                    || (GetLeftCrtX() > st_.pmgMyTm_->GetAtcLineX(FALSE, TRUE))) //ジャンプしないダッシュマンはこのラインが最大限界
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
                    || (GetLeftCrtX() > st_.pmgMyTm_->GetAtcLineX(FALSE, TRUE))) //ジャンプしないダッシュマンはこのラインが最大限界
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

    void COMAction_Std()
    {
    }
}