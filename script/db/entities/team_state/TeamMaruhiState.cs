namespace db;

/// <summary>
/// マル秘作成に関するステータス
/// </summary>
public class TeamMaruhiState : ITeamMaruhiStateGetter, ITeamMaruhiStateSetter
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

public interface ITeamMaruhiStateGetter
{
    int CallNum { get; }
    int CallDelay { get; }
    int DmType { get; }
    int CallC { get; }
    int DelayC { get; }
}

public interface ITeamMaruhiStateSetter
{
    int CallNum { set; }
    int CallDelay { set; }
    int DmType { set; }
    int CallC { set; }
    int DelayC { set; }
    void Initialize();
}