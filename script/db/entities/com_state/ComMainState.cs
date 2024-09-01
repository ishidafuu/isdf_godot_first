namespace db;

/// <summary>
/// COM思考に関するステータス
/// 一括で入っているんので、あとで細分化する
/// </summary>
public class ComMainState
{
    bool ResseiF { get; set; } // 劣勢
    bool ResseiF2 { get; set; } // じつは劣勢
    int FomRev { get; set; } // フォーメーション前後変動
    bool QuickF { get; set; } // クイック行動
    bool CCaScF { get; set; } // カウンターキャッチ成功

    bool HelpF { get; set; } // ヘルプガード7

    int DmWait { get; set; } // COMDM通常ウエイト
    int DmFmWait { get; set; } // COMDM態勢ウエイト
    int DmStWait { get; set; } // COMDMセッターウエイト
    int DmDelay { get; set; } // COMDMディレイ
    int DmPaItv { get; set; } // COMDMパスインターバル
    int ChMakeLoop { get; set; } // COMチャンスメイクパスループ

    int BallTakePer { get; set; } // COMボール拾い行動確率
    bool CatchF { get; set; } // COMキャッチ行動確率

    bool DsKeepF { get; set; } // ３メン、２メンでパス元もダッシュを継続する
    bool DsOneItvWaitF { get; set; } // シングルの時は１インターバル待つ

    //サイコロ振った時点で初期化
    int ComatcStF { get; set; } // ＣＯＭ動作開始（ボール持った人が一旦フリーに鳴るのを待つ）

    int DmwaitC { get; set; } // ダッシュマン通常ウエイト
    int DmfmwaitC { get; set; } // ダッシュマン態勢ウエイト
    int DmstwaitC { get; set; } // ダッシュマンセッターウエイト
    int DmclwaitC { get; set; } // ダッシュマンコールウエイト
    int DmallC { get; set; } // ダッシュマン経過カウンタ

    int DmstwaitStF { get; set; } // ダッシュマンセッターウエイト開始

    bool DashmanF { get; set; } // ダッシュマン作戦
    bool DashmanFmWaitF { get; set; } // ダッシュマン作戦態勢ウエイト中
    bool DashmanShOkF { get; set; } // ダッシュマン作戦シュートＯＫでた
    bool DashStCtrlF { get; set; } // 操作キャラのダッシュマン予約

    int DmcallNum { get; set; } // ダッシュマン発動人数
    int DmcalledNum { get; set; } // ダッシュマン発動した人数
    int SetterNo { get; set; } // セッターNo

    bool DmwaitOkF { get; set; } // ダッシュマンウエイトＯＫ
    bool CallOkF { get; set; } // コールＯＫ
    bool CallNgF { get; set; } // コールＮＧ
    bool SetterOkF { get; set; } // セッターに渡った
    bool SetterBmokWaitF { get; set; } // セッターのボールマンダッシュOKでウエイト
    bool SetterBmRsvF { get; set; } // セッターのボールマンダッシュ
    bool SetterBmRsvGoF { get; set; } // セッターのボールマンダッシュＯＫ

    int DmstopF { get; set; } // ダッシュマン作戦停止のタイミング
    int TmwaitC { get; set; } // チーム行動ウエイトカウンタ
    bool SgodpassF { get; set; } // シングルパスおよびオーダーパス
    int ShcgpassC { get; set; } // パスカウンタ
    int ChpassC { get; set; } // チャンスメイクパスカウンタ
    int ComResetF { get; set; } // COM作戦リセットタイミング
    int ShAfterC { get; set; } // シュートしてからカウンタ(守備用)

    bool ChanceF { get; set; } // チャンス行動フラグ
    bool VnChanceF { get; set; } // 至近距離チャンス
    bool NgChanceF { get; set; } // ノーガードチャンス
    int VnTagNo { get; set; } // 至近距離タゲ番号
    int NgTagNo { get; set; } // ノーガードタゲ番号

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

    public bool Isdm() //ダッシュマン作戦中
    {
        return DashmanF || DashmanFmWaitF;
    }
}