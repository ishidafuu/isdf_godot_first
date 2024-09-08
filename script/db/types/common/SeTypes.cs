namespace db;

public enum SeType
{
    None,
    Shot,
    NShot,
    HShot,
    CatchLv0,
    CatchLv1,
    CatchLv2,
    CatchLv3,
    CatchLv4,
    CatchLv5,
    CatchLv6,
    Jump,
    Dash,
    Bound,
    Take,
    Catch,
    SlipLv1,
    SlipLv2,
    SlipLv3,
    NiceCatch,
    HitLv0,
    HitLv1,
    DownLv0,
    DownLv1,
    DownLv2,
    DownLv3,
    Roll,
    Dead,
    DmOK,
    DmNG,
    CountDown,
}

public enum SystemSeType
{
    None,
    Cursol,
    Pause,
    NAME,
    NAME2,
    JudgeLong,
    JudgeShort,
    JudgeThrow,
    Shutter,
    Unlock,
    Copy,
    GachaFall, //sys10 筐体落下
    GachaBound, //sys11 筐体バウンド
    Punch, //sys12 パンチ音
    PunchHit, //sys13 物語ヒット音
    Toudou, //sys14 ビックリマーク音（藤堂の歯）
    Kime, //sys15 決め攻撃
    Tame, //sys16 ため音
    Yamada, //sys17 山田の術＿持ち上げ
    Yamada2, //sys18 山田の術＿武器とばし
    BigBung, //sys19 ビッグバング
    CupBound, //sys20 巨大カプセルバウンド
    NaBoxPanel, //sys21 パネルランダム点灯
    NaBoxDecide, //sys22 パネル決定
    Kansei, //sys23 歓声
    Kuchibue, //sys24 口笛
    NaBoxOtona, //sys25 おとながい
    Hissatsu, //sys26 必殺音
    Cancel, //sys27 キャンセル音／FCドッジキャッチ音　高
    MsgLo, //sys28 店員メッセージ音　低／FC物語　声低
    MsgMid, //sys29 店員メッセージ音　中／FC物語　声中
    MsgHi, //sys30 店員メッセージ音　高／FC物語　声高
    OK, //sys31 決定音／FC運動会
    Henshin, //sys32 変身技
    Nitro, //sys33 シュート外し／時代劇ニトロアタック
}