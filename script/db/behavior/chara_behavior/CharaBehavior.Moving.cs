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
            || Order.OrderIndex == OrderIndexType.O3)
        {
#if DEBUG_SHIAI
            if (state.MySideNo == 0 && DebugSystem.Instance.IsNotWalk())
            {
                return;
            }
#endif
            if (common.GetMoveMuki(false) == Direction.Left)
            {
                state.MyChar.Zahyou.dX = -common.GetWkX();
            }
            else if (common.GetMoveMuki(false) == Direction.Right)
            {
                state.MyChar.Zahyou.dX = common.GetWkX();
            }
        }

        //Z
        if (IsInfield()
            || state.MyChar.Auto.AutoType != AutoType.Free
            || state.PosNo == (int)Position.O4)
        {
#if DEBUG_SHIAI
            if (state.MySideNo == 0 && DebugSystem.Instance.IsNotWalk())
            {
                return;
            }
#endif
            if (common.GetMoveMukiZ(false) == DirectionZ.Back)
            {
                state.MyChar.Zahyou.dZ = common.GetWkZ();
            }
            else if (common.GetMoveMukiZ(false) == DirectionZ.Front)
            {
                state.MyChar.Zahyou.dZ = -common.GetWkZ();
            }
        }

        //斜め
        if (state.MyChar.Zahyou.dX != 0 && state.MyChar.Zahyou.dZ != 0)
        {
            state.MyChar.Zahyou.dX = LibNum.Sign(state.MyChar.Zahyou.dX) * common.GetNWkX();
            state.MyChar.Zahyou.dZ = LibNum.Sign(state.MyChar.Zahyou.dZ) * common.GetNWkZ();
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
            if (Pad.IsWalk2(dxL))//歩き入力
            {
                return maL;
            }
            else if (MyPad()->IsWalk2(dxR))//歩き入力
            {
                return maR;
            }
            else
            {
                return maN;
            }
        }
        else
        {
            return st_.pstMyCh_->Auto.AMuki;
        }
    }

    enMukiZTypeA TChCommon::GetMoveMukiZ(BOOL Input_f)
    {
        if (IsSelfCtrl() == FALSE)
        {
            return st_.pstMyCh_->Auto.AMukiZ;
        }
        else if (Input_f || (st_.pstMyCh_->Auto.AutoType == dbatFree))
        {
            if (MyPad()->IsWalk2(dxU))//歩き入力
            {
                return mzaB;
            }
            else if (MyPad()->IsWalk2(dxD))//歩き入力
            {
                return mzaF;
            }
            else
            {
                return mzaN;
            }
        }
        else
        {
            return st_.pstMyCh_->Auto.AMukiZ;
        }
    }

    enMukiZTypeA TChCommon::GetMoveMukiZMukiLock(BOOL Input_f)
    {
        if (IsSelfCtrl() == FALSE)
        {
            return st_.pstMyCh_->Auto.AMukiZ;
        }
        else if (Input_f || (st_.pstMyCh_->Auto.AutoType == dbatFree))
        {
            if (MyPad()->IsWalk2(dxD))
            {
                return mzaB;
            }
            else if (MyPad()->IsWalk2(dxU))
            {
                return mzaF;
            }
            else
            {
                return mzaN;
            }
        }
        else
        {
            return st_.pstMyCh_->Auto.AMukiZ;
        }
    }
}
