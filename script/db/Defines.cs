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
    public const int Percent = 100;

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

}

// //グローバル扱いな変数
// namespace db
// {
//   typedef lib_num::TRect TRect;
//
//   enum enSceneName
//   {
//     snTitle,//titleはリセットなどで使うため0で固定
//     snHome,
//     snScenario,
//     snShiai,
// 	snTutorial,
//     snEND
//   };
//   
//   //ポップアップフォントカラー
//   enum enPopUp
//   {
//     pu_Act,
//     pu_Pow,
//     pu_DsMn,
//     pu_Damage,
//     pu_Call,
//     pu_Cutin,
//   };
//
//
//   //ステータスタイプ
//   enum endbStateType
//   {
//     dbst_Level,//レベル
//     dbst_Pow,//ぼーるぱわー,
//     dbst_Guts,//きあい,
//     dbst_Tech,//てく,
//     dbst_Kire,//きれ,
//     dbst_Speed,//すばやさ
//     dbst_HP,//のこりＨＰ
//   };
//
//   //baseからの引き継ぎ
//   public const int NGNUM = base::NGNUM;
//   const u32 NGID = base::NGID;
//   public const int Percent = base::Percent;
//   public const int GRIDSIZE = base::GRIDSIZE;
//   public const int GRIDSIZE_HF = base::GRIDSIZE_HF;
//   public const int GRIDSIZEx2 = base::GRIDSIZEx2;
//   public const int SERIFULINE = base::SERIFULINE;
//   public const int SIDEPADNUM = base::SIDEPADNUM;
//
//   public const int DBSIDE = 2;//試合中のチーム数
//   public const int SIDE0 = 0;//左サイド
//   public const int SIDE1 = 1;//右サイド
//   public const int CAPNO = 0;//キャプテン
//   public const int DBMEMBER_ALL = 7;//１チーム人数
//   public const int DBMEMBER_INF = 4;//１チーム内野人数
//   public const int RANKNUM = 17;//レベル17段階
//   public const int JHSRANGE = 4;//ジャンプ必殺タイミング幅
//   public const int JHSRANGE_Hf = JHSRANGE / 2;//ジャンプ必殺タイミング幅
//   public const int JHSSTART = -(JHSRANGE * 3 + JHSRANGE_Hf);//頂点が３なので
//   public const int DHSRANGE = 2;//ダッシュ必殺タイミング幅
//   public const int DBFOMMAX = 41;//フォーメーションの数
//   public const int COMTYPE = 100;//COM思考タイプ
//   public const int COMPTN = 4;//COM思考分岐
//   public const int DBETCOBJ = 8;//試合中の小物オブジェの数
//   public const int PLAYERNUM = 4;	// 他のプレイヤー数
//   public const int RANKERNUM = 1000; // ランキング表示名
//
//   //座標
//   public const int GAME_W = 448;
//   public const int GAME_H = 288;
//   public const int GAME_WHf = GAME_W / 2;
//   public const int GAME_HHf = GAME_H / 2;
//   public const int DRAWBASELINE = 240;
//   public const int DRAWBASELINE_PREV = 200;
//   public const int DAMPOSY = 48;
//   public const int TIPSIZE800 = 8 * Percent;//100
//   public const int MAXSPD = 16 * Percent;//100
//   public const int DBCRT_W = GAME_W * Percent;//コート全幅
//   public const int DBCRT_CL = DBCRT_W / 2;//216//センターライン
//   public const int DBCRT_CLI = DBCRT_CL - (4 * Percent);//216//センターライン
//   public const int DBCRT_FL = (18) * Percent;//手前サイドライン
//   public const int DBCRT_BL = (98) * Percent;//奥サイドライン
//   public const int DBCRT_SL = 42 * Percent;//左右ベースライン(内野キャラの足の位置で一番外側)
//   public const int DBCRT_SL2 = DBCRT_CL - (DBCRT_SL - DBCRT_CL);//右コートのベースライン
//   public const int DBCRT_CLXL = DBCRT_SL + (DBCRT_CL - DBCRT_SL) / 2;//Ｘ軸中心線（えんりん用）129
//   public const int DBCRT_CLXR = DBCRT_W - DBCRT_CLXL;//Ｘ軸中心線（えんりん用）
//   public const int DBCRT_CLZ = DBCRT_FL + (DBCRT_BL - DBCRT_FL) / 2;//Ｚ軸中心線（かっくん用）
//   public const int DBCRT_SLR = 33;//傾き
//   public const int DBCRT_WAL = 112 * Percent;//奥壁
//   public const int CRPOS_B_XL = DBCRT_SL + (16 * Percent);//奥外野Ｘ左限界
//   public const int CRPOS_B_Z = DBCRT_BL + (10 * Percent);//奥外野Ｚ
//   public const int CRPOS_F_XL = DBCRT_SL;//手前外野Ｘ左限界
//   public const int CRPOS_F_Z = DBCRT_FL - (16 * Percent);//手前外野Ｚ
//   public const int CRPOS_S_X = 16 * Percent;//サイド外野Ｘ（ここに傾きが加わる）
//   public const int CRPOS_S_ZB = DBCRT_BL;//サイド外野Ｚ（手前サイドラインと同じ）
//   public const int CRPOS_S_ZF = DBCRT_FL;//サイド外野Ｚ（奥サイドラインと同じ）
//
//   public const int POSPOST_X = 192 * Percent;
//   public const int POSPOST_Z = 57 * Percent;
//   public const int POS0_X = 100 * Percent;
//   public const int POS0_Z = 57 * Percent;
//   public const int POS1_X = 132 * Percent;
//   public const int POS1_Z = 77 * Percent;
//   public const int POS2_X = 132 * Percent;
//   public const int POS2_Z = 37 * Percent;
//   public const int POS4_X = 300 * Percent;
//   public const int POS4_Z = 114 * Percent;
//   public const int POS5_X = 320 * Percent;
//   public const int POS5_Z = 8 * Percent;
//   public const int POS6_X = 440 * Percent;
//   public const int POS6_Z = 56 * Percent;
// }