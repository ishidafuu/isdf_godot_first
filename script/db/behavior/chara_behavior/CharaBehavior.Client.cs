namespace db;

public partial class CharaBehavior
{
    /// <summary>
    /// SE再生
    /// </summary>
    /// <param name="seType"></param>
    private void PlaySe(SeType seType)
    {
        if (seType == SeType.None)
        {
            return;
        }
        SoundManager.Instance.PlaySe(seType);
    }

    /// <summary>
    /// シュートSE停止
    /// </summary>
    private void StopHomingSe()
    {
        SoundManager.Instance.StopHomingSe();
    }

    /// <summary>
    /// オーバーライン
    /// </summary>
    private void CallRefereeWhistleOverLine()
    {
        RefereeBehaviorManager.Instance.Get().CallWhistleOverLine(MySideIndex);
    }

    /// <summary>
    /// オーバーラインリセット
    /// </summary>
    private void CallRefereeResetOverLine()
    {
        RefereeBehaviorManager.Instance.Get().CallResetOverLine(MySideIndex);
    }

    /// <summary>
    /// 天使生成
    /// </summary>
    private void CallTeamGenerateAngel()
    {
        TeamBehaviorManager.Instance.Get(MySideIndex).CallGenerateAngel(MyOrderIndex);
    }

    /// <summary>
    /// ポジションチェンジ
    /// </summary>
    private void CallTeamCheckChangePos()
    {
        MyTeamBehavior.CallCheckChangePos();
    }

    /// <summary>
    /// ボール保持
    /// </summary>
    private void CallTeamHoldBall()
    {
        TeamBehaviorManager.Instance.Get(MySideIndex).CallHoldBall(MyOrderIndex);
    }

    /// <summary>
    /// シュート状態
    /// </summary>
    private void CallBallShootMotion()
    {
        // // TODO:ボール状態の変更位置確認
        // BallState.ShotMotion = true;
    }

    /// <summary>
    /// ボール保持
    /// </summary>
    private void CallBallHold()
    {
        BallBehaviorManager.Instance.Get().CallHold(MySideIndex, MyOrderIndex);
    }

    /// <summary>
    /// パスターゲット変更
    /// </summary>
    /// <param name="passTargetOrderIndex"></param>
    private void CallBallChangePassTarget(OrderIndexType passTargetOrderIndex)
    {
        BallBehaviorManager.Instance.Get().CallChangePassTarget(MySideIndex, passTargetOrderIndex);
    }
}