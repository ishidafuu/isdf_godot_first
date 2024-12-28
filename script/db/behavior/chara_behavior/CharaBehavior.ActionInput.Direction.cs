using System;
using isdf;

namespace db;

public partial class CharaBehavior
{

    /// <summary>
    /// キャッチ時の向き設定を行います
    /// ボールをキャッチする際の適切な向きを設定します
    /// </summary>
    private void SetCatchMuki()
    {
        AutoMukiInit();
        SetBallMukiX();
        SetBallMukiZ();
        MukiSetAuto();
    }


    /// <summary>
    /// ボールに対するX方向の向きを設定します
    /// ボールの位置に基づいて左右の向きを決定します
    /// </summary>
    private void SetBallMukiX()
    {
        if (Composite.IsBallHolder)
        {
            return;
        }

        //自分がタゲでパスが飛んできてるときはそのままの向き自分が投げたときも
        if (Order.IsInfield == false
            && Ball.Main.MotionType == BallMotionType.Pass
            && Ball.Main.ThrowerSideNo == MySideIndex
            && Ball.Main.ThrowerOrderNo == MyOrderIndex)
        {
            return;
        }

        if (Ball.Main.Coordinate.X < Coordinate.X)
        {
            NextAutoSet.DirectionX = DirectionXType.Left;
        }
        else if (Ball.Main.Coordinate.X < Coordinate.X)
        {
            NextAutoSet.DirectionX = DirectionXType.Right;
        }
    }



    /// <summary>
    /// ボールに対するZ方向の向きを設定します
    /// ボールの位置に基づいて前後の向きを決定します
    /// </summary>
    private void SetBallMukiZ()
    {
        if (Composite.IsBallHolder)
        {
            return;
        }

        // 自分がタゲでパスが飛んできてるときはそのままの向き
        // 自分が投げたときも
        if (Order.IsInfield == false && Ball.Main.MotionType == BallMotionType.Pass)
        {
            if (Ball.Main.ThrowerSideNo == MySideIndex
                && Ball.Main.ThrowerOrderNo == MyOrderIndex)
            {
                return;
            }

            // パスの時はパス先を向く
            NextAutoSet.DirectionZ = Ball.Main.ThrowerOrderNo switch
            {
                OrderIndexType.Outfield2 => DirectionZType.Backward,
                OrderIndexType.Outfield3 => DirectionZType.Forward,
                _ => DirectionZType.Neutral,
            };
        }
        else
        {
            if (Ball.Main.Coordinate.Z < Coordinate.Z)
            {
                if (Ball.Main.Coordinate.Z < Coordinate.Z - Defines.DEFDISTZ)
                {
                    NextAutoSet.DirectionZ = DirectionZType.Forward;
                }
                else if (Ball.Main.Coordinate.Z > Coordinate.Z - Defines.DEFDISTZ / 2
                         || NextAuto.DirectionZ == DirectionZType.Backward)
                {
                    NextAutoSet.DirectionZ = DirectionZType.Neutral;
                }
            }
            else
            {
                if (Ball.Main.Coordinate.Z > Coordinate.Z + Defines.DEFDISTZ)
                {
                    NextAutoSet.DirectionZ = DirectionZType.Backward;
                }
                else if (Ball.Main.Coordinate.Z < Coordinate.Z + Defines.DEFDISTZ / 2
                         || NextAuto.DirectionZ == DirectionZType.Forward)
                {
                    NextAutoSet.DirectionZ = DirectionZType.Neutral;
                }
            }
        }
    }


    /// <summary>
    /// 自動向きの設定を行います
    /// キャラクターの向きを自動的に設定し、変更があったかを返します
    /// </summary>
    /// <returns>向きが変更された場合はtrue</returns>
    private bool MukiSetAuto()
    {
        var lastDirectionX = Coordinate.DirectionX;
        var lastDirectionZ = Coordinate.DirectionZ;

        MoveSet.LastDirectionX = lastDirectionX;
        MoveSet.LastDirectionZ = lastDirectionZ;

        CoordinateSet.DirectionX = Auto.DirectionX switch
        {
            DirectionXType.Left => DirectionXType.Left,
            DirectionXType.Right => DirectionXType.Right,
            _ => Coordinate.DirectionX,
        };

        CoordinateSet.DirectionZ = Auto.DirectionZ switch
        {
            DirectionZType.Forward => DirectionZType.Forward,
            DirectionZType.Backward => DirectionZType.Backward,
            DirectionZType.Neutral => DirectionZType.Neutral,
            _ => Coordinate.DirectionZ,
        };

        // 向き変えありかどうか
        var isChanged = lastDirectionX != Coordinate.DirectionX
                        || lastDirectionZ != Coordinate.DirectionZ;

        if (isChanged)
        {
            //タゲも変える
            ShootSet.Angle12 = GetMukiAglFromDirection();
        }

        return isChanged;
    }

    /// <summary>
    /// 自動向きの初期化を行います
    /// キャラクターの現在の向きを基に自動向きを初期化します
    /// </summary>
    private void ResetAutoDirection()
    {
        AutoSet.DirectionX = Coordinate.DirectionX switch
        {
            DirectionXType.Left => DirectionXType.Left,
            DirectionXType.Neutral => DirectionXType.Neutral,
            DirectionXType.Right => DirectionXType.Right,
            _ => throw new ArgumentOutOfRangeException(),
        };

        AutoSet.DirectionZ = Coordinate.DirectionZ switch
        {
            DirectionZType.Forward => DirectionZType.Forward,
            DirectionZType.Neutral => DirectionZType.Neutral,
            DirectionZType.Backward => DirectionZType.Backward,
            _ => throw new ArgumentOutOfRangeException(),
        };
    }

    /// <summary>
    /// 現在の向きから角度を取得します
    /// キャラクターの向きから12時方向の角度を計算します
    /// </summary>
    /// <returns>12時方向の角度</returns>
    private int GetMukiAglFromDirection()
    {
        return GetMukiAgl(Coordinate.DirectionX, Coordinate.DirectionZ);
    }

    /// <summary>
    /// 指定された向きから角度を取得します
    /// 指定されたX方向とZ方向から12時方向の角度を計算します
    /// </summary>
    /// <param name="directionX">X方向の向き</param>
    /// <param name="directionZ">Z方向の向き</param>
    /// <returns>12時方向の角度</returns>
    private int GetMukiAgl(DirectionXType directionX, DirectionZType directionZ)
    {
        return GetMukiAgl(directionX == DirectionXType.Left,
            directionX == DirectionXType.Right,
            directionZ == DirectionZType.Backward,
            directionZ == DirectionZType.Forward);
    }

    /// <summary>
    /// ターゲットの方向を向く処理を行います
    /// パスやシュート時のターゲットに対する向きを設定します
    /// </summary>
    /// <param name="TgPNo">ターゲットのオーダー</param>
    /// <param name="Pa_f">パス処理かどうか</param>
    /// <param name="AtLook_f">自動向き設定を行うかどうか</param>
    private void LookTg(OrderIndexType TgPNo, bool Pa_f, bool AtLook_f)
    {
        TgPNo = 0;

        CharaBehavior TgSt = null;
        bool Notag_f = false;
        int futureX, futureZ; //SHLAG後の自分の位置

        bool dmPass_f = false;
        bool alleyoop_f = false;

        if (Pa_f)
        {
            if (Ball.Main.PassTargetSide != MySideIndex
                || Ball.Main.PassTargetOrder == OrderIndexType.Disabled)
            {
                Notag_f = true;
            }
            else
            {
                TgSt = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, Ball.Main.PassTargetOrder);
                dmPass_f = TgSt.Composite.IsDashman;
                alleyoop_f = Ball.Main.PaAlPa;
            }
        }
        else
        {
            if (Ball.Main.ShotTargetSide != MySideIndex
                || Ball.Main.ShotTargetOrder == OrderIndexType.Disabled)
            {
                Notag_f = true;
            }
            else //タゲがいる
            {
                TgSt = CharaBehaviorManager.Instance.GetOrderChara(MySideIndex, Ball.Main.ShotTargetOrder);
            }
        }

        // シュートタイミングの位置
        futureX = Coordinate.X + Coordinate.VelocityX * Defines.SHLAG;
        futureZ = Coordinate.Z + Coordinate.VelocityZ * Defines.SHLAG;

        //タゲあり
        if (Notag_f == false && TgSt != null)
        {
            bool near_f = Math.Abs(TgSt.Coordinate.X - futureX) < Defines.NEARDISTX
                          && Math.Abs(TgSt.Coordinate.Z - futureZ) < Defines.NEARDISTZ;

            if (Order.IsInfield == false
                || Pa_f
                || near_f == false)
            {
                //オート向き初期化
                AutoMukiInit();

                var nowDist = TgSt.Coordinate.X - Coordinate.X;
                var futureDist = TgSt.Coordinate.X - futureX;
                var isSameDirection = nowDist * futureDist > 0;

                if (isSameDirection)
                {
                    AutoSet.DirectionX = TgSt.Coordinate.X < futureX
                        ? DirectionXType.Left
                        : DirectionXType.Right;
                }

                //ダッシュマンパス
                if (dmPass_f)
                {
                    AutoSet.DirectionZ = DirectionZType.Neutral;
                }
                else
                {
                    bool Z_f = Math.Abs(TgSt.Coordinate.Z - futureZ) > Math.Abs(TgSt.Coordinate.X - futureX); //★斜めのとき

                    if (Z_f)
                    {
                        AutoSet.DirectionZ = TgSt.Coordinate.Z < futureZ
                            ? DirectionZType.Forward
                            : DirectionZType.Backward;
                    }
                    else
                    {
                        AutoSet.DirectionZ = DirectionZType.Neutral;
                    }
                }

                MukiSetAuto();
            }
        }
        else //タゲ無し
        {
            if (Order.IsInfield == false
                || Composite.IsSelfControl == false
                || (AtLook_f && Notag_f))
            {
                //２，３番外野が真横に投げてしまうのを阻止
                switch (MyOrderIndex)
                {
                    case OrderIndexType.Outfield2:
                        AutoSet.DirectionZ = DirectionZType.Forward;
                        MukiSetAuto();
                        SetShTagFromMyShootAngle12(false);
                        break;
                    case OrderIndexType.Outfield3:
                        AutoSet.DirectionZ = DirectionZType.Backward;
                        MukiSetAuto();
                        SetShTagFromMyShootAngle12(false);
                        break;
                    case OrderIndexType.Outfield4:
                        AutoSet.DirectionX = MySideIndex == 0
                            ? DirectionXType.Left
                            : DirectionXType.Right;
                        MukiSetAuto();
                        SetShTagFromMyShootAngle12(false);
                        break;
                }

                if (Pa_f == false && IsShTag() == false)
                {
                    //全方向チェック
                    var directionX = Coordinate.DirectionX;
                    //ニュートラルから探す
                    var directionZ = DirectionZType.Neutral;

                    if (SearchShootTarget(directionX, directionZ))
                    {
                        SetAutoTarget(directionX, directionZ);
                        return;
                    }

                    directionZ = DirectionZType.Forward;

                    if (SearchShootTarget(directionX, directionZ))
                    {
                        SetAutoTarget(directionX, directionZ);
                        return;
                    }

                    directionZ = DirectionZType.Backward;

                    if (SearchShootTarget(directionX, directionZ))
                    {
                        SetAutoTarget(directionX, directionZ);
                        return;
                    }

                    //逆向き
                    directionX = Coordinate.DirectionX == DirectionXType.Left
                        ? DirectionXType.Right
                        : DirectionXType.Left;

                    //ニュートラルから探す
                    directionZ = DirectionZType.Neutral;

                    if (SearchShootTarget(directionX, directionZ))
                    {
                        SetAutoTarget(directionX, directionZ);
                        return;
                    }

                    directionZ = DirectionZType.Forward;

                    if (SearchShootTarget(directionX, directionZ))
                    {
                        SetAutoTarget(directionX, directionZ);
                        return;
                    }

                    directionZ = DirectionZType.Backward;

                    if (SearchShootTarget(directionX, directionZ))
                    {
                        SetAutoTarget(directionX, directionZ);
                    }
                }
            }
        }
    }

    //新タゲ角度(12時計算)
    private int GetTagAgl2(int tX, int tZ)
    {
        var res = -1;
        var absX = Math.Abs(tX);
        var absZ = Math.Abs(tZ);
        var isMinusX = tX < 0;
        var isMinusZ = tZ < 0;

        const int root3 = 173;
        const int root1 = 100;
        const int angle90 = 3;

        if (absX * root3 < absZ * root1)
        {
            res = -1;
        }
        else if (absZ * root3 < absX * root1)
        {
            res = 0;
        }
        else
        {
            res = +1;
        }

        if (isMinusZ ^ isMinusX) //符号反転
        {
            res *= -1;
        }

        res += 1;

        if (isMinusX && isMinusZ)
        {
            res += angle90 * 2;
        }
        else if (isMinusX)
        {
            res += angle90 * 3;
        }
        else if (isMinusZ)
        {
            res += angle90 * 1;
        }

        return res;
    }

    /// <summary>
    /// シュートターゲットの探索を行います
    /// 指定された方向でシュート可能なターゲットを探します
    /// </summary>
    /// <param name="directionX">X方向の向き</param>
    /// <param name="directionZ">Z方向の向き</param>
    /// <returns>ターゲットが見つかった場合はtrue</returns>
    private bool SearchShootTarget(DirectionXType directionX, DirectionZType directionZ)
    {
        var angle12 = GetMukiAgl(directionX, directionZ);
        return GetShootTarget(angle12, false) != OrderIndexType.Disabled;
    }

    /// <summary>
    /// 自動ターゲットの設定を行います
    /// 指定された方向に基づいて自動的にターゲットを設定します
    /// </summary>
    /// <param name="directionX">X方向の向き</param>
    /// <param name="directionZ">Z方向の向き</param>
    private void SetAutoTarget(DirectionXType directionX, DirectionZType directionZ)
    {
        AutoSet.DirectionX = directionX;
        AutoSet.DirectionZ = directionZ;
        MukiSetAuto();
        SetShTagFromMyShootAngle12(false);
    }


}