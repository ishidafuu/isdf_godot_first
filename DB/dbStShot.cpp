
#include "dbStShot.h"
#include "dbDtElement.h"
#include "dbDtBallAnime.h"

namespace db
{
    const f32 PI = 3.1415f;
    const s32 ELMTYPEDIV = 1000;
    const f32 HEAD = +8;//キャラ座標から頭上までの長さ
    const f32 MATA = -8;//キャラ座標から股までの長さ
    const f32 SIDE = 24;//キャラ座標からの手前奥判定幅
    const f32 LOWLEVEL = 14;//ボールの通常限界高さ（特に地上シュート）

    const f32 SPD_LOW_LIMIT = 0.15f;

    //Yメイン・Zメインの速度低下
    const f32 DEC_SPD = 1.0f / 8.0f;//速度低下ベース
    const f32 DEC_SPD_LV1 = DEC_SPD * 7.0f;//速度低下Lv1
    const f32 DEC_SPD_LV2 = DEC_SPD * 6.0f;//速度低下Lv2
    const f32 DEC_SPD_LV3 = DEC_SPD * 5.0f;//速度低下Lv3

    //多角処理　定点位置の設定
    const f32 MLTANGL_DIF_X = 64.0f;//センターからの差
    const f32 MLTANGL_HGT_Y = 72.0f;//高さ
    const f32 MLTANGL_DPT_Z = 72.0f;//奥行
    const f32 SL_IN = 2.0f;//サイドライン内側への内挿値（外野にキャッチされないように）
    const f32 SL_IN_SW = (f32)(DBCRT_FL / XYMAG) + SL_IN;//サイドライン手前　内野側
    const f32 SL_IN_DP = (f32)(DBCRT_BL / XYMAG) - SL_IN;//サイドライン奥　内野側


    enum enElmType
    {
        elt_None,
        elt_change,
        elt_effect,
        elt_shape,
        elt_orbit,
        elt_target,
        eltEND,
    };


    //TSortData---------------------------------------------------------------------------------
    void TSortData::SortElem(INTVECTOR2 nonesort_elms)
    {

        INTLIST  sortelm;
        sortelm.clear();

        //ソートおよび書き換え
        for (s32 i = 0; i < nonesort_elms.size(); ++i)
        {
            if (i == 0)//ベース
            {
                for (s32 i2 = 0; i2 < nonesort_elms[i].size(); ++i2)
                {
                    s32 atm = nonesort_elms[i][i2];
                    sortelm.push_back(atm);
                    CheckGroundElm(nonesort_elms[i][i2]);
                }
            }
            else if (i == 1)
            {
                for (s32 i2 = 0; i2 < nonesort_elms[i].size(); ++i2)
                {
                    s32 i3 = (nonesort_elms[i].size() - i2 - 1);
                    sortelm.push_front(nonesort_elms[i][i3]);//sortelmの戦闘に合成１回目の末尾からpushしていく
                    CheckGroundElm(nonesort_elms[i][i3]);
                }
            }
            else if (i == 2)
            {
                BOOL prt_f = (prt_init != 0);
                BOOL insert_f = FALSE;
                s32 changeNo = 0;
                for (INTLIST::iterator ite = sortelm.begin(); ite != sortelm.end(); ++ite)
                {
                    //１段階目終了の変化素材が見つかった
                    if ((*ite / ELMTYPEDIV) == elt_change)
                    {
                        ++changeNo;
                        //多角処理の素材が入ってきたらカウントをする
                        //                        if (*ite == 1029) mltAngle = mltAngle + 1;
                        //保護の間には入らない
                        if (prt_f && (prt_init <= changeNo) && (changeNo <= prt_term)) continue;

                        //その次の要素
                        INTLIST::iterator ite2 = ite;
                        ++ite2;
                        if (ite2 != sortelm.end())
                        {
                            for (s32 i2 = 0; i2 < nonesort_elms[i].size(); ++i2)
                            {
                                s32 i3 = (nonesort_elms[i].size() - i2 - 1);
                                ite2 = sortelm.insert(ite2, nonesort_elms[i][i3]);//１段階目終了の変化素材の次にインサートしていく
                                CheckGroundElm(nonesort_elms[i][i3]);
                            }
                            insert_f = TRUE;
                        }
                        break;
                    }
                }

                //１段階目しかなかった
                if (insert_f == FALSE)
                {
                    for (s32 i2 = 0; i2 < nonesort_elms[i].size(); ++i2)
                    {
                        s32 i3 = (nonesort_elms[i].size() - i2 - 1);
                        sortelm.push_front(nonesort_elms[i][i3]);//sortelmの戦闘に合成１回目の末尾からpushしていく
                        CheckGroundElm(nonesort_elms[i][i3]);
                    }
                }

            }
            else
            {
                //合成 ３ 回目以降の場合、ベース側 最終段階あたまに挿入素材を貼り付ける。
                for (s32 i2 = 0; i2 < nonesort_elms[i].size(); ++i2)
                {
                    sortelm.push_back(nonesort_elms[i][i2]);//sortelmの戦闘に合成１回目の末尾からpushしていく
                    CheckGroundElm(nonesort_elms[i][i2]);
                }
            }
        }

        // phase_elmsに段階分けして写す
        s32 phase = 0;
        INTVECTOR2 tmp_phase_elms;
        tmp_phase_elms.clear();

        INTVECTOR firstphase;
        firstphase.clear();
        tmp_phase_elms.push_back(firstphase);

        for (INTLIST::iterator ite = sortelm.begin(); ite != sortelm.end(); ++ite)
        {
            tmp_phase_elms[phase].push_back(*ite);

            if ((*ite / ELMTYPEDIV) == elt_change)
            {
                INTVECTOR newphase;
                newphase.clear();
                tmp_phase_elms.push_back(newphase);
                ++phase;
                phaseAll = phaseAll + 1;
            }
        }


        phase_elms.Init();
        phase_elms.linenum = tmp_phase_elms.size();
        for (s32 i2 = 0; i2 < tmp_phase_elms.size(); ++i2)
        {
            phase_elms.elmnum[i2] = tmp_phase_elms[i2].size();
            for (s32 i3 = 0; i3 < tmp_phase_elms[i2].size(); ++i3)
            {
                phase_elms.elms[i2][i3] = tmp_phase_elms[i2][i3];
            }
        }
    }

    //地上変化素材を持っているかのチェック
    void TSortData::CheckGroundElm(s32 elmNo)
    {
        switch (elmNo){
        case 1048:
        case 1012:
            groundElm_f = TRUE; break;
        default: break;
        }
    }

    //TStShot---------------------------------------------------------------------------------
    //(lv[i] == 0)をジャッジしたいので第4引数にNULLは入れない
    s32 SetPhaseLv(s32 nowphase, s32 addlv, s32* phasedt, s32* lv)
    {
        s32 res = 0;
        //ダブりの段階を探す
        for (s32 i = 0; i < EFMAX; ++i)
        {
            if (phasedt[i] == nowphase)
            {
                if (lv != NULL) lv[i] += addlv;
                res = i;
                return res;
            }
        }

        //ダブり無ければ末尾に情報を入れる

        for (s32 i = 0; i < EFMAX; ++i)
        {
            if ((phasedt[i] == 0) && (lv[i] == 0))//(lv[i] == 0)がないと”０”で更新できない
            {
                phasedt[i] = nowphase;
                if (lv != NULL) lv[i] = addlv;
                res = i;
                return res;
            }
        }

        return res;
    }
    void TStShot::SortElem_ShpEff(TDtElement* pdtElem)
    {
        BOOL transform_f = FALSE;
        //全段階
        for (s32 i = 0; i < sdata.phase_elms.linenum; ++i)
        {
            //段階ごとの素材
            for (s32 i2 = 0; i2 < sdata.phase_elms.elmnum[i]; ++i2)
            {
                s32 elmtype = (sdata.phase_elms.elms[i][i2] / ELMTYPEDIV);
                s32 elmid = (sdata.phase_elms.elms[i][i2] % ELMTYPEDIV);
                if (sdata.phase_elms.elms[i][i2] == 1029) cmn.mltAngle = cmn.mltAngle + 1;
                if (elmtype == elt_effect)
                {
                    s32 effno = pdtElem->GetEffectElmNo(elmid);
                    s32 lv = pdtElem->GetEffectDt(elmid, 1);//dt1がレベル
                    s32 setno = 0;
                    switch (effno)
                    {
                    case 1: pdtElem->SetEffectDt(elmid, &idata.eff.penet, &idata.eff.penetTime); break;//1:貫き効果
                    case 2: pdtElem->SetEffectDt(elmid, &idata.eff.suction); break;//2:吸引効果
                    case 3: pdtElem->SetEffectDt(elmid, &idata.eff.speedInc); break;//3:速度＋効果
                    case 4: pdtElem->SetEffectDt(elmid, &idata.eff.speedDec); break;//4:速度－効果
                    case 5: pdtElem->SetEffectDt(elmid, &idata.eff.compress); break;//5:攻撃＋判定－効果
                    case 6: pdtElem->SetEffectDt(elmid, &idata.eff.wEdge); break;//6:諸刃効果
                    case 7: pdtElem->SetEffectDt(elmid, &idata.eff.flyReverse); break;//7:引っ張り効果
                    case 8: pdtElem->SetEffectDt(elmid, &idata.eff.flyFire); break;//8:吹っ飛び火効果
                    case 9: pdtElem->SetEffectDt(elmid, &idata.eff.flyElec); break;//9:吹っ飛び電効果
                    case 10: pdtElem->SetEffectDt(elmid, &idata.eff.rollDown); break;//10:ゴロゴロダウン効果
                    case 11: pdtElem->SetEffectDt(elmid, &idata.eff.flyLoose); break;//11:吹っ飛び緩効果
                    case 12: pdtElem->SetEffectDt(elmid, &idata.eff.rndWorld); break;//12:世界一周効果
                    case 13://13:段階変形効果
                        pdtElem->SetEffectDt(elmid, &idata.eff.Transform);
                        transform_f = TRUE;
                        break;
                    case 14: pdtElem->SetEffectDt(elmid, &idata.eff.spinPlus); break;//14:スピン強化
                    case 15: pdtElem->SetEffectDt(elmid, &idata.eff.pass); break;//15:パス挙動効果
                    case 16: pdtElem->SetEffectDt(elmid, &idata.eff.finalSE); break;//16:最終段階ＳＥ効果
                    case 17: pdtElem->SetEffectDt(elmid, &idata.eff.waveKeep); break;//17:波持続効果
                    case 18: pdtElem->SetEffectDt(elmid, &idata.eff.catchSlip); break;//17:キャッチスリップ
                    case 19: pdtElem->SetEffectDt(elmid, &idata.eff.catchSlipR); break;//17:逆キャッチスリップ
                    case 20: pdtElem->SetEffectDt(elmid, &idata.eff.symCtrl); break;//17:マリオネット効果
                    case 101:SetPhaseLv(i, lv, idata.eff.accel_Phase, idata.eff.accel_Lv); break;//101:加速効果＿
                    case 102:SetPhaseLv(i, lv, idata.eff.decel_Phase, idata.eff.decel_Lv); break;//102:減速効果＿
                    case 103:SetPhaseLv(i, lv, idata.eff.yAccel_Phase, idata.eff.yAccel_Lv); break;//103:y加速効果＿
                    case 104:SetPhaseLv(i, lv, idata.eff.spdChg_Phase, idata.eff.spdChg_Lv); break;//104:速度可変効果
                    case 105:SetPhaseLv(i, lv, idata.eff.trace_Phase, idata.eff.trace_Lv); break;//105:追跡効果＿
                    case 106:SetPhaseLv(i, lv, idata.eff.reflect_Phase, idata.eff.reflect_Lv); break;//106:強反射効果＿
                    case 107:SetPhaseLv(i, lv, idata.eff.mySide_Phase, idata.eff.mySide_Lv); break;//107:味方被弾効果
                    case 108:SetPhaseLv(i, lv, idata.eff.normal_Phase, idata.eff.normal_Lv); break; //108:通常球化効果
                    case 109:SetPhaseLv(i, lv, idata.eff.flyVer_Phase, idata.eff.flyVer_Lv); break; //109:真上吹っ飛び効果
                    case 110:SetPhaseLv(i, lv, idata.eff.flyHor_Phase, idata.eff.flyHor_Lv); break; //110:真横吹っ飛び効果＿
                    case 111:SetPhaseLv(i, lv, idata.eff.press_Phase, idata.eff.press_Lv); break;//111:圧殺効果
                    case 112:SetPhaseLv(i, lv, idata.eff.copy_Phase, idata.eff.copy_Lv); break;//112:複写効果
                    case 125:SetPhaseLv(i, lv, idata.eff.wEdge_Phase, idata.eff.wEdge_Lv); break;//125:諸刃効果（段階指定）
                    case 127:SetPhaseLv(i, lv, idata.eff.tagChange_Phase, idata.eff.tagChange_Lv); break;//127:標的変化効果（段階指定）
                    case 128:SetPhaseLv(i, lv, idata.eff.spdMomUp_Phase, idata.eff.spdMomUp_Lv); break;//128:一時速度上昇効果（段階指定）
                    case 129:SetPhaseLv(i, lv, idata.eff.spdMomDown_Phase, idata.eff.spdMomDown_Lv); break;//129:一時速度減少効果（段階指定）
                    case 130:SetPhaseLv(i, lv, idata.eff.noDmg_Phase, idata.eff.noDmg_Lv); break;//130:ダメージナシ効果（段階指定）
                    case 131:SetPhaseLv(i, lv, idata.eff.phantom_Phase, idata.eff.phantom_Lv); break;//130:山田効果（段階指定）
                    case 113://113:その場ダウン効果
                        setno = SetPhaseLv(i, lv, idata.eff.noMvDwn_Phase, idata.eff.noMvDwn_Lv);
                        pdtElem->SetEffectDt(elmid, NULL, &idata.eff.noMvDwn_Type[setno]);
                        break;
                    case 114://114:同一標的連続被弾効果
                        setno = SetPhaseLv(i, lv, idata.eff.combo_Phase, idata.eff.combo_Lv);
                        pdtElem->SetEffectDt3(elmid, &idata.eff.combo_Max[setno], &idata.eff.combo_Time[setno], &idata.eff.combo_Dam[setno]);
                        break;
                    case 115://115:拍効果
                        setno = SetPhaseLv(i, lv, idata.eff.pulse_Phase, idata.eff.pulse_Lv);
                        pdtElem->SetEffectDt2(elmid, &idata.eff.pulse_Type[setno], &idata.eff.pulse_TmStop[setno], &idata.eff.pulse_TmStrt[setno]);
                        break;
                    case 116://116:効果音段階変化時
                        setno = SetPhaseLv(i, lv, idata.eff.sePhase_Phase, idata.eff.sePhase_Num);
                        pdtElem->SetEffectDt2(elmid, &idata.eff.sePhase_Num[setno], 0, 0);
                        break;
                    case 117: //117:効果音被弾時
                        setno = SetPhaseLv(i, lv, idata.eff.seHit_Phase, idata.eff.seHit_NumNrml);
                        pdtElem->SetEffectDt3(elmid, &idata.eff.seHit_NumFrnt[setno], &idata.eff.seHit_NumBack[setno], &idata.eff.seHit_NumNrml[setno]);
                        break;
                    case 118: SetPhaseLv(i, lv, idata.eff.orbPaint_Phase, NULL); break;//118:軌跡描画効果
                    case 119: SetPhaseLv(i, lv, idata.eff.orbErase_Phase, NULL); break;//119:軌跡消失効果
                    case 120: SetPhaseLv(i, lv, idata.eff.effClear_Phase, NULL); break;//120:効果クリア
                    case 121: SetPhaseLv(i, lv, idata.eff.end_Phase, NULL); break;//121:シュート終了効果
                        //                    case 122: SetPhaseLv(i, lv, idata.eff.refVer_Phase, idata.eff.refVer_Lv); break; //一時単球化効果
                    case 123: SetPhaseLv(i, lv, idata.eff.refVer_Phase, idata.eff.refVer_Lv); break; //109:縦跳吹っ飛び効果
                    case 124: SetPhaseLv(i, lv, idata.eff.refHor_Phase, idata.eff.refHor_Lv); break; //110:横跳吹っ飛び効果＿
                    case 126: SetPhaseLv(i, lv, idata.eff.sprtOff_Phase, idata.eff.sprtOff_Lv); break;//121:分裂シュート終了効果
                    default:
                        break;
                    }
                }
                else  if (elmtype == elt_shape)
                {
                    //ぶんれつ系素材は別個処理 分裂系開始 (ssl_sinFqX + SEPARATE_START_NO) ～ 分裂系終了(ssl_NUMMAX + SEPARATE_START_NO)
                    //150320 やまだが入ってきたのでssl_NUMMAX - 1とする
                    //                    if ((elmid >= (ssl_sinFqX + SEPARATE_START_NO)) && (elmid < (ssl_NUMMAX + SEPARATE_START_NO)))
                    if ((elmid >= (ssl_sinFqX + SEPARATE_START_NO)) && (elmid < (ssl_NUMMAX - 1 + SEPARATE_START_NO)))
                    {
                        enShapeSeparateLevel no = ssl_sinFqX;
                        switch (elmid)
                        {
                        case 77: no = ssl_sinFqX; break;//単振X
                        case 78: no = ssl_sinFqY; break;//単振Y
                        case 79: no = ssl_sinFqZ; break;//単振Z
                        case 80: no = ssl_mulFqX; break;//複振X
                        case 81: no = ssl_mulFqY; break;//複振Y
                        case 82: no = ssl_mulFqZ; break;//複振Z
                        case 83: no = ssl_shadow; break;//追跡
                        case 84: no = ssl_crclYZ; break;//円周YZ
                        case 85: no = ssl_crclXZ; break;//円周XZ
                        case 86: no = ssl_crclXY; break;//円周XY
                        case 87: no = ssl_cr8VXY; break;//８周XY
                        case 88: no = ssl_cr8VXZ; break;//８周XZ
                        case 89: no = ssl_cr8VYZ; break;//８周YZ
                        case 90: no = ssl_cr8HXY; break;//∞周XY
                        case 91: no = ssl_cr8HXZ; break;//∞周XZ
                        case 92: no = ssl_cr8HYZ; break;//∞周YZ
                        }
                        if (idata.sprtMain == 0) idata.sprtMain = (s32)no + 1;//メインの分裂を探る
                        idata.sprtDt[(s32)no].lv = idata.sprtDt[(s32)no].lv + 1;//指定の配列にレベルを追加
                        idata.sprtDt[(s32)no].use_f = TRUE;//指定の配列にレベルを追加
                        idata.sprt_f = TRUE;//ぶんれつ系フラグをON
                        //高速分裂玉以外に分裂玉があった場合
                        if (idata.sprtMain != 0)
                        {
                            //メイン分裂玉に高速分裂Lvを渡していない場合
                            if (idata.sprtQuickLvGave_f == FALSE)
                            {//Lvを渡して、次回は高速分裂Lvアップ時＋１へ
                                idata.sprtDt[idata.sprtMain - 1].lv = idata.sprtDt[idata.sprtMain - 1].lv + idata.sprtQuickLv;
                                idata.sprtQuickLvGave_f = TRUE;
                                //idata.sprtDt[ssl_sinFqY].lv = 0;
                                //Y分裂フラグが無いときはY分裂をオフにする
                                if (idata.sprtDt[ssl_sinFqY].use_f == FALSE) idata.sprtDt[ssl_sinFqY].lv = 0;
                                //idata.sprtQuickLv = 0;
                            }
                        }
                    }
                    else if (elmid == 93)//高速分裂
                    {
                        //高速分裂速度１Up
                        idata.sprtQuickLv = idata.sprtQuickLv + 1;
                        BOOL maxLv_f = FALSE;//最大レベルフラグ
                        //最大レベルを超えたら最大レベルに戻しフラグを立て、分裂数アップを阻害
                        if (idata.sprtQuickLv > 3) idata.sprtQuickLv = 3, maxLv_f = TRUE;
                        idata.sprt_f = TRUE;//ぶんれつ系フラグをON

                        //高速分裂玉以外に分裂玉があった場合
                        if (idata.sprtMain != 0)
                        {
                            //メイン分裂玉に高速分裂Lvを渡していない場合
                            if (idata.sprtQuickLvGave_f == FALSE)
                            {//Lvを渡して、次回は高速分裂Lvアップ時＋１へ
                                idata.sprtDt[idata.sprtMain - 1].lv = idata.sprtDt[idata.sprtMain - 1].lv + idata.sprtQuickLv;
                                idata.sprtQuickLvGave_f = TRUE;
                                if (idata.sprtDt[ssl_sinFqY].use_f == FALSE) idata.sprtDt[ssl_sinFqY].lv = 0;
                                //idata.sprtDt[ssl_sinFqY].lv = 0;
                            }
                            else
                            {//上限を超えてなければ速度アップとともに分裂玉UP
                                if (maxLv_f == FALSE) idata.sprtDt[idata.sprtMain - 1].lv = idata.sprtDt[idata.sprtMain - 1].lv + 1;
                            }
                        }//メイン分裂が無い場合は速度アップするが、分裂玉数は２つのまま
                        else idata.sprtDt[ssl_sinFqY].lv = 1;

                    }
                    else
                        if (transform_f)
                        {
                            transform_f = FALSE;
                            for (s32 i = 0; i < ELMSHPNUM; ++i)
                            {
                                if (idata.shptf[i].id == elmid)
                                {
                                    idata.shptf[i].lv += 1;
                                    idata.shptf[i].no = idata.shpNo;//同段階内形状変化する段階を保存
                                    idata.shp[idata.shpNo].tf_f = TRUE;//その元の形状データに同段変形するフラグを立てる
                                    break;
                                }
                                else if (idata.shptf[i].id == 0)
                                {
                                    idata.shptf[i].id = elmid;
                                    idata.shptf[i].lv = 1;
                                    idata.shptf[i].no = idata.shpNo;//同段階内形状変化する段階を保存
                                    idata.shp[idata.shpNo].tf_f = TRUE;//その元の形状データに同段変形するフラグを立てる
                                    break;
                                }
                            }
                        }
                        else
                        {
                            for (s32 i = 0; i < ELMSHPNUM; ++i)
                            {
                                if (idata.shp[i].id == elmid)
                                {
                                    idata.shp[i].lv += 1;
                                    idata.shpNo = i;//現在の要素数を保存
                                    break;
                                }
                                else if (idata.shp[i].id == 0)
                                {
                                    idata.shp[i].id = elmid;
                                    idata.shp[i].lv = 1;
                                    idata.shpNo = i;//現在の要素数を保存
                                    break;
                                }
                            }
                        }
                }
            }
        }

    }
    //現段階のsdataからElmに移動
    void TStShot::RemoveSdataToElm(TDtElement* pdtElem)
    {
        //elm初期化
        elm.Init();
        //放物線・波軌道のフラグにもなるので以下を念のためここでもクリア
        elm.orb.palab.point = 0;
        elm.orb.waveX.wvtype = 0;
        elm.orb.waveY.wvtype = 0;
        elm.orb.waveZ.wvtype = 0;

        //現段階
        s32 nowphase = cmn.phaseCrt;
        cmn.phaseAll = sdata.phaseAll;//ソートデータでカウントした全段階数をcmnのデータに入れる

        //サイドカーブフラグ　初期化
        for (s32 k = 0; k < sdj_END; ++k)
        {
            phs.sideCurveDt[k] = FALSE;
        }

        //   cmn.mltAngle = sdata.mltAngle;
        //現段階の素材
        for (s32 i2 = 0; i2 < sdata.phase_elms.elmnum[nowphase]; ++i2)
        {
            s32 elmtype = (sdata.phase_elms.elms[nowphase][i2] / ELMTYPEDIV);
            s32 elmid = (sdata.phase_elms.elms[nowphase][i2] % ELMTYPEDIV);

            SideCurveCheck(sdata.phase_elms.elms[nowphase][i2]);

#ifdef __K_DEBUG_SHIAI__
            // 原子のデータをログに書き込む
            kdebug::DebugSystem::GetInstance()->AddShootDebugLogElementAtm(pdtElem, nowphase, sdata.phase_elms.elms[nowphase][i2]);
#endif	// #ifdef __K_DEBUG_SHIAI__
            if (elmtype == elt_change)
            {
                s32 chgno = pdtElem->GetChangeElmNo(elmid);

                switch (chgno)
                {
                case 1: //変化素材：01時間 時間（取得時に×４してこの変数に入れる）
                    pdtElem->SetChangeDt(elmid, &elm.chg.time);
                    elm.chg.time *= 4;
                    break;
                case 2:  pdtElem->SetChangeDt(elmid, &elm.chg.hit); break;//変化素材：02ヒット フラグ
                case 3:  pdtElem->SetChangeDt(elmid, &elm.chg.height); break;//変化素材：03高さ ドット数【ボール位置y】
                case 4:  pdtElem->SetChangeDt(elmid, &elm.chg.ground); break;//変化素材：04地上 地上到達時フラグ
                case 5:  pdtElem->SetChangeDt(elmid, &elm.chg.pos); break; //変化素材：05位置 フラグ条件０～６、３は標的素材側で座標設定【ボール位置】
                case 6:  pdtElem->SetChangeDt(elmid, &elm.chg.dist); break;//変化素材：06距離 １～８段階 提供時２、４、６、８のみ
                case 7:  pdtElem->SetChangeDt(elmid, &elm.chg.loopTime, &elm.chg.loopMax); break;//変化素材：07ループ 戻る段階（0でオフ、１以降で成立）
                case 8:  pdtElem->SetChangeDt(elmid, &elm.chg.waveXTimes); break;//変化素材：08波X回数 波１周期を４、１象限を１とした回数
                case 9:  pdtElem->SetChangeDt(elmid, &elm.chg.waveYTimes); break;//変化素材：09波Y回数 波１周期を４、１象限を１とした回数
                case 10:  pdtElem->SetChangeDt(elmid, &elm.chg.waveZTimes); break;//変化素材：10波Z回数 波１周期を４、１象限を１とした回数
                case 11:  pdtElem->SetChangeDt(elmid, &elm.chg.multiAngleFin); break;//変化素材：11多角処理終 終了フラグ【角数】【時間】
                case 12:  pdtElem->SetChangeDt(elmid, &elm.chg.passOrbitFin); break;//変化素材：12パス挙動終 終了フラグ【ボール位置x】
                    //               case 105:  pdtElem->SetChangeDt(elmid, &elm.chg.multiAngle_Num_c, &elm.chg.multiAngle_Time_c); break;//標的素材：105多角処理 角数カウント
                default:
                    break;
                }
            }
            else  if (elmtype == elt_target)
            {
                s32 trgno = pdtElem->GetTargetElmNo(elmid);

                switch (trgno)
                {
                case 1: pdtElem->SetTargetDt(elmid, &elm.trg.awayY_Dir, &elm.trg.awayY_Mtd); break;
                case 2: pdtElem->SetTargetDt(elmid, &elm.trg.awayZ_Dir, &elm.trg.awayZ_Mtd); break;
                case 3: pdtElem->SetTargetDt(elmid, &elm.trg.backward_Time, &elm.trg.backward_Mtd); break;
                case 4: pdtElem->SetTargetDt(elmid, &elm.trg.stopAll_Time, &elm.trg.stopAll_Mtd); break;
                case 5: pdtElem->SetTargetDt(elmid, &elm.trg.trgSelect, &elm.trg.trgSelect_Mtd); break;
                case 6: pdtElem->SetTargetDt(elmid, &elm.trg.stopForward_Time, &elm.trg.stopForward_Mtd); break;
                case 101: pdtElem->SetTargetDt(elmid, &elm.trg.posX_Dot, &elm.trg.posX_Mtd); break;
                case 102: pdtElem->SetTargetDt(elmid, &elm.trg.posY_Dot, &elm.trg.posY_Mtd); break;
                case 103: pdtElem->SetTargetDt(elmid, &elm.trg.posZ_Dot, &elm.trg.posZ_Mtd); break;
                case 104: pdtElem->SetTargetDt(elmid, &elm.trg.posXZ_Point, &elm.trg.posXZ_Mtd); break;
                case 105:
                    pdtElem->SetTargetDt(elmid, &elm.trg.multiAngle_Time, &elm.trg.multiAngle_Num);
                    elm.chg.multiAngle_Num_c = 0, elm.chg.multiAngle_Time_c = 0;
                    break;
                case 106: pdtElem->SetTargetDt(elmid, &elm.trg.keepCeiling_Time, &elm.trg.keepCeiling_Mtd); break;
                case 107: pdtElem->SetTargetDt(elmid, &elm.trg.reflect_Time, &elm.trg.reflect_Dir); break;
                case 108: pdtElem->SetTargetDt(elmid, &elm.trg.passOrbit); break;
                default:
                    break;
                }
            }
            else  if (elmtype == elt_orbit)
            {
                s32 orbno = pdtElem->GetOrbitElmNo(elmid);

                switch (orbno)
                {
                case 1: pdtElem->SetOrbitDt(elmid, &elm.orb.palab.point, &elm.orb.palab.grav, &elm.orb.palab.dist); break;//山なりデータ構造体
                case 101:
                    elm.orb.waveY.wvtypeMem = elm.orb.waveY.wvtype;//変更前の波タイプ
                    pdtElem->SetOrbitDt(elmid, &elm.orb.waveY.wvtype, &elm.orb.waveY.width, &elm.orb.waveY.freq,//波データ構造体⇒波Y用
                        &elm.orb.waveY.widthChg_Mtd, &elm.orb.waveY.widthChg_Spd, &elm.orb.waveY.peakStop, &elm.orb.waveY.wvDiv, &elm.orb.waveY.wvAbs);
                    if ((elm.orb.waveY.wvtype > 0) && (elm.orb.waveY.wvtype < 9)) crt.orb.waveY.wvtype_f[elm.orb.waveY.wvtype - 1] = TRUE;
                    //                    else if (elm.orb.waveY.wvtype == 9) SetWvTypeRev(elm.orb.waveY.wvtype, elm.orb.waveY.wvRv_f, elm.orb.waveY.wvtypeMem);
                    else if (elm.orb.waveY.wvtype == 9) elm.orb.waveY.wvRvRsrv_f = TRUE;
                    break;
                case 102:
                    elm.orb.waveZ.wvtypeMem = elm.orb.waveZ.wvtype;//変更前の波タイプ
                    pdtElem->SetOrbitDt(elmid, &elm.orb.waveZ.wvtype, &elm.orb.waveZ.width, &elm.orb.waveZ.freq,//波データ構造体⇒波Z用
                        &elm.orb.waveZ.widthChg_Mtd, &elm.orb.waveZ.widthChg_Spd, &elm.orb.waveZ.peakStop, &elm.orb.waveZ.wvDiv, &elm.orb.waveZ.wvAbs);
                    if ((elm.orb.waveZ.wvtype > 0) && (elm.orb.waveZ.wvtype < 9)) crt.orb.waveZ.wvtype_f[elm.orb.waveZ.wvtype - 1] = TRUE;
                    //                    else if (elm.orb.waveZ.wvtype == 9) SetWvTypeRev(elm.orb.waveZ.wvtype, elm.orb.waveZ.wvRv_f, elm.orb.waveZ.wvtypeMem);
                    else if (elm.orb.waveZ.wvtype == 9) elm.orb.waveZ.wvRvRsrv_f = TRUE;
                    break;
                case 103:
                    elm.orb.waveX.wvtypeMem = elm.orb.waveX.wvtype;//変更前の波タイプ
                    pdtElem->SetOrbitDt(elmid, &elm.orb.waveX.wvtype, &elm.orb.waveX.width, &elm.orb.waveX.freq,//波データ構造体⇒波X用
                        &elm.orb.waveX.widthChg_Mtd, &elm.orb.waveX.widthChg_Spd, &elm.orb.waveX.peakStop, &elm.orb.waveX.wvDiv, &elm.orb.waveX.wvAbs);
                    if ((elm.orb.waveX.wvtype > 0) && (elm.orb.waveX.wvtype < 9)) crt.orb.waveX.wvtype_f[elm.orb.waveX.wvtype - 1] = TRUE;
                    //                    else if (elm.orb.waveX.wvtype == 9) SetWvTypeRev(elm.orb.waveX.wvtype, elm.orb.waveX.wvRv_f, elm.orb.waveX.wvtypeMem);
                    else if (elm.orb.waveX.wvtype == 9) elm.orb.waveX.wvRvRsrv_f = TRUE;
                    break;
                default:
                    break;
                }
            }
        }
        //さいどかーぶだま　全原子チェック
        s32 sideCurveFgCount = 0;
        for (s32 i3 = 0; i3 < sdj_END; ++i3)
        {
            if (phs.sideCurveDt[i3] == TRUE) sideCurveFgCount = sideCurveFgCount + 1;
        }
        //サイドカーブフラグ
        phs.sideCurve_f = FALSE;
        //サイドカーブフラグ　↑でカウントが４ならTRUE
        if (sideCurveFgCount >= 4) phs.sideCurve_f = TRUE;

    }

    //さいどかーぶだま　各素材が全て同段階にあれば　さいどかーぶだま認定→波で振幅をいじる
    void TStShot::SideCurveCheck(s32 elmNo)
    {
        enSideCurveJudge sdj = sdj_END;//デフォ値
        switch (elmNo){
        case 5259:sdj = sdj_trg; break;//サイドライン方向に飛ぶ素材
        case 4014:sdj = sdj_wvl; break;//振幅指定素材
        case 4038:sdj = sdj_wvs; break;//振動数指定素材
        case 1031:sdj = sdj_chg; break;//波前後回数指定素材
        default: break;
        }
        //上記４素材の場合 該当する素材の配列に現段階数とフラグをセット
        if (sdj != sdj_END) phs.sideCurveDt[sdj] = TRUE;
    }




    //逆転波の設定・・・既に登録されている波に対して逆転を行う
    void TStShot::SetWvTypeRev(s32& wvtype, BOOL* wvRv_f)
        //        void TStShot::SetWvTypeRev(s32& wvtype, BOOL& wvRv_f, s32& wvtypeMem)
    {
        //タイプが９→逆転波
        //符号をひっくり返したときのフラグを一時保存
        BOOL crtDt[wvt_NUMMAX];
        BOOL noneWv_f = TRUE;//波ナシ・・・次のfor文でTRUEのままだったら+sin波とする
        //登録されている波をチェックして波があったら符号を変えた波タイプに一時保存
        for (s32 i = 0; i < wvt_NUMMAX; i++)
        {
            if (wvRv_f[i] == TRUE)
            {
                noneWv_f = FALSE;//波アリ
                s32 sign = 1;//符号：iが奇数の場合はマイナスになる
                if (i % 2 == 1) sign = -1;//符号：iが奇数の場合はマイナス
                s32 arrNo = i + 1 * sign;////フラグ一時保存用要素数　奇数なら－、偶数なら＋　
                if (crtDt[arrNo] == TRUE) crtDt[arrNo] = FALSE;
                else crtDt[arrNo] = TRUE;
            }
        }

        //WAVETYPE　９　のみの場合は-SinをOnにする
        if (noneWv_f == TRUE) crtDt[1] = TRUE;

        //一時保存した波タイプを実際のフラグに投入
        for (s32 i = 0; i < wvt_NUMMAX; i++)
        {
            wvRv_f[i] = crtDt[i];
        }
    }

    //標的素材Elm⇒Crt--------------------------------------------------------------------------------
    void TStShot::RemoveChangeElmToCrt()
    {
        //変化素材は例外を除いて基本的に段階ごとに切り替わる
        //＆段階切替関数(CheckStepChange())でON・OFFを監視しているので、
        ///Elm⇒Crtに制限の必要なし
        crt.chg.time = elm.chg.time;//変化素材：時間
        crt.chg.height = elm.chg.height;//変化素材：高さ
        crt.chg.pos = elm.chg.pos;//変化素材：一
        crt.chg.dist = elm.chg.dist;//変化素材：距離
        crt.chg.waveXTimes = elm.chg.waveXTimes;//変化素材：波X回数
        crt.chg.waveYTimes = elm.chg.waveYTimes;//変化素材：波Y回数
        crt.chg.waveZTimes = elm.chg.waveZTimes;//変化素材：波Z回数

        crt.chg.hit = elm.chg.hit;//変化素材：ヒット
        crt.chg.ground = elm.chg.ground;//変化素材：地上
        crt.chg.multiAngleFin = elm.chg.multiAngleFin;//変化素材：多角終
        crt.chg.passOrbitFin = elm.chg.passOrbitFin;//変化素材：パス挙動終

        //外部で参照する為のフラグ
        if (crt.chg.ground != 0)
        {//0112
            cmn.shotEff_f[sef_changeGround_f] = TRUE;//地上変化フラグON
            phs.landAfter_f = TRUE;
            if (crt.chg.ground == 2) phs.landAfterOnly_f = TRUE;//１回だけのやつ
            else phs.landAfterOnly_f = FALSE;
        }

        //変化素材：ループのみ例外処理
        //ループ開始前もループ中もelmには指定の時間を入れcrtに渡す
        //⇒段階変更で素材有無チェックに使う。
        if (elm.chg.loopTime != 0)
        {
            //戻る段階loopTimeとループ最大回数loopMaxを実データに入れる
            crt.chg.loopTime = elm.chg.loopTime;
            crt.chg.loopMax = elm.chg.loopMax;
        }

    }

    //波XElm⇒Crt
    void TStShot::RemoveWaveXElmToCrt()
    {
        if ((crt.orb.waveX.wvtype == 0) && (elm.orb.waveX.wvtype == 0)) return;

        s32 elmWvType = elm.orb.waveX.wvtype;//波軌道素材：波タイプ1-8
        s32 elmWvWd = elm.orb.waveX.width;//波軌道素材：振幅1-8
        s32 elmWvFq = elm.orb.waveX.freq;//波軌道素材：振動数1-8
        s32 elmWvWCMt = elm.orb.waveX.widthChg_Mtd;//波軌道素材：振幅変化方法1-4
        s32 elmWvWCSp = elm.orb.waveX.widthChg_Spd;//波軌道素材：振幅変化速度1-5
        s32 elmWvPS = elm.orb.waveX.peakStop;//波軌道素材：頂点停止
        s32 elmWvDir = elm.orb.waveX.wvDiv;//波軌道素材：波分割⇒波有効状態と波無効状態に分割
        s32 crtWvType = crt.orb.waveX.wvtype;//波軌道素材：波タイプ1-8
        s32 crtWvWd = crt.orb.waveX.width;//波軌道素材：振幅1-8
        s32 crtWvFq = crt.orb.waveX.freq;//波軌道素材：振動数1-8
        s32 crtWvWCMt = crt.orb.waveX.widthChg_Mtd;//波軌道素材：振幅変化方法1-4
        s32 crtWvWCSp = crt.orb.waveX.widthChg_Spd;//波軌道素材：振幅変化速度1-5
        s32 crtWvPS = crt.orb.waveX.peakStop;//波軌道素材：頂点停止
        s32 crtWvDir = crt.orb.waveX.wvDiv;//波軌道素材：波分割⇒波有効状態と波無効状態に分割
        s32 elmWvAbs = elm.orb.waveX.wvAbs;//波の絶対値
        s32 crtWvAbs = crt.orb.waveX.wvAbs;//波の絶対値
        //波データ更新チェック
        RemoveWaveElmToCrt(elmWvType, elmWvWd, elmWvFq,
            elmWvWCMt, elmWvWCSp, elmWvPS, elmWvDir, elmWvAbs,
            crtWvType, crtWvWd, crtWvFq,
            crtWvWCMt, crtWvWCSp, crtWvPS, crtWvDir, crtWvAbs,
            crt.orb.waveX.wvtype_f, crt.orb.waveX.wvtypeNoSort, crt.orb.waveX.wvtypeNoMax);

        //実データを更新
        crt.orb.waveX.wvtype = crtWvType;//波軌道素材：波タイプ1-8
        crt.orb.waveX.width = crtWvWd;//波軌道素材：振幅1-8
        crt.orb.waveX.freq = crtWvFq;//波軌道素材：振動数1-8
        crt.orb.waveX.widthChg_Mtd = crtWvWCMt;//波軌道素材：振幅変化方法1-4
        crt.orb.waveX.widthChg_Spd = crtWvWCSp;//波軌道素材：振幅変化速度1-5
        crt.orb.waveX.peakStop = crtWvPS;//波軌道素材：頂点停止
        crt.orb.waveX.wvDiv = crtWvDir;//波軌道素材：波分割⇒波有効状態と波無効状態に分割
        crt.orb.waveX.wvAbs = crtWvAbs;//波軌道素材：波絶対値

        phs.triFunc_f = TRUE;//三角関数フラグオン
        phs.waveX_f = TRUE;
    }

    //波YElm⇒Crt
    void TStShot::RemoveWaveYElmToCrt()
    {
        if ((crt.orb.waveY.wvtype == 0) && (elm.orb.waveY.wvtype == 0)) return;

        s32 elmWvType = elm.orb.waveY.wvtype;//波軌道素材：波タイプ1-8
        s32 elmWvWd = elm.orb.waveY.width;//波軌道素材：振幅1-8
        s32 elmWvFq = elm.orb.waveY.freq;//波軌道素材：振動数1-8
        s32 elmWvWCMt = elm.orb.waveY.widthChg_Mtd;//波軌道素材：振幅変化方法1-4
        s32 elmWvWCSp = elm.orb.waveY.widthChg_Spd;//波軌道素材：振幅変化速度1-5
        s32 elmWvPS = elm.orb.waveY.peakStop;//波軌道素材：頂点停止
        s32 elmWvDir = elm.orb.waveY.wvDiv;//波軌道素材：波分割⇒波有効状態と波無効状態に分割
        s32 crtWvType = crt.orb.waveY.wvtype;//波軌道素材：波タイプ1-8
        s32 crtWvWd = crt.orb.waveY.width;//波軌道素材：振幅1-8
        s32 crtWvFq = crt.orb.waveY.freq;//波軌道素材：振動数1-8
        s32 crtWvWCMt = crt.orb.waveY.widthChg_Mtd;//波軌道素材：振幅変化方法1-4
        s32 crtWvWCSp = crt.orb.waveY.widthChg_Spd;//波軌道素材：振幅変化速度1-5
        s32 crtWvPS = crt.orb.waveY.peakStop;//波軌道素材：頂点停止
        s32 crtWvDir = crt.orb.waveY.wvDiv;//波軌道素材：波分割⇒波有効状態と波無効状態に分割
        s32 elmWvAbs = elm.orb.waveY.wvAbs;//波の絶対値
        s32 crtWvAbs = crt.orb.waveY.wvAbs;//波の絶対値

        //波データ更新チェック
        RemoveWaveElmToCrt(elmWvType, elmWvWd, elmWvFq,
            elmWvWCMt, elmWvWCSp, elmWvPS, elmWvDir, elmWvAbs,
            crtWvType, crtWvWd, crtWvFq,
            crtWvWCMt, crtWvWCSp, crtWvPS, crtWvDir, crtWvAbs,
            crt.orb.waveY.wvtype_f, crt.orb.waveY.wvtypeNoSort, crt.orb.waveY.wvtypeNoMax);

        //実データを更新
        crt.orb.waveY.wvtype = crtWvType;//波軌道素材：波タイプ1-8
        crt.orb.waveY.width = crtWvWd;//波軌道素材：振幅1-8
        crt.orb.waveY.freq = crtWvFq;//波軌道素材：振動数1-8
        crt.orb.waveY.widthChg_Mtd = crtWvWCMt;//波軌道素材：振幅変化方法1-4
        crt.orb.waveY.widthChg_Spd = crtWvWCSp;//波軌道素材：振幅変化速度1-5
        crt.orb.waveY.peakStop = crtWvPS;//波軌道素材：頂点停止
        crt.orb.waveY.wvDiv = crtWvDir;//波軌道素材：波分割⇒波有効状態と波無効状態に分割
        crt.orb.waveY.wvAbs = crtWvAbs;//波軌道素材：波絶対値

        phs.triFunc_f = TRUE;//三角関数フラグオン
        phs.waveY_f = TRUE;

    }

    //波ZElm⇒Crt
    void TStShot::RemoveWaveZElmToCrt()
    {
        if ((crt.orb.waveZ.wvtype == 0) && (elm.orb.waveZ.wvtype == 0)) return;

        s32 elmWvType = elm.orb.waveZ.wvtype;//波軌道素材：波タイプ1-8
        s32 elmWvWd = elm.orb.waveZ.width;//波軌道素材：振幅1-8
        s32 elmWvFq = elm.orb.waveZ.freq;//波軌道素材：振動数1-8
        s32 elmWvWCMt = elm.orb.waveZ.widthChg_Mtd;//波軌道素材：振幅変化方法1-4
        s32 elmWvWCSp = elm.orb.waveZ.widthChg_Spd;//波軌道素材：振幅変化速度1-5
        s32 elmWvPS = elm.orb.waveZ.peakStop;//波軌道素材：頂点停止
        s32 elmWvDir = elm.orb.waveZ.wvDiv;//波軌道素材：波分割⇒波有効状態と波無効状態に分割
        s32 crtWvType = crt.orb.waveZ.wvtype;//波軌道素材：波タイプ1-8
        s32 crtWvWd = crt.orb.waveZ.width;//波軌道素材：振幅1-8
        s32 crtWvFq = crt.orb.waveZ.freq;//波軌道素材：振動数1-8
        s32 crtWvWCMt = crt.orb.waveZ.widthChg_Mtd;//波軌道素材：振幅変化方法1-4
        s32 crtWvWCSp = crt.orb.waveZ.widthChg_Spd;//波軌道素材：振幅変化速度1-5
        s32 crtWvPS = crt.orb.waveZ.peakStop;//波軌道素材：頂点停止
        s32 crtWvDir = crt.orb.waveZ.wvDiv;//波軌道素材：波分割⇒波有効状態と波無効状態に分割
        s32 elmWvAbs = elm.orb.waveZ.wvAbs;//波の絶対値
        s32 crtWvAbs = crt.orb.waveZ.wvAbs;//波の絶対値

        //波データ更新チェック
        RemoveWaveElmToCrt(elmWvType, elmWvWd, elmWvFq,
            elmWvWCMt, elmWvWCSp, elmWvPS, elmWvDir, elmWvAbs,
            crtWvType, crtWvWd, crtWvFq,
            crtWvWCMt, crtWvWCSp, crtWvPS, crtWvDir, crtWvAbs,
            crt.orb.waveZ.wvtype_f, crt.orb.waveZ.wvtypeNoSort, crt.orb.waveZ.wvtypeNoMax);

        //実データを更新
        crt.orb.waveZ.wvtype = crtWvType;//波軌道素材：波タイプ1-8
        crt.orb.waveZ.width = crtWvWd;//波軌道素材：振幅1-8
        crt.orb.waveZ.freq = crtWvFq;//波軌道素材：振動数1-8
        crt.orb.waveZ.widthChg_Mtd = crtWvWCMt;//波軌道素材：振幅変化方法1-4
        crt.orb.waveZ.widthChg_Spd = crtWvWCSp;//波軌道素材：振幅変化速度1-5
        crt.orb.waveZ.peakStop = crtWvPS;//波軌道素材：頂点停止
        crt.orb.waveZ.wvDiv = crtWvDir;//波軌道素材：波分割⇒波有効状態と波無効状態に分割
        crt.orb.waveZ.wvAbs = crtWvAbs;//波軌道素材：波絶対値

        phs.triFunc_f = TRUE;//三角関数フラグオン
        phs.waveZ_f = TRUE;

    }

    //波共通処理
    void TStShot::RemoveWaveElmToCrt(s32 elmWvType, s32 elmWvWd, s32 elmWvFq,
        s32 elmWvWCMt, s32 elmWvWCSp, s32 elmWvPS, s32 elmWvDir, s32 elmWvAbs,
        s32& crtWvType, s32& crtWvWd, s32& crtWvFq,
        s32& crtWvWCMt, s32& crtWvWCSp, s32& crtWvPS, s32& crtWvDir, s32& crtWvAbs,
        BOOL* wvType, s32* wvTypesort, s32& wvTypeMax)
    {
        //現データと素材データを比較し、各々処理後現データ用変数を更新
        //（元の関数に戻り現データ用変数を、現データに反映する）

        //ためしで一旦ｃｒｔ０にしてみる。
        //後々波継続処理を作る
        //イジしない波なら最初に全データをクリア
        if (cmn.efEl.waveKeep_f == FALSE)
        {
            crtWvType = 0;
            crtWvWd = 0;
            crtWvFq = 0;
            crtWvWCMt = 0;
            crtWvWCSp = 0;
            crtWvPS = 0;
            crtWvDir = 0;
            crtWvAbs = 0;
        }


        //先にelmWvType=0ならcrtにはすべて０を入れる
        if (elmWvType == 0)
        {
            //            crtWvType = 0; 
            //            crtWvWd = 0; 
            //            crtWvFq = 0;
            //            crtWvWCMt = 0; 
            //            crtWvWCSp = 0; 
            //            crtWvPS = 0; 
            //            crtWvDir = 0; 
            //            crtWvAbs = 0;
        }
        else
            //elmWvType!=0ならcrt更新
        {
            //波タイプ…とりあえず数がでかい方に更新
            if (crtWvType < elmWvType) crtWvType = elmWvType;
            //但し、現波タイプが１以上で素材が-1の場合は波クリアとする
            if ((crtWvType > elmWvType) && (elmWvType == -1)) crtWvType = 0;

            //振幅…たし合わせる ８を超えない
            if ((elmWvWd == 0) && (crtWvWd == 0)) crtWvWd = 3;
            if (elmWvWd > crtWvWd) crtWvWd = elmWvWd;
            if (crtWvWd > 5) crtWvWd = 5;
            if (crtWvWd < 1) crtWvWd = 1;

            //振動数…たし合わせる ８を超えない
            if ((elmWvFq == 0) && (crtWvFq == 0)) crtWvFq = 0;
            if (abs(elmWvFq) > abs(crtWvFq)) crtWvFq = elmWvFq;
            if (crtWvFq > 2) crtWvFq = 2;
            if (crtWvFq < -2) crtWvFq = -2;

            //振幅変化方法…でかい方に更新
            if (crtWvWCMt < elmWvWCMt) crtWvWCMt = elmWvWCMt;

            //振幅変化速度
            crtWvWCSp = crtWvWCSp + elmWvWCSp;
            if (crtWvWCSp > 8) crtWvWCSp = 8;
            if (crtWvWCSp < -8) crtWvWCSp = -8;

            //頂点停止…フラグが立っていたら立てる
            if ((elmWvPS != crtWvPS) && ((elmWvPS > crtWvPS))) crtWvPS = elmWvPS;
            if (crtWvPS > 3) crtWvPS = 3;

            //波分割…でかい方に更新
            if (crtWvDir < elmWvDir) crtWvDir = elmWvDir;

            //波絶対値…でかい方に更新
            if (crtWvAbs < elmWvAbs) crtWvAbs = elmWvAbs;

            //同方向波の処理
            //プールしていた波タイプをタイプ別から使用順＋使用しないものは破棄とする
            s32 arrNo = 0;//並び替え時の配列番号…０から１ずつ増える
            for (s32 i = 0; i < wvt_NUMMAX; i++)//使用する波タイプを抽出
            {//波タイプ０～７をチェック→TRUEなら波タイプ１～８にしてソート配列に入れ
                //ソート配列要素数を１アップ
                if (wvType[i] == TRUE) wvTypesort[arrNo] = i + 1, arrNo = arrNo + 1;
            }
            wvTypeMax = arrNo;//最大値を入れる
            for (s32 i = arrNo; i < wvt_NUMMAX; i++)//あまった配列に０を入れる
            {
                wvTypesort[i] = 0;
            }

        }
    }

    //軌道素材Elm⇒Crt
    void TStShot::RemoveOrbitElmToCrt()
    {
        //波反転フラグがあったら反転処理を行う
        if (elm.orb.waveY.wvRvRsrv_f == TRUE) SetWvTypeRev(elm.orb.waveY.wvtype, crt.orb.waveY.wvtype_f);
        if (elm.orb.waveZ.wvRvRsrv_f == TRUE) SetWvTypeRev(elm.orb.waveZ.wvtype, crt.orb.waveZ.wvtype_f);
        if (elm.orb.waveX.wvRvRsrv_f == TRUE) SetWvTypeRev(elm.orb.waveX.wvtype, crt.orb.waveX.wvtype_f);

        //山なりデータは素材があれば必ず照準ポイントに１以上が設定されているので
        //素材データ！＝0を確認。次に現データを上回る素材データがあれば更新する。
        s32 elmPPoint = elm.orb.palab.point;//山なり軌道素材：照準ポイント
        s32 crtPPoint = crt.orb.palab.point;//山なり実軌道データ：照準ポイント
        s32 elmPGrav = elm.orb.palab.grav;//山なり軌道素材：重力
        s32 crtPGrav = crt.orb.palab.grav;//山なり実軌道データ：重力
        s32 elmPDist = elm.orb.palab.dist;//山なり軌道素材：距離
        s32 crtPDist = crt.orb.palab.dist;//山なり実軌道データ：距離

        crt.orb.palab.point = 0;
        crt.orb.palab.grav = 0;
        crt.orb.palab.dist = 0;
        phs.accelFunc_f = FALSE;//二次関数フラグオン
        phs.accelFuncInit_f = FALSE;
        //照準ポイントから素材データON・OFFの確認
        if (elmPPoint != 0)
        {
            //現行データ＜素材データ の場合、素材データで現行データを更新。
            if (elmPPoint != 0) crt.orb.palab.point = elmPPoint;
            crt.orb.palab.grav = elmPGrav;
            if (crtPDist < elmPDist) crt.orb.palab.dist = elmPDist;
            phs.accelFunc_f = TRUE;//二次関数フラグオン
            phs.accelFuncInit_f = FALSE;
            //            phs.accelFuncInit_f = FALSE;
        }

        //波データ
        //それぞれの関数へ一旦飛ばす
        RemoveWaveXElmToCrt();
        RemoveWaveYElmToCrt();
        RemoveWaveZElmToCrt();

    }

    //変化方法設定
    void TStShot::SetChangeMtd(s32 mtd)
    {
        if (mtd > 0)
        {
            //変化方法番号が大きい時は方法チェンジ
            if (mtd > phs.changeMtd) phs.changeMtd = mtd, phs.changeMtdFromTarget = mtd;
            //ただし、現状３で更新値が違っていたら更新する
            if ((mtd != phs.changeMtd) && (phs.changeMtd == 3)) phs.changeMtd = mtd, phs.changeMtdFromTarget = mtd;
        }
    }

    //標的素材Elm⇒Crt
    void TStShot::RemoveTargetElmToCrt()
    {
        //標的素材：反れ縦・反れサイドは必ず更新
        crt.trg.awayY_Dir = elm.trg.awayY_Dir;
        SetChangeMtd(elm.trg.awayY_Mtd);

        crt.trg.awayZ_Dir = elm.trg.awayZ_Dir;
        crt.trg.awayZ_DirNoTg = 0;
        //反れサイドの場合Mtdより反れ方向が変わる　７以上なら広いほう、４以上なら近いほう
        if (elm.trg.awayZ_Mtd > 3)
        {
            if (elm.trg.awayZ_Mtd > 6) crt.trg.awayZ_Mtd = elm.trg.awayZ_Mtd - 7, crt.trg.awayZ_DirNoTg = 1;
            else crt.trg.awayZ_Mtd = elm.trg.awayZ_Mtd - 4, crt.trg.awayZ_DirNoTg = -1;
            phs.changeMtd = crt.trg.awayZ_Mtd;
            phs.changeMtdFromTarget = crt.trg.awayZ_Mtd;
        }
        else SetChangeMtd(elm.trg.awayZ_Mtd);

        //標的素材：逆進
        crt.trg.backward_Time = elm.trg.backward_Time;
        SetChangeMtd(elm.trg.backward_Mtd);
        //標的素材：停止
        crt.trg.stopAll_Time = elm.trg.stopAll_Time;
        SetChangeMtd(elm.trg.stopAll_Mtd);
        //標的素材：初速０
        crt.trg.stopForward_Time = elm.trg.stopForward_Time;
        SetChangeMtd(elm.trg.stopForward_Mtd);
        //標的素材：標的選択
        crt.trg.trgSelect = elm.trg.trgSelect;
        SetChangeMtd(elm.trg.trgSelect_Mtd);
        BOOL objPos_f = FALSE;
        s32 objPX = 0;
        s32 objPY = 0;
        s32 objPZ = 0;
        //内野なら１
        s32 fieldDir = 1;
        //2Pなら負
        s32 teamDir = idata.side * (-2) + 1;//左なら０右なら１
        s32 baseX = DBCRT_CL / XYMAG;//基本線･･･外野ならセンターライン
        //コートエンド変化で使用
        s32 gradLine = ((phs.posZ * XYMAG - DBCRT_FL) * 10 / DBCRT_SLR);
        s32 gradLineR = (f32)(DBCRT_W - DBCRT_SL - gradLine) / (f32)XYMAG;//速度＋⇒右コートエンド
        s32 gradLineL = (f32)(DBCRT_SL + gradLine) / (f32)XYMAG;//左コートエンド

        switch (crt.trg.trgSelect)
        {
            //敵内野少し侵入＋上空
        case 7:
            objPY = 80;//Yはジャンプシュートより少し高い位置

            //ポジションで狙い位置を決定
            //対面外野なら負にして基本線を自分位置にする
            if (idata.frmPos == 6) teamDir = -1, baseX = phs.posX;
            objPX = baseX + 48 * teamDir * fieldDir;
            objPZ = phs.posZ;

            //手前奥外野
            if ((idata.frmPos == 4) || (idata.frmPos == 5))
            {
                fieldDir = 1;//手前外野なら1、奥外野なら-1
                if (idata.frmPos == 4) fieldDir = -1;
                objPX = phs.posX;
                objPZ = phs.posZ + 16 * fieldDir;
            }
            objPos_f = TRUE;
            break;
            //対面ライン上空
        case 8:
            objPY = 80;//Yはジャンプシュートより少し高い位置
            //ポジションで狙い位置を決定

            //内野野基本線をコートエンドに
            if (idata.frmPos != 6)
            {
                baseX = gradLineL;//速度＋⇒右コートエンド,２Pなら左コートエンド
                if (teamDir == 1) baseX = gradLineR;
            }

            objPX = baseX;
            objPZ = phs.posZ;

            //手前奥外野
            if ((idata.frmPos == 4) || (idata.frmPos == 5))
            {
                objPX = phs.posX;//そのまま
                objPZ = DBCRT_BL / XYMAG;//手前外野なら奥へ
                if (idata.frmPos == 4) objPZ = DBCRT_FL / XYMAG;//奥外野なら手前へ
            }
            objPos_f = TRUE;
            break;

        default: break;
        }
        if (objPos_f == TRUE)
        {
            cmn.objPX = objPX;
            cmn.objPY = objPY;
            cmn.objPZ = objPZ;
        }
        SetChangeMtd(elm.trg.trgSelect_Mtd);

        //標的素材：多角終
        crt.trg.multiAngle_Time = elm.trg.multiAngle_Time;
        crt.trg.multiAngle_Num = elm.trg.multiAngle_Num;
        //標的素材：天井維持
        crt.trg.keepCeiling_Time = elm.trg.keepCeiling_Time;
        SetChangeMtd(elm.trg.keepCeiling_Mtd);
        //標的素材：跳ね返り
        crt.trg.reflect_Time = elm.trg.reflect_Time;
        crt.trg.reflect_Dir = elm.trg.reflect_Dir;

        //標的素材：パス挙動
        crt.trg.passOrbit = 0;
        if (elm.trg.passOrbit != 0) crt.trg.passOrbit = 1;

        //標的素材：位置指定X
        crt.trg.posX_Dot = elm.trg.posX_Dot;
        SetChangeMtd(elm.trg.posX_Mtd);
        //標的素材：位置指定Y
        crt.trg.posY_Dot = elm.trg.posY_Dot;
        SetChangeMtd(elm.trg.posY_Mtd);
        //標的素材：位置指定Z
        crt.trg.posZ_Dot = elm.trg.posZ_Dot;
        SetChangeMtd(elm.trg.posZ_Mtd);
        //標的素材：位置指定XZ
        crt.trg.posXZ_Point = elm.trg.posXZ_Point;
        SetChangeMtd(elm.trg.posXZ_Mtd);

        //特殊処理…反れ縦水平＆変化高さ＆逆進ナシの場合、
        //条件がいつまでたっても満たされないので逆進１６Frを変わりに条件とする
        if (
            (crt.trg.awayY_Dir == 3)
            &&
            (crt.chg.height != 0)
            &&
            (crt.chg.time == 0)
            )
        {
            crt.trg.backward_Time = 16;
            crt.chg.time = 16;
        }
    }

    //３種の素材データElmToCrt
    void TStShot::RemoveElmToCrt()
    {
        RemoveChangeElmToCrt();
        RemoveOrbitElmToCrt();
        RemoveTargetElmToCrt();
    }

    //効果素材処理**************************************************************************************
    void TStShot::SetEffectElmFlag()
    {
        //各効果素材の有無を確認
        cmn.efEl.Init();

        //全段階適用の効果素材のフラグ管理
        if (idata.eff.penet != 0) cmn.efEl.penet_f = TRUE;     //1:貫き効果
        if (idata.eff.suction != 0) cmn.efEl.suction_f = TRUE;   //2:吸引効果
        if (idata.eff.flyReverse != 0) cmn.efEl.flyReverse_f = TRUE;//7:引っ張り効果
        if (idata.eff.flyFire != 0) cmn.efEl.flyFire_f = TRUE;   //8:吹っ飛び火効果
        if (idata.eff.flyElec != 0) cmn.efEl.flyElec_f = TRUE;   //9:吹っ飛び電効果
        if (idata.eff.rollDown != 0) cmn.efEl.rollDown_f = TRUE;  //10:ゴロゴロダウン効果
        if (idata.eff.flyLoose != 0) cmn.efEl.flyLoose_f = TRUE;  //11:吹っ飛び緩効果
        if (idata.eff.rndWorld != 0) cmn.efEl.rndWorld_f = TRUE;  //12:世界一周効果
        if (idata.eff.pass != 0) cmn.efEl.pass_f = TRUE;      //15:パス挙動効果
        if (idata.eff.finalSE != 0) cmn.efEl.finalSE_f = TRUE;      //16:最終段階ＳＥ効果
        if (idata.eff.waveKeep != 0) cmn.efEl.waveKeep_f = TRUE;      //16:最終段階ＳＥ効果
        if (idata.eff.catchSlip != 0) cmn.efEl.catchSlip_f = TRUE;      //16:キャッチスリップ
        if (idata.eff.catchSlipR != 0) cmn.efEl.catchSlipR_f = TRUE;      //16:Reverseキャッチスリップ
        if (idata.eff.symCtrl != 0) cmn.efEl.symCtrl_f = TRUE;      //16:マリオネット効果



        SetExternalEffData();//外部参照Effデータのセット

        //段階指定の効果素材…１回目発動予定のデータが０でなければ「発動予定」とする
        if (idata.eff.accel_Lv[0] != 0) cmn.efEl.accel.incld_f = TRUE;     //1:貫き効果
        if (idata.eff.decel_Lv[0] != 0) cmn.efEl.decel.incld_f = TRUE;  //102:減速効果
        if (idata.eff.yAccel_Lv[0] != 0) cmn.efEl.yAccel.incld_f = TRUE; //103:y加速効果
        if (idata.eff.spdChg_Lv[0] != 0) cmn.efEl.spdChg.incld_f = TRUE; //104:速度可変効果
        if (idata.eff.trace_Lv[0] != 0) cmn.efEl.trace.incld_f = TRUE;  //105:追跡効果
        if (idata.eff.reflect_Lv[0] != 0) cmn.efEl.reflect.incld_f = TRUE;//106:強反射効果
        if (idata.eff.mySide_Lv[0] != 0) cmn.efEl.mySide.incld_f = TRUE; //107:味方被弾効果
        if (idata.eff.normal_Lv[0] != 0) cmn.efEl.normal.incld_f = TRUE; //108:通常球化効果
        if (idata.eff.flyVer_Lv[0] != 0) cmn.efEl.flyVer.incld_f = TRUE; //109:真上吹っ飛び効果
        if (idata.eff.flyHor_Lv[0] != 0) cmn.efEl.flyHor.incld_f = TRUE; //110:真横吹っ飛び効果
        if (idata.eff.press_Lv[0] != 0) cmn.efEl.press.incld_f = TRUE;  //111:圧殺効果
        if (idata.eff.copy_Lv[0] != 0) cmn.efEl.copy.incld_f = TRUE;   //112:複写効果
        if (idata.eff.noMvDwn_Lv[0] != 0) cmn.efEl.noMvDwn.incld_f = TRUE;//113:その場ダウン効果
        if (idata.eff.combo_Max[0] != 0) cmn.efEl.combo.incld_f = TRUE;  //114:同一標的連続被弾効果
        if (idata.eff.pulse_Type[0] != 0) cmn.efEl.pulse.incld_f = TRUE;  //115:拍効果
        if (idata.eff.sePhase_Num[0] != 0) cmn.efEl.sePhase.incld_f = TRUE; //116:効果音段階変化時
        if (idata.eff.seHit_NumFrnt[0] != 0) cmn.efEl.seHit.incld_f = TRUE;   //117:効果音被弾時
        if (idata.eff.refVer_Lv[0] != 0) cmn.efEl.refVer.incld_f = TRUE; //109:上跳吹っ飛び効果
        if (idata.eff.refHor_Lv[0] != 0) cmn.efEl.refHor.incld_f = TRUE; //110:横跳吹っ飛び効果
        if (idata.eff.sprtOff_Lv[0] != 0) cmn.efEl.sprtOff.incld_f = TRUE; //110:分裂Off効果
        if (idata.eff.tagChange_Lv[0] != 0) cmn.efEl.tagChange.incld_f = TRUE; //127:標的変化効果
        if (idata.eff.wEdge_Lv[0] != 0) cmn.efEl.wEdge.incld_f = TRUE; //127:標的変化効果
        if (idata.eff.spdMomUp_Lv[0] != 0) cmn.efEl.spdMomUp.incld_f = TRUE; //128:一時速度上昇効果
        if (idata.eff.spdMomDown_Lv[0] != 0) cmn.efEl.spdMomDown.incld_f = TRUE; //129:一時速度減少効果
        if (idata.eff.noDmg_Lv[0] != 0) cmn.efEl.noDmg.incld_f = TRUE; //130:ダメージナシ効果
        if (idata.eff.phantom_Lv[0] != 0) cmn.efEl.phantom.incld_f = TRUE; //130:ダメージナシ効果


#ifdef __K_DEBUG_SHIAI__
        // 原子のデータをログに書き込む
        //kdebug::DebugSystem::GetInstance()->AddShootDebugLogElementAtm(nowphase, sdata.phase_elms[nowphase][i2]);
#endif	// #ifdef __K_DEBUG_SHIAI__
    }

    void TStShot::CheckEffectElmFlag()
    {
        //段階指定の効果素材の段階一致のチェック
        //まずフラグをFALSEに
        //但し、指定段階以降効果を持続させる場合はFALSEにしない（各行をコメントアウト）
        if (cmn.efEl.accel.phase_f == TRUE) cmn.efEl.accel.phase_f = FALSE, cmn.efEl.accel.times_c += 1;  //101:加速効果
        if (cmn.efEl.decel.phase_f == TRUE) cmn.efEl.decel.phase_f = FALSE, cmn.efEl.decel.times_c += 1;  //102:減速効果
        if (cmn.efEl.yAccel.phase_f == TRUE) cmn.efEl.yAccel.phase_f = FALSE, cmn.efEl.yAccel.times_c += 1; //103:y加速効果
        if (cmn.efEl.spdChg.phase_f == TRUE) cmn.efEl.spdChg.phase_f = FALSE, cmn.efEl.spdChg.times_c += 1; //104:速度可変効果
        //if (cmn.efEl.trace.phase_f == TRUE) cmn.efEl.trace.phase_f = FALSE, cmn.efEl.trace.times_c += 1;  //105:追跡効果
        //if (cmn.efEl.reflec.phase_f == TRUE) cmn.efEl.reflect.phase_f = FALSE, cmn.efEl.reflec.times_c += 1;//106:強反射効果
        if (cmn.efEl.mySide.phase_f == TRUE) cmn.efEl.mySide.phase_f = FALSE, cmn.efEl.mySide.times_c += 1; //107:味方被弾効果
        if (cmn.efEl.normal.phase_f == TRUE) cmn.efEl.normal.phase_f = FALSE, cmn.efEl.normal.times_c += 1; //108:通常球化効果
        //if (cmn.efEl.flyVer.phase_f == TRUE) cmn.efEl.flyVer.phase_f = FALSE, cmn.efEl.flyVer.times_c += 1; //109:真上吹っ飛び効果
        //if (cmn.efEl.flyHor.phase_f == TRUE) cmn.efEl.flyHor.phase_f = FALSE, cmn.efEl.flyHor.times_c += 1; //110:真横吹っ飛び効果
        //if (cmn.efEl.press.phase_f == TRUE) cmn.efEl.press.phase_f = FALSE, cmn.efEl.press.times_c += 1;  //111:圧殺効果
        //if (cmn.efEl.copy.phase_f == TRUE) cmn.efEl.copy.phase_f = FALSE, cmn.efEl.copy.times_c += 1;   //112:複写効果
        //if (cmn.efEl.noMvDwn.phase_f == TRUE) cmn.efEl.noMvDwn.phase_f = FALSE, cmn.efEl.noMvDwn.times_c += 1;//113:その場ダウン効果
        //if (cmn.efEl.combo.phase_f == TRUE) cmn.efEl.combo.phase_f = FALSE, cmn.efEl.combo.times_c += 1;  //114:同一標的連続被弾効果
        //if (cmn.efEl.pulse.phase_f == TRUE) cmn.efEl.pulse.phase_f = FALSE, cmn.efEl.pulse.times_c += 1;  //115:拍効果
        if (cmn.efEl.sePhase.phase_f == TRUE) cmn.efEl.sePhase.phase_f = FALSE, cmn.efEl.sePhase.times_c += 1; //116:効果音段階変化時
        //if (cmn.efEl.seHit.phase_f == TRUE) cmn.efEl.seHit.phase_f = FALSE, cmn.efEl.seHit.times_c += 1;   //117:効果音被弾時
        if (cmn.efEl.sprtOff.phase_f == TRUE) cmn.efEl.sprtOff.phase_f = FALSE, cmn.efEl.sprtOff.times_c += 1; //107:味方被弾効果
        if (cmn.efEl.wEdge.phase_f == TRUE) cmn.efEl.wEdge.phase_f = FALSE, cmn.efEl.wEdge.times_c += 1; //1:諸刃効果
        if (cmn.efEl.tagChange.phase_f == TRUE) cmn.efEl.tagChange.phase_f = FALSE, cmn.efEl.tagChange.times_c += 1; //1:標的チェンジ効果
        if (cmn.efEl.spdMomUp.phase_f == TRUE) cmn.efEl.spdMomUp.phase_f = FALSE, cmn.efEl.spdMomUp.times_c += 1; //1:一時速度アップ効果
        if (cmn.efEl.spdMomDown.phase_f == TRUE) cmn.efEl.spdMomDown.phase_f = FALSE, cmn.efEl.spdMomDown.times_c += 1; //1:一時速度ダウン効果
        if (cmn.efEl.phantom.phase_f == TRUE) cmn.efEl.phantom.phase_f = FALSE, cmn.efEl.phantom.times_c += 1; //1:一時速度ダウン効果

        //段階指定の効果素材…現在の効果素材発動回数の指定する段階が現段階と一致したらphase_fをOnにする
        if (idata.eff.accel_Phase[cmn.efEl.accel.times_c] == cmn.phaseCrt) cmn.efEl.accel.phase_f = TRUE;  //101:加速効果
        if (idata.eff.decel_Phase[cmn.efEl.decel.times_c] == cmn.phaseCrt) cmn.efEl.decel.phase_f = TRUE;  //102:減速効果
        if (idata.eff.yAccel_Phase[cmn.efEl.yAccel.times_c] == cmn.phaseCrt) cmn.efEl.yAccel.phase_f = TRUE; //103:y加速効果
        if (idata.eff.spdChg_Phase[cmn.efEl.spdChg.times_c] == cmn.phaseCrt) cmn.efEl.spdChg.phase_f = TRUE; //104:速度可変効果
        if (idata.eff.trace_Phase[cmn.efEl.trace.times_c] == cmn.phaseCrt) cmn.efEl.trace.phase_f = TRUE;  //105:追跡効果
        if (idata.eff.reflect_Phase[cmn.efEl.reflect.times_c] == cmn.phaseCrt) cmn.efEl.reflect.phase_f = TRUE;//106:強反射効果
        if (idata.eff.mySide_Phase[cmn.efEl.mySide.times_c] == cmn.phaseCrt) cmn.efEl.mySide.phase_f = TRUE; //107:味方被弾効果
        if (idata.eff.normal_Phase[cmn.efEl.normal.times_c] == cmn.phaseCrt) cmn.efEl.normal.phase_f = TRUE; //108:通常球化効果
        if (idata.eff.flyVer_Phase[cmn.efEl.flyVer.times_c] == cmn.phaseCrt) cmn.efEl.flyVer.phase_f = TRUE; //109:真上吹っ飛び効果
        if (idata.eff.flyHor_Phase[cmn.efEl.flyHor.times_c] == cmn.phaseCrt) cmn.efEl.flyHor.phase_f = TRUE; //110:真横吹っ飛び効果
        if (idata.eff.press_Phase[cmn.efEl.press.times_c] == cmn.phaseCrt) cmn.efEl.press.phase_f = TRUE;  //111:圧殺効果
        if (idata.eff.copy_Phase[cmn.efEl.copy.times_c] == cmn.phaseCrt) cmn.efEl.copy.phase_f = TRUE;   //112:複写効果
        if (idata.eff.noMvDwn_Phase[cmn.efEl.noMvDwn.times_c] == cmn.phaseCrt) cmn.efEl.noMvDwn.phase_f = TRUE;//113:その場ダウン効果
        if (idata.eff.combo_Phase[cmn.efEl.combo.times_c] == cmn.phaseCrt) cmn.efEl.combo.phase_f = TRUE;  //114:同一標的連続被弾効果
        if (idata.eff.pulse_Phase[cmn.efEl.pulse.times_c] == cmn.phaseCrt) cmn.efEl.pulse.phase_f = TRUE;  //115:拍効果
        if (idata.eff.sePhase_Phase[cmn.efEl.sePhase.times_c] == cmn.phaseCrt) cmn.efEl.sePhase.phase_f = TRUE; //116:効果音段階変化時
        if (idata.eff.seHit_Phase[cmn.efEl.seHit.times_c] == cmn.phaseCrt) cmn.efEl.seHit.phase_f = TRUE;   //117:効果音被弾時
        if (idata.eff.refVer_Phase[cmn.efEl.refVer.times_c] == cmn.phaseCrt) cmn.efEl.refVer.phase_f = TRUE; //109:上跳吹っ飛び効果
        if (idata.eff.refHor_Phase[cmn.efEl.refHor.times_c] == cmn.phaseCrt) cmn.efEl.refHor.phase_f = TRUE; //110:横跳吹っ飛び効果
        if (idata.eff.wEdge_Phase[cmn.efEl.wEdge.times_c] == cmn.phaseCrt) cmn.efEl.wEdge.phase_f = TRUE; //125:横跳吹っ飛び効果
        if (idata.eff.sprtOff_Phase[cmn.efEl.sprtOff.times_c] == cmn.phaseCrt) cmn.efEl.sprtOff.phase_f = TRUE; //126:分裂オフ効果
        if (idata.eff.tagChange_Phase[cmn.efEl.tagChange.times_c] == cmn.phaseCrt) cmn.efEl.tagChange.phase_f = TRUE; //1:標的変化効果
        if (idata.eff.spdMomUp_Phase[cmn.efEl.spdMomUp.times_c] == cmn.phaseCrt) cmn.efEl.spdMomUp.phase_f = TRUE; //1:一時速度上昇効果
        if (idata.eff.spdMomDown_Phase[cmn.efEl.spdMomDown.times_c] == cmn.phaseCrt) cmn.efEl.spdMomDown.phase_f = TRUE; //1:一時速度減少効果
        if (idata.eff.noDmg_Phase[cmn.efEl.noDmg.times_c] == cmn.phaseCrt) cmn.efEl.noDmg.phase_f = TRUE; //1:ダメージナシ効果
        if (idata.eff.phantom_Phase[cmn.efEl.phantom.times_c] == cmn.phaseCrt) cmn.efEl.phantom.phase_f = TRUE; //1:ダメージナシ効果

        SetExternalEffData();//外部参照Effデータのセット

        //フラグが経ったら初期化フラグをOFFにする必要があるものはここで処理する
        if (cmn.efEl.pulse.phase_f == TRUE) phs.pulse_Init_f = FALSE;

        //以降段階以降等における速度処理*****************
        //ε効果素材処理
        //ε加速・減速・y加速
        //加速系効果素材を持つシュートはひとまず加速系関数に入る為のフラグを以下に立てる。
        //（加速系関数内で段階を見て実行ON・OFFを決める）
        if ((cmn.efEl.accel.incld_f == TRUE) || (cmn.efEl.decel.incld_f == TRUE) || (cmn.efEl.yAccel.incld_f == TRUE))
        {
            phs.accelFunc_f = TRUE;
            //            phs.accelFuncInit_f = FALSE;
            if (idata.eff.accel_Lv[cmn.efEl.accel.times_c] != 0) phs.acc = (f32)idata.eff.accel_Lv[cmn.efEl.accel.times_c];     //101:加速効果
            if (idata.eff.decel_Lv[cmn.efEl.decel.times_c] != 0) phs.acc = (f32)idata.eff.decel_Lv[cmn.efEl.decel.times_c];  //102:減速効果
            if (idata.eff.yAccel_Lv[cmn.efEl.yAccel.times_c] != 0) phs.acc = (f32)idata.eff.yAccel_Lv[cmn.efEl.yAccel.times_c]; //103:y加速効果
        }

        //ε速度可変
        //速度可変素材が発動条件を満たしていたら速度を変える
        if ((cmn.efEl.spdChg.phase_f == TRUE) && (cmn.efEl.spdChg.incld_f == TRUE))
        {
            //速度レベルチェンジ
            phs.spdLv = phs.spdLv + idata.eff.spdChg_Lv[cmn.efEl.spdChg.times_c] * 2;
        }

        //ε速度一時可変
        //速度可変素材が発動条件を満たしていたら速度を変える 満たしていないときは０を入れる
        if (
            ((cmn.efEl.spdMomUp.phase_f == TRUE) && (cmn.efEl.spdMomUp.incld_f == TRUE))
            || ((cmn.efEl.spdMomDown.phase_f == TRUE) && (cmn.efEl.spdMomDown.incld_f == TRUE))
            )
        {
            //速度レベルチェンジ
            s32 spdLvInc = idata.eff.spdMomUp_Lv[cmn.efEl.spdMomUp.times_c];
            s32 spdLvDec = idata.eff.spdMomDown_Lv[cmn.efEl.spdMomDown.times_c];
            s32 onInc_f = (s32)cmn.efEl.spdMomUp.phase_f;
            s32 onDec_f = (s32)cmn.efEl.spdMomDown.phase_f;
            phs.spdMomLv = spdLvInc * onInc_f - spdLvDec * onDec_f;
        }
        else phs.spdMomLv = 0;

        //諸刃効果　発動段階で標的チェンジフラグを立てる
        if ((cmn.efEl.wEdge.phase_f == TRUE) && (cmn.efEl.wEdge.incld_f == TRUE)) cmn.wEdgeTgChange_f = TRUE;
        else cmn.wEdgeTgChange_f = FALSE;
        //敵標的変え効果
        if ((cmn.efEl.tagChange.phase_f == TRUE) && (cmn.efEl.tagChange.incld_f == TRUE)) cmn.gncdTgChange_f = TRUE;
        else cmn.gncdTgChange_f = FALSE;

        //～波の方向数により速度を下げる
        //まず現在の波の個数を調べる
        s32 waveX_f = 0;
        if (crt.orb.waveX.wvtypeNoSort[0] != 0) waveX_f = 1;
        s32 waveY_f = 0;
        if (crt.orb.waveY.wvtypeNoSort[0] != 0) waveY_f = 1;
        s32 waveZ_f = 0;
        if (crt.orb.waveZ.wvtypeNoSort[0] != 0) waveZ_f = 1;

        s32 waveNow_c = waveX_f + waveY_f + waveZ_f;

        //速度減少率を波データ数から設定
        f32 decRatio = 100.0f;
        switch (waveNow_c){
        case 1: decRatio = 90.0f; break;
        case 2: decRatio = 75.0f; break;
        case 3: decRatio = 60.0f; break;
        default: decRatio = 100.0f; break;
        }
        //％から少数に直して速度減少率を正規データに入れる
        phs.spdDecFromWave = decRatio / 100.0f;

        //速度下限
        if (phs.spd < SPD_LOW_LIMIT) phs.spd = SPD_LOW_LIMIT;//SPD_LOW_LIMIT未満にはしない
    }

    //外部から参照するEffデータをセットする
    void TStShot::SetExternalEffData()
    {
        //1:貫き効果
        if (cmn.efEl.penet_f == TRUE)
        {
            if (cmn.phaseCrt == 0) cmn.efEl.penet_c = idata.eff.penet; //貫通カウント　初段階で更新　0で貫通しない        
            if (idata.eff.penetTime > 0) cmn.efEl.peneStick_f = TRUE; //くっつき発動フラグ
            cmn.efEl.peneStickTime_c = idata.eff.penetTime; //くっつき時間カウント　０リリース
        }
        //1:吸引効果
        if (cmn.efEl.suction_f == TRUE) cmn.efEl.suctionLv = idata.eff.suction;

        //7:引っ張り効果
        if (cmn.efEl.flyReverse_f == TRUE) cmn.efEl.flyReverseLv = idata.eff.flyReverse;

        //11:吹っ飛び緩効果
        if (cmn.efEl.flyLoose_f == TRUE) cmn.efEl.flyLooseLv = idata.eff.flyLoose;

        //12:世界一周効果
        if (cmn.efEl.rndWorld_f == TRUE) cmn.efEl.rndWorldLv = idata.eff.rndWorld;

        //12:キャッチスリップ
        if (cmn.efEl.catchSlip_f == TRUE) cmn.efEl.catchSlipLv = idata.eff.catchSlip;
        if (cmn.efEl.catchSlipR_f == TRUE) cmn.efEl.catchSlipRLv = idata.eff.catchSlipR;

        //109:真上吹っ飛び効果
        if ((cmn.efEl.flyVer.phase_f == TRUE) && (cmn.efEl.flyVer.incld_f == TRUE))
        {
            cmn.efEl.flyVer_f = TRUE;
            cmn.efEl.flyVerLv = idata.eff.flyVer_Lv[cmn.efEl.flyVer.times_c]; //109:真上吹っ飛び効果
        }

        //109:真上吹っ飛び効果
        if ((cmn.efEl.flyVer.phase_f == TRUE) && (cmn.efEl.flyVer.incld_f == TRUE))
        {
            cmn.efEl.flyVer_f = TRUE;
            cmn.efEl.flyVerLv = idata.eff.flyVer_Lv[cmn.efEl.flyVer.times_c]; //109:真上吹っ飛び効果
        }

        //109:真上吹っ飛び効果
        if ((cmn.efEl.flyVer.phase_f == TRUE) && (cmn.efEl.flyVer.incld_f == TRUE))
        {
            cmn.efEl.flyVer_f = TRUE;
            cmn.efEl.flyVerLv = idata.eff.flyVer_Lv[cmn.efEl.flyVer.times_c]; //109:真上吹っ飛び効果
        }


        //109:真上吹っ飛び効果
        if ((cmn.efEl.flyVer.phase_f == TRUE) && (cmn.efEl.flyVer.incld_f == TRUE))
        {
            cmn.efEl.flyVer_f = TRUE;
            cmn.efEl.flyVerLv = idata.eff.flyVer_Lv[cmn.efEl.flyVer.times_c]; //109:真上吹っ飛び効果
        }

        //110:真横吹っ飛び効果
        if ((cmn.efEl.flyHor.phase_f == TRUE) && (cmn.efEl.flyHor.incld_f == TRUE))
        {
            cmn.efEl.flyHor_f = TRUE;
            cmn.efEl.flyHorLv = idata.eff.flyHor_Lv[cmn.efEl.flyHor.times_c]; //110:真横吹っ飛び効果
        }

        //114:同一標的連続被弾効果
        if ((cmn.efEl.combo.phase_f == TRUE) && (cmn.efEl.combo.incld_f == TRUE))
        {
            cmn.efEl.combo_f = TRUE;
            cmn.efEl.combo_c = idata.eff.combo_Max[cmn.efEl.combo.times_c];//114:連続被弾効果＿最大コンボ数※
            cmn.efEl.comboNoHitTime_c = idata.eff.combo_Time[cmn.efEl.combo.times_c]; //114:同一標的連続被弾効果＿
            cmn.efEl.comboDam = idata.eff.combo_Dam[cmn.efEl.combo.times_c]; //114:同一標的連続被弾効果＿
        }
        else
        {
            cmn.efEl.combo_c = 0;//114:連続被弾効果＿最大コンボ数※
            cmn.efEl.comboNoHitTime_c = 0; //114:同一標的連続被弾効果＿
            cmn.efEl.comboDam = 0; //114:同一標的連続被弾効果＿
        }

        //122:上跳吹っ飛び効果
        if ((cmn.efEl.refVer.phase_f == TRUE) && (cmn.efEl.refVer.incld_f == TRUE))
        {
            cmn.efEl.refVer_f = TRUE;
            cmn.efEl.refVerLv = idata.eff.refVer_Lv[cmn.efEl.refVer.times_c]; //109:真上吹っ飛び効果
        }

        //123:横跳吹っ飛び効果
        if ((cmn.efEl.refHor.phase_f == TRUE) && (cmn.efEl.refHor.incld_f == TRUE))
        {
            cmn.efEl.refHor_f = TRUE;
            cmn.efEl.refHorLv = idata.eff.refHor_Lv[cmn.efEl.refHor.times_c]; //110:真横吹っ飛び効果
        }

        //その場ダウン
        if ((cmn.efEl.noMvDwn.phase_f == TRUE) && (cmn.efEl.noMvDwn.incld_f == TRUE))
        {
            cmn.efEl.noMvDwn_f = TRUE;
            cmn.efEl.noMvDwnLv = idata.eff.noMvDwn_Lv[cmn.efEl.noMvDwn.times_c]; //110:真横吹っ飛び効果
        }

        //ダメナシ
        if ((cmn.efEl.noDmg.phase_f == TRUE) && (cmn.efEl.noDmg.incld_f == TRUE)) cmn.efEl.noDmg_f = TRUE;

        //やまだ
        if ((cmn.efEl.phantom.phase_f == TRUE) && (cmn.efEl.phantom.incld_f == TRUE))
        {
            cmn.efEl.phantom_f = TRUE;
            idata.eff.phantomStep = phs_FOff;//山田初期化へ
        }
    }

    //段階別SEのデータを返す
    enSEID TStShot::GetSENum(s32 id)
    {
        enSEID res;
        enSEID soundNum = (enSEID)id;
        s32 soundDef = 0;
        if (soundNum > se_END)
        {
            soundDef = soundNum - se_END;
            res = (enSEID)(se_END + soundDef);
        }
        else res = soundNum;
        return res;
    }

    //SAngle********************************************************************************************

    //反れ縦・鋭角側反れポイント
    void TStShot::SAngleSoreTateAcute()
    {
        //crt.trg.awayY_Dirの取りうる値は-4～-2
        //球→標的の場合、
        //反れ縦角度により、術者～標的の足元間の１／３距離を敵位置から引いていく。
        //標的←球の場合は、↑「引く」が「足す」になる。
        agldt.incTimes = crt.trg.awayY_Dir + 1;//加算回数(実際は負の値が入る)
        //crt.trg.away
        agldt.soreTateDirX = phs.tagposX + agldt.incTimes * (phs.distX / 3.0f);
        agldt.soreTateDirY = 0.0f;// 地上
    }

    //反れ縦・鈍角側９０度まで反れポイント
    void TStShot::SAngleSoreTateObtuse90()
    {
        //crt.trg.awayY_Dirの取りうる値は+2～+3
        agldt.incTimes = crt.trg.awayY_Dir - 1;//加算回数
        agldt.soreTateDirX = phs.tagposX;
        //反れ縦角度により、術者水平方向～標的の頭上間の１／２距離を
        //足していく。地上シュートの場合は頭狙いになる
        agldt.soreTateDirY = phs.tagposY + agldt.incTimes * (phs.distY / 2.0f);
    }

    //反れ縦・鈍角側１３５度まで反れポイント
    void TStShot::SAngleSoreTateObtuse135()
    {
        //crt.trg.awayY_Dirの取りうる値は+4～+7
        agldt.incTimes = crt.trg.awayY_Dir - 3;//加算回数
        agldt.soreTateDirX = phs.tagposX;
        //反れ縦角度により、術者水平方向からphs.distX/4ずつ足していく。
        agldt.soreTateDirY = phs.posY + agldt.incTimes * (fabs(phs.distX) / 4.0f);
    }

    //反れ縦・鈍角側１８０度まで反れポイント
    void TStShot::SAngleSoreTateObtuse180()
    {
        //crt.trg.awayY_Dirの取りうる値は+8～+11
        agldt.incTimes = (f32)(crt.trg.awayY_Dir) - 7;//加算回数
        agldt.soreTateDirY = phs.posY + fabs(phs.distX);
        //反れ縦角度により、術者水平方向からphs.distX / 4分引いていく。
        agldt.soreTateDirX = phs.tagposX - agldt.incTimes * (phs.distX / 4.0f);
    }

    //●反れ縦反れポイント算出
    void TStShot::SoreTatePoint()
    {
        //標的と術者の位置関係で処理が変わるのでジャッジ
        if (phs.distX != 0) agldt.posXJudge = phs.distX / fabs(phs.distX);
        //＋１：球→標的、－１：標的←球

        switch (crt.trg.awayY_Dir){
        case -4:
        case -3:
        case -2:
            //0-2★標的手前～術者真下狙い
            SAngleSoreTateAcute(); break;
        case -1:
            //3★yは標的の股を狙う
            agldt.soreTateDirX = phs.tagposX;
            agldt.soreTateDirY = phs.tagposY - MATA;// 股狙い
            break;
        case 1:
        case 2:
            //5-6★標的頭～頭上
            SAngleSoreTateObtuse90(); break;
        case 3:
            //★水平狙い
            agldt.soreTateDirX = phs.tagposX;//標的x座標
            agldt.soreTateDirY = phs.posY;//シュート開始y座標
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            //★水平以上～術者垂直真上
            //★術者y方向：０度～４５度
            SAngleSoreTateObtuse135(); break;
        case 8:
        case 9:
        case 10:
        case 11:
            //★術者y方向：４５度～９０度
            SAngleSoreTateObtuse180(); break;
        default:
            //4★標的直狙い
            agldt.soreTateDirX = phs.tagposX;
            agldt.soreTateDirY = phs.tagposY + LOWLEVEL;
            break;
        }
    }


    //反れサイドについて
    //術者（O）の手前（S）奥（N）（サイド）と進行方向（E）に直線EO（＝手前奥線と直交）を引く
    //標的（T）と術者を結ぶ直線をTとして、標的がEOより手前にいる場合、
    //鋭角側（Acute）はTOS、鈍角側（Obtuse）はTONとなる。
    //★鋭角側
    //直線SOと直行するよう点Tから引いた直線TS上に鋭角側の反れサイドの反れポイントを持たせる
    //★鈍角９０度まで
    //直線EOに直行するよう点Tから引いた直線TE上に鈍角側９０度までの反れポイントを持たせる
    //★鈍角１３５度まで
    //直線EOに直行するよう点Tから引いた直線TE上に鈍角側９０度までの反れポイントを持たせる
    //直線EOと直線TEが交わる点をPとし、点Oと点Pの距離（＝術者・標的間x距離）分だけ
    //鈍角側に直線TEから延長してできたポイントを点Qとする。
    //直線PQを４分割してそこを反れポイントとする。
    //★鈍角１８０度まで
    //点Qから直線NOに直行する線を引き、それを４分割。
    //そこを反れポイントとする。
    //真正面の場合、鋭角側、鈍角９０度までは特殊処理を行う。

    //反れサイド素材の各処理
    //-4～-2　鋭角側処理
    //-1  鋭角側標的ヒット
    //0　標的に向かう（default処理）
    //1　鈍角側標的ヒット
    //2　鈍角側90度未満処理
    //3　鈍角側90度処理
    //4～7　鈍角側90度～135度 （7でX軸成分＝Z軸成分）
    //8～11　鈍角側135度～180度

    //反れサイド・鋭角側反れポイント
    void TStShot::SAngleSoreSideAcute()
    {
        //-4～-2　鋭角側処理
        //agldt.soreSideDirXには反れサイド角度により、
        //標的Xから術者～標的のx方向１／３距離を引いていく。（crt.trg.awayZ_Dir＜-1）
        //phs.distX＜0 すなわち”標的←球”の場合は負符号が相殺されるので同じ式でOK
        //反れサイド角度0なら標的側垂直方向（真北or真南）に投げる。
        agldt.incTimes = crt.trg.awayZ_Dir + 1;//加算回数
        agldt.soreSideDirX = phs.tagposX + agldt.incTimes * (phs.distX / 3.0f);
        agldt.soreSideDirZ = phs.tagposZ;// 標的側Z

        //真正面標的（同一Z）の場合
        if (agldt.posZJudge == 0)
        {
            //標的側垂直狙いの場合…鈍角・鋭角のジャッジができない時⇒真南へ
            if (crt.trg.awayZ_Dir == -4)
            {
                agldt.soreSideDirX = phs.posX;//X変動なし
                agldt.soreSideDirZ = phs.posZ + 16.0f;//取り敢えず現位置から１６手前側に設定
            }
            else
            {
                agldt.soreSideDirX = phs.tagposX;//X変動
                agldt.soreSideDirZ = phs.tagposZ + SIDE * (f32)crt.trg.awayZ_Dir * (-1.0f);
                //反れサイド-３ならagldt.SIDE１回足すことで
                //かならず手前側からアクセス
            }
        }

    }


    //反れサイド・鈍角側９０度まで反れポイント
    void TStShot::SAngleSoreSideObtuse90()
    {
        //2　鈍角側90度未満処理
        //3　鈍角側90度処理
        agldt.incTimes = crt.trg.awayZ_Dir - 1;//加算回数
        agldt.soreSideDirX = phs.tagposX;
        //反れサイド角度により、術者水平方向～標的のサイド間の１／２距離を
        //足していく。
        //標的手前なら標的位置からagldt.SIDEなどを引いていく。⇒標的から離れる。
        //標的奥なら標的位置からagldt.SIDEなどを足していく。⇒標的から離れる。
        agldt.soreSideDirZ = phs.tagposZ + (agldt.incTimes * fabs(phs.distZ / 2.0f)) * (f32)agldt.posZJudge * (-1.0f);
        //真正面の時は手前に少し逸れる
        if (agldt.posZJudge == 0) agldt.soreSideDirZ = phs.tagposZ + (SIDE * agldt.incTimes);
    }

    //反れサイド・鈍角側１３５度まで反れポイント
    void TStShot::SAngleSoreSideObtuse135()
    {
        //4～7　鈍角側90度～135度 （7でX軸成分＝Z軸成分）
        agldt.incTimes = crt.trg.awayZ_Dir - 3;//加算回数
        agldt.soreSideDirX = phs.tagposX;
        //反れサイド角度により、術者水平方向からphs.distX/4ずつ処理。
        //標的手前：phs.distX/4引いていく
        //標的奥：phs.distX/4足していく＝正面もこの処理
        agldt.soreSideDirZ = phs.posZ - agldt.incTimes * fabs(phs.distX / 4.0f) * (f32)agldt.posZJudge;
        if (agldt.posZJudge == 0) agldt.soreSideDirZ = phs.posZ + agldt.incTimes * fabs(phs.distX / 4.0f);
    }

    //反れサイド・鈍角側１８０度まで反れポイント
    void TStShot::SAngleSoreSideObtuse180()
    {
        //8～11　鈍角側135度～180度
        agldt.incTimes = crt.trg.awayZ_Dir - 7;//加算回数
        //敵手前なら引いて、敵奥なら足す
        agldt.soreSideDirZ = phs.posZ + fabs(phs.distX) * (f32)agldt.posZJudge * (-1.0f);
        //正面なら奥方向へ
        if (agldt.posZJudge == 0) agldt.soreSideDirZ = phs.posZ + fabs(phs.distX) * (-1.0f);

        //反れサイド角度により、術者水平方向からphs.distX / 4分引いていく。
        agldt.soreSideDirX = phs.tagposX - agldt.incTimes * (phs.distX / 4.0f);
    }

    //反れサイド反れポイント算出
    void TStShot::SoreSidePoint()
    {
        //標的と術者の位置関係で処理が変わるので先にジャッジしておく
        agldt.posZJudge = 0;//取り敢えず０を入れておく。↓で更新されない場合は距離Z＝０（０除算回避）
        if (phs.distZ != 0) agldt.posZJudge = phs.distZ / fabs(phs.distZ);
        //＋１：標的手前、－１：標的奥、０：正面（Z同一）

        //反れサイド素材の各処理
        //-4～-2　鋭角側処理
        //-1  鋭角側標的ヒット
        //0　標的に向かう（default処理）
        //1　鈍角側標的ヒット
        //2　鈍角側90度未満処理
        //3　鈍角側90度処理
        //4～7　鈍角側90度～135度 （7でX軸成分＝Z軸成分）
        //8～11　鈍角側135度～180度

        switch (crt.trg.awayZ_Dir)
        {
        case -4:
        case -3:
        case -2:
            //-4 ～ -2鋭角側処理
            SAngleSoreSideAcute(); break;
        case -1:
            //-1  鋭角側標的ヒット
            agldt.soreSideDirX = phs.tagposX;
            agldt.soreSideDirZ = phs.tagposZ + SIDE * agldt.posZJudge;// 判定Z幅SIDEを足し込む（相手の位置により符号が変わる）
            if (agldt.posZJudge == 0) agldt.soreSideDirZ = phs.tagposZ + SIDE;
            break;
        case 1:
            //1  鈍角側標的ヒット
            agldt.soreSideDirX = phs.tagposX;
            agldt.soreSideDirZ = phs.tagposZ - SIDE * agldt.posZJudge;// 判定Z幅SIDEを引く（相手の位置により符号が変わる）
            if (agldt.posZJudge == 0) agldt.soreSideDirZ = phs.tagposZ + SIDE;
            break;
        case 2:
        case 3:
            //★2-3鈍角90度まで
            SAngleSoreSideObtuse90(); break;
        case 4:
        case 5:
        case 6:
        case 7:
            SAngleSoreSideObtuse135(); break;
        case 8:
        case 9:
        case 10:
        case 11:
            //★術者z方向：４５度～９０度
            SAngleSoreSideObtuse180(); break;
        default:
            //0★標的直狙い
            agldt.soreSideDirX = phs.tagposX;
            agldt.soreSideDirZ = phs.tagposZ;
            break;
        }
    }


    //反れ縦・反れサイドの比率を求める
    void TStShot::SoreTateSideRate()
    {
        agldt.soreTateZeroCheck = FALSE;
        agldt.soreSideZeroCheck = FALSE;
        //agldt.speedDiv;

        //y／x比
        if (
            (agldt.soreTateDirX == phs.posX)//０除算回避
            ||
            (crt.trg.awayY_Dir == -4) //真下飛び
            ||
            (crt.trg.awayY_Dir == 11) //真上飛び
            )
        {
            agldt.soreTateZeroCheck = TRUE;
            agldt.soreTateRateXY = agldt.soreTateDirY - (f32)phs.posY;
        }
        else
        {
            agldt.soreTateRateXY = (agldt.soreTateDirY - (f32)phs.posY) / (agldt.soreTateDirX - (f32)phs.posX);
        }

        //z／x比
        if (
            (agldt.soreSideDirX == phs.posX)//０除算回避
            ||
            (crt.trg.awayZ_Dir == -4)//真南飛び
            ||
            (crt.trg.awayZ_Dir == 11)//真北飛び
            )
        {
            agldt.soreSideZeroCheck = TRUE;
            agldt.soreSideRateXZ = agldt.soreSideDirZ - (f32)phs.posZ;
        }
        else
        {
            agldt.soreSideRateXZ = (agldt.soreSideDirZ - (f32)phs.posZ) / (agldt.soreSideDirX - (f32)phs.posX);
        }
    }

    //SAnlge再計算用データ保存
    void TStShot::SetSAngleData(f32 dirX, f32 dirY, f32 dirZ, f32 vecX, f32 spdRtXY, f32 spdRtXZ, f32 spdDiv)
    {
        phs.dirX = dirX;//方向X（SAngle再計算用データ）
        phs.dirY = dirY;//方向Y（SAngle再計算用データ）
        phs.dirZ = dirZ;//方向Z（SAngle再計算用データ）
        phs.vecX = vecX;//単位ベクトルX（SAngle再計算用データ）
        phs.spdRtXY = spdRtXY;//速度XY比率（SAngle再計算用データ）
        phs.spdRtXZ = spdRtXZ;//速度XZ比率（SAngle再計算用データ）
        phs.spdDiv = spdDiv;//速度分割（SAngle再計算用データ）
    }

    //●x移動アリ
    void TStShot::SetSpeedXYZ()
    {
        //基本はX移動があれば問題ないのでまずそれを処理
        //反れ縦・反れサイド x!=0なら
        //xを１とした時のy、zの絶対値をx（＝１）と足しこんで速度パワーを分割
        f32 scaleVec = 1;//単位ベクトル⇒逆進の場合コレを－１にして逆方向に飛ばす
        f32 speedDiv = phs.spd / (1.0f + fabs(agldt.soreTateRateXY) + fabs(agldt.soreSideRateXZ));

        //逆進中ならXのみ逆に
        if (crt.trg.backward_Time > 0) scaleVec = -1;//逆進用単位方向ベクトル

        //Xが負の時、符号が逆転するための方向処理
        f32 dirX = 0;//一応Xの方向
        f32 dirY = 0;//Yの方向
        f32 dirZ = 0;//Zの方向

        if ((agldt.soreTateDirX - phs.posX) != 0) dirX = (agldt.soreTateDirX - phs.posX) / fabs(agldt.soreTateDirX - phs.posX);//Xの方向
        if ((agldt.soreTateDirY - phs.posY) != 0) dirY = (agldt.soreTateDirY - phs.posY) / fabs(agldt.soreTateDirY - phs.posY);//Yの方向
        if ((agldt.soreSideDirZ - phs.posZ) != 0) dirZ = (agldt.soreSideDirZ - phs.posZ) / fabs(agldt.soreSideDirZ - phs.posZ);//Zの方向

        phs.spdX = speedDiv * scaleVec * dirX;
        //	  phs.spdX = speedDiv * scaleVec * phs.distX / abs(phs.distX);
        //反れ縦・サイド比率の絶対値を取ることでxの符号による不具合を防止する
        phs.spdY = speedDiv * fabs(agldt.soreTateRateXY) * dirY;//反れ縦絶対値に方向を掛ける
        phs.spdZ = speedDiv * fabs(agldt.soreSideRateXZ) * dirZ;//反れサイド絶対値に方向を掛ける
        //SAngleデータ保存
        SetSAngleData(dirX, dirY, dirZ, scaleVec, fabs(agldt.soreTateRateXY), fabs(agldt.soreSideRateXZ), speedDiv);
    }

    //●y移動メイン
    void TStShot::SetSpeedYMain()
    {
        //反れ縦が０で、反れサイド！＝０なら
        //Yは現位置から指定位置へのベクトル…agldt.soreTateRateXY算出時に処理済
        //反れサイドにX成分があるのでX,Zでそれぞれ算出
        f32 speedDiv = 0;//分割した速度
        f32 scaleVec = 1;//単位ベクトル⇒逆進の場合コレを－１にして逆方向に飛ばす
        f32 dirX = agldt.soreSideDirX - (f32)phs.posX;//反れサイドX成分
        f32 dirY = agldt.soreTateRateXY;
        f32 dirZ = agldt.soreSideDirZ - (f32)phs.posZ;//反れサイドZ成分
        //Xが負の時狂うための単位方向ベクトル
        f32 scaleVecX = dirX / fabs(dirX);

        //反れ縦垂直系はXZ速度０に
        if ((crt.trg.awayY_Dir == -4) || (crt.trg.awayY_Dir == 11))
        {
            dirX = 0;//反れサイドX成分
            dirY = agldt.soreTateRateXY;
            dirZ = 0;//反れサイドZ成分
        }
        speedDiv = phs.spd / (fabs(dirX) + fabs(dirY) + fabs(dirZ));

        phs.spdX = speedDiv * dirX * scaleVec;
        phs.spdY = speedDiv * dirY;
        phs.spdZ = speedDiv * dirZ * scaleVecX;
        //SAngleデータ保存
        SetSAngleData(dirX, dirY, dirZ * scaleVecX, scaleVec, fabs(agldt.soreTateRateXY), fabs(agldt.soreSideRateXZ), speedDiv);
    }

    //●z移動メイン
    void TStShot::SetSpeedZMain()
    {
        //反れ縦が０で、反れサイド！＝０なら
        //Yは現位置から指定位置へのベクトル…agldt.soreTateRateXY算出時に処理済
        //反れサイドにX成分があるのでX,Zでそれぞれ算出
        f32 speedDiv = 0;//分割した速度
        f32 scaleVec = 1;//単位ベクトル⇒逆進の場合コレを－１にして逆方向に飛ばす
        f32 dirX = agldt.soreTateDirX - (f32)phs.posX;//反れサイドX成分
        f32 dirY = agldt.soreTateDirY - (f32)phs.posY;//反れサイドZ成分
        f32 dirZ = agldt.soreSideRateXZ;
        //Xが負の時狂うための単位方向ベクトル
        f32 scaleVecX = dirX / fabs(dirX);

        //反れ縦垂直系はXZ速度０に
        if ((crt.trg.awayZ_Dir == -4) || (crt.trg.awayZ_Dir == 11))
        {
            dirX = 0;//反れ縦X成分
            dirY = 0;//反れ縦Y成分
            dirZ = agldt.soreSideRateXZ;
        }
        speedDiv = phs.spd / (fabs(dirX) + fabs(dirY) + fabs(dirZ));


        phs.spdX = speedDiv * dirX * scaleVec;
        phs.spdY = speedDiv * dirY * scaleVecX;
        phs.spdZ = speedDiv * dirZ;
        //SAngleデータ保存
        SetSAngleData(dirX, dirY, dirZ * scaleVecX, scaleVec, fabs(agldt.soreTateRateXY), fabs(agldt.soreSideRateXZ), speedDiv);
    }

    //●yz移動
    void TStShot::SetSpeedYZ()
    {
        f32 speedDiv = 0;//分割した速度
        //x=0なのでx関連は全く必要としない
        f32 dirY = agldt.soreTateRateXY;
        f32 dirZ = agldt.soreSideRateXZ;

        speedDiv = phs.spd / (fabs(dirY) + fabs(dirZ));

        phs.spdX = 0;
        phs.spdY = speedDiv * dirY;
        phs.spdZ = speedDiv * dirZ;
        //SAngleデータ保存
        SetSAngleData(0, dirY, dirZ, 0, fabs(agldt.soreTateRateXY), fabs(agldt.soreSideRateXZ), speedDiv);
    }


    //速度分解⇒xyzに速度割り当て
    void TStShot::XYZSpeedDiv()
    {
        //一旦３方向それぞれの速度を０とする
        phs.spdX = 0;
        phs.spdY = 0;
        phs.spdZ = 0;

        //反れ縦反れサイド０チェック
        if (agldt.soreSideZeroCheck == FALSE)
        {
            //反れサイドでのX！＝0
            if (agldt.soreTateZeroCheck == FALSE)
            {
                SetSpeedXYZ();//●x移動アリ 基本はX移動があれば問題ないのでまずそれを処理
            }
            else {
                SetSpeedYMain();//●y移動メイン
            }
        }
        else {
            //反れサイドでX=0
            if (agldt.soreTateZeroCheck == FALSE)
            {
                SetSpeedZMain();//●z移動メイン
            }
            else {
                SetSpeedYZ();//●yz移動
            }
        }
    }

    //フラグの確認
    BOOL TStShot::IsShotEffFlag(enShotEffFlags shotEff)
    {
        return cmn.shotEff_f[shotEff];
    }
    //***効果系処理********************************************************

    //拍効果処理
    void TStShot::SetStopOrMoveByPulseEff()
    {
        //カウンタを進める
        phs.pulse_c = phs.pulse_c + 1;

        //初期化
        if (phs.pulse_Init_f == FALSE)//初期化
        {
            phs.pulse_Init_f = TRUE;//初期化済み
            phs.pulseShape_Init_f = FALSE;
            phs.pulse_Stop_f = FALSE;//動く
            phs.pulseByWave_f = FALSE;//波主導で動くか
            phs.pulseByWaveX_f = FALSE;//波主導で動くか
            phs.pulseByWaveY_f = FALSE;//波主導で動くか
            phs.pulseByWaveZ_f = FALSE;//波主導で動くか
            if (idata.eff.pulse_Type[cmn.efEl.pulse.times_c] == 2) phs.pulse_Stop_f = TRUE;//停止からスタートなのでTRUEへ
            phs.pulse_c = 0;//カウンタクリア
            if (idata.eff.pulse_TmStop[cmn.efEl.pulse.times_c] > 3) idata.eff.pulse_TmStop[cmn.efEl.pulse.times_c] = 3;
            if (idata.eff.pulse_TmStrt[cmn.efEl.pulse.times_c] > 3) idata.eff.pulse_TmStrt[cmn.efEl.pulse.times_c] = 3;
            phs.pulse_StopTime = 22 - idata.eff.pulse_TmStop[cmn.efEl.pulse.times_c] * 6;//停止時間
            phs.pulse_MoveTime = 22 - idata.eff.pulse_TmStrt[cmn.efEl.pulse.times_c] * 6;//移動時間
        }
        //波と関わり合いがありそうならその時間を移動・停止時間に割り当てる
        if (phs.waveXDivTime > 0)
        {
            phs.pulse_StopTime = phs.waveXDivTime + 1;
            phs.pulse_MoveTime = phs.waveXDivTime + 1;
        }
        if (phs.waveYDivTime > 0)
        {
            phs.pulse_StopTime = phs.waveYDivTime + 1;
            phs.pulse_MoveTime = phs.waveYDivTime + 1;
        }
        if (phs.waveZDivTime > 0)
        {
            phs.pulse_StopTime = phs.waveZDivTime + 1;
            phs.pulse_MoveTime = phs.waveZDivTime + 1;
        }

        //どれも波分割がない場合
        if (
            (phs.waveXDivTime == 0)
            &&
            (phs.waveYDivTime == 0)
            &&
            (phs.waveZDivTime == 0)
            )
        {//波があったら波に停止駆動を依存させるフラグを立てる
            if (crt.orb.waveX.wvtype != 0) phs.pulseByWave_f = TRUE, phs.pulseByWaveX_f = TRUE;
            if (crt.orb.waveY.wvtype != 0) phs.pulseByWave_f = TRUE, phs.pulseByWaveY_f = TRUE;
            if (crt.orb.waveZ.wvtype != 0) phs.pulseByWave_f = TRUE, phs.pulseByWaveZ_f = TRUE;
        }

        //停止・駆動の終わりフラグ
        BOOL checkStopEnd_f = FALSE;
        BOOL checkMoveEnd_f = FALSE;

        //動く⇔止まる切替
        if (phs.pulseByWave_f == FALSE)
        {//波メインでないとき
            //停止中にカウンタが上限超えたら動くへ
            if (phs.pulse_StopTime <= phs.pulse_c) checkStopEnd_f = TRUE;
            //移動中にカウンタが上限を超えたら停止へ
            if (phs.pulse_MoveTime <= phs.pulse_c) checkMoveEnd_f = TRUE;
        }
        else
        {//波メインのとき
            //象限数が切り替えの対象となる
            //動いている象限数･･･０なら１に
            s32 moveAnglePhase = idata.eff.pulse_TmStrt[cmn.efEl.pulse.times_c];
            if (moveAnglePhase < 1) moveAnglePhase = 1;
            //止まっている象限数･･０なら１に
            s32 stopAnglePhase = idata.eff.pulse_TmStop[cmn.efEl.pulse.times_c];
            if (stopAnglePhase < 1) stopAnglePhase = 1;
            //チェックする象限数･･･デフォでＸ方向
            s32 checkAnglePhase = crt.orb.waveX.angleRtTms * 4 + crt.orb.waveX.anglePhase;

            //Ｙ方向に波データがあればＹをチェック対象、次いでＺ、最後にＸ（デフォ）
            if (phs.pulseByWaveY_f == TRUE) checkAnglePhase = crt.orb.waveY.angleRtTms * 4 + crt.orb.waveY.anglePhase;
            else if (phs.pulseByWaveZ_f == TRUE) checkAnglePhase = crt.orb.waveZ.angleRtTms * 4 + crt.orb.waveZ.anglePhase;

            //停止・駆動の象限数を足し合わせて全体の象限数とする
            s32 anglePhaseAllNo = moveAnglePhase + stopAnglePhase;

            //全体象限数で、現在のチェック対象の象限数を割った余り・・・現在の象限数
            s32 nowAnglePhaseNo = checkAnglePhase % anglePhaseAllNo;

            //全体象限数は停止象限数＋駆動象限数なので現在の象限数が
            //停止象限数内であれば停止となる
            if (idata.eff.pulse_Type[cmn.efEl.pulse.times_c] == 2)
            {//停止からスタート
                //停止中にカウンタが上限超えたら動くへ
                if (stopAnglePhase == nowAnglePhaseNo) checkStopEnd_f = TRUE;
                //移動中にカウンタが上限を超えたら停止へ
                if ((checkAnglePhase != 0) && (nowAnglePhaseNo == 0))  checkMoveEnd_f = TRUE;
            }
            else
            {//駆動からスタート
                //停止中にカウンタが上限超えたら動くへ
                if (moveAnglePhase == nowAnglePhaseNo) checkMoveEnd_f = TRUE;
                //移動中にカウンタが上限を超えたら停止へ
                if ((checkAnglePhase != 0) && (nowAnglePhaseNo == 0))  checkStopEnd_f = TRUE;
            }


        }

        //停止中にカウンタが上限超えたら動くへ
        if ((phs.pulse_Stop_f == TRUE) && (checkStopEnd_f == TRUE))
        {
            phs.pulse_Stop_f = FALSE;//移動へ
            phs.pulse_c = 0;//カウンタクリア
            cmn.spdtShpModeChange_f = TRUE;//画像変更フラグ
            cmn.spdtShpMode = 0;
        }
        else
            if ((phs.pulse_Stop_f == FALSE) && (checkMoveEnd_f == TRUE))
                //移動中にカウンタが上限を超えたら停止へ
            {
                phs.pulse_Stop_f = TRUE;//停止へ
                phs.pulse_c = 0;//カウンタクリア
                cmn.spdtShpModeChange_f = TRUE;//画像変更フラグ
                cmn.spdtShpMode = 0;
            }

    }

    //トレース効果処理
    void TStShot::ResetSpdByTraceEff()
    {
        s32 lv = idata.eff.trace_Lv[cmn.efEl.trace.times_c];//トレースレベル
        f32 distX = phs.tagposX - phs.posX;
        f32 tgY = 14.0f;//標的狙い高さ・・・反れ縦１で頭狙い、反れ縦－1で股狙い
        if (crt.trg.awayY_Dir == 1) tgY = 24.0f;
        if (crt.trg.awayY_Dir == -1) tgY = 9.0f;
        if (lv >= 2)
        {
            if (cmn.tagFly_f == TRUE) tgY = 0.0f;//ふっとび中
            if ((cmn.targetYZero_f == TRUE) || (cmn.targetDead_f)) tgY = 0.0f;//しゃがみ、ダウン、天使中もY座標点狙い
        }
        f32 distY = (phs.tagposY + tgY) - phs.posY;

        //不味くなったら上に戻す！！
        //        if ((lv == 1) && (phs.posY <= 14)) distY = phs.tagposY - phs.posY;
        if ((lv == 1) && (phs.posY < tgY)) distY = phs.tagposY - phs.posY;
        f32 distZ = phs.tagposZ - phs.posZ;
        f32 distX_a = fabs(distX);
        f32 distY_a = fabs(distY);
        f32 distZ_a = fabs(distZ);
        f32 dirX = 1.0f;
        if (distX != 0.0f) dirX = distX / distX_a;
        f32 dirY = 1.0f;
        if (distY != 0.0f) dirY = distY / distY_a;
        f32 dirZ = 1.0f;
        if (distZ != 0.0f) dirZ = distZ / distZ_a;

        //初期のXZ方向メイン決め
        if (idata.eff.trace_init_f == FALSE)
        {
            idata.eff.trace_init_f = TRUE;
            idata.eff.trace_initDirZ_f = FALSE;
            if (distZ_a > distX_a) idata.eff.trace_initDirZ_f = TRUE;
        }

        //方向による減速を見越した分割前速度
        f32 spdAll = phs.spd;
        //メイン方向
        f32 mainDir = distX_a;
        if ((distY_a > distZ_a) && (distY_a > distX_a))
        {
            mainDir = distY_a;
            spdAll = spdAll * 0.8f;
        }
        if ((distZ_a > distY_a) && (distZ_a > distX_a))
        {
            mainDir = distZ_a;
            spdAll = spdAll * 0.7f;
        }
        //        f32 spdDv = mainDir / (distX_a + distY_a + distZ_a);
        f32 spdRatioTotal = distX_a + distY_a + distZ_a;
        if (spdRatioTotal == 0.0f)
        {
            phs.spdX = 0.0f;
            phs.spdY = 0.0f;//Yはレベルによって処理が変わる
            phs.spdZ = 0.0f;
        }

        BOOL tgBeyond_f = FALSE;//ターゲット追い越しフラグ
        if (idata.eff.trace_initDirZ_f == FALSE)
        {//トレース開始時Xメインの場合
            if (//右方向進行時　　
                //ゼロ距離ストップ回避用に　phs.spdX >= 0.0f、phs.posX > phs.tagposX、phs.posX < phs.tagposX　を変更
                ((phs.spdX > 0.0f) && (phs.posX >= phs.tagposX))
                ||//左方向進行時
                ((phs.spdX < 0.0f) && (phs.posX <= phs.tagposX))
                ||//ゼロ距離ストップ回避用
                ((phs.spdX == 0.0f) && (phs.posX == phs.tagposX))//ゼロ距離ストップ回避用
                ) tgBeyond_f = TRUE;
        }
        else
        {//トレース開始時Zメインの場合
            if (//奥方向進行時
                ((phs.spdZ > 0.0f) && (phs.posZ >= phs.tagposZ))
                ||//手前方向進行時
                ((phs.spdZ < 0.0f) && (phs.posZ <= phs.tagposZ))
                ||//ゼロ距離ストップ回避用
                ((phs.spdZ == 0.0f) && (phs.posZ == phs.tagposZ))//ゼロ距離ストップ回避用
                ) tgBeyond_f = TRUE;
        }

        if (lv >= 3) tgBeyond_f = FALSE;//Lv3以上なら追い越し処理はしないが
        if (cmn.targetDead_f == TRUE) tgBeyond_f = TRUE;//標的が死亡していたら追い越し状態とする

        //ターゲット追い越していないなら以下を処理
        //追い越しているならｓｐｄ書き替え不要
        if (tgBeyond_f == FALSE)
        {
            phs.spdX = spdAll * (lv / 3.0f) * distX / spdRatioTotal;
            phs.spdY = 0.0f;//Yはレベルによって処理が変わる
            f32 spdYCheck = spdAll * (lv / 3.0f)  * distY / spdRatioTotal;//一旦標的方向に向かう速度を出す
            //ｌｖ３なら問答無用、ｌｖ２以下の場合は下降のみ受け付ける
            if ((lv == 3) || (spdYCheck <= 0.0f)) phs.spdY = spdYCheck;
            phs.spdZ = spdAll * (lv / 3.0f)  * distZ / spdRatioTotal;
            f32 spdAll = fabs(phs.spdX) + fabs(phs.spdY) + fabs(phs.spdZ);
            f32 DEC_RATIO = 98.0f / 100.0f;//減少Lvがコレを切ったら速度代入をやめる
            if ((phs.spdTcPrAll * DEC_RATIO < spdAll) || (lv >= 3))
            {
                phs.spdTcPrX = phs.spdX;
                phs.spdTcPrY = phs.spdY;
                phs.spdTcPrZ = phs.spdZ;
                phs.spdTcPrAll = spdAll;
            }
            else
            {
                phs.spdX = phs.spdTcPrX;
                phs.spdY = phs.spdTcPrY;
                phs.spdZ = phs.spdTcPrZ;
                spdAll = phs.spdTcPrAll;
            }
        }
        else
        {//標的追い越しフラグが立っていて
            //標的が吹っ飛び中でない場合・・・敵が飛び越したとみなして速度を戻し飛ばす
            if ((cmn.traceAway_f == FALSE) && (cmn.tagFly_f == FALSE))
            {
                //                f32 spdRatioXZ = spdAll * (lv / 3.0f) / (fabs(phs.spdX) + fabs(phs.spdZ));
                //                phs.spdX = phs.spdX * spdRatioXZ;
                //                phs.spdY = 0;//Yはレベルによって処理が変わる
                //                phs.spdZ = phs.spdZ * spdRatioXZ;
                f32 spdRatioXZ = spdAll * (lv / 3.0f) / (fabs(phs.spdTcPrX) + fabs(phs.spdTcPrZ));
                phs.spdX = phs.spdTcPrX * spdRatioXZ;
                phs.spdY = 0;//Yはレベルによって処理が変わる
                phs.spdZ = phs.spdTcPrZ * spdRatioXZ;
            }
            else
            {
                if (//Lv３未満なら標的をスルーする処理
                    (lv < 3)
                    ||//Lv３以上でも死んでいたら標的をスルーする
                    ((cmn.targetDead_f == TRUE) && (lv >= 3))
                    )
                {
                    f32 spdRatioXZ = spdAll * (lv / 3.0f) / (fabs(phs.spdTcPrX) + fabs(phs.spdTcPrZ));
                    phs.spdX = phs.spdTcPrX * spdRatioXZ;
                    phs.spdZ = phs.spdTcPrZ * spdRatioXZ;
                }
            }
            cmn.traceAway_f = TRUE;
            //吹っ飛び中の場合は速度そのまま…ほぼ停滞
        }
    }

    //多角処理
    void TStShot::SetSpdByMltAngle()
    {
        //cmn.mltAngleSetSpd_f で セット待ちか次段階移行チェック状態かを判別する
        if (cmn.mltAngleSetSpd_f == TRUE)
        {//各段階の速度設定が済み→段階移行チェック
            switch (crt.chg.multiAngle_Num_c){
            case 0: CheckMltAngleInit(); break;//定点移動チェック
            case 1: CheckMltAngleFrst(); break;//初角移動チェック
            default: CheckMltAngleMain(); break;//多角処理頂点チェック
            }
        }

        if (cmn.mltAngleSetSpd_f == FALSE)
        {//各段階の速度設定がまだ→速度設定
            switch (crt.chg.multiAngle_Num_c){
            case 0: SetMltAngleInit(); break;//定点移動
            case 1: SetMltAngleFrst(); break;//初角移動
            default: SetMltAngleMain(); break;//多角処理移動
            }
        }
    }

    //多角処理…定点移動セット
    void TStShot::SetMltAngleInit()
    {
        //cmn.mltAngleから角数を割り当てる
        crt.trg.multiAngle_Num = (cmn.mltAngle * 2) + 1;//L1 3,L2 5,L3 7
        if (cmn.mltAngle > 5) crt.trg.multiAngle_Num = (cmn.mltAngle * 2) - 1;//L6 11,L7 13,L8 15 
        else if (cmn.mltAngle > 3) crt.trg.multiAngle_Num = cmn.mltAngle + 4;//L4 8,L5 9
        //角カウントを定点移動段階(0)にする
        crt.chg.multiAngle_Num_c = 0;

        s32 dir = 1;//加算するXの方向；右へのシュートならセンター＋DIF_Xとする
        //        if (phs.tagposX - phs.posX < 0) dir = -1;
        if (idata.side == 1) dir = -1;//外野シュート対応で投げたチームから向かうコートを決定。右チームが投げれば左へ

        //センターラインから少しずれたX位置の設定
        f32 dirPosX = 224.0f + MLTANGL_DIF_X * dir;
        cmn.mltAngleInitXDir = dirPosX;//ほかでも使うので保存

        //X情報
        f32 distX = dirPosX - phs.posX;
        f32 distX_a = fabs(distX);
        f32 dirX = 1;
        if (distX - distX_a != 0) dirX = -1;

        //Y情報
        f32 distY = MLTANGL_HGT_Y - phs.posY;
        f32 distY_a = fabs(distY);
        f32 dirY = 1;
        if (distY - distY_a != 0) dirY = -1;

        //Z情報
        f32 arrPosZ = MLTANGL_DPT_Z;
        if (crt.trg.multiAngle_Num == 3) arrPosZ = MLTANGL_DPT_Z + 12.0f;
        f32 distZ = arrPosZ - phs.posZ;
        f32 distZ_a = fabs(distZ);
        f32 dirZ = 1;
        if (distZ - distZ_a != 0) dirZ = -1;

        f32 divSpd = 1.0f;
        if ((distX_a == 0) && (distY_a == 0) && (distZ_a == 0))
        {
        }
        else
        {
            divSpd = phs.spd / (distX_a + distY_a + distZ_a);
            phs.spdX = divSpd * distX_a * dirX;
            phs.spdY = divSpd * distY_a * dirY;
            phs.spdZ = divSpd * distZ_a * dirZ;
        }
        cmn.mltAngleSetSpd_f = TRUE;//セット完了
    }

    //多角処理 定点移動→次段階チェック
    void TStShot::CheckMltAngleInit()
    {
        f32 arrPosZ = MLTANGL_DPT_Z;
        if (crt.trg.multiAngle_Num == 3) arrPosZ = MLTANGL_DPT_Z + 12.0f;

        //３点とも定点を超えたかのチェック
        BOOL beyond_f = CheckBeyond3Pos(phs.posX, phs.posY, phs.posZ,
            cmn.mltAngleInitXDir, MLTANGL_HGT_Y, arrPosZ,
            phs.spdX, phs.spdY, phs.spdZ);
        if (beyond_f == TRUE)
        {
            crt.chg.multiAngle_Num_c = 1;//０→１段階へ
            cmn.mltAngleSetSpd_f = FALSE;//次段階Spd未セット状態
        }
    }

    //多角処理…最初に曲がるまでの処理
    void TStShot::SetMltAngleFrst()
    {
        //タイムカウントを０にする
        crt.chg.multiAngle_Time_c = 0;

        //初期位置（XのみでOK）を保存する
        crt.chg.multiAngle_FtPosI = cmn.mltAngleInitXDir;

        //Xの方向（右コート方向）
        f32 dirX = 1.0f;
        //定点移動に使った値が右コート用値でないなら（＝左コート用値）左コート方向に飛ぶ
        if (cmn.mltAngleInitXDir != (224.0f + MLTANGL_DIF_X)) dirX = -1.0f;
        //XのみのSPDになるのでちょいと遅くする
        const f32 DIVSPD = 2.0f / 3.0f;
        phs.spdMltAngle = DIVSPD * phs.spd;
        phs.dirMltAngle = dirX;

        //速度設定　Xのみ生きる
        phs.spdX = DIVSPD * phs.spd * dirX;
        phs.spdY = 0;
        phs.spdZ = 0;

        //セット完了
        cmn.mltAngleSetSpd_f = TRUE;
    }

    //多角処理 最初に曲がるまでのチェック
    void TStShot::CheckMltAngleFrst()
    {
        //距離データ
        const f32 DIST_LIMIT = 86.0f;//X距離上限
        f32 distLimit = DIST_LIMIT;
        if (crt.trg.multiAngle_Num == 3) distLimit = DIST_LIMIT - 8.0f;//三角形のときはちょいと短く
        f32 distNow = fabs(crt.chg.multiAngle_FtPosI - phs.posX);//始点から現在の距離

        if (distNow >= distLimit)
        {//上限距離を超えたら次のポイントへ向かうのでその準備
            //角数＋１
            crt.chg.multiAngle_Num_c = crt.chg.multiAngle_Num_c + 1;//多分２になる
            //本段階でしていたタイムカウントを次段階以降の移動上限時間として設定
            crt.trg.multiAngle_Time = crt.chg.multiAngle_Time_c;
            //タイムカウントを0に
            crt.chg.multiAngle_Time_c = 0;
            //次段階初期化処理フラグ
            cmn.mltAngleSetSpd_f = FALSE;
        }
        //上限距離を超えていない場合はタイムカウントを加算
        else crt.chg.multiAngle_Time_c = crt.chg.multiAngle_Time_c + 1;
    }



    //多角処理…２段階目移行処理
    void TStShot::SetMltAngleMain()
    {
        //曲がる角度（180/n）の設定
        f32 turnAngle = PI / crt.trg.multiAngle_Num;
        //角数が偶数の場合、曲がる角度は２倍（現状８角形のみ対応）
        if (crt.trg.multiAngle_Num % 2 == 0) turnAngle = turnAngle * 2;

        //実際に使用する角度…上の曲がる角度を曲がるたびに足していく
        f32 trueAngle = turnAngle * (f32)(crt.chg.multiAngle_Num_c - 1);

        //方向スイッチ
        //角数が奇数の時はX,Zともに正(右向き時)
        f32 dirSwX = 1.0f; f32 dirSwZ = 1.0f;

        //角数が偶数の時はX,Zともに負(右向き時)
        if (crt.chg.multiAngle_Num_c % 2 == 0) dirSwX = -1.0f, dirSwZ = -1.0f;

        //実際の方向と速度
        phs.spdX = phs.spdMltAngle * phs.dirMltAngle * dirSwX * cos(trueAngle);
        phs.spdY = 0.0f;
        phs.spdZ = phs.spdMltAngle * dirSwZ * sin(trueAngle);

        //セット完了
        cmn.mltAngleSetSpd_f = TRUE;
    }

    //多角処理 n段階→n+1段階チェック
    void TStShot::CheckMltAngleMain()
    {
        if (crt.chg.multiAngle_Time_c >= crt.trg.multiAngle_Time)
        {//上限時間を超えたら次のポイントへ向かうのでその準備
            //角数＋１
            crt.chg.multiAngle_Num_c = crt.chg.multiAngle_Num_c + 1;//多分２になる
            //タイムカウントを0に
            crt.chg.multiAngle_Time_c = 0;
            //次段階初期化処理フラグ
            cmn.mltAngleSetSpd_f = FALSE;
        }
        //上限時間を超えていない場合はタイムカウントを加算
        else crt.chg.multiAngle_Time_c = crt.chg.multiAngle_Time_c + 1;
    }

    //指定点超えチェック
    BOOL TStShot::CheckBeyond3Pos(f32 posIX, f32 posIY, f32 posIZ, f32 posTX, f32 posTY, f32 posTZ, f32 spdX, f32 spdY, f32 spdZ)
    {
        BOOL beyondX_f = FALSE;
        BOOL beyondY_f = FALSE;
        BOOL beyondZ_f = FALSE;
        BOOL beyondAll_f = FALSE;
        if (CheckBeyondPos(posIX, posTX, spdX) == TRUE) beyondX_f = TRUE;
        if (CheckBeyondPos(posIY, posTY, spdY) == TRUE) beyondY_f = TRUE;
        if (CheckBeyondPos(posIZ, posTZ, spdZ) == TRUE) beyondZ_f = TRUE;
        if ((beyondX_f == TRUE) && (beyondY_f == TRUE) && (beyondZ_f == TRUE)) beyondAll_f = TRUE;
        return beyondAll_f;
    }
    BOOL TStShot::CheckBeyondPos(f32 posI, f32 posT, f32 spd)
    {
        BOOL res = FALSE;
        if (
            ((spd >= 0.0f) && (posI >= posT))
            ||
            ((spd < 0.0f) && (posI <= posT))
            )
            res = TRUE;
        return res;
    }

    //***グラフィック系処理**************************************************

    //波の方向でアニメを変える場合の処理
    BOOL TStShot::CheckWaveVec(BOOL ud)
    {
        s32 valX = crt.orb.waveX.width;
        s32 valY = crt.orb.waveY.width;
        s32 valZ = crt.orb.waveZ.width;

        //一番強い波を選定しその波にアニメを依存させる
        s32 val = valX;//ひとまずXを一番の波とする
        if (val < valY) val = valY;//X<YならYがイチバン
        if (val < valZ) val = valZ;//Zと上の勝者を比べてイチバンを決める

        //一番がどの方向かで処理を分ける…優先順位が高い方を先に処理している
        if (val == valZ) return CheckWaveVec(phs.addZ, phs.addZPr, ud);
        if (val == valY) return CheckWaveVec(phs.addY, phs.addYPr, ud);
        if (val == valX) return CheckWaveVec(phs.addX, phs.addXPr, ud);

        return FALSE;//念のため
    }

    //波の方向でアニメを変える場合の処理…共通処理
    BOOL TStShot::CheckWaveVec(f32 add, f32 addPr, BOOL ud)
    {

        if ((ud == FALSE) && ((add - addPr) < 0.0f))  return TRUE;//下降ジャッジ＆下降中
        if ((ud == TRUE) && ((add - addPr) > 0.0f))  return TRUE;//上昇ジャッジ＆上昇中
        return FALSE;//引っかからなかったらFALSE
    }

    //アニメタイプ毎のチェック
    BOOL TStShot::CheckShapeModeType(s32 mType)
    {
        BOOL judge = FALSE;
        switch (mType)
        {
        case 1: if (((phs.posY + phs.addY) > 0) && (cmn.groundElm_f == TRUE)) judge = TRUE; break;//空中
        case 2: if (((phs.posY + phs.addY) <= 0)) judge = TRUE; break;//地上
        case 4://上昇
            //波なし
            if ((phs.addX == 0.0f) && (phs.addY == 0.0f) && (phs.addZ == 0.0f))
            {
                //Y上昇
                if ((s32)phs.spdY > 0) judge = TRUE;
            }
            else
            {   //波有り…振幅の大きい波方向にアニメを依存させる
                judge = CheckWaveVec(TRUE);
            }

            break;
        case 5://下降
            //波なし
            if ((phs.addX == 0.0f) && (phs.addY == 0.0f) && (phs.addZ == 0.0f))
            {
                //Y下降
                if (phs.spdY <= 0.0f) judge = TRUE;
            }
            else
            {   //波有り…振幅の大きい波方向にアニメを依存させる
                judge = CheckWaveVec(FALSE);
            }
            break;
        case 6: if (phs.spdX == 0) judge = TRUE; break;//X停止
        case 7: if (phs.shpLv >= 4) judge = TRUE; break;//X停止
        default: judge = FALSE; break;
        }

        return judge;
    }

    //球の絵を変えるかどうかの判断
    void TStShot::CheckShapeMode(TDtBallAnime* pdtBlAm, TDtElement* pdtElem)
    {
        //必殺シュートなら処理をする
        if (cmn.hShot_f == TRUE)
        {

            s32 ShpId = pdtElem->GetShapeDt(phs.shpId, 0);//アニメセットID
            //現段階のデータが０の場合は保存した前段階のデータを引き継ぐ
            if (phs.shpLv == 0) ShpId = cmn.spdtGrhPrId;
            s32 AnmDataEx1Id = pdtBlAm->GetAniSet(ShpId, bad_shpExt1Id);//例外１のアニメデータID
            s32 AnmDataMode1 = pdtBlAm->GetAniData(AnmDataEx1Id, bad_anmTyp);//例外１のアニメタイプ
            if (AnmDataMode1 != 0)//例外１のアニメタイプが０以外ならチェック（例外にはタイプ０が入らないため）
            {
                s32 AnmDataEx2Id = pdtBlAm->GetAniSet(ShpId, bad_shpExt2Id);//例外２のアニメデータ
                s32 AnmDataMode2 = pdtBlAm->GetAniData(AnmDataEx2Id, bad_anmTyp);//例外２のアニメタイプ
                //例外１の条件満たしたら
                if (CheckShapeModeType(AnmDataMode1) == TRUE)
                {
                    if (cmn.spdtShpMode != 1)
                    {
                        cmn.spdtShpMode = 1;
                        cmn.spdtShpModeChange_f = TRUE;
                    }
                }
                else
                    //例外条件２満たしたら
                    if (CheckShapeModeType(AnmDataMode2) == TRUE)
                    {
                        if (cmn.spdtShpMode != 2)
                        {
                            cmn.spdtShpMode = 2;
                            cmn.spdtShpModeChange_f = TRUE;
                        }
                    }
                    else
                    {
                        //普通モードにもどす場合
                        if (cmn.spdtShpMode != 0) cmn.spdtShpModeChange_f = TRUE;
                        cmn.spdtShpMode = 0;
                    }

                //上昇・下降で速度が０になったらモードチェンジ
                if (((AnmDataMode1 == 4) || (AnmDataMode1 == 5) || (AnmDataMode2 == 4) || (AnmDataMode2 == 5))
                    && (phs.spdY == 0.0)
                    && (cmn.spdtShpMode != 0)
                    && (AnmDataMode2 != 0)//例外２まである場合も含む
                    && ((phs.addX == 0.0f) && (phs.addY == 0.0f) && (phs.addZ == 0.0f))//波無しの場合
                    )
                {
                    cmn.spdtShpMode = 0;
                    cmn.spdtShpModeChange_f = TRUE;
                }

            }
        }

    }

    //波依存アニメが依存する波方向を決める＆コマ番号をcmn.spdtPtnStepに入れてしまう
    void TStShot::SetShapeValFromWave(TDtBallAnime* pdtBlAm)
    {
        //アニメタイプが3＝波依存アニメ
        if (pdtBlAm->GetAniData(cmn.spdtAnmDataId, bad_anmTyp) == 3)
        {
            //波依存する波方向の決定
            //Z>Y>Xの優先順位
            if (crt.orb.waveZ.wvtype != 0)
            {
                cmn.spdtPtnStep = GetShapeWaveKoma(pdtBlAm, crt.orb.waveZ.triFuncVal);
            }
            else
                if (crt.orb.waveY.wvtype != 0)
                {
                    cmn.spdtPtnStep = GetShapeWaveKoma(pdtBlAm, crt.orb.waveY.triFuncVal);
                }
                else
                    if (crt.orb.waveX.wvtype != 0)
                    {
                        cmn.spdtPtnStep = GetShapeWaveKoma(pdtBlAm, crt.orb.waveX.triFuncVal);
                    }
        }

    }


    //波依存アニメの使用するコマ番号の取得
    s32 TStShot::GetShapeWaveKoma(TDtBallAnime* pdtBlAm, f32 triFuncVal)
    {
        //振動関数にて取りうる値は-1～+1。
        //その間に限界値を決めて０と対称にマイナス側にも限界値を設定。
        //＋側限界値・０・－側限界値の３つより4区分が作られる。
        //この区分で使用するアニメ番号を返す
        f32 LIMIT_VAL = 0.7f;//限界値設定

        if (triFuncVal > LIMIT_VAL)
        {
            return 0;
        }
        else
            if (triFuncVal >= 0.00f)
            {
                return 1;
            }
            else
                if (triFuncVal >= LIMIT_VAL * (-1.0f))
                {
                    return 2;
                }
                else
                {
                    return 3;
                }
    }

    //形状IDとLvの初期化
    void TStShot::InitShapeIdLv()
    {
        //pulse_Initしてないなら無条件、pulse_Initしてるなら1回のみ
        if (
            (phs.pulse_Init_f == FALSE)
            ||
            ((phs.pulse_Init_f == TRUE) && (phs.pulseShape_Init_f == FALSE))
            )
        {
            phs.shpId = idata.shp[cmn.spdtShpPhs].id;//現段階の形状ID
            phs.shpLv = idata.shp[cmn.spdtShpPhs].lv;//現段階の形状Lv
            phs.shpPulseMoveId = idata.shp[cmn.spdtShpPhs].id;//現段階の形状ID
            phs.shpPulseMoveLv = idata.shp[cmn.spdtShpPhs].lv;//現段階の形状Lv

            //７段階目を超えるとヤバイので
            //７段階目は次の段階を０段階としてPulse用次画像データを読み込む
            if (cmn.spdtShpPhs == 7)
            {
                phs.shpPulseStopId = idata.shp[0].id;//Pulse用に次の画像データを読み込んでおく
                phs.shpPulseStopLv = idata.shp[0].lv;//Pulse用に次の画像データを読み込んでおく
            }
            else
            {
                phs.shpPulseStopId = idata.shp[cmn.spdtShpPhs + 1].id;//Pulse用に次の画像データを読み込んでおく
                phs.shpPulseStopLv = idata.shp[cmn.spdtShpPhs + 1].lv;//Pulse用に次の画像データを読み込んでおく
            }

            phs.shpMtoS_f = idata.shp[cmn.spdtShpPhs].tf_f;//M-Sフラグ
            cmn.spdtShpMode = 0;//モード戻す

            phs.pulseShape_Init_f = TRUE;
        }
    }

    void TStShot::SetSlaveShapeData()
    {
        //MasterShapeにSlaveShapeがあった時の処理
        if (phs.shpMtoS_f == TRUE)
        {
            //idata.shptf[i].noより現段階のSlaveを探す
            for (s32 i = 0; i < ELMSHPNUM; ++i)
            {
                //現段階番号と一致するSlave側番号 ＆ 現IDとSlave側ID不一致 ＆ 未読み込みの配列
                if ((cmn.spdtShpPhs == idata.shptf[i].no) && (phs.shpId != idata.shptf[i].id) && (i > phs.shpTfNo))
                {
                    phs.shpId = idata.shptf[i].id;//現段階の形状ID
                    phs.shpLv = idata.shptf[i].lv;//現段階の形状Lv
                    cmn.spdtShpModeChange_f = TRUE;//形状のモードチェンジフラグをTRUEに（Init関数用）
                    phs.shpTfNo = i;//要素数を保存、次回コレ以下の要素数では検知されないように
                    break;
                }
            }
        }

    }

    //SlaveShapeデータの採用等
    void TStShot::SetNormalSlaveShapeId()
    {
        SetSlaveShapeData();//SlaveShapeのセット
        //該当データが見つからない場合 
        if (cmn.spdtShpModeChange_f == FALSE)
        {
            //Masterと違ったらMasterに戻す（Master同士だったらInit不要）
            if (phs.shpId != idata.shp[cmn.spdtShpPhs].id)
            {
                //見つからない場合
                InitShapeIdLv();//現段階MasterShapeで初期化
                cmn.spdtShpModeChange_f = TRUE;//形状のモードチェンジフラグをTRUEに（Init関数用）
                phs.shpTfNo = 0;//要素数を0にもどす
            }

            //最終段階で形状次配列のLvが０でない場合は更に形状データがある場合は形状用仮想段階を用意
            if ((cmn.spdtShpPhs == cmn.phaseAll) && (idata.shp[cmn.spdtShpPhs].lv != 0))
            {
                phs.shpVrPh_f = TRUE;//仮想段階突入へ
                phs.shpSlave_f = FALSE;//MasterShape
            }
        }
    }

    //過剰分変形素材処理
    void TStShot::SetExcessShapeId()
    {
        if (phs.shpSlave_f == TRUE)
        {
            //SlaveShapeData
            SetSlaveShapeData();//SlaveShapeのセット
            //該当データが見つからない場合 
            if (cmn.spdtShpModeChange_f == FALSE)
            {
                phs.shpPhAdd = phs.shpPhAdd + 1;//仮想段階を追加
                phs.shpSlave_f = FALSE;//MasterShape側の処理に行く為のフラグ
            }
        }
        //MasterShapeData
        if (phs.shpSlave_f == FALSE)
        {
            //正当な形状データが入っていたらそのデータを入れる
            if (idata.shp[cmn.spdtShpPhs + phs.shpPhAdd + 1].lv != 0)
            {
                phs.shpId = idata.shp[cmn.spdtShpPhs + phs.shpPhAdd + 1].id;//現段階の形状ID
                phs.shpLv = idata.shp[cmn.spdtShpPhs + phs.shpPhAdd + 1].lv;//現段階の形状Lv
                phs.shpMtoS_f = idata.shp[cmn.spdtShpPhs + phs.shpPhAdd + 1].tf_f;//M-Sフラグ
            }
            else
            {
                //次データがない場合は元の最終段階形状Master側に戻す
                InitShapeIdLv();//現段階MasterShapeで初期化
                phs.shpVrPh_f = FALSE;//仮想段階突入フラグOFF
                phs.shpPhAdd = 0;//仮想段階OFF
            }
            cmn.spdtShpModeChange_f = TRUE;//形状のモードチェンジフラグをTRUEに（Init関数用）
            phs.shpTfNo = 0;//要素数を0にもどす
            if (phs.shpMtoS_f == TRUE) phs.shpSlave_f = TRUE;//Slaveがあるなら次回来た時はSlave側に行くように
        }

    }

    //同段階中変形時の新たなIDをセット
    void TStShot::SetAnotherShapeId()
    {
        //＊＊＊通常処理
        //SlaveShapeのセット
        if (phs.shpPhAdd == 0) SetNormalSlaveShapeId();

        //＊＊＊形状過剰分の処理…過剰分は最終段階・Master⇒Slaveのあとにアニメを追加していく
        //形状用仮想段階始動
        if (phs.shpVrPh_f == TRUE) SetExcessShapeId();

        cmn.spdtShpMode = 0;//モード戻す
    }

    //画像初期化(段階変わるごとに初期化)関数
    void TStShot::InitShapeData(TDtBallAnime* pdtBlAm, TDtElement* pdtElem)
    {
        //必殺シュートなら処理をする
        if (cmn.hShot_f == TRUE)
        {

            //現形状段階番号（cmn.spdtShpNum）に則した形状番号
            //            cmn.spdtShpid = idata.shptf[cmn.spdtShpPhs].id;

            s32 sId = phs.shpId;//形状ID
            s32 sLv = phs.shpLv;//形状Lv

            //Pulse効果のときは特殊処理
            if ((cmn.efEl.pulse.incld_f == TRUE) && (cmn.efEl.pulse.phase_f == TRUE))
            {
                sId = phs.shpPulseMoveId;//形状ID
                sLv = phs.shpPulseMoveLv;//形状Lv
                if (phs.pulse_Stop_f == TRUE)
                {
                    sId = phs.shpPulseStopId;
                    sLv = phs.shpPulseStopLv;
                }
                phs.shpId = sId;
                phs.shpLv = sLv;
            }

            if (pdtElem->GetShapeDt(sId, 0) == 99) cmn.spdtGrhPrId = cmn.spdtShpid;

            cmn.spdtShpid = pdtElem->GetShapeDt(sId, 0);

            cmn.spdtShpLv = sLv;

            //現段階のデータが０の場合は保存した前段階のデータを引き継ぐ
            //もし、複数段階で且つ２段階目以上で未だにボールの姿を表してない場合は必殺ボールに
            if ((cmn.phaseAll > 0) && (cmn.phaseCrt > 0) && (cmn.ballAprAlr_f == FALSE))
            {
                if (sId == 0) sId = 1, sLv = 1;
                cmn.spdtShpid = pdtElem->GetShapeDt(sId, 0);
                cmn.spdtShpLv = 1;
                cmn.spdtVnsBal_f = FALSE;//球消しフラグ
                cmn.spdtVnsSdw_f = FALSE;//影消しフラグcmn.spdtGrhPrId
                cmn.ballAprAlr_f = TRUE;
                cmn.spdtGrhPrId = cmn.spdtShpid;
            }
            else if ((phs.shpId == 0) && (phs.shpLv == 0)) cmn.spdtShpid = cmn.spdtGrhPrId;

            //一旦特殊アニメフラグを消す
            cmn.spdtAnmDataIdSp_f = FALSE;

            //形状番号からアニメセットIDを読み込む
            //状態（cmn.spdtShpMode＋1）を参照してセット番号を取得
            cmn.spdtAnmDataId = pdtBlAm->GetAniSet(cmn.spdtShpid, (enBallAnimeSet)(cmn.spdtShpMode + 1));

            //もし特殊タイプ：１･･･Ｚシュートで形状が変わる系ならID再考
            if (pdtBlAm->GetAniSet(cmn.spdtShpid, bad_shpExAnTp) == 1)
            {
                if (//Ｚシュートシュートなら
                    (fabs(phs.spdZ) > (fabs(phs.spdX) * 3.5f))
                    //                    ||
                    //                    (cmn.shotNSOF_f == TRUE)
                    )
                {//特殊形状IDに切り替える
                    cmn.spdtAnmDataId = pdtBlAm->GetAniSet(cmn.spdtShpid, bad_shpOFAnId);
                    cmn.spdtAnmDataIdSp_f = TRUE;//特殊アニメフラグオン
                }
            }

            //アニメ番号からパターン番号を取得
            cmn.spdtAnmPattId = pdtBlAm->GetAniData(cmn.spdtAnmDataId, bad_anmPtID);

            SetShapeData(pdtBlAm);

        }
        else
        {
            //通常シュートのボール形状セット　7番
            cmn.spdtShpid = pdtElem->GetShapeDt(7, 0);
            cmn.spdtAnmDataId = pdtBlAm->GetAniSet(cmn.spdtShpid, (enBallAnimeSet)(cmn.spdtShpMode + 1));
            cmn.spdtAnmPattId = pdtBlAm->GetAniData(cmn.spdtAnmDataId, bad_anmPtID);
        }

#ifdef __K_DEBUG_SHIAI__
        // 原子のデータをログに書き込む
        //kdebug::DebugSystem::GetInstance()->AddShootDebugLogElementAtm(nowphase, sdata.phase_elms[nowphase][i2]);
#endif	// #ifdef __K_DEBUG_SHIAI__
    }

    //画像初期化(段階変わるごとに初期化)関数
    void TStShot::CheckShapeData(TDtBallAnime* pdtBlAm)
    {
        //        //必殺シュートなら処理をする
        //        if (cmn.hShot_f == TRUE)
        {
            //フレーム数
            s32 frameMax = pdtBlAm->GetAniData(cmn.spdtAnmDataId, (enBallAnimeData)(bad_anmFrm));
            s32 endStep = pdtBlAm->GetAniPatt(cmn.spdtAnmPattId, (enBallAnimePatt)(bad_pttLpEnd)) - 1;
            s32 srtStep = pdtBlAm->GetAniPatt(cmn.spdtAnmPattId, (enBallAnimePatt)(bad_pttLpSrt)) - 1;

            //カウントが指定フレーム数を超えたら⇒パターン段階プラス
            if (cmn.spdtFrame_c > frameMax)
            {
                cmn.spdtPtnStep = cmn.spdtPtnStep + 1;
                cmn.spdtFrame_c = 0;
            }
            //新たなパターン段階がループ終了パターン段階を超えたら⇒ループ開始パターン段階に
            if (cmn.spdtPtnStep > endStep)
            {
                cmn.spdtPtnStep = srtStep;
                cmn.spdtLoop_c = cmn.spdtLoop_c + 1;//ループカウント
                if (cmn.spdtLoop_c >= 4)//ループカウントが４以上になったら
                {
                    SetAnotherShapeId();//SlaveShapeや最終段階で使っていない画像を探して使用
                    cmn.spdtLoop_c = 0;//ループカウントを０に
                }
            }
        }
    }

    //画像初期化(段階変わるごとに初期化)関数
    void TStShot::SetShapeData(TDtBallAnime* pdtBlAm)
    {
        //フラグの初期化
        cmn.spdtRevVer_f = FALSE;//垂直反転フラグOFF
        cmn.spdtRevHor_f = FALSE;//水平反転フラグOFF
        cmn.spdtRevClr_f = FALSE;//色反転フラグOFF
        cmn.spdtVnsBal_f = FALSE;//球消しフラグOFF
        cmn.spdtVnsSdw_f = FALSE;//影消しフラグOFF

        //必殺シュートなら処理をする
        if (cmn.hShot_f == TRUE)
        {
            //指定のパターン番号から、使用する絵の番号1～4などを決めるが
            //先に波依存アニメーションタイプの場合の使用する絵番号の算出。
            SetShapeValFromWave(pdtBlAm);

            //指定のパターン番号から、使用する絵の番号1～4と反転の有無チェック
            //( + cmn.spdtPtnStep * 2)は段階ごとに２データあるため
            cmn.spdtAnmUseNum = pdtBlAm->GetAniPatt(cmn.spdtAnmPattId, (enBallAnimePatt)(bad_pttGr1Num + cmn.spdtPtnStep * 2));
            cmn.spdtAnmUseRevNum = pdtBlAm->GetAniPatt(cmn.spdtAnmPattId, (enBallAnimePatt)(bad_pttGr1Rev + cmn.spdtPtnStep * 2));

            //外部読み込み用データ
            //画像番号：現在のパターン番号を参照して読み込む
            cmn.spdtGrhNum = pdtBlAm->GetAniData(cmn.spdtAnmDataId, (enBallAnimeData)(bad_anmGrId1 + cmn.spdtAnmUseNum - 1));
            //垂直反転フラグ
            if (cmn.spdtAnmUseRevNum == 2) cmn.spdtRevVer_f = TRUE;
            //色反転フラグ カウンタが２で割って余りが２以上だったら色反転（）
            if ((pdtBlAm->GetAniData(cmn.spdtAnmDataId, bad_anmGrhClr) == 1)
                && ((cmn.spdtFrame_c % 4) >= 2))
            {
                cmn.spdtRevClr_f = TRUE;
            }
            //左右反転フラグ
            //速度から向きを取得し、反転フラグの有無のチェック。
            //速度０のときは標的－球位置より方向を決める
            f32 dirX = phs.tagposX - phs.posX;//速度０用方向
            if (
                //反転フラグあって
                ((cmn.spdtAnmUseRevNum == 1)
                && ((phs.spdX > 0) || ((phs.spdX == 0) && (dirX > 0))))//速度＋か、速度０＆標的＋位置
                ||
                //反転フラグなくて
                ((cmn.spdtAnmUseRevNum != 1)
                && ((phs.spdX < 0) || ((phs.spdX == 0) && (dirX < 0))))//速度－か。速度０＆標的－位置
                ) cmn.spdtRevHor_f = TRUE;

            //球＋影消しフラグ
            //            if ((cmn.spdtShpid >= 97) && (cmn.spdtShpid <= 99))
            if (cmn.spdtShpid == 99)
            {
                if (cmn.spdtShpLv == 1) cmn.spdtGrhNum = 130;//ワッカボール
                if (cmn.spdtShpLv >= 2) cmn.spdtVnsBal_f = TRUE;//球消しフラグ
                if (cmn.spdtShpLv >= 3) cmn.spdtVnsSdw_f = TRUE;//影消しフラグ
            }
            else
            {
                cmn.ballAprAlr_f = TRUE;
            }
            //                if (cmn.spdtShpLv >= 2) cmn.spdtVnsBal_f = TRUE;//球消しフラグ
            //                if (cmn.spdtShpLv >= 3) cmn.spdtVnsSdw_f = TRUE;//影消しフラグ
            cmn.spdtShpModeChange_f = FALSE;//形状のモードチェンジフラグを戻す
        }
        else
        {
            //通常シュート処理
            //指定のパターン番号から、使用する絵の番号1～4と反転の有無チェック
            //( + cmn.spdtPtnStep * 2)は段階ごとに２データあるため
            cmn.spdtAnmUseNum = pdtBlAm->GetAniPatt(cmn.spdtAnmPattId, (enBallAnimePatt)(bad_pttGr1Num + cmn.spdtPtnStep * 2));
            cmn.spdtAnmUseRevNum = pdtBlAm->GetAniPatt(cmn.spdtAnmPattId, (enBallAnimePatt)(bad_pttGr1Rev + cmn.spdtPtnStep * 2));

            //外部読み込み用データ
            //画像番号：現在のパターン番号を参照して読み込む
            cmn.spdtGrhNum = pdtBlAm->GetAniData(cmn.spdtAnmDataId, (enBallAnimeData)(bad_anmGrId1 + cmn.spdtAnmUseNum - 1));
            //左右反転フラグ
            if ((cmn.spdtAnmUseRevNum == 1) && (phs.spdX > 0))  cmn.spdtRevHor_f = TRUE;
            if ((cmn.spdtAnmUseRevNum != 1) && (phs.spdX <= 0))  cmn.spdtRevHor_f = TRUE;

        }

        //水平反転不可の絵なら強制的にFALSE
        if (pdtBlAm->GetAniData(cmn.spdtAnmDataId, (enBallAnimeData)(bad_anmSym)) == 1) cmn.spdtRevHor_f = FALSE;

    }

    //***分裂系処理**************************************************

    //分裂系処理
    void TStShot::SetSeparateBall()
    {
        if (idata.sprtDt[ssl_sinFqX].lv != 0) SetSeparateSinFq(ssl_sinFqX);//単振X
        if (idata.sprtDt[ssl_sinFqY].lv != 0) SetSeparateSinFq(ssl_sinFqY);//単振Y
        if (idata.sprtDt[ssl_sinFqZ].lv != 0) SetSeparateSinFq(ssl_sinFqZ);//単振Z

        if (idata.sprtDt[ssl_mulFqX].lv != 0) SetSeparateMulFq(ssl_mulFqX);//複振X
        if (idata.sprtDt[ssl_mulFqY].lv != 0) SetSeparateMulFq(ssl_mulFqY);//複振Y
        if (idata.sprtDt[ssl_mulFqZ].lv != 0) SetSeparateMulFq(ssl_mulFqZ);//複振Z

        if (idata.sprtDt[ssl_crclYZ].lv != 0) SetSeparateCircle(ssl_crclYZ);//円周YZ
        if (idata.sprtDt[ssl_crclXZ].lv != 0) SetSeparateCircle(ssl_crclXZ);//円周XZ
        if (idata.sprtDt[ssl_crclXY].lv != 0) SetSeparateCircle(ssl_crclXY);//円周XY

        if (idata.sprtDt[ssl_cr8VXY].lv != 0) SetSeparateCircle8(ssl_cr8VXY);//８周XY
        if (idata.sprtDt[ssl_cr8VXZ].lv != 0) SetSeparateCircle8(ssl_cr8VXZ);//８周XZ
        if (idata.sprtDt[ssl_cr8VYZ].lv != 0) SetSeparateCircle8(ssl_cr8VYZ);//８周YZ

        if (idata.sprtDt[ssl_cr8HXY].lv != 0) SetSeparateCircle8(ssl_cr8HXY);//∞周XY
        if (idata.sprtDt[ssl_cr8HXZ].lv != 0) SetSeparateCircle8(ssl_cr8HXZ);//∞周XZ
        if (idata.sprtDt[ssl_cr8HYZ].lv != 0) SetSeparateCircle8(ssl_cr8HYZ);//∞周YZ

        if (idata.sprtDt[ssl_shadow].lv != 0)//追跡
        {
            SetSeparateShadow(ssl_shadow, idata.sprtDt[ssl_shadow].init_f, idata.sprtDt[ssl_shadow].lv);
        }
    }

    void TStShot::InitSeparateData(enShapeSeparateLevel spType)
    {
        //空きの分裂用配列を探す
        s32 times = idata.sprtDt[spType].lv + 1;//分裂系はボールの数がLv+1になる
        if (spType == ssl_shadow) times = times - 1;//残像の場合はボールの数＝Lvなので-1する

        for (s32 i = 0; i < times; ++i)
        {
            //10の分裂配列から空きを探す
            for (s32 i2 = 0; i2 < SPARATE_MAX; ++i2)
            {
                if (phs.sprtDt[i2].use_f == FALSE)
                {
                    phs.sprtDt[i2].use_f = TRUE;
                    idata.sprtDt[spType].useDtNo[i] = i2;//使用する分裂配列の要素数を登録
                    idata.sprtDt[spType].dt_c = idata.sprtDt[spType].dt_c + 1;
                    break;
                }
            }
        }
        //必要なぶんまで入れることができなかったら満杯フラグを建てて処理をしない
        if (times != idata.sprtDt[spType].dt_c) idata.sprtDt[spType].full_f = TRUE;
    }

    //単振動分裂データセット
    void TStShot::SetSeparateSinFq(enShapeSeparateLevel spType)
    {
        SetSeparateSinFq(spType, idata.sprtDt[spType].init_f, idata.sprtDt[spType].lv, idata.sprtDt[spType].angle);
        idata.sprtDt[spType].angle = idata.sprtDt[spType].angle + 1;
    }

    //単振動分裂データセット
    void TStShot::SetSeparateSinFq(enShapeSeparateLevel spType, BOOL& init_f, s32 lv, s32 waveAngle)
    {
        //初期化
        if (init_f == FALSE)
        {
            InitSeparateData(spType);
            phs.realBlHide_f = TRUE;//実際のボールを消す
            init_f = TRUE;
        }
        //配列が満杯でなければ分裂処理を行う
        if (idata.sprtDt[spType].full_f == FALSE)
        {
            f32 waveWidth = (f32)lv * 8.0f;// 1/2コートサイズとwaveWidLvをかけあわせたものを振幅とする
            if (lv < 4.0f) waveWidth = 16 + (f32)(lv - 1) * 4.0f;
            f32 angleSpd = PI * (idata.sprtQuickLv * 1.5f + 1) / (30.0f);//1Frで6度
            f32 sinDt = sin((f32)waveAngle * angleSpd);
            //末端のボール座標に加える値を設定PosAが若い配列数に入る値
            f32 limitPosA = waveWidth * sinDt;//座標への追加
            f32 limitPosB = limitPosA * (-1);//反対側ボール加算値
            f32 addDt[SPARATE_MAX];//一旦保存用配列
            addDt[0] = limitPosA;//元となる末端ボール加算値

            //分割幅両端の距離をLVで割る（LV＋１が球数 をれを－１したものを分母とするためLVでOK）
            f32 divDist = fabs(limitPosA - limitPosB) / lv;
            for (s32 i = 0; i < lv; ++i)
            {
                f32 dir = 1.0f;//加算方向
                if (limitPosA > 0) dir = -1.0f;//始点が＋方向なら減算する
                addDt[i + 1] = limitPosA + (i + 1) * divDist * dir;
            }
            if (lv == 1) addDt[1] = limitPosB;

            for (s32 j = 0; j < lv + 1; ++j)
            {//使用する配列分処理する
                for (s32 j2 = 0; j2 < SPARATE_MAX; ++j2)
                {//分裂配列
                    if (idata.sprtDt[spType].useDtNo[j] == j2)
                    {//使用中の配列の番号と合致したら値を入れていく
                        switch (spType)
                        {
                        case ssl_sinFqX: phs.sprtDt[j2].addX = addDt[j]; break;
                        case ssl_sinFqY: phs.sprtDt[j2].addY = addDt[j]; break;
                        case ssl_sinFqZ: phs.sprtDt[j2].addZ = addDt[j]; break;
                        }
                        phs.sprtDt[j2].disp_f = TRUE;//表示フラグ
                        phs.sprtDt[j2].grphNo = cmn.spdtGrhNum;//画像番号
                        phs.sprtDt[j2].revHor_f = cmn.spdtRevHor_f;//左右反転
                        phs.sprtDt[j2].revVer_f = cmn.spdtRevVer_f;//上下反転
                        phs.sprtDt[j2].revClr_f = cmn.spdtRevClr_f;//色反転
                        phs.sprtDt[j2].vnsBal_f = cmn.spdtVnsBal_f;//玉消し
                        phs.sprtDt[j2].vnsSdw_f = cmn.spdtVnsSdw_f;//影消し
                    }
                }
            }
        }
    }

    //実データに入れる
    void TStShot::SetSeparateAddData(enShapeSeparateLevel spType, s32 lv, f32 addPDt[], f32 addQDt[])
    {
        for (s32 j = 0; j < lv + 1; ++j)
        {//使用する配列分処理する
            for (s32 j2 = 0; j2 < SPARATE_MAX; ++j2)
            {//分裂配列
                if (idata.sprtDt[spType].useDtNo[j] == j2)
                {//使用中の配列の番号と合致したら値を入れていく
                    switch (spType)
                    {
                    case ssl_mulFqX:
                    case ssl_crclYZ://円周YZ
                    case ssl_cr8VYZ://８周YZ
                        phs.sprtDt[j2].addY = addQDt[j];
                        phs.sprtDt[j2].addZ = addPDt[j];
                        break;
                    case ssl_mulFqY:
                    case ssl_crclXZ://円周XZ
                    case ssl_cr8VXZ://８周XZ
                        phs.sprtDt[j2].addX = addPDt[j];
                        phs.sprtDt[j2].addZ = addQDt[j];
                        break;
                    case ssl_mulFqZ:
                    case ssl_crclXY://円周XY
                    case ssl_cr8VXY://８周XY
                        phs.sprtDt[j2].addX = addPDt[j];
                        phs.sprtDt[j2].addY = addQDt[j];
                        break;
                    case ssl_cr8HXY://∞周XY
                        phs.sprtDt[j2].addX = addQDt[j];
                        phs.sprtDt[j2].addY = addPDt[j];
                        break;
                    case ssl_cr8HXZ://∞周XZ
                        phs.sprtDt[j2].addX = addQDt[j];
                        phs.sprtDt[j2].addZ = addPDt[j];
                        break;
                    case ssl_cr8HYZ://∞周YZ
                        phs.sprtDt[j2].addY = addPDt[j];
                        phs.sprtDt[j2].addZ = addQDt[j];
                        break;
                    }
                    phs.sprtDt[j2].disp_f = TRUE;//表示フラグ
                    phs.sprtDt[j2].grphNo = cmn.spdtGrhNum;//画像番号
                    phs.sprtDt[j2].revHor_f = cmn.spdtRevHor_f;//左右反転
                    phs.sprtDt[j2].revVer_f = cmn.spdtRevVer_f;//上下反転
                    phs.sprtDt[j2].revClr_f = cmn.spdtRevClr_f;//色反転
                    phs.sprtDt[j2].vnsBal_f = cmn.spdtVnsBal_f;//玉消し
                    phs.sprtDt[j2].vnsSdw_f = cmn.spdtVnsSdw_f;//影消し
                }
            }
        }
    }

    //複振動分裂データセット
    void TStShot::SetSeparateMulFq(enShapeSeparateLevel spType)
    {
        SetSeparateMulFq(spType, idata.sprtDt[spType].init_f, idata.sprtDt[spType].lv, idata.sprtDt[spType].angle);
        idata.sprtDt[spType].angle = idata.sprtDt[spType].angle + 1;
    }
    //複振動分裂データセット
    void TStShot::SetSeparateMulFq(enShapeSeparateLevel spType, BOOL& init_f, s32 lv, s32 waveAngle)
    {
        //初期化
        if (init_f == FALSE)
        {
            InitSeparateData(spType);
            phs.realBlHide_f = TRUE;//実際のボールを消す
            init_f = TRUE;
        }
        //配列が満杯でなければ分裂処理を行う
        if (idata.sprtDt[spType].full_f == FALSE)
        {
            f32 waveWidth = (f32)lv * 8.0f;// 1/2コートサイズとwaveWidLvをかけあわせたものを振幅とする
            if (lv < 4.0f) waveWidth = 16 + (f32)(lv - 1) * 4.0f;
            f32 angleSpd = PI * (idata.sprtQuickLv * 0.5f + 1) / (30.0f);//1Frで6度

            //一般座標軸P、Q。Pがメイン（方向を必要とするXがメイン）
            //***P座標***************************************************************
            //まずP座標を求める…X軸を想定する。
            //右進行なら0→+1なので+sin、左進行なら0→-1なので-sin
            f32 wvDir = 1;
            if (phs.spdX != 0) wvDir = phs.spdX / fabs(phs.spdX);//方向決定
            f32 sinDt = wvDir * sin((f32)waveAngle * angleSpd);
            //末端のボール座標に加える値を設定PosAが若い配列数に入る値
            f32 limitPosPA = waveWidth * sinDt;//座標への追加
            f32 limitPosPB = limitPosPA * (-1);//反対側ボール加算値
            f32 addPDt[SPARATE_MAX];//一旦保存用配列
            addPDt[0] = limitPosPA;//元となる末端ボール加算値

            //分割幅両端の距離をLVで割る（LV＋１が球数 をれを－１したものを分母とするためLVでOK）
            f32 divDistP = fabs(limitPosPA - limitPosPB) / lv;
            for (s32 i = 0; i < lv; ++i)
            {
                f32 dir = 1.0f;//加算方向
                if (limitPosPA > 0) dir = -1.0f;//始点が＋方向なら減算する
                addPDt[i + 1] = limitPosPA + (i + 1) * divDistP * dir;
            }
            if (lv == 1) addPDt[1] = limitPosPB;

            //***Q座標***************************************************************
            //向きによらず+1からスタートすればOKなのでCOSを使用
            f32 cosDt = cos((f32)waveAngle * angleSpd);
            //末端のボール座標に加える値を設定PosAが若い配列数に入る値
            f32 limitPosQA = waveWidth * cosDt;//座標への追加
            f32 limitPosQB = limitPosQA * (-1);//反対側ボール加算値
            f32 addQDt[SPARATE_MAX];//一旦保存用配列
            addQDt[0] = limitPosQA;//元となる末端ボール加算値

            //分割幅両端の距離をLVで割る（LV＋１が球数 をれを－１したものを分母とするためLVでOK）
            f32 divDistQ = fabs(limitPosQA - limitPosQB) / lv;
            for (s32 i = 0; i < lv; ++i)
            {
                f32 dir = 1.0f;//加算方向
                if (limitPosQA > 0) dir = -1.0f;//始点が＋方向なら減算する
                addQDt[i + 1] = limitPosQA + (i + 1) * divDistQ * dir;
            }
            if (lv == 1) addQDt[1] = limitPosQB;

            //***実座標へ***************************************************************
            SetSeparateAddData(spType, lv, addPDt, addQDt);
        }
    }



    //分裂円周データセット
    void TStShot::SetSeparateCircle(enShapeSeparateLevel spType)
    {
        SetSeparateCircle(spType, idata.sprtDt[spType].init_f, idata.sprtDt[spType].lv, idata.sprtDt[spType].angle);
        idata.sprtDt[spType].angle = idata.sprtDt[spType].angle + 1;
    }
    //分裂円周データセット
    void TStShot::SetSeparateCircle(enShapeSeparateLevel spType, BOOL& init_f, s32 lv, s32 waveAngle)
    {
        //初期化
        if (init_f == FALSE)
        {
            InitSeparateData(spType);
            phs.realBlHide_f = TRUE;//実際のボールを消す
            init_f = TRUE;
        }
        //配列が満杯でなければ分裂処理を行う
        if (idata.sprtDt[spType].full_f == FALSE)
        {
            f32 waveWidth = (f32)lv * 4.0f;// 1/2コートサイズとwaveWidLvをかけあわせたものを振幅とする
            if (lv < 4.0f) waveWidth = 8 + (f32)(lv - 1) * 2.0f;
            //            f32 angleSpd = PI / 30.0f;//1Frで6度
            f32 angleSpd = PI * (idata.sprtQuickLv * 0.5f + 1) / (30.0f);//1Frで6度

            //一般座標軸P、Q。Pがメイン（方向を必要とするXがメイン）
            //右進行なら0→+1なので+sin、左進行なら0→-1なので-sin
            f32 wvDir = 1;
            if (phs.spdX != 0) wvDir = phs.spdX / fabs(phs.spdX);//方向決定
            f32 addPDt[SPARATE_MAX];//一旦保存用配列
            f32 addQDt[SPARATE_MAX];//一旦保存用配列
            f32 angle = 0;//計算用角度

            //一番の球の角度から((360/(N+1))*回数i)度分を差し引いた角度に球を置いていく
            for (s32 i = 0; i < lv + 1; ++i)
            {
                angle = (f32)waveAngle * angleSpd - ((2.0f * PI) / (f32)(lv + 1)) * (f32)i;
                addPDt[i] = waveWidth * wvDir * sin(angle);//座標への追加
                addQDt[i] = waveWidth * cos(angle);//座標への追加
            }
            //実座標へ
            SetSeparateAddData(spType, lv, addPDt, addQDt);
        }
    }


    //分裂８の字データセット
    void TStShot::SetSeparateCircle8(enShapeSeparateLevel spType)
    {
        SetSeparateCircle8(spType, idata.sprtDt[spType].init_f, idata.sprtDt[spType].lv, idata.sprtDt[spType].angle);
        idata.sprtDt[spType].angle = idata.sprtDt[spType].angle + 1;
    }
    //分裂８の字データセット
    void TStShot::SetSeparateCircle8(enShapeSeparateLevel spType, BOOL& init_f, s32 lv, s32 waveAngle)
    {
        //初期化
        if (init_f == FALSE)
        {
            InitSeparateData(spType);
            phs.realBlHide_f = TRUE;//実際のボールを消す
            init_f = TRUE;
        }
        //配列が満杯でなければ分裂処理を行う
        if (idata.sprtDt[spType].full_f == FALSE)
        {
            //            f32 waveWidth = (f32)lv * 4.0f;// 1/2コートサイズとwaveWidLvをかけあわせたものを振幅とする
            //            if (lv < 4.0f) waveWidth = 8 + (f32)(lv - 1) * 2.0f;
            f32 waveWidth = 8 + (f32)lv * 3.0f;// 1/2コートサイズとwaveWidLvをかけあわせたものを振幅とする
            //            f32 angleSpd = PI / 10.0f;//1Frで6度
            f32 angleSpd = PI * (idata.sprtQuickLv * 0.5f + 1) / (30.0f);//1Frで6度

            //一般座標軸P、Q。Pがメイン（二回振動する軸（幅の小さい方）がメイン）
            //右進行なら0→+1なので+sin、左進行なら0→-1なので-sin
            f32 addPDt[SPARATE_MAX];//一旦保存用配列
            f32 addQDt[SPARATE_MAX];//一旦保存用配列
            f32 angle = 0;//計算用角度

            //一番の球の角度から((360/(N+1))*回数i)度分を差し引いた角度に球を置いていく
            for (s32 i = 0; i < lv + 1; ++i)
            {
                angle = (f32)waveAngle * angleSpd - ((2.0f * PI) / (f32)(lv + 1)) * (f32)i;
                addPDt[i] = waveWidth * sin(angle);//座標への追加
                addQDt[i] = waveWidth * sin(angle / 2.0f);//座標への追加
            }
            //実座標へ
            SetSeparateAddData(spType, lv, addPDt, addQDt);
        }
    }




    //残像関数
    void TStShot::SetSeparateShadow(enShapeSeparateLevel spType, BOOL& init_f, s32 lv)
    {
        //初期化
        if (init_f == FALSE)
        {
            InitSeparateData(spType);
            phs.realBlHide_f = FALSE;//実際のボールを消さない
            init_f = TRUE;
        }
        //配列が満杯でなければ分裂処理を行う
        if (idata.sprtDt[spType].full_f == FALSE)
        {
            for (s32 j = 0; j < lv; ++j)
            {//使用する配列分処理する
                for (s32 j2 = 0; j2 < SPARATE_MAX; ++j2)
                {//分裂配列
                    if (idata.sprtDt[spType].useDtNo[j] == j2)
                    {//使用中の配列の番号と合致したら値を入れていく→(j+1) * UPDATE_TIME Fr前のデータを読+み込む
                        s32 upDateTime = UPDATE_TIME;//アップデート時間
                        s32 checkDt = cmn.pastShpDt[(j + 1) * upDateTime].grphNo;//入れる予定の画像データ番号
                        if (j2 == 0) checkDt = cmn.spdtGrhNum;//最初のデータは現在の本体の画像番号を見る

                        switch (checkDt)
                        {//巨大系はアップデート時間を早めて保存
                        case 5:
                        case 6:
                        case 7:
                        case 8:
                        case 96:
                        case 97:
                        case 98:
                            upDateTime = UPDATE_TIME - 1;
                            break;
                        case 9://圧縮は時間を遅めてデータを入れる
                            upDateTime = UPDATE_TIME + 1;
                            break;
                        }
                        s32 no = (j + 1) * upDateTime;
                        phs.sprtDt[j2].posX = cmn.pastShpDt[no].posX * XYMAG;//位置
                        phs.sprtDt[j2].posY = cmn.pastShpDt[no].posY * XYMAG;//位置
                        phs.sprtDt[j2].posZ = cmn.pastShpDt[no].posZ * XYMAG;//位置
                        phs.sprtDt[j2].disp_f = cmn.pastShpDt[no].disp_f;//表示フラグ
                        phs.sprtDt[j2].grphNo = cmn.pastShpDt[no].grphNo;//画像番号
                        phs.sprtDt[j2].revHor_f = cmn.pastShpDt[no].revHor_f;//左右反転
                        phs.sprtDt[j2].revVer_f = cmn.pastShpDt[no].revVer_f;//上下反転
                        phs.sprtDt[j2].revClr_f = cmn.pastShpDt[no].revClr_f;//色反転
                        phs.sprtDt[j2].vnsBal_f = cmn.pastShpDt[no].vnsBal_f;//ボール消しフラグ
                        phs.sprtDt[j2].vnsSdw_f = cmn.pastShpDt[no].vnsSdw_f;//影消しフラグ
                        phs.sprtDt[j2].shadow_f = TRUE;//座標直入力なのでこれを立ててBaShotで計算をさせない
                    }
                }
            }

        }
    }

    //データ保存１
    void TStShot::SetPastData(s32 posX, s32 posY, s32 posZ)
    {
        //先に過去データをシフトする
        const s32 PAST_DATA_AR = PAST_DATA - 1;//配列数用にMAXから１を引いた値を使う
        for (s32 i = 0; i < PAST_DATA_AR; ++i)//iはPAST_DATA_AR-2まで（PAST_DATA_AR-1だと先の関数で-1が要素数に入るため）
        {
            RemovePastData(PAST_DATA_AR - i);//配列マックス-1 から処理を始める
        }
        //現在のデータを配列０に入れる
        //BaShotから読み込むため座標だけは引数から読む
        cmn.pastShpDt[0].posX = posX;//位置
        cmn.pastShpDt[0].posY = posY;//位置
        cmn.pastShpDt[0].posZ = posZ;//位置
        cmn.pastShpDt[0].disp_f = phs.realBlHide_f;//表示フラグ
        cmn.pastShpDt[0].grphNo = cmn.spdtGrhNum;//画像番号
        cmn.pastShpDt[0].revHor_f = cmn.spdtRevHor_f;//左右反転
        cmn.pastShpDt[0].revVer_f = cmn.spdtRevVer_f;//上下反転
        cmn.pastShpDt[0].revClr_f = cmn.spdtRevClr_f;//色反転
        cmn.pastShpDt[0].vnsBal_f = cmn.spdtVnsBal_f;//ボール消しフラグ
        cmn.pastShpDt[0].vnsSdw_f = cmn.spdtVnsSdw_f;//影消しフラグ
    }
    //データ保存２
    void TStShot::RemovePastData(s32 no)
    {
        cmn.pastShpDt[no].posX = cmn.pastShpDt[no - 1].posX;//位置
        cmn.pastShpDt[no].posY = cmn.pastShpDt[no - 1].posY;//位置
        cmn.pastShpDt[no].posZ = cmn.pastShpDt[no - 1].posZ;//位置
        cmn.pastShpDt[no].disp_f = cmn.pastShpDt[no - 1].disp_f;//表示フラグ
        cmn.pastShpDt[no].grphNo = cmn.pastShpDt[no - 1].grphNo;//画像番号
        cmn.pastShpDt[no].revHor_f = cmn.pastShpDt[no - 1].revHor_f;//左右反転
        cmn.pastShpDt[no].revVer_f = cmn.pastShpDt[no - 1].revVer_f;//上下反転
        cmn.pastShpDt[no].revClr_f = cmn.pastShpDt[no - 1].revClr_f;//色反転
        cmn.pastShpDt[no].vnsBal_f = cmn.pastShpDt[no - 1].vnsBal_f;//ボール消しフラグ
        cmn.pastShpDt[no].vnsSdw_f = cmn.pastShpDt[no - 1].vnsSdw_f;//影消しフラグ
    }

    //---山田の術関連------------------------------------------------------------
    void TStShot::SetPhantomData(TDtBallAnime* pdtBlAm, TDtElement* pdtElem)
    {
        switch (idata.eff.phantomStep)
        {
        case phs_FOff: SetPhantomDataInit(pdtBlAm, pdtElem); break;//最初
        case phs_Init: SetPhantomDataStart(pdtBlAm, pdtElem); break;//広がる
        case phs_Stay: SetPhantomDataWait(pdtBlAm, pdtElem); break;//停止
        case phs_Term: SetPhantomDataTerm(pdtBlAm, pdtElem); break;//終わり
        }
    }
    void TStShot::SetPhantomDataInit(TDtBallAnime* pdtBlAm, TDtElement* pdtElem)
    {
        //空きの分裂用配列を探す
        s32 times = 6;//分裂系はボールの数がLv+1になる

        //形状のソート…形状種類→６未満→形状Lv
        SortPhantomBallsData(pdtBlAm, pdtElem);

        for (s32 i = 0; i < times; ++i)
        {
            //各ボールのデータから絵情報をセット
            SetPhantomBallsData(i, pdtBlAm, pdtElem);

            //本体から離れる座標
            f32 addX = 0;
            f32 addY = 0;
            f32 addZ = 0;
            if (cmn.shotNSOF_f == FALSE)
            {
                switch (i)
                {
                case 0:addX = 0, addY = 0, addZ = -8; break;
                case 1:addX = 0, addY = 0, addZ = 8; break;
                case 2:addX = -16, addY = 0, addZ = -8; break;
                case 3:addX = -16, addY = 0, addZ = 8; break;
                case 4:addX = -16, addY = 0, addZ = 0; break;
                case 5:addX = -32, addY = 0, addZ = 0; break;
                }
                addX = addX + (mid::midGetRandGm(5) - 5) * 8;
                addY = addY + (mid::midGetRandGm(8) - 3) * 8;
                addZ = addZ + (mid::midGetRandGm(7) - 3) * 4;
            }
            else
            {
                switch (i)
                {
                case 0:addX = -16, addY = 0, addZ = 0; break;
                case 1:addX = 16, addY = 0, addZ = 0; break;
                case 2:addX = -16, addY = 16, addZ = 0; break;
                case 3:addX = 16, addY = 16, addZ = 0; break;
                case 4:addX = 0, addY = 16, addZ = 0; break;
                case 5:addX = 0, addY = 32, addZ = 0; break;
                }
                addX = addX + (mid::midGetRandGm(5) - 2) * 8;
                addY = addY + (mid::midGetRandGm(8) - 3) * 8;
            }
            f32 distX = phs.tagposX - phs.posX;
            f32 dirX = fabs(distX);
            if (dirX != 0) dirX = distX / dirX;

            phs.sprtDt[i].addMaxX = addX * dirX;
            phs.sprtDt[i].addMaxY = addY;
            phs.sprtDt[i].addMaxZ = addZ;
            phs.sprtDt[i].use_f = TRUE;
            //            phs.sprtDt[i].posX = (phs.posX) * XYMAG;//位置
            //            phs.sprtDt[i].posY = (phs.posY) * XYMAG;//位置
            //            phs.sprtDt[i].posZ = (phs.posZ) * XYMAG;//位置
            phs.sprtDt[i].posX = cmn.realBallPX;//位置
            phs.sprtDt[i].posY = cmn.realBallPY;//位置
            phs.sprtDt[i].posZ = cmn.realBallPZ;//位置
            phs.sprtDt[i].disp_f = TRUE;//表示フラグ
            phs.sprtDt[i].shadow_f = TRUE;//座標直入力なのでこれを立ててBaShotで計算をさせない

        }
        idata.eff.distRatioRealBall = 0.0f;
        idata.eff.phantomStep = phs_Init;
        idata.eff.startPhase = cmn.phaseCrt;
        idata.eff.decSpd = 0.0f;
    }
    void TStShot::SetPhantomDataStart(TDtBallAnime* pdtBlAm, TDtElement* pdtElem)
    {
        s32 times = 6;//分裂系はボールの数がLv+1になる
        idata.eff.distRatioRealBall = idata.eff.distRatioRealBall + 9.0f;
        if (idata.eff.distRatioRealBall >= 100) idata.eff.distRatioRealBall = 100.0f, idata.eff.phantomStep = phs_Stay;

        for (s32 i = 0; i < times; ++i)
        {
            //各ボールのデータから絵情報をセット
            SetPhantomBallsData(i, pdtBlAm, pdtElem);
            phs.sprtDt[i].use_f = TRUE;
            //            phs.sprtDt[i].posX = (phs.posX + phs.sprtDt[i].addMaxX * (idata.eff.distRatioRealBall / 100)) * XYMAG;//位置
            //            phs.sprtDt[i].posY = (phs.posY + phs.sprtDt[i].addMaxY * (idata.eff.distRatioRealBall / 100)) * XYMAG;//位置
            //            phs.sprtDt[i].posZ = (phs.posZ + phs.sprtDt[i].addMaxZ * (idata.eff.distRatioRealBall / 100)) * XYMAG;//位置
            phs.sprtDt[i].posX = (phs.sprtDt[i].addMaxX * (idata.eff.distRatioRealBall / 100)) * XYMAG + cmn.realBallPX;//位置
            phs.sprtDt[i].posY = (phs.sprtDt[i].addMaxY * (idata.eff.distRatioRealBall / 100)) * XYMAG + cmn.realBallPY;//位置
            phs.sprtDt[i].posZ = (phs.sprtDt[i].addMaxZ * (idata.eff.distRatioRealBall / 100)) * XYMAG + cmn.realBallPZ;//位置
            phs.sprtDt[i].disp_f = TRUE;//表示フラグ
            phs.sprtDt[i].shadow_f = TRUE;//座標直入力なのでこれを立ててBaShotで計算をさせない
        }

    }
    void TStShot::SetPhantomDataWait(TDtBallAnime* pdtBlAm, TDtElement* pdtElem)
    {
        s32 times = 6;//分裂系はボールの数がLv+1になる

        for (s32 i = 0; i < times; ++i)
        {
            //各ボールのデータから絵情報をセット
            SetPhantomBallsData(i, pdtBlAm, pdtElem);
            phs.sprtDt[i].use_f = TRUE;
            //            phs.sprtDt[i].posX = (phs.posX + phs.sprtDt[i].addMaxX) * XYMAG;//位置
            //            phs.sprtDt[i].posY = (phs.posY + phs.sprtDt[i].addMaxY) * XYMAG;//位置
            //            phs.sprtDt[i].posZ = (phs.posZ + phs.sprtDt[i].addMaxZ) * XYMAG;//位置
            phs.sprtDt[i].posX = (phs.sprtDt[i].addMaxX) * XYMAG + cmn.realBallPX;//位置
            phs.sprtDt[i].posY = (phs.sprtDt[i].addMaxY) * XYMAG + cmn.realBallPY;//位置
            phs.sprtDt[i].posZ = (phs.sprtDt[i].addMaxZ) * XYMAG + cmn.realBallPZ;//位置
            phs.sprtDt[i].disp_f = TRUE;//表示フラグ
            phs.sprtDt[i].shadow_f = TRUE;//座標直入力なのでこれを立ててBaShotで計算をさせない
        }
        if (idata.eff.startPhase < cmn.phaseCrt) idata.eff.phantomStep = phs_Term;
    }
    //山田終わりに向けた準備
    void TStShot::SetPhantomDataTerm(TDtBallAnime* pdtBlAm, TDtElement* pdtElem)
    {
        s32 times = 6;
        //中心ボールからの距離･･･２回目以降減衰値で引いていく
        idata.eff.distRatioRealBall = idata.eff.distRatioRealBall - idata.eff.decSpd;
        if (idata.eff.distRatioRealBall <= 0)
        {
            idata.eff.distRatioRealBall = 0.0f;
            if (idata.eff.endPhase < cmn.phaseCrt)
            {
                idata.eff.phantomStep = phs_FOff;
                cmn.efEl.phantom_f = FALSE;
                cmn.efEl.phantomEnd_f = TRUE;
            }
        }
        if (idata.eff.decSpd == 0)
        {
            f32 arrTime = 20.0f; //適当に２０Fr
            switch (cmn.maxSpdAx)
            {
            case sca_X: arrTime = fabs((phs.tagposX - phs.posX) / phs.spdX); break;
            case sca_Y: arrTime = fabs((phs.tagposY - phs.posY) / phs.spdY); break;
            case sca_Z: arrTime = fabs((phs.tagposZ - phs.posZ) / phs.spdZ); break;
            }
            idata.eff.decSpd = 100 / arrTime;//速度減算
            idata.eff.endPhase = cmn.phaseCrt;//最後の段階を記憶しておく
        }

        for (s32 i = 0; i < times; ++i)
        {
            //各ボールのデータから絵情報をセット
            SetPhantomBallsData(i, pdtBlAm, pdtElem);
            phs.sprtDt[i].use_f = TRUE;
            //            phs.sprtDt[i].posX = (phs.posX + phs.sprtDt[i].addMaxX * (idata.eff.distRatioRealBall / 100)) * XYMAG;//位置
            //            phs.sprtDt[i].posY = (phs.posY + phs.sprtDt[i].addMaxY * (idata.eff.distRatioRealBall / 100)) * XYMAG;//位置
            //            phs.sprtDt[i].posZ = (phs.posZ + phs.sprtDt[i].addMaxZ * (idata.eff.distRatioRealBall / 100)) * XYMAG;//位置
            phs.sprtDt[i].posX = (phs.sprtDt[i].addMaxX * (idata.eff.distRatioRealBall / 100)) * XYMAG + cmn.realBallPX;//位置
            phs.sprtDt[i].posY = (phs.sprtDt[i].addMaxY * (idata.eff.distRatioRealBall / 100)) * XYMAG + cmn.realBallPY;//位置
            phs.sprtDt[i].posZ = (phs.sprtDt[i].addMaxZ * (idata.eff.distRatioRealBall / 100)) * XYMAG + cmn.realBallPZ;//位置
            phs.sprtDt[i].disp_f = TRUE;//表示フラグ
            phs.sprtDt[i].shadow_f = TRUE;//座標直入力なのでこれを立ててBaShotで計算をさせない
        }

    }


    void TStShot::SetPhantomBallsData(s32 i, TDtBallAnime* pdtBlAm, TDtElement* pdtElem)
    {
        //フラグの初期化
        phs.sprtDt[i].revVer_f = FALSE;//垂直反転フラグOFF
        phs.sprtDt[i].revHor_f = FALSE;//水平反転フラグOFF
        phs.sprtDt[i].revClr_f = FALSE;//色反転フラグOFF
        phs.sprtDt[i].vnsBal_f = FALSE;//球消しフラグOFF
        phs.sprtDt[i].vnsSdw_f = FALSE;//影消しフラグOFF

        phs.sprtDt[i].sId = phs.shpPhantomSort[i].id;//形状ID
        if (phs.sprtDt[i].sId == 0)//０なら固定形状
        {
            s32 sNo = 3;
            switch (i){
            case 1: sNo = 2; break;
            case 2: sNo = 8; break;
            case 3: sNo = 9; break;
            case 4: sNo = 10; break;
            case 5: sNo = 34; break;
            default: sNo = 3; break;
            }
            phs.sprtDt[i].sId = sNo;//形状ID
        }
        phs.sprtDt[i].spdtShpid = pdtElem->GetShapeDt(phs.sprtDt[i].sId, 0);
        phs.sprtDt[i].spdtAnmDataId = pdtBlAm->GetAniSet(phs.sprtDt[i].spdtShpid, bad_shpNormId);
        //10tシュートのみ２番目の画像を使う
        if (phs.sprtDt[i].spdtShpid == 41) phs.sprtDt[i].spdtAnmDataId = pdtBlAm->GetAniSet(phs.sprtDt[i].spdtShpid, bad_shpExt1Id);
        phs.sprtDt[i].spdtAnmPattId = pdtBlAm->GetAniData(phs.sprtDt[i].spdtAnmDataId, bad_anmPtID);
        phs.sprtDt[i].spdtAnmUseNum = pdtBlAm->GetAniPatt(phs.sprtDt[i].spdtAnmPattId, (enBallAnimePatt)(bad_pttGr1Num + phs.sprtDt[i].spdtPtnStep * 2));
        phs.sprtDt[i].spdtAnmUseRevNum = pdtBlAm->GetAniPatt(phs.sprtDt[i].spdtAnmPattId, (enBallAnimePatt)(bad_pttGr1Rev + phs.sprtDt[i].spdtPtnStep * 2));
        //外部読み込み用データ
        //画像番号：現在のパターン番号を参照して読み込む
        phs.sprtDt[i].grphNo = pdtBlAm->GetAniData(phs.sprtDt[i].spdtAnmDataId, (enBallAnimeData)(bad_anmGrId1 + phs.sprtDt[i].spdtAnmUseNum - 1));
        //垂直反転フラグ
        if (phs.sprtDt[i].spdtAnmUseRevNum == 2) phs.sprtDt[i].revVer_f = TRUE;
        //色反転フラグ カウンタが２で割って余りが２以上だったら色反転（）
        if ((pdtBlAm->GetAniData(phs.sprtDt[i].spdtAnmDataId, bad_anmGrhClr) == 1)
            && ((phs.sprtDt[i].frame_c % 4) >= 2))
        {
            phs.sprtDt[i].revClr_f = TRUE;
        }
        //左右反転フラグ
        //速度から向きを取得し、反転フラグの有無のチェック。
        //速度０のときは標的－球位置より方向を決める
        f32 dirX = phs.tagposX - phs.posX;//速度０用方向
        if (
            //反転フラグあって
            ((phs.sprtDt[i].spdtAnmUseRevNum == 1)
            && ((phs.spdX > 0) || ((phs.spdX == 0) && (dirX > 0))))//速度＋か、速度０＆標的＋位置
            ||
            //反転フラグなくて
            ((cmn.spdtAnmUseRevNum != 1)
            && ((phs.spdX < 0) || ((phs.spdX == 0) && (dirX < 0))))//速度－か。速度０＆標的－位置
            ) phs.sprtDt[i].revHor_f = TRUE;

        //球＋影消しフラグ
        //            if ((cmn.spdtShpid >= 97) && (cmn.spdtShpid <= 99))
        //        if (cmn.spdtShpid == 99)
        //        {
        //            if (cmn.spdtShpLv == 1) cmn.spdtGrhNum = 130;//ワッカボール
        //            if (cmn.spdtShpLv >= 2) phs.sprtDt[i].vnsBal_f = TRUE;//球消しフラグ
        //            if (cmn.spdtShpLv >= 3) phs.sprtDt[i].vnsSdw_f = TRUE;//影消しフラグ
        //        }
        if (phs.sprtDt[i].spdtShpid == 99)
        {
            if (phs.shpPhantomSort[i].lv == 1) phs.sprtDt[i].grphNo = 130;//ワッカボール
            if (phs.shpPhantomSort[i].lv >= 2) phs.sprtDt[i].vnsBal_f = TRUE;//球消しフラグ
            if (phs.shpPhantomSort[i].lv >= 3) phs.sprtDt[i].vnsSdw_f = TRUE;//影消しフラグ
        }

        //水平反転ナシアニメなら強制的に反転オフ
        if (pdtBlAm->GetAniData(phs.sprtDt[i].spdtAnmDataId, bad_anmSym) == 1) phs.sprtDt[i].revHor_f = FALSE;

        //フレーム数チェックデータ
        s32 frameMax = pdtBlAm->GetAniData(phs.sprtDt[i].spdtAnmDataId, (enBallAnimeData)(bad_anmFrm));
        s32 endStep = pdtBlAm->GetAniPatt(phs.sprtDt[i].spdtAnmPattId, (enBallAnimePatt)(bad_pttLpEnd)) - 1;
        s32 srtStep = pdtBlAm->GetAniPatt(phs.sprtDt[i].spdtAnmPattId, (enBallAnimePatt)(bad_pttLpSrt)) - 1;
        //カウンタ＋
        phs.sprtDt[i].frame_c = phs.sprtDt[i].frame_c + 1;

        //カウントが指定フレーム数を超えたら⇒パターン段階プラス
        if (phs.sprtDt[i].frame_c > frameMax)
        {
            phs.sprtDt[i].spdtPtnStep = phs.sprtDt[i].spdtPtnStep + 1;
            //新たなパターン段階がループ終了パターン段階を超えたら⇒ループ開始パターン段階に
            if (phs.sprtDt[i].spdtPtnStep > endStep) phs.sprtDt[i].spdtPtnStep = srtStep;
            phs.sprtDt[i].frame_c = 0;
        }

    }

    //形状のソート…形状種類→６未満→形状Lv
    void TStShot::SortPhantomBallsData(TDtBallAnime* pdtBlAm, TDtElement* pdtElem)
    {
        s32 originBallNo = idata.shp[0].lv - 1;//本ボールのレベルを同素材合成数とする；減算用
        //形状素材として既に本ボールで使われているのでLvを１減らしておく

        //６種の分裂ボール形状を決定
        for (s32 i = 0; i < PHANTOM_MAX; ++i)
        {
            phs.shpPhantomSort[i].id = idata.shp[i + 1].id;//形状ID
            phs.shpPhantomSort[i].lv = idata.shp[i + 1].lv;//形状Lv
            phs.shpPhantomSort[i].no = idata.shp[i + 1].lv - 1;//形状No(同形状が合成された個数；減算用)

            //もしID＝０の場合はLv２以上のIDからデータをもらってそのデータはno(Lv)を１下げる
            if (phs.shpPhantomSort[i].id == 0)
            {
                for (s32 i2 = 0; i2 < PHANTOM_MAX; ++i2)
                {//６データからID０以外＆no１以上（Lv２以上）を探す
                    if ((phs.shpPhantomSort[i2].no > 0) && (phs.shpPhantomSort[i2].id > 0))
                    {
                        phs.shpPhantomSort[i].id = phs.shpPhantomSort[i2].id;//重複IDをソートデータに入れる
                        phs.shpPhantomSort[i].lv = phs.shpPhantomSort[i2].lv;
                        phs.shpPhantomSort[i2].no = phs.shpPhantomSort[i2].no - 1;//重複回数を減らす
                        break;
                    }
                }
                //該当データが見つからない場合は元ボールのIDと同素材合成数が生きているかをチェックし
                //生きていたら元ボールのデータをあてがう
                //                if ((phs.shpPhantomSort[i].id == 0) && (originBallNo > 0) && (idata.shp[0].id > 0))

                //該当データが見つからない場合で元ボールのIDが０以外の場合元ボールを複写する
                if ((phs.shpPhantomSort[i].id == 0) && (idata.shp[0].id > 0))
                {
                    phs.shpPhantomSort[i].id = idata.shp[0].id;//元ボールの重複IDをソートデータに入れる
                    phs.shpPhantomSort[i].lv = idata.shp[0].lv;
                    originBallNo = originBallNo - 1;
                }
                //それでも該当データが見つからない場合は、そのままID０とする→山田固定形状が後に選ばれる
            }
        }
    }


    //---山田の術関連------------------------------------------------------------



    //***分裂系処理**************************************************

    //***方向決め処理**************************************************

    //新　方向決め
    void TStShot::GetSpeedXYZ()
    {
        //距離と方向
        f32 distX = phs.tagposX - phs.posX;//距離X
        f32 distY = phs.tagposY - phs.posY + LOWLEVEL;//距離Y・・・標的Yを胸辺りとする＝地上で水平に飛ぶ処理
        f32 distZ = phs.tagposZ - phs.posZ;//距離Z

        //最大速度座標軸…初期化
        cmn.maxSpdAx = sca_Zero;
        enMaxSpdCoordinateAxis maxSpdDir = sca_Zero;//この関数内で各処理へ飛ばす用
        agldt.distX_a = fabs(distX);//絶対値距離X
        agldt.distY_a = fabs(distY);//絶対値距離Y
        agldt.distZ_a = fabs(distZ);//絶対値距離Z
        agldt.dirX = 0;//方向X
        if (distX != 0.0f) agldt.dirX = distX / agldt.distX_a;
        agldt.dirY = 0;//方向Y
        if (distY != 0.0f) agldt.dirY = distY / agldt.distY_a;
        agldt.dirZ = 0;//方向Z
        if (distZ != 0.0f) agldt.dirZ = distZ / agldt.distZ_a;

        //XZ比率
        agldt.ratioXZ = GetRatioFromAngle(FALSE, distX, distZ, crt.trg.awayZ_Dir, agldt.ignXZ_f);//XZ比率
        //XY比率
        agldt.ratioXY = GetRatioFromAngle(TRUE, distX, distY, crt.trg.awayY_Dir, agldt.ignXY_f);//XY比率
        //YZ比率                                                  
        //Z=0 のとき（ratioXZ＝0）の対応としてagldt.ratioXYを入れておく
        agldt.ratioYZ = agldt.ratioXY;
        //Z!=0 なら Y/Zが成立するので割る
        if (agldt.ratioXZ != 0) agldt.ratioYZ = agldt.ratioXY / agldt.ratioXZ;

        //Yが標的非依存の場合、Zを求める際XZではなくYZを使用するためYZの値を変更
        if (//-2以下or3以上
            ((crt.trg.awayY_Dir <= -2) || (crt.trg.awayY_Dir >= 3))
            &&
            (agldt.ratioXZ > 1.0f)
            )
        {
            SetRatioYZIndepOfX(distY, distZ);
        }



        //YZ比率　サイドラインへ飛ぶ際のYZ（ｙ/ｚ）比率の設定
        agldt.ratioYZ_SLineSlZ0_f = FALSE;//0フラグオフ
        agldt.ratioYZ_SLineDpZ0_f = FALSE;//0フラグオフ
        agldt.ratioYZ_SLineSl = GetRatioForSideLine(FALSE, agldt.distY_a, agldt.ratioYZ_SLineSlZ0_f);
        agldt.ratioYZ_SLineDp = GetRatioForSideLine(TRUE, agldt.distY_a, agldt.ratioYZ_SLineDpZ0_f);

        //Xの比率は1とする
        const f32 PRM_X = 1.0f;

        //絶対値距離比較
        BOOL cmprXY = TRUE;
        //      //XYが１より大きい…X<Y　or　XY＝０　…X少
        //      if ((PRM_X < agldt.ratioXY) || (agldt.ignXY_f == TRUE))  cmprXY = FALSE;
        //XYが１より大きい…X少
        if (PRM_X < agldt.ratioXY)  cmprXY = FALSE;
        BOOL cmprXZ = TRUE;
        //      //XZが１より大きい…X<Z　or　XZ＝０　…X少
        //      if ((PRM_X < agldt.ratioXZ) || (agldt.ignXZ_f == TRUE))  cmprXZ = FALSE;
        //XZが１より大きい……X少
        if (PRM_X < agldt.ratioXZ)  cmprXZ = FALSE;
        BOOL cmprYZ = TRUE;
        //      //XY < XZ　…Y少
        //      if (agldt.ratioXY < agldt.ratioXZ) cmprYZ = FALSE;
        //YZ(y/z)が１より大きい……Z少
        if (PRM_X < agldt.ratioYZ)  cmprYZ = FALSE;

        //地上で反れ縦下降系の場合はＹ優先になりかねないのでX>Yとする
        if ((phs.posY == 0.0f) && (crt.trg.awayY_Dir < 0))
        {
            cmprXY = TRUE;
            agldt.groundSAngle_f = TRUE;//SAngle上の地上フラグ→ｙ速度を負にする
        }

        //標的素材がXZ速度維持を指定していない場合
        if (phs.changeMtdFromTarget != 3)
            //        if (cmn.changeMtdPr != 3)
        {

            if (cmprXY == TRUE) // X > Y(X：一位か二位)
            {
                // X > Z
                if (cmprXZ == TRUE) maxSpdDir = sca_X;//X優位(X：一位) X > ( Y or Z) 
                else maxSpdDir = sca_Z;//Z優位(X：二位) Z > X > Y
            }
            else //if (cmprXY == FALSE) Y > X(X：三位か二位)
            {
                if (cmprXZ == FALSE)// Z > X(X：三位) → (Y or Z) > X   YZ頂上決定戦
                {
                    //ｙ/ｚ　< 1.0 → Z > Y　
                    if (cmprYZ == TRUE) maxSpdDir = sca_Z;//Z優位
                    else maxSpdDir = sca_Y;//Y優位
                }
                else maxSpdDir = sca_Y;//Y優位 Y > X > Z   X > Z (X：二位) Y1位 Z3位
            }

            //Y=0、Z=0、X距離０の場合はXYZ＝０とし、敵側コートに飛ぶよう処理する
            if ((agldt.distX_a + agldt.distY_a + agldt.distZ_a) == 0.0f) maxSpdDir = sca_Zero;
            if ((agldt.ratioXY == 0.0f) && (agldt.ratioXZ == 0.0f) && (distX == 0.0f)) maxSpdDir = sca_Zero;

            if ((crt.trg.awayY_Dir == 11) || (crt.trg.awayY_Dir == -4)) maxSpdDir = sca_Y;//Y優位
            if ((crt.trg.awayZ_Dir == 11) || (crt.trg.awayZ_Dir == -4)) maxSpdDir = sca_Z;//Z優位


            //            if ((crt.chg.pos == 2) && (crt.trg.backward_Time != 0));
            //標的位置を見るような反れ縦で逆進なら敵対面外野上空に定点移動
            if ((crt.trg.awayY_Dir >= -1) && (crt.trg.backward_Time != 0)) SetSpdBackward();
            else if (crt.trg.trgSelect != 0) SetSpdFromObjPos();
            //２・３番外野の場合は外野シュートへ
            else if (((cmn.shotNSOF_f == TRUE))
                //            else if (((cmn.shotNSOF_f == TRUE) && (maxSpdDir != sca_X))
                || (cmn.shotNSOFZ_f == TRUE))
            {
                //外野北側or南側シュート
                //外野シュートで密着の場合
                if (maxSpdDir == sca_Zero)
                {
                    //XYZ＝０成立時にはとりあえず敵コート側に飛ばす。意図して停止中の場合はBaShotで処理しているかと。
                    //idata.side：シューターのコート　０左、１右　
                    //だが、速度の方向に使うので変換。０左→右＝＋１　１左←右＝－１
                    f32 dirZ = 1.0f;//0なら1　1なら2にして↓で-1にする
                    if (cmn.shotFromNorthOF_f == TRUE) dirZ = -1.0f;//北側からのシュートならマイナスに

                    phs.spdX = 0.0f;
                    phs.spdY = 0.0f;
                    phs.spdZ = phs.spd * DEC_SPD_LV3 * phs.spdDecFromWave * dirZ;//トータル速度にコート情報から得られた方向をかける。
                    //SAngleデータ保存
                    SetSAngleData(0.0f, 0.0f, dirZ, 0.0f, 0.0f, dirZ, phs.spd);
                    //最大速度座標軸…波で使用
                    cmn.maxSpdAx = sca_Z;

                    //予定到着点
                    phs.arrPosX = phs.posX;
                    phs.arrPosY = phs.posY;
                    phs.arrPosZ = phs.posZ;
                }
                else
                {
                    SetSpdOFSNShoot();
                }
            }
            else
            {//逆進じゃないなら諸々方向を確認
                switch (maxSpdDir){
                case sca_X: SetSpdXYZ(); break;//X優位
                case sca_Y: SetSpdYMain(); break;//Y優位
                case sca_Z: SetSpdZMain(); break;//Z優位
                default:SetSpdZeroToX(); break;//XYZ＝０処理
                }
            }

        }
        else
            //前段階標的素材がXZ速度維持を指定してる場合
        {
            f32 distY = phs.tagposY - phs.posY + LOWLEVEL;//距離Y
            f32 dirY = GetDirFromPosSore(distY, crt.trg.awayY_Dir, TRUE);

            //参照するspd＝基本X：Y
            f32 spdRef = fabs(phs.spdX * agldt.ratioXY);
            //X<Zなら　参照するspd＝Z：Y
            if (cmprXZ == FALSE) spdRef = phs.spdZ * fabs(agldt.ratioYZ);

            phs.spdY_ChangeYOnly = spdRef * dirY;//反れ縦絶対値に方向を掛ける
        }
        //-----------------------------------------------------------------------------------------------------------
        /*
                // 文字列用
                std::string str;
                std::string str_log;
                char str_c[256];
                std::string str_new = "New\n";		// 最新データにはNewの文字を

                // 読み込みモード
                FILE *pFile = fopen("k_debuglog.txt", "r+");
                if (pFile)
                {
                // ファイル内の過去分を保存
                while (fgets(str_c, 256, pFile) != NULL) {
                if (!str_new.compare(str_c)) continue;
                str_log += str_c;
                }
                // 読み出しモード終了
                fclose(pFile);


                }

                // 書き込みモード
                pFile = fopen("k_debuglog.txt", "w");
                assert(pFile);

                // 今回の分
                str = "/------------------------------------------------------------------/\n";
                str += str_new;
                str += "ratio\n";
                sprintf_s(str_c, "Time = %d\n", cmn.timeTotal_c);
                str += str_c;
                sprintf_s(str_c, "PhaseNum = %d\n", cmn.phaseCrt);
                str += str_c;
                sprintf_s(str_c, "ratioXZ = %f\n", agldt.ratioXZ);
                str += str_c;
                sprintf_s(str_c, "ratioXY = %f\n", agldt.ratioXY);
                str += str_c;
                sprintf_s(str_c, "ratioXY = %f\n", agldt.ratioYZ);
                str += str_c;
                str += "\nspd\n";
                sprintf_s(str_c, "phs.spdX = %f\n", phs.spdX);
                str += str_c;
                sprintf_s(str_c, "phs.spdY = %f\n", phs.spdY);
                str += str_c;
                sprintf_s(str_c, "phs.spdZ = %f\n", phs.spdZ);
                str += str_c;
                str += "/------------------------------------------------------------------/\n";

                // 過去分を追加
                str += str_log;

                // 書き込み
                fprintf(pFile, str.c_str());
                // 書き込みモード終了
                fclose(pFile);
                */
        //-----------------------------------------------------------------------------------------------------------

    }

    //比率を返す
    f32 TStShot::GetRatioFromAngle(BOOL yCheck, f32 distA, f32 distB, s32 soreDir, BOOL& ign_f)
    {
        f32 res = 1.0f;

        //反れ 直角 (念のため絶対値距離を返しておく)
        if ((soreDir == -4) || (soreDir == 11)) ign_f = TRUE, res = fabs(distB);

        //反れ 標的非依存
        if ((soreDir >= 3) && (soreDir < 11))
        {
            //反れ 標的非依存A（X）メイン 45度を４分割にして反れ方向Lvで加算。３で０度、７で４５度
            res = tan((PI / 4.0f) * (((f32)soreDir - 3.0f) / 4.0f));

            //反れ 標的非依存B（YorZ）メイン 45度を４分割にして反れ方向Lvで減算。８で４５-１１．２５度、１０で１１．２５度
            if ((soreDir >= 8) && (soreDir <= 10)) res = tan((PI / 4.0f) + (PI / 4.0f) * (((f32)soreDir - 7.0f) / 4.0f));
        }

        //反れ 標的非依存 その２　標的依存から非依存へ
        if ((soreDir >= -3) && (soreDir < -1) && (yCheck == TRUE))
        {
            //反れ 標的非依存。-３で11.25度、-2で22.5度
            res = 1.0f / (tan((PI / 6.0f) * ((f32)soreDir + 4.0f)));
        }

        //反れ 標的依存
        //        if ((soreDir >= -3) && (soreDir < 3))
        //yなら-1～２　ｚなら-3～２
        if ((soreDir < 3)
            &&
            (
            ((soreDir >= -1) && (yCheck == TRUE))
            ||
            ((soreDir >= -3) && (yCheck == FALSE))
            )
            )
        {
            f32 distA_a = fabs(distA);//X距離
            f32 distB_a = fabs(distB);//YorZ距離

            //Xが同一でない場合
            //if ((s32)distA_a != 0)
            if (distA_a != 0.0f)
            {
                f32 distARtLv = 1.0f;//X方向比率Lv
                f32 distBRtLv = 1.0f;//B方向比率Lv
                f32 distBAdd = 0.0f;//股狙いなどで使用

                //T側反れの場合はX方向距離を分割
                if ((soreDir >= -3) && (soreDir < -1)) distARtLv = ((f32)soreDir + 4.0f) / 3.0f;
                //E側反れの場合はB方向距離を分割
                if ((soreDir == 1) || (soreDir == 2)) distBRtLv = (3.0f - (f32)soreDir) / 3.0f;
                //股狙い・頭狙い
                //                if ((yCheck == TRUE) && (distB == 0))
                if (yCheck == TRUE)
                {
                    //y飛ぶ方向で加減算が変わる
                    f32 addDir = 1.0f;//上昇なら加算
                    f32 airAdd = MATA / 2.0f;//下降ならMATA狙いの半分を加算
                    if (distB < 0) addDir = -1.0f;//下降の時は加算値分を引く
                    else airAdd = 0.0f;//上昇ならMATA半分加算をしない

                    if (soreDir == -1) distBAdd = addDir * (MATA + airAdd);
                    //                    if (soreDir == 1) distBAdd = addDir * HEAD;
                    if (soreDir == 1) distBAdd = HEAD;
                    if ((soreDir >= -3) && (soreDir < -1)) distBAdd = -14.0f;
                }
                //サイド狙い
                if ((yCheck == FALSE) && (distB == (phs.tagposZ - phs.posZ)))
                {
                    if (soreDir == -1) distBAdd = -8;
                    if (soreDir == 1) distBAdd = +8;
                }

                //B/X比率
                res = (distB_a * distBRtLv + distBAdd) / (distA_a * distARtLv);
            }
            //Xが同一の場合は軸Bを優先しXを無視 比率に距離をまんま入れる
            else ign_f = TRUE, res = distB_a;
        }
        return res;
    }

    //YorZがX非依存のときのYZ比率計算
    void TStShot::SetRatioYZIndepOfX(f32 distY, f32 distZ)
    {
        //Yは非依存、Zはどちらかなので
        //ZのX依存or非依存別処理をしてYを処理

        //反れサイド敵無視
        if (crt.trg.awayZ_Dir > 3)
        {
            //距離データをサイドラインに変更
            agldt.distZ_a = fabs(SL_IN_DP - phs.posZ);
            f32 dirZ = GetDirFromPosSore(distZ, crt.trg.awayZ_Dir, FALSE);

            if (//手前側サイドラインからの距離を入れる
                (dirZ < 0) //反れデータが手前側変化なら
                ||//広い方に変化＆Ｚ大→マイナス方向へ
                ((crt.trg.awayZ_DirNoTg > 0) && (cmn.courtDirRevZ_f == TRUE))
                ||//狭い方に変化＆＆Ｚ小→マイナス方向へ
                ((crt.trg.awayZ_DirNoTg < 0) && (cmn.courtDirRevZ_f == FALSE))
                )agldt.distZ_a = fabs(SL_IN_SW - phs.posZ);
        }

        //狙うY位置
        f32 distY_a = agldt.distY_a;//基本距離Y
        if (crt.trg.awayY_Dir <= -2) distY_a = fabs(phs.tagposY - phs.posY);//地上狙い
        else
        {
            //上昇系の場合　一旦一定高さを目指す
            f32 addHeight = 40.0f;
            if (phs.posY <= 14.0f) addHeight = 72.0f;
            distY_a = addHeight;
            //変化点高さがあるならその高さを指定
            if (crt.chg.height != 0) distY_a = fabs(abs(crt.chg.height) - phs.posY);
        }

        //Z!=0 の場合
        if (agldt.distZ_a != 0.0f)
        {
            //ｙ/ｚを距離から算出
            agldt.ratioYZ = (distY_a * agldt.ratioXY) / (agldt.distZ_a * agldt.ratioXZ);
            //            agldt.ratioYZ = distY_a * agldt.ratioXZ / agldt.distZ_a;
            //ｙ！＝0ならXZを書き換える。　ｙ＝0ならXY＝0、YZ＝0となっているから、XZがそのまま活きでOK
            //                if (agldt.ratioYZ != 0.0f) agldt.ratioXZ = agldt.ratioXY / agldt.ratioYZ;
        }
        //Z=0 の場合
        else
        {
            //ｙ距離
            agldt.ratioYZ = distY_a;
            //ｙ！＝0ならXZを書き換える。Z=0なのでX距離を入れる。　
            if (agldt.ratioYZ != 0.0f) agldt.ratioXZ = agldt.distX_a;
        }
    }


    //サイドラインへ飛ぶ時の角度比率
    f32 TStShot::GetRatioForSideLine(BOOL dpCheck, f32 distY, BOOL& zzero_f)
    {
        f32 res = 1.0f;
        f32 arrivalPosZ = SL_IN_SW;//まずはサイドライン手前位置
        if (dpCheck == TRUE) arrivalPosZ = SL_IN_DP;//サイドライン奥位置

        f32 distSLtoZ = fabs(arrivalPosZ - phs.posZ);

        if (//SW側で座標が下回ったら
            ((dpCheck == FALSE) && (phs.posZ <= arrivalPosZ))
            ||//DP側で座標が上回ったら
            ((dpCheck == TRUE) && (phs.posZ >= arrivalPosZ))
            )
            res = distY;//比率は距離Yとする
        if (distSLtoZ != 0) res = distY / distSLtoZ;//サイドライン奥を到達点としたZY比率
        else zzero_f = TRUE;//Z=0フラグ
        return res;
    }


    //３方向指定
    void TStShot::SetSpdXYZ()
    {
        //基本はX移動があれば問題ないのでまずそれを処理
        //単位ベクトル⇒逆進の場合コレを－１にして逆方向に飛ばす
        f32 scaleVec = 1;
        //逆進中ならXのみ逆に
        if ((crt.trg.backward_Time > 0)) scaleVec = -1;//逆進用単位方向ベクトル
        //標的位置と反れ素材等によるY,Zの方向
        f32 distY = phs.tagposY - phs.posY + LOWLEVEL;//距離Y
        f32 dirY = GetDirFromPosSore(distY, crt.trg.awayY_Dir, TRUE);
        f32 distZ = phs.tagposZ - phs.posZ;//距離Z
        f32 dirZ = GetDirFromPosSore(distZ, crt.trg.awayZ_Dir, FALSE);
        if (//広い方or狭い方変化処理　
            ((crt.trg.awayZ_DirNoTg > 0) && (cmn.courtDirRevZ_f == TRUE))
            ||//狭い方に変化＆＆Ｚ小→マイナス方向へ
            ((crt.trg.awayZ_DirNoTg < 0) && (cmn.courtDirRevZ_f == FALSE))
            ) dirZ = -1.0f;//広い方＆奥側　or　狭い方＆手前側　マイナス
        else if (crt.trg.awayZ_DirNoTg != 0) dirZ = 1.0f;//広い方＆手前側　or　狭い方＆奥側　プラス
        //crt.trg.awayZ_DirNoTg = 0　のときはすでに入っているDirZを保持

        //速度分割
        f32 speedDiv = phs.spd * phs.spdDecFromWave / (1.0f + fabs(agldt.ratioXZ) + fabs(agldt.ratioXY));

        //速度X
        phs.spdX = speedDiv * scaleVec * agldt.dirX;

        //SAngle上の地上フラグ→ｙ速度を負にする
        if (agldt.groundSAngle_f == TRUE) dirY = -1.0f;

        //反れ縦・サイド比率の絶対値を取ることでxの符号による不具合を防止する
        phs.spdY = speedDiv * fabs(agldt.ratioXY) * dirY;//反れ縦絶対値に方向を掛ける
        phs.spdZ = speedDiv * fabs(agldt.ratioXZ) * dirZ;//反れサイド絶対値に方向を掛ける

        //予定到着点
        phs.arrPosX = phs.tagposX;
        f32 arrivalTime = fabs((phs.arrPosX - phs.posX) / phs.spdX);
        phs.arrPosY = phs.spdY * arrivalTime + phs.posY + LOWLEVEL;
        phs.arrPosZ = phs.spdZ * arrivalTime + phs.posZ;
        if ((crt.trg.awayY_Dir == 0) && (crt.trg.awayZ_Dir == 0))
        {
            phs.arrPosX = phs.tagposX;
            phs.arrPosY = LOWLEVEL;
            phs.arrPosZ = phs.tagposZ;
        }

        //変化点サイドラインで空中シュートの場合
        //Zを中心とした速度に
        if (
            (crt.chg.pos == 3)
            && (distY < 0)
            && (crt.trg.awayZ_Dir != 0)
            && (crt.trg.awayZ_Dir != 3)
            && (crt.trg.awayY_Dir < 4)//上昇するヤツはココで処理しちゃうとダメっぽい
            )
        {
            f32 arrPosZ;//サイドライン

            if (dirZ < 0)
            {
                //サイドライン手前に飛ぶ比率と位置
                agldt.ratioYZ = agldt.ratioYZ_SLineSl;
                arrPosZ = SL_IN_SW;//サイドライン手前　内野側
                if (agldt.ratioYZ_SLineSlZ0_f == TRUE) agldt.ratioYZ = fabs(distY);
            }
            else
            {
                //速度が正なら奥へ飛ぶ比率と位置
                agldt.ratioYZ = agldt.ratioYZ_SLineDp;
                arrPosZ = SL_IN_DP;
                if (agldt.ratioYZ_SLineDpZ0_f == TRUE) agldt.ratioYZ = fabs(distY);
            }


            //XZの逆数でX/Z
            f32 ratioZX = 1.0f / fabs(agldt.ratioXZ);
            speedDiv = phs.spd * phs.spdDecFromWave / (ratioZX + fabs(agldt.ratioYZ) + 1.0f);
            //速度X
            phs.spdX = speedDiv * ratioZX * scaleVec * agldt.dirX;
            //反れ縦・サイド比率の絶対値を取ることでxの符号による不具合を防止する
            phs.spdY = speedDiv * fabs(agldt.ratioYZ) * dirY;//反れ縦絶対値に方向を掛ける
            phs.spdZ = speedDiv * dirZ;//反れサイド絶対値に方向を掛ける
            //予定到着点
            phs.arrPosZ = arrPosZ;
            f32 arrivalTime = fabs((phs.arrPosZ - phs.posZ) / phs.spdZ);
            phs.arrPosY = phs.spdY * arrivalTime + phs.posY + LOWLEVEL;
            phs.arrPosX = phs.spdX * arrivalTime + phs.posX;
        }

        //SAngleデータ保存
        SetSAngleData(agldt.dirX, dirY, dirZ, scaleVec, fabs(agldt.ratioXY), fabs(agldt.ratioXZ), speedDiv);
        //最大速度座標軸…波で使用
        cmn.maxSpdAx = sca_X;
    }

    //Ｙメイン
    void TStShot::SetSpdYMain()
    {
        f32 decSpd = DEC_SPD_LV1;//Yメインの場合は基本的に速度低下Lv1
        if ((agldt.ignXY_f == TRUE) && (agldt.ratioXZ == 0)) decSpd = DEC_SPD_LV2;//Xが０なら速度低下Lv2

        //単位ベクトル⇒逆進の場合コレを－１にして逆方向に飛ばす
        f32 scaleVec = 1;
        //逆進中ならXのみ逆に
        if ((crt.trg.backward_Time > 0)) scaleVec = -1;//逆進用単位方向ベクトル

        //速度分割Xアリ
        f32 ratioYX = 1.0f / fabs(agldt.ratioXY);//ｙ/ｘの逆数
        f32 ratioYZ = 1.0f / fabs(agldt.ratioYZ);//agldt.ratioYZは実際にはｙ/ｚなのでその逆数を取る
        //反れ縦的座標非依存の場合、XZを掛けてZの値を補正
        f32 ratioYZtoXZ = 1.0f;//基本は1.0
        f32 lowLv_f = 1.0f;//１４ドット上を狙うためのLOWLEVELにかける。下の条件式に当てはまった場合０が入る。
        if ((crt.trg.awayY_Dir == -3) || (crt.trg.awayY_Dir == -2))
        {
            ratioYZtoXZ = fabs(agldt.ratioXZ);
            ratioYZ = 1.0f;//該当時にはXZ値
            lowLv_f = 0.0f;
        }
        f32 speedDiv = phs.spd * phs.spdDecFromWave * decSpd / (ratioYX + 1.0f + ratioYZ * ratioYZtoXZ);

        //        f32 speedDiv = phs.spd * phs.spdDecFromWave* decSpd / (1.0f + fabs(agldt.ratioXZ) + fabs(agldt.ratioXY));
        //標的位置と反れ素材等によるY,Zの方向
        f32 distX = phs.tagposX - phs.posX;//距離Z
        f32 distY = (phs.tagposY + LOWLEVEL) * lowLv_f - phs.posY;//距離Y
        f32 distZ = phs.tagposZ - phs.posZ;//距離Z
        f32 dirY = GetDirFromPosSore(distY, crt.trg.awayY_Dir, TRUE);
        f32 dirZ = GetDirFromPosSore(distZ, crt.trg.awayZ_Dir, FALSE);
        if (//広い方or狭い方変化処理　
            ((crt.trg.awayZ_DirNoTg > 0) && (cmn.courtDirRevZ_f == TRUE))
            ||//狭い方に変化＆＆Ｚ小→マイナス方向へ
            ((crt.trg.awayZ_DirNoTg < 0) && (cmn.courtDirRevZ_f == FALSE))
            ) dirZ = -1.0f;//広い方＆奥側　or　狭い方＆手前側　マイナス
        else if (crt.trg.awayZ_DirNoTg != 0) dirZ = 1.0f;//広い方＆手前側　or　狭い方＆奥側　プラス
        //crt.trg.awayZ_DirNoTg = 0　のときはすでに入っているDirZを保持

        f32 distX_a = fabs(distX);
        f32 distZ_a = fabs(distZ);
        f32 distXZ = distX_a + distZ_a;

        //反れ縦・サイド比率の絶対値を取ることでxの符号による不具合を防止する
        phs.spdY = speedDiv * dirY;//反れ縦絶対値に方向を掛ける
        //速度X、Z
        //反れ縦的非依存の場合XY=YZになる。また、X=Z・・・・XZ=１以外だとX:Zが不正になるので調整

        if ((ratioYX == ratioYZ) && (agldt.ratioXZ != 1.0f))
        {
            ratioYX = ratioYX * distX_a / distXZ;
            ratioYZ = ratioYX * distZ_a / distXZ;
        }

        phs.spdX = speedDiv * scaleVec * ratioYX * agldt.dirX;
        phs.spdZ = speedDiv * ratioYZ * dirZ * ratioYZtoXZ;//反れサイド絶対値に方向を掛ける

        //予定到着点
        phs.arrPosY = phs.tagposY + LOWLEVEL;
        f32 arrivalTime = fabs((phs.arrPosY - phs.posY) / phs.spdY);
        phs.arrPosX = phs.spdX * arrivalTime + phs.posX;
        phs.arrPosZ = phs.spdZ * arrivalTime + phs.posZ;

        //XY角度のみが指定された場合・・・角度とY距離からX距離を求めなくてはならぬ
        if ((crt.trg.awayY_Dir == -3) || (crt.trg.awayY_Dir == -2))
        {
            agldt.ratioXY = GetRatioFromAngle(TRUE, distX, distY, crt.trg.awayY_Dir, agldt.ignXY_f);//XY比率
            distX_a = fabs(distY / agldt.ratioXY);

            speedDiv = phs.spd * phs.spdDecFromWave * decSpd / (distX_a + fabs(distY) + distZ_a);

            phs.spdY = speedDiv * dirY * fabs(distY);//反れ縦絶対値に方向を掛ける
            phs.spdX = speedDiv * scaleVec * distX_a * agldt.dirX;
            phs.spdZ = speedDiv * distZ_a * dirZ;//反れサイド絶対値に方向を掛ける

            //予定到着点
            phs.arrPosY = phs.tagposY + LOWLEVEL;
            f32 arrivalTime = fabs((phs.arrPosY - phs.posY) / phs.spdY);
            phs.arrPosX = phs.spdX * arrivalTime + phs.posX;
            phs.arrPosZ = phs.spdZ * arrivalTime + phs.posZ;
        }
        else
            //Xが０
            if ((agldt.ignXY_f == TRUE) || (agldt.ignXZ_f == TRUE))
            {
                //            speedDiv = phs.spd * phs.spdDecFromWave* decSpd / (fabs(agldt.ratioXZ) + fabs(agldt.ratioXY));
                speedDiv = phs.spd * phs.spdDecFromWave * decSpd / (1.0f + ratioYZ);
                //速度X
                phs.spdX = 0.0f;
                //反れ縦・サイド比率の絶対値を取ることでxの符号による不具合を防止する
                //            phs.spdY = speedDiv * fabs(agldt.ratioXY) * dirY;//反れ縦絶対値に方向を掛ける
                //            phs.spdZ = speedDiv * fabs(agldt.ratioXZ) * dirZ;//反れサイド絶対値に方向を掛ける
                phs.spdY = speedDiv * dirY;//反れ縦絶対値に方向を掛ける
                phs.spdZ = speedDiv * ratioYZ * dirZ;//反れサイド絶対値に方向を掛ける

                //変化点サイドラインで空中シュートの場合
                //Zを中心とした速度に
                if ((crt.chg.pos == 3) && (distY < 0) && (crt.trg.awayZ_Dir != 0) && (crt.trg.awayZ_Dir != 3))
                {

                    f32 arrPosZ;//サイドライン
                    SetSpdToSideline(distY, dirZ, arrPosZ);//agldt.ratioYZ適正化
                    f32 zeroZ = 1.0f;//Z0オンになったら０にする
                    if (agldt.zeroZ_f == TRUE) zeroZ = 0.0f;

                    speedDiv = phs.spd * phs.spdDecFromWave * decSpd / (fabs(agldt.ratioYZ) + (1.0f * zeroZ));

                    //速度X
                    phs.spdX = 0.0f;
                    //反れ縦・サイド比率の絶対値を取ることでxの符号による不具合を防止する
                    phs.spdY = speedDiv * fabs(agldt.ratioYZ) * dirY;//反れ縦絶対値に方向を掛ける
                    phs.spdZ = speedDiv * dirZ * zeroZ;//反れサイド絶対値に方向を掛ける

                    //予定到着点
                    phs.arrPosZ = arrPosZ;
                    f32 arrivalTime = fabs((phs.arrPosZ - phs.posZ) / phs.spdZ);
                    phs.arrPosY = phs.spdY * arrivalTime + phs.posY + LOWLEVEL;
                    phs.arrPosX = phs.posX;
                }
            }
            else
            {
                //変化点サイドラインで空中シュートの場合
                //Zを中心とした速度に
                if (
                    (crt.chg.pos == 3)
                    && (distY < 0)
                    && (crt.trg.awayZ_Dir != 0)
                    && (crt.trg.awayZ_Dir != 3)
                    && (crt.trg.awayY_Dir < 4)//上昇するヤツはココで処理しちゃうとダメっぽい
                    )
                {
                    f32 arrPosZ;//サイドライン
                    SetSpdToSideline(distY, dirZ, arrPosZ);//agldt.ratioYZ適正化

                    //XZの逆数でX/Z
                    f32 ratioZX;
                    f32 zeroZ = 1.0f;//Z0オンになったら０にする
                    if (agldt.zeroZ_f == TRUE) zeroZ = 0.0f, ratioZX = fabs(phs.tagposX - phs.posX);
                    else ratioZX = 1.0f / fabs(agldt.ratioXZ);

                    speedDiv = phs.spd * phs.spdDecFromWave * decSpd / (ratioZX + fabs(agldt.ratioYZ) + (1.0f * zeroZ));

                    //速度X
                    phs.spdX = speedDiv * ratioZX * scaleVec * agldt.dirX;
                    //反れ縦・サイド比率の絶対値を取ることでxの符号による不具合を防止する
                    phs.spdY = speedDiv * fabs(agldt.ratioYZ) * dirY;//反れ縦絶対値に方向を掛ける
                    phs.spdZ = speedDiv * dirZ * zeroZ;//反れサイド絶対値に方向を掛ける

                    //予定到着点
                    phs.arrPosZ = arrPosZ;
                    f32 arrivalTime = fabs((phs.arrPosZ - phs.posZ) / phs.spdZ);
                    phs.arrPosY = phs.spdY * arrivalTime + phs.posY + LOWLEVEL;
                    phs.arrPosX = phs.spdX * arrivalTime + phs.posX;
                }

            }

        //Yのみの場合は分割しない
        if ((decSpd == DEC_SPD_LV2) || (crt.trg.awayY_Dir == 11) || (crt.trg.awayY_Dir == -4))
        {
            speedDiv = phs.spd * phs.spdDecFromWave * decSpd;
            //速度XZ
            phs.spdX = 0.0f;
            phs.spdZ = 0.0f;
            //反れ縦
            phs.spdY = speedDiv * dirY;//反れ縦絶対値に方向を掛ける
            phs.arrPosY = 260.0f;
            phs.arrPosZ = phs.posZ;
            phs.arrPosX = phs.posX;
        }

        //SAngleデータ保存
        SetSAngleData(agldt.dirX, dirY, dirZ, scaleVec, fabs(agldt.ratioXY), fabs(agldt.ratioXZ), speedDiv);
        //最大速度座標軸…波で使用
        cmn.maxSpdAx = sca_Y;

    }

    //Ｚメイン
    void TStShot::SetSpdZMain()
    {
        f32 decSpd = DEC_SPD_LV2;//Yメインの場合は基本的に速度低下Lv1
        if ((agldt.ignXZ_f == TRUE) && (agldt.ratioXY == 0)) decSpd = DEC_SPD_LV3;//Xが０なら速度低下Lv3

        //単位ベクトル⇒逆進の場合コレを－１にして逆方向に飛ばす
        f32 scaleVec = 1;
        //逆進中ならXのみ逆に
        if ((crt.trg.backward_Time > 0)) scaleVec = -1;//逆進用単位方向ベクトル

        //速度分割Xアリ
        f32 ratioZX = 1.0f / fabs(agldt.ratioXZ);// z /ｘの逆数
        f32 speedDiv = phs.spd * phs.spdDecFromWave * decSpd / (ratioZX + fabs(agldt.ratioYZ) + 1.0f);
        //        f32 speedDiv = phs.spd * phs.spdDecFromWave* decSpd / (1.0f + fabs(agldt.ratioXZ) + fabs(agldt.ratioXY));
        //標的位置と反れ素材等によるY,Zの方向
        f32 distY = phs.tagposY - phs.posY + LOWLEVEL;//距離Y
        f32 dirY = GetDirFromPosSore(distY, crt.trg.awayY_Dir, TRUE);
        f32 distZ = phs.tagposZ - phs.posZ;//距離Z
        f32 dirZ = GetDirFromPosSore(distZ, crt.trg.awayZ_Dir, FALSE);
        if (//広い方or狭い方変化処理　
            ((crt.trg.awayZ_DirNoTg > 0) && (cmn.courtDirRevZ_f == TRUE))
            ||//狭い方に変化＆＆Ｚ小→マイナス方向へ
            ((crt.trg.awayZ_DirNoTg < 0) && (cmn.courtDirRevZ_f == FALSE))
            ) dirZ = -1.0f;//広い方＆奥側　or　狭い方＆手前側　マイナス
        else if (crt.trg.awayZ_DirNoTg != 0) dirZ = 1.0f;//広い方＆手前側　or　狭い方＆奥側　プラス
        //crt.trg.awayZ_DirNoTg = 0　のときはすでに入っているDirZを保持

        //速度X
        //        phs.spdX = speedDiv * scaleVec * agldt.dirX;
        //反れ縦・サイド比率の絶対値を取ることでxの符号による不具合を防止する
        //        phs.spdY = speedDiv * fabs(agldt.ratioXY) * dirY;//反れ縦絶対値に方向を掛ける
        //        phs.spdZ = speedDiv * fabs(agldt.ratioXZ) * dirZ;//反れサイド絶対値に方向を掛ける

        //SAngle上の地上フラグ→ｙ速度を負にする
        if (agldt.groundSAngle_f == TRUE) dirY = -1.0f;

        //速度X
        phs.spdX = speedDiv * ratioZX * agldt.dirX;
        //反れ縦・サイド比率の絶対値を取ることでxの符号による不具合を防止する
        phs.spdY = speedDiv * fabs(agldt.ratioYZ) * dirY;//反れ縦絶対値に方向を掛ける
        phs.spdZ = speedDiv * scaleVec * dirZ;//反れサイド絶対値に方向を掛ける

        //予定到着点
        phs.arrPosZ = phs.tagposZ;
        f32 arrivalTime = fabs((phs.arrPosZ - phs.posZ) / phs.spdZ);
        phs.arrPosY = phs.spdY * arrivalTime + phs.posY + LOWLEVEL;
        phs.arrPosX = phs.spdX * arrivalTime + phs.posX;


        //Xが０
        if ((agldt.ignXY_f == TRUE) || (agldt.ignXZ_f == TRUE)){
            speedDiv = phs.spd * phs.spdDecFromWave * decSpd / (fabs(agldt.ratioYZ) + 1.0f);
            //速度X
            phs.spdX = 0.0f;
            //反れ縦・サイド比率の絶対値を取ることでxの符号による不具合を防止する
            phs.spdY = speedDiv * fabs(agldt.ratioYZ) * dirY;//反れ縦絶対値に方向を掛ける
            phs.spdZ = speedDiv * dirZ;//反れサイド絶対値に方向を掛ける
            //変化点サイドラインで空中シュートの場合
            //Zを中心とした速度に
            if ((crt.chg.pos == 3) && (distY < 0) && (crt.trg.awayZ_Dir != 0) && (crt.trg.awayZ_Dir != 3))
            {


                f32 arrPosZ;//サイドライン
                SetSpdToSideline(distY, dirZ, arrPosZ);//agldt.ratioYZ適正化

                //XZの逆数でX/Z
                f32 ratioZX;
                f32 zeroZ = 1.0f;//Z0オンになったら０にする
                if (agldt.zeroZ_f == TRUE) zeroZ = 0.0f, ratioZX = fabs(phs.tagposX - phs.posX);
                else ratioZX = 1.0f / fabs(agldt.ratioXZ);

                speedDiv = phs.spd * phs.spdDecFromWave * decSpd / (fabs(agldt.ratioYZ) + (1.0f * zeroZ));
                //速度X
                phs.spdX = 0.0f;
                //反れ縦・サイド比率の絶対値を取ることでxの符号による不具合を防止する
                phs.spdY = speedDiv * fabs(agldt.ratioYZ) * dirY;//反れ縦絶対値に方向を掛ける
                phs.spdZ = speedDiv * dirZ * scaleVec * zeroZ;//反れサイド絶対値に方向を掛ける

                //予定到着点
                phs.arrPosZ = arrPosZ;
                f32 arrivalTime = fabs((phs.arrPosZ - phs.posZ) / phs.spdZ);
                phs.arrPosY = phs.spdY * arrivalTime + phs.posY + LOWLEVEL;
                phs.arrPosX = phs.spdX * arrivalTime + phs.posX;
            }
        }
        else
        {
            //変化点サイドラインで空中シュートの場合
            //Zを中心とした速度に
            if (
                (crt.chg.pos == 3)
                && (distY < 0)
                && (crt.trg.awayZ_Dir != 0)
                && (crt.trg.awayZ_Dir != 3)
                && (crt.trg.awayY_Dir < 4)//上昇するヤツはココで処理しちゃうとダメっぽい
                )
            {

                f32 arrPosZ;//サイドライン
                SetSpdToSideline(distY, dirZ, arrPosZ);//agldt.ratioYZ適正化

                //XZの逆数でX/Z
                f32 ratioZX;
                f32 zeroZ = 1.0f;//Z0オンになったら０にする
                if (agldt.zeroZ_f == TRUE) zeroZ = 0.0f, ratioZX = fabs(phs.tagposX - phs.posX);
                else ratioZX = 1.0f / fabs(agldt.ratioXZ);

                speedDiv = phs.spd * phs.spdDecFromWave * decSpd / (ratioZX + fabs(agldt.ratioYZ) + (1.0f * zeroZ));

                //速度X
                phs.spdX = speedDiv * ratioZX * scaleVec * agldt.dirX;
                //反れ縦・サイド比率の絶対値を取ることでxの符号による不具合を防止する
                phs.spdY = speedDiv * fabs(agldt.ratioYZ) * dirY;//反れ縦絶対値に方向を掛ける
                phs.spdZ = speedDiv * dirZ * zeroZ;//反れサイド絶対値に方向を掛ける

                //予定到着点
                phs.arrPosZ = arrPosZ;
                f32 arrivalTime = fabs((phs.arrPosZ - phs.posZ) / phs.spdZ);
                phs.arrPosY = phs.spdY * arrivalTime + phs.posY + LOWLEVEL;
                phs.arrPosX = phs.spdX * arrivalTime + phs.posX;

            }

        }


        //Zのみの場合は分割しない
        if ((decSpd == DEC_SPD_LV3) || (crt.trg.awayZ_Dir == 11) || (crt.trg.awayZ_Dir == -4))
        {
            speedDiv = phs.spd * phs.spdDecFromWave * decSpd;
            //速度XZ
            phs.spdX = 0.0f;
            phs.spdY = 0.0f;
            //反れサイド
            phs.spdZ = speedDiv * scaleVec * dirZ;//反れサイド絶対値に方向を掛ける
            //予定到着点
            phs.arrPosZ = SL_IN_DP;
            if (dirZ < 0) phs.arrPosZ = SL_IN_SW;
            phs.arrPosY = phs.posY;
            phs.arrPosX = phs.posX;
        }

        //SAngleデータ保存
        SetSAngleData(agldt.dirX, dirY, dirZ, scaleVec, fabs(agldt.ratioXY), fabs(agldt.ratioXZ), speedDiv);
        //最大速度座標軸…波で使用
        cmn.maxSpdAx = sca_Z;
    }

    //３方向速度０時の処理。　…敵コート側になげる。
    void TStShot::SetSpdZeroToX()
    {
        //XYZ＝０成立時にはとりあえず敵コート側に飛ばす。意図して停止中の場合はBaShotで処理しているかと。
        //idata.side：シューターのコート　０左、１右　
        //だが、速度の方向に使うので変換。０左→右＝＋１　１左←右＝－１
        f32 dirX = (f32)idata.side + 1.0f;//0なら1　1なら2にして↓で-1にする
        if (dirX > 1.0) dirX = -1.0f;

        phs.spdX = phs.spd * phs.spdDecFromWave * dirX;//トータル速度にコート情報から得られた方向をかける。
        phs.spdY = 0.0f;
        phs.spdZ = 0.0f;
        //SAngleデータ保存
        SetSAngleData(dirX, 0.0f, 0.0f, dirX, 0.0f, 0.0f, phs.spd);
        //最大速度座標軸…波で使用
        cmn.maxSpdAx = sca_X;

        //予定到着点
        phs.arrPosX = phs.posX;
        phs.arrPosY = phs.posY;
        phs.arrPosZ = phs.posZ;

    }

    //逆進時３方向指定
    void TStShot::SetSpdBackward()
    {
        //方向
        f32 dirX = 1.0f;
        f32 dirY = 1.0f;
        f32 dirZ = 1.0f;

        //距離
        f32 distX = CST_POS_XR - phs.posX;
        f32 distY = CST_POS_Y - phs.posY;//距離Y
        f32 distZ = phs.tagposZ - phs.posZ;

        //距離絶対値
        f32 distX_a = fabs(distX);
        f32 distY_a = fabs(distY);
        f32 distZ_a = fabs(distZ);

        //X逆進 逆進中のみ逆に
        if (fabs(phs.spdX) >= fabs(phs.spdZ))
        {
            //一応右壁にむかう計算
            phs.arrPosX = CST_POS_XR;
            if (idata.frmPos == 6) dirX = dirX * (-1);//４外野ならＸ反転

            if (cmn.sAngle_c == 0)
            {//SAngle初回の場合は所属チームで方向を決める
                if (idata.side == 0) dirX = dirX * (-1);//右チームなら左へ
            }
            else
            {//SAngle１度目以上なら現在の速度と逆へ
                if (phs.spdX > 0) dirX = dirX * (-1);
            }
            //dirXが負のときは左壁に向かう
            if (dirX < 0)
            {
                distX = CST_POS_XL - phs.posX;
                phs.arrPosX = CST_POS_XL;
            }


            //２・３外野ならＹのみ変動
            if ((idata.frmPos == 4) || (idata.frmPos == 5)) distX_a = 0;

            if (distY != 0.0f) dirY = distY / distY_a;

            //速度分割の分母
            f32 bunbo = distX_a + distY_a;
            f32 spdDiv = phs.spd * phs.spdDecFromWave / bunbo;

            if ((bunbo) == 0.0f)
            {//X,Yともに０のみココに
                phs.spdX = 0;
                phs.spdY = 0;
                phs.spdZ = 0;
            }
            else
            {//どちらも０でなければ以下の処理でおｋ

                phs.spdX = spdDiv * distX_a * dirX;
                phs.spdY = spdDiv * distY_a * dirY;
                phs.spdZ = 0;
                if (phs.spdX == 0) phs.spdY = spdDiv * distY_a * dirY * 0.5f;
            }

            //予定到着点
            f32 arrivalTime = fabs((phs.arrPosX - phs.posX) / phs.spdX);
            if (phs.spdX == 0) arrivalTime = 0;
            phs.arrPosY = phs.spdY * arrivalTime + phs.posY + CST_POS_Y;
            phs.arrPosZ = phs.spdZ * arrivalTime + phs.posZ;

            //SAngleデータ保存
            SetSAngleData(agldt.dirX, dirY, 0, -1, fabs(agldt.ratioXY), 0, spdDiv);
            //最大速度座標軸…波で使用
            cmn.maxSpdAx = sca_X;
        }
        else
        {
            if (distZ > 0) dirZ = dirZ * (-1);//標的が手前側にいたらＺ反転

            //dirZが負のときは手前ラインに向かう
            if (dirZ < 0)
            {
                distZ = CST_POS_ZSL - phs.posZ;
                phs.arrPosZ = CST_POS_ZSL;
            }
            else
            {
                //奥ラインにむかう計算
                distZ = CST_POS_ZDP - phs.posZ;
                phs.arrPosZ = CST_POS_ZDP;
            }
            distZ_a = fabs(distZ);

            //２・３外野ならＹのみ変動
            if ((phs.posZ < CST_POS_ZSL) || (phs.posZ > CST_POS_ZDP)) distZ_a = 0;

            if (distY != 0.0f) dirY = distY / distY_a;

            //速度分割の分母
            f32 bunbo = distZ_a + distY_a;
            f32 spdDiv = phs.spd * phs.spdDecFromWave / bunbo;

            if ((bunbo) == 0.0f)
            {//X,Yともに０のみココに
                phs.spdX = 0;
                phs.spdY = 0;
                phs.spdZ = 0;
            }
            else
            {//どちらも０でなければ以下の処理でおｋ

                phs.spdZ = spdDiv * distZ_a * dirZ;
                phs.spdY = spdDiv * distY_a * dirY;
                phs.spdX = 0;
                if (phs.spdZ == 0) phs.spdY = spdDiv * distY_a * dirY * 0.5f;
            }

            //予定到着点
            f32 arrivalTime = fabs((phs.arrPosZ - phs.posZ) / phs.spdZ);
            if (phs.spdZ == 0) arrivalTime = 0;
            phs.arrPosY = phs.spdY * arrivalTime + phs.posY + CST_POS_Y;
            phs.arrPosX = phs.spdX * arrivalTime + phs.posX;

            //SAngleデータ保存
            SetSAngleData(0, dirY, agldt.dirZ, 1, fabs(agldt.ratioXY), -1, spdDiv);
            //最大速度座標軸…波で使用
            cmn.maxSpdAx = sca_Z;

        }
    }


    //特定位置指定
    void TStShot::SetSpdFromObjPos()
    {
        f32 distX = (f32)cmn.objPX - phs.posX;
        f32 distY = (f32)cmn.objPY - phs.posY;
        f32 distZ = (f32)cmn.objPZ - phs.posZ;
        f32 distX_a = fabs(distX);
        f32 distY_a = fabs(distY);
        f32 distZ_a = fabs(distZ);
        f32 dirX = 1;
        if (distX_a != 0) dirX = distX / distX_a;
        f32 dirY = 1;
        if (distY_a != 0) dirY = distY / distY_a;
        f32 dirZ = 1;
        if (distZ_a != 0) dirZ = distZ / distZ_a;

        //段階変化チェック用に入れる
        crt.trg.posX_Dot = cmn.objPX;
        crt.trg.posY_Dot = cmn.objPY;
        crt.trg.posZ_Dot = cmn.objPZ;

        //最大速度座標軸…波で使用
        cmn.maxSpdAx = sca_X;
        if ((distX_a < distY_a) || (distX_a < distZ_a))
        {
            cmn.maxSpdAx = sca_Y;
            if (distY_a <= distZ_a) cmn.maxSpdAx = sca_Z;
        }

        f32 decSpd;//Yメインの場合は基本的に速度低下Lv1
        switch (cmn.maxSpdAx)
        {
        case sca_Z: decSpd = DEC_SPD_LV3; break;
        case sca_Y: decSpd = DEC_SPD_LV2; break;
        default: decSpd = 1.0f; break;
        }

        //速度分割
        f32 spdDiv = phs.spd * phs.spdDecFromWave * decSpd / (distX_a + distY_a + distZ_a);

        phs.spdX = spdDiv * distX_a * dirX;
        phs.spdY = spdDiv * distY_a * dirY;
        phs.spdZ = spdDiv * distZ_a * dirZ;

        if ((distX_a + distY_a + distZ_a) == 0.0f)
        {//X,Yともに０のみココに
            phs.spdX = 0;
            phs.spdY = 0;
            phs.spdZ = 0;
        }

        //予定到着点
        phs.arrPosX = crt.trg.posX_Dot;
        phs.arrPosY = crt.trg.posY_Dot;
        phs.arrPosZ = crt.trg.posZ_Dot;

        //SAngleデータ保存
        SetSAngleData(dirX, dirY, dirZ, 0, 0, 0, spdDiv);
    }

    //外野シュート時Zメインの処理
    void TStShot::SetSpdOFSNShoot()
    {
        cmn.shotNSOFZ_f = TRUE;//次回以降すべてこの処理へ
        //基本はX移動があれば問題ないのでまずそれを処理
        //単位ベクトル⇒逆進の場合コレを－１にして逆方向に飛ばす
        f32 scaleVec = 1;
        //逆進中ならXのみ逆に
        if ((crt.trg.backward_Time > 0)) scaleVec = -1;//逆進用単位方向ベクトル
        f32 decSpd = DEC_SPD_LV3;//Yメインの場合は基本的に速度低下Lv1

        //標的位置と反れ素材等によるY,Zの方向
        f32 distX = phs.tagposX - phs.posX;//距離X
        f32 dirX = GetDirFromPosSore(distX, crt.trg.awayZ_Dir, TRUE);
        f32 distY = phs.tagposY - phs.posY + LOWLEVEL;//距離Y
        f32 dirY = GetDirFromPosSore(distY, crt.trg.awayY_Dir, TRUE);
        f32 distZ = phs.tagposZ - phs.posZ;//距離Z
        f32 dirZ = 0.0f;
        if (distZ != 0.0f) dirZ = distZ / fabs(distZ);

        if (//広い方or狭い方変化処理　
            ((crt.trg.awayZ_DirNoTg > 0) && (cmn.courtDirRevX_f == TRUE))
            ||//狭い方に変化＆＆Ｚ小→マイナス方向へ
            ((crt.trg.awayZ_DirNoTg < 0) && (cmn.courtDirRevX_f == FALSE))
            ) dirX = -1.0f;//広い方＆奥側　or　狭い方＆手前側　マイナス
        else if (crt.trg.awayZ_DirNoTg != 0) dirX = 1.0f;//広い方＆手前側　or　狭い方＆奥側　プラス

        //①到達点→②距離→③比率を求める
        //①到達点
        f32 aposX = phs.tagposX;
        f32 aposY = phs.tagposY + 14.0f;
        f32 aposZ = phs.tagposZ;
        //ソレで到達点が変わるときの処理
        if ((crt.trg.awayY_Dir != 0) || (crt.trg.awayZ_Dir != 0)) SetArrivalPosFromSoreDir(aposX, aposY, aposZ);

        distX = aposX - phs.posX;
        distY = aposY - phs.posY;
        distZ = aposZ - phs.posZ;
        f32 distX_a = fabs(distX);
        f32 distY_a = fabs(distY);
        f32 distZ_a = fabs(distZ);

        if (distY_a > distZ_a) decSpd = DEC_SPD_LV2;
        if ((distX_a > distZ_a) && (distX_a > distY_a)) decSpd = DEC_SPD_LV1;
        //速度分割
        f32 speedDiv = phs.spd * decSpd * phs.spdDecFromWave / (distX_a + distY_a + distZ_a);

        //速度 Zメインなので逆進符号はZに
        phs.spdX = speedDiv * distX_a * dirX;
        phs.spdY = speedDiv * distY_a * dirY;
        phs.spdZ = speedDiv * distZ_a * scaleVec * dirZ;

        //予定到着点
        phs.arrPosX = phs.tagposX;
        f32 arrivalTime = fabs((phs.arrPosX - phs.posX) / phs.spdX);
        phs.arrPosY = phs.spdY * arrivalTime + phs.posY + LOWLEVEL;
        phs.arrPosZ = phs.spdZ * arrivalTime + phs.posZ;

        //変化点サイドラインで空中シュートの場合
        //Zを中心とした速度に
        if (
            (crt.chg.pos == 3)
            && (distY < 0)
            && (crt.trg.awayZ_Dir != 0)
            && (crt.trg.awayZ_Dir != 3)
            && (crt.trg.awayY_Dir < 4)//上昇するヤツはココで処理しちゃうとダメっぽい
            )
        {
            f32 arrPosZ;//サイドライン

            if (dirZ < 0)
            {
                //サイドライン手前に飛ぶ比率と位置
                agldt.ratioYZ = agldt.ratioYZ_SLineSl;
                arrPosZ = SL_IN_SW;//サイドライン手前　内野側
                if (agldt.ratioYZ_SLineSlZ0_f == TRUE) agldt.ratioYZ = fabs(distY);
            }
            else
            {
                //速度が正なら奥へ飛ぶ比率と位置
                agldt.ratioYZ = agldt.ratioYZ_SLineDp;
                arrPosZ = SL_IN_DP;
                if (agldt.ratioYZ_SLineDpZ0_f == TRUE) agldt.ratioYZ = fabs(distY);
            }


            //XZの逆数でX/Z
            f32 ratioZX = 1.0f / fabs(agldt.ratioXZ);
            speedDiv = phs.spd * phs.spdDecFromWave / (ratioZX + fabs(agldt.ratioYZ) + 1.0f);
            //速度X
            phs.spdX = speedDiv * ratioZX * scaleVec * agldt.dirX;
            //反れ縦・サイド比率の絶対値を取ることでxの符号による不具合を防止する
            phs.spdY = speedDiv * fabs(agldt.ratioYZ) * dirY;//反れ縦絶対値に方向を掛ける
            phs.spdZ = speedDiv * dirZ;//反れサイド絶対値に方向を掛ける
            //予定到着点
            phs.arrPosZ = arrPosZ;
            f32 arrivalTime = fabs((phs.arrPosZ - phs.posZ) / phs.spdZ);
            phs.arrPosY = phs.spdY * arrivalTime + phs.posY + LOWLEVEL;
            phs.arrPosX = phs.spdX * arrivalTime + phs.posX;
        }

        //SAngleデータ保存
        SetSAngleData(agldt.dirX, dirY, dirZ, scaleVec, fabs(agldt.ratioXY), fabs(agldt.ratioXZ), speedDiv);
        //最大速度座標軸…波で使用
        //cmn.maxSpdAx = sca_X;


    }


    //２・３外野用到達点処理
    void TStShot::SetArrivalPosFromSoreDir(f32& aposX, f32 & aposY, f32& aposZ)
    {
        s32 soreY = crt.trg.awayY_Dir;
        s32 soreX = crt.trg.awayZ_Dir;//Zが進行方向になるのでここではXとする
        BOOL noTagY_f = FALSE;//標的無視Y
        BOOL noTagX_f = FALSE;//標的無視X
        f32 angleY = 1.0f;//標的無視時の角度ｙ
        f32 angleX = 1.0f;//標的無視時の角度ｘ
        f32 distARtLv = 1.0f;//Z方向比率Lv
        f32 distBRtLv = 1.0f;//XorY方向比率Lv
        f32 distBAdd = 0.0f;//股狙いなどで使用
        f32 distX = phs.tagposX - phs.posX;
        f32 distY = phs.tagposY - phs.posY + 14.0f;
        f32 distZ = phs.tagposZ - phs.posZ;
        f32 distX_a = fabs(distX);
        f32 distY_a = fabs(distY);
        f32 distZ_a = fabs(distZ);
        BOOL verX_f = FALSE;//垂直フラグｘ
        BOOL verY_f = FALSE;//垂直フラグｙ

        BOOL dummy_f;//引数に適当なやつを入れるためのダミー


        //Y
        //反れ 標的非依存
        if (soreY != 0)
        {
            if ((soreY >= 3) && (soreY < 11))
            {
                angleY = GetRatioFromAngle(FALSE, distY, distX, soreY, dummy_f);//soreY以外引数適当
                noTagY_f = TRUE;
            }
            //-1～2:標的依存
            else if ((soreY < 3) && (soreY >= -1))
            {
                //T側反れの場合はX方向距離を分割
                if ((soreY >= -3) && (soreY < -1)) distARtLv = ((f32)soreY + 4.0f) / 3.0f;
                //E側反れの場合はB方向距離を分割
                if ((soreY == 1) || (soreY == 2)) distBRtLv = (3.0f - (f32)soreY) / 3.0f;

                //y飛ぶ方向で加減算が変わる
                f32 addDir = 1.0f;//上昇なら加算
                f32 airAdd = MATA / 2.0f;//下降ならMATA狙いの半分を加算
                if ((phs.tagposY - phs.posY) < 0) addDir = -1.0f;//下降の時は加算値分を引く
                else airAdd = 0.0f;//上昇ならMATA半分加算をしない

                if (soreY == -1) aposY = aposY - 10.0f;
                else if (soreY == 1) aposY = aposY + 8.0f;
                else if ((soreY >= -3) && (soreY < -1))
                {
                    distBAdd = -14.0f;
                    aposY = aposY + (distY_a * distBRtLv + distBAdd) / distARtLv;
                }
                else aposY = aposY + (distY_a * distBRtLv + distBAdd) / distARtLv;
            }
            else if ((soreY == -3) || (soreY == -2))
            {//-3,-2 標的非依存
                //反れ 標的非依存。-３で11.25度、-2で22.5度
                angleY = 1.0f / (tan((PI / 6.0f) * ((f32)soreY + 4.0f)));
                noTagY_f = TRUE;
            }
            else
            {
                //反れ 直角 (念のため絶対値距離を返しておく)
                if ((soreY == -4) || (soreY == 11)) noTagY_f = TRUE, verY_f = TRUE;
            }
        }

        //X
        //反れ 標的非依存
        if (soreX != 0)
        {
            if ((soreX >= 3) && (soreX < 11))
            {
                angleX = GetRatioFromAngle(FALSE, distY, distX, soreX, dummy_f);//soreX以外引数適当
                noTagX_f = TRUE;
            }
            //-3 ～ 2:標的依存
            else if ((soreX < 3) && (soreX >= -3))
            {
                //T側反れの場合はX方向距離を分割
                if ((soreX >= -3) && (soreX < -1)) distARtLv = ((f32)soreX + 4.0f) / 3.0f;
                //E側反れの場合はB方向距離を分割
                if ((soreX == 1) || (soreX == 2)) distBRtLv = (3.0f - (f32)soreX) / 3.0f;
                if (soreX == -1) distBAdd = -8;
                if (soreX == 1) distBAdd = +8;
                aposX = aposX + (distX_a * distBRtLv + distBAdd) / distARtLv;
            }
            else
            {
                //反れ 直角 (念のため絶対値距離を返しておく)
                if ((soreX == -4) || (soreX == 11)) noTagX_f = TRUE, verX_f = TRUE;
            }
        }
        //標的無視処理
        if (noTagX_f == TRUE) aposX = distZ_a * angleX + phs.posX;
        if (noTagY_f == TRUE) aposY = distZ_a * angleY + phs.posY;

        if ((verX_f == TRUE) && (verY_f == TRUE))
        {//XYのみ加算、Z加減なし
            aposX = phs.posY;//後にY-X
            aposY = phs.posX;//後にX-Y　で　同じ値にして飛ばす
            aposZ = phs.posZ;//後にZ-Z　で０とする
        }
        else if ((verX_f == TRUE) && (verY_f == FALSE))
        {
            aposX = phs.tagposX;
            aposY = phs.posY;//後にY-Y　で０とする
            //            if (noTagY_f == TRUE) aposY = ((f32)soreY - 3.0f) / (11.0f - 3.0f);
            aposZ = phs.posZ;//後にZ-Z　で０とする
        }
        else if ((verX_f == FALSE) && (verY_f == TRUE))
        {
            aposX = phs.posX;
            //            if (noTagX_f == TRUE) aposX = ((f32)soreX - 3.0f) / (11.0f - 3.0f);
            aposY = phs.tagposY;
            aposZ = phs.posZ;
        }

    }

    //サイドラインに飛ばすときのagldt.ratioYZ適正化
    void TStShot::SetSpdToSideline(f32 distY, f32 dirZ, f32& arrPosZ)
    {
        agldt.zeroZ_f = FALSE;//Z0フラグ

        if (dirZ < 0)
        {
            //サイドライン手前に飛ぶ比率と位置
            agldt.ratioYZ = agldt.ratioYZ_SLineSl;
            arrPosZ = SL_IN_SW;//サイドライン手前　内野側
            if (agldt.ratioYZ_SLineSlZ0_f == TRUE) agldt.ratioYZ = fabs(distY), agldt.zeroZ_f = TRUE;
        }
        else
        {
            //速度が正なら奥へ飛ぶ比率と位置
            agldt.ratioYZ = agldt.ratioYZ_SLineDp;
            arrPosZ = SL_IN_DP;
            if (agldt.ratioYZ_SLineDpZ0_f == TRUE) agldt.ratioYZ = fabs(distY), agldt.zeroZ_f = TRUE;
        }
    }







    //距離と反れ素材から飛ぶ方向を決める
    f32 TStShot::GetDirFromPosSore(f32 dist, s32 soreDt, BOOL enmIgn_f)
    {
        f32 res = 1.0f;
        if (dist < 0.0f){//敵がマイナス方向にいる場合
            if (soreDt < 3) res = -1.0f;//反れ素材水平より手前or下側への指定ならマイナス

        }
        else if (dist == 0.0f){//敵が水平方向にいる場合
            if (soreDt < 0) res = -1.0f;//反れ素材水平より手前or下側への指定ならマイナス

        }
        else {//敵がプラス方向にいる場合
            if ((soreDt > 3) && (enmIgn_f == FALSE)) res = -1.0f;//反れ素材水平より奥への指定ならマイナス　
            //敵上空にいるとき（distYが＋(enmIgn_f=TRUE)）は反転させない
        }
        return res;
    }

    //外野シュートや-cosで広いほうに球を飛ばすためのフラグ
    void TStShot::CheckZDir()
    {//一応X,Z両方フラグチェックをする
        //デフォでFALSE
        cmn.courtDirRevX_f = FALSE;//コートＸ位置によるマイナス方向進行フラグ２・３外野用
        cmn.courtDirRevZ_f = FALSE;//コートＺ位置によるマイナス方向進行フラグ２・３外野以外用

        //Z奥ラインと手前ラインの中点（中央線）
        const f32 MID_LINE_Z = (DBCRT_BL + DBCRT_FL) / 2;

        //X外野可動域左端・右端を求めて、それとコート真ん中から中点Xを求める。
        //X外野可動域左端　デフォで手前外野
        f32 ofMvTr_L = CRPOS_F_XL;//手前外野左端

        //Z現在のZ位置が、奥ラインと手前ラインの中点（中央線）より奥ならマイナスフラグON
        //Xさらに外野可動域Xを奥外野として設定する。
        if ((phs.posZ * XYMAG) > MID_LINE_Z)
        {
            cmn.courtDirRevZ_f = TRUE;//Zをマイナスに・・・Zチェックはココで終わり以降Xチェック
            ofMvTr_L = CRPOS_B_XL;//X奥外野左端
        }
        //センターラインＺ位置近いフラグ→サイドカーブだまで使用
        cmn.courtNearCenterZ_f = FALSE;
        const f32 CENTER_LINE_Z_NEAR = 16.0f * XYMAG;//センターラインＺに近いとする距離
        //センターラインに近い場合はフラグを立てる
        if (fabs(phs.posZ * XYMAG - MID_LINE_Z) <= CENTER_LINE_Z_NEAR) cmn.courtNearCenterZ_f = TRUE;

        //X外野可動域左端　デフォで手前外野
        f32 ofMvTr_R = DBCRT_W - ofMvTr_L;//画面右端 － 手前外野左端 ＝ 手前外野右端

        //X外野可動域区分けポイント
        f32 ofMvTrMidPos_L = (DBCRT_CLI + ofMvTr_L) / 2;//(画面中央 + 外野左端)/2 ＝ 左外野中点
        f32 ofMvTrMidPos_R = (DBCRT_CLI + ofMvTr_R) / 2;//(画面中央 + 外野右端)/2 ＝ 右外野中点

        if (((idata.side == 0) && (ofMvTrMidPos_R < phs.posX * XYMAG))
            ||
            ((idata.side == 1) && (ofMvTrMidPos_L < phs.posX * XYMAG))
            ) cmn.courtDirRevX_f = TRUE;//Xをマイナスに

    }



    //主軌道の算出
    //反れ縦・反れサイドからy、z速度を算出する
    void TStShot::SAngle()
    {
        cmn.addDir_f = TRUE;//GlobalMoveで波の加算時に敵方向を１回だけ確認するフラグ。同処理内でFALSEに
        cmn.reSAngle_f = FALSE;//一旦、再SAngleフラグをOFFにする→ONになる時は徐々に処理最終段階→放物線軌道も再計算
        agldt.Init();
        CheckZDir();
        GetSpeedXYZ();
        cmn.sAngle_c = cmn.sAngle_c + 1;
        //                       SoreTatePoint();
        //                       SoreSidePoint();
        //                       SoreTateSideRate();
        //                       XYZSpeedDiv();
    }




}
