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

        var lastMotionNo = MyState.Motion.MotionNo;
        var lastMotionFlag = MyState.Motion.MotionFlag;

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

        MyState.Motion.MotionCount.Clear();
        MyState.Catch.CatchCount.Clear();
        MyState.Dodge.EnemyCortDodgeCount.Clear();
        MyState.Move.DashAccelCount.Clear();
        MyState.Coordinate.FrictionCount.Clear();
        MyState.Air.IsLandSet = false;
        MyState.Shoot.IsUTurn = false;

        // キャッチ待ち時間クリア
        if (motionType != CharaMotionType.St
            && motionType != CharaMotionType.Wk
            && motionType != CharaMotionType.Ca
            && motionType != CharaMotionType.JCa)
        {
            MyState.Catch.ResetCacheWait();
        }

        // 立ちパス待ちカウンタ
        if (IsPassWait == false)
        {
            MyState.Pass.PassStandWaitCount.Clear();
        }

        // ダウンカウンタクリア
        if (motionType != CharaMotionType.FlF
            && motionType != CharaMotionType.FlB
            && motionType != CharaMotionType.DnF
            && motionType != CharaMotionType.DnB
            && motionType != CharaMotionType.DnHF
            && motionType != CharaMotionType.DnHB)
        {
            MyState.Damage.DownCount.Clear();
        }

        // 歩数リセット
        // TODO:（ダッシュ以外にしなくてよいか確認）
        // ダッシュ継続以外
        if (motionType == CharaMotionType.St
            || motionType == CharaMotionType.Wk
            || (motionType == CharaMotionType.Ds && MyState.Motion.HasFlag(CharaMotionFlag.Ds) == false))
        {
            MyState.Move.MadStepCount.Clear();
            MyState.Shoot.Step.Clear();
        }

        if (motionType == CharaMotionType.ARv)
        {
            MyState.Air.IsAirAction = true;
        }

        if (motionType == CharaMotionType.DnF || motionType == CharaMotionType.DnB)
        {
            PlaySe(SeType.DownLv0);
        }

        // シュートモーション時間
        if (motionType is CharaMotionType.Sh or CharaMotionType.RtSh or CharaMotionType.JSh
            or CharaMotionType.RtJSh)
        {
            MyState.Shoot.SetWaitCount(GetLevelRank(RankLevelType.ShStMotion), GetLevelRank(RankLevelType.ShEdMotion));
            CallBallShootMotion();
        }

        // Uターンシュート
        if (motionType is CharaMotionType.RtSh or CharaMotionType.RtJSh)
        {
            MyState.Coordinate.DirectionZ = DirectionZType.Neutral;
            MyState.Shoot.IsUTurn = true;
        }

        if (motionType is CharaMotionType.Pa or CharaMotionType.JPa)
        {
            var passWait = GetSettingPass(SettingPassType.PaStWait);
            MyState.Pass.ResetPassStandWaitCount(passWait);
            MyState.Pass.IsTossPass = true;
        }

        if (motionType is CharaMotionType.JSh or CharaMotionType.RtJSh or CharaMotionType.JPa
            or CharaMotionType.JCa or CharaMotionType.JDg)
        {
            MyState.Air.IsAirAction = true;
        }

        if (motionType is CharaMotionType.JCa or CharaMotionType.JDg)
        {
            MyState.Air.IsAirCatch = true;
        }

        if (motionType is CharaMotionType.Ca)
        {
            var catchWait = GetSettingCatch(SettingCatchType.CatchWait);
            MyState.Catch.SetCacheWait(catchWait);
        }

        // ごろごろ
        if (motionType is CharaMotionType.RoF or CharaMotionType.RoB)
        {
            PlaySe(SeType.Roll);
        }

        // オーバーライン
        if (motionType is CharaMotionType.OvL)
        {
            CallRefereeWhistleOverLine();
        }

        // 天使
        if (motionType is CharaMotionType.ANG)
        {
            MyState.Live.IsDead = true;
            MyState.Coordinate.ZeroVelocity();
            //天使生成
            CallTeamGenerateAngel();
        }

        // ドロー、勝ち、負け
        if (motionType is CharaMotionType.DRAW or CharaMotionType.WIN or CharaMotionType.LOSE)
        {
            MyState.Coordinate.SetGameOverCoordinate(MySideIndex, MyState.Order.IsInfield);
        }

        // 接地した
        if (lastMotionFlag.HasFlag(CharaMotionFlag.Ar)
            && MyState.Motion.HasFlag(CharaMotionFlag.Ar) == false)
        {
            MyState.Air.Initialize();
        }

        // フリーアクションフラグ管理
        if (MyState.Auto.AutoType == AutoType.Free
            && MyState.Motion.HasFlag(CharaMotionFlag.Act))
        {
            MyState.Auto.IsFreeAction = true;
        }
        else if (motionType is CharaMotionType.JCr or CharaMotionType.JUp)
        {
            // フリー状態を継続
        }
        else
        {
            MyState.Auto.IsFreeAction = false;
        }

        // ダッシュ終了
        if (MyState.Motion.HasFlag(CharaMotionFlag.Ds) == false
            && MyState.Dashman.IsCalledDashman)
        {
            MyState.Dashman.Initialize();
        }

        // コマスタート処理
        StartKoma(false);
    }

    private CharaMotionType ShiftMotionType(CharaMotionType motionType)
    {
        // 空中の場合、地上モーションを空中用に変更
        if (MyState.Motion.HasFlag(CharaMotionFlag.Ar))
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
        if (motionType == CharaMotionType.ANG && RefereeState.IsGameSet)
        {
            motionType = MyState.Motion.MotionType == CharaMotionType.FlF
                ? CharaMotionType.DnF
                : CharaMotionType.DnB;
            // TODO:審判にアクセスするタイミングとしてはここではないので移動する
            //  st_.pmgRf_->SetMotion(dbrfLongWhistle);
        }

        // 下降中はジャンプ下降中に変更
        if (motionType == CharaMotionType.JUp && MyState.Coordinate.VelocityY < 0)
        {
            motionType = CharaMotionType.JDn;
        }

        return motionType;
    }

    private bool CanSetMotion(CharaMotionType motionType, bool isForce)
    {
        // 同じモーション間は変更不可
        if (isForce == false
            && MyState.Motion.MotionType == motionType)
        {
            return false;
        }

        // 天使状態は変更付加
        if (MyState.Motion.MotionType == CharaMotionType.ANG)
        {
            return false;
        }

        // 勝ち負けポーズ同士の変更不可
        if (MyState.Motion.HasFlag(CharaMotionFlag.RES)
            && motionType is CharaMotionType.WIN or CharaMotionType.LOSE or CharaMotionType.DRAW)
        {
            return false;
        }

        // かがみは立ち歩き走り以外ではならない
        if (motionType == CharaMotionType.KG)
        {
            if (MyState.Motion.MotionType != CharaMotionType.St
                && MyState.Motion.MotionType != CharaMotionType.Wk
                && MyState.Motion.MotionType != CharaMotionType.Ds
                && MyState.Motion.MotionType != CharaMotionType.KG)
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
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.STF,
                    CharaMotionFlag.JpOK | CharaMotionFlag.Muki | CharaMotionFlag.DmOK | CharaMotionFlag.AtCa,
                    MyState.Coordinate.DirectionZ);
                break;
            // ブレス
            case CharaMotionType.Breath:
                // 扱いは立ち状態
                MyState.Motion.SetMotionNo(CharaMotionType.St, CharaMotionNo.BRF,
                    CharaMotionFlag.JpOK | CharaMotionFlag.Muki | CharaMotionFlag.DmOK | CharaMotionFlag.AtCa,
                    MyState.Coordinate.DirectionZ);
                break;
            // 歩き
            case CharaMotionType.Wk:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.WKF,
                    CharaMotionFlag.JpOK | CharaMotionFlag.Muki | CharaMotionFlag.DmOK | CharaMotionFlag.AtCa,
                    MyState.Coordinate.DirectionZ);
                break;
            // ダッシュ
            case CharaMotionType.Ds:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.DS,
                    CharaMotionFlag.Ds | CharaMotionFlag.JpOK | CharaMotionFlag.Muki | CharaMotionFlag.AtCa,
                    MyState.Coordinate.DirectionZ);
                break;
            // ジャンプ前しゃがみ
            case CharaMotionType.JCr:
            {
                var flag = MyState.Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionFlag.Act | CharaMotionFlag.Ar | CharaMotionFlag.Ds
                    : CharaMotionFlag.Act | CharaMotionFlag.Ar;
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.JCF, flag,
                    MyState.Coordinate.DirectionZ);
            }
                break;
            // キャンセルジャンプ前しゃがみ
            case CharaMotionType.CJCr:
                // ダッシュフラグ消す、扱いはジャンプ前しゃがみ
                MyState.Motion.SetMotionNo(CharaMotionType.JCr, CharaMotionNo.CRF,
                    CharaMotionFlag.Act | CharaMotionFlag.Ar,
                    MyState.Coordinate.DirectionZ);
                break;
            // ジャンプ
            case CharaMotionType.JUp:
            case CharaMotionType.JDn:
            {
                var motionNo = motionType is CharaMotionType.JUp
                    ? CharaMotionNo.AIRUPF
                    : CharaMotionNo.AIRDNF;
                var flag = MyState.Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionFlag.Ar | CharaMotionFlag.Muki | CharaMotionFlag.AtCa | CharaMotionFlag.Ds
                    : CharaMotionFlag.Ar | CharaMotionFlag.Muki | CharaMotionFlag.AtCa;
                MyState.Motion.SetMotionNo(motionType, motionNo, flag,
                    MyState.Coordinate.DirectionZ);
            }
                break;
            // 空中復帰
            case CharaMotionType.ARv:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.AIRDNF,
                    CharaMotionFlag.Ar | CharaMotionFlag.Muki | CharaMotionFlag.AtCa,
                    MyState.Coordinate.DirectionZ);
                break;
            // しゃがみ
            case CharaMotionType.Cr:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.CRF,
                    CharaMotionFlag.Act | CharaMotionFlag.AtCa,
                    MyState.Coordinate.DirectionZ);
                break;
            // ふっとび
            case CharaMotionType.FlF:
            case CharaMotionType.FlB:
            {
                var motionNo = motionType is CharaMotionType.FlF
                    ? CharaMotionNo.FLF
                    : CharaMotionNo.FLB;
                MyState.Motion.SetMotionNo(motionType, motionNo,
                    CharaMotionFlag.Ar | CharaMotionFlag.Dam | CharaMotionFlag.MTK,
                    MyState.Coordinate.DirectionZ);
            }
                break;
            // パスヒット
            case CharaMotionType.PHF:
            case CharaMotionType.PHB:
            {
                var motionNo = motionType is CharaMotionType.PHF
                    ? CharaMotionNo.PHF
                    : CharaMotionNo.PHB;
                MyState.Motion.SetMotionNo(motionType, motionNo, CharaMotionFlag.PHit, MyState.Coordinate.DirectionZ);
            }
                break;
            // ダウンヒット
            case CharaMotionType.DnHF:
            case CharaMotionType.DnHB:
            {
                var motionNo = motionType is CharaMotionType.DnHF
                    ? CharaMotionNo.DNHF
                    : CharaMotionNo.DNHB;
                MyState.Motion.SetMotionNo(motionType, motionNo,
                    CharaMotionFlag.Dam | CharaMotionFlag.Dn, MyState.Coordinate.DirectionZ);
            }
                break;
            // かがみ
            case CharaMotionType.KG:
            {
                var motionNo = MyState.Live.Hp > Defines.KAGAMI2HP
                    ? CharaMotionNo.KG
                    : CharaMotionNo.KG2;
                MyState.Motion.SetMotionNo(motionType, motionNo,
                    CharaMotionFlag.Dam | CharaMotionFlag.KG, MyState.Coordinate.DirectionZ);
            }
                break;
            // ダウン
            case CharaMotionType.DnF:
            case CharaMotionType.DnB:
            {
                var motionNo = motionType is CharaMotionType.DnF
                    ? CharaMotionNo.DNF
                    : CharaMotionNo.DNB;
                MyState.Motion.SetMotionNo(motionType, motionNo,
                    CharaMotionFlag.Dam | CharaMotionFlag.Dn, MyState.Coordinate.DirectionZ);
            }
                break;
            // ダウンからの復帰
            case CharaMotionType.DRv:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.DRVF,
                    CharaMotionFlag.Dam | CharaMotionFlag.Dn, MyState.Coordinate.DirectionZ);
                break;
            // キャッチモーション
            case CharaMotionType.CM:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.CMF,
                    CharaMotionFlag.Act, MyState.Coordinate.DirectionZ);
                break;
            case CharaMotionType.JCM:
                break;
            // ファンブル
            case CharaMotionType.FB:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.FBF,
                    CharaMotionFlag.Act,
                    MyState.Coordinate.DirectionZ);
                break;
            // ジャンプファンブル
            case CharaMotionType.JFB:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.JFBF,
                    CharaMotionFlag.Act,
                    MyState.Coordinate.DirectionZ);
                break;
            // パス待ち
            case CharaMotionType.PW:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.PWF,
                    CharaMotionFlag.JpOK | CharaMotionFlag.Muki | CharaMotionFlag.PW | CharaMotionFlag.DmOK,
                    MyState.Coordinate.DirectionZ);
                break;
            // パス待ち歩き
            case CharaMotionType.PWWk:
                // 歩き扱い
                MyState.Motion.SetMotionNo(CharaMotionType.Wk, CharaMotionNo.PWWKF,
                    CharaMotionFlag.JpOK | CharaMotionFlag.Muki | CharaMotionFlag.PW | CharaMotionFlag.DmOK,
                    MyState.Coordinate.DirectionZ);
                break;
            // パス待ちダッシュ
            case CharaMotionType.PWDs:
                // ダッシュ扱い
                MyState.Motion.SetMotionNo(CharaMotionType.Ds, CharaMotionNo.PWDS,
                    CharaMotionFlag.Ds | CharaMotionFlag.JpOK | CharaMotionFlag.PW,
                    MyState.Coordinate.DirectionZ);
                break;
            // スリップ
            case CharaMotionType.Sl:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.SL,
                    CharaMotionFlag.Act | CharaMotionFlag.JpOK | CharaMotionFlag.Slip | CharaMotionFlag.AtCa,
                    MyState.Coordinate.DirectionZ);
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
                if (MyState.Motion.HasFlag(CharaMotionFlag.Slip))
                {
                    flag |= CharaMotionFlag.Slip;
                }
                else if (MyState.Motion.HasFlag(CharaMotionFlag.Ds))
                {
                    flag |= CharaMotionFlag.Ds;
                }

                MyState.Motion.SetMotionNo(type, motionNo, flag, MyState.Coordinate.DirectionZ);
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

                var flag = MyState.Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionFlag.Ar | CharaMotionFlag.Act | CharaMotionFlag.Ds
                    : CharaMotionFlag.Ar | CharaMotionFlag.Act;

                MyState.Motion.SetMotionNo(type, motionNo, flag, MyState.Coordinate.DirectionZ);
            }
                break;
            // キャッチ
            case CharaMotionType.Ca:
            {
                var flag = MyState.Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionFlag.Act | CharaMotionFlag.Ds
                    : CharaMotionFlag.Act;

                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.CAF,
                    flag, MyState.Coordinate.DirectionZ);
            }
                break;
            // ジャンプキャッチ
            case CharaMotionType.JCa:
            {
                var flag = MyState.Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionFlag.Ar | CharaMotionFlag.Act | CharaMotionFlag.Ds
                    : CharaMotionFlag.Ar | CharaMotionFlag.Act;

                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.CAF,
                    flag, MyState.Coordinate.DirectionZ);
            }
                break;
            // よけ
            case CharaMotionType.Dg:
            {
                var flag = MyState.Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionFlag.Act | CharaMotionFlag.Ds
                    : CharaMotionFlag.Act;

                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.DGF,
                    flag, MyState.Coordinate.DirectionZ);
            }
                break;
            // ジャンプよけ
            case CharaMotionType.JDg:
            {
                var flag = MyState.Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionFlag.Ar | CharaMotionFlag.Act | CharaMotionFlag.Ds
                    : CharaMotionFlag.Ar | CharaMotionFlag.Act;

                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.JDGF,
                    flag, MyState.Coordinate.DirectionZ);
            }
                break;
            // ごろごろ
            case CharaMotionType.RoF:
            case CharaMotionType.RoB:
            {
                var motionNo = motionType is CharaMotionType.RoF
                    ? CharaMotionNo.ROF
                    : CharaMotionNo.ROB;
                MyState.Motion.SetMotionNo(motionType, motionNo,
                    CharaMotionFlag.Dam | CharaMotionFlag.Dn, MyState.Coordinate.DirectionZ);
            }
                break;
            case CharaMotionType.DRAW:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.DRAW,
                    CharaMotionFlag.RES, MyState.Coordinate.DirectionZ);
                break;
            case CharaMotionType.WIN:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.WIN,
                    CharaMotionFlag.RES, MyState.Coordinate.DirectionZ);
                break;
            case CharaMotionType.LOSE:
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.LOSE,
                    CharaMotionFlag.RES, MyState.Coordinate.DirectionZ);
                break;
            // オーバーライン
            case CharaMotionType.OvL:
            {
                var flag = MyState.Motion.HasFlag(CharaMotionFlag.Ds)
                    ? CharaMotionFlag.Act | CharaMotionFlag.Ds
                    : CharaMotionFlag.Act;

                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.JDGF,
                    flag, MyState.Coordinate.DirectionZ);
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
                MyState.Motion.SetMotionNo(motionType, CharaMotionNo.ANG,
                    CharaMotionFlag.Ar | CharaMotionFlag.MTK | CharaMotionFlag.Dam | CharaMotionFlag.ANG,
                    MyState.Coordinate.DirectionZ);
                break;
            default:
                throw new ArgumentOutOfRangeException(nameof(motionType), motionType, null);
        }
    }

    private void PlayBaseMotionKomaSe()
    {
        var se = CurrentBaseMotionKoma.Se;
        PlaySe(se);
    }

    private void InvokeActionPoint()
    {
        switch (MyState.Motion.MotionType)
        {
            case CharaMotionType.Ds:
                MyState.Shoot.Step.Add();
                if (MyState.Shoot.Step.Value % 2 == 1)
                {
                    PlaySe(SeType.Dash);
                }
                break;
            case CharaMotionType.Sh:
                SetShoot(false);
                break;

            case CharaMotionType.JSh:
                SetShoot(true);
                break;

            case CharaMotionType.Pa:
            case CharaMotionType.JPa:
                SetPass();
                break;

            case CharaMotionType.Ca:
            case CharaMotionType.JCa:
                MyState.Catch.CatchCount.Clear();
                break;
            case CharaMotionType.OvL:
                if (IsBallHolder)
                {
                    CallRefereeResetOverLine();
                }
                break;
        }
    }

    /// <summary>
    /// コマスタート
    /// </summary>
    private void StartKoma(bool isInLoop)
    {
        var komaData = CurrentBaseMotionKoma;
        MyState.Motion.StartKoma(komaData);
        if (CurrentBaseMotionKoma.IsActionPoint)
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
        MyState.Motion.IncKomaNo();
        StartKoma(false);
    }

    /// <summary>
    /// ループスタートのコマに戻る
    /// </summary>
    private void GotoLoopStartKoma()
    {
        MyState.Motion.BackToLoopStartKomaNo();
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
        var velocityY = MyState.Motion.HasFlag(CharaMotionFlag.Ds)
            ? GetSettingJump(SettingJumpType.DashJump_vY0)
            : GetSettingJump(SettingJumpType.Jump_vY0);

        MyState.Coordinate.VelocityY = velocityY;

        // 頂点タイミング
        var gravity = GetSettingGravity(SettingGravityType.GRV_Y);
        var topTiming = velocityY / gravity;
        MyState.Air.TopTiming = topTiming;

        if (IsControl == false
            && IsDashman == false
            && MyState.Air.IsVerticalJump)
        {
            MyState.Coordinate.VelocityX = 0;
            MyState.Coordinate.VelocityZ = 0;
        }
        else if (MyState.Motion.HasFlag(CharaMotionFlag.Ds))
        {
            var xSign = (int)MyState.Coordinate.DashDirection;
            var isLongJump = false;
            if (IsCom)
            {
                // 何らかの形でロングジャンプ判定
            }
            else
            {
                isLongJump = IsPressedDashAccelKey || MyState.Air.IsLongJump;
            }

            var dX = isLongJump
                ? GetSpeedRank(RankSpeedType.LDSJPX)
                : GetSettingCourt(SetingCourtType.DashJpX);

            MyState.Coordinate.VelocityX = xSign * dX;

            var zSign = 0;
            if (IsControl && IsCom == false)
            {
                if (MyPad.KeyUp.IsPressed)
                {
                    zSign = +1;
                }
                else if (MyPad.KeyDown.IsPressed)
                {
                    zSign = -1;
                }
            }

            MyState.Coordinate.VelocityZ = zSign * DiagonalWalkJumpVelocityZ;
        }
        else
        {
            var orderType = MyState.Order.GetOrderType();
            var xSign = 0;
            var zSign = 0;
            if (IsCom == false && (IsFree(true) || MyState.Auto.IsFreeAction))
            {
                if (orderType != OrderType.Outfield2 && MyPad.KeyUp.IsPressed)
                {
                    zSign = +1;
                }
                else if (orderType != OrderType.Outfield3 && MyPad.KeyDown.IsPressed)
                {
                    zSign = -1;
                }

                if ((MySideIndex == 1 || orderType != OrderType.Outfield4) && MyPad.KeyRight.IsPressed)
                {
                    xSign = +1;
                }
                else if ((MySideIndex == 0 || orderType != OrderType.Outfield4) && MyPad.KeyLeft.IsPressed)
                {
                    xSign = -1;
                }
            }
            else if (MyState.Auto.AutoType == AutoType.Return)
            {
                xSign = (int)MyState.Auto.DirectionX;
                zSign = (int)MyState.Auto.DirectionZ;
            }

            var idDiagonal = xSign != 0 && zSign != 0;

            var dx = idDiagonal
                ? zSign * DiagonalWalkJumpVelocityX
                : zSign * WalkJumpVelocityX;

            var dz = idDiagonal
                ? zSign * DiagonalWalkJumpVelocityZ
                : zSign * WalkJumpVelocityZ;

            if (orderType is OrderType.Outfield2 or OrderType.Outfield3)
            {
                dz = dz * Defines.Percent / Defines.JPGAIYAPER;
            }

            MyState.Coordinate.VelocityX = dx;
            MyState.Coordinate.VelocityZ = dz;
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