namespace db;

/// <summary>
/// インデックスに関するステータス
/// 生成後変化しない
/// </summary>
public class CharaIndexState
{
    // サイド番号（0～1）
    public long SideIndex { get; private set; }

    // メンバー番号（0～6）
    public long MemberIndex { get; private set; }

    public void Initialize(long sideIndex, long memberIndex)
    {
        SideIndex = sideIndex;
        MemberIndex = memberIndex;
    }
}