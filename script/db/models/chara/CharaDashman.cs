namespace db;

/// <summary>
/// ダッシュマンに関するステータス
/// </summary>
public class CharaDashman
{
    // ダッシュマン番号
    public int DashmanNo { get; set; }

    // ダッシュマン向かうＺ軸
    public int TargetZ { get; set; }

    // ダッシュマンにパスを出しても大丈夫な時間
    public int EnabledPassCount { get; set; }

    public void Initialize()
    {
        DashmanNo = 0;
        TargetZ = 0;
        EnabledPassCount = 0;
    }
}