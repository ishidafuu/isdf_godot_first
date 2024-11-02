namespace db;

/// <summary>
/// COMアクション思考に関するステータス
/// </summary>
public class TeamComActionState : IComActionStateGetter, IComActionStateSetter
{
    //COMシングルオーダーパスする
    public bool SgOdPaF { get; set; }

    //COMシュート変更タイプ
    public enShChangeType ShCg { get; set; }

    //COMシングルパスウエイト
    public int PaWait { get; set; }

    //COMDSh歩数
    public int ShWait { get; set; }

    //COMJShタイミング
    public int DShStep { get; set; }

    //COMJShタイミング
    public int JShTime { get; set; }

    public int JShTime2 { get; set; }

    //CODs開始位置  
    public int DsPos { get; set; }

    //ボール触った
    public bool BallTouchedF { get; set; }

    //COM避けられないシュートはキャッチ
    public bool DgToCa_f { get; set; }

    public void Initialize()
    {
        SgOdPaF = false;
        ShCg = enShChangeType.scWait;
        PaWait = 0;
        ShWait = 0;
        DShStep = 0;
        JShTime = 0;
        JShTime2 = 0;
        DsPos = 0;
        BallTouchedF = false;
        DgToCa_f = false;
    }
}

public interface IComActionStateGetter
{
    bool SgOdPaF { get; }
    enShChangeType ShCg { get; }
    int PaWait { get; }
    int ShWait { get; }
    int DShStep { get; }
    int JShTime { get; }
    int JShTime2 { get; }
    int DsPos { get; }
    bool BallTouchedF { get; }
    bool DgToCa_f { get; }
}

public interface IComActionStateSetter
{
    bool SgOdPaF { set; }
    enShChangeType ShCg { set; }
    int PaWait { set; }
    int ShWait { set; }
    int DShStep { set; }
    int JShTime { set; }
    int JShTime2 { set; }
    int DsPos { set; }
    bool BallTouchedF { set; }
    bool DgToCa_f { set; }

    void Initialize();
}