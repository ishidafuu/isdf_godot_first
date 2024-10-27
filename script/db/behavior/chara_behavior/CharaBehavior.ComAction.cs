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
        if (MyMotion.HasFlag(CharaMotionFlag.Act)
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
        if (MyTeamState.SemiAutoState.SemiF
            && MyTeamState.SemiAutoState.SemiOrderF
            && MyTeamState.SemiAutoState.SemiShotF == false)
        {
            st_.pmgMyTm_->st_.pstMyTm_->stCOM.paTag = (IsInfield())
                ? pCommon_->GetNaiyaPassTag()
                : pCommon_->GetGaiyaPassTag();

            COMPass(false); //シュート切り替えパス
        }
        else if (paok_f && (ngshng_f || shtgnone_f))
        {
            st_.pmgMyTm_->st_.pstMyTm_->stCOM.paTag = (IsInfield())
                ? pCommon_->GetNaiyaPassTag()
                : pCommon_->GetGaiyaPassTag();

            COMPass(false); //シュート切り替えパス
        }
        else
        {
            //タゲ方向向く
            pCommon_->LookTg(pmgSG_->stBa_.ShTgPNo, false, TRUE); //居ないときはオートで探す
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

    void COMAction_DM()
    {
    }

    void COMAction_Std()
    {
    }
}