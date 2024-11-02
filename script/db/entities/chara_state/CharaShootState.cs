﻿namespace db;

/// <summary>
/// シュート・ターゲットに関するステータス
/// </summary>
public class CharaShootStateGetter : ICharaShootStateGetter, ICharaShootStateSetter
{
    // シュート待ちカウンタ
    public DownCounter ShootWaitCount { get; set; }
    public int ShootWaitCountValue => ShootWaitCount.Value;

    // シュート終了待ちカウンタ
    public DownCounter ShootEndWaitCount { get; set; }
    public int ShootEndWaitCountValue => ShootEndWaitCount.Value;

    // 必殺タイミング
    public UpCounter Step { get; set; }
    public int StepValue => Step.Value;

    // 必殺アシスト状態
    public ShootTiming ShootTiming { get; set; }

    // ターゲッティング角度（１２分割、一番左側の番号、そこから４つ）
    public int Angle12 { get; set; }

    // がたつき防止
    public int ShCv { get; set; }

    // ４番外野後ろ狙う
    public bool IsBackSh { get; set; }

    // 振り返りシュート
    public bool IsUTurn { get; set; }

    // ファンブルした後オートシュートしないためのカウンタ
    public int FumbleCount { get; set; }

    public void Initialize()
    {
        ShootWaitCount.Clear();
        ShootEndWaitCount.Clear();
        Step.Clear();
        ShootTiming = default;
        Angle12 = -1;
        ShCv = 0;
        IsBackSh = false;
        IsUTurn = false;
        FumbleCount = 0;
    }

    public void SetWaitCount(int startCount, int endCount)
    {
        ShootWaitCount.Set(startCount);
        ShootEndWaitCount.Set(endCount);
    }
}

public interface ICharaShootStateGetter
{
    int ShootWaitCountValue { get; }
    int ShootEndWaitCountValue { get; }
    int StepValue { get; }
    ShootTiming ShootTiming { get; }
    int Angle12 { get; }
    int ShCv { get; }
    bool IsBackSh { get; }
    bool IsUTurn { get; }
    int FumbleCount { get; }
}

public interface ICharaShootStateSetter
{
    DownCounter ShootWaitCount { get; }
    DownCounter ShootEndWaitCount { get; }
    UpCounter Step { get; }
    ShootTiming ShootTiming { set; }
    int Angle12 { set; }
    int ShCv { set; }
    bool IsBackSh { set; }
    bool IsUTurn { set; }
    int FumbleCount { set; }

    void Initialize();
    void SetWaitCount(int startCount, int endCount);
}