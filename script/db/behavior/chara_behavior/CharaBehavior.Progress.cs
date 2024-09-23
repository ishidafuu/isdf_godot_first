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
            ProgressFrame(false);
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
            ? GetSettingCatch(SettingCatchType.CaMotionFrm)
            : GetSettingCatch(SettingCatchType.CatchWait); // パスキャッチ時間（8）に変更

        var isOverCatchFrame = nowCatchCount + 1 > catchFrame;

        return isOverCatchFrame;
    }

    //現モーション終了

    /// <summary>
    /// アニメーションを進める
    /// </summary>
    private void ProgressFrame(bool isForce)
    {
        MyState.Motion.KomaFrameCount.Add();

        //フレーム終了の時間がきた、もしくは強制次フレーム
        if (isForce || MyState.Motion.KomaFrameCount.Value >= CurrentBaseMotionKoma.DefFrm)
        {
            bool isLoop = false;
            switch (CurrentBaseMotionKoma.LoopSt)
            {
                // ループ終了
                case enBMLoopSt.Ed:
                case enBMLoopSt.StEd:
                    if (MyState.Motion.LoopCount.Sub() == false)
                    {
                        isLoop = true;
                    }
                    break;
            }

            if (isLoop)
            {
                GotoLoopStartKoma();
            }
            else
            {
                if (IsLastKoma)
                {
                    // ラスコマ
                    MotionEnd();
                }
                else
                {
                    GotoNextKoma();
                }
            }
        }
    }

    private void MotionEnd()
    {
        switch (MyState.Motion.MotionType)
        {
            // ジャンプしゃがみ
            case CharaMotionType.JCr:
                Jumping();
                break;

            // 転がり
            case CharaMotionType.RoF:
            case CharaMotionType.RoB:
                // 転がりカウンタで終了を決める
                break;

            // ダウンダメージ
            case CharaMotionType.DnHF:
                SetMotionType(CharaMotionType.DnF);
                break;
            case CharaMotionType.DnHB:
                SetMotionType(CharaMotionType.DnB);
                break;

            // 屈み
            case CharaMotionType.KG:
                if (IsKagami)
                {
                    MyState.Damage.KagamiCount.Sub();
                    SetMotionType(CharaMotionType.KG);
                }
                else
                {
                    SetMotionType(CharaMotionType.St);
                }
                break;

            case CharaMotionType.St:
                break;
            case CharaMotionType.Breath:
                break;
            case CharaMotionType.Wk:
                break;
            case CharaMotionType.Ds:
                break;

            case CharaMotionType.CJCr:
                break;
            case CharaMotionType.JUp:
                break;
            case CharaMotionType.JDn:
                break;
            case CharaMotionType.ARv:
                break;
            case CharaMotionType.Cr:
                break;
            case CharaMotionType.FlF:
                break;
            case CharaMotionType.FlB:
                break;
            case CharaMotionType.PHF:
                break;
            case CharaMotionType.PHB:
                break;

            case CharaMotionType.DnF:
                break;
            case CharaMotionType.DnB:
                break;
            case CharaMotionType.DRv:
                break;
            case CharaMotionType.CM:
                break;
            case CharaMotionType.JCM:
                break;
            case CharaMotionType.FB:
                break;
            case CharaMotionType.JFB:
                break;
            case CharaMotionType.PW:
                break;
            case CharaMotionType.PWWk:
                break;
            case CharaMotionType.PWDs:
                break;
            case CharaMotionType.Sl:
                break;
            case CharaMotionType.Sh:
                break;
            case CharaMotionType.RtSh:
                break;
            case CharaMotionType.Pa:
                break;
            case CharaMotionType.JSh:
                break;
            case CharaMotionType.RtJSh:
                break;
            case CharaMotionType.JPa:
                break;
            case CharaMotionType.Ca:
                break;
            case CharaMotionType.JCa:
                break;
            case CharaMotionType.Dg:
                break;
            case CharaMotionType.JDg:
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
                break;
            default:
                throw new ArgumentOutOfRangeException();
        }
        
        ここから

        //
        //     //シュート（ジャンプシュートはどのみち落下まで何も出来ない）
        //     case dbmtSh:
        //         //終了カウンタ回す
        //         if (lib_num::AprTo0(&st_.pstMyCh_->ShEdWait_c))
        //         {
        //             pCommon_->SetMtypeReset(dbmtSt);
        //         }
        //         break;
        //
        //     //パス
        //     case dbmtPa:
        //     {
        //         //ボールダッシュマン
        //         if (pCommon_->IsBMDashOK()
        //             && (st_.pmgMyTm_->st_.pstMyTm_->DashmanNoBM == st_.posNo_)) //予約状態でもある
        //         {
        //             pCommon_->SetMtype(dbmtDs);
        //         }
        //         else
        //         {
        //             pCommon_->SetMtypeReset(dbmtSt);
        //         }
        //     }
        //         break;
        //
        //     //キャッチ
        //     case dbmtCa:
        //         if (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
        //         {
        //             pCommon_->SetMtype(dbmtDs);
        //         }
        //         else
        //         {
        //             pCommon_->SetMtype(dbmtSt);
        //         }
        //
        //         break;
        //
        //     //キャッチモーション
        //     case dbmtCM:
        //         if (st_.pstMyCh_->Motion.IsMFlags(dbmfDs))
        //         {
        //             pCommon_->SetMtype(dbmtDs);
        //         }
        //         else
        //         {
        //             pCommon_->SetMtype(dbmtSt);
        //         }
        //         break;
        //
        //     //その他
        //     default:
        //         if (st_.pstMyCh_->Motion.IsMFlags(dbmfAr))
        //         {
        //             pCommon_->SetMtype(dbmtJDn);
        //         }
        //         else
        //         {
        //             pCommon_->SetMtypeReset(dbmtSt);
        //         }
        //         break;
        // }
    }
}