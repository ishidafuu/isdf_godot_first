#pragma once
#include "dbInclude.h"
#include "dbMgEngineObj.h"
namespace db
{
    enum enUGGrPart
    {
        ugp_box_part,
        ugp_btn_key2,
        ugp_btn_key,
        ugp_cupselbig,
        ugp_cupsel,
        ugp_finger,
        ugp_gacha,
        ugp_game_bar,
        ugp_game_maru,
        ugp_game_time,
        ugp_handle,
        ugp_hibi,
        ugp_huku_crank,
        ugp_icon_album,
        ugp_icon_films,
        ugp_icon_item,
        ugp_icon_nabox,
        ugp_icon_rarity,
        ugp_icon_supply,
        ugp_icon_tapes,
        ugp_level_num,
        ugp_map_noise,
        ugp_map_panel,
        ugp_map_route,
        ugp_msg_frame,
        ugp_msg_frame_mini,
        ugp_pad_btn,
        ugp_pad_lever,
        ugp_quality_bar,
        ugp_quality_icon,
        ugp_shop,
        ugp_shot_card2,
        ugp_shot_card,
        ugp_shot_genre,
        ugp_shot_miniball,
        ugp_soubi_step,
        ugp_souko_num,
        ugp_team_card,
        ugp_time,
        ugp_tv_noise,
        ugp_tv_rotat,
        UGGrPartEND,
    };

    const s32 UGGRPART_SIZE[] =
    {
        11, 9,
        18, 24,
        24, 24,
        40, 40,
        20, 20,
        32, 32,
        64, 72,
        128, 4,
        10, 10,
        40, 24,
        12, 12,
        32, 16,
        48, 48,
        22, 16,
        16, 16,
        24, 24,
        20, 20,
        24, 16,
        24, 24,
        18, 12,
        5, 8,
        94, 42,
        104, 72,
        24, 32,
        152, 80,
        152, 54,
        48, 40,
        84, 100,
        22, 10,
        10, 10,
        152, 88,
        152, 96,
        152, 96,
        16, 16,
        8, 8,
        24, 8,
        8, 12,
        152, 96,
        8, 8,
        46, 32,
        2, 4,
    };

    const std::string UGGRPART_STR[] =
    {
        "ugp_box_part[4]",
        "ugp_btn_key2[2]",
        "ugp_btn_key[2,4]",
        "ugp_cupselbig[3,5]",
        "ugp_cupsel[8,12]",
        "ugp_finger[2]",
        "ugp_gacha[5]",
        "ugp_game_bar[3]",
        "ugp_game_maru[3]",
        "ugp_game_time[3]",
        "ugp_handle[4,5]",
        "ugp_hibi[2]",
        "ugp_huku_crank[10]",
        "ugp_icon_album[5,5]",
        "ugp_icon_films[5,5]",
        "ugp_icon_item[10,10]",
        "ugp_icon_nabox[6,5]",
        "ugp_icon_rarity[4,7]",
        "ugp_icon_supply[2,10]",
        "ugp_icon_tapes[5,5]",
        "ugp_level_num[13]",
        "ugp_map_noise[3]",
        "ugp_map_panel[4,6]",
        "ugp_map_route[4,4]",
        "ugp_msg_frame[2]",
        "ugp_msg_frame_mini[2]",
        "ugp_pad_btn[2]",
        "ugp_pad_lever[3,3]",
        "ugp_quality_bar[2,11]",
        "ugp_quality_icon[2,11]",
        "ugp_shop[4]",
        "ugp_shot_card2[3,5]",
        "ugp_shot_card[7]",
        "ugp_shot_genre[8]",
        "ugp_shot_miniball[7]",
        "ugp_soubi_step[8]",
        "ugp_souko_num[10]",
        "ugp_team_card[3,5]",
        "ugp_time[8]",
        "ugp_tv_noise[3]",
        "ugp_tv_rotate",
    };





  enum enUGGrp
  {
      ug_bar_stamina,
      ug_box,
      ug_boxnum,
      ug_btn_balloon,
      ug_btn_credit,
      ug_btn_footer,
      ug_btn_footer2,
      ug_btn_footer2_type2,
      ug_btn_footer_type2,
      ug_btn_foottsh,
      ug_btn_ggz,
      ug_btn_icon,
      ug_btn_icon2,
      ug_btn_icon3,
      ug_btn_inputtab,
      ug_btn_inputtab2,
      ug_btn_ok,
      ug_btn_ok2,
      ug_btn_oto,
      ug_btn_profile,
      ug_btn_profile2,
      ug_btn_tsh,
      ug_elm_base,
      ug_elm_base2,
      ug_game_ai,
      ug_game_ai2,
      ug_game_bar,
      ug_game_cacth,
      ug_game_kantokubg,
      ug_hk_frame,
      ug_home_board,
      ug_inp_boad,
      ug_inp_boad2,
      ug_inp_out,
      ug_inp_outend,
      ug_itemget_panel,
      ug_itemget_title,
      ug_itemget_title_mini,
      ug_lab_end,
      ug_lab_inputtip,
      ug_lab_word,
      ug_line,
      ug_list_frame,
      ug_list_frameshop,
      ug_list_framewide,
      ug_list_marker,
      ug_list_panel,
      ug_list_panelavatar,
      ug_list_panelavatar_long,
      ug_list_panelshop,
      ug_list_record,
      ug_list_record2,
      ug_list_record3,
      ug_list_recordmem,
      ug_list_recordmem2,
      ug_list_recordshop,
      ug_lock,
      ug_map_body,
      ug_msg_frame,
      ug_prof_message,
      ug_reward_bg,
      ug_serifu_frame,
      ug_serifu_frame2,
      ug_shot_space,
      ug_vs_vsset,
      UGGrpEND,
  };

  const std::string UGGRP_STR[] =
  {
      "ug_bar_stamina",
      "ug_box",
      "ug_boxnum",
      "ug_btn_balloon",
      "ug_btn_credit",
      "ug_btn_footer",
      "ug_btn_footer2",
      "ug_btn_footer2_type2",
      "ug_btn_footer_type2",
      "ug_btn_foottsh",
      "ug_btn_ggz",
      "ug_btn_icon",
      "ug_btn_icon2",
      "ug_btn_icon3",
      "ug_btn_inputtab",
      "ug_btn_inputtab2",
      "ug_btn_ok",
      "ug_btn_ok2",
      "ug_btn_oto",
      "ug_btn_profile",
      "ug_btn_profile2",
      "ug_btn_tsh",
      "ug_elm_base",
      "ug_elm_base2",
      "ug_game_ai",
      "ug_game_ai2",
      "ug_game_bar",
      "ug_game_cacth",
      "ug_game_kantokubg",
      "ug_hk_frame",
      "ug_home_board",
      "ug_inp_boad",
      "ug_inp_boad2",
      "ug_inp_out",
      "ug_inp_outend",
      "ug_itemget_panel",
      "ug_itemget_title",
      "ug_itemget_title_mini",
      "ug_lab_end",
      "ug_lab_inputtip",
      "ug_lab_word",
      "ug_line",
      "ug_list_frame",
      "ug_list_frameshop",
      "ug_list_framewide",
      "ug_list_marker",
      "ug_list_panel",
      "ug_list_panelavatar",
      "ug_list_panelavatar_long",
      "ug_list_panelshop",
      "ug_list_record",
      "ug_list_record2",
      "ug_list_record3",
      "ug_list_recordmem",
      "ug_list_recordmem2",
      "ug_list_recordshop",
      "ug_lock",
      "ug_map_body",
      "ug_msg_frame",
      "ug_prof_message",
      "ug_reward_bg",
      "ug_serifu_frame",
      "ug_serifu_frame2",
      "ug_shot_space",
      "ug_vs_vsset",
  };








  //アイコン
  enum enUGPIcon
  {
    ugi_Star,
    ugi_Todd,
    ugi_HukuMini,
    ugi_FKT,
    ugi_GGZ,
    ugi_MGK,
    ugi_Info,
    ugi_GetLog,
    ugi_UseLog,
    ugi_Mobage,
    ugi_GotoTitle,
    ugi_Logout,
    ugi_GiftBox,
    ugi_CheckStar,
    ugi_BuyTechno,
    ugi_BuyStar,
    ugi_TechnoLog,
    ugi_StarLog,
    ugi_Quick,
    ugi_Bara,
    ugi_AddDock,
    ugi_ShotInv,
    ugi_EditProf,
    ugi_Zura,
    ugi_Kao,
    ugi_Col,
    ugi_Ranking,
    ugi_Gacha,
    ugi_StarGacha,
    ugi_NaBox,
    ugi_NaBoxOtona,
    ugi_HukuGara,
    ugi_Prof,
    ugi_BuySozai,
    ugi_SellSozai,
    ugi_SozaiLog,
    ugi_HomeBGM,
    ugi_Nazo,
    ugi_Ball,
    ugi_Invite,
    ugi_RarityAll,
    ugi_RarityD,
    ugi_RarityC,
    ugi_RarityB,
    ugi_RarityA,
    ugi_RarityS,
    ugi_RaritySS,
    ugi_RaritySSS,
    ugi_SortTeamMID,
    ugi_SortLV,
    ugi_SortUse,
    ugi_SortShSetID,
    ugi_FilTokkou,
    ugi_SortShPow,
    ugi_SortShSpin,
    ugi_Huku,
    ugi_ShLock,
    ugi_SortTeamNew,
    ugi_SortShNew,
    ugi_ShSetAuto,
    ugi_ShSetClear,
    ugi_ShSet1,
    ugi_ShSet2,
    ugi_ShSet3,
    ugi_ShSetCstm,
    ugi_FilSolo,
    ugi_FilMixed,
    ugi_FilFullMixed,
    ugi_FilGenre0,
    ugi_FilGenre1,
    ugi_FilGenre2,
    ugi_FilGenre3,
    ugi_FilGenre4,
    ugi_FilGenre5,
    ugi_FilGenre6,
    ugi_FilGenre7,
    ugi_FilGenre8,
    ugi_FilGenre9,
    ugi_FilGenre10,
    ugi_GachaKen,
    ugi_GachaKenTodd,
    ugi_AllCheck,
    ugi_AllClear,
    ugi_CtrlSmart,
    ugi_CtrlRetro,
    ugi_Home,
    ugi_Away,
    ugi_Search,
    ugi_Search2,
    ugi_Rank,
    ugi_RankBest,
    ugi_RankNow,
    ugi_Tokkou,
    ugi_ScHome,
    ugi_ScAway,
    ugi_BuyMbCoin,
  };

  enum enUGPRarity
  {
    ugr_Team,
    ugr_Ball = (RARITYNUM * 1),
    ugr_Cup = (RARITYNUM * 2),
    ugr_Huku = (RARITYNUM * 3),
  };

  enum enUGBG
  {
    bg_None,
    bg_BG,
    bg_Court,
  };

  class TUIFrame;
  class TUILayer;
  class TUIObj;

  //UIレイヤ管理////////////////////////////////
  class TMgUI 
  {
  public:
    TMgEngineObj* pmgEO_;
    base::TGrBG* pgr_UIBG_;
    base::TGrp* pgr_UIFilter_;

    //コンストラクタ
    explicit TMgUI(TMgEngineObj* pmgEO);
    virtual ~TMgUI(void);

    void FreeEvIcon();

    void Init(enSceneName scenename);//初期化

    void LoadEvIcon();

    void SetUIFrame(TUIFrame* pframe);//ＵＩフレームセット
    void SetUILayer(TUILayer* pui);//ＵＩレイヤセット
    void FreeUILayer();//ＵＩレイヤセットされたものを全て解放
    void LoadBG(const char* filename);
    void LoadCourt(s64 mid_court);
    void LoadLayer();//読み込み
    void ReloadTex();//テクスチャ再読み込み（アンドロイドむけ）



    base::TGrp* GetUGGrp(enUGGrp gtype);
    base::TGrp* GetEvIcon(s32 iconno);
    base::TGrPart* GetUGGrPart(enUGGrPart gptype);

    void Update();//更新
    void Draw(BOOL bg_f);//描画

    void SetModal(BOOL modal_f);//モーダル窓
    BOOL IsModal();//モーダル窓
    void SetConnect(BOOL connect_f);
    BOOL IsConnect();//モーダル窓
    void SetAction();//現フレームでアクションが発生した（２つ以上発生しないようにする）
    BOOL IsAction();//現フレームでアクションが発生した
    s32 GetDepth();
    void AddUIObj(TUIObj* obj);//再構築用
    void RemoveUIObj(TUIObj* obj);//再構築用
    void SetWaitSceneMove();//シーン移動待ち
    BOOL IsWaitSceneMove();//シーン移動待ち
  protected:
    BOOL settex_f_;
    BOOL modal_f_;
    BOOL connect_f_;
    s32 action_c_;
    s32 depth_c_;
    BOOL needReload_f_;
    enSceneName scenename_;
    BOOL waitSceneMove_f_;

    //画像のポインタ
    mid::TBmpObj bmo_UIBG_;
    mid::TBmpObj bmo_UIFilter_;
    //ＵＩレイヤのポインタ
    TUIFrame* frame_;
    std::vector<TUILayer*> layers_;
    std::vector<mid::TBmpObj*> bmo_uggrp_;
    std::vector<mid::TBmpObj*> bmo_uggrpart_;
    std::vector<mid::TBmpObj*> bmo_evicon_;
    std::vector<base::TGrp*> pgr_uggrp_;
    std::vector<base::TGrp*> pgr_evicon_;
    std::vector<base::TGrPart*> pgr_uggrpart_;
    std::list<TUIObj*> uiobjlist_;
  };
 
}
