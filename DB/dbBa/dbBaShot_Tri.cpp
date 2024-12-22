#include "dbBaShot_Tri.h"
#include "dbGame.h"

namespace db
{

    const f32 COURT = 64;// コート手前奥方向の半分の長さ
    const f32 PI = (f32)3.1415;// π
    const f32 ANGLE_SCALE = PI / 60;//毎時変化角度の最小単位 3度


    //コンストラクタ
    TBaShot_Tri::TBaShot_Tri(TGame* pGame, TBaCommon* pCommon)
        : TBaHaveCommon(pGame, pCommon)
    {
    }

    TBaShot_Tri::~TBaShot_Tri()
    {
    }


    //波データ初期化
    void TBaShot_Tri::WaveData(s32 direction, f32 shotPos, s32 waveType, s32 waveLenLv, s32 waveWidLv,
        s32 waveWChangeType, s32 waveWChngeSpeed, s32 waveWChageCount,
        f32 waveAngle, s32 waveDiv, f32 waveZigAdd)
    {
        //加速系処理中でないなら三角関数用に速度をいじる
        //if (st_.pstSh_->phs.accelFuncMove_f == FALSE) ReSAngle();

        //波増減用比率
        f32 waveRatio = 1.0f;//デフォは１
        //波増減タイプが１以上なら比率を変える
        if (waveWChangeType > 0) waveRatio = GetWaveWidthRatio(waveWChangeType, waveWChngeSpeed, waveWChageCount);
        if ((waveWChangeType == 2) && (waveRatio <= 0.0f)) waveWChangeType = 3, waveWChageCount = -1;//増減タイプで最小になったら増幅に転換
        if ((waveWChangeType == 3) && (waveRatio >= 1.0f)) waveWChangeType = 2, waveWChageCount = -1;//増減タイプで最大になったら減衰に転換

        //各波・鋸処理
        switch (waveType)
        {
            //最後の引数はFALSE⇒プラス、TRUE⇒マイナス
        case 1: WaveSinFunction(waveLenLv, waveWidLv, waveAngle, FALSE, waveRatio); break;//+sin関数
        case 2: WaveSinFunction(waveLenLv, waveWidLv, waveAngle, TRUE, waveRatio);  break;//-sin関数
        case 3: WaveCosFunction(waveLenLv, waveWidLv, waveAngle, FALSE, waveRatio); break;//+cos関数
        case 4: WaveCosFunction(waveLenLv, waveWidLv, waveAngle, TRUE, waveRatio);  break;//-cos関数
        case 5: WaveSinZigFunction(waveLenLv, waveWidLv, waveAngle, FALSE, waveRatio); break;//+sinギザ関数
        case 6: WaveSinZigFunction(waveLenLv, waveWidLv, waveAngle, TRUE, waveRatio);  break;//-sinギザ関数
        case 7: WaveCosZigFunction(waveLenLv, waveWidLv, waveAngle, FALSE, waveRatio); break;//+cosギザ関数
        case 8: WaveCosZigFunction(waveLenLv, waveWidLv, waveAngle, TRUE, waveRatio);  break;//-cosギザ関数
        }

        //加算する値を該当の方向の座標に加え,振動関数内の値を現在データに入れる
        switch (direction)
        {
        case 1: //Y軸
            st_.pstSh_->tridt.shotPosY = st_.pstSh_->tridt.addVal;
            st_.pstSh_->crt.orb.waveY.triFuncVal = st_.pstSh_->tridt.triFuncVal;
            st_.pstSh_->crt.orb.waveY.triFuncValPr = st_.pstSh_->tridt.triFuncValPr;
            st_.pstSh_->crt.orb.waveY.triFuncValFt = st_.pstSh_->tridt.triFuncValFt;
            st_.pstSh_->phs.waveYDivTime = st_.pstSh_->tridt.triFuncPeakTime;
            st_.pstSh_->crt.orb.waveY.widthChg_Mtd = waveWChangeType;
            st_.pstSh_->tridt.waveYWChangeCount = waveWChageCount;
            break;
        case 2: //Z軸
            st_.pstSh_->tridt.shotPosZ = st_.pstSh_->tridt.addVal;
            st_.pstSh_->crt.orb.waveZ.triFuncVal = st_.pstSh_->tridt.triFuncVal;
            st_.pstSh_->crt.orb.waveZ.triFuncValPr = st_.pstSh_->tridt.triFuncValPr;
            st_.pstSh_->crt.orb.waveZ.triFuncValFt = st_.pstSh_->tridt.triFuncValFt;
            st_.pstSh_->phs.waveZDivTime = st_.pstSh_->tridt.triFuncPeakTime;
            st_.pstSh_->crt.orb.waveZ.widthChg_Mtd = waveWChangeType;
            st_.pstSh_->tridt.waveZWChangeCount = waveWChageCount;
            break;
        default://X軸
            st_.pstSh_->tridt.shotPosX = st_.pstSh_->tridt.addVal;
            st_.pstSh_->crt.orb.waveX.triFuncVal = st_.pstSh_->tridt.triFuncVal;
            st_.pstSh_->crt.orb.waveX.triFuncValPr = st_.pstSh_->tridt.triFuncValPr;
            st_.pstSh_->crt.orb.waveX.triFuncValFt = st_.pstSh_->tridt.triFuncValFt;
            st_.pstSh_->phs.waveXDivTime = st_.pstSh_->tridt.triFuncPeakTime;
            st_.pstSh_->crt.orb.waveX.widthChg_Mtd = waveWChangeType;
            st_.pstSh_->tridt.waveXWChangeCount = waveWChageCount;
            break;
        }

    }

    //sin関数
    void TBaShot_Tri::WaveSinFunction(s32 waveLenLv, s32 waveWidLv, f32 waveAngle, BOOL dir, f32 waveRatio)
    {
        //基本情報
        f32 waveWidth = GetWaveWidthMax(waveWidLv);//振幅
        f32 angleSpdLv = GetAngleSpdLv(waveLenLv);//角速度レベル
        s32 direction = GetWaveDirection(dir);//＋sin or －sin
        f32 angleSpd = GetAngleSpd(waveWidth);//角速度

        //sin関数
        f32 sinDt = sin(waveAngle * angleSpd * angleSpdLv);

        //計算結果
        st_.pstSh_->tridt.addVal = waveWidth * sinDt * (f32)direction * waveRatio;//座標への追加
        //        st_.pstSh_->tridt.triFuncVal = sinDt;//振動関数データ更新
        //        st_.pstSh_->tridt.triFuncValPr = sin((waveAngle - 1) * angleSpd * angleSpdLv);
        st_.pstSh_->tridt.triFuncVal = waveAngle * angleSpd * angleSpdLv;//振動関数データ更新
        st_.pstSh_->tridt.triFuncValPr = ((waveAngle - 1) * angleSpd * angleSpdLv);
        st_.pstSh_->tridt.triFuncValFt = ((waveAngle + 1) * angleSpd * angleSpdLv);
        if (angleSpd != 0)st_.pstSh_->tridt.triFuncPeakTime = waveWidth / (angleSpd * angleSpdLv);
    }
    //cos関数
    void TBaShot_Tri::WaveCosFunction(s32 waveLenLv, s32 waveWidLv, f32 waveAngle, BOOL dir, f32 waveRatio)
    {
        //基本情報
        f32 waveWidth = GetWaveWidthMax(waveWidLv);//振幅
        f32 angleSpdLv = GetAngleSpdLv(waveLenLv);//角速度レベル
        s32 direction = GetWaveDirection(dir);//＋cos or －cos
        f32 angleSpd = GetAngleSpd(waveWidth);//角速度

        //cos関数
        f32 cosDt = cos(waveAngle * angleSpd * angleSpdLv);

        //計算結果
        st_.pstSh_->tridt.addVal = waveWidth * (direction * cosDt + ((-1) * direction)) * waveRatio;//座標への追加
        //        st_.pstSh_->tridt.triFuncVal = cosDt;//振動関数データ更新
        //        st_.pstSh_->tridt.triFuncValPr = cos((waveAngle - 1) * angleSpd * angleSpdLv);
        st_.pstSh_->tridt.triFuncVal = waveAngle * angleSpd * angleSpdLv;//振動関数データ更新
        st_.pstSh_->tridt.triFuncValPr = ((waveAngle - 1) * angleSpd * angleSpdLv);
        st_.pstSh_->tridt.triFuncValFt = ((waveAngle + 1) * angleSpd * angleSpdLv);
        if (angleSpd != 0)st_.pstSh_->tridt.triFuncPeakTime = waveWidth / (angleSpd * angleSpdLv);
    }

    //sinギザ関数
    void TBaShot_Tri::WaveSinZigFunction(s32 waveLenLv, s32 waveWidLv, f32 waveAngle, BOOL dir, f32 waveRatio)
    {
        //Sinと表記しているが実際には、sinを使わない。
        //ギザギザの頂点から増減する量を計算する。
        //頂点は最大waveWidth、コレに到達するにはwaveLength * waveAngle ＝π/2＝９０度に
        //達する必要があり、waveAngle ＝π/（2*waveLength）＝時間となり、
        //距離waveWidth / （π/（2*waveLength））時間＝速度となる。

        //基本情報
        f32 waveWidth = GetWaveWidthMax(waveWidLv);//振幅
        f32 angleSpdLv = GetAngleSpdLv(waveLenLv);//角速度レベル
        s32 direction = GetWaveDirection(dir);//＋sin or －sin
        f32 angleSpd = GetAngleSpd(waveWidth);//角速度

        //sin関数
        f32 gizaSin = GetZigAdd(FALSE, waveAngle * angleSpd * angleSpdLv * 2.0f / PI);//角度（＋傾き）

        //計算結果
        st_.pstSh_->tridt.addVal = waveWidth * gizaSin * (f32)direction * waveRatio;//座標への追加
        //        st_.pstSh_->tridt.triFuncVal = gizaSin;//振動関数データ更新
        //        st_.pstSh_->tridt.triFuncValPr = GetZigAdd(FALSE, (waveAngle - 1) * angleSpd * angleSpdLv * 2.0f / PI);
        st_.pstSh_->tridt.triFuncVal = waveAngle * angleSpd * angleSpdLv;//振動関数データ更新
        st_.pstSh_->tridt.triFuncValPr = ((waveAngle - 1) * angleSpd * angleSpdLv);
        st_.pstSh_->tridt.triFuncValFt = ((waveAngle + 1) * angleSpd * angleSpdLv);
        if (angleSpd != 0)st_.pstSh_->tridt.triFuncPeakTime = waveWidth / (angleSpd * angleSpdLv);

    }
    //cosギザ関数
    void TBaShot_Tri::WaveCosZigFunction(s32 waveLenLv, s32 waveWidLv, f32 waveAngle, BOOL dir, f32 waveRatio)
    {
        //基本情報
        f32 waveWidth = GetWaveWidthMax(waveWidLv);//振幅
        f32 angleSpdLv = GetAngleSpdLv(waveLenLv);//角速度レベル
        s32 direction = GetWaveDirection(dir);//＋sin or －sin
        f32 angleSpd = GetAngleSpd(waveWidth);//角速度

        //cos関数
        f32 gizaCos = GetZigAdd(TRUE, waveAngle * angleSpd * angleSpdLv * 2.0f / PI);//角度（＋傾き）

        //計算結果
        st_.pstSh_->tridt.addVal = waveWidth * (direction * gizaCos + ((-1) * direction)) * waveRatio;//座標への追加
        //        st_.pstSh_->tridt.triFuncVal = gizaCos;//振動関数データ更新
        //        st_.pstSh_->tridt.triFuncValPr = GetZigAdd(TRUE, (waveAngle - 1)* angleSpd * angleSpdLv * 2.0f / PI);
        st_.pstSh_->tridt.triFuncVal = waveAngle * angleSpd * angleSpdLv;//振動関数データ更新
        st_.pstSh_->tridt.triFuncValPr = ((waveAngle - 1) * angleSpd * angleSpdLv);
        st_.pstSh_->tridt.triFuncValFt = ((waveAngle + 1) * angleSpd * angleSpdLv);

        if (angleSpd != 0)st_.pstSh_->tridt.triFuncPeakTime = waveWidth / (angleSpd * angleSpdLv);
    }

    //ギザ振動関数
    f32 TBaShot_Tri::GetZigAdd(BOOL cosSwitch, f32 zigVal)
    {
        f32 baseVal = 0;//基本値
        f32 variVal = 1;//変化方向（正）
        s32 zigVal_i = (s32)zigVal;//zigValue整数部分
        f32 zigVal_f = zigVal - (f32)zigVal_i;//zigVal小数部分
        switch (zigVal_i % 4)//第何象限か（0～3）
        {
        case 1://Sin Bs1 Dr- : Cos Bs0 Dr-
            if (cosSwitch == FALSE) baseVal = 1;//Sinのみ Bs1
            variVal = -1;//ともにDr-
            break;
        case 2: //Sin Bs0 Dr- : Cos Bs-1 Dr+
            if (cosSwitch == FALSE) variVal = -1;//Sinのみ Dr-
            if (cosSwitch == TRUE) baseVal = -1;//Cosのみ Bs-1
            break;
        case 3: //Sin Bs-1 Dr+ : Cos Bs0 Dr+
            if (cosSwitch == FALSE) baseVal = -1;//Sinのみ Bs-1
            break;
        default://Sin Bs0 Dr+ : Cos Bs1 Dr-1
            if (cosSwitch == TRUE)
            {
                baseVal = 1;
                variVal = -1;
            }
            break;
        }
        return baseVal + variVal * zigVal_f;//基本値に傾き×方向を足す
    }

    //振動の開始方向を返す
    s32 TBaShot_Tri::GetWaveDirection(s32 dir)
    {
        s32 res = 1;
        if (dir == TRUE) res = -1;
        return res;
    }
    //振幅を返すLv1～5
    f32 TBaShot_Tri::GetWaveWidthMax(s32 waveWidLv)
    {
        //Lv1～5
        if (waveWidLv > 5) waveWidLv = 5;
        if (waveWidLv < 1) waveWidLv = 1;

        const f32 DEF_WIDTH = 10.0;//デフォの振幅
        f32 res = DEF_WIDTH;

        if (waveWidLv >= 3) res = DEF_WIDTH + 8.0f * (f32)(waveWidLv - 3);//振幅加算
        else res = 2.0f * (f32)(waveWidLv);//振幅減算

        //サイドカーブだまで　スタートがセンター寄りなら少し振幅を落とす
        if (st_.pstSh_->phs.sideCurveWidSp_f == TRUE) res = 6.0f;

        //Zメインのときは振幅を75％にする
        if (st_.pstSh_->cmn.maxSpdAx == sca_Z) res = res * 0.75f;


        return res;
    }

    //角速度倍率を返す Lv-2～0～+2
    f32 TBaShot_Tri::GetAngleSpdLv(s32 waveLenLv)
    {
        //Lv-2～0～+2
        if (waveLenLv > 2) waveLenLv = 2;
        if (waveLenLv < -2) waveLenLv = -2;

        f32 res = 0.5f + (f32)(waveLenLv) / 4.0f;//デフォルトの角速度倍率

        if (waveLenLv < 0) res = 0.5f + (f32)(waveLenLv) * 0.1f;

        return res;
    }

    //角速度を返す
    f32 TBaShot_Tri::GetAngleSpd(f32 waveWidth)
    {
        //速度の基準となるspd
        f32 spd = st_.pstSh_->phs.spdZ * 1.5f;
        f32 spdX_a = fabs(st_.pstSh_->phs.spdX);
        f32 spdY_a = fabs(st_.pstSh_->phs.spdY);
        f32 spdZ_a = fabs(st_.pstSh_->phs.spdZ);
        BOOL cmprXY_f = FALSE;//比較XY
        BOOL cmprYZ_f = FALSE;//比較YZ
        BOOL cmprXZ_f = FALSE;//比較XZ

        if (spdX_a >= spdY_a) cmprXY_f = TRUE;
        if (spdY_a >= spdZ_a) cmprYZ_f = TRUE;
        if (spdX_a >= spdZ_a) cmprXZ_f = TRUE;
        if ((cmprXY_f == TRUE) && (cmprXZ_f == TRUE)) spd = st_.pstSh_->phs.spdX;
        if ((cmprXY_f == FALSE) && (cmprYZ_f == TRUE)) spd = st_.pstSh_->phs.spdY;

        if (fabs(spd) <= 0.5f) spd = 0.5f;//速度が０なら適当に3.0fを
        if (st_.pstSh_->phs.spdWaveInit_f == FALSE)
        {
            st_.pstSh_->phs.spdWave = spd;
            st_.pstSh_->phs.spdWaveInit_f = TRUE;
        }
        else spd = st_.pstSh_->phs.spdWave;

        spd = 3.0f;

        f32 res = fabs(spd * 0.5f * PI / waveWidth);//デフォルトの角速度
//        if (fabs(spd) <= 0.5f) res = 0.5f;//速度が０なら適当に3.0fを

        return res;
    }

    //波振幅増減処理…比率を返す
    f32 TBaShot_Tri::GetWaveWidthRatio(s32 waveWChangeType, s32 waveWChngeSpeed, s32 waveWChageCount)
    {

        //増減速度の基準となるspdX
        f32 spd = fabs(st_.pstSh_->phs.spdX);
        BOOL shootZ_f = FALSE;
        if (fabs(st_.pstSh_->phs.spdZ) > spd) spd = st_.pstSh_->phs.spdZ , shootZ_f = TRUE;
        f32 SPD_LOW_LIMIT = 0.75;//速度最低値
        if (spd < SPD_LOW_LIMIT) spd = SPD_LOW_LIMIT;//速度が０なら速度最低値を入れる

        //３種の波の振動数より絶対値が一番大きいものを選んでそれを波の増減速度とする
        //この処理だと苦情があったので上に戻します
/*        s32 lenLv = st_.pstSh_->tridt.waveYLenLv;//いったんY振動数

        //|Z|>|Y|
        if (//|Z|>|Y|ならZ
            (abs(st_.pstSh_->tridt.waveZLenLv) > abs(lenLv))
            ||//|Z|=|Y|でZが正ならZ　（|Z|=|Y|なのでZが正ならZをlenLvに入れればおｋ）
            ((abs(st_.pstSh_->tridt.waveZLenLv) == abs(lenLv)) && (st_.pstSh_->tridt.waveZLenLv > 0))
            ) lenLv = st_.pstSh_->tridt.waveZLenLv;//lenLvをZの値にする
        
        //|X|>|lenLv|
        if (//|X|>|lenLv|ならX
            (abs(st_.pstSh_->tridt.waveXLenLv) > abs(lenLv))
            ||//|X|=|lenLv|でXが正ならX　（|X|=|lenLv|なのでXが正ならXをlenLvに入れればおｋ）
            ((abs(st_.pstSh_->tridt.waveXLenLv) == abs(lenLv)) && (st_.pstSh_->tridt.waveXLenLv > 0))
            ) lenLv = st_.pstSh_->tridt.waveXLenLv;//lenLvをXの値にする

        //lenLvに波の増減速度Lvが入ったので
        //Lv分けでspdに実際の増減速度を入れる
        f32 spd;

        switch (lenLv)
        {
        case 1: spd = 1.00f; break;
        case 2: spd = 1.25f; break;
        case -1: spd = 0.50f; break;
        case -2: spd = 0.25f; break;
        default: spd = 0.75f; break;
        }
*/

        //変化の距離を求めるための処理
        //指定距離を変化方法で仕分ける
        f32 distConst = 1.0f;//増幅のみor減衰のみの場合は指定距離そのまま
        if ((waveWChangeType == 2) || (waveWChangeType == 3)) distConst = 0.5f;//増減する場合は指定距離の半分で

        //距離指定
        f32 defDist = 200.0f;//デフォルトの距離
//        if (shootZ_f == TRUE) defDist = 36.0f;
        //変化距離(waveWChngeSpeed…実際は速度Lvを掛け合わせ正なら”早く”減衰　
        //distConstによって更に変化方法によってはその半分になることも)
        f32 distChange = (defDist - ((defDist * (f32)waveWChngeSpeed) / 10.0f)) * distConst;


        //変化に掛かる時間を求める
        f32 timeAll = distChange / spd;

        //現在の時間カウント÷変化にかかる時間より振幅比率を求める
        f32 widthRatio = waveWChageCount / timeAll;//増幅
        if (waveWChangeType < 3) widthRatio = 1.0f - (waveWChageCount / timeAll);//減衰

        if ((waveWChangeType == 1) && (widthRatio < 0.1f)) widthRatio = 0.1f;//減衰の場合最低0.1f

        return widthRatio;
    }

    //*******************************************************************
    //波３方向
    void TBaShot_Tri::WaveXOrbit()//波前後素材
    {
        if (st_.pstSh_->tridt.waveXInit_f == FALSE)

        {
            st_.pstSh_->tridt.waveXAngle = 0;//角度を０にする
            st_.pstSh_->tridt.waveXInit_f = TRUE;//フラグON
        }
        waveXOrbit();//毎Fr処理

    }
    void TBaShot_Tri::WaveYOrbit()//波縦素材
    {
        if (st_.pstSh_->tridt.waveYInit_f == FALSE)
        {
            st_.pstSh_->tridt.waveYAngle = 0;//角度を０にする
            st_.pstSh_->tridt.waveYInit_f = TRUE;//フラグON
        }
        waveYOrbit();//毎Fr処理
    }
    void TBaShot_Tri::WaveZOrbit()//波サイド素材
    {
        if (st_.pstSh_->tridt.waveZInit_f == FALSE)
        {
            st_.pstSh_->tridt.waveZAngle = 0;//角度を０にする
            st_.pstSh_->tridt.waveZInit_f = TRUE;//フラグON
        }
        waveZOrbit();//毎Fr処理
    }


    //波３方向
    void TBaShot_Tri::waveXOrbit()//波x
    {
        //波更新
        s32 waveType = st_.pstSh_->crt.orb.waveX.wvtypeNoSort[st_.pstSh_->crt.orb.waveX.wvtypeNo_c];

        //さいどかーぶだま で センターラインＺ付近ならWaveLenLvを下げるための処理
        st_.pstSh_->phs.sideCurveWidSp_f = FALSE;//一般にはLvは下げない
        if ((st_.pstSh_->cmn.courtNearCenterZ_f == TRUE) && (st_.pstSh_->phs.sideCurve_f == TRUE)) st_.pstSh_->phs.sideCurveWidSp_f = TRUE;

        WaveData(0, st_.pstSh_->tridt.shotPosX, waveType, st_.pstSh_->tridt.waveXLenLv, st_.pstSh_->tridt.waveXWidLv,
            st_.pstSh_->tridt.waveXWChangeType, st_.pstSh_->tridt.waveXWChangeSpeed, st_.pstSh_->tridt.waveXWChangeCount,
            st_.pstSh_->tridt.waveXAngle, 0, st_.pstSh_->tridt.waveXZigAdd);
        //addデータ更新有無
        if (CheckAddWaveAngle(st_.pstSh_->tridt.waveXDiv, st_.pstSh_->tridt.waveXDiv_f, st_.pstSh_->tridt.waveXDiv_c, st_.pstSh_->tridt.waveXDivTime,
//            st_.pstSh_->crt.orb.waveX.triFuncVal, st_.pstSh_->crt.orb.waveX.triFuncValPr, st_.pstSh_->tridt.waveXAngle,
            st_.pstSh_->crt.orb.waveX.triFuncValFt, st_.pstSh_->crt.orb.waveX.triFuncVal, st_.pstSh_->tridt.waveXAngle,
            st_.pstSh_->tridt.waveXAnglePhase, st_.pstSh_->tridt.waveXAngleRtTms,
            st_.pstSh_->tridt.waveXType, st_.pstSh_->tridt.waveXPeakStop, st_.pstSh_->tridt.waveXPeakStop_f, st_.pstSh_->tridt.waveXPeakStop_c))
        {
            st_.pstSh_->tridt.waveXAngle = st_.pstSh_->tridt.waveXAngle + 1;//毎時変化波角度を追加（関数先で単位角度と掛け合わせて実際の値となる）
            st_.pstSh_->tridt.waveXWChangeCount = st_.pstSh_->tridt.waveXWChangeCount + 1;
        }
        //現在の週回数を波タイプ数最大値で割ったあまりを波タイプカウントに入れる
        st_.pstSh_->crt.orb.waveX.wvtypeNo_c = (st_.pstSh_->tridt.waveXAngleRtTms % st_.pstSh_->crt.orb.waveX.wvtypeNoMax);
    }
    void TBaShot_Tri::waveYOrbit()//波y
    {
        //波更新
        s32 waveType = st_.pstSh_->crt.orb.waveY.wvtypeNoSort[st_.pstSh_->crt.orb.waveY.wvtypeNo_c];
        WaveData(1, st_.pstSh_->tridt.shotPosY, waveType, st_.pstSh_->tridt.waveYLenLv, st_.pstSh_->tridt.waveYWidLv,
            st_.pstSh_->tridt.waveYWChangeType, st_.pstSh_->tridt.waveYWChangeSpeed, st_.pstSh_->tridt.waveYWChangeCount,
            st_.pstSh_->tridt.waveYAngle, st_.pstSh_->tridt.waveYDiv, st_.pstSh_->tridt.waveYZigAdd);
        //addデータ更新有無
        if (CheckAddWaveAngle(st_.pstSh_->tridt.waveYDiv, st_.pstSh_->tridt.waveYDiv_f, st_.pstSh_->tridt.waveYDiv_c, st_.pstSh_->tridt.waveYDivTime,
//            st_.pstSh_->crt.orb.waveY.triFuncVal, st_.pstSh_->crt.orb.waveY.triFuncValPr, st_.pstSh_->tridt.waveYAngle,
            st_.pstSh_->crt.orb.waveY.triFuncValFt, st_.pstSh_->crt.orb.waveY.triFuncVal, st_.pstSh_->tridt.waveYAngle,
            st_.pstSh_->tridt.waveYAnglePhase, st_.pstSh_->tridt.waveYAngleRtTms,
            st_.pstSh_->tridt.waveYType, st_.pstSh_->tridt.waveYPeakStop, st_.pstSh_->tridt.waveYPeakStop_f, st_.pstSh_->tridt.waveYPeakStop_c))
        {
            st_.pstSh_->tridt.waveYAngle = st_.pstSh_->tridt.waveYAngle + 1;//毎時変化波角度を追加（関数先で単位角度と掛け合わせて実際の値となる）
            st_.pstSh_->tridt.waveYWChangeCount = st_.pstSh_->tridt.waveYWChangeCount + 1;
        }
        //現在の週回数を波タイプ数最大値で割ったあまりを波タイプカウントに入れる
        st_.pstSh_->crt.orb.waveY.wvtypeNo_c = (st_.pstSh_->tridt.waveYAngleRtTms % st_.pstSh_->crt.orb.waveY.wvtypeNoMax);
    }
    void TBaShot_Tri::waveZOrbit()//波z
    {
        //波更新
        s32 waveType = st_.pstSh_->crt.orb.waveZ.wvtypeNoSort[st_.pstSh_->crt.orb.waveZ.wvtypeNo_c];
        WaveData(2, st_.pstSh_->tridt.shotPosZ, waveType, st_.pstSh_->tridt.waveZLenLv, st_.pstSh_->tridt.waveZWidLv,
            st_.pstSh_->tridt.waveZWChangeType, st_.pstSh_->tridt.waveZWChangeSpeed, st_.pstSh_->tridt.waveZWChangeCount,
            st_.pstSh_->tridt.waveZAngle, st_.pstSh_->tridt.waveZDiv, st_.pstSh_->tridt.waveZZigAdd);
        //addデータ更新有無
        if (CheckAddWaveAngle(st_.pstSh_->tridt.waveZDiv, st_.pstSh_->tridt.waveZDiv_f, st_.pstSh_->tridt.waveZDiv_c, st_.pstSh_->tridt.waveZDivTime,
//            st_.pstSh_->crt.orb.waveZ.triFuncVal, st_.pstSh_->crt.orb.waveZ.triFuncValPr, st_.pstSh_->tridt.waveZAngle,
            st_.pstSh_->crt.orb.waveZ.triFuncValFt, st_.pstSh_->crt.orb.waveZ.triFuncVal, st_.pstSh_->tridt.waveZAngle,
            st_.pstSh_->tridt.waveZAnglePhase, st_.pstSh_->tridt.waveZAngleRtTms, 
            st_.pstSh_->tridt.waveZType, st_.pstSh_->tridt.waveZPeakStop, st_.pstSh_->tridt.waveZPeakStop_f, st_.pstSh_->tridt.waveZPeakStop_c))
        {
            st_.pstSh_->tridt.waveZAngle = st_.pstSh_->tridt.waveZAngle + 1;//毎時変化波角度を追加（関数先で単位角度と掛け合わせて実際の値となる）
            st_.pstSh_->tridt.waveZWChangeCount = st_.pstSh_->tridt.waveZWChangeCount + 1;
        }
        //現在の週回数を波タイプ数最大値で割ったあまりを波タイプカウントに入れる
        st_.pstSh_->crt.orb.waveZ.wvtypeNo_c = (st_.pstSh_->tridt.waveZAngleRtTms % st_.pstSh_->crt.orb.waveZ.wvtypeNoMax);
    }

    //波分割処理&頂点停止処理
    BOOL TBaShot_Tri::CheckAddWaveAngle(s32 div, BOOL& div_f, s32& div_c, s32& divTime, f32 nowVal, f32 pstVal, s32 waveAngle,
        s32& wavePhase, s32& waveRt, s32 waveType, s32 peakStop, BOOL& peakStop_f, s32& peakStop_c)
    {
        BOOL res = TRUE;
        //波の状態・象限数を調べる用*********************************************
        s32 wvphs = 0;//仮の象限数
        BOOL ph_f = FALSE;//象限チェンジフラグ
        BOOL rt_f = FALSE;//象限一周フラグ
        //現在と過去の角度
        f32 nowAgl_All = nowVal;
        f32 pstAgl_All = pstVal;
        //現在と過去の角度のうちの２π以上の部分
        s32 nowAgl_Round = nowAgl_All / (2.0f * PI);
        s32 pstAgl_Round = pstAgl_All / (2.0f * PI);
        //現在と過去の角度のうちの２π未満の部分
        f32 nowAgl = nowAgl_All - (f32)nowAgl_Round * 2.0f * PI;
        f32 pstAgl = pstAgl_All - (f32)pstAgl_Round * 2.0f * PI;
        //現在と過去の象限数
        s32 nowQdrt = GetQdrtDt(nowAgl, waveType);
        s32 pstQdrt = GetQdrtDt(pstAgl, waveType);
        //***********************************************************************

        //divが０なら加算OK
        if ((div < 1) && (peakStop < 1))
        {
            //象限数のチェック
        //    CheckQdrtDt(nowQdrt, pstQdrt, wavePhase, waveRt, ph_f);
            res = TRUE;
        }

        //div中は時間経過でスイッチングを判断
        if (div_f == TRUE)
        {
            if (div_c >= divTime)
            {//時間をカウンタが超えたら
                div_f = FALSE;//div終了
                div_c = 0;//カウンタを０に戻す
                res =  TRUE;//加算開始
            }
            else
            {//div中内なら加算しない
                div_c = div_c + 1;//カウンタ加算
                res =  FALSE;//加算しない
            }
        }
        else//peakStop中は時間経過でスイッチングを判断
        if (peakStop_f == TRUE)
        {
            if (peakStop_c <= 0)
            {//時間をカウンタが超えたら
                peakStop_f = FALSE;//div終了
                res =  TRUE;//加算開始
            }
            else
            {//peakStop中内なら加算しない
                peakStop_c = peakStop_c - 1;//カウンタ加算
                res =  FALSE;//加算しない
            }
        }
        else
        {//div・peakStop中でない

            //象限数チェンジ…カウンタ０のときはやらん
            if ((div_c != 0) || (peakStop_c == 0)){
                //初期角度では必ず象限数を変えてしまうので初期＋１の角度移行で象限数を監視する
                if (waveAngle > 0) CheckQdrtDt(nowQdrt, pstQdrt, wavePhase, waveRt, ph_f);
            }

            //象限数が変わったときに各種処理が必要かどうかをジャッジし、必要ならスタートするよう処理をする
            //●波分割
            //総合象限数（回転数*４象限＋現在象限数）を分割数で割って余りが０の時分割状態へ
            if (div > 0)
            {
                if ((((waveRt * 4) + wavePhase) % div == 0) && (div_c != 0) && (ph_f == TRUE))
                {
                    div_f = TRUE;//分割状態
                    divTime = div_c;//分割時間を分割しない時間と同じにする
                    div_c = 0;//カウンタを０に戻す
                    res =  FALSE;//加算しない
                }
                else
                {
                    div_c = div_c + 1;//カウンタ加算
                    res =  TRUE;
                }
            }

            //●波頂点停止
            //peakStop中ではないとき
            if ((peakStop > 0) && (peakStop_f == FALSE) && (peakStop_c == 0) && (ph_f == TRUE))
            {
                const s32 STOP_TIME = 4;

                if (GetPeakStop(peakStop, nowQdrt, pstQdrt) == TRUE)
                {//時間をカウンタが超えたら
                    peakStop_f = TRUE;//頂点停止開始
                    peakStop_c = STOP_TIME;//カウンタ
                    res =  FALSE;//加算しない
                }
                else res =  TRUE;//加算する
            }
        }
        return res;
    }

    //象限数が変わるかのチェック
    void TBaShot_Tri::CheckQdrtDt(s32 nowQdrt, s32 pstQdrt , s32& wavePhase, s32& waveRt, BOOL& ph_f)
    {
        //切り替わる場合
        if ((nowQdrt != pstQdrt))
        {
            //象限数をあげる
            wavePhase = wavePhase + 1;
            ph_f = TRUE;
            if (wavePhase > 3)
            {//第４象限を超えたら
                wavePhase = 0;//第一象限に戻し
                waveRt = waveRt + 1;//一周回って加算
            }
        }
    }

    //象限数を求める（Quadrant）
    s32 TBaShot_Tri::GetQdrtDt(f32 angleDt, s32 waveType)
    {
        const f32 CIR_PHS_1 = 1.0f * PI / 2.0f;//第一象限(CirclePhase)終
        const f32 CIR_PHS_2 = 2.0f * PI / 2.0f;//第二象限(CirclePhase)終
        const f32 CIR_PHS_3 = 3.0f * PI / 2.0f;//第三象限(CirclePhase)終
        const f32 CIR_PHS_4 = 4.0f * PI / 2.0f;//第四象限(CirclePhase)終
        s32 res = 0;

        //角度０～９０度
        if (((0 <= angleDt) && (angleDt < CIR_PHS_1)) || (angleDt == CIR_PHS_4)){
            switch (waveType % 4){//４で割った余り(1：+sin、2：-sin、3：+cos、0：-cos)
            case 1: res = 0; break;
            case 2: res = 2; break;
            case 3: res = 1; break;
            default: res = 3; break;
            }
        }
        // 角度９０～１８０度
        else if ((CIR_PHS_1 <= angleDt) && (angleDt < CIR_PHS_2)) {
            switch (waveType % 4){//４で割った余り(1：+sin、2：-sin、3：+cos、0：-cos)
            case 1: res = 1; break;
            case 2: res = 3; break;
            case 3: res = 2; break;
            default: res = 0; break;
            }
        }
        // 角度１８０～２７０度
        else if ((CIR_PHS_2 <= angleDt) && (angleDt < CIR_PHS_3)){
            switch (waveType % 4){//４で割った余り(1：+sin、2：-sin、3：+cos、0：-cos)
            case 1: res = 2; break;
            case 2: res = 0; break;
            case 3: res = 3; break;
            default: res = 1; break;
            }
        }
        else {// 角度２７０～３６０度
            switch (waveType % 4){//４で割った余り(1：+sin、2：-sin、3：+cos、0：-cos)
            case 1: res = 3; break;
            case 2: res = 1; break;
            case 3: res = 0; break;
            default: res = 2; break;
            }
        }
        return res;
    }

    //頂点停止ONOFFチェック
    BOOL TBaShot_Tri::GetPeakStop(s32 peakStopType,s32 nowQdrt,s32 pstQdrt)
    {
        BOOL res = FALSE;
        //頂点停止の場合
        if ((peakStopType == 1) || (peakStopType == 3))
        {//第１象限→第２象限　or　第３象限→第４象限移行時
            if (((nowQdrt == 1) && (pstQdrt == 0)) || ((nowQdrt == 3) && (pstQdrt == 2))) res = TRUE;
        }

        //腹停止の場合
        if ((peakStopType == 2) || (peakStopType == 3))
        {//第２象限→第３象限　or　第４象限→第１象限移行時
            if (((nowQdrt == 2) && (pstQdrt == 1)) || ((nowQdrt == 0) && (pstQdrt == 3))) res = TRUE;
        }
        return res;
    }
    //SAngle再計算
    void TBaShot_Tri::ReSAngle()
    {

        st_.pstSh_->phs.spdX = (st_.pstSh_->phs.spd - 1.0f) * st_.pstSh_->phs.vecX * st_.pstSh_->phs.dirX;
        //反れ縦・サイド比率の絶対値を取ることでxの符号による不具合を防止する
        st_.pstSh_->phs.spdY = (st_.pstSh_->phs.spd - 1.0f) * st_.pstSh_->phs.spdRtXY * st_.pstSh_->phs.dirY;//反れ縦絶対値に方向を掛ける
        st_.pstSh_->phs.spdZ = (st_.pstSh_->phs.spd - 1.0f) * st_.pstSh_->phs.spdRtXZ * st_.pstSh_->phs.dirZ;//反れサイド絶対値に方向を掛ける
    }




    //三角関数が関係ない場合の処理
    //仮想位置を表示位置とする
    void TBaShot_Tri::DispPosForNoTriFunction()
    {
        st_.pstSh_->tridt.dispPosX = st_.pstSh_->tridt.shotPosX;//実際の表示位置x＝ボール位置x（仮想位置）
        st_.pstSh_->tridt.dispPosY = st_.pstSh_->tridt.shotPosY;//実際の表示位置y＝ボール位置y（仮想位置）
        st_.pstSh_->tridt.dispPosZ = st_.pstSh_->tridt.shotPosZ;//実際の表示位置z＝ボール位置z（仮想位置）
    }

    //三角関数系処理フラグ
    void TBaShot_Tri::CheckTriFlag()
    {
        //波３方向フラグ
        if (st_.pstSh_->crt.orb.waveX.wvtype > 0) st_.pstSh_->tridt.waveX_f = TRUE;
        if (st_.pstSh_->crt.orb.waveY.wvtype > 0) st_.pstSh_->tridt.waveY_f = TRUE;
        if (st_.pstSh_->crt.orb.waveZ.wvtype > 0) st_.pstSh_->tridt.waveZ_f = TRUE;

        //どれか一つでもフラグが立っていたら三角関数フラグをオンにする
        if ((st_.pstSh_->tridt.waveX_f == TRUE) || (st_.pstSh_->tridt.waveY_f == TRUE) || (st_.pstSh_->tridt.waveZ_f == TRUE))
            st_.pstSh_->phs.triFunc_f = TRUE;//三角関数フラグ
    }

    //セットデータ：波X
    void TBaShot_Tri::SetWaveXData()
    {
        //波前後
        st_.pstSh_->tridt.waveXType = st_.pstSh_->crt.orb.waveX.wvtype;//タイプ：sin・cos、ギザ・曲線
        st_.pstSh_->tridt.waveXLenLv = st_.pstSh_->crt.orb.waveX.freq;//波長Lv　５段階
        st_.pstSh_->tridt.waveXWidLv = st_.pstSh_->crt.orb.waveX.width;//振幅Lv　５段階
        st_.pstSh_->tridt.waveXWChangeType = st_.pstSh_->crt.orb.waveX.widthChg_Mtd;//振幅変化タイプ　減衰・減増減…・増減増…・増幅
        st_.pstSh_->tridt.waveXWChangeSpeed = st_.pstSh_->crt.orb.waveX.widthChg_Spd;//振幅変化速度　５段階
        st_.pstSh_->tridt.waveXPeakStop = st_.pstSh_->crt.orb.waveX.peakStop;//頂点停止
        st_.pstSh_->tridt.waveXPeakStop_f = st_.pstSh_->crt.orb.waveX.peakStop_f;//頂点停止フラグ
        st_.pstSh_->tridt.waveXPeakStop_c = st_.pstSh_->crt.orb.waveX.peakStopTime_c;//頂点停止時間
        st_.pstSh_->tridt.waveXAngle = st_.pstSh_->crt.orb.waveX.angle;//掛振動中の現角度
        st_.pstSh_->tridt.waveXWChangeCount = st_.pstSh_->crt.orb.waveX.widthChg_c;//掛振幅変化加算値
        st_.pstSh_->tridt.waveXZigAdd = st_.pstSh_->crt.orb.waveX.zigAdd;//掛ジグザグ時の移動量
        st_.pstSh_->tridt.waveXDiv = st_.pstSh_->crt.orb.waveX.wvDiv;//波分割
        st_.pstSh_->tridt.waveXDiv_f = st_.pstSh_->phs.waveXDiv_f;//波分割中フラグ
        st_.pstSh_->tridt.waveXDiv_c = st_.pstSh_->phs.waveXDiv_c;//分割中時間カウント
        st_.pstSh_->tridt.waveXDivTime = st_.pstSh_->phs.waveXDivTime;//分割中時間
        st_.pstSh_->tridt.waveXAnglePhase = st_.pstSh_->crt.orb.waveX.anglePhase;//象限数：第１象限～第４象限
        st_.pstSh_->tridt.waveXAngleRtTms = st_.pstSh_->crt.orb.waveX.angleRtTms;//何周したか
    }


    //セットデータ：波Y
    void TBaShot_Tri::SetWaveYData()
    {
        //波縦
        st_.pstSh_->tridt.waveYType = st_.pstSh_->crt.orb.waveY.wvtype;//タイプ：sin・cos、ギザ・曲線
        st_.pstSh_->tridt.waveYLenLv = st_.pstSh_->crt.orb.waveY.freq;//波長Lv　５段階
        st_.pstSh_->tridt.waveYWidLv = st_.pstSh_->crt.orb.waveY.width;//振幅Lv　５段階
        st_.pstSh_->tridt.waveYWChangeType = st_.pstSh_->crt.orb.waveY.widthChg_Mtd;//振幅変化タイプ　減衰・減増減…・増減増…・増幅
        st_.pstSh_->tridt.waveYWChangeSpeed = st_.pstSh_->crt.orb.waveY.widthChg_Spd;//振幅変化速度　５段階
        st_.pstSh_->tridt.waveYPeakStop = st_.pstSh_->crt.orb.waveY.peakStop;//頂点停止フラグ
        st_.pstSh_->tridt.waveYPeakStop_f = st_.pstSh_->crt.orb.waveY.peakStop_f;//頂点停止フラグ
        st_.pstSh_->tridt.waveYPeakStop_c = st_.pstSh_->crt.orb.waveY.peakStopTime_c;//頂点停止時間

        st_.pstSh_->tridt.waveYAngle = st_.pstSh_->crt.orb.waveY.angle;//掛振動中の現角度
        st_.pstSh_->tridt.waveYWChangeCount = st_.pstSh_->crt.orb.waveY.widthChg_c;//掛振幅変化加算値
        st_.pstSh_->tridt.waveYZigAdd = st_.pstSh_->crt.orb.waveY.zigAdd;//掛ジグザグ時の移動量
        st_.pstSh_->tridt.waveYDiv = st_.pstSh_->crt.orb.waveY.wvDiv;//波分割
        st_.pstSh_->tridt.waveYDiv_f = st_.pstSh_->phs.waveYDiv_f;//波分割中フラグ
        st_.pstSh_->tridt.waveYDiv_c = st_.pstSh_->phs.waveYDiv_c;//分割中時間カウント
        st_.pstSh_->tridt.waveYDivTime = st_.pstSh_->phs.waveYDivTime;//分割中時間
        st_.pstSh_->tridt.waveYAnglePhase = st_.pstSh_->crt.orb.waveY.anglePhase;//象限数：第１象限～第４象限
        st_.pstSh_->tridt.waveYAngleRtTms = st_.pstSh_->crt.orb.waveY.angleRtTms;//何周したか
    }

    //セットデータ：波Z
    void TBaShot_Tri::SetWaveZData()
    {
        //波サイド
        st_.pstSh_->tridt.waveZType = st_.pstSh_->crt.orb.waveZ.wvtype;//タイプ：sin・cos、ギザ・曲線
        st_.pstSh_->tridt.waveZLenLv = st_.pstSh_->crt.orb.waveZ.freq;//波長Lv　５段階
        st_.pstSh_->tridt.waveZWidLv = st_.pstSh_->crt.orb.waveZ.width;//振幅Lv　５段階
        st_.pstSh_->tridt.waveZWChangeType = st_.pstSh_->crt.orb.waveZ.widthChg_Mtd;//振幅変化タイプ　減衰・減増減…・増減増…・増幅
        st_.pstSh_->tridt.waveZWChangeSpeed = st_.pstSh_->crt.orb.waveZ.widthChg_Spd;//振幅変化速度　５段階
        st_.pstSh_->tridt.waveZPeakStop = st_.pstSh_->crt.orb.waveZ.peakStop;//頂点停止フラグ
        st_.pstSh_->tridt.waveZPeakStop_f = st_.pstSh_->crt.orb.waveZ.peakStop_f;//頂点停止フラグ
        st_.pstSh_->tridt.waveZPeakStop_c = st_.pstSh_->crt.orb.waveZ.peakStopTime_c;//頂点停止時間
        st_.pstSh_->tridt.waveZAngle = st_.pstSh_->crt.orb.waveZ.angle;//掛振動中の現角度
        st_.pstSh_->tridt.waveZWChangeCount = st_.pstSh_->crt.orb.waveZ.widthChg_c;//掛振幅変化加算値
        st_.pstSh_->tridt.waveZZigAdd = st_.pstSh_->crt.orb.waveZ.zigAdd;//掛ジグザグ時の移動量
        st_.pstSh_->tridt.waveZDiv = st_.pstSh_->crt.orb.waveZ.wvDiv;//波分割
        st_.pstSh_->tridt.waveZDiv_f = st_.pstSh_->phs.waveZDiv_f;//波分割中フラグ
        st_.pstSh_->tridt.waveZDiv_c = st_.pstSh_->phs.waveZDiv_c;//分割中時間カウント
        st_.pstSh_->tridt.waveZDivTime = st_.pstSh_->phs.waveZDivTime;//分割中時間
        st_.pstSh_->tridt.waveZAnglePhase = st_.pstSh_->crt.orb.waveZ.anglePhase;//象限数：第１象限～第４象限
        st_.pstSh_->tridt.waveZAngleRtTms = st_.pstSh_->crt.orb.waveZ.angleRtTms;//何周したか
    }


    //セットデータ：基本情報…三角関数未使用時にも使用する
    void TBaShot_Tri::SetBasicData()
    {
        st_.pstSh_->tridt.shotPosX = st_.pstSh_->phs.posX;//ボール位置x（仮想位置）
        st_.pstSh_->tridt.shotPosY = st_.pstSh_->phs.posY;//ボール位置y（仮想位置）
        st_.pstSh_->tridt.shotPosZ = st_.pstSh_->phs.posZ;//ボール位置z（仮想位置）
        st_.pstSh_->tridt.dispPosX = st_.pstSh_->phs.dispBallX;//実際の表示位置x この値を使わず更新されるけど一応
        st_.pstSh_->tridt.dispPosY = st_.pstSh_->phs.dispBallY;//実際の表示位置y この値を使わず更新されるけど一応
        st_.pstSh_->tridt.dispPosZ = st_.pstSh_->phs.dispBallZ;//実際の表示位置z この値を使わず更新されるけど一応
    }
    //全体初期化：st_.pstSh_->tridtに情報を入れる
    void TBaShot_Tri::SetTriData()
    {

        //基本情報
        SetBasicData();
        st_.pstSh_->tridt.shotPosX = st_.pstSh_->phs.realX;//ボール位置x（仮想位置）
        st_.pstSh_->tridt.shotPosY = st_.pstSh_->phs.realY;//ボール位置y（仮想位置）
        st_.pstSh_->tridt.shotPosZ = st_.pstSh_->phs.realZ;//ボール位置z（仮想位置）
        //前情報の保存
        st_.pstSh_->phs.addXPr = st_.pstSh_->phs.addX;//ボール位置x（仮想位置）
        st_.pstSh_->phs.addYPr = st_.pstSh_->phs.addY;//ボール位置y（仮想位置）
        st_.pstSh_->phs.addZPr = st_.pstSh_->phs.addZ;//ボール位置z（仮想位置）

        st_.pstSh_->phs.addX = 0;//ボール位置x（仮想位置）
        st_.pstSh_->phs.addY = 0;//ボール位置y（仮想位置）
        st_.pstSh_->phs.addZ = 0;//ボール位置z（仮想位置）

        //波データ
        if (st_.pstSh_->tridt.waveX_f == TRUE) SetWaveXData();
        if (st_.pstSh_->tridt.waveY_f == TRUE) SetWaveYData();
        if (st_.pstSh_->tridt.waveZ_f == TRUE) SetWaveZData();
    }

    //計算結果から値を取得
    void TBaShot_Tri::GetTriData()
    {
        //基本情報
        st_.pstSh_->phs.addX = st_.pstSh_->tridt.shotPosX;//ボール位置x（仮想位置）
        st_.pstSh_->phs.addY = st_.pstSh_->tridt.shotPosY;//ボール位置y（仮想位置）st_.pstSh_->crt.orb.waveX.angle
        st_.pstSh_->phs.addZ = st_.pstSh_->tridt.shotPosZ;//ボール位置z（仮想位置）

        //波データ
        //波前後
        st_.pstSh_->crt.orb.waveX.angle = st_.pstSh_->tridt.waveXAngle;//掛振動中の現角度
        st_.pstSh_->crt.orb.waveX.widthChg_c = st_.pstSh_->tridt.waveXWChangeCount;//掛振幅変化加算値
        st_.pstSh_->phs.waveXDiv_f = st_.pstSh_->tridt.waveXDiv_f;//波分割中フラグ
        st_.pstSh_->phs.waveXDiv_c = st_.pstSh_->tridt.waveXDiv_c;//分割中時間カウント
        st_.pstSh_->phs.waveXDivTime = st_.pstSh_->tridt.waveXDivTime;//分割中時間
        st_.pstSh_->crt.orb.waveX.anglePhase = st_.pstSh_->tridt.waveXAnglePhase;//象限数：第１象限～第４象限
        st_.pstSh_->crt.orb.waveX.angleRtTms = st_.pstSh_->tridt.waveXAngleRtTms;//何周したか
        st_.pstSh_->crt.orb.waveX.peakStop = st_.pstSh_->tridt.waveXPeakStop;//頂点停止
        st_.pstSh_->crt.orb.waveX.peakStop_f = st_.pstSh_->tridt.waveXPeakStop_f;//頂点停止フラグ
        st_.pstSh_->crt.orb.waveX.peakStopTime_c = st_.pstSh_->tridt.waveXPeakStop_c;//頂点停止時間


        //波縦
        st_.pstSh_->crt.orb.waveY.angle = st_.pstSh_->tridt.waveYAngle;//掛振動中の現角度
        st_.pstSh_->crt.orb.waveY.widthChg_c = st_.pstSh_->tridt.waveYWChangeCount;//掛振幅変化加算値
        st_.pstSh_->phs.waveYDiv_f = st_.pstSh_->tridt.waveYDiv_f;//波分割中フラグ
        st_.pstSh_->phs.waveYDiv_c = st_.pstSh_->tridt.waveYDiv_c;//分割中時間カウント
        st_.pstSh_->phs.waveYDivTime = st_.pstSh_->tridt.waveYDivTime;//分割中時間
        st_.pstSh_->crt.orb.waveY.anglePhase = st_.pstSh_->tridt.waveYAnglePhase;//象限数：第１象限～第４象限
        st_.pstSh_->crt.orb.waveY.angleRtTms = st_.pstSh_->tridt.waveYAngleRtTms;//何周したか
        st_.pstSh_->crt.orb.waveY.peakStop = st_.pstSh_->tridt.waveYPeakStop;//頂点停止
        st_.pstSh_->crt.orb.waveY.peakStop_f = st_.pstSh_->tridt.waveYPeakStop_f;//頂点停止フラグ
        st_.pstSh_->crt.orb.waveY.peakStopTime_c = st_.pstSh_->tridt.waveYPeakStop_c;//頂点停止時間

        //波サイド
        st_.pstSh_->crt.orb.waveZ.angle = st_.pstSh_->tridt.waveZAngle;//掛振動中の現角度
        st_.pstSh_->crt.orb.waveZ.widthChg_c = st_.pstSh_->tridt.waveZWChangeCount;//掛振幅変化加算値
        st_.pstSh_->phs.waveZDiv_f = st_.pstSh_->tridt.waveZDiv_f;//波分割中フラグ
        st_.pstSh_->phs.waveZDiv_c = st_.pstSh_->tridt.waveZDiv_c;//分割中時間カウント
        st_.pstSh_->phs.waveZDivTime = st_.pstSh_->tridt.waveZDivTime;//分割中時間
        st_.pstSh_->crt.orb.waveZ.anglePhase = st_.pstSh_->tridt.waveZAnglePhase;//象限数：第１象限～第４象限
        st_.pstSh_->crt.orb.waveZ.angleRtTms = st_.pstSh_->tridt.waveZAngleRtTms;//何周したか
        st_.pstSh_->crt.orb.waveZ.peakStop = st_.pstSh_->tridt.waveZPeakStop;//頂点停止
        st_.pstSh_->crt.orb.waveZ.peakStop_f = st_.pstSh_->tridt.waveZPeakStop_f;//頂点停止フラグ
        st_.pstSh_->crt.orb.waveZ.peakStopTime_c = st_.pstSh_->tridt.waveZPeakStop_c;//頂点停止時間
    }

    //初期化関数（ほかから呼び出すため）
    void TBaShot_Tri::TriInit()
    {
        st_.pstSh_->tridt.Init();
    }

    //三角関数の計算…
    //反れ縦・反れサイドからy、z速度を算出したあとで処理。
    //２次関数処理もされていたらそちらの座標・速度を使用する
    //上記で計算された位置から振動するよう計算。
    //初期化で振幅・振動数と、初期角度を入力したら
    //あとは計算式に基づいて毎Fr位置の計算を行う
    void TBaShot_Tri::UpDate()
    {
        //三角関数が関わる素材全部をチェック
        CheckTriFlag();

        //三角関数の有無で処理を分ける
        if (st_.pstSh_->phs.triFunc_f == FALSE)
        {
            //三角関数関係ない場合は専用の関数で表示位置を決定
            //（仮想ボール位置を表示位置とする処理）
            SetBasicData();//基本データをセット
            DispPosForNoTriFunction();//三角関数不使用時はこの関数で表示位置を決定
        }
        else
            //以降三角関数アリ
        {
            if (st_.pstSh_->phs.triFuncInit_f == FALSE)
            {
                //まずゼロクリア
                st_.pstSh_->tridt.Init();
                //再度三角関数が関わる素材全部をチェック
                CheckTriFlag();
                //三角関数系データ初期化終了フラグ
                st_.pstSh_->phs.triFuncInit_f = TRUE;
            }

            SetTriData();//三角関数全体データセット

            //波３方向
            if (st_.pstSh_->cmn.reTriInitData_f == TRUE)
            {
                st_.pstSh_->tridt.waveXInit_f = FALSE;
                st_.pstSh_->tridt.waveYInit_f = FALSE;
                st_.pstSh_->tridt.waveZInit_f = FALSE;
                st_.pstSh_->cmn.reTriInitData_f = FALSE;
            }
            if ((st_.pstSh_->tridt.waveXType > 0) && (st_.pstSh_->tridt.waveX_f == TRUE)) WaveXOrbit();//波前後素材
            if ((st_.pstSh_->tridt.waveYType > 0) && (st_.pstSh_->tridt.waveY_f == TRUE)) WaveYOrbit();//波縦素材
            if ((st_.pstSh_->tridt.waveZType > 0) && (st_.pstSh_->tridt.waveZ_f == TRUE)) WaveZOrbit();//波サイド素材

            //計算結果から値を取得
            GetTriData();
        }

        // //計算結果から値を取得
        //GetTriData();

    }


}
