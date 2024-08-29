#pragma once
#include "dbStGen.h"
//#include "dbBaShot_Tri.h"
//#include "dbBaShot_Acc.h"
namespace db
{
    class TDtElement;
    class TDtBallAnime;
    const s32 SPARATE_MAX = 24;//分裂球MAX個数
    const s32 PHANTOM_MAX = 6;//山田本体以外の数
    const s32 PAST_DATA = 36;//過去データ保存数
    const s32 UPDATE_TIME = 3;//過去データの遡り刻み数（Fr）
    const s32 SEPARATE_START_NO = 77;//分裂系素材開始番号

    //逆進系素材用データ
    const f32 CST_POS_FROM_SIDE = 4.0f;//壁からの距離
    const f32 CST_POS_XL = CST_POS_FROM_SIDE;//左壁
    const f32 CST_POS_XR = 448.0f - CST_POS_FROM_SIDE;//右壁
    const f32 CST_POS_Y = 80.0f;
    const f32 CST_POS_ZSL = 16.0f;
    const f32 CST_POS_ZDP = 100.0f;

    //加速系処理名
    enum enAccDt{
      adt_Non,//処理なし
      adt_Acl,//加速処理
      adt_Dcl,//減速処理
      adt_YAc,//Y加速処理
      adt_YAA,//Y加速＋XZ加速減速処理
      adt_Plb,//放物線処理
      adt_PlA,//放物線＋XZ加速減速処理
      adt_END
    };

    //さいどかーぶだま 判定
    enum enSideCurveJudge{
        sdj_trg,
        sdj_wvl,
        sdj_wvs,
        sdj_chg,
        sdj_END
    };

    //主軌道計算用データ
    struct TShotAngleData{

      //phs.tagposX;//標的位置x ＊targPosX
      //phs.tagposY;//標的位置y ＊targPosY
      //phs.tagposZ;//標的位置z ＊targPosZ
      //phs.posX; //術者X（シュート開始位置）＊shotPosX
      //phs.posY; //術者Y（シュート開始位置）＊shotPosY
      //phs.posZ; //術者Z（シュート開始位置）＊shotPosZ
      //phs.distX; //標的距離X～術者X  ＊targetDistX = targPosX - shotPosX ;
      //phs.distY; //術者Y～標的頭上距離Y ＊targetDistY = shotPosY - ( targPosY + HEAD ) ;
      //phs.distZ; //術者Z～標的サイド判定幅 ＊targetDistZ = shotPosZ - ( targPosZ + SIDE ) ;

      f32 incTimes;//加算回数・・・分割したtargetDistをsoreTateAngleから何回加算するかを決める

      //crt.trg.awayY_Dir;//反れ縦方向 ＊soreTateAngle
      f32 posXJudge;//標的のX方向： 「球→標的」なら＋1、「標的←球」なら-1
      f32 soreTateDirX;//反れ縦方向X　狙いのx座標
      f32 soreTateDirY;//反れ縦方向Y　狙いのy座標。標的胸狙いを０、頭方向を＋。 後で標的座標に足す
      f32 soreTateRateXY;//反れ縦比率y／x
      BOOL soreTateZeroCheck;//反れ縦x＝０時、TRUE(y垂直飛び)：ゼロ除算回避

      //crt.trg.awayZ_Dir;//反れサイド方向 ＊soreSideAngle
      f32 posZJudge;//正面、標的手前、標的奥のジャッジ
      f32 soreSideDirX;//反れ縦方向X　狙いのx座標
      f32 soreSideDirZ;//反れ縦方向z　狙いのz座標。
      f32 soreSideRateXZ;//反れ縦比率z／x
      BOOL soreSideZeroCheck;//反れ縦x＝０時、TRUE(z垂直飛び)：ゼロ除算回避

      //phs.spd; //未分割の速度
      //phs.spdX; //主軌道x速度 ＊mainSpeedX
      //phs.spdY; //主軌道y速度 ＊mainSpeedY
      //phs.spdZ; //主軌道z速度 ＊mainSpeedZ
      //crt.trg.backward_Time;//逆進状態

      BOOL prrY_f;//XY比率でY優先
      BOOL prrZ_f;//XZ比率でZ優先
      BOOL ignXY_f;//XYのX無視
      BOOL ignXZ_f;//XZのX無視
      BOOL ignYZ_f;//XZのX無視
      f32 ratioXY;//XY比率
      f32 ratioXZ;//XZ比率
      f32 ratioYZ;//XZ比率
      f32 ratioYZ_SLineSl;//サイドライン手前を到達点としたZY比率
      BOOL ratioYZ_SLineSlZ0_f;//サイドライン手前を到達点としたZY比率Z=0フラグ
      f32 ratioYZ_SLineDp;//サイドライン奥を到達点としたZY比率
      BOOL ratioYZ_SLineDpZ0_f;//サイドライン手前を到達点としたZY比率Z=0フラグ
      BOOL zeroZ_f;//Z移動無フラグ
      f32 distX_a;//絶対値距離X
      f32 distY_a;//絶対値距離Y
      f32 distZ_a;//絶対値距離Z
      s32 dirX;//方向X
      s32 dirY;//方向Y
      s32 dirZ;//方向Z
      BOOL groundSAngle_f;//SAngleでの地上フラグ→コレがあったらdy<0で移動
      void Init() { Zeromem(this); }
    };

    //二次関数用データ
    struct TShotAccData{
      //使用変数等

      f32 shotPosX;//術者X（シュート開始位置）
      f32 shotPosY;//術者Y（シュート開始位置）
      f32 shotPosZ;//術者Z（シュート開始位置）
      f32 targPosX;//標的位置x
      f32 targPosY;//標的位置y
      f32 targPosZ;//標的位置z
      f32 paraGrav;//山なり素材に入っている重力データを式に使用できるよう変換したデータ
      f32 accelPow;//加速度関連データを使用できるよう変換したデータ
      f32 spd;//xyz未分割の速度…処理の簡略化の為、放物線に大きく関わる方向間での分割を優先する
      f32 dclInitPosX;//減速開始位置x（減速効果の段階に突入した時の点）
      f32 dclInitActPosX;//減速本番開始位置x（減速効果が実際に発動する点）
      //f32 COURT_LENGTH = 448;//コート長さ
      //基本的にはx進行方向y重力加速方向となるので、x方向y方向間での速度分割となる。
      //その際、z方向の速度は、z方向の必要な距離を計算し、x方向で割り出された速度などから
      //z方向速度を決める
      f32 accelSpeedX;//加速系軌道の速度x
      f32 accelSpeedY;//加速系軌道の速度y
      f32 accelSpeedZ;//加速系軌道の速度z

      f32 mainSpeedX;//SAngleにて速度分割直後の速度X
      f32 mainSpeedY;//SAngleにて速度分割直後の速度Y
      f32 mainSpeedZ;//SAngleにて速度分割直後の速度Z

      BOOL palabOrb_f;//山なり軌道素材
      BOOL accelEff_f;//加速効果素材
      BOOL decelEff_f;//減速効果素材
      BOOL yAccelEff_f;//y加速効果素材
      BOOL yxAccelEff_f;//y加速効果素材
      BOOL plAccOrb_f;//山なり＋加速軌道素材

      BOOL palabOrbInit_f;//山なり軌道素材
      BOOL accelEffInit_f;//加速効果素材
      BOOL decelEffInit_f;//減速効果素材
      BOOL yAccelEffInit_f;//y加速効果素材
      BOOL yxAccelEffInit_f;//y加速効果素材
      BOOL plAccOrbInit_f;//山なり＋加速軌道素材


      void Init() { Zeromem(this); }
    };

    //三角関数用データ
    struct TShotTriData{

      //ボール位置は仮想位置と表示位置の２つがあり、
      //今までは仮想位置を計算してきたが
      //ココでは仮想位置から表示位置に換算する。
      f32 shotPosX;//ボール位置x（仮想位置）
      f32 shotPosY;//ボール位置y（仮想位置）
      f32 shotPosZ;//ボール位置z（仮想位置）
      f32 ballMasterPosX;//振動によりシフトしたボール位置x（表示位置）
      f32 ballMasterPosY;//振動によりシフトしたボール位置y（表示位置）
      f32 ballMasterPosZ;//振動によりシフトしたボール位置z（表示位置）

      s32 dispPosX;//実際の表示位置x
      s32 dispPosY;//実際の表示位置y
      s32 dispPosZ;//実際の表示位置z

      //データから取得
      //波フラグ
      BOOL waveX_f;//波前後フラグ
      BOOL waveY_f;//波縦フラグ
      BOOL waveZ_f;//波サイドフラグ


      //各種イニシャルフラグ
      BOOL waveXInit_f;//波xイニシャルフラグ
      BOOL waveYInit_f;//波yイニシャルフラグ
      BOOL waveZInit_f;//波zイニシャルフラグ

      //波前後
      s32 waveXType;//タイプ：sin・cos、ギザ・曲線
      s32 waveXLenLv;//波長Lv　５段階
      s32 waveXWidLv;//振幅Lv　５段階
      s32 waveXWChangeType;//振幅変化タイプ　減衰・減増減…・増減増…・増幅
      s32 waveXWChangeSpeed;//振幅変化速度　５段階
      s32 waveXPeakStop;//頂点停止
      BOOL waveXPeakStop_f;//頂点停止フラグ
      s32 waveXPeakStop_c;//頂点停止C
      f32 waveXAngle;//掛振動中の現角度
      s32 waveXWChangeCount;//掛振幅変化時間カウント
      s32 waveXDiv;//波分割
      f32 waveXZigAdd;//掛ジグザグ時の移動量
      BOOL waveXDiv_f;//波分割中フラグ
      s32 waveXDiv_c;//分割中時間カウント
      s32 waveXDivTime;//分割中時間
      s32 waveXAnglePhase;//象限数：第１象限～第４象限
      s32 waveXAngleRtTms;//何周したか

      //波縦
      s32 waveYType;//タイプ：sin・cos、ギザ・曲線
      s32 waveYLenLv;//波長Lv　５段階
      s32 waveYWidLv;//振幅Lv　５段階
      s32 waveYWChangeType;//振幅変化タイプ　減衰・減増減…・増減増…・増幅
      s32 waveYWChangeSpeed;//振幅変化速度　５段階
      s32 waveYPeakStop;//頂点停止
      BOOL waveYPeakStop_f;//頂点停止フラグ
      s32 waveYPeakStop_c;//頂点停止C
      f32 waveYAngle;//掛振動中の現角度
      s32 waveYWChangeCount;//掛振幅変化加算値
      s32 waveYDiv;//波分割
      f32 waveYZigAdd;//掛ジグザグ時の移動量
      BOOL waveYDiv_f;//波分割中フラグ
      s32 waveYDivTime;//分割中時間
      s32 waveYDiv_c;//分割中時間カウント
      s32 waveYAnglePhase;//象限数：第１象限～第４象限
      s32 waveYAngleRtTms;//何周したか

      //波サイド
      s32 waveZType;//タイプ：sin・cos、ギザ・曲線
      s32 waveZLenLv;//波長Lv　５段階
      s32 waveZWidLv;//振幅Lv　５段階
      s32 waveZWChangeType;//振幅変化タイプ　減衰・減増減…・増減増…・増幅
      s32 waveZWChangeSpeed;//振幅変化速度　５段階
      s32 waveZPeakStop;//頂点停止
      BOOL waveZPeakStop_f;//頂点停止フラグ
      s32 waveZPeakStop_c;//頂点停止C
      f32 waveZAngle;//掛振動中の現角度
      s32 waveZWChangeCount;//掛振幅変化加算値
      s32 waveZDiv;//波分割
      f32 waveZZigAdd;//掛ジグザグ時の移動量
      BOOL waveZDiv_f;//波分割中フラグ
      s32 waveZDivTime;//分割中時間
      s32 waveZDiv_c;//分割中時間カウント
      s32 waveZAnglePhase;//象限数：第１象限～第４象限
      s32 waveZAngleRtTms;//何周したか

      f32 addVal;//主軌道等に追加される値
      f32 triFuncVal;//三角関数等で得られる、振動-1～+1の値。⇒アニメ等で使用
      f32 triFuncValPr;//三角関数等で得られる、振動-1～+1の値。⇒アニメ等で使用
      f32 triFuncValFt;
      s32 triFuncPeakTime;
      void Init() { Zeromem(this); }
    };





    //***外部関連フラグ＆チェック
    enum enShotEffFlags{
        //●変化素材
        sef_changeHit_f,      //ヒットの確認（ヒット変化素材）
        sef_changeGround_f,   //着地の確認（地上変化素材）& posY＝0でも効果持続
        //ε効果素材
        //ヒット系効果
        sef_hitAndAnother_f,  //ヒット⇒別の標的を選ぶ
        sef_hitPenet_f,       //貫通効果
        sef_hitMySideHit_f,   //味方ヒット効果
        //特殊効果
        sef_effSuction_f,     //吸引効果
        sef_effOrbit_f,       //軌道描画効果
        //吹っ飛び系効果
        sef_flyFire_f,        //火吹っ飛び
        sef_flyElec_f,        //電気吹っ飛び
        sef_flyLoose_f,       //緩い吹っ飛び
        sef_flyRound_f,       //世界一周吹っ飛び
        sef_flyTogether_f,    //球にくっつき
        sef_flyVert_f,        //真上吹っ飛び
        sef_flyHori_f,        //真横吹っ飛び
        //ダウン系効果
        sef_dwnRoll_f,        //ゴロゴロダウン
        sef_dwnPress_f,       //プレスダウン
        sef_dwnNMNormal_f,    //その場ダウン＿普通
        sef_dwnNMUp_f,        //その場ダウン＿仰向け
        sef_dwnNMDown_f,      //その場ダウン＿俯せ
        sef_dwnNMRoll_f,      //その場ダウン＿回転
        sef_dwnNMRollUp_f,    //その場ダウン＿回転仰向け
        sef_dwnNMRollDown_f,  //その場ダウン＿回転俯せ
        //効果音
        sef_sePhase_f,        //効果音段階以降時
        sef_seDir_f,           //効果音方向違い
        sef_NUMMAX
    };

    //ぶんれつ系レベル用列挙型
    enum enShapeSeparateLevel
    {
        ssl_sinFqX,//単振X
        ssl_sinFqY,//単振Y
        ssl_sinFqZ,//単振Z
        ssl_mulFqX,//複振X
        ssl_mulFqY,//複振Y
        ssl_mulFqZ,//複振Z
        ssl_shadow,//追跡
        ssl_crclYZ,//円周YZ
        ssl_crclXZ,//円周XZ
        ssl_crclXY,//円周XY
        ssl_cr8VXY,//８周XY
        ssl_cr8VXZ,//８周XZ
        ssl_cr8VYZ,//８周YZ
        ssl_cr8HXY,//∞周XY
        ssl_cr8HXZ,//∞周XZ
        ssl_cr8HYZ,//∞周YZ
        ssl_phantom,
        ssl_NUMMAX
    };

    //山田の術状態
    enum enPhantomStep
    {
        phs_FOff,
        phs_Init,
        phs_Stay,
        phs_Term,
        phs_NUMMAX
    };



    //最大速度座標軸
    enum enMaxSpdCoordinateAxis
    {
        sca_Zero,
        sca_X,
        sca_Y,
        sca_Z,
        sca_NUMMAX
    };

    //波タイプ
    enum enWaveType
    {
        wvt_SinPWv,//Sin波（＋）
        wvt_SinNWv,//Sin波（－）
        wvt_CosPWv,//Cos波（＋）
        wvt_CosNWv,//Cos波（－）
        wvt_SinPZg,//Sin鋸（＋）
        wvt_SinNZg,//Sin鋸（－）
        wvt_CosPZg,//Cos鋸（＋）
        wvt_CosNZg,//Cos鋸（－）
        wvt_NUMMAX
    };


    //素材（elements）データ”elm”と実際(Current)の値を入れる現データ”crt”
    //同じ構造体を使用する

    //◆ ●変化素材 Chg = Change
    // このデータが！＝０ or TRUE ならその変化素材が発動し、その素材の項目を毎Fr監視し、
    // 条件を満たしたら次段階へ移行させる

    struct TChg{//変化データ構造体
        s32  time;          //変化素材：01時間 時間（取得時に×４してこの変数に入れる）
        s32  height;       //変化素材：03高さ ドット数【ボール位置y】

        s32  pos;          //変化素材：05位置 フラグ条件０～６、３は標的素材側で座標設定【ボール位置】
        s32  dist;         //変化素材：06距離 １～８段階 提供時２、４、６、８のみ
        //合成で残りの段階を使用　取り込み時に+1して取り込む【ボール位置x】
        s32  loopTime;     //変化素材：07ループ 戻る段階（0でオフ、１以降で成立）
        s32  loopMax;      //変化素材：07ループ 最大回数
        s32  waveXTimes;   //変化素材：08波X回数 波１周期を４、１象限を１とした回数
        s32  waveYTimes;   //変化素材：09波Y回数 波１周期を４、１象限を１とした回数
        s32  waveZTimes;   //変化素材：10波Z回数 波１周期を４、１象限を１とした回数

        s32 hit;          //変化素材：02ヒット フラグ
        s32 ground;       //変化素材：04地上 地上到達時フラグ
        s32 multiAngleFin;//変化素材：11多角処理終 終了フラグ【角数】【時間】
        s32 passOrbitFin; //変化素材：12パス挙動終 終了フラグ【ボール位置x】

        // 実際の現データcrtで使用
        s32  waveXTimes_c; //変化素材：08波X回数 カウント
        s32  waveYTimes_c; //変化素材：09波Y回数 カウント
        s32  waveZTimes_c; //変化素材：10波Z回数 カウント
        s32  multiAngle_Num_c; //標的素材：105多角処理 角数カウント
        s32  multiAngle_Time_c; //標的素材：105多角処理 曲がるまでのカウント
        f32  multiAngle_FtPosI;//標的素材：105多角処理 初角のスタート位置

    };

    //---------------------------------------------------------------------------------

    //◆ ♂標的素材 Trg = Target  Mtd = Method
    // 標的方向から逸れたりなど。基本的に標的素材は変化素材と対で存在する。
    // 反れ縦、反れサイドの素材が無い場合はそれぞれ
    // 方向04（標的方向）、方法00（直に向かう）を入れる
    struct TTrg{//標的データ構造体
        s32 awayY_Dir;       //標的素材：01反れ縦 方向0-15
        s32 awayY_Mtd;       //標的素材：01反れ縦 方法0-2
        s32 awayZ_Dir;       //標的素材：02反れサイド 方向0-15
        s32 awayZ_DirNoTg;   //標的素材：02反れサイド 敵無視データ・・・２番目のデータから読み取る
        s32 awayZ_Mtd;       //標的素材：02反れサイド 方法0-2
        s32 backward_Time;   //標的素材：03逆進 時間 ●変化素材時間でも処理
        s32 backward_Mtd;    //標的素材：03逆進 方法0-2
        s32 stopAll_Time;    //標的素材：04完全停止 時間 ●変化素材時間でも処理
        s32 stopAll_Mtd;     //標的素材：04完全停止 0-2
        s32 trgSelect;       //標的素材：05標的選択 0-3
        s32 trgSelect_Mtd;       //標的素材：05標的選択 0-3
        s32 stopForward_Time;//標的素材：06前進停止 時間 ●変化素材時間でも処理
        s32 stopForward_Mtd; //標的素材：06前進停止 0-2

        //位置Xは左右のコートにてセンターラインで線対称になるが
        //位置XZの場合は線対称にしない（軌道で色々描くための仕様のため）
        s32 posX_Dot;        //標的素材：101位置X ドット数
        s32 posX_Mtd;        //標的素材：101位置X 方法0-2
        s32 posY_Dot;        //標的素材：102位置Y ドット数
        s32 posY_Mtd;        //標的素材：102位置Y 方法0-2
        s32 posZ_Dot;         //標的素材：103位置Z ドット数
        s32 posZ_Mtd;        //標的素材：103位置Z 方法0-2
        s32 posXZ_Point;     //標的素材：104位置XZ XZ8*8ポイント指定
        s32 posXZ_Mtd;       //標的素材：104位置XZ 方法0-2

        s32 multiAngle_Time;  //標的素材：105多角処理 時間 ●変化素材多角終で処理
        s32 multiAngle_Num;   //標的素材：105多角処理 角数
        s32 keepCeiling_Time; //標的素材：106天井維持 時間 ●変化素材時間でも処理
        s32 keepCeiling_Mtd;  //標的素材：106天井維持 方法0-2
        s32 reflect_Time;     //標的素材：107跳ね返り 時間 ●変化素材時間でも処理
        s32 reflect_Dir;      //標的素材：107跳ね返り 方向0-3

        s32 passOrbit;       //標的素材：108パス挙動 ●変化素材多角終でも処理

    };

    //---------------------------------------------------------------------------------

    //◆ ～軌道素材 Orb = Orb
    // 標的素材で決まった方向に以下の方法でアプローチ
    // 001番山なり、100番台波101X、102Y、103Z
    struct TPalab{//山なりデータ構造体
        s32  point;     //軌道素材：001山なり 照準ポイント1-8
        s32  grav;      //軌道素材：001山なり 重力0-3
        s32  dist;      //軌道素材：001山なり 距離指定*4
    };

    struct TWave{//波データ構造体
        s32  wvtype;      //波軌道素材：101~103波タイプ1-8★typeはＣ＋＋の予約語とかぶる
        s32  width;     //波軌道素材：101~103振幅1-8
        s32  freq;      //波軌道素材：101~103振動数1-8
        s32  widthChg_Mtd;//波軌道素材：101~103振幅変化方法1-4
        s32  widthChg_Spd;//波軌道素材：101~103振幅変化速度1-5
        s32 peakStop;  //波軌道素材：101~103頂点停止
        s32  wvDiv;   //波軌道素材：101~103波分割⇒波有効状態と波無効状態に分割
        s32  wvAbs;//絶対値使用フラグ
        BOOL  wvRv_f;//波反転フラグ
        BOOL  wvRvRsrv_f;//波反転予約フラグElemToCrtで反転実行
        s32  wvtypeMem;      //波軌道素材：変更前データ
        BOOL wvtype_f[wvt_NUMMAX];//同一方向の波をプールしておく
        s32  wvtypeNoSort[wvt_NUMMAX];//同一方向の波プール 要素数が１波毎に＋されていく
        s32  wvtypeNo_c;
        s32  wvtypeNoMax;
        // 実際の現データcrtで使用
        f32  angle;//三角関数用角度（Rad）
        s32  widthChg_c;//振幅変化時の振幅の割合100の時 = width
        s32  peakStopTime;//peakStop時の止める時間（ステータスなどから算出）
        s32  peakStopTime_c;//peakStop時の止まっている時間のカウント
        s32  peakStop_f;//peakStop時の止まっているフラグ
        BOOL wvDivOff_f;//波分割時で、波無効状態の時のフラグ　TRUEで無効
        s32  anglePhase;//象限数：第１象限～第４象限
        s32  angleRtTms;//何周したか
        f32  triFuncVal;//三角関数の部分の値⇒取りうる値が-1～0～+1
        f32  triFuncValPr;//1fr前の三角関数の部分の値⇒取りうる値が-1～0～+1
        f32  triFuncValFt;//1fr前の三角関数の部分の値⇒取りうる値が-1～0～+1

        s32  zigAdd;//ジグザグ時の加算値
    };

    //軌道データ構造体
    struct TOrb{

        TPalab palab;//山なりデータ構造体
        TWave waveY;//波データ構造体⇒波Y用
        TWave waveZ;//波データ構造体⇒波Z用
        TWave waveX;//波データ構造体⇒波X用

    };

    //---------------------------------------------------------------------------------

    //◆ ☆★形状素材 shp = shape
    // 素材は１段階目にシフトされるので、
    // 段階共通データに「どの段階で」「何の形に」変化するかを
    // 登録しておく必要がある。★の場合はLvによって形などが変わる。
    //struct TShapeData{//形状データ読み込み用構造体
    //  s32 id;    //型番(csv)【素材番号】
    //  s32 lv;    //★追加形状素材で必要【素材データ】
    //  s32 phase; //発動段階…ソート⇒１段階目に来ているので毎回チェック
    //};

    //elmShpNum = //発動必殺シュートの中から形状素材の数をカウントしたもの
    //struct TShapeData shpData[elmShpNum];//↑の数の分だけ形状データを用意
    //素材データからこの配列にデータを移す処理をするので
    //elm.shp構造体は使用しない。と思う。

    //段階が変わるごとに、shpData[elmShpNum]を全てチェック
    //該当段階（phase）のデータがあればcrt.shp構造体に入れる。
    //複数で同じ段階を指定してくることもあるのでcrt.shp構造体も複数用意

    struct TShp{//アニメーション関連の現時点データ用構造体
        s32 id;    //ID
        s32 lv;
        BOOL tf_f;//同段階内形状変化アリフラグ
        //s32 fr;    //次に切り替わるフレーム数
        //s32 fr_c;    //フレーム数カウント
        //s32 koma;    //何枚目の絵か
        //s32 shptype;    //特殊アニメ用データ（IDから読み取る）★typeはＣ＋＋の予約語とかぶる
    };
    struct TShpTf{//アニメーション関連の現時点データ用構造体：同段階変形ある場合使用
        s32 id;    //ID
        s32 lv;
        s32 no;//TShpの要素数＝どの段階に属するか
        //s32 fr;    //次に切り替わるフレーム数
        //s32 fr_c;    //フレーム数カウント
        //s32 koma;    //何枚目の絵か
        //s32 shptype;    //特殊アニメ用データ（IDから読み取る）★typeはＣ＋＋の予約語とかぶる
    };

    struct TShpSprt{//ぶんれつ系
        s32 posX;//位置
        s32 posY;//位置
        s32 posZ;//位置
        f32 addX;//位置
        f32 addY;//位置
        f32 addZ;//位置
        BOOL disp_f;//表示フラグ
        s32 grphNo;//画像番号
        BOOL revHor_f;//左右反転
        BOOL revVer_f;//上下反転
        BOOL revClr_f;//色反転
        BOOL vnsBal_f;//ボール消し
        BOOL vnsSdw_f;//影消し
        BOOL use_f;//使用フラグ
        BOOL shadow_f;//追跡フラグ

        s32 sId;//形状ID
        s32 spdtShpid;
        s32 spdtAnmDataId;
        s32 spdtAnmPattId;
        s32 spdtPtnStep;
        s32 spdtAnmUseNum;
        s32 spdtAnmUseRevNum;
        s32 frame_c;
        f32 addMaxX;
        f32 addMaxY;
        f32 addMaxZ;
    };

    struct TShpSprtVar{//分裂系関数の変数
        s32 lv;//レベル
        s32 angle;//振動系の角度
        s32 delayTime_c;//追跡のディレイ時間
        s32 useDtNo[SPARATE_MAX];//使う配列の番号を保存する配列
        s32 dt_c;//分裂配列に入ったデータ数のカウント入らなかったらfull_fフラグを建てて中止
        BOOL init_f;
        BOOL full_f;//配列がいっぱいだった時のフラグ
        BOOL use_f;//使用フラグ

    };

    //---------------------------------------------------------------------------------


    struct TEff_PhLv
    {
        s32  phase;
        s32  lv;
    };

    const s32 EFMAX = 8;

    //◆ ε効果素材 eff = effect
    // 素材は１段階目にシフトされるので、
    // 段階共通データに「どの段階で」「何の形に」変化するかを
    // 登録しておく必要があるものもある。
    //以下は段階指定がなく、合成時該当素材が入っていれば
    //１段階目から効果が発揮される。
    struct TEff{
        s32  penet;     //1:貫き効果
        s32  penetTime; //1:貫き効果＋くっつき時間
        s32  suction;   //2:吸引効果
        s32  speedInc;  //3:速度＋効果
        s32  speedDec;  //4:速度－効果
        s32  compress;  //5:攻撃＋判定－効果
        s32  cntrlPlus; //122:変化＋効果
        s32  wEdge;     //6:諸刃効果
        s32  flyReverse;//7:引っ張り効果
        s32  flyFire;   //8:吹っ飛び火効果
        s32  flyElec;   //9:吹っ飛び電効果
        s32  rollDown;  //10:ゴロゴロダウン効果
        s32  flyLoose;  //11:吹っ飛び緩効果
        s32  rndWorld;  //12:世界一周効果
        s32  Transform; //13:段階変形効果
        s32  finalSE; //13:最終段階SE効果
        s32  waveKeep; //13:波持続効果
        s32  catchSlip;//キャッチスリップ
        s32  catchSlipR;//キャッチスリップ
        s32  symCtrl;//マリオネット効果：標的対称操作

        s32  spinPlus; //14:スピン強化
        s32  pass; //15:パス挙動効果
        //以下は段階指定があるので、合成により段階が
        //シフトしたデータを素材データバッファに入れておく必要がある
        //Phase（段階指定）、Lv、２データ群
        s32  accel_Phase[EFMAX];  //101:加速効果＿段階指定最初にみつかった段階
        s32  accel_Lv[EFMAX];     //101:加速効果＿Lvダブった分だけレベルアップ

        s32  decel_Phase[EFMAX];  //102:減速効果＿段階指定
        s32  decel_Lv[EFMAX];     //102:減速効果＿Lv

        s32  yAccel_Phase[EFMAX]; //103:y加速効果＿段階指定
        s32  yAccel_Lv[EFMAX];    //103:y加速効果＿Lv

        s32  spdChg_Phase[EFMAX]; //104:速度可変効果＿段階指定
        s32  spdChg_Lv[EFMAX];    //104:速度可変効果＿Lv

        s32  trace_Phase[EFMAX];  //105:追跡効果＿段階指定
        s32  trace_Lv[EFMAX];     //105:追跡効果＿Lv
        f32  trace_initSpdX[EFMAX]; //105:追跡効果＿初期速度X
        f32  trace_initDirX[EFMAX]; //105:追跡効果＿初期方向X
        f32  trace_init_f; //105:追跡効果
        BOOL trace_initDirZ_f;//トーレスの最初のメイン方向Z フラグ

        s32  reflect_Phase[EFMAX];//106:強反射効果＿段階指定
        s32  reflect_Lv[EFMAX];   //106:強反射効果＿Lv

        s32  mySide_Phase[EFMAX]; //107:味方被弾効果＿段階指定
        s32  mySide_Lv[EFMAX];    //107:味方被弾効果＿Lv

        s32  wEdge_Phase[EFMAX];  //111:圧殺効果＿段階指定
        s32  wEdge_Lv[EFMAX];     //111:圧殺効果＿Lv

        s32  normal_Phase[EFMAX]; //108:通常球化効果＿段階指定
        s32  normal_Lv[EFMAX];    //108:通常球化効果＿Lv

        s32  flyVer_Phase[EFMAX]; //109:真上吹っ飛び効果＿段階指定
        s32  flyVer_Lv[EFMAX];    //109:真上吹っ飛び効果＿Lv

        s32  flyHor_Phase[EFMAX]; //110:真横吹っ飛び効果＿段階指定
        s32  flyHor_Lv[EFMAX];    //110:真横吹っ飛び効果＿Lv

        s32  press_Phase[EFMAX];  //111:圧殺効果＿段階指定
        s32  press_Lv[EFMAX];     //111:圧殺効果＿Lv

        s32  copy_Phase[EFMAX];   //112:複写効果＿段階指定
        s32  copy_Lv[EFMAX];      //112:複写効果＿Lv

        s32  refVer_Phase[EFMAX]; //109:縦跳吹っ飛び効果＿段階指定
        s32  refVer_Lv[EFMAX];    //109:縦跳吹っ飛び効果＿Lv

        s32  refHor_Phase[EFMAX]; //110:横跳吹っ飛び効果＿段階指定
        s32  refHor_Lv[EFMAX];    //110:横跳吹っ飛び効果＿Lv

        s32  sprtOff_Phase[EFMAX];  //111:分裂オフ＿段階指定
        s32  sprtOff_Lv[EFMAX];     //111:分裂オフ＿Lv

        s32  tagChange_Phase[EFMAX];  //127:標的変化＿段階指定
        s32  tagChange_Lv[EFMAX];     //127:標的変化＿Lv

        s32  spdMomUp_Phase[EFMAX];  //128:一時速度上昇＿段階指定
        s32  spdMomUp_Lv[EFMAX];     //128:一時速度上昇＿Lv

        s32  spdMomDown_Phase[EFMAX];  //129:一時速度減少＿段階指定
        s32  spdMomDown_Lv[EFMAX];     //129:一時速度減少＿Lv

        s32  noDmg_Phase[EFMAX];  //130:ダメージナシ＿段階指定
        s32  noDmg_Lv[EFMAX];     //130:ダメージナシ＿Lv

        s32  flyReversePh_Phase[EFMAX];//7:引っ張り効果
        s32  flyReversePh_Lv[EFMAX];//7:引っ張り効果
        s32  flyLoosePh_Phase[EFMAX];  //11:吹っ飛び緩効果
        s32  flyLoosePh_Lv[EFMAX];  //11:吹っ飛び緩効果
        s32  rndWorldPh_Phase[EFMAX];  //12:世界一周効果
        s32  rndWorldPh_Lv[EFMAX];  //12:世界一周効果



        s32  phantom_Phase[EFMAX];  //131:山田＿段階指定
        s32  phantom_Lv[EFMAX];     //131:山田＿Lv
        f32  distRatioRealBall; //実際のボールからの距離率
        s32  startPhase;
        s32  endPhase;
        f32  decSpd;
        enPhantomStep phantomStep;//山田の現段階

        //３データ以上群
        s32  noMvDwn_Phase[EFMAX];//113:その場ダウン効果＿段階指定
        s32  noMvDwn_Lv[EFMAX];   //113:その場ダウン効果＿Lv
        s32  noMvDwn_Type[EFMAX]; //113:その場ダウン効果＿タイプ

        s32  combo_Phase[EFMAX];  //114:同一標的連続被弾効果＿段階指定
        s32  combo_Max[EFMAX];    //114:同一標的連続被弾効果＿最大コンボ数※
        s32  combo_Time[EFMAX];   //114:同一標的連続被弾効果＿
        s32  combo_Dam[EFMAX];   //114:同一標的連続被弾効果＿
        s32  combo_Lv[EFMAX];    //114:同一標的連続被弾効果＿コンボLv(実際は使わないけど読み取り時のバグ回避)
        //コンボ不可⇒コンボ可能フラグ解放時間※
        s32  combo_FlyAdd[EFMAX]; //114:同一標的連続被弾効果＿追加吹っ飛び　★確認する

        s32  pulse_Phase[EFMAX];  //115:拍効果＿段階指定
        s32  pulse_Lv[EFMAX];  //115:拍効果＿段階指定
        s32  pulse_Type[EFMAX];   //115:拍効果＿タイプ
        s32  pulse_TmStop[EFMAX]; //115:拍効果＿停止時間※
        s32  pulse_TmStrt[EFMAX]; //115:拍効果＿開始時間※
        //※複数段階にまたがる可能性があるので
        //カウンタを全段階対応にする。

        s32  sePhase_Phase[EFMAX]; //116:効果音段階変化時＿段階指定
        s32  sePhase_Num[EFMAX];   //116:効果音段階変化時＿SE番号

        s32  seHit_Phase[EFMAX];   //117:効果音被弾時＿段階指定
        s32  seHit_NumFrnt[EFMAX]; //117:効果音被弾時＿標的前ヒット＿SE番号
        s32  seHit_NumBack[EFMAX]; //117:効果音被弾時＿標的後ヒット＿SE番号
        s32  seHit_NumNrml[EFMAX]; //117:効果音被弾時＿標的普通ヒット＿SE番号

        //Phase１Datumのみ
        s32  orbPaint_Phase[EFMAX];//118:軌跡描画効果＿段階指定
        s32  orbErase_Phase[EFMAX];//119:軌跡消失効果＿段階指定
        s32  effClear_Phase[EFMAX];//120:効果クリア＿段階指定
        s32  end_Phase[EFMAX];     //121:シュート終了効果＿段階指定
        void Init() { Zeromem(this); }
    };


    //---------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------

    //◆段階素材データ構造体
    struct TPhaseElements{

        TChg chg;//変化データ構造体
        TTrg trg;//標的データ構造体
        TOrb orb;//軌道データ構造体

        void Init() { Zeromem(this); }
    };

    const s32 ELMSHPNUM = 72;

    ////◆全素材データ構造体
    //struct TCrtElements{

    //  TChg chg;//変化データ構造体
    //  TTrg trg;//標的データ構造体
    //  TOrb orb;//軌道データ構造体
    //  //TShpCrtPhase shpcrt[ELMSHPNUM];//形状データ現段階用構造体
    //  //TShp shp;//形状データ現時点用構造体
    //  //TEff eff;//効果データ構造体

    //  void Init() { Zeromem(this); }
    //};

    //---------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------



    //◆ 全体データ
    //各素材で使用するデータ。
    //段階ごと用構造体と全段階共通構造体をセットで用意する。



    //ボールデータ
    struct TBallData{
        //※distを求める際 distX = tagposX - posX とし、絶対値は取らないでOKです。distY、Zも同様です。
        BOOL spShot_f;//必殺シュートフラグ
        f32 posX;//位置x
        f32 posY;//位置y
        f32 posZ;//位置z
        f32 spd;//速度（未分割）
        f32 spdLv;
        f32 spdMomLv;//１段階分の速度Lv追加
        f32 spdX;//速度x（位置に追加）
        f32 spdY;//速度y（位置に追加）
        f32 spdZ;//速度z（位置に追加）
        f32 spdBfX;//速度x（保存用）
        f32 spdBfY;//速度y（保存用）
        f32 spdBfZ;//速度z（保存用）
        f32 acc;//加速度（未分割）
        f32 accX;//加速度x（実際は速度速度に追加）
        f32 accY;//加速度y（速度に追加）
        f32 accZ;//加速度z（速度に追加）
        f32 accHX;//加速度xX加速
        f32 accHY;//加速度yX加速
        f32 accHZ;//加速度zX加速
        f32 accVX;//加速度xY加速
        f32 accVY;//加速度yY加速
        f32 accVZ;//加速度zY加速
        f32 graAcc;//重力加速度
        f32 distX;//距離x
        f32 distY;//距離y
        f32 distZ;//距離z
        f32 realX;//実際の位置x（速度＋三角関数の前にココに保存し次フレームではこの値を最初に呼び出す）
        f32 realY;//実際の位置y（速度＋三角関数の前にココに保存し次フレームではこの値を最初に呼び出す）
        f32 realZ;//実際の位置z（速度＋三角関数の前にココに保存し次フレームではこの値を最初に呼び出す）
        f32 addX;//三角関数による加算x（実際は速度速度に追加）
        f32 addY;//三角関数による加算y（速度に追加）
        f32 addZ;//三角関数による加算z（速度に追加）
        f32 addXPr;//三角関数による加算x（実際は速度速度に追加）
        f32 addYPr;//三角関数による加算y（速度に追加）
        f32 addZPr;//三角関数による加算z（速度に追加）
        s32 tagID;  //標的ID
        f32 tagposX;//標的位置x
        f32 tagposY;//標的位置y
        f32 tagposZ;//標的位置z
        f32 arrPosX;//標的に行かない時の到達地点座標X
        f32 arrPosY;//標的に行かない時の到達地点座標Y
        f32 arrPosZ;//標的に行かない時の到達地点座標Z
        BOOL arrPosAlr_f;//進入禁止内に既に入っている場合のフラグ
        BOOL arrPosX_f;//標的に行かない時のフラグX
        BOOL arrPosY_f;//標的に行かない時のフラグY
        BOOL arrPosZ_f;//標的に行かない時のフラグZ
        s32 dispBallX;//表示位置x
        s32 dispBallY;//表示位置y
        s32 dispBallZ;//表示位置z
        BOOL realBlHide_f;//本体ボールを隠すフラグ（ダブルシュートで使用）ONで隠す
        TShpSprt sprtDt[SPARATE_MAX];//位置と表示フラグセット（ひとまず10用意）
        TShpTf shpPhantomSort[PHANTOM_MAX];//山田　表示形状ソートデータ 　データ個数的にTShpTfを使っているけど本来のTShpTfとは無関係
        BOOL accelFunc_f;//２次関数フラグ…段階読み込んだときに２次関数があればONにする
        BOOL accelFuncMove_f;//２次関数作動中
        BOOL accelFuncInit_f;//２次関数初期化フラグ
        BOOL accelStop_f;//２次関数加速前停止フラグ
        BOOL decelRev_f;//減速逆戻り防止フラグ
        BOOL decelAct_f;//減速開始フラグ
        BOOL decelActInit_f;//減速開始時の初期化フラグ
        BOOL triFunc_f;//三角関数フラグ…段階読み込んだときに２次関数があればONにする
        BOOL triFuncInit_f;//三角関数初期化フラグ
        s32 changeMtd;//現段階変化方法（標的素材から読み取る）
        s32 changeMtdFromTarget;//現段階変化方法（標的素材から読み取る）
        BOOL waveX_f;//波Xフラグ
        BOOL waveY_f;//波Yフラグ
        BOOL waveZ_f;//波Zフラグ
        s32 wave_c;//波個数
        f32 spdDecFromWave;//速度ダウン率・・・波個数より
        f32 dirX;//方向X（SAngle再計算用データ）
        f32 dirY;//方向Y（SAngle再計算用データ）
        f32 dirZ;//方向Z（SAngle再計算用データ）
        f32 vecX;//単位ベクトルX（SAngle再計算用データ）
        f32 spdRtXY;//速度XY比率（SAngle再計算用データ）
        f32 spdRtXZ;//速度XZ比率（SAngle再計算用データ）
        f32 spdDiv;//速度分割（SAngle再計算用データ）
        f32 spdWave;//波の段階に入ったときのメイン軸から算出された速度
        BOOL spdWaveInit_f;//波の段階に入ったときのメイン軸から算出された速度をいれたかどうかのチェック
        BOOL landAfter_f;//着地後フラグ
        BOOL landAfterOnly_f;//着地後フラグ
        f32 spdMltAngle;//多角処理中の初期速度
        f32 dirMltAngle;//多角処理中の初期向き
        f32 initDirX;//最初の向き（標的－球位置から）
        f32 spdY_ChangeYOnly;//Yのみ変化時の速度Y

        //さいどかーぶだま判定
        BOOL sideCurveDt[sdj_END];
        BOOL sideCurve_f;//さいどかーぶ素材アリフラグ
        BOOL sideCurveWidSp_f;//さいどかーぶ素材アリフラグ

        s32 shpId;//shp[],shptf[] 共用のId
        s32 shpLv;//shp[],shptf[] 共用のLv
        s32 shpPulseStopId;//shp[],shptf[] 共用のId
        s32 shpPulseStopLv;//shp[],shptf[] 共用のLv
        s32 shpPulseMoveId;//shp[],shptf[] 共用のId
        s32 shpPulseMoveLv;//shp[],shptf[] 共用のLv
        s32 shpTfNo;//shptf[]
        s32 shpPhAdd;//全段階数以上の形状素材が入っていた時の追加段階
        BOOL shpVrPh_f;//仮想追加段階突入フラグ
        BOOL shpMtoS_f;//同段階内形状変化ありフラグ（Master：shp[]、Slave：shptf[]）
        BOOL shpSlave_f;//同段階内形状変化中フラグ（Master：shp[]、Slave：shptf[]）

        f32 spdTcPrX;//速度x（追いかけ効果中１Fr前速度）
        f32 spdTcPrY;//速度y（追いかけ効果中１Fr前速度）
        f32 spdTcPrZ;//速度z（追いかけ効果中１Fr前速度）
        f32 spdTcPrAll;//速度（追いかけ効果中１Fr前速度）

        //拍関連
        BOOL  pulse_Init_f; //115:拍効果＿開始時間※
        BOOL  pulseShape_Init_f; //115:拍効果＿開始時間※
        s32 pulse_c;//カウンタ
        BOOL pulse_Stop_f;//ストップ中フラグ
        s32 pulse_StopTime;//停止時間
        s32 pulse_MoveTime;//駆動時間
        BOOL pulseByWave_f;//波による拍フラグ
        BOOL pulseByWaveX_f;//波による拍フラグ
        BOOL pulseByWaveY_f;//波による拍フラグ
        BOOL pulseByWaveZ_f;//波による拍フラグ

        //波関連
        BOOL waveXDiv_f;//波分割中フラグ
        s32 waveXDiv_c;//分割中時間カウント
        s32 waveXDivTime;//分割中時間
        BOOL waveYDiv_f;//波分割中フラグ
        s32 waveYDivTime;//分割中時間
        s32 waveYDiv_c;//分割中時間カウント
        BOOL waveZDiv_f;//波分割中フラグ
        s32 waveZDivTime;//分割中時間
        s32 waveZDiv_c;//分割中時間カウント

        void Init() { Zeromem(this); }
    };
    //初期情報
    struct TInitData{
        TBallData iball;    //初期シュートデータ
        //※distを求める際 distX = tagposX - posX とし、絶対値は取らないでOKです。distY、Zも同様です。
        //s32 stPow;//ステータス：ちから
        //s32 stTeq;//ステータス：うまさ
        //s32 stAgl;//ステータス：はやさ
        //s32 stGut;//ステータス：がっつ
        s32 pow;//攻撃力
        s32 sharp;//キレ
        s32 rank;//ランク
        s32 side; //サイド（左チーム0 ,右サイド1）★teamをsideに変更
        s32 frmPos; //フォーメーションにおけるポジション・・・０～３：内野、４：奥外野、５：手前外野、６：対面外野
        s32 spin;//スピン方向＆スピン速度★
        BOOL sp_f;//必殺シュートフラグ★
        BOOL nice_f;//ナイスシュート★
        BOOL counter_f;//カウンターシュート★
        BOOL jsh_f;//ジャンプシュート★
        BOOL dsh_f;//ダッシュシュート★
        BOOL roll_f;//ごろごろフラグ★
        s32  shotId;//シュートID
        BOOL initAccTri_f;//加速・三角関数の初期化フラグ
        BOOL initZShot_f;//Ｚシュートフラグ…初期の標的＆ボール位置で決定→波の方向とかで使う
        TShp shp[ELMSHPNUM];//形状データ
        TShpTf shptf[ELMSHPNUM];//段階変形用形状データ
        s32 shpNo;//形状が入る番号⇒shp[shpNo]
        TShpSprtVar sprtDt[ssl_NUMMAX];//ぶんれつ系（enShapeSeparateLevelを使用）
        s32 sprtQuickLv;//高速化
        BOOL sprtQuickLvGave_f;//高速化であがったLvを１度渡したかどうかのフラグ
        s32 sprtMain;//メイン分裂・・・指定があれば高速化で使用
        BOOL sprt_f;//ぶんれつ系フラグ
        TEff eff;//効果データ

        void Init() { Zeromem(this); }
    };

    struct TCommonEffectFlagData{//効果素材のフラグ等…どの段階からでも確認できるようCommonに入れる
        BOOL incld_f;//本シュートに該当効果素材があるかのチェック
        BOOL phase_f;//指定段階と現段階が一致しているかチェック
        s32  times_c;//同効果素材＆段階違いがある場合⇒同効果素材の発動回数＝配列の要素数
        void Init() { Zeromem(this); }
    };

    struct TCommonEffectData{//効果素材のフラグ等…どの段階からでも確認できるようCommonに入れる
        //初段階から発動する効果素材のフラグ…吹っ飛びなどBall以外で判断が必要な奴のフラグ
        //掛カウンタは減算で、０以下で無効
        BOOL penet_f;     //1:貫き効果
        s32  penet_c;     //貫通カウント　0で貫通しない 
        BOOL peneStick_f;//くっつき効果
        s32  peneStickTime_c; //くっつき時間カウント　０でリリース＆↑０でもリリースorくっつき非発動
        BOOL suction_f;  //12:吸引効果
        s32  suctionLv;//12:吸引Lv
        BOOL flyReverse_f;//7:引っ張り効果
        s32  flyReverseLv;//引っ張りLv
        BOOL flyLoose_f;  //11:吹っ飛び緩効果
        s32  flyLooseLv;//11:吹っ飛び緩Lv
        BOOL flyVer_f;  //11:垂直吹っ飛び効果
        s32  flyVerLv;//11:垂直吹っ飛びLv
        BOOL flyHor_f;  //11:横吹っ飛び効果
        s32  flyHorLv;//11:横吹っ飛びLv
        BOOL rndWorld_f;  //12:世界一周効果
        s32  rndWorldLv;//12:世界一周Lv
        BOOL refHor_f;  //12:横跳ね返り吹っ飛び
        s32  refHorLv;//12:横跳ね返り吹っ飛びLv
        BOOL refVer_f;  //12:縦跳ね返り吹っ飛び
        s32  refVerLv;//12:縦跳ね返り吹っ飛びLv
        BOOL noMvDwn_f;  //12:その場ダウン
        s32  noMvDwnLv;//12:その場ダウンLv
        BOOL combo_f;//コンボフラグ
        s32  combo_c;//コンボ回数　０でコンボしない（他キャラによる貫通力減少が起こるので貫通力と別で持つ）
        s32  comboNoHitTime_c;//コンボ中ヒットフラグ発生までの時間→０より大きい場合は無敵＆０でヒットOK
        s32  comboDam;//コンボダメージ（０なら固定ダメージなし）
        BOOL noDmg_f;//ダメージナシフラグ
        BOOL phantom_f;//ダメージナシフラグ
        BOOL phantomEnd_f;//ダメージナシフラグ

        BOOL catchSlip_f;  //キャッチスリップ
        s32  catchSlipLv;//キャッチスリップ

        BOOL catchSlipR_f;  //キャッチスリップ
        s32  catchSlipRLv;//キャッチスリップ


        BOOL symCtrl_f;  //マリオネット効果

        BOOL flyFire_f;   //8:吹っ飛び火効果
        BOOL flyElec_f;   //9:吹っ飛び電効果
        BOOL rollDown_f;  //10:ゴロゴロダウン効果
        BOOL pass_f; //15:パス挙動効果
        BOOL finalSE_f; //15:パス挙動効果
        BOOL waveKeep_f; //15:波イジ効果

        //段階指定のある効果素材のフラグ群
        //以下３種のデータを持つ構造体
        //BOOL incld_f;//本シュートに該当効果素材があるかのチェック
        //BOOL phase_f;//指定段階と現段階が一致しているかチェック
        //s32  times_c;//同効果素材＆段階違いがある場合⇒同効果素材の発動回数＝配列の要素数
        TCommonEffectFlagData accel;  //101:加速効果
        TCommonEffectFlagData decel;  //102:減速効果
        TCommonEffectFlagData yAccel; //103:y加速効果
        TCommonEffectFlagData spdChg; //104:速度可変効果
        TCommonEffectFlagData trace;  //105:追跡効果
        TCommonEffectFlagData reflect;//106:強反射効果
        TCommonEffectFlagData mySide; //107:味方被弾効果
        TCommonEffectFlagData normal; //108:通常球化効果
        TCommonEffectFlagData flyVer; //109:真上吹っ飛び効果
        TCommonEffectFlagData flyHor; //110:真横吹っ飛び効果
        TCommonEffectFlagData flyHorPhs; //110:真横吹っ飛び効果
        TCommonEffectFlagData press;  //111:圧殺効果
        TCommonEffectFlagData copy;   //112:複写効果
        TCommonEffectFlagData noMvDwn;//113:その場ダウン効果
        TCommonEffectFlagData combo;  //114:同一標的連続被弾効果
        TCommonEffectFlagData pulse;  //115:拍効果
        TCommonEffectFlagData sePhase; //116:効果音段階変化時
        TCommonEffectFlagData seHit;   //117:効果音被弾時
        TCommonEffectFlagData orbPaint;//118:軌跡描画効果
        TCommonEffectFlagData refVer; //109:縦跳吹っ飛び効果
        TCommonEffectFlagData refHor; //110:横跳吹っ飛び効果
        TCommonEffectFlagData wEdge; //125:もろは効果
        TCommonEffectFlagData tagChange; //127:標的変化効果
        TCommonEffectFlagData spdMomUp; //128:一時速度上昇効果
        TCommonEffectFlagData spdMomDown; //129:一時速度減少効果
        TCommonEffectFlagData noDmg; //130:ダメージナシ効果
        TCommonEffectFlagData sprtOff; //126:分裂オフ効果
        TCommonEffectFlagData phantom; //126:分裂オフ効果



        void Init() { Zeromem(this); }
    };

    struct TCommonData{//全段階共有データ
        //全段階で可変のデータ（phaseAllは初段階のみ更新）
        s32  timeTotal_c; //トータル時間カウント
        s32  timePhase_c; //現段階内時間カウント
        BOOL hit_f;        //ヒットフラグ
        s32  hit_c;        //ヒット人数 カウント
        s32  loop_c;       //ループ回数カウント ※段階を跨いで使用するためココに移動
        s32  phaseAll;     //全段階数
        s32  phaseCrt;     //現段階数
        BOOL groundElm_f;//どこかに地上変化素材があるかのチェック
        BOOL phaseChange_f;//段階変わったフラグ
        s32  hit_SameChr_c;//連続ヒット効果中の同一キャラヒット回数
        s32  comboTime_c;  //連続ヒット効果のコンボ可能フラグ解放時間カウント
        s32  pulseTime_c;  //拍効果＿時間カウント
        s32  spin_c;//回転カウンタ★
        f32  spdPrX;//前段階速度x（phsに入れると段階移行でクリアされるのでここで定義）
        f32  spdPrY;//前段階速度y（phsに入れると段階移行でクリアされるのでここで定義）
        f32  spdPrZ;//前段階速度z（phsに入れると段階移行でクリアされるのでここで定義）
        s32  changeMtdPr;//前段階変化方法（段階移行時読み取る）（phsに入れると段階移行でクリアされるのでここで定義）
        BOOL targetY_f;//標的ダウンフラグ…コレが立っていたらそいつにYも合わせて投げるがそれ以外は自分より高い位置を狙わない
        BOOL targetYZero_f;//標的０フラグ…おいかけ効果でコレが立っていたら標的の足元（＝Y座標点）を狙う。(しゃがみ・ダウン中)
        BOOL targetDead_f;//標的死亡フラグ…おいかけ効果で天使状態ならスルーさせる
        TShpSprt pastShpDt[PAST_DATA];//過去の形状関連データ：座標や形状番号を記録
        BOOL reSAngle_f;//徐々に処理を行ったあと再度SAngleを行った事を示すフラグ→放物線系も再度イニシャライズを
        BOOL addTriData_f;//段階変更時に三角関数加算値をposに入れるフラグ
        BOOL reTriInitData_f;//段階変更時に三角関数加算値をposに入れるフラグ
        f32 addDirX;//SAngle後X波加算方向を指定・・・SAngle付近でフラグを立て、GlobalMoveでチェック
        f32 addDirY;//SAngle後Y波加算方向を指定・・・SAngle付近でフラグを立て、GlobalMoveでチェック
        f32 addDirZ;//SAngle後Z波加算方向を指定・・・SAngle付近でフラグを立て、GlobalMoveでチェック
        BOOL addDir_f;//↑更新フラグ
        BOOL palabLinePeak_f;//放物直線頂点超えフラグ
        f32  palabLinePeakSpdY;//放物直線頂点超え時のY速度
        f32  palabLinePeakPosY;//放物直線頂点超え判別Y位置
        BOOL wEdgeTgChange_f;//もろは効果による標的チェンジフラグ
        BOOL gncdTgChange_f;//敵標的チェンジ（全員）効果による標的チェンジフラグ
        BOOL ballAprAlr_f;//既に一度ボールを表したかフラグ　BallAppearAlready　複数段階でボールを消したままにしたくないので
        BOOL wEdgeTgChangeRsv_f;//もろは効果による標的チェンジ予約フラグ
        BOOL gncdTgChangeRsv_f;//敵標的チェンジ（全員）効果による標的チェンジ予約フラグ
        //･･･上記２種は効果素材確認→次段階移行時の素材チェック→発動となるが
        //次段階移行時の素材チェックで上書きされるのでその前に上記フラグに情報を移す必要がある
        BOOL tagFly_f;//標的の吹っ飛びフラグ
        BOOL traceAway_f;//標的の吹っ飛びフラグ

        BOOL thruDist_f;//距離変化素材で敵を超えたフラグ
        s32 pstPX;//１Fr前の位置X
        s32 pstPY;//１Fr前の位置Y
        s32 pstPZ;//１Fr前の位置Z
        s32 pstSX;//１Fr前の速度X
        s32 pstSY;//１Fr前の速度Y
        s32 pstSZ;//１Fr前の速度Z
        s32 pstTX;//１Fr前の敵位置X
        s32 pstTY;//１Fr前の敵位置Y
        s32 pstTZ;//１Fr前の敵位置Z
        s32 prsPX;//現在の位置X
        s32 prsPY;//現在の位置Y
        s32 prsPZ;//現在の位置Z
        s32 prsSX;//現在の速度X
        s32 prsSY;//現在の速度Y
        s32 prsSZ;//現在の速度Z
        s32 prsTX;//現在の敵位置X
        s32 prsTY;//現在の敵位置Y
        s32 prsTZ;//現在の敵位置Z
        s32 objPX;//ステージ表示物指定の位置X
        s32 objPY;//ステージ表示物指定の位置Y
        s32 objPZ;//ステージ表示物指定の位置Z
        s32 realBallPX;//現在の本体ボール位置X　やまだ分裂用
        s32 realBallPY;//現在の本体ボール位置Y　やまだ分裂用
        s32 realBallPZ;//現在の本体ボール位置Z　やまだ分裂用
        BOOL courtDirRevX_f;//コートＸ位置によるマイナス方向進行フラグ２・３外野用
        BOOL courtDirRevZ_f;//コートＺ位置によるマイナス方向進行フラグ２・３外野以外用
        BOOL courtNearCenterZ_f;//コート中央付近（Ｚ）フラグ→TREU で さいどかーぶだまならWaveLenLvを下げる

        enSEID poolSeNoForChanging;//段階移行時に止まったり緩やかに変化するときに鳴らす番号を保存
        BOOL poolSeNoForChanging_f;//段階移行時に止まったり緩やかに変化するときに鳴らす番号を保存

        //外野シュート判別用変数
        BOOL shotFromNorthOF_f;//北側外野ポジションシュートフラグ
        BOOL shotFromSouthOF_f;//南側外野ポジションシュートフラグ
        BOOL shotNSOF_f;//上記２フラグどちらかが立っていたら立てるフラグ
        BOOL shotNSOFZ_f;//外野＆Zシュートフラグ
        BOOL shotNSOFPhaseChange_f;//外野シュート特別段階移行フラグ

        BOOL stopPstData_f;//１Fr前データ取得を止めるフラグ
        BOOL ceelingHit_f;//天井ヒットフラグ→敵に向かうor真下落下、段階以降処理はこれ以降しない
        s32 sAngle_c;//SAngle回数
        //コンボ時の状態データ（未ヒット、１ヒット、複数ヒット）
        s32 comboState[2][7];//諸刃系チーム数・キャラ数
        BOOL comboGncdHit[4];//全員系ヒット状態　FALSE優先
        BOOL comboGncdDeath[4];//全員系状態　FALSE候補に
        f32 comboGncdDist[4];//全員系距離　最寄優先
        //最大速度座標軸
        enMaxSpdCoordinateAxis maxSpdAx;
        f32 defSpd;//デフォのSpd・・・最初にsettingDtから読み込んじゃう

        BOOL shotFinish_f;//シュート終了→ルーズボールへ
        BOOL shotFinishFall_f;//シュート垂直落下→着地→ルーズボールへ
        f32 shotFinishFallSpdY;//シュート垂直落下速度

        TCommonEffectData efEl;//効果素材フラグ等
        BOOL shotEff_f[sef_NUMMAX];
        BOOL hShot_f;//必殺シュートフラグ…通常シュートのアニメと別扱いなため

        s32 mltAngle;//多角処理の数…多角終了の変化素材の個数から算出
        BOOL mltAngle_f;//多角処理終了フラグ
        s32 mltAnglePhs;//多角処理中の段階
        BOOL mltAngleSetSpd_f;//速度設定フラグ
        f32 mltAngleInitXDir;//多角処理の数…多角終了の変化素材の個数から算出

        //形状データ
        //保存用データ
        s32 spdtShpNum;//現在の番号…形状配列の要素数
        s32 spdtShpPhs;//現在の番号…形状配列の要素数（段階指定用）
        s32 spdtShpMode;//現在の形状モード（普通・例外１・例外２）
        s32 spdtShpModeChange_f;//現在の形状モードチェンジフラグ
        s32 spdtPtnStep;//現在のパターン段階
        s32 spdtFrame_c;//アニメカウンタ　パターン段階切り替わるごとに０にする
        s32 spdtLoop_c;//アニメループカウンタ　同段階変形がある時用に


        //形状番号（PK）⇒形状素材番号
        s32 spdtShpElmId;
        //現形状段階番号（cmn.spdtShpNum）に則した形状番号
        s32 spdtShpid;
        s32 spdtShpLv;
        //状態（cmn.spdtShpMode＋1）を参照してセット番号を取得
        s32 spdtAnmDataId;
        BOOL spdtAnmDataIdSp_f;
        //アニメ番号からパターン番号を取得
        s32 spdtAnmPattId;
        //指定のパターン番号から、使用する絵の番号1～4と反転の有無チェック
        s32 spdtAnmUseNum;
        s32 spdtAnmUseRevNum;

        //外部読み込み用データ
        s32 spdtNrmBallNum;
        s32 spdtNrmBall_c;
        s32  spdtGrhNum;//画像番号
        s32  spdtGrhPrId;//前回の画像番号
        BOOL spdtGrhPrId_f;//既に変わっているときは変える必要ないフラグ
        s32  spdtGrhPrLv;//前回の画像Lv
        BOOL spdtRevVer_f;//垂直反転フラグ
        BOOL spdtRevHor_f;//左右反転フラグ（アニメ内左右反転と方向による左右反転をかけあわせた結果）
        BOOL spdtRevClr_f;//色反転フラグ
        BOOL spdtVnsBal_f;//ボール消しフラグ
        BOOL spdtVnsSdw_f;//影消しフラグ

        void Init() { Zeromem(this); }
    };
    const s32 ELM_LINE = 64;
    const s32 ELM_MAX = 20;
    struct TPhaseElmData{
        s32 elms[ELM_LINE][ELM_MAX];//段階ごとの素材
        s32 linenum;
        s32 elmnum[ELM_LINE];
        void Init() { Zeromem(this); }
    };
    struct TSortData{
    public:
        //INTVECTOR2 nonesort_elms;//合成回そのままの素材（段階分けもソートもしてない）
        TPhaseElmData phase_elms;//段階ごとの素材
        s32 prt_init;
        s32 prt_term;
        s32 phaseAll;
        s32 mltAngle;
        BOOL groundElm_f;
        void Init() { Zeromem(this); }
        //cppファイルで定義
        void AddPhase();
        void SortElem(INTVECTOR2 nonesort_elms);
        void CheckGroundElm(s32 elmNo);
    };
    struct TStShot{//各段階共通データ
    protected:
        //RemoveElmToCrt();
        void RemoveChangeElmToCrt();
        void RemoveWaveXElmToCrt();
        void RemoveWaveYElmToCrt();
        void RemoveWaveZElmToCrt();
        void RemoveWaveElmToCrt(s32 elmWvType, s32 elmWvWd, s32 elmWvFq,
            s32 elmWvWCMt, s32 elmWvWCSp, s32 elmWvPS, s32 elmWvDir, s32 elmWvAbs,
            s32& crtWvType, s32& crtWvWd, s32& crtWvFq,
            s32& crtWvWCMt, s32& crtWvWCSp, s32& crtWvPS, s32& crtWvDir, s32& crtWvAbs,
            BOOL* wvType, s32* wvTypesort, s32& wvTypeMax);
        void RemoveOrbitElmToCrt();
        void RemoveTargetElmToCrt();
        void SideCurveCheck(s32 elmNo);
//        void SetWvTypeRev(s32& wvtype, BOOL& wvRv_f, s32& wvtypeMem);
        void SetWvTypeRev(s32& wvtype, BOOL* wvRv_f);

        void SAngleSoreTateAcute();
        void SAngleSoreTateObtuse90();
        void SAngleSoreTateObtuse135();
        void SAngleSoreTateObtuse180();
        void SAngleSoreSideAcute();
        void SAngleSoreSideObtuse90();
        void SAngleSoreSideObtuse135();
        void SAngleSoreSideObtuse180();
        void SoreTatePoint();
        void SoreSidePoint();
        void SoreTateSideRate();
        void XYZSpeedDiv();
        void SetSpeedXYZ();
        void SetSpeedYMain();
        void SetSpeedZMain();
        void SetSpeedYZ();

        void SetRatioYZIndepOfX(f32 distY,f32 distZ);
        f32 GetRatioFromAngle(BOOL yCheck, f32 distA, f32 distB, s32 soreDir, BOOL& ign_f);
        f32 GetRatioForSideLine(BOOL dpCheck, f32 distY, BOOL& zzero_f);
        void GetSpeedXYZ();
        void SetSpdBackward();
        void SetSpdFromObjPos();
        void SetSpdOFSNShoot();
        void SetSpdXYZ();
        void SetSpdYMain();
        void SetSpdZMain();
        void SetSpdYZ();
        void SetSpdZeroToX();
        void SetArrivalPosFromSoreDir(f32& aposX, f32 & aposY, f32& aposZ);
        void SetSpdToSideline(f32 distY, f32 dirZ, f32& arrPosZ);
        f32 GetDirFromPosSore(f32 dist, s32 soreDt, BOOL enmIgn_f);

        void SetSAngleData(f32 dirX, f32 dirY, f32 dirZ, f32 vecX, f32 spdRtXY, f32 spdRtXZ, f32 spdDiv);
        void SetNormalSlaveShapeId();
        void SetExcessShapeId();
        void SetSeparateSinFq(enShapeSeparateLevel spType);
        void SetSeparateSinFq(enShapeSeparateLevel spType, BOOL& init_f, s32 lv, s32 waveAngle);
        void SetSeparateMulFq(enShapeSeparateLevel spType);
        void SetSeparateMulFq(enShapeSeparateLevel spType, BOOL& init_f, s32 lv, s32 waveAngle);
        void SetSeparateShadow(enShapeSeparateLevel spType, BOOL& init_f, s32 lv);
        void SetSeparateCircle(enShapeSeparateLevel spType);
        void SetSeparateCircle(enShapeSeparateLevel spType, BOOL& init_f, s32 lv, s32 waveAngle);
        void SetSeparateCircle8(enShapeSeparateLevel spType);
        void SetSeparateCircle8(enShapeSeparateLevel spType, BOOL& init_f, s32 lv, s32 waveAngle);
        void SetSeparateAddData(enShapeSeparateLevel spType, s32 lv, f32 addPDt[], f32 addQDt[]);
        void InitSeparateData(enShapeSeparateLevel spType);
        void SetExternalEffData();


    public:
        TInitData idata; //初期情報
        TCommonData cmn;//全段階共通データ
        TPhaseElements elm;// 素材データ用の構造体
        TPhaseElements crt;// 実際の現データ用の構造体
        TBallData phs;//各段階共通データ★変化のタイミングで書き換え
        TSortData sdata;

        TShotTriData tridt;
        TShotAccData accdt;

        //主軌道計算用データ
        TShotAngleData agldt;

        void Init()
        {
            //sdata.Init();
            //idata.Init(); //初期情報
            //cmn.Init();//全段階共通データ
            //elm.Init();// 素材データ用の構造体
            //crt.Init();// 実際の現データ用の構造体
            //phs.Init();//各段階共通データ★変化のタイミングで書き換え
            Zeromem(this); //VECTORをゼロクリアするとメモリリークする
        }

        //形状と効果の抽出
        void SortElem_ShpEff(TDtElement* pdtElem);

        //現段階のsdataからElmに移動
        void RemoveSdataToElm(TDtElement* pdtElem);

        //変化方法設定
        void SetChangeMtd(s32 mtd);
        //標的素材Elm⇒Crt
        void RemoveElmToCrt();
        //初段階の効果素材フラグセット
        void SetEffectElmFlag();
        //効果素材フラグの段階チェック
        void CheckEffectElmFlag();
        //段階で発動する効果音の番号取得
        enSEID GetSENum(s32 id);
        //形状素材のデータセット（phs.spdXを見ているのでSAngle等のあとに処理を入れること）
        void InitShapeIdLv();
        void SetAnotherShapeId();
        void SetSlaveShapeData();
        void InitShapeData(TDtBallAnime* pdtBlAm, TDtElement* pdtElem);
        void CheckShapeMode(TDtBallAnime* pdtBlAm, TDtElement* pdtElem);
        BOOL CheckShapeModeType(s32 mType);
        void SetShapeValFromWave(TDtBallAnime* pdtBlAm);
        s32 GetShapeWaveKoma(TDtBallAnime* pdtBlAm, f32 triFuncVal);
        void CheckShapeData(TDtBallAnime* pdtBlAm);
        void SetShapeData(TDtBallAnime* pdtBlAm);
        BOOL CheckWaveVec(BOOL ud);
        BOOL CheckWaveVec(f32 add, f32 addPr, BOOL ud);
        void CheckZDir();
        //主軌道計算
        void SAngle();
        //ぶんれつ系
        void SetSeparateBall();

        //山田
        void SetPhantomData(TDtBallAnime* pdtBlAm, TDtElement* pdtElem);
        void SetPhantomDataInit(TDtBallAnime* pdtBlAm, TDtElement* pdtElem);
        void SetPhantomDataStart(TDtBallAnime* pdtBlAm, TDtElement* pdtElem);
        void SetPhantomDataWait(TDtBallAnime* pdtBlAm, TDtElement* pdtElem);
        void SetPhantomDataTerm(TDtBallAnime* pdtBlAm, TDtElement* pdtElem);
        void SetPhantomBallsData(s32 i, TDtBallAnime* pdtBlAm, TDtElement* pdtElem);
        void SortPhantomBallsData(TDtBallAnime* pdtBlAm, TDtElement* pdtElem);

        //過去データ整理
        void SetPastData(s32 posX, s32 posY, s32 posZ);
        void RemovePastData(s32 no);
        //フラグの確認
        BOOL IsShotEffFlag(enShotEffFlags shotEff);

        //効果素材系関数
        void SetStopOrMoveByPulseEff();//拍効果
        void ResetSpdByTraceEff();//トレース効果による速度再設定
        void SetSpdByMltAngle();//多角処理速度セット
        void SetMltAngleInit();//多角処理開始
        void SetMltAngleFrst();//多角処理計時段階
        void SetMltAngleMain();//多角処理メイン
        void CheckMltAngleInit();//多角処理開始
        void CheckMltAngleFrst();//多角処理計時段階
        void CheckMltAngleMain();//多角処理メイン
        BOOL CheckBeyond3Pos(f32 posIX, f32 posIY, f32 posIZ,
            f32 posTX, f32 posTY, f32 posTZ,
            f32 spdX, f32 spdY, f32 spdZ);//指定点超えチェック
        BOOL CheckBeyondPos(f32 posI, f32 posT, f32 spd);


    };


}
