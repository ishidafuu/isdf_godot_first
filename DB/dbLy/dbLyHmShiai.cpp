#include "dbLyHmShiai.h"
#include "dbSnHome.h"

namespace db
{
    enum enPage
    {
        pg_Tuto,
        pg_Main,
        pg_GotoScenario,
        pg_InVS,
        pg_Idle,
    };

    enum enConnect
    {
        cn_Omakase,
    };

    //コンストラクタ
    TLyHmShiai::TLyHmShiai(TSnHome* pScene) :TLyHmGen(pScene)
        , ugrmsg_scenario_(this)
        , ugrmsg_vs_(this)
        , ugshbtn_scenario_(this)
        , ugshbtn_vs_(this)
    {
    }

    TLyHmShiai::~TLyHmShiai()
    {
        for (s32 i = 0; i < uglock_.size(); ++i) Freemem(uglock_[i]);
    }

    void TLyHmShiai::SetupShiai(s32 shiaiNo)
    {
        // ここでショップ内ジャンプのパラメータを取得する
        //INTVECTOR v;
        switch (shiaiNo)
        {
        default:
            // シナリオマップ
        case 1:
            // ここにはこないはず
            break;
            // 通信対戦
        case 2:
            pScene_->lyHmShVS_->Ready();
            ChangePage(pg_InVS);
            break;
        }

        // ジャンプが終わったらジャンプデータはクリアしておく
        pmgEO_->stJump_.ClearJumpAreaData();
    }

    void TLyHmShiai::DoLoad()
    {
        ugshbtn_scenario_.MakeGroupShiai(field_, hmgrp_->hmshiai_next_, 0, "シナリオマップ");
        ugshbtn_vs_.MakeGroupVS(field_, hmgrp_->hmshiai_next_);


        ugshbtn_scenario_.SetPos(-80, -40);
        ugshbtn_vs_.SetPos(80, -40);


        ugrmsg_scenario_.MakeGroup(field_, TRUE);
        ugrmsg_scenario_.SetPos(-156, 0);
        STRVECTOR btvec = {
            "ＳＣＥＮＡＲＩＯ＿ＭＡＰ",
            "シナリオ＿クリアで＿アイテムＧＥＴ",
            "すすめて＿いけば＿くにおくんの",
            "まめちしきも＿わかるかも",
            "イベントの＿あいまに＿すすめよう" };
        ugrmsg_scenario_.SetData(&btvec);
        ugrmsg_scenario_.SetDraw(TRUE);

        ugrmsg_vs_.MakeGroup(field_, TRUE);
        ugrmsg_vs_.SetPos(4, 0);
        STRVECTOR wifivec = {
            "ＮＥＴＷＯＲＫ＿ＢＡＴＴＬＥ",
            "めのまえの＿おともだちと＿たのしく",
            "ぜんこくの＿プレイヤーと＿あつく",
            "きみだけの＿ひっさつ＿しゅーと＿と",
            "うでを＿みがいて＿しょうり＿しよう" };
        ugrmsg_vs_.SetData(&wifivec);
        ugrmsg_vs_.SetDraw(TRUE);

    }
    void TLyHmShiai::DoReady()
    {
        SetFoot_Tab();
    }
    void TLyHmShiai::Idle()
    {
        ChangePage(pg_Idle);
    }

    void TLyHmShiai::SetScenarioOnly(BOOL isOnly)
    {
        ugshbtn_vs_.GetField()->b_SetBtn(!isOnly);
    }

    void TLyHmShiai::MvPage_Tuto(void)
    {
        BOOL isEnd = IsSubEventOpen();


        // ここでのチュートリアルが終わった時用
        if (pScene_->lySTuShiai_ && pScene_->lySTuShiai_->IsTutoEnd())
        {
            // 初回のはじめて試合タブ開いた時のチュートリアルが終わっただけなら
            // シナリオマップのボタンだけを押せるようにする
            if (IsSubEventOpen() == FALSE)
            {
                // ロックをかける
                pScene_->pmgUI_->SetModal(TRUE);

                //ヘッダとフッタきかないように
                frame_->ugheadder_->SetDisable();
                frame_->ugfooter_->SetDisable();

                // これだけ押せるように
                ugshbtn_scenario_.SetModal(TRUE);
                ugshbtn_vs_.SetDisable(TRUE);
                {
                    TUGLock* newlock = new TUGLock(this);
                    newlock->MakeGroup(ugshbtn_vs_.GetField());
                    uglock_.push_back(newlock);
                }
            }

            isEnd = TRUE;
        }

        if (isEnd)
        {
            ChangePage(pg_Main);
        }
    }
    void TLyHmShiai::MvPage_Main()
    {
        if (pmgEO_->stSeq_.PopNeedOmakase())//おまかせ更新フラグチェック＆たたみ
        {
            shsetvec_ = pmgEO_->mgCoU_.GetShotOmakase();
            Post_Omakase();
        }
        else if (ugshbtn_scenario_.IsAct())//通常マップ
        {
            pScene_->lyHmBar_->SetFilterAll(FALSE);
            pScene_->SetNextScenario();
#ifndef __K_MAKE_HIDE_EVENT_SOUND__
			pmgEO_->stSeq_.SetMapMidBGM(nsf_scenario);
#endif
            //pmgEO_->mgStSn_.EtcSS.SetShiaiType_Normal();//
            pmgEO_->stSeq_.SetIDEvent(0, 0);// .EtcSS.SetEventID(0, 0);
            ChangePage(pg_GotoScenario);
        }
        else if (ugshbtn_vs_.IsAct())//対戦
        {

            if (mid::midIsADR())
            {
                BOOL googleplay_f = FALSE;

//                if (mid::midIsLite())
//                {
//                    googleplay_f = pmgEO_->mgDt_.dtClient_.IsAccessGooglePlayGoogle();
//                }
//                else
//                {
//                    googleplay_f = pmgEO_->mgDt_.dtClient_.IsAccessGooglePlayMobage();
//                }

                googleplay_f = pmgEO_->mgDt_.dtClient_.IsAccessGooglePlayMobage();


                if (googleplay_f)
                {
                    if ((mid::midIsGooglePlay() == FALSE) || mid::midIsSignIn())
                    {
                        pScene_->lyHmShVS_->Ready();
                        ChangePage(pg_InVS);
                    }
                    else
                    {
                        mid::midStartAuthorizationUI();
                    }
                }
                else
                {
                    STRVECTOR strvec = { "サービスは＿じゅんびちゅうです", "しばらく＿おまちください" };
                    SetHudMsg(FALSE, FALSE, &strvec);
                }

            }
            else
            {
                pScene_->lyHmShVS_->Ready();
                ChangePage(pg_InVS);
            }
        }
    }
    void TLyHmShiai::MvPage_GotoScenario()
    {

    }
    void TLyHmShiai::MvPage_InVS()
    {

    }
    void TLyHmShiai::MvPage_Idle()
    {

    }

    //POST：おまかせ
    void TLyHmShiai::Post_Omakase()
    {
        StartConnect(cn_Omakase);
        pmgEO_->mgPo_.poHmTeam_.Ready_attachshset(pmgEO_->mgCoU_.mdKantokuSub_.GetMyDt(mdkantokusub_id_shset), &shsetvec_);
    }
    //通信：おまかせ
    void TLyHmShiai::Connect_Omakase()
    {
        //ChangePage(pg_Menu);
    }
    //接続完了
    void TLyHmShiai::DoConnect(BOOL rqok_f)
    {
        if (rqok_f)//ＯＫのみ
        {
            //code0
            if (pmgEO_->mgPo_.IsCodeZero())
            {
                switch (stLy_.connect_)
                {
                case cn_Omakase: Connect_Omakase(); break;
                }
            }
            else
            {
                //エラーメッセージ
                EndConnectNG(pmgEO_->mgPo_.GetCode(), pmgEO_->mgPo_.GetMsg().c_str());
                //ChangePage(pg_Error);
            }
        }
        else
        {
            //ChangePage(pg_Error);
        }
    }

    void TLyHmShiai::DoUpdate(BOOL draw_f)
    {
        switch (stLy_.pageNo_)
        {
        case db::pg_InVS:
        case db::pg_Idle:
            break;

        default:
            for (s32 i = 0; i < uglock_.size(); ++i) uglock_[i]->Update(draw_f);
            ugshbtn_scenario_.Update(draw_f);
            ugrmsg_scenario_.Update(draw_f);
            ugrmsg_vs_.Update(draw_f);
            ugshbtn_vs_.Update(draw_f);
            //ugicon_.Update(draw_f);
            break;
        }

        if (draw_f == FALSE)
        {
            if (pScene_->lyHmBar_->IsBarAction()) return;
            switch (stLy_.pageNo_)
            {
            case db::pg_Tuto:MvPage_Tuto(); break;
            case db::pg_Main:MvPage_Main(); break;
            case db::pg_GotoScenario:MvPage_GotoScenario(); break;
            case db::pg_InVS:MvPage_InVS(); break;
            case db::pg_Idle:MvPage_Idle(); break;
            }
        }
    }

    BOOL TLyHmShiai::IsSubEventOpen()
    {
        BOOL res = FALSE;
#ifdef __K_DEBUG_USER_DATA__
        if (kdebug::DebugUDManager::GetInstance()->IsTutoEnd(enTutoBitType_SubEventOpen))	// チュートリアルが全て終了している
#else
        if (pmgEO_->mgCoU_.IsTutoClear(enTutoBitType_SubEventOpen))
#endif
        {
            res = TRUE;
        }
        return res;
        }

    }
