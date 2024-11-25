using System;
using System.Diagnostics;

namespace db;

public partial class CharaBehavior
{

    private void SetMotionType(CharaMotionType motionType, bool isForce = false)
    {
        // モーション変更可能チェック
        if (CanSetMotion(motionType, isForce) == false)
        {
            return;
        }

        // モーション差し替え
        motionType = ShiftMotionType(motionType);

        var lastMotionNo = Motion.MotionNo;
        var lastMotionFlag = Motion.MotionFlag;

        // パス待ちモーションの時は絵柄だけ変わるように、それ以外の時は初期化
        var isSwitchPassWait =
            (motionType == CharaMotionType.Ds && lastMotionNo == CharaMotionNo.PWDS)
            || (motionType == CharaMotionType.PWDs && lastMotionNo == CharaMotionNo.DS)
            || (motionType == CharaMotionType.Wk && lastMotionNo == CharaMotionNo.PWWKF)
            || (motionType == CharaMotionType.Wk && lastMotionNo == CharaMotionNo.PWWKN)
            || (motionType == CharaMotionType.Wk && lastMotionNo == CharaMotionNo.PWWKR)
            || (motionType == CharaMotionType.PWWk && lastMotionNo == CharaMotionNo.WKF)
            || (motionType == CharaMotionType.PWWk && lastMotionNo == CharaMotionNo.WKN)
            || (motionType == CharaMotionType.PWWk && lastMotionNo == CharaMotionNo.WKR);

        SetMotionNo(motionType);

        if (isSwitchPassWait)
        {
            return;
        }

        MotionSet.MotionCount.Clear();
        CatchSet.CatchCount.Clear();
        CourtSet.EnemyCortDodgeCount.Clear();
        MoveSet.DashAccelCount.Clear();
        CoordinateSet.FrictionCount.Clear();
        AirSet.IsLandSet = false;
        ShootSet.IsUTurn = false;

        // キャッチ待ち時間クリア
        if (motionType != CharaMotionType.St
            && motionType != CharaMotionType.Wk
            && motionType != CharaMotionType.Ca
            && motionType != CharaMotionType.JCa)
        {
            CatchSet.ResetCacheWait();
        }

        // 立ちパス待ちカウンタ
        if (Composite.IsPassWait == false)
        {
            Pass.PassStandWaitCount.Clear();
        }

        // ダウンカウンタクリア
        if (motionType != CharaMotionType.FlF
            && motionType != CharaMotionType.FlB
            && motionType != CharaMotionType.DnF
            && motionType != CharaMotionType.DnB
            && motionType != CharaMotionType.DnHF
            && motionType != CharaMotionType.DnHB)
        {
            DamageSet.DownCount.Clear();
        }

        // 歩数リセット
        // TODO:（ダッシュ以外にしなくてよいか確認）
        // ダッシュ継続以外
        if (motionType == CharaMotionType.St
            || motionType == CharaMotionType.Wk
            || (motionType == CharaMotionType.Ds && Motion.HasFlag(CharaMotionFlag.Ds) == false))
        {
            MoveSet.MadStepCount.Clear();
            MoveSet.StepCount.Clear();
        }

        if (motionType == CharaMotionType.ARv)
        {
            AirSet.IsAirAction = true;
        }

        if (motionType == CharaMotionType.DnF || motionType == CharaMotionType.DnB)
        {
            PlaySe(SeType.DownLv0);
        }

        // シュートモーション時間
        if (motionType is CharaMotionType.Sh or CharaMotionType.RtSh or CharaMotionType.JSh
            or CharaMotionType.RtJSh)
        {
            ShootSet.SetWaitCount(GetLevelRank(RankLevelType.ShStMotion), GetLevelRank(RankLevelType.ShEdMotion));
            CallBallShootMotion();
        }

        // Uターンシュート
        if (motionType is CharaMotionType.RtSh or CharaMotionType.RtJSh)
        {
            CoordinateSet.DirectionZ = DirectionZType.Neutral;
            ShootSet.IsUTurn = true;
        }

        if (motionType is CharaMotionType.Pa or CharaMotionType.JPa)
        {
            var passWait = GetSettingPass(SettingPassType.PaStWait);
            PassSet.ResetPassStandWaitCount(passWait);
            PassSet.IsTossPass = true;
        }

        if (motionType is CharaMotionType.JSh or CharaMotionType.RtJSh or CharaMotionType.JPa
            or CharaMotionType.JCa or CharaMotionType.JDg)
        {
            AirSet.IsAirAction = true;
        }

        if (motionType is CharaMotionType.JCa or CharaMotionType.JDg)
        {
            AirSet.IsAirCatch = true;
        }

        if (motionType is CharaMotionType.Ca)
        {
            var catchWait = GetSettingCatch(SettingCatchType.CatchWait);
            CatchSet.SetCacheWait(catchWait);
        }

        // ごろごろ
        if (motionType is CharaMotionType.RoF or CharaMotionType.RoB)
        {
            PlaySe(SeType.Roll);
        }

        // オーバーライン
        if (motionType is CharaMotionType.OvL)
        {
            Referee.CallWhistleOverLine(MySideIndex);
        }

        // 天使
        if (motionType is CharaMotionType.ANG)
        {
            LiveSet.IsDead = true;
            CoordinateSet.ZeroVelocity();
            //天使生成
            MyTeam.CallGenerateAngel(MyOrderIndex);
        }

        // ドロー、勝ち、負け
        if (motionType is CharaMotionType.DRAW or CharaMotionType.WIN or CharaMotionType.LOSE)
        {
            CoordinateSet.SetGameOverCoordinate(MySideIndex, Order.IsInfield);
        }

        // 接地した
        if (lastMotionFlag.HasFlag(CharaMotionFlag.Ar)
            && Motion.HasFlag(CharaMotionFlag.Ar) == false)
        {
            AirSet.Initialize();
        }

        // フリーアクションフラグ管理
        if (Auto.AutoType == AutoType.Free
            && Motion.HasFlag(CharaMotionFlag.Act))
        {
            AutoSet.IsFreeAction = true;
        }
        else if (motionType is CharaMotionType.JCr or CharaMotionType.JUp)
        {
            // フリー状態を継続
        }
        else
        {
            AutoSet.IsFreeAction = false;
        }

        // ダッシュ終了
        if (Motion.HasFlag(CharaMotionFlag.Ds) == false
            && Dashman.IsCalledDashman)
        {
            DashmanSet.Initialize();
        }

        // コマスタート処理
        StartKoma(false);
    }

    private CharaMotionType ShiftMotionType(CharaMotionType motionType)
    {
        // 空中の場合、地上モーションを空中用に変更
        if (Motion.HasFlag(CharaMotionFlag.Ar))
        {
            motionType = motionType switch
            {
                CharaMotionType.St => CharaMotionType.JUp,
                CharaMotionType.Sh => CharaMotionType.JSh,
                CharaMotionType.RtSh => CharaMotionType.RtJSh,
                CharaMotionType.Pa => CharaMotionType.JPa,
                CharaMotionType.Ca => CharaMotionType.JCa,
                _ => motionType
            };
        }
        else
        {
            motionType = motionType switch
            {
                CharaMotionType.JUp or CharaMotionType.JDn => CharaMotionType.St,
                CharaMotionType.JSh => CharaMotionType.Sh,
                CharaMotionType.RtJSh => CharaMotionType.RtSh,
                CharaMotionType.JPa => CharaMotionType.Pa,
                CharaMotionType.JCa => CharaMotionType.Ca,
                _ => motionType
            };
        }

        // 試合終了してるときは天使にならないでダウンに変化
        if (motionType == CharaMotionType.ANG && Referee.Main.IsGameSet)
        {
            motionType = Motion.MotionType == CharaMotionType.FlF
                ? CharaMotionType.DnF
                : CharaMotionType.DnB;
            // TODO:審判にアクセスするタイミングとしてはここではないので移動する
            //  st_.pmgRf_->SetMotion(dbrfLongWhistle);
        }

        // 下降中はジャンプ下降中に変更
        if (motionType == CharaMotionType.JUp && Coordinate.VelocityY < 0)
        {
            motionType = CharaMotionType.JDn;
        }

        return motionType;
    }

    private bool CanSetMotion(CharaMotionType motionType, bool isForce)
    {
        // 同じモーション間は変更不可
        if (isForce == false
            && Motion.MotionType == motionType)
        {
            return false;
        }

        // 天使状態は変更付加
        if (Motion.MotionType == CharaMotionType.ANG)
        {
            return false;
        }

        // 勝ち負けポーズ同士の変更不可
        if (Motion.HasFlag(CharaMotionFlag.RES)
            && motionType is CharaMotionType.WIN or CharaMotionType.LOSE or CharaMotionType.DRAW)
        {
            return false;
        }

        // かがみは立ち歩き走り以外ではならない
        if (motionType == CharaMotionType.KG)
        {
            if (Motion.MotionType != CharaMotionType.St
                && Motion.MotionType != CharaMotionType.Wk
                && Motion.MotionType != CharaMotionType.Ds
                && Motion.MotionType != CharaMotionType.KG)
            {
                return false;
            }
        }

        return true;
    }

    private void SetMotionNo(CharaMotionType motionType)
    {
        switch (motionType)
        {
            // 立ち
            case CharaMotionType.St:
                MotionSet.SetMotionNo(motionType, CharaMotionNo.STF,
                    CharaMotionFlag.JpOK | CharaMotionFlag.Muki | CharaMotionFlag.DmOK | CharaMotionFlag.AtCa,
                    Coordinate.DirectionZ);
                break;
            // ブレス
            case CharaMotionType.Breath:
                // 扱いは立ち状態
                MotionSet.SetMotionNo(CharaMotionType.St, CharaMotionNo.BRF,
                    CharaMotionFlag.JpOK | CharaMotionFlag.Muki | CharaMotionFlag.DmOK | CharaMotionFlag.AtCa,
                    Coordinate.DirectionZ);
                break;
            // 歩き
            case CharaMotionType.Wk:
                MotionSet.SetMotionNo(motionType, CharaMotionNo.WKF,
                    CharaMotionFlag.JpOK | CharaMotionFlag.Muki | CharaMotionFlag.DmOK | CharaMotionFlag.AtCa,
                    Coordinate.DirectionZ);
                break;
            // ダッシュ
            case CharaMotionType.Ds:
                MotionSet.SetMotionNo(motionType, CharaMotionNo.DS,
                    CharaMotionFlag.Ds | CharaMotionFlag.JpOK | CharaMotionFlag.Muki | CharaMotionFlag.AtCa,
                    Coordinate.DirectionZ);
                break;
            // ジャンプ前しゃがみ
            case CharaMotionType.JCr:
            {
                var flag = Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionFlag.Act | CharaMotionFlag.Ar | CharaMotionFlag.Ds
                    : CharaMotionFlag.Act | CharaMotionFlag.Ar;
                MotionSet.SetMotionNo(motionType, CharaMotionNo.JCF, flag,
                    Coordinate.DirectionZ);
            }
                break;
            // キャンセルジャンプ前しゃがみ
            case CharaMotionType.CJCr:
                // ダッシュフラグ消す、扱いはジャンプ前しゃがみ
                MotionSet.SetMotionNo(CharaMotionType.JCr, CharaMotionNo.CRF,
                    CharaMotionFlag.Act | CharaMotionFlag.Ar,
                    Coordinate.DirectionZ);
                break;
            // ジャンプ
            case CharaMotionType.JUp:
            case CharaMotionType.JDn:
            {
                var motionNo = motionType is CharaMotionType.JUp
                    ? CharaMotionNo.AIRUPF
                    : CharaMotionNo.AIRDNF;
                var flag = Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionFlag.Ar | CharaMotionFlag.Muki | CharaMotionFlag.AtCa | CharaMotionFlag.Ds
                    : CharaMotionFlag.Ar | CharaMotionFlag.Muki | CharaMotionFlag.AtCa;
                MotionSet.SetMotionNo(motionType, motionNo, flag,
                    Coordinate.DirectionZ);
            }
                break;
            // 空中復帰
            case CharaMotionType.ARv:
                MotionSet.SetMotionNo(motionType, CharaMotionNo.AIRDNF,
                    CharaMotionFlag.Ar | CharaMotionFlag.Muki | CharaMotionFlag.AtCa,
                    Coordinate.DirectionZ);
                break;
            // しゃがみ
            case CharaMotionType.Cr:
                MotionSet.SetMotionNo(motionType, CharaMotionNo.CRF,
                    CharaMotionFlag.Act | CharaMotionFlag.AtCa,
                    Coordinate.DirectionZ);
                break;
            // ふっとび
            case CharaMotionType.FlF:
            case CharaMotionType.FlB:
            {
                var motionNo = motionType is CharaMotionType.FlF
                    ? CharaMotionNo.FLF
                    : CharaMotionNo.FLB;
                MotionSet.SetMotionNo(motionType, motionNo,
                    CharaMotionFlag.Ar | CharaMotionFlag.Dam | CharaMotionFlag.MTK,
                    Coordinate.DirectionZ);
            }
                break;
            // パスヒット
            case CharaMotionType.PHF:
            case CharaMotionType.PHB:
            {
                var motionNo = motionType is CharaMotionType.PHF
                    ? CharaMotionNo.PHF
                    : CharaMotionNo.PHB;
                MotionSet.SetMotionNo(motionType, motionNo, CharaMotionFlag.PHit, Coordinate.DirectionZ);
            }
                break;
            // ダウンヒット
            case CharaMotionType.DnHF:
            case CharaMotionType.DnHB:
            {
                var motionNo = motionType is CharaMotionType.DnHF
                    ? CharaMotionNo.DNHF
                    : CharaMotionNo.DNHB;
                MotionSet.SetMotionNo(motionType, motionNo,
                    CharaMotionFlag.Dam | CharaMotionFlag.Dn, Coordinate.DirectionZ);
            }
                break;
            // かがみ
            case CharaMotionType.KG:
            {
                var motionNo = Live.Hp > Defines.KAGAMI2HP
                    ? CharaMotionNo.KG
                    : CharaMotionNo.KG2;
                MotionSet.SetMotionNo(motionType, motionNo,
                    CharaMotionFlag.Dam | CharaMotionFlag.KG, Coordinate.DirectionZ);
            }
                break;
            // ダウン
            case CharaMotionType.DnF:
            case CharaMotionType.DnB:
            {
                var motionNo = motionType is CharaMotionType.DnF
                    ? CharaMotionNo.DNF
                    : CharaMotionNo.DNB;
                MotionSet.SetMotionNo(motionType, motionNo,
                    CharaMotionFlag.Dam | CharaMotionFlag.Dn, Coordinate.DirectionZ);
            }
                break;
            // ダウンからの復帰
            case CharaMotionType.DRv:
                MotionSet.SetMotionNo(motionType, CharaMotionNo.DRVF,
                    CharaMotionFlag.Dam | CharaMotionFlag.Dn, Coordinate.DirectionZ);
                break;
            // キャッチモーション
            case CharaMotionType.CM:
                MotionSet.SetMotionNo(motionType, CharaMotionNo.CMF,
                    CharaMotionFlag.Act, Coordinate.DirectionZ);
                break;
            case CharaMotionType.JCM:
                break;
            // ファンブル
            case CharaMotionType.FB:
                MotionSet.SetMotionNo(motionType, CharaMotionNo.FBF,
                    CharaMotionFlag.Act,
                    Coordinate.DirectionZ);
                break;
            // ジャンプファンブル
            case CharaMotionType.JFB:
                MotionSet.SetMotionNo(motionType, CharaMotionNo.JFBF,
                    CharaMotionFlag.Act,
                    Coordinate.DirectionZ);
                break;
            // パス待ち
            case CharaMotionType.PW:
                MotionSet.SetMotionNo(motionType, CharaMotionNo.PWF,
                    CharaMotionFlag.JpOK | CharaMotionFlag.Muki | CharaMotionFlag.PW | CharaMotionFlag.DmOK,
                    Coordinate.DirectionZ);
                break;
            // パス待ち歩き
            case CharaMotionType.PWWk:
                // 歩き扱い
                MotionSet.SetMotionNo(CharaMotionType.Wk, CharaMotionNo.PWWKF,
                    CharaMotionFlag.JpOK | CharaMotionFlag.Muki | CharaMotionFlag.PW | CharaMotionFlag.DmOK,
                    Coordinate.DirectionZ);
                break;
            // パス待ちダッシュ
            case CharaMotionType.PWDs:
                // ダッシュ扱い
                MotionSet.SetMotionNo(CharaMotionType.Ds, CharaMotionNo.PWDS,
                    CharaMotionFlag.Ds | CharaMotionFlag.JpOK | CharaMotionFlag.PW,
                    Coordinate.DirectionZ);
                break;
            // スリップ
            case CharaMotionType.Sl:
                MotionSet.SetMotionNo(motionType, CharaMotionNo.SL,
                    CharaMotionFlag.Act | CharaMotionFlag.JpOK | CharaMotionFlag.Slip | CharaMotionFlag.AtCa,
                    Coordinate.DirectionZ);
                break;
            // シュート、振り返りシュート、パス
            case CharaMotionType.Sh:
            case CharaMotionType.RtSh:
            case CharaMotionType.Pa:
            {
                var type = motionType == CharaMotionType.Pa
                    ? CharaMotionType.Pa
                    : CharaMotionType.Sh;

                var motionNo = motionType switch
                {
                    CharaMotionType.Sh => CharaMotionNo.SHF,
                    CharaMotionType.RtSh => CharaMotionNo.RTNSH,
                    _ => CharaMotionNo.PAF
                };

                var flag = CharaMotionFlag.Act;
                if (Motion.HasFlag(CharaMotionFlag.Slip))
                {
                    flag |= CharaMotionFlag.Slip;
                }
                else if (Motion.HasFlag(CharaMotionFlag.Ds))
                {
                    flag |= CharaMotionFlag.Ds;
                }

                MotionSet.SetMotionNo(type, motionNo, flag, Coordinate.DirectionZ);
            }
                break;
            // ジャンプシュート、振り返りジャンプシュート、ジャンプパス
            case CharaMotionType.JSh:
            case CharaMotionType.RtJSh:
            case CharaMotionType.JPa:
            {
                var type = motionType == CharaMotionType.JPa
                    ? CharaMotionType.JPa
                    : CharaMotionType.JSh;

                var motionNo = motionType switch
                {
                    CharaMotionType.JSh => CharaMotionNo.JSHF,
                    CharaMotionType.RtJSh => CharaMotionNo.RTNSH,
                    _ => CharaMotionNo.JPAF
                };

                var flag = Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionFlag.Ar | CharaMotionFlag.Act | CharaMotionFlag.Ds
                    : CharaMotionFlag.Ar | CharaMotionFlag.Act;

                MotionSet.SetMotionNo(type, motionNo, flag, Coordinate.DirectionZ);
            }
                break;
            // キャッチ
            case CharaMotionType.Ca:
            {
                var flag = Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionFlag.Act | CharaMotionFlag.Ds
                    : CharaMotionFlag.Act;

                MotionSet.SetMotionNo(motionType, CharaMotionNo.CAF,
                    flag, Coordinate.DirectionZ);
            }
                break;
            // ジャンプキャッチ
            case CharaMotionType.JCa:
            {
                var flag = Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionFlag.Ar | CharaMotionFlag.Act | CharaMotionFlag.Ds
                    : CharaMotionFlag.Ar | CharaMotionFlag.Act;

                MotionSet.SetMotionNo(motionType, CharaMotionNo.CAF,
                    flag, Coordinate.DirectionZ);
            }
                break;
            // よけ
            case CharaMotionType.Dg:
            {
                var flag = Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionFlag.Act | CharaMotionFlag.Ds
                    : CharaMotionFlag.Act;

                MotionSet.SetMotionNo(motionType, CharaMotionNo.DGF,
                    flag, Coordinate.DirectionZ);
            }
                break;
            // ジャンプよけ
            case CharaMotionType.JDg:
            {
                var flag = Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionFlag.Ar | CharaMotionFlag.Act | CharaMotionFlag.Ds
                    : CharaMotionFlag.Ar | CharaMotionFlag.Act;

                MotionSet.SetMotionNo(motionType, CharaMotionNo.JDGF,
                    flag, Coordinate.DirectionZ);
            }
                break;
            // ごろごろ
            case CharaMotionType.RoF:
            case CharaMotionType.RoB:
            {
                var motionNo = motionType is CharaMotionType.RoF
                    ? CharaMotionNo.ROF
                    : CharaMotionNo.ROB;
                MotionSet.SetMotionNo(motionType, motionNo,
                    CharaMotionFlag.Dam | CharaMotionFlag.Dn, Coordinate.DirectionZ);
            }
                break;
            case CharaMotionType.DRAW:
                MotionSet.SetMotionNo(motionType, CharaMotionNo.DRAW,
                    CharaMotionFlag.RES, Coordinate.DirectionZ);
                break;
            case CharaMotionType.WIN:
                MotionSet.SetMotionNo(motionType, CharaMotionNo.WIN,
                    CharaMotionFlag.RES, Coordinate.DirectionZ);
                break;
            case CharaMotionType.LOSE:
                MotionSet.SetMotionNo(motionType, CharaMotionNo.LOSE,
                    CharaMotionFlag.RES, Coordinate.DirectionZ);
                break;
            // オーバーライン
            case CharaMotionType.OvL:
            {
                var flag = Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionFlag.Act | CharaMotionFlag.Ds
                    : CharaMotionFlag.Act;

                MotionSet.SetMotionNo(motionType, CharaMotionNo.JDGF,
                    flag, Coordinate.DirectionZ);
            }
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
                MotionSet.SetMotionNo(motionType, CharaMotionNo.ANG,
                    CharaMotionFlag.Ar | CharaMotionFlag.MTK | CharaMotionFlag.Dam | CharaMotionFlag.ANG,
                    Coordinate.DirectionZ);
                break;
            default:
                throw new ArgumentOutOfRangeException(nameof(motionType), motionType, null);
        }
    }

    private void PlayBaseMotionKomaSe()
    {
        var se = Composite.CurrentBaseMotionKoma.Se;
        PlaySe(se);
    }

    private void InvokeActionPoint()
    {
        switch (Motion.MotionType)
        {
            case CharaMotionType.Ds:
                MoveSet.StepCount.Add();
                if (MoveSet.StepCount.Value % 2 == 1)
                {
                    PlaySe(SeType.Dash);
                }
                break;
            case CharaMotionType.Sh:
                SetShooting(false);
                break;

            case CharaMotionType.JSh:
                SetShooting(true);
                break;

            case CharaMotionType.Pa:
            case CharaMotionType.JPa:
                SetPassing();
                break;

            case CharaMotionType.Ca:
            case CharaMotionType.JCa:
                CatchSet.CatchCount.Clear();
                break;
            case CharaMotionType.OvL:
                if (Composite.IsBallHolder)
                {
                    Referee.CallResetOverLine(MySideIndex);
                }
                break;
        }
    }

    /// <summary>
    /// コマスタート
    /// </summary>
    private void StartKoma(bool isInLoop)
    {
        var komaData = Composite.CurrentBaseMotionKoma;
        MotionSet.StartKoma(komaData);
        if (Composite.CurrentBaseMotionKoma.IsActionPoint)
        {
            InvokeActionPoint();
        }

        if (isInLoop == false || komaData.SeLoopF)
        {
            PlayBaseMotionKomaSe();
        }
    }

    /// <summary>
    /// 次のコマに進む
    /// </summary>
    private void GotoNextKoma()
    {
        MotionSet.IncKomaNo();
        StartKoma(false);
    }

    /// <summary>
    /// ループスタートのコマに戻る
    /// </summary>
    private void GotoLoopStartKoma()
    {
        MotionSet.BackToLoopStartKomaNo();
        StartKoma(true);
    }

    /// <summary>
    /// ジャンプ
    /// </summary>
    private void Jumping()
    {
        SetMotionType(CharaMotionType.JUp);
        PlaySe(SeType.Jump);

        // Y初速
        var velocityY = Motion.HasFlag(CharaMotionFlag.Ds)
            ? GetSettingJump(SettingJumpType.DashJump_vY0)
            : GetSettingJump(SettingJumpType.Jump_vY0);

        CoordinateSet.VelocityY = velocityY;

        // 頂点タイミング
        var gravity = GetSettingGravity(SettingGravityType.GRV_Y);
        var topTiming = velocityY / gravity;
        AirSet.TopTiming = topTiming;

        if (Composite.IsControl == false
            && Composite.IsDashman == false
            && Air.IsVerticalJump)
        {
            CoordinateSet.VelocityX = 0;
            CoordinateSet.VelocityZ = 0;
        }
        else if (Motion.HasFlag(CharaMotionFlag.Ds))
        {
            var xSign = (int)Coordinate.DashDirection;
            var isLongJump = false;
            if (Composite.IsCom)
            {
                // 何らかの形でロングジャンプ判定
            }
            else
            {
                isLongJump = Composite.IsPressedDashAccelKey || Air.IsLongJump;
            }

            var dX = isLongJump
                ? GetSpeedRank(RankSpeedType.LDSJPX)
                : GetSettingCourt(SetingCourtType.DashJpX);

            CoordinateSet.VelocityX = xSign * dX;

            var zSign = 0;
            if (Composite.IsControl && Composite.IsCom == false)
            {
                if (Pad.KeyUp.IsPressed)
                {
                    zSign = +1;
                }
                else if (Pad.KeyDown.IsPressed)
                {
                    zSign = -1;
                }
            }

            CoordinateSet.VelocityZ = zSign * DiagonalWalkJumpVelocityZ;
        }
        else
        {
            var orderType = Order.GetOrderFieldType();
            var xSign = 0;
            var zSign = 0;
            if (Composite.IsCom == false && (Composite.IsFree(true) || Auto.IsFreeAction))
            {
                if (orderType != OrderFieldType.Outfield2 && Pad.KeyUp.IsPressed)
                {
                    zSign = +1;
                }
                else if (orderType != OrderFieldType.Outfield3 && Pad.KeyDown.IsPressed)
                {
                    zSign = -1;
                }

                if ((MySideIndex == 1 || orderType != OrderFieldType.Outfield4) && Pad.KeyRight.IsPressed)
                {
                    xSign = +1;
                }
                else if ((MySideIndex == 0 || orderType != OrderFieldType.Outfield4) && Pad.KeyLeft.IsPressed)
                {
                    xSign = -1;
                }
            }
            else if (Auto.AutoType == AutoType.Return)
            {
                xSign = (int)Auto.DirectionX;
                zSign = (int)Auto.DirectionZ;
            }

            var idDiagonal = xSign != 0 && zSign != 0;

            var dx = idDiagonal
                ? zSign * DiagonalWalkJumpVelocityX
                : zSign * WalkJumpVelocityX;

            var dz = idDiagonal
                ? zSign * DiagonalWalkJumpVelocityZ
                : zSign * WalkJumpVelocityZ;

            if (orderType is OrderFieldType.Outfield2 or OrderFieldType.Outfield3)
            {
                dz = dz * Defines.Percent / Defines.JPGAIYAPER;
            }

            CoordinateSet.VelocityX = dx;
            CoordinateSet.VelocityZ = dz;
        }
    }
}

//     //  //うさぎ跳び
//     //case dbmtUSA:
//     //  st_.pstMyCh_->Motion.MNo = dbmnUSA;
//     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
//     //  break;
//     //
//     //  //うさぎ跳び2
//     //case dbmtUSA2:
//     //  st_.pstMyCh_->Motion.MNo = dbmnUSA2;
//     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
//     //  break;
//     //
//     //  //息切れ
//     //case dbmtIKI:
//     //  st_.pstMyCh_->Motion.MNo = dbmnIKI;
//     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
//     //  break;
//     //
//     //  //こっちみる
//     //case dbmtLOOK:
//     //  st_.pstMyCh_->Motion.MNo = dbmnLOOK;
//     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
//     //  break;
//     //
//     //  //こっちみて笑う
//     //case dbmtLOOK2:
//     //  st_.pstMyCh_->Motion.MNo = dbmnLOOK2;
//     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
//     //  break;
//     //
//     //  ////おちる
//     //  //case dbmtFALL:
//     //  //  st_.pstMyCh_->Motion.MNo = dbmnFALL;
//     //  //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
//     //  //  break;
//     //
//     //  //胴上げられ
//     //case dbmtDO1:
//     //  st_.pstMyCh_->Motion.MNo = dbmnDO1;
//     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
//     //  break;
//     //
//     //  //胴上げられ
//     //case dbmtDO2:
//     //  st_.pstMyCh_->Motion.MNo = dbmnDO2;
//     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
//     //  break;
//     //
//     //  //胴上げ
//     //case dbmtAGE2:
//     //  st_.pstMyCh_->Motion.MNo = dbmnAGEF;
//     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
//     //  break;
//     //  //胴上げ
//     //case dbmtAGE3:
//     //  st_.pstMyCh_->Motion.MNo = dbmnAGEN;
//     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
//     //  break;
//     //  //胴上げ
//     //case dbmtAGE4:
//     //  st_.pstMyCh_->Motion.MNo = dbmnAGER;
//     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
//     //  break;
//     //  //胴上げ
//     //case dbmtAGE5:
//     //  st_.pstMyCh_->Motion.MNo = dbmnAGEC;
//     //  st_.pstMyCh_->Motion.MFlags = (dbmfNone);
//     //  break;
//     //
//   default:
//     ////ないと思うけど一応
//     //st_.pstMyCh_->Motion.MNo = dbmnFALL;
//     break;
//   }
//
//
