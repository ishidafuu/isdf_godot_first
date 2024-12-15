using db;

namespace isdf;

public interface IPad
{
    public IButton KeyUp { get; }
    public IButton KeyDown { get; }
    public IButton KeyLeft { get; }
    public IButton KeyRight { get; }
    public IButton ButtonA { get; }
    public IButton ButtonB { get; }
    public IButton ButtonX { get; }
    public IButton ButtonY { get; }
}

public static class IPadExtensions
{
    public static bool IsPressedAnyCross(this IPad pad)
    {
        return pad.KeyUp.IsPressed || pad.KeyDown.IsPressed || pad.KeyLeft.IsPressed || pad.KeyRight.IsPressed;
    }

    public static bool IsJustPressedAnyButton(this IPad pad)
    {
        return pad.ButtonA.IsJustPressed || pad.ButtonB.IsJustPressed;
    }

    public static bool IsJustPressedAbButton(this IPad pad)
    {
        return pad.ButtonA.IsJustPressed && pad.ButtonB.IsPressed
               || pad.ButtonA.IsPressed && pad.ButtonB.IsJustPressed;
    }


    //指定以外の十字が押されチェック
    public static bool IsOtherDown(this IPad pad, DirectionCrossType oBtn, DirectionCrossType crs, int delay)
    {
        if (crs == oBtn) return false;

        switch (oBtn)
        {
            case DirectionCrossType.Up: return pad.KeyDown.IsPressed || pad.KeyLeft.IsPressed || pad.KeyRight.IsPressed;
            case DirectionCrossType.Down: return pad.KeyUp.IsPressed || pad.KeyLeft.IsPressed || pad.KeyRight.IsPressed;
            case DirectionCrossType.Left: return pad.KeyUp.IsPressed || pad.KeyDown.IsPressed || pad.KeyRight.IsPressed;
            case DirectionCrossType.Right: return pad.KeyUp.IsPressed || pad.KeyDown.IsPressed || pad.KeyLeft.IsPressed;
        }

        return false;
    }

    //指定以外の十字が押されてる
    public static bool IsOtherCrs(this IPad pad, DirectionCrossType crs, int delay)
    {
        if (IsOtherDown(pad, DirectionCrossType.Up, crs, delay)) return true;
        if (IsOtherDown(pad, DirectionCrossType.Down, crs, delay)) return true;
        if (IsOtherDown(pad, DirectionCrossType.Left, crs, delay)) return true;
        if (IsOtherDown(pad, DirectionCrossType.Right, crs, delay)) return true;

        return false;
    }

    //十字が押された（操作権委譲）
    public static bool IsPaTgCtrl(this IPad pad)
    {
        return pad.KeyUp.IsJustPressed || pad.KeyDown.IsJustPressed || pad.KeyLeft.IsJustPressed || pad.KeyRight.IsJustPressed;
    }

    //ダッシュマン
    public static bool IsDMCall(this IPad pad)
    {
        return pad.ButtonX.IsJustPressed;
    }



    //ダッシュマン方向キー
    public static void GetDMCallCrs(this IPad pad, out bool crsL_f, out bool crsR_f, out bool crsU_f, out bool crsD_f)
    {
        crsL_f = pad.KeyLeft.IsPressed;
        crsR_f = pad.KeyRight.IsPressed;
        crsU_f = pad.KeyUp.IsPressed;
        crsD_f = pad.KeyDown.IsPressed;
    }

    public static bool IsNext(this IPad pad)
    {
        return pad.ButtonA.IsPressed || pad.ButtonB.IsPressed || pad.ButtonX.IsPressed || pad.ButtonY.IsPressed;
    }

    //十字入力おしっぱ（歩き）
    public static bool IsAnyWalk2(this IPad pad)
    {
        return pad.KeyUp.IsPressed || pad.KeyDown.IsPressed || pad.KeyLeft.IsPressed || pad.KeyRight.IsPressed;
    }
    public static bool IsWalk2(this IPad pad, DirectionCrossType crs)
    {
        switch (crs)
        {
            case DirectionCrossType.Up: return pad.KeyUp.IsPressed;
            case DirectionCrossType.Down: return pad.KeyDown.IsPressed;
            case DirectionCrossType.Left: return pad.KeyLeft.IsPressed;
            case DirectionCrossType.Right: return pad.KeyRight.IsPressed;
            default: return false;
        }
    }


    //ダッシュ入力
    public static bool IsDash(this IPad pad, bool ball_f)
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
    public static bool IsDashLR(this IPad pad, DirectionCrossType crs)
    {
        switch (pad.GetPadType())
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
            case enPadType_MT: break;
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
            case enPadType_DXL: res = ppad_->IsCrs2_DXL(crs); break;
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
            case enPadType_DXL: res = ppad_->IsBtn_DXL(dxA); break;
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
            case enPadType_DXL: res = ppad_->IsBtn2_DXL(dxA); break;
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
            case enPadType_DXL: res = ppad_->IsCrs2_DXL(crs); break;
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
            case enPadType_DXL: res = (ppad_->IsAnyCrs2_DXL() == FALSE); break;
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
            case enPadType_DXL: res = ppad_->IsBtn_DXL(dxB); break;
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
            case enPadType_DXL: res = ppad_->IsBtn_DXL(dxB); break;
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
            case enPadType_DXL: res = ppad_->IsBtn2_DXL(dxB); break;
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
            case enPadType_FC: break;
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
            case enPadType_DXL: res = ppad_->IsBtn_DXL(dxB); break;
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
            case enPadType_MT: break;
            case enPadType_FC: break;
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
            case enPadType_DXL: res = ppad_->IsBtn_DXL(dxA); break;
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
            case enPadType_DXL: res = ppad_->IsBtn2_DXL(dxA); break;
            case enPadType_MT: res = ppad_->IsBit2(tbTR_b); break;//右上
            case enPadType_FC: res = ppad_->IsBit2(tbTR_b); break;//右上
            case enPadType_SP: res = ppad_->IsBit2(tbTR_b); break;//右上
            case enPadType_AT: break;
            default: break;
        }
        return res;
    }

}