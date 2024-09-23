namespace db;

/// <summary>
/// ダッシュマンに関するステータス
/// ダッシュマン・ダッシュ状態でなくなったらリセット
/// </summary>
public class CharaDashmanState
{
    // ダッシュマン(操作ダッシュマンもあるので、ここでは呼ばれたダッシュマンフラグ)
    public bool IsCalledDashman { get; set; }

    // ダッシュマン番号
    public int DashmanNo { get; set; }

    // ダッシュマン向かうＺ軸
    public int TargetZ { get; set; }

    // ダッシュマンにパスを出しても大丈夫な時間
    public DownCounter EnabledPassCount { get; set; }

    public void Initialize()
    {
        IsCalledDashman = false;
        DashmanNo = 0;
        TargetZ = 0;
        EnabledPassCount.Clear();
    }

    public void DecrementEnabledPassCount()
    {
        EnabledPassCount.Sub();
    }
}