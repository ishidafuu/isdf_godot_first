namespace db;

public partial class CharaBehavior
{
    private void SetPassing()
    {
        
    }
    
  // //パス
  // void TChCommon::SetPass()
  // {
  //   //攻撃時間で手放された
  //   if (IsBall() == FALSE) return;
  //
  //
  //   const s32 BALLHOLDY = 16;//ちょい補正
  //   TPassInfo PaDt;
  //   PaDt.Init();
  //
  //
  //   if (IsSelfCtrl())
  //   {
  //     //短い押し＝クイック
  //     PaDt.Quick_f = !(st_.pstMyCh_->TossPass_f);
  //
  //     if (IsInfield() && (st_.pmgMyTm_->st_.pmgMyCh_[pmgSG_->stBa_.PaTgPNo]->st_.posNo_ <= (s32)dbpoI3))
  //     {
  //       //内野間はクイック
  //       PaDt.Quick_f = TRUE;
  //     }
  //   }
  //   else
  //   {
  //     PaDt.Quick_f = !(st_.pmgMyTm_->st_.pstMyTm_->stCOM.toss_f);
  //   }
  //
  //
  //
  //
  //   PaDt.tHoldNo = pmgSG_->stBa_.HoldPNo;
  //   //PaDt.tJPa_f = tJPa_f;
  //
  //   st_.pmgBa_->baCommon_.SetMtype(bmPass);
  //   pmgSG_->stBa_.Zahyou.X = st_.pstMyCh_->Zahyou.X;
  //   pmgSG_->stBa_.Zahyou.Y = st_.pstMyCh_->Zahyou.Y + (BALLHOLDY * XYMAG);
  //   pmgSG_->stBa_.Zahyou.Z = st_.pstMyCh_->Zahyou.Z;
  //
  //   GetPassTagDist(&PaDt);
  //   PGrvSpdSet(&PaDt);
  //   PAngle(&PaDt);
  //
  //   //ボールに情報を送る
  //   pmgSG_->stBa_.Zahyou.dX = PaDt.tdx;
  //   pmgSG_->stBa_.Zahyou.dY = PaDt.tdy;
  //   pmgSG_->stBa_.Zahyou.dZ = PaDt.tdz;
  //
  //   pmgSG_->stBa_.Zahyou.Muki = st_.pstMyCh_->Zahyou.Muki;
  //   pmgSG_->stBa_.Zahyou.MukiZ = st_.pstMyCh_->Zahyou.MukiZ;
  //
  //   pmgSG_->stBa_.PaETA_c = PaDt.PaETA_c;
  //   pmgSG_->stBa_.PaETA = PaDt.PaETA_c;
  //
  //   pmgSG_->stBa_.NoHit_c = pmgEO_->mgDt_.dtSet_.GetDtPass(setPaHitStartTime);
  //   pmgSG_->stBa_.PaSelfHit_c = pmgEO_->mgDt_.dtSet_.GetDtShot(setHitStartTime) * 3;
  //   pmgSG_->stBa_.PaJPa_f = !(PaDt.Quick_f);
  //   pmgSG_->stBa_.PaAlPa_f = (PaDt.alleyoop > 0);
  //   pmgSG_->stBa_.PaLine = PaDt.tPaLine;
  //   pmgSG_->stBa_.PaGrv = PaDt.tPaGrv;
  //
  //   //シュートとしてのデータ
  //   pmgSG_->stSh_.Init();//初期化
  //   pmgSG_->stSh_.idata.spin = 10;
  //
  //   //カバー出す前に落下地点
  //   st_.pmgBa_->baCommon_.GetBoundLandPos(pmgSG_->stBa_.PaGrv);
  //
  //   //カバーマンとりあえず空に★
  //   st_.pmgMyTm_->st_.pstMyTm_->CvrNo = NGNUM;
  //   st_.pmgMyTm_->st_.pstMyTm_->Cvr2No = NGNUM;
  //   //st_.pmgMyTm_->st_.pstMyTm_->Through_f = FALSE;
  //   pmgSG_->stBa_.MvCtrl_f = FALSE;
  //
  //   //ボールマンダッシュ予約を解除(ＣＯＭがダッシュマン以外でも走り出す)
  //   st_.pmgMyTm_->st_.pstMyTm_->DashmanNoBM = NGNUM;
  //
  //   //ダッシュマン以外へのパス
  //   if (PaDt.tdm_f == FALSE)
  //   {
  //     //敵のパスカッターにカーソル移動
  //     st_.pmgEnTm_->SetCtrl(pmgSG_->stBa_.PaCtPNo);
  //
  //     //カバーマン
  //     const s32 PANOMVCTRLTIME = 12;
  //     if (pmgSG_->stBa_.PaETA_c > PANOMVCTRLTIME)
  //     {
  //       pmgSG_->stBa_.MvCtrl_f = TRUE;//操作権委譲のあるパス
  //       st_.pmgMyTm_->SeekCover(PaDt.tHoldNo, pmgSG_->stBa_.PichPNo, pmgSG_->stBa_.PaTgPNo, FALSE);
  //     }
  //
  //   }
  //   else//ダッシュマンへのパス
  //   {
  //     if (IsInfield() == FALSE)//自分が外野の時は敵のパスカッターにカーソル移動★
  //     {
  //       st_.pmgEnTm_->SetCtrl(pmgSG_->stBa_.PaCtPNo);
  //     }
  //
  //
  //     //ボールダッシュマン
  //     if ((IsBMDashOK() && PaDt.Quick_f)
  //       //|| st_.pmgMyTm_->st_.pstMyTm_->COMDt.setterBMRsvGo_f
  //       )//敵方向ダッシュが入っていたら予約//トスパスの時はダッシュ継続しない
  //     {
  //       st_.pmgMyTm_->st_.pstMyTm_->DashmanNoBM = st_.posNo_;//予約
  //     }
  //   }
  //
  //   CatchSE();
  //   //st_.pmgBa_->baCommon_.HomingSESet(sePass);
  //
  //   //★成績//パス数合計
  //   AddRec(recPassTry_c);
  //   //RecMsg(AddRec(recPass_c, "パス数");
  //
  // }
}