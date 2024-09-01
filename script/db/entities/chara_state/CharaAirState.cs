namespace db;

/// <summary>
/// 空中・ジャンプ・着地に関するステータス
/// </summary>
public class CharaAirState
{
    // ジャンプしゃがみカウンタ
    public int JumpCrouchCount { get; set; }

    // 滞空時間
    public Counter AirCount { get; set; }

    // シュートタイミング用滞空時間
    public Counter ShootAirCount { get; set; }

    // 頂点の時間
    public int TopTiming { get; set; }

    // 空中キャッチ
    public bool IsAirCatch { get; set; }

    // 空中アクションした（キャッチしたらまたオフ）
    public bool IsAirAction { get; set; }

    // 空中復帰した（キャッチしたらまたオフ）
    public bool IsAirRev { get; set; }

    // 世界一周します
    public bool IsRound { get; set; }

    // 着地予想地点
    public int LandX { get; set; }

    public int LandZ { get; set; }

    // 着地予想までの時間
    public int LandRest { get; set; }

    // 現在の重力
    public int NowGravity { get; set; }

    // 着地計算済み
    public bool IsLandSet { get; set; }

    public void Initialize()
    {
        JumpCrouchCount = 0;
        AirCount.Clear();
        ShootAirCount.Clear();
        TopTiming = 0;
        IsAirCatch = false;
        IsAirAction = false;
        IsAirRev = false;
        IsRound = false;
        LandX = 0;
        LandZ = 0;
        LandRest = 0;
        NowGravity = 0;
        IsLandSet = false;
    }

    public void Progress(bool isProgressShootAirCount)
    {
        AirCount.Add();
        if (isProgressShootAirCount)
        {
            ShootAirCount.Add();
        }
    }
}