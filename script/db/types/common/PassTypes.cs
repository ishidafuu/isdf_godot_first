namespace db;

public enum PassTargetType
{
    Out2, //２番外野
    Out3, //３番外野
    Out4, //４番外野
    UFDsmn, //上部前方ダッシュマン
    URDsmn, //上部後方ダッシュマン
    DFDsmn, //下部前方ダッシュマン
    DRDsmn, //下部後方ダッシュマン
    UDsmn, //上部ダッシュマン
    DDsmn, //下部ダッシュマン
    FDsmn, //最前列ダッシュマン
    RDsmn, //最後尾ダッシュマン
    NDsmn, //最寄りダッシュマン
}

public enum PassLineType
{
    plNml,
    pl23,
    pl14,
    pl11,
    plKyo
}