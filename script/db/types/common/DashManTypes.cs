namespace db;

/// <summary>
/// ダッシュマンタイプ
/// </summary>
public enum DashmanType
{
    None,
    Straight, //ストレート
    Up, //上ライン
    Down, //下ライン
    Approach, //アプローチ
}

/// <summary>
/// コールタイプ
/// </summary>
public enum CallType
{
    None,
    Pass,
    Dash,
}

/// <summary>
/// ターゲット
/// </summary>
public enum DashmanTargetType
{
    NG,
    OK,
    WAIT
}