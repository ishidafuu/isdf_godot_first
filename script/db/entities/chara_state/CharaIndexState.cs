﻿namespace db;

/// <summary>
/// インデックスに関するステータス
/// 生成後変化しない
/// </summary>
public class CharaIndexState
{
    // サイド番号（0～1）
    public int SideIndex { get; private set; }

    // メンバー番号（0～6）
    public int MemberIndex { get; private set; }

    public void Initialize(int sideIndex, int memberIndex)
    {
        SideIndex = sideIndex;
        MemberIndex = memberIndex;
    }
}