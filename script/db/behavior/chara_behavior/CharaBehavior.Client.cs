namespace db;

public partial class CharaBehavior
{
    /// <summary>
    /// 効果音を再生します
    /// </summary>
    /// <param name="seType">再生する効果音の種類</param>
    private void PlaySe(SeType seType)
    {
        if (seType == SeType.None)
        {
            return;
        }
        SoundManager.Instance.PlaySe(seType);
    }
    
    /// <summary>
    /// キャッチ時の効果音を再生します
    /// キャラクターの技術ランクに応じて異なる効果音を再生する予定でしたが、
    /// 現在は固定のキャッチ音を再生します
    /// </summary>
    private void PlaySeCatchSe()
    {
        // (int)SeType.CatchLv0 +  GetTechRank(RankTechType.CatchTime)% 7
       
        // SESetInt(seCatchLv0 + ((MyStLv(dbst_Tech) / 3) % 7));
        PlaySe(SeType.CatchLv0);
    }


    /// <summary>
    /// シュート追尾音を停止します
    /// シュートのホーミング効果音の再生を停止します
    /// </summary>
    private void StopHomingSe()
    {
        SoundManager.Instance.StopHomingSe();
    }
    
    
    /// <summary>
    /// ボールのシュートモーション状態を設定します
    /// 現在は未実装ですが、将来的にボールの状態を変更する予定です
    /// </summary>
    private void CallBallShootMotion()
    {
        // // TODO:ボール状態の変更位置確認
        // BallState.ShotMotion = true;
    }
}
