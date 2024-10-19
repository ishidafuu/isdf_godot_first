namespace db;

/// <summary>
/// ポジショニングに関するステータス
/// </summary>
public class TeamPositionState
{
    ///敵の重心
    public int EnCX { get; private set; }
    public int EnCX3 { get; private set; }
    public int EnCZ { get; private set; }
    public int EnC_c { get; private set; }
    //内野位置取り
    public enInfLineType InfLine { get; private set; }
    public enFormationType FType { get; private set; }
    //ダッシュマン人数
    public int DashmanNum { get; private set; }
    //オート動作ダッシュマン人数
    public int DashmanNum_Auto { get; private set; }
    //走ってるダッシュマン人数
    public int DashmanNum_Run { get; private set; }
    //ボールに触ってないダッシュマン人数
    public int DashmanNum_Run_NT { get; private set; }
    //パス可能人数＝ダッシュマン待機人数
    public int PassAbleNum { get; private set; }
    //現在のポストマン（居ないときはNGNUM）
    public OrderIndexType Postman { get; private set; }
    //ダッシュマン待機
    public int[] DashmanStock { get; private set; } = new int[Defines.InfieldCount];
    //ダッシュマンNG理由
    public int[] dmng { get; private set; } = new int[Defines.InfieldCount];

    public void Initialize()
    {
    }
}