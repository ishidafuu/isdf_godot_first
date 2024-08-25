namespace db;

public class CharaMotionState
{
    public CharaMotionType MotionType { get; set; }
    public CharaMotionFlag CharaMotionFlag { get; set; }
    public int MotionCount { get; set; }
    public CharaMotionNo MotionNo { get; set; }

    public void Initialize()
    {
        MotionType = default;
        CharaMotionFlag = default;
        MotionCount = 0;
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

}