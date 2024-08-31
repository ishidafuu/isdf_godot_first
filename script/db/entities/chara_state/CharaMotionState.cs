namespace db;

/// <summary>
/// モーションに関するステータス
/// </summary>
public class CharaMotionState
{
    public CharaMotionType MotionType { get; private set; }
    public CharaMotionFlag CharaMotionFlag { get; private set; }
    public Counter MotionCount { get; private set; }
    public CharaMotionNo MotionNo { get; private set; }

    public void Initialize()
    {
        MotionType = default;
        CharaMotionFlag = default;
        MotionCount.Clear();
        MotionNo = default;
    }

    public bool HasFlag(CharaMotionFlag flag)
    {
        return CharaMotionFlag.HasFlag(flag);
    }

    public void SubMotionFlag(CharaMotionFlag targetFlag)
    {
        CharaMotionFlag &= ~targetFlag;
    }

    public void AddMotionFlag(CharaMotionFlag flag)
    {
        CharaMotionFlag |= flag;
    }

    public void IncrementMotionCount()
    {
        MotionCount.Add();
    }

}