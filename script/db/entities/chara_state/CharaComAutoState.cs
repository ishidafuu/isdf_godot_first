using System;

namespace db;

/// <summary>
/// COMオート動作に関するステータス
/// </summary>
public class CharaComAutoState : ICharaComAutoStateGetter, ICharaComAutoStateSetter
{
    public int JumpBallStep { get; set; }
    public int JumpBallWaitCount { get; set; }

    public int ComMoveCount { get; set; }
    public int KeepDirectionCount { get; set; }
    public DirectionXType MoveDirectionX { get; set; }
    public DirectionZType MoveDirectionZ { get; set; }
    public int[] CatchRec { get; set; } = new int[Defines.ComAttackCacheLength];

    public int ComCatchProbability { get; set; }

    // 敵ナイスキャッチ確率
    public int ComNiceCatchProbability { get; set; }

    // キャッチカウンター状態
    public bool IsComCounter { get; set; }

    // 内野からのトスパスを受けた状態
    public bool IsComTossPassGet { get; set; }


    public void Initialize()
    {
        JumpBallStep = 0;
        JumpBallWaitCount = 0;
        ComMoveCount = 0;
        KeepDirectionCount = 0;
        MoveDirectionX = default;
        MoveDirectionZ = default;
        ComCatchProbability = 0;
        ComNiceCatchProbability = 0;
        IsComCounter = false;
        IsComTossPassGet = false;
        Array.Clear(CatchRec, 0, CatchRec.Length);
    }
}


public interface ICharaComAutoStateGetter
{
    int JumpBallStep { get; }
    int JumpBallWaitCount { get; }
    int ComMoveCount { get; }
    int KeepDirectionCount { get; }
    DirectionXType MoveDirectionX { get; }
    DirectionZType MoveDirectionZ { get; }
    int ComCatchProbability { get; }
    int ComNiceCatchProbability { get; }
    bool IsComCounter { get; }
    bool IsComTossPassGet { get; }
    int[] CatchRec { get; }
}


public interface ICharaComAutoStateSetter
{
    int JumpBallStep { set; }
    int JumpBallWaitCount { set; }
    int ComMoveCount { set; }
    int KeepDirectionCount { set; }
    DirectionXType MoveDirectionX { set; }
    DirectionZType MoveDirectionZ { set; }
    int ComCatchProbability { set; }
    int ComNiceCatchProbability { set; }
    bool IsComCounter { set; }
    bool IsComTossPassGet { set; }
    int[] CatchRec { set; }
}
