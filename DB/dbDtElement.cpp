#include "dbDtElement.h"

namespace db
{
    const s32 ELMNO = 1;

    TDtElement::TDtElement(void)
    {
    }

    TDtElement::~TDtElement(void)
    {
    }

#ifdef __K_DEBUG_SHIAI__
    void TDtElement::GenLoadDt(const char *CSVDir, const char *CSVFilename, INTVECTOR2* intvec2, STRVECTOR2* strvec2)
    {
        intvec2->clear();
        strvec2->clear();

        mid::TCSV csv;
        csv.midLoadCSV(CSVDir, CSVFilename, FALSE, FALSE);

        for (s32 i = 0; i < csv.GetLine(); ++i)
        {
            INTVECTOR intvec;
            STRVECTOR strvec;
            intvec2->push_back(intvec);
            strvec2->push_back(strvec);
            for (s32 i2 = 0; i2 < csv.GetIntSize(0); ++i2) intvec2->at(i).push_back(csv.GetInt(i, i2));
            for (s32 i2 = 0; i2 < csv.GetStrSize(0); ++i2) strvec2->at(i).push_back(csv.GetStr(i, i2));
        }
    }
#else
    void TDtElement::GenLoadDt(const char *CSVDir, const char *CSVFilename, INTVECTOR2* intvec2)
    {
        intvec2->clear();

        mid::TCSV csv;
        csv.midLoadCSV(CSVDir, CSVFilename, FALSE, TRUE);//NUMONLY
        for (s32 i = 0; i < csv.GetLine(); ++i)
        {
            INTVECTOR intvec;
            intvec2->push_back(intvec);
            for (s32 i2 = 0; i2 < csv.GetIntSize(0); ++i2) intvec2->at(i).push_back(csv.GetInt(i, i2));
        }
    }
#endif	// #ifdef __K_DEBUG_SHIAI__

    //読み込み（追記対応）
    void TDtElement::LoadDt(const char *CSVDir, const char *CSV_Change, const char *CSV_Effect, const char *CSV_Shape, const char *CSV_Orbit, const char *CSV_Target)
    {
#ifdef __K_DEBUG_SHIAI__
        //CSV_Change
        GenLoadDt(CSVDir, CSV_Change, &change_, &changeStr_);
        //CSV_Effect
        GenLoadDt(CSVDir, CSV_Effect, &effect_, &effectStr_);
        //CSV_Shape
        GenLoadDt(CSVDir, CSV_Shape, &shape_, &shapeStr_);
        //CSV_Orbit
        GenLoadDt(CSVDir, CSV_Orbit, &orbit_, &orbitStr_);
        //CSV_Target
        GenLoadDt(CSVDir, CSV_Target, &target_, &targetStr_);
#else
        //CSV_Change
        GenLoadDt(CSVDir, CSV_Change, &change_);
        //CSV_Effect
        GenLoadDt(CSVDir, CSV_Effect, &effect_);
        //CSV_Shape
        GenLoadDt(CSVDir, CSV_Shape, &shape_);
        //CSV_Orbit
        GenLoadDt(CSVDir, CSV_Orbit, &orbit_);
        //CSV_Target
        GenLoadDt(CSVDir, CSV_Target, &target_);
#endif	// // #ifdef __K_DEBUG_SHIAI__
    }

    s32 TDtElement::GetChangeElmNo(s32 id)
    {
        return change_[id][ELMNO];
    }
    s32 TDtElement::GetEffectElmNo(s32 id)
    {
        return effect_[id][ELMNO];
    }
    s32 TDtElement::GetShapeElmNo(s32 id)
    {
        return shape_[id][ELMNO];
    }
    s32 TDtElement::GetOrbitElmNo(s32 id)
    {
        return orbit_[id][ELMNO];
    }
    s32 TDtElement::GetTargetElmNo(s32 id)
    {
        return target_[id][ELMNO];
    }

    s32 TDtElement::GetChangeDt(s32 id, s32 dtNo)
    {
        return change_[id][ELMNO + dtNo];
    }
    s32 TDtElement::GetEffectDt(s32 id, s32 dtNo)
    {
        return effect_[id][ELMNO + dtNo];
    }
    s32 TDtElement::GetShapeDt(s32 id, s32 dtNo)
    {
        return shape_[id][ELMNO + dtNo];
    }
    s32 TDtElement::GetOrbitDt(s32 id, s32 dtNo)
    {
        return orbit_[id][ELMNO + dtNo];
    }
    s32 TDtElement::GetTargetDt(s32 id, s32 dtNo)
    {
        return target_[id][ELMNO + dtNo];
    }

    void TDtElement::SetChangeDt(s32 id, s32* dt1)
    {
        SetChangeDt(id, dt1, NULL);
    }
    void TDtElement::SetChangeDt(s32 id, s32* dt1, s32* dt2)
    {
        if (dt1 != NULL) *dt1 = change_[id][ELMNO + 1];
        if (dt2 != NULL) *dt2 = change_[id][ELMNO + 2];
    }

    void TDtElement::SetTargetDt(s32 id, s32* dt1)
    {
        SetTargetDt(id, dt1, NULL);
    }
    void TDtElement::SetTargetDt(s32 id, s32* dt1, s32* dt2)
    {
        if (dt1 != NULL) *dt1 = target_[id][ELMNO + 1];
        if (dt2 != NULL) *dt2 = target_[id][ELMNO + 2];
    }

    void TDtElement::SetOrbitDt(s32 id, s32* dt1, s32* dt2, s32* dt3)
    {
        //    SetOrbitDt(id, dt1, dt2, dt3, NULL, NULL, NULL, NULL, NULL);
        if (dt1 != NULL) *dt1 = orbit_[id][ELMNO + 1];
        if (dt2 != NULL) *dt2 = orbit_[id][ELMNO + 2];
        if (dt3 != NULL) *dt3 = orbit_[id][ELMNO + 3];
    }
    void TDtElement::SetOrbitDt(s32 id, s32* dt1, s32* dt2, s32* dt3, s32* dt4, s32* dt5, s32* dt6, s32* dt7, s32* dt8)
    {
        //①波タイプ
        if (dt1 != NULL)
        {
            //          if ((dt1 != NULL) && (*dt1 < (orbit_[id][ELMNO + 1]))) *dt1 = orbit_[id][ELMNO + 1];
            //          if (orbit_[id][ELMNO + 1] == 0) *dt1 = orbit_[id][ELMNO + 1];//０の時は波をオフにする
            if (orbit_[id][ELMNO + 1] != 0) *dt1 = orbit_[id][ELMNO + 1];//０の時は波をオフにする
        }
        //②振幅
        if (dt2 != NULL)
        {
            //元データと素材がともに負の場合 素材のデータに更新
            s32 udData = orbit_[id][ELMNO + 2];
            switch (*dt2)//最新データごとに篩い分け
            {
            case 0:
                *dt2 = udData;//更新データで更新
                if (udData == 0) *dt2 = 3;//更新データも０なら強制的に３で更新
                break;
            case 1:
            case 2:
                if (udData == 2) *dt2 = 1;//更新データが２のときは１に更新
                if (udData == 4) *dt2 = 4;//更新データが４のときは４に更新
                break;
            case 3: *dt2 = udData; break;//更新データで上書き
            case 4: if (udData == 4) *dt2 = 5; break;//更新データも４なら５で更新
            default: break;
            }
            //if ((*dt2 == 0) && (orbit_[id][ELMNO + 2] == 0)) *dt2 = 3;
            //else if ((orbit_[id][ELMNO + 2] == 0)) *dt2 = *dt2;//*dt2維持
            //else if (*dt2 == 0) *dt2 = orbit_[id][ELMNO + 2];
            //else if (*dt2 == 4) *dt2 = orbit_[id][ELMNO + 2];
            //      else if ((abs(*dt2 - 3) < abs(orbit_[id][ELMNO + 2] - 3))) *dt2 = orbit_[id][ELMNO + 2];

        }
        //③振動数
        if (dt3 != NULL)
        {
            //更新データ
            s32 udData = orbit_[id][ELMNO + 3];
            switch (*dt3)//元データごとに篩い分け
            {
            case 0:
                *dt3 = udData;//元データが０のときは更新データで更新
                break;
            case 1: if (udData == 1) *dt3 = 2; break;//元データが１のとき、更新データも１なら２へ
            case -1:
                if (udData == -1) *dt3 = -2;//元データ＆更新データが－１なら－２へ
                if (udData >= 0) *dt3 = udData;//０以上なら更新データで更新
                break;
            default: break;
            }
            //元データと素材がともに負の場合 素材のデータに更新
            //if ((*dt3 == 0) && (orbit_[id][ELMNO + 3] == 0)) *dt3 = 0;
            //else if ((orbit_[id][ELMNO + 3] == 0)) *dt3 = *dt3;//*dt2維持
            //else if (abs(*dt3) < abs(orbit_[id][ELMNO + 3])) *dt3 = orbit_[id][ELMNO + 3];
        }
        //④振幅変化…方法
        if ((dt4 != NULL) && (*dt4 < (orbit_[id][ELMNO + 4]))) *dt4 = orbit_[id][ELMNO + 4];

        //⑤振幅変化…速度
        if (dt5 != NULL)
        {
            *dt5 += orbit_[id][ELMNO + 5];//素材データを現在のデータに足す
            if (*dt5 > 8) *dt5 = 8;//８を超えたら８に
            if (*dt5 < -8) *dt5 = -8;//－８より下回ったら－８に
        }
        //⑥頂点・腹停止
        if ((dt6 != NULL) && (orbit_[id][ELMNO + 6] != 0))//頂点・腹停止　素材にデータが入っていた時のみ更新
        {//本データが０で更新。０以外で素材と一致していたら更新しない。本データと素材で違っていたら足す
            //１：頂点停止、２：腹停止、３以上：頂点＋腹停止
            if (*dt6 == 0) *dt6 = orbit_[id][ELMNO + 6];//本データが０の時はそのまま更新
            if (*dt6 != orbit_[id][ELMNO + 6]) *dt6 += orbit_[id][ELMNO + 6];//違っていたら足す
        }
        //⑦波分割数
        if (dt7 != NULL) *dt7 += orbit_[id][ELMNO + 7];//波分割数
        //⑧絶対値波
        if ((dt8 != NULL) && (orbit_[id][ELMNO + 8] > 0)) *dt8 = 1;//波絶対値
        //    if (dt1 != NULL) *dt1 = orbit_[id][ELMNO + 1];
        //    if (dt2 != NULL) *dt2 = orbit_[id][ELMNO + 2];
        //    if (dt3 != NULL) *dt3 = orbit_[id][ELMNO + 3];
        //    if (dt4 != NULL) *dt4 = orbit_[id][ELMNO + 4];
        //    if (dt5 != NULL) *dt5 = orbit_[id][ELMNO + 5];
        //    if (dt6 != NULL) *dt6 = orbit_[id][ELMNO + 6];
        //    if (dt7 != NULL) *dt7 = orbit_[id][ELMNO + 7];
        //    if (dt8 != NULL) *dt8 = orbit_[id][ELMNO + 8];
    }

    void TDtElement::SetEffectDt(s32 id, s32* dt1)
    {
        SetEffectDt(id, dt1, NULL, NULL);
    }
    void TDtElement::SetEffectDt(s32 id, s32* dt1, s32* dt2)
    {
        SetEffectDt(id, dt1, dt2, NULL);
    }
    void TDtElement::SetEffectDt(s32 id, s32* dt1, s32* dt2, s32* dt3)
    {
        if (dt1 != NULL) *dt1 += effect_[id][ELMNO + 1];
        if (dt2 != NULL) *dt2 += effect_[id][ELMNO + 2];
        if (dt3 != NULL) *dt3 += effect_[id][ELMNO + 3];
    }
    void TDtElement::SetEffectDt2(s32 id, s32* dt1, s32* dt2, s32* dt3)
    {
        if (dt1 != NULL) *dt1 = effect_[id][ELMNO + 1];
        if (dt2 != NULL) *dt2 += effect_[id][ELMNO + 2];
        if (dt3 != NULL) *dt3 += effect_[id][ELMNO + 3];
    }

    void TDtElement::SetEffectDt3(s32 id, s32* dt1, s32* dt2, s32* dt3)
    {
        if (dt1 != NULL) *dt1 = effect_[id][ELMNO + 1];
        if (dt2 != NULL) *dt2 = effect_[id][ELMNO + 2];
        if (dt3 != NULL) *dt3 = effect_[id][ELMNO + 3];
    }
    void TDtElement::SetEffectDt4(s32 id, s32* dt1, s32* dt2, s32* dt3)
    {
        if (dt1 != NULL)
        {
            if (*dt1 < effect_[id][ELMNO + 1]) *dt1 = effect_[id][ELMNO + 1];
        }

        if (dt2 != NULL)
        {
            if ((effect_[id][ELMNO + 2] != 0)) *dt2 = effect_[id][ELMNO + 2];
        }
        if (dt3 != NULL)
        {
            if (dt3 == 0)
            {
                *dt3 = effect_[id][ELMNO + 3];
            }
            else
            { 
                *dt3 = (effect_[id][ELMNO + 3] + *dt3) / 2;
            }
        }
    }


#ifdef __K_DEBUG_SHIAI__
    std::string TDtElement::GetChangeElmName(s32 id)
    {
        return changeStr_[id][0];
    }
    std::string TDtElement::GetEffectElmName(s32 id)
    {
        return effectStr_[id][0];
    }
    std::string TDtElement::GetShapeElmName(s32 id)
    {
        return shapeStr_[id][0];
    }
    std::string TDtElement::GetOrbitElmName(s32 id)
    {
        return orbitStr_[id][0];
    }
    std::string TDtElement::GetTargetElmName(s32 id)
    {
        return targetStr_[id][0];
    }
#endif	// #ifdef __K_DEBUG_SHIAI__
}
