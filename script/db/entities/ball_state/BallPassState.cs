﻿namespace db;

public class BallPassState : IBallPassStateGetter, IBallPassStateSetter
{
    public int DistanceX { get; set; }
    public int DistanceZ { get; set; }
    public int DistanceY { get; set; }
    public int DistanceXSign { get; set; }
    public int DistanceZSign { get; set; }
    public int DistanceYSign { get; set; }
    public PassLineType PassLineType { get; set; }
    public int TargetVelocityX { get; set; }
    public int TargetVelocityZ { get; set; }
    public int TargetVelocityY { get; set; }

    public int TargetGravity { get; set; }

    // ダッシュマンZ移動してる時間
    public int TargetDashmanMoveZTime { get; set; }

    // ダッシュマンZ移動の行き先（目標ではなくジャンプラインまでに移動できる分）
    public int TargetDashmanEndZ { get; set; }

    public bool IsTargetDashman { get; set; }

    // 長押し：アリウープ　短押し：クイック
    public bool IsQuickPass { get; set; }

    // 間に合わない
    public bool IsLate { get; set; }

    // 偏差射撃を行わない
    public bool IsStopDashman { get; set; }

    // 到着予定時間
    public int ArriveTime { get; set; }
    public int Alleyoop { get; set; }
    public int PassGravity { get; set; }
    public int PassSpeed { get; set; }
    public int HoldNo { get; set; }
    public int PassArrivalCount { get; set; }

    public void Initialize()
    {
        DistanceX = 0;
        DistanceZ = 0;
        DistanceY = 0;
        DistanceXSign = 0;
        DistanceZSign = 0;
        DistanceYSign = 0;
        PassLineType = default;
        TargetVelocityX = 0;
        TargetVelocityZ = 0;
        TargetVelocityY = 0;
        TargetGravity = 0;
        TargetDashmanMoveZTime = 0;
        TargetDashmanEndZ = 0;
        IsTargetDashman = false;
        IsQuickPass = false;
        IsLate = false;
        IsStopDashman = false;
        ArriveTime = 0;
        Alleyoop = 0;
        PassGravity = 0;
        PassSpeed = 0;
        HoldNo = 0;
        PassArrivalCount = 0;
    }

}

public interface IBallPassStateGetter
{
    int DistanceX { get; }
    int DistanceZ { get; }
    int DistanceY { get; }
    int DistanceXSign { get; }
    int DistanceZSign { get; }
    int DistanceYSign { get; }
    PassLineType PassLineType { get; }
    int TargetVelocityX { get; }
    int TargetVelocityZ { get; }
    int TargetVelocityY { get; }
    int TargetGravity { get; }
    int TargetDashmanMoveZTime { get; }
    int TargetDashmanEndZ { get; }
    bool IsTargetDashman { get; }
    bool IsQuickPass { get; }
    bool IsLate { get; }
    bool IsStopDashman { get; }
    int ArriveTime { get; }
    int Alleyoop { get; }
    int PassGravity { get; }
    int PassSpeed { get; }
    int HoldNo { get; }
    int PassArrivalCount { get; }
}

public interface IBallPassStateSetter
{
    int DistanceX { set; }
    int DistanceZ { set; }
    int DistanceY { set; }
    int DistanceXSign { set; }
    int DistanceZSign { set; }
    int DistanceYSign { set; }
    PassLineType PassLineType { set; }
    int TargetVelocityX { set; }
    int TargetVelocityZ { set; }
    int TargetVelocityY { set; }
    int TargetGravity { set; }
    int TargetDashmanMoveZTime { set; }
    int TargetDashmanEndZ { set; }
    bool IsTargetDashman { set; }
    bool IsQuickPass { set; }
    bool IsLate { set; }
    bool IsStopDashman { set; }
    int ArriveTime { set; }
    int Alleyoop { set; }
    int PassGravity { set; }
    int PassSpeed { set; }
    int HoldNo { set; }
    int PassArrivalCount { set; }
    void Initialize();
}