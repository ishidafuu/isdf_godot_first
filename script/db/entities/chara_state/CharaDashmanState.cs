namespace db;

/// <summary>
/// ダッシュマンに関するステータス
/// ダッシュマン・ダッシュ状態でなくなったらリセット
/// </summary>
public class CharaDashmanState
{
    // ダッシュマン
    public bool IsDashman { get; set; }

    // ダッシュマン番号
    public int DashmanNo { get; set; }

    // ダッシュマン向かうＺ軸
    public int TargetZ { get; set; }

    // ダッシュマンにパスを出しても大丈夫な時間
    public Counter EnabledPassCount { get; set; }

    public void Initialize()
    {
        IsDashman = false;
        DashmanNo = 0;
        TargetZ = 0;
        EnabledPassCount.Clear();
    }

    public void DecrementEnabledPassCount()
    {
        EnabledPassCount.Sub();
    }
}