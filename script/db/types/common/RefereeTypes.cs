namespace db;

//レフェリーステータス************************************
public enum ShiaiPhase
{
    Ready,
    Shiai,
    GameEnd,
    Result,
    Gohome,
};

public enum RefereeMotion
{
    Stand,
    JumpBallWait,
    OvLine,
    JumpBall,
    LongWhistle,
    ShortWhistle,
    IOChange,
    PrevReset,
    TutoReset,
};