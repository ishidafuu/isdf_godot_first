namespace db;

/// <summary>
/// 重力
/// </summary>
public enum SettingGravityType
{
    GRV_Y, //重力Y軸重力
    FlyGrv, //重力（ふっとび）
    FlyGrvLoose, //重力（ゆるふっとび）
    PaGrv, //16,パス重力
    PaGrv14, //12,１４間パス重力
    BoundGrv, //重力（バウンド）
}

/// <summary>
/// ダメージ
/// </summary>
public enum SettingDamageType
{
    HP, //HP
    DeadNum, //昇天数x10
    CatchDamPer, //キャッチダメージ率
}

/// <summary>
/// ジャンプ
/// </summary>
public enum SettingJumpType
{
    Jump_vY0, //JpY初速（通常）
    DashJump_vY0, //JpY初速（ダッシュ）
    JumpCanselTime, //ジャンプキャンセル時間(f)
    JumpLandTime, //ジャンプ着地時間(f)
}

/// <summary>
/// コート
/// </summary>
public enum SetingCourtType
{
    DashX, //ダッシュ速度Ｘ
    DashZ, //ダッシュ速度Ｚ
    DashJpX, //ダッシュジャンプ速度Ｘ
    WkZPer, //奥行き移動速度％
    NWkXPer, //斜めＸ移動速度
    NWkZPer, //斜めＺ移動速度
    DsStopFric, //摩擦係数
    DownBrkIce, //氷床ダウンブレーキ係数
    DsLossPer, //足取られ
    RollSpeed, //転がり速度
}

/// <summary>
/// ボール
/// </summary>
public enum SettingBallType
{
    BoundDefPer, //バウンド
    BoundAbsdX, //バウンド速度減衰率Ｘ(%)
    BoundAbsdY, //バウンド速度減衰量Ｙ(%)
    BoundAbsdZ, //バウンド速度減衰率Ｚ(%)
    BoundStopdY, //これ以下の落下速度の場合バウンドしない
    BoundMaxdY, //バウンド最高Ｙ軸速度
    BoundMindYpdX, //バウンド最低dY/dX(%)これ以上鋭角のバウンドをしない
    AtariWidht, //当たり判定Ｗ
    AtariHeight, //当たり判定Ｈ
    AtariDepth, //当たり判定Ｄ
    ZAtariWidht, //Ｚ方向シュート当たり判定Ｗ
    ZAtariHeight, //Ｚ方向シュート当たり判定Ｈ
    ZAtariDepth, //Ｚ方向シュート当たり判定Ｄ
    FAtariWidht2, //停止ボール当たり判定Ｗ
    FAtariHeight2, //停止ボール当たり判定Ｈ
    FAtariDepth2, //停止ボール当たり判定Ｄ
    BaXWallAbsdX, //左右壁減速率X
    BaXWallAbsdZ, //左右壁減速率X
    BaZWallAbsdX, //左右壁減速率X
    BaZWallAbsdZ, //左右壁減速率X
}

/// <summary>
/// 壁
/// </summary>
public enum SettingWallType
{
    XWallAbsdX, //左右壁減速率X
    XWallAbsdZ, //左右壁減速率X
    ZWallAbsdX, //左右壁減速率X
    ZWallAbsdZ, //左右壁減速率X
    WallBounddY, //壁ぶつかり時跳ね上がり
}

/// <summary>
/// 内野
/// </summary>
public enum SettingInfieldType
{
    CrYokeJudgeWidth, //しゃがみ避け判断判定Ｘ幅
    CrYokeJudgeDepth, //しゃがみ避け判断判定Ｚ幅
    JpYokeJudgeWidth, //ジャンプ避け判断判定Ｘ幅
    JpYokeJudgeDepth, //ジャンプ避け判断判定Ｚ幅
    ShCvJpJudgeWidth, //シュートカバージャンプ判断判定Ｘ幅
    ShCvJpJudgeDepth, //シュートカバージャンプ判断判定Ｚ幅
    EnCourtCrTime, //敵コート避け時間限界(f)
    NoJpHeight, //避けない高さ
}

/// <summary>
/// キャッチ
/// </summary>
public enum SettingCatchType
{
    CatchWait, //総キャッチ時間
    CaMotionFrm, //キャッチ時間
    NiceCatchTime, //ナイスキャッチ時間
    DsCatchRev, //ダッシュキャッチのキャッチ時間マイナス
}

/// <summary>
/// シュート
/// </summary>
public enum SettingShotType
{
    RollPow, //ごろごろ閾値
    HissatsuShotSpd, //必殺シュートデフォ速度
    NormalShotSpd, //普通シュートデフォ速度
    PierceDown, //貫通威力減衰
    ComboDamDownPer, //同一キャラヒット時の威力減衰
    StandShotRate, //立ち投げＢＰ→攻撃値倍率
    JumpShotRate, //ジャンプＢＰ→攻撃値倍率
    DashShotRate, //ダッシュＢＰ→攻撃値倍率
    DashJumpShotRate, //ダッシュジャンプＢＰ→攻撃値倍率
    HitStartTime, //シュートしてから当たり判定が出るまでの時間(f)
    FlyTagHeight, //吹っ飛びでもタゲになる高さ
}

/// <summary>
/// パス
/// </summary>
public enum SettingPassType
{
    PaSpdAdd14, //50,１４間パス速度追加
    DMPaSpd, //ダッシュマンパス速度
    NearDistDef, //近距離判定距離
    QuickPassTagY, //クイックパスタゲ高さ
    TossPassTagY, //トスパスタゲ高さ
    DsmnTossCatchTime, //ダッシュマントスキャッチタイム
    AlleyoopTime, //アリウープ最低時間
    PaHitStartTime, //パスしてから当たり判定が出るまでの時間(f)
    PaStWait, //パス開始までのフレーム
}

/// <summary>
/// フォーメーション
/// </summary>
public enum SettingFormationType
{
    AtcLineX, //弱攻撃ライン（ダッシュシュートリリース位置、ダッシュマンがジャンプする位置など）
    AtcLineX2, //中攻撃ライン
    AtcLineX3, //強攻撃ライン
}

/// <summary>
/// COM思考
/// </summary>
public enum SettingComType
{
    OpeRev, //作戦設定による補正
    NaiyaSteel, //強奪範囲
}

/// <summary>
/// 入力
/// </summary>
public enum SettingInputType
{
    BackDashX, //バックダッシュになる位置
    AutoJumpX, //オーバーライン防止ジャンプ位置
    SemiCaLen, //セミオートキャッチ長さ
    SemiCaBase, //セミオートキャッチ中点
    SemiCaNice, //セミオートキャッチナイス
}