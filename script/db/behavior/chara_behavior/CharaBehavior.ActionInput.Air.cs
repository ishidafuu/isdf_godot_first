using System;
using isdf;

namespace db;

public partial class CharaBehavior
{
    /// <summary>
    /// 空中での攻撃処理を行います
    /// 空中でのパスやシュートなどの攻撃行動を制御します
    /// </summary>
    private void AirAttack()
    {
        if (Pad.ButtonA.IsJustPressed)
        {
            Passing();
        }
        else if (Pad.ButtonB.IsJustPressed)
        {
            bool atlook_f = Pad.IsPressedAnyCross() == false;
            LookTg(Ball.Main.ShotTargetOrder, false, atlook_f);
            SetMotionType(CharaMotionType.JSh);
        }
        else
        {
            MirrorAttack();
        }
    }

    /// <summary>
    /// 空中での防御/フリー状態の処理を行います
    /// 空中でのキャッチや回避などの防御行動を制御します
    /// </summary>
    private void AirDefenceOrFree()
    {
        if (Pad.ButtonA.IsJustPressed)
        {
            SetMotionType(CharaMotionType.JDg);
            // キャッチ時間で変えるとよいかも
            PlaySeCatchSe();

            // パスをスルーしてカバーマンに操作権を渡す
            if (Ball.Main.MotionType == BallMotionType.Pass
                && Ball.Main.PassTargetSide == MySideIndex
                && MyTeam.Main.ControlOrderIndex == Ball.Main.PassTargetOrder
                && Ball.Main.PassTargetOrder >= OrderIndexType.Infield0
                && Ball.Main.PassTargetOrder <= OrderIndexType.Infield3)
            {
                MyTeam.CallChangeControlCoverMan();
            }
        }
        else if (Pad.ButtonB.IsJustPressed)
        {
            SetCatchMuki();
            SetMotionType(CharaMotionType.JCa);
            PlaySeCatchSe();
        }
        else if (IsPickUpPos()) //自動拾い★
        {
            HoldBall(false, false);
        }
    }
}