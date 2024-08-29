#pragma once
#include "dbInclude.h"
#include "dbGrContainer.h"
#include "dbMgEtcGrp.h"
#include "dbMgCharGrp.h"
#include "dbMgIconGrp.h"

namespace db
{
  class TMgEngineObj;



  const s32 SUBNUM = 2;

  struct TStMakeGameTexture
  {
    s32 line_r;
    s32 line_g;
    s32 line_b;
    s32 line_a;
  };

  struct TStMakeIconTexture_Chest
  {
    BOOL seihuku_f[DBSIDE][SUBNUM];
  };
  struct TStMakeIconTexture_Body
  {
    BOOL caponly_f[DBSIDE][SUBNUM];
    s32 capbmpno[DBSIDE][SUBNUM];
    s32 membmpno[DBSIDE][SUBNUM];
  };


  //画像マネージャーステータス
  struct TStMgGrp
  {
    //背景タイプ
    //enBGType bgtype_;
    //セピアです
    BOOL sepia_f_;
    BOOL courtLine_f_;

    //なぞのぐんだん戦コートです
    BOOL nazo_f_;

    //コート番号
    s32 courtNo_;
    s64 mid_court_;

    s32 rain_c_;
    s32 rainx_;
    s32 rainx2_;
    s32 rainy_;
    s32 rainy2_;

    TStMakeGameTexture stgame_;
    TStMakeIconTexture_Chest stchest_;
    TStMakeIconTexture_Body stbody_;
    void Init()
    {
      Zeromem(this);
    }
  };





  class TMgGrp
  {
  protected:
    //統合テクスチャの中身を作成
    void MakeTexture_Court(void* texbuf, s32& texpos, s32 line_r, s32 line_g, s32 line_b, s32 line_a);
    void MakeTexture_Body(void* texbuf, s32& texpos);

    void MakeTexture_Head(void* texbuf, s32& texpos);
    void MakeTexture_Tex2(void* texbuf, s32& texpos, s32& texpos2, mid::TPng* ppng, base::TGrp* pgrp);
    void MakeTexture_Ball(void* texbuf, s32& texpos, s32& texpos2, s32 ballcol);

    void MakeTexture_BodyPrev(void* texbuf, s32& texpos);
    void MakeTexture_HeadPrev(void* texbuf, s32& texpos, s32& texpos2);

    //全かおづらアイコン作成
    void MakeIconTexture_Kaozura();
    void MakeIconTexture_Ago();

    void MakeKantokuTexture_Body(TMgCharGrp* mgcg, mid::TBmpObj* pbmpobj);
    void MakeCaptainTexture_Body(TMgCharGrp* mgcg);
    void MakeCaptainsTexture_Body(TMgCharGrp* mgcg, s32 uino);
    void MakeSensyuTexture_Body(TMgCharGrp* mgcg, s32 uino);
    void MakePersonTexture_Body(TMgCharGrp* mgcg, s32 uino);
    void MakeUITexture_Face(TMgCharGrp* mgcg, mid::TBmpObj* bmoface, base::TGrTile2* face);
    void MakeUITexture_Hair(TMgCharGrp* mgcg, mid::TBmpObj* bmohair, base::TGrTile2* hair);
    void GenSetKantoku();
    void GenSetEnemy();
  public:

    //画像コンテナ
    TGrContainer cntn_;

    //画像マネージャーステータス
    TStMgGrp stMG_;

    //エンジンオブジェ
    TMgEngineObj* pmgEO_;

    //キャラグラフィックマネージャー
    TMgCharGrp* pmgChGrp_[DBSIDE][DBMEMBER_ALL];//人数分

    TMgCharGrp* pmgChRankerGrp_[RANKERNUM];//人数分

    //アイコン
    TMgIconGrp* pmgIcGrp_;

    TMgEtcGrp* pmgEtcGrp_;
    //TMgEtcGrp* pmgEtcGrp_;

    //ボスの描画情報
    std::vector<TStGrChar*> stBoss_;
    std::vector<TStGrChar*> stZako_;

    //画像のハンドル
    base::TGrp* pgr_Game_;//統合テクスチャ

    base::TGrTile* pgr_Body_[DBSIDE][DBMEMBER_ALL];//テクスチャオブジェ
    base::TGrTile2* pgr_Face_[DBSIDE][DBMEMBER_ALL];
    base::TGrTile2* pgr_Hair_[DBSIDE][DBMEMBER_ALL];

    //試合中用アイコン
    base::TGrTile2* pgr_IconHead_[DBSIDE][DBMEMBER_ALL];
//    //アイコン用
//    base::TGrPart* pgr_IconFace_[ICON_FACENUM];
//    base::TGrPart* pgr_IconHair_[ICON_HAIRNUM];
//    base::TGrPart* pgr_IconMouse_[ICON_MOUSENUM];//色数＋モノクロ

    //UI用アイコン
    base::TGrPart* pgr_IconChest_[DBSIDE];
    base::TGrPart* pgr_IconBody_[DBSIDE];
    base::TGrPart* pgr_IconChestSub_[DBSIDE];
    base::TGrPart* pgr_IconBodySub_[DBSIDE];
    base::TGrPart* pgr_IconBodySh_;

    base::TGrPart* pgr_IconRankerChest_[RANKERNUM];
    base::TGrPart* pgr_IconRankerChestSub_[RANKERNUM];

    base::TGrTile* pgr_Referee_;

    base::TGrBG* pgr_Court_;
    base::TGrTile* pgr_Ball_[BALLSPNUM];

    base::TGrTile* pgr_Clerk_;
    base::TGrTile* pgr_Girl_;

    base::TGrTile* pgr_Kantoku_;
    base::TGrTile2* pgr_KantokuFace_;
    base::TGrTile2* pgr_KantokuHair_;
    base::TGrTile* pgr_Captain_;
    base::TGrTile2* pgr_CaptainFace_;
    base::TGrTile2* pgr_CaptainHair_;
    base::TGrTile* pgr_Enemy_;
    base::TGrTile2* pgr_EnemyFace_;
    base::TGrTile2* pgr_EnemyHair_;

    base::TGrTile* pgr_Enemies_[PLAYERNUM];
    base::TGrTile2* pgr_EnemiesFace_[PLAYERNUM];
    base::TGrTile2* pgr_EnemiesHair_[PLAYERNUM];
    base::TGrTile* pgr_Captains_[PLAYERNUM];
    base::TGrTile2* pgr_CaptainsFace_[PLAYERNUM];
    base::TGrTile2* pgr_CaptainsHair_[PLAYERNUM];

    base::TGrTile* pgr_Sensyu_[DBMEMBER_ALL];
    base::TGrTile2* pgr_SensyuFace_[DBMEMBER_ALL];
    base::TGrTile2* pgr_SensyuHair_[DBMEMBER_ALL];

    base::TGrTile2* pgr_Boss_;//ボス画像
    base::TGrTile2* pgr_Zako_;//雑魚３人画像

    // Tipsキャラ
    std::map<s32, base::TGrTile*> pgr_Person_;
    std::map<s32, base::TGrTile2*> pgr_PersonFace_;
    std::map<s32, base::TGrTile2*> pgr_PersonHair_;

    ////汎用ＵＩタイル
    //std::vector<base::TGrTile*> pgr_UITiles_;
    //std::vector<TMgCharGrp*> pmgUIChGrps_;

    //キャラグラフィックマネージャー
    TMgCharGrp* pmgcg_kantoku_;//一人
    TMgCharGrp* pmgcg_captain_;	// キャプテン
    TMgCharGrp* pmgcg_sensyu_[DBMEMBER_ALL];//人数分
    TMgCharGrp* pmgcg_clerk_;//１枚に全員分入ってるので人数分不要
    TMgCharGrp* pmgcg_girl_;
    TMgCharGrp* pmgcg_enemy_;//敵やプロフィールなど
    TMgCharGrp* pmgcg_enemies_[PLAYERNUM];	// 複数の敵
    TMgCharGrp* pmgcg_captains_[PLAYERNUM];	// 複数のキャプテン
    std::map<s32, TMgCharGrp*> pmgcg_person_;// Tipsキャラ




    //コンストラクタ
    explicit TMgGrp(TMgEngineObj* pmgEO);
    virtual ~TMgGrp();

    //読み込み
    void LoadBmpCntn();
    //準備
    void Ready();
    //コート読み込み
    void LoadCourt(s64 mid_court);
    //全かおづらアイコン作成
    void MakeIconTexture();
    //テクスチャ作成
    void MakeGameTexture(s32 line_r, s32 line_g, s32 line_b, s32 line_a);
    //テクスチャ作成
    void MakeGameTexture_Prev();

    void MakeIconTexture_Chest(s32 sideno, BOOL seihuku_f, BOOL sub_f);
    void MakeIconTexture_Body(s32 sideno, s32 capbmpno, s32 membmpno, BOOL caponly_f, BOOL sub_f);
    void MakeIconTexture_BodyCap(s32 sideno, s32 capbmpno);
    void MakeIconTexture_BodySh();

    void MakeIconRankerTexture_Chest(s32 no, BOOL seihuku_f, BOOL sub_f);

    void MakeKantokuTexture();
    void MakeCaptainTexture();
    void MakeSensyuTexture(s32 uino);
    void MakeEnemyTexture();
    void MakeEnemiesTexture(s32 uino);
    void MakeCaptainsTexture(s32 uino);
    void MakePersonTexture(s32 key);
    void MakeBossTexture();
    void MakeZakoTexture();

    void ClearBossSt();
    s32 AddBossSt(s64 mid_char, s64 mid_team);
    void ClearZakoSt();
    s32 AddZakoSt(s64 mid_char, s64 mid_team);

    void SetKantoku();
    void SetProf_Me();
    void SetProf_Ranker(s64 id_ranker);
    void SetProf_Enemy(s32 no);

    void ReloadTex(enSceneName scenename);
  };
}
