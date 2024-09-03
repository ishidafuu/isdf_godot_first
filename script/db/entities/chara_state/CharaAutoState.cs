﻿namespace db;

/// <summary>
/// オート情報
/// </summary>
public class CharaAutoState
{
    public AutoTypes AutoTypes { get; set; }
    public DirectionXType DirectionX { get; set; }
    public DirectionZType DirectionZ { get; set; }

    public bool IsDash { get; set; }

    // 一旦止まったフラグ
    public bool IsDashStopped { get; set; }

    // フリーアクションフラグ
    public bool IsFreeAction { get; set; }

    public void Initialize()
    {
        AutoTypes = default;
        DirectionX = default;
        DirectionZ = default;
        IsDash = false;
        IsDashStopped = false;
        IsFreeAction = false;
    }
}