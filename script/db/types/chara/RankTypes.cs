namespace db;

//ランクデータ///////////////////////////
public enum RankLevelType //レベル
{
    BallSpd = 0, //球速
    ShStMotion, //振りかぶりフレ
    ShEdMotion, //投げ終わりフレ
    PaSpd, //パス速度
    PaBwSpd, //パスバウンド速度
    GrpSpin, //グラフィック回転速度
    LandRefAg, //着地時反射角度
    BwSpCourt, //バウンド速度コート
    BwSpWall, //バウンド速度壁
    DodgeTime, //よけ開始時間範囲
    BallSpdInc, //必殺シュート速度＋
    BallSpdDec, //必殺シュート速度－
}

public enum RankSpeedType //すばやさ
{
    DashAcc, //ダッシュ加速度
    DashMAXAcc, //ダッシュ限界加速量
    WkX, //歩き速度Ｘ
    LDSJPX, //前押しダッシュジャンプ飛距離dX
    WkJPX, //歩Jp飛距離dX
    PaWaitPos, //パス待ち位置
    IkiItv, //息継ぎ間隔
}

public enum RankHpType //のこりＨＰ
{
    HPtoRank, //のこりＨＰ→ランク
    RevFrm, //起き上がりまでの時間
    RollNum, //ごろごろ回数
    Breath, //息切れ回数
    BreathRev, //息切れコート戻り時間
}

public enum RankTechType //技術比
{
    TechtoRank, //技量比→ランク
    CatchTime, //キャッチ時間
    NiceCatchTime, //ナイスキャッチ時間
    SemiCatchTime, //キャッチ時間
    SemiNiceCatchTime, //ナイスキャッチ時間
    RefSp, //反射速度
    RefAg, //反射角度
}

public enum RankPowType //パワー比
{
    PowtoRank, //パワー比→ランク
    SlipSp, //スリップ量
    SlipEff, //スリップ量
    FrSkip, //ふっとびフレームスキップ
    FryNlSpd, //普通シュート吹っ飛び速度　Lv16
    FryNlAgl, //普通シュート吹っ飛び角度　Lv16
    FrySpSpd, //必殺シュート汎用吹っ飛び速度　Lv16
    FrySpAgl, //必殺シュート汎用吹っ飛び角度　Lv16
    FryLsYSp, //緩い吹っ飛び速度　Lv08
    FryLsAgl, //緩い吹っ飛び角度　Lv08
    FryLsXSp, //緩い吹っ飛Ｘ速度　Lv08
    FryLsGrv, //緩い吹っ飛重力　Lv08
    FryBrYSp, //脳天吹っ飛び速度　Lv08
    FryBrAgl, //脳天吹っ飛び角度　Lv08
    FryBrXSp, //脳天吹っ飛Ｘ速度　Lv08
    FryVrYSp, //真上吹っ飛び速度　Lv08
    FryVrAgl, //真上吹っ飛び角度　Lv08
    FryVrXSp, //真上吹っ飛Ｘ速度　Lv08
    FryHrYSp, //真横吹っ飛び速度　Lv08
    FryHrAgl, //真横吹っ飛び角度　Lv08
    FryHrXSp, //真横吹っ飛Ｘ速度　Lv08
    FryRvYSp, //逆吹っ飛び速度　Lv08
    FryRvAgl, //逆吹っ飛び角度　Lv08
    FryRvXSp, //逆吹っ飛Ｘ速度　Lv08
    FryGrYSp, //周回吹っ飛び速度　Lv08
    FryGrAgl, //周回吹っ飛び角度　Lv08
    FryGrXSp, //周回吹っ飛Ｘ速度　Lv08
    FryGrGrv, //周回吹っ飛重力　Lv08
    FryDnYSp, //その場ダウン速度　Lv08
    FryDnAgl, //その場ダウン角度　Lv08
    FryDnXSp, //その場ダウンＸ速度　Lv08
    FryVRYSp, //縦跳吹っ飛び速度　Lv08
    FryVRAgl, //縦跳吹っ飛び角度　Lv08
    FryVRXSp, //縦跳吹っ飛Ｘ速度　Lv08
    FryVRGrv, //縦跳吹っ飛重力　Lv08
    FryHRYSp, //横跳吹っ飛び速度　Lv08
    FryHRAgl, //横跳吹っ飛び角度　Lv08
    FryHRXSp, //横跳吹っ飛Ｘ速度　Lv08
    FryHRGrv, //横跳吹っ飛重力　Lv08
    FryEx1YSp, //特１吹っ飛び速度
    FryEx1Agl, //特１吹っ飛び角度
    FryEx1XSp, //特１吹っ飛Ｘ速度
    FryEx2YSp, //特２吹っ飛び速度
    FryEx2Agl, //特２吹っ飛び角度
    FryEx2XSp, //特２吹っ飛Ｘ速度
    FryEx3YSp, //特３吹っ飛び速度
    FryEx3Agl, //特３吹っ飛び角度
    FryEx3XSp, //特３吹っ飛Ｘ速度
    FryEx4YSp, //特４吹っ飛び速度
    FryEx4Agl, //特４吹っ飛び角度
    FryEx4XSp, //特４吹っ飛Ｘ速度
    FryEx5YSp, //特５吹っ飛び速度
    FryEx5Agl, //特５吹っ飛び角度
    FryEx5XSp, //特５吹っ飛Ｘ速度
    FryEx6YSp, //特６吹っ飛び速度
    FryEx6Agl, //特６吹っ飛び角度
    FryEx6XSp, //特６吹っ飛Ｘ速度
    FryEx7YSp, //特７吹っ飛び速度
    FryEx7Agl, //特７吹っ飛び角度
    FryEx7XSp, //特７吹っ飛Ｘ速度
    FryEx8YSp, //特８吹っ飛び速度
    FryEx8Agl, //特８吹っ飛び角度
    FryEx8XSp, //特８吹っ飛Ｘ速度
}