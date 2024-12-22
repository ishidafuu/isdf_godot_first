#pragma once
#include "dbInclude.h"


namespace db
{
  enum enDtShElm
  {
    sz_shoot_mid,
    //sz_shot_mol_name,
    sz_para_type_pow,
    sz_para_type_spn,
    sz_syn_init,//合成開始合成開、合成終＝合成１回目、合成２回目のときのみ使う情報、３回目以降はすべて使う 0でも全て使う
    sz_syn_term,//合成終了
    sz_prt_init,//保護開始保護＝ベースにしたときに保護された部分には入らない重なったら保護されてない次の段階にシフト　0は保護しない
    sz_prt_term,//保護終了
    sz_lock_rare1,//レア度
    sz_lock_rare2,
    sz_lock_rare3,
    sz_lock_rare4,
    sz_lock_rare5,
    sz_lock_rare6,
    sz_elem1,//素材
    sz_elem2,
    sz_elem3,
    sz_elem4,
    sz_elem5,
    sz_elem6,
    sz_elem7,
    sz_elem8,
    sz_light1,//属性ランプ点灯：分裂：00XX～（0001～0099　0000はダミー）
    sz_light2,//属性ランプ点灯：形状：01XX～（0100～0199：色　0200～0999形状）
    sz_light3,//属性ランプ点灯：波系：1XXX～｛10XX～11YY～12ZZ～（1000番台Ｘ、1100番台Ｙ、1200番台Ｚ）｝
    sz_light4,//属性ランプ点灯：速度：2XXX～(2099)
    sz_light5,//属性ランプ点灯：被弾：21XX～(2999)
    sz_light6,//属性ランプ点灯：距離：3XXX～(3999)
    sz_light7,//属性ランプ点灯：特殊：4XXX～(4999)
    sz_light8,//属性ランプ点灯：落下：5XXX～(5999)
    szEND,
  };

  enum enDtShElmStr
  {
    szs_name,
    szs_name2,
    szs_one,
    szs_text0,
    szs_text1,
    szs_text2,
    szs_text3,
  };
  class TDtShElm
  {
  protected:
    INTVECTOR intvec_genre_;
    STRVECTOR strvec_genre_;
    INTVECTOR intvec_genre8_;

    PKINTVECMAP intvec2_;
    PKSTRVECMAP strvec2_;
  public:
    explicit TDtShElm(void);
    virtual ~TDtShElm(void);

    //読み込み
    void LoadDt(const char *CSVDir, const char *CSVFilename);
    //取得
    //std::string GetName(s32 pk);
    s32 GetDt(s32 pk, enDtShElm dtNo);
    std::string GetDtStr(s32 pk, enDtShElmStr dtNo);
    STRVECTOR GetText(s32 pk);

    s32 GetGenreNum();
    STRVECTOR GetGenreStrvec();
    s32 GetGenre(s32 pk);
    s32 GetGenre8(s32 pk);
    s32 GetGenreID(s32 no);
    std::string GetGenreNamePK(s32 pk);
    std::string GetGenreNameLine(s32 no);
    s32 GetLightFlag(s32 pk);
    BOOL IsLightFlag(s32 pk, s32 genreno);

#if defined(__K_DEBUG_SHIAI__) || defined(__K_DEBUG_USER_DATA__)
	s32 GetSize(void);
	s32 GetKey(s32 no);
#endif	// #if defined(__K_DEBUG_SHIAI__) || defined(__K_DEBUG_USER_DATA__)
  };
}
