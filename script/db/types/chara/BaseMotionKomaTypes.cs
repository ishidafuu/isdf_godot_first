namespace db;

/// <summary>
/// ループ設定
/// </summary>
public enum enBMLoopSt
{
    None, //無関係
    St, //開始位置
    Ed, //終了位置
    StEd, //同コマループ
};

/// <summary>
/// BMグラフィック
/// </summary>
public enum enBMCommonDt
{
    bmpNo, //絵番号
    posX, //Ｘ位置
    posY, //Ｙ位置
    se, //SE
    loopNum, //ループ回数
    DefFrm, //デフォフレーム数
    loopSt, //ループ設定
    seloop_f, //ループ時SEも鳴らし直す
    ActP_f, //アクションポイント
};