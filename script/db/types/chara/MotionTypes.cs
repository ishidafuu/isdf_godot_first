using System;

namespace db;

/// <summary>
/// モーションタイプ
/// </summary>
public enum CharaMotionType
{
    St,
    Breath,
    Wk,
    Ds,
    JCr,
    CJCr,
    JUp,
    JDn,
    ARv,
    Cr,
    FlF,
    FlB,
    PHF,
    PHB,
    DnHF,
    DnHB,
    KG,
    DnF,
    DnB,
    DRv,
    CM,
    JCM,
    FB,
    JFB,
    PW,
    PWWk,
    PWDs,
    Sl,
    Sh,
    RtSh,
    Pa,
    JSh,
    RtJSh,
    JPa,
    Ca,
    JCa,
    Dg,
    JDg,
    RoF,
    RoB,
    DRAW,
    WIN,
    LOSE,
    OvL,
    USA,
    USA2,
    IKI,
    LOOK,
    LOOK2,
    FALL,
    AGE2,
    AGE3,
    AGE4,
    AGE5,
    DO1,
    DO2,
    ANG,
};

/// <summary>
/// モーションフラグ
/// </summary>
[Flags]
public enum CharaMotionFlag
{
    None = 0x00000,
    Ar = 0x00001,
    Ds = 0x00002,
    Act = 0x00004,
    Dam = 0x00008,
    MTK = 0x00010,
    DmOK = 0x00020,
    JpOK = 0x00040,
    Muki = 0x00080,
    ANG = 0x00100,
    RES = 0x00200,
    Dn = 0x00400,
    PHit = 0x00800,
    PW = 0x01000,
    KG = 0x02000,
    Slip = 0x04000,
    AtCa = 0x08000,
    Dg = 0x10000,
};

/// <summary>
/// モーション番号
/// </summary>
public enum CharaMotionNo
{
    STF, //立ち（前）
    STN, //立ち（横）
    STR, //立ち（奥）
    WKF, //歩き（前）
    WKN, //歩き（横）
    WKR, //歩き（奥）
    DS, //ダッシュ

    JCF, //ジャンプ（前）
    JCN, //ジャンプ（横）
    JCR, //ジャンプ（奥）

    CRF, //着地（前）
    CRN, //着地（横）
    CRR, //着地（奥）

    FLF, //吹っ飛び（前から）
    FLB, //吹っ飛び（後ろから）

    KG, //息切れ
    KG2, //息切れ２

    DNF, //ダウン（前から）
    DNB, //ダウン（後ろから）
    CMF, //キャッチング（前）
    CMN, //キャッチング（横）
    CMR, //キャッチング（奥）

    JCMF, //ジャンプキャッチング（前）
    JCMN, //ジャンプキャッチング（横）
    JCMR, //ジャンプキャッチング（奥）

    FBF, //ファンブル（前）
    FBN, //ファンブル（横）
    FBR, //ファンブル（奥）

    JFBF, //ジャンプファンブル（前）
    JFBN, //ジャンプファンブル（横）
    JFBR, //ジャンプファンブル（奥）

    PWF, //パス要求（前）
    PWN, //パス要求（横）
    PWR, //パス要求（奥）

    SL, //スリップ

    SHF, //シュート（前）
    SHN, //シュート（横）
    SHR, //シュート（奥）

    PAF, //パス（前）
    PAN, //パス（横）
    PAR, //パス（奥）

    JSHF, //ジャンプシュート（前）
    JSHN, //ジャンプシュート（横）
    JSHR, //ジャンプシュート（奥）

    CAF, //キャッチモーション（前）
    CAN, //キャッチモーション（横）
    CAR, //キャッチモーション（奥）

    JCAF, //ジャンプキャッチモーション（前）
    JCAN, //ジャンプキャッチモーション（横）
    JCAR, //ジャンプキャッチモーション（奥）

    DGF, //しゃがみよけ（前）
    DGN, //しゃがみよけ（横）
    DGR, //しゃがみよけ（奥）

    AIRUPF, //ジャンプ上昇中（前）
    AIRUPN, //ジャンプ上昇中（横）
    AIRUPR, //ジャンプ上昇中（奥）

    AIRDNF, //ジャンプ下降中（前）
    AIRDNN, //ジャンプ下降中（横）
    AIRDNR, //ジャンプ下降中（奥）

    JPAF, //ジャンプパス（前）
    JPAN, //ジャンプパス（横）
    JPAR, //ジャンプパス（奥）

    RTNSH, //振り返りシュート
    RTNJSH, //振り返りジャンプシュート
    PHF, //立ちパス喰らい（前から）
    PHB, //立ちパス喰らい（後ろから）

    DNHF, //ダウン喰らい（前から）
    DNHB, //ダウン喰らい（後ろから）

    ROF, //転がり前転
    ROB, //転がり後転

    OVLF, //オーバーライン（前）
    OVLN, //オーバーライン（横）
    OVLR, //オーバーライン（奥）
    JDGF, //空中しゃがみよけ（前）
    JDGN, //空中しゃがみよけ（横）
    JDGR, //空中しゃがみよけ（奥）

    DRVF, //ダウン起き上がり（前）
    DRVN, //ダウン起き上がり（横）
    DRVR, //ダウン起き上がり（奥）

    PWWKF, //パス要求歩き（前）
    PWWKN, //パス要求歩き（横）
    PWWKR, //パス要求歩き（奥）
    PWDS, //パス要求走り

    BRF, //息継ぎ（前）
    BRN, //息継ぎ（横）
    BRR, //息継ぎ（奥）

    ANG, //天使
    DRAW, //引き分け
    WIN, //勝ち
    LOSE, //負け

    //USA,//兎跳び
    //USA2,//兎跳び２
    //IKI,//デモ息切れ
    //LOOK,//こっちみる
    //LOOK2,//にっこりする
    //AGEF,//胴上げ前
    //AGEN,//胴上げ横
    //AGER,//胴上げ奥
    //AGEC,//胴上げ真正面
    //DO1,//胴あげられ１
    //DO2,//胴あげられ２
}