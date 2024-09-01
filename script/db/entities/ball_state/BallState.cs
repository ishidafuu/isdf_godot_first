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
    public int HolderOrder { get; set; }
    public int HolderSide { get; set; }
    public int LandX { get; set; }
    public int LandXLine { get; set; }
    public int LandZ { get; set; }
    public int LandZLine { get; set; }
    public int NGGetPNo { get; set; }
    public int NGGetTNo { get; set; }
    public int NGGet { get; set; }
    public int NoHit { get; set; }
    public int PaCtPNo { get; set; }
    public int PaCtTNo { get; set; }
    public int PaETA { get; set; }
    public int PaGrv { get; set; }
    public int PaSelfHit { get; set; }
    public int PassTargetOrder { get; set; }
    public int PassTargetSide { get; set; }
    public int PichPNo { get; set; }
    public int PichTNo { get; set; }
    public int RbwTNo { get; set; }
    public int ShotTargetOrder { get; set; }
    public int ShotTargetSide { get; set; }
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
        NGGet = 0;
        NoHit = 0;
        PaCtPNo = 0;
        PaCtTNo = 0;
        PaETA = 0;
        PaGrv = 0;
        PaSelfHit = 0;
        PassTargetOrder = 0;
        PassTargetSide = 0;
        PichPNo = 0;
        PichTNo = 0;
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