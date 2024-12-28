using System;
using isdf;

namespace db;

public partial class CharaBehavior
{

    //ダッシュ
    private void MvDs()
    {

        //加速カウンタ
        if ((Auto.AutoType == AutoType.Return)
          || (Auto.AutoType == AutoType.KgReturn)
          || (Auto.AutoType == AutoType.Get)
          || ((Auto.AutoType == AutoType.CPUShPa) && MyTeam.AiMain.QuickF))
        {
            MoveSet.DashAccelCount.Add();
        }
        else if (Pad.IsDashAcc())
        {
            ++st_.pstMyCh_->DsAcc_c;
        }
        else
        {
            lib_num::AprTo0(&st_.pstMyCh_->DsAcc_c);
        }

        s32 dsx = pmgEO_->mgDt_.dtSet_.GetDtCourt(setDashX);


        st_.pstMyCh_->Zahyou.dX = (st_.pstMyCh_->Zahyou.DsMuki == mL)
          ? -dsx
          : +dsx;

        //Z
        if (pCommon_->GetMoveMukiZ(FALSE) != mzaN)
        {
            s32 dsz = pmgEO_->mgDt_.dtSet_.GetDtCourt(setDashZ);
            st_.pstMyCh_->Zahyou.dZ = (pCommon_->GetMoveMukiZ(FALSE) != mzaF)
              ? +dsz
              : -dsz;
        }
        else
        {
            st_.pstMyCh_->Zahyou.dZ = 0;
        }

        //加速

        s32 tacc = (st_.pstMyCh_->DsAcc_c * RankSpeed(rkDashAcc));

        //限界加速
        if ((st_.pstMyCh_->NextAuto.AutoType == dbatReturn)
          || (st_.pstMyCh_->NextAuto.AutoType == dbatKgReturn)
          || (tacc > RankSpeed(rkDashMAXAcc)))
        {
            //            if (BackDs_f() == FALSE)//バックダッシュ以外を切ってみる//オートバックダッシュ
            {
                st_.pstMyCh_->DsAcc_c = RankSpeed(rkDashMAXAcc) / RankSpeed(rkDashAcc);
                tacc = RankSpeed(rkDashMAXAcc);
            }
        }

        if (st_.pstMyCh_->DsAcc_c > 0)
        {
            st_.pstMyCh_->Zahyou.dX = (st_.pstMyCh_->Zahyou.dX + (lib_num::Sign(st_.pstMyCh_->Zahyou.dX) * (tacc)));
        }

        ////ぬかるみ★
        //if (pmgEO_->mgStSn_.EtcSS.MudCourt_f())
        //{
        //    const s32 LOSPER = pmgEO_->mgDt_.dtSet_.GetDtCourt(setDsLossPer);
        //    const s32 MADMAX = (100 / LOSPER);
        //    if (st_.pstMyCh_->MadStep_c < MADMAX)
        //    {
        //        s32 nowlosper = LOSPER * st_.pstMyCh_->MadStep_c;
        //        st_.pstMyCh_->Zahyou.dX = lib_num::Percent(st_.pstMyCh_->Zahyou.dX, nowlosper);
        //        st_.pstMyCh_->Zahyou.dZ = lib_num::Percent(st_.pstMyCh_->Zahyou.dZ, nowlosper);
        //    }
        //}
    }
}