using db;

namespace isdf;

/// <summary>
/// パッド入力インターフェース
/// </summary>
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

/// <summary>
/// パッド入力拡張メソッド
/// </summary>
public static class IPadExtensions
{
    /// <summary>
    /// 指定された十字キーが押されているかチェック
    /// </summary>
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

    /// <summary>
    /// いずれかの十字キーが押されているかチェック
    /// </summary>
    public static bool IsPressedAnyCross(this IPad pad)
    {
        return pad.KeyUp.IsPressed || pad.KeyDown.IsPressed || pad.KeyLeft.IsPressed || pad.KeyRight.IsPressed;
    }

    /// <summary>
    /// いずれかのボタンが押された瞬間かチェック
    /// </summary>
    public static bool IsJustPressedAnyButton(this IPad pad)
    {
        return pad.ButtonA.IsJustPressed || pad.ButtonB.IsJustPressed;
    }

    /// <summary>
    /// AボタンとBボタンが同時に押された瞬間かチェック
    /// </summary>
    public static bool IsJustPressedAbButton(this IPad pad)
    {
        return pad.ButtonA.IsJustPressed && pad.ButtonB.IsPressed
               || pad.ButtonA.IsPressed && pad.ButtonB.IsJustPressed;
    }

    /// <summary>
    /// 指定された十字キー以外が押されているかチェック
    /// </summary>
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

    /// <summary>
    /// 指定された十字キー以外が押されているかチェック（全方向）
    /// </summary>
    public static bool IsOtherCrs(this IPad pad, DirectionCrossType crs, int delay)
    {
        if (IsOtherDown(pad, DirectionCrossType.Up, crs, delay)) return true;
        if (IsOtherDown(pad, DirectionCrossType.Down, crs, delay)) return true;
        if (IsOtherDown(pad, DirectionCrossType.Left, crs, delay)) return true;
        if (IsOtherDown(pad, DirectionCrossType.Right, crs, delay)) return true;

        return false;
    }

    /// <summary>
    /// いずれかの十字キーが押された瞬間かチェック
    /// </summary>
    public static bool IsPaTgCtrl(this IPad pad)
    {
        return pad.KeyUp.IsJustPressed || pad.KeyDown.IsJustPressed || pad.KeyLeft.IsJustPressed || pad.KeyRight.IsJustPressed;
    }

    /// <summary>
    /// ダッシュマン発動入力チェック
    /// </summary>
    public static bool IsDMCall(this IPad pad)
    {
        return pad.ButtonX.IsJustPressed;
    }

    /// <summary>
    /// ダッシュマンの方向キー入力状態を取得
    /// </summary>
    public static void GetDMCallCrs(this IPad pad, out bool crsL_f, out bool crsR_f, out bool crsU_f, out bool crsD_f)
    {
        crsL_f = pad.KeyLeft.IsPressed;
        crsR_f = pad.KeyRight.IsPressed;
        crsU_f = pad.KeyUp.IsPressed;
        crsD_f = pad.KeyDown.IsPressed;
    }

    /// <summary>
    /// いずれかのボタンが押されているかチェック
    /// </summary>
    public static bool IsNext(this IPad pad)
    {
        return pad.ButtonA.IsPressed || pad.ButtonB.IsPressed || pad.ButtonX.IsPressed || pad.ButtonY.IsPressed;
    }

    /// <summary>
    /// いずれかの十字キーが押されているかチェック（歩き）
    /// </summary>
    public static bool IsAnyWalk2(this IPad pad)
    {
        return pad.KeyUp.IsPressed || pad.KeyDown.IsPressed || pad.KeyLeft.IsPressed || pad.KeyRight.IsPressed;
    }

    /// <summary>
    /// 指定された方向の歩き入力チェック
    /// </summary>
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

    /// <summary>
    /// 左右いずれかのダッシュ入力チェック
    /// </summary>
    public static bool IsDash(this IPad pad)
    {
        return pad.IsDash(DirectionCrossType.Left) || pad.IsDash(DirectionCrossType.Right);
    }

    /// <summary>
    /// 指定方向のダッシュ入力チェック
    /// </summary>
    public static bool IsDash(this IPad pad, DirectionCrossType crs)
    {
        return crs switch
        {
            DirectionCrossType.Left => pad.KeyLeft.IsDoubleTapped,
            DirectionCrossType.Right => pad.KeyRight.IsDoubleTapped,
            _ => false
        };
    }

    /// <summary>
    /// 指定方向のダッシュ加速入力チェック
    /// </summary>
    public static bool IsDashAcc(this IPad pad, DirectionCrossType crs)
    {
        return crs switch
        {
            DirectionCrossType.Left => pad.KeyLeft.IsPressed,
            DirectionCrossType.Right => pad.KeyRight.IsPressed,
            _ => false
        };
    }

    /// <summary>
    /// ジャンプ入力チェック
    /// </summary>
    public static bool IsJump(this IPad pad)
    {
        return pad.IsJustPressedAbButton();
    }

    /// <summary>
    /// 指定方向のジャンプ移動入力チェック
    /// </summary>
    public static bool IsJumpMvp(this IPad pad, DirectionCrossType crs)
    {
        return pad.IsPressedCrs(crs);
    }

    /// <summary>
    /// パス入力チェック
    /// </summary>
    public static bool IsPass(this IPad pad)
    {
        return pad.ButtonA.IsJustPressed;
    }

    /// <summary>
    /// パス長押し入力チェック（ミラーパス）
    /// </summary>
    public static bool IsPass2(this IPad pad)
    {
        return pad.ButtonA.IsPressed;
    }

    /// <summary>
    /// パスの方向入力チェック
    /// </summary>
    public static bool IsPassCrs(this IPad pad, DirectionCrossType crs)
    {
        return pad.IsPressedCrs(crs);
    }

    /// <summary>
    /// 自動ターゲット向き入力チェック（ニュートラル）
    /// </summary>
    public static bool IsAutoLook(this IPad pad)
    {
        return pad.IsPressedAnyCross() == false;
    }

    /// <summary>
    /// シュート入力チェック
    /// </summary>
    public static bool IsShot(this IPad pad)
    {
        return pad.ButtonB.IsJustPressed;
    }

    /// <summary>
    /// ジャンプシュート入力チェック
    /// </summary>
    public static bool IsJumpShot(this IPad pad)
    {
        return pad.ButtonB.IsJustPressed;
    }

    /// <summary>
    /// シュート長押し入力チェック（ミラーパスからのシュート）
    /// </summary>
    public static bool IsShot2(this IPad pad)
    {
        return pad.ButtonB.IsPressed;
    }

    /// <summary>
    /// キャッチ入力チェック
    /// </summary>
    public static bool IsCatch(this IPad pad)
    {
        return pad.ButtonB.IsJustPressed;
    }

    /// <summary>
    /// キャッチ後の指定方向ダッシュ継続入力チェック
    /// </summary>
    public static bool IsCatchDash(this IPad pad, DirectionCrossType needcrs)
    {
        return pad.IsPressedCrs(needcrs);
    }

    /// <summary>
    /// よけ入力チェック
    /// </summary>
    public static bool IsDodge(this IPad pad)
    {
        return pad.ButtonA.IsJustPressed;
    }

    /// <summary>
    /// よけ長押し入力チェック
    /// </summary>
    public static bool IsDodge2(this IPad pad)
    {
        return pad.ButtonA.IsPressed;
    }
}