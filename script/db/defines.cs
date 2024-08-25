using Godot;

namespace db;

// //シュート
// const s64 MBGIDBASE = 0xFFFFFFFF;
// const s64 MBGIDKETA = 7;
// const s64 SHOTNAMEMAX = 10;
// const s64 SHSETNAMEMAX = 6;
// const s32 RARITYNUM = 7;//レアリティ
// const s32 HSELEMMAX = 8;//最大原料数
// const s32 HSELEMMAX_ADD = (HSELEMMAX - 1);//最大原料数
// const s32 HSATMMAX = 8;//最大素材数
// const s32 STEPNUM = 7;//最大素材数//試合中およびシュート装備などでつかう
// const s32 SOZAIMID = 10;//素材のmidは1の位がレアリティ
// const s32 SLOTALL = 49;//チームやシュートセットのスロット
//
// const s64 SHSETNUM = 4;//おまかせ＋123（行数を数えると、通信した敵のシュートセットの数まで拾ってしまう）
//
// //装備品ショット
// const s32 STEPTYPENUM = 6;//素早さＡ～Ｆの６段階
// //ステップが開くレベル（数字は開くレベル）
// const s32 SLOTLV[STEPNUM * STEPTYPENUM] =
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
// const s32 DEFSTEP = 3;
// const s32 VJOPENSTEP = 3;
// const s32 VJICONSTEP = 7;
// const s32 SLOTNUM = 7;
// //レベルごとのステップ（数字は歩数）
// const s32 SHOTSTEP[SLOTNUM * STEPTYPENUM] =
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
//   const s32 NGNUM = base::NGNUM;
//   const u32 NGID = base::NGID;
//   const s32 XYMAG = base::XYMAG;
//   const s32 GRIDSIZE = base::GRIDSIZE;
//   const s32 GRIDSIZE_HF = base::GRIDSIZE_HF;
//   const s32 GRIDSIZEx2 = base::GRIDSIZEx2;
//   const s32 SERIFULINE = base::SERIFULINE;
//   const s32 SIDEPADNUM = base::SIDEPADNUM;
//
//   const s32 DBSIDE = 2;//試合中のチーム数
//   const s32 SIDE0 = 0;//左サイド
//   const s32 SIDE1 = 1;//右サイド
//   const s32 CAPNO = 0;//キャプテン
//   const s32 DBMEMBER_ALL = 7;//１チーム人数
//   const s32 DBMEMBER_INF = 4;//１チーム内野人数
//   const s32 RANKNUM = 17;//レベル17段階
//   const s32 JHSRANGE = 4;//ジャンプ必殺タイミング幅
//   const s32 JHSRANGE_Hf = JHSRANGE / 2;//ジャンプ必殺タイミング幅
//   const s32 JHSSTART = -(JHSRANGE * 3 + JHSRANGE_Hf);//頂点が３なので
//   const s32 DHSRANGE = 2;//ダッシュ必殺タイミング幅
//   const s32 DBFOMMAX = 41;//フォーメーションの数
//   const s32 COMTYPE = 100;//COM思考タイプ
//   const s32 COMPTN = 4;//COM思考分岐
//   const s32 DBETCOBJ = 8;//試合中の小物オブジェの数
//   const s32 PLAYERNUM = 4;	// 他のプレイヤー数
//   const s32 RANKERNUM = 1000; // ランキング表示名
//
//   //座標
//   const s32 GAME_W = 448;
//   const s32 GAME_H = 288;
//   const s32 GAME_WHf = GAME_W / 2;
//   const s32 GAME_HHf = GAME_H / 2;
//   const s32 DRAWBASELINE = 240;
//   const s32 DRAWBASELINE_PREV = 200;
//   const s32 DAMPOSY = 48;
//   const s32 TIPSIZE800 = 8 * XYMAG;//100
//   const s32 MAXSPD = 16 * XYMAG;//100
//   const s32 DBCRT_W = GAME_W * XYMAG;//コート全幅
//   const s32 DBCRT_CL = DBCRT_W / 2;//216//センターライン
//   const s32 DBCRT_CLI = DBCRT_CL - (4 * XYMAG);//216//センターライン
//   const s32 DBCRT_FL = (18) * XYMAG;//手前サイドライン
//   const s32 DBCRT_BL = (98) * XYMAG;//奥サイドライン
//   const s32 DBCRT_SL = 42 * XYMAG;//左右ベースライン(内野キャラの足の位置で一番外側)
//   const s32 DBCRT_SL2 = DBCRT_CL - (DBCRT_SL - DBCRT_CL);//右コートのベースライン
//   const s32 DBCRT_CLXL = DBCRT_SL + (DBCRT_CL - DBCRT_SL) / 2;//Ｘ軸中心線（えんりん用）129
//   const s32 DBCRT_CLXR = DBCRT_W - DBCRT_CLXL;//Ｘ軸中心線（えんりん用）
//   const s32 DBCRT_CLZ = DBCRT_FL + (DBCRT_BL - DBCRT_FL) / 2;//Ｚ軸中心線（かっくん用）
//   const s32 DBCRT_SLR = 33;//傾き
//   const s32 DBCRT_WAL = 112 * XYMAG;//奥壁
//   const s32 CRPOS_B_XL = DBCRT_SL + (16 * XYMAG);//奥外野Ｘ左限界
//   const s32 CRPOS_B_Z = DBCRT_BL + (10 * XYMAG);//奥外野Ｚ
//   const s32 CRPOS_F_XL = DBCRT_SL;//手前外野Ｘ左限界
//   const s32 CRPOS_F_Z = DBCRT_FL - (16 * XYMAG);//手前外野Ｚ
//   const s32 CRPOS_S_X = 16 * XYMAG;//サイド外野Ｘ（ここに傾きが加わる）
//   const s32 CRPOS_S_ZB = DBCRT_BL;//サイド外野Ｚ（手前サイドラインと同じ）
//   const s32 CRPOS_S_ZF = DBCRT_FL;//サイド外野Ｚ（奥サイドラインと同じ）
//
//   const s32 POSPOST_X = 192 * XYMAG;
//   const s32 POSPOST_Z = 57 * XYMAG;
//   const s32 POS0_X = 100 * XYMAG;
//   const s32 POS0_Z = 57 * XYMAG;
//   const s32 POS1_X = 132 * XYMAG;
//   const s32 POS1_Z = 77 * XYMAG;
//   const s32 POS2_X = 132 * XYMAG;
//   const s32 POS2_Z = 37 * XYMAG;
//   const s32 POS4_X = 300 * XYMAG;
//   const s32 POS4_Z = 114 * XYMAG;
//   const s32 POS5_X = 320 * XYMAG;
//   const s32 POS5_Z = 8 * XYMAG;
//   const s32 POS6_X = 440 * XYMAG;
//   const s32 POS6_Z = 56 * XYMAG;
// }
