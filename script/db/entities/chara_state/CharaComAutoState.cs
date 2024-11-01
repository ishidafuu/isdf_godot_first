using System;

namespace db;

/// <summary>
/// COMオート動作に関するステータス
/// </summary>
public class CharaComAutoState : ICharaComAutoState
{
    public int JumpBallStep { get; set; }
    public int JumpBallWaitCount { get; set; }

    public int ComMoveCount { get; set; }
    public int KeepDirectionCount { get; set; }
    public DirectionXType MoveDirectionX { get; set; }
    public DirectionZType MoveDirectionZ { get; set; }
    public int[] CatchRec { get; set; } = new int[Defines.ComAttackCacheLength];

    public void Initialize()
    {
        JumpBallStep = 0;
        JumpBallWaitCount = 0;
        ComMoveCount = 0;
        KeepDirectionCount = 0;
        MoveDirectionX = default;
        MoveDirectionZ = default;
        Array.Clear(CatchRec, 0, CatchRec.Length);
    }
}

public interface ICharaComAutoState
{
    int JumpBallStep { get; }
    int JumpBallWaitCount { get; }
    int ComMoveCount { get; }
    int KeepDirectionCount { get; }
    DirectionXType MoveDirectionX { get; }
    DirectionZType MoveDirectionZ { get; }
    int[] CatchRec { get; }
}