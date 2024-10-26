using Godot;

namespace db;

// //シュート
// const s64 MBGIDBASE = 0xFFFFFFFF;
// const s64 MBGIDKETA = 7;
// const s64 SHOTNAMEMAX = 10;
// const s64 SHSETNAMEMAX = 6;
// public const int RARITYNUM = 7;//レアリティ
// public const int HSELEMMAX = 8;//最大原料数
// public const int HSELEMMAX_ADD = (HSELEMMAX - 1);//最大原料数
// public const int HSATMMAX = 8;//最大素材数
// public const int STEPNUM = 7;//最大素材数//試合中およびシュート装備などでつかう
// public const int SOZAIMID = 10;//素材のmidは1の位がレアリティ
// public const int SLOTALL = 49;//チームやシュートセットのスロット
//
// const s64 SHSETNUM = 4;//おまかせ＋123（行数を数えると、通信した敵のシュートセットの数まで拾ってしまう）
//
// //装備品ショット
// public const int STEPTYPENUM = 6;//素早さＡ～Ｆの６段階
// //ステップが開くレベル（数字は開くレベル）
// public const int SLOTLV[STEPNUM * STEPTYPENUM] =
// {
//     //歩数０１２３４５６
//     6, 5, 3, 4, 2, 1, 0,//鈍足
//     6, 5, 3, 4, 2, 0, 1,
//     6, 5, 2, 4, 0, 1, 3,
//     5, 2, 0, 4, 1, 3, 6,
//     2, 0, 1, 4, 3, 5, 6,
//     0, 1, 2, 4, 3, 5, 6,//俊足（１歩目の開くレベルが低い）
// };
//
// public const int DEFSTEP = 3;
// public const int VJOPENSTEP = 3;
// public const int VJICONSTEP = 7;
// public const int SLOTNUM = 7;
// //レベルごとのステップ（数字は歩数）
// public const int SHOTSTEP[SLOTNUM * STEPTYPENUM] =
// {
//     //レベル０１２３４５６
//     6, 5, 4, 2, VJICONSTEP, 1, 0,
//     5, 6, 4, 2, VJICONSTEP, 1, 0,
//     4, 5, 2, 6, VJICONSTEP, 1, 0,
//     2, 4, 1, 5, VJICONSTEP, 0, 6,
//     1, 2, 0, 4, VJICONSTEP, 5, 6,
//     0, 1, 2, 4, VJICONSTEP, 5, 6,
// };
public static class Defines
{
    /// <summary>
    /// COMの攻撃キャッシュの長さ
    /// </summary>
    public const int ComAttackCacheLength = 5;

    /// <summary>
    /// チーム人数
    /// </summary>
    public const int MemberCount = 7;

    /// <summary>
    /// 内野人数
    /// </summary>
    public const int InfieldCount = 4;

    /// <summary>
    /// サイドの数
    /// </summary>
    public const int SideCount = 2;

    /// <summary>
    /// 試合人数
    /// </summary>
    public const int AllCharaCount = MemberCount * SideCount;

    public const int RarityNum = 7;
    public const int HsElemMax = 8;
    public const int HsElemMaxAdd = HsElemMax - 1;
    public const int HsAtmMax = 8;
    public const int StepNum = 7;
    public const int SozaiMid = 10;
    public const int SlotAll = 49;
    public const int DefStep = 3;
    public const int VjOpenStep = 3;
    public const int VjIconStep = 7;
    public const int SlotNum = 7;
    public const int ComPattern = 4; //COM思考分岐

    public const int BufferPositionMax = 16;
    public const int KAGAMI2HP = 8; //辛息切れボーダーＨＰ
    public const int DIVELIMDY = -1 * Percent; //ダッシュマンジャンプのパス可能下降限界
    public const int RANKNUM = 17; //レベル17段階
    public const int SHLAG = 10; //約10フレ後に投げる
    public const int NEARDISTX = 24 * Percent;
    public const int NEARDISTZ = 12 * Percent;
    public const int DEFDISTZ = 16 * Percent;
    public const int MIRWAIT = 4;
    public const int Percent = 100;

    public const int PACAJPTIME = 13; //ジャンプパスキャッチタイミング
    public const int JPINVALIDHEIGHT = 16; // 操作無効高さ
    public const int JPGAIYAPER = 70; //外野斜めジャンプのXZ

    //baseからの引き継ぎ
    public const int NGNUM = -1;
    public const int GRIDSIZE = 8;
    public const int GRIDSIZE_HF = 4;
    public const int GRIDSIZEx2 = 16;

    // const u32 NGID = base::NGID;
    // public const int Percent = base::Percent;
    // public const int SERIFULINE = base::SERIFULINE;
    // public const int SIDEPADNUM = base::SIDEPADNUM;

    public const int DBSIDE = 2; //試合中のチーム数
    public const int SIDE0 = 0; //左サイド
    public const int SIDE1 = 1; //右サイド
    public const int CAPNO = 0; //キャプテン
    public const int DBMEMBER_ALL = 7; //１チーム人数
    public const int DBMEMBER_INF = 4; //１チーム内野人数
    public const int JHSRANGE = 4; //ジャンプ必殺タイミング幅
    public const int JHSRANGE_Hf = JHSRANGE / 2; //ジャンプ必殺タイミング幅
    public const int JHSSTART = -(JHSRANGE * 3 + JHSRANGE_Hf); //頂点が３なので
    public const int DHSRANGE = 2; //ダッシュ必殺タイミング幅
    public const int DBFOMMAX = 41; //フォーメーションの数
    public const int COMTYPE = 100; //COM思考タイプ
    public const int COMPTN = 4; //COM思考分岐
    public const int DBETCOBJ = 8; //試合中の小物オブジェの数
    public const int PLAYERNUM = 4; // 他のプレイヤー数
    public const int RANKERNUM = 1000; // ランキング表示名
    public const int MIRLIM = 10; //ミラーシュート受付時間
    public const int SHTAG_NEARDIST = 9 * Percent; //シュートターゲット近すぎ
    public const int SHTAG_FARDIST_Y = 150; //シュートターゲットY遠すぎ

    //座標
    public const int GAME_W = 448;
    public const int GAME_H = 288;
    public const int GAME_WHf = GAME_W / 2;
    public const int GAME_HHf = GAME_H / 2;
    public const int DRAWBASELINE = 240;
    public const int DRAWBASELINE_PREV = 200;
    public const int DAMPOSY = 48;
    public const int TIPSIZE800 = 8 * Percent; //100
    public const int MAXSPD = 16 * Percent; //100
    public const int DBCRT_W = GAME_W * Percent; //コート全幅
    public const int DBCRT_CL = DBCRT_W / 2; //216//センターライン
    public const int DBCRT_CLI = DBCRT_CL - (4 * Percent); //216//センターライン
    public const int DBCRT_FL = (18) * Percent; //手前サイドライン
    public const int DBCRT_BL = (98) * Percent; //奥サイドライン
    public const int DBCRT_SL = 42 * Percent; //左右ベースライン(内野キャラの足の位置で一番外側)
    public const int DBCRT_SL2 = DBCRT_CL - (DBCRT_SL - DBCRT_CL); //右コートのベースライン
    public const int DBCRT_CLXL = DBCRT_SL + (DBCRT_CL - DBCRT_SL) / 2; //Ｘ軸中心線（えんりん用）129
    public const int DBCRT_CLXR = DBCRT_W - DBCRT_CLXL; //Ｘ軸中心線（えんりん用）
    public const int DBCRT_CLZ = DBCRT_FL + (DBCRT_BL - DBCRT_FL) / 2; //Ｚ軸中心線（かっくん用）
    public const int DBCRT_SLR = 33; //傾き
    public const int DBCRT_WAL = 112 * Percent; //奥壁
    public const int CRPOS_B_XL = DBCRT_SL + (16 * Percent); //奥外野Ｘ左限界
    public const int CRPOS_B_Z = DBCRT_BL + (10 * Percent); //奥外野Ｚ
    public const int CRPOS_F_XL = DBCRT_SL; //手前外野Ｘ左限界
    public const int CRPOS_F_Z = DBCRT_FL - (16 * Percent); //手前外野Ｚ
    public const int CRPOS_S_X = 16 * Percent; //サイド外野Ｘ（ここに傾きが加わる）
    public const int CRPOS_S_ZB = DBCRT_BL; //サイド外野Ｚ（手前サイドラインと同じ）
    public const int CRPOS_S_ZF = DBCRT_FL; //サイド外野Ｚ（奥サイドラインと同じ）

    public const int POSPOST_X = 192 * Percent;
    public const int POSPOST_Z = 57 * Percent;
    public const int POS0_X = 100 * Percent;
    public const int POS0_Z = 57 * Percent;
    public const int POS1_X = 132 * Percent;
    public const int POS1_Z = 77 * Percent;
    public const int POS2_X = 132 * Percent;
    public const int POS2_Z = 37 * Percent;
    public const int POS4_X = 300 * Percent;
    public const int POS4_Z = 114 * Percent;
    public const int POS5_X = 320 * Percent;
    public const int POS5_Z = 8 * Percent;
    public const int POS6_X = 440 * Percent;
    public const int POS6_Z = 56 * Percent;

    /// <summary>
    /// 斜辺
    /// </summary>
    public static int Hypot(int x, int y)
    {
        return (int)System.Math.Sqrt(System.Math.Pow(x, 2) + System.Math.Pow(y, 2));
    }

    /// <summary>
    /// パーセンテージ
    /// </summary>
    public static int PercentageOf(int value, int percent)
    {
        return value * percent / Percent;
    }

    // enum enCrtLine
    // {
    //     crtLLine,
    //     crtRLine,
    //     crtFLine,
    //     crtBLine
    // };
    //
    // enum enDBPOS
    // {
    //     dbpoI0,
    //     dbpoI1,
    //     dbpoI2,
    //     dbpoI3,
    //     dbpoO2,
    //     dbpoO3,
    //     dbpoO4
    // }; //ポジション
    //
    // enum enATType
    // {
    //     ATA,
    //     ATD,
    //     ATF
    // }; //動作
    //
    // enum enCKeyType
    // {
    //     CKTX,
    //     CKTZ
    // }; //変化方向
    //
    // enum enJudgeType
    // {
    //     JdNoHit,
    //     JdCatch,
    //     JdNiceCatch,
    //     JdHit
    // };
    //
    // enum enGetTagType
    // {
    //     gtLevel,
    //     gtTgY,
    //     gtTg16,
    //     gtBase
    // }; //変化方向
    //
    // //外見
    // enum enCharLooks
    // {
    //     clk_id, //キャラＩＤ
    //     clk_FaceNo, //かお番号
    //     clk_ZuraNo, //ずら番号
    //     clk_SkinCol, //はだいろ
    //     clk_HairCol, //かみいろ
    //     clk_AcceCol, //あくせいろ
    //     clk_EyesCol, //めいろ
    //     clk_SearchFlag, //検索ひっかかるフラグ
    //     clk_LooksFlag, //みためフラグ
    //     clk_END,
    // };
    //
    // //レベル
    // enum enCharLv
    // {
    //     clv_Pow = clk_END, //ぱわー
    //     clv_Guts, //がっつ
    //     clv_Kire, //きれ
    //     clv_Tech, //てく
    //     clv_Speed, //すばやさ（必殺ステップ）
    //     clv_Rev, //レベル補正
    //     clv_DefShotID, //シュートＩＤ
    //     clv_Kakusi, //上田スイッチなど
    //     clv_Formation, //フォーメーション
    //     clv_COM, //COM思考タイプ
    //     clv_AtcLine, //攻撃ライン
    //     clv_DsmnNoJp, //ダッシュマンがジャンプしない
    //     clv_RarityLv, //レアリティレベル
    //     clv_BodyType, //体技
    //     clv_CharacterType, //心
    //     clv_RandomVal, //ブレ値（乱数値）
    //     clv_END,
    // };
    //
    // enum enSearchFlag
    // {
    //     sf_None = 0x00,
    //     sf_Face = 0x01, //顔検索する
    //     sf_Hair = 0x02, //づらけんさくする
    // };
    //
    // enum enLooksFlag
    // {
    //     lf_None = 0x00,
    //     lf_Hage = 0x01, // スキンヘッド
    //     lf_BarCode = 0x02, //バーコード
    //     lf_Pants = 0x04, //パンツ
    //     lf_Mask = 0x08, //マスク
    //     lf_ZuraKaibutsu = 0x10, //怪物髪
    //     lf_KaoKaibutsu = 0x20, //怪物ズラ
    // };
    //
    // //星座
    // enum enCharStar
    // {
    //     cdst_00_ARIES, //おひつじ3月21日?4月19日生まれ
    //     cdst_01_TAURUS, //おうし4月20日?5月20日生まれ
    //     cdst_02_GEMINI, //ふたご5月21日?6月21日生まれ
    //     cdst_03_CANCER, //かに6月22日?7月22日生まれ
    //     cdst_04_LEO, //しし7月23日?8月22日生まれ
    //     cdst_05_VIRGO, //おとめ8月23日?9月22日生まれ
    //     cdst_06_LIBRA, //てんびん9月23日?10月23日生まれ
    //     cdst_07_SCORPIO, //さそり10月24日?11月22日生まれ
    //     cdst_08_SAGITTARIUS, //いて11月23日?12月21日生まれ
    //     cdst_09_CAPRICORN, //やぎ12月22日?1月19日生まれ
    //     cdst_10_AQUARIUS, //みずがめ1月20日?2月18日生まれ
    //     cdst_11_PISCES, //うお2月19日?3月20日生まれ
    //     cdst_UNKNOWN, //ひみつ
    //     cdst_END,
    // };
    //
    // // キャラデータフラグ
    // enum enCharSp_fs
    // {
    //     csNone = 0x00, // 通常
    //     csUeda = 0x01, //うえだ
    //     cdRandom = 0x02, //ランダム
    // };

}