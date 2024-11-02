namespace db;

/// <summary>
/// セミオートに関するステータス
/// </summary>
public class TeamSemiAutoState : ITeamSemiAutoStateGetter, ITeamSemiAutoStateSetter
{
    //セミオート用追加変数
    public bool SemiF { get; set; } // セミオート
    public bool SemiOrderF { get; set; } // セミオートシュートオーダー
    public int SemiCatchC { get; set; } // キャッチアシストカウンター
    public int SemiCatchingC { get; set; } // キャッチアシストカウンター
    public bool SemiShotF { get; set; } // シュート命令
    public int SemiShotC { get; set; } // キャッチアシストカウンター
    public int SemiAi { get; set; }
    public int SemiCaPos { get; set; }
    public int SemiNCaLen { get; set; }

    public void Initialize()
    {
        SemiF = false;
        SemiOrderF = false;
        SemiCatchC = 0;
        SemiCatchingC = 0;
        SemiShotF = false;
        SemiShotC = 0;
        SemiAi = 0;
        SemiCaPos = 0;
        SemiNCaLen = 0;
    }
}

public interface ITeamSemiAutoStateGetter
{
    bool SemiF { get; }
    bool SemiOrderF { get; }
    int SemiCatchC { get; }
    int SemiCatchingC { get; }
    bool SemiShotF { get; }
    int SemiShotC { get; }
    int SemiAi { get; }
    int SemiCaPos { get; }
    int SemiNCaLen { get; }
}

public interface ITeamSemiAutoStateSetter
{
    bool SemiF { set; }
    bool SemiOrderF { set; }
    int SemiCatchC { set; }
    int SemiCatchingC { set; }
    bool SemiShotF { set; }
    int SemiShotC { set; }
    int SemiAi { set; }
    int SemiCaPos { set; }
    int SemiNCaLen { set; }
    void Initialize();
}