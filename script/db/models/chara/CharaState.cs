namespace db;

public class CharaState
{
    public CoordinateState Coordinate { get; set; }
    public CharaMotionState Motion { get; set; }
    public CharaAnimeState Anime { get; set; }
    public CharaAutoState Auto { get; set; }
    public CharaAutoState NextAuto { get; set; }
    public CharaComAutoState ComAuto { get; set; }
    public CharaPadState Pad { get; set; }
    public CharaGouseiState Gousei { get; set; }

}

/// <summary>
/// 生存・体力・判定に関するステータス
/// </summary>
public class CharaLive
{
    // キャラのＩＤ
    public long CharaId { get; set; }

    // 生存
    public bool IsAlive { get; set; }

    // 死亡 天使の間 GenSaiObjのLive_f=Falseは完全消滅
    public bool IsDead { get; set; }

    // 外野天使
    public bool IsAngel { get; set; }

    // 喰らい判定
    public Rect HitBox { get; set; }

    // 体力
    public int Hp { get; set; }

    // 体力全快
    public bool IsMaxHp { get; set; }

    // 天使カウンタ
    public int AngelCount { get; set; }

    // 復活回数
    public int ReviveCount { get; set; }

    // 汎用カウンタ
    public int EtcCount { get; set; }

    // 何週目か
    public int LoopCount { get; set; }

    // バイオリズム
    public int BiorhythmCount { get; set; }
}

/// <summary>
/// ダメージに関するステータス
/// </summary>
public class CharaDamage
{
    // キャッチダメージ顔
    public int DamageAfterCount { get; set; }

    // ダウンカウンタ
    public int DownCount { get; set; }

    // 回転カウンタ
    public int RollCount { get; set; }

    // 屈みカウンタ
    public int KagamiCount { get; set; }

    // よりコート中心に戻れるカウンタ
    public int KagamiRevCount { get; set; }

    // 屈み戻り予定位置
    public int KagamiRevKgNo { get; set; }

    // ヒットストップ
    public bool IsHitStop { get; set; }
}

/// <summary>
/// 能力値に関するステータス
/// </summary>
public class CharaLevel
{
    // レベル（チームレア＋補正）
    public int Level { get; set; }

    // ぱわー
    public long Power { get; set; }

    // きあい
    public long Guts { get; set; }

    // きれ
    public long Kire { get; set; }

    // てく
    public long Tech { get; set; }
}

/// <summary>
/// 位置、距離に関するステータス
/// </summary>
public class CharaPosition
{
    // ボールとの距離
    public int BallDist { get; set; }

    // ボール着地地点との距離
    public int BallLandDist { get; set; }

    // ボール着地地点のラインとの距離
    public int BallLandLineDist { get; set; }
}

/// <summary>
/// 向き・移動・ダッシュ・汎用アクションに関するステータス
/// </summary>
public class CharaMove
{
    // ダッシュ加速カウンタ
    public int DashAccelCount { get; set; }

    // ダッシュ加速IOSフラグ
    public bool IsDashAccelIOS { get; set; }

    // 最後に向いた向き（２，３番外野パス用）
    public DirectionX LastKeyDirectionX { get; set; }

    // 動きなしフラグ
    public bool IsNoMove { get; set; }

    // オート向き変えが入る前の向き
    public DirectionX LastDirectionX { get; set; }

    // オート向き変えが入る前の奥行き向き
    public DirectionZ LastDirectionZ { get; set; }

    // フリーアクションフラグ
    public bool IsFreeAction { get; set; }
}

/// <summary>
/// 空中・ジャンプ・着地に関するステータス
/// </summary>
public class CharaAir
{
    // ジャンプしゃがみカウンタ
    public int JumpCrouchCount { get; set; }

    // 滞空時間
    public int AirCount { get; set; }

    // シュートタイミング用滞空時間
    public int ShootAirCount { get; set; }

    // 頂点の時間
    public int TopTiming { get; set; }

    // 空中キャッチ
    public bool IsAirCatch { get; set; }

    // 空中アクションした（キャッチしたらまたオフ）
    public bool IsAirAction { get; set; }

    // 空中復帰した（キャッチしたらまたオフ）
    public bool IsAirRev { get; set; }

    // 世界一周します
    public bool IsRound { get; set; }

    // 着地予想地点
    public int LandX { get; set; }

    public int LandZ { get; set; }

    // 着地予想までの時間
    public int LandRest { get; set; }

    // 現在の重力
    public int NowGravity { get; set; }
}

/// <summary>
/// アニメーション・見た目・セリフに関するステータス
/// </summary>
public class CharaView
{
    // 息継ぎカウンタ
    public int BreathCount { get; set; }

    // 長持ちセリフ用
    public int LongKeepCount { get; set; }

    // パス待ちセリフ用
    public int PassWaitCount { get; set; }
}

/// <summary>
/// キャッチングに関するステータス
/// </summary>
public class CharaCatch
{
    // ボールをキャッチできるカウンタ
    public int CatchCount { get; set; }

    // 次のキャッチまでのラグ
    public int CatchWaitCount { get; set; }

    // ブロックカウンタ
    public int BlockCount { get; set; }

    // キャッチ滑り時摩擦
    public int FricX { get; set; }

    public int FricZ { get; set; }

    // 敵シュートキャッチ
    public bool IsEnemyShotCatch { get; set; }

    // キャッチ後長押し維持でジャンプしてしまうのを防ぐためのフラグ
    public bool IsEnabledJump { get; set; }

    // キャッチ後リリースでシュートしてしまうのを防ぐためのフラグ
    public bool IsEnabledShot { get; set; }
}

/// <summary>
/// シュート・ターゲットに関するステータス
/// </summary>
public class CharaShoot
{
    // シュート待ちカウンタ
    public int ShootWaitCount { get; set; }

    // シュート終了待ちカウンタ
    public int ShootEndWaitCount { get; set; }

    // シュートターゲット
    public int ShootTarget { get; set; }

    // 必殺タイミング
    public int Step { get; set; }

    // 泥足場ステップ数
    public int MadStep { get; set; }

    // 必殺アシスト状態
    public ShootTiming ShootTiming { get; set; }

    // ターゲッティング角度（１２分割、一番左側の番号、そこから４つ）
    public int Angle12 { get; set; }

    // がたつき防止
    public int ShCv { get; set; }

    // ４番外野後ろ狙う
    public bool IsBackSh { get; set; }

    // 振り返りシュート
    public bool IsUTurn { get; set; }

    // ファンブルした後オートシュートしないためのカウンタ
    public int FumbleCount { get; set; }
}

/// <summary>
/// 避けに関するステータス
/// </summary>
public class CharaDodge
{
    // 敵コート避け時間
    public int EnemyCortDodgeCount { get; set; }
}

/// <summary>
/// ドッジボールのコートに関するステータス
/// </summary>
public class CharaCourt
{
    //外野でもオーバーラインになるフラグ
    public bool IsOutFieldOverLine { get; set; }

    //敵コートに着地予定
    public bool IsLandEnemyCourt { get; set; }
}

/// <summary>
/// パスに関するステータス
/// </summary>
public class CharaPass
{
    // 立ちパス待ちカウンタ
    public int PassStandWaitCount { get; set; }

    // トスパスフラグ
    public bool IsTossPass { get; set; }

    // ミラーパスカウンタ
    public int MirrorPassCount { get; set; }

    // ミラーパス的シュートカウンタ
    public int MirrorShotCount { get; set; }

    // ミラーパス的シュート受付時間
    public int MirrorShotLimitCount { get; set; }
}

/// <summary>
/// ダッシュマンに関するステータス
/// </summary>
public class CharaDashman
{
    // ダッシュマン番号
    public int DashmanNo { get; set; }

    // ダッシュマン向かうＺ軸
    public int TargetZ { get; set; }

    // ダッシュマンにパスを出しても大丈夫な時間
    public int EnabledPassCount { get; set; }
}

/// <summary>
/// Com操作に関するステータス
/// </summary>
public class CharaCom
{
    // 敵キャッチ確率
    public int ComCatchProbability { get; set; }

    // 敵ナイスキャッチ確率
    public int ComNiceCatchProbability { get; set; }

    // キャッチカウンター状態
    public bool IsCatchCounter { get; set; }

    // 内野からのトスパスを受けた状態
    public bool IsComTossPassGet { get; set; }

}

/// <summary>
/// ボールから受け取る吹っ飛び周りに関するステータス
/// </summary>
public class CharaBallEffect
{
    //ボールから受け取る吹っ飛び絡みのデータ
    public bool Stick_f { get; set; } //アッパーシュートくっつきフラグ
    public bool Sticking_f { get; set; } //くっつき"中"フラグ　くっつき中はヒット状態なので非ヒット状態になったらオフ
    public bool StickingPr_f { get; set; } //１Fr前くっつき"中"フラグ　くっつき中はヒット状態なので非ヒット状態になったらオフ
    public bool Sticked_f { get; set; } //くっつき"後"フラグ　ノーマル吹っ飛びに移行するけど無敵はダウンしてから
    public int StickTime_c; //くっつき時間カウント　０でリリース＆↑０でもリリースorくっつき非発動
    public bool suction_f; //12:吸引効果
    public bool suction_c; //12:吸引効果発生時からのカウント
    public int suctionLv; //12:吸引効果Lv
    public float suctionFlySpdBase { get; set; } //100%のときのXZ合計の速度･･･最初に読み込んでおく
    public float suctionFlySpdRatio { get; set; } //↑を０から倍々にしていく
    public bool suctionLockX_f { get; set; } //吸引効果でボールとキャラとの間が４以下になったらロックするためのフラグ
    public float suctionLockDistX { get; set; } //吸引効果ロック中のボール～キャラ間
    public bool suctionLockZ_f { get; set; } //吸引効果でボールとキャラとの間が４以下になったらロックするためのフラグ
    public float suctionLockDistZ { get; set; } //吸引効果ロック中のボール～キャラ間
    public bool rndWorld_f; //12:世界一周効果
    public int rndWorldLv; //12:世界一周効果Lv
    public int flying_c; //12:吹っ飛び中カウント
    public bool flyLoose_f { get; set; } //緩跳ね吹っ飛び
    public bool noMvDwn_f { get; set; } //その場吹っ飛び
    public bool noMvDwnLv { get; set; } //その場吹っ飛び
    public bool refVer_f { get; set; } //縦跳ね吹っ飛び
    public int refVerLv { get; set; } //縦跳ね吹っ飛びLv
    public bool refHor_f { get; set; } //横跳ね吹っ飛び
    public int refHorLv { get; set; } //横跳ね吹っ飛びLv
    public bool combo_f { get; set; } //コンボフラグ
    public int combo_c { get; set; } //コンボ回数　０でコンボしない（他キャラによる貫通力減少が起こるので貫通力と別で持つ）
    public int comboMax { get; set; } //コンボ回数　０でコンボしない（他キャラによる貫通力減少が起こるので貫通力と別で持つ）
    public int comboNoHitTime_c { get; set; } //コンボ中ヒットフラグ発生までの時間→０より大きい場合は無敵＆０でヒットOK
    public bool flySpGrav_f { get; set; } //特殊吹っ飛び重力処理フラグ
    public int hitMTime_cd { get; set; } //食らったあとの無敵時間カウント…減算
    public int hitMltDam { get; set; } //連続ヒット時のダメージ
    public bool noDmg_f { get; set; } //ダメナシフラグ
    public int hitSound;
    public bool rndWorldCh_f; //12:世界一周効果シュートのキャッチ
    public bool refHorCh_f { get; set; } //横跳ね吹っ飛び効果シュートのキャッチ
    public bool symCtrl_f { get; set; } //マリオネット効果フラグ
    public int seChiu { get; set; } //ちうちう用予約SE enSEID
    CharaMotionType ctMotionPast { get; set; } //ChCount用１フレ前のモーションタイプ

    public bool stk1StNow_f { get; set; } //現Fr時、判定１ジャッジでくっつき状態か･･･くっつき効果とは無関係
    public bool stk1StPst_f { get; set; } //１Fr前、判定１ジャッジでくっつき状態か･･･くっつき効果とは無関係
    public bool stk2StNow_f { get; set; } //現Fr時、判定２ジャッジでくっつき状態か･･･くっつき効果とは無関係
    public bool stk2StPst_f { get; set; } //１Fr前、判定２ジャッジでくっつき状態か･･･くっつき効果とは無関係
    public int FlySkip { get; set; } //吹っ飛びスキップ量
    public int FlySkip_c { get; set; } //吹っ飛びスキップ量
    public bool ECDdg_f { get; set; } //敵コート避け
    public bool ECDjp_f { get; set; } //敵コート避け
    public bool VJump_f { get; set; } //垂直ジャンプ
    public bool LJump_f { get; set; } //ロングジャンプ
}