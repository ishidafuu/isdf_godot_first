using System;
using db.masters;

namespace db;

/// <summary>
/// チームに関するステータス
/// </summary>
public class TeamMainState
{
    public ResultType RType { get; set; } // 試合結果
    public bool ComOverTimeF { get; set; } // ＣＯＭ思考強制シュート
    public bool RsvResetComF { get; set; } // ＣＯＭ思考リセット★
    public bool CallingDmResetF { get; set; } // コールによるＣＯＭ思考リセット★
    public bool CallingRdCgF { get; set; } // コールによるリーダチェンジ＆ＣＯＭ思考リセット★
    public int CtrlNo { get; set; } // 操作中のキャラポジション
    public int DmCallmanNo { get; set; } // ダッシュマンを呼んだ人（センターダッシュ用）
    public int CtrlRsvNo { get; set; } // 操作権予約キャラ
    public int CvrNo { get; set; } // パス先に操作権が移ったときカバーにはいるキャラ
    public int Cvr2No { get; set; } // カバーマンのカバーマン
    public int GetNo { get; set; } // ボール拾いマン
    public int NoInputC { get; set; } // 操作権自動切り替え用
    public bool InputCancelF { get; set; } // 操作権変わったときなど同フレームの入力を反映しない

    public int DrwHsNo { get; set; } // 表示必殺シュート番号
    public int DrwHsC { get; set; } // 表示必殺シュートカウンタ　これらはチームで管理した方がよさそう
    public int DrwPosX { get; set; }

    public int BallCrtSideL { get; set; }
    public int BallCrtPosL { get; set; }
    public int BallCrtSideZ { get; set; }
    public int BallCrtPosZ { get; set; }

    public int DashmanCallNo { get; set; } // ★ダッシュマン予約
    public bool DashmanAllF { get; set; } // 全員出発
    public int DashmanNoBm { get; set; } // ボールマンからのダッシュマン予約
    DashmanType DashmanType { get; set; } // ★ダッシュマン予約
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
    public int CallingNo;
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
    }
}