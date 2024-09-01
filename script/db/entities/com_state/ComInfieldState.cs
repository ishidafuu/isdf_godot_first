using System;
using db.masters;

namespace db;

/// <summary>
/// COM思考内野に関するステータス
/// </summary>
public class ComInfieldState
{
    //避けタイプ
    enDodgeType DgType { get; set; }

    //避け開始時間
    int DgTime { get; set; }

    public void Initialize()
    {
        DgType = enDodgeType.dgBest;
        DgTime = 0;
    }
}