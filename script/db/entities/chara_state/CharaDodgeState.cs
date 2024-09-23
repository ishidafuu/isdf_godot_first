﻿namespace db;

/// <summary>
/// 避けに関するステータス
/// </summary>
public class CharaDodgeState
{
    // 敵コート避け時間
    public DownCounter EnemyCortDodgeCount { get; set; }

    public void Initialize()
    {
        EnemyCortDodgeCount.Clear();
    }
}