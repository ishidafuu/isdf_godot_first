namespace db;

/// <summary>
/// ポジショニングに関するステータス
/// </summary>
public class TeamPositionState : ITeamPositionStateGetter, ITeamPositionStateSetter
{
    ///敵の重心
    public int EnCX { get; set; }

    public int EnCX3 { get; set; }
    public int EnCZ { get; set; }

    public int EnC_c { get; set; }

    //内野位置取り
    public enInfLineType InfLine { get; set; }

    public enFormationType FType { get; set; }

    //ダッシュマン人数
    public int DashmanNum { get; set; }

    //オート動作ダッシュマン人数
    public int DashmanNum_Auto { get; set; }

    //走ってるダッシュマン人数
    public int DashmanNum_Run { get; set; }

    //ボールに触ってないダッシュマン人数
    public int DashmanNum_Run_NT { get; set; }

    //パス可能人数＝ダッシュマン待機人数
    public int PassAbleCount { get; set; }

    //現在のポストマン（居ないときはNGNUM）
    public OrderIndexType Postman { get; set; }

    //ダッシュマン待機
    public int[] DashmanStock { get; } = new int[Defines.InfieldCount];

    //ダッシュマンNG理由
    public int[] dmng { get; } = new int[Defines.InfieldCount];

    public void Initialize()
    {
    }
}

public interface ITeamPositionStateGetter
{
    int EnCX { get; }
    int EnCX3 { get; }
    int EnCZ { get; }
    int EnC_c { get; }
    enInfLineType InfLine { get; }
    enFormationType FType { get; }
    int DashmanNum { get; }
    int DashmanNum_Auto { get; }
    int DashmanNum_Run { get; }
    int DashmanNum_Run_NT { get; }
    int PassAbleCount { get; }
    OrderIndexType Postman { get; }
    int[] DashmanStock { get; }
    int[] dmng { get; }
}

public interface ITeamPositionStateSetter
{
    int EnCX { set; }
    int EnCX3 { set; }
    int EnCZ { set; }
    int EnC_c { set; }
    enInfLineType InfLine { set; }
    enFormationType FType { set; }
    int DashmanNum { set; }
    int DashmanNum_Auto { set; }
    int DashmanNum_Run { set; }
    int DashmanNum_Run_NT { set; }
    int PassAbleCount { set; }
    OrderIndexType Postman { set; }
    void Initialize();
}