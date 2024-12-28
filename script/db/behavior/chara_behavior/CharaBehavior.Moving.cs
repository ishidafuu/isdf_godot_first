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

}
