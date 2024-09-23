namespace db;

public partial class CharaBehavior
{
    private void PlaySe(SeType seType)
    {
        if (seType == SeType.None)
        {
            return;
        }
        SoundManager.Instance.PlaySe(seType);
    }

    private void CallRefereeWhistleOverLine()
    {
        RefereeBehaviorManager.Instance.Get().CallWhistleOverLine(MySideIndex);
    }

    private void CallRefereeResetOverLine()
    {
        RefereeBehaviorManager.Instance.Get().CallResetOverLine(MySideIndex);
    }

    private void CallBallShootMotion()
    {
        // // TODO:ボール状態の変更位置確認
        // BallState.ShotMotion = true;
    }

    private void CallTeamGenerateAngel()
    {
        TeamBehaviorManager.Instance.Get(MySideIndex).CallGenerateAngel(MyState.Order.OrderIndex);
    }

    private void CallTeamCheckChangePos()
    {
       MyTeamBehavior.CallCheckChangePos();
    }
    
}