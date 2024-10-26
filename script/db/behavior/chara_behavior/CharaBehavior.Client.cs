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
    
    //効果音
    void PlaySeCatchSe()
    {
        // (int)SeType.CatchLv0 +  GetTechRank(RankTechType.CatchTime)% 7
       
        // SESetInt(seCatchLv0 + ((MyStLv(dbst_Tech) / 3) % 7));
        PlaySe(SeType.CatchLv0);
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
    /// 現在のカバーマンに操作権を渡す
    /// </summary>
    private void CallTeamChangeControlCoverMan()
    {
        TeamBehaviorManager.Instance.Get(MySideIndex).CallChangeControlCoverMan();
    }

    /// <summary>
    /// 操作権変更（敵チーム）
    /// </summary>
    private void CallEnemyTeamChangeControl(OrderIndexType controlOrderIndex)
    {
        TeamBehaviorManager.Instance.Get(EnemySideIndex).CallChangeControl(controlOrderIndex);
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
    private void CallBallChangePassTarget(OrderIndexType passTargetOrderIndex)
    {
        BallBehaviorManager.Instance.Get().CallChangePassTarget(MySideIndex, passTargetOrderIndex);
    }

    /// <summary>
    /// シュートターゲット変更
    /// </summary>
    private void CallBallChangeShootTarget(OrderIndexType shootTargetOrderIndex)
    {
        BallBehaviorManager.Instance.Get().CallChangePassTarget(EnemySideIndex, shootTargetOrderIndex);
    }
}