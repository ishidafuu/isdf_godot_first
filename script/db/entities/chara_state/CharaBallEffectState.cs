namespace db;

/// <summary>
/// ボールから受け取る吹っ飛び周りに関するステータス
/// </summary>
public class CharaBallEffectState : ICharaBallEffectStateGetter, ICharaBallEffectStateSetter
{
    //ボールから受け取る吹っ飛び絡みのデータ
    public bool Stick_f { get; set; } //アッパーシュートくっつきフラグ
    public bool Sticking_f { get; set; } //くっつき"中"フラグ　くっつき中はヒット状態なので非ヒット状態になったらオフ
    public bool StickingPr_f { get; set; } //１Fr前くっつき"中"フラグ　くっつき中はヒット状態なので非ヒット状態になったらオフ
    public bool Sticked_f { get; set; } //くっつき"後"フラグ　ノーマル吹っ飛びに移行するけど無敵はダウンしてから
    public int StickTime_c { get; set; } //くっつき時間カウント　０でリリース＆↑０でもリリースorくっつき非発動
    public bool suction_f { get; set; } //12:吸引効果
    public bool suction_c { get; set; } //12:吸引効果発生時からのカウント
    public int suctionLv { get; set; } //12:吸引効果Lv
    public float suctionFlySpdBase { get; set; } //100%のときのXZ合計の速度･･･最初に読み込んでおく
    public float suctionFlySpdRatio { get; set; } //↑を０から倍々にしていく
    public bool suctionLockX_f { get; set; } //吸引効果でボールとキャラとの間が４以下になったらロックするためのフラグ
    public float suctionLockDistX { get; set; } //吸引効果ロック中のボール～キャラ間
    public bool suctionLockZ_f { get; set; } //吸引効果でボールとキャラとの間が４以下になったらロックするためのフラグ
    public float suctionLockDistZ { get; set; } //吸引効果ロック中のボール～キャラ間
    public bool rndWorld_f { get; set; } //12:世界一周効果
    public int rndWorldLv { get; set; } //12:世界一周効果Lv
    public int flying_c { get; set; } //12:吹っ飛び中カウント
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
    public int hitSound { get; set; }
    public bool rndWorldCh_f { get; set; } //12:世界一周効果シュートのキャッチ
    public bool refHorCh_f { get; set; } //横跳ね吹っ飛び効果シュートのキャッチ
    public bool symCtrl_f { get; set; } //マリオネット効果フラグ
    public int seChiu { get; set; } //ちうちう用予約SE enSEID
    public CharaMotionType ctMotionPast { get; set; } //ChCount用１フレ前のモーションタイプ

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
    }
}

public interface ICharaBallEffectStateGetter
{
    bool Stick_f { get; }
    bool Sticking_f { get; }
    bool StickingPr_f { get; }
    bool Sticked_f { get; }
    int StickTime_c { get; }
    bool suction_f { get; }
    bool suction_c { get; }
    int suctionLv { get; }
    float suctionFlySpdBase { get; }
    float suctionFlySpdRatio { get; }
    bool suctionLockX_f { get; }
    float suctionLockDistX { get; }
    bool suctionLockZ_f { get; }
    float suctionLockDistZ { get; }
    bool rndWorld_f { get; }
    int rndWorldLv { get; }
    int flying_c { get; }
    bool flyLoose_f { get; }
    bool noMvDwn_f { get; }
    bool noMvDwnLv { get; }
    bool refVer_f { get; }
    int refVerLv { get; }
    bool refHor_f { get; }
    int refHorLv { get; }
    bool combo_f { get; }
    int combo_c { get; }
    int comboMax { get; }
    int comboNoHitTime_c { get; }
    bool flySpGrav_f { get; }
    DownCounter hitMTime_cd { get; }
    int hitMltDam { get; }
    bool noDmg_f { get; }
    int hitSound { get; }
    bool rndWorldCh_f { get; }
    bool refHorCh_f { get; }
    bool symCtrl_f { get; }
    int seChiu { get; }
    CharaMotionType ctMotionPast { get; }
    bool stk1StNow_f { get; }
    bool stk1StPst_f { get; }
    bool stk2StNow_f { get; }
    bool stk2StPst_f { get; }
    int FlySkip { get; }
    int FlySkip_c { get; }
}

public interface ICharaBallEffectStateSetter
{
    bool Stick_f { set; }
    bool Sticking_f { set; }
    bool StickingPr_f { set; }
    bool Sticked_f { set; }
    int StickTime_c { set; }
    bool suction_f { set; }
    bool suction_c { set; }
    int suctionLv { set; }
    float suctionFlySpdBase { set; }
    float suctionFlySpdRatio { set; }
    bool suctionLockX_f { set; }
    float suctionLockDistX { set; }
    bool suctionLockZ_f { set; }
    float suctionLockDistZ { set; }
    bool rndWorld_f { set; }
    int rndWorldLv { set; }
    int flying_c { set; }
    bool flyLoose_f { set; }
    bool noMvDwn_f { set; }
    bool noMvDwnLv { set; }
    bool refVer_f { set; }
    int refVerLv { set; }
    bool refHor_f { set; }
    int refHorLv { set; }
    bool combo_f { set; }
    int combo_c { set; }
    int comboMax { set; }
    int comboNoHitTime_c { set; }
    bool flySpGrav_f { set; }
    DownCounter hitMTime_cd { get; }
    int hitMltDam { set; }
    bool noDmg_f { set; }
    int hitSound { set; }
    bool rndWorldCh_f { set; }
    bool refHorCh_f { set; }
    bool symCtrl_f { set; }
    int seChiu { set; }
    CharaMotionType ctMotionPast { set; }
    bool stk1StNow_f { set; }
    bool stk1StPst_f { set; }
    bool stk2StNow_f { set; }
    bool stk2StPst_f { set; }
    int FlySkip { set; }
    int FlySkip_c { set; }
}