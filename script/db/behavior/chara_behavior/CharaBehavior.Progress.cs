using System;

namespace db;

public partial class CharaBehavior
{
    public void UpdateProgress()
    {
        // ボールとの距離も必要な場所に移動する
        UpdateBallDistance();

        RawState.Motion.Progress();
        RawState.Damage.Progress();
        RawState.Catch.Progress();

        RawState.Pass.DecrementMirrorShotLimitCount();
        RawState.Live.IncrementBiorhythmCount();
        RawState.Dashman.DecrementEnabledPassCount();

        if (BallState.MotionType == BallMotionType.Shoot)
        {
            RawState.BallEffect.DecrementHitMTimeCount();
            // ボールと重なっていた過去を消す処理は、モーションがダウンから復帰したタイミングと、シュート開始のタイミングで行う
        }
        else
        {
            // シュート以外のクリアはそのタイミングで行うように変更する
        }

        // ダウンから起き上がった際に無敵フラグを消す

        if (RawState.Motion.HasFlag(CharaMotionFlag.Ar))
        {
            var isProgressShootAirCount = RawState.Motion.MotionType != CharaMotionType.JSh
                                          && RawState.Motion.MotionType != CharaMotionType.JCr;
            RawState.Air.Progress(isProgressShootAirCount);
        }

        if (RawState.Motion.MotionType == CharaMotionType.Ds)
        {
            RawState.Move.IncrementMadStepCount();
        }

        if (IsShotTarget && RawState.Motion.MotionType != CharaMotionType.ANG)
        {
            RawState.View.IncrementTargetCount();
        }
        else if (IsPassTarget && RawState.Motion.MotionType != CharaMotionType.ANG)
        {
            // パスターゲットのときは初期値２０から？
            RawState.View.IncrementTargetCount();
        }
        else
        {
            RawState.View.ResetTargetCount();
        }

        var isProgressAnimation = true;
        
        var isSetKagami = IsKagami && RawState.Auto.AutoType == AutoType.Free;
        
        switch (RawState.Motion.MotionType)
        {
            case CharaMotionType.St:
                if (isSetKagami)
                {
                    SetMotionType(CharaMotionType.KG);
                }
                else if (IsPassWait)
                {
                    SetMotionType(CharaMotionType.PW);
                }
                else if (IsBallHolder == false)
                {
                    if (RawState.View.BreathCount.AddUntil(GetSpeedRank(RankSpeedType.IkiItv)))
                    {
                        RawState.View.BreathCount.Clear();
                        SetMotionType(CharaMotionType.Breath);
                    }
                }

                break;
            case CharaMotionType.Breath:
                break;
            case CharaMotionType.Wk:
                if (isSetKagami)
                {
                    SetMotionType(CharaMotionType.KG);
                }
                else if (IsPassWait)
                {
                    SetMotionType(CharaMotionType.PWWk);
                }
                break;
            case CharaMotionType.Ds:
                if (isSetKagami)
                {
                    SetMotionType(CharaMotionType.KG);
                }
                else if (IsPassWait)
                {
                    SetMotionType(CharaMotionType.PWDs);
                }
                break;
            case CharaMotionType.JCr:
                break;
            case CharaMotionType.CJCr:
                break;
            case CharaMotionType.JUp:
                if (RawState.Coordinate.VelocityY < 0)
                {
                    SetMotionType(CharaMotionType.JDn);
                }
                break;
            case CharaMotionType.JDn:
                break;
            case CharaMotionType.ARv:
                break;
            case CharaMotionType.Cr:
                if (RawState.Move.JumpCrouchCount.Value > 0)
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
                RawState.Damage.DownCount.AddUntil(GetHpRank(RankHpType.RevFrm));
                break;
            case CharaMotionType.KG:
                break;
            case CharaMotionType.DnF:
            case CharaMotionType.DnB:
                if (RawState.Damage.DownCount.AddUntil(GetHpRank(RankHpType.RevFrm)))
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
                if (RawState.Coordinate.FrictionCount.Value != 0)
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
                if (RawState.Coordinate.VelocityX != 0)
                {
                    isProgressAnimation = false;
                }
                break;
            case CharaMotionType.Sh:
            case CharaMotionType.RtSh:
            case CharaMotionType.JSh:
            case CharaMotionType.RtJSh:
                if (RawState.Shoot.ShootWaitCount.Sub() == false)
                {
                    isProgressAnimation = false;
                }
                break;
            case CharaMotionType.Pa:
            case CharaMotionType.JPa:
                // 指が離れたらクイックパス
                if (IsSelfControl && MyPad.ButtonA.IsPressed == false)
                {
                    RawState.Pass.IsTossPass = false;
                }

                if (RawState.Pass.PassStandWaitCount.Sub() == false)
                {
                    isProgressAnimation = false;
                }

                break;
            case CharaMotionType.Ca:
            case CharaMotionType.JCa:
                if (RawState.Catch.CatchCount.Value > 0)
                {
                    if (IsOverCatchFrame(RawState.Catch.CatchCount.Value))
                    {
                        RawState.Catch.CatchCount.Clear();
                        GotoNextKoma();
                    }
                    else
                    {
                        RawState.Catch.CatchCount.Add();
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
                if (RawState.Damage.RollCount.Sub())
                {
                    var isDead = RawState.Live.Hp <= 0 && MyTeamState.IsAllOut == false;
                    if (isDead)
                    {
                        RawState.Live.IsAngel = true;
                        CallTeamCheckChangePos();
                        // TeamBehaviorがAngelにするかどうか決める
                    }
                    else
                    {
                        SetMotionType(RawState.Motion.MotionType == CharaMotionType.RoF
                            ? CharaMotionType.DnB
                            : CharaMotionType.DnF);
                        RawState.Coordinate.ZeroVelocity();
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
                RawState.Live.IsAlive = false;
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
                             && BallState.ThrowerSideNo != RawState.Index.SideIndex;

        var catchFrame = enemyShootBall
            ? GetSettingCatch(SettingCatchType.CaMotionFrm)
            : GetSettingCatch(SettingCatchType.CatchWait); // パスキャッチ時間（8）に変更

        var isOverCatchFrame = nowCatchCount + 1 > catchFrame;

        return isOverCatchFrame;
    }

    /// <summary>
    /// コマを進める
    /// </summary>
    private void ProgressFrame(bool isForce)
    {
        RawState.Motion.KomaFrameCount.Add();

        //フレーム終了の時間がきた、もしくは強制次フレーム
        if (isForce || RawState.Motion.KomaFrameCount.Value >= CurrentBaseMotionKoma.DefFrm)
        {
            bool isLoop = false;
            switch (CurrentBaseMotionKoma.LoopSt)
            {
                // ループ終了
                case enBMLoopSt.Ed:
                case enBMLoopSt.StEd:
                    if (RawState.Motion.LoopCount.Sub() == false)
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

    /// <summary>
    /// モーション終了
    /// </summary>
    private void MotionEnd()
    {
        switch (RawState.Motion.MotionType)
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
                    RawState.Damage.KagamiCount.Sub();
                    SetMotionType(CharaMotionType.KG);
                }
                else
                {
                    SetMotionType(CharaMotionType.St);
                }
                break;
            // キャッチモーション
            case CharaMotionType.CM:
                SetMotionType(RawState.Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionType.Ds
                    : CharaMotionType.St);
                break;
            // シュート
            case CharaMotionType.Sh:
                if (RawState.Shoot.ShootEndWaitCount.Sub())
                {
                    SetMotionType(CharaMotionType.St);
                }
                break;
            // パス
            case CharaMotionType.Pa:
                // ボールダッシュマン
                if (CanContinuationDash
                    && MyTeamState.MainState.DashmanNoBm == MyOrderIndex) //予約状態でもある
                {
                    SetMotionType(CharaMotionType.Ds);
                }
                else
                {
                    SetMotionType(CharaMotionType.St);
                }
                break;
            // キャッチ
            case CharaMotionType.Ca:
                SetMotionType(RawState.Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionType.Ds
                    : CharaMotionType.St);
                break;
            // その他
            default:
                SetMotionType(RawState.Motion.HasFlag(CharaMotionFlag.Ar)
                    ? CharaMotionType.JDn
                    : CharaMotionType.St);
                break;
        }
    }
}