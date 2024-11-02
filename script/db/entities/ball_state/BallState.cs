using System;

namespace db;

public partial class BallState
{
    public bool Invs { get; set; }
    public bool LandLine { get; set; }
    public bool LandSet { get; set; }
    public bool Land { get; set; }
    public bool LowBound { get; set; }
    public bool MvCtrl { get; set; }
    public bool OvLine { get; set; }
    public bool PaAlPa { get; set; }
    public bool PaJPa { get; set; }
    public bool ShootFirstBound { get; set; }
    public bool Start { get; set; }
    public bool Counter { get; set; }
    public bool ShotMotion { get; set; }
    public Rect Atari2 { get; set; }
    public Rect Atari { get; set; }
    public Rect AtariFr { get; set; }
    public Rect ComCaAtari { get; set; }
    public CoordinateState Coordinate { get; set; }
    public BallMotionType MotionType { get; set; }
    public JumpBallType JumpBallType { get; set; }
    public DirectionZType PaNMuki { get; set; }
    public PassLineType PaLineType { get; set; }
    public int AniNo { get; set; }
    public int Atari2X { get; set; }
    public int Atari2Y { get; set; }
    public int Atari2Z { get; set; }
    public int AtariD2 { get; set; }
    public int AtariD { get; set; }
    public int AtariDFr { get; set; }
    public int AtariZ2 { get; set; }
    public int AtariZ { get; set; }
    public int AtariZFr { get; set; }
    public int Bound { get; set; }
    public int CameraPosX2 { get; set; }
    public int CameraPosX { get; set; }
    public int CameraPosdX { get; set; }
    public int ComCaAtariZ { get; set; }
    public int CounterPow { get; set; }
    public int CrvX { get; set; }
    public int CrvY { get; set; }
    public int CrvZ { get; set; }
    public int EnemTNo { get; set; }
    public int HitPNo { get; set; }
    public int HitTNo { get; set; }
    public OrderIndexType HolderOrder { get; set; }
    public int HolderSide { get; set; }
    public int LandX { get; set; }
    public int LandXLine { get; set; }
    public int LandZ { get; set; }
    public int LandZLine { get; set; }
    public int NGGetPNo { get; set; }
    public int NGGetTNo { get; set; }
    public bool IsNGGet { get; set; }
    public int NoHit { get; set; }
    public int PaCtPNo { get; set; }
    public int PaCtTNo { get; set; }
    public int PaETA { get; set; }
    public int PaGrv { get; set; }
    public int PaSelfHit { get; set; }
    public OrderIndexType PassTargetOrder { get; set; }
    public int PassTargetSide { get; set; }
    public OrderIndexType ThrowerOrderNo { get; set; }
    public int ThrowerSideNo { get; set; }
    public int RbwTNo { get; set; }
    public OrderIndexType ShotTargetOrder { get; set; }

    public int ShotTargetSide { get; set; }

    // TIMER周りはreferee
    public int TimerAutoReturn { get; set; }
    public int TimerCrtPos { get; set; }
    public int TimerCrtSide { get; set; }
    public int TimerSide { get; set; }
    public int Timer { get; set; }
    public int debugFlyRankLv { get; set; }
    public int[] BufSpin { get; set; } = new int[Defines.BufferPositionMax];
    public int[] BufX { get; set; } = new int[Defines.BufferPositionMax];
    public int[] BufY { get; set; } = new int[Defines.BufferPositionMax];
    public int[] BufZ { get; set; } = new int[Defines.BufferPositionMax];

    public void Initialize()
    {
        Invs = false;
        LandLine = false;
        LandSet = false;
        Land = false;
        LowBound = false;
        MvCtrl = false;
        OvLine = false;
        PaAlPa = false;
        PaJPa = false;
        ShootFirstBound = false;
        Start = false;
        Counter = false;
        ShotMotion = false;
        Atari2 = new Rect();
        Atari = new Rect();
        AtariFr = new Rect();
        ComCaAtari = new Rect();
        Coordinate = new CoordinateState();
        MotionType = default;
        JumpBallType = default;
        PaNMuki = default;
        PaLineType = default;
        AniNo = 0;
        Atari2X = 0;
        Atari2Y = 0;
        Atari2Z = 0;
        AtariD2 = 0;
        AtariD = 0;
        AtariDFr = 0;
        AtariZ2 = 0;
        AtariZ = 0;
        AtariZFr = 0;
        Bound = 0;
        CameraPosX2 = 0;
        CameraPosX = 0;
        CameraPosdX = 0;
        ComCaAtariZ = 0;
        CounterPow = 0;
        CrvX = 0;
        CrvY = 0;
        CrvZ = 0;
        EnemTNo = 0;
        HitPNo = 0;
        HitTNo = 0;
        HolderOrder = 0;
        HolderSide = 0;
        LandX = 0;
        LandXLine = 0;
        LandZ = 0;
        LandZLine = 0;
        NGGetPNo = 0;
        NGGetTNo = 0;
        IsNGGet = false;
        NoHit = 0;
        PaCtPNo = 0;
        PaCtTNo = 0;
        PaETA = 0;
        PaGrv = 0;
        PaSelfHit = 0;
        PassTargetOrder = 0;
        PassTargetSide = 0;
        ThrowerOrderNo = 0;
        ThrowerSideNo = 0;
        RbwTNo = 0;
        ShotTargetOrder = 0;
        ShotTargetSide = 0;
        TimerAutoReturn = 0;
        TimerCrtPos = 0;
        TimerCrtSide = 0;
        TimerSide = 0;
        Timer = 0;
        debugFlyRankLv = 0;
        Array.Clear(BufSpin, 0, BufSpin.Length);
        Array.Clear(BufX, 0, BufX.Length);
        Array.Clear(BufY, 0, BufY.Length);
        Array.Clear(BufZ, 0, BufZ.Length);
    }

}

public interface IBallStateGetter
{
    bool Invs { get; }
    bool LandLine { get; }
    bool LandSet { get; }
    bool Land { get; }
    bool LowBound { get; }
    bool MvCtrl { get; }
    bool OvLine { get; }
    bool PaAlPa { get; }
    bool PaJPa { get; }
    bool ShootFirstBound { get; }
    bool Start { get; }
    bool Counter { get; }
    bool ShotMotion { get; }
    Rect Atari2 { get; }
    Rect Atari { get; }
    Rect AtariFr { get; }
    Rect ComCaAtari { get; }
    CoordinateState Coordinate { get; }
    BallMotionType MotionType { get; }
    JumpBallType JumpBallType { get; }
    DirectionZType PaNMuki { get; }
    PassLineType PaLineType { get; }
    int AniNo { get; }
    int Atari2X { get; }
    int Atari2Y { get; }
    int Atari2Z { get; }
    int AtariD2 { get; }
    int AtariD { get; }
    int AtariDFr { get; }
    int AtariZ2 { get; }
    int AtariZ { get; }
    int AtariZFr { get; }
    int Bound { get; }
    int CameraPosX2 { get; }
    int CameraPosX { get; }
    int CameraPosdX { get; }
    int ComCaAtariZ { get; }
    int CounterPow { get; }
    int CrvX { get; }
    int CrvY { get; }
    int CrvZ { get; }
    int EnemTNo { get; }
    int HitPNo { get; }
    int HitTNo { get; }
    OrderIndexType HolderOrder { get; }
    int HolderSide { get; }
    int LandX { get; }
    int LandXLine { get; }
    int LandZ { get; }
    int LandZLine { get; }
    int NGGetPNo { get; }
    int NGGetTNo { get; }
    bool IsNGGet { get; }
    int NoHit { get; }
    int PaCtPNo { get; }
    int PaCtTNo { get; }
    int PaETA { get; }
    int PaGrv { get; }
    int PaSelfHit { get; }
    OrderIndexType PassTargetOrder { get; }
    int PassTargetSide { get; }
    OrderIndexType ThrowerOrderNo { get; }
    int ThrowerSideNo { get; }
    int RbwTNo { get; }
    OrderIndexType ShotTargetOrder { get; }
    int ShotTargetSide { get; }
    int TimerAutoReturn { get; }
    int TimerCrtPos { get; }
    int TimerCrtSide { get; }
    int TimerSide { get; }
    int Timer { get; }
    int debugFlyRankLv { get; }
    int[] BufSpin { get; }
    int[] BufX { get; }
    int[] BufY { get; }
    int[] BufZ { get; }
}

public interface IBallStateSetter
{
    bool Invs { set; }
    bool LandLine { set; }
    bool LandSet { set; }
    bool Land { set; }
    bool LowBound { set; }
    bool MvCtrl { set; }
    bool OvLine { set; }
    bool PaAlPa { set; }
    bool PaJPa { set; }
    bool ShootFirstBound { set; }
    bool Start { set; }
    bool Counter { set; }
    bool ShotMotion { set; }
    Rect Atari2 { set; }
    Rect Atari { set; }
    Rect AtariFr { set; }
    Rect ComCaAtari { set; }
    CoordinateState Coordinate { set; }
    BallMotionType MotionType { set; }
    JumpBallType JumpBallType { set; }
    DirectionZType PaNMuki { set; }
    PassLineType PaLineType { set; }
    int AniNo { set; }
    int Atari2X { set; }
    int Atari2Y { set; }
    int Atari2Z { set; }
    int AtariD2 { set; }
    int AtariD { set; }
    int AtariDFr { set; }
    int AtariZ2 { set; }
    int AtariZ { set; }
    int AtariZFr { set; }
    int Bound { set; }
    int CameraPosX2 { set; }
    int CameraPosX { set; }
    int CameraPosdX { set; }
    int ComCaAtariZ { set; }
    int CounterPow { set; }
    int CrvX { set; }
    int CrvY { set; }
    int CrvZ { set; }
    int EnemTNo { set; }
    int HitPNo { set; }
    int HitTNo { set; }
    OrderIndexType HolderOrder { set; }
    int HolderSide { set; }
    int LandX { set; }
    int LandXLine { set; }
    int LandZ { set; }
    int LandZLine { set; }
    int NGGetPNo { set; }
    int NGGetTNo { set; }
    bool IsNGGet { set; }
    int NoHit { set; }
    int PaCtPNo { set; }
    int PaCtTNo { set; }
    int PaETA { set; }
    int PaGrv { set; }
    int PaSelfHit { set; }
    OrderIndexType PassTargetOrder { set; }
    int PassTargetSide { set; }
    OrderIndexType ThrowerOrderNo { set; }
    int ThrowerSideNo { set; }
    int RbwTNo { set; }
    OrderIndexType ShotTargetOrder { set; }
    int ShotTargetSide { set; }
    int TimerAutoReturn { set; }
    int TimerCrtPos { set; }
    int TimerCrtSide { set; }
    int TimerSide { set; }
    int Timer { set; }
    int debugFlyRankLv { set; }
    int[] BufSpin { set; }
    int[] BufX { set; }
    int[] BufY { set; }
    int[] BufZ { set; }
    void Initialize();
}