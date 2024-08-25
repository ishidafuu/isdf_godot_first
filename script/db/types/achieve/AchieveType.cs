namespace db.types.Achieve;

public enum AchieveType
{
    //シュート・パス処理部分
    Shot_c, //シュート数合計
    JSPShot_c, //ＤＪ必殺シュート数
    DSPShot_c, //Ｄ必殺シュート数
    DNiceShot_c, //Ｄナイスシュート数
    JNiceShot_c, //Ｊナイスシュート数
    DJNiceShot_c, //ＤＪナイスシュート数
    GaiyaShot_c, //外野シュート数合計
    DashManShot_c, //ダッシュマンシュート
    PassTry_c, //パスした数
    Tag_c, //タゲになった回数
    EnShot_c, //シュート数合計

    //天使処理部分
    Kill_c, //アウトにした数
    Death_c, //アウトにされた数

    //ヒット処理部分
    TagDam_c, //ボールに当たった数
    NoTagDam_c, //ボールに当たった数
    Hit_c, //シュートを当てた数
    JSPHit_c, //必殺シュートを当てた数
    DSPHit_c, //必殺シュートを当てた数
    NiceHit_c, //ナイスシュートを当てた数
    MoreHit_c, //追い打ちシュートを当てた数
    NoTagHit_c, //ターゲット以外にあてた数
    GaiyaHit_c, //外野にあてた数

    //キャッチ処理部分
    PassOK_c, //パスした数
    PassCut_c, //パスカットした数
    CatchOK_c, //キャッチ成功した数
    CatchTry_c, //キャッチした数(守備機会数合計ヒット＋キャッチ)
    NiceCatch_c, //ナイスキャッチ成功

    //オーバーライン処理部分
    OverLine_c, //オーバーラインした数
    Rebound_c, //リバウンドキャッチ
    Maxdam,
    Totaldam,
    END,
}