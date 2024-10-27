namespace db;

/// <summary>
/// ボールから受け取る吹っ飛び周りに関するステータス
/// </summary>
public class CharaBallEffectState
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
    public DownCounter hitMTime_cd { get; set; } //食らったあとの無敵時間カウント…減算
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

    public void Initialize()
    {
        Stick_f = false;
        Sticking_f = false;
        StickingPr_f = false;
        Sticked_f = false;
        StickTime_c = 0;
        suction_f = false;
        suction_c = false;
        suctionLv = 0;
        suctionFlySpdBase = 0;
        suctionFlySpdRatio = 0;
        suctionLockX_f = false;
        suctionLockDistX = 0;
        suctionLockZ_f = false;
        suctionLockDistZ = 0;
        rndWorld_f = false;
        rndWorldLv = 0;
        flying_c = 0;
        flyLoose_f = false;
        noMvDwn_f = false;
        noMvDwnLv = false;
        refVer_f = false;
        refVerLv = 0;
        refHor_f = false;
        refHorLv = 0;
        combo_f = false;
        combo_c = 0;
        comboMax = 0;
        comboNoHitTime_c = 0;
        flySpGrav_f = false;
        hitMTime_cd.Clear();
        hitMltDam = 0;
        noDmg_f = false;
        hitSound = 0;
        rndWorldCh_f = false;
        refHorCh_f = false;
        symCtrl_f = false;
        seChiu = 0;
        ctMotionPast = default;
        stk1StNow_f = false;
        stk1StPst_f = false;
        stk2StNow_f = false;
        stk2StPst_f = false;
        FlySkip = 0;
        FlySkip_c = 0;
        ECDdg_f = false;
        ECDjp_f = false;
        VJump_f = false;
        LJump_f = false;
    }
    
    public void DecrementHitMTimeCount()
    {
        hitMTime_cd.Sub();
    }
}