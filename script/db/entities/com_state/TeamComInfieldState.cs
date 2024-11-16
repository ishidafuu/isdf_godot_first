namespace db;

/// <summary>
/// COM思考内野に関するステータス
/// </summary>
public class TeamComInfieldState : ITeamComInfieldStateGetter, ITeamComInfieldStateSetter
{
    //避けタイプ
    public enDodgeType DgType { get; set; }

    //避け開始時間
    public int DgTime { get; set; }

    public void Initialize()
    {
        DgType = enDodgeType.dgBest;
        DgTime = 0;
    }
}

public interface ITeamComInfieldStateGetter
{
    enDodgeType DgType { get; }
    int DgTime { get; }
}

public interface ITeamComInfieldStateSetter
{
    enDodgeType DgType { set; }
    int DgTime { set; }

    void Initialize();
}