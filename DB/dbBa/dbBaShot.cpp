#include "dbBaShot.h"
#include "dbGame.h"

namespace db
{
    //移動////////////////////////////////

    //コンストラクタ
    TBaShot::TBaShot(TGame* pGame, TBaCommon* pCommon)
        : TBaHaveCommon(pGame, pCommon)
    {
    }

    TBaShot::~TBaShot()
    {
    }

    //どこかで設定or計算----------------------------------------------
    const f32 CENTER_LINE = DBCRT_CL / XYMAG;//センターラインX位置
    //const f32 courtEndL;//左コートエンドの位置（どこかで計算の必要有り）
    //const f32 courtEndR;//右コートエンドの位置（どこかで計算の必要有り）
    const f32 SIDE_LINE_DEEP = 98;//奥コートライン
    const f32 SIDE_LINE_SHALLOW = 18;//手前コートライン
    const f32 CHANGE_STOP_TIME = 16;//停止後変化の待ち時間
    const f32 CHANGE_LOOSE_TIME = 12.0;//停止後変化の待ち時間
    //----------------------------------------------------------------

    //XZ→ZXにおける波の減少率　後に100で割る
    const f32 DEC_VAL_ZX = 50.0f;

    //現在の座標に速度を追加
    void TBaShot::NormalMove(f32 flag, f32 valdX, f32 valdY, f32 valdZ)
    {
        //位置に速度を加える
        //X,Z
        st_.pstSh_->phs.posX = st_.pstSh_->phs.posX + (f32)flag * valdX / (f32)XYMAG;//現在の位置xに速度xを足す

        //if (st_.pstSh_->crt.trg.stopForward_Time > 0) flag = TRUE;//初速０処理
        st_.pstSh_->phs.posZ = st_.pstSh_->phs.posZ + (f32)flag * valdZ / (f32)XYMAG;//現在の位置zに速度zを足す

        //Y
        //逆進時で地上シュート高さ未満になったらy速度０
        //        if ((st_.pstSh_->crt.trg.backward_Time > 0) && (st_.pstSh_->phs.posY <= 14.0f)) st_.pstSh_->phs.spdY = 0.0f;

        //強制落下処理ならフラグを１にして強制落下速度を入れる
        if (st_.pstSh_->cmn.shotFinishFall_f == TRUE) valdY = st_.pstSh_->cmn.shotFinishFallSpdY, flag = 1.0f;
        st_.pstSh_->phs.posY = st_.pstSh_->phs.posY + (f32)flag * valdY / (f32)XYMAG;//現在の位置yに速度yを足す
    }

    //波絶対値処理
    f32 TBaShot::CheckWaveAbs(f32 wvAbs_f, s32 wvType, f32 addVal)
    {
        f32 rtVal = 1.0f;//通常では正の値を返す
        //フラグが立っているときのみチェック 一応波データあるかもチェック
        if ((wvAbs_f != 0) && (wvType > 0))
        {
            //正の関数で加算値が負になったら、正にするため返り値を-1に
            if (((wvType % 2) == 1) && (addVal < 0)) rtVal = -1.0f;
            //負の関数で加算値が正になったら、負にするため返り値を-1に
            if (((wvType % 2) == 0) && (addVal > 0)) rtVal = -1.0f;
        }
        return rtVal;
    }

    //敵越えチェック関数１　XorZ
    BOOL TBaShot::CheckBeyondTarget()
    {
        BOOL res = FALSE;
        //初期Ｘシュート
        if (st_.pstSh_->idata.initZShot_f == FALSE)
        {
            res = CheckBeyondTargetXZ(st_.pstSh_->phs.spdX, st_.pstSh_->phs.posX, st_.pstSh_->phs.tagposX);
        }
        else
        {//初期Ｚシュート
            res = CheckBeyondTargetXZ(st_.pstSh_->phs.spdZ, st_.pstSh_->phs.posZ, st_.pstSh_->phs.tagposZ);
        }
        return res;
    }
    //敵越えチェック関数２
    BOOL TBaShot::CheckBeyondTargetXZ(f32 spd, f32 pos, f32 tagpos)
    {
        BOOL res = FALSE;//速度０のときはジャッジせずFALSEでOK
        if (//速度＋　右向き　で標的を超える
            //            ((spd > 0) && ((s32)(tagpos) <= ((s32)st_.pstBa_->Zahyou.X / 100.0f)))
            ((spd > 0) && ((s32)(tagpos) <= pos))
            ||//速度－　左向き　で標的を超える
            //            ((spd < 0) && ((s32)(tagpos) >= ((s32)st_.pstBa_->Zahyou.X / 100.0f)))
            ((spd < 0) && ((s32)(tagpos) >= pos))
            ) res = TRUE;
        return res;
    }

    void TBaShot::GlobalMove()
    {
        //次段階速度XYZ
        f32 XYMAGF = (f32)XYMAG;//float型XYMAG
        f32 valdX = st_.pstSh_->phs.spdX * XYMAGF;
        f32 valdY = (f32)st_.pstSh_->phs.spdY * XYMAGF;
        f32 valdZ = st_.pstSh_->phs.spdZ * XYMAGF;
        //次段階比率と前段階比率
        //次段階比率×次段階速度＋前段階比率×前段階速度＝徐々に変化中速度
        //次段階比率＝時間カウント／徐々に変化の時間⇒（0/n、1/n…n/n）
        //前段階比率＝（徐々に変化の時間－時間カウント）／徐々に変化の時間⇒（n/n、(n-1)/n…0/n）
        //次段階比率＋前段階比率＝n/n＝1になるよう計算
        f32 spdRtNxt = (f32)st_.pstSh_->cmn.timePhase_c / CHANGE_LOOSE_TIME;//次段階速度比率
        f32 spdRtPrv = (f32)(CHANGE_LOOSE_TIME - st_.pstSh_->cmn.timePhase_c) / CHANGE_LOOSE_TIME;//前段階速度比率
        BOOL move_f = TRUE;//一時停止変化ならFALSEになる
        BOOL groundKeep_f = FALSE;//地面についても持続フラグ・・・徐々に系で使用

        //Yのみ徐々に変化
        if (st_.pstSh_->phs.changeMtdFromTarget == 3)
        {
            groundKeep_f = TRUE;
            if (st_.pstSh_->cmn.timePhase_c < (CHANGE_LOOSE_TIME + 1))
            {
                f32 pastSpd = st_.pstSh_->cmn.spdPrY * spdRtPrv;//過去の速度・・・だんだん比率が下がる
                f32 prstSpd = st_.pstSh_->phs.spdY_ChangeYOnly * spdRtNxt;//現在の速度・・・だんだん比率が上がる
                valdY = XYMAGF * (pastSpd + prstSpd);
                if (st_.pstSh_->phs.posY < 0) valdY = 0;
            }
            else if (st_.pstSh_->cmn.timePhase_c == (CHANGE_LOOSE_TIME + 1))
            {

                valdY = XYMAGF * st_.pstSh_->phs.spdY_ChangeYOnly;
                st_.pstSh_->phs.spdY = st_.pstSh_->phs.spdY_ChangeYOnly;
            }
        }
        else
        {
            //前段階の標的素材から得られた変化方法で速度追加の仕方を変える
            switch (st_.pstSh_->cmn.changeMtdPr)
            {
            case 1:
                //停止後変化
                //停止時間内は処理せず、停止時間外なら通常変化
                if (st_.pstSh_->cmn.timePhase_c < CHANGE_STOP_TIME)
                {
                    valdX = 0;
                    valdY = 0;
                    valdZ = 0;
                    move_f = FALSE;
                }
                else if (st_.pstSh_->cmn.timePhase_c == CHANGE_STOP_TIME)
                {
                    st_.pstSh_->SAngle();
                    valdX = st_.pstSh_->phs.spdX * XYMAGF;
                    valdY = st_.pstSh_->phs.spdY * XYMAGF;
                    valdZ = st_.pstSh_->phs.spdZ * XYMAGF;
                    st_.pstSh_->cmn.reSAngle_f = TRUE;
                    st_.pstSh_->cmn.addDir_f = FALSE;//GlobalMoveで波の加算時に敵方向を１回だけ確認するフラグ。
                    st_.pstSh_->phs.accelFuncInit_f = FALSE;
                }
                else CheckDelaySE();
                break;
            case 2:
                //徐々に変化
                groundKeep_f = TRUE;
                if (st_.pstSh_->cmn.timePhase_c < CHANGE_LOOSE_TIME)
                {
                    valdX = (valdX * spdRtNxt) + (st_.pstSh_->cmn.spdPrX * XYMAGF * spdRtPrv);
                    valdY = (valdY * spdRtNxt) + (st_.pstSh_->cmn.spdPrY * XYMAGF * spdRtPrv);
                    valdZ = (valdZ * spdRtNxt) + (st_.pstSh_->cmn.spdPrZ * XYMAGF * spdRtPrv);
//                    if (
//                        (st_.pstSh_->cmn.spdPrX == 0)
//                        &&
//                        (st_.pstSh_->cmn.spdPrY == 0)
//                        &&
//                        (st_.pstSh_->cmn.spdPrZ == 0)
//                    )
                    if (st_.pstSh_->cmn.timePhase_c == (CHANGE_LOOSE_TIME / 2))
                    {
                        CheckDelaySE();
                    }

                }
//                else CheckDelaySE();

                //徐々に変化した後、一度SAngle通して通常変化
                //前段階は上昇、本段階は下降であればSAngle
                if (//ユル変化時間マックスのときに
                    (st_.pstSh_->cmn.timePhase_c == CHANGE_LOOSE_TIME)
                    &&
                    (//放物線軌道段階or下降中ならSAngle処理
                     (st_.pstSh_->accdt.plAccOrb_f == TRUE) //山なり＋加速軌道素材
                     || (st_.pstSh_->accdt.palabOrb_f == TRUE)//山なり軌道素材
                     || (st_.pstSh_->accdt.yAccelEff_f == TRUE)//y加速効果素材
                     || (st_.pstSh_->accdt.yxAccelEff_f == TRUE)//yx加速効果素材
                     || (valdY < 0)
                     )
                    )
                {
                    st_.pstSh_->SAngle();
                    valdX = st_.pstSh_->phs.spdX * XYMAGF;
                    valdY = st_.pstSh_->phs.spdY * XYMAGF;
                    valdZ = st_.pstSh_->phs.spdZ * XYMAGF;
                    st_.pstSh_->cmn.reSAngle_f = TRUE;
                    st_.pstSh_->cmn.addDir_f = FALSE;//GlobalMoveで波の加算時に敵方向を１回だけ確認するフラグ。
                    //ここでは段階代わっていないのでFALSEとする
                    st_.pstSh_->phs.accelFuncInit_f = FALSE;
                }
                
                break;
            }
        }
        //天井256処理
        if ((st_.pstSh_->phs.posY > 256.0f) && (st_.pstSh_->phs.spdY > 0.0f))
        {
            if (st_.pstSh_->crt.trg.keepCeiling_Time != 0)
            {//天井維持の指示があれば
                f32 xzTotal = fabs(valdX) + fabs(valdZ);//XZ比率の為の合計値を算出
                if (xzTotal == 0.0f) xzTotal = 1.0f;
                f32 xzRatio = fabs(valdY) / xzTotal;//XZ比率
                if (valdX != 0) valdX = (xzRatio * fabs(valdX) + fabs(valdX)) * (valdX / fabs(valdX));//Xの取り分を加えて方向ベクトルを掛ける
                if (valdZ != 0) valdZ = (xzRatio * fabs(valdZ) + fabs(valdZ)) * (valdZ / fabs(valdZ));//Zの取り分を加えて方向ベクトルを掛ける
                valdY = 0.0f;//Yはそのまま
                st_.pstSh_->phs.spdX = valdX / XYMAGF;
                st_.pstSh_->phs.spdY = valdY;
                st_.pstSh_->phs.spdZ = valdZ / XYMAGF;
                st_.pstSh_->phs.posY = 256.0f;
                st_.pstSh_->cmn.maxSpdAx = sca_X;
                if (st_.pstSh_->phs.spdZ > st_.pstSh_->phs.spdX) st_.pstSh_->cmn.maxSpdAx = sca_Z;
            }
            else
            {
                //天井越え
                st_.pstSh_->cmn.ceelingHit_f = TRUE;//天井ヒットフラグオン

                BOOL hit_f = FALSE;//敵ヒットフラグチェック
                if (st_.pstSh_->cmn.hit_c > 0) hit_f = TRUE;

                st_.pstSh_->crt.orb.palab.point = 0;//放物線軌道OFFの代わり


                //通常シュートの場合はシュート無効にして垂直効果
                if (st_.pstSh_->idata.iball.spShot_f == FALSE)
                {
                    st_.pstSh_->cmn.shotFinish_f = TRUE;
                    valdX = 0.0f;
                    valdZ = 0.0f;
                    valdY = (f32)st_.pstSh_->phs.spd * (-1) * XYMAGF;
                    st_.pstSh_->phs.spdX = valdX / XYMAGF;
                    st_.pstSh_->phs.spdY = valdY / XYMAGF;
                    st_.pstSh_->phs.spdZ = valdZ / XYMAGF;
                }
                //変化距離素材　未ヒット&敵を超えてないならSAngle
                else if ((hit_f == FALSE) && (st_.pstSh_->cmn.thruDist_f == FALSE))
                {
                    st_.pstSh_->crt.trg.awayY_Dir = 0;
                    st_.pstSh_->crt.trg.awayZ_Dir = 0;
                    SetAddDtToMainPos();
                    st_.pstSh_->SAngle();
                    st_.pstSh_->cmn.reSAngle_f = TRUE;
                }
                else
                {
                    //既にヒットor敵を超えたなら垂直降下
                    SetAddDtToMainPos();
                    valdX = 0.0f;
                    valdZ = 0.0f;
                    valdY = (f32)st_.pstSh_->phs.spd * (-1) * XYMAGF;
                    st_.pstSh_->cmn.shotFinishFall_f = TRUE;//強制落下フラグ
                    st_.pstSh_->cmn.shotFinishFallSpdY = valdY;//強制落下速度
                    st_.pstSh_->phs.spdX = valdX / XYMAGF;
                    st_.pstSh_->phs.spdY = valdY / XYMAGF;
                    st_.pstSh_->phs.spdZ = valdZ / XYMAGF;
                }

                //                st_.pstSh_->cmn.shotFinish_f = TRUE;
#ifdef __K_DEBUG_SHIAI__
                // 自動シュートモードであれば天井にあたってしまったのをログに残しておく
                //if (kdebug::DebugSystem::GetInstance()->IsAutoShootMode())
                {
                    kdebug::DebugSystem::GetInstance()->AddShootDebugLogError(kdebug::DEBUG_LOG_ERROR::DEBUG_ERROR_HIT_POSY);
                }
#endif	// #ifdef __K_DEBUG_SHIAI__
            }
        }

        //逆進指定点オーバーの処理
        if (st_.pstSh_->crt.trg.backward_Time != 0)
        {
            if ((st_.pstSh_->phs.posX <= CST_POS_XL) || (st_.pstSh_->phs.posX >= (CST_POS_XR - 4)))
                //                if ((st_.pstSh_->phs.posX <= CST_POS_XL) || (st_.pstSh_->phs.posX >= (CST_POS_XR)))
            {//左壁or右壁付近なら止める
                valdX = 0.0f;
                st_.pstSh_->phs.spdX = valdX / XYMAGF;

            }
            if ((st_.pstSh_->phs.posY >= CST_POS_Y) && (st_.pstSh_->crt.chg.height == 0))
            {//逆進中で高さ変化が無くY上限超えたらとめる
                //                valdY = 0.0f;
                //                st_.pstSh_->phs.spdY = valdY / XYMAGF;
            }
        }

        //反れサイド標的方向以外の場合でコートサイドラインに掛かったらZ移動を止める
        if (st_.pstSh_->crt.trg.awayZ_Dir != 0)
        {
            if (
                ((st_.pstSh_->phs.spdZ > 0) && (((f32)st_.pstBa_->Zahyou.Z / XYMAGF) >= (f32)SIDE_LINE_DEEP))
                ||
                ((st_.pstSh_->phs.spdZ < 0) && (((f32)st_.pstBa_->Zahyou.Z / XYMAGF) <= (f32)SIDE_LINE_SHALLOW))
                )
            {
                //141125 バグが出るかもなので一応OFF　DEBUG
                //                valdZ = 0;
            }
        }

        //前のデータを１Fr前のデータとして保存
        if (st_.pstSh_->cmn.stopPstData_f == FALSE)
        {
            st_.pstSh_->cmn.pstSX = st_.pstSh_->cmn.prsSX;
            st_.pstSh_->cmn.pstSY = st_.pstSh_->cmn.prsSY;
            st_.pstSh_->cmn.pstSZ = st_.pstSh_->cmn.prsSZ;
            st_.pstSh_->cmn.pstPX = st_.pstSh_->cmn.prsPX;
            st_.pstSh_->cmn.pstPY = st_.pstSh_->cmn.prsPY;
            st_.pstSh_->cmn.pstPZ = st_.pstSh_->cmn.prsPZ;
            st_.pstSh_->cmn.pstTX = st_.pstSh_->cmn.prsTX;
            st_.pstSh_->cmn.pstTY = st_.pstSh_->cmn.prsTY;
            st_.pstSh_->cmn.pstTZ = st_.pstSh_->cmn.prsTZ;
        }

        //実際の速度に反映
        st_.pstBa_->Zahyou.dX = valdX;
        st_.pstBa_->Zahyou.dY = valdY;
        st_.pstBa_->Zahyou.dZ = valdZ;

        //posデータに速度を追加する
        if (st_.pstSh_->phs.pulse_Stop_f == TRUE) move_f = FALSE;//拍効果で停止中なら動かさない
        if ((st_.pstSh_->crt.orb.waveX.wvtype != 0) && (st_.pstSh_->crt.orb.waveX.peakStop_f == TRUE)) move_f = FALSE;//頂点停止
        if ((st_.pstSh_->crt.orb.waveY.wvtype != 0) && (st_.pstSh_->crt.orb.waveY.peakStop_f == TRUE)) move_f = FALSE;//頂点停止
        if ((st_.pstSh_->crt.orb.waveZ.wvtype != 0) && (st_.pstSh_->crt.orb.waveZ.peakStop_f == TRUE)) move_f = FALSE;//頂点停止
        if (st_.pstSh_->crt.trg.stopForward_Time > 0) move_f = FALSE;//初速０処理
        if (st_.pstSh_->crt.trg.stopAll_Time > 0) move_f = FALSE;//初速０処理
        NormalMove((f32)move_f, valdX, valdY, valdZ);
        
        f32 wvAbsX = CheckWaveAbs(st_.pstSh_->crt.orb.waveX.wvAbs,
            st_.pstSh_->crt.orb.waveX.wvtype, st_.pstSh_->phs.addX);//波絶対値フラグ
        f32 wvAbsY = CheckWaveAbs(st_.pstSh_->crt.orb.waveY.wvAbs,
            st_.pstSh_->crt.orb.waveY.wvtype, st_.pstSh_->phs.addY);//波絶対値フラグ
        f32 wvAbsZ = CheckWaveAbs(st_.pstSh_->crt.orb.waveZ.wvAbs,
            st_.pstSh_->crt.orb.waveZ.wvtype, st_.pstSh_->phs.addZ);//波絶対値フラグ

        //座標XYZに、「加速系までの計算で出された座標」に「三角関数等により主座標からどれだけ離れたか」を足す
        if (st_.pstSh_->cmn.addDir_f == TRUE) SetAddDir(valdX, valdY, valdZ);


        f32 posAddX = (st_.pstSh_->phs.posX + st_.pstSh_->cmn.addDirX * st_.pstSh_->phs.addX * wvAbsX) * XYMAGF;
        f32 posAddZ = (st_.pstSh_->phs.posZ + st_.pstSh_->cmn.addDirZ * st_.pstSh_->phs.addZ * wvAbsZ) * XYMAGF;
        if (pmgSG_->stSh_.cmn.shotNSOF_f == TRUE)
        {
            posAddX = (st_.pstSh_->phs.posX + st_.pstSh_->cmn.addDirX * st_.pstSh_->phs.addZ * wvAbsZ) * XYMAGF;
            posAddZ = (st_.pstSh_->phs.posZ + st_.pstSh_->cmn.addDirZ * st_.pstSh_->phs.addX * wvAbsX) * XYMAGF;
            //            posAddX = (st_.pstSh_->phs.posX + st_.pstSh_->cmn.addDirZ * st_.pstSh_->phs.addZ * wvAbsZ) * XYMAGF;
            //            posAddZ = (st_.pstSh_->phs.posZ + st_.pstSh_->cmn.addDirX * st_.pstSh_->phs.addX * wvAbsX) * XYMAGF;

            //総合段階数が２以上で、現段階が１のとき北側外野の北方向へのシュートと
            //南側外野の南方向へのシュートが壁に当たったら段階移行させる
            if ((st_.pstSh_->cmn.phaseAll > 0) && (st_.pstSh_->cmn.phaseCrt == 0))
            {
                if ((st_.pstSh_->cmn.shotFromNorthOF_f == TRUE) && (posAddZ >= DBCRT_WAL))
                {
                    posAddZ = DBCRT_WAL - 100;
                    st_.pstSh_->cmn.shotNSOFPhaseChange_f = TRUE;
                }
                if ((st_.pstSh_->cmn.shotFromSouthOF_f == TRUE) && (posAddZ <= 0))
                {
                    posAddZ = 100;
                    st_.pstSh_->cmn.shotNSOFPhaseChange_f = TRUE;
                }
            }
            //前進無効中じゃないときＺ波（進行方向波）のせいでＺ壁にぶつかった場合は壁手前とする
            //前進無効中のときや、Z波増幅中、うずまきしゅーと等でＺ壁に当たる可能性があるためここの処理はスルー
            if ((st_.pstSh_->crt.trg.stopForward_Time == 0) && (st_.pstSh_->crt.orb.waveZ.widthChg_Mtd != 4))
            {
                if ((st_.pstSh_->phs.posZ < (DBCRT_WAL / XYMAG)) && (posAddZ >= DBCRT_WAL)) posAddZ = DBCRT_WAL - 100;
                if ((st_.pstSh_->phs.posZ > 0) && (posAddZ <= 0)) posAddZ = 100;
            }
        }
        st_.pstBa_->Zahyou.X = posAddX;
        //強制段階移行が無いなら更新
        if (st_.pstSh_->cmn.shotNSOFPhaseChange_f == FALSE) st_.pstBa_->Zahyou.Z = posAddZ;

#ifdef __K_DEBUG_SHIAI__
        // X,Z座標0ワープは壁ヒットのエラーで検出できないのでここでログを出す
        if (st_.pstBa_->Zahyou.X <= 0)
        {
            kdebug::DebugSystem::GetInstance()->AddShootDebugLogError(kdebug::DEBUG_LOG_ERROR::DEBUG_ERROR_POS_X_ZERO);
        }
        if (st_.pstBa_->Zahyou.Z <= 0)
        {
            kdebug::DebugSystem::GetInstance()->AddShootDebugLogError(kdebug::DEBUG_LOG_ERROR::DEBUG_ERROR_POS_Z_ZERO);
        }
#endif	// #ifdef __K_DEBUG_SHIAI__


        if (
            //位置Yが０より高いのに波のせいで0以下になったら座標は０として処理
            (st_.pstSh_->phs.posY > 0)
            &&
            (((st_.pstSh_->phs.posY + st_.pstSh_->phs.addY * wvAbsY) * XYMAGF) <= 0)
            )
        {
            st_.pstBa_->Zahyou.Y = 1;
        }
        else
        {
            //地上はいずり処理　条件：天井未ヒット
            if (st_.pstSh_->cmn.ceelingHit_f == FALSE)
            {
                //地面這いずりフラグがたっていた場合で、posY,spdYが負の場合０とする
                if (
                    (st_.pstSh_->phs.landAfter_f == TRUE)
                    ||
                    (st_.pstSh_->cmn.shotEff_f[sef_changeGround_f] == TRUE)
                    ||
                    //徐々に変化中に地面に着いたら下降しない
                    ((groundKeep_f == TRUE) && (st_.pstSh_->cmn.timePhase_c < CHANGE_LOOSE_TIME + 1))
                    ||
                    //それ縦敵無視上昇も下降しない
                    ((st_.pstSh_->crt.trg.awayY_Dir >= 4) && (st_.pstSh_->phs.spdY > 0))
                    )
                {
                    BOOL groundNow_f = FALSE;
                    if (st_.pstSh_->phs.posY <= 0) st_.pstSh_->phs.posY = 0, groundNow_f = TRUE;
                    if ((st_.pstSh_->phs.posY <= 0) && (st_.pstSh_->phs.spdY <= 0))
                    {
                        f32 spd = st_.pstSh_->phs.spd;
                        const f32 DIV_X = 1.5f;
                        const f32 DIV_Z = 1.75f;
                        st_.pstSh_->phs.spdY = 0;
                        if ((st_.pstSh_->phs.spdX == 0.0f) && (st_.pstSh_->phs.spdZ == 0.0f))
                        {//完全に座標が一致した場合、投げ手のポジションで進む方向を決める
                            f32 spdX = 0.0f;
                            f32 spdZ = 0.0f;
                            f32 dirX = 1.0f;//X方向；コートと、内野外野で変わる
                            //２Ｐなら逆に
                            if (pmgSG_->stBa_.PichTNo == 1) dirX = -1.0f;

                            switch (pmgSG_->stBa_.PichPNo)
                            {
                            case 4: spdZ = spd * (-1.0f); break;//奥⇒手前
                            case 5: spdZ = spd; break;//手前⇒奥
                            case 6: spdX = spd * dirX * (-1.0f); break;//対面外野⇒内野（１Ｐサイドなら負、２Ｐサイドなら負の打消しで正）
                            default: spdX = spd * dirX; break;//内野⇒対面外野（１Ｐサイドなら正、２Ｐサイドなら負）
                            }

                            st_.pstSh_->phs.spdX = spdX / DIV_X;
                            st_.pstSh_->phs.spdZ = spdZ / DIV_Z;

                        }
                        else
                        {//｜Ｘ｜+｜Ｚ｜！＝0　ならＹを抜いた速度に変換する
                            f32 spdX_a = fabs(st_.pstSh_->phs.spdX);
                            f32 spdY_a = fabs(st_.pstSh_->phs.spdY);
                            f32 spdZ_a = fabs(st_.pstSh_->phs.spdZ);
                            f32 dirX = 0;
                            f32 dirZ = 0;
                            if ((spdX_a + spdZ_a) < 1.0f)
                            {
                            if (st_.pstSh_->phs.spdX != 0.0f) dirX = st_.pstSh_->phs.spdX / spdX_a;
                            if (st_.pstSh_->phs.spdZ != 0.0f) dirZ = st_.pstSh_->phs.spdZ / spdZ_a;
                            f32 divSpd = spd / (spdX_a + spdZ_a);

                            if (spdX_a < spdZ_a)
                            {
                                st_.pstSh_->phs.spdX = divSpd * spdX_a * dirX / DIV_Z;
                                st_.pstSh_->phs.spdZ = divSpd * spdZ_a * dirZ / DIV_Z;
                            }
                            else
                            {
                                st_.pstSh_->phs.spdX = divSpd * spdX_a * dirX / DIV_X;
                                st_.pstSh_->phs.spdZ = divSpd * spdZ_a * dirZ / DIV_X;
                            }
                            }
                        }
                        groundNow_f = TRUE;

                    }
                    if (groundNow_f == TRUE) st_.pstSh_->cmn.shotEff_f[sef_changeGround_f] = TRUE;
                    else st_.pstSh_->cmn.shotEff_f[sef_changeGround_f] = FALSE;
                }
                BOOL afaf = (st_.pstSh_->cmn.shotEff_f[sef_changeGround_f]);

                if (//はいずり中で浮かび上がったらもう次回以降着地してもはいずりはしない
                    (st_.pstSh_->phs.landAfter_f == TRUE)
                    &&
                    (st_.pstSh_->cmn.shotEff_f[sef_changeGround_f] == TRUE)
                    )
                {
                    if ((st_.pstSh_->phs.landAfterOnly_f == TRUE))
                    {
                        st_.pstSh_->cmn.shotEff_f[sef_changeGround_f] = FALSE;//地上変化フラグON
                        st_.pstSh_->phs.landAfter_f = FALSE;
                    }
                }



            }
            else
            {
                //天井ヒットしたら地面はいずりはやめる
                st_.pstSh_->phs.landAfter_f = FALSE;
            }
            if ((st_.pstSh_->phs.spdY < 0) && (st_.pstSh_->phs.landAfter_f == FALSE)) st_.pstSh_->cmn.shotEff_f[sef_changeGround_f] = FALSE;
            if (st_.pstSh_->crt.orb.waveY.wvtype == 0) st_.pstSh_->phs.addY = 0.0f;
            st_.pstSh_->cmn.addDirY = 1.0f;//Yだけは振動加算常に正でいいのかも

            //強制段階移行がなければ更新
            if (st_.pstSh_->cmn.shotNSOFPhaseChange_f == FALSE)
            {
                st_.pstBa_->Zahyou.Y = (st_.pstSh_->phs.posY + st_.pstSh_->cmn.addDirY * st_.pstSh_->phs.addY * wvAbsY) * XYMAGF;
            }
        }

        //更新おｋなら更新
        if (st_.pstSh_->cmn.stopPstData_f == FALSE)
        {
            st_.pstSh_->cmn.prsSX = st_.pstBa_->Zahyou.dX;
            st_.pstSh_->cmn.prsSY = st_.pstBa_->Zahyou.dY;
            st_.pstSh_->cmn.prsSZ = st_.pstBa_->Zahyou.dZ;
            st_.pstSh_->cmn.prsPX = st_.pstBa_->Zahyou.X;
            st_.pstSh_->cmn.prsPY = st_.pstBa_->Zahyou.Y;
            st_.pstSh_->cmn.prsPZ = st_.pstBa_->Zahyou.Z;
            st_.pstSh_->cmn.prsTX = st_.pstSh_->phs.tagposX * XYMAG;
            st_.pstSh_->cmn.prsTY = st_.pstSh_->phs.tagposY * XYMAG;
            st_.pstSh_->cmn.prsTZ = st_.pstSh_->phs.tagposZ * XYMAG;
        }
        //座標０のときは今後保存用データを更新しない用フラグを立てる
        if (((st_.pstBa_->Zahyou.X <= 0))
            ||
            ((st_.pstBa_->Zahyou.Z <= 0))
            ) st_.pstSh_->cmn.stopPstData_f = TRUE;

        //やまだ分裂用、本体ボール位置
        st_.pstSh_->cmn.realBallPX = st_.pstBa_->Zahyou.X;
        st_.pstSh_->cmn.realBallPY = st_.pstBa_->Zahyou.Y;
        st_.pstSh_->cmn.realBallPZ = st_.pstBa_->Zahyou.Z;

#ifdef __K_DEBUG_SHIAI__
        // おおやまさん依頼のデバッグログ
        //吐き出しの条件は、
        if ((st_.pstBa_->Zahyou.X <= 0) || (st_.pstBa_->Zahyou.Z <= 0))
        {
            s32 data[15];	// ベタ持ち
            //1Fr前データ（pst）
            //球速度
            data[0] = st_.pstSh_->cmn.pstSX;
            data[1] = st_.pstSh_->cmn.pstSY;
            data[2] = st_.pstSh_->cmn.pstSZ;
            //球位置
            data[3] = st_.pstSh_->cmn.pstPX;
            data[4] = st_.pstSh_->cmn.pstPY;
            data[5] = st_.pstSh_->cmn.pstPZ;
            //敵位置
            data[6] = st_.pstSh_->cmn.pstTX;
            data[7] = st_.pstSh_->cmn.pstTY;
            data[8] = st_.pstSh_->cmn.pstTZ;

            //現在データ（prs）
            //球速度
            data[9] = st_.pstSh_->cmn.prsSX;
            data[10] = st_.pstSh_->cmn.prsSY;
            data[11] = st_.pstSh_->cmn.prsSZ;
            //球位置
            data[12] = st_.pstSh_->cmn.prsPX;
            data[13] = st_.pstSh_->cmn.prsPY;
            data[14] = st_.pstSh_->cmn.prsPZ;

            //kdebug::DebugSystem::GetInstance()->AddShootDebugLogBallParam(&data[0]);
        }
#endif	// #ifdef __K_DEBUG_SHIAI__

    }

    //変化素材毎の処理
    void TBaShot::ChangeStepNumAtTime()//変化素材：時間
    {
        //現段階時間カウントが変化素材：時間×４を超えたら次段階へ
        if (st_.pstSh_->cmn.timePhase_c >= (st_.pstSh_->crt.chg.time)) AddPhaseNum();
    }

    void TBaShot::ChangeStepNumAtHeight()//変化素材：高さ
    {
        //変化高さ…変化を開始するY値。基本この値を超えると条件を満たすが
        //負の場合はこの値を下回ると条件を満たしたことになる。
        //なので一旦正の値を入れる。
        s32 height = abs(st_.pstSh_->crt.chg.height);
        //変化高さが負のとき用のフラグ
        BOOL lower_f = FALSE;
        if (st_.pstSh_->crt.chg.height < 0) lower_f = TRUE;

        //×現在のYの速度（方向）で仕分け
        //○変化高さが負のときは変化高さを下回ったとき（＝以下）に変化する
        if (lower_f == TRUE)
        {
            //現在の位置Yが変化素材：高さより低くなったら次段階へ
            if (st_.pstSh_->phs.posY <= height) AddPhaseNum();
        }
        else
        {
            //上昇中⇒現在の位置Yが変化素材：高さより高くなったら次段階へ
            if (st_.pstSh_->phs.posY >= height) AddPhaseNum();
        }
    }

    void TBaShot::ChangeStepNumAtPosDir()//変化素材：位置 固定位置
    {

        f32 valXObj = st_.pstSh_->crt.trg.posX_Dot;//標的X位置
        //※センターラインで対称になるように加工する必要有り
        f32 valYObj = st_.pstSh_->crt.trg.posY_Dot;//標的Y位置
        f32 valZObj = st_.pstSh_->crt.trg.posZ_Dot;//標的Z位置

        //標的素材がXZ位置指定の場合は加工
        if (st_.pstSh_->crt.trg.posXZ_Point != 0)
        {
            //※XZはのちに加工する
            valXObj = st_.pstSh_->crt.trg.posXZ_Point;//標的X位置
            valZObj = st_.pstSh_->crt.trg.posXZ_Point;//標的Z位置
            valYObj = 64;//ジャンプ頂点の値で固定。定数があればそれを。
        }

        //指定３次元座標全てを超えていたら次段階移行処理
        if (
            (CheckBeyondObjPos(st_.pstSh_->phs.spdX, st_.pstSh_->phs.posX, valXObj, valXObj) == TRUE)
            &&
            (CheckBeyondObjPos(st_.pstSh_->phs.spdY, st_.pstSh_->phs.posY, valYObj, valYObj) == TRUE)
            &&
            (CheckBeyondObjPos(st_.pstSh_->phs.spdZ, st_.pstSh_->phs.posZ, valZObj, valZObj) == TRUE)
            )
        {
            AddPhaseNum();//次段階移行処理
        }
    }

    void TBaShot::ChangeStepNumAtPosObj()//変化素材：位置 指定物位置
    {
        f32 valDir = 0;//代替変数：速度方向
        f32 valPos = 0;//代替変数：位置
        f32 valObjP = 0;//代替変数：対象値正
        f32 valObjN = 0;//代替変数：対象値負

        //Zシュート中なら色々切り替える用フラグを
        BOOL shootZ_f = FALSE;
        if (st_.pstSh_->cmn.shotNSOFZ_f == TRUE) shootZ_f = TRUE;

        //ボール位置が敵位置orコートライン位置で段階移行
        switch ((s32)st_.pstSh_->crt.chg.pos)
        {
            //コート位置で変化するタイプ
        case 1://センターライン上(X方向)
            valDir = st_.pstSh_->phs.spdX;//方向
            valPos = (f32)st_.pstBa_->Zahyou.X / 100.0f;//ボール位置
            valObjP = CENTER_LINE;//センターライン
            valObjN = CENTER_LINE;//センターライン
            break;
        case 2://コートエンド(X方向)
            valDir = st_.pstSh_->phs.spdX;//方向
            valPos = st_.pstBa_->Zahyou.X;//ボール位置
            valObjP = DBCRT_CL + (DBCRT_SL + ((st_.pstBa_->Zahyou.Z - DBCRT_FL) * 10 / DBCRT_SLR));//速度＋⇒右コートエンド
            valObjN = DBCRT_CL - (DBCRT_SL + ((st_.pstBa_->Zahyou.Z - DBCRT_FL) * 10 / DBCRT_SLR));//速度－⇒左コートエンド
            break;
        case 3://サイドライン（Z方向）
            valDir = st_.pstSh_->phs.spdZ;//方向
            //		  valPos = st_.pstSh_->phs.posZ;//ボール位置
            valPos = (f32)st_.pstBa_->Zahyou.Z / 100.0f;//ボール位置
            valObjN = SIDE_LINE_SHALLOW + 2.0f;//速度＋⇒手前サイドライン
            valObjP = SIDE_LINE_DEEP - 2.0f;//速度－⇒奥サイドライン
            if (shootZ_f)//Zシュートによる変更
            {
                valDir = st_.pstSh_->phs.spdX;//方向
                valPos = (f32)st_.pstBa_->Zahyou.X / 100.0f;//ボール位置
                //左側チームの２・３外野
                valObjP = (f32)(DBCRT_W - DBCRT_SL - ((st_.pstBa_->Zahyou.Z - DBCRT_FL) * 10 / DBCRT_SLR)) / (f32)XYMAG;//速度＋⇒右コートエンド
                valObjN = CENTER_LINE;//センターライン
                if (st_.pstSh_->idata.side == 1)
                {//右側チームの２・３外野
                    valObjP = CENTER_LINE;//センターライン
                    valObjN = (f32)((DBCRT_SL + ((st_.pstBa_->Zahyou.Z - DBCRT_FL) * 10 / DBCRT_SLR))) / (f32)XYMAG;//左コートエンド
                }
            }
            break;
            //敵位置で変化するタイプ
        case 5://敵X同一
            valDir = st_.pstSh_->phs.spdX;//方向
            valPos = (f32)st_.pstBa_->Zahyou.X / 100.0f;//ボール位置
            valObjP = st_.pstSh_->phs.tagposX;//標的X位置
            valObjN = st_.pstSh_->phs.tagposX;//標的X位置
            break;
        case 6://敵Y同一
            valDir = st_.pstSh_->phs.spdX;//方向
            valPos = (f32)st_.pstBa_->Zahyou.X / 100.0f;//ボール位置
            valObjP = st_.pstSh_->phs.tagposY;//標的Y位置
            valObjN = st_.pstSh_->phs.tagposY;//標的Y位置
            break;
        case 7://敵Z同一
            valDir = st_.pstSh_->phs.spdZ;//方向
            valPos = (f32)st_.pstBa_->Zahyou.Z / 100.0f;//ボール位置
            valObjP = st_.pstSh_->phs.tagposZ;//標的Z位置
            valObjN = st_.pstSh_->phs.tagposZ;//標的Z位置
            if (shootZ_f)//Zシュートによる変更
            {
                valDir = st_.pstSh_->phs.spdX;//方向
                valPos = (f32)st_.pstBa_->Zahyou.X / 100.0f;//ボール位置
                valObjP = st_.pstSh_->phs.tagposX;//標的Z位置
                valObjN = st_.pstSh_->phs.tagposX;//標的Z位置
            }
            break;
        default:
            break;
        }
        //指定物座標を超えていたら次段階移行処理
        if (CheckBeyondObjPos(valDir, valPos, valObjN, valObjP) == TRUE) AddPhaseNum();
    }

    //指定物座標を超えたかのチェック(方向，位置，最小位置，最大位置)
    BOOL TBaShot::CheckBeyondObjPos(f32 dir, f32 pos, f32 objN, f32 objP)
    {
        //方向チェック
        if (dir < 0)
        {
            //現在の位置が対象物位置より小さくなったらTRUE
            if (pos <= objN)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
        else
        {
            //現在の位置が対象物位置より大きくなったらTRUE
            if (pos >= objP)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
    }


    void TBaShot::ChangeStepNumAtDist()//変化素材：距離
    {
        //素材データは１（遠）～８（近）、
        //changeDistでは、８ - 素材データとし、８（遠）～１（近）とする
        f32 changeDist = 8.0f - (f32)st_.pstSh_->crt.chg.dist; //素材データ変換

        //基準距離をボール開始位置～現在の敵位置とする
        f32 distAll = fabs(st_.pstSh_->phs.tagposX - st_.pstSh_->idata.iball.posX);
        //現在距離をボール開始位置～ボール現在位置とする
        f32 distNow = fabs(st_.pstSh_->phs.posX - st_.pstSh_->idata.iball.posX);
        //標的超えて進んだ距離上限
        f32 beyondDist = 32.0f;
        f32 ballSpd = st_.pstSh_->phs.spdX;
        f32 tagPos = st_.pstSh_->phs.tagposX;
        s32 realZahyou = st_.pstBa_->Zahyou.X;

        //ZシュートのときはそれぞれZ方向データを入れる
        if (st_.pstSh_->idata.initZShot_f == TRUE)
        {
            distAll = fabs(st_.pstSh_->phs.tagposZ - st_.pstSh_->idata.iball.posZ);
            distNow = fabs(st_.pstSh_->phs.posZ - st_.pstSh_->idata.iball.posZ);
            beyondDist = 4.0f;
            ballSpd = st_.pstSh_->phs.spdZ;
            tagPos = st_.pstSh_->phs.tagposZ;
            realZahyou = st_.pstBa_->Zahyou.Z;
        }

        //現在の距離が変化距離より狭まったら（標的に接近したら）段階を変える
        if (st_.pstSh_->crt.chg.dist < 9)
        {
            //基準距離と現在距離を見て段階移行を確認
            if (distNow >= ((f32)st_.pstSh_->crt.chg.dist * (distAll / 8.0f))) AddPhaseNum();

        }
        //標的座標を超えたら変化
        if (st_.pstSh_->crt.chg.dist >= 9)
        {
            //crt.chg.dist = 9 なら標的座標を超えた時点で変化、10なら標的を超えてBEYOND_DIST分進んだら変化
            f32 changeDist = beyondDist * ((f32)st_.pstSh_->crt.chg.dist - 9.0f);
            if (
                ((ballSpd >= 0) && ((s32)(tagPos + changeDist) <= ((s32)realZahyou / 100.0f)))
                ||
                ((ballSpd < 0) && ((s32)(tagPos - changeDist) >= ((s32)realZahyou / 100.0f)))
                )
            {
                //敵越えフラグオン
                st_.pstSh_->cmn.thruDist_f = TRUE;
                AddPhaseNum();
            }
        }
        //もし距離で段階移行フラグが立ってない場合で敵にヒットしたら強制的に段階移行させる
        if (st_.pstSh_->cmn.phaseChange_f == FALSE) ChangeStepNumAtHit();
    }

    //この素材だけ現段階よりも前に飛ぶ処理も入る
    void TBaShot::ChangeStepNumAtLoop()//変化素材：ループ
    {
        //ひとまずループカウント進める
        st_.pstSh_->cmn.loop_c = st_.pstSh_->cmn.loop_c + 1;

        //ループ回数がループ素材のループ最大回数を超えたら次段階へ
        if (st_.pstSh_->cmn.loop_c > st_.pstSh_->crt.chg.loopMax)
        {
            AddPhaseNum(); //次段階処理
            st_.pstSh_->cmn.loop_c = 0; //念のためループカウント初期化
        }
        else
        {
            //段階をループ素材指定の前段階へ
            st_.pstSh_->cmn.phaseCrt = st_.pstSh_->crt.chg.loopTime;
            st_.pstSh_->cmn.phaseChange_f = TRUE;//段階変わったフラグオン
        }

    }

    //変化素材：波回数
    void TBaShot::ChangeStepNumAtWave(s32 waveTimes_c, s32 waveTimes_Max)
    {
        st_.pstSh_->cmn.addTriData_f = FALSE;
        if (waveTimes_c > waveTimes_Max)
        {
            st_.pstSh_->cmn.addTriData_f = TRUE;
            AddPhaseNum(); //次段階処理
        }
    }


    void TBaShot::ChangeStepNumAtHit()//変化素材：ヒット
    {
        //もろは効果→標的チェンジフラグを一旦オフ
        //        st_.pstSh_->cmn.wEdgeTgChange_f = FALSE;
        if (st_.pstSh_->cmn.hit_f == TRUE)
        {
            //ヒット発動のもろは効果→標的チェンジフラグオン
            //            if (st_.pstSh_->idata.eff.wEdge != 0) st_.pstSh_->cmn.wEdgeTgChange_f = TRUE;
            AddPhaseNum(); //次段階処理
        }
    }

    void TBaShot::ChangeStepNumAtGround()//変化素材：地上
    {
        if (st_.pstSh_->phs.posY <= 0)
        {
            st_.pstSh_->phs.posY = 0;
            st_.pstSh_->phs.spdY = 0;
            st_.pstSh_->phs.landAfter_f = TRUE;

//            st_.pstSh_->cmn.shotEff_f[sef_changeGround_f] = TRUE;

            AddPhaseNum(); //次段階処理
        }
    }

    void TBaShot::ChangeStepNumAtMultiAnglr()//変化素材：多角終
    {
        if (st_.pstSh_->cmn.mltAngle_f == FALSE)
        {//最初の多角処理の場合は多角処理の終了をチェック
            //変化素材の中の多角処理時間カウント・角数カウントと
            //標的素材の中の多角処理時間・角数の両方を満たしたら次段階へ
            if (
                //変化素材多角処理時間カウントが標的素材多角処理時間を超え
                (st_.pstSh_->crt.chg.multiAngle_Num_c >= st_.pstSh_->crt.trg.multiAngle_Num)
                &&
                //変化素材多角処理角数カウントが標的素材多角処理角数を超えたら
                (st_.pstSh_->crt.chg.multiAngle_Time_c >= st_.pstSh_->crt.trg.multiAngle_Time)
                )
            {
                AddPhaseNum(); //次段階処理
                st_.pstSh_->cmn.mltAngle_f = TRUE;//次に多角処理を見つけた場合は即次段階に移行するためフラグを立てる
            }
        }//２回目以降、多角処理を見つけたら即次段階へ
        else AddPhaseNum(); //次段階処理
    }

    void TBaShot::ChangeStepNumAtPassOrbit()//変化素材：パス挙動終
    {
        //放物線下降中 ＆ 高さジャンプ頂点（仮で６４）以下なら次段階へ
        if ((st_.pstSh_->phs.spdY < 0) && (st_.pstSh_->phs.posY <= 64)) AddPhaseNum(); //次段階処理
    }

    //次段階へ進める共通処理
    void TBaShot::AddPhaseNum()
    {
        //現段階に1を足す
        st_.pstSh_->cmn.phaseCrt = st_.pstSh_->cmn.phaseCrt + 1;//次段階へ
        st_.pstSh_->cmn.phaseChange_f = TRUE;//段階変わったフラグオン
    }

    //変化素材毎に分ける
    void TBaShot::CheckStepChange()
    {
        //crt内の変化素材のデータを見て0以外ならその変化素材は「生き」なので処理を進める
        //その先の関数で段階移行のタイミングを見極める
        if (st_.pstSh_->crt.chg.time != 0) ChangeStepNumAtTime();//変化素材：時間
        if (st_.pstSh_->crt.chg.height != 0) ChangeStepNumAtHeight();//変化素材：高さ
        if (st_.pstSh_->crt.chg.dist != 0) ChangeStepNumAtDist();//変化素材：距離
        if (st_.pstSh_->crt.chg.loopTime != 0) ChangeStepNumAtLoop();//変化素材：ループ

        //変化素材：位置
        if (st_.pstSh_->crt.chg.pos == 4)
        {
            ChangeStepNumAtPosDir();//変化素材：位置 指定位置
        }
        else
            if (st_.pstSh_->crt.chg.pos != 0)
            {
                ChangeStepNumAtPosObj();//変化素材：位置 指定物位置
            }

        //フラグ系変化素材
        if (st_.pstSh_->crt.chg.hit != 0) ChangeStepNumAtHit();//変化素材：ヒット
        st_.pstSh_->cmn.hit_f = FALSE;//ヒットの有無問わずチェック後フラグをオフにする
        if (st_.pstSh_->crt.chg.ground != 0) ChangeStepNumAtGround();//変化素材：地上
        if (st_.pstSh_->crt.chg.multiAngleFin != 0) ChangeStepNumAtMultiAnglr();//変化素材：多角終
        if (st_.pstSh_->crt.chg.passOrbitFin != 0) ChangeStepNumAtPassOrbit();//変化素材：パス挙動終


        //変化素材：波回数の場合の処理
        //以下の変数を取り出し、ChangeStepNumAtWave()でチェックを行う
        s32 waveTimes_c = 0; //変化素材：波回数カウント
        s32 waveTimes_Max = 0; //変化素材：波回数

        if (st_.pstSh_->crt.chg.waveXTimes != 0) //変化素材：波X回数
        {
            //象限数に周回数×４を足し合わせたものを波回数とする
            waveTimes_c = st_.pstSh_->crt.orb.waveX.angleRtTms * 4 + st_.pstSh_->crt.orb.waveX.anglePhase;
            waveTimes_Max = st_.pstSh_->crt.chg.waveXTimes;
        }
        if (st_.pstSh_->crt.chg.waveYTimes != 0) //変化素材：波Y回数
        {
            //象限数に周回数×４を足し合わせたものを波回数とする
            waveTimes_c = st_.pstSh_->crt.orb.waveY.angleRtTms * 4 + st_.pstSh_->crt.orb.waveY.anglePhase;
            waveTimes_Max = st_.pstSh_->crt.chg.waveYTimes;
        }
        if (st_.pstSh_->crt.chg.waveZTimes != 0) //変化素材：波Z回数
        {
            //象限数に周回数×４を足し合わせたものを波回数とする
            waveTimes_c = st_.pstSh_->crt.orb.waveZ.angleRtTms * 4 + st_.pstSh_->crt.orb.waveZ.anglePhase;
            waveTimes_Max = st_.pstSh_->crt.chg.waveZTimes;
        }
        //waveTimes_Maxが正当な値に書き変わっていたら段階変更チェックを行う
        if (waveTimes_Max != 0) ChangeStepNumAtWave(waveTimes_c, waveTimes_Max);

        //２・３番外野によるシュートで初段階壁にぶつかったら強制的に次段階へ
        if (st_.pstSh_->cmn.shotNSOFPhaseChange_f == TRUE)
        {
            st_.pstSh_->cmn.shotNSOFPhaseChange_f = FALSE;
            AddPhaseNum();
        }

    }

    //毎Fr更新データ
    void TBaShot::SetMomentData()
    {
        //ボール座標
        //st_.pstSh_->phs.posX = (pmgSG_->stBa_.Zahyou.X / XYMAG);
        //st_.pstSh_->phs.posY = (pmgSG_->stBa_.Zahyou.Y / XYMAG);
        //st_.pstSh_->phs.posZ = (pmgSG_->stBa_.Zahyou.Z / XYMAG);
        //st_.pstSh_->phs.posX = (st_.pstSh_->phs.realX / (f32)XYMAG);
        //st_.pstSh_->phs.posY = (st_.pstSh_->phs.realY / (f32)XYMAG);
        //st_.pstSh_->phs.posZ = (st_.pstSh_->phs.realZ / (f32)XYMAG);
        //標的座標
        st_.pstSh_->phs.tagID = pmgSG_->stBa_.ShTgPNo;
        TZahyouRec* tagzahyou = &(st_.pmgTm_[pmgSG_->stBa_.ShTgTNo]->st_.pmgMyCh_[pmgSG_->stBa_.ShTgPNo]->st_.pstMyCh_->Zahyou);
        st_.pstSh_->phs.tagposX = (tagzahyou->X / XYMAG);
        st_.pstSh_->phs.tagposY = (tagzahyou->Y / XYMAG);
        st_.pstSh_->phs.tagposZ = (tagzahyou->Z / XYMAG);
        //標的状態
        TStChMotion* tagmotion = &(st_.pmgTm_[pmgSG_->stBa_.ShTgTNo]->st_.pmgMyCh_[pmgSG_->stBa_.ShTgPNo]->st_.pstMyCh_->Motion);
        //標的状態フラグの初期化
        st_.pstSh_->cmn.targetY_f = FALSE;
        //標的ダウン中はYも照準合わせる
        if (tagmotion->IsMFlags(dbmfDn)) st_.pstSh_->cmn.targetY_f = TRUE;
        //標的吹っ飛び中はYも照準合わせる
        if ((tagmotion->IsMFlags(dbmfAr)) && (tagmotion->IsMFlags(dbmfDam)))
        {
            st_.pstSh_->cmn.targetY_f = TRUE;
        }

        //標的ダウン中,しゃがみ中はY座標点に照準合わせる（おいかけ効果中）
        if (tagmotion->IsMFlags(dbmfDn) || tagmotion->IsMFlags(dbmfDg)) st_.pstSh_->cmn.targetYZero_f = TRUE;
        else st_.pstSh_->cmn.targetYZero_f = FALSE;

        //標的天使中はおいかけ効果をスルーする
        if (tagmotion->IsMFlags(dbmfANG)) st_.pstSh_->cmn.targetDead_f = TRUE;
        else st_.pstSh_->cmn.targetDead_f = FALSE;

        //ボール～標的距離
        st_.pstSh_->phs.distX = (pmgSG_->stSh_.idata.iball.tagposX - pmgSG_->stSh_.phs.posX);
        st_.pstSh_->phs.distY = (pmgSG_->stSh_.idata.iball.tagposY - pmgSG_->stSh_.phs.posY);
        st_.pstSh_->phs.distZ = (pmgSG_->stSh_.idata.iball.tagposZ - pmgSG_->stSh_.phs.posZ);
    }


    //PhaseData更新
    void TBaShot::SetPhaseData()
    {
        //段階時間カウントの初期化
        st_.pstSh_->cmn.timePhase_c = 0;

        //標的チェンジ系素材は次段階発動のため効果が上書きされる前にRsvに保存する
        st_.pstSh_->cmn.wEdgeTgChangeRsv_f = FALSE;
        st_.pstSh_->cmn.gncdTgChangeRsv_f = FALSE;
        if (st_.pstSh_->cmn.wEdgeTgChange_f == TRUE) st_.pstSh_->cmn.wEdgeTgChangeRsv_f = TRUE;
        if (st_.pstSh_->cmn.gncdTgChange_f == TRUE) st_.pstSh_->cmn.gncdTgChangeRsv_f = TRUE;

        //現段階のsdataからElmに移動
        st_.pstSh_->RemoveSdataToElm(&pmgEO_->mgDt_.dtElem_);

        //前段階データとして保存
        //↓変化方法を前段階情報として保存…次段階以降直後処理用
        st_.pstSh_->cmn.changeMtdPr = st_.pstSh_->phs.changeMtd;
        st_.pstSh_->phs.changeMtd = 0;
        st_.pstSh_->cmn.spdPrX = st_.pstSh_->phs.spdX;//前段階速度X
        st_.pstSh_->cmn.spdPrY = st_.pstSh_->phs.spdY;//前段階速度Y
        st_.pstSh_->cmn.spdPrZ = st_.pstSh_->phs.spdZ;//前段階速度Z

        //三角関数加算値をposに加算する処理
        if ((st_.pstSh_->phs.addX != 0.0f)
            || (st_.pstSh_->phs.addY != 0.0f)
            || (st_.pstSh_->phs.addZ != 0.0f)
            )SetAddDtToMainPos();

        //標的素材Elm⇒Crt
        st_.pstSh_->RemoveElmToCrt();
        st_.pstSh_->cmn.phaseChange_f = FALSE;

        //効果素材の発動をチェック　とともに速度可変素材による速度変化
        st_.pstSh_->CheckEffectElmFlag();
        SetEffSpd();

        //諸刃効果による標的チェンジ
        if (st_.pstSh_->cmn.wEdgeTgChangeRsv_f == TRUE) SetTargetByWEdgeEff();

        //敵標的チェンジ効果による標的チェンジ
        if (st_.pstSh_->cmn.gncdTgChangeRsv_f == TRUE) SetTargetByGenocideEff();

        //標的素材が敵から離れる ＆ 変化素材がヒットの場合、強制的に標的に向かわせる
        if (st_.pstSh_->crt.chg.hit != 0)
        {
            if ((st_.pstSh_->crt.trg.awayY_Dir != 0) || (st_.pstSh_->crt.trg.awayZ_Dir != 0))
            {
                st_.pstSh_->crt.trg.awayY_Dir = 0;
                st_.pstSh_->crt.trg.awayY_Mtd = 0;
                st_.pstSh_->crt.trg.awayZ_Dir = 0;
                st_.pstSh_->crt.trg.awayZ_Mtd = 0;
                st_.pstSh_->phs.changeMtdFromTarget = 0;
            }
        }

        //方向決め
        st_.pstSh_->SAngle();

        //段階形状のデータを保存
        if (
            (st_.pstSh_->idata.shp[st_.pstSh_->cmn.spdtShpPhs].id != 0)
            &&
            (st_.pstSh_->idata.shp[st_.pstSh_->cmn.spdtShpPhs].lv != 0)
            )
        {
            if (st_.pstSh_->cmn.spdtShpid != 99) st_.pstSh_->cmn.spdtGrhPrId = st_.pstSh_->cmn.spdtShpid;
            //            st_.pstSh_->cmn.spdtGrhPrLv = st_.pstSh_->cmn.spdtS;
        }
        //形決め
        if (st_.pstSh_->cmn.efEl.phantom_f == FALSE) st_.pstSh_->cmn.spdtShpPhs = st_.pstSh_->cmn.spdtShpPhs + 1;
        st_.pstSh_->InitShapeIdLv();//ID,Lvの初期化
        st_.pstSh_->InitShapeData(&pmgEO_->mgDt_.dtBaAni_, &pmgEO_->mgDt_.dtElem_);
        st_.pstSh_->cmn.poolSeNoForChanging_f = FALSE;
        st_.pstSh_->cmn.poolSeNoForChanging = seHShot;
        //必殺シュートの音をデフォで入れておく
        //０段階目に鳴る効果音があればその番号の音IDをセットする
        if ((st_.pstSh_->cmn.efEl.sePhase.phase_f == TRUE) && (st_.pstSh_->cmn.efEl.sePhase.incld_f == TRUE))
        {
            enSEID seID = seHShot;
            s32 zero_f = 1;
            if (pmgSG_->stSh_.idata.eff.sePhase_Num[0] == 100) zero_f = 0;

            s32 sePhaseNum = st_.pstSh_->cmn.efEl.sePhase.times_c;
            seID = (enSEID)((st_.pstSh_->GetSENum(st_.pstSh_->idata.eff.sePhase_Num[sePhaseNum] + 1)) * zero_f);

            //段階チェンジですぐに変わるなら音もすぐに鳴らす
            if (st_.pstSh_->cmn.changeMtdPr == 0)
            {
                if (st_.pstSh_->idata.eff.sePhase_Num[sePhaseNum] != 0) st_.pmgBa_->baCommon_.HomingSESet(seID);
            }
            else
            {//段階チェンジですぐに変わらないなら音データを保存
                if (st_.pstSh_->idata.eff.sePhase_Num[sePhaseNum] != 0) st_.pstSh_->cmn.poolSeNoForChanging = seID;
                st_.pstSh_->cmn.poolSeNoForChanging_f = TRUE;
            }
        }
        else//多段階＆最終段階突入時ならファイナルSEを鳴らす。設定がないなら必殺音を鳴らす 
            if ((st_.pstSh_->cmn.phaseAll > 0) && (st_.pstSh_->cmn.phaseAll == st_.pstSh_->cmn.phaseCrt))
            {
                enSEID seID = seHShot;
                if (st_.pstSh_->cmn.efEl.finalSE_f == TRUE)
                {//ファイナルSEあればそれを鳴らす。
                    s32 zero_f = 1;
                    if (pmgSG_->stSh_.idata.eff.finalSE == 100) zero_f = 0;
                    seID = (enSEID)((pmgSG_->stSh_.idata.eff.finalSE + 1) * zero_f);
                }

                //今鳴らすかどうかのフラグ
                BOOL nowSE_f = TRUE;

                if (st_.pstSh_->cmn.changeMtdPr != 0)
                {//段階チェンジですぐに変わらないなら音データを保存
                    st_.pstSh_->cmn.poolSeNoForChanging = seID;
                    st_.pstSh_->cmn.poolSeNoForChanging_f = TRUE;
                    nowSE_f = FALSE;
                }

                //ファイナルSE無くて、２段階しかないシュートは鳴らさない
                if ((st_.pstSh_->cmn.efEl.finalSE_f == FALSE) && (st_.pstSh_->cmn.phaseAll == 1))
                {
                    st_.pstSh_->cmn.poolSeNoForChanging_f = FALSE;
                    nowSE_f = FALSE;
                }

                //鳴らしておｋなら鳴らす
                if (nowSE_f == TRUE) st_.pmgBa_->baCommon_.HomingSESet(seID);

            }
    }

    void TBaShot::CheckAccFlag()
    {
        //	  if ((st_.pstSh_->idata.eff.accel_Lv != 0) && (st_.pstSh_->idata.eff.accel_Phase == st_.pstSh_->cmn.phaseCrt))

    }

    //波回数段階変化で三角関数加算値をposに加算する処理
    void TBaShot::SetAddDtToMainPos()
    {
        if (st_.pstSh_->cmn.efEl.waveKeep_f == FALSE)
        {
        //方向X
        //        f32 dirX = 1.0f;
        //        if (st_.pstSh_->phs.spdX != 0.0f)
        //        {//X速度０でない
        //            dirX = st_.pstSh_->phs.spdX / fabs(st_.pstSh_->phs.spdX);
        //      }
        //        else
        //        {//X速度０のときは初期向きを
        //            dirX = st_.pstSh_->idata.iball.initDirX;
        //        }
        //        //方向Y
        //        f32 dirY = 1.0f;
        //        if (st_.pstSh_->phs.spdY != 0.0f) dirY = st_.pstSh_->phs.spdY / fabs(st_.pstSh_->phs.spdY);
        //        //方向Z
        //        f32 dirZ = 1.0f;
        //        if (st_.pstSh_->phs.spdZ != 0.0f) dirZ = st_.pstSh_->phs.spdZ / fabs(st_.pstSh_->phs.spdZ);
        //        st_.pstSh_->phs.posX = st_.pstSh_->phs.posX + st_.pstSh_->phs.addX * dirX;//波加算値を追加
        //        st_.pstSh_->phs.posY = st_.pstSh_->phs.posY + st_.pstSh_->phs.addY * dirY;//波加算値を追加
        //        st_.pstSh_->phs.posZ = st_.pstSh_->phs.posZ + st_.pstSh_->phs.addZ * dirZ;//波加算値を追加
        st_.pstSh_->phs.posX = st_.pstBa_->Zahyou.X / XYMAG;
        st_.pstSh_->phs.posY = st_.pstBa_->Zahyou.Y / XYMAG;
        st_.pstSh_->phs.posZ = st_.pstBa_->Zahyou.Z / XYMAG;
        st_.pstSh_->phs.addX = 0;//波加算値をクリア
        st_.pstSh_->phs.addY = 0;//波加算値をクリア
        st_.pstSh_->phs.addZ = 0;//波加算値をクリア
        st_.pstSh_->cmn.addTriData_f = FALSE;
        st_.pstSh_->cmn.reTriInitData_f = TRUE;
        st_.pstSh_->phs.spdWaveInit_f = FALSE;
        }
    }


    //諸刃効果による標的切り替え
    void TBaShot::SetTargetByWEdgeEff()
    {
        //標的チェンジ・・・チームIDとキャラIDを術者に変更
        pmgSG_->stBa_.ShTgTNo = pmgSG_->stBa_.PichTNo;
        pmgSG_->stBa_.ShTgPNo = pmgSG_->stBa_.PichPNo;

        //諸刃２回目以降で起こりうるシューターの体力０以下
        if (st_.pmgTm_[pmgSG_->stBa_.ShTgTNo]->st_.pmgMyCh_[pmgSG_->stBa_.ShTgPNo]->st_.pstMyCh_->HP <= 0)
        {
            s32 aliveNo = 0;
            for (s32 i = 0; i < 4; ++i)
            {//敵内野４人分のデータ
                //生死の確認　生存者に飛ばす
                if (st_.pmgTm_[pmgSG_->stBa_.ShTgTNo]->st_.pmgMyCh_[i]->st_.pstMyCh_->HP > 0)
                {
                    pmgSG_->stBa_.ShTgPNo = i;
                    aliveNo = aliveNo + 1;
                }

            }
            //生存者０なら３番外野に飛ばす
            if (aliveNo == 0) pmgSG_->stBa_.ShTgPNo = 5;
        }
        //ピッチャー所属・敵所属を転換　０→１　１→０
        pmgSG_->stBa_.PichTNo = (pmgSG_->stBa_.PichTNo - 1) * (-1);
        pmgSG_->stBa_.EnemTNo = (pmgSG_->stBa_.EnemTNo - 1) * (-1);
        SetMomentData();//再度情報更新
        st_.pstSh_->cmn.wEdgeTgChangeRsv_f = FALSE;//この処理に入ってこないようにする
        st_.pstSh_->phs.spd = st_.pstSh_->phs.spd + 1.5f;//諸刃で速度追加
    }

    //敵標的効果による標的切り替え
    void TBaShot::SetTargetByGenocideEff()
    {
        for (s32 i = 0; i < 4; ++i)
        {//敵内野４人分のデータ
            //生死の確認
            if (st_.pmgTm_[pmgSG_->stBa_.ShTgTNo]->st_.pmgMyCh_[i]->st_.pstMyCh_->HP <= 0)
            {
                st_.pstSh_->cmn.comboGncdDeath[i] = TRUE;
                st_.pstSh_->cmn.comboGncdDist[i] = 10000;
            } 
            else
            {
            //絶対値距離の確認
            TZahyouRec* tagzahyou = &(st_.pmgTm_[pmgSG_->stBa_.ShTgTNo]->st_.pmgMyCh_[i]->st_.pstMyCh_->Zahyou);
            s32 distX = tagzahyou->X - st_.pstBa_->Zahyou.X;
            s32 distY = tagzahyou->Y - st_.pstBa_->Zahyou.Y;
            s32 distZ = tagzahyou->Z - st_.pstBa_->Zahyou.Z;
            st_.pstSh_->cmn.comboGncdDist[i] = (abs(distX) + abs(distY) + abs(distZ)) / XYMAG;
            }
        }
        //生きてるID抽出
        s32 aliveNo = 0;
        s32 aliveID[4];
        s32 aliveDist[4];
        BOOL aliveHit[4];
        for (s32 i = 0; i < 4; ++i)
        {
            aliveDist[i] = 10000;//適当な大きい数字＝死亡フラグの代わり
        }


        //iは０～３のキャラID
        //aliveNoは人数
        //くたばっているヤツはaliveNoのケツに
        for (s32 i = 0; i < 4; ++i)
        {
            //生死の確認
            if (st_.pstSh_->cmn.comboGncdDeath[i] == FALSE)
            {
                aliveID[aliveNo] = i;
                aliveDist[aliveNo] = st_.pstSh_->cmn.comboGncdDist[i];
                aliveHit[aliveNo] = st_.pstSh_->cmn.comboGncdHit[i];
                aliveNo = aliveNo + 1;
            }
        }
        //並び替え 距離の近い順に
        //生きていているヤツの中で距離が近い順
        for (s32 k = 0; k < 3; ++k)
        {
            for (s32 i = k + 1; i < 4; ++i)
            {
                if (aliveDist[i] < aliveDist[k])
                {
                    f32 rsvID = aliveID[k];
                    f32 rsvDist = aliveDist[k];
                    f32 rsvHit = aliveHit[k];
                    aliveID[k] = aliveID[i];
                    aliveDist[k] = aliveDist[i];
                    aliveHit[k] = aliveHit[i];
                    aliveID[i] = rsvID;
                    aliveDist[i] = rsvDist;
                    aliveHit[i] = rsvHit;
                }
            }
        }

        //近い順からヒットの確認。未ヒットキャラがいればそいつを狙う
        s32 tgID = 0;
        BOOL tgDecide_f = FALSE;
        for (s32 i = 0; i < 4; ++i)
        {
            //近い順からヒット確認　距離10000＝距離更新していない＝おまえは既に死んでいる
            if ((aliveDist[i] < 10000) && (aliveHit[i] == FALSE)) tgID = aliveID[i], tgDecide_f = TRUE;
        }
        //全員ヒットしていたら身近で生きているやつをターゲットにする
        if (tgDecide_f == FALSE) tgID = aliveID[0];
        if (aliveNo == 0) tgID = 5;//もし全員くたばっていたら３番外野に飛ばす
        pmgSG_->stBa_.ShTgPNo = tgID;

        SetMomentData();//再度情報更新
        st_.pstSh_->cmn.gncdTgChangeRsv_f = FALSE;//この処理に入ってこないようにする
        st_.pstSh_->phs.spd = st_.pstSh_->phs.spd * 0.80f;//速度追加
    }



    //SpdLvから正式なSpdをセットする
    void TBaShot::SetEffSpd()
    {
        //基礎速度レベルと速度可変効果の速度、更に一時速度可変効果のLvを足す
        s32 spdLv = pmgSG_->stSh_.phs.spdLv + pmgSG_->stSh_.phs.spdMomLv;
        //Lv０のときのデフォ速度
        st_.pstSh_->phs.spd = st_.pstSh_->cmn.defSpd;
        //Lv０以外のとき
        if (spdLv != 0)
        {
            //とりあえず速度加算データ
            enRankLevel rankSwitch = rkBallSpdInc;

            //加算or減算
            f32 spdSign = spdLv / abs(spdLv);

            //速度減算データ→読み込みデータは速度マイナス用、Lvはー１を掛けて正にする
            if (spdLv < 0)  rankSwitch = rkBallSpdDec, spdLv = spdLv * (-1);
            //
            f32 rkSpd = pmgEO_->mgDt_.dtRank_.GetRankDtLevel(rankSwitch, spdLv);
            //速度加算or減算があったらデフォ速度を更新する
            pmgSG_->stSh_.phs.spd = pmgSG_->stSh_.phs.spd + (spdSign * rkSpd) / XYMAG;
            if (pmgSG_->stSh_.phs.spd <= 0.5f) pmgSG_->stSh_.phs.spd = 0.5f;

        }
    }

    //波の加算方向算出
    void TBaShot::SetAddDir(f32 valdX, f32 valdY, f32 valdZ)
    {
        st_.pstSh_->cmn.addDirX = GetAddDir(st_.pstSh_->phs.posX, st_.pstSh_->phs.tagposX, valdX);
        st_.pstSh_->cmn.addDirY = GetAddDir(st_.pstSh_->phs.posY, st_.pstSh_->phs.tagposY, valdY);
        st_.pstSh_->cmn.addDirZ = GetAddDir(st_.pstSh_->phs.posZ, st_.pstSh_->phs.tagposZ, valdZ);
        //Z方向に関して-cosの場合広いほうに加算していく
        if (
            (st_.pstSh_->crt.orb.waveZ.wvtype == 4)
            ||
            (st_.pstSh_->crt.orb.waveZ.wvtype == 8)
            )
        {
            if (st_.pstSh_->cmn.maxSpdAx == sca_X)
            {
                if (st_.pstSh_->cmn.courtDirRevZ_f == TRUE) st_.pstSh_->cmn.addDirZ = (-1.0f);
                else st_.pstSh_->cmn.addDirZ = (1.0f);
            }
            if (st_.pstSh_->cmn.maxSpdAx == sca_Z)
            {
                if (st_.pstSh_->cmn.courtDirRevX_f == TRUE) st_.pstSh_->cmn.addDirX = (-1.0f);
                else st_.pstSh_->cmn.addDirX = (1.0f);
            }
        }

        st_.pstSh_->cmn.addDir_f = FALSE;
    }
    f32 TBaShot::GetAddDir(f32 pos, f32 tagpos, f32 vald)
    {
        f32 res = 1.0f;
        if (vald != 0)
        {
            res = vald / fabs(vald);
        }
        else
            if (tagpos != pos)
            {
                f32 dist = tagpos - pos;
                res = dist / fabs(dist);
            }
        return res;
    }


    //段階変化後音を鳴らす処理
    void TBaShot::CheckDelaySE()
    {
        //プールしてあるSEナンバー
        enSEID seID = st_.pstSh_->cmn.poolSeNoForChanging;
        //それの使用フラグが立っていたら発声
        if (st_.pstSh_->cmn.poolSeNoForChanging_f == TRUE) st_.pmgBa_->baCommon_.HomingSESet(seID);
        //一度発声したら使わないようフラグをたたむ
        st_.pstSh_->cmn.poolSeNoForChanging_f = FALSE;
    }


    //二次関数・三角関数の初期化
    void TBaShot::InitAccTriData()
    {
        pbaShot_Acc_->AccInit();
        pbaShot_Tri_->TriInit();
        st_.pstSh_->idata.initAccTri_f = TRUE;
    }

    //更新
    void TBaShot::UpDate()
    {
        ////速度・角度処理
        ////段階移行時のみ処理
        //SAngle();//主軌道算出【主軌道計算】

        //初回のみ二次関数・三角関数データの初期化
        if (st_.pstSh_->idata.initAccTri_f == FALSE) InitAccTriData();

        //毎Frデータ更新
        SetMomentData();

        //段階変化チェック
        //天井ヒットフラグオンのときは段階移行しない
        if (st_.pstSh_->cmn.ceelingHit_f == FALSE) CheckStepChange();

        //段階変わったら　sdata⇒Elm⇒Crt　の手続きを
        if (st_.pstSh_->cmn.phaseChange_f == TRUE) SetPhaseData();

        //毎Fr処理⇒GlobalMove()で処理phs.pulse_Stop_f
        //    NormalMove();//現在の座標に速度を追加【主軌道計算】

        //拍効果があってその段階に来たら処理を行う
        if ((st_.pstSh_->cmn.efEl.pulse.incld_f == TRUE) && (st_.pstSh_->cmn.efEl.pulse.phase_f == TRUE))
        {
            st_.pstSh_->SetStopOrMoveByPulseEff();//処理関数へ
            st_.pstSh_->InitShapeIdLv();//ID,Lvの初期化
        }
        else
        {//該当しないときは時は拍素材からのデータとしては動く命令を
            st_.pstSh_->phs.pulse_Stop_f = FALSE;
            st_.pstSh_->phs.pulseShape_Init_f = FALSE;
        }

        //加速系関数処理【２次関数計算】拍効果停止中は全面ストップ
        if ((st_.pstSh_->phs.accelFunc_f == TRUE) && (st_.pstSh_->phs.pulse_Stop_f == FALSE)) pbaShot_Acc_->UpDate();

        //位置処理　仮想位置⇒表示位置
        pbaShot_Tri_->UpDate();//三角関数系処理【三角関数計算】

        //トレース効果があればGlobalMove前に方向（速度）を再計算
        if (
            (st_.pstSh_->cmn.efEl.trace.incld_f == TRUE)
            && (st_.pstSh_->cmn.efEl.trace.phase_f == TRUE)
            && ((st_.pstSh_->crt.trg.awayY_Dir >= -1) || (st_.pstSh_->crt.trg.awayY_Dir <= 1))
            && (st_.pstSh_->crt.trg.awayZ_Dir == 0)
            && (st_.pstSh_->crt.trg.backward_Time <= 0)//逆進外
            )
        {
            //それたて・それサイドナシ、逆進ナシならトレース
            st_.pstSh_->ResetSpdByTraceEff();
        }
        else st_.pstSh_->idata.eff.trace_init_f = FALSE;

        //多角処理
        if ((st_.pstSh_->cmn.mltAngle_f == FALSE) && (st_.pstSh_->crt.chg.multiAngleFin != 0)) st_.pstSh_->SetSpdByMltAngle();

        //最終的にst_.pstBa_->Zahyou.dXYZに値を入れる
        GlobalMove();

        //敵越えフラグオン
        if (CheckBeyondTarget() == TRUE) st_.pstSh_->cmn.thruDist_f = TRUE;

        //カウント系
        st_.pstSh_->cmn.timeTotal_c = st_.pstSh_->cmn.timeTotal_c + 1; //トータル時間カウント
        st_.pstSh_->cmn.timePhase_c = st_.pstSh_->cmn.timePhase_c + 1; //段階時間カウント
        st_.pstSh_->cmn.spdtFrame_c = st_.pstSh_->cmn.spdtFrame_c + 1; //形状用カウント
        st_.pstSh_->CheckShapeData(&pmgEO_->mgDt_.dtBaAni_);//Fr増加に伴う画像替えチェック
        st_.pstSh_->CheckShapeMode(&pmgEO_->mgDt_.dtBaAni_, &pmgEO_->mgDt_.dtElem_);//例外アニメのチェック

        //例外画像必要なら初期化
        if (st_.pstSh_->cmn.spdtShpModeChange_f == TRUE) st_.pstSh_->InitShapeData(&pmgEO_->mgDt_.dtBaAni_, &pmgEO_->mgDt_.dtElem_);
        st_.pstSh_->SetShapeData(&pmgEO_->mgDt_.dtBaAni_);//画像のセット

        //現在の画像などのデータを保存
        if (st_.pstSh_->idata.sprtDt[ssl_shadow].lv != 0)
        {
            st_.pstSh_->SetPastData(st_.pstBa_->Zahyou.X / XYMAG, st_.pstBa_->Zahyou.Y / XYMAG, st_.pstBa_->Zahyou.Z / XYMAG);
        }

        //ぶんれつ系処理
        if ((st_.pstSh_->idata.sprt_f == TRUE))
        {
            st_.pstSh_->SetSeparateBall();
            //追跡分裂が０の時は実際のボールを消す
            if (st_.pstSh_->idata.sprtDt[ssl_shadow].lv == 0) st_.pstSh_->phs.realBlHide_f = TRUE;
            for (s32 i = 0; i < SPARATE_MAX; ++i)
            {//全ての分裂球加算値と元の球の座標を合わせたものを分裂球座標とする
                if (st_.pstSh_->phs.sprtDt[i].shadow_f == FALSE)//追跡分裂以外はここで実座標計算
                {
                    st_.pstSh_->phs.sprtDt[i].posX = st_.pstBa_->Zahyou.X + st_.pstSh_->phs.sprtDt[i].addX * (f32)XYMAG;
                    st_.pstSh_->phs.sprtDt[i].posY = st_.pstBa_->Zahyou.Y + st_.pstSh_->phs.sprtDt[i].addY * (f32)XYMAG;
                    st_.pstSh_->phs.sprtDt[i].posZ = st_.pstBa_->Zahyou.Z + st_.pstSh_->phs.sprtDt[i].addZ * (f32)XYMAG;
                }
            }
        }
        //ぶんれつオフ処理
        if (
            ((st_.pstSh_->cmn.efEl.sprtOff.phase_f == TRUE) && (st_.pstSh_->cmn.efEl.sprtOff.incld_f == TRUE))
            ||
            ((st_.pstSh_->cmn.efEl.phantom_f == TRUE) && (st_.pstSh_->idata.sprt_f == TRUE))
            ||
            (st_.pstSh_->cmn.efEl.phantomEnd_f == TRUE)
            )
        {
            st_.pstSh_->phs.realBlHide_f = FALSE;//実際のボールを出す
            //分裂ボールは全消し
            for (s32 i = 0; i < SPARATE_MAX; ++i)
            {//全ての分裂球加算値と元の球の座標を合わせたものを分裂球座標とする
                st_.pstSh_->phs.sprtDt[i].disp_f = FALSE;
                st_.pstSh_->phs.sprtDt[i].vnsBal_f = TRUE;//玉消し
                st_.pstSh_->phs.sprtDt[i].vnsSdw_f = TRUE;//影消し

            }
            if (st_.pstSh_->cmn.efEl.phantom_f == TRUE) st_.pstSh_->idata.sprt_f = FALSE;
            st_.pstSh_->cmn.efEl.phantomEnd_f = FALSE;
        }

        //山田の処理
        if (st_.pstSh_->cmn.efEl.phantom_f == TRUE) st_.pstSh_->SetPhantomData(&pmgEO_->mgDt_.dtBaAni_, &pmgEO_->mgDt_.dtElem_);


        mid::midSetDbgMsg(0, pmgSG_->stBa_.Zahyou.X, "Zahyou.X");
        mid::midSetDbgMsg(3, pmgSG_->stSh_.idata.shotId, "ShotNum");

    }



}
