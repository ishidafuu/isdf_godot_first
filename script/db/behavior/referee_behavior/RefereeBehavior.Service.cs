namespace db;

public partial class RefereeBehavior
{

    public void CallWhistleOverLine(int sideNo)
    {
        // st_.pmgRf_->RefereeMsg("オーバーライン");
        // st_.pmgRf_->SetMotion(dbrfShortWhistle);
    }

    public void CallResetOverLine(int sideNo)
    {
        // st_.pmgRf_->RefereeMsg("オーバーライン");
        // st_.pmgRf_->SetMotion(dbrfShortWhistle);
        //                 if (IsBall())//攻撃時間オーバーとの重なり防止
        //                 {
        //                     //審判の元へ
        //                     if (st_.mysideNo_ == 0)
        //                     {
        //                         st_.pmgRf_->SetMotion(dbrfOvLine, mR);
        //                     }
        //                     else
        //                     {
        //                         st_.pmgRf_->SetMotion(dbrfOvLine, mL);
        //                     }
        //                     //タイマーリセット
        //                     st_.pmgBa_->baCommon_.ResetTimer(NGNUM, FALSE);
        //                 }
    }
}