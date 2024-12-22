using System;
using isdf;

namespace db;

public partial class CharaBehavior
{
    /// <summary>
    /// 地上アクションを管理するpartialクラス
    /// 地上での攻撃、防御、ジャンプなどの行動を制御します
    /// </summary>

    /// <summary>
    /// 地上での攻撃処理を行います
    /// 地上でのパスやシュートなどの攻撃行動を制御します
    /// </summary>
    private void GroundAttack()
    {
        if (Pad.ButtonA.IsJustPressed)
        {
            //内野の場合後ろ内野内パスなので、相手の方向を向かないと行けない
            Passing();
        }

        if (Pad.ButtonB.IsJustPressed) //シュート入力
        {
            bool atlook_f = Pad.IsPressedAnyCross() == false;
            //外野２３からＺ軸シュートのとき、一応相手の方向を向く
            LookTg(Ball.Main.ShotTargetOrder, false, atlook_f);

            Shooting();
        }
        else
        {
            MirrorAttack();
        }
    }

    /// <summary>
    /// 地上での防御処理を行います
    /// 地上での回避やキャッチなどの防御行動を制御します
    /// </summary>
    private void GroundDefence()
    {
        if (Pad.ButtonA.IsJustPressed) //避けボタン
        {
            SetMotionType(CharaMotionType.Dg);
            PlaySeCatchSe();
        }
        else if (Pad.ButtonB.IsJustPressed && Catch.CatchWaitCountValue == 0) //キャッチ入力
        {
            //キャッチもボール方向向くようにしてみる
            SetCatchMuki();
            SetMotionType(CharaMotionType.Ca);
        }
        else if (IsPickUpPos()) //自動拾い★
        {
            HoldBall(false, false);
        }
    }

    /// <summary>
    /// 地上でのフリー状態の処理を行います
    /// 地上でのボール拾いやキャッチなどの行動を制御します
    /// </summary>
    private void GroundFree()
    {
        if (Pad.IsJustPressedAnyButton() && Catch.CatchWaitCountValue == 0)
        {
            //キャッチもボール方向向くようにしてみる
            SetCatchMuki();
            SetMotionType(CharaMotionType.Ca);
        }
        else if (IsPickUpPos()) //自動拾い★
        {
            HoldBall(false, false);
        }
    }

    /// <summary>
    /// キャンセルジャンプの処理を行います
    /// </summary>
    /// <param name="canselDs_f">ダッシュキャンセルかどうか</param>
    /// <returns>キャンセルジャンプが実行された場合はtrue</returns>
    private bool CanselJump(bool canselDs_f)
    {
        //判定出るまでもしくは6f以内はジャンプキャンセル可能
        //ココでも可能にして大丈夫か

        if (Motion.HasFlag(CharaMotionFlag.Ar)
            || Motion.MotionCountValue >= GetSettingJump(SettingJumpType.JumpCanselTime)
            || Composite.IsSelfControl == false
            || Pad.IsJustPressedAbButton() == false)
        {
            return false;
        }

        //キャンセルが掛かる前のモーションの向きに戻す
        CoordinateSet.DirectionX = Move.LastDirectionX;
        CoordinateSet.DirectionZ = Move.LastDirectionZ;
        JumpSet(false, canselDs_f, false); //ジャンプ

        return true;
    }

    /// <summary>
    /// ジャンプの設定を行います
    /// </summary>
    /// <param name="mfCheck_f">モーションフラグをチェックするかどうか</param>
    /// <param name="canselDs_f">ダッシュキャンセルかどうか</param>
    /// <param name="vjp_f">垂直ジャンプかどうか</param>
    private void JumpSet(bool mfCheck_f, bool canselDs_f, bool vjp_f)
    {
        if (Motion.HasFlag(CharaMotionFlag.PHit))
        {
            return;
        }

        if (mfCheck_f && Motion.HasFlag(CharaMotionFlag.JpOK) == false)
        {
            return;
        }

        if (canselDs_f || Order.IsInfield == false) //外野はダッシュフラグ消す
        {
            MotionSet.SubMotionFlag(CharaMotionFlag.Ds);
        }

        SetMotionType(CharaMotionType.JCr);
        AirSet.IsVerticalJump = vjp_f; //垂直ジャンプ
        AirSet.IsLongJump = vjp_f; //垂直ジャンプ
        AirSet.IsLongJump = Move.IsDashAccelIOS; //ロングジャンプ
        CoordinateSet.ZeroVelocity();
    }
} 