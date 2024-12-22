#include "dbMgPad.h"

namespace db
{
    const s32 DEF_DHITTIME = 12;//連打受付時間
    const s32 DEF_NANAMEDHITTIME = 1;//連打受付時間

    TMgPad::TMgPad(TGame* pGame, base::TPad* ppad)
        : TGameObjGen(pGame)
    {
        ppad_ = ppad;

        stPad_.dhtime_ = DEF_DHITTIME;
    }

    TMgPad::~TMgPad()
    {
    }

    //入力方式
    enPadType TMgPad::GetPadType()
    {
        enPadType res = enPadType_AT;
#ifdef __K_DEBUG_ROOT__
        if (mid::midIsDXL())
        {

            if (GenIsShift2(sftA))//マニュアルフラグ
            {
                res = (GenIsShift2(sftB))
                    ? enPadType_MT//AB
                    : enPadType_FC;//A
            }
            else
            {
                res = (GenIsShift2(sftB))
                    ? enPadType_SP//B
                    : enPadType_AT;//
            }

            //if (mid::midIsSemiAuto())
            //{
            //    if (GenIsShift2(sftA))//マニュアルフラグ
            //    {
            //        res = (GenIsShift2(sftB))
            //            ? enPadType_MT//AB
            //            : enPadType_FC;//A
            //    }
            //    else
            //    {
            //        res = (GenIsShift2(sftB))
            //            ? enPadType_SP//B
            //            : enPadType_AT;//
            //    }
            //}
            //else
            //{
            //    res = enPadType_DXL;

            //}

        }
        else
#endif	// #ifdef __K_DEBUG_ROOT__
        {
            if (GenIsShift2(sftA))//マニュアルフラグ
            {
                res = (GenIsShift2(sftB))
                    ? enPadType_MT//AB
                    : enPadType_FC;//A
            }
            else
            {
                res = (GenIsShift2(sftB))
                    ? enPadType_SP//B
                    : enPadType_AT;//
            }
        }


        return res;
    }
    //入力方式
    BOOL TMgPad::IsPadType(enPadType padtype)
    {
        return (GetPadType() == padtype);
    }

    //ＵＩからの入力（セミオート）
    BOOL TMgPad::GenIsUIBit(enUIBitType uibit)
    {
        if (IsPadType(enPadType_AT) == FALSE) return FALSE;
        return ppad_->IsBit(uibit);
    }
    BOOL TMgPad::GenIsUIBit2(enUIBitType uibit)
    {
        if (IsPadType(enPadType_AT) == FALSE) return FALSE;
        return ppad_->IsBit2(uibit);
    }

    //共通シフト入力(ポーズも含む)
    BOOL TMgPad::GenIsShift(enShiftBitType sft)
    {
        return ppad_->IsShift(sft);
    }
    BOOL TMgPad::GenIsShift2(enShiftBitType sft)
    {
        return ppad_->IsShift2(sft);
    }


    //指定以外の十字が押されチェック
    BOOL TMgPad::IsOtherDown(enCrsType oBtn, enCrsType crs, s32 delay)
    {
        if (mid::midIsTBL()) return FALSE;

        return  ((crs != oBtn) && (ppad_->NowBuf_D(delay).IsBit(oBtn)));
    };
    //指定以外の十字が押されてる
    BOOL TMgPad::IsOtherCrs(enCrsType crs, s32 delay)
    {
        if (mid::midIsTBL()) return FALSE;

        if (IsOtherDown(dxU, crs, delay)) return TRUE;
        if (IsOtherDown(dxD, crs, delay)) return TRUE;
        if (IsOtherDown(dxL, crs, delay)) return TRUE;
        if (IsOtherDown(dxR, crs, delay)) return TRUE;

        return FALSE;
    }

    void TMgPad::SetBitPause()
    {
        mid::midSetUIInput(sftC);
    }
    void TMgPad::SetBitTouch()
    {
        mid::midSetUIInput(uiTouch);
    }
    void TMgPad::SetBitOrder()
    {
        mid::midSetUIInput(uiOrdBtn);
    }
    void TMgPad::SetBitAuto()
    {
        //フラグ無し
    }
    void TMgPad::SetBitSmart()
    {
        mid::midSetUIInput(sftB);
    }
    void TMgPad::SetBitRetro()
    {
        mid::midSetUIInput(sftA);
    }
    void TMgPad::SetBitTech()
    {
        mid::midSetUIInput(sftA | sftB);
    }
    void TMgPad::SetBitCmd(s32 cmdNo)
    {
        switch (cmdNo)
        {
        case 1: mid::midSetUIInput(uiCmdA); break;
        case 2: mid::midSetUIInput(uiCmdB); break;
        case 3: mid::midSetUIInput(uiCmdC); break;
        case 4: mid::midSetUIInput(uiCmdD); break;
        }
    }


    //十字が押された（操作権委譲）
    BOOL TMgPad::IsPaTgCtrl()
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL: res = ppad_->IsAnyCrs_DXL(); break;
        case enPadType_MT: res = ppad_->IsVec_TBL(TRUE, FALSE); break;
        case enPadType_FC: res = ppad_->IsVec_TBL(TRUE, FALSE); break;
        case enPadType_SP: res = ppad_->IsVec_TBL(TRUE, FALSE); break;
        case enPadType_AT: break;
        default: break;
        }
        return res;
    }
    //ダッシュマン
    BOOL TMgPad::IsDMCall()
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL: res = ppad_->IsBtn_DXL(dxRB); break;
        case enPadType_MT:
            res = ((ppad_->IsBit(tbTL_r) && (ppad_->IsBitL2(tbTL_b) == FALSE))
                || ppad_->IsVec_TBL(FALSE, FALSE)
                || ppad_->IsBitL(tbTL_b));//左上リリースか方向か長押し
            break;
        case enPadType_FC: break;
        case enPadType_SP: break;
        case enPadType_AT: break;
        default: break;
        }
        return res;
    }

    //TBL十六方向
    void TMgPad::GenGetVec16Crs(BOOL btm_f, BOOL right_f, BOOL* crsL_f, BOOL* crsR_f, BOOL* crsU_f, BOOL* crsD_f)
    {
        if (ppad_->IsVec_TBL(btm_f, right_f))
        {
            s32 vecagl = ppad_->GetVec16_TBL(btm_f, right_f);
            switch (vecagl) { case 0: case 1: case 2: case 14: case 15: *crsU_f = TRUE; break; }
            switch (vecagl) { case 6: case 7: case 8: case 9: case 10: *crsD_f = TRUE; break; }
            switch (vecagl) { case 2: case 3: case 4: case 5: case 6: *crsR_f = TRUE; break; }
            switch (vecagl) { case 10: case 11: case 12: case 13: case 14: *crsL_f = TRUE; break; }
        }
    }

    //ＴＢＬ入力で上下左右チェック
    BOOL TMgPad::GenIsVecCrs(BOOL btm_f, BOOL right_f, enCrsType crs)
    {
        BOOL res = FALSE;

        //方向入ってる
        if (ppad_->IsVec_TBL(btm_f, right_f))
        {
            BOOL crsL_f = FALSE;
            BOOL crsR_f = FALSE;
            BOOL crsU_f = FALSE;
            BOOL crsD_f = FALSE;
            GenGetVec16Crs(btm_f, right_f, &crsL_f, &crsR_f, &crsU_f, &crsD_f);

            switch (crs)
            {
            case dxD: res = crsD_f; break;
            case dxL:res = crsL_f; break;
            case dxR:res = crsR_f; break;
            case dxU:res = crsU_f; break;
            }
        }

        return res;
    }
    //フリック入力が入ってる
    BOOL TMgPad::GenIsFlicVecCrs(BOOL btm_f, BOOL right_f, enCrsType crs)
    {
        BOOL res = FALSE;
        u32 rbit = tbTL_r;//離したフラグ
        //ビットシフト
        if (btm_f) rbit = (rbit << BITSHIFT_BTM);
        if (right_f) rbit = (rbit << BITSHIFT_RIGHT);

        //離したフラグが立っていて、かつ、方向が入ってる
        if (ppad_->IsBit(rbit)
            && ppad_->IsVec_TBL(btm_f, right_f))
        {
            //方向の向きもあってる
            res = GenIsVecCrs(btm_f, right_f, crs);
        }

        return res;
    }

    //ダッシュマン方向キー
    void TMgPad::GetDMCallCrs(BOOL* crsL_f, BOOL* crsR_f, BOOL* crsU_f, BOOL* crsD_f)
    {
        switch (GetPadType())
        {
        case enPadType_DXL:
            *crsL_f = ppad_->IsCrs2_DXL(dxL);
            *crsR_f = ppad_->IsCrs2_DXL(dxR);
            *crsU_f = ppad_->IsCrs2_DXL(dxU);
            *crsD_f = ppad_->IsCrs2_DXL(dxD);
            break;
        case enPadType_MT: GenGetVec16Crs(FALSE, FALSE, crsL_f, crsR_f, crsU_f, crsD_f); break;//左上
        case enPadType_FC: break;
        case enPadType_SP: break;
        case enPadType_AT: break;
        default: break;
        }
    }

    //ＤＸＬボタン入力（シュート切り替えデバッグ用）
    BOOL TMgPad::IsDXLBtn(enBtnType btn)
    {
        return ppad_->IsBtn_DXL(btn);
    }

    //ポーズボタン
    BOOL TMgPad::IsPause()
    {
        return GenIsShift(sftC);
    }

    //画面タッチ（セミオート）
    BOOL TMgPad::IsTouch()
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_AT: res = GenIsUIBit(uiTouch); break;
        default: break;
        }
        return res;
    }

    BOOL TMgPad::IsNext()
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL: res = ppad_->IsAnyBtn_DXL(); break;
        case enPadType_MT: res = (ppad_->IsBit(tbBL_b) || ppad_->IsBit(tbBR_b) || ppad_->IsBit(tbTL_b) || ppad_->IsBit(tbTR_b)); break;
        case enPadType_FC: res = (ppad_->IsBit(tbBL_b) || ppad_->IsBit(tbBR_b) || ppad_->IsBit(tbTL_b) || ppad_->IsBit(tbTR_b)); break;
        case enPadType_SP: res = (ppad_->IsBit(tbBL_b) || ppad_->IsBit(tbTR_b)); break;
        case enPadType_AT: res = GenIsUIBit(uiTouch); break;
        default: break;
        }
        return res;
    }

    //セミオートか
    BOOL TMgPad::IsSemiAuto()
    {
        return IsPadType(enPadType_AT);
    }
    //オーダーか
    BOOL TMgPad::IsSemiAutoOrder()
    {
        return GenIsUIBit(uiOrdBtn);
    }
    //スマートか
    BOOL TMgPad::IsSmart()
    {
        return IsPadType(enPadType_SP);
    }

    //作戦番号
    s32 TMgPad::GetCmdNo()
    {
        s32 res = 0;
        if (GenIsUIBit2(uiCmdA)) res = 1;
        else if (GenIsUIBit2(uiCmdB)) res = 2;
        else if (GenIsUIBit2(uiCmdC)) res = 3;
        else if (GenIsUIBit2(uiCmdD)) res = 4;
        return res;
    }

    //十字入力おしっぱ（歩き）
    BOOL TMgPad::IsAnyWalk2()
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL: res = ppad_->IsAnyCrs2_DXL(); break;
        case enPadType_MT: res = ppad_->IsVec_TBL(TRUE, FALSE); break;//左下
        case enPadType_FC: res = ppad_->IsVec_TBL(TRUE, FALSE); break;//左下
        case enPadType_SP: res = ppad_->IsVec_TBL(TRUE, FALSE); break;//左下
        case enPadType_AT: break;
        default: break;
        }
        return res;
    }
    BOOL TMgPad::IsWalk2(enCrsType crs)
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL: res = ppad_->IsCrs2_DXL(crs); break;
        case enPadType_MT: res = GenIsVecCrs(TRUE, FALSE, crs); break;//左下
        case enPadType_FC: res = GenIsVecCrs(TRUE, FALSE, crs); break;//左下
        case enPadType_SP: res = GenIsVecCrs(TRUE, FALSE, crs); break;//左下
        case enPadType_AT: break;
        default: break;
        }
        return res;
    }
    //アナログ歩き
    BOOL TMgPad::IsWalkVec()
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL: break;
        case enPadType_MT: res = ppad_->IsVec_TBL(TRUE, FALSE); break;//左下
        case enPadType_FC: res = ppad_->IsVec_TBL(TRUE, FALSE); break;//左下
        case enPadType_SP: res = ppad_->IsVec_TBL(TRUE, FALSE); break;//左下
        case enPadType_AT: break;
        default: break;
        }
        return res;
    }
    u32 TMgPad::GetWalkVec()
    {
        u32 res = 0;
        switch (GetPadType())
        {
        case enPadType_DXL: break;
        case enPadType_MT: res = ppad_->GetVec16_TBL(TRUE, FALSE); break;//左下
        case enPadType_FC: res = ppad_->GetVec16_TBL(TRUE, FALSE); break;//左下
        case enPadType_SP: res = ppad_->GetVec16_TBL(TRUE, FALSE); break;//左下
        case enPadType_AT: break;
        default: break;
        }
        return res;
    }

    //レバー画像番号
    s32 TMgPad::GetLeverNo()
    {
        s32 res = 0;//押してなければ０
        if (ppad_->IsVec_TBL(TRUE, FALSE))
        {
            res = (ppad_->GetVec16_TBL(TRUE, FALSE) / 2) + 1;//1スタート
        }
        return res;
    }

    //ダッシュ入力
    BOOL TMgPad::IsDash(BOOL ball_f)
    {
        if (ball_f)
        {
            return (IsDashLR(dxL) || IsDashLR(dxR) || IsDashAuto());
        }
        else
        {
            return (IsDashLR(dxL) || IsDashLR(dxR));
        }
    }

    //ダッシュ方向（斜めダッシュも必要であればそういう）
    BOOL TMgPad::IsDashLR(enCrsType crs)
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL: res = (stPad_.nowDHKeyLR_ == crs); break;
        case enPadType_MT: res = GenIsFlicVecCrs(TRUE, FALSE, crs); break;//左下
        case enPadType_FC: res = GenIsFlicVecCrs(TRUE, FALSE, crs); break;//左下
        case enPadType_SP: res = GenIsFlicVecCrs(TRUE, FALSE, crs); break;//左下
        case enPadType_AT: break;
        default: break;
        }

        return res;
    }

    BOOL TMgPad::IsDashAuto()
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL: break;
        case enPadType_MT: break;
        case enPadType_FC: break;
        case enPadType_SP: res = (ppad_->IsBit(tbBL_r) && (ppad_->IsBitL2(tbBL_b) == FALSE)); break;
        case enPadType_AT: break;
        default: break;
        }

        return res;
    }
    //BOOL TMgPad::IsCaCounter()
    //{
    //  BOOL res = FALSE;
    //  switch (GetPadType())
    //  {
    //  case enPadType_DXL: break;
    //  case enPadType_MT: break;
    //  case enPadType_FC: break;
    //  case enPadType_SP: res = ppad_->IsBit2(tbBL_b); break;
    //  case enPadType_AT: break;
    //  default: break;
    //  }

    //  return res;
    //}

    //ダッシュ加速入力
    BOOL TMgPad::IsDashAcc(enCrsType needcrs)
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL: res = ppad_->IsCrs2_DXL(needcrs); break;
        case enPadType_MT: res = GenIsVecCrs(TRUE, FALSE, needcrs); break;//左下
        case enPadType_FC: res = GenIsVecCrs(TRUE, FALSE, needcrs); break;//左下
        case enPadType_SP: res = GenIsVecCrs(TRUE, FALSE, needcrs); break;//左下
        case enPadType_AT: break;
        default: break;
        }
        return res;
    }
    //ダッシュストップ
    BOOL TMgPad::IsDashStop(enCrsType needcrs)
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL: break;
        case enPadType_MT: ppad_->IsBitL(tbBL_b); break;//左下長押し
        case enPadType_FC: break;
        case enPadType_SP: res = GenIsFlicVecCrs(TRUE, FALSE, needcrs); break;//左下
        case enPadType_AT: break;
        default: break;
        }
        return res;
    }

    //ジャンプ入力
    BOOL TMgPad::IsJump()//ジャンプ
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL:
            res = (ppad_->IsBtn2_DXL(dxA) && ppad_->IsBtn_DXL(dxB))
                || ((ppad_->IsBtn2_DXL(dxB) && ppad_->IsBtn_DXL(dxA)));
            break;
        case enPadType_MT: res = GenIsFlicVecCrs(TRUE, TRUE, dxU); break;//右下 上フリック
        case enPadType_FC:
            res = (GenIsFlicVecCrs(TRUE, FALSE, dxU) || ppad_->IsBit(tbTL_b)); //左下 上フリックか左上タッチ
            break;
        case enPadType_SP:
            res = (GenIsFlicVecCrs(TRUE, FALSE, dxU));//上フリック
            break;
        case enPadType_AT: break;
        default: break;
        }
        return res;
    }
    BOOL TMgPad::IsJumpLongTouch()//ジャンプ
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL:
            break;
        case enPadType_MT:
            break;
        case enPadType_FC:
            break;
        case enPadType_SP:
            res = (ppad_->IsBitL(tbBL_b) && (ppad_->IsVec_TBL(TRUE, FALSE) == FALSE));//長押し＆方向はいらず
            break;
        case enPadType_AT: break;
        default: break;
        }
        return res;
    }
    BOOL TMgPad::IsDsJumpLongTouch()//ダッシュ中スワイプジャンプ
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL:
            break;
        case enPadType_MT:
            break;
        case enPadType_FC:
            break;
        case enPadType_SP:
            res = (ppad_->IsBitL(tbBL_b));//長押し＆方向はいらず
            break;
        case enPadType_AT: break;
        default: break;
        }
        return res;
    }

    //オートジャンプ入力
    BOOL TMgPad::IsJump2()//ジャンプ
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL: break;
        case enPadType_MT:  break;
        case enPadType_FC:
            res = ppad_->IsBit2(tbTL_b);
            break;
        case enPadType_SP:
            res = ppad_->IsBit2(tbBL_b);//押してる
            break;
        case enPadType_AT: break;
        default: break;
        }
        return res;
    }
    //十字入力ジャンプ
    BOOL TMgPad::IsJumpMv(enCrsType crs)
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL:res = ppad_->IsCrs2_DXL(crs); break;
        case enPadType_MT: res = GenIsVecCrs(TRUE, FALSE, crs); break;//左下
        case enPadType_FC: break;
        case enPadType_SP: break;
        case enPadType_AT: break;
        default: break;
        }
        return res;
    }

    //パス入力
    BOOL TMgPad::IsPass()
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL:res = ppad_->IsBtn_DXL(dxA); break;
        case enPadType_MT:
            res = ((ppad_->IsBit(tbTR_r) && (ppad_->IsBitL2(tbTR_b) == FALSE))
                || ppad_->IsVec_TBL(FALSE, TRUE)
                || ppad_->IsBitL(tbTR_b));//右上短リリースか方向か長押し
            break;
        case enPadType_FC: res = ppad_->IsBit(tbTR_b); break;//右上押された瞬間か
        case enPadType_SP:
            res = ((ppad_->IsBit(tbTR_r) && (ppad_->IsBitL2(tbTR_b) == FALSE))
                || ppad_->IsBitL(tbTR_b));//右上短リリースか長押し
            break;
        case enPadType_AT: break;
        default: break;
        }
        return res;
    }
    //パス入力おしっぱ（ミラーパス）
    BOOL TMgPad::IsPass2()
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL:res = ppad_->IsBtn2_DXL(dxA); break;
        case enPadType_MT: res = ppad_->IsBit2(tbTR_b); break;//右上
        case enPadType_FC: res = ppad_->IsBit2(tbTR_b); break;//右上
        case enPadType_SP: res = ppad_->IsBit2(tbTR_b); break;//右上
        case enPadType_AT: break;
        default: break;
        }
        return res;
    }
    //パス方向
    BOOL TMgPad::IsPassCrs(enCrsType crs)
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL:res = ppad_->IsCrs2_DXL(crs); break;
        case enPadType_MT: res = GenIsVecCrs(FALSE, TRUE, crs); break;//右上
        case enPadType_FC: res = GenIsVecCrs(TRUE, FALSE, crs); break;//左下
        case enPadType_SP: res = GenIsVecCrs(FALSE, TRUE, crs); break;//右上
        case enPadType_AT: break;
        default: break;
        }
        return res;
    }

    //自動タゲ向き（ニュートラル）
    BOOL TMgPad::IsAutoLook()
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL:res = (ppad_->IsAnyCrs2_DXL() == FALSE); break;
        case enPadType_MT: res = (ppad_->IsVec_TBL(TRUE, FALSE) == FALSE); break;//左下
        case enPadType_FC: res = (ppad_->IsVec_TBL(TRUE, FALSE) == FALSE); break;//左下
        case enPadType_SP: res = TRUE; break;//自動タゲ
        case enPadType_AT: res = TRUE; break;//自動タゲ
        default: break;
        }
        return res;
    }

    //シュート
    BOOL TMgPad::IsShot()
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL:res = ppad_->IsBtn_DXL(dxB); break;
        case enPadType_MT: res = ppad_->IsBit(tbBR_b); break;//右下
        case enPadType_FC: res = ppad_->IsBit(tbBR_b); break;//右下
        case enPadType_SP: res = (ppad_->IsBit(tbBL_r) && (ppad_->IsBitL2(tbBL_b) == FALSE) && (ppad_->IsVec_TBL(TRUE, FALSE) == FALSE)); break;//左下短押し
        case enPadType_AT: res = GenIsUIBit(uiTouch); break;//タッチ
        default: break;
        }

        return res;

    }
    //シュート
    BOOL TMgPad::IsJumpShot()
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL:res = ppad_->IsBtn_DXL(dxB); break;
        case enPadType_MT: res = ppad_->IsBit(tbBR_b); break;//右下
        case enPadType_FC: res = ppad_->IsBit(tbBR_b); break;//右下
        case enPadType_SP: res = ppad_->IsBit(tbBL_b); break;//左下
        case enPadType_AT: res = GenIsUIBit(uiTouch); break;//タッチ
        default: break;
        }

        return res;

    }
    //シュートおしっぱ（ミラーパスからのシュート）
    BOOL TMgPad::IsShot2()
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL:res = ppad_->IsBtn2_DXL(dxB); break;
        case enPadType_MT: res = ppad_->IsBit2(tbBR_b); break;//右下
        case enPadType_FC: res = ppad_->IsBit2(tbBR_b); break;//右下
        case enPadType_SP: res = ppad_->IsBit2(tbBL_b); break;//左下
        case enPadType_AT: res = GenIsUIBit2(uiTouch); break;//タッチ
        default: break;
        }
        return res;
    }

    //ダッシュシュート
    BOOL TMgPad::IsDsShot(enCrsType crs)
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL: break;
        case enPadType_MT: break;
        case enPadType_FC:  break;
        case enPadType_SP: res = GenIsFlicVecCrs(TRUE, FALSE, crs); break;//左下
        case enPadType_AT: break;
        default: break;
        }

        return res;
    }

    //キャッチ入力
    BOOL TMgPad::IsCatch()
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL:res = ppad_->IsBtn_DXL(dxB); break;
        case enPadType_MT: res = ppad_->IsBit(tbBR_b); break;//右下
        case enPadType_FC: res = ppad_->IsBit(tbBR_b); break;//右下
        case enPadType_SP: res = ppad_->IsBit(tbBL_b); break;//左下
        case enPadType_AT: res = GenIsUIBit(uiTouch); break;//タッチ
        default: break;
        }
        return res;
    }

    //キャッチ入力（リリース）
    BOOL TMgPad::IsCatch3()
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL: break;
        case enPadType_MT:break;
        case enPadType_FC:  break;
        case enPadType_SP:
            res = ((ppad_->IsBit(tbBL_r) && (ppad_->IsBitL2(tbBL_b) == FALSE))
                //|| ppad_->IsBitL(tbTR_b)
                );//左下短リリースか長押し
            break;
        case enPadType_AT: break;
        default: break;
        }
        return res;
    }


    //キャッチ後ダッシュ継続入力
    BOOL TMgPad::IsCatchDash(enCrsType needcrs)
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL: res = ppad_->IsCrs2_DXL(needcrs); break;
        case enPadType_MT: res = GenIsVecCrs(TRUE, FALSE, needcrs); break;//左下
        case enPadType_FC: res = GenIsVecCrs(TRUE, FALSE, needcrs); break;
        case enPadType_SP: break;
        case enPadType_AT: break;
        default: break;
        }
        return res;
    }

    //よけ入力
    BOOL TMgPad::IsDodge()
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL:res = ppad_->IsBtn_DXL(dxA); break;
        case enPadType_MT: res = ppad_->IsBit(tbTR_b); break;//右上
        case enPadType_FC: res = ppad_->IsBit(tbTR_b); break;//右上
        case enPadType_SP: res = ppad_->IsBit(tbTR_b); break;//右上
        case enPadType_AT: break;
        default: break;
        }
        return res;
    }
    //よけ入力おしっぱ
    BOOL TMgPad::IsDodge2()
    {
        BOOL res = FALSE;
        switch (GetPadType())
        {
        case enPadType_DXL:res = ppad_->IsBtn2_DXL(dxA); break;
        case enPadType_MT: res = ppad_->IsBit2(tbTR_b); break;//右上
        case enPadType_FC: res = ppad_->IsBit2(tbTR_b); break;//右上
        case enPadType_SP: res = ppad_->IsBit2(tbTR_b); break;//右上
        case enPadType_AT: break;
        default: break;
        }
        return res;
    }



    //変化球
    //BOOL TMgPad::IsCarve2(enCrsType crs);


    //連打チェック
    void TMgPad::SetDHit()
    {
        if (mid::midIsTBL()) return;

        //暫定連打ボタン
        enCrsType tCrs;
        enCrsType tDH;
        BOOL neutral_f = FALSE;

        //とりあえず初期化
        //２フレ連続で連打はないようにする
        if (stPad_.nowDHKey_ != dxcNone)
        {
            stPad_.nowDHKey_ = dxcNone;
            return;
        }

        //押した瞬間のみ
        if (ppad_->IsAnyCrs_DXL() == FALSE) return;

        //現在のキーから連打判定キー１つを決める
        if (ppad_->NowBuf().IsBit(dxU))
        {
            tCrs = dxU;
            tDH = dxU;
        }
        else if (ppad_->NowBuf().IsBit(dxD))
        {
            tCrs = dxD;
            tDH = dxD;
        }
        else if (ppad_->NowBuf().IsBit(dxL))
        {
            tCrs = dxL;
            tDH = dxL;
        }
        else if (ppad_->NowBuf().IsBit(dxR))
        {
            tCrs = dxR;
            tDH = dxR;
        }
        else
        {
            //押されてない時点で連打ではない
            return;
        }

        const s32 CHECKSTARTPOS = 1;
        for (s32 i = CHECKSTARTPOS; i < (stPad_.dhtime_ + 1); ++i)
        {
            //違うキーが見つかったらその時点で連打ではない
            if (IsOtherCrs(tCrs, i)) return;

            //ニュートラルが見つかってないとき
            if (neutral_f == FALSE)
            {
                if (ppad_->IsAnyCrs2_D_DXL(i) == FALSE)
                {
                    neutral_f = TRUE;
                }
            }
            else//見つかったら１打目を探す
            {
                if (ppad_->NowBuf_D(i).IsBit(tCrs))
                {
                    stPad_.nowDHKey_ = tDH;//見つかった時点でＯＫ
                    return;
                }
            }
        }

    }

    void TMgPad::SetDHitUDLR()
    {
        if (mid::midIsTBL()) return;

        if (stPad_.dhndelay_f_ == FALSE)
        {
            SettmpDHitUD();
            SettmpDHitLR();
            stPad_.nowDHKeyUD_ = dxcNone;
            stPad_.nowDHKeyLR_ = dxcNone;
        }
        else//片方が入って１フレ以内に別軸をスキャン
        {
            if (stPad_.tmpDHKeyUD_ == dxcNone)
            {
                SettmpDHitUD();
            }
            else if (stPad_.tmpDHKeyLR_ == dxcNone)
            {
                SettmpDHitLR();
            }

            stPad_.nowDHKeyUD_ = stPad_.tmpDHKeyUD_;
            stPad_.nowDHKeyLR_ = stPad_.tmpDHKeyLR_;

            stPad_.dhndelay_f_ = FALSE;
        }
    }

    //連打チェック
    void TMgPad::SettmpDHitUD()
    {
        if (mid::midIsTBL()) return;

        //暫定連打ボタン
        enCrsType tCrs;
        enCrsType tDH;
        BOOL neutral_f = FALSE;

        //とりあえず初期化
        //２フレ連続で連打はないようにする
        if (stPad_.tmpDHKeyUD_ != dxcNone)
        {
            stPad_.tmpDHKeyUD_ = dxcNone;
            return;
        }

        //現在のキーから連打判定キー１つを決める
        if (ppad_->NowBuf().IsBit(dxU))
        {
            tCrs = dxU;
            tDH = dxU;
        }
        else if (ppad_->NowBuf().IsBit(dxD))
        {
            tCrs = dxD;
            tDH = dxD;
        }
        else
        {
            //押されてない時点で連打ではない
            return;
        }

        const s32 CHECKSTARTPOS = 1;
        for (s32 i = CHECKSTARTPOS; i < (stPad_.dhtime_ + 1); ++i)
        {
            //違うキーが見つかったらその時点で連打ではない
            //if (IsOtherCrs(tCrs, i)) return;
            if (IsOtherDown(dxU, tCrs, i)) return;
            if (IsOtherDown(dxD, tCrs, i)) return;


            //ニュートラルが見つかってないとき
            if (neutral_f == FALSE)
            {
                //ニュートラルは上下左右無入力にしないと右押しながら↑↑とかで連打になってしまう

                if (ppad_->IsAnyCrs2_D_DXL(i) == FALSE)
                    //if (((ppad_->NowBuf_D(i).IsBit(dxU) || ppad_->NowBuf_D(i).IsBit(dxD))) == FALSE)
                {
                    neutral_f = TRUE;
                }
            }
            else//見つかったら１打目を探す
            {
                if (ppad_->NowBuf_D(i).IsBit(tCrs))
                {
                    stPad_.tmpDHKeyUD_ = tDH;//見つかった時点でＯＫ
                    stPad_.dhndelay_f_ = TRUE;
                    return;
                }
            }
        }

    }

    //連打チェック
    void TMgPad::SettmpDHitLR()
    {
        if (mid::midIsTBL()) return;

        //暫定連打ボタン
        enCrsType tCrs;
        enCrsType tDH;
        BOOL neutral_f = FALSE;

        //とりあえず初期化
        //２フレ連続で連打はないようにする
        if (stPad_.tmpDHKeyLR_ != dxcNone)
        {
            stPad_.tmpDHKeyLR_ = dxcNone;
            return;
        }

        //現在のキーから連打判定キー１つを決める
        if (ppad_->NowBuf().IsBit(dxL))
        {
            tCrs = dxL;
            tDH = dxL;
        }
        else if (ppad_->NowBuf().IsBit(dxR))
        {
            tCrs = dxR;
            tDH = dxR;
        }
        else
        {
            //押されてない時点で連打ではない
            return;
        }

        const s32 CHECKSTARTPOS = 1;
        for (s32 i = CHECKSTARTPOS; i < (stPad_.dhtime_ + 1); ++i)
        {
            //違うキーが見つかったらその時点で連打ではない
            //if (IsOtherCrs(tCrs, i)) return;
            if (IsOtherDown(dxL, tCrs, i)) return;
            if (IsOtherDown(dxR, tCrs, i)) return;


            //ニュートラルが見つかってないとき
            if (neutral_f == FALSE)
            {
                if (ppad_->IsAnyCrs2_D_DXL(i) == FALSE)
                {
                    neutral_f = TRUE;
                }
            }
            else//見つかったら１打目を探す
            {
                if (ppad_->NowBuf_D(i).IsBit(tCrs))
                {
                    stPad_.tmpDHKeyLR_ = tDH;//見つかった時点でＯＫ
                    stPad_.dhndelay_f_ = TRUE;
                    return;
                }
            }
        }

    }

    //更新
    void TMgPad::Update()//更新
    {
        //if (mid::midIsTBL()) return;

        //すべてＤＸＬ

        //連打チェック
        SetDHit();
        SetDHitUDLR();
    }
}
