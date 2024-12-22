using System;

namespace db;

public partial class CharaBehavior
{
    /// <summary>
    /// キャラクターの状態を更新します
    /// ボールとの距離、モーション、ダメージ、キャッチなどの各種状態を進行させます
    /// </summary>
    public void UpdateProgress()
    {
        // ボールとの距離も必要な場所に移動する
        UpdateBallDistance();

        MotionSet.Progress();
        DamageSet.Progress();
        CatchSet.Progress();

        PassSet.DecrementMirrorShotLimitCount();
        LiveSet.IncrementBiorhythmCount();
        DashmanSet.DecrementEnabledPassCount();

        if (Ball.Main.MotionType == BallMotionType.Shoot)
        {
            BallEffectSet.hitMTime_cd.Sub();
            // ボールと重なっていた過去を消す処理は、モーションがダウンから復帰したタイミングと、シュート開始のタイミングで行う
        }
        else
        {
            // シュート以外のクリアはそのタイミングで行うように変更する
        }

        // ダウンから起き上がった際に無敵フラグを消す

        if (Motion.HasFlag(CharaMotionFlag.Ar))
        {
            var isProgressShootAirCount = Motion.MotionType != CharaMotionType.JSh
                                          && Motion.MotionType != CharaMotionType.JCr;
            AirSet.Progress(isProgressShootAirCount);
        }

        if (Motion.MotionType == CharaMotionType.Ds)
        {
            MoveSet.IncrementMadStepCount();
        }

        
        if (Composite.IsShotTarget && Motion.MotionType != CharaMotionType.ANG)
        {
            ViewSet.IncrementTargetCount();
        }
        else if (Composite.IsPassTarget && Motion.MotionType != CharaMotionType.ANG)
        {
            // パスターゲットのときは初期値２０から？
            ViewSet.IncrementTargetCount();
        }
        else
        {
            ViewSet.ResetTargetCount();
        }

        var isProgressAnimation = true;
        
        var isSetKagami = Composite.IsKagami && Auto.AutoType == AutoType.Free;
        
        switch (Motion.MotionType)
        {
            case CharaMotionType.St:
                if (isSetKagami)
                {
                    SetMotionType(CharaMotionType.KG);
                }
                else if (Composite.IsPassWait)
                {
                    SetMotionType(CharaMotionType.PW);
                }
                else if (Composite.IsBallHolder == false)
                {
                    if (View.BreathCount.AddUntil(GetSpeedRank(RankSpeedType.IkiItv)))
                    {
                        View.BreathCount.Clear();
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
                else if (Composite.IsPassWait)
                {
                    SetMotionType(CharaMotionType.PWWk);
                }
                break;
            case CharaMotionType.Ds:
                if (isSetKagami)
                {
                    SetMotionType(CharaMotionType.KG);
                }
                else if (Composite.IsPassWait)
                {
                    SetMotionType(CharaMotionType.PWDs);
                }
                break;
            case CharaMotionType.JCr:
                break;
            case CharaMotionType.CJCr:
                break;
            case CharaMotionType.JUp:
                if (Coordinate.VelocityY < 0)
                {
                    SetMotionType(CharaMotionType.JDn);
                }
                break;
            case CharaMotionType.JDn:
                break;
            case CharaMotionType.ARv:
                break;
            case CharaMotionType.Cr:
                if (Move.JumpCrouchCountValue > 0)
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
                DamageSet.DownCount.AddUntil(GetHpRank(RankHpType.RevFrm));
                break;
            case CharaMotionType.KG:
                break;
            case CharaMotionType.DnF:
            case CharaMotionType.DnB:
                if (DamageSet.DownCount.AddUntil(GetHpRank(RankHpType.RevFrm)))
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
                if (CoordinateSet.FrictionCount.Value != 0)
                {
                    isProgressAnimation = false;
                }
                break;
            case CharaMotionType.JFB:
                break;
            case CharaMotionType.PW:
                if (Composite.IsPassWait == false)
                {
                    SetMotionType(CharaMotionType.St);
                }
                break;
            case CharaMotionType.PWWk:
                if (Composite.IsPassWait == false)
                {
                    SetMotionType(CharaMotionType.Wk);
                }
                break;
            case CharaMotionType.PWDs:
                if (Composite.IsPassWait == false)
                {
                    SetMotionType(CharaMotionType.Ds);
                }
                break;
            case CharaMotionType.Sl:
                // スリップ中は進行なし
                if (Coordinate.VelocityX != 0)
                {
                    isProgressAnimation = false;
                }
                break;
            case CharaMotionType.Sh:
            case CharaMotionType.RtSh:
            case CharaMotionType.JSh:
            case CharaMotionType.RtJSh:
                if (ShootSet.ShootWaitCount.Sub() == false)
                {
                    isProgressAnimation = false;
                }
                break;
            case CharaMotionType.Pa:
            case CharaMotionType.JPa:
                // 指が離れたらクイックパス
                if (Composite.IsSelfControl && Pad.ButtonA.IsPressed == false)
                {
                    PassSet.IsTossPass = false;
                }

                if (Pass.PassStandWaitCount.Sub() == false)
                {
                    isProgressAnimation = false;
                }

                break;
            case CharaMotionType.Ca:
            case CharaMotionType.JCa:
                if (CatchSet.CatchCount.Value > 0)
                {
                    if (IsOverCatchFrame(Catch.CatchCountValue))
                    {
                        CatchSet.CatchCount.Clear();
                        GotoNextKoma();
                    }
                    else
                    {
                        CatchSet.CatchCount.Add();
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
                if (DamageSet.RollCount.Sub())
                {
                    var isDead = Live.Hp <= 0 && MyTeam.IsAllOut == false;
                    if (isDead)
                    {
                        LiveSet.IsAngel = true;
                        MyTeam.CallCheckChangePos();
                        // TeamBehaviorがAngelにするかどうか決める
                    }
                    else
                    {
                        SetMotionType(Motion.MotionType == CharaMotionType.RoF
                            ? CharaMotionType.DnB
                            : CharaMotionType.DnF);
                        CoordinateSet.ZeroVelocity();
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
                LiveSet.IsAlive = false;
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
    /// キャッチフレームが指定値を超えるかどうかを判定します
    /// 敵のシュートボールかパスかによって、適切なキャッチフレーム値を使用して判定を行います
    /// </summary>
    /// <param name="nowCatchCount">現在のキャッチカウント</param>
    /// <returns>キャッチフレームが指定値を超える場合はtrue</returns>
    private bool IsOverCatchFrame(int nowCatchCount)
    {
        // 敵のシュートボール
        var enemyShootBall = Ball.Main.MotionType == BallMotionType.Shoot
                             && Ball.Main.ThrowerSideNo != Index.SideIndex;

        var catchFrame = enemyShootBall
            ? GetSettingCatch(SettingCatchType.CaMotionFrm)
            : GetSettingCatch(SettingCatchType.CatchWait); // パスキャッチ時間（8）に変更

        var isOverCatchFrame = nowCatchCount + 1 > catchFrame;

        return isOverCatchFrame;
    }

    /// <summary>
    /// コマを進める処理を実行します
    /// フレームカウントを更新し、必要に応じてループ処理やモーション終了処理を行います
    /// </summary>
    /// <param name="isForce">強制的に次のフレームに進むかどうか</param>
    private void ProgressFrame(bool isForce)
    {
        MotionSet.KomaFrameCount.Add();

        //フレーム終了の時間がきた、もしくは強制次フレーム
        if (isForce || MotionSet.KomaFrameCount.Value >= Composite.CurrentBaseMotionKoma.DefFrm)
        {
            bool isLoop = false;
            switch (Composite.CurrentBaseMotionKoma.LoopSt)
            {
                // ループ終了
                case enBMLoopSt.Ed:
                case enBMLoopSt.StEd:
                    if (MotionSet.LoopCount.Sub() == false)
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
                if (Composite.IsLastKoma)
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
    /// モーション終了時の処理を実行します
    /// モーションタイプに応じて、適切な次のモーションへの遷移を行います
    /// </summary>
    private void MotionEnd()
    {
        switch (Motion.MotionType)
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
                if (Composite.IsKagami)
                {
                    DamageSet.KagamiCount.Sub();
                    SetMotionType(CharaMotionType.KG);
                }
                else
                {
                    SetMotionType(CharaMotionType.St);
                }
                break;
            // キャッチモーション
            case CharaMotionType.CM:
                SetMotionType(Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionType.Ds
                    : CharaMotionType.St);
                break;
            // シュート
            case CharaMotionType.Sh:
                if (ShootSet.ShootEndWaitCount.Sub())
                {
                    SetMotionType(CharaMotionType.St);
                }
                break;
            // パス
            case CharaMotionType.Pa:
                // ボールダッシュマン
                if (Composite.CanContinuationDash
                    && MyTeam.Main.DashmanNoBm == MyOrderIndex) //予約状態でもある
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
                SetMotionType(Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionType.Ds
                    : CharaMotionType.St);
                break;
            // その他
            default:
                SetMotionType(Motion.HasFlag(CharaMotionFlag.Ar)
                    ? CharaMotionType.JDn
                    : CharaMotionType.St);
                break;
        }
    }
}
