namespace db;

public enum enInfLineType
{
    iltInf,
    iltO2,
    iltO3,
    iltO4,
    iltOfO
};

public enum enFormationType
{
    ftDfI,
    ftDf2,
    ftDf3,
    ftDf4,
    ftOfI,
    ftOfO
};

public enum enDodgeType
{
    dgSw,
    dgJp,
    dgCr,
    dgBest
};

public enum enShChangeType
{
    scWait,
    scNone,
    scPass,
    scCrv
};

public enum ResultType
{
    rtREADY = 0,
    rtPLAY = 1,
    rtWIN = 2,
    rtLOSE = 3,
    rtTIMEUPWIN = 4,
    rtTIMEUPLOSE = 5,
    rtTIMEUPDRAW = 6,
    rtRETIRE = 10,
};