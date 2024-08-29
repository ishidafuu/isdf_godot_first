#include "dbLyHmShVSHum.h"
#include "dbSnHome.h"

namespace db
{

    BOOL NOCONNECT_F = FALSE;
    s32 ROOMNAMELEN = 4;

    enum enPage
    {
        pg_Visit,
        pg_Menu,
        pg_WifiRoom,

#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__
        pg_WifiRoomReward,
#endif
        pg_WifiReady,
        pg_WifiReadyError,
        pg_WifiRoomName,
        pg_WifiSearch,
        pg_BTRoom,

#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__
        pg_BTRoomReward,
#endif
        pg_BTReady,
        pg_BTReadyError,
        pg_BTSearch,
        pg_VSHumReady,
        pg_Match,
        pg_TmSelect,

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
        pg_Mgk,
#endif

        pg_ShSetSelect,
        pg_Filter,
        pg_GotoVS,
        pg_VS,
        pg_GotoShiai,
        pg_Out,
        pg_GKError,
        pg_Error,
        pg_Wifi_Help,
        pg_BT_Help,
    };

    enum enRoomList
    {
        rl_RoomKb,
        rl_RoomNm,
        rl_RoomKbName,
        rl_RoomNmName,
        rl_Help_Tushin,		// ヘルプ　「つうしんたいせん」について
        rlEND,
    };

    enum enBlueTooth
    {
        bt_RoomKb,
        bt_RoomNm,
        bt_Search,
        bt_Help_Tushin,		// ヘルプ　「つうしんたいせん」について
        btEND,
    };

    enum enRoomType
    {
        rt_Search,
        rt_Kb,
        rt_Nm,
    };

    enum enConnect
    {
        cn_SwitchShSet,
        cn_VSHumReady,
        cn_VSHumPlay,

        cn_WifiReady,
        cn_BTReady,

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
        cn_mgk,
#endif
    };

    const s32 ONESEC = 60;
    const s32  WAITTIME = 5 * ONESEC;
    const s32 TSLTIME = 30 * ONESEC;
    const s32 ABLETMFIL = 30;

    //コンストラクタ
    TLyHmShVSHum::TLyHmShVSHum(TSnHome* pScene)
        :TLyHmGen(pScene)
        , ugshbtn_bt_(this)
        , ugshbtn_wifi_(this)
        , uglistWifiRoom_(this)
        , uglistBTRoom_(this)
        , uglistTmFilter_(this)
        , uglistShSetSelectAll_(this)
        , uglistShSetSelect_(this)
        , uglistMember_(this)
        , ugdrTeam_(this)
        , ugTeam_(this)
        , ugprof_(this)
        , ugrmsg_bt_(this)
        , ugrmsg_wifi_(this)
        , ugvs_(this)
        , ugtitle_me_(this)
        , ugtitle_en_(this)
        , ugbtn_bt_host_(this)
        , ugbtn_bt_client_(this)
        , ugbtn_wifi_free_(this)
        , ugbtn_wifi_select_(this)
#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__
        , ugtitle_reward_(this)
        , ugLiReward_(this)
#endif
#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
        , pCursor_(NULL)
#endif
    {
        for (s32 i = 0; i < DBSIDE; ++i) ugvsteam_.push_back(new TUGScVSTeam(this));
        stVSHum_.Init();
    }

    TLyHmShVSHum::~TLyHmShVSHum()
    {
        for (s32 i = 0; i < ugvsteam_.size(); ++i) Freemem(ugvsteam_[i]);
#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__
        for (s32 i = 0; i < ugLiRewardInfo_.size(); i++){ Freemem(ugLiRewardInfo_[i]); }
#endif
    }

    void TLyHmShVSHum::DoLoad()
    {
        ugprof_.MakeGroup(field_, FALSE);
        ugprof_.SetDraw(FALSE);

        ugshbtn_bt_.MakeGroupShiai(field_, hmgrp_->hmshiai_next_, 2, "ブルートゥース");
        ugshbtn_bt_.SetBanner(hmgrp_->hmshiai_banner_, 0, "めのまえの＿おともだちと＿しょうぶ");
        ugshbtn_bt_.SetPos(-80, -40);
        ugshbtn_wifi_.MakeGroupShiai(field_, hmgrp_->hmshiai_next_, 2, "Ｗｉ負Ｆｉ");
        ugshbtn_wifi_.SetBanner(hmgrp_->hmshiai_banner_, 1, "ぜんこくのプレーヤーと＿うでだめし");
        ugshbtn_wifi_.SetPos(80, -40);

        uglistWifiRoom_.MakeGroup(field_, FALSE);
        uglistBTRoom_.MakeGroup(field_, FALSE);
        uglistTmFilter_.MakeGroup(field_, FALSE);

        uglistShSetSelect_.MakeGroup(field_, FALSE);
        uglistShSetSelectAll_.MakeGroup(field_, FALSE);
        uglistMember_.MakeGroup(field_, TRUE);


        ugdrTeam_.MakeGroup(field_, TRUE, SIDE0, &uglistTmFilter_);
        ugdrTeam_.SetDraw(FALSE);
        for (s32 i = 0; i < DBSIDE; ++i)
        {
            ugvsteam_[i]->MakeGroup(field_, i);
        }

        ugTeam_.MakeGroup(field_, SIDE0);
        //ugTeam_.SetPos(-160, -40);
        ugTeam_.SetLRPos(TRUE);
        ugTeam_.SetDraw(FALSE);


        ugtitle_me_.MakeGroup(field_, FALSE);
        ugtitle_me_.SetMessage("あなたのチーム");
        ugtitle_me_.SetLRPos(TRUE);
        ugtitle_en_.MakeGroup(field_, FALSE);
        ugtitle_en_.SetMessage("あいてのチーム");
        ugtitle_en_.SetLRPos(FALSE);
        ugtitle_me_.SetDraw(FALSE);
        ugtitle_en_.SetDraw(FALSE);

        ugrmsg_bt_.MakeGroup(field_, TRUE);
        ugrmsg_bt_.SetPos(-156, 0);
        STRVECTOR btvec = {
            "ＢＬＵＥＴＯＯＴＨ",
            "Ｂｌｕｅｔｏｏｔｈを＿オンにして",
            "へやを＿つくって＿たいきします",
            "だれかが＿つくったへやに＿はいると",
            "しあいが＿はじまります" };
        ugrmsg_bt_.SetData(&btvec);
        ugrmsg_wifi_.MakeGroup(field_, TRUE);
        ugrmsg_wifi_.SetPos(4, 0);
        STRVECTOR wifivec = {
            "ＷＩ負ＦＩ",
            "ネットせつぞくできる＿じょうたいで",
            "だれかとたいせんを＿せんたくして",
            "たいきします＿あいてが＿みつかると",
            "しあいが＿はじまります" };
        ugrmsg_wifi_.SetData(&wifivec);

        //VS
        ugvs_.MakeGroup(field_);

        listvec_.clear();
        listvec_.push_back(&uglistWifiRoom_);
        listvec_.push_back(&uglistBTRoom_);
        //メニューレコード
        for (s32 i = 0; i < rlEND; ++i)
        {
            TUGRcNormal* newrecord = (TUGRcNormal*)uglistWifiRoom_.MakeObj_Record();
            switch (i)
            {
            case rl_RoomKb:newrecord->ugname_.SetNameIcon("だれかと＿きびきびたいせん", ugi_SortUse); break;
            case rl_RoomNm:newrecord->ugname_.SetNameIcon("だれかと＿なめらかたいせん", ugi_SortUse); break;
            case rl_RoomKbName:newrecord->ugname_.SetNameIcon("してい＿きびきびべやマッチ", ugi_Search); break;
            case rl_RoomNmName:newrecord->ugname_.SetNameIcon("してい＿なめらかべやマッチ", ugi_Search); break;
            case rl_Help_Tushin:newrecord->ugname_.SetNameIcon("ヘルプ「つうしんたいせん」", ugi_Nazo); break;
            }


            waitTxt_ = MakeObj();
            waitTxt_->o_SetParent(field_);
            waitTxt_->z_SetPos(-mid::midGetDotL_Hf() + 50, mid::midGetDotS_Hf() - 20);
            waitTxt_->f_MakeFont("しばらく＿そのままで＿おまちください", FC2_normal);
            waitTxt_->g_SetDepth(UIDP_09MSGBOX);
            waitTxt_->g_SetDraw(FALSE);
        }

        //メニューレコード
        for (s32 i = 0; i < btEND; ++i)
        {
            TUGRcNormal* newrecord = (TUGRcNormal*)uglistBTRoom_.MakeObj_Record();
            switch (i)
            {
            case bt_RoomKb:newrecord->ugname_.SetNameIcon("きびきびべやを＿たてる", ugi_Home); break;
            case bt_RoomNm:newrecord->ugname_.SetNameIcon("なめらかべやを＿たてる", ugi_Home); break;
            case bt_Search:newrecord->ugname_.SetNameIcon("ちかくの＿へやをさがす", ugi_Away); break;
            case bt_Help_Tushin:newrecord->ugname_.SetNameIcon("ヘルプ「つうしんたいせん」", ugi_Nazo); break;
            }
        }



        ugbtn_bt_host_.MakeGroupShiai(field_, hmgrp_->hmshiai_next_, 2, "たいせんべやを＿たてる");
        ugbtn_bt_host_.SetBanner(hmgrp_->hmshiai_menu_bt_, 0, "あいての＿せつぞくを＿まちます");
        ugbtn_bt_host_.SetPos(80, -40);

        ugbtn_bt_client_.MakeGroupShiai(field_, hmgrp_->hmshiai_next_, 2, "たいせんべやを＿さがす");
        ugbtn_bt_client_.SetBanner(hmgrp_->hmshiai_menu_bt_, 1, "だれかの＿へやに＿せつぞくします");
        ugbtn_bt_client_.SetPos(80, 40);


        ugbtn_wifi_free_.MakeGroupShiai(field_, hmgrp_->hmshiai_next_, 2, "だれかと＿つうしんたいせん");
        ugbtn_wifi_free_.SetBanner(hmgrp_->hmshiai_menu_wifi_, 0, "じどうてきに＿マッチングします");
        ugbtn_wifi_free_.SetPos(80, -40);

        ugbtn_wifi_select_.MakeGroupShiai(field_, hmgrp_->hmshiai_next_, 2, "してい＿つうしんたいせん");
        ugbtn_wifi_select_.SetBanner(hmgrp_->hmshiai_menu_wifi_, 1, "へやめいを＿きめて＿マッチング");
        ugbtn_wifi_select_.SetPos(80, 40);


#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__

        ugtitle_reward_.MakeGroup_Mini(field_, FALSE);
        ugtitle_reward_.SetMessage("しあいご＿どれかもらえる");
        ugtitle_reward_.SetLRPos(TRUE);

        ugLiRewardInfo_.clear();
        ugLiReward_.MakeGroup(field_, TRUE);
        s32 vsDropTypeNum = pmgEO_->mgCoM_.mdm_VSDrop_.GetLineNum();	// ここで報酬のアイテムタイプの数を取得


        // 先に確立表示を計算しておく必要がある
        LONGVECTOR2 probVec;
        LONGVECTOR probAllVec;
        probVec.clear();
        probAllVec.resize(4, 0);

        for (s32 i = 0; i < vsDropTypeNum; i++)
        {
            LONGVECTOR v;
            v.clear();

            s32 DROP_TYPE = 4;	// 確立の種類は4つ：対人勝利、対人敗北、対COM勝利、対COM敗北
            for (s32 j = 0; j < DROP_TYPE; j++)
            {
                enmdm_VSDrop en = (enmdm_VSDrop)(enmdm_VSDrop::mdm_vsdrop_prob_hw + j);
                s64 prob = pmgEO_->mgCoM_.mdm_VSDrop_.GetLineDt(i, en);
                v.push_back(prob);

                probAllVec[j] += prob;
            }
            probVec.push_back(v);
        }

        for (s32 i = 0; i < vsDropTypeNum; ++i)
        {
            // アイテムのレコード作成
            TUGRcNormal* newrecord = (TUGRcNormal*)ugLiReward_.MakeObj_Record();

            s64 itemType = pmgEO_->mgCoM_.mdm_VSDrop_.GetLineDt(i, enmdm_VSDrop::mdm_vsdrop_itemType);
            s64 itemID = pmgEO_->mgCoM_.mdm_VSDrop_.GetLineDt(i, enmdm_VSDrop::mdm_vsdrop_itemID);
            s64 itemNum = pmgEO_->mgCoM_.mdm_VSDrop_.GetLineDt(i, enmdm_VSDrop::mdm_vsdrop_itemNum);

            switch (itemType)
            {
            case enItemType_Star: newrecord->ugname_.SetNameStar(itemNum); break;
            case enItemType_Todd: newrecord->ugname_.SetNameTodd(itemNum); break;
            case enItemType_Supply: newrecord->ugname_.SetNameSupply(itemID, itemNum); break;
            }

            // アイテムの確立リスト作成
            s32 DROP_TYPE = 4;	// 確立の種類は4つ：対人勝利、対人敗北、対COM勝利、対COM敗北
            TUGLiVSHumReward* newlist = new TUGLiVSHumReward(this);
            newlist->MakeGroup(field_, TRUE);
            for (s32 j = 0; j < DROP_TYPE; j++)
            {
                TUGRcNormal* newrecord2 = (TUGRcNormal*)newlist->MakeObj_Record();

                enmdm_VSDrop en = (enmdm_VSDrop)(enmdm_VSDrop::mdm_vsdrop_prob_hw + j);
                //s64 prob = pmgEO_->mgCoM_.mdm_VSDrop_.GetLineDt(i, en);
                //std::string probValStr = lib_str::IntToStr(prob) + "％";

                s64 prob = (probVec[i][j] * 10000);
                prob /= probAllVec[j];

                s32 probper = prob / 100;
                s32 probcnm = prob % 100;
                std::string probValStr = lib_str::IntToStr(probper) + "．" + lib_str::IntToStr(probcnm, 2) + "％";

                std::string probStr = "";
                switch (en)
                {
                case mdm_vsdrop_prob_hw: probStr = "たいじん＿かち"; break;
                case mdm_vsdrop_prob_hl: probStr = "たいじん＿まけ"; break;
                case mdm_vsdrop_prob_cw: probStr = "たいＣＯＭ＿かち"; break;
                case mdm_vsdrop_prob_cl: probStr = "たいＣＯＭ＿まけ"; break;
                }
                newrecord2->ugname_.SetName(probStr.c_str());
                newrecord2->SetValue(probValStr.c_str());
            }
            ugLiRewardInfo_.push_back(newlist);
        }
#endif
#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
        // スタミナ回復先導カーソル
        pCursor_ = MakeObj();
        pCursor_->o_SetObjGrPart(pmgUI_->GetUGGrPart(ugp_finger));
        pCursor_->g_SetPartNo(1);
        //pCursor_->o_SetParent(field_);
        pCursor_->o_SetParent(ugbtn_mgk_->GetField());
        pCursor_->g_SetDraw(FALSE);
        pCursor_->z_SetPos(50, -28);
#endif
    }

    void TLyHmShVSHum::DoReady()
    {
        stVSHum_.Init();

        pmgEO_->stSeq_.SetIDEvent(0, 0);//イベントＩＤ空に
        uglistTmFilter_.Ready(NGNUM);
        ugdrTeam_.ReadyFilter();

        DrawList(NULL);
        ugbtn_back_->SetFont("もどる");
        ugbtn_ok_->SetFont("たいけつ！");
        ugbtn_next_->SetFont("ならびかえ");
        ugbtn_mid_->SetFont("しゅーと");
        ugbtn_back_->SetDraw(TRUE);
        ugbtn_ok_->SetDraw(FALSE);
    }

    void TLyHmShVSHum::DoDown()
    {
        pScene_->lyHmShiai_->Ready();
    }

    void TLyHmShVSHum::DrawList(TUGLiGen* drawlist)
    {
        for (s32 i = 0; i < listvec_.size(); ++i)
        {
            listvec_[i]->SetDraw(FALSE);
        }
        if (drawlist != NULL)
        {
            drawlist->SetDraw(TRUE);
            //drawlist->SetZeroPos();
        }
        ugbtn_back_->SetDraw(TRUE);
        ugbtn_ok_->SetDraw(FALSE);
        ugbtn_mid_->SetDraw(FALSE);
        ugbtn_next_->SetDraw(FALSE);
        ugprof_.SetDraw(FALSE);
        waitTxt_->g_SetDraw(FALSE);

#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__
        ugLiReward_.SetDraw(FALSE);
        for (s32 i = 0; i < ugLiRewardInfo_.size(); i++)
        {
            ugLiRewardInfo_[i]->SetDraw(FALSE);
        }
#endif
        ResetObj();
    }
    void TLyHmShVSHum::ResetObj()
    {
        ugshbtn_bt_.SetDraw(FALSE);
        ugshbtn_wifi_.SetDraw(FALSE);
        ugdrTeam_.SetDraw(FALSE);
        ugTeam_.SetDraw(FALSE);
        ugrmsg_bt_.SetDraw(FALSE);
        ugrmsg_wifi_.SetDraw(FALSE);
#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__
        ugtitle_reward_.SetDraw(FALSE);
#endif
        ugbtn_bt_host_.SetDraw(FALSE);
        ugbtn_bt_client_.SetDraw(FALSE);
        ugbtn_wifi_free_.SetDraw(FALSE);
        ugbtn_wifi_select_.SetDraw(FALSE);
        ugbtn_mgk_->SetDraw(FALSE);	// みなげんきーを消す
    }

    //ドラムロール
    void TLyHmShVSHum::LoadLookPosTeam()
    {
        //同じＩＤの時は読み込みいらない
        s32 mid_team = pmgEO_->mgCoU_.mdTeam_.GetLineDt(ugdrTeam_.GetDataNo(), mdteam_id_m_team);
        BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_gb_f);
        if (stVSHum_.mid_team_dr_ == mid_team) return;

        stVSHum_.mid_team_dr_ = mid_team;
        stVSHum_.id_team_dr_ = pmgEO_->mgCoU_.mdTeam_.GetLineDt(ugdrTeam_.GetDataNo(), mdteam_pk);
        for (s32 c = 0; c < DBMEMBER_ALL; ++c)
        {
            s32 charid = pmgEO_->mgCoM_.GetCharID(stVSHum_.mid_team_dr_, c);
            pmgEO_->mgGrp_.pmgChGrp_[SIDE0][c]->SetCharDt(charid, stVSHum_.mid_team_dr_, gb_f);
            pmgEO_->mgGrp_.pmgChGrp_[SIDE0][c]->SetGrTile(SIDE0, c);
        }
        pmgEO_->mgGrp_.MakeIconTexture_Chest(SIDE0, FALSE, FALSE);
        pmgEO_->mgGrp_.MakeIconTexture_Body(SIDE0, 0, 0, FALSE, FALSE);
    }

    //敵監督画像作成
    void TLyHmShVSHum::RefreshEnemyTex()
    {
        pmgEO_->mgGrp_.SetProf_Enemy(0);
    }
    BOOL TLyHmShVSHum::Is30FPS()
    {
        return  (stVSHum_.roomtype_ == rt_Kb);
    }
    //両者情報セット
    void TLyHmShVSHum::SetEtcSS()
    {
        s32 mid_court = (mid::midGetMyIdx() == SIDE0)
            ? pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_haikei)
            : stVSHum_.mid_court_en_;

        s32 mid_bgm = (mid::midGetMyIdx() == SIDE0)
            ? pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_m_bgm)
            : stVSHum_.mid_bgm_en_;

        pmgEO_->stResult_.Init();
        pmgEO_->stDrop_.Init();
        pmgEO_->stSeq_.ReadyEvent();
        pmgEO_->stShiai_.SetVSHum(
            mid::midGetMyIdx(),
            pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_pk),
            stVSHum_.id_kantoku_en_,
            stVSHum_.id_team_my_,
            stVSHum_.id_team_en_,
            pmgEO_->mgCoU_.mdTeam_.GetPKDt(stVSHum_.id_team_my_, mdteam_id_m_team),
            pmgEO_->mgCoU_.mdTeam_.GetPKDt(stVSHum_.id_team_en_, mdteam_id_m_team),
            mid_court,
            mid_bgm,
#ifndef __K_MAKE_HIDE_EVENT_SOUND__
			nsf_jgl_ready,
			nsf_jgl_win,
			nsf_jgl_lose,
#endif
            pmgEO_->mgCoU_.GetCtrl(),
            240,
            Is30FPS(),
            stVSHum_.wifi_f_);

        for (s32 i = 0; i < ugvsteam_.size(); ++i) ugvsteam_[i]->Refresh();
        for (s32 i = 0; i < DBSIDE; ++i)
        {
            s32 mid_team = pmgEO_->stShiai_.GetMidTeam(i);//  mgStSn_.EtcSS.mid_team_[i];
            BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_gb_f);
            for (s32 c = 0; c < DBMEMBER_ALL; ++c)
            {
                s32 charid = pmgEO_->mgCoM_.GetCharID(mid_team, c);
                pmgEO_->mgGrp_.pmgChGrp_[i][c]->SetCharDt(charid, mid_team, gb_f);
                pmgEO_->mgGrp_.pmgChGrp_[i][c]->SetGrTile(i, c);
            }
            pmgEO_->mgGrp_.MakeIconTexture_Body(i, 1, 1, FALSE, FALSE);
        }
    }

    void TLyHmShVSHum::DoChangePage(s32 pageno)
    {
        switch ((enPage)pageno)
        {
        case db::pg_Visit:
            break;
        case db::pg_Menu:
            DrawList(NULL);
            pScene_->lyHmBar_->SetNoneBar(FALSE);
            mid::midSetDisconnect(FALSE);//切断
            pmgEO_->mgCoU_.CleanOtherKantoku();//他人情報削除（取得して逆送してきたときにここで消す）
            ugbtn_back_->SetFont("もどる");
            ugbtn_back_->SetDraw(TRUE);
            ugshbtn_bt_.SetDraw(TRUE);
            ugshbtn_wifi_.SetDraw(TRUE);
            //ugvsbonus_.SetDraw(TRUE);
            ugrmsg_bt_.SetDraw(TRUE);
            ugrmsg_wifi_.SetDraw(TRUE);
            break;
        case db::pg_WifiRoom:
            DrawList(NULL);
            stVSHum_.InitVS();
            // いしださんより：ここにも処理をコピペ
            pmgEO_->mgCoU_.CleanOtherKantoku();//他人情報削除（取得して逆送してきたときにここで消す）
#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__
            ugtitle_reward_.SetDraw(TRUE);
            ugtitle_reward_.SetMessage("しあいご＿どれかもらえる");
            ugLiReward_.SetDraw(TRUE);
#endif
            ugbtn_wifi_free_.SetDraw(TRUE);
            ugbtn_wifi_select_.SetDraw(TRUE);
            ugbtn_back_->SetFont("もどる");
            ugbtn_ok_->SetDraw(TRUE);
            ugbtn_ok_->SetFont("ヘルプ");
            break;

#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__
        case db::pg_WifiRoomReward:
            DrawList(NULL);
            ugLiRewardInfo_[stVSHum_.rewardRcNo_]->SetDraw(TRUE);
            ugtitle_reward_.SetMessage(ugLiReward_.records_[stVSHum_.rewardRcNo_]->ugname_.GetName() + "＿かくりつ");
            stVSHum_.InitVS();
            ugtitle_reward_.SetDraw(TRUE);
            ugbtn_wifi_free_.SetDraw(TRUE);
            ugbtn_wifi_select_.SetDraw(TRUE);
            ugbtn_back_->SetFont("とじる");
            ugbtn_ok_->SetDraw(TRUE);
            ugbtn_ok_->SetFont("ヘルプ");
            break;
#endif

        case db::pg_WifiReady:
            DrawList(NULL);
            ugbtn_back_->SetDraw(FALSE);//戻るボタン消す
            isSearchStart_ = FALSE;
            break;
        case db::pg_WifiReadyError:
            DrawList(NULL);
            ugbtn_back_->SetDraw(FALSE);
            break;
        case db::pg_WifiRoomName:
            DrawList(NULL);
            ugbtn_back_->SetDraw(FALSE);
            frame_->SetHudInput(ROOMNAMELEN, TRUE);
            break;
        case db::pg_WifiSearch:
            stVSHum_.InitVS();
            stVSHum_.wifi_f_ = TRUE;
            if ((stVSHum_.roomNo_ == rl_RoomNm) || (stVSHum_.roomNo_ == rl_RoomNmName))
            {
                stVSHum_.roomtype_ = rt_Nm;
            }
            DrawList(NULL);
            SetHudVSWait_Search();
            mid::midSearchWifi(stVSHum_.roomid_);
            waitTxt_->g_SetDraw(TRUE);
            break;
        case db::pg_BTRoom:
            DrawList(NULL);
            stVSHum_.InitVS();

#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__
            ugtitle_reward_.SetDraw(TRUE);
            ugtitle_reward_.SetMessage("しあいご＿どれかもらえる");
            ugLiReward_.SetDraw(TRUE);
#endif

            ugbtn_bt_host_.SetDraw(TRUE);
            ugbtn_bt_client_.SetDraw(TRUE);
            ugbtn_back_->SetFont("もどる");
            ugbtn_ok_->SetDraw(TRUE);
            ugbtn_ok_->SetFont("ヘルプ");
            break;

#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__
        case db::pg_BTRoomReward:
            DrawList(NULL);
            ugLiRewardInfo_[stVSHum_.rewardRcNo_]->SetDraw(TRUE);
            ugtitle_reward_.SetMessage(ugLiReward_.records_[stVSHum_.rewardRcNo_]->ugname_.GetName() + "＿かくりつ");
            stVSHum_.InitVS();
            ugtitle_reward_.SetDraw(TRUE);
            ugbtn_bt_host_.SetDraw(TRUE);
            ugbtn_bt_client_.SetDraw(TRUE);
            ugbtn_back_->SetFont("とじる");
            ugbtn_ok_->SetDraw(TRUE);
            ugbtn_ok_->SetFont("ヘルプ");
            break;
#endif

        case db::pg_BTReady:
            DrawList(NULL);
            ugbtn_back_->SetDraw(FALSE);//戻るボタン消す
            isSearchStart_ = FALSE;
            break;
        case db::pg_BTReadyError:
            DrawList(NULL);
            ugbtn_back_->SetDraw(FALSE);
            break;
        case db::pg_BTSearch:
            DrawList(NULL);
            stVSHum_.InitVS();
            stVSHum_.wifi_f_ = FALSE;
            if ((stVSHum_.roomNo_ == bt_Search))
            {
                stVSHum_.roomtype_ = rt_Search;
            }
            else if (stVSHum_.roomNo_ == bt_RoomKb)
            {
                stVSHum_.roomtype_ = rt_Kb;
            }
            else if (stVSHum_.roomNo_ == bt_RoomNm)
            {
                stVSHum_.roomtype_ = rt_Nm;
            }

            if (stVSHum_.roomNo_ == bt_Search)
            {
                SetHudVSWait_Search();
                mid::midSearchBT(FALSE);
            }
            else
            {
                SetHudVSWait_EnWait();
                mid::midSearchBT(TRUE);
            }
            waitTxt_->g_SetDraw(TRUE);
            break;
        case db::pg_VSHumReady:
            DrawList(NULL);
            stVSHum_.discn_c_ = WAITTIME;
            ugbtn_back_->SetDraw(FALSE);//戻るボタン消す
            break;
        case db::pg_Match:
            DrawList(NULL);
            ugtitle_en_.SetDraw(TRUE);
            stVSHum_.wait_c_ = WAITTIME;
            ugbtn_back_->SetDraw(FALSE);//戻るボタン消す
            ugprof_.SetProfEnemy(0);
            stVSHum_.mid_bgm_en_ = pmgEO_->mgCoU_.mdre_Enemy_.GetLineDt(0, mdre_enemy_id_m_bgm);
            stVSHum_.mid_court_en_ = pmgEO_->mgCoU_.mdre_Enemy_.GetLineDt(0, mdre_enemy_id_m_haikei);
            RefreshEnemyTex();
            ugbtn_ok_->SetDraw(TRUE);
            ugbtn_ok_->SetFont("たいけつ！");
            pmgEO_->mgSound_.PlaySYS(2);		// マッチングした際にSE「なめんなよ」
            break;
        case db::pg_TmSelect:
            DrawList(NULL);
            ugtitle_me_.SetDraw(TRUE);
            ugbtn_mgk_->SetDraw(TRUE);	// みなげんきーを消す
            ugbtn_mgk_->Refresh();
            ugbtn_back_->SetDraw(FALSE);//戻るボタン消す
            ugprof_.SetDraw(TRUE);
            ugdrTeam_.SetDraw(TRUE);
            ugbtn_ok_->SetDraw(TRUE);
            ugbtn_ok_->SetFont("けってい");
            ugbtn_next_->SetDraw(TRUE);
            ugbtn_mid_->SetDraw(TRUE);
            stVSHum_.id_team_dr_ = ugdrTeam_.GetIDTeam();
            stVSHum_.mid_team_dr_ = ugdrTeam_.GetMIDTeam();
            break;

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
        case db::pg_Mgk:
        {
            DrawList(NULL);
            SetFoot_Btn();
            stVSHum_.mgknum_ = pmgEO_->mgCoU_.GetSupplyNum_MGK();
            if (stVSHum_.mgknum_ > 0)
            {
                std::string strnum = "もっているかず：";
                strnum += lib_str::IntToStr(stVSHum_.mgknum_);
                STRVECTOR strvec = { "ミナゲンキーを＿しようします", "よろしいですか？", strnum };
                SetHudMsg(TRUE, FALSE, &strvec);
            }
            else
            {
                SetHudMsgStarSupply(SPL_MGK, TRUE);
            }
        }
        break;
#endif

        case db::pg_ShSetSelect:
            DrawList(&uglistShSetSelectAll_);
            uglistShSetSelectAll_.SetDraw(TRUE);
            uglistMember_.SetDraw(TRUE);
            ugbtn_back_->SetFont("もどる");
            ugbtn_back_->SetDraw(TRUE);
            break;
        case db::pg_Filter:
            DrawList(&uglistTmFilter_);
            ugdrTeam_.SetDraw(TRUE);
            ugbtn_back_->SetFont("もどる");
            ugbtn_back_->SetDraw(TRUE);
            break;
        case db::pg_GotoVS:
            pScene_->lyHmBar_->SetFilterAll(FALSE);//フィルタかける
            ugbtn_mgk_->SetDraw(FALSE);	// みなげんきーを消す
            break;
        case db::pg_VS:
            SetEtcSS();
            pScene_->lyHmBar_->SetNoneBar(TRUE);
            stVSHum_.wait_c_ = WAITTIME;
            stVSHum_.discn_c_ = WAITTIME * 2;
            SetHudCount(stVSHum_.wait_c_ / ONESEC);
            break;
        case db::pg_GotoShiai:
            pScene_->lyHmBar_->SetFilterAll(FALSE);
            pScene_->SetNextVSShiai();
            break;
        case db::pg_Error:
            //とりあえず切断
            DrawList(NULL);
            SetHudVSWaitEnd();
            mid::midSetDisconnect(FALSE);
            break;
        case db::pg_GKError:
            DrawList(NULL);
            SetFoot_Btn();//基本的にはボタン、タブにすべきならDoReady内で指定
            ugbtn_back_->SetDraw(FALSE);//戻るボタン消す
            SetHudVSWaitEnd();
            SetHudGKErrorMsg();//エラーメッセージ
            mid::midSetDisconnect(FALSE);//切断
            break;
        case db::pg_Wifi_Help:
            mid::midModalPanel_HelpList(&pmgEO_->mgCoM_.mdm_Help_.model_, 3);
            break;
        case db::pg_BT_Help:
            mid::midModalPanel_HelpList(&pmgEO_->mgCoM_.mdm_Help_.model_, 3);
            break;
        }
    }
    void TLyHmShVSHum::Backto_Menu()
    {
        //googleplayではワイファイのみ
//      if (mid::midIsGooglePlay() && mid::midIsADR())
        if (mid::midIsADR())
        {
            Down();
        }
        else
        {
            ChangePage(pg_Menu);
        }
    }

    void TLyHmShVSHum::MvPage_Visit()
    {
        //googleplayではワイファイのみ
//        if (mid::midIsGooglePlay())
      if (mid::midIsADR())
        {
            ChangePage(pg_WifiRoom);
        }
        else
        {
            ChangePage(pg_Menu);
        }
    }
    //対戦メニュー
    void TLyHmShVSHum::MvPage_Menu()
    {
        if (ugbtn_back_->IsAct())
        {
            Down();
        }
        else if (ugshbtn_bt_.IsAct())
        {
            //接続無しテスト
            if (NOCONNECT_F)
            {
                ChangePage(pg_VSHumReady);
            }
            else
            {
                ChangePage(pg_BTRoom);
            }
        }
        else if (ugshbtn_wifi_.IsAct())
        {
            // GooglePlayを使う
            if(pmgEO_->mgDt_.dtClient_.IsAccessGooglePlayByIOS())
            {
                if ((mid::midIsGooglePlay() == FALSE) || mid::midIsSignIn())
                {
                    ChangePage(pg_WifiRoom);
                }
                else
                {
                    mid::midStartAuthorizationUI();
                }
            }
            // 使わないので今まで通り
            else
            {
                ChangePage(pg_WifiRoom);
            }
        }
    }
    //ワイファイ部屋選択
    void TLyHmShVSHum::MvPage_WifiRoom()
    {
        if (ugbtn_back_->IsAct())
        {
            //googleplayではワイファイのみ
            Backto_Menu();

        }
        else if (ugbtn_ok_->IsAct())
        {
            stVSHum_.roomNo_ = rl_Help_Tushin;
            ChangePage(pg_Wifi_Help);
        }

#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__
        else if (ugLiReward_.IsAct())
        {
            stVSHum_.rewardRcNo_ = ugLiReward_.GetActIndex();
            ChangePage(pg_WifiRoomReward);
        }
#endif

        else if (ugbtn_wifi_free_.IsAct())
        {
            stVSHum_.roomNo_ = rl_RoomKb;//部屋番号
            stVSHum_.roomid_ = (0x0FFFFFFF - 1 - stVSHum_.roomNo_);

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
            // ここで消費スタミナをチェック
            s32 needStamina = 20;// pmgEO_->mgCoM_.mdm_Connect_.GetPKDt(0, enmdm_Connect::);

            // 監督の消費HPが0以下ならとくにチェックシーンに行かない
            if (needStamina <= 0)
            {
                isStaminaCheck_ = FALSE;
                ChangePage(pg_WifiSearch);
            }
            else
            {
                isStaminaCheck_ = TRUE;
                //STRVECTOR msg{ "かんとくＨＰが＿負" + lib_str::IntToStr(needStamina) +"＿されます", "またチームのスタミナも＿しょうひされます" , "つうしんたいせんを＿はじめますか？" };
                STRVECTOR msg{ "しあいかいしで、チームのスタミナと", "かんとくＨＰ（負" + lib_str::IntToStr(needStamina) + "）が＿しょうひされます" };
                SetHudMsg(TRUE, FALSE, &msg);
                ChangePage(pg_WifiReady);
            }
#else
            ChangePage(pg_WifiSearch);
#endif
        }
        else if (ugbtn_wifi_select_.IsAct())
        {
            stVSHum_.roomNo_ = rl_RoomKbName;//部屋番号

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
            // ここで消費スタミナをチェック
            s32 needStamina = 20;// pmgEO_->mgCoM_.mdm_Connect_.GetPKDt(0, enmdm_Connect::);

            // 監督の消費HPが0以下ならとくにチェックシーンに行かない
            if (needStamina <= 0)
            {
                isStaminaCheck_ = FALSE;
                ChangePage(pg_WifiRoomName);
            }
            else
            {
                isStaminaCheck_ = TRUE;
                STRVECTOR msg{ "しあいかいしで、チームのスタミナと", "かんとくＨＰ（負" + lib_str::IntToStr(needStamina) + "）が＿しょうひされます" };
                SetHudMsg(TRUE, FALSE, &msg);
                ChangePage(pg_WifiReady);
            }
#else
            ChangePage(pg_WifiRoomName);
#endif
        }
    }

#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__
    void TLyHmShVSHum::MvPage_WifiRoomReward()
    {
        if (ugbtn_back_->IsAct())
        {
            ChangePage(pg_WifiRoom);
        }
        else if (ugbtn_ok_->IsAct())
        {
            stVSHum_.roomNo_ = rl_Help_Tushin;
            ChangePage(pg_Wifi_Help);
        }
        else if (ugbtn_wifi_free_.IsAct())
        {
            stVSHum_.roomNo_ = rl_RoomKb;//部屋番号
            stVSHum_.roomid_ = (0x0FFFFFFF - 1 - stVSHum_.roomNo_);

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
            // ここで消費スタミナをチェック
            s32 needStamina = 20;// pmgEO_->mgCoM_.mdm_Connect_.GetPKDt(0, enmdm_Connect::);

            // 監督の消費HPが0以下ならとくにチェックシーンに行かない
            if (needStamina <= 0)
            {
                isStaminaCheck_ = FALSE;
                ChangePage(pg_WifiSearch);
            }
            else
            {
                isStaminaCheck_ = TRUE;
                STRVECTOR msg{ "しあいかいしで、チームのスタミナと", "かんとくＨＰ（負" + lib_str::IntToStr(needStamina) + "）が＿しょうひされます" };
                SetHudMsg(TRUE, FALSE, &msg);
                ChangePage(pg_WifiReady);
            }
#else
            ChangePage(pg_WifiSearch);
#endif
        }
        else if (ugbtn_wifi_select_.IsAct())
        {
            stVSHum_.roomNo_ = rl_RoomKbName;//部屋番号

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__

            // ここで消費スタミナをチェック
            s32 needStamina = 20;// pmgEO_->mgCoM_.mdm_Connect_.GetPKDt(0, enmdm_Connect::);

            // 監督の消費HPが0以下ならとくにチェックシーンに行かない
            if (needStamina <= 0)
            {
                isStaminaCheck_ = FALSE;
                ChangePage(pg_WifiRoomName);
            }
            else
            {
                isStaminaCheck_ = TRUE;
                STRVECTOR msg{ "しあいかいしで、チームのスタミナと", "かんとくＨＰ（負" + lib_str::IntToStr(needStamina) + "）が＿しょうひされます" };
                SetHudMsg(TRUE, FALSE, &msg);
                ChangePage(pg_WifiReady);
            }
#else
            ChangePage(pg_WifiRoomName);
#endif
        }
    }
#endif

    void TLyHmShVSHum::MvPage_WifiReady(void)
    {
        if (IsHudOK())//おーけー
        {
#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
            // チームのスタミナも大丈夫かチェック
            if (CheckMyTmStamina())
            {
                Post_WifiReady();
            }
            else
            {
                // HudにOKを押した時と同一フレームでセットしても閉じられてしまうので一度更新をかける
                frame_->Update(FALSE);

                STRVECTOR msg{ "しようできる＿チームがありません", "チームのスタミナ＿を＿かいふくしてください" };
                SetHudMsg(FALSE, FALSE, &msg);
                ChangePage(pg_WifiReadyError);
            }
#else
            Post_WifiReady();
#endif
        }
        else if (IsHudCancel())//きゃんせる
        {
            ChangePage(pg_WifiRoom);
        }

        if (isSearchStart_)
        {
            ChangePage(pg_WifiSearch);
            isSearchStart_ = FALSE;
        }

            }
    void TLyHmShVSHum::MvPage_WifiReadyError(void)
    {
        if (IsHudOK())//おーけー
        {
            ChangePage(pg_WifiRoom);
        }
    }

    void TLyHmShVSHum::MvPage_WifiRoomName()
    {
        if (frame_->IsHudCancel())
        {
            Backto_Menu();//もどる
        }
        else if (frame_->IsHudOK())//確定ボタンにかえる
        {
            stVSHum_.roomid_ = 0;
            if (stVSHum_.roomNo_ == rl_RoomKbName) stVSHum_.roomid_ = 0x02000000;
            if (stVSHum_.roomNo_ == rl_RoomNmName) stVSHum_.roomid_ = 0x04000000;

            stVSHum_.roomid_ += lib_str::Str1b36codeToInt(frame_->HudOutputString());
            ChangePage(pg_WifiSearch);
        }
    }
    //ワイファイ検索
    void TLyHmShVSHum::MvPage_WifiSearch()
    {
        if (mid::midIsGKError())//エラー検知
        {
            ChangePage(pg_GKError);
        }
        else if (mid::midIsConnect())//相手見つかったら
        {
            SendKantokuData();//送信
            ChangePage(pg_VSHumReady);
        }
        else if (ugbtn_back_->IsAct())
        {
            SetHudVSWaitEnd();
            mid::midCancelSearchWifi();
            //      mid::midSetDisconnect(FALSE);//切断
            ChangePage(pg_WifiRoom);
        }
    }
    //BT部屋選択
    void TLyHmShVSHum::MvPage_BTRoom()
    {
        if (ugbtn_back_->IsAct())
        {
            Backto_Menu();
        }
        else if (ugbtn_ok_->IsAct())
        {
            stVSHum_.roomNo_ = bt_Help_Tushin;
            ChangePage(pg_BT_Help);
        }

#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__
        else if (ugLiReward_.IsAct())
        {
            stVSHum_.rewardRcNo_ = ugLiReward_.GetActIndex();
            ChangePage(pg_BTRoomReward);
        }
#endif

        else if (ugbtn_bt_host_.IsAct())
        {
            stVSHum_.roomNo_ = bt_RoomKb;//部屋番号

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
            // ここで消費スタミナをチェック
            s32 needStamina = pmgEO_->mgCoM_.mdm_Connect_.GetPKDt(0, enmdm_Connect::mdm_connect_staminaMin);

            // 監督の消費HPが0以下ならとくにチェックシーンに行かない
            if (needStamina <= 0)
            {
                isStaminaCheck_ = FALSE;
                ChangePage(pg_BTSearch);
            }
            else
            {
                isStaminaCheck_ = TRUE;
                STRVECTOR msg{ "かんとくＨＰが＿負" + lib_str::IntToStr(needStamina) + "＿されます", "つうしんたいせんを＿はじめますか？" };
                SetHudMsg(TRUE, FALSE, &msg);
                ChangePage(pg_BTReady);
            }
#else
            ChangePage(pg_BTSearch);
#endif
        }
        else if (ugbtn_bt_client_.IsAct())
        {
            stVSHum_.roomNo_ = bt_Search;//部屋番号

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__

            // ここで消費スタミナをチェック
            s32 needStamina = pmgEO_->mgCoM_.mdm_Connect_.GetPKDt(0, enmdm_Connect::mdm_connect_staminaMin);

            // 監督の消費HPが0以下ならとくにチェックシーンに行かない
            if (needStamina <= 0)
            {
                isStaminaCheck_ = FALSE;
                ChangePage(pg_BTSearch);
            }
            else
            {
                isStaminaCheck_ = TRUE;
                STRVECTOR msg{ "かんとくＨＰが＿負" + lib_str::IntToStr(needStamina) + "＿されます", "つうしんたいせんを＿はじめますか？" };
                SetHudMsg(TRUE, FALSE, &msg);
                ChangePage(pg_BTReady);
            }
#else
            ChangePage(pg_BTSearch);
#endif
        }
    }

#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__
    void TLyHmShVSHum::MvPage_BTRoomReward()
    {
        if (ugbtn_back_->IsAct())
        {
            ChangePage(pg_BTRoom);
        }
        else if (ugbtn_ok_->IsAct())
        {
            stVSHum_.roomNo_ = bt_Help_Tushin;
            ChangePage(pg_BT_Help);
        }
        else if (ugbtn_bt_host_.IsAct())
        {
            stVSHum_.roomNo_ = bt_RoomKb;//部屋番号

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
            // ここで消費スタミナをチェック
            s32 needStamina = pmgEO_->mgCoM_.mdm_Connect_.GetPKDt(0, enmdm_Connect::mdm_connect_staminaMin);

            // 監督の消費HPが0以下ならとくにチェックシーンに行かない
            if (needStamina <= 0)
            {
                ChangePage(pg_BTSearch);
            }
            else
            {
                STRVECTOR msg{ "かんとくＨＰが＿負" + lib_str::IntToStr(needStamina) + "＿されます", "つうしんたいせんを＿はじめますか？" };
                SetHudMsg(TRUE, FALSE, &msg);
                ChangePage(pg_BTReady);
            }
#else
            ChangePage(pg_BTSearch);
#endif
        }
        else if (ugbtn_bt_client_.IsAct())
        {
            stVSHum_.roomNo_ = bt_Search;//部屋番号

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
            // ここで消費スタミナをチェック
            s32 needStamina = pmgEO_->mgCoM_.mdm_Connect_.GetPKDt(0, enmdm_Connect::mdm_connect_staminaMin);

            // 監督の消費HPが0以下ならとくにチェックシーンに行かない
            if (needStamina <= 0)
            {
                ChangePage(pg_BTSearch);
            }
            else
            {
                STRVECTOR msg{ "かんとくＨＰが＿負" + lib_str::IntToStr(needStamina) + "＿されます", "つうしんたいせんを＿はじめますか？" };
                SetHudMsg(TRUE, FALSE, &msg);
                ChangePage(pg_BTReady);
            }
#else
            ChangePage(pg_BTSearch);
#endif
        }
    }
#endif

    void TLyHmShVSHum::MvPage_BTReady(void)
    {
        if (IsHudOK())//おーけー
        {
#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
            // チームのスタミナも大丈夫かチェック
            if (CheckMyTmStamina())
            {
                Post_BTReady();
            }
            else
            {
                // HudにOKを押した時と同一フレームでセットしても閉じられてしまうので一度更新をかける
                frame_->Update(FALSE);

                STRVECTOR msg{ "しようできる＿チームがありません", "チームのスタミナ＿を＿かいふくしてください" };
                SetHudMsg(FALSE, FALSE, &msg);
                ChangePage(pg_BTReadyError);
            }
#else
            Post_BTReady();
#endif
        }
        else if (IsHudCancel())//きゃんせる
        {
            ChangePage(pg_BTRoom);
        }

        if (isSearchStart_)
        {
            ChangePage(pg_BTSearch);
            isSearchStart_ = FALSE;
        }
            }

    void TLyHmShVSHum::MvPage_BTReadyError(void)
    {
        if (IsHudOK())//おーけー
        {
            ChangePage(pg_BTRoom);
        }
    }

    //ブルートゥース検索
    void TLyHmShVSHum::MvPage_BTSearch()
    {
        if (mid::midIsGKError())//エラー検知
        {
            ChangePage(pg_GKError);
        }
        else if (ugbtn_back_->IsAct())
        {
            SetHudVSWaitEnd();
            mid::midSetDisconnect(FALSE);//切断
            Backto_Menu();
        }
        else if (mid::midIsConnect())//相手見つかったら
        {
            SendKantokuData();//送信
            ChangePage(pg_VSHumReady);
        }
#ifdef __K_DEBUG_ROOT__
        else
        {
            ChangePage(pg_Match);
        }
#endif
    }
    void TLyHmShVSHum::MvPage_VSHumReady()
    {
        //    //時間切れ切断
        //    if (lib_num::AprTo0(&stVSHum_.discn_c_))
        //    {
        //      ChangePage(pg_GKError);
        //      return;
        //    }

        if (mid::midIsDisconnect())//切断検知
        {
            ChangePage(pg_GKError);
        }
        else if (stVSHum_.id_kantoku_en_ != 0)//敵の監督情報受信した
        {
            if (stVSHum_.wifi_f_)
            {
                s64 mysideno = (stVSHum_.id_team_my_ * stVSHum_.id_kantoku_en_) % 1000;
                s64 ensideno = (stVSHum_.id_team_en_ * pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_pk)) % 1000;

                BOOL sv_f = (mysideno == ensideno)
                    ? (pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_pk) > stVSHum_.id_kantoku_en_)
                    : (mysideno > ensideno);

                mid::midSetPeerSide(sv_f);
            }
            SendKantokuData();//送信
            Post_VSHumReady();
        }
        else
        {
            stVSHum_.senditv_++;
            if (stVSHum_.senditv_ % 60 == 0) SendKantokuData();//送信
        }


    }
    void TLyHmShVSHum::MvPage_Match()
    {
        lib_num::AprTo0(&stVSHum_.wait_c_);
        //カウントダウン
        if ((stVSHum_.wait_c_ % ONESEC) == 0)
        {
            SetHudCount(stVSHum_.wait_c_ / ONESEC);
        }

        if (mid::midIsDisconnect())//切断検知
        {
            ChangePage(pg_GKError);
        }
        else if (ugbtn_ok_->IsAct() || (stVSHum_.wait_c_ == 0))
        {
            SendKantokuData();//送信
            ChangePage(pg_TmSelect);

            stVSHum_.wait_c_ = TSLTIME;
            stVSHum_.discn_c_ = (TSLTIME + WAITTIME + WAITTIME);

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
            // チームのスタミナチェックがあるならカウント表示消す
            if (isStaminaCheck_)
            {
                SetHudCountEnd();
            }
            else
            {
                SetHudCount(stVSHum_.wait_c_ / ONESEC);
            }
#else
            SetHudCount(stVSHum_.wait_c_ / ONESEC);
#endif
        }

    }
    void TLyHmShVSHum::MvPage_TmSelect()
    {
#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
        // 更新
        CheckSelectTmStamina();

        // チームのスタミナチェックがない時はカウントダウン
        if (!isStaminaCheck_)
#endif
        {
            //自分のチームが決まってないときはカウントダウン
            if (stVSHum_.id_team_my_ == 0)
            {
                lib_num::AprTo0(&stVSHum_.wait_c_);
                //カウントダウン
                if ((stVSHum_.wait_c_ % ONESEC) == 0)
                {
                    SetHudCount(stVSHum_.wait_c_ / ONESEC);
                }
        }

            //時間切れ切断
            if (lib_num::AprTo0(&stVSHum_.discn_c_))
            {
                ChangePage(pg_GKError);
                return;
            }
            }


        if (mid::midIsDisconnect())//切断検知
        {
            ChangePage(pg_GKError);
        }
        else if (stVSHum_.id_team_my_ == 0)  //自分のチームが決まってない
        {
            //ドラムアクション
            //if (ugdrTeam_.IsActRes(enUGDrTeamAct_Roll))//ドラム回転＆カウンタ処理
            //{
            //  //ugdrTeam_.RefreshRoll(FALSE);
            //}
            //else
            if (ugdrTeam_.IsActRes(enUGDrTeamAct_Stop))//読み込みが必要なタイミング
            {
                stVSHum_.id_team_dr_ = ugdrTeam_.GetIDTeam();
                stVSHum_.mid_team_dr_ = ugdrTeam_.GetMIDTeam();
                //キャプテンとメンバーの画像をロード
                //LoadLookPosTeam();
            }
            else if (ugbtn_ok_->IsAct() || (stVSHum_.wait_c_ == 0))
            {

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
                // スタミナ不足
                if (stVSHum_.isNeedStamina_)
                {
                    stVSHum_.cursorDispTimer_ = 60;
                    pCursor_->g_SetDraw(TRUE);
                    return;
                }
#endif

                DecideMyTeam();
            }
            else if (ugbtn_mid_->IsAct() && (stVSHum_.wait_c_ > ABLETMFIL))//シュートセットボタン
            {
                ChangePage(pg_ShSetSelect);
                uglistShSetSelectAll_.Refresh(stVSHum_.id_team_dr_, NGNUM, shsetvec_);
                uglistMember_.Refresh(stVSHum_.id_team_dr_, stVSHum_.mid_team_dr_, FALSE);
                stVSHum_.shsetmem_f_ = FALSE;
                stVSHum_.id_shset_first_ = uglistShSetSelectAll_.GetShSet();

            }
            else if (ugbtn_next_->IsAct() && (stVSHum_.wait_c_ > ABLETMFIL))
            {
                ChangePage(pg_Filter);//ならびかえ
            }

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
            else if (ugbtn_mgk_->IsAct())//ＭＧＫボタン
            {
                ChangePage(pg_Mgk);
            }
#endif

        }
        else
        {
#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
            // チームのスタミナチェックがない時はカウントダウン
            if (!isStaminaCheck_)
#endif
            {
                if (lib_num::AprTo0(&stVSHum_.wait_c_))
                {
                    ChangePage(pg_GKError);
                }
            }

            //敵と自分のチームが判明
            if ((stVSHum_.id_team_en_ != 0) && (stVSHum_.id_team_my_ != 0))
            {
                stVSHum_.ready_f_my_ = TRUE;
                SetHudVSWaitEnd();
                SendKantokuData();//送信
                Post_VSHumPlay();

                s32 delay = pmgEO_->mgCoM_.mdm_Client_.GetLineDt(0, mdm_client_vsbuf);
                s32 mid_connect = 0;
                if (stVSHum_.wifi_f_)
                {
                    mid_connect = (Is30FPS())
                        ? 2
                        : 3;
                }
                else//BT
                {
                    mid_connect = (Is30FPS())
                        ? 0
                        : 1;
                }

                mid::midSetConnectData(
                    pmgEO_->mgCoM_.mdm_Connect_.GetLineDt(mid_connect, mdm_connect_delay),
                    pmgEO_->mgCoM_.mdm_Connect_.GetLineDt(mid_connect, mdm_connect_buf),
                    pmgEO_->mgCoM_.mdm_Connect_.GetLineDt(mid_connect, mdm_connect_senditv),
                    pmgEO_->mgCoM_.mdm_Connect_.GetLineDt(mid_connect, mdm_connect_rollitv),
                    pmgEO_->mgCoM_.mdm_Connect_.GetLineDtBOOL(mid_connect, mdm_connect_poscheck_f)
                    );
            }
                }
            }

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
    //みなげんき
    void TLyHmShVSHum::MvPage_MGK()
    {
        if (IsHudOK())//おーけー
        {
            //POST：合成ＮＧ
            Post_MGK();
        }
        else if (IsHudCancel())//きゃんせる
        {
            ChangePage(pg_TmSelect);
        }
    }
#endif

    //シュートセット切り替え
    void TLyHmShVSHum::MvPage_ShSetSelect()
    {
#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
        // チームのスタミナチェックがない時はカウントダウン
        if (!isStaminaCheck_)
#endif
        {

            //自分のチームが決まってないときはカウントダウン
            if (stVSHum_.id_team_my_ == 0)
            {
                lib_num::AprTo0(&stVSHum_.wait_c_);
                //カウントダウン
                if ((stVSHum_.wait_c_ % ONESEC) == 0)
                {
                    SetHudCount(stVSHum_.wait_c_ / ONESEC);
                }
        }

            //時間切れ切断
            if (lib_num::AprTo0(&stVSHum_.discn_c_))
            {
                ChangePage(pg_GKError);
                return;
            }
        }

        if (mid::midIsDisconnect())//切断検知
        {
            ChangePage(pg_GKError);
        }
#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
        else if (stVSHum_.wait_c_ <= ABLETMFIL && !isStaminaCheck_)//時間切れ決定
#else
        else if (stVSHum_.wait_c_ <= ABLETMFIL)//時間切れ決定
#endif
        {
            //DecideMyTeam();
            ChangePage(pg_TmSelect);
        }
        else
        {
            if (stVSHum_.shsetmem_f_)//個別
            {

                if (ugbtn_back_->IsAct())//おーけー
                {
                    if (stVSHum_.id_shset_first_ != uglistShSetSelect_.GetShSet())
                    {
                        stVSHum_.id_shset_ = uglistShSetSelect_.GetShSet();
                        Post_SwitchShSet();
                    }
                    else
                    {
                        ChangePage(pg_TmSelect);
                    }
                }
                else if (uglistMember_.IsAct())//個別に
                {
                    stVSHum_.id_shset_ = uglistShSetSelect_.GetShSet();
                    if (uglistMember_.IsSelected())
                    {
                        uglistShSetSelect_.Refresh(stVSHum_.id_team_dr_, stVSHum_.id_shset_, uglistMember_.GetActIndex(), shsetvec_);
                    }
                    else
                    {
                        uglistShSetSelectAll_.Refresh(stVSHum_.id_team_dr_, stVSHum_.id_shset_, shsetvec_);//全体に戻る
                    }
                }
            }
            else//全体
            {
                if (ugbtn_back_->IsAct())//おーけー
                {
                    if (stVSHum_.id_shset_first_ != uglistShSetSelectAll_.GetShSet())
                    {
                        stVSHum_.id_shset_ = uglistShSetSelectAll_.GetShSet();
                        Post_SwitchShSet();
                    }
                    else
                    {
                        ChangePage(pg_TmSelect);
                    }
                }
                else if (uglistMember_.IsAct())//個別に
                {
                    stVSHum_.id_shset_ = uglistShSetSelectAll_.GetShSet();
                    uglistShSetSelect_.Refresh(stVSHum_.id_team_dr_, stVSHum_.id_shset_, uglistMember_.GetActIndex(), shsetvec_);
                }
            }

            stVSHum_.shsetmem_f_ = uglistMember_.IsSelected();

            uglistShSetSelectAll_.SetDraw(!uglistMember_.IsSelected());
            uglistShSetSelect_.SetDraw(uglistMember_.IsSelected());
        }
        }

    void TLyHmShVSHum::MvPage_TmFilter()
    {
#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
        // チームのスタミナチェックがない時はカウントダウン
        if (!isStaminaCheck_)
#endif
        {

            //自分のチームが決まってないときはカウントダウン
            if (stVSHum_.id_team_my_ == 0)
            {
                lib_num::AprTo0(&stVSHum_.wait_c_);
                //カウントダウン
                if ((stVSHum_.wait_c_ % ONESEC) == 0)
                {
                    SetHudCount(stVSHum_.wait_c_ / ONESEC);
                }
        }

            //時間切れ切断
            if (lib_num::AprTo0(&stVSHum_.discn_c_))
            {
                ChangePage(pg_GKError);
                return;
            }
    }

        if (mid::midIsDisconnect())//切断検知
        {
            ChangePage(pg_GKError);
        }
#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
        else if (stVSHum_.wait_c_ <= ABLETMFIL && !isStaminaCheck_)//時間切れ決定
#else
        else if (stVSHum_.wait_c_ <= ABLETMFIL)//時間切れ決定
#endif
        {
            //DecideMyTeam();
            ChangePage(pg_TmSelect);
        }
        else if (ugbtn_back_->IsAct())//おーけー
        {
            ChangePage(pg_TmSelect);
        }
        else if (uglistTmFilter_.IsActRes2(enUGLiTmFilter_Filter))
        {
            ugdrTeam_.ReadyFilter();
        }
        else if (uglistTmFilter_.IsActRes2(enUGLiTmFilter_Sort))
        {
            ugdrTeam_.ReadySort();
        }
        }


    void TLyHmShVSHum::DecideMyTeam()
    {
        stVSHum_.id_team_dr_ = ugdrTeam_.GetIDTeam();
        stVSHum_.mid_team_dr_ = ugdrTeam_.GetMIDTeam();
        stVSHum_.id_team_my_ = stVSHum_.id_team_dr_;//自分のチーム
        ugbtn_ok_->SetDraw(FALSE);//もう押せない
        ugbtn_prev_->SetDraw(FALSE);//もう押せない
        ugbtn_next_->SetDraw(FALSE);//もう押せない
        ugbtn_mid_->SetDraw(FALSE);//もう押せない
        ugbtn_back_->SetDraw(FALSE);//もう押せない
        ugbtn_mgk_->SetDraw(FALSE);//もう押せない

        s64 mid_team = pmgEO_->mgCoU_.mdTeam_.GetPKDt(stVSHum_.id_team_my_, mdteam_id_m_team);
        BOOL gb_f = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_gb_f);
        for (s32 c = 0; c < DBMEMBER_ALL; ++c)
        {
            s32 charid = pmgEO_->mgCoM_.GetCharID(mid_team, c);
            pmgEO_->mgGrp_.pmgChGrp_[SIDE0][c]->SetCharDt(charid, mid_team, gb_f);
            pmgEO_->mgGrp_.pmgChGrp_[SIDE0][c]->SetGrTile(SIDE0, c);
        }
        pmgEO_->mgGrp_.MakeIconTexture_Chest(SIDE0, FALSE, FALSE);
        pmgEO_->mgGrp_.MakeIconTexture_Body(SIDE0, 0, 0, TRUE, FALSE);

        s64 id_kantoku = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(enmdKantoku::mdkantoku_pk);
        ugTeam_.SetTeamDt3(id_kantoku, stVSHum_.id_team_my_, mid_team, NGNUM, TRUE);
        ugTeam_.SetTipsDraw_f(TRUE);
        ugTeam_.SetDraw(TRUE);

        ugdrTeam_.SetDraw(FALSE);
        SendKantokuData();//送信
        //お待ちください画面を出す
        SetHudVSWait_EnWait();

        stVSHum_.wait_c_ = TSLTIME;
    }

    void TLyHmShVSHum::MvPage_GotoVS()
    {
    }
    void TLyHmShVSHum::MvPage_VS()
    {
        lib_num::AprTo0(&stVSHum_.wait_c_);

        //カウントダウン
        if ((stVSHum_.wait_c_ % ONESEC) == 0)
        {
            SetHudCount(stVSHum_.wait_c_ / ONESEC);
        }

        //時間切れ切断
        if (lib_num::AprTo0(&stVSHum_.discn_c_))
        {
            //ChangePage(pg_GKError);
            ChangePage(pg_GotoShiai);
            return;
        }

        if (mid::midIsDisconnect())//切断検知
        {
            //ChangePage(pg_GKError);
            ChangePage(pg_GotoShiai);
        }
        else if (stVSHum_.ready_f_en_ && (field_->sc_IsCamEnter() || (stVSHum_.wait_c_ == 0))) //さわったら即
        {
            ChangePage(pg_GotoShiai);
        }
    }
    void TLyHmShVSHum::MvPage_GotoShiai()
    {
    }
    void TLyHmShVSHum::MvPage_Out()
    {
        Down();
    }
    void TLyHmShVSHum::MvPage_Error()
    {
        if (ugbtn_back_->IsAct())
        {
            Backto_Menu();
        }
    }
    void TLyHmShVSHum::MvPage_GKError()
    {
        if (IsHudOK())//おーけー
        {
            Backto_Menu();
        }
    }

    void TLyHmShVSHum::MvPage_Wifi_Help(void)
    {
        if (mid::midIsDrawModalPanel() == FALSE)
        {
            Backto_Menu();
        }
    }
    void TLyHmShVSHum::MvPage_BT_Help(void)
    {
        if (mid::midIsDrawModalPanel() == FALSE)
        {
            Backto_Menu();
        }
    }

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
    BOOL TLyHmShVSHum::CheckMyTmStamina(void)
    {
        BOOL isOK = FALSE;

        s64 teamNum = pmgEO_->mgCoU_.mdTeam_.GetLineNum();
        for (s32 i = 0; i < teamNum; i++)
        {
            s32 mid_team = pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_id_m_team);		// チームマスターのID
            s32 id_team = pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_pk);				// ユーザーの中のチームID

            const s32 MINTOSEC = 60;
            //const s32 MAXSTA = 100;

            s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_rarity);
            s32 waittime = pmgEO_->mgCoM_.mdm_Rarity_.GetLineDt(rarity, mdm_rarity_staminaLim);

            s64 nowwait = waittime;

            s64 nowtime = mid::midNowTime();
            s64 endtime = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_useSec);
            nowwait = (nowtime - endtime) / MINTOSEC;
            if (nowwait > waittime) nowwait = waittime;

            // 使えるチームが一つでもあればOK
            if (nowwait == waittime)
            {
                isOK = TRUE;
                break;
            }
        }
        return isOK;
    }

    void TLyHmShVSHum::CheckSelectTmStamina(void)
    {
        ugdrTeam_.stDrGen_.recno_;
        s64 id_team = ugdrTeam_.GetIDTeam();
        s64 mid_team = ugdrTeam_.GetMIDTeam();

        if (id_team == NGNUM) { return; }				//敵

        const s32 MINTOSEC = 60;
        //const s32 MAXSTA = 100;

        s32 rarity = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_rarity);
        s32 waittime = pmgEO_->mgCoM_.mdm_Rarity_.GetLineDt(rarity, mdm_rarity_staminaLim);

        s64 nowwait = waittime;

        s64 nowtime = mid::midNowTime();
        s64 endtime = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_useSec);
        nowwait = (nowtime - endtime) / MINTOSEC;
        if (nowwait > waittime) nowwait = waittime;

        // 満タンじゃない
        BOOL mgk_f = (nowwait != waittime);
        if (mgk_f)
        {
            if (!stVSHum_.isNeedStamina_)
            {
                stVSHum_.isNeedStamina_ = TRUE;
                if (stVSHum_.isMGKBtn_ == FALSE)
                {
                    ugbtn_ok_->SetFont("ＳＴかいふく");
                    stVSHum_.isMGKBtn_ = TRUE;
                }
            }
        }
        else
        {
            if (stVSHum_.isNeedStamina_)
            {
                stVSHum_.isNeedStamina_ = FALSE;
                if (stVSHum_.isMGKBtn_)
                {
                    ugbtn_ok_->SetFont("けってい");
                    stVSHum_.isMGKBtn_ = FALSE;
                }
            }
        }
    }

    void TLyHmShVSHum::UpdateCursor(BOOL draw_f)
    {
        if (stVSHum_.isNeedStamina_)
        {
            if (stVSHum_.cursorDispTimer_ < 0)
            {
                pCursor_->g_SetDraw(FALSE);
                stVSHum_.cursorDispTimer_ = -1;
            }
            else
            {
                stVSHum_.cursorDispTimer_--;
            }
        }
        pCursor_->Update(draw_f);
    }
#endif

    //送信
    void TLyHmShVSHum::SendKantokuData()
    {
        //相手の準備完了
        //if (stVS_.ready_f_en_) return;

        mid::TStMidKantoku* mykantoku = mid::midGetStMyKantoku();
        mykantoku->id_kantoku_ = pmgEO_->mgCoU_.mdKantoku_.GetMyDt(mdkantoku_pk);
        mykantoku->id_team_ = stVSHum_.id_team_my_;
        mykantoku->ready_f_ = stVSHum_.ready_f_my_;
        mykantoku->roomtype_ = stVSHum_.roomtype_;
        mid::midSendKantoku();
        }

    //受信
    void TLyHmShVSHum::RcvData()
    {
        mid::TStMidKantoku* enkantoku = mid::midGetStEnKantoku();
        if (enkantoku->id_kantoku_ != 0) stVSHum_.id_kantoku_en_ = enkantoku->id_kantoku_;//敵の監督
        if (enkantoku->id_team_ != 0) stVSHum_.id_team_en_ = enkantoku->id_team_;//敵のチーム
        if (enkantoku->ready_f_) stVSHum_.ready_f_en_ = enkantoku->ready_f_;//敵の準備完了
        //ブルートゥースでクライアント側は相手のきびなめせっていを受け取る
        if ((stVSHum_.wifi_f_ == FALSE) && (stVSHum_.roomtype_ == rt_Search))
        {
            //一回でもfps30がきたらそれで有効
            if (enkantoku->roomtype_ != rt_Search) stVSHum_.roomtype_ = enkantoku->roomtype_;
        }

        //接続無しテスト
        if (NOCONNECT_F)
        {
            stVSHum_.id_kantoku_en_ = 1;
            stVSHum_.id_team_en_ = 1;
            stVSHum_.ready_f_en_ = TRUE;
        }
    }

    //*******************
    //POSTVS敵情報取得
    //POST：シュートセット切り替え
    void TLyHmShVSHum::Post_SwitchShSet()
    {
        StartConnect(cn_SwitchShSet);
        pmgEO_->mgPo_.poHmTeam_.Ready_switchshset(stVSHum_.id_team_dr_, stVSHum_.id_shset_);
    }
    void TLyHmShVSHum::Post_VSHumReady()
    {
        StartConnect(cn_VSHumReady);
        pmgEO_->mgPo_.poHmShiai_.Ready_vshumready(stVSHum_.id_kantoku_en_, stVSHum_.wifi_f_, stVSHum_.roomNo_);
    }
    //POSTVS選択チーム
    void TLyHmShVSHum::Post_VSHumPlay()
    {
        StartConnect(cn_VSHumPlay);
        pmgEO_->mgPo_.poHmShiai_.Ready_vshumplay(stVSHum_.id_team_en_, stVSHum_.id_team_my_);
    }

    // 
    void TLyHmShVSHum::Post_WifiReady()
    {
        if (pmgEO_->stSeq_.IsNoSv())
        {
            Connect_WifiReady();
        }
        else
        {
            StartConnect(cn_WifiReady);
            pmgEO_->mgPo_.poHmShiai_.Ready_vshumcheck();
        }
    }
    // 
    void TLyHmShVSHum::Post_BTReady()
    {
        if (pmgEO_->stSeq_.IsNoSv())
        {
            Connect_BTReady();
        }
        else
        {
            StartConnect(cn_BTReady);
            pmgEO_->mgPo_.poHmShiai_.Ready_vshumcheck();
        }
    }

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
    //POST：ＭＧＫ
    void TLyHmShVSHum::Post_MGK()
    {
        if (pmgEO_->stSeq_.IsNoSv())
        {
            Connect_MGK();
        }
        else
        {
            StartConnect(cn_mgk);
            if (stVSHum_.mgknum_ == 0)
            {
                pmgEO_->mgPo_.poHmTeam_.Ready_mgkquick(stVSHum_.id_team_dr_);
            }
            else
            {
                pmgEO_->mgPo_.poHmTeam_.Ready_mgk(stVSHum_.id_team_dr_);
            }
        }
    }
#endif

    //通信：シュートセット切り替え
    void TLyHmShVSHum::Connect_SwitchShSet()
    {
        ugdrTeam_.RefreshStamina();
        ChangePage(pg_TmSelect);
    }
    void TLyHmShVSHum::Connect_VSHumReady()
    {
        //他人情報入りました
        pmgEO_->mgCoU_.SetOtherKantoku();
        ChangePage(pg_Match);
    }
    void TLyHmShVSHum::Connect_VSHumPlay()
    {
        ChangePage(pg_GotoVS);
    }

    // 
    void TLyHmShVSHum::Connect_WifiReady()
    {
        frame_->ugheadder_->Refresh(FALSE);//HPリフレッシュ
        if (stVSHum_.roomNo_ == rl_RoomKb)
        {
            isSearchStart_ = TRUE;
            //ChangePage(pg_WifiSearch);
        }
        else
        {
            ChangePage(pg_WifiRoomName);
        }
    }
    // 
    void TLyHmShVSHum::Connect_BTReady()
    {
        frame_->ugheadder_->Refresh(FALSE);//HPリフレッシュ
        //ChangePage(pg_BTSearch);
        isSearchStart_ = TRUE;
    }

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
    //接続：ＭＧＫ
    void TLyHmShVSHum::Connect_MGK()
    {
        ugdrTeam_.RefreshStamina();
        ugbtn_mgk_->Refresh();
        SetHudEnd();
        ChangePage(pg_TmSelect);
    }
#endif

    //接続完了
    void TLyHmShVSHum::DoConnect(BOOL rqok_f)
    {
        if (rqok_f)//ＯＫのみ
        {
            //code0
            if (pmgEO_->mgPo_.IsCodeZero())
            {
                switch (stLy_.connect_)
                {
                case cn_SwitchShSet: Connect_SwitchShSet(); break;
                case cn_VSHumReady: Connect_VSHumReady(); break;
                case cn_VSHumPlay: Connect_VSHumPlay(); break;
                case cn_WifiReady: Connect_WifiReady(); break;
                case cn_BTReady: Connect_BTReady(); break;
#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
                case cn_mgk: Connect_MGK(); break;
#endif
                }
            }
            else
            {
                switch (stLy_.connect_)
                {
                case cn_SwitchShSet:
                    ChangePage(pg_TmSelect);
                    break;
                case cn_VSHumReady:
                case cn_VSHumPlay:
                    Backto_Menu();
                    mid::midSetDisconnect(FALSE);//切断
                    break;
                case cn_WifiReady:
                    ChangePage(pg_WifiRoom);
                    break;
                case cn_BTReady:
                    ChangePage(pg_BTRoom);
                    break;

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
                case cn_mgk:
                    ChangePage(pg_TmSelect);
                    break;
#endif

            }
                //エラーメッセージ(ここでstLy_.connect_のクリアが入ってしまう)
                EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());

            }
        }
        else
        {
            //通信エラー
            SetHudEnd();
            switch (stLy_.connect_)
            {
            case cn_SwitchShSet:
                ChangePage(pg_TmSelect);
                break;
            case cn_VSHumReady:
            case cn_VSHumPlay:
                Backto_Menu();
                mid::midSetDisconnect(FALSE);//切断
                break;
            case cn_WifiReady:
                ChangePage(pg_WifiRoom);
                break;
            case cn_BTReady:
                ChangePage(pg_BTRoom);
                break;

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
            case cn_mgk:
                ChangePage(pg_TmSelect);
                break;
#endif

            }

        }
    }
    //*******************

    BOOL TLyHmShVSHum::IsGotoVS()
    {
        return (stLy_.pageNo_ == pg_GotoVS);
    }
    void TLyHmShVSHum::SetVS()
    {
        ChangePage(pg_VS);
    }

    void TLyHmShVSHum::DoUpdate(BOOL draw_f)
    {
        switch (stLy_.pageNo_)
        {
        case db::pg_GKError:
        case db::pg_Error:
            //なにもださない
            break;
        case db::pg_GotoShiai:
        case db::pg_VS:
            //VS画面
            ugvs_.Update(draw_f);
            for (s32 i = 0; i < ugvsteam_.size(); ++i) ugvsteam_[i]->Update(draw_f);
            break;
        case db::pg_Filter:
            ugdrTeam_.Update(draw_f);
            uglistTmFilter_.Update(draw_f);
            break;
        case db::pg_ShSetSelect:
            uglistShSetSelect_.Update(draw_f);
            uglistShSetSelectAll_.Update(draw_f);
            uglistMember_.Update(draw_f);
            break;
        default:
            ugprof_.Update(draw_f);
            uglistWifiRoom_.Update(draw_f);
            uglistBTRoom_.Update(draw_f);
            ugshbtn_bt_.Update(draw_f);
            ugshbtn_wifi_.Update(draw_f);
            ugdrTeam_.Update(draw_f);
            ugTeam_.Update(draw_f);
            ugrmsg_bt_.Update(draw_f);
            ugrmsg_wifi_.Update(draw_f);
            waitTxt_->Update(draw_f);

#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__
            ugtitle_reward_.Update(draw_f);
            ugLiReward_.Update(draw_f);
            for (s32 i = 0; i < ugLiRewardInfo_.size(); i++)
            {
                ugLiRewardInfo_[i]->Update(draw_f);
            }
#endif

            ugbtn_bt_host_.Update(draw_f);
            ugbtn_bt_client_.Update(draw_f);
            ugbtn_wifi_free_.Update(draw_f);
            ugbtn_wifi_select_.Update(draw_f);

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
            UpdateCursor(draw_f);
#endif
            break;
        }

        if (mid::midIsConnect()) RcvData();

        if (draw_f == FALSE)
        {
            switch (stLy_.pageNo_)
            {
            case db::pg_Visit:MvPage_Visit(); break;
            case db::pg_Menu:MvPage_Menu(); break;
            case db::pg_WifiRoom:MvPage_WifiRoom(); break;

#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__
            case db::pg_WifiRoomReward:MvPage_WifiRoomReward(); break;
#endif

            case db::pg_WifiReady:MvPage_WifiReady(); break;
            case db::pg_WifiReadyError:MvPage_WifiReadyError(); break;
            case db::pg_WifiRoomName:MvPage_WifiRoomName(); break;
            case db::pg_WifiSearch:MvPage_WifiSearch(); break;
            case db::pg_BTRoom:MvPage_BTRoom(); break;

#ifndef __K_DEBUG_HIDE_VSHUM_REWARD__
            case db::pg_BTRoomReward:MvPage_BTRoomReward(); break;
#endif

            case db::pg_BTReady:MvPage_BTReady(); break;
            case db::pg_BTReadyError:MvPage_BTReadyError(); break;
            case db::pg_BTSearch:MvPage_BTSearch(); break;
            case db::pg_VSHumReady:MvPage_VSHumReady(); break;
            case db::pg_Match:MvPage_Match(); break;
            case db::pg_TmSelect:MvPage_TmSelect(); break;

#ifdef __K_DEBUG_VSHUM_NEED_STAMINA__
            case db::pg_Mgk:MvPage_MGK(); break;
#endif

            case db::pg_ShSetSelect:MvPage_ShSetSelect(); break;
            case db::pg_Filter:MvPage_TmFilter(); break;
            case db::pg_GotoVS:MvPage_GotoVS(); break;
            case db::pg_VS:MvPage_VS(); break;
            case db::pg_GotoShiai:MvPage_GotoShiai(); break;
            case db::pg_Out:MvPage_Out(); break;
            case db::pg_GKError:MvPage_GKError(); break;
            case db::pg_Error:MvPage_Error(); break;
            case db::pg_Wifi_Help:MvPage_Wifi_Help(); break;
            case db::pg_BT_Help:MvPage_BT_Help(); break;
            }
    }
        }
    }


