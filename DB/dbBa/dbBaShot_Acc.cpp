#include "dbBaShot_Acc.h"
#include "dbGame.h"

namespace db
{
    //固定重力・固定速度（共通放物線処理）
    const f32 CO_GRAV = 0.25f;
    const f32 CO_SPDX = 0.70f;
    const f32 CO_GR_MIN_HEIGHT = 48.0f;//共通重力使用時の最低頂点高度

    //放物線計算
    const f32 MAX_HEIGHT = 72.0f;//放物線頂点最高値
    const f32 MIN_HEIGHT = 48.0f;//放物線頂点最低値
    const f32 MIN_TIME = 48.0f;//放物線最低時間
    const f32 PALAB_NEG_LOW_LEVEL = 4.0f;//負の放物線最低高度

    //コンストラクタ
    TBaShot_Acc::TBaShot_Acc(TGame* pGame, TBaCommon* pCommon)
        : TBaHaveCommon(pGame, pCommon)
    {
    }

    TBaShot_Acc::~TBaShot_Acc()
    {
    }

    //x等速放物線軌道初期化
    void TBaShot_Acc::InitParabolicOrbit()
    {
        //加速度等の初期化
        SetUpdatedData();

        f32 distY = 14 - st_.pstSh_->accdt.shotPosY;//ボール初期位置 ～ 標的位置(地上スレスレ高さ)

        //放物線等速度軸を仮にQとしておく。基本的にはx軸になるので予めxの想定。
        f32 distQ = st_.pstSh_->accdt.targPosX - st_.pstSh_->accdt.shotPosX;//標的～術者距離x
        f32 distR;
        if (st_.pstSh_->accdt.targPosZ - st_.pstSh_->accdt.shotPosZ != 0)
        {
            distR = st_.pstSh_->phs.spdRtXZ * fabs(distQ) * (st_.pstSh_->accdt.targPosZ - st_.pstSh_->accdt.shotPosZ) / fabs(st_.pstSh_->accdt.targPosZ - st_.pstSh_->accdt.shotPosZ);//標的～術者距離z
        }
        else
        {
            distR = 0;//標的～術者距離z
        }

        if (st_.pstSh_->accdt.mainSpeedX == 0) //主軌道X＝０の時はyzで放物線軌道を。
        {
            distQ = st_.pstSh_->accdt.targPosZ - st_.pstSh_->accdt.shotPosZ;//標的～術者距離
            distR = st_.pstSh_->accdt.targPosX - st_.pstSh_->accdt.shotPosX;//標的～術者距離
        }

        //放物線等速度軸Qの速度を現在の速度spdにしてしまう
        f32 accelSpeedQ = (st_.pstSh_->accdt.spd / 2.5f) * distQ / (fabs(distQ));//等速度軸Qの速度
        if (fabs((st_.pstSh_->accdt.spd / 2.5f)) < 0.75f) accelSpeedQ = 0.75f * distQ / (fabs(distQ));//1.0を下回らないように

        //距離データが入っていたらその距離を到達点とする（distQから単位方向ベクトルを取得して掛ける）
        if (st_.pstSh_->crt.orb.palab.dist > 0)
        {
            distQ = (f32)st_.pstSh_->crt.orb.palab.dist * distQ / fabs(distQ);
            accelSpeedQ = CO_SPDX * distQ / (fabs(distQ));//速度0.75f
            st_.pstSh_->accdt.paraGrav = CO_GRAV;//重力
            st_.pstSh_->accdt.accelSpeedY = sqrt(2.0f * st_.pstSh_->accdt.paraGrav * (72.0f - st_.pstSh_->accdt.shotPosY));//初速
        }
        else
            //距離データが入っていない場合は重力を算出
        {
            //開始地点S→放物線頂点A→開始地点同高さB→標的地点E
            //SAの時間を算出して、初速と終速を算出、そこから重力を決定する
            f32 gravLv = st_.pstSh_->crt.orb.palab.grav;
            f32 peakPosY = fabs(st_.pstSh_->accdt.targPosX - st_.pstSh_->accdt.shotPosX) * ((1.5f + (0.5f * gravLv) / 8.0f));//頂点座標Yを距離XとLvから算出
            f32 peakMax = 72.0f + ((72.0f * gravLv) / 8.0f);//頂点上限をLvから算出
            if (peakPosY > peakMax) peakPosY = peakMax;//上限144
            if (((14.0f - st_.pstSh_->accdt.shotPosY) != 0) && (peakPosY < 72)) peakPosY = 72;//下限72（ジャンプでの発動が予想されるのでジャンプ上限を超えたあたりの値）

            f32 fallTiming = 8.0f;//落下ポイント：基本８で標的
            if (st_.pstSh_->crt.orb.palab.point != 0) fallTiming = 8.0f / (f32)st_.pstSh_->crt.orb.palab.point;
            f32 timeAll = fabs(distQ / accelSpeedQ) * fallTiming;//放物線全体の時間＝距離÷速度
            f32 yAS = peakPosY - st_.pstSh_->accdt.shotPosY;//頂点～開始点の差
            f32 yES = 14.0f - st_.pstSh_->accdt.shotPosY;//標的高さ（14）～開始点の差

            if (yES != 0)
            {//始点と終点で高さが違う場合

                //根号計算
                f32 root = sqrt(yAS*yAS - yAS * yES);
                //SA時間
                f32 timePeakP = timeAll * (yAS + root) / yES;
                f32 timePeakN = timeAll * (yAS - root) / yES;
                f32 timePeak = timePeakN;

                //初速
                st_.pstSh_->accdt.accelSpeedY = 2.0f * yAS / timePeak;
                //終速
                f32 endSpeed = (2.0f * yES / timeAll) - st_.pstSh_->accdt.accelSpeedY;
                //重力
                st_.pstSh_->accdt.paraGrav = -1.0f * (((endSpeed * endSpeed) - (st_.pstSh_->accdt.accelSpeedY * st_.pstSh_->accdt.accelSpeedY)) / (2.0f * yES));
            }
            else
            {//始点と終点で高さが同じ場合

                //初速
                st_.pstSh_->accdt.accelSpeedY = 4.0f * yAS / timeAll;
                //重力
                st_.pstSh_->accdt.paraGrav = 8.0f * yAS / (timeAll * timeAll);

            }
        }

        //放物線無関係軸Rは放物線等速度軸Qが標的に到達する時間から速度を算出…XZ同時に到着すればOK
        f32 accelSpeedR = distR * fabs(accelSpeedQ / distQ);//符号はdistRに依存させる

        ////等速度軸Qの速度と放物線にかかわらない軸Rの速度をx、z軸速度に割り当てる
        if (st_.pstSh_->accdt.mainSpeedX != 0)
        {
            //x速度０以外の時は等速度軸をxとする
            st_.pstSh_->accdt.accelSpeedX = accelSpeedQ;//放物線等速度軸をx
            st_.pstSh_->accdt.accelSpeedZ = accelSpeedR;//放物線無関係軸をz
        }
        else
        {
            //x速度０の時は等速度軸をzとする
            st_.pstSh_->accdt.accelSpeedX = accelSpeedR;//放物線無関係軸をx
            st_.pstSh_->accdt.accelSpeedZ = accelSpeedQ;//放物線等速度軸をz
        }

    }

    //x等速放物線軌道初期化
    void TBaShot_Acc::InitParabolicOrbit2()
    {
        //加速度等の初期化
        SetUpdatedData();

        f32 distY = 14 - st_.pstSh_->accdt.shotPosY;//ボール初期位置 ～ 標的位置(地上スレスレ高さ)

        //距離データが入っていたらその距離を到達点とする
        if (st_.pstSh_->crt.orb.palab.dist > 0)
        {
            f32 spdX = fabs(st_.pstSh_->phs.spdX);
            f32 spdZ = fabs(st_.pstSh_->phs.spdZ);
            //            f32 distX;
//            if (st_.pstSh_->phs.spdX != 0) distX = (f32)st_.pstSh_->crt.orb.palab.dist * spdX / fabs(spdX);
//            else if (((f32)st_.pstSh_->idata.side + 1.0f) > 1.0f) distX = (f32)st_.pstSh_->crt.orb.palab.dist * (-1.0f);
//            else distX = (f32)st_.pstSh_->crt.orb.palab.dist;

//            st_.pstSh_->accdt.accelSpeedX = CO_SPDX * distX / (fabs(distX));//速度0.75f
//            st_.pstSh_->accdt.paraGrav = CO_GRAV;//重力
//            f32 yDist = 72.0f - st_.pstSh_->accdt.shotPosY;
//            if (yDist <= CO_GR_MIN_HEIGHT) yDist = CO_GR_MIN_HEIGHT;
//            st_.pstSh_->accdt.accelSpeedY = sqrt(2.0f * st_.pstSh_->accdt.paraGrav * yDist);//初速

            //距離
            f32 dist = (f32)st_.pstSh_->crt.orb.palab.dist;
            //高さ 上がる距離StartToPeak
            f32 distStoP = (f32)st_.pstSh_->crt.orb.palab.grav;
            //ジャンプ中は減少
//            if (st_.pstSh_->phs.posY > 14.0f) dist = dist * 0.75f, distStoP = distStoP * 0.75f;

            //頂点～下がる距離PeakToEnd
            f32 distPtoE = distStoP + st_.pstSh_->phs.posY - 14.0f;

            //落下時間
            f32 arrTime;
            if (spdZ >= spdX)
            {
                const f32 DEC_RATIO = 1.0f / 5.0f;
                if (spdZ == 0) arrTime = dist;//(ZXともに０)
                else
                {
                st_.pstSh_->phs.spdZ = (st_.pstSh_->phs.spdZ / spdZ)* st_.pstSh_->phs.spd * DEC_RATIO;
                st_.pstSh_->phs.spdX = 0;
 //               st_.pstSh_->phs.spdX = st_.pstSh_->phs.spdX * DEC_RATIO;
                arrTime = dist / (fabs(st_.pstSh_->phs.spdZ) * 2.0f);
                }
            }
            else
            {
                const f32 DEC_RATIO = 2.0f / 5.0f;
                st_.pstSh_->phs.spdX = (st_.pstSh_->phs.spdX / spdX)* st_.pstSh_->phs.spd * DEC_RATIO;
                st_.pstSh_->phs.spdZ = 0;
//                st_.pstSh_->phs.spdZ = st_.pstSh_->phs.spdZ * DEC_RATIO;
                arrTime = dist / (fabs(st_.pstSh_->phs.spdX));
            }

            //到達時間が最低値未満なら最低値にしてしまう
            f32 MIN_TIME2 = 40.0f;
            if (arrTime < MIN_TIME2) arrTime = MIN_TIME2;

            //重力
            st_.pstSh_->accdt.paraGrav = 2.0f * (distStoP + distPtoE + sqrt(distStoP * distPtoE) * 2.0f) / (arrTime * arrTime);
            //初速
            st_.pstSh_->accdt.accelSpeedY = sqrt(fabs(2.0f * st_.pstSh_->accdt.paraGrav * distStoP));

            st_.pstSh_->accdt.accelSpeedX = st_.pstSh_->phs.spdX;
            st_.pstSh_->accdt.accelSpeedZ = st_.pstSh_->phs.spdZ;

        }
        else
            //距離データが入っていない場合は高さ・時間より重力・初速を算出する
        {
            //***逆放物線フラグ
            BOOL palabNgt_f = FALSE;
            if (st_.pstSh_->crt.orb.palab.point < 0) palabNgt_f = TRUE;

            //***放物直線フラグ
            BOOL palabLine_f = FALSE;
            if (st_.pstSh_->crt.orb.palab.grav < 0) palabLine_f = TRUE;

            //高さ 上がる距離StartToPeak
            f32 distStoP = GetParabHeight();

            //頂点～下がる距離PeakToEnd
            f32 distPtoE = distStoP + st_.pstSh_->phs.posY - 14.0f;

            //***逆放物線フラグ
            if (palabNgt_f == TRUE)
            {
                //高さ 上がる距離StartToPeak
                distStoP = st_.pstSh_->phs.posY - PALAB_NEG_LOW_LEVEL;
                distPtoE = 24.0f - PALAB_NEG_LOW_LEVEL;
                if (st_.pstSh_->phs.posY < PALAB_NEG_LOW_LEVEL)
                {
                    distStoP = st_.pstSh_->phs.posY;
                    distPtoE = 24.0f - st_.pstSh_->phs.posY;
                }

            }

            //時間 + 落下位置
            f32 fallTime = 8.0f - fabs((f32)st_.pstSh_->crt.orb.palab.point);//落下ポイント：基本８で標的
            f32 DIV_DIST = 4.0f;//放物線分解距離↓分割したモノを落下位置番号分足していく
            f32 arrTime = GetParabTime() * ((DIV_DIST + fallTime) / DIV_DIST);

            //重力
            st_.pstSh_->accdt.paraGrav = 2.0f * (distStoP + distPtoE + sqrt(distStoP * distPtoE) * 2.0f) / (arrTime * arrTime);
            //初速
            st_.pstSh_->accdt.accelSpeedY = sqrt(fabs(2.0f * st_.pstSh_->accdt.paraGrav * distStoP));

            //***逆放物線フラグ
            if (palabNgt_f == TRUE)
            {
                st_.pstSh_->accdt.paraGrav = st_.pstSh_->accdt.paraGrav * (-1.0f);
                st_.pstSh_->accdt.accelSpeedY = sqrt(fabs(2.0f * st_.pstSh_->accdt.paraGrav * distStoP)) * (-1.0f);
            }
            //***放物直線フラグ
            if (palabLine_f == TRUE)
            {
                //頂点Y位置→この点を越えたら敵に向かう速度を反映させる
                st_.pstSh_->cmn.palabLinePeakPosY = distStoP + st_.pstSh_->phs.posY;
                if (palabNgt_f == TRUE) st_.pstSh_->cmn.palabLinePeakPosY = st_.pstSh_->phs.posY - distStoP;

                //重力→重力処理しない
                st_.pstSh_->accdt.paraGrav = 0.0f;

                //頂点までの絶対距離
                f32 distY_a = fabs(distStoP);
                if (palabNgt_f == TRUE) distY_a = distStoP;

                //逆放物線フラグが立っていたら負にする処理
                f32 dirY = 1.0f;
                if (palabNgt_f == TRUE) dirY = -1.0f;
                //初速
                st_.pstSh_->accdt.accelSpeedY = dirY * (distY_a + distPtoE) / arrTime;
            }

            //時間が最小値を下回る場合はXZ速度を補正
            if (arrTime <= MIN_TIME)
            {
                f32 minTime = MIN_TIME;
                if (st_.pstSh_->crt.orb.palab.point >= 9)
                {//重力初速を再計算
//                    minTime = arrTime;
                    //重力
                    st_.pstSh_->accdt.paraGrav = 2.0f * (distStoP + distPtoE + sqrt(distStoP * distPtoE) * 2.0f) / (minTime * minTime);
                    //初速
                    st_.pstSh_->accdt.accelSpeedY = sqrt(fabs(2.0f * st_.pstSh_->accdt.paraGrav * distStoP));
                }

                //更新後の速度逓減率
                f32 decPer = (fabs(st_.pstSh_->phs.arrPosX - st_.pstSh_->phs.posX) / minTime) / fabs(st_.pstSh_->phs.spdX);
                if (fabs(st_.pstSh_->phs.spdZ) > fabs(st_.pstSh_->phs.spdX))
                {
                    
                    decPer = (fabs(st_.pstSh_->phs.arrPosZ - st_.pstSh_->phs.posZ) / minTime) / fabs(st_.pstSh_->phs.spdZ);
                }
                if ((fabs(st_.pstSh_->phs.spdZ) == 0) && (fabs(st_.pstSh_->phs.spdX) == 0)) decPer = 0;
                f32 dirX = st_.pstSh_->phs.spdX;
                if (dirX != 0.0f) dirX = dirX / fabs(dirX);
                st_.pstSh_->phs.spdX = dirX * fabs(st_.pstSh_->phs.spdX) * decPer;

                f32 dirZ = st_.pstSh_->phs.spdZ;
                if (dirZ != 0.0f) dirZ = dirZ / fabs(dirZ);
                st_.pstSh_->phs.spdZ = dirZ * fabs(st_.pstSh_->phs.spdZ) * decPer;

//                if (st_.pstSh_->phs.spdX != 0.0f)//X速度０でない場合は処理
//                {
//                    f32 dirX = st_.pstSh_->phs.spdX / fabs(st_.pstSh_->phs.spdX);
//                    st_.pstSh_->phs.spdX = dirX * fabs(st_.pstSh_->phs.arrPosX - st_.pstSh_->phs.posX) / minTime;
//                }
//                if (st_.pstSh_->phs.spdZ != 0.0f)//Z速度０でない場合は処理
//                {
//                    f32 dirZ = st_.pstSh_->phs.spdZ / fabs(st_.pstSh_->phs.spdZ);
//                    st_.pstSh_->phs.spdZ = dirZ * fabs(st_.pstSh_->phs.arrPosZ - st_.pstSh_->phs.posZ) / minTime;
//                }
                if (st_.pstSh_->phs.arrPosAlr_f == TRUE)
                {
                    st_.pstSh_->phs.spdX = st_.pstSh_->phs.spdX * 0.5f;
                    st_.pstSh_->phs.spdZ = st_.pstSh_->phs.spdZ * 0.5f;
                }

            }
            st_.pstSh_->accdt.accelSpeedX = st_.pstSh_->phs.spdX;
            st_.pstSh_->accdt.accelSpeedZ = st_.pstSh_->phs.spdZ;
        }
    }

    //放物線高さを求める
    f32 TBaShot_Acc::GetParabHeight()
    {
        f32 res = MIN_HEIGHT;
        f32 maxHeight = (f32)abs(st_.pstSh_->crt.orb.palab.grav);
        //Xの値がZより大きい場合はXの長さから高さを算出
        if (fabs(st_.pstSh_->phs.spdX) >= fabs(st_.pstSh_->phs.spdZ))
        {
            res = maxHeight * fabs(st_.pstSh_->phs.arrPosX - st_.pstSh_->phs.posX) / 224.0f;
            if (res > maxHeight) res = maxHeight;
        }
        else
        {
            res = maxHeight * fabs(st_.pstSh_->phs.arrPosZ - st_.pstSh_->phs.posZ) / 224.0f;
            if (res > maxHeight) res = maxHeight;
        }

        if ((res < MIN_HEIGHT) && (maxHeight > MIN_HEIGHT)) res = MIN_HEIGHT;
        else if ((res < MIN_HEIGHT) && (maxHeight < MIN_HEIGHT)) res = maxHeight;

        return res;
    }

    //放物線時間を求める
    f32 TBaShot_Acc::GetParabTime()
    {
        f32 res = MIN_TIME;
        s32 arrPosLv = st_.pstSh_->crt.orb.palab.point;
        f32 arrDist = st_.pstSh_->phs.arrPosX - st_.pstSh_->phs.posX;
        f32 arrDist_a = fabs(arrDist);
        f32 arrDir;
        f32 DISTLIMIT_X = 36.0f;
        f32 DISTLIMIT_Z = 24.0f;
//        f32 decPer = 1.0f;
        f32 decPer = (4.0f - ((f32)arrPosLv - 8.0)) / 4.0f;
        f32 spd_a = fabs(st_.pstSh_->phs.spdX);
        f32 arrDistAP;
        st_.pstSh_->phs.arrPosAlr_f = FALSE;

        //Xの値がZより大きい場合はXの長さから高さを算出
        switch (st_.pstSh_->cmn.maxSpdAx)
        {
        case sca_X:
            if (arrPosLv >= 9)
            {
                arrDir = arrDist / arrDist_a;
                arrDistAP = arrDist_a * decPer;
                if (arrDistAP < DISTLIMIT_X) arrDist = DISTLIMIT_X, arrDistAP = DISTLIMIT_X;
                st_.pstSh_->phs.arrPosX = arrDistAP * arrDir + st_.pstSh_->phs.posX;
                if (arrDist_a < DISTLIMIT_X) st_.pstSh_->phs.arrPosAlr_f = TRUE;
            }
            break;
        case sca_Y:
            arrDist = st_.pstSh_->phs.arrPosY - st_.pstSh_->phs.posY;
            arrDist_a = fabs(arrDist);
            if (arrPosLv >= 9)
            {
                arrDir = arrDist / arrDist_a;
                arrDistAP = arrDist_a * decPer;
                if (arrDistAP < DISTLIMIT_Z) arrDist = DISTLIMIT_Z, arrDistAP = DISTLIMIT_Z;
                st_.pstSh_->phs.arrPosY = arrDistAP * arrDir + st_.pstSh_->phs.posY;
                if (st_.pstSh_->phs.spdX >= st_.pstSh_->phs.spdZ)
                {
                    if (fabs(st_.pstSh_->phs.arrPosX - st_.pstSh_->phs.posX) < DISTLIMIT_X) st_.pstSh_->phs.arrPosAlr_f = TRUE;
                }
                if (st_.pstSh_->phs.spdZ >= st_.pstSh_->phs.spdX)
                {
                    if (fabs(st_.pstSh_->phs.arrPosZ - st_.pstSh_->phs.posZ) < DISTLIMIT_Z) st_.pstSh_->phs.arrPosAlr_f = TRUE;
                }
            }
            spd_a = fabs(st_.pstSh_->phs.spdY);
            break;
        case sca_Z:
            arrDist = st_.pstSh_->phs.arrPosZ - st_.pstSh_->phs.posZ;
            arrDist_a = fabs(arrDist);
            if (arrPosLv >= 9)
            {
                arrDir = arrDist / arrDist_a;
                arrDistAP = arrDist_a * decPer;
                if (arrDistAP < DISTLIMIT_Z) arrDist = DISTLIMIT_Z, arrDistAP = DISTLIMIT_Z;
                st_.pstSh_->phs.arrPosZ = arrDistAP * arrDir + st_.pstSh_->phs.posZ;
                if (arrDist_a < DISTLIMIT_Z) st_.pstSh_->phs.arrPosAlr_f = TRUE;
            }
            spd_a = fabs(st_.pstSh_->phs.spdZ);
            break;
        default:
            res = MIN_TIME;
            break;
        }
        res = fabs(arrDist) / spd_a;
        if ((res < MIN_TIME)) res = MIN_TIME;

        return res;
    }

    //加速系放物線時間を求める
    f32 TBaShot_Acc::GetParabAcTime(f32 acc)
    {
        f32 res = MIN_TIME;
        f32 acl;//加速度
        f32 vlcF; //加速の最大、減速の最低
        f32 vlcI = 0.0f; //加速の初速＝０、減速の初速
        f32 dist;
        //Xの値がZより大きい場合はXの長さから高さを算出
        switch (st_.pstSh_->cmn.maxSpdAx)
        {
        case sca_Y:
            acl = st_.pstSh_->phs.accHY;
            vlcF = fabs(st_.pstSh_->phs.accHY) * 100.0f;
            dist = fabs(st_.pstSh_->phs.tagposY - st_.pstSh_->phs.posY);
            break;
        case sca_Z:
            acl = st_.pstSh_->phs.accHZ;
            vlcF = fabs(st_.pstSh_->phs.accHZ) * 100.0f;
            dist = fabs(st_.pstSh_->phs.tagposZ - st_.pstSh_->phs.posZ);
            break;
        default:
            acl = st_.pstSh_->phs.accHX;
            vlcF = fabs(st_.pstSh_->phs.accHX) * 100.0f;
            dist = fabs(st_.pstSh_->phs.tagposX - st_.pstSh_->phs.posX);
            break;
        }

        //t=(v/a) * (VMax / 100(%))
        //加速
        res = fabs(sqrt(2.0f * fabs(acl) * dist) / acl);
//        if (acc < 0) res = fabs(vlcF - sqrt(vlcF * vlcF + 2.0f * acl * dist)) / (fabs(acl));
        //減速
        if (acc < 0)
        {
            res = fabs(sqrt(2.0f * dist * fabs(acl)) / acl);
        }
        if (res < MIN_TIME) res = MIN_TIME;

        return res;
    }



    //加速シュート系初期化
    void TBaShot_Acc::InitXAccelOrbit()
    {
        //加速度等の初期化
        SetUpdatedData();

        //各加速度単位（1/100）に加速レベルを掛ける
        const f32 ACL_CONST = 1.5f;
        if (st_.pstSh_->phs.acc >= 3.0f) st_.pstSh_->phs.acc = 3.0f;
        f32 accLv = st_.pstSh_->phs.acc * ACL_CONST;
        st_.pstSh_->phs.accHX = accLv * st_.pstSh_->phs.spdX / 100.0f;//速度x（保存用）
        st_.pstSh_->phs.accHY = accLv * st_.pstSh_->phs.spdY / 100.0f;//速度y（保存用）
        st_.pstSh_->phs.accHZ = accLv * st_.pstSh_->phs.spdZ / 100.0f;//速度z（保存用）

        //初速を０とする
        st_.pstSh_->accdt.accelSpeedX = 0;
        st_.pstSh_->accdt.accelSpeedY = 0;
        st_.pstSh_->accdt.accelSpeedZ = 0;

        /*
                //加速度等の初期化
                SetUpdatedData();

                //SAngleで分割した速度を保存
                //このst_.pstSh_->phs.spdBfに、TimeCountと加速倍率を掛け合わせて
                //st_.pstSh_->phs.spdに毎時入れていく
                st_.pstSh_->phs.accX = st_.pstSh_->phs.spdX;//速度x（保存用）
                st_.pstSh_->phs.accY = st_.pstSh_->phs.spdY;//速度y（保存用）
                st_.pstSh_->phs.accZ = st_.pstSh_->phs.spdZ;//速度z（保存用）

                st_.pstSh_->accdt.accelSpeedX = 0;
                st_.pstSh_->accdt.accelSpeedY = 0;
                st_.pstSh_->accdt.accelSpeedZ = 0;
                */
    }

    //減速シュート系初期化
    void TBaShot_Acc::InitXDecelOrbit()
    {
        //各加速度単位（1/100）に加速レベルを掛ける
        const f32 DCL_CONST = 0.5f;
        if (st_.pstSh_->phs.acc >= 3.0f) st_.pstSh_->phs.acc = 3.0f;
        f32 dclLv = (st_.pstSh_->phs.acc + 1.0f) * DCL_CONST;
        st_.pstSh_->phs.accHX = dclLv * st_.pstSh_->phs.spdX / 100.0f;//速度x（保存用）
        st_.pstSh_->phs.accHY = dclLv * st_.pstSh_->phs.spdY / 100.0f;//速度y（保存用）
        st_.pstSh_->phs.accHZ = dclLv * st_.pstSh_->phs.spdZ / 100.0f;//速度z（保存用）

        //初速を既定倍する
//        const f32 INIT_SPD_CONST = 1.5f;
//        st_.pstSh_->accdt.accelSpeedX = st_.pstSh_->phs.spdX * INIT_SPD_CONST;
//        st_.pstSh_->accdt.accelSpeedY = st_.pstSh_->phs.spdY * INIT_SPD_CONST;
//        st_.pstSh_->accdt.accelSpeedZ = st_.pstSh_->phs.spdZ * INIT_SPD_CONST;
        st_.pstSh_->accdt.accelSpeedX = st_.pstSh_->phs.spdX;
        st_.pstSh_->accdt.accelSpeedY = st_.pstSh_->phs.spdY;
        st_.pstSh_->accdt.accelSpeedZ = st_.pstSh_->phs.spdZ;

        st_.pstSh_->phs.accX = st_.pstSh_->accdt.accelSpeedX;//速度x（保存用）以降はこの速度に減速度を掛け合わせていく
        st_.pstSh_->phs.accY = st_.pstSh_->accdt.accelSpeedY;//速度y（保存用）
        st_.pstSh_->phs.accZ = st_.pstSh_->accdt.accelSpeedZ;//速度z（保存用）

        /*
                st_.pstSh_->accdt.accelSpeedX = st_.pstSh_->phs.spdX;//速度x
                st_.pstSh_->accdt.accelSpeedY = st_.pstSh_->phs.spdY;//速度y
                st_.pstSh_->accdt.accelSpeedZ = st_.pstSh_->phs.spdZ;//速度z
                st_.pstSh_->phs.accX = st_.pstSh_->accdt.accelSpeedX;//速度x（保存用）以降はこの速度に減速度を掛け合わせていく
                st_.pstSh_->phs.accY = st_.pstSh_->accdt.accelSpeedY;//速度y（保存用）
                st_.pstSh_->phs.accZ = st_.pstSh_->accdt.accelSpeedZ;//速度z（保存用）
                */
    }


    //直落下系初期化…もずの最後など
    void TBaShot_Acc::InitYAccelOrbit()
    {
        //加速度等の初期化
        SetUpdatedData();

        //頂点～下がる距離PeakToEnd
        f32 distPtoE = fabs(st_.pstSh_->phs.tagposY - st_.pstSh_->phs.posY + 14.0f);
        if (distPtoE == 0.0f) distPtoE = 14.0f;

        //時間
        f32 arrTime = GetParabTime();

        //重力
        st_.pstSh_->accdt.paraGrav = 2.0f * distPtoE / (arrTime * arrTime);

        //初速
        st_.pstSh_->accdt.accelSpeedY = 0.0f;

        //反れ縦上方向的無視時の重力と初速
        if (st_.pstSh_->crt.trg.awayY_Dir > 3)
        {
            //到達位置
            st_.pstSh_->phs.arrPosX = st_.pstSh_->phs.tagposX;
            st_.pstSh_->phs.arrPosZ = st_.pstSh_->phs.tagposZ;
            //時間
            arrTime = GetParabTime();
            //重力
            st_.pstSh_->accdt.paraGrav = 2.0f * (st_.pstSh_->phs.spdY * arrTime - distPtoE) / (arrTime * arrTime);
            //初速
            st_.pstSh_->accdt.accelSpeedY = st_.pstSh_->phs.spdY;
        }


        //時間が最小値を下回る場合はXZ速度を補正
        if (arrTime <= MIN_TIME)
        {
            if (st_.pstSh_->phs.spdX != 0.000000000000f)//X速度０でない場合は処理
            {
                f32 dirX = st_.pstSh_->phs.spdX / fabs(st_.pstSh_->phs.spdX);
                st_.pstSh_->phs.spdX = dirX * fabs(st_.pstSh_->phs.arrPosX - st_.pstSh_->phs.posX) / MIN_TIME;
            }
            if (st_.pstSh_->phs.spdZ != 0.000000000000f)//Z速度０でない場合は処理
            {
                f32 dirZ = st_.pstSh_->phs.spdZ / fabs(st_.pstSh_->phs.spdZ);
                st_.pstSh_->phs.spdZ = dirZ * fabs(st_.pstSh_->phs.arrPosZ - st_.pstSh_->phs.posZ) / MIN_TIME;
            }
        }
        st_.pstSh_->accdt.accelSpeedX = st_.pstSh_->phs.spdX;
        st_.pstSh_->accdt.accelSpeedZ = st_.pstSh_->phs.spdZ;

    }


    //直落下系初期化…もずの最後など
    void TBaShot_Acc::InitYXAccelOrbit()
    {
        //加速度等の初期化
        SetUpdatedData();

        //頂点～下がる距離PeakToEnd
        f32 distPtoE = fabs(st_.pstSh_->phs.tagposY - st_.pstSh_->phs.posY + 14.0f);
        if (distPtoE == 0.0f) distPtoE = 14.0f;

        //XZ加速系処理
        //各加速度単位（1/100）に加速レベルを掛ける
        const f32 ACL_CONST = 1.5f;
        if (st_.pstSh_->phs.acc >= 3.0f) st_.pstSh_->phs.acc = 3.0f;
        if (st_.pstSh_->phs.acc <= -3.0f) st_.pstSh_->phs.acc = -3.0f;
        f32 accLv = 1.00f;

        switch ((s32)st_.pstSh_->phs.acc)
        {
            //加速系最高速の倍率
        case 2: accLv = 1.50f; break;
        case 3: accLv = 2.00f; break;
            //減速系初速倍率
        case -1: accLv = 0.75f; break;
        case -2: accLv = 0.875f; break;
        }

        f32 aclSign = st_.pstSh_->phs.acc / fabs(st_.pstSh_->phs.acc);

        st_.pstSh_->phs.accHX = accLv * st_.pstSh_->phs.spdX / 100.0f;//速度x（保存用）
        st_.pstSh_->phs.accHY = accLv * st_.pstSh_->phs.spdY / 100.0f;//速度y（保存用）
        st_.pstSh_->phs.accHZ = accLv * st_.pstSh_->phs.spdZ / 100.0f;//速度z（保存用）

        if (st_.pstSh_->phs.acc < 0)
        {
            f32 distX = st_.pstSh_->phs.tagposX - st_.pstSh_->phs.posX;
            f32 distY = st_.pstSh_->phs.tagposY - st_.pstSh_->phs.posY;
            f32 distZ = st_.pstSh_->phs.tagposZ - st_.pstSh_->phs.posZ;
            f32 spdX = accLv * fabs(st_.pstSh_->phs.spdX);
            f32 spdY = accLv * fabs(st_.pstSh_->phs.spdY);
            f32 spdZ = accLv * fabs(st_.pstSh_->phs.spdZ);

            st_.pstSh_->phs.accHX = 0;
            if (distX != 0.0f) st_.pstSh_->phs.accHX = (-1) * spdX * spdX / fabs(distX * 2.0f);//速度x（保存用）
            st_.pstSh_->phs.accHY = 0;
            if (distY != 0.0f) st_.pstSh_->phs.accHY = (-1) * spdY * spdY / fabs(distY * 2.0f);//速度Y（保存用）
            st_.pstSh_->phs.accHZ = 0;
            if (distZ != 0.0f) st_.pstSh_->phs.accHZ = (-1) * spdZ * spdZ / fabs(distZ * 2.0f);//速度Z（保存用）
        }

        //加速のときは初速を０にするフラグ
        f32 initSpd_f = 1.0f;
        if (st_.pstSh_->phs.acc > 0) initSpd_f = 0.0f;
        st_.pstSh_->accdt.accelSpeedX = accLv * st_.pstSh_->phs.spdX * initSpd_f;
        st_.pstSh_->accdt.accelSpeedZ = accLv * st_.pstSh_->phs.spdZ * initSpd_f;

        f32 arrTime = GetParabAcTime(st_.pstSh_->phs.acc);

        f32 acl;//加速度
        f32 dist;
        //Xの値がZより大きい場合はXの長さから高さを算出
        switch (st_.pstSh_->cmn.maxSpdAx)
        {
        case sca_Y:
            acl = st_.pstSh_->phs.accHY;
            dist = fabs(st_.pstSh_->phs.tagposY - st_.pstSh_->phs.posY);
            break;
        case sca_Z:
            acl = st_.pstSh_->phs.accHZ;
            dist = fabs(st_.pstSh_->phs.tagposZ - st_.pstSh_->phs.posZ);
            break;
        default:
            acl = st_.pstSh_->phs.accHX;
            dist = fabs(st_.pstSh_->phs.tagposX - st_.pstSh_->phs.posX);
            break;
        }

        //重力
//        st_.pstSh_->accdt.paraGrav = 2.0f * distPtoE / (arrTime * arrTime);
        st_.pstSh_->accdt.paraGrav = fabs(distPtoE * acl/ dist);

        //初速
        st_.pstSh_->accdt.accelSpeedY = 0.0f;

        //反れ縦上方向的無視時の重力と初速
        if (st_.pstSh_->crt.trg.awayY_Dir > 3)
        {
            //到達位置
            st_.pstSh_->phs.arrPosX = st_.pstSh_->phs.tagposX;
            st_.pstSh_->phs.arrPosZ = st_.pstSh_->phs.tagposZ;
            //時間
            arrTime = GetParabTime();
            //重力
            st_.pstSh_->accdt.paraGrav = 2.0f * (st_.pstSh_->phs.spdY * arrTime - distPtoE) / (arrTime * arrTime);
            //初速
            st_.pstSh_->accdt.accelSpeedY = st_.pstSh_->phs.spdY;
        }


        //時間が最小値を下回る場合はXZ速度を補正
        if (arrTime <= MIN_TIME)
        {
            if (st_.pstSh_->phs.spdX != 0.000000000000f)//X速度０でない場合は処理
            {
                f32 dirX = st_.pstSh_->phs.spdX / fabs(st_.pstSh_->phs.spdX);
                st_.pstSh_->phs.spdX = dirX * fabs(st_.pstSh_->phs.arrPosX - st_.pstSh_->phs.posX) / MIN_TIME;
            }
            if (st_.pstSh_->phs.spdZ != 0.000000000000f)//Z速度０でない場合は処理
            {
                f32 dirZ = st_.pstSh_->phs.spdZ / fabs(st_.pstSh_->phs.spdZ);
                st_.pstSh_->phs.spdZ = dirZ * fabs(st_.pstSh_->phs.arrPosZ - st_.pstSh_->phs.posZ) / MIN_TIME;
            }
            st_.pstSh_->accdt.accelSpeedX = st_.pstSh_->phs.spdX;
            st_.pstSh_->accdt.accelSpeedZ = st_.pstSh_->phs.spdZ;
        }

    }



    //x加速放物線軌道初期化
    void TBaShot_Acc::InitParabolicXAccelOrbit()
    {
        //加速度等の初期化
        SetUpdatedData();

        f32 distY = 14 - st_.pstSh_->accdt.shotPosY;//ボール初期位置 ～ 標的位置(地上スレスレ高さ)

        //距離データが入っていたらその距離を到達点とする
        if (st_.pstSh_->crt.orb.palab.dist > 0)
        {
            f32 spdX = fabs(st_.pstSh_->phs.spdX);
            f32 spdZ = fabs(st_.pstSh_->phs.spdZ);

            //距離
            f32 dist = (f32)st_.pstSh_->crt.orb.palab.dist;
            //高さ 上がる距離StartToPeak
            f32 distStoP = (f32)st_.pstSh_->crt.orb.palab.grav;

            //ジャンプ中は減少
            if (st_.pstSh_->phs.posY > 14.0f) dist = dist * 0.75f, distStoP = distStoP * 0.75f;

            //頂点～下がる距離PeakToEnd
            f32 distPtoE = distStoP + st_.pstSh_->phs.posY - 14.0f;

            //落下時間
            f32 arrTime;
            if (spdZ >= spdX)
            {
                const f32 DEC_RATIO = 1.0f / 5.0f;
                st_.pstSh_->phs.spdZ = (st_.pstSh_->phs.spdZ / spdZ)* st_.pstSh_->phs.spd * DEC_RATIO;
                st_.pstSh_->phs.spdX = 0;
                arrTime = dist / (fabs(st_.pstSh_->phs.spdZ) * 2.0f);
            }
            else
            {
                const f32 DEC_RATIO = 2.0f / 5.0f;
                st_.pstSh_->phs.spdX = (st_.pstSh_->phs.spdX / spdX)* st_.pstSh_->phs.spd * DEC_RATIO;
                st_.pstSh_->phs.spdZ = 0;
                arrTime = dist / (fabs(st_.pstSh_->phs.spdX));
            }
            //重力
            st_.pstSh_->accdt.paraGrav = 2.0f * (distStoP + distPtoE + sqrt(distStoP * distPtoE) * 2.0f) / (arrTime * arrTime);
            //初速
            st_.pstSh_->accdt.accelSpeedY = sqrt(fabs(2.0f * st_.pstSh_->accdt.paraGrav * distStoP));

            st_.pstSh_->accdt.accelSpeedX = st_.pstSh_->phs.spdX;
            st_.pstSh_->accdt.accelSpeedZ = st_.pstSh_->phs.spdZ;
        }
        else
            //距離データが入っていない場合は高さ・時間より重力・初速を算出する
        {
            //***逆放物線フラグ
            BOOL palabNgt_f = FALSE;
            if (st_.pstSh_->crt.orb.palab.point < 0) palabNgt_f = TRUE;

            //***放物直線フラグ
            BOOL palabLine_f = FALSE;
            if (st_.pstSh_->crt.orb.palab.grav < 0) palabLine_f = TRUE;

            //高さ 上がる距離StartToPeak
            f32 distStoP = GetParabHeight();

            //頂点～下がる距離PeakToEnd
            f32 distPtoE = distStoP + st_.pstSh_->phs.posY - 14.0f;

            //***逆放物線フラグ
            if (palabNgt_f == TRUE)
            {
                //高さ 上がる距離StartToPeak
                distStoP = st_.pstSh_->phs.posY - PALAB_NEG_LOW_LEVEL;
                distPtoE = 24.0f - PALAB_NEG_LOW_LEVEL;
                if (st_.pstSh_->phs.posY < PALAB_NEG_LOW_LEVEL)
                {
                    distStoP = st_.pstSh_->phs.posY;
                    distPtoE = 24.0f - st_.pstSh_->phs.posY;
                }
            }

            //時間 + 落下位置
            f32 fallTime = 8.0f - fabs((f32)st_.pstSh_->crt.orb.palab.point);//落下ポイント：基本８で標的
            f32 DIV_DIST = 4.0f;//放物線分解距離↓分割したモノを落下位置番号分足していく

            //XZ加速系処理
            //各加速度単位（1/100）に加速レベルを掛ける
            const f32 ACL_CONST = 1.5f;
            if (st_.pstSh_->phs.acc >= 3.0f) st_.pstSh_->phs.acc = 3.0f;
            if (st_.pstSh_->phs.acc <= -3.0f) st_.pstSh_->phs.acc = -3.0f;
            f32 accLv = 1.00f;

            switch ((s32)st_.pstSh_->phs.acc)
            {
                //加速系最高速の倍率
                case 2: accLv = 1.50f; break;
                case 3: accLv = 2.00f; break;
                //減速系初速倍率
                case -1: accLv = 0.75f; break;
                case -2: accLv = 0.875f; break;
            }

            f32 aclSign = st_.pstSh_->phs.acc / fabs(st_.pstSh_->phs.acc);

            st_.pstSh_->phs.accHX = accLv * st_.pstSh_->phs.spdX / 100.0f;//速度x（保存用）
            st_.pstSh_->phs.accHY = accLv * st_.pstSh_->phs.spdY / 100.0f;//速度y（保存用）
            st_.pstSh_->phs.accHZ = accLv * st_.pstSh_->phs.spdZ / 100.0f;//速度z（保存用）

            if (st_.pstSh_->phs.acc < 0)
            {
                f32 distX = st_.pstSh_->phs.tagposX - st_.pstSh_->phs.posX;
                f32 distY = st_.pstSh_->phs.tagposY - st_.pstSh_->phs.posY;
                f32 distZ = st_.pstSh_->phs.tagposZ - st_.pstSh_->phs.posZ;
                f32 spdX = accLv * fabs(st_.pstSh_->phs.spdX);
                f32 spdY = accLv * fabs(st_.pstSh_->phs.spdY);
                f32 spdZ = accLv * fabs(st_.pstSh_->phs.spdZ);

                st_.pstSh_->phs.accHX = 0;
                if (distX != 0.0f) st_.pstSh_->phs.accHX = (-1) * spdX * spdX / fabs(distX * 2.0f);//速度x（保存用）
                st_.pstSh_->phs.accHY = 0;
                if (distY != 0.0f) st_.pstSh_->phs.accHY = (-1) * spdY * spdY / fabs(distY * 2.0f);//速度Y（保存用）
                st_.pstSh_->phs.accHZ = 0;
                if (distZ != 0.0f) st_.pstSh_->phs.accHZ = (-1) * spdZ * spdZ / fabs(distZ * 2.0f);//速度Z（保存用）
            }

            //加速のときは初速を０にするフラグ
            f32 initSpd_f = 1.0f;
            if (st_.pstSh_->phs.acc > 0) initSpd_f = 0.0f;
            st_.pstSh_->accdt.accelSpeedX = accLv * st_.pstSh_->phs.spdX * initSpd_f;
            st_.pstSh_->accdt.accelSpeedZ = accLv * st_.pstSh_->phs.spdZ * initSpd_f;

            f32 arrTime = GetParabAcTime(st_.pstSh_->phs.acc) * ((DIV_DIST + fallTime) / DIV_DIST);

            //重力
            st_.pstSh_->accdt.paraGrav = 2.0f * (distStoP + distPtoE + sqrt(distStoP * distPtoE) * 2.0f) / (arrTime * arrTime);

            //初速
            st_.pstSh_->accdt.accelSpeedY = sqrt(fabs(2.0f * st_.pstSh_->accdt.paraGrav * distStoP));

            //***逆放物線フラグ
            if (palabNgt_f == TRUE)
            {
                st_.pstSh_->accdt.paraGrav = st_.pstSh_->accdt.paraGrav * (-1.0f);
                st_.pstSh_->accdt.accelSpeedY = sqrt(fabs(2.0f * st_.pstSh_->accdt.paraGrav * distStoP)) * (-1.0f);
            }
            //***放物直線フラグ
            if (palabLine_f == TRUE)
            {
                //頂点Y位置→この点を越えたら敵に向かう速度を反映させる
                st_.pstSh_->cmn.palabLinePeakPosY = distStoP + st_.pstSh_->phs.posY;
                if (palabNgt_f == TRUE) st_.pstSh_->cmn.palabLinePeakPosY = st_.pstSh_->phs.posY - distStoP;

                //重力→重力処理しない
                st_.pstSh_->accdt.paraGrav = 0.0f;

                //頂点までの絶対距離
                f32 distY_a = fabs(distStoP);
                if (palabNgt_f == TRUE) distY_a = distStoP;

                //逆放物線フラグが立っていたら負にする処理
                f32 dirY = 1.0f;
                if (palabNgt_f == TRUE) dirY = -1.0f;
                //初速
                st_.pstSh_->accdt.accelSpeedY = dirY * (distY_a + distPtoE) / arrTime;
            }


            //時間が最小値を下回る場合はXZ速度を補正
            if (arrTime <= MIN_TIME)
            {
                if (st_.pstSh_->phs.spdX != 0.0f)//X速度０でない場合は処理
                {
                    f32 dirX = st_.pstSh_->phs.spdX / fabs(st_.pstSh_->phs.spdX);
                    st_.pstSh_->phs.spdX = dirX * fabs(st_.pstSh_->phs.arrPosX - st_.pstSh_->phs.posX) / MIN_TIME;
                }
                if (st_.pstSh_->phs.spdZ != 0.0f)//Z速度０でない場合は処理
                {
                    f32 dirZ = st_.pstSh_->phs.spdZ / fabs(st_.pstSh_->phs.spdZ);
                    st_.pstSh_->phs.spdZ = dirZ * fabs(st_.pstSh_->phs.arrPosZ - st_.pstSh_->phs.posZ) / MIN_TIME;
                }
                st_.pstSh_->phs.accHX = 0.0f;//速度x（保存用）
                st_.pstSh_->phs.accHZ = 0.0f;//速度z（保存用）
                st_.pstSh_->accdt.accelSpeedX = st_.pstSh_->phs.spdX;
                st_.pstSh_->accdt.accelSpeedZ = st_.pstSh_->phs.spdZ;
            }
        }
    }



    ////毎Fr処理
    //加速処理
    void TBaShot_Acc::AddXAccelOrbit()
    {
        st_.pstSh_->accdt.accelSpeedX = st_.pstSh_->phs.accX + st_.pstSh_->phs.accHX;
        st_.pstSh_->accdt.accelSpeedY = st_.pstSh_->phs.accY + st_.pstSh_->phs.accHY;
        st_.pstSh_->accdt.accelSpeedZ = st_.pstSh_->phs.accZ + st_.pstSh_->phs.accHZ;
        st_.pstSh_->phs.accX = st_.pstSh_->accdt.accelSpeedX;//速度x（保存用）以降はこの速度に減速度を掛け合わせていく
        st_.pstSh_->phs.accY = st_.pstSh_->accdt.accelSpeedY;//速度y（保存用）
        st_.pstSh_->phs.accZ = st_.pstSh_->accdt.accelSpeedZ;//速度z（保存用）

        //保存した速度に段階時間と加速度を掛けて速度を算出
        //        st_.pstSh_->accdt.accelSpeedX = st_.pstSh_->phs.accX + (f32)st_.pstSh_->cmn.timePhase_c * st_.pstSh_->accdt.accelPow;
        //        st_.pstSh_->accdt.accelSpeedY = st_.pstSh_->phs.accY + (f32)st_.pstSh_->cmn.timePhase_c * st_.pstSh_->accdt.accelPow;
        //        st_.pstSh_->accdt.accelSpeedZ = st_.pstSh_->phs.accZ + (f32)st_.pstSh_->cmn.timePhase_c * st_.pstSh_->accdt.accelPow;
    }
    //減速処理
    void TBaShot_Acc::AddXDecelOrbit()
    {
        st_.pstSh_->accdt.accelSpeedX = st_.pstSh_->phs.accX - st_.pstSh_->phs.accHX;
        st_.pstSh_->accdt.accelSpeedY = st_.pstSh_->phs.accY - st_.pstSh_->phs.accHY;
        st_.pstSh_->accdt.accelSpeedZ = st_.pstSh_->phs.accZ - st_.pstSh_->phs.accHZ;

        const f32 LOWEST_SPD = 1.0f; //速度下限値
        if ((fabs(st_.pstSh_->accdt.accelSpeedX) + fabs(st_.pstSh_->accdt.accelSpeedY) + fabs(st_.pstSh_->accdt.accelSpeedZ)) < LOWEST_SPD)
        {//速度下限値を下回ったら、accを更新させないようにspeedに入れちゃう
            st_.pstSh_->accdt.accelSpeedX = st_.pstSh_->phs.accX;
            st_.pstSh_->accdt.accelSpeedY = st_.pstSh_->phs.accY;
            st_.pstSh_->accdt.accelSpeedZ = st_.pstSh_->phs.accZ;
        }
        st_.pstSh_->phs.accX = st_.pstSh_->accdt.accelSpeedX;//速度x（保存用）以降はこの速度に減速度を掛け合わせていく
        st_.pstSh_->phs.accY = st_.pstSh_->accdt.accelSpeedY;//速度y（保存用）
        st_.pstSh_->phs.accZ = st_.pstSh_->accdt.accelSpeedZ;//速度z（保存用）

    }
    //重力処理
    void TBaShot_Acc::AddYAccelOrbit()
    {
        //放物直線の処理
        if (st_.pstSh_->crt.orb.palab.grav < 0)//放物直線
        {
                if (//正の放物直線で頂点座標を上回ったら
                    ((st_.pstSh_->crt.orb.palab.point > 0) && (st_.pstSh_->cmn.palabLinePeakPosY <= st_.pstSh_->phs.posY))
                    ||//負の放物直線で頂点座標を下回ったら
                    ((st_.pstSh_->crt.orb.palab.point < 0) && (st_.pstSh_->cmn.palabLinePeakPosY >= st_.pstSh_->phs.posY))
                    ) st_.pstSh_->phs.spdY = st_.pstSh_->phs.spdY * (-1.0f);//速度向きをひっくりかえす
        }

        //重力で引く
        st_.pstSh_->accdt.accelSpeedY = st_.pstSh_->phs.spdY - st_.pstSh_->accdt.paraGrav;
        st_.pstSh_->accdt.accelSpeedX = st_.pstSh_->phs.spdX;
        st_.pstSh_->accdt.accelSpeedZ = st_.pstSh_->phs.spdZ;
    }

    //重力＋加速処理
    void TBaShot_Acc::AddParabolicXAccelOrbit()
    {
        //放物直線の処理
        if (st_.pstSh_->crt.orb.palab.grav < 0)//放物直線
        {
            if (//正の放物直線で頂点座標を上回ったら
                ((st_.pstSh_->crt.orb.palab.point > 0) && (st_.pstSh_->cmn.palabLinePeakPosY <= st_.pstSh_->phs.posY))
                ||//負の放物直線で頂点座標を下回ったら
                ((st_.pstSh_->crt.orb.palab.point < 0) && (st_.pstSh_->cmn.palabLinePeakPosY >= st_.pstSh_->phs.posY))
                ) st_.pstSh_->phs.spdY = st_.pstSh_->phs.spdY * (-1.0f);//速度向きをひっくりかえす
        }

        //重力で引く
        st_.pstSh_->accdt.accelSpeedY = st_.pstSh_->phs.spdY - st_.pstSh_->accdt.paraGrav;

        f32 signX = 1.0f;//減速用符号
        f32 signZ = 1.0f;//減速用符号
        if ((st_.pstSh_->phs.spdX < 0) && (st_.pstSh_->phs.acc < 0)) signX = -1.0f;
        if ((st_.pstSh_->phs.spdZ < 0) && (st_.pstSh_->phs.acc < 0)) signZ = -1.0f;

        st_.pstSh_->accdt.accelSpeedX = st_.pstSh_->phs.spdX + st_.pstSh_->phs.accHX * signX;
        st_.pstSh_->accdt.accelSpeedZ = st_.pstSh_->phs.spdZ + st_.pstSh_->phs.accHZ * signZ;

    }


    //山なり処理
    void TBaShot_Acc::ParabolicOrbit()
    {
        //山なり素材用初期化終了フラグチェック
        if (st_.pstSh_->accdt.palabOrbInit_f == TRUE)
        {
            //初期化済みなら更新
            SetUpdatedData();//毎時更新するデータをセット
            AddYAccelOrbit();//x等速放物線軌道加算 yのみ二次関数なのでコレを使用
        }
        else {
            //まだなら初期化して初期化済フラグON
            //            InitParabolicOrbit();//x等速放物線軌道初期化
            InitParabolicOrbit2();//x等速放物線軌道初期化
            st_.pstSh_->accdt.palabOrbInit_f = TRUE;//初期化終了フラグON
        }
    }

    //加速系処理
    void TBaShot_Acc::XAccelOrbit()//加速素材
    {
        //加速効果と減速効果両方この処理でOK
        if (st_.pstSh_->accdt.accelEffInit_f == TRUE)
        {
            //初期化済みなら更新
            SetUpdatedData();//毎時更新するデータをセット
            AddXAccelOrbit();//加速シュート系加算
        }
        else {
            //まだなら初期化して初期化済フラグON
            InitXAccelOrbit();//加速シュート系初期化
            st_.pstSh_->accdt.accelEffInit_f = TRUE;//初期化終了フラグON
        }
    }

    //減速処理
    void TBaShot_Acc::XDecelOrbit()//加速素材
    {
        //加速効果と減速効果両方この処理でOK
        if (st_.pstSh_->accdt.decelEffInit_f == TRUE)
        {
            //初期化済みなら更新
            SetUpdatedData();//毎時更新するデータをセット
            AddXDecelOrbit();//加速シュート系加算
        }
        else {
            //まだなら初期化して初期化済フラグON
            InitXDecelOrbit();//加速シュート系初期化
            st_.pstSh_->accdt.decelEffInit_f = TRUE;//初期化終了フラグON
        }
    }

    //y加速系処理
    void TBaShot_Acc::YAccelOrbit()//y加速素材
    {
        if (st_.pstSh_->accdt.yAccelEffInit_f == TRUE)
        {
            //初期化済みなら更新
            SetUpdatedData();//毎時更新するデータをセット
            AddYAccelOrbit();//直落下系加算
        }
        else {
            //まだなら初期化して初期化済フラグON
            InitYAccelOrbit();//直落下系初期化
            st_.pstSh_->accdt.yAccelEffInit_f = TRUE;//初期化終了フラグON
        }
    }
    //yx加速系処理
    void TBaShot_Acc::YXAccelOrbit()//y加速素材
    {
        if (st_.pstSh_->accdt.yxAccelEffInit_f == TRUE)
        {
            //初期化済みなら更新
            SetUpdatedData();//毎時更新するデータをセット
            AddParabolicXAccelOrbit();//直落下系加算
        }
        else {
            //まだなら初期化して初期化済フラグON
            InitYXAccelOrbit();//直落下系初期化
            st_.pstSh_->accdt.yxAccelEffInit_f = TRUE;//初期化終了フラグON
        }
    }

    //x加速放物線処理
    void TBaShot_Acc::ParabolicXAccelOrbit()//x加速放物線軌道初期化
    {
        if (st_.pstSh_->accdt.plAccOrbInit_f == TRUE)
        {
            SetUpdatedData();//毎時更新するデータをセット
            AddParabolicXAccelOrbit();//x加速放物線軌道加算
        }
        else {
            InitParabolicXAccelOrbit();//x加速放物線軌道初期化
            st_.pstSh_->accdt.plAccOrbInit_f = TRUE;
        }
    }

    //二次関数系処理フラグ
    BOOL TBaShot_Acc::CheckAccFlag()
    {
        //放物線軌道フラグ
        st_.pstSh_->accdt.palabOrb_f = FALSE;//一旦オフ　継続素材を作ったらココで調整
        if (st_.pstSh_->crt.orb.palab.point != 0) st_.pstSh_->accdt.palabOrb_f = TRUE;

        //効果素材に関してはst_.pstSh_->phs.accelFunc_f = TRUE をチェック後ココにくる。
        //各素材Lv１以上で効果発動段階と現段階が一致したらフラグON
        st_.pstSh_->accdt.accelEff_f = FALSE;
        st_.pstSh_->accdt.decelEff_f = FALSE;
        st_.pstSh_->accdt.yAccelEff_f = FALSE;
        st_.pstSh_->accdt.yxAccelEff_f = FALSE;
        st_.pstSh_->accdt.plAccOrb_f = FALSE;
        f32 accDt = (f32)st_.pstSh_->idata.eff.accel_Lv[st_.pstSh_->cmn.efEl.accel.times_c];
        f32 decDt = (f32)st_.pstSh_->idata.eff.decel_Lv[st_.pstSh_->cmn.efEl.decel.times_c];

        //加速効果
        if ((st_.pstSh_->cmn.efEl.accel.incld_f == TRUE) && (st_.pstSh_->cmn.efEl.accel.phase_f == TRUE)) st_.pstSh_->accdt.accelEff_f = TRUE;//加速フラグ
        //減速効果
        if ((st_.pstSh_->cmn.efEl.decel.incld_f == TRUE) && (st_.pstSh_->cmn.efEl.decel.phase_f == TRUE)) st_.pstSh_->accdt.decelEff_f = TRUE;//減速フラグ
        //y加速効果
        if ((st_.pstSh_->cmn.efEl.yAccel.incld_f == TRUE) && (st_.pstSh_->cmn.efEl.yAccel.phase_f == TRUE)) st_.pstSh_->accdt.yAccelEff_f = TRUE;//y加速フラグ

        //加速と減速が両立してしまった場合
        if ((st_.pstSh_->accdt.accelEff_f == TRUE) && (st_.pstSh_->accdt.decelEff_f == TRUE))
        {
            //レベルの高い方を優先
            //減速が強い
            if ((accDt - decDt) < 0)
            {
                st_.pstSh_->phs.acc = decDt - accDt;
                st_.pstSh_->accdt.accelEff_f = FALSE;//加速OFF
            }
            else
            {
                st_.pstSh_->phs.acc = accDt - decDt;
                st_.pstSh_->accdt.decelEff_f = FALSE;//減速OFF
            }
        }
        //加速か減速と山なりが両立してしまった場合
        if (
            ((st_.pstSh_->accdt.accelEff_f == TRUE) || (st_.pstSh_->accdt.decelEff_f == TRUE))
            &&
            (st_.pstSh_->accdt.palabOrb_f == TRUE)
            )
        {//山なり＋加減速フラグON ＆ それ以外はOFF
            st_.pstSh_->accdt.plAccOrb_f = TRUE;
            if (st_.pstSh_->accdt.decelEff_f == TRUE) st_.pstSh_->phs.acc = decDt * (-1);//減速はマイナス加速としておく
            st_.pstSh_->accdt.accelEff_f = FALSE;
            st_.pstSh_->accdt.decelEff_f = FALSE;
            st_.pstSh_->accdt.palabOrb_f = FALSE;
        }

        //加速か減速と落下が両立してしまった場合
        if (
            ((st_.pstSh_->accdt.accelEff_f == TRUE) || (st_.pstSh_->accdt.decelEff_f == TRUE))
            &&
            (st_.pstSh_->accdt.yAccelEff_f == TRUE)
            )
        {//落下＋加減速フラグON ＆ それ以外はOFF
            st_.pstSh_->accdt.yxAccelEff_f = TRUE;
            if (st_.pstSh_->accdt.decelEff_f == TRUE) st_.pstSh_->phs.acc = decDt * (-1);//減速はマイナス加速としておく
            st_.pstSh_->accdt.accelEff_f = FALSE;
            st_.pstSh_->accdt.decelEff_f = FALSE;
//            st_.pstSh_->accdt.yAccelEff_f = FALSE;
        }




        //どれかフラグが経ったらTRUEを返す
        if ((st_.pstSh_->accdt.palabOrb_f == TRUE) || (st_.pstSh_->accdt.accelEff_f == TRUE)
            || (st_.pstSh_->accdt.decelEff_f == TRUE) || (st_.pstSh_->accdt.yAccelEff_f == TRUE) || (st_.pstSh_->accdt.yxAccelEff_f == TRUE)
            || (st_.pstSh_->accdt.plAccOrb_f == TRUE))
        {
            return TRUE;
        }
        else
        {
            return FALSE;
        }
    }

    void TBaShot_Acc::SetPosData()
    {
        st_.pstSh_->accdt.shotPosX = st_.pstSh_->phs.posX;//術者X（シュート開始位置）
        st_.pstSh_->accdt.shotPosY = st_.pstSh_->phs.posY;//術者Y（シュート開始位置）
        st_.pstSh_->accdt.shotPosZ = st_.pstSh_->phs.posZ;//術者Z（シュート開始位置）
        st_.pstSh_->accdt.targPosX = st_.pstSh_->phs.tagposX;//標的位置x
        st_.pstSh_->accdt.targPosY = st_.pstSh_->phs.tagposY;//標的位置y
        st_.pstSh_->accdt.targPosZ = st_.pstSh_->phs.tagposZ;//標的位置z
        st_.pstSh_->accdt.spd = st_.pstSh_->phs.spd;//xyz未分割の速度…処理の簡略化の為、放物線に大きく関わる方向間での分割を優先する
        st_.pstSh_->accdt.mainSpeedX = st_.pstSh_->phs.spdX;//SAngleにて速度分割を行った後の速度X
        st_.pstSh_->accdt.mainSpeedY = st_.pstSh_->phs.spdY;//SAngleにて速度分割を行った後の速度Y
        st_.pstSh_->accdt.mainSpeedZ = st_.pstSh_->phs.spdZ;//SAngleにて速度分割を行った後の速度Z
    }

    //st_.pstSh_->accdtに情報を入れる…全体初期化時のみ使用のモノをここで初期化
    void TBaShot_Acc::SetAccData()
    {
        //まずゼロクリア
        st_.pstSh_->accdt.Init();
        //全体初期化以降、毎時更新するデータをココでも更新
        //SetUpdatedData();

        //st_.pstSh_->accdtに情報を入れる
        SetPosData();
        //減速シュートの最初だけ更新したいのでここで減速初期位置Xを入れておく
        st_.pstSh_->accdt.dclInitPosX = st_.pstSh_->phs.posX;//術者X（シュート開始位置）

        CheckAccFlag();//フラグもクリアされちゃうのでもう一度フラグチェック＆セット
        st_.pstSh_->phs.accelFuncInit_f = TRUE;//加速系関数全般初期化フラグOn
        st_.pstSh_->cmn.reSAngle_f = FALSE;//再SAngleフラグOFF
        st_.pstSh_->accdt.accelEffInit_f = FALSE;//加速初期化終了フラグOFF
        st_.pstSh_->accdt.decelEffInit_f = FALSE;//加速初期化終了フラグOFF
        st_.pstSh_->accdt.yAccelEffInit_f = FALSE;//y加速初期化終了フラグOFF
        st_.pstSh_->accdt.yxAccelEffInit_f = FALSE;//y加速初期化終了フラグOFF
    }


    //加速素材⇒加速度レベルから加速度パワーを算出
    void TBaShot_Acc::SetAccelPow()
    {
        //基本は以下の式で加速度を設定
        st_.pstSh_->accdt.accelPow = (st_.pstSh_->phs.acc + 2.0f) * 0.005f;

        if (st_.pstSh_->phs.acc > 6)
        {
            //Lv7-　４Fr間超低速⇒４Fr間低速
            if (st_.pstSh_->cmn.timePhase_c < 4)
            {
                st_.pstSh_->accdt.accelPow = st_.pstSh_->phs.acc * 0.001f;
            }
            else
            if (st_.pstSh_->cmn.timePhase_c < 17)
            {
                st_.pstSh_->accdt.accelPow = (st_.pstSh_->phs.acc + 1.0f)  * 0.002f;
            }
        }
        else
        if (st_.pstSh_->phs.acc > 3)
        {
            //Lv4-6　４Fr間低速
            if (st_.pstSh_->cmn.timePhase_c < 11) st_.pstSh_->accdt.accelPow = st_.pstSh_->phs.acc * 0.003f;
        }
    }


    //現状の速度を入れる＋加速度レベルの取得…初期化時と、毎時更新のため関数化
    void TBaShot_Acc::SetUpdatedData()
    {
        //st_.pstSh_->accdtに最新の位置情報を入れる
        SetPosData();

        //山なり軌道 現素材データを重力に入れる。

        //二回目以降重力が保存されていたらそれを採用
        if (st_.pstSh_->phs.graAcc != 0) st_.pstSh_->accdt.paraGrav = st_.pstSh_->phs.graAcc;
        //重力設定がない場合はレベルで設定
        if ((st_.pstSh_->accdt.paraGrav == 0) && (st_.pstSh_->crt.orb.palab.grav == 0)) st_.pstSh_->accdt.paraGrav = st_.pstSh_->crt.orb.palab.grav * 0.1f;
        //距離指定の場合は重力が固定
        //if (st_.pstSh_->crt.orb.palab.dist > 0) st_.pstSh_->accdt.paraGrav = CO_GRAV;

        //加速系効果
        //加速素材　Lvで加速度と変化の仕方を変える
        //       if (st_.pstSh_->accdt.accelEff_f == TRUE) SetAccelPow();//加速度設定

        //減速素材
        //if (st_.pstSh_->accdt.decelEff_f == TRUE)
        //{
        //}

        //y加速
        if ((st_.pstSh_->accdt.yxAccelEff_f == FALSE) && (st_.pstSh_->accdt.yAccelEff_f == TRUE)) st_.pstSh_->accdt.accelPow = st_.pstSh_->phs.acc * 0.002f;

    }

    //計算結果から値を取得
    void TBaShot_Acc::GetAccData()
    {
        st_.pstSh_->phs.spdX = st_.pstSh_->accdt.accelSpeedX;//現状態の速度Xへ
        st_.pstSh_->phs.spdY = st_.pstSh_->accdt.accelSpeedY;//現状態の速度Yへ
        st_.pstSh_->phs.spdZ = st_.pstSh_->accdt.accelSpeedZ;//現状態の速度Zへ
        st_.pstSh_->phs.graAcc = st_.pstSh_->accdt.paraGrav;//重力の保存
    }

    //初期化用関数（ほかで呼び出すため）
    void TBaShot_Acc::AccInit()
    {
        st_.pstSh_->accdt.Init();
    }

    //２次関数の計算…加速系処理（加速効果、山なり（＝重力加速効果））
    //反れ縦・反れサイドからy、z速度を算出したあとで処理。標的位置も使う。
    //ボール＆標的初期位置・初速（未分解の速度）・加速度（重力加速度）をココで設定・計算し
    //初速の分割を行い、別の場所で毎Fr処理を行う。
    void TBaShot_Acc::UpDate()
    {
        //二次関数系素材のチェック…既に”accelFunc_f == TRUE”（シュートのどこかに加速系素材アリ）を通ってきている
        if (CheckAccFlag() == TRUE)//発動段階かどうかのチェック
        {
            st_.pstSh_->phs.accelFuncMove_f = TRUE;
            //全体初期化まだ or 再SAngleしたら⇒st_.pstSh_->accdtに一旦情報を入れる
            if ((st_.pstSh_->phs.accelFuncInit_f == FALSE) || (st_.pstSh_->cmn.reSAngle_f == TRUE)) SetAccData();



            //素材別初期化⇒加速度を算出し以降加速度を加算する
            enAccDt accMode = adt_Non;//行う加速or減速
            enAccDt accMtd = adt_Non;//行う処理選別

            //優先順位が高いほうを下に持っていく
            //加速減速チェック
            if (st_.pstSh_->accdt.decelEff_f == TRUE) accMode = adt_Dcl;//減速素材
            if (st_.pstSh_->accdt.accelEff_f == TRUE) accMode = adt_Acl;//加速素材
            //ｙ加速放物線チェック
            if (st_.pstSh_->accdt.yAccelEff_f == TRUE) accMtd = adt_YAc;//y加速素材
            if (st_.pstSh_->accdt.palabOrb_f == TRUE) accMtd = adt_Plb;//山なり素材

            //加速or減速フラグが立っている
            if (accMode != adt_Non)
            {
                //y加速フラグが立っている
                if (accMtd == adt_YAc) accMode = adt_YAA;//ｙ加速＋XZ加速減速処理
                //放物線軌道フラグが立っている
                if (accMtd == adt_Plb) accMode = adt_PlA;//x加速放物線軌道初期化
            }
            else
            {
            //加速or減速がない場合は変化方法をModeに入れる
                accMode = accMtd;
            }

            //加速減速放物線軌道フラグじかに立っている
            if (st_.pstSh_->accdt.plAccOrb_f == TRUE) accMode = adt_PlA;//x加速放物線軌道初期化
            if (st_.pstSh_->accdt.yxAccelEff_f == TRUE) accMode = adt_YAA;//x加速放物線軌道初期化

            //accModeより各処理に飛ばす
            switch (accMode){
            case adt_Acl: XAccelOrbit(); break;//加速素材
            case adt_Dcl: XDecelOrbit(); break;//加速素材
            case adt_YAc: YAccelOrbit(); break;//y加速素材
            case adt_Plb: ParabolicOrbit(); break;//山なり素材
            case adt_YAA: YXAccelOrbit(); break;//Y加速＋XZ加速減速処理
            case adt_PlA: ParabolicXAccelOrbit(); break;//放物線＋XZ加速減速処理
            default: break;
            }


            ////計算結果から値を取得
            GetAccData();
        }
    }


}
