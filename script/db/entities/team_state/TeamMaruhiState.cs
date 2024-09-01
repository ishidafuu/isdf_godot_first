using System;
using db.masters;

namespace db;

/// <summary>
/// マル秘作成に関するステータス
/// </summary>
public class TeamMaruhiState
{
    public int CallNum { get; set; } // 呼ぶ人数
    public int CallDelay { get; set; } // ディレイ
    public int DmType { get; set; } // ダッシュマンタイプ
    public int CallC { get; set; } // 呼んだ人数
    public int DelayC { get; set; } // ディレイ用カウンタ

    public void Initialize()
    {
        CallNum = 0;
        CallDelay = 0;
        DmType = 0;
        CallC = 0;
        DelayC = 0;
    }
}