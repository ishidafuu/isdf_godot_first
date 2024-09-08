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

        if (IsShotTarget() && MyState.Motion.MotionType != CharaMotionType.ANG)
        {
            MyState.View.IncrementTargetCount();
        }
        else if (IsPassTarget() && MyState.Motion.MotionType != CharaMotionType.ANG)
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
                if (IsPassWait())
                {
                    SetMotionType(CharaMotionType.PW);
                }
                else if (IsBallHolder() == false)
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
                if (IsPassWait())
                {
                    SetMotionType(CharaMotionType.PWWk);
                }
                break;
            case CharaMotionType.Ds:
                if (IsPassWait())
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
                break;
            case CharaMotionType.JFB:
                break;
            case CharaMotionType.PW:
                if (IsPassWait() == false)
                {
                    SetMotionType(CharaMotionType.St);
                }
                break;
            case CharaMotionType.PWWk:
                if (IsPassWait() == false)
                {
                    SetMotionType(CharaMotionType.Wk);
                }
                break;
            case CharaMotionType.PWDs:
                if (IsPassWait() == false)
                {
                    SetMotionType(CharaMotionType.Ds);
                }
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
            case CharaMotionType.RoF:
            case CharaMotionType.RoB:
                ここから
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
                break;
        }
    }
}