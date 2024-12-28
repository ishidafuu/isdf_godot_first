using System;
using isdf;

namespace db;

public partial class CharaBehavior
{
    //座標移動
    public void UpdateMoving()
    {

        // if (IsInplay() == FALSE) return;

        // switch (st_.pstMyCh_->Motion.Mtype)
        // {
        //     //歩き
        //     case dbmtWk: MvWk(); break;
        //     //ダッシュ
        //     case dbmtDs: MvDs(); break;
        //     //ジャンプ
        //     case dbmtJUp:
        //     case dbmtJDn:
        //     case dbmtJCM:
        //     case dbmtJFB:
        //     case dbmtJSh:
        //     case dbmtJPa:
        //     case dbmtJCa:
        //     case dbmtJDg: MvJp(); break;

        //     //しゃがみ
        //     case dbmtCr: MvCr(); break;
        //     //ふっとび
        //     case dbmtFlF:
        //     case dbmtFlB: MvFl(); break;
        //     //ころがり
        //     case dbmtRoF:
        //     case dbmtRoB:
        //         break;
        //     //キャッチモーション
        //     case dbmtCM:
        //     case dbmtFB:
        //         MvCM();
        //         break;
        //     //スリップ
        //     case dbmtSl:
        //         MvDefSl_DSFric();
        //         break;
        //     //シュートパス
        //     case dbmtSh:
        //     case dbmtPa: MvShPa(); break;
        //     //よけキャッチ
        //     case dbmtDg:
        //     case dbmtCa: MvDgCa(); break;
        //     //氷床ダウン滑り
        //     case dbmtDnF:
        //     case dbmtDnB:
        //         //if (pmgEO_->mgStSn_.EtcSS.IceCourt_f())
        //         //{
        //         //    MvDefSl(pmgEO_->mgDt_.dtSet_.GetDtCourt(setDownBrkIce));
        //         //}
        //         //else
        //         {
        //             pCommon_->ZeroXYZ();
        //         }
        //         break;
        //     default:
        //         pCommon_->ZeroXYZ();
        //         break;
        // }


        // //吹っ飛びスキップ
        // if ((st_.pstMyCh_->Motion.Mtype == dbmtFlF)
        //   || (st_.pstMyCh_->Motion.Mtype == dbmtFlB))
        // {
        //     if ((st_.pstMyCh_->FlySkip > 0) && (st_.pstMyCh_->FlySkip_c == 0))
        //     {
        //         for (s32 i = 0; i < st_.pstMyCh_->FlySkip; ++i)
        //         {
        //             //ふっとび
        //             MvFl();
        //             //座標移動
        //             MoveZahyou();
        //         }
        //         st_.pstMyCh_->FlySkip = 0;//スキップ終了
        //     }
        // }
        // else
        // {
        //     st_.pstMyCh_->FlySkip_c = 0;
        // }

        // //座標移動
        // if (st_.pstMyCh_->FlySkip_c == 0)
        // {
        //     MoveZahyou();
        // }

        // //オーバーライン補正
        // CheckOverLine();

        // //着地位置
        // SetLand();

    }

    //歩き
    private void MvWk()
    {
        CoordinateSet.ZeroVelocity();

        //X
        if (Order.IsInfield
            || Auto.AutoType != AutoType.Free
            || Order.OrderIndex == OrderIndexType.Outfield2
            || Order.OrderIndex == OrderIndexType.Outfield3)
        {
            var walkX = GetSpeedRank(RankSpeedType.WkX);
            var direction = GetMoveMuki(false);
            if (direction != DirectionXType.Neutral)
            {
                CoordinateSet.VelocityX = walkX * (direction == DirectionXType.Right ? 1 : -1);
            }
        }

        //Z
        if (Order.IsInfield
            || Auto.AutoType != AutoType.Free
            || Order.OrderIndex == OrderIndexType.Outfield4)
        {

            var walkZ = GetSpeedRank(RankSpeedType.WkX) * Defines.Percent / GetSettingCourt(SetingCourtType.WkZPer);

            var direction = GetMoveMukiZ(false);
            if (direction != DirectionZType.Neutral)
            {
                CoordinateSet.VelocityZ = walkZ * (direction == DirectionZType.Forward ? 1 : -1);
            }
        }

        //斜め
        if (Coordinate.VelocityX != 0 && Coordinate.VelocityZ != 0)
        {
            CoordinateSet.VelocityX = Coordinate.VelocityX * (Coordinate.VelocityX > 0 ? 1 : -1);
            CoordinateSet.VelocityZ = Coordinate.VelocityZ * (Coordinate.VelocityZ > 0 ? 1 : -1);
        }
    }

    private DirectionXType GetMoveMuki(bool Input_f)
    {
        if (Composite.IsSelfControl == false)
        {
            return Auto.DirectionX;
        }
        else if (Input_f || (Auto.AutoType == AutoType.Free))
        {
            if (Pad.IsWalk2(DirectionCrossType.Left))//歩き入力
            {
                return DirectionXType.Left;
            }
            else if (Pad.IsWalk2(DirectionCrossType.Right))//歩き入力
            {
                return DirectionXType.Right;
            }
            else
            {
                return DirectionXType.Neutral;
            }
        }
        else
        {
            return Auto.DirectionX;
        }
    }

    private DirectionZType GetMoveMukiZ(bool Input_f)
    {
        if (Composite.IsSelfControl == false)
        {
            return Auto.DirectionZ;
        }
        else if (Input_f || (Auto.AutoType == AutoType.Free))
        {
            if (Pad.IsWalk2(DirectionCrossType.Up))//歩き入力
            {
                return DirectionZType.Backward;
            }
            else if (Pad.IsWalk2(DirectionCrossType.Down))//歩き入力
            {
                return DirectionZType.Forward;
            }
            else
            {
                return DirectionZType.Neutral;
            }
        }
        else
        {
            return Auto.DirectionZ;
        }
    }

    private DirectionZType GetMoveMukiZMukiLock(bool Input_f)
    {
        if (Composite.IsSelfControl == false)
        {
            return Auto.DirectionZ;
        }
        else if (Input_f || (Auto.AutoType == AutoType.Free))
        {
            if (Pad.IsWalk2(DirectionCrossType.Down))
            {
                return DirectionZType.Backward;
            }
            else if (Pad.IsWalk2(DirectionCrossType.Up))
            {
                return DirectionZType.Forward;
            }
            else
            {
                return DirectionZType.Neutral;
            }
        }
        else
        {
            return Auto.DirectionZ;
        }
    }


}
