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

    private void StopHomingSe()
    {
        SoundManager.Instance.StopHomingSe();
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
        TeamBehaviorManager.Instance.Get(MySideIndex).CallGenerateAngel(MyOrderIndex);
    }

    private void CallTeamCheckChangePos()
    {
        MyTeamBehavior.CallCheckChangePos();
    }

    private void CallTeamHoldBall()
    {
        TeamBehaviorManager.Instance.Get(MySideIndex).CallHoldBall(MyOrderIndex);
    }

    private void CallBallHold()
    {
        BallBehaviorManager.Instance.Get().CallHold(MySideIndex, MyOrderIndex);
    }
}