namespace db;

public class RefereeState : IRefereeStateGetter, IRefereeStateSetter
{

    // //BOOL TutoEnd_f;
    // BOOL Demo_f;
    // BOOL DisCnct_f;
    // BOOL GameSet_f;
    // BOOL JglSE_f;
    // BOOL NoSE_f;
    // BOOL Option_f;
    // BOOL PauseStepNext_f;
    // BOOL PauseStep_f;
    // BOOL Pause_f;
    // BOOL Prev_f;//シュートプレビューモード
    // BOOL Suddun_f;
    // BOOL TimeUp_f;
    // BOOL Tour_f;//疑似対人
    // BOOL TutoGameAct_f;//実戦開始
    // BOOL TutoSend_f;//チュートアクション発生（レフェリー開始時初期化）
    // BOOL actbtn_f;//キャッチ、ドッジボタンが押された（ＯＫかＮＧが送られるまで保存）
    // BOOL prevreturn_f;//チュートアクション発生（レフェリー開始時初期化）
    // enMukiType Muki;
    // enRfMotion Motion;
    // enRfShiaiPhase DBSSt;
    // enSceneName NextScene;
    // s32 Ani_c;
    // s32 DrawLimitTime;
    // s32 JglTime;
    // s32 Jgl_c;
    // s32 LimitTime;
    // s32 M_c;//一応今のところアニメーションカウンタ的な物も兼ねる
    // s32 PauseCsrPos;
    // s32 Slow_c;
    // s32 TutoNo;//現状のチュートアクション番号（とりあえず値はそのまま）
    // s32 TutoRcv;//受信したチュートアクション番号
    // s32 TutoSend;//チュートアクション番号(レフェリー開始時初期化、ここに数値を入れておいてpGame処理後拾うようにする)
    // s32 Whistle_c;//ホイッスル
    // s32 WinSide;
    // s32 Win_c;
    // s32 act_c;
    // u32 PlayTime_c;//成績用試合時間
    //
    // BOOL isTimeAttackMode_;	// タイムアタックモード

    public bool TutoEnd_f { get; set; }
    public bool Demo_f { get; set; }
    public bool DisCnct_f { get; set; }
    public bool IsGameSet { get; set; }
    public bool JglSE_f { get; set; }
    public bool NoSE_f { get; set; }
    public bool Option_f { get; set; }
    public bool PauseStepNext_f { get; set; }
    public bool PauseStep_f { get; set; }
    public bool Pause_f { get; set; }
    public bool Prev_f { get; set; }
    public bool Suddun_f { get; set; }
    public bool TimeUp_f { get; set; }
    public bool Tour_f { get; set; }
    public bool TutoGameAct_f { get; set; }
    public bool TutoSend_f { get; set; }
    public bool actbtn_f { get; set; }
    public bool prevreturn_f { get; set; }
    public DirectionXType Muki { get; set; }
    public RefereeMotion Motion { get; set; }

    public ShiaiPhase ShiaiPahse { get; set; }

    // public enSceneName NextScene { get; set; }
    public int Ani_c { get; set; }
    public int DrawLimitTime { get; set; }
    public int JglTime { get; set; }
    public int Jgl_c { get; set; }
    public int LimitTime { get; set; }
    public int M_c { get; set; }
    public int PauseCsrPos { get; set; }
    public int Slow_c { get; set; }
    public int TutoNo { get; set; }
    public int TutoRcv { get; set; }
    public int TutoSend { get; set; }
    public int Whistle_c { get; set; }
    public int WinSide { get; set; }
    public int Win_c { get; set; }
    public int act_c { get; set; }
    public uint PlayTime_c { get; set; }
    public bool isTimeAttackMode_ { get; set; }

    public void Initialize()
    {
        TutoEnd_f = false;
        Demo_f = false;
        DisCnct_f = false;
        IsGameSet = false;
        JglSE_f = false;
        NoSE_f = false;
        Option_f = false;
        PauseStepNext_f = false;
        PauseStep_f = false;
        Pause_f = false;
        Prev_f = false;
        Suddun_f = false;
        TimeUp_f = false;
        Tour_f = false;
        TutoGameAct_f = false;
        TutoSend_f = false;
        actbtn_f = false;
        prevreturn_f = false;
        Muki = default;
        Motion = default;
        ShiaiPahse = default;
        Ani_c = 0;
        DrawLimitTime = 0;
        JglTime = 0;
        Jgl_c = 0;
        LimitTime = 0;
        M_c = 0;
        PauseCsrPos = 0;
        Slow_c = 0;
        TutoNo = 0;
        TutoRcv = 0;
        TutoSend = 0;
        Whistle_c = 0;
        WinSide = 0;
        Win_c = 0;
        act_c = 0;
        PlayTime_c = 0;
        isTimeAttackMode_ = false;
    }

    public bool IsShiai()
    {
        return ShiaiPahse == ShiaiPhase.Shiai;
    }
}

public interface IRefereeStateGetter
{
    bool TutoEnd_f { get; }
    bool Demo_f { get; }
    bool DisCnct_f { get; }
    bool IsGameSet { get; }
    bool JglSE_f { get; }
    bool NoSE_f { get; }
    bool Option_f { get; }
    bool PauseStepNext_f { get; }
    bool PauseStep_f { get; }
    bool Pause_f { get; }
    bool Prev_f { get; }
    bool Suddun_f { get; }
    bool TimeUp_f { get; }
    bool Tour_f { get; }
    bool TutoGameAct_f { get; }
    bool TutoSend_f { get; }
    bool actbtn_f { get; }
    bool prevreturn_f { get; }
    DirectionXType Muki { get; }
    RefereeMotion Motion { get; }
    ShiaiPhase ShiaiPahse { get; }
    int Ani_c { get; }
    int DrawLimitTime { get; }
    int JglTime { get; }
    int Jgl_c { get; }
    int LimitTime { get; }
    int M_c { get; }
    int PauseCsrPos { get; }
    int Slow_c { get; }
    int TutoNo { get; }
    int TutoRcv { get; }
    int TutoSend { get; }
    int Whistle_c { get; }
    int WinSide { get; }
    int Win_c { get; }
    int act_c { get; }
    uint PlayTime_c { get; }
    bool isTimeAttackMode_ { get; }
    bool IsShiai();
}

public interface IRefereeStateSetter
{
    bool TutoEnd_f { set; }
    bool Demo_f { set; }
    bool DisCnct_f { set; }
    bool IsGameSet { set; }
    bool JglSE_f { set; }
    bool NoSE_f { set; }
    bool Option_f { set; }
    bool PauseStepNext_f { set; }
    bool PauseStep_f { set; }
    bool Pause_f { set; }
    bool Prev_f { set; }
    bool Suddun_f { set; }
    bool TimeUp_f { set; }
    bool Tour_f { set; }
    bool TutoGameAct_f { set; }
    bool TutoSend_f { set; }
    bool actbtn_f { set; }
    bool prevreturn_f { set; }
    DirectionXType Muki { set; }
    RefereeMotion Motion { set; }
    ShiaiPhase ShiaiPahse { set; }
    int Ani_c { set; }
    int DrawLimitTime { set; }
    int JglTime { set; }
    int Jgl_c { set; }
    int LimitTime { set; }
    int M_c { set; }
    int PauseCsrPos { set; }
    int Slow_c { set; }
    int TutoNo { set; }
    int TutoRcv { set; }
    int TutoSend { set; }
    int Whistle_c { set; }
    int WinSide { set; }
    int Win_c { set; }
    int act_c { set; }
    uint PlayTime_c { set; }
    bool isTimeAttackMode_ { set; }
    void Initialize();
}