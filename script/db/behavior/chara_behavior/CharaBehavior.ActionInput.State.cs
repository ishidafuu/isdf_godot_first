using System;
using isdf;

namespace db;

public partial class CharaBehavior
{
    /// <summary>
    /// アクションタイプを取得します
    /// 現在の状態に基づいて適切なアクションタイプを決定します
    /// </summary>
    /// <returns>決定されたアクションタイプ</returns>
    private ActionType GetActionType()
    {
        ActionType actionType;

        switch (Ball.Main.MotionType)
        {
            case BallMotionType.Hold when Ball.Main.HolderSide == MySideIndex:
                actionType = ActionType.ATA;
                break;
            case BallMotionType.Free:
            case BallMotionType.Bound:
            case BallMotionType.Referee:
            case BallMotionType.Hold when Ball.Main.HolderSide == MySideIndex:
            case BallMotionType.Pass when Ball.Main.ThrowerSideNo == MySideIndex:
                actionType = ActionType.ATF;
                break;
            default:
                actionType = ActionType.ATD;
                break;
        }
        return actionType;
    }

    /// <summary>
    /// 敵コートを回避できるかどうかを判定します
    /// キャラクターの状態と位置に基づいて敵コートへの侵入を回避できるか判定します
    /// </summary>
    /// <returns>敵コートを回避できる場合はtrue</returns>
    private bool GetCanDodgeEnemyCourt()
    {
        bool result;

        if (Composite.IsBallHolder || Order.IsOutfield)
        {
            // ボール持ち、外野はよけNG
            result = false;
        }
        else if (Composite.IsSelfControl || Auto.IsFreeAction)
        {
            // 自由状態
            result = true;
        }
        else if (Ball.Main.MotionType == BallMotionType.Hold)
        {
            // 敵がボール保持
            result = Ball.Main.HolderSide == EnemySideIndex;
        }
        else if (Ball.Main.MotionType == BallMotionType.Shoot)
        {
            // 味方がターゲットのシュート
            result = Ball.Main.ShotTargetSide == MySideIndex;
        }
        else
        {
            // 敵ボール
            result = Ball.Main.TimerSide == EnemySideIndex;
        }
        return result;
    }


    /// <summary>
    /// ボール保持時のミラー状態を設定します
    /// ミラーショットとミラーパスの状態を管理します
    /// </summary>
    private void HoldBallSetMirrorState()
    {
        const int activeCount = 1;
        const int inactiveCount = 0;

        if (Ball.Main.MotionType == BallMotionType.Pass
            && Ball.Main.ThrowerSideNo == MySideIndex)
        {
            Pass.MirrorShotCount.Set(activeCount);

            var mirrorPassCount = Pad.ButtonB.IsPressed
                ? activeCount
                : inactiveCount;
            Pass.MirrorPassCount.Set(mirrorPassCount);
        }
        else
        {
            Pass.MirrorPassCount.Set(inactiveCount);

            // キャッチボタン不要で取れるボール
            var isActiveMirrorShot = Ball.Main.MotionType is BallMotionType.Free or BallMotionType.Bound or BallMotionType.Referee
                                     && Damage.FumbleCountValue == 0;

            var mirrorShotCount = isActiveMirrorShot
                ? activeCount
                : inactiveCount;
            Pass.MirrorShotCount.Set(mirrorShotCount);
        }
    }
}