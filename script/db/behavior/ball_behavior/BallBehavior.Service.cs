namespace db;

public partial class BallBehavior
{
    public void CallHold(int sideIndex, OrderIndexType orderIndex)
    {
        // //とりあえずAniNoをクリア
        // st_.pmgBa_->baCommon_.SetNmlBall(FALSE);
        //
        // //タイマーリセット
        // st_.pmgBa_->baCommon_.ResetTimer(st_.mysideNo_, FALSE);
        
        
        // st_.pmgBa_->baCommon_.SetMTypeHold(st_.mysideNo_, st_.posNo_);

        // pmgSG_->stBa_.NGGet_f = FALSE;
        // pmgSG_->stBa_.ShTgTNo = st_.ensideNo_;
        // pmgSG_->stBa_.ShTgPNo = 0;
        // pmgSG_->stBa_.PaTgTNo = st_.mysideNo_;
        // pmgSG_->stBa_.PaTgPNo = 0;
    }

    public void CallChangePassTarget(int sideIndex, OrderIndexType orderIndex)
    {
        
        // 敵チーム側のパスカットキャラセット
        // PaCtTagSet();
    }
    
    //パスカットタゲセット
    // void TChCommon::PaCtTagSet()
    // {
    //     //パスカットキャラセット
    //
    //     //敵の内野と現在パスタゲとの距離
    //     pmgSG_->stBa_.PaCtTNo = st_.ensideNo_;
    //     pmgSG_->stBa_.PaCtPNo = (s32)dbpoI0;
    //
    //     const s32 ZPER = 150;
    //     s32 pcX[DBMEMBER_INF];
    //     s32 pcZ[DBMEMBER_INF];
    //     f32 pcXZ[DBMEMBER_INF];
    //     f32 pcXZ2 = NGNUM;
    //
    //     for (s32 i = 0; i < DBMEMBER_INF; ++i)
    //     {
    //         pcX[i] = (st_.pmgMyTm_->st_.pmgEnTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.X
    //                   - st_.pmgMyTm_->st_.pmgMyCh_[pmgSG_->stBa_.PaTgPNo]->st_.pstMyCh_->Zahyou.X);
    //         pcZ[i] = (st_.pmgMyTm_->st_.pmgEnTm_->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou.Z
    //                   - st_.pmgMyTm_->st_.pmgMyCh_[pmgSG_->stBa_.PaTgPNo]->st_.pstMyCh_->Zahyou.Z);
    //         pcZ[i] = lib_num::Percent(pcZ[i], ZPER);
    //
    //         pcXZ[i] = lib_num::Hypot(pcX[i], pcZ[i]);
    //     }
    //
    //     for (s32 i = 0; i < DBMEMBER_INF; ++i)
    //     {
    //         if ((pcXZ2 == NGNUM) || (pcXZ[i] < pcXZ2))
    //         {
    //             //ボールに直にタゲを入れてしまえばよいか
    //             pmgSG_->stBa_.PaCtPNo = i;
    //             pcXZ2 = pcXZ[i];
    //         }
    //     }
    // }
}