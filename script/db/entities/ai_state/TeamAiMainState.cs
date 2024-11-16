namespace db;

/// <summary>
/// COM思考に関するステータス
/// 一括で入っているんので、あとで細分化する
/// </summary>
public class TeamAiMainState : ITeamAiMainStateGetter, ITeamAiMainStateSetter
{
    public bool ResseiF { get; set; } // 劣勢
    public bool ResseiF2 { get; set; } // じつは劣勢
    public int FomRev { get; set; } // フォーメーション前後変動
    public bool QuickF { get; set; } // クイック行動
    public bool CCaScF { get; set; } // カウンターキャッチ成功

    public bool HelpF { get; set; } // ヘルプガード7

    public int DmWait { get; set; } // COMDM通常ウエイト
    public int DmFmWait { get; set; } // COMDM態勢ウエイト
    public int DmStWait { get; set; } // COMDMセッターウエイト
    public int DmDelay { get; set; } // COMDMディレイ
    public int DmPaItv { get; set; } // COMDMパスインターバル
    public int ChMakeLoop { get; set; } // COMチャンスメイクパスループ

    public int BallTakePer { get; set; } // COMボール拾い行動確率
    public bool CatchF { get; set; } // COMキャッチ行動確率

    public bool DsKeepF { get; set; } // ３メン、２メンでパス元もダッシュを継続する
    public bool DsOneItvWaitF { get; set; } // シングルの時は１インターバル待つ

    //サイコロ振った時点で初期化
    public int ComatcStF { get; set; } // ＣＯＭ動作開始（ボール持った人が一旦フリーに鳴るのを待つ）

    public int DmwaitC { get; set; } // ダッシュマン通常ウエイト
    public int DmfmwaitC { get; set; } // ダッシュマン態勢ウエイト
    public int DmstwaitC { get; set; } // ダッシュマンセッターウエイト
    public int DmclwaitC { get; set; } // ダッシュマンコールウエイト
    public int DmallC { get; set; } // ダッシュマン経過カウンタ

    public int DmstwaitStF { get; set; } // ダッシュマンセッターウエイト開始

    public bool DashmanF { get; set; } // ダッシュマン作戦
    public bool DashmanFmWaitF { get; set; } // ダッシュマン作戦態勢ウエイト中
    public bool DashmanShOkF { get; set; } // ダッシュマン作戦シュートＯＫでた
    public bool DashStCtrlF { get; set; } // 操作キャラのダッシュマン予約

    public int DmcallNum { get; set; } // ダッシュマン発動人数
    public int DmcalledNum { get; set; } // ダッシュマン発動した人数
    public int SetterNo { get; set; } // セッターNo

    public bool DmwaitOkF { get; set; } // ダッシュマンウエイトＯＫ
    public bool CallOkF { get; set; } // コールＯＫ
    public bool CallNgF { get; set; } // コールＮＧ
    public bool SetterOkF { get; set; } // セッターに渡った
    public bool SetterBmokWaitF { get; set; } // セッターのボールマンダッシュOKでウエイト
    public bool SetterBmRsvF { get; set; } // セッターのボールマンダッシュ
    public bool SetterBmRsvGoF { get; set; } // セッターのボールマンダッシュＯＫ

    public int DmstopF { get; set; } // ダッシュマン作戦停止のタイミング
    public int TmwaitC { get; set; } // チーム行動ウエイトカウンタ
    public bool SgodpassF { get; set; } // シングルパスおよびオーダーパス
    public int ShcgpassC { get; set; } // パスカウンタ
    public int ChpassC { get; set; } // チャンスメイクパスカウンタ
    public int ComResetF { get; set; } // COM作戦リセットタイミング
    public int ShAfterC { get; set; } // シュートしてからカウンタ(守備用)

    public bool ChanceF { get; set; } // チャンス行動フラグ
    public bool VnChanceF { get; set; } // 至近距離チャンス
    public bool NgChanceF { get; set; } // ノーガードチャンス
    public int VnTagNo { get; set; } // 至近距離タゲ番号
    public int NgTagNo { get; set; } // ノーガードタゲ番号

    //ダッシュマン作戦中
    public bool Isdm => DashmanF || DashmanFmWaitF;

    
    public void Initialize()
    {
        ResseiF = false;
        ResseiF2 = false;
        FomRev = 0;
        QuickF = false;
        CCaScF = false;
        HelpF = false;
        DmWait = 0;
        DmFmWait = 0;
        DmStWait = 0;
        DmDelay = 0;
        DmPaItv = 0;
        ChMakeLoop = 0;
        BallTakePer = 0;
        CatchF = false;
        DsKeepF = false;
        DsOneItvWaitF = false;
        ComatcStF = 0;
        DmwaitC = 0;
        DmfmwaitC = 0;
        DmstwaitC = 0;
        DmclwaitC = 0;
        DmallC = 0;
        DmstwaitStF = 0;
        DashmanF = false;
        DashmanFmWaitF = false;
        DashmanShOkF = false;
        DashStCtrlF = false;
        DmcallNum = 0;
        DmcalledNum = 0;
        SetterNo = 0;
        DmwaitOkF = false;
        CallOkF = false;
        CallNgF = false;
        SetterOkF = false;
        SetterBmokWaitF = false;
        SetterBmRsvF = false;
        SetterBmRsvGoF = false;
        DmstopF = 0;
        TmwaitC = 0;
        SgodpassF = false;
        ShcgpassC = 0;
        ChpassC = 0;
        ComResetF = 0;
        ShAfterC = 0;
        ChanceF = false;
        VnChanceF = false;
        NgChanceF = false;
        VnTagNo = 0;
        NgTagNo = 0;
    }
}

public interface ITeamAiMainStateGetter
{
    bool ResseiF { get; }
    bool ResseiF2 { get; }
    int FomRev { get; }
    bool QuickF { get; }
    bool CCaScF { get; }
    bool HelpF { get; }
    int DmWait { get; }
    int DmFmWait { get; }
    int DmStWait { get; }
    int DmDelay { get; }
    int DmPaItv { get; }
    int ChMakeLoop { get; }
    int BallTakePer { get; }
    bool CatchF { get; }
    bool DsKeepF { get; }
    bool DsOneItvWaitF { get; }
    int ComatcStF { get; }
    int DmwaitC { get; }
    int DmfmwaitC { get; }
    int DmstwaitC { get; }
    int DmclwaitC { get; }
    int DmallC { get; }
    int DmstwaitStF { get; }
    bool DashmanF { get; }
    bool DashmanFmWaitF { get; }
    bool DashmanShOkF { get; }
    bool DashStCtrlF { get; }
    int DmcallNum { get; }
    int DmcalledNum { get; }
    OrderIndexType SetterNo { get; }
    bool DmwaitOkF { get; }
    bool CallOkF { get; }
    bool CallNgF { get; }
    bool SetterOkF { get; }
    bool SetterBmokWaitF { get; }
    bool SetterBmRsvF { get; }
    bool SetterBmRsvGoF { get; }
    int DmstopF { get; }
    int TmwaitC { get; }
    bool SgodpassF { get; }
    int ShcgpassC { get; }
    int ChpassC { get; }
    int ComResetF { get; }
    int ShAfterC { get; }
    bool ChanceF { get; }
    bool VnChanceF { get; }
    bool NgChanceF { get; }
    int VnTagNo { get; }
    int NgTagNo { get; }
    bool Isdm { get; }
}

public interface ITeamAiMainStateSetter
{
    bool ResseiF { set; }
    bool ResseiF2 { set; }
    int FomRev { set; }
    bool QuickF { set; }
    bool CCaScF { set; }
    bool HelpF { set; }
    int DmWait { set; }
    int DmFmWait { set; }
    int DmStWait { set; }
    int DmDelay { set; }
    int DmPaItv { set; }
    int ChMakeLoop { set; }
    int BallTakePer { set; }
    bool CatchF { set; }
    bool DsKeepF { set; }
    bool DsOneItvWaitF { set; }
    int ComatcStF { set; }
    int DmwaitC { set; }
    int DmfmwaitC { set; }
    int DmstwaitC { set; }
    int DmclwaitC { set; }
    int DmallC { set; }
    int DmstwaitStF { set; }
    bool DashmanF { set; }
    bool DashmanFmWaitF { set; }
    bool DashmanShOkF { set; }
    bool DashStCtrlF { set; }
    int DmcallNum { set; }
    int DmcalledNum { set; }
    int SetterNo { set; }
    bool DmwaitOkF { set; }
    bool CallOkF { set; }
    bool CallNgF { set; }
    bool SetterOkF { set; }
    bool SetterBmokWaitF { set; }
    bool SetterBmRsvF { set; }
    bool SetterBmRsvGoF { set; }
    int DmstopF { set; }
    int TmwaitC { set; }
    bool SgodpassF { set; }
    int ShcgpassC { set; }
    int ChpassC { set; }
    int ComResetF { set; }
    int ShAfterC { set; }
    bool ChanceF { set; }
    bool VnChanceF { set; }
    bool NgChanceF { set; }
    int VnTagNo { set; }
    int NgTagNo { set; }

    void Initialize();
}
