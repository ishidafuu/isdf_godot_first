#include "dbMgCoUser.h"
#include "dbMgEngineObj.h"
namespace db
{


    TMgCoUser::TMgCoUser(TMgEngineObj* pmgEO) :TMgCoGen(pmgEO, "co_user")
    {
        stCoUser_.Init();

        mdvec_.clear();

        mdvec_.push_back(&mdre_Ranker_);//返信ランキング
        mdvec_.push_back(&mdre_Enemy_);//返信ＶＳ敵監督
        mdvec_.push_back(&mdre_Refresh_);//ホーム更新系
        mdvec_.push_back(&mdKantoku_);//監督（通信の度に参照、チェックされるようなもの）
        mdvec_.push_back(&mdKantokuSub_);//監督データ（主に更新のみされてクライアントに表示だけされてればよいもの）
        mdvec_.push_back(&mdShSet_);//必殺セット
        mdvec_.push_back(&mdTeam_);//チーム
        mdvec_.push_back(&mdTeamCharLv_);//チーム選手レベル
        mdvec_.push_back(&mdShot_);//シュート（インベントリ）
        mdvec_.push_back(&mdDock_);//合成ドック
        mdvec_.push_back(&mdSupply_);//消耗品
        mdvec_.push_back(&mdBGM_);//タイトル（取得情報）
        mdvec_.push_back(&mdHaikei_);//背景（取得情報）
        mdvec_.push_back(&mdGiftbox_);//贈り物ＢＯＸ（バッチ廃棄必要）
        mdvec_.push_back(&mdMaptip_);//シナリオマップチップ
        mdvec_.push_back(&mdInvite_);//招待
        mdvec_.push_back(&mdNaBox_);//なつかしＢＯＸ
        mdvec_.push_back(&mdMap_);//マップ
        mdvec_.push_back(&mdVSDrop_);//対戦ドロップ
        mdvec_.push_back(&mdlog_BStar_);//購入スターログ兼管理
        mdvec_.push_back(&mdlog_Event_);//イベントログ兼管理
        mdvec_.push_back(&mdlog_ShiaiMap_);//試合ログ兼管理
        mdvec_.push_back(&mdlog_ShiaiHum_);//試合ログ対人戦兼管理
        mdvec_.push_back(&mdlog_ShiaiTour_);//試合ログ疑似対人兼管理
        mdvec_.push_back(&mdlog_ShiaiTmat_);//試合ログタイムアタック兼管理
        mdvec_.push_back(&mdlog_Maruka_);//まるかログ兼管理
        mdvec_.push_back(&mdlog_ShotDel_);//シュート削除ログ
        mdvec_.push_back(&mdlog_ItemGet_);//アイテム取得ログ
        mdvec_.push_back(&mdlog_SupplyUse_);//消耗品使用ログ
        mdvec_.push_back(&mdlog_CpCode_);//キャンペーンコード取得ログ兼管理
        //mdvec_.push_back(&mdlog_CharLvUp_);//キャラレベルアップログ
        mdvec_.push_back(&mdw_Version_);//マスターバージョン管理#キャッシュでよいのでは
        mdvec_.push_back(&mdw_Event_);//イベント回数管理
        mdvec_.push_back(&mdw_HukuGrp_);//福引きグループ管理
        mdvec_.push_back(&mdw_HukuCnt_);//福引き中身カウンタ
        mdvec_.push_back(&mdw_log_TousenGacha_);//ガチャ当選ログ
        mdvec_.push_back(&mdw_log_TousenHuku_);//ふくびき当選ログ


        //通信のたびにクリアするログなど
        mdvec_log_.clear();
        mdvec_log_.push_back(&mdre_Ranker_);//ランキング
        mdvec_log_.push_back(&mdre_Enemy_);//返信ＶＳ敵監督
        mdvec_log_.push_back(&mdre_Refresh_);//ホーム更新系

        mdvec_log_.push_back(&mdlog_BStar_);//購入スターログ兼管理
        mdvec_log_.push_back(&mdlog_Event_);//イベントログ兼管理
        mdvec_log_.push_back(&mdlog_ShiaiMap_);//試合ログ
        mdvec_log_.push_back(&mdlog_ShiaiHum_);//試合ログ対人戦
        mdvec_log_.push_back(&mdlog_ShiaiTour_);//試合ログ疑似対人
        mdvec_log_.push_back(&mdlog_ShiaiTmat_);//試合ログタイムアタック兼管理
        mdvec_log_.push_back(&mdlog_Maruka_);//まるか素材件ログ
        mdvec_log_.push_back(&mdlog_ShotDel_);//シュート削除ログ
        mdvec_log_.push_back(&mdlog_ItemGet_);//アイテム取得ログ
        mdvec_log_.push_back(&mdlog_SupplyUse_);//消耗品使用ログ
        mdvec_log_.push_back(&mdlog_CpCode_);//キャンペーンコード取得ログ

        mdvec_log_.push_back(&mdw_log_TousenGacha_);//ガチャ当選ログ
        mdvec_log_.push_back(&mdw_log_TousenHuku_);//ふくびき当選ログ


        models_.clear();
        for (s32 i = 0; i < mdvec_.size(); ++i)
        {
            models_.push_back(&mdvec_[i]->model_);
        }

        stCoUser_.otherkantoku_f_ = FALSE;

    }

    TMgCoUser::~TMgCoUser(void)
    {

    }

    void TMgCoUser::DoInitSendPack()
    {
        //チェック不要＆再生成
        sendmap_.longmap_["scheck"] = enSessionCheckType_Reset;
        if (mid::midIsIOS())
        {
            sendmap_.longmap_["os"] = 1;
        }
        else if (mid::midIsADR())
        {
            sendmap_.longmap_["os"] = 2;
        }
    }

    //ログを削除
    void TMgCoUser::CleanLog()
    {
        for (s32 i = 0; i < mdvec_log_.size(); ++i)
        {
            mdvec_log_[i]->model_.Clear();
        }
    }

    //全て削除(ログアウト時)
    void TMgCoUser::CleanAll(void)
    {
        for (s32 i = 0; i < mdvec_.size(); ++i)
        {
            mdvec_[i]->model_.Clear();
        }
    }
    //他人監督情報が入った
    void TMgCoUser::SetOtherKantoku(void)
    {
        stCoUser_.otherkantoku_f_ = TRUE;
    }
    //他人監督情報を全て削除（シュート、シュートセット、チーム）
    void TMgCoUser::CleanOtherKantoku(void)
    {
        if (stCoUser_.otherkantoku_f_ == FALSE) return;

        s64 id_kantoku = mdKantoku_.GetMyDt(mdkantoku_pk);

        //消す安に削除フラグを立てる
        //シュートセット
        for (s32 i = 0; i < mdShSet_.GetLineNum(); ++i)
        {
            if (mdShSet_.GetLineDt(i, mdshset_id_kantoku) != id_kantoku)
            {
                mdShSet_.model_.delvec_.push_back(mdShSet_.GetLineDt(i, mdshset_pk));
            }
        }
        //シュート
        for (s32 i = 0; i < mdShot_.GetLineNum(); ++i)
        {
            if (mdShot_.GetLineDt(i, mdshot_id_kantoku) != id_kantoku) //ついてるシュート
            {
                mdShot_.model_.delvec_.push_back(mdShot_.GetLineDt(i, mdshot_pk));
            }
        }

        //チーム
        for (s32 i = 0; i < mdTeam_.GetLineNum(); ++i)
        {
            if (mdTeam_.GetLineDt(i, mdteam_id_kantoku) != id_kantoku) //ついてるシュート
            {
                mdTeam_.model_.delvec_.push_back(mdTeam_.GetLineDt(i, mdteam_pk));
            }
        }

        //削除
        CleanDel();

        stCoUser_.otherkantoku_f_ = FALSE;
    }

    //監督モデルに自分のPK登録
    void TMgCoUser::SetMyPK()
    {
        s64 pk = mdKantoku_.GetLineDt(0, mdkantoku_pk);
        mdKantoku_.SetMyPK(pk);
        mdKantokuSub_.SetMyPK(pk);
    }

    //モバゲーＩＤの３６進数変換
    std::string TMgCoUser::CnvMBGID(s64 mbgid)
    {
        return lib_str::IntTo36code((MBGIDBASE - mbgid), MBGIDKETA);//ＩＤは36変換
    }

    //鯖から取得したランカーデータからランキングの形にセット
    void TMgCoUser::SetupRankerDataByAnimBefore(void)
    {
        rankerData_.model_.intvecmap_.clear();
        rankerData_.model_.strvecmap_.clear();

        s32 dataCnt = 0;
        for (s32 i = 0; i < mdre_Ranker_.GetLineNum(); i++)
        {
            // 0番目のデータは試合前の自分のデータなのでスキップ
            if (i == 0) { continue; }

            rankerData_.model_.intvecmap_["pk"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_pk));
            rankerData_.model_.intvecmap_["id_m_ranking"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_id_m_ranking));
            rankerData_.model_.intvecmap_["rank"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_rank));
            rankerData_.model_.intvecmap_["score"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_score));
            rankerData_.model_.intvecmap_["mbgID"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_mbgID));
            rankerData_.model_.intvecmap_["avtHair"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_avtHair));
            rankerData_.model_.intvecmap_["avtFace"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_avtFace));
            rankerData_.model_.intvecmap_["avtHairCol"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_avtHairCol));
            rankerData_.model_.intvecmap_["avtSkinCol"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_avtSkinCol));
            rankerData_.model_.intvecmap_["avtEyeCol"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_avtEyeCol));
            rankerData_.model_.intvecmap_["avtAccCol"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_avtAccCol));
            rankerData_.model_.intvecmap_["avtClotheCol"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_avtClotheCol));
            rankerData_.model_.intvecmap_["create_date"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_create_date));
            rankerData_.model_.intvecmap_["updated_date"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_updated_date));
            rankerData_.model_.intvecmap_["del_f"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_del_f));

            rankerData_.model_.strvecmap_["name_c32"].push_back(mdre_Ranker_.GetLineDtStr(i, enmdre_RankerStr::mdre_ranker_name_c32));
            rankerData_.model_.strvecmap_["prof0_c128"].push_back(mdre_Ranker_.GetLineDtStr(i, enmdre_RankerStr::mdre_ranker_prof0_c128));
            rankerData_.model_.strvecmap_["prof1_c128"].push_back(mdre_Ranker_.GetLineDtStr(i, enmdre_RankerStr::mdre_ranker_prof1_c128));
            rankerData_.model_.strvecmap_["prof2_c128"].push_back(mdre_Ranker_.GetLineDtStr(i, enmdre_RankerStr::mdre_ranker_prof2_c128));

            dataCnt++;

            // データ数が多い時は終わる
            if (dataCnt > RANKERNUM){ break; }
        }
        rankerData_.model_.MappingPK(FALSE);
    }
    //鯖から取得したランカーデータからランキングの形にセット
    void TMgCoUser::SetupRankerDataByAnimAfter(void)
    {
        rankerData_.model_.intvecmap_.clear();
        rankerData_.model_.strvecmap_.clear();

        s32 dataCnt = 0;
        for (s32 i = 0; i < mdre_Ranker_.GetLineNum(); i++)
        {
            // 0番目のデータは試合前の自分のデータなのでスキップ
            if (i == 0) { continue; }

            rankerData_.model_.intvecmap_["pk"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_pk));
            rankerData_.model_.intvecmap_["id_m_ranking"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_id_m_ranking));
            rankerData_.model_.intvecmap_["rank"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_rank));
            rankerData_.model_.intvecmap_["score"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_score));
            rankerData_.model_.intvecmap_["mbgID"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_mbgID));
            rankerData_.model_.intvecmap_["avtHair"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_avtHair));
            rankerData_.model_.intvecmap_["avtFace"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_avtFace));
            rankerData_.model_.intvecmap_["avtHairCol"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_avtHairCol));
            rankerData_.model_.intvecmap_["avtSkinCol"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_avtSkinCol));
            rankerData_.model_.intvecmap_["avtEyeCol"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_avtEyeCol));
            rankerData_.model_.intvecmap_["avtAccCol"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_avtAccCol));
            rankerData_.model_.intvecmap_["avtClotheCol"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_avtClotheCol));
            rankerData_.model_.intvecmap_["create_date"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_create_date));
            rankerData_.model_.intvecmap_["updated_date"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_updated_date));
            rankerData_.model_.intvecmap_["del_f"].push_back(mdre_Ranker_.GetLineDt(i, enmdre_Ranker::mdre_ranker_del_f));

            rankerData_.model_.strvecmap_["name_c32"].push_back(mdre_Ranker_.GetLineDtStr(i, enmdre_RankerStr::mdre_ranker_name_c32));
            rankerData_.model_.strvecmap_["prof0_c128"].push_back(mdre_Ranker_.GetLineDtStr(i, enmdre_RankerStr::mdre_ranker_prof0_c128));
            rankerData_.model_.strvecmap_["prof1_c128"].push_back(mdre_Ranker_.GetLineDtStr(i, enmdre_RankerStr::mdre_ranker_prof1_c128));
            rankerData_.model_.strvecmap_["prof2_c128"].push_back(mdre_Ranker_.GetLineDtStr(i, enmdre_RankerStr::mdre_ranker_prof2_c128));

            dataCnt++;

            // データ14つセットして終わり
            if (dataCnt > (DBSIDE*DBMEMBER_ALL)){ break; }
        }
        rankerData_.model_.MappingPK(FALSE);
    }

    //持ってきたランカーデータからアイコン作成
    void TMgCoUser::MakeChestRanker()
    {
        s32 rankernum = mdre_Ranker_.GetLineNum();
        for (s32 i = 0; i < DBSIDE; ++i)
        {
            for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
            {
                s32 i3 = (i * DBMEMBER_ALL) + i2;

                if (i3 >= rankernum) break;

                s32 hair = mdre_Ranker_.GetLineDt(i3, mdre_ranker_avtHair);
                s32 face = mdre_Ranker_.GetLineDt(i3, mdre_ranker_avtFace);
                s32 haircol = mdre_Ranker_.GetLineDt(i3, mdre_ranker_avtHairCol);
                s32 skincol = mdre_Ranker_.GetLineDt(i3, mdre_ranker_avtSkinCol);
                s32 eyecol = mdre_Ranker_.GetLineDt(i3, mdre_ranker_avtEyeCol);
                s32 acccol = mdre_Ranker_.GetLineDt(i3, mdre_ranker_avtAccCol);
                s32 clothecol = mdre_Ranker_.GetLineDt(i3, mdre_ranker_avtClotheCol);
                BOOL gb_f = FALSE;
                pmgEO_->mgGrp_.pmgChGrp_[i][i2]->SetCharDt(hair, face, haircol, skincol, eyecol, acccol, clothecol, gb_f);
            }
            pmgEO_->mgGrp_.MakeIconTexture_Chest(i, TRUE, FALSE);
        }
    }

    //持ってきたランカーデータからアイコン作成
    void TMgCoUser::MakeChestRankerByData(void)
    {
        s32 rankernum = rankerData_.GetLineNum();
        for (s32 i = 0; i < RANKERNUM; ++i)
        {
            if (i >= rankernum) break;

            s32 hair = rankerData_.GetLineDt(i, mdre_ranker_avtHair);
            s32 face = rankerData_.GetLineDt(i, mdre_ranker_avtFace);
            s32 haircol = rankerData_.GetLineDt(i, mdre_ranker_avtHairCol);
            s32 skincol = rankerData_.GetLineDt(i, mdre_ranker_avtSkinCol);
            s32 eyecol = rankerData_.GetLineDt(i, mdre_ranker_avtEyeCol);
            s32 acccol = rankerData_.GetLineDt(i, mdre_ranker_avtAccCol);
            s32 clothecol = rankerData_.GetLineDt(i, mdre_ranker_avtClotheCol);
            BOOL gb_f = FALSE;
            pmgEO_->mgGrp_.pmgChRankerGrp_[i]->SetCharDt(hair, face, haircol, skincol, eyecol, acccol, clothecol, gb_f);
            pmgEO_->mgGrp_.MakeIconRankerTexture_Chest(i, TRUE, FALSE);

        }
    }
    //持ってきた他監督データからアイコン作成
    void TMgCoUser::MakeChestEnemy()
    {
        s32 enemynum = mdre_Enemy_.GetLineNum();
        for (s32 i = 0; i < DBSIDE; ++i)
        {
            for (s32 i2 = 0; i2 < DBMEMBER_ALL; ++i2)
            {
                s32 i3 = (i * DBMEMBER_ALL) + i2;

                if (i3 >= enemynum) break;

                s32 hair = mdre_Enemy_.GetLineDt(i3, mdre_enemy_avtHair);
                s32 face = mdre_Enemy_.GetLineDt(i3, mdre_enemy_avtFace);
                s32 haircol = mdre_Enemy_.GetLineDt(i3, mdre_enemy_avtHairCol);
                s32 skincol = mdre_Enemy_.GetLineDt(i3, mdre_enemy_avtSkinCol);
                s32 eyecol = mdre_Enemy_.GetLineDt(i3, mdre_enemy_avtEyeCol);
                s32 acccol = mdre_Enemy_.GetLineDt(i3, mdre_enemy_avtAccCol);
                s32 clothecol = mdre_Enemy_.GetLineDt(i3, mdre_enemy_avtClotheCol);
                BOOL gb_f = FALSE;
                pmgEO_->mgGrp_.pmgChGrp_[i][i2]->SetCharDt(hair, face, haircol, skincol, eyecol, acccol, clothecol, gb_f);
            }
            pmgEO_->mgGrp_.MakeIconTexture_Chest(i, TRUE, FALSE);
        }
    }

    //装備可能か
    BOOL TMgCoUser::IsShotEnable(s64 id_shot)
    {
        BOOL res = FALSE;
        if (id_shot != NGID)
        {
            const s32 ENABLE = 1;
            res = (mdShot_.GetPKDt(id_shot, mdshot_state) == ENABLE);
        }
        return res;
    }

    //合成中か
    BOOL TMgCoUser::IsShotGousei(s64 id_shot)
    {
        BOOL res = FALSE;
        if (id_shot != NGID)
        {
            const s32 GOUSEI = 2;
            res = (mdShot_.GetPKDt(id_shot, mdshot_state) == GOUSEI);
        }
        return res;
    }

    void TMgCoUser::GetShotTotalPowSpin(s64 mid_team, s32 posNo, s32* pPow, s32* pSpin)
    {
        s64 id_team = 0;
        for (s32 i = 0; i < pmgEO_->mgCoU_.mdTeam_.GetLineNum(); ++i)
        {
            if (pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_id_m_team) == mid_team)
            {
                id_team = pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_pk);
                break;
            }
        }
        if (id_team == 0){ return; }

        s64 id_shset = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_id_shset);
        s32 tmrarity = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_rarity);
        s32 overlap = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);

        for (s32 i = 0; i < SLOTNUM; ++i)
        {
            //問い合わせるチームのスロット番号0~48
            s32 slot = ((i * DBMEMBER_ALL) + posNo);
            s32 needover = pmgEO_->mgCoM_.mdm_Slot_.GetLineDt(tmrarity, (enmdm_Slot)(mdm_slot_slot00 + slot));//必要なオーバーラップ

            s64 id_shot = NGID;
            if (needover <= overlap)//レベル足りてる
            {
                if (id_shset == NGID)//チーム設定
                {
                    id_shot = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, (enmdTeam)(mdteam_id_shot00 + slot));
                }
                else//シュートセット
                {
                    id_shot = pmgEO_->mgCoU_.mdShSet_.GetPKDt(id_shset, (enmdShSet)(mdshset_id_shot00 + slot));
                }
            }
            if (id_shot == NGID) continue;

            //素材をさらう
            SOZAIDTVECTOR sozaidtvec = pmgEO_->mgCoU_.GetSozaiDataAll(id_shot);

            // まずベースのステータス取得
            s32 totalpow = 0;
            s32 totalspin = 0;
            pmgEO_->mgCoM_.GetBasePowSpin(sozaidtvec[0], &totalpow, &totalspin, FALSE);

            for (s32 i3 = 0; i3 < sozaidtvec.size(); ++i3)
            {
                s32 revpow = 0;
                s32 revspin = 0;
                pmgEO_->mgCoM_.GetRevPowSpin(sozaidtvec[i3], &revpow, &revspin);
                totalpow += revpow;
                totalspin += revspin;
            }

            *pPow += totalpow;
            *pSpin *= totalspin;
        }
    }

    s32 TMgCoUser::GetTotalSetShotNum(s64 mid_team, s32 posNo)
    {
        s64 id_team = 0;
        for (s32 i = 0; i < pmgEO_->mgCoU_.mdTeam_.GetLineNum(); ++i)
        {
            if (pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_id_m_team) == mid_team)
            {
                id_team = pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_pk);
                break;
            }
        }
        if (id_team == 0){ return 0; }

        s64 id_shset = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_id_shset);
        s32 tmrarity = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_rarity);
        s32 overlap = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, mdteam_overlap);

        s32 shotNum = 0;
        for (s32 i = 0; i < SLOTNUM; ++i)
        {
            //問い合わせるチームのスロット番号0~48
            s32 slot = ((i * DBMEMBER_ALL) + posNo);
            s32 needover = pmgEO_->mgCoM_.mdm_Slot_.GetLineDt(tmrarity, (enmdm_Slot)(mdm_slot_slot00 + slot));//必要なオーバーラップ

            s64 id_shot = NGID;
            if (needover <= overlap)//レベル足りてる
            {
                if (id_shset == NGID)//チーム設定
                {
                    id_shot = pmgEO_->mgCoU_.mdTeam_.GetPKDt(id_team, (enmdTeam)(mdteam_id_shot00 + slot));
                }
                else//シュートセット
                {
                    id_shot = pmgEO_->mgCoU_.mdShSet_.GetPKDt(id_shset, (enmdShSet)(mdshset_id_shot00 + slot));
                }
            }
            if (id_shot == NGID) continue;
            shotNum++;
        }

        return shotNum;
    }

    s32 TMgCoUser::GetOpenShotNum(s64 mid_team, s64 id_team, s64 posNo, s32 overlap)
    {
        if (posNo < 0){ return 0; }

		s32 tmLv = overlap;
		if (tmLv == NGNUM)
		{
			s64 teamID = id_team;
			if (teamID == NGNUM)
			{
				for (s32 i = 0; i < pmgEO_->mgCoU_.mdTeam_.GetLineNum(); ++i)
				{
					if (pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_id_m_team) == mid_team)
					{
						id_team = pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_pk);
						break;
					}
				}
			}
			if (id_team == 0){ return 0; }

			tmLv = pmgEO_->mgCoU_.mdTeam_.GetPKDt(teamID, mdteam_overlap);
		}

		s32 tmrarity = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, mdm_team_rarity);


        s32 openNum = 0;
        for (s32 i = 0; i < SLOTNUM; ++i)
        {
            //問い合わせるチームのスロット番号0~48
            s32 slot = ((i * DBMEMBER_ALL) + posNo);
            s32 needLv = pmgEO_->mgCoM_.mdm_Slot_.GetLineDt(tmrarity, (enmdm_Slot)(mdm_slot_slot00 + slot));//必要なオーバーラップ

            //レベルチェック
            if (needLv > tmLv){ continue; }
            openNum++;
        }
        return openNum;
    }



    s32 TMgCoUser::GetShotNum_Inv()
    {
        //有効なシュートインベントリ数
        s32 shotnum = 0;
        s64 id_kantoku = mdKantoku_.GetMyDt(mdkantoku_pk);
        for (s32 i = 0; i < mdShot_.GetLineNum(); ++i)
        {
            if (mdShot_.GetLineDt(i, mdshot_id_kantoku) != id_kantoku) continue;
            ++shotnum;
        }
        return shotnum;
    }
    s32 TMgCoUser::GetShotNum_Enable()
    {
        //有効なシュートインベントリ数
        s32 shotnum = 0;
        s64 id_kantoku = mdKantoku_.GetMyDt(mdkantoku_pk);
        for (s32 i = 0; i < mdShot_.GetLineNum(); ++i)
        {
            if (mdShot_.GetLineDt(i, mdshot_id_kantoku) != id_kantoku) continue;//他人のやつ

            if (mdShot_.GetLineDt(i, mdshot_state) == 1)//ENABLE
            {
                ++shotnum;
            }
        }
        return shotnum;
    }
    s32 TMgCoUser::GetShotNum_Souko()
    {
        //有効なシュートインベントリ数
        s32 shotnum = 0;
        s64 id_kantoku = mdKantoku_.GetMyDt(mdkantoku_pk);
        for (s32 i = 0; i < mdShot_.GetLineNum(); ++i)
        {
            if (mdShot_.GetLineDt(i, mdshot_id_kantoku) != id_kantoku) continue;//他人のやつ

            if (mdShot_.GetLineDt(i, mdshot_state) != 0)//ENABLE
            {
                ++shotnum;
            }
        }
        return shotnum;
    }
    s32 TMgCoUser::GetShotNum_Emp()
    {
        //有効なシュートインベントリ数
        s32 empnum = 0;
        s64 id_kantoku = mdKantoku_.GetMyDt(mdkantoku_pk);
        for (s32 i = 0; i < mdShot_.GetLineNum(); ++i)
        {
            if (mdShot_.GetLineDt(i, mdshot_id_kantoku) != id_kantoku) continue;//他人のやつ

            if (mdShot_.GetLineDt(i, mdshot_state) == 0)
            {
                ++empnum;
            }
        }
        return empnum;
    }
    //有効なシュートインベントリ内番号からシュートのPK
    s64 TMgCoUser::GetShotid_Enable(s32 enableno, BOOL sozaionly_f)
    {
        //有効なシュートインベントリ数
        s32 shotnum = 0;
        s64 res = NGNUM;
        const s32 ENABLE = 1;
        for (s32 i = 0; i < mdShot_.GetLineNum(); ++i)
        {
            if (mdShot_.GetLineDt(i, mdshot_state) == ENABLE)//ENABLE
            {
                if ((sozaionly_f == FALSE)
                    || (mdShot_.GetLineDt(i, mdshot_useslot) == 0))//まだ合成されてない
                {
                    if (enableno == shotnum)
                    {
                        res = mdShot_.GetLineDt(i, mdshot_pk);
                        break;
                    }
                    else
                    {
                        ++shotnum;
                    }
                }
            }
        }

        return res;
    }


    //合成できるベースと素材があるか★のちほど
    BOOL TMgCoUser::IsIntoGousei()
    {
        BOOL res = FALSE;
        const s32 ENABLE = 1;
        for (s32 i = 0; i < mdShot_.GetLineNum(); ++i)
        {
            if (mdShot_.GetLineDt(i, mdshot_state) != ENABLE) continue;

            s32 useslot = mdShot_.GetLineDt(i, mdshot_useslot);
            s32 rarity = (mdShot_.GetLineDt(i, mdshot_id_m_sozai0) % SOZAIMID);

            if ((useslot == 0) || ((useslot - 1) < rarity))
            {
                return TRUE;
            }
        }

        return res;
    }
    //シュートのインベントリの番号
    s32 TMgCoUser::GetShotInvNo(s64 id_shot)
    {
        s32 res = NGNUM;
        for (s32 i = 0; i < mdShot_.GetLineNum(); ++i)
        {
            if (mdShot_.GetLineDt(i, mdshot_pk) == id_shot)
            {
                res = i;
                break;
            }
        }
        return res;
    }

    s32 TMgCoUser::GetShotHaveMember(s64 id_shot, s64 id_team)
    {
        if (id_team == 0)
        {
            return NGNUM;
        }

        s32 res = NGNUM;
        s32 shotnum = (mdteam_id_shot41 - mdteam_id_shot00);
        for (s32 i = 0; i < shotnum; ++i)
        {
            enmdTeam shotno = (enmdTeam)(mdteam_id_shot00 + i);
            if (mdTeam_.GetPKDt(id_team, shotno) == id_shot)
            {
                res = (i % DBMEMBER_ALL);
                break;
            }
        }
        return res;
    }

    //シュートＩＤとスロットから素材MID
    TSozaiData TMgCoUser::GetSozaiData(s64 id_shot, s32 slotNo)
    {
        TSozaiData res;
        res.Init();

        const s32 INDVPOS[] =
        {
            1,
            10,
            100,
            1000,
            10000,
            100000,
            1000000,
            10000000,
            100000000 };

        enmdShot slotsozai = (enmdShot)(mdshot_id_m_sozai0 + slotNo);
        res.mid_sozai_ = mdShot_.GetPKDt(id_shot, slotsozai) / SOZAIMID;
        res.rarity_ = mdShot_.GetPKDt(id_shot, slotsozai) % SOZAIMID;
        res.indvpow_ = (mdShot_.GetPKDt(id_shot, mdshot_indvpow) % INDVPOS[slotNo + 1]) / INDVPOS[slotNo];
        res.indvspin_ = (mdShot_.GetPKDt(id_shot, mdshot_indvspin) % INDVPOS[slotNo + 1]) / INDVPOS[slotNo];

        return res;
    }

    //消耗品MIDから消耗品PK
    s64 TMgCoUser::GetSupply_id(s64 mid_supply)
    {
        s64 res = NGNUM;
        for (s32 i = 0; i < mdSupply_.GetLineNum(); ++i)
        {
            if (mdSupply_.GetLineDt(i, mdsupply_id_m_supply) == mid_supply)
            {
                res = mdSupply_.GetLineDt(i, mdsupply_pk);
                break;
            }
        }
        return res;
    }
    //消耗品の数
    s64 TMgCoUser::GetSupplyNum(s64 mid_supply)
    {
        s32 res = 0;
        s64 id_supply = GetSupply_id(mid_supply);
        if (id_supply != NGNUM) res = mdSupply_.GetPKDt(id_supply, mdsupply_supplyNum);
        return res;
    }
    //ぎんぎんぜっとの数
    s64 TMgCoUser::GetSupplyNum_GGZ()
    {
        return GetSupplyNum(SPL_GGZ);
    }
    //みなげんきーの数
    s64 TMgCoUser::GetSupplyNum_MGK()
    {
        return GetSupplyNum(SPL_MGK);
    }
    //ふっかつのたねの数
    s64 TMgCoUser::GetSupplyNum_FKT()
    {
        return GetSupplyNum(SPL_FKT);
    }
    //消耗品の数
    s64 TMgCoUser::GetGachaKenNum(s64 mid_gacha)
    {
        s64 mid_supply_ken = pmgEO_->mgCoM_.mdm_Gacha_.GetPKDt(mid_gacha, mdm_gacha_id_m_supply_ken);
        //s32 kennum = pmgEO_->mgCoM_.mdm_Gacha_.GetPKDt(mid_gacha, mdm_gacha_kenNum);
        s32 res = GetSupplyNum(mid_supply_ken);
        return res;
    }
    //消耗品の数
    BOOL TMgCoUser::IsGachaKenUse(s64 mid_gacha)
    {
        s32 kennum = pmgEO_->mgCoM_.mdm_Gacha_.GetPKDt(mid_gacha, mdm_gacha_kenNum);
        return (GetGachaKenNum(mid_gacha) >= kennum);
    }
	//コーチチケットの数
	s64 TMgCoUser::GetCoachKenNum(s64 mid_supply)
	{
		s32 res = GetSupplyNum(mid_supply);
		return res;
	}
	//コーチチケットの数
	BOOL TMgCoUser::IsCoachKenUse(s64 mid_supply)
	{
		s32 kennum = 1;//pmgEO_->mgCoM_.mdm_Gacha_.GetPKDt(mid_gacha, mdm_gacha_kenNum);
		return (GetCoachKenNum(mid_supply) >= kennum);
	}
    //懐かしボックスID
    s64 TMgCoUser::GetNaBox_id(s64 mid_nabox)
    {
        s64 res = NGNUM;
        for (s32 i = 0; i < mdNaBox_.GetLineNum(); ++i)
        {
            if (mdNaBox_.GetLineDt(i, mdnabox_id_m_nabox) == mid_nabox)
            {
                res = mdNaBox_.GetLineDt(i, mdnabox_pk);
                break;
            }
        }
        return res;
    }

    BOOL TMgCoUser::IsNaBoxOpen(s64 id_nabox, s32 no)
    {
        s64 openbit = mdNaBox_.GetPKDt(id_nabox, mdnabox_openBit);
        u32 checkbit = (0x1 << no);
        return ((openbit & checkbit) != 0);
    }
    //チュートリアルのオープンビット
    BOOL TMgCoUser::IsTutoClear(enTutoBitType tutono)
    {
        s64 openbit = mdKantokuSub_.GetMyDt(mdkantokusub_tutoBit);
        u32 checkbit = (0x1 << tutono);
        return ((openbit & checkbit) != 0);
    }

    //ＢＧＭの獲得ビット
    BOOL TMgCoUser::IsBGM(s32 mid_bgm)
    {
        s32 bitNo = (mid_bgm / 32);
        s64 openbit = mdBGM_.GetLineDt(0, (enmdBGM)(mdbgm_bgmBit00 + bitNo));
        u32 checkbit = (0x1 << (mid_bgm % 32));
        if (mid::midIsIOS())
        {
            return ((openbit & checkbit) != 0);
        }
        else
        {
            u32 openbit32 = (u32)abs(openbit);
            if (openbit < 0) openbit32 |= (0x1 << 31);
            return ((openbit32 & checkbit) != 0);
        }
    }
    //背景の獲得ビット
    BOOL TMgCoUser::IsHaikei(s32 mid_haikei)
    {
        s32 bitNo = (mid_haikei / 32);
        s64 openbit = mdHaikei_.GetLineDt(0, (enmdHaikei)(mdhaikei_haikeiBit00 + bitNo));
        u32 checkbit = (0x1 << (mid_haikei % 32));
        if (mid::midIsIOS())
        {
            return ((openbit & checkbit) != 0);
        }
        else
        {
            u32 openbit32 = (u32)abs(openbit);
            if (openbit < 0) openbit32 |= (0x1 << 31);
            return ((openbit & checkbit) != 0);
        }
    }

    //LONGVECTOR TMgCoUser::GetSozaiKeep()
    //{
    //  LONGVECTOR res;
    //  res.clear();
    //  for (s32 i = 0; i < mdSozai_.GetLineNum(); ++i)
    //  {
    //    //シュートボックスフラグ
    //    if (mdSozai_.GetLineDtBOOL(i, mdsozai_keep_f))
    //    {
    //      res.push_back(mdSozai_.GetLineDt(i, mdsozai_pk));
    //    }
    //  }
    //  return res;
    //}

    LONGVECTOR TMgCoUser::GetGiftBox()
    {
        LONGVECTOR res;
        res.clear();
        for (s32 i = 0; i < mdGiftbox_.GetLineNum(); ++i)
        {
            res.push_back(mdGiftbox_.GetLineDt(i, mdgiftbox_pk));
        }
        return res;
    }


    SOZAIDTVECTOR TMgCoUser::GetSozaiDataAll(s64 id_shot)
    {
        SOZAIDTVECTOR sozaivec;
        s32 allnum = (mdShot_.GetPKDt(id_shot, mdshot_useslot) + 1);
        for (s32 i = 0; i < allnum; ++i)
        {
            TSozaiData sozaidt = GetSozaiData(id_shot, i);
            sozaivec.push_back(sozaidt);
        }
        return sozaivec;
    }
    TSozaiData TMgCoUser::GetSozaiDataLast(s64 id_shot)//最後の素材
    {
        s32 lastslot = mdShot_.GetPKDt(id_shot, mdshot_useslot);//最後のスロット番号
        TSozaiData sozaidt = GetSozaiData(id_shot, lastslot);
        return sozaidt;
    }

    SOZAIDTVECTOR TMgCoUser::GetSozaiDataSyn(s64 defshotid)
    {
        SOZAIDTVECTOR sozaivec;

        for (s32 i = 0; i < HSELEMMAX; ++i)
        {
            TSozaiData sozaidt;
            sozaidt.Init();
            sozaidt.mid_sozai_ = pmgEO_->mgDt_.dtShSyn_.GetDt(defshotid, (enDtShSyn)(gs_base + (i * 2)));
            sozaidt.rarity_ = pmgEO_->mgDt_.dtShSyn_.GetDt(defshotid, (enDtShSyn)(gs_base_rare + (i * 2)));
            if (sozaidt.mid_sozai_ == 0) break;

            sozaivec.push_back(sozaidt);
        }
        return sozaivec;
    }

    s32 TMgCoUser::GetShotPrice(s64 id_shot)
    {
        SOZAIDTVECTOR sozaivec = GetSozaiDataAll(id_shot);
        s32 res = 0;
        for (s32 i = 0; i < sozaivec.size(); ++i)
        {
            s32 price = pmgEO_->mgCoM_.mdm_Rarity_.GetLineDt(sozaivec[i].rarity_, mdm_rarity_priceSell);
            res += price;
        }
        return res;
    }


    // 比較関数を定義
    bool cmpsh_rarity(const TSortShot& left, const TSortShot& right)
    {
        return (left.rarity_ == right.rarity_)
            ? (left.mid_sozai_base_ < right.mid_sozai_base_)
            : (left.rarity_ > right.rarity_);//高い方が先頭
    }
    bool cmpsh_rarity_rev(const TSortShot& left, const TSortShot& right)
    {
        return (left.rarity_ == right.rarity_)
            ? (left.mid_sozai_base_ < right.mid_sozai_base_)
            : (left.rarity_ < right.rarity_);//低い方が先頭
    }
    bool cmpsh_mid(const TSortShot& left, const TSortShot& right)
    {
        return (left.mid_sozai_base_ < right.mid_sozai_base_);//小さい方が先頭
    }
    bool cmpsh_mid_rev(const TSortShot& left, const TSortShot& right)
    {
        return  (left.mid_sozai_base_ > right.mid_sozai_base_);//大きい方が先頭
    }
    bool cmpsh_totalpow(const TSortShot& left, const TSortShot& right)
    {
        return (left.totalpow_ == right.totalpow_)
            ? (left.mid_sozai_base_ < right.mid_sozai_base_)
            : (left.totalpow_ > right.totalpow_);//高い方が先頭
    }
    bool cmpsh_totalpow_rev(const TSortShot& left, const TSortShot& right)
    {
        return (left.totalpow_ == right.totalpow_)
            ? (left.mid_sozai_base_ < right.mid_sozai_base_)
            : (left.totalpow_ < right.totalpow_);//低い方が先頭
    }
    bool cmpsh_basepow(const TSortShot& left, const TSortShot& right)
    {
        return (left.basepow_ == right.basepow_)
            ? (left.mid_sozai_base_ < right.mid_sozai_base_)
            : (left.basepow_ > right.basepow_);//高い方が先頭
    }
    bool cmpsh_basepow_rev(const TSortShot& left, const TSortShot& right)
    {
        return (left.basepow_ == right.basepow_)
            ? (left.mid_sozai_base_ < right.mid_sozai_base_)
            : (left.basepow_ < right.basepow_);//低い方が先頭
    }
    bool cmpsh_revpow(const TSortShot& left, const TSortShot& right)
    {
        return (left.revpows_ == right.revpows_)
            ? (left.mid_sozai_base_ < right.mid_sozai_base_)
            : (left.revpows_ > right.revpows_);//高い方が先頭
    }
    bool cmpsh_revpow_rev(const TSortShot& left, const TSortShot& right)
    {
        return (left.revpows_ == right.revpows_)
            ? (left.mid_sozai_base_ < right.mid_sozai_base_)
            : (left.revpows_ < right.revpows_);//低い方が先頭
    }
    bool cmpsh_totalspin(const TSortShot& left, const TSortShot& right)
    {
        return (left.totalspin_ == right.totalspin_)
            ? (left.mid_sozai_base_ < right.mid_sozai_base_)
            : (left.totalspin_ > right.totalspin_);//高い方が先頭
    }
    bool cmpsh_totalspin_rev(const TSortShot& left, const TSortShot& right)
    {
        return (left.totalspin_ == right.totalspin_)
            ? (left.mid_sozai_base_ < right.mid_sozai_base_)
            : (left.totalspin_ < right.totalspin_);//低い方が先頭
    }
    bool cmpsh_basespin(const TSortShot& left, const TSortShot& right)
    {
        return (left.basespin_ == right.basespin_)
            ? (left.mid_sozai_base_ < right.mid_sozai_base_)
            : (left.basespin_ > right.basespin_);//高い方が先頭
    }
    bool cmpsh_basespin_rev(const TSortShot& left, const TSortShot& right)
    {
        return (left.basespin_ == right.basespin_)
            ? (left.mid_sozai_base_ < right.mid_sozai_base_)
            : (left.basespin_ < right.basespin_);//低い方が先頭
    }
    bool cmpsh_revspin(const TSortShot& left, const TSortShot& right)
    {
        return (left.revspins_ == right.revspins_)
            ? (left.mid_sozai_base_ < right.mid_sozai_base_)
            : (left.revspins_ > right.revspins_);//高い方が先頭
    }
    bool cmpsh_revspin_rev(const TSortShot& left, const TSortShot& right)
    {
        return (left.revspins_ == right.revspins_)
            ? (left.mid_sozai_base_ < right.mid_sozai_base_)
            : (left.revspins_ < right.revspins_);//低い方が先頭
    }
    bool cmpsh_new(const TSortShot& left, const TSortShot& right)
    {
        return (left.updated_time_ > right.updated_time_);//高い方が先頭
    }
    bool cmpsh_new_rev(const TSortShot& left, const TSortShot& right)
    {
        return (left.updated_time_ < right.updated_time_);//低い方が先頭
    }
    bool cmpsh_omakase(const TSortShot& left, const TSortShot& right)
    {
        s32 ldt = (left.totalpow_ + left.totalspin_);
        s32 rdt = (right.totalpow_ + right.totalspin_);
        return (ldt == rdt)
            ? (left.mid_sozai_base_ < right.mid_sozai_base_)
            : (ldt > rdt);//高い方が先頭
    }

    //シュートのリスト
    SORTSHOTVECTOR TMgCoUser::GetShotVec(enShotFilTypeS filtypeS, LONGVECTOR* ngidvec)
    {
        sortshotmap_.clear();

        SORTSHOTVECTOR res;

        std::list<s64> nglist;
        if (ngidvec != NULL)
        {
            for (auto& item : *ngidvec)//リストに写す
            {
                if (item != NGID) nglist.push_back(item);
            }
        }

        LONGVECTOR id_shot_vecall;
        //NGIDを省く
        for (s32 i = 0; i < mdShot_.GetLineNum(); ++i)
        {
            BOOL ngid_f = FALSE;
            s64 id_shot = mdShot_.GetLineDt(i, mdshot_pk);

            //有効でないシュートはこの時点でカット
            if (IsShotEnable(id_shot) == FALSE) continue;

            if (ngidvec != NULL)
            {
                std::list<s64>::iterator nglist_it;
                for (nglist_it = nglist.begin(); nglist_it != nglist.end(); ++nglist_it)
                {
                    if (*nglist_it == id_shot)
                    {
                        ngid_f = TRUE;
                        nglist.erase(nglist_it);//みつかったＮＧは削る
                        break;
                    }
                }
            }
            if (ngid_f == FALSE) id_shot_vecall.push_back(mdShot_.GetLineDt(i, mdshot_pk));
        }


        //素材をすべてさらってシュートリストに追記していく
        //for (s32 i = 0; i < mdShot_.GetLineNum(); ++i)
        for (auto& id_shot : id_shot_vecall)
        {
            TSortShot sshot;
            sshot.Init();
            s32 useslot = mdShot_.GetPKDt(id_shot, mdshot_useslot);
            sortshotmap_[id_shot].id_shot_ = id_shot;
            sortshotmap_[id_shot].updated_time_ = mdShot_.GetPKDt(id_shot, mdshot_updated_date);
            sortshotmap_[id_shot].lock_f_ = mdShot_.GetPKDt(id_shot, mdshot_lock_f);

            SOZAIDTVECTOR sozaidtvec;
            for (s32 i2 = 0; i2 < (useslot + 1); ++i2)
            {
                TSozaiData sozaidt = GetSozaiData(id_shot, i2);
                sozaidtvec.push_back(sozaidt);
            }

            for (s32 i2 = 0; i2 < sozaidtvec.size(); ++i2)
            {
                s32 revpow = 0;
                s32 revspin = 0;
                //個体値
                pmgEO_->mgCoM_.GetRevPowSpin(sozaidtvec[i2], &revpow, &revspin);
                sortshotmap_[id_shot].useslot_ = useslot;
                sortshotmap_[id_shot].revpows_ += revpow;//上乗せ
                sortshotmap_[id_shot].revspins_ += revspin;//上乗せ
                sortshotmap_[id_shot].totalpow_ += revpow;//上乗せ
                sortshotmap_[id_shot].totalspin_ += revspin;//上乗せ


                //ベース素材
                if (i2 == 0)
                {
                    //質
                    sortshotmap_[id_shot].indvpow_ = sozaidtvec[i2].indvpow_;
                    sortshotmap_[id_shot].indvspin_ = sozaidtvec[i2].indvspin_;
                    //基礎値
                    s32 basepow = 0;
                    s32 basespin = 0;
                    pmgEO_->mgCoM_.GetBasePowSpin(sozaidtvec[i2], &basepow, &basespin, FALSE);
                    sortshotmap_[id_shot].mid_sozai_base_ = sozaidtvec[i2].mid_sozai_;
                    sortshotmap_[id_shot].rarity_ = sozaidtvec[i2].rarity_;
                    sortshotmap_[id_shot].basepow_ = basepow;
                    sortshotmap_[id_shot].basespin_ = basespin;
                    sortshotmap_[id_shot].totalpow_ += basepow;//上乗せ
                    sortshotmap_[id_shot].totalspin_ += basespin;//上乗せ
                    sortshotmap_[id_shot].genre_ = pmgEO_->mgDt_.dtShElm_.GetGenre(sozaidtvec[i2].mid_sozai_);
                }
                else//合成素材
                {
                    sortshotmap_[id_shot].id_sozai_add_[i2 - 1] = sozaidtvec[i2].mid_sozai_;
                }
            }
        }


        for (auto& item : sortshotmap_)
        {
            BOOL push_f = TRUE;
            BOOL unlock_f = (mdShot_.GetPKDt(item.second.id_shot_, mdshot_lock_f) == FALSE);
            switch (filtypeS)
            {
            case db::enShotFilTypeS_Baseable:push_f = (item.second.useslot_ < (item.second.rarity_ + 1)); break;//ベース可能
            case db::enShotFilTypeS_Mixed: push_f = (item.second.useslot_ != 0); break;//合成（バラ可能）
            case db::enShotFilTypeS_SellD: push_f = (unlock_f && (item.second.rarity_ <= 0)); break;//売却可能D
            case db::enShotFilTypeS_SellC: push_f = (unlock_f && (item.second.rarity_ <= 1)); break;//売却可能C
            case db::enShotFilTypeS_SellB: push_f = (unlock_f && (item.second.rarity_ <= 2)); break;//売却可能B
            case db::enShotFilTypeS_SellA: push_f = (unlock_f && (item.second.rarity_ <= 3)); break;//売却可能A
            case db::enShotFilTypeS_SellALL: push_f = (unlock_f); break;//売却可能ALL
            case db::enShotFilTypeS_Under1: push_f = (item.second.useslot_ < 1); break;//素材1つ以下
            case db::enShotFilTypeS_Under2: push_f = (item.second.useslot_ < 2); break;//素材2つ以下
            case db::enShotFilTypeS_Under3: push_f = (item.second.useslot_ < 3); break;//素材3つ以下
            case db::enShotFilTypeS_Under4: push_f = (item.second.useslot_ < 4); break;//素材4つ以下
            case db::enShotFilTypeS_Under5: push_f = (item.second.useslot_ < 5); break;//素材5つ以下
            case db::enShotFilTypeS_Under6: push_f = (item.second.useslot_ < 6); break;//素材6つ以下
            case db::enShotFilTypeS_Under7: push_f = (item.second.useslot_ < 7); break;//素材7つ以下
            }
            if (push_f) res.push_back(item.second);
        }

        return res;
    }

    SORTSHOTVECTOR TMgCoUser::GenGetShotList_FilRarity(SORTSHOTVECTOR* lnovec, s32 rarity)
    {
        SORTSHOTVECTOR res;
        for (s32 i = 0; i < lnovec->size(); ++i)
        {
            if (lnovec->at(i).rarity_ == rarity) res.push_back(lnovec->at(i));
        }
        return res;
    }
    SORTSHOTVECTOR TMgCoUser::GenGetShotList_FilGenre(SORTSHOTVECTOR* lnovec, s32 genre)
    {
        SORTSHOTVECTOR res;
        for (auto& item : *lnovec)
        {
            if (item.genre_ == genre) res.push_back(item);
        }
        return res;
    }
    SORTSHOTVECTOR TMgCoUser::GenGetShotList_FilMixed(SORTSHOTVECTOR* lnovec, s32 mixlv)
    {
        SORTSHOTVECTOR res;
        for (s32 i = 0; i < lnovec->size(); ++i)
        {
            BOOL solo_f = (lnovec->at(i).useslot_ == 0);
            BOOL full_f = ((lnovec->at(i).useslot_ - 1) == lnovec->at(i).rarity_);
            switch (mixlv)
            {
            case 0: if (solo_f)  res.push_back(lnovec->at(i)); break;
            case 1: if ((solo_f || full_f) == FALSE) res.push_back(lnovec->at(i)); break;
            case 2: if (full_f)  res.push_back(lnovec->at(i)); break;
            }
        }
        return res;
    }
    SORTSHOTVECTOR TMgCoUser::GenGetShotList_FilLocked(SORTSHOTVECTOR* lnovec)
    {
        SORTSHOTVECTOR res;
        for (s32 i = 0; i < lnovec->size(); ++i)
        {
            if (lnovec->at(i).lock_f_)  res.push_back(lnovec->at(i));
        }
        return res;
    }

    LONGVECTOR TMgCoUser::GetShotList_Filter(SORTSHOTVECTOR* shotvec, enShotSrtType srttype, enShotFilType filtype, enShotFilTypeS filtypeS, BOOL rev_f, LONGVECTOR* ngidvec)
    {
        //ベクタ作り直し
        *shotvec = GetShotVec(filtypeS, ngidvec);

        switch (filtype)
        {
        case db::enShotFilType_RarityD: *shotvec = GenGetShotList_FilRarity(shotvec, 0); break;
        case db::enShotFilType_RarityC: *shotvec = GenGetShotList_FilRarity(shotvec, 1); break;
        case db::enShotFilType_RarityB: *shotvec = GenGetShotList_FilRarity(shotvec, 2); break;
        case db::enShotFilType_RarityA: *shotvec = GenGetShotList_FilRarity(shotvec, 3); break;
        case db::enShotFilType_RarityS: *shotvec = GenGetShotList_FilRarity(shotvec, 4); break;
        case db::enShotFilType_RaritySS: *shotvec = GenGetShotList_FilRarity(shotvec, 5); break;
        case db::enShotFilType_RaritySSS: *shotvec = GenGetShotList_FilRarity(shotvec, 6); break;

        case db::enShotFilType_Locked:*shotvec = GenGetShotList_FilLocked(shotvec); break;
        case db::enShotFilType_Solo:*shotvec = GenGetShotList_FilMixed(shotvec, 0); break;
        case db::enShotFilType_Mixed:*shotvec = GenGetShotList_FilMixed(shotvec, 1); break;
        case db::enShotFilType_FullMixed:*shotvec = GenGetShotList_FilMixed(shotvec, 2); break;

        case db::enShotFilType_Genre0:*shotvec = GenGetShotList_FilGenre(shotvec, 0); break;
        case db::enShotFilType_Genre1:*shotvec = GenGetShotList_FilGenre(shotvec, 1); break;
        case db::enShotFilType_Genre2:*shotvec = GenGetShotList_FilGenre(shotvec, 2); break;
        case db::enShotFilType_Genre3:*shotvec = GenGetShotList_FilGenre(shotvec, 3); break;
        case db::enShotFilType_Genre4:*shotvec = GenGetShotList_FilGenre(shotvec, 4); break;
        case db::enShotFilType_Genre5:*shotvec = GenGetShotList_FilGenre(shotvec, 5); break;
        case db::enShotFilType_Genre6:*shotvec = GenGetShotList_FilGenre(shotvec, 6); break;
        case db::enShotFilType_Genre7:*shotvec = GenGetShotList_FilGenre(shotvec, 7); break;
        case db::enShotFilType_Genre8:*shotvec = GenGetShotList_FilGenre(shotvec, 8); break;
        case db::enShotFilType_Genre9:*shotvec = GenGetShotList_FilGenre(shotvec, 9); break;
        case db::enShotFilType_Genre10:*shotvec = GenGetShotList_FilGenre(shotvec, 10); break;
        }

        return GetShotList_Sort(shotvec, srttype, filtypeS, rev_f);
    }

    LONGVECTOR TMgCoUser::GetShotList_Sort(SORTSHOTVECTOR* shotvec, enShotSrtType srttype, enShotFilTypeS filtypeS, BOOL rev_f)
    {
        LONGVECTOR res;

        switch (srttype)
        {
            //case db::enShotSrtType_MID:res = GenGetShotList_SrtMID(shotvec, rev_f); break;
        case db::enShotSrtType_Rarity:res = GenGetShotList_SrtRarity(shotvec, rev_f); break;
        case db::enShotSrtType_Pow:
        {
            switch (filtypeS)
            {
            case db::enShotFilTypeS_Under1:
            case db::enShotFilTypeS_Under2:
            case db::enShotFilTypeS_Under3:
            case db::enShotFilTypeS_Under4:
            case db::enShotFilTypeS_Under5:
            case db::enShotFilTypeS_Under6:
            case db::enShotFilTypeS_Under7:
                res = GenGetShotList_SrtRevPow(shotvec, rev_f);
                break;
            case db::enShotFilTypeS_Baseable:res = GenGetShotList_SrtBasePow(shotvec, rev_f); break;
            default:res = GenGetShotList_SrtTotalPow(shotvec, rev_f); break;
            }
        }
        break;
        case db::enShotSrtType_Spin:
        {
            switch (filtypeS)
            {
            case db::enShotFilTypeS_Under1:
            case db::enShotFilTypeS_Under2:
            case db::enShotFilTypeS_Under3:
            case db::enShotFilTypeS_Under4:
            case db::enShotFilTypeS_Under5:
            case db::enShotFilTypeS_Under6:
            case db::enShotFilTypeS_Under7:
                res = GenGetShotList_SrtRevSpin(shotvec, rev_f);
                break;
            case db::enShotFilTypeS_Baseable:res = GenGetShotList_SrtBaseSpin(shotvec, rev_f); break;
            default:res = GenGetShotList_SrtTotalSpin(shotvec, rev_f); break;
            }
        }
        break;
        case db::enShotSrtType_New:res = GenGetShotList_SrtMID(shotvec, rev_f); break;
        }
        return res;
    }


    LONGVECTOR TMgCoUser::GenGetShotList(SORTSHOTVECTOR* lnovec)
    {
        LONGVECTOR res;
        for (s32 i = 0; i < lnovec->size(); ++i) res.push_back(lnovec->at(i).id_shot_);
        return res;
    }
    LONGVECTOR TMgCoUser::GenGetShotList_SrtMID(SORTSHOTVECTOR* lnovec, BOOL rev_f)
    {
        if (rev_f) std::sort(lnovec->begin(), lnovec->end(), cmpsh_mid_rev);
        else std::sort(lnovec->begin(), lnovec->end(), cmpsh_mid);
        return GenGetShotList(lnovec);
    }
    LONGVECTOR TMgCoUser::GenGetShotList_SrtRarity(SORTSHOTVECTOR* lnovec, BOOL rev_f)
    {
        if (rev_f) std::sort(lnovec->begin(), lnovec->end(), cmpsh_rarity_rev);
        else std::sort(lnovec->begin(), lnovec->end(), cmpsh_rarity);
        return GenGetShotList(lnovec);
    }
    LONGVECTOR TMgCoUser::GenGetShotList_SrtTotalPow(SORTSHOTVECTOR* lnovec, BOOL rev_f)
    {
        if (rev_f) std::sort(lnovec->begin(), lnovec->end(), cmpsh_totalpow_rev);
        else std::sort(lnovec->begin(), lnovec->end(), cmpsh_totalpow);
        return GenGetShotList(lnovec);
    }
    LONGVECTOR TMgCoUser::GenGetShotList_SrtBasePow(SORTSHOTVECTOR* lnovec, BOOL rev_f)
    {
        if (rev_f) std::sort(lnovec->begin(), lnovec->end(), cmpsh_basepow_rev);
        else std::sort(lnovec->begin(), lnovec->end(), cmpsh_basepow);
        return GenGetShotList(lnovec);
    }
    LONGVECTOR TMgCoUser::GenGetShotList_SrtRevPow(SORTSHOTVECTOR* lnovec, BOOL rev_f)
    {
        if (rev_f) std::sort(lnovec->begin(), lnovec->end(), cmpsh_revpow_rev);
        else std::sort(lnovec->begin(), lnovec->end(), cmpsh_revpow);
        return GenGetShotList(lnovec);
    }
    LONGVECTOR TMgCoUser::GenGetShotList_SrtTotalSpin(SORTSHOTVECTOR* lnovec, BOOL rev_f)
    {
        if (rev_f) std::sort(lnovec->begin(), lnovec->end(), cmpsh_totalspin_rev);
        else std::sort(lnovec->begin(), lnovec->end(), cmpsh_totalspin);
        return GenGetShotList(lnovec);
    }
    LONGVECTOR TMgCoUser::GenGetShotList_SrtBaseSpin(SORTSHOTVECTOR* lnovec, BOOL rev_f)
    {
        if (rev_f) std::sort(lnovec->begin(), lnovec->end(), cmpsh_basespin_rev);
        else std::sort(lnovec->begin(), lnovec->end(), cmpsh_basespin);
        return GenGetShotList(lnovec);
    }
    LONGVECTOR TMgCoUser::GenGetShotList_SrtRevSpin(SORTSHOTVECTOR* lnovec, BOOL rev_f)
    {
        if (rev_f) std::sort(lnovec->begin(), lnovec->end(), cmpsh_revspin_rev);
        else std::sort(lnovec->begin(), lnovec->end(), cmpsh_revspin);
        return GenGetShotList(lnovec);
    }
    LONGVECTOR TMgCoUser::GenGetShotList_SrtNew(SORTSHOTVECTOR* lnovec, BOOL rev_f)
    {
        if (rev_f) std::sort(lnovec->begin(), lnovec->end(), cmpsh_new);
        else std::sort(lnovec->begin(), lnovec->end(), cmpsh_new_rev);
        return GenGetShotList(lnovec);
    }

    s32 TMgCoUser::GetNumShotList(SORTSHOTVECTOR* shotvec, enShotFilType filtype)
    {
        s32 res = 0;
        switch (filtype)
        {
        case db::enShotFilType_All: res = shotvec->size(); break;

        case db::enShotFilType_RarityD: res = GenGetNumShotList_Rarity(shotvec, 0); break;
        case db::enShotFilType_RarityC: res = GenGetNumShotList_Rarity(shotvec, 1); break;
        case db::enShotFilType_RarityB: res = GenGetNumShotList_Rarity(shotvec, 2); break;
        case db::enShotFilType_RarityA: res = GenGetNumShotList_Rarity(shotvec, 3); break;
        case db::enShotFilType_RarityS: res = GenGetNumShotList_Rarity(shotvec, 4); break;
        case db::enShotFilType_RaritySS: res = GenGetNumShotList_Rarity(shotvec, 5); break;
        case db::enShotFilType_RaritySSS: res = GenGetNumShotList_Rarity(shotvec, 6); break;

        case db::enShotFilType_Locked:  res = GenGetNumShotList_Locked(shotvec); break;
        case db::enShotFilType_Solo:  res = GenGetNumShotList_Mixed(shotvec, 0); break;
        case db::enShotFilType_Mixed: res = GenGetNumShotList_Mixed(shotvec, 1); break;
        case db::enShotFilType_FullMixed: res = GenGetNumShotList_Mixed(shotvec, 2); break;

        case db::enShotFilType_Genre0: res = GenGetNumShotList_Genre(shotvec, 0); break;
        case db::enShotFilType_Genre1: res = GenGetNumShotList_Genre(shotvec, 1); break;
        case db::enShotFilType_Genre2: res = GenGetNumShotList_Genre(shotvec, 2); break;
        case db::enShotFilType_Genre3: res = GenGetNumShotList_Genre(shotvec, 3); break;
        case db::enShotFilType_Genre4: res = GenGetNumShotList_Genre(shotvec, 4); break;
        case db::enShotFilType_Genre5: res = GenGetNumShotList_Genre(shotvec, 5); break;
        case db::enShotFilType_Genre6: res = GenGetNumShotList_Genre(shotvec, 6); break;
        case db::enShotFilType_Genre7: res = GenGetNumShotList_Genre(shotvec, 7); break;
        case db::enShotFilType_Genre8: res = GenGetNumShotList_Genre(shotvec, 8); break;
        case db::enShotFilType_Genre9: res = GenGetNumShotList_Genre(shotvec, 9); break;
        case db::enShotFilType_Genre10: res = GenGetNumShotList_Genre(shotvec, 10); break;

        }
        return res;
    }

    s32 TMgCoUser::GenGetNumShotList_Rarity(SORTSHOTVECTOR* lnovec, s32 rarity)
    {
        s32 res = 0;
        for (s32 i = 0; i < lnovec->size(); ++i)
        {
            if (lnovec->at(i).rarity_ == rarity) ++res;
        }
        return res;
    }
    s32 TMgCoUser::GenGetNumShotList_Mixed(SORTSHOTVECTOR* lnovec, s32 mixlv)
    {
        s32 res = 0;
        for (s32 i = 0; i < lnovec->size(); ++i)
        {
            BOOL solo_f = (lnovec->at(i).useslot_ == 0);
            BOOL full_f = ((lnovec->at(i).useslot_ - 1) == lnovec->at(i).rarity_);
            switch (mixlv)
            {
            case 0: if (solo_f) ++res; break;
            case 1: if ((solo_f || full_f) == FALSE) ++res; break;
            case 2: if (full_f)  ++res; break;
            }
        }
        return res;
    }
    s32 TMgCoUser::GenGetNumShotList_Locked(SORTSHOTVECTOR* lnovec)
    {
        s32 res = 0;
        for (s32 i = 0; i < lnovec->size(); ++i)
        {
            if (lnovec->at(i).lock_f_) ++res;
        }
        return res;
    }
    s32 TMgCoUser::GenGetNumShotList_Genre(SORTSHOTVECTOR* lnovec, s32 genre)
    {
        s32 res = 0;
        for (s32 i = 0; i < lnovec->size(); ++i)
        {
            //素材のみ
            if ((lnovec->at(i).useslot_ == 0) && (lnovec->at(i).genre_ == genre)) ++res;
        }
        return res;
    }
    TSortShot* TMgCoUser::GetSortShot(s64 id_shot)
    {
        return &(sortshotmap_[id_shot]);
    }
    SORTSHOTMAP* TMgCoUser::GetSortShotMap()
    {
        return &sortshotmap_;
    }

    //おまかせシュート
    LONGVECTOR TMgCoUser::GetShotOmakase()
    {
        SORTSHOTVECTOR sortshotvec = GetShotVec(enShotFilTypeS_All, NULL);
        std::sort(sortshotvec.begin(), sortshotvec.end(), cmpsh_omakase);
        return GenGetShotList(&sortshotvec);
    }
    //ロック切り替え
    void TMgCoUser::SwitchShLock(s64 id_shot)
    {
        sortshotmap_[id_shot].lock_f_ = !(sortshotmap_[id_shot].lock_f_);
        if (shlockmap_.find(id_shot) == shlockmap_.end())
        {
            shlockmap_[id_shot] = TRUE;//変更あり
            ++stCoUser_.shlocknum_;
        }
        else
        {
            shlockmap_[id_shot] = !(shlockmap_[id_shot]);//変更を変更
            if (shlockmap_[id_shot] == FALSE)
            {
                --stCoUser_.shlocknum_;
            }
        }

    }
    LONGVECTOR TMgCoUser::GetShLockVec()
    {
        LONGVECTOR res;
        for (SHLOCKMAP::iterator it = shlockmap_.begin(); it != shlockmap_.end(); ++it)
        {
            //変更有りなら
            if (it->second) res.push_back(it->first);
        }
        shlockmap_.clear();
        stCoUser_.shlocknum_ = 0;
        return res;
    }
    s32 TMgCoUser::GetShLockNum()
    {
        return stCoUser_.shlocknum_;
    }
    //#############################################


    SORTTEAMVECTOR TMgCoUser::GetTeamVec()
    {
        SORTTEAMVECTOR res;
        s64 nowtime = mid::midNowTime();
        const s32 MINSEC = 60;
        s64 id_kantoku = mdKantoku_.GetMyDt(mdkantoku_pk);
        for (s32 i = 0; i < mdTeam_.GetLineNum(); ++i)
        {
            //違う監督のチーム
            if (id_kantoku != mdTeam_.GetLineDt(i, mdteam_id_kantoku)) continue;

			// データは入っているがレベルが0 == 売却したチーム
			if (pmgEO_->mgCoU_.mdTeam_.GetLineDt(i, mdteam_overlap) == 0) { continue; }

            TSortTeam steam;
            steam.Init();
            steam.id_team_ = mdTeam_.GetLineDt(i, mdteam_pk);
            steam.mid_team_ = mdTeam_.GetLineDt(i, mdteam_id_m_team);
            steam.rarity_ = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(steam.mid_team_, mdm_team_rarity);
            steam.id_shset_ = mdTeam_.GetLineDt(i, mdteam_id_shset);
            steam.overlap_ = mdTeam_.GetLineDt(i, mdteam_overlap);

            s32 staminalim = (pmgEO_->mgCoM_.mdm_Rarity_.GetLineDt(steam.rarity_, mdm_rarity_staminaLim) * MINSEC);
            s64 endtime = mdTeam_.GetPKDt(steam.id_team_, mdteam_useSec);
            s32 wait = (staminalim - (nowtime - endtime));
            if (wait < 0) wait = 0;
            steam.wait_ = wait;

            res.push_back(steam);
        }
        return res;
    }

    SORTTEAMVECTOR TMgCoUser::GenGetTeamList_FilRarity(SORTTEAMVECTOR* lnovec, s32 rarity)
    {
        SORTTEAMVECTOR res;
        for (s32 i = 0; i < lnovec->size(); ++i)
        {
            if (lnovec->at(i).rarity_ == rarity) res.push_back(lnovec->at(i));
        }
        return res;
    }
    SORTTEAMVECTOR TMgCoUser::GenGetTeamList_FilShSet(SORTTEAMVECTOR* lnovec, s32 shsetNo)
    {
        SORTTEAMVECTOR res;
        if (shsetNo != NGNUM)
        {
            s64 id_shset = mdShSet_.GetLineDt(shsetNo, mdshset_pk);
            for (s32 i = 0; i < lnovec->size(); ++i)
            {
                if (lnovec->at(i).id_shset_ == id_shset) res.push_back(lnovec->at(i));
            }
        }
        else
        {
            for (s32 i = 0; i < lnovec->size(); ++i)
            {
                if (lnovec->at(i).id_shset_ == 0) res.push_back(lnovec->at(i));//カスタム
            }
        }
        return res;
    }
    SORTTEAMVECTOR TMgCoUser::GenGetTeamList_FilTokkou(SORTTEAMVECTOR* lnovec, s32 mid_event)
    {

        s32 tokkouNo = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(mid_event, mdm_event_tokkouNo);
        LONGVECTOR vectokkou;
        for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_Tokkou_.GetLineNum(); ++i)
        {
            if (tokkouNo == pmgEO_->mgCoM_.mdm_Tokkou_.GetLineDt(i, mdm_tokkou_tokkouNo))
            {
                vectokkou.push_back(pmgEO_->mgCoM_.mdm_Tokkou_.GetLineDt(i, mdm_tokkou_id_m_team));
            }
        }

        SORTTEAMVECTOR res;
        for (s32 i = 0; i < lnovec->size(); ++i)
        {
            for (s32 i2 = 0; i2 < vectokkou.size(); ++i2)
            {
                if (lnovec->at(i).mid_team_ == vectokkou[i2])
                {
                    res.push_back(lnovec->at(i));
                    break;
                }
            }
        }
        return res;

    }

    // 比較関数を定義
    bool cmptm_rarity(const TSortTeam& left, const TSortTeam& right)
    {
        return (left.rarity_ == right.rarity_)
            ? (left.mid_team_ < right.mid_team_)
            : (left.rarity_ > right.rarity_);//高い方が先頭
    }
    bool cmptm_rarity_rev(const TSortTeam& left, const TSortTeam& right)
    {
        return (left.rarity_ == right.rarity_)
            ? (left.mid_team_ < right.mid_team_)
            : (left.rarity_ < right.rarity_);//高い方が先頭
    }
    bool cmptm_lv(const TSortTeam& left, const TSortTeam& right)
    {
        return (left.overlap_ == right.overlap_)
            ? (left.mid_team_ < right.mid_team_)
            : (left.overlap_ > right.overlap_);//低い方が前
    }
    bool cmptm_lv_rev(const TSortTeam& left, const TSortTeam& right)
    {
        return (left.overlap_ == right.overlap_)
            ? (left.mid_team_ < right.mid_team_)
            : (left.overlap_ < right.overlap_);//低い方が前
    }
    bool cmptm_new(const TSortTeam& left, const TSortTeam& right)
    {
        return (left.id_team_ > right.id_team_);//大きい方が先頭
    }
    bool cmptm_new_rev(const TSortTeam& left, const TSortTeam& right)
    {
        return (left.id_team_ < right.id_team_);//小さい方が先頭
    }
    bool cmptm_mid(const TSortTeam& left, const TSortTeam& right)
    {
        return (left.mid_team_ < right.mid_team_);//小さい方が先頭
    }
    bool cmptm_mid_rev(const TSortTeam& left, const TSortTeam& right)
    {
        return (left.mid_team_ > right.mid_team_);//大きい方が先頭
    }
    bool cmptm_wait(const TSortTeam& left, const TSortTeam& right)
    {
        if (left.wait_ == right.wait_)
        {
            if (left.rarity_ == right.rarity_)
            {
                return (left.mid_team_ < right.mid_team_);//小さい方が先頭
            }
            else
            {
                return (left.rarity_ > right.rarity_);//大きい方が先頭
            }
        }
        else
        {
            return (left.wait_ < right.wait_);//小さい方が先頭
        }
    }
    bool cmptm_wait_rev(const TSortTeam& left, const TSortTeam& right)
    {
        if (left.wait_ == right.wait_)
        {
            if (left.rarity_ == right.rarity_)
            {
                return (left.mid_team_ < right.mid_team_);//小さい方が先頭
            }
            else
            {
                return (left.rarity_ > right.rarity_);//大きい方が先頭
            }
        }
        else
        {
            return (left.wait_ > right.wait_);//大きい方が先頭
        }
    }

    LONGVECTOR TMgCoUser::GenGetTeamList(SORTTEAMVECTOR* lnovec)
    {
        LONGVECTOR res;
        for (s32 i = 0; i < lnovec->size(); ++i) res.push_back(lnovec->at(i).id_team_);
        return res;
    }
    //LONGVECTOR TMgCoUser::GenGetTeamList_SrtMID(SORTTEAMVECTOR* lnovec, BOOL rev_f)
    //{
    //  if (rev_f) std::sort(lnovec->begin(), lnovec->end(), cmptm_mid_rev);
    //  else std::sort(lnovec->begin(), lnovec->end(), cmptm_mid);
    //  return GenGetTeamList(lnovec);
    //}
    LONGVECTOR TMgCoUser::GenGetTeamList_SrtWait(SORTTEAMVECTOR* lnovec, BOOL rev_f)
    {
        if (rev_f) std::sort(lnovec->begin(), lnovec->end(), cmptm_wait_rev);
        else std::sort(lnovec->begin(), lnovec->end(), cmptm_wait);
        return GenGetTeamList(lnovec);
    }
    LONGVECTOR TMgCoUser::GenGetTeamList_SrtRarity(SORTTEAMVECTOR* lnovec, BOOL rev_f)
    {
        if (rev_f) std::sort(lnovec->begin(), lnovec->end(), cmptm_rarity_rev);
        else std::sort(lnovec->begin(), lnovec->end(), cmptm_rarity);
        return GenGetTeamList(lnovec);
    }
    LONGVECTOR TMgCoUser::GenGetTeamList_SrtLv(SORTTEAMVECTOR* lnovec, BOOL rev_f)
    {
        if (rev_f) std::sort(lnovec->begin(), lnovec->end(), cmptm_lv_rev);
        else std::sort(lnovec->begin(), lnovec->end(), cmptm_lv);
        return GenGetTeamList(lnovec);
    }
    LONGVECTOR TMgCoUser::GenGetTeamList_SrtNew(SORTTEAMVECTOR* lnovec, BOOL rev_f)
    {
        if (rev_f) std::sort(lnovec->begin(), lnovec->end(), cmptm_new_rev);
        else std::sort(lnovec->begin(), lnovec->end(), cmptm_new);
        return GenGetTeamList(lnovec);
    }

    s32 TMgCoUser::GenGetNumTeamList_Tokkou(SORTTEAMVECTOR* lnovec, s32 mid_event)
    {
        s32 res = 0;
        if (mid_event == NGNUM) return res;

        s32 tokkouNo = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(mid_event, mdm_event_tokkouNo);
        LONGVECTOR vectokkou;
        for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_Tokkou_.GetLineNum(); ++i)
        {
            if (tokkouNo == pmgEO_->mgCoM_.mdm_Tokkou_.GetLineDt(i, mdm_tokkou_tokkouNo))
            {
                vectokkou.push_back(pmgEO_->mgCoM_.mdm_Tokkou_.GetLineDt(i, mdm_tokkou_id_m_team));
            }
        }

        for (s32 i = 0; i < lnovec->size(); ++i)
        {
            for (s32 i2 = 0; i2 < vectokkou.size(); ++i2)
            {
                if (lnovec->at(i).mid_team_ == vectokkou[i2])
                {
                    ++res;
                    break;
                }
            }
        }
        return res;
    }
    s32 TMgCoUser::GenGetNumTeamList_Rarity(SORTTEAMVECTOR* lnovec, s32 rarity)
    {
        s32 res = 0;
        for (s32 i = 0; i < lnovec->size(); ++i)
        {
            if (lnovec->at(i).rarity_ == rarity) ++res;
        }
        return res;
    }
    s32 TMgCoUser::GenGetNumTeamList_ShSet(SORTTEAMVECTOR* lnovec, s32 shsetNo)
    {
        s32 res = 0;
        s64 id_shset = mdShSet_.GetLineDt(shsetNo, mdshset_pk);
        for (s32 i = 0; i < lnovec->size(); ++i)
        {
            if (lnovec->at(i).id_shset_ == id_shset)++res;
        }
        return res;
    }

    s32 TMgCoUser::GetNumTeamList(SORTTEAMVECTOR* teamvec, enTeamFilType filtype, s32 mid_event)
    {
        s32 res = 0;
        switch (filtype)
        {
        case db::enTeamFilType_All: res = teamvec->size(); break;
        case db::enTeamFilType_Tokkou: res = GenGetNumTeamList_Tokkou(teamvec, mid_event); break;
        case db::enTeamFilType_RarityD: res = GenGetNumTeamList_Rarity(teamvec, 0); break;
        case db::enTeamFilType_RarityC: res = GenGetNumTeamList_Rarity(teamvec, 1); break;
        case db::enTeamFilType_RarityB: res = GenGetNumTeamList_Rarity(teamvec, 2); break;
        case db::enTeamFilType_RarityA: res = GenGetNumTeamList_Rarity(teamvec, 3); break;
        case db::enTeamFilType_RarityS: res = GenGetNumTeamList_Rarity(teamvec, 4); break;
        case db::enTeamFilType_RaritySS: res = GenGetNumTeamList_Rarity(teamvec, 5); break;
            //    case db::enTeamFilType_RaritySSS: res = GenGetNumTeamList_Rarity(teamvec, 6); break;
            //case db::enTeamFilType_ShSetA: res = GenGetNumTeamList_ShSet(teamvec, 0); break;
            //case db::enTeamFilType_ShSetB: res = GenGetNumTeamList_ShSet(teamvec, 1); break;
            //case db::enTeamFilType_ShSetC: res = GenGetNumTeamList_ShSet(teamvec, 2); break;
            //case db::enTeamFilType_ShSetD: res = GenGetNumTeamList_ShSet(teamvec, 3); break;
        }
        return res;
    }

    //フィルタのときはチームベクタを再生成
    LONGVECTOR TMgCoUser::GetTeamList_Filter(SORTTEAMVECTOR* teamvec, enTeamSrtType srttype, enTeamFilType filtype, BOOL rev_f)
    {
        return GetTeamList_Filter(teamvec, srttype, filtype, rev_f, 0);
    }

    LONGVECTOR TMgCoUser::GetTeamList_Filter(SORTTEAMVECTOR* teamvec, enTeamSrtType srttype, enTeamFilType filtype, BOOL rev_f, s32 mid_event)
    {
        //ベクタ作り直し
        *teamvec = GetTeamVec();

        switch (filtype)
        {
        case db::enTeamFilType_Tokkou: *teamvec = GenGetTeamList_FilTokkou(teamvec, mid_event); break;
        case db::enTeamFilType_RarityD: *teamvec = GenGetTeamList_FilRarity(teamvec, 0); break;
        case db::enTeamFilType_RarityC: *teamvec = GenGetTeamList_FilRarity(teamvec, 1); break;
        case db::enTeamFilType_RarityB: *teamvec = GenGetTeamList_FilRarity(teamvec, 2); break;
        case db::enTeamFilType_RarityA: *teamvec = GenGetTeamList_FilRarity(teamvec, 3); break;
        case db::enTeamFilType_RarityS: *teamvec = GenGetTeamList_FilRarity(teamvec, 4); break;
        case db::enTeamFilType_RaritySS: *teamvec = GenGetTeamList_FilRarity(teamvec, 5); break;
            //   case db::enTeamFilType_RaritySSS: *teamvec = GenGetTeamList_FilRarity(teamvec, 6); break;
            //case db::enTeamFilType_ShSetA: *teamvec = GenGetTeamList_FilShSet(teamvec, 0); break;
            //case db::enTeamFilType_ShSetB: *teamvec = GenGetTeamList_FilShSet(teamvec, 1); break;
            //case db::enTeamFilType_ShSetC: *teamvec = GenGetTeamList_FilShSet(teamvec, 2); break;
            //case db::enTeamFilType_ShSetD: *teamvec = GenGetTeamList_FilShSet(teamvec, 3); break;
            //case db::enTeamFilType_ShSetE: *teamvec = GenGetTeamList_FilShSet(teamvec, NGNUM); break;
        }

        return GetTeamList_Sort(teamvec, srttype, rev_f);
    }
    //ソートのときはチームベクタをそのまま利用
    LONGVECTOR TMgCoUser::GetTeamList_Sort(SORTTEAMVECTOR* teamvec, enTeamSrtType srttype, BOOL rev_f)
    {
        LONGVECTOR res;

        switch (srttype)
        {
            //case db::enTeamSrtType_MID:res = GenGetTeamList_SrtMID(teamvec, rev_f); break;
        case db::enTeamSrtType_Rarity:res = GenGetTeamList_SrtRarity(teamvec, rev_f); break;
        case db::enTeamSrtType_Wait:res = GenGetTeamList_SrtWait(teamvec, rev_f); break;
        case db::enTeamSrtType_Lv:res = GenGetTeamList_SrtLv(teamvec, rev_f); break;
        case db::enTeamSrtType_New:res = GenGetTeamList_SrtNew(teamvec, rev_f); break;
        }
        return res;
    }


    //チームのレベル
    s64 TMgCoUser::GetTeamLv(s64 mid_team)
    {
        s64 res = 1;
        for (s32 i = 0; i < mdTeam_.GetLineNum(); ++i)
        {
            if (mdTeam_.GetLineDt(i, mdteam_id_m_team) == mid_team)
            {
                res = mdTeam_.GetLineDt(i, mdteam_overlap);
                break;
            }
        }
        return res;
    }

	// キャラのレベル
	s64 TMgCoUser::GetMyTmCharaLv(s64 mid_team, s32 charaPos)
	{
		return GetTmCharaLvByKantokuID(mdKantoku_.GetMyDt(enmdKantoku::mdkantoku_pk) ,mid_team, charaPos);
	}
	// キャラのレベル
	s64 TMgCoUser::GetTmCharaLvByKantokuID(s64 id_kantoku, s64 mid_team, s32 charaPos)
	{
		s64 res = 1;
		for (s32 i = 0; i < mdTeamCharLv_.GetLineNum(); i++)
		{
			// 監督IDとチームのマスターIDが一致すればOK
			if (mdTeamCharLv_.GetLineDt(i, enmdTeamCharLv::mdteamcharlv_id_m_team) == mid_team &&
				mdTeamCharLv_.GetLineDt(i, enmdTeamCharLv::mdteamcharlv_id_kantoku) == id_kantoku)
			{
				// *2はlvの間にexpがあるので2間隔にする為
				enmdTeamCharLv posNo = (enmdTeamCharLv)(enmdTeamCharLv::mdteamcharlv_char0_lv + (charaPos * 2));
				res = mdTeamCharLv_.GetLineDt(i, posNo);
				break;
			}
		}
		return res;
	}

	// キャラのレベル制限チェック
	BOOL TMgCoUser::IsMyTmCharaLvMax(s64 mid_team, s32 posNo)
	{
		BOOL ret = FALSE;

		// 今の経験値
		s64 nowLv = pmgEO_->mgCoU_.GetMyTmCharaLv(mid_team, posNo);
		s64 rarity = pmgEO_->mgCoM_.mdm_Team_.GetPKDt(mid_team, enmdm_Team::mdm_team_rarity);

		ret = IsCharaLvMax(nowLv, rarity);
		return ret;
	}

	BOOL TMgCoUser::IsCharaLvMax(s64 lv, s32 rarity)
	{
		BOOL ret = FALSE;

		s64 limitLv = 0;
		for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_Rarity_.GetLineNum(); i++)
		{
			if (pmgEO_->mgCoM_.mdm_Rarity_.GetLineDt(i, enmdm_Rarity::mdm_rarity_pk) == rarity)
			{
				limitLv = pmgEO_->mgCoM_.mdm_Rarity_.GetLineDt(i, enmdm_Rarity::mdm_rarity_charaLvCap);
				break;
			}
		}

		if (lv >= limitLv)
		{
			ret = TRUE;
		}
		return ret;
	}


	// キャラの経験値
	s64 TMgCoUser::GetMyTmCharaExp(s64 mid_team, s32 charaPos)
	{
		return GetTmCharaExpByKantokuID(mdKantoku_.GetMyDt(enmdKantoku::mdkantoku_pk), mid_team, charaPos);
	}
	// キャラの経験値
	s64 TMgCoUser::GetTmCharaExpByKantokuID(s64 id_kantoku, s64 mid_team, s32 charaPos)
	{
		s64 res = 0;
		for (s32 i = 0; i < mdTeamCharLv_.GetLineNum(); i++)
		{
			// 監督IDとチームのマスターIDが一致すればOK
			if (mdTeamCharLv_.GetLineDt(i, enmdTeamCharLv::mdteamcharlv_id_m_team) == mid_team &&
				mdTeamCharLv_.GetLineDt(i, enmdTeamCharLv::mdteamcharlv_id_kantoku) == id_kantoku)
			{
				// *2はlvの間にlvがあるので2間隔にする為
				enmdTeamCharLv posNo = (enmdTeamCharLv)(enmdTeamCharLv::mdteamcharlv_char0_exp + (charaPos * 2));
				res = mdTeamCharLv_.GetLineDt(i, posNo);
				break;
			}
		}
		return res;
	}


    bool CheckPriority(const INTVECTOR& left, const INTVECTOR& right)
    {
        return (left[1] > right[1]);//大きい方が先頭
    }

    //現在開催中のイベント
    LONGVECTOR TMgCoUser::GetNowEvent()
    {
        // 今の時間
        s64 nowDate = mid::midNowTime();

        INTVECTOR2 data;
        data.clear();

        // 先にソートする
        for (s32 i = 0; i < mdw_Event_.GetLineNum(); ++i)
        {
#ifndef __K_DEBUG_ROOT__
            if (mdw_Event_.GetLineDt(i, mdw_event_end_date) < nowDate) continue;//終了日時を過ぎてる
            if (mdw_Event_.GetLineDtBOOL(i, mdw_event_open_f) == FALSE) continue;//開催中じゃない
#endif

            INTVECTOR v;
            v.clear();
            s64 mid_event = mdw_Event_.GetLineDt(i, mdw_event_id_m_event);
            s64 priority = mdw_Event_.GetLineDt(i, mdw_event_priority);
            v.push_back(mid_event);
            v.push_back(priority);
            data.push_back(v);
        }
        std::sort(data.begin(), data.end(), CheckPriority);


        LONGVECTOR res;
        for (s32 i = 0; i < data.size(); ++i)
        {
            // ソートされた順にデータを入れる
            res.push_back(data[i][0]);
        }

        return res;
    }


    //現在結果表示中のイベント
    LONGVECTOR TMgCoUser::GetResultEvent()
    {
        // 今の時間
        s64 nowDate = mid::midNowTime();

        INTVECTOR2 data;
        data.clear();

        // 先にソートする
        for (s32 i = 0; i < mdw_Event_.GetLineNum(); ++i)
        {
#ifndef __K_DEBUG_ROOT__
            if (mdw_Event_.GetLineDt(i, mdw_event_end_date) > nowDate) continue;//終了日時を過ぎてない
            if (mdw_Event_.GetLineDt(i, mdw_event_result_date) < nowDate) continue;//結果表示日時を過ぎてる
#endif

            INTVECTOR v;
            v.clear();
            s64 mid_event = mdw_Event_.GetLineDt(i, mdw_event_id_m_event);
            s64 priority = mdw_Event_.GetLineDt(i, mdw_event_priority);
            v.push_back(mid_event);
            v.push_back(priority);
            data.push_back(v);
        }
        std::sort(data.begin(), data.end(), CheckPriority);


        LONGVECTOR res;
        for (s32 i = 0; i < data.size(); ++i)
        {
            // ソートされた順にデータを入れる
            res.push_back(data[i][0]);
        }

        return res;
    }


    //イベントの日時
    std::string TMgCoUser::GetEventDate(s32 mid_event)
    {
        std::string res = "";
        for (s32 i = 0; i < mdw_Event_.GetLineNum(); ++i)
        {
            if (mdw_Event_.GetLineDt(i, mdw_event_id_m_event) == mid_event)
            {
#ifdef __K_DEBUG_USER_DATA__
                res = "でばっぐもーど";
                break;
#else
                res = base::UnixtimeToDateString(mdw_Event_.GetLineDt(i, mdw_event_start_date), TRUE)
                    + "＿負＿"
                    + base::UnixtimeToDateString(mdw_Event_.GetLineDt(i, mdw_event_end_date) - 1, TRUE);
#endif
            }
        }

        return res;
    }

    s32 TMgCoUser::GetEventWID(s32 mid_event, BOOL opencheck_f)
    {
        s32 res = 0;
        for (s32 i = 0; i < mdw_Event_.GetLineNum(); ++i)
        {
            if (mdw_Event_.GetLineDt(i, mdw_event_id_m_event) == mid_event)
            {
                if (opencheck_f && (mdw_Event_.GetLineDt(i, mdw_event_open_f) == FALSE)) continue;

                res = mdw_Event_.GetLineDt(i, mdw_event_pk);
                break;
            }
        }
        return res;
    }

    BOOL TMgCoUser::IsOverTimeEvent(s32 wid_event)
    {
        if (pmgEO_->stSeq_.IsNoSv())
        {
            return FALSE;
        }

        BOOL res = FALSE;
        if (wid_event != 0)
        {
            s64 nowtime = mid::midNowTime();
            s64 endtime = mdw_Event_.GetPKDt(wid_event, mdw_event_end_date);
            //残り時間
            res = (nowtime > endtime);
        }
        return res;
    }
    std::string TMgCoUser::GetShSetName(s32 shsetNo, BOOL set_f)
    {
        std::string res = mdShSet_.GetLineDtStr(shsetNo, mdshset_name_c32);
        if (res == "") res = "ＮＯ．" + lib_str::IntToStr(shsetNo);
        if (set_f) res += "セット";
        return res;
    }
    //シュートの名前
    std::string TMgCoUser::GetShotName(s64 id_shot)
    {
        std::string res = mdShot_.GetPKDtStr(id_shot, mdshot_name_c32);
        s32 mid_sozai = mdShot_.GetPKDt(id_shot, mdshot_id_m_sozai0) / SOZAIMID;
        if (res == "")
        {
            res = pmgEO_->mgDt_.dtShElm_.GetDtStr(mid_sozai, szs_name);
        }

        return res;
    }

    //シュートの空き枠
    s32 TMgCoUser::GetShotFreeSlotNum(s64 id_shot)
    {
        s32 rarity = (mdShot_.GetPKDt(id_shot, mdshot_id_m_sozai0) % SOZAIMID);
        return (rarity + 1 - mdShot_.GetPKDt(id_shot, mdshot_useslot));
    }

    //最後に使ったチームのマスターＩＤ
    s32 TMgCoUser::GetLastMIDTeam()
    {
        s32 res = 0;
        s64 id_team = mdKantokuSub_.GetMyDt(mdkantokusub_id_team);
        if (id_team == NGID)
        {
            res = mdTeam_.GetLineDt(0, mdteam_id_m_team);
        }
        else
        {
            res = mdTeam_.GetPKDt(id_team, mdteam_id_m_team);
        }

        return res;
    }
    //カプセルの数
    s32 TMgCoUser::GetCapsuleNum()
    {
        s32 res = 0;

        for (s32 i = 0; i < mdlog_ItemGet_.GetLineNum(); ++i)
        {
            if (mdlog_ItemGet_.GetLineDtBOOL(i, mdlog_itemget_outPack_f) == FALSE)
            {
                ++res;
            }
        }
        return res;
    }

    //特効アイテム倍率
    s32 TMgCoUser::TokkouItemNum(s32 mid_event, enTokkouEffectType effectType)
    {
        //ヒットしなければ０を返す
        s32 res = 0;

        if (mid_event == NGNUM) return res;

        s32 tokkouNo = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(mid_event, mdm_event_tokkouNo);
        BOOL haikei_f = FALSE;
        BOOL bgm_f = FALSE;
        BOOL supply_f = FALSE;

        INTVECTOR tokkouvec;
        //タイプサーチ
        for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_TokkouItem_.GetLineNum(); ++i)
        {
            //タイプ違いをはじく
            if (effectType != pmgEO_->mgCoM_.mdm_TokkouItem_.GetLineDt(i, mdm_tokkouitem_effectType)) continue;
            //違う特効番号をはじく
            if (tokkouNo != pmgEO_->mgCoM_.mdm_TokkouItem_.GetLineDt(i, mdm_tokkouitem_tokkouNo)) continue;

            tokkouvec.push_back(i);
            s32 tokkoutype = (pmgEO_->mgCoM_.mdm_TokkouItem_.GetLineDt(i, mdm_tokkouitem_itemType));

            if (tokkoutype == enItemType_BGM) bgm_f = TRUE;
            if (tokkoutype == enItemType_Haikei) haikei_f = TRUE;
            if (tokkoutype == enItemType_Supply) supply_f = TRUE;
        }

        //ＢＧＭ
        if (bgm_f)
        {
            for (s32 i = 0; i < tokkouvec.size(); ++i)
            {
                if (enItemType_BGM != pmgEO_->mgCoM_.mdm_TokkouItem_.GetLineDt(tokkouvec[i], mdm_tokkouitem_itemType)) continue;
                s32 mid_bgm = pmgEO_->mgCoM_.mdm_TokkouItem_.GetLineDt(tokkouvec[i], mdm_tokkouitem_itemID);
                if (IsBGM(mid_bgm)) ++res;
            }
        }

        //背景
        if (haikei_f)
        {
            for (s32 i = 0; i < tokkouvec.size(); ++i)
            {
                if (enItemType_Haikei != pmgEO_->mgCoM_.mdm_TokkouItem_.GetLineDt(tokkouvec[i], mdm_tokkouitem_itemType)) continue;
                s32 mid_haikei = pmgEO_->mgCoM_.mdm_TokkouItem_.GetLineDt(tokkouvec[i], mdm_tokkouitem_itemID);
                if (IsHaikei(mid_haikei)) ++res;
            }
        }

        //消耗品
        if (supply_f)
        {
            for (s32 i = 0; i < tokkouvec.size(); ++i)
            {
                if (enItemType_Supply != pmgEO_->mgCoM_.mdm_TokkouItem_.GetLineDt(tokkouvec[i], mdm_tokkouitem_itemType)) continue;
                s32 mid_supply = pmgEO_->mgCoM_.mdm_TokkouItem_.GetLineDt(tokkouvec[i], mdm_tokkouitem_itemID);
                //mdSupply_.GetLineDt()
                for (s32 i2 = 0; i2 < mdSupply_.GetLineNum(); ++i2)
                {
                    if (mid_supply == mdSupply_.GetLineDt(i2, mdsupply_id_m_supply))
                    {
                        res += mdSupply_.GetLineDt(i2, mdsupply_supplyNum);
                    }
                }
            }
        }
        return res;
    }
    //特効アイテム倍率
    s32 TMgCoUser::TokkouItemMag(s32 mid_event, enTokkouEffectType effectType, s32 tokkouNum)
    {
        //ヒットしなければ０を返す
        s32 res = 0;
        if (tokkouNum == 0) return res;
        if (mid_event == NGNUM) return res;

        s32 tokkouNo = pmgEO_->mgCoM_.mdm_Event_.GetPKDt(mid_event, mdm_event_tokkouNo);
        for (s32 i = 0; i < pmgEO_->mgCoM_.mdm_TokkouItemMag_.GetLineNum(); ++i)
        {
            if (tokkouNo != pmgEO_->mgCoM_.mdm_TokkouItemMag_.GetLineDt(i, mdm_tokkouitemmag_tokkouNo)) continue;
            if (effectType != pmgEO_->mgCoM_.mdm_TokkouItemMag_.GetLineDt(i, mdm_tokkouitemmag_effectType)) continue;
            if (tokkouNum >= pmgEO_->mgCoM_.mdm_TokkouItemMag_.GetLineDt(i, mdm_tokkouitemmag_haveNum))
            {
                res = pmgEO_->mgCoM_.mdm_TokkouItemMag_.GetLineDt(i, mdm_tokkouitemmag_effect);
            }
            else
            {
                break;
            }
        }

        return res;
    }
    //操作
    s32 TMgCoUser::GetCtrl()
    {
        const s32 CTRL_RETRO = 0;
        const s32 CTRL_SMART = 1;
        s32 res = CTRL_RETRO;
        s64 openbit = mdKantokuSub_.GetMyDt(mdkantokusub_optionBit);
        u32 checkbit = (0x1 << enOptBitType_Smart);
        if ((openbit & checkbit) != 0)
        {
            res = CTRL_SMART;
        }
        return res;
    }

    //現在の特効イベント
    LONGVECTOR TMgCoUser::GetNowTokkouEvent()
    {
        LONGVECTOR res;

        for (s32 i = 0; i < mdw_Event_.GetLineNum(); ++i)
        {
            if (mdw_Event_.GetLineDtBOOL(i, mdw_event_open_f) == FALSE) continue;
            s32 mid_event = mdw_Event_.GetLineDt(i, mdw_event_id_m_event);

            if (pmgEO_->mgCoM_.mdm_Event_.GetPKDt(mid_event, mdm_event_tokkouNo) > 0)
            {
                res.push_back(mid_event);
            }
        }

        return res;
    }
}
