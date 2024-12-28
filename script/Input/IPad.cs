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
    public static bool IsPressedCrs(this IPad pad, DirectionCrossType crs)
    {
        switch (crs)
        {
            case DirectionCrossType.Up: return pad.KeyUp.IsPressed;
            case DirectionCrossType.Down: return pad.KeyDown.IsPressed;
            case DirectionCrossType.Left: return pad.KeyLeft.IsPressed;
            case DirectionCrossType.Right: return pad.KeyRight.IsPressed;
        }
        return false;
    }

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
    public static bool IsDash(this IPad pad)
    {
        return pad.IsDash(DirectionCrossType.Left) || pad.IsDash(DirectionCrossType.Right);
    }

    //ダッシュ方向（斜めダッシュも必要であればそういう）
    public static bool IsDash(this IPad pad, DirectionCrossType crs)
    {
        return crs switch
        {
            DirectionCrossType.Left => pad.KeyLeft.IsDoubleTapped,
            DirectionCrossType.Right => pad.KeyRight.IsDoubleTapped,
            _ => false
        };
    }

    //ダッシュ加速入力
    public static bool IsDashAcc(this IPad pad, DirectionCrossType crs)
    {
        return crs switch
        {
            DirectionCrossType.Left => pad.KeyLeft.IsPressed,
            DirectionCrossType.Right => pad.KeyRight.IsPressed,
            _ => false
        };
    }

    public static bool IsJump(this IPad pad)
    {
        return pad.IsJustPressedAbButton();
    }

    //十字入力ジャンプ
    public static bool IsJumpMvp(this IPad pad, DirectionCrossType crs)
    {
        return pad.IsPressedCrs(crs);
    }

    //パス入力
    public static bool IsPass(this IPad pad)
    {
        return pad.ButtonA.IsJustPressed;
    }
    //パス入力おしっぱ（ミラーパス）
    public static bool IsPass2(this IPad pad)
    {
        return pad.ButtonA.IsPressed;
    }
    //パス方向
    public static bool IsPassCrs(this IPad pad, DirectionCrossType crs)
    {
        return pad.IsPressedCrs(crs);
    }

    //自動タゲ向き（ニュートラル）
    public static bool IsAutoLook(this IPad pad)
    {
        return pad.IsPressedAnyCross() == false;
    }

    //シュート
    public static bool IsShot(this IPad pad)
    {
        return pad.ButtonB.IsJustPressed;
    }
    //シュート
    public static bool IsJumpShot(this IPad pad)
    {
        return pad.ButtonB.IsJustPressed;
    }
    //シュートおしっぱ（ミラーパスからのシュート）
    public static bool IsShot2(this IPad pad)
    {
        return pad.ButtonB.IsPressed;
    }


    //キャッチ入力
    public static bool IsCatch(this IPad pad)
    {
        return pad.ButtonB.IsJustPressed;
    }

    //キャッチ後ダッシュ継続入力
    public static bool IsCatchDash(this IPad pad, DirectionCrossType needcrs)
    {
        return pad.IsPressedCrs(needcrs);
    }

    //よけ入力
    public static bool IsDodge(this IPad pad)
    {
        return pad.ButtonA.IsJustPressed;
    }

    //よけ入力おしっぱ
    public static bool IsDodge2(this IPad pad)
    {
        return pad.ButtonA.IsPressed;
    }
}