using System;

namespace db;

public partial class CharaBehavior
{
    public void UpdateProgress()
    {
        // ボールとの距離も必要な場所に移動する
        UpdateBallDistance();

        MyState.Motion.Progress();
        MyState.Damage.Progress();
        MyState.Catch.Progress();

        MyState.Pass.DecrementMirrorShotLimitCount();
        MyState.Live.IncrementBiorhythmCount();
        MyState.Dashman.DecrementEnabledPassCount();

        if (BallState.MotionType == BallMotionType.Shoot)
        {
            MyState.BallEffect.DecrementHitMTimeCount();
            // ボールと重なっていた過去を消す処理は、モーションがダウンから復帰したタイミングと、シュート開始のタイミングで行う
        }
        else
        {
            // シュート以外のクリアはそのタイミングで行うように変更する
        }

        // ダウンから起き上がった際に無敵フラグを消す

        if (MyState.Motion.HasFlag(CharaMotionFlag.Ar))
        {
            var isProgressShootAirCount = MyState.Motion.MotionType != CharaMotionType.JSh
                                          && MyState.Motion.MotionType != CharaMotionType.JCr;
            MyState.Air.Progress(isProgressShootAirCount);
        }

        if (MyState.Motion.MotionType == CharaMotionType.Ds)
        {
            MyState.Move.IncrementMadStepCount();
        }

        if (IsShotTarget && MyState.Motion.MotionType != CharaMotionType.ANG)
        {
            MyState.View.IncrementTargetCount();
        }
        else if (IsPassTarget && MyState.Motion.MotionType != CharaMotionType.ANG)
        {
            // パスターゲットのときは初期値２０から？
            MyState.View.IncrementTargetCount();
        }
        else
        {
            MyState.View.ResetTargetCount();
        }

        var isProgressAnimation = true;

        switch (MyState.Motion.MotionType)
        {
            case CharaMotionType.St:
                if (IsPassWait)
                {
                    SetMotionType(CharaMotionType.PW);
                }
                else if (IsBallHolder == false)
                {
                    if (MyState.View.BreathCount.AddUntil(GetSpeedRank(RankSpeedType.IkiItv)))
                    {
                        MyState.View.BreathCount.Clear();
                        SetMotionType(CharaMotionType.Breath);
                    }
                }

                break;
            case CharaMotionType.Breath:
                break;
            case CharaMotionType.Wk:
                if (IsPassWait)
                {
                    SetMotionType(CharaMotionType.PWWk);
                }
                break;
            case CharaMotionType.Ds:
                if (IsPassWait)
                {
                    SetMotionType(CharaMotionType.PWDs);
                }
                break;
            case CharaMotionType.JCr:
                break;
            case CharaMotionType.CJCr:
                break;
            case CharaMotionType.JUp:
                if (MyState.Coordinate.VelocityY < 0)
                {
                    SetMotionType(CharaMotionType.JDn);
                }
                break;
            case CharaMotionType.JDn:
                break;
            case CharaMotionType.ARv:
                break;
            case CharaMotionType.Cr:
                if (MyState.Move.JumpCrouchCount.Value > 0)
                {
                    isProgressAnimation = false;
                }
                break;
            // case CharaMotionType.FlF:
            //     break;
            // case CharaMotionType.FlB:
            //     break;
            case CharaMotionType.PHF:
                break;
            case CharaMotionType.PHB:
                break;
            case CharaMotionType.DnHF:
            case CharaMotionType.DnHB:
                MyState.Damage.DownCount.AddUntil(GetHpRank(RankHpType.RevFrm));
                break;
            case CharaMotionType.KG:
                break;
            case CharaMotionType.DnF:
            case CharaMotionType.DnB:
                if (MyState.Damage.DownCount.AddUntil(GetHpRank(RankHpType.RevFrm)))
                {
                    SetMotionType(CharaMotionType.DRv);
                }
                break;
            case CharaMotionType.DRv:
                break;
            case CharaMotionType.CM:
                break;
            case CharaMotionType.JCM:
                break;
            case CharaMotionType.FB:
                // スリップ中は進行なし
                if (MyState.Coordinate.FrictionCount.Value != 0)
                {
                    isProgressAnimation = false;
                }
                break;
            case CharaMotionType.JFB:
                break;
            case CharaMotionType.PW:
                if (IsPassWait == false)
                {
                    SetMotionType(CharaMotionType.St);
                }
                break;
            case CharaMotionType.PWWk:
                if (IsPassWait == false)
                {
                    SetMotionType(CharaMotionType.Wk);
                }
                break;
            case CharaMotionType.PWDs:
                if (IsPassWait == false)
                {
                    SetMotionType(CharaMotionType.Ds);
                }
                break;
            case CharaMotionType.Sl:
                // スリップ中は進行なし
                if (MyState.Coordinate.VelocityX != 0)
                {
                    isProgressAnimation = false;
                }
                break;
            case CharaMotionType.Sh:
            case CharaMotionType.RtSh:
            case CharaMotionType.JSh:
            case CharaMotionType.RtJSh:
                if (MyState.Shoot.ShootWaitCount.Sub() == false)
                {
                    isProgressAnimation = false;
                }
                break;
            case CharaMotionType.Pa:
            case CharaMotionType.JPa:
                // 指が離れたらクイックパス
                if (IsSelfControl && MyState.Pad.Pad.ButtonA.IsPressed == false)
                {
                    MyState.Pass.IsTossPass = false;
                }

                if (MyState.Pass.PassStandWaitCount.Sub() == false)
                {
                    isProgressAnimation = false;
                }

                break;
            case CharaMotionType.Ca:
            case CharaMotionType.JCa:
                if (MyState.Catch.CatchCount.Value > 0)
                {
                    if (IsOverCatchFrame(MyState.Catch.CatchCount.Value))
                    {
                        MyState.Catch.CatchCount.Clear();
                        GotoNextKoma();
                    }
                    else
                    {
                        MyState.Catch.CatchCount.Add();
                    }

                    isProgressAnimation = false;
                }
                break;
            case CharaMotionType.Dg:
                break;
            case CharaMotionType.JDg:
                break;
            case CharaMotionType.RoF:
            case CharaMotionType.RoB:
                if (MyState.Damage.RollCount.Sub())
                {
                    var isDead = (MyState.Live.Hp <= 0 && MyTeamState.IsAllOut() == false);
                    if (isDead)
                    {
                        MyState.Live.IsAngel = true;
                        CallTeamCheckChangePos();
                        // TeamBehaviorがAngelにするかどうか決める
                    }
                    else
                    {
                        SetMotionType(MyState.Motion.MotionType == CharaMotionType.RoF
                            ? CharaMotionType.DnB
                            : CharaMotionType.DnF);
                        MyState.Coordinate.ZeroVelocity();
                    }
                }
                break;
            case CharaMotionType.DRAW:
                break;
            case CharaMotionType.WIN:
                break;
            case CharaMotionType.LOSE:
                break;
            case CharaMotionType.OvL:
                break;
            case CharaMotionType.USA:
                break;
            case CharaMotionType.USA2:
                break;
            case CharaMotionType.IKI:
                break;
            case CharaMotionType.LOOK:
                break;
            case CharaMotionType.LOOK2:
                break;
            case CharaMotionType.FALL:
                break;
            case CharaMotionType.AGE2:
                break;
            case CharaMotionType.AGE3:
                break;
            case CharaMotionType.AGE4:
                break;
            case CharaMotionType.AGE5:
                break;
            case CharaMotionType.DO1:
                break;
            case CharaMotionType.DO2:
                break;
            case CharaMotionType.ANG:
                // 天使になるタイミングで設定するように修正する
                MyState.Live.IsAlive = false;
                isProgressAnimation = false;
                break;
            default:
                break;
        }

        if (isProgressAnimation)
        {
            ProgressAnimation(false);
        }
    }

    /// <summary>
    /// キャッチフレームが指定値を超えるか
    /// </summary>
    private bool IsOverCatchFrame(int nowCatchCount)
    {
        // 敵のシュートボール
        var enemyShootBall = BallState.MotionType == BallMotionType.Shoot
                             && BallState.ThrowerSideNo != MyState.Index.SideIndex;

        var catchFrame = enemyShootBall
            ? MasterManager.Instance.SettingMaster.GetCatch(SettingCatchType.CaMotionFrm)
            : MasterManager.Instance.SettingMaster.GetCatch(SettingCatchType.CatchWait); // パスキャッチ時間（8）に変更

        var isOverCatchFrame = nowCatchCount + 1 > catchFrame;

        return isOverCatchFrame;
    }

    /// <summary>
    /// 次のコマへ
    /// </summary>
    private void GotoNextKoma()
    {
        // //ラスコマ
        // if (IsLastKoma())//(NowKoma(bmc_Last_f != 0)
        // {
        //     MotionEnd();
        // }
        // else//次コマ
        // {
        //     ++st_.pstMyCh_->Anime.FrameNo;
        //     pCommon_->SetFrameData(FALSE);
        // }
    }

    /// <summary>
    /// アニメーションを進める
    /// </summary>
    private void ProgressAnimation(bool isForce)
    {
        // アニメーションカウンタ増加
        MyState.Motion.AnimationCount.Add();

        //フレーム終了の時間がきましたもしくは強制次フレーム
        if (isForce || MyState.Motion.AnimationCount.Value >= NowBaseMotionKoma.DefFrm)
        {
            switch (NowBaseMotionKoma.LoopSt)
            {
                // ループ終了
                case enBMLoopSt.Ed:
                    if (MyState.Motion.LoopCount.Sub())
                    {
                        GotoNextKoma();
                    }
                    else
                    {
                        MyState.Motion.BackToLoopStartFrame();
                        SetFrameData(true);
                    }
                    break;
                case enBMLoopSt.StEd:
                    break;
                default:
                    break;

                //     //ループ終了
                //     case (s32)bmlp_Ed:
                //
                //         //カウンタ
                //         --st_.pstMyCh_->Anime.Loop_c;
                //
                //         //ループ終了
                //         if (st_.pstMyCh_->Anime.Loop_c == 0)
                //         {
                //             NextFrame();
                //         }
                //         else //ループ中
                //         {
                //             //ループ始点まで戻る
                //             st_.pstMyCh_->Anime.FrameNo = st_.pstMyCh_->Anime.LoopStNo;
                //             pCommon_->SetFrameData(TRUE);
                //
                //             if (st_.pstMyCh_->Anime.Loop_c <= NGNUM)
                //             {
                //                 st_.pstMyCh_->Anime.Loop_c = 0;
                //                 //st_.pstMyCh_->Motion.M_c = 0;
                //                 //攻撃モーションで無い無限ループはM_cを初期化してみる
                //                 //ジャンプ必殺のタイミングをM_cで取るのでやっぱり初期化しない
                //                 //空中以外は初期化してみる
                //                 //下の同コマループはいいかな
                //             }
                //         }
                //         break;
                //     //同コマループ
                //     case (s32)bmlp_StEd:
                //         //カウンタ
                //         --st_.pstMyCh_->Anime.Loop_c;
                //
                //         //ループ終了
                //         if (st_.pstMyCh_->Anime.Loop_c == 0)
                //         {
                //             NextFrame();
                //         }
                //         else if (st_.pstMyCh_->Anime.Loop_c <= NGNUM)
                //         {
                //             st_.pstMyCh_->Anime.Loop_c = 0;
                //         }
                //         break;
                //
                //     //非ループ
                //     default:
                //         NextFrame();
                //         break;
                // }
            }
        }
    }

    //フレーム情報のセット
    private void SetFrameData(bool isInLoop)
    {
        //ループ始点の場合
        if (isInLoop == false)
        {
            switch (NowBaseMotionKoma.LoopSt)
            {
                case enBMLoopSt.St:
                    MyState.Motion.StartLoop(NowBaseMotionKoma.LoopNum);
                    break;
                case enBMLoopSt.StEd:
                    MyState.Motion.StartLoop(NowBaseMotionKoma.LoopNum);
                    break;
            }

            PlaySe(NowBaseMotionKoma.Se);
        }
        else
        {
            if (NowBaseMotionKoma.SeLoopF)
            {
                PlaySe(NowBaseMotionKoma.Se);
            }
        }

        MyState.Motion.AnimationCount.Clear();
        MyState.Motion.IsActionPoint = NowBaseMotionKoma.IsActionPoint;
    }
}