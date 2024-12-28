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
          || ((Auto.AutoType == AutoType.CPUShPa) && MyTeam.AiMain.QuickF)
          || Pad.IsDashAcc(Coordinate.DashDirection.ToCrossType()))
        {
            MoveSet.DashAccelCount.Add();
        }
        else
        {
            MoveSet.DashAccelCount.Set(0);
        }

        var dsx = GetSettingCourt(SetingCourtType.DashX);

        CoordinateSet.VelocityX = (Coordinate.DashDirection == DirectionXType.Left)
            ? -dsx
            : +dsx;

        //Z
        if (GetMoveMukiZ(false) != DirectionZType.Neutral)
        {
            var dsz = GetSettingCourt(SetingCourtType.DashZ);
            CoordinateSet.VelocityZ = (GetMoveMukiZ(false) != DirectionZType.Forward)
              ? +dsz
              : -dsz;
        }
        else
        {
            CoordinateSet.VelocityZ = 0;
        }

        var tacc = (MoveSet.DashAccelCount.Value * GetSpeedRank(RankSpeedType.DashAcc));

        //限界加速
        if ((Auto.AutoType == AutoType.Return)
          || (Auto.AutoType == AutoType.KgReturn)
          || (tacc > GetSpeedRank(RankSpeedType.DashMAXAcc)))
        {

            MoveSet.DashAccelCount.Set(GetSpeedRank(RankSpeedType.DashMAXAcc) / GetSpeedRank(RankSpeedType.DashAcc));
            tacc = GetSpeedRank(RankSpeedType.DashMAXAcc);
        }

        if (MoveSet.DashAccelCount.Value > 0)
        {
            CoordinateSet.VelocityX = Coordinate.VelocityX + (Coordinate.VelocityX > 0 ? 1 : -1) * tacc;
        }

        ////ぬかるみ★
        //if (pmgEO_->mgStSn_.EtcSS.MudCourt_f())
        //{
        //    const var LOSPER = pmgEO_->mgDt_.dtSet_.GetDtCourt(setDsLossPer);
        //    const var MADMAX = (100 / LOSPER);
        //    if (st_.pstMyCh_->MadStep_c < MADMAX)
        //    {
        //        var nowlosper = LOSPER * st_.pstMyCh_->MadStep_c;
        //        st_.pstMyCh_->Zahyou.dX = lib_num::Percent(st_.pstMyCh_->Zahyou.dX, nowlosper);
        //        st_.pstMyCh_->Zahyou.dZ = lib_num::Percent(st_.pstMyCh_->Zahyou.dZ, nowlosper);
        //    }
        //}
    }
}