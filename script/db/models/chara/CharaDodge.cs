﻿namespace db;

/// <summary>
/// 避けに関するステータス
/// </summary>
public class CharaDodge
{
    // 敵コート避け時間
    public int EnemyCortDodgeCount { get; set; }

    public void Initialize()
    {
        EnemyCortDodgeCount = 0;
    }
}