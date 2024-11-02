namespace db;

/// <summary>
/// チームに関するステータス
/// </summary>
public class TeamMainState : ITeamMainStateGetter, ITeamMainStateSetter
{
    public ResultType RType { get; set; } // 試合結果
    public bool ComOverTimeF { get; set; } // ＣＯＭ思考強制シュート
    public bool RsvResetComF { get; set; } // ＣＯＭ思考リセット★
    public bool CallingDmResetF { get; set; } // コールによるＣＯＭ思考リセット★
    public bool CallingRdCgF { get; set; } // コールによるリーダチェンジ＆ＣＯＭ思考リセット★
    public OrderIndexType ControlOrderIndex { get; set; } // 操作中のキャラポジション(orderindex)
    public int DmCallmanNo { get; set; } // ダッシュマンを呼んだ人（センターダッシュ用）
    public int CtrlRsvNo { get; set; } // 操作権予約キャラ
    public int CvrNo { get; set; } // パス先に操作権が移ったときカバーにはいるキャラ
    public int Cvr2No { get; set; } // カバーマンのカバーマン
    public int GetNo { get; set; } // ボール拾いマン
    public int NoInputC { get; set; } // 操作権自動切り替え用
    public bool IsCancelActionInput { get; set; } // 操作権変わったときなど同フレームの入力を反映しない

    public int DrwHsNo { get; set; } // 表示必殺シュート番号
    public int DrwHsC { get; set; } // 表示必殺シュートカウンタ　これらはチームで管理した方がよさそう
    public int DrwPosX { get; set; }

    public int BallCrtSideL { get; set; }
    public int BallCrtPosL { get; set; }
    public int BallCrtSideZ { get; set; }
    public int BallCrtPosZ { get; set; }

    public int DashmanCallNo { get; set; } // ★ダッシュマン予約
    public bool DashmanAllF { get; set; } // 全員出発
    public OrderIndexType DashmanNoBm { get; set; } // ボールマンからのダッシュマン予約
    public DashmanType DashmanType { get; set; } // ★ダッシュマン予約
    public int DashmanCallC { get; set; } // 呼んだ人数

    public int IoChangeC { get; set; } // 内外野交代カウンタ
    public int UseMotoGaiyaC { get; set; } // 元外野
    public int DeadC { get; set; } // 倒された数

    public int TmUedaC { get; set; } // チーム上田カウンタ

    public int DbgSerifuPNo { get; set; }
    public int DbgSerifuTypeNo { get; set; }

    public bool OfsideF { get; set; } // オフェンスサイド
    public bool NosideF { get; set; } // サイドナシ
    public bool RandresetF { get; set; } // 行動確率リセット
    public bool GetJpballF { get; set; } // ジャンプボール

    public bool SideMaruhiF { get; set; } // 丸秘作戦開始
    public bool MaruhiCallF { get; set; }

    public bool ManSideF { get; set; } // サイド操作
    public bool ManSidePadNo { get; set; } // サイド操作
    public bool OmksSideF { get; set; } // おまかせ

    public int OmakaseC { get; set; } // チーム用おまかせ連打防止カウンタ

    public int EncrC { get; set; } // エンカレカウンタ
    public int TutoWaitC { get; set; } // エンカレカウンタ

    public bool CallingDashmanF { get; set; } // Calling開始
    public bool CallingDashmanF2 { get; set; } // Switchでオン
    public bool CallingDashmanF3 { get; set; } // 発動
    public int CallingNo { get; set; }
    public int RdNo { get; set; } // チームリーダー番号（posNoではなく、charNo）
    public int ComNo { get; set; } // リーダーに応じたＣＯＭ番号
    public int FomNo { get; set; }
    public int LastBallmanNo { get; set; }
    public bool NeedTimeRedrawF { get; set; }

    //スマート操作
    public bool SmartF { get; set; } // スマート

    public int DamC { get; set; } // ＨＰ表示用
    public int Dampos { get; set; } // ダメージキャラ

    public void Initialize()
    {
        RType = ResultType.rtREADY;
        ComOverTimeF = false;
        RsvResetComF = false;
        CallingDmResetF = false;
        CallingRdCgF = false;
        ControlOrderIndex = 0;
        DmCallmanNo = 0;
        CtrlRsvNo = 0;
        CvrNo = 0;
        Cvr2No = 0;
        GetNo = 0;
        NoInputC = 0;
        IsCancelActionInput = false;
        DrwHsNo = 0;
        DrwHsC = 0;
        DrwPosX = 0;
        BallCrtSideL = 0;
        BallCrtPosL = 0;
        BallCrtSideZ = 0;
        BallCrtPosZ = 0;
        DashmanCallNo = 0;
        DashmanAllF = false;
        DashmanNoBm = 0;
        DashmanType = DashmanType.None;
        DashmanCallC = 0;
        IoChangeC = 0;
        UseMotoGaiyaC = 0;
        DeadC = 0;
        TmUedaC = 0;
        DbgSerifuPNo = 0;
        DbgSerifuTypeNo = 0;
        OfsideF = false;
        NosideF = false;
        RandresetF = false;
        GetJpballF = false;
        SideMaruhiF = false;
        MaruhiCallF = false;
        ManSideF = false;
        ManSidePadNo = false;
        OmksSideF = false;
        OmakaseC = 0;
        EncrC = 0;
        TutoWaitC = 0;
        CallingDashmanF = false;
        CallingDashmanF2 = false;
        CallingDashmanF3 = false;
        CallingNo = 0;
        RdNo = 0;
        ComNo = 0;
        FomNo = 0;
        LastBallmanNo = 0;
        NeedTimeRedrawF = false;
        SmartF = false;
        DamC = 0;
        Dampos = 0;
    }
}

public interface ITeamMainStateGetter
{
    ResultType RType { get; }
    bool ComOverTimeF { get; }
    bool RsvResetComF { get; }
    bool CallingDmResetF { get; }
    bool CallingRdCgF { get; }
    OrderIndexType ControlOrderIndex { get; }
    int DmCallmanNo { get; }
    int CtrlRsvNo { get; }
    int CvrNo { get; }
    int Cvr2No { get; }
    int GetNo { get; }
    int NoInputC { get; }
    bool IsCancelActionInput { get; }
    int DrwHsNo { get; }
    int DrwHsC { get; }
    int DrwPosX { get; }
    int BallCrtSideL { get; }
    int BallCrtPosL { get; }
    int BallCrtSideZ { get; }
    int BallCrtPosZ { get; }
    int DashmanCallNo { get; }
    bool DashmanAllF { get; }
    OrderIndexType DashmanNoBm { get; }
    DashmanType DashmanType { get; }
    int DashmanCallC { get; }
    int IoChangeC { get; }
    int UseMotoGaiyaC { get; }
    int DeadC { get; }
    int TmUedaC { get; }
    int DbgSerifuPNo { get; }
    int DbgSerifuTypeNo { get; }
    bool OfsideF { get; }
    bool NosideF { get; }
    bool RandresetF { get; }
    bool GetJpballF { get; }
    bool SideMaruhiF { get; }
    bool MaruhiCallF { get; }
    bool ManSideF { get; }
    bool ManSidePadNo { get; }
    bool OmksSideF { get; }
    int OmakaseC { get; }
    int EncrC { get; }
    int TutoWaitC { get; }
    bool CallingDashmanF { get; }
    bool CallingDashmanF2 { get; }
    bool CallingDashmanF3 { get; }
    int CallingNo { get; }
    int RdNo { get; }
    int ComNo { get; }
    int FomNo { get; }
    int LastBallmanNo { get; }
    bool NeedTimeRedrawF { get; }
    bool SmartF { get; }
    int DamC { get; }
    int Dampos { get; }
}

public interface ITeamMainStateSetter
{
    ResultType RType { set; }
    bool ComOverTimeF { set; }
    bool RsvResetComF { set; }
    bool CallingDmResetF { set; }
    bool CallingRdCgF { set; }
    OrderIndexType ControlOrderIndex { set; }
    int DmCallmanNo { set; }
    int CtrlRsvNo { set; }
    int CvrNo { set; }
    int Cvr2No { set; }
    int GetNo { set; }
    int NoInputC { set; }
    bool IsCancelActionInput { set; }
    int DrwHsNo { set; }
    int DrwHsC { set; }
    int DrwPosX { set; }
    int BallCrtSideL { set; }
    int BallCrtPosL { set; }
    int BallCrtSideZ { set; }
    int BallCrtPosZ { set; }
    int DashmanCallNo { set; }
    bool DashmanAllF { set; }
    OrderIndexType DashmanNoBm { set; }
    DashmanType DashmanType { set; }
    int DashmanCallC { set; }
    int IoChangeC { set; }
    int UseMotoGaiyaC { set; }
    int DeadC { set; }
    int TmUedaC { set; }
    int DbgSerifuPNo { set; }
    int DbgSerifuTypeNo { set; }
    bool OfsideF { set; }
    bool NosideF { set; }
    bool RandresetF { set; }
    bool GetJpballF { set; }
    bool SideMaruhiF { set; }
    bool MaruhiCallF { set; }
    bool ManSideF { set; }
    bool ManSidePadNo { set; }
    bool OmksSideF { set; }
    int OmakaseC { set; }
    int EncrC { set; }
    int TutoWaitC { set; }
    bool CallingDashmanF { set; }
    bool CallingDashmanF2 { set; }
    bool CallingDashmanF3 { set; }
    int CallingNo { set; }
    int RdNo { set; }
    int ComNo { set; }
    int FomNo { set; }
    int LastBallmanNo { set; }
    bool NeedTimeRedrawF { set; }
    bool SmartF { set; }
    int DamC { set; }
    int Dampos { set; }
    void Initialize();
}