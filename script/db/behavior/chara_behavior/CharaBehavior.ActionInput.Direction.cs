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
}