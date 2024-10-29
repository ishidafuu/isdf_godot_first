namespace db;

public partial class TeamBehavior
{
    //自チームのダッシュマンジャンプ不可
    public bool IsNoJpDashman()
    {
        return false;
        // return (pmgGO_->pmgCh_[st_.mysideNo_][st_.pstMyTm_->RdNo]->MyPDt(clv_DsmnNoJp) != 0);
    }

}