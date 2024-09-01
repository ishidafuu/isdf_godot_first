namespace db;

//Grv,//
public enum endtSet_Grv
{
    setGRV_Y, //重力Y軸重力
    setFlyGrv, //重力（ふっとび）
    setFlyGrvLoose, //重力（ゆるふっとび）
    setPaGrv, //16,パス重力
    setPaGrv14, //12,１４間パス重力
    setBoundGrv, //重力（バウンド）
    setgrEND,
};

//Damage,//
public enum endtSet_Damage
{
    setHP, //HP
    setDeadNum, //昇天数x10
    setCatchDamPer, //キャッチダメージ率
    setdmEND,
};

//Jump,//
public enum endtSet_Jump
{
    setJump_vY0, //JpY初速（通常）
    setDashJump_vY0, //JpY初速（ダッシュ）
    setJumpCanselTime, //ジャンプキャンセル時間(f)
    setJumpLandTime, //ジャンプ着地時間(f)
    setjpEND,
};

//Court,//
public enum endtSet_Court
{
    setDashX, //ダッシュ速度Ｘ
    setDashZ, //ダッシュ速度Ｚ
    setDashJpX, //ダッシュジャンプ速度Ｘ
    setWkZPer, //奥行き移動速度％
    setNWkXPer, //斜めＸ移動速度
    setNWkZPer, //斜めＺ移動速度
    setDsStopFric, //摩擦係数
    setDownBrkIce, //氷床ダウンブレーキ係数
    setDsLossPer, //足取られ
    setRollSpeed, //転がり速度
    setctEND,
};

//Ball,//
public enum endtSet_Ball
{
    setBoundDefPer, //バウンド
    setBoundAbsdX, //バウンド速度減衰率Ｘ(%)
    setBoundAbsdY, //バウンド速度減衰量Ｙ(%)
    setBoundAbsdZ, //バウンド速度減衰率Ｚ(%)
    setBoundStopdY, //これ以下の落下速度の場合バウンドしない
    setBoundMaxdY, //バウンド最高Ｙ軸速度
    setBoundMindYpdX, //バウンド最低dY/dX(%)これ以上鋭角のバウンドをしない
    setAtariWidht, //当たり判定Ｗ
    setAtariHeight, //当たり判定Ｈ
    setAtariDepth, //当たり判定Ｄ
    setZAtariWidht, //Ｚ方向シュート当たり判定Ｗ
    setZAtariHeight, //Ｚ方向シュート当たり判定Ｈ
    setZAtariDepth, //Ｚ方向シュート当たり判定Ｄ
    setFAtariWidht2, //停止ボール当たり判定Ｗ
    setFAtariHeight2, //停止ボール当たり判定Ｈ
    setFAtariDepth2, //停止ボール当たり判定Ｄ
    setBaXWallAbsdX, //左右壁減速率X
    setBaXWallAbsdZ, //左右壁減速率X
    setBaZWallAbsdX, //左右壁減速率X
    setBaZWallAbsdZ, //左右壁減速率X
    setblEND,
};

//Wall,//
public enum endtSet_Wall
{
    setXWallAbsdX, //左右壁減速率X
    setXWallAbsdZ, //左右壁減速率X
    setZWallAbsdX, //左右壁減速率X
    setZWallAbsdZ, //左右壁減速率X
    setWallBounddY, //壁ぶつかり時跳ね上がり
    setwlEND,
};

//Infield,//
public enum endtSet_Infield
{
    setCrYokeJudgeWidth, //しゃがみ避け判断判定Ｘ幅
    setCrYokeJudgeDepth, //しゃがみ避け判断判定Ｚ幅
    setJpYokeJudgeWidth, //ジャンプ避け判断判定Ｘ幅
    setJpYokeJudgeDepth, //ジャンプ避け判断判定Ｚ幅
    setShCvJpJudgeWidth, //シュートカバージャンプ判断判定Ｘ幅
    setShCvJpJudgeDepth, //シュートカバージャンプ判断判定Ｚ幅
    setEnCourtCrTime, //敵コート避け時間限界(f)
    setNoJpHeight, //避けない高さ
    setifEND,
};

//Catch,//
public enum endtSet_Catch
{
    setCatchWait, //総キャッチ時間
    setCaMotionFrm, //キャッチ時間
    setNiceCatchTime, //ナイスキャッチ時間
    setDsCatchRev, //ダッシュキャッチのキャッチ時間マイナス
    setcaEND,
};

//Shot,//
public enum endtSet_Shot
{
    setRollPow, //ごろごろ閾値
    setHissatsuShotSpd, //必殺シュートデフォ速度
    setNormalShotSpd, //普通シュートデフォ速度
    setPierceDown, //貫通威力減衰
    setComboDamDownPer, //同一キャラヒット時の威力減衰
    setStandShotRate, //立ち投げＢＰ→攻撃値倍率
    setJumpShotRate, //ジャンプＢＰ→攻撃値倍率
    setDashShotRate, //ダッシュＢＰ→攻撃値倍率
    setDashJumpShotRate, //ダッシュジャンプＢＰ→攻撃値倍率
    setHitStartTime, //シュートしてから当たり判定が出るまでの時間(f)
    setFlyTagHeight, //吹っ飛びでもタゲになる高さ
    setshEND,
};

//Pass,//
public enum endtSet_Pass
{
    setPaSpdAdd14, //50,１４間パス速度追加
    setDMPaSpd, //ダッシュマンパス速度
    setNearDistDef, //近距離判定距離
    setQuickPassTagY, //クイックパスタゲ高さ
    setTossPassTagY, //トスパスタゲ高さ
    setDsmnTossCatchTime, //ダッシュマントスキャッチタイム
    setAlleyoopTime, //アリウープ最低時間
    setPaHitStartTime, //パスしてから当たり判定が出るまでの時間(f)
    setPaStWait, //パス開始までのフレーム
    setpaEND,
};

//Formation,//
public enum endtSet_Formation
{
    setAtcLineX, //弱攻撃ライン（ダッシュシュートリリース位置、ダッシュマンがジャンプする位置など）
    setAtcLineX2, //中攻撃ライン
    setAtcLineX3, //強攻撃ライン
    setfmEND,
};

//COM,//
public enum endtSet_COM
{
    setOpeRev, //作戦設定による補正
    setNaiyaSteel, //強奪範囲
    setcmEND,
};

//Input,//
public enum endtSet_Input
{
    setBackDashX, //バックダッシュになる位置
    setAutoJumpX, //オーバーライン防止ジャンプ位置
    setSemiCaLen, //セミオートキャッチ長さ
    setSemiCaBase, //セミオートキャッチ中点
    setSemiCaNice, //セミオートキャッチナイス
    setinEND,
};