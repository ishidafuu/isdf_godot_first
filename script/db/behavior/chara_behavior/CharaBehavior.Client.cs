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
    /// シュート状態
    /// </summary>
    private void CallBallShootMotion()
    {
        // // TODO:ボール状態の変更位置確認
        // BallState.ShotMotion = true;
    }
}
