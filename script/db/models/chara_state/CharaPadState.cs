﻿namespace db;

/// <summary>
/// パッド入力に関するステータス
/// </summary>
public class CharaPadState
{
    /// <summary>
    /// 操作キャラ
    /// </summary>
    public bool IsManualControl;

    public int PadNo;
    // public TMgPad pCharPad_; // パッド

    public void Initialize()
    {
        IsManualControl = false;
        PadNo = 0;
    }
}