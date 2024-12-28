using System;
using isdf;

namespace db;

public partial class CharaBehavior
{
    /// <summary>
    /// パス処理を実行します
    /// パスの方向設定とモーション実行を行います
    /// </summary>
    private void Passing()
    {
        //パスタゲがパス出せないとき
        // NGPaTagShift();

        //内野の場合後ろ内野内パスなので、
        //相手の方向を向かないと行けない
        //★ミラーダッシュマンのときはそちらを向かない
        //if (st_.pmgMyTm_.st_.pmgMyCh_[pmgSG_.stBa_.PaTgPNo].IsDashman() == false)
        //{
        LookTg(Ball.Main.PassTargetOrder, true, false);
        //}

        CharaMotionType passMotion = Motion.HasFlag(CharaMotionFlag.Ar)
            ? CharaMotionType.JPa
            : CharaMotionType.Pa;
        SetMotionType(passMotion);
    }

    /// <summary>
    /// シュート処理を実行します
    /// シュートモーションの設定と実行を行います
    /// </summary>
    private void Shooting()
    {
        //ダッシュ方向とシュート方向があっているときは振り返り扱いにしない
        bool nortst_f = Motion.HasFlag(CharaMotionFlag.Ds)
                        && Coordinate.DashDirection == Coordinate.DirectionX;

        //内野で向きに変わるときは振り向きシュート
        bool isReturnShot = Order.IsInfield
                            && nortst_f == false
                            && Move.LastDirectionX != Coordinate.DirectionX;

        CharaMotionType shootMotion;

        if (Motion.HasFlag(CharaMotionFlag.Ar))
        {
            shootMotion = CharaMotionType.JSh;
        }
        else
        {
            shootMotion = isReturnShot ? CharaMotionType.RtSh : CharaMotionType.Sh;
        }

        SetMotionType(shootMotion);
    }

    /// <summary>
    /// ミラー攻撃の処理を行います
    /// ミラーパスとミラーシュートの制御を行います
    /// </summary>
    private void MirrorAttack()
    {
        if (Pad.ButtonB.IsPressed) //シュート入力おしっぱ
        {
            Pass.MirrorPassCount.Clear();
        }

        if (Pass.MirrorPassCount.Value > 0) //ミラーパス状態
        {
            if (Pad.ButtonA.IsPressed)
            {
                if (Pass.MirrorPassCount.AddUntil(Defines.MIRWAIT))
                {
                    if (Ball.Main.PassTargetOrder != OrderIndexType.Disabled)
                    {
                        var chara = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, Ball.Main.PassTargetOrder);

                        if (chara.Composite.IsDashman)
                        {
                            Passing();
                        }
                        else
                        {
                            Pass.MirrorPassCount.Clear();
                        }
                    }
                }
            }
            else
            {
                Pass.MirrorPassCount.Clear();
            }
        }

        if (Pass.MirrorShotCount.Value > 0) //ミラーシュート状態
        {
            if (Pad.ButtonB.IsPressed && Pad.ButtonA.IsPressed == false) //シュート入力おしっぱ
            {
                if (Pass.MirrorPassCount.AddUntil(Defines.MIRWAIT))
                {
                    LookTg(Ball.Main.ShotTargetOrder, false, true);
                    Shooting();
                }
            }
            else
            {
                Pass.MirrorShotCount.Set(1);
            }
        }
    }
}