﻿namespace db;

/// <summary>
/// 描画情報
/// </summary>
public class CharaAnimeState
{
    public int FrameNo { get; set; }
    public int AnimationCount { get; set; }
    public int LoopCount { get; set; }
    public int LoopStartNo { get; set; }
    public bool IsActionPoint { get; set; }

    public void Initialize()
    {
        FrameNo = 0;
        AnimationCount = 0;
        LoopCount = 0;
        LoopStartNo = 0;
        IsActionPoint = false;
    }
}