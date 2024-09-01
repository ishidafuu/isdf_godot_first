namespace db;

/// <summary>
/// モーションに関するステータス
/// </summary>
public class CharaMotionState
{
    public CharaMotionType MotionType { get; private set; }
    public CharaMotionFlag MotionFlag { get; private set; }
    public Counter MotionCount { get; private set; }
    public CharaMotionNo MotionNo { get; private set; }

    public void Initialize()
    {
        MotionType = default;
        MotionFlag = default;
        MotionCount.Clear();
        MotionNo = default;
    }

    public bool HasFlag(CharaMotionFlag flag)
    {
        return MotionFlag.HasFlag(flag);
    }

    public void SubMotionFlag(CharaMotionFlag targetFlag)
    {
        MotionFlag &= ~targetFlag;
    }

    public void AddMotionFlag(CharaMotionFlag flag)
    {
        MotionFlag |= flag;
    }

    public void Progress()
    {
        MotionCount.Add();
    }

    public void SetMotionNo(CharaMotionType motionType, CharaMotionNo motionNo, CharaMotionFlag motionFlag,
        DirectionZType directionZType)
    {
        MotionType = motionType;
        MotionFlag = motionFlag;
        MotionNo = directionZType switch
        {
            DirectionZType.Neutral => motionNo + 1,
            DirectionZType.Backward => motionNo + 2,
            _ => motionNo
        };
    }

    public void SetMotionNo(CharaMotionType motionType, CharaMotionNo motionNo, CharaMotionFlag motionFlag)
    {
        MotionType = motionType;
        MotionFlag = motionFlag;
        MotionNo = motionNo;
    }
}