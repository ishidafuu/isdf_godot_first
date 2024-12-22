#pragma once
#include "dbInclude.h"

//グローバル扱いな変数
namespace db
{
  enum enSEID
  {
    seShot,
    seNShot,
    seHShot,
    seCatchLv0,
    seCatchLv1,
    seCatchLv2,
    seCatchLv3,
    seCatchLv4,
    seCatchLv5,
    seCatchLv6,
    seJump,
    seDash,
    seBound,
    seTake,
    seCatch,
    seSlipLv1,
    seSlipLv2,
    seSlipLv3,
    seNiceCatch,
    seHitLv0,
    seHitLv1,
    seDownLv0,
    seDownLv1,
    seDownLv2,
    seDownLv3,
    seRoll,
    seDead,
    seDmOK,
    seDmNG,
    seCountDown,
    se_END,
  };

  enum enSYSID
  {
    sysCursol,
    sysPause,
    sysNAME,
    sysNAME2,
    sysJudgeLong,
    sysJudgeShort,
    sysJudgeThrow,
    sysShutter,
    sysUnlock,
    sysCopy,
    sysGachaFall,//sys10 筐体落下
    sysGachaBound,//sys11 筐体バウンド
    sysPunch,//sys12 パンチ音
    sysPunchHit,//sys13 物語ヒット音
    sysToudou,//sys14 ビックリマーク音（藤堂の歯）
    sysKime,//sys15 決め攻撃
    sysTame,//sys16 ため音
    sysYamada,//sys17 山田の術＿持ち上げ
    sysYamada2,//sys18 山田の術＿武器とばし
    sysBigBung,//sys19 ビッグバング
    sysCupBound,//sys20 巨大カプセルバウンド
    sysNaBoxPanel,//sys21 パネルランダム点灯
    sysNaBoxDecide,//sys22 パネル決定
    sysKansei,//sys23 歓声
    sysKuchibue,//sys24 口笛
    sysNaBoxOtona,//sys25 おとながい
    sysHissatsu,//sys26 必殺音
    sysCancel,//sys27 キャンセル音／FCドッジキャッチ音　高
    sysMsgLo,//sys28 店員メッセージ音　低／FC物語　声低
    sysMsgMid,//sys29 店員メッセージ音　中／FC物語　声中
    sysMsgHi,//sys30 店員メッセージ音　高／FC物語　声高
    sysOK,//sys31 決定音／FC運動会
    sysHenshin,//sys32 変身技
    sysNitro,//sys33 シュート外し／時代劇ニトロアタック
    sys_END,

  };

}
