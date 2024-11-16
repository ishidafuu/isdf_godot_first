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
        else
        {
            // 仮
            int tgx = GetSettingFormation(SettingFormationType.AtcLineX) + CaptainChara.Level.AttackLine;

            //ジャンプ踏切位置は少し後ろに
            if (jp_f)
            {
                const int JPLINE = 24;
                tgx += JPLINE;
            }

            return (tgx * Defines.Percent);
        }
    }

    //自チームの攻撃位置目標Ｘ★
    public int GetAtcLineX(bool jp_f, bool dsmn_f)
    {
        return (Defines.DBCRT_CL - GenGetAtcLineX(jp_f, dsmn_f));
    }
    
    /// <summary>
    /// 全員アウト
    /// </summary>
    public bool IsAllOut
    {
        get
        {
            foreach (var mySideChara in MySideCharas)
            {
                if (mySideChara.IsOut == false)
                {
                    return false;
                }
            }

            return true;
        }
    }

    public bool IsCom => SemiAuto.SemiF || MainState.ManSideF == false;

    
    //待ち時間超えてる
    public bool IsWaitOver()
    {
        bool res = false;
        if (st_.pmgEnTm_->IsAllNoShTg())//誰もねらえない
        {
            res = false;
        }
        else if (st_.pstMyTm_->stCOM.quick_f)//クイック
        {
            res = true;
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

}
